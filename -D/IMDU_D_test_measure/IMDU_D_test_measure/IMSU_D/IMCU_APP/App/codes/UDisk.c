#include "main.h"
#include "usbh_core.h"
#include "UDisk.h"
#include "bsp_usb.h"

NOINIT U_DISK_DATA_ST gs_UDiskData;
NOINIT __CHARGE_RECORD UDiskReadRecord;
NOINIT UCHAR ucUDiskWriteBuff[BYTES_NUM_PER_PAGE*2+6];


static void InitUDiskData(void)
{
	gs_UDiskData.ucCopyRecordFlag = FALSE;
	gs_UDiskData.u16RecordStartNum =0;
	gs_UDiskData.u16RecordEndNum = 0;
	gs_UDiskData.ucRecordWriteStep = 0;
	gs_UDiskData.u16RecordWriteIndex = 0;
	gs_UDiskData.u16RecordWriteTestIndex = 0;
	gs_UDiskData.u16RecordWriteAgingIndex = 0;
}

static UCHAR NumberToChar(UCHAR data)
{
	UCHAR RTN = 0;
	if((data>=0)&&(data<=9))
	{
		RTN = data+'0';
	}
	else if((data>=10)&&(data<16))
	{
		RTN = data-10+'A';
	}
	else
	{
		RTN = 0;
	}
	return RTN;
}
static void TimeToFileName(UCHAR* pBuff,UCHAR*pData)
{
	struct tm  tmNow;
	UCHAR temp;

	pBuff[0] = NumberToChar(pData[1]);
	temp = (UCHAR)((pData[2]/10)%10);
	pBuff[1] = NumberToChar(temp);
	temp = (UCHAR)(pData[2]%10);
	pBuff[2] = NumberToChar(temp);
	temp = (UCHAR)((pData[3]/10)%10);
	pBuff[3] = NumberToChar(temp);
	temp = (UCHAR)(pData[3]%10);
	pBuff[4] = NumberToChar(temp);
	temp = (UCHAR)((pData[4]/10)%10);
	pBuff[5] = NumberToChar(temp);
	temp = (UCHAR)(pData[4]%10);
	pBuff[6] = NumberToChar(temp);
	/*temp = (UCHAR)((pData[0]/10)%10);
	pBuff[0] = NumberToChar(temp);
	temp = (UCHAR)(pData[0]%10);
	pBuff[1] = NumberToChar(temp);
	temp = (UCHAR)((pData[1]/10)%10);
	pBuff[2] = NumberToChar(temp);
	temp = (UCHAR)(pData[1]%10);
	pBuff[3] = NumberToChar(temp);
	temp = (UCHAR)((pData[2]/10)%10);
	pBuff[4] = NumberToChar(temp);
	temp = (UCHAR)(pData[2]%10);
	pBuff[5] = NumberToChar(temp);
	temp = (UCHAR)((pData[3]/10)%10);
	pBuff[6] = NumberToChar(temp);
	temp = (UCHAR)(pData[3]%10);
	pBuff[7] = NumberToChar(temp);
	temp = (UCHAR)((pData[4]/10)%10);
	pBuff[8] = NumberToChar(temp);
	temp = (UCHAR)(pData[4]%10);
	pBuff[9] = NumberToChar(temp);
	temp = (UCHAR)((pData[5]/10)%10);
	pBuff[10] = NumberToChar(temp);
	temp = (UCHAR)(pData[5]%10);
	pBuff[11] = NumberToChar(temp);*/
}
static void TimeToChar(UCHAR* pBuff,UCHAR*pData)
{
	struct tm  tmNow;
	UCHAR temp;

	temp = (UCHAR)((pData[0]/10)%10);
	pBuff[0] = NumberToChar(temp);
	temp = (UCHAR)(pData[0]%10);
	pBuff[1] = NumberToChar(temp);
	pBuff[2] = '/';
	temp = (UCHAR)((pData[1]/10)%10);
	pBuff[3] = NumberToChar(temp);
	temp = (UCHAR)(pData[1]%10);
	pBuff[4] = NumberToChar(temp);
	pBuff[5] = '/';
	temp = (UCHAR)((pData[2]/10)%10);
	pBuff[6] = NumberToChar(temp);
	temp = (UCHAR)(pData[2]%10);
	pBuff[7] = NumberToChar(temp);
	pBuff[8] = ' ';

	temp = (UCHAR)((pData[3]/10)%10);
	pBuff[9] = NumberToChar(temp);
	temp = (UCHAR)(pData[3]%10);
	pBuff[10] = NumberToChar(temp);
	pBuff[11] = ':';
	temp = (UCHAR)((pData[4]/10)%10);
	pBuff[12] = NumberToChar(temp);
	temp = (UCHAR)(pData[4]%10);
	pBuff[13] = NumberToChar(temp);
	pBuff[14] = ':';

	temp = (UCHAR)((pData[5]/10)%10);
	pBuff[15] = NumberToChar(temp);
	temp = (UCHAR)(pData[5]%10);
	pBuff[16] = NumberToChar(temp);
}
static void FloatTOChar(UCHAR* pBuff,float fdata)
{
	float ftemp;
	UCHAR temp,index =0,i;
	ftemp = fdata;
	if(ftemp>=1000)
	{
		temp = ((UINT32)(ftemp/1000))%10;
		pBuff[index++] = NumberToChar(temp);
	}
	if(ftemp>=100)
	{
		temp = ((UINT32)(ftemp/100))%10;
		pBuff[index++] = NumberToChar(temp);
	}
	if(ftemp>=10)
	{
		temp =  ((UINT32)(ftemp/10))%10;
		pBuff[index++] = NumberToChar(temp);
	}
	
	temp = ((UINT32)ftemp)%10;
	pBuff[index++] = NumberToChar(temp);
	pBuff[index++] = '.';
	temp =  ((UINT32)(ftemp*10))%10;
	pBuff[index++] = NumberToChar(temp);
	if(index<6)
	{
		for(i=index;i<6;i++)
		{
			pBuff[index++]= ' ';
		}
	}
}
static void IntTOChar(UCHAR* pBuff,UINT16 u16data)
{
	UINT16 u16temp;
	UCHAR temp,index =0,i;
	u16temp = u16data;

	if(u16temp >= 10000)
	{
		temp = ((UCHAR)(u16temp/10000))%10;
		pBuff[index++] = NumberToChar(temp);
	}
	if(u16temp >= 1000)
	{
		temp = ((UCHAR)(u16temp/1000))%10;
		pBuff[index++] = NumberToChar(temp);
	}
	if(u16temp>=100)
	{
		temp = ((UCHAR)(u16temp/100))%10;
		pBuff[index++] = NumberToChar(temp);
	}
	if(u16temp>=10)
	{
		temp =  ((UCHAR)(u16temp/10))%10;
		pBuff[index++] = NumberToChar(temp);
	}

	temp =  (UCHAR)(u16temp%10);
	pBuff[index++] = NumberToChar(temp);

	if(index<5)
	{
		for(i=index;i<5;i++)
		{
			pBuff[index++]= ' ';
		}
	}
}
static void FileNameProcess(UCHAR TestType)
{
	struct tm  tmNow;
	UCHAR ucTimeNow[7],ucIndex =0;

	gRTCData.fGetLocalTime(&tmNow);
	ucTimeNow[0] = (UCHAR)(tmNow.tm_year/100);
	ucTimeNow[1] = (UCHAR)(tmNow.tm_year%100);
	ucTimeNow[2] = (UCHAR)tmNow.tm_mon;
	ucTimeNow[3] = (UCHAR)tmNow.tm_mday;
	ucTimeNow[4] = (UCHAR)tmNow.tm_hour;
	ucTimeNow[5] = (UCHAR)tmNow.tm_min;
	ucTimeNow[6] = (UCHAR)tmNow.tm_sec;
	if(TestType >=2)
	{
		gs_UDiskData.ucFileName[ucIndex++]='0';
		gs_UDiskData.ucFileName[ucIndex++]=':';
		gs_UDiskData.ucFileName[ucIndex++]='T';
		//gs_UDiskData.ucFileName[ucIndex++]='E';
		//gs_UDiskData.ucFileName[ucIndex++]='S';
		//gs_UDiskData.ucFileName[ucIndex++]='T';
		//gs_UDiskData.ucFileName[ucIndex++]='R';
		//gs_UDiskData.ucFileName[ucIndex++]='E';
		//gs_UDiskData.ucFileName[ucIndex++]='C';
		//gs_UDiskData.ucFileName[ucIndex++]='O';
		//gs_UDiskData.ucFileName[ucIndex++]='R';
		//gs_UDiskData.ucFileName[ucIndex++]='D';
	}
	else
	{
		gs_UDiskData.ucFileName[ucIndex++]='0';
		gs_UDiskData.ucFileName[ucIndex++]=':';
		gs_UDiskData.ucFileName[ucIndex++]='A';
		//gs_UDiskData.ucFileName[ucIndex++]='G';
		//gs_UDiskData.ucFileName[ucIndex++]='I';
		//gs_UDiskData.ucFileName[ucIndex++]='N';
		//gs_UDiskData.ucFileName[ucIndex++]='G';
		//gs_UDiskData.ucFileName[ucIndex++]='R';
		//gs_UDiskData.ucFileName[ucIndex++]='E';
		//gs_UDiskData.ucFileName[ucIndex++]='C';
		//gs_UDiskData.ucFileName[ucIndex++]='O';
		//gs_UDiskData.ucFileName[ucIndex++]='R';
		//gs_UDiskData.ucFileName[ucIndex++]='D';
	}
	TimeToFileName(&gs_UDiskData.ucFileName[ucIndex],&ucTimeNow[1]);
	ucIndex =ucIndex+7;
	gs_UDiskData.ucFileName[ucIndex++]='.';
	gs_UDiskData.ucFileName[ucIndex++]='t';
	gs_UDiskData.ucFileName[ucIndex++]='x';
	gs_UDiskData.ucFileName[ucIndex++]='t';
	if(ucIndex<30)
	{	
		for(;ucIndex<30;ucIndex++)
		{
			gs_UDiskData.ucFileName[ucIndex] = 0;
		}
	}
}
static UINT16 WriteRecordContent(UINT16 DataLength,UCHAR ucWriteHeadFlag )
{
	UINT16 i,u16Idx = 0,RecordIndex;

	if((UDiskReadRecord.u8TestResult < 2)&&(gs_UDiskData.u16RecordWriteAgingIndex == 0))
	{
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = 'I';
		ucUDiskWriteBuff[u16Idx++] = 'N';
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = 'R';
		ucUDiskWriteBuff[u16Idx++] = 'E';
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'O';
		ucUDiskWriteBuff[u16Idx++] = 'R';
		ucUDiskWriteBuff[u16Idx++] = 'D';
	}
	else if((UDiskReadRecord.u8TestResult >= 2)&&(gs_UDiskData.u16RecordWriteTestIndex == 0))
	{
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = 'E';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = 'R';
		ucUDiskWriteBuff[u16Idx++] = 'E';
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'O';
		ucUDiskWriteBuff[u16Idx++] = 'R';
		ucUDiskWriteBuff[u16Idx++] = 'D';
	}
	FileNameProcess(UDiskReadRecord.u8TestResult);
	ucUDiskWriteBuff[u16Idx++] = '\r';
	ucUDiskWriteBuff[u16Idx++] = '\n';
	if(UDiskReadRecord.u8TestResult < 2)
	{
                RecordIndex = gs_UDiskData.u16RecordWriteAgingIndex++;
	}
	else
	{
		RecordIndex = gs_UDiskData.u16RecordWriteTestIndex++;	
	}
	ucUDiskWriteBuff[u16Idx++] = (UINT8)(RecordIndex/100)+'0';
	ucUDiskWriteBuff[u16Idx++] = (UINT8)((RecordIndex%100)/10)+'0';
	ucUDiskWriteBuff[u16Idx++] = (UINT8)(RecordIndex%10)+'0';
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	for(i=0;i<32;i++)
	{
		ucUDiskWriteBuff[u16Idx++] = UDiskReadRecord.u8EVSerialNum[i];
	}
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	if(UDiskReadRecord.u8TestResult == 0)
	{
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = 'I';
		ucUDiskWriteBuff[u16Idx++] = 'N';
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = 'P';
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
	}
	else if(UDiskReadRecord.u8TestResult == 1)
	{
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = 'I';
		ucUDiskWriteBuff[u16Idx++] = 'N';
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = 'F';
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'I';
		ucUDiskWriteBuff[u16Idx++] = 'L';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
	}
	else if(UDiskReadRecord.u8TestResult == 2)
	{
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = 'E';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = 'P';
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
	}
	else if(UDiskReadRecord.u8TestResult == 3)
	{
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = 'E';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
		ucUDiskWriteBuff[u16Idx++] = 'F';
		ucUDiskWriteBuff[u16Idx++] = 'A';
		ucUDiskWriteBuff[u16Idx++] = 'I';
		ucUDiskWriteBuff[u16Idx++] = 'L';
		ucUDiskWriteBuff[u16Idx++] = ',';
		ucUDiskWriteBuff[u16Idx++] = ' ';
	}
	
	TimeToChar(&ucUDiskWriteBuff[u16Idx],&UDiskReadRecord.u8StartTime[1]);
	u16Idx = u16Idx+17;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	TimeToChar(&ucUDiskWriteBuff[u16Idx],&UDiskReadRecord.u8EndTime[1]);
	u16Idx = u16Idx+17;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	ucUDiskWriteBuff[u16Idx++] = '0'+UDiskReadRecord.u8GunNum;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	if(UDiskReadRecord.u8GunType == PROTOCOL_TYPE_GBT)
	{
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = 'B';
		ucUDiskWriteBuff[u16Idx++] = 'T';
		ucUDiskWriteBuff[u16Idx++] = ' ';
	}
	else if(UDiskReadRecord.u8GunType == PROTOCOL_TYPE_GBT_PT)
	{
		ucUDiskWriteBuff[u16Idx++] = 'G';
		ucUDiskWriteBuff[u16Idx++] = '-';
		ucUDiskWriteBuff[u16Idx++] = 'P';
		ucUDiskWriteBuff[u16Idx++] = 'T';
	}
	else if(UDiskReadRecord.u8GunType == PROTOCOL_TYPE_CCS1)
	{
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = '1';
	}
	else if(UDiskReadRecord.u8GunType == PROTOCOL_TYPE_CCS2)
	{
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'S';
		ucUDiskWriteBuff[u16Idx++] = '2';
	}
	else if(UDiskReadRecord.u8GunType == PROTOCOL_TYPE_CHADEMO)
	{
		ucUDiskWriteBuff[u16Idx++] = 'C';
		ucUDiskWriteBuff[u16Idx++] = 'H';
		ucUDiskWriteBuff[u16Idx++] = 'D';
		ucUDiskWriteBuff[u16Idx++] = 'M';
	}
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fUserSetVolt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fUserSetCurr);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';
	
	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fEVOutputMaxVolt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fEVOutputMinVolt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fEVOutputMaxCurr);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fEVOutputMinCurr);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fMeasuerMaxVolt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';
	
	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fMeasuerMinVolt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fMeasuerMaxCurr);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fMeasuerMinCurr);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fCC2Volt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	FloatTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.fBMSVolt);
	u16Idx = u16Idx+6;
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';
		
	IntTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.u16UserSetTime);	
	u16Idx = u16Idx+5;	
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	IntTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.u16TestTime);	
	u16Idx = u16Idx+5;	
	ucUDiskWriteBuff[u16Idx++] = ',';
	ucUDiskWriteBuff[u16Idx++] = ' ';

	IntTOChar(&ucUDiskWriteBuff[u16Idx],UDiskReadRecord.u8TestFailReason);	
	u16Idx = u16Idx+5;	
	ucUDiskWriteBuff[u16Idx++] = '@';

	return u16Idx;
}
void TASK_UDiskProc(void* pdata)
{
	static UCHAR ucCopyRecordFlagBack = FALSE;
	UINT16 u16RecordWritePageIdx = 0;
	UINT16 DataLength = 0;
	UCHAR ucWriteHeadFlag =0;
	pdata = pdata;
	InitUDiskData();
        OSTimeDly(1000);
	dv_USB.fUSBInit();
	for(;;)
	{
		gSelfTest.fSetTaskID(TASK_ID_UDisk);
		OSTimeDly(100);  

         dv_USB.fUSBProcess();
		if((dv_USB.fGetUDiskState() == APPLICATION_DISCONNECT)
		||(gs_UDiskData.ucRecordWriteStep >=5))
		{
			InitUDiskData();
			continue;
		}
		if(gs_UDiskData.ucRecordWriteStep ==0)
		{
			if(ucCopyRecordFlagBack != gs_UDiskData.ucCopyRecordFlag)
			{
				ucCopyRecordFlagBack = gs_UDiskData.ucCopyRecordFlag;
				if(gs_UDiskData.ucCopyRecordFlag == FALSE)
				{
					continue;
				}
				else if(gs_UDiskData.ucCopyRecordFlag == TRUE)
				{
					gs_UDiskData.u16RecordWriteIndex = gs_UDiskData.u16RecordStartNum;
					ucWriteHeadFlag  = TRUE;
				}
			}
			else
			{
				if(gs_UDiskData.ucCopyRecordFlag == TRUE)
				{
					if(gs_UDiskData.u16RecordWriteIndex <= gs_UDiskData.u16RecordEndNum)
					{
						gs_UDiskData.u16RecordWriteIndex++;
					}
				}
			}
			if(gs_UDiskData.u16RecordWriteIndex > gs_UDiskData.u16RecordEndNum)
			{
				gs_UDiskData.ucCopyRecordFlag =FALSE;
			}
			else if(gs_UDiskData.ucCopyRecordFlag == TRUE)
			{
				u16RecordWritePageIdx  = FLASH_GetPageIdx(gs_UDiskData.u16RecordWriteIndex);
				DataLength = sizeof(__CHARGE_RECORD)/sizeof(UINT8);
				FLASH_ReadRecords(u16RecordWritePageIdx *BYTES_NUM_PER_PAGE, (UINT8*)(&UDiskReadRecord),DataLength);
				DataLength = WriteRecordContent(DataLength,ucWriteHeadFlag);
				ucWriteHeadFlag =FALSE;
				gs_UDiskData.ucRecordWriteStep = 1;
                dv_USB.ucUSBFlag = 0;
			}
				
			if(gs_UDiskData.ucCopyRecordFlag == FALSE)
			{
				continue;
			}
		}

		if(dv_USB.ucUSBFlag !=1)
		{
            if((dv_USB.ucUSBFlag >=2)&&(gs_UDiskData.ucRecordWriteStep == 1))
            {
                gs_UDiskData.ucRecordWriteStep++;
            }
			if(dv_USB.ucUSBFlag >= 200)
			{
				gs_UDiskData.ucRecordWriteStep++;
				dv_USB.fUSBInit();
				OSTimeDly(10);
				dv_USB.ucUSBFlag = 0;
			}
			 
			dv_USB.fUSBWriteData(ucUDiskWriteBuff,DataLength,gs_UDiskData.ucFileName);
			if(dv_USB.ucUSBFlag == 1)
			{
				gs_UDiskData.ucRecordWriteStep = 0 ;
			}
		}
	}
}
