#include "main.h"
#include "Uart6_new.h"
#include "bsp_usb.h"
#include "ff.h"
#include "usbh_msc.h"




/************************************************/
/*函数功能:AD工作路选通	 CPU引脚选择！*/
/************************************************/
static void ADWorkPortSelect(UCHAR ChanelID)
{
	extern ADC_HandleTypeDef    AdcHandle;
	BSP_AdcChannelSelect(&AdcHandle,ChanelID); 
}

/************************************************/
/*函数功能:开始测量*/
/************************************************/
static void StartMeasure(UINT32* pValueBuffer,UINT32 Num)
{
	extern ADC_HandleTypeDef    AdcHandle;
	BSP_AdcStart(&AdcHandle,(uint32_t*)pValueBuffer,(uint32_t)Num);
}

/************************************************/
/*函数功能:读取测量值*/
/************************************************/
static UCHAR GetAdcValue(UCHAR ChanelID,UINT16* pRTNData)
{
	UCHAR i, j, k;
	UCHAR u16RTN = FALSE;
	double f64sum;    
	UINT32 u32AdcMax[mSLOW_DISCARD_NUM][2];
	UINT32 u32AdcMin[mSLOW_DISCARD_NUM][2];
	f64sum = 0;
	
	//TXY-AD
	if (ChanelID >= ADC_ALL_CHANAL_NUM)
	{
		u16RTN = FALSE;//防止越界！定义数组加3也是防止越界。
	}
	else if(ChanelID < ADC_CHANNEL_3_SLOW)
	{
		*pRTNData =  (UINT16)(dv_Adc.u32FastChannelADCValue[ChanelID]);
		u16RTN = TRUE;
	}
	else
	{

		if(dv_Adc.stSlowChannelData[ChanelID-2].ucGetDataNum< mSLOW_ALL_POINT_NUM)
		{
			u16RTN = FALSE;
		}
		else
		{
			/*初始化*/
			for(i = 0; i < mSLOW_DISCARD_NUM; i++)
			{
				u32AdcMax[i][0] = 0x0000;
				u32AdcMin[i][0] = 0xffff;
				u32AdcMax[i][1] = 0xffff;
				u32AdcMin[i][1] = 0xffff;
			}
			for(i = 0; i < mSLOW_DISCARD_NUM; i++)
			{
				for(j = 0; j< dv_Adc.stSlowChannelData[ChanelID-2].ucGetDataNum; j++)
				{
					if(i == 0)
					{
						f64sum += dv_Adc.stSlowChannelData[ChanelID-2].u32ADCValue[j];
					}
					for(k=0;k<i;k++)
					{
						if((u32AdcMax[k][1] == j)||(u32AdcMin[k][1] == j))
						{
							break;
						}
					}
					if(k ==i)
					{
						if(u32AdcMax[i][0] < dv_Adc.stSlowChannelData[ChanelID-2].u32ADCValue[j])
						{
							u32AdcMax[i][0] = dv_Adc.stSlowChannelData[ChanelID-2].u32ADCValue[j];
							u32AdcMax[i][1] = j;
						}
						if(u32AdcMin[i][0] > dv_Adc.stSlowChannelData[ChanelID-2].u32ADCValue[j])
						{
							u32AdcMin[i][0] = dv_Adc.stSlowChannelData[ChanelID-2].u32ADCValue[j];
							u32AdcMin[i][1] = j;
						}
					}
				}
			}

			for(i = 0; i < mSLOW_DISCARD_NUM; i++)
			{
				f64sum -= (u32AdcMax[i][0]+u32AdcMin[i][0]);
			}
			f64sum /= (dv_Adc.stSlowChannelData[ChanelID-2].ucGetDataNum- mSLOW_DISCARD_NUM*2); 
			*pRTNData  = (UINT16)f64sum;
			dv_Adc.stSlowChannelData[ChanelID-2].ucGetDataNum = 0;
			u16RTN = TRUE;

		}
	}
	return u16RTN;
}
static void InitAdc(void)
{
	 BSP_AdcInit();
}
/************************************************/
/*函数功能:DMA 停止*/
/************************************************/
static void DMAStop(void)
{
	extern ADC_HandleTypeDef  AdcHandle;
	BSP_AdcStop(&AdcHandle);
}

dv_Adc_TypeDef dv_Adc =
{
	0,//ucDMASampelTime
	0,  //ucStartMeasureFlag
	0,  // OneLoopFlag;
	0,  // ucSampleIntervalCount;
	{0},//u32OneLoopValue[mPOINT_NUM_EACH_TIME];
	{0},//u32FastChannelADCValue[ADC_FAST_CHANNEL_NUM];//快速采集，则用一次采集到底值进行计算
	{{0,{0}},{0,{0}},{0,{0}},{0,{0}},{0,{0}}},
	ADWorkPortSelect,
	StartMeasure,
	GetAdcValue,
	DMAStop,
	InitAdc,
};

