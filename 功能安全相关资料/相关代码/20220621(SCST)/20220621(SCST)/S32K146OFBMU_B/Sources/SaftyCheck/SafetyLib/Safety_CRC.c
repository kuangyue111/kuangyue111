/*************************************************************************************
Checksum Math related variables and functions
**************************************************************************************/
#include "McuSelfCheck.h"
#include "Safety_CRC.h"

/************************************************************************************************
This CRC_DMA demo implement 4 methods to calculate 16bit CRC according to given POLY and INIT value.
1) slow calculation, bit by bit;
2) fast calculation, table based;
3) use CRC HW to calculation CRC value;
4) use DMA to move data into CRC module, sync and async;
************************************************************************************************/
/***********************************************************************************************
Customer may need generate checksum of the specified area using IAR compiler.
Step 1: define symbols in linker.
Options.. => Linker => Extra Options => Use command line options:
--place_holder __checksum1,2,.checksum,2
--place_holder __checksum2,4,.checksum,4
--place_holder __checksum3,4,.checksum,4
--define_symbol __checksum1_begin=0x0420
--define_symbol __checksum1_end=0x042F
--define_symbol __checksum2_begin=0x1000
--define_symbol __checksum2_end=0xFFFF
--define_symbol __checksum3_begin=0x10000
--define_symbol __checksum3_end=0x11002
--keep __checksum1
--keep __checksum2
--keep __checksum3

Step 2: use "ielftool" to generate checksum of specified area.
Options.. => Build Actions => Post-build command line:
ielftool --fill 0xFF;0x0420-0x042F --fill 0xFF;0x1000-0xFFFF --fill 0xFF;0x10000-0x11002 --checksum __checksum1:2,crc16,0xFFFF;0x0420-0x042F --checksum __checksum2:4,crc32,0xFFFFFFFF;0x1000-0xFFFF --checksum __checksum3:4,crc32,0xFFFFFFFF;0x10000-0x11002 "$TARGET_PATH$" "$TARGET_PATH$" --verbose

Step 3: define section .checksum to hold the checksum results in icf file.

Step 4: (optional)
use the crcgen.bat in the project folder to generate .s19 file with checksum if .s19 is needed in your project.
************************************************************************************************/

/***********************************************************************************************
Global Variables/Constants
************************************************************************************************/
uint32_t u32CrcDmaRslt = 0;
uint32_t bCrcInUse = 0;			// 1 - CRC is in use; 0 - CRC is in idle;

uint8_t CrcSelfTestStatus = 0;/*0:Õý³£   1£ºÒì³£*/

/***********************************************************************************************
crc calculation function, very basic calculation method, calculate one byte for each call.
************************************************************************************************/
uint32_t CRC_ByteUpdate (uint32_t crc, uint8_t data)
{
  uint32_t i;

  crc = crc ^ ((uint32_t)data << 8);
  for (i=0; i<8; i++)
  {
      if (crc & 0x8000)
          crc = (crc << 1) ^ MY_CRC16_POLY;
      else
          crc <<= 1;
  }

  crc &= 0x0000FFFF;
  return crc;
}

/***********************************************************************************************
  Function:     CRC_Crc16SoftSlow - calculate 16bit CRC with direct bit-by-bit method.
                Poly 0x1021(CCITT) is defined by MY_CRC16_POLY 
  pDataSrc      - Pointer to the source data;
  u32Size       - Number of bytes of the source data;
  u16InitData   - Init value for this CRC calculation;
  return value  - Calculated CRC value;
************************************************************************************************/
uint16_t CRC_Crc16SoftSlow(uint8_t *pDataSrc, uint32_t u32Size, uint16_t u16InitData)
{
  uint32_t i,j;
  uint32_t rslt;
  uint8_t *p1 = pDataSrc;
  rslt = u16InitData<<0;
  for(j=0; j< u32Size; j++)
  {
      rslt = rslt ^ ((*p1)<<8);
      for(i=0;i<8;i++)
      {
        if(rslt & 0x8000)
          rslt = (rslt << 1) ^ MY_CRC16_POLY;
        else
          rslt <<= 1;
      }
      p1++;
  }
  return rslt;
}

