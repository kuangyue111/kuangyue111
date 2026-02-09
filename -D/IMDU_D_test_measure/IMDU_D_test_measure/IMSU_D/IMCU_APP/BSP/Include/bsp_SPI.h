
#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include     "base_def.h"
#include "stm32f4xx_hal.h"
#include "bsp_can.h"
// ================* Include Files ************************
//#include "includes.h"

// ================* Definitions and Marcos ***************
typedef enum spi_status_e {
	SPI_STATUS_FAIL,  
	SPI_STATUS_OK,    
	SPI_STATUS_INIT,  
	SPI_STATUS_WAIT,
	SPI_NUM_STATUS
}spi_status_t;

typedef enum flash_status_e {
	FLASH_STATUS_ACCESS_FAIL,      
	FLASH_STATUS_ACCESS_OK,   
	FLASH_STATUS_INVALID_ADD,
	FLASH_STATUS_WAIT,
	FLASH_NUM_STATUS,   
}flash_status_t;

#define DUMMY_BYTE                      (0xAA)
#define SF_M25P16_IDENTIFICATION        (uint32_t) (0x00C22015)
#define SF_M25P32_IDENTIFICATION        (uint32_t) (0x00C22016)

#define SF_M25P16                       (0)
#define SF_M25P32                       (1)

#define SF_RD_DATA_HIGHSPEED            (0x0b)
#define SF_RELASE_DEEP_PWR_DOWN         (0xAB)
#define SF_DEEP_PWR_DOWN                (0xB9)

#define SF_M25PXX_PAGE_SIZE             (256)
#define SF_M25PXX_PAGES_IN_SECT         (16)//(256)
#define SF_M25PXX_SECTOR_SIZE  (uint32_t)    (SF_M25PXX_PAGES_IN_SECT*SF_M25PXX_PAGE_SIZE)   // 0xFFFF
#define SF_M25PXX_NUM_SECTORS_IN_BLOCK           (16)  

#define SF_M25P16_NUM_SECTORS           (32)    
#define SF_M25P16_SIZE         (uint32_t)    (0x200000)                              // 32*256*256 = 2097152 Bytes = 2MBytes
#define SF_M25P32_NUM_SECTORS           (64)    
#define SF_M25P32_SIZE         (uint32_t)    (0x400000)                              // 64*256*256 = 4194304 Bytes = 4MBytes

#define SF_M25P16_BLOCK_SIZE   (uint32_t)    (65536)                              // 32*256*256 = 2097152 Bytes = 2MBytes

#define SF_M25PXX_WIP_FLAG              (0x01)                                  // Write In Progress (WIP) flag
#define SF_M25PXX_WEL_FLAG              (0x02)                                  // Write Enable Latch Bit
#define SF_M25PXX_SRWD_FLAG             (0x80)                                  // Status Rgister Write Protect
#define SF_M25PXX_BP0_FLAG              (0x04)                                  // Block Protect Selcetion Bit0
#define SF_M25PXX_BP1_FLAG              (0x08)                                  // Block Protect Selcetion Bit1
#define SF_M25PXX_BP2_FLAG              (0x10)                                  // Block Protect Selcetion Bit2
#define SF_M25PXX_BP3_FLAG              (0x20)                                  // Block Protect Selcetion Bit3
//  HW Specific Defintions
#define SPI_PERIPHERAL            (SPI2)
#define SPI_GPIO_PORT             (GPIOB)
#define SPI_FORCE_RESET()         __SPI2_FORCE_RESET()
#define SPI_RELEASE_RESET()       __SPI2_RELEASE_RESET()
#define SPI_CLK_ENABLE()          __SPI2_CLK_ENABLE()
#define SPI_CLK_DISABLE()         __SPI2_CLK_DISABLE()
#define SPI_GPIO_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define SPI_GPIO_CLK_DISABLE()    __GPIOB_CLK_DISABLE()
#define SPI_CLK_PIN               (GPIO_PIN_10)
#define SPI_MOSI_PIN              (GPIO_PIN_14)
#define SPI_MISO_PIN              (GPIO_PIN_15)
#define SPI_CS_PIN                (GPIO_PIN_2)
#define SPI_AF                    (GPIO_AF5_SPI2)
#define SPI3_AF                    (GPIO_AF6_SPI3)