static void ADC_3_WorkPortSelect(UCHAR ChanelID)
{
	extern ADC_HandleTypeDef  ADC_3_Handle;
	BSP_AdcChannelSelect(&ADC_3_Handle, ChanelID); 
}
static void ADC_3_StartMeasure(UINT32* pValueBuffer,UINT32 Num)
{
	extern ADC_HandleTypeDef  ADC_3_Handle;
	BSP_AdcStart(&ADC_3_Handle, (uint32_t*)pValueBuffer,(uint32_t)Num);
}
static void ADC_3_DMAStop(void)
{
	extern ADC_HandleTypeDef  ADC_3_Handle;
	BSP_AdcStop(&ADC_3_Handle);
}
static UCHAR ADC_3_Get_Value(UCHAR ChanelID,UINT16*pValueBuffer)
{
	UCHAR i, j, k;
	UCHAR u16RTN = FALSE;
	double f64sum;    
	UINT32 u32AdcMax[mSLOW_DISCARD_NUM][2];
	UINT32 u32AdcMin[mSLOW_DISCARD_NUM][2];
	f64sum = 0;

	//TXY-AD
	if (ChanelID >= ADC_ALL_CHANAL_NUM)
	{
		u16RTN = FALSE;//防止越界！定义数组加3也是防止越界。
	}
	else 
	{

		if(dv_ADC_3.stSlowChannelData[0].ucGetDataNum< mSLOW_ALL_POINT_NUM)
		{
			u16RTN = FALSE;
		}
		else
		{
			/*初始化*/
			for(i = 0; i < mSLOW_DISCARD_NUM; i++)
			{
				u32AdcMax[i][0] = 0x0000;
				u32AdcMin[i][0] = 0xffff;
				u32AdcMax[i][1] = 0xffff;
				u32AdcMin[i][1] = 0xffff;
			}
			for(i = 0; i < mSLOW_DISCARD_NUM; i++)
			{
				for(j = 0; j< dv_ADC_3.stSlowChannelData[0].ucGetDataNum; j++)
				{
					if(i == 0)
					{
						f64sum += dv_ADC_3.stSlowChannelData[0].u32ADCValue[j];
					}
					for(k=0;k<i;k++)
					{
						if((u32AdcMax[k][1] == j)||(u32AdcMin[k][1] == j))
						{
							break;
						}
					}
					if(k ==i)
					{
						if(u32AdcMax[i][0] < dv_ADC_3.stSlowChannelData[0].u32ADCValue[j])
						{
							u32AdcMax[i][0] = dv_ADC_3.stSlowChannelData[0].u32ADCValue[j];
							u32AdcMax[i][1] = j;
						}
						if(u32AdcMin[i][0] > dv_ADC_3.stSlowChannelData[0].u32ADCValue[j])
						{
							u32AdcMin[i][0] = dv_ADC_3.stSlowChannelData[0].u32ADCValue[j];
							u32AdcMin[i][1] = j;
						}
					}
				}
			}

			for(i = 0; i < mSLOW_DISCARD_NUM; i++)
			{
				f64sum -= (u32AdcMax[i][0]+u32AdcMin[i][0]);
			}
			f64sum /= (dv_ADC_3.stSlowChannelData[0].ucGetDataNum- mSLOW_DISCARD_NUM*2); 
			*pValueBuffer  = (UINT16)f64sum;
			dv_ADC_3.stSlowChannelData[0].ucGetDataNum = 0;
			u16RTN = TRUE;

		}
	}
	return u16RTN;
}
static void InitAdc3(void)
{
	BSP_ADC3_Init();
}

dv_Adc_TypeDef dv_ADC_3 =
{
	0,//ucDMASampelTime
	0,  //ucStartMeasureFlag
	0,  // OneLoopFlag;
	0,  // ucSampleIntervalCount;
	{0},//u32OneLoopValue[mPOINT_NUM_EACH_TIME];
	{0},//u32FastChannelADCValue[ADC_FAST_CHANNEL_NUM];//快速采集，则用一次采集到底值进行计算
	{{0,{0}},{0,{0}},{0,{0}},{0,{0}},{0,{0}}},
	ADC_3_WorkPortSelect,
	ADC_3_StartMeasure,
	ADC_3_Get_Value,
	ADC_3_DMAStop,
	InitAdc3,
};


/************************************************/
/*函数功能:喂狗			                */
/************************************************/
static void FeedDog(void)
{
#ifdef WDOG_ENABLE
	BSP_WDGRefresh();
	BSP_vFeedDogLed();
#endif
}

dv_Dog_TypeDef  dv_Dog =
{
	FeedDog,
};
/************************************************/
/*函数功能:CANBus初始化*/
/************************************************/
static void CAN1BusInit(void)
{
	BSP_Can_1_Init(250,CAN_EXTEND);
}

/************************************************/
/*函数功能:CAN发送*/
/************************************************/
static void CAN1TxData(canmsg*p)
{
	//UINT32 u32temp;
	//UCHAR address;
	//dv_CAN1.fCAN1RxInit();//发之前要准备收
	//address = (UCHAR)((p->Id>>11)&0x000000FF);
	//u32temp = p->Id &0x1ff00000 ;   
	//u32temp |=(((UINT32)(address))<<3) & 0x7f8;
	//dv_CAN1.ucRxCheckInfo = u32temp;
	//BSP_CanTx(p,8);
	//dv_CAN1.ucRxReady = mTRUE;
	//dv_CAN1.ucGetFrameNum = 0;
	//dv_CAN1.ucRxFrameNum = 0;
}


/*函数功能:CAN发送*/
static void CAN1RxInit(void)
{
	BSP_Can1_RxEnable();
}

/*函数功能:CAN接收*/
static UINT8 CAN1ReadFrame(UINT8 FrameIdx,UINT8 NeedRxNum) 
{
	UINT8 RTN = 0;
        UNUSED(FrameIdx);
        UNUSED(NeedRxNum);
	return RTN;
}


dv_CAN_TypeDef dv_CAN1 =
{
	0,
	0,
	0,
	0,
	CAN1BusInit,
	CAN1TxData,
	CAN1RxInit,
	CAN1ReadFrame,
};


/*函数功能:设置RTC*/
#pragma optimize=no_cse
static void init_rtc(void)
{
    UCHAR temp;
    if(BSP_ReadRtcReg(&temp,SECONDSREG)== EEPROM_FAIL) /*秒,时间无效*/
    {
        gRTCData.fInitGMTTime();
    }
}

