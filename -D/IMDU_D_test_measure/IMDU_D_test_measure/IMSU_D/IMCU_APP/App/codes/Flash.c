
#include "main.h"
#include "EtherNet.h"
#include "can_ring.h"
 /***********************************************************************************************
	#define RECORD_BLOCK_MAX_NUM		3			3个扇区
	#define PAGE_NUM_PER_BLOCK			256			每个扇区 256页
	#define BYTES_NUM_PER_PAGE			256			每页 256字节
						______________
						|			  |
						|_____________|
	i16CurrPageIdx----->|			  |
						|_____________|
						|			  |
						|_____________|

	gFlashOptInfo.u32TotalRecordNum				总记录数
	gFlashOptInfo.u32CurrPageIdx				当前记录页的索引

	SaveRecord2Flash							这个信号由	API_CreateRecordLog 生产！
	gEvChargeInfo.u8SendCharge RecordFlag		只有收到2A 下发的交易号或是09远程停止充电指令才置位，
	所以离线卡充电无需处理。
	A/B 枪 存储再一个片区域内,然后记录的时候用一个字节标记A枪还是B枪的
************************************************************************************************/

#define	TABLE_STEP_BY_PAGE			(sizeof(gChargeLogTable) / BYTES_NUM_PER_PAGE)			/**/
#define	TABLE_BYTES_BY_END_PAGE		(sizeof(gChargeLogTable) % BYTES_NUM_PER_PAGE)		/**/
__FLASH_CTRL_INFO gFlashOptInfo = {0};
NOINIT __CHARGE_RECORD SaveRecord2Flash, stViewChargeRecordFlash;
NOINIT ChargeLogTable	gChargeLogTable;
NOINIT __CHARGE_RECORD SendRecord_1;
NOINIT __CHARGE_RECORD SendRecord_2;

NOINIT UINT8 DEBUG_Flash[300];
NOINIT UINT16 u16DEBUG_Flash_ADDR;
NOINIT UINT16 u16DEBUG_Flash_LEN;

void FlashCleanRecords(void);

void FLASH_ClrUnRptFlag(UINT16 u16PageIdx)
{
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	UINT32 Addres = 0;
	UINT8 u8Num[6];
	if (u16PageIdx > RECORD_BLOCK_MAX_NUM * PAGE_NUM_PER_BLOCK)
	{
		MutexUnlock(gEvChargeInfo.hMetex_FLASH);
		return;
	}
	Addres = ((u16PageIdx) * BYTES_NUM_PER_PAGE) + ADDR_UN_RPT_FLAG_PST;
	u8Num[0] = VAL_RPT_FLAG;
	
	SPIFlashWrite(u8Num, Addres, 1);			
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
}

UINT16 FLASH_GetUnRptNum(UINT16* pLastUnRptIDX)
{
	UINT16 u16UnRptNum = 0;
	UINT8 u8BlockNo = 0;
	UINT16 u16PageIdx = 0;
	UINT32 Addres = 0;
	UINT8 u8Num[6];
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	for (u8BlockNo = 0; u8BlockNo < RECORD_BLOCK_MAX_NUM; u8BlockNo++)
	{
		for (u16PageIdx = 0; u16PageIdx < PAGE_NUM_PER_BLOCK; u16PageIdx++)
		{
			Addres = ((u8BlockNo * PAGE_NUM_PER_BLOCK + u16PageIdx) * BYTES_NUM_PER_PAGE) + ADDR_TOTAL_NUM_PST;
			SPIFlashRead(u8Num, Addres, 6);
			if (u8Num[0] == 0xff
				&& u8Num[1] == 0xff
				&& u8Num[2] == 0xff
				&& u8Num[3] == 0xff
				&& u8Num[4] == 0xff
				&& u8Num[5] == 0xff)//全FF 说明这一页没有使用
			{
				//break;
			}
			else  if(u8Num[5] == PAGE_WriteOk_FLAG)
			{
				if (u8Num[4] == VAL_UN_RPT_FLAG)  //0x5A 已经上传。	FF未上传
				{
					*pLastUnRptIDX = (u8BlockNo * PAGE_NUM_PER_BLOCK + u16PageIdx);
					u16UnRptNum++;													
				}
				gEvChargeInfo.u32FLASHUnRptNum = u16UnRptNum;
			}
		}
	}
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
	return u16UnRptNum;
}