#define SPI_3_GPIO_PORT             (GPIOB)
#define SPI_3__CLK_PIN               (GPIO_PIN_3)
#define SPI_3_MOSI_PIN              (GPIO_PIN_5)
#define SPI_3_MISO_PIN              (GPIO_PIN_4)
#define SPI_3_CS_PIN                (GPIO_PIN_15)
#define SPI_3_NSS_PORT			(GPIOA)

#define CANSENDTIMEOUT	3		
#define CANSENDTIMEOUT_2	100	

#define SPI_IRQn                  (SPI2_IRQn)
#define SPI_IRQHandler            (SPI2_IRQHandler)
#define SPI_NSS_PORT			(GPIOE)
#define FLASH_ENABLE()                  HAL_GPIO_WritePin(SPI_NSS_PORT, SPI_CS_PIN, GPIO_PIN_RESET)
#define FLASH_DISABLE()                 HAL_GPIO_WritePin(SPI_NSS_PORT, SPI_CS_PIN, GPIO_PIN_SET)

#define FLASH_WP_ENABLE			GPIO_PIN_RESET
#define FLASH_WP_DISENABLE		GPIO_PIN_SET

//#define SPI3_NSS_ENABLE()                  {HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);OSTimeDly(2);}
//#define SPI3_NSS_DISABLE()                 {OSTimeDly(2);HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);OSTimeDly(2);}
#define SPI3_NSS_ENABLE() do {\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_RESET);\
	} while (0);

#define SPI3_NSS_DISABLE()do {\
	Delay(10);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	HAL_GPIO_WritePin(SPI_3_NSS_PORT, GPIO_PIN_15, GPIO_PIN_SET);\
	} while (0);

#define SPI3_INT_ENABLE() do{\
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);\
}while(0);

#define SPI3_INT_DISABLE() do{\
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);\
}while(0);


#define FLASH_FREQ                      SPI_BAUDRATEPRESCALER_8                // SYSCLK 168MHZ/8 = 21 MHz

#define FLASH_WIP_TIMEOUT               (40000)

#define MEM_FLH_OTA_START_ADD                           (u32)   (0x00000000)           // FLASH Start memory address for OTA
#define MEM_FLH_OTA_END_ADD                             (u32)   (0x000FFFFF)           // FLASH End memory address for OTA
#define MEM_FLH_SERVER_DATA_STORAGE_START_ADD           (u32)   (0x001000)           // FLASH Start memory address for ESA
#define MEM_FLH_SERVER_DATA_STORAGE_END_ADD             (u32)   (MEM_FLH_SERVER_DATA_STORAGE_START_ADD + 0x3FFFF)           // FLASH End memory address for ESA
#define MEM_FLH_SERVER_DATA_STORAGE_OVERFLOW_ADD_LIMIT      (u32)   ((MEM_FLH_SERVER_DATA_STORAGE_END_ADD - MODEM_DATA_PACKET_SIZE))           // EEPROM Overflow memory address limit for MODEM Data Packet


/*********************************************************************************************************************
											MCP2515 芯片定义
**********************************************************************************************************************/
#define MCP_SIDH			0
#define MCP_SIDL			1
#define MCP_EID8			2
#define MCP_EID0			3

#define MCP_TXB_EXIDE_M     0x08    /* In TXBnSIDL */
#define MCP_DLC_MASK        0x0F    /* 4 LSBits */
#define MCP_RTR_MASK        0x40    /* (1<<6) Bit 6 */
#define MCP_IDE_MASK        0x80    /* (1<<6) Bit 6 */

#define MCP_RXB_RX_ANY      0x60
#define MCP_RXB_RX_EXT      0x40
#define MCP_RXB_RX_STD      0x20
#define MCP_RXB_RX_STDEXT   0x00
#define MCP_RXB_RX_MASK     0x60  //RXM1 RXM0
#define MCP_RXB_BUKT_MASK   (1<<2)
/*
** Bits in the TXBnCTRL registers.
*/
#define MCP_TXB_TXBUFE_M    0x80
#define MCP_TXB_ABTF_M      0x40
#define MCP_TXB_MLOA_M      0x20
#define MCP_TXB_TXERR_M     0x10
#define MCP_TXB_TXREQ_M     0x08
#define MCP_TXB_TXIE_M      0x04
#define MCP_TXB_TXP10_M     0x03

#define MCP_TXB_RTR_M       0x40		// In TXBnDLC
#define MCP_RXB_IDE_M       0x08		// In RXBnSIDL
#define MCP_RXB_RTR_M       0x40		// In RXBnDLC