/*函数功能:获取RTC*/
static INT32 get_rtc_counter(void)
{
  return  dv_RTC.RTC_Counter;
}


/*函数功能:设置RTC*/
static void set_rtc_counter(INT32 data)
{
	struct tm  tmNow;

	dv_RTC.RTC_Counter = data;
	dv_RTC.RTC_Counter_Back = data;
	//write to RTC
	gmtime_r((const time_t *)(&dv_RTC.RTC_Counter), &tmNow);
	dv_RTC.fWriteRTC(SECONDSREG, HexToBCD(tmNow.tm_sec));			/*秒*/
	dv_RTC.fWriteRTC(MINUTESREG, HexToBCD(tmNow.tm_min));			/*分*/
	dv_RTC.fWriteRTC(HOURSREG, HexToBCD(tmNow.tm_hour));			/*时*/
	dv_RTC.fWriteRTC(DAYSREG, HexToBCD(tmNow.tm_mday));			/*日*/
	dv_RTC.fWriteRTC(WEEKDAYSREG, HexToRTCWeek(tmNow.tm_wday));/*星期*/
	dv_RTC.fWriteRTC(MONTHSREG, HexToBCD(tmNow.tm_mon + 1));			/*月*/
	dv_RTC.fWriteRTC(YEARSREG, HexToBCD(tmNow.tm_year + 1900 - 2000));	/*年*/
}


/*函数功能:读出RTC字节值*/
static UCHAR ReadRTC(UCHAR Address)
{
	UINT8 u8Idx = 0;
	UCHAR out,RTN;
    for(u8Idx = 0; u8Idx<3;u8Idx++)
    {
		if(dv_E2PROM.fIsE2PROMTaskRun())
		{
			MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
		}

		RTN = BSP_ReadRtcReg(&out,(UINT16)Address);
		if(dv_E2PROM.fIsE2PROMTaskRun())
		{
			MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
		}
		if (EEPROM_OK == RTN)
		{
			break;
		}
	}
	return  out;
}

/*函数功能:写入RTC字节值*/
static UCHAR	WriteRTC(UCHAR Address, UCHAR inChar)
{
	UCHAR  ret;
	UINT8 u8Idx = 0;
	
	for (u8Idx = 0; u8Idx < 3; u8Idx++)
	{
		if(dv_E2PROM.fIsE2PROMTaskRun())
		{
			MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
		}
		ret = BSP_WriteRtcReg(inChar,(UINT16)Address);
		
		if(dv_E2PROM.fIsE2PROMTaskRun())
		{
			MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
		}
		if (EEPROM_OK == ret)
		{
			break;
		}
	}
	
	if(ret == EEPROM_OK)  
	{
		return	mTRUE;
	}
	else
	{
		return mFALSE;
	}
}


/*函数功能:读取并返回RTC时间*/
#pragma optimize=no_cse
INT32 ReadTime_Try(void)
{
	unsigned char i;
	struct tm  tmNow;
	INT32 RTC_Counter;
	dv_E2PROM.fInitEEPROMI2CBus();
	tmNow.tm_sec = BCDToHex(dv_RTC.fReadRTC(SECONDSREG) & 0x7f);		/*秒*/
	tmNow.tm_min = BCDToHex(dv_RTC.fReadRTC(MINUTESREG) & 0x7f);		/*分*/
	tmNow.tm_hour = BCDToHex(dv_RTC.fReadRTC(HOURSREG) & 0x3f);		/*时*/
	tmNow.tm_mday = BCDToHex(dv_RTC.fReadRTC(DAYSREG) & 0x3f);		/*日*/
	tmNow.tm_wday = RTCWeekToHex(dv_RTC.fReadRTC(WEEKDAYSREG) & 0x7f);	/*星期*/
	tmNow.tm_mon = BCDToHex(dv_RTC.fReadRTC(MONTHSREG) & 0x1f) - 1;	/*月*/
	tmNow.tm_year = BCDToHex(dv_RTC.fReadRTC(YEARSREG)) + 2000 - 1900;/*年*/

	tmNow.tm_yday = 0;
	tmNow.tm_isdst = 0;

	RTC_Counter = mktime_r(&tmNow);

	//若时间读取错误，则增加2次读取
	for(i = 0; i < 2; i++)
	{
		if((RTC_Counter == -1) 
			||(RTC_Counter > GMT_TIME_MAX) 
			||(RTC_Counter < GMT_TIME_MIN))
		{
			UDELAY(100);
			
			dv_E2PROM.fInitEEPROMI2CBus();
			tmNow.tm_sec = BCDToHex(dv_RTC.fReadRTC(SECONDSREG) & 0x7f);		/*秒*/
			tmNow.tm_min = BCDToHex(dv_RTC.fReadRTC(MINUTESREG) & 0x7f);		/*分*/
			tmNow.tm_hour = BCDToHex(dv_RTC.fReadRTC(HOURSREG) & 0x3f);		/*时*/
			tmNow.tm_mday = BCDToHex(dv_RTC.fReadRTC(DAYSREG) & 0x3f);		/*日*/
			tmNow.tm_wday = RTCWeekToHex(dv_RTC.fReadRTC(WEEKDAYSREG) & 0x07);	/*星期*/
			tmNow.tm_mon = BCDToHex(dv_RTC.fReadRTC(MONTHSREG) & 0x1f) - 1;	/*月*/
			tmNow.tm_year = BCDToHex(dv_RTC.fReadRTC(YEARSREG)) + 2000 - 1900;/*年*/
			tmNow.tm_yday = 0;
			tmNow.tm_isdst = 0;
			RTC_Counter = mktime_r(&tmNow);
		}
		else
		{
			break;
		}
	}

	
	return RTC_Counter;
}

