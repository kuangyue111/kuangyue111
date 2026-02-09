
#include "main.h"
//#include "TelaidianProto.h"
#include "Uart6_new.h"
#include "EtherNet.h"

extern INT32 ReadTime_Try(void);
#include "can_ring.h"

//初始化GMT时间
static void InitGMTTime(void)
{
	struct tm * ptmNow;
	ptmNow = &gRTCData.RTCset;
	ptmNow->tm_year -= 1900;
	ptmNow->tm_mon -= 1;
	dv_RTC.fSetGMTSeconds(mktime_r(ptmNow));
}
//刷新GMT时间
static void  RefreshGMTtime(struct tm * ptmNow)
{
	gRTCData.refresh = TRUE;
	gRTCData.GMTtime.tm_sec = ptmNow->tm_sec;
	gRTCData.GMTtime.tm_min = ptmNow->tm_min;
	gRTCData.GMTtime.tm_hour = ptmNow->tm_hour;
	gRTCData.GMTtime.tm_mday = ptmNow->tm_mday;
	gRTCData.GMTtime.tm_mon = ptmNow->tm_mon + 1;
	gRTCData.GMTtime.tm_year = ptmNow->tm_year + 1900;
	gRTCData.GMTtime.tm_wday = ptmNow->tm_wday;
	gRTCData.GMTtime.tm_yday = ptmNow->tm_yday;
	gRTCData.GMTtime.tm_isdst = ptmNow->tm_isdst;
}
//刷新本地时间
static void  RefreshLocalTime(struct tm * ptmNow)
{
	gRTCData.localtime.tm_sec = ptmNow->tm_sec;
	gRTCData.localtime.tm_min = ptmNow->tm_min;
	gRTCData.localtime.tm_hour = ptmNow->tm_hour;
	gRTCData.localtime.tm_mday = ptmNow->tm_mday;
	gRTCData.localtime.tm_mon = ptmNow->tm_mon + 1;
	gRTCData.localtime.tm_year = ptmNow->tm_year + 1900;
	gRTCData.localtime.tm_wday = ptmNow->tm_wday;
	gRTCData.localtime.tm_yday = ptmNow->tm_yday;
	gRTCData.localtime.tm_isdst = ptmNow->tm_isdst;
}

//读取本地时间
static void  GetLocalTime(struct tm * tmNow)
{
	tmNow->tm_sec = gRTCData.localtime.tm_sec;
	tmNow->tm_min = gRTCData.localtime.tm_min;
	tmNow->tm_hour = gRTCData.localtime.tm_hour;
	tmNow->tm_mday = gRTCData.localtime.tm_mday;
	tmNow->tm_mon = gRTCData.localtime.tm_mon;
	tmNow->tm_year = gRTCData.localtime.tm_year;
	return;
}

//设定本地时间
static void  SetLocalTime(struct tm * tmNow)
{
	gRTCData.RTCset.tm_sec = tmNow->tm_sec;
	gRTCData.RTCset.tm_min = tmNow->tm_min;
	gRTCData.RTCset.tm_hour = tmNow->tm_hour;
	gRTCData.RTCset.tm_mday = tmNow->tm_mday;
	gRTCData.RTCset.tm_mon = tmNow->tm_mon;
	gRTCData.RTCset.tm_year = tmNow->tm_year;
	gRTCData.settimezone = gRTCData.localtimezone;
	gRTCData.set = TRUE;
}

//获取本地时区
static float  GetLocalTimeZone(void)
{
	GetFloatSetData(ADDR_LocalTimeZone, &gRTCData.localtimezone);
	gRTCData.localtimezone = 480;
	return  gRTCData.localtimezone;
}

//设定时区
static void  SetLocalTimeZone(void)
{
	gRTCData.localtimezone = 480;
	SetFloatSetData(ADDR_LocalTimeZone, &gRTCData.localtimezone);
}
//设本地时间
static void  SetGMTTime(struct tm * ptmNow)
{
	gRTCData.set = FALSE;
	gRTCData.refresh = TRUE;
	gRTCData.localtimezone = gRTCData.settimezone;
	ptmNow = &gRTCData.RTCset;
	ptmNow->tm_year -= 1900;
	ptmNow->tm_mon -= 1;
	dv_RTC.fSetGMTSeconds(ULONG_ADD_SLONG( mktime_r(ptmNow), -(long)(gRTCData.localtimezone * MIN_SECONDS)));
}

//获取GMT 秒
static UCHAR  GetGMTOldSecond(void)
{
  return  (UCHAR)gRTCData.GMTtime.tm_sec;
}
//获取驱动GMT秒
static time_t  GetDvGMTSeconds(void)
{
	gRTCData.RTCGMTSec = dv_RTC.fGetGMTSeconds();//get GMT seconds
	return  gRTCData.RTCGMTSec;
}

//GMT秒换算到本地秒
static time_t  GetSecondsGMT2LOCAL(void)
{
  return  ULONG_SUB_SLONG( gRTCData.RTCGMTSec, -(long)(gRTCData.localtimezone * MIN_SECONDS));
}