void FLASH_GetRecordsTable(ChargeLogTable* pTable)
{
	UINT8 u8BlockNo = 0;
	UINT16 u16PageIdx = 0;
	UINT32 Addres = 0;
	UINT8 u8Num[6];
	pTable->u16UnRptNum = 0;
	pTable->u32CurrPageIdx = 0;
	pTable->u32TotalNum = 0;
	pTable->u16LastPageIdx = 0;
	gEvChargeInfo.u32FLASHUnRptNum = 0;
	UINT8 u8FindPageFlag = FALSE;
	UINT32 uMaxSqcNo = 0;
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	for (u8BlockNo = 0; u8BlockNo < RECORD_BLOCK_MAX_NUM; u8BlockNo++)
	{
		for (u16PageIdx = 0; u16PageIdx < PAGE_NUM_PER_BLOCK; u16PageIdx++)
		{
			Addres = ((u8BlockNo * PAGE_NUM_PER_BLOCK + u16PageIdx) * BYTES_NUM_PER_PAGE) + ADDR_TOTAL_NUM_PST;
			SPIFlashRead(u8Num, Addres, 6);
			if (u8Num[0] == 0xff && u8Num[1] == 0xff && u8Num[2] == 0xff && u8Num[3] == 0xff && u8Num[4] == 0xff && u8Num[5] == 0xff)
			{
				if (u8FindPageFlag==FALSE)
				{
					pTable->u32CurrPageIdx = u8BlockNo * PAGE_NUM_PER_BLOCK + u16PageIdx;
					u8FindPageFlag = TRUE;
				}
			}
			else  if(u8Num[5] == PAGE_WriteOk_FLAG)  //数据有效
			{
				//pTable->u32CurrPageIdx = u8BlockNo * PAGE_NUM_PER_BLOCK + u16PageIdx;
				uMaxSqcNo = ((UINT32)u8Num[0]<<24) + ((UINT32)u8Num[1]<<16) + ((UINT32)u8Num[2]<<8) + (UINT32)u8Num[3];
				if (uMaxSqcNo >= pTable->u32TotalNum)
				{
					pTable->u32TotalNum = uMaxSqcNo;
					pTable->u16LastPageIdx = u8BlockNo * PAGE_NUM_PER_BLOCK + u16PageIdx;
				}

				if (u8Num[4] == VAL_UN_RPT_FLAG)
				{
					pTable->u16UnRptNum++;						//0x5A 已经上传 	0XFF未上传。
				}
				gEvChargeInfo.u32FLASHUnRptNum = pTable->u16UnRptNum;
			}
		}
	}
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
}

void FlashCleanRecords(void)
{
	UINT8 u8BlockNo = 0;
	UINT32 Addres = 0;
	gChargeLogTable.u16UnRptNum = 0;
	gChargeLogTable.u32CurrPageIdx = 0;
	gChargeLogTable.u32TotalNum = 0;
	gChargeLogTable.u16LastPageIdx = 0;
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	//将FLASH 刷为 FF
	for (u8BlockNo = 0; u8BlockNo < RECORD_BLOCK_MAX_NUM; u8BlockNo++)
	{
		Addres = (u8BlockNo * PAGE_NUM_PER_BLOCK * BYTES_NUM_PER_PAGE) + 1;
		SPIFlashEraseBlock(Addres);
		OSTimeDly(10);
	}
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
}