/*函数功能:读取RTC时间*/
/************************************************/
#pragma optimize=no_cse
static void	ReadTime(unsigned char IsInitFlag)
{
        unsigned char i;
	static INT32 RTC_Counter_bak = 0;
	INT32 dv_RTC_RTC_Counter;

	dv_E2PROM.fInitEEPROMI2CBus();

	dv_RTC.fReadRTC(SECONDSREG);
	
	dv_RTC.tmNow.tm_sec = BCDToHex(dv_RTC.fReadRTC(SECONDSREG) & 0x7f);		/*秒*/
	dv_RTC.tmNow.tm_min = BCDToHex(dv_RTC.fReadRTC(MINUTESREG) & 0x7f);		/*分*/
	dv_RTC.tmNow.tm_hour = BCDToHex(dv_RTC.fReadRTC(HOURSREG) & 0x3f);		/*时*/
	dv_RTC.tmNow.tm_mday = BCDToHex(dv_RTC.fReadRTC(DAYSREG) & 0x3f);		/*日*/
	dv_RTC.tmNow.tm_wday = RTCWeekToHex(dv_RTC.fReadRTC(WEEKDAYSREG) & 0x07);	/*星期*/
	dv_RTC.tmNow.tm_mon = BCDToHex(dv_RTC.fReadRTC(MONTHSREG) & 0x1f) - 1;	/*月*/
	dv_RTC.tmNow.tm_year = BCDToHex(dv_RTC.fReadRTC(YEARSREG)) + 2000 - 1900;/*年*/

	dv_RTC.tmNow.tm_yday = 0;
	dv_RTC.tmNow.tm_isdst = 0;

	dv_RTC_RTC_Counter = mktime_r(&dv_RTC.tmNow);

	//初始化时间
	if(IsInitFlag)
	{
		//读取错误，则增加2次读取
		for(i = 0; i < 2; i++)
		{
			if((dv_RTC_RTC_Counter == -1) 
				||(dv_RTC_RTC_Counter > GMT_TIME_MAX) 
				||(dv_RTC_RTC_Counter < GMT_TIME_MIN))
			{
				UDELAY(100);
				dv_E2PROM.fInitEEPROMI2CBus();
				dv_RTC.tmNow.tm_sec = BCDToHex(dv_RTC.fReadRTC(SECONDSREG) & 0x7f);		/*秒*/
				dv_RTC.tmNow.tm_min = BCDToHex(dv_RTC.fReadRTC(MINUTESREG) & 0x7f);		/*分*/
				dv_RTC.tmNow.tm_hour = BCDToHex(dv_RTC.fReadRTC(HOURSREG) & 0x3f);		/*时*/
				dv_RTC.tmNow.tm_mday = BCDToHex(dv_RTC.fReadRTC(DAYSREG) & 0x3f);		/*日*/
				dv_RTC.tmNow.tm_wday = RTCWeekToHex(dv_RTC.fReadRTC(WEEKDAYSREG) & 0x07);	/*星期*/
				dv_RTC.tmNow.tm_mon = BCDToHex(dv_RTC.fReadRTC(MONTHSREG) & 0x1f) - 1;	/*月*/
				dv_RTC.tmNow.tm_year = BCDToHex(dv_RTC.fReadRTC(YEARSREG)) + 2000 - 1900;/*年*/
				dv_RTC.tmNow.tm_yday = 0;
				dv_RTC.tmNow.tm_isdst = 0;
				dv_RTC_RTC_Counter = mktime_r(&dv_RTC.tmNow);
			}
			else
			{
				break;
			}
		}

		//时间读取出错,采取恢复措施
		if((dv_RTC_RTC_Counter > GMT_TIME_MAX) || (dv_RTC_RTC_Counter < GMT_TIME_MIN) || (dv_RTC_RTC_Counter == -1))
		{
			//备份时间不符合要求，恢复缺省值
			if((RTC_Counter_bak > GMT_TIME_MAX) 
				|| (RTC_Counter_bak < GMT_TIME_MIN) 
				|| (RTC_Counter_bak == -1))
			{			
				dv_RTC_RTC_Counter = GMT_TIME_MIN;
				RTC_Counter_bak = GMT_TIME_MIN;			
			}
			//备份时间满足要求，恢复备份时间
			else
			{
				dv_RTC_RTC_Counter = RTC_Counter_bak;
			}
			//write to RTC
			gmtime_r((const time_t *)(&dv_RTC_RTC_Counter), &dv_RTC.tmNow);
			dv_RTC.fWriteRTC(SECONDSREG, HexToBCD(dv_RTC.tmNow.tm_sec));			/*秒*/
			dv_RTC.fWriteRTC(MINUTESREG, HexToBCD(dv_RTC.tmNow.tm_min));			/*分*/
			dv_RTC.fWriteRTC(HOURSREG, HexToBCD(dv_RTC.tmNow.tm_hour));			/*时*/
			dv_RTC.fWriteRTC(DAYSREG, HexToBCD(dv_RTC.tmNow.tm_mday));			/*日*/
			dv_RTC.fWriteRTC(WEEKDAYSREG, HexToRTCWeek(dv_RTC.tmNow.tm_wday));/*星期*/
			dv_RTC.fWriteRTC(MONTHSREG, HexToBCD(dv_RTC.tmNow.tm_mon + 1));			/*月*/
			dv_RTC.fWriteRTC(YEARSREG, HexToBCD(dv_RTC.tmNow.tm_year + 1900 - 2000));	/*年*/
		}
		//时间读取成功,备份时间
		else
		{
			RTC_Counter_bak = dv_RTC_RTC_Counter;
		}

		dv_RTC.RTC_Counter = dv_RTC_RTC_Counter;
		dv_RTC.RTC_Counter_Back = dv_RTC_RTC_Counter;
	}
	//不是初始化阶段，需要判断有效性
	else
	{		
		if((dv_RTC_RTC_Counter == -1) 
			||(dv_RTC_RTC_Counter > GMT_TIME_MAX) 
			||(dv_RTC_RTC_Counter < GMT_TIME_MIN))
		{
			//不正确，直接跳过	
		}
		else if (ABS(dv_RTC_RTC_Counter - dv_RTC.RTC_Counter_Back) < (60*60*2)) //误差2小时，认为正确?
		{
			dv_RTC.RTC_Counter = dv_RTC_RTC_Counter;
			dv_RTC.RTC_Counter_Back = dv_RTC_RTC_Counter;
			gRTCData.ucSecondChangeFlag = mFALSE;
		}
	}
}

