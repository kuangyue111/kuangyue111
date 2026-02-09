
#include  <main.h>
#include "EtherNet.h"

UINT8 u8DebugVal = 0;

static  OS_STK  STK_AppTask[APP_CFG_TASK_START_STK_SIZE];
//OS_STK	STK_DisplayTask[TASK_STK_SIZE_DISPLAY];
OS_STK	STK_UART4Task[TASK_STK_SIZE_UART4];
OS_STK	STK_HuaShangCtrlTask[TASK_STK_SIZE_UART1];
OS_STK	STK_E2PROMTask[TASK_STK_SIZE_EEPROM];
OS_STK	STK_PWMTask[TASK_STK_SIZE_PWM];

OS_STK	STK_CAN1Task[TASK_STK_SIZE_CAN1];
OS_STK	STK_ReadRTCTask[TASK_STK_SIZE_RTC];
OS_STK	STK_TestSelfTask[TASK_STK_SIZE_SELF_TEST];
OS_STK	STK_CAN3Task[TASK_STK_SIZE_CAN3];
OS_STK	STK_RingTask[TASK_STK_SIZE_RING];
OS_STK	STK_ModemTask[TASK_STK_SIZE_MODEM];
OS_STK	STK_HostTask[TASK_STK_SIZE_UART5];
OS_STK	STK_AlmTask[TASK_STK_SIZE_ALARM];
OS_STK	STK_MeasureTask[TASK_STK_SIZE_MEASURE];
OS_STK	STK_CAN2Task[TASK_STK_SIZE_CAN2];
OS_STK  STK_EthernetTask[TASK_STK_SIZE_ETHERNET];
OS_STK	STK_NetZhiXinTask[TASK_STK_NET_ZHIXIN];
OS_STK	STK_FlashTask[TASK_STK_SIZE_FLASH];
OS_STK	STK_UDiskTask[TASK_STK_SIZE_UDISK];
static  void  TASK_AppStartProc(void  *p_arg);
static void   TASK_Create(void);

CPU_INT32U  AppProbe_Slider;
float gADCC1Val = 0;
float gADCC2Val = 0;
float gADTestFVal1 = 0;
float gADVoltFVal1 = 0;
float gADTestFVal2 = 0;
float gTestFVal3 = 0;
uint32_t gTestEXID = 0;
UCHAR gOutPutSwitchState =0;
UCHAR gAllRectCtrlState =0;												//预留接口
UCHAR gFanFullCtrlState =0; 
UCHAR gContactorCtrlState =0;