//初始化RTC
static void  DataInitRTC(void)
{
	struct tm  tmNow;
	time_t now;
	dv_RTC.fReadTime(1);
	now = gRTCData.fGetDvGMTSeconds();//get GMT seconds
	gRTCData.fGetLocalTimeZone();//Get_LocalTimeZone();
	gmtime_r(&now, &tmNow);
	gRTCData.fRefreshGMTTime(&tmNow);//RefreshGMTtime(ptmNow);
	now = gRTCData.fGetSecondsGMT2LOCAL();
	gmtime_r(&now, &tmNow);
	gRTCData.fRefreshLocalTime(&tmNow);//RefreshLocalTime(ptmNow);
}

static void AddSecondsToTimenow(UINT32 ui32Seconds)
{
	dv_RTC.RTC_Counter += ui32Seconds;
	dv_RTC.RTC_Counter_Back += ui32Seconds;
	gmtime_r((const time_t *)(&dv_RTC.RTC_Counter), &dv_RTC.tmNow);
}

RTCData_Type gRTCData =
{
	0,
	{0,0,0,1,1,2019,0,0,0},
	{0,0,8,1,1,2019,0,0,0},
	480,
	0,
	{0,0,0,1,1,2019,0,0,0},
	480,
	0,
	0,
	DataInitRTC,
	GetLocalTime,
	SetLocalTime,
	RefreshLocalTime,
	GetLocalTimeZone,
	SetLocalTimeZone,
	InitGMTTime,
	RefreshGMTtime,
	SetGMTTime,
	GetGMTOldSecond,
	GetDvGMTSeconds,
	GetSecondsGMT2LOCAL,
};



/**********************************************************************************************
	RTC 时钟处理线程
**********************************************************************************************/
void TASK_RTCProc (void* pdata)
{
	struct tm  tmNow;
	time_t now;
	UINT8 i;
	INT32 in32RTC_Counter = 0;
	INT32 in32RTC_CounterBak = 0;
	UINT8  isRefresh = 0;
	static UCHAR  s_Sec = 0, s_Min = 0;
	UNUSED(pdata);

	for(;;)
	{
		//RTC
		isRefresh = 0;
		gSelfTest.fSetTaskID(TASK_ID_RTC);
		gRTCData.fGetLocalTime(&tmNow);
		if ((UCHAR)tmNow.tm_sec != s_Sec)
		{
			isRefresh = 1;
			s_Sec = (UCHAR)tmNow.tm_sec;

			if(s_Min != (UCHAR)tmNow.tm_min)
			{	
				s_Min = (UCHAR)tmNow.tm_min;
				
				for(i = 0; i < ALL_GUN_NUM; i++)
				{
					if(gEvChargeInfo.u16GunOverTemLmtCurrDelay[i] > 0)
					{
						gEvChargeInfo.u16GunOverTemLmtCurrDelay[i]++;						
					}					
				}

				gs_CANComm.u16RectCommPeriodResetDelay++;

				if(gEvChargeInfo.u16ACInputContactorDisconCnt > 0)
				{
					gEvChargeInfo.u16ACInputContactorDisconCnt++;
				}
			}
				
			if(gEvChargeInfo.u32OffLineTimeCount > 0)
			{
				gEvChargeInfo.u32OffLineTimeCount++;
			}

			
			
		}  //END:  if ((UCHAR)tmNow.tm_sec != s_Sec)


		

		gSelfTest.fSetTaskID(TASK_ID_RTC);
		if (isRefresh && (s_Min == 28) && (s_Sec == 0))
		{
			//in32RTC_Counter = ReadTime_Try();
			////判断时间合理性
			//if (in32RTC_Counter != in32RTC_CounterBak
			//	&& in32RTC_Counter <= GMT_TIME_MAX
			//	&& in32RTC_Counter >= GMT_TIME_MIN
			//	&& in32RTC_Counter != -1)
			//{
			//	in32RTC_CounterBak = in32RTC_Counter;
			//	dv_RTC.fReadTime(0);
			//}
			dv_RTC.fReadTime(0);
		}
		else
		{
			if (gRTCData.ucSecondChangeFlag == mTRUE)
			{
				AddSecondsToTimenow(1);
				gRTCData.ucSecondChangeFlag = mFALSE;
			}
		}
		now = gRTCData.fGetDvGMTSeconds();
		//timezone
		gRTCData.fGetLocalTimeZone();
		gmtime_r(&now, &tmNow);
		if(gRTCData.fGetGMTOldSecond() != tmNow.tm_sec)
		{
			gRTCData.fRefreshGMTTime(&tmNow);
			now = gRTCData.fGetSecondsGMT2LOCAL();
			gmtime_r(&now, &tmNow);
			gRTCData.fRefreshLocalTime(&tmNow);
		}
		gSelfTest.fSetTaskID(TASK_ID_RTC);
		if(gRTCData.set == TRUE)
		{
			gRTCData.fSetGMTTime(&tmNow);
			gRTCData.fSetLocalTimeZone();
		}
		OSTimeDly(150);
		gSelfTest.fSetTaskID(TASK_ID_RTC);
	}
}