UINT16 FLASH_GetPageIdx(UINT32 uSqsNo)
{
	UINT16	PageIdx = 0;
	UINT32	Addres = 0;
	UINT8 u8Val[8];
	UINT32 u32TotalRecordNuM = 0;
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	for (PageIdx = 0; PageIdx < (RECORD_BLOCK_MAX_NUM * PAGE_NUM_PER_BLOCK); PageIdx++)
	{
		Addres = (PageIdx * BYTES_NUM_PER_PAGE) + ADDR_TOTAL_NUM_PST;
		SPIFlashRead(u8Val, Addres, 6);
		u32TotalRecordNuM = ((u8Val[0]<<24) + (u8Val[1]<<16) + (u8Val[2]<<8) + u8Val[3]);
		if ((u8Val[5] == PAGE_WriteOk_FLAG) && (u32TotalRecordNuM == uSqsNo))
		{
			MutexUnlock(gEvChargeInfo.hMetex_FLASH);
			return PageIdx;
		}
	}
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
	return PageIdx;
}

UINT8 FLASH_GetUnRptPageIdx(UINT16 *pPageIdx, UINT16 u16LastOpPageIdx)
{
	UINT16	PageIdx = 0;
	UINT32	Addres = 0;
	UINT8 u8Val[8];
	//UINT32 u32TotalNum, u32ViewRecordNo;
	UINT16 u16LastPageIdx;
	
	u16LastPageIdx = u16LastOpPageIdx;

	MutexLock(gEvChargeInfo.hMetex_FLASH);

	for (PageIdx = 0; PageIdx < (RECORD_BLOCK_MAX_NUM * PAGE_NUM_PER_BLOCK); PageIdx++)
	{
		Addres = (u16LastPageIdx * BYTES_NUM_PER_PAGE) + ADDR_TOTAL_NUM_PST;
		SPIFlashRead(u8Val, Addres, 6);
		//u32TotalRecordNuM = ((u8Val[0]<<24) + (u8Val[1]<<16) + (u8Val[2]<<8) + u8Val[3]);
		if (u8Val[5] == PAGE_WriteOk_FLAG)
		{
			/*u16ValidRecordNum++;

			if(u16ValidRecordNum >= u32TotalNum)
			{
				MutexUnlock(gEvChargeInfo.hMetex_FLASH);
				return FALSE;
			}*/

			if (u8Val[4] == VAL_UN_RPT_FLAG)
			{
				*pPageIdx = u16LastPageIdx;
				MutexUnlock(gEvChargeInfo.hMetex_FLASH);
				return TRUE;
			}			
		}		
		
		if(u16LastPageIdx > 0)
		{
			u16LastPageIdx--;
		}
		else
		{
			u16LastPageIdx = (PAGE_NUM_PER_BLOCK * RECORD_BLOCK_MAX_NUM - 1);
		}
	}
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
	return FALSE;
}

UINT8 FLASH_ReadRecords(UINT32 u32Addr, UINT8* pBuf, UINT16 u16Len)
{
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	if (u32Addr >= (RECORD_BLOCK_MAX_NUM * PAGE_NUM_PER_BLOCK * BYTES_NUM_PER_PAGE))
	{
		memset(pBuf, 0, u16Len);
		MutexUnlock(gEvChargeInfo.hMetex_FLASH);
		return FALSE;
	}
	SPIFlashRead(pBuf, u32Addr, u16Len);
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
	return TRUE;
}