__CHARGE_ARGS gTempChargeArgs;
NOINIT CHARGE_CTRL_INFO gChargeCtrl;
NOINIT EV_CHARGE_INFO gEvChargeInfo;
NOINIT __ZTE_FAULT_LOG gZTEFaultLogView[FAULT_LOG_VIEW_NUM + 2];
NOINIT __ST_ZTE_FAULT_LOG_RAM gZTEFaultLog[FAULT_LOG_RAM_NUM];				//中兴故障日志
//0x84d1   MAX_FAULT_LOG_NUM 存储这么多条
NOINIT __ZTE_FAULT_LOG_IDX gZTEFaultLogIdx;
__VIEW_RESULT_INFO gA_Result;
__VIEW_RESULT_INFO gB_Result;
TASK_XXXInfo sTaskArray[TASK_MAX_NUM] =
{
	{ &TASK_RTCProc,		TASK_STK_SIZE_RTC,				PRIO_TASK_START+1,  TASK_START_ID+1,	STK_ReadRTCTask},		//RTC
	{ &TASK_SelfTestProc,	TASK_STK_SIZE_SELF_TEST,	                PRIO_TASK_START+2,  TASK_START_ID+2,	STK_TestSelfTask},		//自检
	{ &TASK_CAN2Proc,		TASK_STK_SIZE_CAN2,				PRIO_TASK_START+3,   TASK_START_ID+3,	STK_CAN2Task},
	{ &TASK_CAN1Proc,		TASK_STK_SIZE_CAN1,				PRIO_TASK_START+4,  TASK_START_ID+4,	STK_CAN1Task},
	{ &TASK_Ethernet,		TASK_STK_SIZE_ETHERNET,			        PRIO_TASK_START+5,  TASK_START_ID+5,	STK_EthernetTask},	
	{ &TASK_Uart5Proc,		TASK_STK_SIZE_UART5,			        PRIO_TASK_START+6,   TASK_START_ID+6,	STK_HostTask},			//UART5 电表 等
	{ &TASK_Uart4Proc,		TASK_STK_SIZE_UART4,			        PRIO_TASK_START+7,  TASK_START_ID+7,	STK_UART4Task},			//UART4 2枪 电表
	{ &TASK_NET_ProtocolProc,	TASK_STK_NET_ZHIXIN,		                PRIO_TASK_START+8,   TASK_START_ID+8,	STK_NetZhiXinTask},		//智芯应用层
	{ &TASK_MeasureProc,    TASK_STK_SIZE_MEASURE,			        PRIO_TASK_START+9,  TASK_START_ID+9,	STK_MeasureTask},		//测量
	{ &TASK_CAN3_Proc,	TASK_STK_SIZE_CAN3,				PRIO_TASK_START+11,   TASK_START_ID+11,	STK_CAN3Task},
	{ &TASK_E2promProc,	TASK_STK_SIZE_EEPROM,			        PRIO_TASK_START+13, TASK_START_ID+13,	STK_E2PROMTask},			//E2PROM
	{ &TACK_Uart1Proc,	TASK_STK_SIZE_UART1,			        PRIO_TASK_START+14,  TASK_START_ID+14,	STK_HuaShangCtrlTask},		//UART1 刷卡机
	{ &TASK_Uart6Proc,	TASK_STK_SIZE_MODEM,			        PRIO_TASK_START+15,  TASK_START_ID+15,	STK_ModemTask},				//UART6 后台
	{ &TASK_AlmProc,	TASK_STK_SIZE_ALARM,			        PRIO_TASK_START+16, TASK_START_ID+16,	STK_AlmTask},				//告警处理
	{ &TASK_FlashProc,	TASK_STK_SIZE_FLASH,			        PRIO_TASK_START+17,  TASK_START_ID+17,	STK_FlashTask},				//Flash 
	{ &TAST_FanProc,	TASK_STK_SIZE_PWM,				PRIO_TASK_START+19,  TASK_START_ID+19,	STK_PWMTask},				//FAN
	{ &TASK_UDiskProc,	TASK_STK_SIZE_UDISK,			PRIO_TASK_START+18,  TASK_START_ID+18,	STK_UDiskTask},				//FAN								
};

/***********************************************************************************************************************
	增加线程	需要更改三个地方：
	1）TASK_MAX_NUM
	2）#define TASK_ID_UART4				13
	3）#define CHECKTASKNUM					14
*******************************************************************************************************************/
void InitChageInfo(void);
void InitParasSramVariables(void);
void InitEVInfo(void);

void FaultLogIdxRefresh(void)
{
		UINT8 ucValueH = dv_E2PROM.fReadE2PROM(FAULT_LOG_NUM_ADDR_H);
		UINT8 ucValueL = dv_E2PROM.fReadE2PROM(FAULT_LOG_NUM_ADDR_L);
		gZTEFaultLogIdx.u16TotalLOGNum = (ucValueH<<8) + ucValueL;
		ucValueH = dv_E2PROM.fReadE2PROM(FAULT_LOG_PST_ADDR_H);
		ucValueL = dv_E2PROM.fReadE2PROM(FAULT_LOG_PST_ADDR_L);
		gZTEFaultLogIdx.u16BytesPst = (ucValueH<<8) + ucValueL;
		return;
}



