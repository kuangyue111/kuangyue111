#include "main.h"
void InitSelfTestId(void);
static UCHAR  Check_ucOSID(UINT16 * ptaskstop);

UINT32 u32CheckID_VAL = 0;

static void  set_TaskID(UCHAR ID)
{
	gSelfTest.TaskID[ID]++;
}

static void  set_stopfeeddog(void)
{
	gSelfTest.StopFeedDog = TRUE;
}

static UCHAR  get_stopfeeddog(void)
{
  return  gSelfTest.StopFeedDog;
}


static UCHAR  is_stopfeeddog(void)
{
  if((gSelfTest.StopFeedDog > 0) && (gSelfTest.StopFeedDog < 6))
  {
    if(!gs_E2PROM.fIsHistoryEventUpdate())
    {
      gSelfTest.StopFeedDog++;
    }
  }
  if(gSelfTest.StopFeedDog < 6)
  {
    return  FALSE;
  }
  return  TRUE;
}

static UCHAR  Check_ucOSID(UINT16 * ptaskstop)
{
	UNUSED(ptaskstop);
	static int siCheckTimes = 0;
	if (gSelfTest.iSDOGPeriod == SFT_WDOG_PERIOD)		//TICK 没有起博了。
	{
		siCheckTimes++;
		if (siCheckTimes > 30)
		{
			siCheckTimes = 0;
			if(gSelfTest.StopFeedDogReason == 0)
			{
				gSelfTest.StopFeedDogReason = 3;
				gSelfTest.u32DogBit_Bak = gSelfTest.u32DogBit;
				gs_E2PROM.fSetHistoryEventUpdate();
			}
			gSelfTest.fSetStopFeedDog();				//WAIT GAME OVER
		}
	}
	else
	{
		siCheckTimes = 0;
	}
	return 0;
}

static UCHAR  Check_taskID(UINT16 * ptaskstop)
{
	//, alltaskrun;
	UCHAR  i;
	UINT16 * ptaskID;
	UNUSED(ptaskstop);
	UINT8 u8ErrId = 0xff;

	Check_ucOSID(ptaskstop);												//Check TICK

	ptaskID = gSelfTest.TaskID;
	for(i = 0; i < CHECKTASKNUM; i++)
	{
		if(*ptaskID > 0)													//task running
		{
			*ptaskID = 0;
			gSelfTest.u32DogBit &= (~(0x01<<i));							//有喂过心跳, 就将BIT位清0
		}
		else																//task stop
		{
			u8ErrId = i;
		}
		ptaskID++;
	}

	WTDOG_FEED();

	if (gSelfTest.iSDOGPeriod <= 0)
	{
		if (0 != (u32CheckID_VAL & gSelfTest.u32DogBit))
		{
			if(gSelfTest.StopFeedDogReason == 0)
			{
				gSelfTest.u32DogBit_Bak = gSelfTest.u32DogBit;
				gSelfTest.StopFeedDogReason = 4;
				gs_E2PROM.fSetHistoryEventUpdate();
			}		
			gSelfTest.fSetStopFeedDog();									//WAIT GAME OVER
		}
		gSelfTest.iSDOGPeriod = SFT_WDOG_PERIOD;
		gSelfTest.u32DogBit = u32CheckID_VAL;
	}

	return u8ErrId;
}

static UINT32  get_mintaskstkfree(void)
{
  return  gSelfTest.minTaskStkFree;
}

static void  set_mintaskstkfree(INT32U data)
{
  gSelfTest.minTaskStkFree = data;
}

static void  set_oscpuusage(INT8S data)
{
  gSelfTest.OSCPUUsage = data;
}

static void  set_osctxswctr(INT32U data)
{
  gSelfTest.OSCtxSwCtr = data;
}


SeltTest_TypeDef gSelfTest =
{
	{0},									//uCOSID;
	0,										//StopFeedDog
	0,										//CheckSetDataVersion
	TASK_STK_SIZE * sizeof(OS_STK),			//minTaskStkFree
	0,										//OSCPUUsage
	0,										//OSCtxSwCtr
	0xffffffff,
	SFT_WDOG_PERIOD,									//2分钟
	0,
	0,
	0,    //UINT16  LastSetDataID;
	0xff,  //float fLastSetDataValue;
	set_TaskID,
	set_stopfeeddog,
	get_stopfeeddog,
	is_stopfeeddog,

	Check_taskID,

	get_mintaskstkfree,
	set_mintaskstkfree,
	set_oscpuusage,
	set_osctxswctr,
};

void InitSelfTestId(void)
{
	int i = 0;
	
	for(i = 0; i < CHECKTASKNUM; i++)
	{
		u32CheckID_VAL |= (0x01<<i);
	}
	gSelfTest.u32DogBit = u32CheckID_VAL;
	WTDOG_FEED();
	gSelfTest.iSDOGPeriod = SFT_WDOG_PERIOD;

	gSelfTest.StopFeedDogReason = 0;
}

/************任务自检************/
void TASK_SelfTestProc(void* pdata)
{
	//, alltaskrun;
        UCHAR  i;
	UINT16 taskstop = 0;
	//float ftemp;
	INT8U err;
	OS_STK_DATA data = {0};
	INT32U minfree = TASK_STK_SIZE * sizeof(OS_STK);
	UCHAR u8Buf[4];
	UNUSED(data);
	u8Buf[0] =0;
	u8Buf[1] =0;
	dv_E2PROM.fReadE2PROMPage(u8Buf,E2PROMFLAGADD1,2);
	if (u8Buf[0]!=0|| u8Buf[1]!=0)
	{
		u8Buf[0] = 0;
		u8Buf[1] = 0;
		dv_E2PROM.fWriteE2PROMPage(u8Buf, E2PROMFLAGADD1, 2);
		dv_E2PROM.fWriteE2PROMPage(u8Buf, E2PROMFLAGADD1, 2);
	}

	InitSelfTestId();

	for(;;)
	{
		//check stack
		for(i = 0; i < TASK_MAX_NUM; i++)
		{
			err = OSTaskStkChk(sTaskArray[i].u8Priority, &data);
			if(err == OS_ERR_NONE)
			{
				if(minfree > data.OSFree)
				{
					minfree = data.OSFree;
				}
			}
		}
		gSelfTest.fSetMinTaskStkFree(minfree);
		gSelfTest.fSetOSCPUUsage(OSCPUUsage);
		gSelfTest.fSetOSCtxSwCtr(OSCtxSwCtr);
		OSCtxSwCtr = 0;

		if (gSelfTest.fIsStopFeedDog() == TRUE)
		{
			continue;
		}

		//check  ID
		//alltaskrun = gSelfTest.fcheck_taskID(&taskstop);
		//must here!
		gSelfTest.fCheck_taskID(&taskstop);

		OSTimeDly(500);
	}
}
