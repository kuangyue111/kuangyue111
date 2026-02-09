//#include "includes.h"
#include "stm32f4xx_hal.h"
#include "bsp_SPI.h"
//#include "uip-conf.h"
#include "main.h"


#define IFNAME0 'Z'
#define IFNAME1 'Z'
#define SPI3_BUFF_BYTES_NUM		132

CanRxMsgTypeDef CanRxMsgBuff_3[CAN_3_MAX_BUFF + 20];
CanRxMsgTypeDef CanRxMsgBuff_3_Ring[CAN_3_MAX_BUFF + 20];

uint32_t pRecvCAN3_Idx = 0;
uint32_t pRecvCAN3_Idx_Ring = 0;
NOINIT uint8_t SPI3_bufR[SPI3_BUFF_BYTES_NUM + 50];
NOINIT uint8_t SPI3_bufS[SPI3_BUFF_BYTES_NUM + 50];
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END;/* Ethernet Rx MA Descriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END;/* Ethernet Tx DMA Descriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN NOINIT uint8_t ETH_Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN NOINIT uint8_t ETH_Tx_Buff[ETH_TXBUFNB+1][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

// ================* Definitions and Marcos ***************
// General SPI Flash Commands
#define SF_WR_STATUSREG         (0x01)
#define SF_PAGE_PROGRAM         (0x02)
#define SF_RD_DATA_NORMAL       (0x03)
#define SF_WR_DISABLE           (0x04)
#define SF_RD_STATUSREG         (0x05)
#define SF_WR_ENABLE            (0x06)

#define SF_READ_IDENTIFICATION  (0x9F)												// (0x9E)

#define SF_BULK_ERASE           (0xc7)
#define SF_SECTOR_ERASE         (0x20)
#define SF_BLOCK_ERASE          (0xd8)

#define SpiTimeout              300  //(0x1000)
#define SPI3Timeout             300  // (0x1000)
// ================* Variables ****************************
SPI_HandleTypeDef SpiHandle;
SPI_HandleTypeDef SPI3Handle;
uint8_t spi_status_f;
HANDLE hMutexSPIFlashAccess;
struct sf_m25pxx_s sf_m25pxx_info;

ETH_HandleTypeDef EthHandle;
HANDLE hMutexWriteNetBuffer;


static void mcp2515_ReadCanMsg(uint8_t buffer_sidh_addr, CanMessage* msg);

static UCHAR init_netMutex(void)
{
    hMutexWriteNetBuffer = MutexCreate(UPPER_PRIO_READ_WRITE_NETBUF_DATA);
    if(hMutexWriteNetBuffer == NULL)
    {
        return FALSE;
    }
	    return  TRUE;
}

/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: ETH handle
  * @retval None
  */
 void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.Pin = 134;
  GPIO_InitStructure.Speed = 3;
  GPIO_InitStructure.Mode = 2;
  GPIO_InitStructure.Pull = 0 ; 
  GPIO_InitStructure.Alternate = 0x0B;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIO_InitStructure.Pin = 0x20;  //GPIO_PIN_5
  //HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.Pin = 50;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);                             
  GPIO_InitStructure.Pin = 26624;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure); 
}

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
  * In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
void low_level_init(void)
{ 
  __XXX_UNION_VALUE unVal;
  uint8_t macaddress[6],i,mac[6];
  mac[5] = 0x00;
  mac[4] = 0x09;
  mac[3] = 0xf5;
  mac[2] = 0x0c;
  mac[1] = 0xE2;
  mac[0] = 0x02;
  for(i=0;i<6;i++)
  {
	//GetCharSetData(SETDATA_Mac_NUM+i,&macaddress[5-i]);
	//ExGetSigInfo(E_TYPE_SYS, ID_u8Mac_1 + i, &unVal, NULL, BY_SRAM);
        macaddress[5-i] = mac[i];
  }
  EthHandle.Instance = ETH;  
  EthHandle.Init.MACAddr = macaddress;
  EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  EthHandle.Init.Speed = ETH_SPEED_100M;
  EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
  EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
  EthHandle.Init.RxMode = ETH_RXPOLLING_MODE;
  EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_SOFTWARE;
  EthHandle.Init.PhyAddress = 0x01;
  HAL_ETH_Init(&EthHandle);
  /* Initialize Tx Descriptors list: Chain Mode */
  HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &ETH_Tx_Buff[0][0], ETH_TXBUFNB);

  /* Initialize Rx Descriptors list: Chain Mode  */
  HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &ETH_Rx_Buff[0][0], ETH_RXBUFNB);

  /* Enable MAC and DMA transmission and reception */
  HAL_ETH_Start(&EthHandle);
  init_netMutex();
}