//返回值，当前操作页
UINT16 FLASH_WriteRecords(ChargeLogTable *pTable, __CHARGE_RECORD* pStRecord)
{
	UINT16 u16OptPageIdx = 0;
	UINT32 Addres = 0;
	UINT32 u32Val = 0;
	UINT8 u8Num[6];
	UINT8 u8CheckFLASH[6];
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	//擦写 BLOCK
	if (0 == pTable->u32CurrPageIdx
		|| PAGE_NUM_PER_BLOCK == pTable->u32CurrPageIdx
		|| (PAGE_NUM_PER_BLOCK *(RECORD_BLOCK_MAX_NUM - 1)) == pTable->u32CurrPageIdx)																																//擦除该扇区！！
	{
		Addres = (pTable->u32CurrPageIdx * BYTES_NUM_PER_PAGE) + 1;																							//这个地址只要在这个扇区即可
		SPIFlashEraseBlock(Addres);
		Delay(500);
	}
	else
	{
		Addres = pTable->u32CurrPageIdx * BYTES_NUM_PER_PAGE;
		//防止 写一半断电情况,导致下一条记录 也无法存储，	只要有一个字节不是 0XFF  说明被动过
		SPIFlashRead(u8CheckFLASH, Addres, 6);
		if (u8CheckFLASH[0] != 0xff || u8CheckFLASH[1] != 0xff || u8CheckFLASH[2] != 0xff || u8CheckFLASH[3] != 0xff  || u8CheckFLASH[4] != 0xff || u8CheckFLASH[5] != 0xff)
		{
			//当前页面的成功标志位置要写上失败标志，以防下次继续判断错误
			u8Num[0] = PAGE_WriteFail_FLAG;
			Addres = ((pTable->u32CurrPageIdx) * BYTES_NUM_PER_PAGE) + PAGE_WriteOk_FLAG_PST;
			SPIFlashWrite(u8Num, Addres, 1);

			pTable->u32CurrPageIdx++;

			if (pTable->u32CurrPageIdx >= RECORD_BLOCK_MAX_NUM * PAGE_NUM_PER_BLOCK)
			{
				pTable->u32CurrPageIdx = 0;
			}

			if (0 == pTable->u32CurrPageIdx
				|| PAGE_NUM_PER_BLOCK == pTable->u32CurrPageIdx
				|| (PAGE_NUM_PER_BLOCK *(RECORD_BLOCK_MAX_NUM - 1)) == pTable->u32CurrPageIdx)														//擦除该扇区！！
			{
				Addres = (pTable->u32CurrPageIdx * BYTES_NUM_PER_PAGE) + 1;																			//这个地址只要在这个扇区即可
				SPIFlashEraseBlock(Addres);
			}
		}
	}

	UINT16 u16LEN11 = 0;
	u16OptPageIdx = pTable->u32CurrPageIdx;
	Addres = pTable->u32CurrPageIdx * BYTES_NUM_PER_PAGE;
	UINT8* pRecord = (UINT8*)&(pStRecord->u8GunType);
	u16LEN11 = (sizeof(__CHARGE_RECORD)/sizeof(UINT8));

	SPIFlashWrite(pRecord, Addres, u16LEN11);		//只要不超过250字节即可！
	
	//写 总记录数
	pTable->u32TotalNum++;
	u32Val = pTable->u32TotalNum;
	Addres = ((pTable->u32CurrPageIdx) * BYTES_NUM_PER_PAGE) + ADDR_TOTAL_NUM_PST;
	u8Num[0] = (((u32Val) >> 24)& 0xff);
	u8Num[1] = ((u32Val >> 16) & 0xff);
	u8Num[2] = ((u32Val >> 8) & 0xff);
	u8Num[3] = (u32Val & 0xff);
	SPIFlashWrite(u8Num, Addres, (4));

	//页成功标志
	u8Num[0] = PAGE_WriteOk_FLAG;
	Addres = ((pTable->u32CurrPageIdx) * BYTES_NUM_PER_PAGE) + PAGE_WriteOk_FLAG_PST;
	SPIFlashWrite(u8Num, Addres, 1);

	pTable->u16LastPageIdx = pTable->u32CurrPageIdx;

		
	pTable->u32CurrPageIdx++;
	if (pTable->u32CurrPageIdx >= RECORD_BLOCK_MAX_NUM * PAGE_NUM_PER_BLOCK)
	{
		pTable->u32CurrPageIdx = 0;		
	}

	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
	
	return u16OptPageIdx;
}

void DebugFlash(void);
void DebugFlash(void)
{
	OSTimeDly(100);
	if (u16DEBUG_Flash_LEN <= 0)
	{
		return;
	}
	MutexLock(gEvChargeInfo.hMetex_FLASH);
	SPIFlashRead(DEBUG_Flash, u16DEBUG_Flash_ADDR, u16DEBUG_Flash_LEN);
	MutexUnlock(gEvChargeInfo.hMetex_FLASH);
}