/***********************************************************************************************
  Function:     CRC_Crc16Hw
                Calculate CRC value using CRC hardware module on S32K,
                Using POLY=0x1021 defined by MY_CRC16_POLY, input data format is word.
                Do crc with 16bit halfword in each iteration.
  pDataSrc      - Pointer to the source data;
  u32Size       - Number of bytes of the source data;
  u16InitData   - Init value for this CRC calculation;
  pCrcRslt      - Pointer to the address where crc result is saved;
  return value  - CRC_ERROR_STATUS_NO_ERROR if CRC calculation goes OK;
                  CRC_ERROR_STATUS_HW_BUSY if CRC module is occupied by other tasks.
************************************************************************************************/
uint16_t CRC_Crc16Hw(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg)
{
	CRC_DRV_Init(INST_CRC1, pCrcUsrCfg);
	CRC_DRV_WriteData(INST_CRC1, pDataSrc, u32Size);
	return (CRC_DRV_GetCrcResult(INST_CRC1));
}

uint32_t CRC_Crc16HwDma(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg, uint32_t u32DmaChNo)
{
	u32CrcDmaRslt = 0;
	CRC_DRV_Init(INST_CRC1, pCrcUsrCfg);
	//EDMA_DRV_ClearTCD(u32DmaChNo);
	EDMA_DRV_ChannelInit(&dmaController1Chn0_State, edmaChnConfigArray[u32DmaChNo]);
	EDMA_DRV_SetSrcAddr(u32DmaChNo, (uint32_t)pDataSrc);
	EDMA_DRV_SetSrcOffset(u32DmaChNo, 1);
	EDMA_DRV_SetSrcReadChunkSize(u32DmaChNo, 0);			//source read size 8bit
	EDMA_DRV_SetDestAddr(u32DmaChNo, (uint32_t)((uint8_t *)&(CRC->DATAu.DATA)));
	EDMA_DRV_SetDestOffset(u32DmaChNo, 0);
	EDMA_DRV_SetDestWriteChunkSize(u32DmaChNo, 0);
	EDMA_DRV_SetMinorLoopBlockSize(u32DmaChNo, 1);		//minor loop size 1byte (8bit)
	EDMA_DRV_SetMajorLoopIterationCount(u32DmaChNo, u32Size);
	EDMA_DRV_DisableRequestsOnTransferComplete(u32DmaChNo, true);
	EDMA_DRV_SetSrcLastAddrAdjustment(u32DmaChNo, -u32Size);
	EDMA_DRV_SetDestLastAddrAdjustment(u32DmaChNo, 0);
	//EDMA_DRV_ClearIntStatus(u32DmaChNo);
	EDMA_DRV_ConfigureInterrupt(u32DmaChNo, EDMA_CHN_MAJOR_LOOP_INT, true);
	EDMA_DRV_StartChannel(u32DmaChNo);

	return (CRC_ERROR_STATUS_NO_ERROR);
}

/*
void DMA1_IRQHandler(void)	//it's already included in edma_irq.c
{
	EDMA_DRV_IRQHandler(EDMA_CHN1_NUMBER);
}
*/
/********************************************************************************************
 * This callback function will be called in DMA channel 1 IRQ handler.
 ********************************************************************************************/
void DMA_Ch1Callback(void *parameter, edma_chn_status_t status)
{
	u32CrcDmaRslt = CRC_DRV_GetCrcResult(INST_CRC1);
}

/***********************************************************************************************
  Function:     Crc32CalcSlow, calculate 32bit CRC with direct bit-by-bit method.
                Poly 0x04C11DB7 is defined by MY_CRC32_POLY 
  pDataSrc      - Pointer to the source data;
  u32Size       - Number of bytes of the source data;
  u32InitData   - Init value for this CRC calculation;
  return value  - Calculated CRC value;
************************************************************************************************/
uint32_t CRC_Crc32SoftSlow(uint8_t *pDataSrc, uint32_t u32Size, uint32_t u32InitData)
{
  uint32_t i,j;
  uint32_t rslt;
  uint8_t *p1 = pDataSrc;
  rslt = u32InitData<<0;
  for(j=0; j< u32Size; j++)
  {
      rslt = rslt ^ ((*p1)<<24);
      for(i=0;i<8;i++)
      {
        if(rslt & 0x80000000)
          rslt = (rslt << 1) ^ MY_CRC32_POLY;
        else
          rslt <<= 1;
      }
      p1++;
  }
  return rslt;
}