#define MCP_STAT_RXIF_MASK   (0x03)
#define MCP_STAT_MERRF  (1<<7)
#define MCP_STAT_WAKIF  (1<<6)
#define MCP_STAT_ERRIF  (1<<5)
#define MCP_STAT_TX2IF  (1<<4)
#define MCP_STAT_TX1IF  (1<<3)
#define MCP_STAT_TX0IF  (1<<2)
#define MCP_STAT_RX1IF  (1<<1)
#define MCP_STAT_RX0IF  (1<<0)
#define MCP_STAT_TX0IF_MASK  (1<<3)

#define CANINTF_RX (MCP_STAT_RX0IF | MCP_STAT_RX1IF )
#define CANINTF_TX (MCP_STAT_TX0IF | MCP_STAT_TX1IF | MCP_STAT_TX2IF)
#define CANINTF_ERR (MCP_STAT_ERRIF | MCP_STAT_MERRF)

#define MCP_EFLG_RX1OVR (1<<7)
#define MCP_EFLG_RX0OVR (1<<6)
#define MCP_EFLG_TXBO   (1<<5)
#define MCP_EFLG_TXEP   (1<<4)
#define MCP_EFLG_RXEP   (1<<3)
#define MCP_EFLG_TXWAR  (1<<2)
#define MCP_EFLG_RXWAR  (1<<1)
#define MCP_EFLG_EWARN  (1<<0)
#define MCP_EFLG_ERRORMASK  (0xF8)		/* 5 MS-Bits */
// End mt

// Define MCP2515 register addresses
#define MCP_RXF0SIDH	0x00
#define MCP_RXF0SIDL	0x01
#define MCP_RXF0EID8	0x02
#define MCP_RXF0EID0	0x03
#define MCP_RXF1SIDH	0x04
#define MCP_RXF1SIDL	0x05
#define MCP_RXF1EID8	0x06
#define MCP_RXF1EID0	0x07
#define MCP_RXF2SIDH	0x08
#define MCP_RXF2SIDL	0x09
#define MCP_RXF2EID8	0x0A
#define MCP_RXF2EID0	0x0B
#define MCP_TXRTSCTRL	0x0D

#define MCP_CANSTAT		0x0E
#define MCP_CANCTRL		0x0F
#define MCP_RXF3SIDH	0x10
#define MCP_RXF3SIDL	0x11
#define MCP_RXF3EID8	0x12
#define MCP_RXF3EID0	0x13
#define MCP_RXF4SIDH	0x14
#define MCP_RXF4SIDL	0x15
#define MCP_RXF4EID8	0x16
#define MCP_RXF4EID0	0x17
#define MCP_RXF5SIDH	0x18
#define MCP_RXF5SIDL	0x19
#define MCP_RXF5EID8	0x1A
#define MCP_RXF5EID0	0x1B
#define MCP_TEC			0x1C
#define MCP_REC			0x1D
#define MCP_RXM0SIDH	0x20
#define MCP_RXM0SIDL	0x21
#define MCP_RXM0EID8	0x22
#define MCP_RXM0EID0	0x23
#define MCP_RXM1SIDH	0x24
#define MCP_RXM1SIDL	0x25
#define MCP_RXM1EID8	0x26
#define MCP_RXM1EID0	0x27
#define MCP_CNF3		0x28
#define MCP_CNF2		0x29
#define MCP_CNF1		0x2A
#define MCP_CANINTE		0x2B
#define MCP_CANINTF		0x2C
#define MCP_EFLG		0x2D
#define MCP_TXB0CTRL	0x30
#define MCP_TXB1CTRL	0x40
#define MCP_TXB2CTRL	0x50
#define MCP_RXB0CTRL	0x60
#define MCP_RXB0SIDH	0x61
#define MCP_RXB1CTRL	0x70
#define MCP_RXB1SIDH	0x71

#define MCP_MERRE_INT	0x80
#define MCP_ERRIE_INT	0x20
#define MCP_TX_INT		0x1C			// Enable all transmit interrupts
#define MCP_TX01_INT	0x04			// Enable TXB0 and TXB1 interrupts
#define MCP_RX_INT		0x03			// Enable receive interrupts
#define MCP_NO_INT		0x00			// Disable all interrupts
#define MCP_USED_INT	(MCP_MERRE_INT | MCP_ERRIE_INT | MCP_TX01_INT | MCP_RX_INT)