void InitParasSramVariables(void)
{
	gEvChargeInfo.u32_B_CTS_DB_T1 = 140;
	gEvChargeInfo.u32_B_DB_DB_T2 = 140;
	gEvChargeInfo.u16SetBCLTimeOutVal_1 = 901;
	gEvChargeInfo.u16SetBCLTimeOutVal_2 = 902;
	gEvChargeInfo.u16SetBRMTimeOutVal_1 = 1603;
	gEvChargeInfo.u16SetBRMTimeOutVal_2 = 1601;
	gEvChargeInfo.u16SetBCSTimeOutVal_1 = 1601;
	gEvChargeInfo.u16SetBCSTimeOutVal_2 = 1603;
	gEvChargeInfo.fSetLimitMaxOutCurr_1 = 241;
	gEvChargeInfo.fSetLimitMaxOutCurr_2 = 241;
	gEvChargeInfo.fMaxOutCurr = 240;
	gEvChargeInfo.fMaxOutVolt = 750;
	gEvChargeInfo.fMinOutVolt = 50;
	gEvChargeInfo.fBattVolt_1CoefA = 1;
	gEvChargeInfo.fBattVolt_1CoefB = 0;

	gEvChargeInfo.fBattVolt_2CoefA = 1;
	gEvChargeInfo.fBattVolt_2CoefB = 0;

	gEvChargeInfo.fCC1_1Coef_A = 1;
	gEvChargeInfo.fCC1_1Coef_B = 0;
	gEvChargeInfo.fCC1_2Coef_A = 1;
	gEvChargeInfo.fCC1_2Coef_B = 0;
	gEvChargeInfo.fSysSetOverVolt = 770;

	gEvChargeInfo.u8PWMPolarityCover = FALSE;

	gEvChargeInfo.fRectLoadStartTime = 4;
	gEvChargeInfo.u8_A_LOCKType = 0;								//电子枪控制为 广州知崇新能源 电子枪，给电平吸合，不给电平断开。 0 新贵电子枪，需要方向
	gEvChargeInfo.u8EV_StartTemp = 40;
	gEvChargeInfo.u8EV_StopTemp = 50;
	
	gEvChargeInfo.u32PowerOnTimes = 0;

	int i = 0;
	for ( i = 0; i < EV_STATION_CODE_NUM; i++)
	{
		gEvChargeInfo.u8EVCode[i] = 0;
	}

	//for (i = 0; i < EV_OFF_LINE_PSWD; i++)
	//{
	//	gEvChargeInfo.u16OffLinePSWD[i] = 0;
	//}

	gEvChargeInfo.u32NET_PORT = 4460;
	gEvChargeInfo.u32NET_PORT2 = 9800;
	gEvChargeInfo.u8CommMode = 1;			//0应答模式  1上报模式

	/*环网*/
	gEvChargeInfo.bySelfAddress = 0; //本桩地址/**/
	gEvChargeInfo.nTotalRingStub = 0;//所有环网桩数/**/
	gEvChargeInfo.u8IsRingEnabled = 0;
	/*环网 end*/

	gEvChargeInfo.u8IsSingleGunMode = 0;

	for ( i = 0; i < 128; i++)
	{
		gEvChargeInfo.u8GunASetQrCode[i] = 0;
		gEvChargeInfo.u8GunBSetQrCode[i] = 0;
	}
}