static void RefreshFlashTime(void)
{
	struct tm  tmNow;
	static UCHAR  Sec = 0;
	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)/*1秒钟*/
	{
		Sec = (UCHAR)tmNow.tm_sec;
		/*if(gEvChargeInfo.i8ProcRecordDelay>0)
		{
			gEvChargeInfo.i8ProcRecordDelay--;
		}*/
	}
}
static void SaveAgingRecordLog(UCHAR ucGunNo)
{
	gChargeCtrl.stAgingRecord[ucGunNo].u8GunType =1;
	gChargeCtrl.stAgingRecord[ucGunNo].u8GunNum  = 2;						//2 测试记录序号			1		
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[0] = 20;				//3 老化开始时间			7	
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[1] = 21;				//3 老化开始时间			7	
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[2] = 8;				//3 老化开始时间			7	
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[3] = 11;				//3 老化开始时间			7	
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[4] = 11;				//3 老化开始时间			7
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[5] = 1;				//3 老化开始时间			7	
	gChargeCtrl.stAgingRecord[ucGunNo].u8StartTime[6] = 11;				//3 老化开始时间			7

	SaveRecord2Flash = gChargeCtrl.stAgingRecord[ucGunNo];
	gFlashOptInfo.u16OptPageIdx = FLASH_WriteRecords(&gChargeLogTable, &SaveRecord2Flash);
}
static void ChargeRecordDeal(void)
{
	UCHAR i;
	for(i =0;i<ALL_GUN_NUM;i++)
	{
		if(gChargeCtrl.ucTestRecordState[i] == 3)
		{
			SaveAgingRecordLog(i);
			gChargeCtrl.ucTestRecordState[i] = 0;
		}
		else if(gChargeCtrl.ucAgingRecordState[i] == 2)
		{
			SaveAgingRecordLog(i);
			gChargeCtrl.ucAgingRecordState[i] = 0;
		}
	}
	
}
/*********************************************************************************************
	FLAHS 立即记录逻辑：
	SetCharge STOP  Cmd	  停止命令后	
	API_CreateRecordLog   并置写FLASH 的标志。
	如果离线 记录未上报索引
***********************************************************************************************/
void TASK_FlashProc(void* pdata)
{
	UINT16 u16temp;
	OSTimeDly(500);
	ChargeLogTable LogTable;
	FLASH_GetRecordsTable(&LogTable);
	gChargeLogTable = LogTable;
	u16DEBUG_Flash_ADDR = 0; 
	u16DEBUG_Flash_LEN = sizeof(SaveRecord2Flash);
	for(;;)
	{
		gSelfTest.fSetTaskID(Task_ID_Flash);
		RefreshFlashTime();

		if(TRUE == gFlashOptInfo.u8CleanRecordsFlag)
		{
			FlashCleanRecords();
			gFlashOptInfo.u8CleanRecordsFlag = FALSE;
			OSTimeDly(100);
			FLASH_GetRecordsTable(&LogTable);
			gChargeLogTable = LogTable;
			gSelfTest.fSetTaskID(Task_ID_Flash);
		}

		if (TRUE == gFlashOptInfo.u8CleanBulkFlag)
		{
			SPIFlashEraseChip();
			FlashCleanRecords();
			gFlashOptInfo.u8CleanBulkFlag = FALSE;
			OSTimeDly(100);
			FLASH_GetRecordsTable(&LogTable);
			gChargeLogTable = LogTable;
			gSelfTest.fSetTaskID(Task_ID_Flash);
		}


		ChargeRecordDeal();
		u16temp= FLASH_GetPageIdx(1);
		FLASH_ReadRecords(u16temp*BYTES_NUM_PER_PAGE, &stViewChargeRecordFlash.u8GunType,sizeof(stViewChargeRecordFlash)/sizeof(UINT8));

		gSelfTest.fSetTaskID(Task_ID_Flash);
		OSTimeDly(300);
	}
}