#define MCP_MERRE_MASK	0x80
#define MCP_ERRIE_MASK	0x20
#define MCP_TX01_MASK	0x04
#define MCP_TX_MASK		0x54
#define MCP_RX_MASK		0x03
#define MCP_USED_INT_MASK	(MCP_MERRE_MASK | MCP_ERRIE_MASK | MCP_TX01_MASK | MCP_RX_MASK)

/*************************************************************/
/*MCP2515寄存器*/
/*************************************************************/
#define MCP_RXBUF_0 (MCP_RXB0SIDH)  //MCP2515接收缓冲区0
#define MCP_RXBUF_1 (MCP_RXB1SIDH)  //MCP2515接收缓冲区1

// Define SPI Instruction Set
#define MCP_WRITE		0x02
#define MCP_READ		0x03
#define MCP_BITMOD		0x05
#define MCP_LOAD_TX0	0x40
#define MCP_LOAD_TX1	0x42
#define MCP_LOAD_TX2	0x44
#define MCP_RTS_TX0		0x81
#define MCP_RTS_TX1		0x82
#define MCP_RTS_TX2		0x84
#define MCP_RTS_ALL		0x87
#define MCP_READ_RX0	0x90
#define MCP_READ_RX1	0x94
#define MCP_READ_STATUS	0xA0
#define MCP_RX_STATUS	0xB0
#define MCP_RESET		0xC0


#define CAN_20KBPS   (1)				//CAN 波特率20kbps
#define CAN_100KBPS  (CAN_20KBPS+1)		//CAN 波特率100kbps
#define CAN_125KBPS  (CAN_20KBPS+2)		//CAN 波特率125kbps
#define CAN_250KBPS  (CAN_20KBPS+3)		//CAN 波特率125kbps

#define CAN_STDID (0)		//标准帧
#define CAN_EXTID (1)		//扩展帧

//#define CAN_ID_MASK			(0x1f078000 << 0)		//0xFFF00787	//屏蔽寄存器
//#define CAN_ID_FILTER		(0x06078000 << 0)		//0x06A00783	//滤波寄存器

#define CAN_ID_MASK			(0x0380E000 << 0)		//0xFFF00787	//屏蔽寄存器
#define CAN_ID_FILTER		(0x0280F000 << 0)		//0x06A00783	//滤波寄存器

// CANCTRL Register Values
#define MODE_NORMAL     0x00
#define MODE_SLEEP      0x20
#define MODE_LOOPBACK   0x40
#define MODE_LISTENONLY 0x60

//#define MODE_CONFIG     0x80
#define MODE_CONFIG     0x87		//REQOP2  CLKEN		   设置为配置模式，且CLK引脚使能
#define MODE_POWERUP	0xE0
#define MODE_STAT_MASK		0xE0

#define MCP2515_MODE_MASK	0xE7  //REQOP2	REQOP1	REQOP0	CLKEN	CLKPRE1	CLKPRE0

#define ABORT_TX        0x10
#define MODE_ONESHOT		0x08
#define CLKOUT_ENABLE		0x04
#define CLKOUT_DISABLE	0x00
#define CLKOUT_PS1			0x00
#define CLKOUT_PS2			0x01
#define CLKOUT_PS4			0x02
#define CLKOUT_PS8			0x03

// CNF1 Register Values
#define SJW1            0x00
#define SJW2            0x40
#define SJW3            0x80
#define SJW4            0xC0


// CNF2 Register Values
#define BTLMODE			0x90
#define SAMPLE_1X       0x00
#define SAMPLE_3X       0x40


// CNF3 Register Values
#define SOF_ENABLE		0x80
#define SOF_DISABLE		0x00
#define WAKFIL_ENABLE	0x40
#define WAKFIL_DISABLE	0x00


// CANINTF Register Bits
#define MCP_RX0IF		0x01
#define MCP_RX1IF		0x02
#define MCP_TX0IF		0x04
#define MCP_TX1IF		0x08
#define MCP_TX2IF		0x10
#define MCP_ERRIF		0x20
#define MCP_WAKIF		0x40
#define MCP_MERRF		0x80

/*-----------------------*/
/* 125 kBPS @ 16MHZ F_OSC */
/*-----------------------*/

/* CNF1 */
// SJW as defined in mcp_defs.h
// here: SJW = 1*TQ
// Prescaler = (BRP+1)*2
// here Prescaler = 8 -> BRP=3