USHORT ReadBuff(UCHAR *pData)
{
    USHORT length = 0,i;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    
    MutexLock(hMutexWriteNetBuffer);
    if (HAL_ETH_GetReceivedFrame(&EthHandle) != HAL_OK)
    {
        MutexUnlock(hMutexWriteNetBuffer);
        return length;
    }
    MutexUnlock(hMutexWriteNetBuffer);
    
    length = EthHandle.RxFrameInfos.length;
    if((length >0)&&(length <= 1500))
    {
         
        memcpy( pData, (UCHAR*)EthHandle.RxFrameInfos.buffer,length);
        dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;

        /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
        for (i=0; i< (EthHandle.RxFrameInfos).SegCount; i++)
        {  
            dmarxdesc->Status = ETH_DMARXDESC_OWN;
            dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
        }

        /* Clear Segment_Count */
        (EthHandle.RxFrameInfos).SegCount =0;
        /* When Rx Buffer unavailable flag is set: clear it and resume reception */
        if (((EthHandle.Instance)->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
        {
            /* Clear RBUS ETHERNET DMA flag */
            (EthHandle.Instance)->DMASR = ETH_DMASR_RBUS;
            /* Resume DMA reception */
            (EthHandle.Instance)->DMARPDR = 0;
        }  
    }
    else
    {
        length = 0;
    }
    
    
    return length ;
}
static void ETH_FlushTransmitFIFO_1(ETH_HandleTypeDef *heth);
static void ETH_FlushTransmitFIFO_1(ETH_HandleTypeDef *heth)
{
  __IO uint32_t tmpreg = 0;
  
  /* Set the Flush Transmit FIFO bit */
  (heth->Instance)->DMAOMR |= ETH_DMAOMR_FTF;
  
  /* Wait until the write operation will be taken into account:
     at least four TX_CLK/RX_CLK clock cycles */
  tmpreg = (heth->Instance)->DMAOMR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->DMAOMR = tmpreg;
}

UCHAR WriteBuff(UINT16 u16Number, UCHAR *pData)
{
    //UINT8* PSendData;
   // UINT16 u16IdxI,u16IdxJ;
    UCHAR  RTN;
    HAL_StatusTypeDef TxRTN;
    MutexLock(hMutexWriteNetBuffer);
    if((EthHandle.TxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
        RTN = 0;
    }
    else if( u16Number > ETH_TX_BUF_SIZE)
    {  
        RTN = 0;
    }
    else
    {
        /* Copy data to Tx buffer*/
	ETH_FlushTransmitFIFO_1(&EthHandle);
        memcpy((UCHAR*)EthHandle.TxDesc->Buffer1Addr,pData ,u16Number);
        
        TxRTN = HAL_ETH_TransmitFrame(&EthHandle, u16Number);
        if(TxRTN != HAL_OK)
        {
            RTN = 0;
        }
        else
        {
            RTN = 1;
        }  
    }
	
    if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)		//如果还没下溢，则直接强制重发
    {
	    TxRTN = HAL_ETH_TransmitFrame(&EthHandle, u16Number);
	    /* Clear TUS ETHERNET DMA flag */
	    EthHandle.Instance->DMASR = ETH_DMASR_TUS;
	    /* Resume DMA transmission*/
	    EthHandle.Instance->DMATPDR = 0;
    }

    //(EthHandle.Instance)->DMAOMR &= ~ETH_DMAOMR_ST;
    MutexUnlock(hMutexWriteNetBuffer);    

    return RTN;
}

// ================* Function Prototypes ******************
// Local Function Prototypes
uint8_t SPI_Send_Byte (uint8_t u8Byte);
uint8_t SPI_Flash_WriteEnable(void);
uint8_t SPI_Flash_Wait4Write2End(void);
static void bsp_SPI_Error(void);

/*
*==============================================================================*
*                               CODE                                           *
*==============================================================================*
*/

/*=============================================================================
* Function      : HAL_SPI_MspDeInit
* Arguments     : SPI_HandleTypeDef *hspi
* Returns       : uint8_t
* Description   : DeInitializes SPI Peripheral and Communication
*=============================================================================*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI2)
	{
		FLASH_DISABLE();
		SPI_FORCE_RESET();                                                      // Force Reset 
		SPI_RELEASE_RESET();                                                    // Release from Force Reset 

		HAL_GPIO_DeInit(SPI_GPIO_PORT, SPI_CLK_PIN);                      // Configure SPI SCK as alternate function 
		HAL_GPIO_DeInit(SPI_GPIO_PORT, SPI_MISO_PIN);                     // Configure SPI MISO as alternate function 
		HAL_GPIO_DeInit(SPI_GPIO_PORT, SPI_MOSI_PIN);                     // Configure SPI MOSI as alternate function 
		HAL_NVIC_DisableIRQ(SPI_IRQn);                                          // Disable the NVIC for SPI
	}
	else
	{
		SPI3_NSS_DISABLE();
		__SPI3_FORCE_RESET();                                                      // Force Reset 
		__SPI3_RELEASE_RESET();                                                    // Release from Force Reset 

		HAL_GPIO_DeInit(SPI_3_GPIO_PORT, SPI_3__CLK_PIN);                      // Configure SPI SCK as alternate function 
		HAL_GPIO_DeInit(SPI_3_GPIO_PORT, SPI_3_MISO_PIN);                     // Configure SPI MISO as alternate function 
		HAL_GPIO_DeInit(SPI_3_GPIO_PORT, SPI_3_MOSI_PIN);                     // Configure SPI MOSI as alternate function 
		HAL_NVIC_DisableIRQ(SPI3_IRQn);                                          // Disable the NVIC for SPI
	}

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	if (hspi->Instance == SPI2)
	{
		SPI_CLK_DISABLE();                                                      // Disable SPI peripheral Clock
		SPI_GPIO_CLK_DISABLE();                                                 // Disable GPIO clock (SCK,MOSI,MISO)
		SPI_GPIO_CLK_ENABLE();                                                  // Enable GPIO clock (SCK,MOSI,MISO)

		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;                                  // Push Pull  or GPIO_MODE_AF_OD
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI_AF;

		GPIO_InitStruct.Pin       = SPI_CLK_PIN;                                // SPI SCK GPIO pin configuration
		GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
		HAL_GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pull      = GPIO_PULLUP;                                      // SPI MISO GPIO pin configuration 
		GPIO_InitStruct.Pin       = SPI_MISO_PIN;
		HAL_GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin       = SPI_MOSI_PIN;										// SPI MOSI GPIO pin configuration 
		HAL_GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);

		__GPIOE_CLK_ENABLE();
		GPIO_InitStruct.Pin       = SPI_CS_PIN;											// SPI CS GPIO pin configuration 
		GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;								// Push Pull  or GPIO_MODE_AF_OD
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;										// SPI CS GPIO pin configuration 
		HAL_GPIO_Init(SPI_NSS_PORT, &GPIO_InitStruct);
		SPI_CLK_ENABLE();																// Enable SPI peripheral Clock
	}
	else
	{
		__SPI3_CLK_DISABLE();                                                      // Disable SPI peripheral Clock
		__GPIOB_CLK_DISABLE();														//AAAA		Disable GPIO clock (SCK,MOSI,MISO)
		__GPIOB_CLK_ENABLE();														//AAAA		Enable GPIO clock (SCK,MOSI,MISO)

		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;								// Push Pull  or GPIO_MODE_AF_OD
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI3_AF;	 //GPIO_AF6_SPI3

		GPIO_InitStruct.Pin       = SPI_3__CLK_PIN;									// SPI SCK GPIO pin configuration
		GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
		HAL_GPIO_Init(SPI_3_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pull      = GPIO_PULLUP;									// SPI MISO GPIO pin configuration			
		GPIO_InitStruct.Pin       = SPI_3_MISO_PIN;									//	PB4
		HAL_GPIO_Init(SPI_3_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin       = SPI_3_MOSI_PIN;									// SPI MOSI GPIO pin configuration			PB5
		HAL_GPIO_Init(SPI_3_GPIO_PORT, &GPIO_InitStruct);

		__GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin       = SPI_3_CS_PIN;									// SPI CS GPIO pin configuration 
		GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;							// Push Pull  or GPIO_MODE_AF_OD
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;									// SPI CS GPIO pin configuration 
		HAL_GPIO_Init(SPI_3_NSS_PORT, &GPIO_InitStruct);
		__SPI3_CLK_ENABLE();														// Enable SPI peripheral Clock
	}

}

/*=============================================================================
* Function      : SPI_Flash_DeInit
* Arguments     : none
* Returns       : none
* Description   : DeInitialize SPI for SPi Configuration 
*=============================================================================*/
void SPI_Flash_DeInit(void)
{
	HAL_SPI_DeInit(&SpiHandle);
}

/*=============================================================================
* Function      : bsp_InitSpiFlash
* Arguments     : none
* Returns       : none
* Description   : Initialize SPI as Master For Flash communication. The Speed,
Polarity Selected as per Flash Specification
*=============================================================================*/
void bsp_InitSpiFlash(void)
{
	SpiHandle.Instance               = SPI_PERIPHERAL;  
	SPI_Flash_DeInit();
	spi_status_f = SPI_STATUS_WAIT;  
	if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
	{
		// Set the SPI parameters
		SpiHandle.Instance               = SPI_PERIPHERAL;
		SpiHandle.Init.BaudRatePrescaler = FLASH_FREQ;
		SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
		SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
		SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
		SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
		SpiHandle.Init.CRCPolynomial     = 7;                                       // NO CRC
		SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
		SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
		SpiHandle.Init.NSS               = SPI_NSS_SOFT;
		SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
		SpiHandle.Init.Mode              = SPI_MODE_MASTER;  
		HAL_SPI_MspInit(&SpiHandle);
		if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
		{
			/* Initialization Error */
			bsp_SPI_Error();                                                          //Error_Handler();
		}
		FLASH_DISABLE();
		spi_status_f = SPI_STATUS_INIT;
	}

	hMutexSPIFlashAccess = MutexCreate(UPPER_PRIO_OPERATE_SPI_FLASH);

	if (NULL == hMutexSPIFlashAccess) 
	{
		spi_status_f = SPI_STATUS_FAIL;
	}
	SPI_Flash_ReadID(&sf_m25pxx_info.identification);
	if(SF_M25P32_IDENTIFICATION == sf_m25pxx_info.identification)
	{
		sf_m25pxx_info.size = SF_M25P32_SIZE;
		sf_m25pxx_info.num_sectors = SF_M25P32_NUM_SECTORS;
	}
	else
	{
		sf_m25pxx_info.size = SF_M25P16_SIZE;
		sf_m25pxx_info.num_sectors = SF_M25P16_NUM_SECTORS;
	}    
}


/*=============================================================================
* Function      : SPI_Send_Byte
* Arguments     : u8Byte
* Returns       : uint8_t
* Description   : Send Byte- This is just sending Byte on SPI and Not Flash 
*               Write
*=============================================================================*/
uint8_t SPI_Send_Byte (uint8_t u8Byte)
{
	uint8_t    u8Dummy = 0;

	// Read byte from the SPI bus
	if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &u8Byte, (uint8_t*) &u8Dummy, 1, SpiTimeout) != HAL_OK)
	{
		spi_status_f = SPI_STATUS_FAIL;
		bsp_SPI_Error();
	}
	spi_status_f = SPI_STATUS_OK;
	return u8Dummy;
}

/*=============================================================================
* Function      : SPI_Flash_WriteEnable
* Arguments     : None
* Returns       : uint8_t
* Description   : Enables Write of SPI Flash 
*               
*=============================================================================*/
uint8_t SPI_Flash_WriteEnable(void)
{
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;
	FLASH_ENABLE();                                                               // Chip Select Low
	SPI_Send_Byte(SF_WR_ENABLE);                                                  // Write Enable Command
	if(SPI_STATUS_OK == spi_status_f)
		u8RetVal = FLASH_STATUS_ACCESS_OK;
	FLASH_DISABLE();                                                              // Chip Select HIgh
	return u8RetVal ;
}

