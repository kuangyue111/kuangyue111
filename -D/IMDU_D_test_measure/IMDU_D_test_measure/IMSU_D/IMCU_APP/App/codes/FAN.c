
#include "main.h"
#define FAN_ALM_SPEED       8
#define FAN_ALM_TM          6
#define FAN_RUNNING_OK_TM   2
#define FAN_START_DELAY     24
#define mSTOP 0
#define mRUN  1





//只接一个风扇 六个FANALM里有一个有脉冲就不算告警！
#define POWER_PWM_GO() do {\
    dv_Capture_PWM.fOutputFanDriver(hPOWER_PWM, 50);\
} while (0);


void AC_Fan_Ctrl(unsigned char bOn)
{
	static unsigned char s_u8OnStatBak = 0xFF;

	struct tm  tmNow;	
	static UINT8 su8Sec = 0xff;
	static UINT8 su8SecDelay = 0;

	if(s_u8OnStatBak != bOn)
	{
		s_u8OnStatBak = bOn;

		if(bOn)
		{
			gEvChargeInfo.u8CTRLInterDOCMD_B = 0x01;
		}
		else
		{
			gEvChargeInfo.u8CTRLInterDOCMD_B = 0x00;
		}

		gEvChargeInfo.u8CTRLInterDOCMDFlag_B = TRUE;

		su8SecDelay = 0;
	}
	else
	{
		gRTCData.fGetLocalTime(&tmNow);	
		if (su8Sec != tmNow.tm_sec)
		{				
			su8Sec = tmNow.tm_sec;
			su8SecDelay++;
		}
		if(su8SecDelay > 10)
		{
			su8SecDelay = 0;
			s_u8OnStatBak = 0xff;
		}
	}
}
unsigned char g_ucFanCtrlOnOffFlag = 0;	//风扇启停状态 0-停止 1-启动
void TAST_FanProc(void* pdata)
{
	pdata = pdata;
	UINT8 u8STOPDelayTimes = 101;


	
	OSTimeDly(500);
	OSTimeDly(500);
	gSelfTest.fSetTaskID(TASK_ID_FAN);

	OSTimeDly(500);

	POWER_PWM_GO();


	AC_Fan_Ctrl(TRUE);	

	dv_Capture_PWM.fOutputFanDriver(mFAN1,99); //风机调速
	for (u8STOPDelayTimes = 0; u8STOPDelayTimes < 4; u8STOPDelayTimes++)
	{
		OSTimeDly(500);
		gSelfTest.fSetTaskID(TASK_ID_FAN);
		OSTimeDly(500);
	}


	AC_Fan_Ctrl(FALSE);
	dv_Capture_PWM.fOutputFanDriver(mFAN1,45);			//风机调速
	for (u8STOPDelayTimes = 0; u8STOPDelayTimes < 1; u8STOPDelayTimes++)
	{
		OSTimeDly(500);
		gSelfTest.fSetTaskID(TASK_ID_FAN);
		OSTimeDly(500);
	}
	
	dv_Capture_PWM.fOutputFanDriver(mFAN1,10);			//风机调速
	POWER_PWM_GO();
	for (u8STOPDelayTimes = 0; u8STOPDelayTimes < 1; u8STOPDelayTimes++)
	{
		OSTimeDly(500);
		gSelfTest.fSetTaskID(TASK_ID_FAN);
		OSTimeDly(500);
	}

	u8STOPDelayTimes = 101;

	for(;;)
	{
		OSTimeDly(850); 
		gSelfTest.fSetTaskID(TASK_ID_FAN);
	}
}