dv_RTC_TypeDef dv_RTC =
{
	{0},
	0,//RTC_Counter
	0,//RTC_Counter_Back
	init_rtc,
	get_rtc_counter,
	set_rtc_counter,

	ReadRTC,
	WriteRTC,
	ReadTime,
};


/*函数功能:打开蜂鸣器*/
static void open_beep(void)
{
   BSP_vBuzzer_On();
}

/*函数功能:关闭蜂鸣器*/
static void close_beep(void)
{
    BSP_vBuzzer_Off();
}


static UCHAR init_mutex_relay_ctl(void)
{
  dv_Output.hMutexReadWriteRelayData = MutexCreate(UPPER_PRIO_READ_WRITE_RELAY_DATA);
  if(dv_Output.hMutexReadWriteRelayData == NULL)
  {
    return FALSE;
  }
	return  TRUE;
}

dv_Output_TypeDef dv_Output =
{
	NULL,	//hMutexReadWriteRelayData
	init_mutex_relay_ctl,
	open_beep,
	close_beep,
};


/*函数功能:初始化Mutex*/
static UCHAR init_sem_dv_uart6(void)
{
  dv_Uart6.hSemWriteUart6Data = (HANDLE)OSSemCreate(1);
  if(dv_Uart6.hSemWriteUart6Data == NULL)
  {
    return FALSE;
  }
	return  TRUE;
}

/*函数功能:设置波特率*/
static void SetUart6BaudRate(void)
{
	UCHAR  SetBaudRate;
	//GetCharSetData(SETDATA_BaudRate_NUM, &SetBaudRate);
	SetBaudRate = m38400	;
	BSP_Uart6Init(SetBaudRate);
}


static UCHAR	read_modemdcd(void)
{
	return BSP_GetUart6_CTS();
}

static void	write_modemdtr(UCHAR data)
{
	BSP_Uart6_RTS_Ctrl((GPIO_PinState)data);
}

/*函数功能:接收前要进行状态重置        		    */
//static void ResetUrat6State(void)
//{
//  //UCHAR i,RTN;
//  //for(i=0;i<3;i++)
//  //{
//  //      RTN = BSP_Uart6Rx(dv_Uart6_RxBuffer,UART_RX_BUFFER_SIZE);
//  //      if(RTN == HAL_OK)
//  //        break;
//  //}
//}
//TXY
static void Uart6EnableRx(void)
{
	UCHAR i;
	for(i=0; i<3; i++)
	{
		BSP_Uart6RxEnable();
	}
}
static void Uart6_send_data(unsigned char* pSendData, unsigned short iSize)
{
	//HAL_UART_6_Transmit(pSendData, iSize, 100);
	extern UART_HandleTypeDef Uart6Handle;
	HAL_UART_Transmit_IT(&Uart6Handle, pSendData, iSize);
}


dv_Uart6_TypeDef dv_Uart6 =
{
	NULL,					//hMutexWriteUart6Data
	0,						//pData
	0,						//pBuff
	0,						//u8TimeOutFlag;
	0,						//u8TimeOutCount;
	0,						//Head;
	0,						//uRecvFinish
	0,						//SendLastDelay
	init_sem_dv_uart6,
	SetUart6BaudRate,		//fSetUart6BaudRate
	Uart6EnableRx,			//ResetUrat6State,
	Uart6_send_data,
};

/***********************************************
函数名称:Uart4_send_data
函数功能:发送数据
***********************************************/
static void Uart4_send_data(unsigned char* pSendData, unsigned short iSize)
{
	
	dv_Uart4.fRS485CTL(mTransmit);				//收发引脚转换
	UDELAY(5);
	dv_Uart4.pData = 0;
	HAL_UART_4_Transmit(pSendData,iSize,4000);
	UDELAY(20);
	UDELAY(20);
	dv_Uart4.fRS485CTL(mReceive);				//收发引脚转换
	dv_Uart4.fRS485CTL(mReceive);				//收发引脚转换
}
/***********************************************
函数名称:Uart4_send_data  	   						         
函数功能:发送数据 							            
入口参数:无									                 
出口参数:无									                  
资源:				    						                  
子函数:		   								                  
更改日期:2004.5.8      						          
***********************************************/
static void Uart5_send_data(unsigned char* pSendData, unsigned short iSize)
{
	dv_Uart5.fRS485CTL(mTransmit);				//收发引脚转换
	dv_Uart5.fRS485CTL(mTransmit);				//收发引脚转换
	UDELAY(5);
	dv_Uart5.pData = 0;
	dv_Uart5.ucFrameFinishFlag = FALSE;
	HAL_UART_5_Transmit(pSendData, iSize, 4000);
	UDELAY(20);
	dv_Uart5.fRS485CTL(mReceive);
	dv_Uart5.fRS485CTL(mReceive);
}

/*函数功能:Uart4 485 ctrl*/
static void Uart4_RS485CTL(UCHAR state)
{
	if(state == mTransmit)
		BSP_Uart4_RS485_1_Ctrl(GPIO_PIN_SET);
	else
		BSP_Uart4_RS485_1_Ctrl(GPIO_PIN_RESET);
}