uint32_t CRC_Crc32Hw(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg)
{
	CRC_DRV_Init(INST_CRC1, pCrcUsrCfg);
	CRC_DRV_WriteData(INST_CRC1, pDataSrc, u32Size);
	return (CRC_DRV_GetCrcResult(INST_CRC1));
}

/***********************************************************************************************
  Function:     CRC_Crc32HwDma
                Calculate CRC value using CRC hardware module with DMA on S32K,
                Using POLY=0x04C11DB7 defined by MY_CRC32_POLY, input data format is word.
                Do crc with 8bit in each iteration.
  pDataSrc      - Pointer to the source data;
  u32Size       - Number of bytes of the source data;
  pCrcUsrCfg    - pointer to the structure for CRC configuration generated by PE CRC component (including poly and seed value);
  u32DmaChNo    - DMA channel used for CRC calculation;
  return value  - CRC_ERROR_STATUS_NO_ERROR if CRC calculation goes OK;
                  CRC_ERROR_STATUS_HW_BUSY if CRC module is occupied by other tasks.
************************************************************************************************/
uint32_t CRC_Crc32HwDma(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg, uint32_t u32DmaChNo)
{
	u32CrcDmaRslt = 0;
	CRC_DRV_Init(INST_CRC1, pCrcUsrCfg);
	//EDMA_DRV_ClearTCD(u32DmaChNo);
	EDMA_DRV_ChannelInit(&dmaController1Chn0_State, edmaChnConfigArray[u32DmaChNo]);
	EDMA_DRV_SetSrcAddr(u32DmaChNo, (uint32_t)pDataSrc);
	EDMA_DRV_SetSrcOffset(u32DmaChNo, 1);
	EDMA_DRV_SetSrcReadChunkSize(u32DmaChNo, 0);			//source read size 8bit
	EDMA_DRV_SetDestAddr(u32DmaChNo, (uint32_t)((uint8_t *)&(CRC->DATAu.DATA)));
	EDMA_DRV_SetDestOffset(u32DmaChNo, 0);
	EDMA_DRV_SetDestWriteChunkSize(u32DmaChNo, 0);
	EDMA_DRV_SetMinorLoopBlockSize(u32DmaChNo, 1);		//minor loop size 1byte (8bit)
	EDMA_DRV_SetMajorLoopIterationCount(u32DmaChNo, u32Size);
	EDMA_DRV_DisableRequestsOnTransferComplete(u32DmaChNo, true);
	EDMA_DRV_SetSrcLastAddrAdjustment(u32DmaChNo, -u32Size);
	EDMA_DRV_SetDestLastAddrAdjustment(u32DmaChNo, 0);
	//EDMA_DRV_ClearIntStatus(u32DmaChNo);
	EDMA_DRV_ConfigureInterrupt(u32DmaChNo, EDMA_CHN_MAJOR_LOOP_INT, true);
	EDMA_DRV_StartChannel(u32DmaChNo);

	return (CRC_ERROR_STATUS_NO_ERROR);
}

/********************************************************************************
 * Compare the CRC result by software and hardware to validate CRC module operation
 ********************************************************************************/
void CRC_SelfTest(void)
{
	uint32_t crcSwRslt;
	uint32_t crcHwRslt;
	uint32_t dataSrc[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	if(bCrcInUse)		//check whether CRC is in use, to avoid conflicts.
		return;
	else
		bCrcInUse = 1;	//lock CRC for safe boot function
	crcSwRslt = CRC_Crc32SoftSlow( (uint8_t *)dataSrc, 8*4, 0xFFFFFFFF);
	crcHwRslt = CRC_Crc32Hw( (uint8_t *)dataSrc, 8*4, &crc1_InitConfig2);
	if(crcSwRslt == crcHwRslt)
//		selfTestStatus |= SELFTEST_PASS_CRC;
		CrcSelfTestStatus = 0;
	else
//		selfTestStatus &= ~SELFTEST_PASS_CRC;
		CrcSelfTestStatus = 1;
	bCrcInUse = 0;
}

uint8_t GetCrcSelfTestStatus(void)
{
	return(CrcSelfTestStatus);
}