void InitChageInfo(void)
{					
	UINT8 i;

	gEvChargeInfo.u8TEST_MODE_FLAG = FALSE;
	gEvChargeInfo.u8RcvYdn23Flag = FALSE;

	gEvChargeInfo.u8CTRLEx2DOCMD = 0;
	gEvChargeInfo.u8CTRLEx2DOCMDFlag = FALSE;
	gEvChargeInfo.u8CTRLInterDOCMDFlag_2 = FALSE;
	gEvChargeInfo.u8CTRLEx1DOCMDFlag = FALSE;
	gEvChargeInfo.u8CTRLEx1DOCMD = 0;
	gEvChargeInfo.u8CTRLInterDOCMD = 0;
	gEvChargeInfo.u8CTRLInterDOCMDFlag = FALSE;
	gEvChargeInfo.u8CTRLInterDOCMD_B = 0;
	gEvChargeInfo.u8CTRLInterDOCMDFlag_B = FALSE;


	gEvChargeInfo.u8EV_InterDI_COM = 0;
	gEvChargeInfo.u8EV_Ex1DI = 0;
	gEvChargeInfo.u8EV_Ex2DI = 0;
	gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] = SPEAR_NO_ON_LINE;							//0 不在位
	gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] = SPEAR_NO_ON_LINE;							//0 不在位
	gEvChargeInfo.u8EV_InterDO = 0;												//0
	gEvChargeInfo.u8EV_DO9_16  = 0;
	gEvChargeInfo.u16EV_Ex2DO = 0;
	gEvChargeInfo.fEV_InterTemp = 25.0;
	gEvChargeInfo.fEV_Ex2Temp = 25.0;
	gEvChargeInfo.fEV_BattVolt_1 = 699;
	gEvChargeInfo.fEV_BattVolt_2 = 688;
	
	gEvChargeInfo.fBMS_ChargeTime[GUN_NO_A] = 0;
	gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B] = 0;

	gEvChargeInfo.u8Gun_Alm[GUN_NO_A] = 0;
	gEvChargeInfo.u8Gun_Alm[GUN_NO_B] = 0;
	gEvChargeInfo.u8SysAlm = FALSE;
	gEvChargeInfo.u8GunSelect = 0;
	gEvChargeInfo.u8GunSelectTimeOut = 0;

	gEvChargeInfo.u8NetCommAlm = TRUE;
	gEvChargeInfo.u8TelaidianHostCommAlm = FALSE;

	gEvChargeInfo.u8FloatParamUpDataCount = 0;
	gEvChargeInfo.u8CharParamUpDataCount = 0;
	gEvChargeInfo.u8CC1_1Set_A_Flag = FALSE;
	gEvChargeInfo.u8CC1_1Set_B_Flag = FALSE;
	gEvChargeInfo.u8CC1_2Set_A_Flag = FALSE;
	gEvChargeInfo.u8CC1_2Set_B_Flag = FALSE;
	gEvChargeInfo.u8CC1_CH_No = 0;

	gEvChargeInfo.u8NET_Mask = FALSE;
	gEvChargeInfo.u8NET2_Enable = FALSE;

	
	gEvChargeInfo.u8Batt_1VoltSet_A_Flag = FALSE;
	gEvChargeInfo.u8Batt_1VoltSet_B_Flag = FALSE;
	
	
	gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] = STOP_CHARGE_VAL;
	gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] = STOP_CHARGE_VAL;
	gEvChargeInfo.u8IsOptionA = FALSE;
	gEvChargeInfo.u8IsOptionB = FALSE;
	gEvChargeInfo.A_IsParallel = FALSE;
	gEvChargeInfo.B_IsParallel = FALSE;

	
	
	gEvChargeInfo.u8CLRFaultLog = FALSE;
	gEvChargeInfo.fBattSet_A_Volt = 0;
	gEvChargeInfo.fBattSet_B_Volt = 0;
	gEvChargeInfo.u8CC1_CH_No = 0;

	gEvChargeInfo.u8A_NetStartSem = 0;
	gEvChargeInfo.u8B_NetStartSem = 0;

	gEvChargeInfo.fOutEnergyByCal[0] = 0;
	gEvChargeInfo.fOutEnergyByCal[1] = 0;

	gEvChargeInfo.ucPreChgOkFlag[0] = 0;
	gEvChargeInfo.ucPreChgOkFlag[1] = 0;

	//gEvChargeInfo.fOutEnergyByCalc = 0;						//累计计算充入电量
	gEvChargeInfo.u8SPDAlm = FALSE;							//交流防雷器告警
	gEvChargeInfo.u8EPOAlm = FALSE;
	gEvChargeInfo.u8DoorAlm = FALSE;						//门禁告警  打开就不能充电，并且告警。 NC
	gEvChargeInfo.u8ElecLock_AAlm = FALSE;					//A 电子锁告警
	gEvChargeInfo.u8ElecLock_BAlm = FALSE;					//B 电子锁告警

	gEvChargeInfo.u8Polarity_A_Alm = FALSE;					//A 极性告警
	gEvChargeInfo.u8Polarity_B_Alm = FALSE;					//B 极性告警

	gEvChargeInfo.fEV_InterTemp = 25;						//IMCU1外接温度

	gEvChargeInfo.u8SysFanALM = 0;							//系统风扇告警！
	gEvChargeInfo.u8SysFanSpeed = 0;						//系统风扇转速
	gEvChargeInfo.u8BrushSem = FALSE;
	gEvChargeInfo.u8A_NoCardStopSem = FALSE;
	gEvChargeInfo.u8B_NoCardStopSem = FALSE;

	gEvChargeInfo.u8TestMode[GUN_NO_A] = FALSE;
	gEvChargeInfo.u8TestMode[GUN_NO_B] = FALSE;
	gEvChargeInfo.u8_AdjustResult[GUN_NO_A] = 0;
	gEvChargeInfo.u8_AdjustResult[GUN_NO_B] = 0;
	

	gTempChargeArgs.u8ChargeMode = BMS_AUTO;
	gTempChargeArgs.fSOCArgs = 0;
	gTempChargeArgs.fTimeArgs = 0;
	gTempChargeArgs.fEnergeArgs = 0;
	gEvChargeInfo.u8ChargeMode[GUN_NO_A] = BMS_AUTO;
	gEvChargeInfo.u8ChargeMode[GUN_NO_B] = BMS_AUTO;
	gEvChargeInfo.u8ChargeMode[GUN_NO_B] = 0;
	gEvChargeInfo.ParallelMode = PARALLEL_COME_ON_MODE;//PARALLEL_STAND_MODE;
	gEvChargeInfo.RectOwner[GUN_NO_A] = GRP_A;
	gEvChargeInfo.RectOwner[GUN_NO_B] = GRP_B;
	gEvChargeInfo.i32_ParallSem = 0;
	
	
	gEvChargeInfo.u8PSWD_0x70_Flag = FALSE;

	gEvChargeInfo.u8_AutoTestStep = 0;
	gEvChargeInfo.u8_ManualTestStep = 0;

	gEvChargeInfo.ucTimePhase[0] = 0;
	gEvChargeInfo.ucTimePhase[1] = 0;
	gEvChargeInfo.ucTimePhase_InitFlag[0] = TRUE;
	gEvChargeInfo.ucTimePhase_InitFlag[1] = TRUE;
	
	gEvChargeInfo.ucRingCostDelay[0] = 0;
	gEvChargeInfo.ucRingCostDelay[1] = 0;

	gEvChargeInfo.u8GunOverTemAlm[GUN_NO_A] = 0;
	gEvChargeInfo.u8GunOverTemAlm[GUN_NO_B] = 0;
	gEvChargeInfo.u16GunOverTemLmtCurrDelay[GUN_NO_A] = 0;
	gEvChargeInfo.u16GunOverTemLmtCurrDelay[GUN_NO_B] = 0;
	gEvChargeInfo.u8GunOverTemLmtCurrStep[GUN_NO_A] = 0;
	gEvChargeInfo.u8GunOverTemLmtCurrStep[GUN_NO_B] = 0;


	//{4, "865748"},
	gEvChargeInfo.u16UserPassword[3][0] = '8';
	gEvChargeInfo.u16UserPassword[3][1] = '6';
	gEvChargeInfo.u16UserPassword[3][2] = '5';
	gEvChargeInfo.u16UserPassword[3][3] = '7';
	gEvChargeInfo.u16UserPassword[3][4] = '4';
	gEvChargeInfo.u16UserPassword[3][5] = '8';

	for(i = 0; i < ALL_GUN_NUM; i++)
	{
		gEvChargeInfo.fBattOverVoltValue[i][0] = 0;
		gEvChargeInfo.fBattOverVoltValue[i][1] = 0;
		gEvChargeInfo.fBattOverVoltValue[i][2] = 0;

		gEvChargeInfo.u8Is_PUTIAN_V15[i] = 0;

		gEvChargeInfo.u16GunNoCurrEndDelay[i] = 0;
	}

	for(i = 0; i < DEBUG_SIG_MAX; i++)
	{
		gEvChargeInfo.u8DebugSig[i] = 0;
	}

	gEvChargeInfo.u8VinWaitHostConfirmTimeout = 0;
	gEvChargeInfo.u8VinVerifyStat = 0;
	gEvChargeInfo.u8VinVerifyCtrl[0] = 0;
	gEvChargeInfo.u8VinVerifyCtrl[1] = 0;
	gEvChargeInfo.u8VinNetVerifyInterval = 0;
	for(i = 0; i < 17; i++)
	{
		gEvChargeInfo.u8VinCode[i] = 0;
	}
	gEvChargeInfo.u8InsulationAlm = FALSE;
	gEvChargeInfo.u8AdjustHall2Flag = FALSE;
	gEvChargeInfo.u8AdjustHall2Flag_2 = 0;
	gEvChargeInfo.u8AdjustHall2Result = 0;

	gEvChargeInfo.u16ACInputContactorDisconCnt = 0;
	gEvChargeInfo.u8ACInputContactorCtrlStat = DO_ON;
	gEvChargeInfo.u8ACInputContactorCloseSampDelay = ACInputContactorCloseSampDelay;
	gEvChargeInfo.u8ACInputContactorCloseAlmDelayFlag = TRUE;
	gEvChargeInfo.u8DoorOpenDisconACInput_Flag = 0;
	gEvChargeInfo.u8DoorOpenDisconACInput_Delay = 0;
    gEvChargeInfo.ucVINStartFlag[0] = FALSE;
    gEvChargeInfo.ucVINStartFlag[1] = FALSE;
	gEvChargeInfo.u8UserSelectBMSPowerType[0] = 0;
	gEvChargeInfo.u8UserSelectBMSPowerType[1] = 0;
	gEvChargeInfo.u8DisplayBMSPowerType[0] = 0;
	gEvChargeInfo.u8DisplayBMSPowerType[1] = 0;

}