static void ResetUrat4State(void)
{
}

static void InitUart4(UCHAR BaudRateCase, UCHAR ParityCase)
{
	UNUSED(BaudRateCase);
	UNUSED(ParityCase);
	BSP_Uart4Init(BaudRateCase, ParityCase); 
}


//TXY
static void Uart4EnableRx(void)
{
	UCHAR i;
	for(i=0; i<3; i++)
	{
		BSP_Uart4RxEnable();
	}
}

//TXY	
dv_Uart4_TypeDef dv_Uart4 =
{
	0,	//volatile UCHAR  pData;
	0,	//UCHAR  ReceiveFlag;
	0, //  ucFrameFinishFlag;
	0,	//UCHAR  SendLastDelay; //最后一个字节后的delay
	NULL,	//hSemWriteUart4Data
	Uart4_RS485CTL,
	ResetUrat4State,
	InitUart4,
	Uart4EnableRx,
	Uart4_send_data,
};
static void DO1CtrlFunction(UCHAR state)
{
	if(state == TRUE)
		BSP_Realy1_Ctrl(GPIO_PIN_SET);
	else
		BSP_Realy1_Ctrl(GPIO_PIN_RESET);
}
static void DO2CtrlFunction(UCHAR state)
{
	if(state == TRUE)
		BSP_Realy2_Ctrl(GPIO_PIN_SET);
	else
		BSP_Realy2_Ctrl(GPIO_PIN_RESET);
}
dv_DO_TypeDef  dv_DO =
{
	DO1CtrlFunction,
	DO2CtrlFunction,
};

static void  InitUart1(void)
{
	BSP_Uart1Init(m19200); 
	//BSP_Uart1Init(m9600); 
}

static void Uart1_send_data(unsigned char* pSendData, unsigned short iSize)
{
	HAL_UART_1_Transmit(pSendData,iSize,100);
}

//TXY
static void Uart1EnableRx(void)
{
	UINT8 i = 0;
	for(i=0; i<3; i++)
	{
		BSP_Uart1RxEnable();
	}
}

dv_Uart1_TypeDef dv_Uart1 = 
{
	0,	//volatile UCHAR  pData;
	0,	//volatile UCHAR  ReceiveFlag;
	InitUart1,
	Uart1EnableRx,
	Uart1_send_data,

};

static void  InitUart2(void)
{
	BSP_Uart2Init(m9600); 
}

static void Uart2EnableRx(void)
{
	UINT8 i = 0;
	for(i=0; i<3; i++)
	{
		BSP_Uart2RxEnable();
	}
}
static void Uart2_send_data(unsigned char* pSendData, unsigned short iSize)
{
	HAL_UART_2_Transmit(pSendData,iSize,100);
}
dv_Uart2_TypeDef dv_Uart2 = 
{
	0,		//pUart2Data
	0,		//Uart2ReceiveFlag
	InitUart2,
	Uart2EnableRx,
	Uart2_send_data,
};




static void Uart5EnableRx(void)
{
	UCHAR i;
	for(i=0; i<3; i++)
	{
		BSP_Uart5RxEnable();
	}
}


/*函数功能:Uart5 485 ctrl*/
static void Uart5_RS485CTL(UCHAR state)
{
    if(state == mTransmit)
		BSP_Uart5_RS485_2_Ctrl(GPIO_PIN_SET);
    else
        BSP_Uart5_RS485_2_Ctrl(GPIO_PIN_RESET); 
}
/*函数功能:接收前要进行状态重置        		    */
static void ResetUrat5State(void)
{

}

/*函数功能:初始化串口*/
//static void  InitUart5(void)
static void  InitUart5(UCHAR BaudRateCase,UCHAR ParityCase)
{
	BSP_Uart5Init(BaudRateCase, ParityCase);
}

dv_Uart5_TypeDef dv_Uart5 =
{
	0,//volatile UCHAR  pData;
	0,//UCHAR  ReceiveFlag;
	0,//UCHAR  ucFrameFinishFlag; 
	0,//UCHAR  SendLastDelay; //最后一个字节后的delay
	NULL,//hSemWriteUart4Data
	Uart5_RS485CTL,
	ResetUrat5State,
	InitUart5,
	Uart5EnableRx,
	Uart5_send_data,
};

/*函数功能:初始化Mutex*/
static UCHAR init_mutex_dv_e2prom(void)
{
	dv_E2PROM.hMutexReadWriteE2PROMData = MutexCreate(UPPER_PRIO_READ_WRITE_E2PROM_DATA);
	if(dv_E2PROM.hMutexReadWriteE2PROMData == NULL)
	{
		return FALSE;
	}
	return  TRUE;
}

static void InitEEPROMI2CBus(void)
{
	if(dv_E2PROM.fIsE2PROMTaskRun())
	{
		MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
	}
	EEPROM_IO_Init();
	if(dv_E2PROM.fIsE2PROMTaskRun())
	{
		MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
	}
}
static UCHAR ReadE2PROM(UINT16 Address)
{
	UCHAR i,out;
	UINT32 RTN;
	for(i =0;i<3;i++)
	{
		if(dv_E2PROM.fIsE2PROMTaskRun())
		{
			MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
		}
	
		RTN = BSP_EEPROM_ReadBuffer(&out,Address, 1);

		if(dv_E2PROM.fIsE2PROMTaskRun())
		{
			MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
		}
		
		if(RTN == EEPROM_OK)
		{
			break;
		}
		Delay(500);
	}
	if(RTN != EEPROM_OK)  //20190108 lkf test
	{
		return 0xA1;
	}
	return  out;
}

