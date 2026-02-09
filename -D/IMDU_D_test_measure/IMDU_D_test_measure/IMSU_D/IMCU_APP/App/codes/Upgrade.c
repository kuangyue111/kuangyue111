
#include "Upgrade.h"

NOINIT __UPGRADE_INFO  gUpGradeData;


void InitUpgrade(void)
{
	gUpGradeData.u8UpGradeOpt =	 UPGRADE_OPT_IDL;
	gUpGradeData.u32FileSize = 0;
	gUpGradeData.u32TatolBlock = 0;
}

void FLASH_EXTableWriteBytes(UINT8 *pData,UINT32 u32Addr, UINT32 u32Len)
{
	UINT32 u32WriteAddr = 0;
	//MutexLock(gEvChargeInfo.hMetex_FLASH);
	u32WriteAddr = (EX_UPGRADE_START_BLOCK_NO * PAGE_NUM_PER_BLOCK * BYTES_NUM_PER_PAGE);
	u32WriteAddr = u32Addr + u32WriteAddr;
	SPIFlashWrite(pData, u32WriteAddr, u32Len);
	//MutexUnlock(gEvChargeInfo.hMetex_FLASH);
}

void FLASH_UpgradeWriteBytes(UINT8 *pData,UINT32 u32Addr, UINT32 u32Len)
{
	UINT32 u32WriteAddr = 0;
	//MutexLock(gEvChargeInfo.hMetex_FLASH);
	u32WriteAddr = (EX_UPGRADE_START_BLOCK_NO * PAGE_NUM_PER_BLOCK * BYTES_NUM_PER_PAGE) + EX_FLASH_INFO;
	u32WriteAddr = u32Addr + u32WriteAddr;
	SPIFlashWrite(pData, u32WriteAddr, u32Len);
	//MutexUnlock(gEvChargeInfo.hMetex_FLASH);
}

void UpGradeEraseExFlash(void)
{
	//½«FLASH (8Blocks) Ë¢Îª FF
	UINT8 u8BlockNo = 0;
	UINT32 Addres = 0;
	for (u8BlockNo = 0; u8BlockNo < EX_UPGRADE_BLOCK_MAX_NUM; u8BlockNo++)
	{
		Addres = ((u8BlockNo + EX_UPGRADE_START_BLOCK_NO) * PAGE_NUM_PER_BLOCK * BYTES_NUM_PER_PAGE) + 1;
		SPIFlashEraseBlock(Addres);
		OSTimeDly(10);
	}
}