/* CNF2 */
// BLT-Mode defined in CNF3 (0 or BTLMODE from mcp_defs.h)
// 3 samples (SAMPLE_3X) or 1 sample (SAMPLE_1X)
// (Phase Segment 1) PHSEG1 = PS1 - 1
// here: PS1 calculated as 2 
// (Propagation Delay) PRSEG = PRSEQTQ-1
// here PRSEQTQ = 2
/* CNF3 */
// (Phase Segment 2) PHSEG2 = PS2 - 1
// here: PS2 calculated as 3
#define MCP_16MHz_125KBPS_CFG1		0x03
#define MCP_16MHz_125KBPS_CFG2		0x9E
#define MCP_16MHz_125KBPS_CFG3		0x03 

#define MCP_16MHz_250KBPS_CFG1		0x03
#define MCP_16MHz_250KBPS_CFG2		0x8A
#define MCP_16MHz_250KBPS_CFG3		0x01 

/*-----------------------*/
/* 100 kBPS @ 16MHZ F_OSC  */
/*-----------------------*/
/* CNF1 */
// SJW as defined in mcp_defs.h
// here: SJW = 1*TQ
// Prescaler = (BRP+1)*2
// here Prescaler = 10 -> BRP=4  (20TQ)
/* CNF2 */
// BLT-Mode defined in CNF3 (0 or BTLMODE from mcp_defs.h)
// 3 samples (SAMPLE_1X) or 1 sample (SAMPLE_1X)
// (Phase Segment 1) PHSEG1 = PS1 - 1
// here: PS1 calculated as 8
// (Propagation Delay) PRSEG = PRSEQTQ-1
// here PRSEQTQ = 3
/* CNF3 */
// (Phase Segment 2) PHSEG2 = PS2 - 1
// here: PS2 calculated as 8
#define MCP_16MHz_100KBPS_CFG1		0x04 	
#define MCP_16MHz_100KBPS_CFG2		0x9E 
#define MCP_16MHz_100KBPS_CFG3		0x03

/*-----------------------*/
/* 20 kBPS @ 16MHZ F_OSC  */
/*-----------------------*/
/* CNF1 */
// SJW as defined in mcp_defs.h
// here: SJW = 1*TQ
// Prescaler = (BRP+1)*2
// here Prescaler = 10 -> BRP=4  (20TQ)
/* CNF2 */
// BLT-Mode defined in CNF3 (0 or BTLMODE from mcp_defs.h)
// 3 samples (SAMPLE_1X) or 1 sample (SAMPLE_1X)
// (Phase Segment 1) PHSEG1 = PS1 - 1
// here: PS1 calculated as 8
// (Propagation Delay) PRSEG = PRSEQTQ-1
// here PRSEQTQ = 3
/* CNF3 */
// (Phase Segment 2) PHSEG2 = PS2 - 1
// here: PS2 calculated as 8
#define MCP_16MHz_20KBPS_CFG1		0x18 	
#define MCP_16MHz_20KBPS_CFG2		0xA4 
#define MCP_16MHz_20KBPS_CFG3		0x04


#define MCP2515_OK         (0)      //设置成功 
#define MCP2515_FAIL       (1)      //设置失败
#define MCP_ALLTXBUSY      (2)      //发送缓冲区忙

#define MCP_N_TXBUFFERS (1)         //发送缓冲区数量

#define CAN_MAX_CHAR_IN_MESSAGE				8				//CAN数据长度
#define CAN_FRAME_SIZE						13				//CanMessage 占用13字节,用户发送和接受的buffer的占13字节，需要在read和write中做转换ID和flag处理
#define CAN_FRAME_NUM						100				// CAN RX FRAM buffer 
#define CAN_FRAME_TX_NUM					20				// CAN TX FRAM buffer 


typedef struct sf_m25pxx_s 
{
	uint32_t identification;
	uint32_t size;
	uint32_t num_sectors;
}sf_m25pxx_t;


typedef struct _tCanMessage
{
	unsigned char can3_flag;	//IDE[bit7], RTR[bit6], dlc[bit3~bit0]
	unsigned int identifier; 
	unsigned char  data[CAN_MAX_CHAR_IN_MESSAGE];
}CanMessage;


// ================* Variables ***************************
extern struct sf_m25pxx_s sf_m25pxx_info;