/*函数名称:WriteE2PROM     		*/
/*函数功能:写入E2PROM字节值		*/
/*入口参数:UINT16 Address(地址)		*/
/*	       UCHAR inChar(字节)	*/
/*出口参数:I2C_Tx_Status                */
static UCHAR	WriteE2PROM(UINT16 Address, UCHAR inChar)
{
    UINT32 RTN;
    UCHAR i;
    for(i=0;i<3;i++)
    {
        if(dv_E2PROM.fIsE2PROMTaskRun())
        {
            MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
        }
      
        RTN = BSP_EEPROM_WriteBuffer(&inChar, Address, 1);

        if(dv_E2PROM.fIsE2PROMTaskRun())
        {
            MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
        }
        
        if(RTN == EEPROM_OK)
        {
            return mTRUE;
        }
        Delay(500);
    }
    return mFALSE;
}

/*************************************************/
/*函数功能:读出一串E2PROM字节值			 */
/*入口参数: UCHAR * pDataBuf读出的数据指针        */
/*          UINT16 StartAddress(起始地址)        */
/*	        UCHAR ReadLength(长度)	必须大于1  */
/*************************************************/
static UCHAR ReadE2PROMPage(UCHAR * pDataBuf, UINT16 StartAddress, UCHAR ReadLength)
{
    UCHAR i;
    UINT32 RTN;

    for(i =0;i<3;i++)
    {
        if(dv_E2PROM.fIsE2PROMTaskRun())
        {
            MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
        }

        RTN = BSP_EEPROM_ReadBuffer(pDataBuf,StartAddress,(UINT16)ReadLength);

        if(dv_E2PROM.fIsE2PROMTaskRun())
        {
            MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
        }
        if(RTN == EEPROM_OK)
        {
          break;
        }
        Delay(500);
    }

	return RTN;
}

/************************************************/
/*函数名称:WriteE2PROMPage     					        */
/*函数功能:写入一串E2PROM字节值					        */
/*入口参数:UINT16 StartAddress(起始地址)	      */
/*	       UCHAR WriteLength(长度)必须大于1     */
/*出口参数:I2C_Tx_Status				                */
/*资源: E2PROMPageRead                       		*/
/*子函数:                                 			*/
/*更改日期:2007.5.10       						          */
/************************************************/
static UCHAR	WriteE2PROMPage(UCHAR * pDataBuf, UINT16 StartAddress, UCHAR WriteLength)
{
	UINT32 RTN;
    UCHAR i;

	for(i =0;i<3;i++)
    {
        if(dv_E2PROM.fIsE2PROMTaskRun())
        {
            MutexLock(dv_E2PROM.hMutexReadWriteE2PROMData);
        }
        
        RTN = BSP_EEPROM_WriteBuffer(pDataBuf, StartAddress, WriteLength);

        if(dv_E2PROM.fIsE2PROMTaskRun())
        {
            MutexUnlock(dv_E2PROM.hMutexReadWriteE2PROMData);
        }
        
        if(RTN == EEPROM_OK)
        {
            return mTRUE;
        } 
        Delay(500);
    }
    return mFALSE;
}

//用于EEPROM和RTC互斥
static void SetE2PROMTaskRun(void)
{
	dv_E2PROM.TaskRun = TRUE;
}
//用于EEPROM和RTC互斥
static UCHAR IsE2PROMTaskRun(void)
{
	return  dv_E2PROM.TaskRun;
}

/*函数名称:ReadE2PROMFloat*/
static float ReadE2PROMFloat(UINT16 usID)
{
	UCHAR ucdata[4];
	float f32data;
	dv_E2PROM.fReadE2PROMPage(ucdata, E2PROMDEFAULTFloatData + usID * 4, 4);
	FourByte2float(ucdata, &f32data);
	return  f32data;
}

/*函数名称:WriteE2PROMFloat*/
static UINT8 WriteE2PROMFloat(UINT16 usID, float f32data)
{
	UINT8 ret;
	UCHAR ucdata[4];
	floattoascii(f32data, ucdata);
	ret = dv_E2PROM.fWriteE2PROMPage(ucdata, E2PROMDEFAULTFloatData + usID * 4, 4);
	return  ret;
}

/*函数名称:ReadE2PROMFloat*/
static float ReadE2PROMFloat_2(UINT16 usID)
{
	UCHAR ucdata[4];
	float f32data;
	dv_E2PROM.fReadE2PROMPage(ucdata, usID, 4);
	FourByte2float(ucdata, &f32data);
	return  f32data;
}

/*函数名称:WriteE2PROMFloat*/
static UINT8 WriteE2PROMFloat_2(UINT16 usID, float f32data)
{
	UINT8 ret;
	UCHAR ucdata[4];
	floattoascii(f32data, ucdata);
	ret = dv_E2PROM.fWriteE2PROMPage(ucdata, usID, 4);
	return  ret;
}

dv_E2PROM_TypeDef dv_E2PROM =
{
	NULL,//hMutexReadWriteE2PROMData
	FALSE,//TaskRun
	init_mutex_dv_e2prom,
	InitEEPROMI2CBus,
	SetE2PROMTaskRun,
	IsE2PROMTaskRun,
	ReadE2PROM,
	WriteE2PROM,
	ReadE2PROMPage,
	WriteE2PROMPage,
	ReadE2PROMFloat,
	WriteE2PROMFloat,

	ReadE2PROMFloat_2,
	WriteE2PROMFloat_2,
};

/*函数功能:初始化I2C总线*/
static void init_i2c3bus(void)
{
	//I2C3_Init();
}

