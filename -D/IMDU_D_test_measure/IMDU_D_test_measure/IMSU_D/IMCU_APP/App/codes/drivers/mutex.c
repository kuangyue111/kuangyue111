#include "main.h"
#include "Measure.h"
extern MeasureData_TypeDef gMeasureInfo;
HANDLE MutexCreate(UCHAR prio)
{
	INT8U	err;
	HANDLE	hMutex;

	hMutex = (HANDLE)OSMutexCreate(prio, &err);

	if(hMutex == NULL)
	{
		return NULL;
	}

	if(err != OS_ERR_NONE)
	{
		return NULL;
	}

	return  hMutex;
}

UCHAR  MutexLock(HANDLE hm)
{
	INT8U   err;
	INT32U	ticks;

	//ticks = OS_TICKS_PER_SEC * ((INT32U)WaitTimeout + 500 / OS_TICKS_PER_SEC) / 1000;
	ticks = OS_TICKS_PER_SEC * MUTEX_LOCK_WAIT_TIME / 1000;
	ticks = ticks % 65536;

	
	if(hm == NULL)
	{
		return FALSE;
	}

	OSMutexPend((OS_EVENT*)hm, ticks, &err);
	
	if(err != OS_ERR_NONE)
	{
		return ERR_MUTEX_TIMEOUT;
	}

	return ERR_MUTEX_OK;
}

void MutexUnlock(HANDLE hm)
{	
	if(hm != NULL)
	{
		OSMutexPost((OS_EVENT*)hm);
	}
}

UCHAR InitMutex(void)
{
	if(gSetDataInfo.fInitMutex() == FALSE)
	{
		return FALSE;
	}
	if(gs_CANComm.fMutexInit() == FALSE)
	{
		return FALSE;
	}
	if(gMeasureInfo.fInitMutex() == FALSE)
	{
		return FALSE;
	}
	if(dv_Uart6.fInitSem() == FALSE)
	{
		return FALSE;
	}
	if(dv_E2PROM.fInitMutex() == FALSE)
	{
		 return FALSE;
	}
	
	if(dv_Output.fInitMutex() == FALSE)
	{
		return FALSE;
	}

	gEvChargeInfo.hMutexWriteEVInfo = MutexCreate(UPPER_PRIO_READ_WRITE_EV_DATA);

	if (NULL == gEvChargeInfo.hMutexWriteEVInfo)//Try Again!
	{
		gEvChargeInfo.hMutexWriteEVInfo = MutexCreate(UPPER_PRIO_READ_WRITE_EV_DATA);
	}

	gEvChargeInfo.hMetexNet_R = MutexCreate(UPPER_PRIO_NET_R_W_BUF);
	if(gEvChargeInfo.hMetexNet_R == NULL)
	{
		gEvChargeInfo.hMetexNet_R = MutexCreate(UPPER_PRIO_NET_R_W_BUF);
	}
	gEvChargeInfo.hMetexNet_W = MutexCreate(UPPER_PRIO_NET_W_BUF);
	if(gEvChargeInfo.hMetexNet_W == NULL)
	{
		gEvChargeInfo.hMetexNet_W = MutexCreate(UPPER_PRIO_NET_W_BUF);
	}

	gEvChargeInfo.hMetex_ALM = MutexCreate(UPPER_PRIO_ALM_INFO);
	if (gEvChargeInfo.hMetex_ALM == NULL)
	{
		gEvChargeInfo.hMetex_ALM = MutexCreate(UPPER_PRIO_ALM_INFO);
	}
	
	/*gEvChargeInfo.hMetex_FLASH = MutexCreate(UPPER_PRIO_FLASH_INFO);
	if (gEvChargeInfo.hMetex_FLASH == NULL)
	{
		gEvChargeInfo.hMetex_FLASH = MutexCreate(UPPER_PRIO_FLASH_INFO);
	}*/
	gEvChargeInfo.hMetex_FLASH = NULL;

	
	
	return  TRUE;
}
















