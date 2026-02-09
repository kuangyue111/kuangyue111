#include "main.h"
//#include "TelaidianProto.h"
#include "can_ring.h"
#include "ALM.h"
NOINIT ST_AlarmMng  gs_AlarmMng;

static void RefreshAlmTime(void)
{
	struct tm  tmNow;
	static UCHAR  Sec = 0;
	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)/*1√Î÷”*/
	{
		Sec = (UCHAR)tmNow.tm_sec;	
	}
}


void TASK_AlmProc(void* pdata)
{
	UINT8 u8Idx = 0;


	for (;;)
	{
		OSTimeDly(50);
		gSelfTest.fSetTaskID(TASK_ID_ALARM);
	}
}