/*=============================================================================
* Function      : SPI_Flash_Wait4Write2End
* Arguments     : none
* Returns       : uint8_t
* Description   : Actual Flash Write require Time and Cna be verified by loking
*               at the WIP (Write In Progress Flag)
*=============================================================================*/
uint8_t SPI_Flash_Wait4Write2End(void)
{
	static uint8_t flashstatus = 0;
	uint16_t  timeout;
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	FLASH_ENABLE();                                                               // Chip Select Low
	timeout = FLASH_WIP_TIMEOUT;

	flashstatus = SPI_Send_Byte(SF_RD_STATUSREG);
	if(SPI_STATUS_OK == spi_status_f) {
		while((flashstatus & SF_M25PXX_WIP_FLAG) == SF_M25PXX_WIP_FLAG) {
			--timeout;
			flashstatus = SPI_Send_Byte(SF_RD_STATUSREG);
			if(SPI_STATUS_OK != spi_status_f)
				break;                                                                  // Might break only the while loop
			if(0 == timeout)
				break;                                                                  // Might break only the while loop
		}
		if((SPI_STATUS_OK == spi_status_f) && (0 < timeout))
			u8RetVal = FLASH_STATUS_ACCESS_OK;
	}
	else {
		u8RetVal = FLASH_STATUS_WAIT;
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	return u8RetVal;
}
/*=============================================================================
* Function      : SPI_Flash_EraseSector
* Arguments     : uint32_t SectorAddr
* Returns       : u8Err
* Description   : Erase Sector
*=============================================================================*/
uint8_t SPI_Flash_EraseSector(uint32_t SectorAddr)
{
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	if(sf_m25pxx_info.size <= SectorAddr )
		return FLASH_STATUS_INVALID_ADD;

	if(FLASH_STATUS_ACCESS_OK == SPI_Flash_WriteEnable()) {                       // Write enable instruction
		FLASH_ENABLE();                                                             // Chip Select Low
		SPI_Send_Byte(SF_SECTOR_ERASE);                                             // Send Sector Erase
		if(SPI_STATUS_OK == spi_status_f) {
			SPI_Send_Byte((SectorAddr & 0xFF0000) >> 16);                             // Send High nibble
			if(SPI_STATUS_OK == spi_status_f) {
				SPI_Send_Byte((SectorAddr & 0xFF00) >> 8);                              // Send Medium nibble
				if(SPI_STATUS_OK == spi_status_f) {
					SPI_Send_Byte(SectorAddr & 0xFF);                                     // Send Low nibble
					if(SPI_STATUS_OK == spi_status_f)
						u8RetVal = FLASH_STATUS_ACCESS_OK;
				}
			}
		}
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	if(FLASH_STATUS_ACCESS_OK == u8RetVal) 
		u8RetVal = SPI_Flash_Wait4Write2End();
	return u8RetVal;
}

/*=============================================================================
* Function      : SPI_Flash_EraseBlock
* Arguments     : uint32_t BlockAddr
* Returns       : u8Err
* Description   : Erase Sector
*=============================================================================*/
uint8_t SPI_Flash_EraseBlock(uint32_t SectorAddr)
{
	MutexLock(hMutexSPIFlashAccess);

	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	if(sf_m25pxx_info.size <= SectorAddr ) {
		MutexUnlock(hMutexSPIFlashAccess);
		return FLASH_STATUS_INVALID_ADD;
	}

	if(FLASH_STATUS_ACCESS_OK == SPI_Flash_WriteEnable()) {                       // Write enable instruction
		FLASH_ENABLE();                                                             // Chip Select Low
		SPI_Send_Byte(SF_BLOCK_ERASE);                                             // Send Sector Erase
		if(SPI_STATUS_OK == spi_status_f) {
			SPI_Send_Byte((SectorAddr & 0xFF0000) >> 16);                             // Send High nibble
			if(SPI_STATUS_OK == spi_status_f) {
				SPI_Send_Byte((SectorAddr & 0xFF00) >> 8);                              // Send Medium nibble
				if(SPI_STATUS_OK == spi_status_f) {
					SPI_Send_Byte(SectorAddr & 0xFF);                                     // Send Low nibble
					if(SPI_STATUS_OK == spi_status_f)
						u8RetVal = FLASH_STATUS_ACCESS_OK;
				}
			}
		}
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	if(FLASH_STATUS_ACCESS_OK == u8RetVal) 
		u8RetVal = SPI_Flash_Wait4Write2End();
	MutexUnlock(hMutexSPIFlashAccess);
	return u8RetVal;
}

/*=============================================================================
* Function      : SPI_Flash_EraseBulk
* Arguments     : none
* Returns       : FLASH_STATUS_ACCESS_OK, FLASH_STATUS_ACCESS_FAIL
* Description   : Erase Complete Flash
*=============================================================================*/
uint8_t SPI_Flash_EraseBulk(void)
{
	MutexLock(hMutexSPIFlashAccess);

	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	if(FLASH_STATUS_ACCESS_OK == SPI_Flash_WriteEnable()) {                       // Write enable instruction
		FLASH_ENABLE();                                                             // Chip Select Low
		SPI_Send_Byte(SF_BULK_ERASE);                                               // Send Sector Erase
		if(SPI_STATUS_OK == spi_status_f) {
			FLASH_DISABLE();                                                          // Chip Select HIgh
			u8RetVal= SPI_Flash_Wait4Write2End();
		}
	}
	FLASH_DISABLE();
	u8RetVal = FLASH_STATUS_WAIT;
	MutexUnlock(hMutexSPIFlashAccess);
	return u8RetVal;
}

/*=============================================================================
* Function      : SPI_Flash_PageProgram
* Arguments     : uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes
* Returns       : u8Err
* Description   : FLash Write Functionality If NUmber of bytes is greater than 
Page size then this function has to be called again
*=============================================================================*/
uint8_t SPI_Flash_PageProgram(uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes)
{
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	if(FLASH_STATUS_ACCESS_OK == SPI_Flash_WriteEnable()) {                     // Write enable instruction
		FLASH_ENABLE();                                                             // Chip Select Low
		SPI_Send_Byte(SF_PAGE_PROGRAM);                                             // Page Program
		if(SPI_STATUS_OK == spi_status_f) {
			SPI_Send_Byte((Addr & 0xFF0000) >> 16);                                   // Send High nibble
			if(SPI_STATUS_OK == spi_status_f) {
				SPI_Send_Byte((Addr & 0xFF00) >> 8);                                    // Send Medium nibble
				if(SPI_STATUS_OK == spi_status_f) {
					SPI_Send_Byte(Addr & 0xFF);                                           // Send Low nibble
					if(SPI_STATUS_OK == spi_status_f) {
						while (NumBytes--)
						{
							SPI_Send_Byte(*pBuf);
							if(SPI_STATUS_OK != spi_status_f)
								break;
							pBuf++;
						}
						if(SPI_STATUS_OK == spi_status_f)
							u8RetVal = FLASH_STATUS_ACCESS_OK;
					}
				}
			}
		}
		FLASH_DISABLE();                                                            // Chip Select HIgh
		u8RetVal = SPI_Flash_Wait4Write2End();
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	return u8RetVal;
}

/*=============================================================================
* Function      : SPI_Flash_ReadBytes
* Arguments     : uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes
* Returns       : u8Err
* Description   : FLash Write Functionality If NUmber of bytes is greater than 
Page size then this function has to be called again
*=============================================================================*/
uint8_t SPI_Flash_ReadBytes(uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes)
{
	MutexLock(hMutexSPIFlashAccess);
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;
	FLASH_ENABLE();                                                               // Chip Select Low
	SPI_Send_Byte(SF_RD_DATA_NORMAL);                                             // Read Byte Normal Speed
	if(SPI_STATUS_OK == spi_status_f) {
		SPI_Send_Byte((Addr & 0xFF0000) >> 16);                                     // Send High nibble
		if(SPI_STATUS_OK == spi_status_f) {
			SPI_Send_Byte((Addr & 0xFF00) >> 8);                                      // Send Medium nibble
			if(SPI_STATUS_OK == spi_status_f) {
				SPI_Send_Byte(Addr & 0xFF);                                             // Send Low nibble
				if(SPI_STATUS_OK == spi_status_f) {
					while (NumBytes--) {
						*pBuf++ = SPI_Send_Byte(DUMMY_BYTE);                                // Dummy Write 
						if(SPI_STATUS_OK != spi_status_f)
							break;
					}
					if(SPI_STATUS_OK == spi_status_f)
						u8RetVal = FLASH_STATUS_ACCESS_OK;
				}
			}
		}
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	MutexUnlock(hMutexSPIFlashAccess);
	return u8RetVal;
}

/*=============================================================================
* Function      : SPI_Flash_Write_Bytes
* Arguments     : uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes
* Returns       : u8Err
* Description   : Handles High level writing of Bytes on SPI FLash
*=============================================================================*/
uint8_t SPI_Flash_Write_Bytes(uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes)
{
	MutexLock(hMutexSPIFlashAccess);

	uint32_t WrAdd;
	uint16_t WrBytes;
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	if(sf_m25pxx_info.size <= (Addr+NumBytes)) {
		MutexUnlock(hMutexSPIFlashAccess);
		return FLASH_STATUS_INVALID_ADD;
	}
	WrAdd = ((Addr / SF_M25PXX_PAGE_SIZE) + 1) * SF_M25PXX_PAGE_SIZE;

	if(WrAdd >= (Addr+NumBytes)) 
		u8RetVal = SPI_Flash_PageProgram(pBuf, Addr,NumBytes);
	else
	{
		WrBytes = WrAdd - Addr;
		WrAdd = Addr;
		u8RetVal = SPI_Flash_PageProgram(pBuf, WrAdd,WrBytes);
		if(FLASH_STATUS_ACCESS_OK == u8RetVal) {
			NumBytes -= WrBytes;
			WrAdd += WrBytes;
			pBuf += WrBytes;
			while (NumBytes > SF_M25PXX_PAGE_SIZE) {
				u8RetVal = SPI_Flash_PageProgram(pBuf, WrAdd,SF_M25PXX_PAGE_SIZE);
				if(FLASH_STATUS_ACCESS_FAIL == u8RetVal) {
					MutexUnlock(hMutexSPIFlashAccess);
					return u8RetVal;
				}
				NumBytes -= SF_M25PXX_PAGE_SIZE;
				WrAdd += SF_M25PXX_PAGE_SIZE;
				pBuf += SF_M25PXX_PAGE_SIZE;
			}
			if(NumBytes)
				u8RetVal = SPI_Flash_PageProgram(pBuf, WrAdd,NumBytes);
		}
	}
	MutexUnlock(hMutexSPIFlashAccess);
	return u8RetVal;
}

/*=============================================================================
* Function      : SPI_Flash_ReadSatusReg
* Arguments     : None
* Returns       : uint8_t
* Description   : Reads Status Register of SPI Flash
*=============================================================================*/
uint8_t SPI_Flash_ReadSatusReg(uint8_t *val)
{
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	FLASH_ENABLE();                                                               // Chip Select Low
	*val = SPI_Send_Byte(SF_RD_STATUSREG);                                        // Read Status Register
	if(SPI_STATUS_OK == spi_status_f) {
		*val = SPI_Send_Byte(DUMMY_BYTE);
		if(SPI_STATUS_OK == spi_status_f) {
			*val = SPI_Send_Byte(DUMMY_BYTE);
			if(SPI_STATUS_OK == spi_status_f)
				u8RetVal = FLASH_STATUS_ACCESS_OK;
		}
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	return (u8RetVal);
}

/*=============================================================================
* Function      : SPI_Flash_WriteSatusReg
* Arguments     : uint8_t u8Byte
* Returns       : uint8_t
* Description   : Write Status Register of SPI Flash
*=============================================================================*/
uint8_t SPI_Flash_WriteSatusReg(uint8_t u8Byte)
{
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	if(FLASH_STATUS_ACCESS_OK == SPI_Flash_WriteEnable()) {                       // Write enable instruction
		FLASH_ENABLE();                                                             // Chip Select Low
		SPI_Send_Byte(SF_WR_STATUSREG);                                    // Write Status Register
		if(SPI_STATUS_OK == spi_status_f) {
			SPI_Send_Byte(u8Byte);
			if(SPI_STATUS_OK == spi_status_f) 
				u8RetVal = FLASH_STATUS_ACCESS_OK;
		}
		FLASH_DISABLE();                                                            // Chip Select HIgh
		if(FLASH_STATUS_ACCESS_OK == u8RetVal)
			u8RetVal = SPI_Flash_Wait4Write2End();                                    // Wait for WIP to get 0
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	return (u8RetVal);
}

/*=============================================================================
* Function      : SPI_Flash_ReadID
* Arguments     : None
* Returns       : uint32_t
* Description   : Reads ID of SPI Flash
*=============================================================================*/
uint8_t SPI_Flash_ReadID(uint32_t *id)
{
	uint8_t u8Temp,u8Temp1,u8Temp2;
	uint8_t u8RetVal = FLASH_STATUS_ACCESS_FAIL;

	*id = 0;
	FLASH_ENABLE();                                                               // Chip Select Low
	u8Temp = SPI_Send_Byte(SF_READ_IDENTIFICATION);                               // Read Byte Normal Speed
	if(SPI_STATUS_OK == spi_status_f) {
		u8Temp = SPI_Send_Byte(DUMMY_BYTE);
		if(SPI_STATUS_OK == spi_status_f) {
			u8Temp1 = SPI_Send_Byte(DUMMY_BYTE);
			if(SPI_STATUS_OK == spi_status_f) {
				u8Temp2 =  SPI_Send_Byte(DUMMY_BYTE);
				if(SPI_STATUS_OK == spi_status_f) {
					*id = (u8Temp << 16) | (u8Temp1 << 8) | u8Temp2;
					u8RetVal = FLASH_STATUS_ACCESS_OK;
				}
			}
		}
	}
	FLASH_DISABLE();                                                              // Chip Select HIgh
	*id = (u8Temp << 16) | (u8Temp1 << 8) | u8Temp2;
	return (u8RetVal);
}


/*=============================================================================
* Function      : bsp_SPI_Error
* Arguments     : void
* Returns       : void
* Description   : SPI Error
*=============================================================================*/
static void bsp_SPI_Error(void)
{
	spi_status_f = SPI_STATUS_FAIL;
	/* De-initialize the SPI comunication bus */
	HAL_SPI_DeInit(&SpiHandle);

	/* Re-Initiaize the SPI comunication bus */
	bsp_InitSpiFlash();
}
//由于需要1000V和750V，所以这里两个版本用于写保护的IO都要初始化
//另外该初始化只在启动时运行一次。如果启动后还需要控制写保护，该函数需要根据版本分别控制
static void BSP_Flash_WP_Ctrl(GPIO_PinState u8CtrlStatus)
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,u8CtrlStatus);
}
void SPIFlashInit(void)	//SPI2
{
	bsp_InitSpiFlash();
	BSP_Flash_WP_Ctrl(FLASH_WP_ENABLE);
}

void SPIFlashRead(UCHAR *pBuff , UINT32 Address, UINT32 Num)
{
	UCHAR i;
	UINT32 RTN;
	for(i =0;i<3;i++)
	{
		RTN = SPI_Flash_ReadBytes(pBuff, Address, Num);
		if(RTN == FLASH_STATUS_ACCESS_OK)
		{
			break;
		}
		Delay(500);
	}
}

void SPIFlashWrite(UCHAR *pBuff , UINT32 Address, UINT32 Num)
{
	UCHAR i;
	UINT32 RTN;
	for(i =0;i<3;i++)
	{
		RTN = SPI_Flash_Write_Bytes(pBuff, Address, Num);
		if(RTN == FLASH_STATUS_ACCESS_OK)
		{
			break;
		}
		Delay(500);
	}
}

void SPIFlashEraseBlock(UINT32 BlockAddress)
{
	UCHAR i;
	UINT32 RTN;
	for(i =0;i<3;i++)
	{
		RTN = SPI_Flash_EraseBlock(BlockAddress);

		if(RTN == FLASH_STATUS_ACCESS_OK)
		{
			break;
		}
		Delay(500);
	}
}

void SPIFlashEraseSector(UINT32 SectorAddress)
{
	UCHAR i;
	UINT32 RTN;
	for(i =0;i<3;i++)
	{
		RTN = SPI_Flash_EraseSector(SectorAddress);

		if(RTN == FLASH_STATUS_ACCESS_OK)
		{
			break;
		}
		Delay(500);
	}
}

void SPIFlashEraseChip(void)
{
	UCHAR i;
	UINT32 RTN;

	for(i =0;i<3;i++)
	{
		RTN = SPI_Flash_EraseBulk();

		if(RTN == FLASH_STATUS_ACCESS_OK)
		{
			break;
		}
		Delay(500);
	}
}

static void bsp_SPI3_Error(void)
{
	/* De-initialize the SPI comunication bus */
	HAL_SPI_DeInit(&SPI3Handle);
	/* Re-Initiaize the SPI comunication bus */
	BSP_InitSPI3();
}


void BSP_InitSPI3(void)
{
	SPI3Handle.Instance = SPI3;  
	HAL_SPI_DeInit(&SPI3Handle);
	if(HAL_SPI_GetState(&SPI3Handle) == HAL_SPI_STATE_RESET)
	{
		// Set the SPI parameters
		SPI3Handle.Instance               = SPI3;
		SPI3Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
		SPI3Handle.Init.Direction         = SPI_DIRECTION_2LINES;
		SPI3Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;			//SPI_PHASE_1EDGE;
		SPI3Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;			//SPI_POLARITY_LOW;
		SPI3Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
		SPI3Handle.Init.CRCPolynomial     = 7;                                       // NO CRC
		SPI3Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
		SPI3Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
		SPI3Handle.Init.NSS               = SPI_NSS_SOFT;
		SPI3Handle.Init.TIMode            = SPI_TIMODE_DISABLED;
		SPI3Handle.Init.Mode              = SPI_MODE_MASTER;  
		HAL_SPI_MspInit(&SPI3Handle);
		if(HAL_SPI_Init(&SPI3Handle) != HAL_OK)
		{
			/* Initialization Error */
			//bsp_SPI3_Error();                                                          //Error_Handler();
			SPI3_NSS_DISABLE();
		}
		SPI3_NSS_DISABLE();
	}
}

uint8_t SPI3_Send_Byte (uint8_t u8Byte)
{
	uint8_t	u8Dummy = 0;

	// Read byte from the SPI bus
	if(HAL_SPI_TransmitReceive(&SPI3Handle, (uint8_t*) &u8Byte, (uint8_t*) &u8Dummy, 1, SPI3Timeout) != HAL_OK)
	{
		bsp_SPI3_Error();
	}
	return u8Dummy;
}

uint8_t SPI3_Send_ByteS(uint8_t* pSData, uint8_t* pRData,uint16_t Num)
{
	// Read byte from the SPI bus
	if(HAL_SPI_TransmitReceive(&SPI3Handle, (uint8_t*)pSData, (uint8_t*)pRData, Num, SPI3Timeout) != HAL_OK)
	{
		bsp_SPI3_Error();
		return FALSE;
	}
	return TRUE;
}

/**************************************************************
 *函数功能:设置屏蔽位，修改制定的位而不影响其它位
 *入口参数:address:MCP2515寄存器地址
 *		   mask:修改保护屏蔽字节，位为1表示该位可更改，为0表示不影响该位
 *		   dat : 修改数据字节
**************************************************************/
static void mcp2515_BitModify(uint8_t reg, uint8_t mask, uint8_t dat)
{
	uint8_t data[5];
	SPI3_NSS_ENABLE();
	data[0] = MCP_BITMOD;//0x05
	data[1] = reg;
	data[2] = mask;
	data[3] = dat;
	SPI3_Send_ByteS(data, SPI3_bufR, 4);
	SPI3_NSS_DISABLE();
	
}

void MCP2515_Reset(void)
{
	SPI3_INT_DISABLE();
	SPI3_NSS_ENABLE();
	SPI3_Send_Byte(MCP_RESET);
	SPI3_NSS_DISABLE();
	SPI3_INT_ENABLE();
}

uint8_t mcp2515_ReadRegister(uint8_t reg)
{
	//struct mcp251x_priv *priv = dev_get_drvdata(&spi->dev);
	uint8_t val = 0;
	uint8_t bufS[4];

	bufS[0] = MCP_READ;
	bufS[1] = reg;
	bufS[2] = 0x55;			//data!!

	SPI3_NSS_ENABLE();
	if (TRUE == SPI3_Send_ByteS(bufS, SPI3_bufR, 3))
	{
		val = SPI3_bufR[2];
	}
	//SPI3_Send_Byte(buf11[0]);
	//SPI3_Send_Byte(buf11[1]);
	//val = SPI3_Send_Byte(buf11[2]);
	SPI3_NSS_DISABLE();
	
	return val;
}

static void mcp2515_read_2regs(uint8_t reg, uint8_t *v1, uint8_t *v2)
{
	uint8_t bufS[4];
	SPI3_NSS_ENABLE();
	bufS[0] = MCP_READ;
	bufS[1] = reg;
	SPI3_Send_ByteS(bufS, SPI3_bufR, 4);
	*v1 = SPI3_bufR[2];
	*v2 = SPI3_bufR[3];
	SPI3_NSS_DISABLE();
}
uint8_t mcp2515_SetCanCtrlMode(const uint8_t newmode)
{
	uint8_t tmp = 0;
	
	//REQOP2 REQOP1 REQOP0 ABAT OSM CLKEN CLKPRE1 CLKPRE0
	mcp2515_BitModify(MCP_CANCTRL, MCP2515_MODE_MASK, newmode);	 //0x05		0x0F	0xE7	0x87
	OSTimeDly(20);
	//OPMOD2 OPMOD1 OPMOD0 — ICOD2 ICOD1 ICOD0 —
	tmp = mcp2515_ReadRegister(MCP_CANSTAT);					//读状态寄存器
	//tmp &= MCP2515_MODE_MASK;
	if((tmp & MODE_STAT_MASK) != (newmode & MODE_STAT_MASK))
	{
		return MCP2515_FAIL;
	}
	return MCP2515_OK; 
}

/**************************************************************
 *函数名称:mcp2515_write_nregister          
 *函数功能:写多个MCP2515寄存器参数
 *入口参数:address:MCP2515寄存器地址
 *		   dat:配置参数地址	 
 *			num: 要写的寄存器个数
 *出口参数:无
**************************************************************/
void mcp2515_WriteRegisterS(uint8_t address, uint8_t *pdat, uint8_t num)
{
	uint8_t u8Idx = 0;
	uint8_t SData[25];
	SData[0] = MCP_WRITE;
	SData[1] = address;
	for (u8Idx = 0; u8Idx < num && u8Idx< 20; u8Idx++)
	{
		SData[u8Idx+2] = pdat[u8Idx];
	}
	
	SPI3_NSS_ENABLE();
	SPI3_Send_ByteS(SData, SPI3_bufR, num+2);
	SPI3_NSS_DISABLE();
}


void mcp2515_WriteRegister(uint8_t address, uint8_t val)
{
	uint8_t SData[20];
	SData[0] = MCP_WRITE;
	SData[1] = address;
	SData[2] = val;

	SPI3_NSS_ENABLE();
	SPI3_Send_ByteS(SData, SPI3_bufR, 3);
	SPI3_NSS_DISABLE();
}



/**************************************************************
 *函数名称:mcp2515_ConfigRate          
 *函数功能:设置mcp2515 can通信波特率
 *入口参数:无
 *出口参数:无    
波特率   CFG1    CFG2    CFG3
125Kbps   0x03   0x9E    0x03
100Kbps   0x04   0x9E    0x03
50Kbps    0x09   0xA4    0x04
20Kbps    0x18   0xA4    0x04
250K	0x03	 0x8a	0x01
//以上数据来源于网上
**************************************************************/
uint8_t mcp2515_ConfigRate(const uint8_t CanBandRate)
{
	uint8_t setflag, cfg1, cfg2, cfg3;
	
	setflag = 0;
	
	switch (CanBandRate) 
	{
		case CAN_20KBPS:
			cfg1 = MCP_16MHz_20KBPS_CFG1 ;
			cfg2 = MCP_16MHz_20KBPS_CFG2 ;
			cfg3 = MCP_16MHz_20KBPS_CFG3 ;
			setflag = 1;
			break;
 		case CAN_100KBPS:
			cfg1 = MCP_16MHz_100KBPS_CFG1 ;
			cfg2 = MCP_16MHz_100KBPS_CFG2 ;
			cfg3 = MCP_16MHz_100KBPS_CFG3 ;
			setflag = 1;
			break;
			case CAN_125KBPS:	
			cfg1 = MCP_16MHz_125KBPS_CFG1;		//0x03
			cfg2 = MCP_16MHz_125KBPS_CFG2;		//0x9E
			cfg3 = MCP_16MHz_125KBPS_CFG3;		//0x03 
			setflag = 1;
			break;
			case CAN_250KBPS:
			cfg1 = MCP_16MHz_250KBPS_CFG1;		//03
			cfg2 = MCP_16MHz_250KBPS_CFG2;		//8A
			cfg3 = MCP_16MHz_250KBPS_CFG3;		//01
			setflag = 1;
			break;

		default:
			setflag = 0;
			break;
	}
	
	if (setflag) 
	{
		//设置CNF1、 CNF2 和CNF3,即波特率 
		mcp2515_WriteRegister(MCP_CNF1, cfg1);
		mcp2515_WriteRegister(MCP_CNF2, cfg2);
		mcp2515_WriteRegister(MCP_CNF3, cfg3);
		return MCP2515_OK;
	}
	else 
	{
		return MCP2515_FAIL;
	}
} 
/**************************************************************
 *函数名称:mcp2515_write_can_mask          
 *函数功能:写MCP2515 CAN 屏蔽寄存器
 *入口参数:mcp_addr: MCP2515地址
 *		   ext:1为扩展帧;0为标准帧
 *		   mask:要写入的CAN帧头屏蔽字
 *出口参数:无 
**************************************************************/
void mcp2515_write_can_mask(uint8_t mcp_addr, uint8_t ext, uint32_t mask )
{
	uint16_t canid = 0;
	uint8_t tbufdata[4];

	canid = (uint16_t)(mask & 0x0FFFF);

	if ( ext == 1) 
	{
		tbufdata[MCP_EID0] = (uint8_t) (canid & 0xFF);
		tbufdata[MCP_EID8] = (uint8_t) (canid >>8);
		canid = (uint16_t)( mask >>16 );
		tbufdata[MCP_SIDL] = (uint8_t) (canid & 0x03);
		tbufdata[MCP_SIDL] += (uint8_t) ((canid & 0x1C )*8);
		//tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
		tbufdata[MCP_SIDH] = (uint8_t) (canid / 32 );
	}
	else 
	{
		tbufdata[MCP_SIDH] = (uint8_t) (canid / 8 );
		tbufdata[MCP_SIDL] = (uint8_t) ((canid & 0x07 )*32);
		tbufdata[MCP_EID0] = 0;
		tbufdata[MCP_EID8] = 0;
	}

	mcp2515_WriteRegisterS(mcp_addr, tbufdata, 4);
}


/**************************************************************
 *函数名称:mcp2515_write_can_filter          
 *函数功能:写MCP2515 CAN 滤波寄存器
 *入口参数:mcp_addr: MCP2515地址
 *		   ext:1为扩展帧;0为标准帧
 *		   filter:要写入的CAN数据滤波字
 *出口参数:无 
**************************************************************/
void mcp2515_write_can_filter(uint8_t mcp_addr, uint8_t ext, uint32_t filter)
{
    uint16_t canid;
    uint8_t tbufdata[4];
	
    canid = (uint16_t)(filter & 0x0FFFF);
    
	if ( ext == 1) 
	{
        tbufdata[MCP_EID0] = (uint8_t) (canid & 0xFF);
        tbufdata[MCP_EID8] = (uint8_t) (canid >>8);
        canid = (uint16_t)( filter >>16 );
        tbufdata[MCP_SIDL] = (uint8_t) (canid & 0x03);
        tbufdata[MCP_SIDL] += (uint8_t) ((canid & 0x1C )*8);
        tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
        tbufdata[MCP_SIDH] = (uint8_t) (canid / 32 );
    }
    else 
	{
        tbufdata[MCP_SIDH] = (uint8_t) (canid / 8 );
        tbufdata[MCP_SIDL] = (uint8_t) ((canid & 0x07 )*32);
        tbufdata[MCP_EID0] = 0;
        tbufdata[MCP_EID8] = 0;
    }
	mcp2515_WriteRegisterS(mcp_addr, tbufdata, 4);
}


/**************************************************************
 *函数名称:mcp2515_InitCanBuffers          
 *函数功能:初始化MCP2515 CAN缓冲区
 *入口参数:无
 *出口参数:无 
**************************************************************/
void mcp2515_InitCanBuffers(void)
{
	uint8_t i, reg1, reg2, reg3;
	//uint8_t val = 0;
	//uint8_t Mask = 0;
	//设置3个发送缓冲区的优先级
	//U-0 R-0 R-0 R-0 R/W-0 U-0 R/W-0 R/W-0
	//— ABTF MLOA TXERR TXREQ — TXP1 TXP0
#define TXP0 1
#define TXP1 2
#define TXREQ 8

	mcp2515_BitModify(MCP_TXB0CTRL, 0x0B, 0x03);       //buf0最高优先级
	//Mask = TXP0 | TXP1 | TXREQ;
	//val = (TXP0 | TXP1)&(~TXREQ);
	//mcp2515_BitModify(MCP_TXB0CTRL, Mask, val);       //buf0最高优先级

	mcp2515_BitModify(MCP_TXB1CTRL, 0x0B, 0x02);       //buf1次高优先级 
	//val =(TXP1)&(~TXREQ);
	//mcp2515_BitModify(MCP_TXB1CTRL, Mask, val);

	mcp2515_BitModify(MCP_TXB2CTRL, 0x0B, 0x01);       //buf2最低优先级 
	//val =(TXP0)&(~TXREQ);
	//mcp2515_BitModify(MCP_TXB2CTRL, Mask, val);

	// Mark all filter bits as don't care:
	//SID10 SID9 SID8 SID7 SID6 SID5 SID4 SID3			RXM0SIDH   ---0X20(MCP_RXM0SIDH)
	//SID2 SID1 SID0 — — — EID17 EID16				RXM0SIDL   ---0X21
	//EID15 EID14 EID13 EID12 EID11 EID10 EID9 EID8		RXM0EID8   ---0X22
	//EID7 EID6 EID5 EID4 EID3 EID2 EID1 EID0			RXM0EID0   ---0X23
	mcp2515_write_can_mask(MCP_RXM0SIDH, CAN_EXTID, CAN_ID_MASK);	//屏蔽寄存器 1需要比较  0不做判断！

	//SID10 SID9 SID8 SID7 SID6 SID5 SID4 SID3			RXM1SIDH   ---0X20(MCP_RXM1SIDH)
	//SID2 SID1 SID0 — — — EID17 EID16				RXM1SIDL   ---0X21
	//EID15 EID14 EID13 EID12 EID11 EID10 EID9 EID8		RXM1EID8   ---0X22
	//EID7 EID6 EID5 EID4 EID3 EID2 EID1 EID0			RXM1EID0   ---0X23
	mcp2515_write_can_mask(MCP_RXM1SIDH, CAN_EXTID, CAN_ID_MASK);

	// Anyway, set all filters to 0:
	//SID10 SID9 SID8 SID7 SID6 SID5 SID4 SID3			0(MCP_RXF0SIDH)	4(MCP_RXF1SIDH)		。。。。。。。。。。
	//SID2 SID1 SID0 — EXIDE — EID17 EID16			1				5					。。。。。。。。。。
	//EID15 EID14 EID13 EID12 EID11 EID10 EID9 EID8		2				6					。。。。。。。。。。	
	//EID7 EID6 EID5 EID4 EID3 EID2 EID1 EID0			3				7					。。。。。。。。。。
	mcp2515_write_can_filter(MCP_RXF0SIDH, CAN_EXTID, CAN_ID_FILTER); // RXB0: extended 
	//mcp2515_write_can_filter(MCP_RXF1SIDH, CAN_EXTID, CAN_ID_FILTER); // RXB0: do not receive standard
	mcp2515_write_can_filter(MCP_RXF1SIDH, CAN_STDID, 0); // RXB0: do not receive standard 

	mcp2515_write_can_filter(MCP_RXF2SIDH, CAN_EXTID, CAN_ID_FILTER); // RXB1: extended 
	//mcp2515_write_can_filter(MCP_RXF3SIDH, CAN_EXTID, CAN_ID_FILTER); // RXB1: do not receive standard
	//mcp2515_write_can_filter(MCP_RXF4SIDH, CAN_EXTID, CAN_ID_FILTER);
	//mcp2515_write_can_filter(MCP_RXF5SIDH, CAN_EXTID, CAN_ID_FILTER);
	mcp2515_write_can_filter(MCP_RXF3SIDH, CAN_STDID, 0); // RXB1: do not receive standard
	mcp2515_write_can_filter(MCP_RXF4SIDH, CAN_STDID, 0);
	mcp2515_write_can_filter(MCP_RXF5SIDH, CAN_STDID, 0);

	// Clear, deactivate the three transmit buffers
	// TXBnCTRL -> TXBnD7
	//— ABTF MLOA TXERR TXREQ — TXP1 TXP0
	reg1 = MCP_TXB0CTRL;
	reg2 = MCP_TXB1CTRL;
	reg3 = MCP_TXB2CTRL;
	for (i = 0; i < 14; i++)  // in-buffer loop
	{
		mcp2515_WriteRegister(reg1, 0);
		mcp2515_WriteRegister(reg2, 0);
		mcp2515_WriteRegister(reg3, 0);
		reg1++;
		reg2++;
		reg3++;
	}

	// and clear, deactivate the two receive buffers.
	//— RXM1 RXM0 — RXRTR BUKT BUKT1 FILHIT0
	mcp2515_WriteRegister(MCP_RXB0CTRL, 0);
	//— RXM1 RXM0 — RXRTR FILHIT2 FILHIT1 FILHIT0
	mcp2515_WriteRegister(MCP_RXB1CTRL, 0);
}

uint8_t MCP2515_init(const uint8_t CanBandRate)
{
	uint8_t res = 0;
	uint8_t uIdx = 0;
	pRecvCAN3_Idx = 0;
	for (uIdx = 0; uIdx < CAN_3_MAX_BUFF; uIdx++)
	{
		CanRxMsgBuff_3[uIdx].ExtId = 0;
	}
	pRecvCAN3_Idx_Ring = 0;
	for (uIdx = 0; uIdx < CAN_3_MAX_BUFF; uIdx++)
	{
		CanRxMsgBuff_3_Ring[uIdx].ExtId = 0;
	}

	SPI3_INT_DISABLE();
	//复位
	MCP2515_Reset();

	//mcp2515进入配置模式
	//CANCTRL		REQOP2 REQOP1 REQOP0 ABAT OSM CLKEN CLKPRE1 CLKPRE0
	res = mcp2515_SetCanCtrlMode(MODE_CONFIG);
	if (res == MCP2515_FAIL)   /* function exit on error */
	{
		mcp2515_SetCanCtrlMode(MODE_NORMAL);
		SPI3_INT_ENABLE();
		return res;
	}
	OSTimeDly(10);
	//由于已经在配置模式了。所以不需要再进
	res = mcp2515_ConfigRate(CanBandRate);
	if ( res == MCP2515_OK ) 
	{
		mcp2515_InitCanBuffers();		//配置MCP2515芯片CAN缓冲区
			
		// enable both receive-buffers to receive messages
		// with std. and ext. identifiers
		// and enable rollover
		OSTimeDly(10);
		//— RXM1 RXM0 — RXRTR BUKT BUKT1 FILHIT0
		mcp2515_BitModify(MCP_RXB0CTRL, 
			MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK, 
			MCP_RXB_RX_EXT | MCP_RXB_BUKT_MASK );			//10 = 只接收符合滤波器条件的带有扩展标识符的有效报文|1 = 如果RXB0 满， RXB0 接收到的报文将被滚存至RXB1
		OSTimeDly(10);
		mcp2515_BitModify(MCP_RXB1CTRL, MCP_RXB_RX_MASK, 
			MCP_RXB_RX_EXT);								//10 = 只接收符合滤波器条件的带有扩展标识符的有效报文
		OSTimeDly(10);
	}
	//使能接收中断，当有接收中断产生时，拉低MCP2515 INT管脚
	//MERRE WAKIE ERRIE TX2IE TX1IE TX0IE RX1IE RX0IE
	//如果CANINTE.RXnIE 位被置1，器件会在INT 引脚产生一个中断，显示接收到报文有效。
	mcp2515_BitModify(MCP_CANINTE, MCP_RX_MASK ,MCP_RX_INT);
	// TXnRTS Bits as Digital Inputs
	//— — B2RTS B1RTS B0RTS B2RTSM B1RTSM B0RTSM
	mcp2515_WriteRegisterS(MCP_TXRTSCTRL, 0x00,1);

	//退出配置模式
	mcp2515_SetCanCtrlMode(MODE_NORMAL);	

	SPI3_INT_ENABLE();
	return res;	
}

//-1 则设置不成功	0 设置成功！
int MCP2515_Set_Recmask(int NO, uint32_t Rec_mask)
{
	int8_t res =0;
	SPI3_INT_DISABLE();
	if((NO <0) || (NO >= 2))
	{
		SPI3_INT_ENABLE();
		return -1;
	}
	res = mcp2515_SetCanCtrlMode(MODE_CONFIG);
	if ( res == MCP2515_FAIL )   /* function exit on error */
	{
		mcp2515_SetCanCtrlMode(MODE_NORMAL);
		SPI3_INT_ENABLE();
		return -1;
	}
	if(NO == 0)
	{
		mcp2515_write_can_mask(MCP_RXM0SIDH, CAN_EXTID, Rec_mask);					//RXBUF0 MASK
	}
	else if(NO == 1)
	{
		mcp2515_write_can_mask(MCP_RXM1SIDH, CAN_EXTID, Rec_mask);					//RXBUF1 MASK
	}
	else
	{
		res = -1;
	}
	mcp2515_SetCanCtrlMode(MODE_NORMAL);
	//MERRF WAKIF ERRIF TX2IF TX1IF TX0IF RX1IF RX0IF
	mcp2515_BitModify(MCP_CANINTF,MCP_USED_INT,0);					//清楚所有中断标识
	SPI3_INT_ENABLE();
	return res;
}


int MCP2515_Set_RecID(int NO, uint32_t Rec_ID)
{
	int8_t res =0;
	SPI3_INT_DISABLE();
	if((NO <0) || (NO >= 6))
	{
		SPI3_INT_ENABLE();
		return -1;
	}
	res = mcp2515_SetCanCtrlMode(MODE_CONFIG);
	if ( res == MCP2515_FAIL )   /* function exit on error */
	{
		mcp2515_SetCanCtrlMode(MODE_NORMAL);
		SPI3_INT_ENABLE();
		return -1;
	}
	if(NO == 0)
	{
		mcp2515_write_can_filter(MCP_RXF0SIDH, CAN_EXTID, Rec_ID); // RXB0: extended
	}
	else if(NO == 1)
	{
		mcp2515_write_can_filter(MCP_RXF1SIDH, CAN_EXTID, Rec_ID); // RXB0: do not receive standard 
	}
	else if(NO == 2)
	{
		mcp2515_write_can_filter(MCP_RXF2SIDH, CAN_EXTID,Rec_ID); // RXB1: extended 
	}
	else if(NO == 3)
	{
		mcp2515_write_can_filter(MCP_RXF3SIDH, CAN_EXTID, Rec_ID); // RXB1: do not receive standard
	}
	else if(NO == 4)
	{
		mcp2515_write_can_filter(MCP_RXF4SIDH, CAN_EXTID, Rec_ID);
	}
	else if(NO == 5)
	{
		mcp2515_write_can_filter(MCP_RXF5SIDH, CAN_EXTID, Rec_ID);
	}
	else
	{
		res = -1;
	}
	mcp2515_SetCanCtrlMode(MODE_NORMAL);
	//MERRF WAKIF ERRIF TX2IF TX1IF TX0IF RX1IF RX0IF
	mcp2515_BitModify(MCP_CANINTF,MCP_USED_INT,0);				//清楚所有中断标识
	SPI3_INT_ENABLE();
	return res;
}


void mcp2515_write_can_id(UINT8 mcp_addr, UINT8 ext, UINT32 can_id);
void mcp2515_write_can_id(UINT8 mcp_addr, UINT8 ext, UINT32 can_id)
{
	UINT16 canid;
	UINT8 tbufdata[4];

	canid = (UINT16)(can_id & 0x0FFFF);

	if ( ext == 1) 
	{
		tbufdata[MCP_EID0] = (UINT8) (canid & 0xFF);
		tbufdata[MCP_EID8] = (UINT8) (canid >>8);
		canid = (UINT16)( can_id >>16 );
		tbufdata[MCP_SIDL] = (UINT8) (canid & 0x03);
		tbufdata[MCP_SIDL] += (UINT8) ((canid & 0x1C )*8);
		tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
		tbufdata[MCP_SIDH] = (UINT8) (canid / 32 );
	}
	else 
	{	
		tbufdata[MCP_SIDH] = (UINT8) (canid / 8 );
		tbufdata[MCP_SIDL] = (UINT8) ((canid & 0x07 )*32);
		tbufdata[MCP_EID0] = 0;
		tbufdata[MCP_EID8] = 0;
	}

	mcp2515_WriteRegisterS(mcp_addr, tbufdata, 4);

}

/**************************************************************
 *函数名称:mcp2515_WriteCanMsg          
 *函数功能:发送CAN数据包
 *入口参数:buffer_sidh_addr：发送缓冲区地址， msg：要发送的CAN数据包
 *出口参数:无 
**************************************************************/
// Buffer can be MCP_TXBUF_0 MCP_TXBUF_1 or MCP_TXBUF_2

/**************************************************************
 *函数名称:MCP2515_RD_Status      
 *函数功能:读取MCP2515状态
 *入口参数:无
 *出口参数:返回MCP2515的状态   
**************************************************************/
uint8_t  mcp2515_RD_Status(void)
{
	UINT8 dat;
	uint8_t data[4];

	data[0] = MCP_READ_STATUS;
	data[1] = 0xff;
	data[2] = 0x00;

	SPI3_NSS_ENABLE();
	SPI3_Send_ByteS(data, SPI3_bufR, 3);
	SPI3_NSS_DISABLE();
	dat = SPI3_bufR[2];
	return dat;
}

void mcp2515_WriteCanMsg(UINT8 buffer_sidh_addr, CanMessage* msg)
{
    UINT8 mcp_addr, dlc, ext=0;

	mcp_addr = buffer_sidh_addr;
	dlc = msg->can3_flag & 0x0F;
	if((msg->can3_flag & MCP_IDE_MASK))
		ext = 1;
	
	mcp2515_WriteRegisterS(mcp_addr+5, &(msg->data[0]), dlc );				// write data bytes
	mcp2515_write_can_id( mcp_addr, ext, msg->identifier );					// write CAN id
	if ((msg->can3_flag & MCP_RTR_MASK))  dlc |= MCP_RTR_MASK;				// if RTR set bit in byte

	mcp2515_WriteRegister( (mcp_addr+4), dlc );								// write the RTR and DLC
}
 /**************************************************************
 *函数名称:mcp2515_StartTransmit          
 *函数功能:Start the transmission from one of the tx buffers.
 *入口参数:buffer_sidh_addr：发送缓冲区地址
 *出口参数:无 
**************************************************************/
// Buffer can be MCP_TXBUF_0 MCP_TXBUF_1 or MCP_TXBUF_2
void mcp2515_StartTransmit(uint8_t buffer_sidh_addr)
{
	// TXBnCTRL_addr = TXBnSIDH_addr - 1
	mcp2515_BitModify( buffer_sidh_addr-1 , MCP_TXB_TXREQ_M, MCP_TXB_TXREQ_M );
	//mcp2515_BitModify( buffer_sidh_addr-1 , 0x0B, 0x0B );
}


int MCP2515_Read(CanRxMsgTypeDef* pRFrame, uint16_t pRFrameNum)
{
	int OutIdx = 0;
	int iIdx = 0;
	for (iIdx = 0; iIdx < CAN_3_MAX_BUFF; iIdx++)
	{
		if (CanRxMsgBuff_3[iIdx].ExtId != 0)				//说明收到数据了
		{
			pRFrame[OutIdx++] = CanRxMsgBuff_3[iIdx];
			CanRxMsgBuff_3[iIdx].ExtId = 0;					//标记为取走了！
		}
		if (OutIdx >= pRFrameNum)
		{
			break;
		}
	}
	return OutIdx;
}
int MCP2515_Read_Ring(CanRxMsgTypeDef* pRFrame, uint16_t pRFrameNum)
{
	int OutIdx = 0;
	int iIdx = 0;
	for (iIdx = 0; iIdx < CAN_3_MAX_BUFF; iIdx++)
	{
		if (CanRxMsgBuff_3_Ring[iIdx].ExtId != 0)				//说明收到数据了
		{
			pRFrame[OutIdx++] = CanRxMsgBuff_3_Ring[iIdx];
			CanRxMsgBuff_3_Ring[iIdx].ExtId = 0;					//标记为取走了！
		}
		if (OutIdx >= pRFrameNum)
		{
			break;
		}
	}
	return OutIdx;
}

UCHAR mcp2515_getNextFreeTXBuf(UCHAR *txbuf_n)
{
	UCHAR res, i, ctrlval;
	//UCHAR ctrlregs[MCP_N_TXBUFFERS] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };
	UCHAR ctrlregs[MCP_N_TXBUFFERS] = { MCP_TXB0CTRL};

	res = MCP_ALLTXBUSY;
	*txbuf_n = 0x00;
	//ctrlval = mcp2515_ReadRegister(MCP_TXRTSCTRL);

	// check all 3 TX-Buffers
	for (i=0; i<MCP_N_TXBUFFERS; i++) 
	{
		//ctrlval = mcp2515_ReadRegister( MCP_EFLG );
		ctrlval = mcp2515_ReadRegister(ctrlregs[i] );
		//mcp2515_WriteRegister(ctrlregs[i],(ctrlval & 0XF7));
		//ctrlval = mcp2515_ReadRegister( ctrlregs[i] );

		if ( (ctrlval & MCP_TXB_TXREQ_M) == 0 ) 
		{
			*txbuf_n = ctrlregs[i]+1; // return SIDH-address of Buffer
			res = MCP2515_OK;
			return res; /* ! function exit */
		}
	}

	return res;
}

void mcp2515_WriteCanMsg_2(UCHAR buffer_sidh_addr, canmsg* msg,UINT8 u8L)
{
	UCHAR mcp_addr;
	mcp_addr = buffer_sidh_addr;
	UINT8 Buff[8];
	Buff[0] = (msg->Data[0] >> 24)&0xff;
	Buff[1] = (msg->Data[0] >> 16)&0xff;
	Buff[2] = (msg->Data[0] >> 8)&0xff;
	Buff[3] = (msg->Data[0])&0xff;
	Buff[4] = (msg->Data[1] >> 24)&0xff;
	Buff[5] = (msg->Data[1] >> 16)&0xff;
	Buff[6] = (msg->Data[1] >> 8)&0xff;
	Buff[7] = (msg->Data[1])&0xff;

	mcp2515_WriteRegisterS(mcp_addr+5, Buff, u8L);		// write data bytes
	mcp2515_write_can_id( mcp_addr, CAN_EXTID, msg->Id);			// write CAN id
	//if ( msg->rtr == 1)  u8L |= MCP_RTR_MASK;						// if RTR set bit in byte
	mcp2515_WriteRegister((mcp_addr+4), u8L);						// write the RTR and DLC
}

int MCP2515_Send_2(canmsg *pMsg, UINT8 DataL)
{
	UCHAR res, txbuf_n;
	WORD timeout = 0;

	SPI3_INT_DISABLE();

	//发送缓冲区为空或者发送超时退出
	do 
	{
		res = mcp2515_getNextFreeTXBuf(&txbuf_n); // info = addr.
		timeout++;
	} while ((res == MCP_ALLTXBUSY)&&(timeout < CANSENDTIMEOUT_2));


	if (timeout < CANSENDTIMEOUT_2) 
	{
		mcp2515_WriteCanMsg_2(txbuf_n, pMsg, DataL);
		mcp2515_StartTransmit(txbuf_n );
		SPI3_INT_ENABLE();
		return TRUE;
	}
	else
	{
		SPI3_INT_ENABLE();
		return FALSE;
	}
}

int MCP2515_Send(uint8_t* pSData, uint16_t Len)
{
	int num = 0;
	int i,timeout=0;
	uint8_t res = 0;
	CanMessage msg;
	uint8_t buff[CAN_FRAME_SIZE];
	SPI3_INT_DISABLE();
	if (Len <= 0)
	{
		SPI3_INT_ENABLE();
		return Len;
	}

	for(num=0; num < (Len / CAN_FRAME_SIZE); num++)
	{
		res = mcp2515_RD_Status();										//info = addr.
		while ((res & MCP_STAT_TX0IF)&&(timeout < CANSENDTIMEOUT))
		{
			timeout++;
			res = mcp2515_RD_Status();
			OSTimeDly(4);												//查询buffer等待时间，暂定1ms测试用，可以改动
		}
		if (timeout >= CANSENDTIMEOUT) 
		{
			SPI3_INT_ENABLE();
			return -1;
		}
		memcpy(buff, (char*)(pSData+num*CAN_FRAME_SIZE), CAN_FRAME_SIZE); 
		msg.can3_flag = 0;
		msg.can3_flag |= (buff[0] & 0x80);							/*IDE*/
		msg.can3_flag |= (buff[0] & 0x40);							/*RTR*/
		msg.can3_flag |= (buff[0] & 0x0f);							/*Data lenth*/
		msg.identifier = 0;
		for(i=0;i<4;i++)
		{
			msg.identifier+= buff[i+1]<<(24 - 8 * i);
			msg.data[i] = buff[i+5];
			msg.data[i+4] = buff[i+9];
		}
		msg.identifier = (msg.identifier >> 3);
		mcp2515_WriteCanMsg( MCP_TXB0CTRL + 1, &msg);
		mcp2515_StartTransmit(MCP_TXB0CTRL + 1);					//启动发送TX0 buffer
	}//end for

	SPI3_INT_ENABLE();
	return Len;
}


/**************************************************************
 *函数名称:mcp2515_ReadRegisterS          
 *函数功能:读多个MCP2515寄存器参数
 *入口参数:address:MCP2515寄存器地址
 *		   dat:配置参数地址	 
 *			num: 要读取的寄存器个数
 *出口参数:无
**************************************************************/
void mcp2515_ReadRegisterS(uint8_t address, uint8_t *pdat, uint8_t num)
{
	uint8_t len = 0;
	if (num >= SPI3_BUFF_BYTES_NUM -2)
	{
		return;
	}
	
	SPI3_NSS_ENABLE();
	SPI3_bufS[0] = MCP_READ;
	SPI3_bufS[1] = address;
	SPI3_Send_ByteS(SPI3_bufS, SPI3_bufR, num+2);
	for(len = 0; len<num;len++)
	{
		*pdat = SPI3_bufR[len+2];
		pdat++;
	}

	SPI3_NSS_DISABLE();
}


/**************************************************************
 *函数名称:mcp2515_read_can_id          
 *函数功能:读取MCP2515 CAN ID号
 *入口参数:mcp_addr: MCP2515地址
 *		   ext:1为扩展帧;0为标准帧
 *		   can_id:要读出的CAN数据ID号地址
 *出口参数:无 
**************************************************************/
void mcp2515_read_can_id(uint8_t mcp_addr, uint8_t* flag, uint32_t* can_id)
{
	uint8_t	tbufdata[4] = {0};
	*flag = 0;
	*can_id = 0;
	mcp2515_ReadRegisterS( mcp_addr, tbufdata, 4);
	*can_id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);
	if ((tbufdata[MCP_SIDL] & MCP_TXB_EXIDE_M) ==  MCP_TXB_EXIDE_M ) 
	{
		// extended id
		*can_id = (*can_id<<2) + (tbufdata[MCP_SIDL] & 0x03);
		*can_id <<= 16;
		*can_id = *can_id +(tbufdata[MCP_EID8]<<8) + tbufdata[MCP_EID0];
		*flag |= MCP_IDE_MASK;
	}
}


/**************************************************************
 *函数名称:mcp2515_ReadCanMsg          
 *函数功能:读取CAN数据包
 *入口参数:buffer_sidh_addr:接收缓冲区地址
 *		   msg:接收数据缓冲区地址
 *出口参数:无 
**************************************************************/
// Buffer can be MCP_RXBUF_0 or MCP_RXBUF_1

static void mcp2515_ReadCanMsg(uint8_t buffer_sidh_addr, CanMessage* msg)
{
	uint8_t mcp_addr, ctrl,dcl_temp;
	mcp_addr = buffer_sidh_addr;
	mcp2515_read_can_id(mcp_addr, &(msg->can3_flag), &(msg->identifier));
	ctrl = mcp2515_ReadRegister(mcp_addr-1 );
	msg->can3_flag &= MCP_IDE_MASK;
	dcl_temp = mcp2515_ReadRegister( mcp_addr+4 );
	dcl_temp &= MCP_DLC_MASK;  //data length
	msg->can3_flag += dcl_temp;
	
	if (/*(*dlc & RTR_MASK) || */(ctrl & 0x08)) 
	{
		msg->can3_flag |= MCP_RTR_MASK;
	}
	mcp2515_ReadRegisterS(mcp_addr+5, &(msg->data[0]), dcl_temp);
		
		
		
		
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	int i = 0;
	int	MaxTimes = 3;
	UINT8	intf = 0;
	UINT8	eflag = 0;
	UINT8	clear_intf = 0;
	CanMessage msg;
	int idx = 0;
	while(1)
	{
		mcp2515_read_2regs(MCP_CANINTF, &intf, &eflag);
		intf &=  MCP_USED_INT;
		if (intf & MCP_STAT_RX0IF)								// Msg in Buffer 0
		{
			mcp2515_ReadCanMsg(MCP_RXBUF_0, &msg);
			clear_intf |=  MCP_STAT_RX0IF;

			//环网协议
			if(((msg.identifier >> 22) & 0x0F) == 0x0C)  
			{
				for (idx = 0; idx < CAN_3_MAX_BUFF; idx++)			//20个缓存，考虑应用环境也别整大了
				{
					if (CanRxMsgBuff_3_Ring[idx].ExtId == 0)				//说明没被存储
					{
						pRecvCAN3_Idx_Ring = idx;
						break;
					}
				}
				if (pRecvCAN3_Idx_Ring >= CAN_3_MAX_BUFF)
				{
					pRecvCAN3_Idx_Ring = 0;
				}
				CanRxMsgBuff_3_Ring[pRecvCAN3_Idx_Ring].ExtId = msg.identifier;
				CanRxMsgBuff_3_Ring[pRecvCAN3_Idx_Ring].DLC  = msg.can3_flag &0x0f;
				for(i = 0;i < 8;i++)
				{
					CanRxMsgBuff_3_Ring[pRecvCAN3_Idx_Ring].Data[i] = msg.data[i];
				}
			}

			//监控-模块  协议
			else
			{
				for (idx = 0; idx < CAN_3_MAX_BUFF; idx++)			//20个缓存，考虑应用环境也别整大了
				{
					if (CanRxMsgBuff_3[idx].ExtId == 0)				//说明没被存储
					{
						pRecvCAN3_Idx = idx;
						break;
					}
				}
				if (pRecvCAN3_Idx >= CAN_3_MAX_BUFF)
				{
					pRecvCAN3_Idx = 0;
				}
				CanRxMsgBuff_3[pRecvCAN3_Idx].ExtId = msg.identifier;
				CanRxMsgBuff_3[pRecvCAN3_Idx].DLC  = msg.can3_flag &0x0f;
				for(i = 0;i < 8;i++)
				{
					CanRxMsgBuff_3[pRecvCAN3_Idx].Data[i] = msg.data[i];
				}
			}			
		}

		if (intf & MCP_STAT_RX1IF)								// Msg in Buffer 1
		{
			mcp2515_ReadCanMsg(MCP_RXBUF_1, &msg);	
			clear_intf |=  MCP_STAT_RX1IF;

			//环网协议
			if(((msg.identifier >> 22) & 0x0F) == 0x0C)  
			{
				for (idx = 0; idx < CAN_3_MAX_BUFF; idx++)			//20个缓存，考虑应用环境也别整大了
				{
					if (CanRxMsgBuff_3_Ring[idx].ExtId == 0)				//说明没被存储
					{
						pRecvCAN3_Idx_Ring = idx;
						break;
					}
				}
				if (pRecvCAN3_Idx_Ring >= CAN_3_MAX_BUFF)
				{
					pRecvCAN3_Idx_Ring = 0;
				}
				CanRxMsgBuff_3_Ring[pRecvCAN3_Idx_Ring].ExtId = msg.identifier;
				CanRxMsgBuff_3_Ring[pRecvCAN3_Idx_Ring].DLC  = msg.can3_flag &0x0f;
				for(i = 0;i < 8;i++)
				{
					CanRxMsgBuff_3_Ring[pRecvCAN3_Idx_Ring].Data[i] = msg.data[i];
				}
			}

			//监控-模块  协议
			else
			{
				for (idx = 0; idx < CAN_3_MAX_BUFF; idx++)			//20个缓存，考虑应用环境也别整大了
				{
					if (CanRxMsgBuff_3[idx].ExtId == 0)				//说明没被存储
					{
						pRecvCAN3_Idx = idx;
						break;
					}
				}
				if (pRecvCAN3_Idx >= CAN_3_MAX_BUFF)
				{
					pRecvCAN3_Idx = 0;
				}
				CanRxMsgBuff_3[pRecvCAN3_Idx].ExtId = msg.identifier;
				CanRxMsgBuff_3[pRecvCAN3_Idx].DLC  = msg.can3_flag &0x0f;
				for(i = 0;i < 8;i++)
				{
					CanRxMsgBuff_3[pRecvCAN3_Idx].Data[i] = msg.data[i];
				}
			}		
		}

		if(intf & MCP_STAT_ERRIF)
		{
			mcp2515_BitModify(MCP_EFLG,(eflag & 0xC0), 0x00);		//请错误中断标识/**/
			clear_intf |= MCP_STAT_ERRIF;
		}

		mcp2515_BitModify(MCP_CANINTF, clear_intf, 0x00); 
	
		MaxTimes--;
		if((!intf)  || (MaxTimes<=0))								//没有需要处理的中断/**/
		{
			break;
		}
	}//END will
}

//uint8_t TEST_SPIRead(uint8_t num)
//{
//	//struct mcp251x_priv *priv = dev_get_drvdata(&spi->dev);
//	uint8_t val = 0;
//	uint8_t buf11[8];
//
//	//MCP2515_Reset();
//	//SPI3_NSS_ENABLE();
//
//	buf11[0] = MCP_READ;
//	buf11[1] = MCP_CANCTRL;
//	buf11[2] = 0x55;
//	SPI3_Send_Byte(buf11[0]);
//	SPI3_Send_Byte(buf11[1]);
//	uint8_t uIdx = 0;
//	// HAL_GPIO_WritePin(SPI_3_GPIO_PORT, GPIO_PIN_5, GPIO_PIN_RESET);
//	for (uIdx = 0; uIdx < num; uIdx++)
//	{
//		val = SPI3_Send_Byte(buf11[2]);
//	}
//	//SPI3_NSS_DISABLE();
//	return val;
//}
/*==============================================================================
*********************************** END ****************************************
==============================================================================*/

//struct file_operations spi2can_fops = 
//{ 
//	.owner	= THIS_MODULE, 
//	.open 	= spi2can_open, 
//	.write	= spi2can_write,
//	.read	= spi2can_read,
//	.ioctl	 = spi2can_ioctl,
//	.release 	= spi2can_release, 
//}; 