void InitEVInfo(void)
{
	InitChageInfo();

	__PARALLEL_OFF();
	__PARALLEL_OFF();
	__PARALLEL_OFF();
	__STOP_CHARGE_CMD();											//开机容错  停止充电！




							//目的是否 是用于 WAIT ARP 总线数据！
	
	gZTEFaultLogIdx.u16TotalLOGNum = 0;
	gZTEFaultLogIdx.u16BytesPst = 0;
	//FaultLogIdxRefresh();

	gZTEFaultLogIdx.u16ViewLogNumPerFive = 1;
	
	dv_Output.fOpenBeep();
	OSTimeDly(500);
	dv_Output.fCloseBeep();
	gEvChargeInfo.i32_A_CreateRecordDelay = INVALID_DELAY;
	gEvChargeInfo.i32_B_CreateRecordDelay = INVALID_DELAY;
        gEvChargeInfo.u8EEPROMNeedRefreshVIN[0] = FALSE;
        gEvChargeInfo.u8EEPROMNeedRefreshVIN[1] = FALSE;
}


/************************************************/
static void  TASK_Create(void)
{
	UINT8 uIdx;
	static const TASK_XXXInfo *sTaskPtr;
	for(uIdx = 0; uIdx < TASK_MAX_NUM ; uIdx++)
	{
		sTaskPtr =  &sTaskArray[uIdx];
		OSTaskCreateExt(sTaskPtr->fTaskProc,
			(void*)0,
			(void*)&(sTaskPtr->StkPtr[sTaskPtr->u16Stk_Size - 1]),
			sTaskPtr->u8Priority,
			sTaskPtr->u8Id,
			(void *)&(sTaskPtr->StkPtr[0]),
			sTaskPtr->u16Stk_Size,
			(void*)0,
			OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	}
}

static  void  TASK_AppStartProc(void *p_arg)
{
	(void)p_arg;
	BSP_IrqDisbl();
	BSP_Init();
	BSP_Tick_Init();
	
	WTDOG_FEED();

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
	App_SerialInit();
#endif
	
	InitOBJ();
	WTDOG_FEED();
	
	InitHIHardware();
	WTDOG_FEED();
	
	start();
	WTDOG_FEED();
	
	InitParasSramVariables();
	WTDOG_FEED();

	InitRunArgs();
	WTDOG_FEED();
	
	InitMutex();
	WTDOG_FEED();
	
	AppProbe_Slider = 200u;

	WTDOG_FEED();
	BSP_IrqEnable();

	WTDOG_FEED();
	InitEVInfo();

	WTDOG_FEED();
	TASK_Create();

	WTDOG_FEED();



	WTDOG_FEED();

	OSTimeDly(1000);
	OSTimeDly(1000);
	while (DEF_TRUE) 
	{
		gSelfTest.fSetTaskID(TASK_ID_START);

		//中断出错，重新初始化。
		if (0xff == dv_Uart4.SendLastDelay)
		{
			dv_Uart4.SendLastDelay = 0;									//这句很重要！！
			dv_Uart4.fInitUart(m9600, UARTX_PARITY_EVEN);
			dv_Uart4.fEnableRx();										//使能收中断
		}

		//中断出错，重新初始化。
		if (0xff == dv_Uart5.SendLastDelay)
		{
			dv_Uart5.SendLastDelay = 0;
			dv_Uart5.fInitUart5(m9600, UARTX_PARITY_EVEN);
			dv_Uart5.fEnableRx();										//使能收中断
		}

		OSTimeDlyHMSM(0u, 0u, 0u, (AppProbe_Slider));
	}
}


int main(void)
{
	unsigned char uctemp;
	uctemp = sizeof(__CHARGE_RECORD);
	UNUSED(uctemp);
	CPU_Init();
	Mem_Init();
	Math_Init();
	OSInit();
	
	OSTaskCreateExt((void (*)(void *))TASK_AppStartProc,    // Create the start task
			(void *) 0,
			(OS_STK *)&STK_AppTask[APP_CFG_TASK_START_STK_SIZE - 1],
			(INT8U) APP_CFG_TASK_START_PRIO,
			(INT16U) APP_CFG_TASK_START_PRIO,
			(OS_STK*)&STK_AppTask[0],
			(INT32U) APP_CFG_TASK_START_STK_SIZE,
			(void*) 0,
			(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));


	OSStart();// Start multitasking (i.e. give control to uC/OS-III).

	while (DEF_ON)
	{
		// Should Never Get Here.
	};
}



/***************************************************************************************
	可以把512条充电记录理解为一个滑动窗口（占两个扇区）
	在这如下三个扇区里滚动
	RECORD_SECTION_MAX_NUM * PAGE_NUM_PER_SECTION
	u8UnResponseIdx  0标识已经上传   1标识未上传！
				______________  
				|	      |
				|_____________|
i16CurrPageIdx----->|           |
				|_____________|
				|	      |
				|_____________|


	??è?1ì?¨?÷òy????μ?éè??o?  oí ??ò?á?
***************************************************************************************/