static UCHAR	I2C3ReadData(UINT16 DevAddress,UINT16 DataAddress)
{
    UCHAR i,out;
    UINT32 RTN;
    for(i =0;i<3;i++)
    {
        RTN = BSP_ReadI2C3Reg(DevAddress,&out, DataAddress);

        if(RTN == EEPROM_OK)
        {
            break;
        }
        Delay(500);
    }
    return  out;
}

/*写入字节值*/
static UCHAR	I2C3WriteData(UINT16 DevAddress,UINT16 DataAddress, UCHAR inChar)
{
    UINT32 RTN;
    UCHAR i;
    for(i=0;i<3;i++)
    {
        RTN = BSP_WriteI2C3Reg(DevAddress,&inChar, DataAddress);
       
        if(RTN == EEPROM_OK)
        {
            return mTRUE;
        }
        Delay(500);
    }
    return mFALSE;
}

dv_I2C3_TypeDef dv_I2C3 =
{
	init_i2c3bus,
	I2C3ReadData,
	I2C3WriteData,
};

//利用定时器捕捉风扇脉冲！
static void GetInputPulseCount(void)
{
	UCHAR i ;
	UINT16 u16Count[ALL_CAPTURE_CHANNEL_NUM];
	dv_Capture_PWM.ucGetPeriodNum++;
	if (dv_Capture_PWM.ucGetPeriodNum >= PER_CYCL_TIME)
	{
		for(i = 0;i < ALL_CAPTURE_CHANNEL_NUM;i++)
		{
			u16Count[i] = dv_Capture_PWM.u16PulseCnt[i];
			if(u16Count[i] <dv_Capture_PWM.u16LastCount[i])
			{
				dv_Capture_PWM.u16FinalData[i] = u16Count[i]+0xFFFF-dv_Capture_PWM.u16LastCount[i];
			}
			else
			{
				dv_Capture_PWM.u16FinalData[i] = u16Count[i]-dv_Capture_PWM.u16LastCount[i];
			}
			dv_Capture_PWM.u16LastCount[i] = u16Count[i];
		}
		dv_Capture_PWM.ucGetPeriodNum = 0;
	}
	
}

static void OutputFanDriver(UCHAR FanNum,UCHAR rate)
{
	if((rate > gEvChargeInfo.u8FanMaxSpeed) && (FanNum == mFAN1))
	{
		rate = gEvChargeInfo.u8FanMaxSpeed;
	}

	if(FanNum == mFAN1)
	{
		BSP_SetPwm1Duty(rate);
        dv_Capture_PWM.Rate[mFAN1] = rate;
	}
	else if(FanNum == hPOWER_PWM)
	{
		BSP_SetPwm2Duty(rate);
        dv_Capture_PWM.Rate[hPOWER_PWM] = rate;
	}
}
/************************************************/
/*函数功能:读取测量值*/
/************************************************/
static UCHAR GetCaptureValue(UCHAR ChanelID,UINT16* pRTNData)
{
	UCHAR ucRTN;

	if (ChanelID >= ALL_CAPTURE_CHANNEL_NUM)
	{
		ucRTN = FALSE;//防止越界！定义数组加3也是防止越界。
	}
	else 
	{
		*pRTNData =  dv_Capture_PWM.u16FinalData[ChanelID];
		ucRTN = TRUE;
	}
	return ucRTN;
}
dv_Capture_PWM_TypeDef  dv_Capture_PWM =
{
	{0},
	{0},
	{0},
	{0},
	0,
	GetInputPulseCount,
	OutputFanDriver,
	GetCaptureValue,
};

unsigned int BasetimeCount = 0;


void BSP_Base_TIME_Deal(void)
{
	CAN_2_SendCallBack();
	CAN_1_SendCallBack();
}
/***********************************************
函数名称:USBProcess   			          
函数功能:              								       
入口参数: 									                 
出口参数: 									                 
资源:           					          
子函数:	         								             
更改日期:  						          
***********************************************/
/***********************************************
函数名称:USBProcess   			          
函数功能:              								       
入口参数: 									                 
出口参数: 									                 
资源:           					          
子函数:	         								             
更改日期:  						          
***********************************************/
static void USBProcess(void)
{
	BSP_USBH_Process();
}
static void USBInit(void)
{
	vUsbInit();
}

#define DEMO_INIT       ((uint8_t)0x00)
#define DEMO_IAP        ((uint8_t)0x01)
static uint8_t Demo_State = DEMO_INIT;
FATFS USBH_fatfs;
extern uint8_t MSC_File_Operations(UCHAR* pBuff,UINT16 length,UCHAR* pFileName);

static void USB_Write_Data(UCHAR* pBuff,UINT16 length,UCHAR* pFileName)
{
	UCHAR temp;
	switch(Demo_State)
	{
	case DEMO_INIT:
		if(f_mount(&USBH_fatfs, "", 0 ) == FR_OK )
		{
			Demo_State = DEMO_IAP;
		}
		break;

	case DEMO_IAP:
		if(USBH_MSC_IsReady(&hUSBHost))
		{  
			if(Appli_state == APPLICATION_READY)
			{
				temp = MSC_File_Operations(pBuff,length,pFileName);
				if(temp ==2)
					dv_USB.ucUSBFlag +=2;
				else
					dv_USB.ucUSBFlag =temp;
				dv_USB.ucLoadflag  = 0;
			}
		}
		else
		{
                        
			dv_USB.ucUSBFlag +=2;
			dv_USB.ucLoadflag  = 1;
		}
		break;

	default:
		break;
	}
	if(Appli_state == APPLICATION_DISCONNECT)
	{
            Demo_State = DEMO_INIT;
            OSTimeDly(100);
	}
}
static UCHAR Get_USB_State(void)
{
	return Appli_state;
}
dv_USB_TypeDef dv_USB =
{
	1,
	1,
	USBInit,
	USBProcess,  
	USB_Write_Data,
	Get_USB_State,
};