// ================* Function Prototypes ******************
// Function Prototypes
void bsp_InitSpiFlash(void);
void SPI_Flash_DeInit(void);
uint8_t SPI_Flash_EraseSector(uint32_t SectorAddr);
uint8_t SPI_Flash_EraseBulk(void);
uint8_t SPI_Flash_PageProgram(uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes);
uint8_t SPI_Flash_ReadBytes(uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes);
uint8_t SPI_Flash_Write_Bytes(uint8_t *pBuf, uint32_t Addr, uint32_t NumBytes);
uint8_t SPI_Flash_ReadID(uint32_t *id);
uint8_t SPI_Flash_ReadSatusReg(uint8_t *val);
uint8_t SPI_Flash_WriteSatusReg(uint8_t u8Byte);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void SPI_IRQHandler(void);
uint8_t SPI_Flash_EraseBlock(uint32_t SectorAddr);
void mcp2515_ReadRegisterS(uint8_t address, uint8_t *pdat, uint8_t num);

USHORT ReadBuff(UCHAR *pData);
UCHAR WriteBuff(USHORT u16Number, UCHAR *pData);
void low_level_init(void);

/*********************************************************************************************
		大写 为APP调用接口，小写为私有的接口，直接调用可能使用环境不行！需要修改！
**********************************************************************************************/
void BSP_InitSPI3(void);
uint8_t SPI3_Send_Byte (uint8_t u8Byte);
uint8_t SPI3_Send_ByteS(uint8_t* pSData, uint8_t* pRData,uint16_t Num);

int MCP2515_Set_Recmask(int NO, uint32_t Rec_mask);
int MCP2515_Set_RecID(int NO, uint32_t Rec_ID);
void MCP2515_Reset(void);
uint8_t MCP2515_init(const uint8_t CanBandRate);
int MCP2515_Send(uint8_t* pSData, uint16_t Len);
int MCP2515_Send_2(canmsg *pMsg, UINT8 DataL);
int MCP2515_Read(CanRxMsgTypeDef* pRFrame, uint16_t pRFrameNum);
int MCP2515_Read_Ring(CanRxMsgTypeDef* pRFrame, uint16_t pRFrameNum);
void mcp2515_WriteCanMsg_2(UCHAR buffer_sidh_addr, canmsg* msg,UINT8 u8L);

/*************************************************************************************************************
	MCP2515 接收到CAN数据 会借助INT脚 进入中断，我们在中断中读数据
	由于读SPI操作是非原子操作，而发送又是在线程中操作SPI，所以存在互斥。
	急于我们整流模块采集应用场合，一问一答，所以发送数据前 把此中断禁掉，然后发送完之后再使能
**************************************************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);		
void mcp2515_StartTransmit(uint8_t buffer_sidh_addr);
uint8_t  mcp2515_RD_Status(void);
void mcp2515_WriteCanMsg(UINT8 buffer_sidh_addr, CanMessage* msg);
static void bsp_SPI3_Error(void);
static void mcp2515_read_2regs(uint8_t reg, uint8_t *v1, uint8_t *v2);

void mcp2515_WriteRegisterS(uint8_t address, uint8_t *pdat, uint8_t num);
void mcp2515_WriteRegister(uint8_t address, uint8_t val);
uint8_t mcp2515_ConfigRate(const uint8_t CanBandRate);
void mcp2515_write_can_mask(uint8_t mcp_addr, uint8_t ext, uint32_t mask );
void mcp2515_write_can_filter(uint8_t mcp_addr, uint8_t ext, uint32_t filter);
void mcp2515_InitCanBuffers(void);
uint8_t mcp2515_SetCanCtrlMode(const uint8_t newmode);
uint8_t mcp2515_ReadRegister(uint8_t reg);
void mcp2515_read_can_id(uint8_t mcp_addr, uint8_t* flag, uint32_t* can_id);
UCHAR mcp2515_getNextFreeTXBuf(UCHAR *txbuf_n);

void SPIFlashInit(void);
void SPIFlashRead(UCHAR *pBuff , UINT32 Address, UINT32 Num);
void SPIFlashWrite(UCHAR *pBuff , UINT32 Address, UINT32 Num);
void SPIFlashEraseBlock(UINT32 BlockAddress);
void SPIFlashEraseSector(UINT32 SectorAddress);
void SPIFlashEraseChip(void);
UINT8 FlashReadRecords(UINT16 u16OffsetNo, UINT16 u16SectNo, UINT8* pBuf, UINT16 u16Len);


#endif

//void WriteBuff(void);
