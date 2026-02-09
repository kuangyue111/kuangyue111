#include "main.h"
#include "TelaidianProto.h"
/*******************************************************************************************************************************************
		本部分		用于屏	通信
*********************************************************************************************************************************************/
 #define ABS(a)	(((a) < 0) ? (-(a)) : (a))
//NOINIT UINT8 Uart6SendBuff[UART6_SEND_MAX_NUM];
//NOINIT unsigned short Uart6RecvBuff[UART6_RECV_MAX_NUM];
NOINIT MODBUS_RECV_INFO gMdbusRcvInfo[MDBUS_RCV_ARRY_NUM + 4];
NOINIT UINT8 u8AnalayseRbuf[MDBUS_BYTES_MAX_NUM + 50];
NOINIT UINT8 u8MdbSendbuf[MDBUS_SEND_BYTES_MAX_NUM+20];
NOINIT UINT8 ViewMin,ViewHour,ViewSec;
NOINIT UINT8 ViewCostBuf[64];
#define EV_DWORD_HI(w)  ((UINT8)((UINT16)(w) >> 8))
#define EV_DWORD_LO(w)  ((UINT8)((UINT16)(w) &0xff))
UINT8 gUart6ArryIdx = 0;
HMI_INFO gHMIData;
void InitMdbusBuf(void);
void CMD_0X03Deal(UINT8* pRcvBuf);
void CMD_0X06Deal(UINT8* pRcvBuf);
void CMD_0X10Deal(UINT8* pRcvBuf);
void CMD_0X05Deal(UINT8* pRcvBuf);
void MdbusDeal(void);
UINT16 MdbusCRC_check(UINT8 *fp_CRC, UINT8 count);
UINT16 u16MdbGetSignal(UINT16 RegAddr);
UINT16 u16MdbSetSignal(UINT16 RegAddr,UINT16 u16Value);
UINT16 u16Fun05SetSignal(UINT16 RegAddr,UINT16 u16Value);
UINT16 CRC16_Mdbcheck(UINT8 *fp_CRC, UINT8 count);
void InitHMIData(void);
void CMD_0X04Deal(UINT8* pRcvBuf);
void RPT_Base_A_B_PageDeal(UINT8* pRcvBuf);
void RefreshQRCode(void);
UINT8 u8CompareLocalPSWD(void);
void Page_76_KeBoard_KEYDeal(UINT16 u16KeyVal);
void Page_77_Business_KEYDeal(UINT16 u16KeyVal);
void Page_79_SelectChargeMode_KEYDeal(UINT16 u16KeyVal);
void Page_80_EnergeMode_KEYDeal(UINT16 u16KeyVal);
void Page_81_TimeMode_KEYDeal(UINT16 u16KeyVal);
void Page_82_SocMode_KEYDeal(UINT16 u16KeyVal);
void Page_78_A_B_SpearSelect_KEYDeal(UINT16 u16KeyVal);
void  Page_83_WaitConnectSpear_KEYDeal(UINT16 u16KeyVal);
void SpecificPageDeal(void);
void SecificPage_83_Deal(void);
void SpecificPage_BRUSHEventDeal(void);
void SecificPage_12_GetA_B_Deal(void);
void SecificPage_11_206_HomeALM_Deal(void);
NOINIT UINT8 u8g_1_QR_Code[80];
NOINIT UINT8 u8g_2_QR_Code[80];


enum TOPWAY_Sig_16bit_Address_def
{
	TOPWAY_16BIT_ADDR_GUN_SELECT = 0x00080004,			//选中哪把枪  0-未选中  1-选中A  2-选中B
	TOPWAY_16BIT_ADDR_CHG_MODE_SELECT = 0x00080006,		//充电模式选择 0-自动充满 1-按时间 2-按电量 3-按金额 4-按SOC
	TOPWAY_16BIT_ADDR_SET_CHG_TIME = 0x00080008,		//充电模式-按时间 设定时间值
	TOPWAY_16BIT_ADDR_SET_CHG_ENERGY = 0x0008000A,		//充电模式-按电量 设定电量值
	TOPWAY_16BIT_ADDR_SET_CHG_MONEY = 0x0008000C,		//充电模式-按金额 设定金额值
	TOPWAY_16BIT_ADDR_SET_CHG_SOC = 0x0008000E,			//充电模式-按SOC 设定SOC值
};

enum MDB_Sig_Reg_Address_Idx
{
	//整机状态	
	CMD_0X03_EV_STATUS_1 = 1,				//1		充电桩整机	状态-1
	CMD_0X03_EV_STATUS_2,					//2		充电桩整机	状态-2
	CMD_0X03_RECT_STAT,						//3		充电模块状态

	CMD_0X03_1_EV_STOP_REASON,				//4		充电桩整机停止原因
	CMD_0X03_1_BMS_COMM_ALM,				//5		充电机-BMS通信告警
	CMD_0X03_1_BMS_STAT,					//6		BMS状态	
	CMD_0X03_1_BMS_STOP_REASON_1,			//7		BMS终止充电故障原因-1
	CMD_0X03_1_BMS_STOP_REASON_2,			//8		BMS终止充电故障原因-2	

	CMD_0X03_2_EV_STOP_REASON,				//9		充电桩整机停止原因
	CMD_0X03_2_BMS_COMM_ALM,				//10	充电机-BMS通信告警
	CMD_0X03_2_BMS_STAT,					//11		BMS状态	
	CMD_0X03_2_BMS_STOP_REASON_1,			//12		BMS终止充电故障原因-1
	CMD_0X03_2_BMS_STOP_REASON_2,			//13		BMS终止充电故障原因-2


	MDB_1_A_SET_REAL_VOLT_14,				//14
	MDB_1_B_SET_REAL_VOLT_15,				//15
	MDB_03_1_K_COEF_16,						//16
	MDB_03_1_B_COEF_17,						//17

	MDB_2_A_SET_REAL_VOLT_18,				//18
	MDB_2_B_SET_REAL_VOLT_19,				//19
	MDB_2_K_COEF_20,						//20
	MDB_2_B_COEF_21,						//21


	MDB_1_AC_OVER_50 = 50,
	MDB_1_AC_UNDER_51,
	MDB_RECT_COMMFail_52,
	MDB_EPO_53,
	MDB_SPD_54,
	MDB_NET_COMM_FAIL_55,
	MDB_BRUSH_FAIL_56,
	MDB_DOOR_FAIL_57,
	MDB_HOST_CONNECT_FAIL_58,
	/*****************************************************************************************
	*****************************************************************************************
	*****************************************************************************************
							遥测量
	*****************************************************************************************
	*****************************************************************************************
	******************************************************************************************/
	CMD_0X03_EV_NO1_90 = 90,				//充电桩编号	1
	CMD_0X03_EV_NO2_90,						//充电桩编号	2
	CMD_0X03_EV_NO3_90,						//充电桩编号	3
	CMD_0X03_EV_NO4_90,						//充电桩编号	4
	
	/***************************		1 号枪		充电参数整机数据	**********************************/
	MDB_1_SPEAR_RUN_STAT_100 = 100,							//0 充电			1未充电
	MDB_1_OUT_VOLT_101,										//充电机输出电压	101	U16			0.1V	0x03	
	MDB_1_OUT_CURR_102,										//充电机输出电流	102	U16			0.1A	0x03	
	MDB_1_REQR_VOLT_103,									//BMS需求电压		103	U16			0.1V	0x03	
	MDB_1_REQR_CURR_104,									//BMS需求电流		104	U16			0.1A	0x03	
	MDB_1_MEASURE_VOLT_105,									//BMS测量电压		105	U16			0.1V	0x03	
	MDB_1_BCS_CURR_106,										//BMS测量电流		106	U16			0.1A	0x03	
	MDB_1_TIME_MIN_107,										//已充电时间-分钟	108	U16			0~59	0x03	
	MDB_1_SOC_108,											//当前SOC			109			U16			0~100	0x03	
	MDB_1_ENERGY_109,										//本次充电电量		110	U16				0x03	
	MDB_1_MONEY_110,										//本次充电金额		110	U16			0.01元	0x03	
	MDB_1_TEST_MODE_111,

	MDB_1_SYS_TEMP_117 = 117,
	MDB_1_RECT_NUM_118,
	MDB_1_WH_REAL_VAL_119,
	MDB_1_WH_REAL_VAL_120,
	MDB_1_AB_L_VOLT_121,
	MDB_1_BC_L_VOLT_122,
	MDB_1_AC_L_VOLT_123,
	MDB_1_CC1_VAL_124,
	MDB_1_ELockStat_125,
	MDB_1_WH_CURR_126,
	MDB_1_IS_OPT_A_127,



	/******************************	1 号枪		充电参数（BMS数据）		**************************************************/
	MDB_1_BATT_TYPE_200 = 200,								//电池类型	200	U16	见注释1		
	CMD_0X03_1_BATT_YEAR,									//电池组生产日期：年	201	U16	1985～2235年		
	CMD_0X03_1_BATT_MON,									//电池组生产日期：月	202	U16	1～12月		
	CMD_0X03_1_BATT_DAY,									//电池组生产日期：日	203	U16	1～31日		
	CMD_0X03_1_BATT_FACTORY,								//电池厂商	204	U16	标准ASCII码		
	MDB_1_BATT_ALLOW_MAX_TEMP_205,							//电池最高允许温度	205	U16		
	MDB_1_BATT_ALLOW_MAX_VOLT_206,							//电池最高允许充电电压	206	U16			0.1V
	MDB_1_BATT_ALLOW_MAX_CURR_207,							//最高允许充电电流	207	U16			0.1A
	MDB_1_BATT_RATED_VOLT_208,								//电池额定总电压	208	U16			0.1V
	MDB_1_BATT_BCS_VOLT_209,								//电池当前电压	209	U16			0.1V
	MDB_1_BATT_RATED_CAP_210,								//电池额定容量	210	U16			0.1Ah
	CMD_0X03_1_BATT_NOMINAL_CAP,							//电池标称能量	211	U16			0.1kWh
	MDB_1_BATT_SOC_212,									//电池SOC	212	U16			1
	MDB_1_BCS_VOLT_213,									//BMS测量电压	218	U16			0.1V
	MDB_1_BCS_CURR_214,									//BMS测量电流	219	U16			0.1A
	MDB_1_BCS_TIME_215,									//BMS估算剩余时间	220	U16		MIN
	MDB_1_CELL_ALLOW_VOLT_216,
	MDB_1_CELL_VOLT_217,



	/***************************		1 号枪		告警	**************************************************/
	MDB_1_INSLT_ALM_300 = 300,
	MDB_1_IMIU_COMM_ALM_301,
	MDB_1_WH_COMM_ALM_302,
	MDB_1_CC1_ALM_303,
	MDB_1_BUS_OVER_ALM_304,
	MDB_1_CELL_OVER_ALM_305,
	MDB_1_OVER_CURR_ALM_306,
	MDB_1_POLARITY_ALM_307,
	MDB_1_NORECT_ALM_308,

	//400 用于获取二维码
	MDB_1_QR_1_400 = 400,
		
		
	
	/***************************		2 号枪		充电参数整机数据	**********************************/
	MDB_2_SPEAR_RUN_STAT_500 = 500,						//0 充电 1未充电
	MDB_2_OUT_VOLT_501,									//充电机输出电压			U16			0.1V	0x03	
	MDB_2_OUT_CURR_502,									//充电机输出电流			U16			0.1A	0x03	
	MDB_2_REQR_VOLT_503,									//BMS需求电压				U16			0.1V	0x03	
	MDB_2_REQR_CURR_504,									//BMS需求电流				U16			0.1A	0x03	
	MDB_2_MEASURE_VOLT_505,									//BMS测量电压			U16			0.1V	0x03	
	MDB_2_BCS_CURR_506,									//BMS测量电流				U16			0.1A	0x03		
	MDB_2_TIME_MIN_507,									//已充电时间-分钟			U16			0~59	0x03	
	MDB_2_SOC_508,											//当前SOC					U16			0~100	0x03	
	MDB_2_ENERGY_509,										//本次充电电量				U16				0x03	
	MDB_2_MONEY_510,										//本次充电金额			510		U16			0.01元	0x03	
	MDB_2_TEST_MODE_511,									//511

	MDB_2_SYS_TEMP_517 = 517,
	MDB_2_RECT_NUM_518,
	MDB_2_WH_REAL_VAL_519,
	MDB_2_WH_REAL_VAL_520,
	MDB_2_AB_L_VOLT_521,
	MDB_2_BC_L_VOLT_522,
	MDB_2_AC_L_VOLT_523,
	MDB_2_CC1_VAL_524,
	MDB_2_ELockStat_525,
	MDB_2_WH_CURR_526,
	MDB_2_IS_OPT_B_527,


	/******************************	2 号枪		（BMS数据）		**************************************************/
	MDB_2_BATT_TYPE_600 = 600,								//电池类型	200	U16	见注释1		
	CMD_0X03_2_BATT_YEAR,									//电池组生产日期：年	201	U16	1985～2235年		
	CMD_0X03_2_BATT_MON,									//电池组生产日期：月	202	U16	1～12月		
	CMD_0X03_2_BATT_DAY,									//电池组生产日期：日	203	U16	1～31日		
	CMD_0X03_2_BATT_FACTORY,								//电池厂商	204	U16	标准ASCII码		
	MDB_2_BATT_ALLOW_MAX_TEMP_605,							//电池最高允许温度	205	U16		
	MDB_2_BATT_ALLOW_MAX_VOLT_606,							//电池最高允许充电电压	206	U16			0.1V
	MDB_2_BATT_ALLOW_MAX_CURR_607,							//最高允许充电电流	207	U16			0.1A
	MDB_2_BATT_RATED_VOLT_608,								//电池额定总电压	208	U16			0.1V
	MDB_2_BATT_BCS_VOLT_609,								//电池当前电压	209	U16			0.1V
	MDB_2_BATT_RATED_CAP_610,								//电池额定容量	210	U16			0.1Ah
	CMD_0X03_2_BATT_NOMINAL_CAP,							//电池标称能量	211	U16			0.1kWh
	MDB_2_BATT_SOC_612,									//电池SOC	212	U16			1
	MDB_2_BCS_VOLT_613,									//BMS测量电压	218	U16			0.1V
	MDB_2_BCS_CURR_614,									//BMS测量电流	219	U16			0.1A
	MDB_2_BCS_TIME_615,									//BMS估算剩余时间	220	U16		MIN
	MDB_2_CELL_ALLOW_VOLT_616,
	MDB_2_CELL_VOLT_617,
	
	/***************************		2 号枪		告警	700-800 **************************************************/
	MDB_2_INSLT_ALM_700 = 700,
	MDB_2_IMIU_COMM_ALM_701,
	MDB_2_WH_COMM_ALM_702,
	MDB_2_CC1_ALM_703,
	MDB_2_BUS_OVER_ALM_704,
	MDB_2_CELL_OVER_ALM_705,
	MDB_2_OVER_CURR_ALM_706,
	MDB_2_POLARITY_ALM_707,
	MDB_2_NORECT_ALM_708,

	//800		用于获取二维码
	MDB_2_QR_1_800 = 800,

	MDB_EV_CODE_900 = 900,			//预留64个字
	MDB_OFFLINE_PSWD_965 = 965,		//6个字

	MDB_CARD_A_INFO_980 = 980,		//卡信息预留32个字
	MDB_CARD_A_END_INFO_1011 = 1011,

	MDB_CARD_B_INFO_1020 = 1020,		//卡信息预留32个字
	MDB_CARD_B_END_INFO_1054 = 1054,


	MDB_PAGE87_STOP_CHG_FLAG_1060 = 1060,
	MDB_PAGE87_STOP_CHG_TIMEOUT_1061,
	MDB_PAGE87_STOP_CHG_CARD_NUMBER_1062,

	MDB_PAGE_NOTIC_PLUG_SPEAR_CARD_NUMBER_1081 = 1081,

	MDB_PAGE_NOTIC_PLUG_SPEAR_1097 = 1097,
	MDB_PAGE_ACCOUNT_CONFIRM_STAT_1098 = 1098,
	MDB_PAGE_ACCOUNT_BALANCE_1099,

	MDB_PAGE_TIME_OUT_1100 = 1100,
	MDB_VIEW_PSWD_1101 = 1101,				//6字节
	MDB_VIEW_PSWD_Result_1107 = 1107,		//1字

	MDB_VIEW_ENERGY_ARGS_1108 = 1108,		//1字
	MDB_VIEW_TIME_ARGS_1109 = 1109,			//1字
	MDB_VIEW_SOC_ARGS_1110 = 1110,			//1字

	/******************************	CMD 10 指令	****************************************/
	MDB_A_RSLT_S_HOUR_1200 = 1200,
	MDB_A_RSLT_S_MIN_1201,
	MDB_A_RSLT_S_SEC_1202,
	MDB_A_RSLT_E_HOUR_1203,
	MDB_A_RSLT_E_MIN_1204,
	MDB_A_RSLT_E_SEC_1205,
	MDB_A_RSLT_KEEP_HOUR_1206,
	MDB_A_RSLT_KEEP_MIN_1207,
	MDB_A_RSLT_KEEP_SEC_1208,
	MDB_A_RSLT_S_SOC_1209,
	MDB_A_RSLT_E_SOC_1210,
	MDB_A_RSLT_ENERGY_1211,
	MDB_A_RSLT_MONEY_1212,
	MDB_A_RSLT_REASON_3_1213,
	MDB_A_RSLT_REASON_12_1214,


	MDB_B_RSLT_S_HOUR_1250 = 1250,
	MDB_B_RSLT_S_MIN_1251,
	MDB_B_RSLT_S_SEC_1252,
	MDB_B_RSLT_E_HOUR_1253,
	MDB_B_RSLT_E_MIN_1254,
	MDB_B_RSLT_E_SEC_1255,
	MDB_B_RSLT_KEEP_HOUR_1256,
	MDB_B_RSLT_KEEP_MIN_1257,
	MDB_B_RSLT_KEEP_SEC_1258,
	MDB_B_RSLT_S_SOC_1259,
	MDB_B_RSLT_E_SOC_1260,
	MDB_B_RSLT_ENERGY_1261,
	MDB_B_RSLT_MONEY_1262,
	MDB_B_RSLT_REASON_3_1263,
	MDB_B_RSLT_REASON_12_1264,


	MDB_RECORD_NO_1299 = 1299,
	MDB_RECORD_COST_NO1_1300,
	MDB_RECORD_COST_NO2_1301,
	//123456....
	MDB_RECORD_COST_NO32_1331 = 1331,

	MDB_RECORD_CARD_NO1_1332,
	MDB_RECORD_CARD_NO2_1333,
	//123456....
	MDB_RECORD_CARD_NO32_1363 = 1363,
	MDB_RECORD_S_YEAR_1364,
	MDB_RECORD_S_MON_1365,
	MDB_RECORD_S_D_1366,
	MDB_RECORD_S_H_1367,
	MDB_RECORD_S_MIN_1368,
	//MDB_RECORD_S_SEC_1369,

	MDB_RECORD_E_YEAR_1369,
	MDB_RECORD_E_MON_13670,
	MDB_RECORD_E_D_1371,
	MDB_RECORD_E_H_1372,
	MDB_RECORD_E_MIN_1373,
	//MDB_RECORD_E_SEC_1375,

	MDB_RECORD_TIME_H_1374,
	MDB_RECORD_TIME_M_1375,
	MDB_RECORD_TIME_S_1376,
	MDB_RECORD_S_WH_1377,
	MDB_RECORD_E_WH_1378,
	MDB_RECORD_OUT_ENERGY_1379,
	MDB_RECORD_TYPE_1380,

	//05 指令！！
/****************************************************************************************/
	MDB_A_CHARGE_CMD_2000 = 2000,
	MDB_B_CHARGE_CMD_2001,

	MDB_A_TEST_MODE_2005 = 2005,
	MDB_B_TEST_MODE_2006 = 2006,

	MDB_KEY_0_2010 = 2010,
	MDB_KEY_1_2011,
	MDB_KEY_2_2012,
	MDB_KEY_3_2013,
	MDB_KEY_4_2014,
	MDB_KEY_5_2015,
	MDB_KEY_6_2016,
	MDB_KEY_7_2017,
	MDB_KEY_8_2018,
	MDB_KEY_9_2019,
	MDB_KEY_CANCEL_GO_A_B_GET_2020,
	MDB_KEY_BACK_2021,
	MDB_KEY_CLEAN_2022,
	MDB_KEY_ENT_2023,
	MDB_KEY_BMS_AUTO_2024,
	MDB_KEY_A_SELECT_2025,
	MDB_KEY_B_SELECT_2026,
	MDB_KEY_OTHER_MODE_2027,			//进79界面
	MDB_KEY_BMS_TIME_MODE_2028,			//进81界面
	MDB_KEY_BMS_ENERGE_MODE_2029,		//进80界面
	MDB_KEY_BMS_SOC_MODE_2030,			//进82界面
	MDB_KEY_BMS_ENERGE_10_2031,
	MDB_KEY_BMS_ENERGE_15_2032,
	MDB_KEY_BMS_ENERGE_20_2033,
	MDB_KEY_BMS_ENERGE_25_2034,
	MDB_KEY_BMS_TIME_30_2035,
	MDB_KEY_BMS_TIME_60_2036,
	MDB_KEY_BMS_TIME_120_2037,
	MDB_KEY_BMS_TIME_180_2038,
	MDB_KEY_BMS_SOC_70_2039,
	MDB_KEY_BMS_SOC_80_2040,
	MDB_KEY_BMS_SOC_90_2041,
	MDB_KEY_BMS_SOC_100_2042,
	MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043,
	MDB_KEY_SOC_TIME_ENERGE_CONFIRM_2044,
	MDB_KEY_SPEAR_CONFIRM_2045,
	
	
	
	//2400		开始
	//参数地址				系统参数向前增加
	MDB_ADDR_u8NetMask_2494 = 2494,					//2494
	MDB_ADDR_u8DoorAlmMask_1_2495,
	MDB_ADDR_SYS_POWER = 2496,
	MDB_ADDR_VERSION = 2497,
	MDB_ADDR_PARALL_MODE = 2498,
	MDB_ADDR_SPD_MASK =2499,							//2513
	MDB_ADDR_fSysSetOverVolt_2500 = 2500,				
	MDB_ADDR_fRectLoadStartTime_2501,					//
	MDB_ADDR_MaxOutCurr_2502,					
	MDB_ADDR_MaxOutVolt_2503,	
	MDB_ADDR_fMaxOffLineTime_2504,
	MDB_ADDR_u8SetCalcMoneyType_2505,	
	MDB_ADDR_u8PWMPolarityCover_2506,					//
	MDB_ADDR_u8EPOAlmVltLEVEL_2507,			
	MDB_ADDR_u8BrushFactorType_2508,				
	MDB_ADDR_u8EV_StartTemp_2509,						//2509	
	MDB_ADDR_u8EV_StopTemp_2510,						//2510
	MDB_ADDR_u8NET_OffLine_Charge_Enable_2511,		//2511
	MDB_ADDR_u32PowerOnTimes_2512,					//2512
	


	//A 枪参数
	MDB_ADDR_CC1_1Coef_A_2513,							//2513
	MDB_ADDR_CC1_1Coef_B_2514,	
	MDB_ADDR_fBattVolt_1CoefA_2515,				
	MDB_ADDR_fBattVolt_1CoefB_2516,	
	MDB_ADDR_u32_A_CTS_DB_T1_2517,				
	MDB_ADDR_u32_A_DB_DB_T2_2518,				
	MDB_ADDR_u16SetBCLTimeOutVal_1_2519,					//2519
	MDB_ADDR_u16SetBRMTimeOutVal_1_2520,			
	MDB_ADDR_u16SetBCSTimeOutVal_1_2521,			
	MDB_ADDR_fSetLimitMaxOutCurr_1_2522,				
	MDB_ADDR_u8PolarityAlmMask_1_2523,			
	MDB_ADDR_u8CC1CheckEnable_1_2524,			
	MDB_ADDR_u8SelfTestEnable_1_2525,	
	MDB_ADDR_u8MCCBOnCheckEnable_1_2526,			
	MDB_ADDR_u8OutOverVoltEnable_1_2527,				//2527
	MDB_ADDR_u8DoorAlmMask_1_2528,				
	MDB_ADDR_u8HasElec_ALock_1_2529,				
	MDB_ADDR_u8_A_LOCKType_2530,						//2530		
	MDB_ADDR_u8LOCK_A_DIType_2531,				
	MDB_ADDR_u8_A_ForceVer2011_2532,				
	MDB_ADDR1_fACWhCoef_2533,			
	MDB_ADDR1_u8WH_TYPE_2534,			
	MDB_ADDR1_u8WH_ALM_Mask_2535,		
	MDB_ADDR1_u8JYMAlmStatMask_2536,					//2536
	MDB_ADDR1_u8SetJYMCtrlMode_2537,		
	MDB_ADDR1_u8InsltCheckByVolt_2538,	
	MDB_ADDR1_fSetJYMAlm_Resist_2539,					//2539



	//B 枪参数！
	MDB_ADDR_CC1_2Coef_A_2540,						//2540
	MDB_ADDR_CC1_2Coef_B_2541,
	MDB_ADDR_fBattVolt_2CoefA,	
	MDB_ADDR_fBattVolt_2CoefB,	
	MDB_ADDR_u32_B_CTS_DB_T1,				
	MDB_ADDR_u32_B_DB_DB_T2,				
	MDB_ADDR_u16SetBCLTimeOutVal_2,	
	MDB_ADDR_u16SetBRMTimeOutVal_2_2547,				//2547
	MDB_ADDR_u16SetBCSTimeOutVal_2,		
	MDB_ADDR_fSetLimitMaxOutCurr_2,		
	MDB_ADDR_u8PolarityAlmMask_2,			
	MDB_ADDR_u8CC1CheckEnable_2,			
	MDB_ADDR_u8SelfTestEnable_2_2552,					//2552	
	MDB_ADDR_u8MCCBOnCheckEnable_2,	
	MDB_ADDR_u8OutOverVoltEnable_2,		
	MDB_ADDR_u8DoorAlmMask_2,	
	MDB_ADDR_u8HasElec_ALock_2_2556,					//2556		
	MDB_ADDR_u8_B_LOCKType,					
	MDB_ADDR_u8LOCK_B_DIType,				
	MDB_ADDR_u8_B_ForceVer2011,				
	MDB_ADDR2_fACWhCoef,	
	MDB_ADDR2_u8WH_TYPE,	
	MDB_ADDR2_u8WH_ALM_Mask,		
	MDB_ADDR2_u8JYMAlmStatMask_2563,					//2563	
	MDB_ADDR2_u8SetJYMCtrlMode,					
	MDB_ADDR2_u8InsltCheckByVolt,
	MDB_ADDR2_fSetJYMAlm_Resist_2566,					//

	MDB_ADDR1_BMS_POWER_2567,

	MDB_ADDR2_BMS_POWER_2568,

	MDB_ADDR_DHCP_VAL_2650 = 2650,
	MDB_ADDR_MAC1_2651,
	MDB_ADDR_MAC2_2652,
	MDB_ADDR_MAC3_2653,
	MDB_ADDR_MAC4_2654,
	MDB_ADDR_MAC5_2655,
	MDB_ADDR_MAC6_2656,
	
	MDB_ADDR_IP1_2657,
	MDB_ADDR_IP2_2658,
	MDB_ADDR_IP3_2659,
	MDB_ADDR_IP4_2660,
	MDB_ADDR_MASK1_2661,
	MDB_ADDR_MASK2_2662,
	MDB_ADDR_MASK3_2663,
	MDB_ADDR_MASK4_2664,
	MDB_ADDR_Gateway1_2665,
	MDB_ADDR_Gateway2_2666,
	MDB_ADDR_Gateway3_2667,
	MDB_ADDR_Gateway4_2668,

	

	MDB_BASE_PAGE_ADDR_2700 = 2700,
	MDB_BASE_PAGE_ADDR_RESERVE,							//1O下发不用！
	MDB_A_PAGE_ADDR_2702,
	MDB_A_PAGE_ADDR_reserve,
	MDB_B_PAGE_ADDR_2704,
	MDB_B_PAGE_ADDR_reserve,
	MDB_TEST_PAGE_ADDR_2706,
	MDB_TEST_PAGE_ADDR_reserve,
	MDB_TEST_2_PAGE_ADDR_2708,
	BUTTONS_MaintenanceLogIn_2709,
	BUTTONS_Go_HOME_2710,
	BUTTONS_Go_Sys_PARAM_2711,
	BUTTONS_A_PARAM_2712,
	BUTTONS_B_PARAM_2713,
	BUTTONS_A_ADJUST_BUS_2714,
	BUTTONS_B_ADJUST_BUS_2715,
	BUTTONS_Go_TestCharge_2716,
	BUTTONS_Go_Sys_PARAM_2_2717,
	//BUTTONS_PSWD_BACK_2718,
	BUTTONS_GET_RECORD_LOG_2718,
	BUTTONS_GET_NEXT_RECORD_LOG_2719,
	BUTTONS_GET_PRE_RECORD_LOG_2720,
	BUTTONS_CLR_RECORD_LOG_2721,

	//BUTTONS_TEST_POP_CARD_2799 = 2799,

	MDB_ADDR_PORTNO_2800 = 2800,
	MDB_ADDR_ServerIp1_2801,
	MDB_ADDR_ServerIp2_2802,
	MDB_ADDR_ServerIp3_2803,
	MDB_ADDR_ServerIp4_2804,
	MDB_ADDR_DNSIp1_2805,
	MDB_ADDR_DNSIp2_2806,
	MDB_ADDR_DNSIp3_2807,
	MDB_ADDR_DNSIp4_2808,	
	MDB_ADDR_RemoteServerIsDomainName_2809,
	MDB_ADDR_ServerName_2810,
	MDB_ADDR_ServerName_2841 = MDB_ADDR_ServerName_2810+31,

	
	
	MDB_ADDR_TelaidianUserName_2900 = 2900,
	MDB_ADDR_TelaidianUserName_2931 = MDB_ADDR_TelaidianUserName_2900+31,
	MDB_ADDR_TelaidianPassword_2932,
	MDB_ADDR_TelaidianPassword_2964 = MDB_ADDR_TelaidianPassword_2932+31,

	MDB_ADDR_TelaidianStarionID_3000 = 3000,
	MDB_ADDR_TelaidianStarionID_3031 = MDB_ADDR_TelaidianStarionID_3000+31,
	MDB_ADDR_StationCode_3032,
	MDB_ADDR_StationCode_3079 = MDB_ADDR_StationCode_3032+47,
	MDB_ADDR_StationCode_3091 = MDB_ADDR_StationCode_3032+59,
	

};

//
//#define PAGE_INVALID_NO_0		0
//#define PAGE_HOME_NO_11			11
//#define PAGE_A_B_SPLIT_NO_12	12
//#define PAGE_A_QR_CODE_20		20
//#define PAGE_A_CHARGE_21		21
//
//
//
//#define PAGE_B_QR_CODE_35		35
//#define PAGE_B_CHARGE_36		36
//
//#define PAGE_MaintenanceLogIn_75		75
//#define PAGE_PSWD_KEYBOAD_76			76
//#define PAGE_BUSINESS_SELECT_77			77
//#define PAGE_SELECT_SPEAR_78			78
//#define PAGE_OTHER_CHARGE_MODE_79		79
//#define PAGE_ENERGE_ARGS_80				80
//#define PAGE_TIME_ARGS_81				81
//#define PAGE_SOC_ARGS_82				82
//#define PAGE_NOTE_PLUG_IN_83			83
//#define PAGE_A_RESULT_84				84
//#define PAGE_B_RESULT_85				85
//
//
//#define PAGE_TEST_CHARGE_200			200
//#define PAGE_SYS_PARAM_201				201
//#define PAGE_A_PARAM_202				202
//#define PAGE_B_PARAM_203				203
//#define PAGE_A_BUS_ADJUST_204			204
//#define PAGE_B_BUS_ADJUST_205			205
//#define PAGE_SYS_ALM_206				206
//#define PAGE_A_ALM_207					207
//#define PAGE_B_ALM_208					208
//#define PAGE_A_MORE_209					209
//#define PAGE_B_MORE_210					210
//#define PAGE_SYS_PARAM_2_211			211

void RefreshQRCode(void)
{
	char *p="ZTEV-";
	char *p2="1000-";
	char *p3="-01";
	char *p4="-01";
	char *p5="-";
	//char *p6="-01";
	//char *p7="-01";
	char *p8="-02";
	char *p9="-02";
	UINT8 u8Idx = 0;
	int i = 0;
	__XXX_UNION_VALUE unVal;
	memset(u8g_1_QR_Code, 0, 48);
	memset(u8g_2_QR_Code, 0, 48);
	//ZTEV-1000-19-150-0000000000000010-1-1
	u8Idx += sprintf((char*)u8g_1_QR_Code,"%s",p);//"ZTEV-";
	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%s",p2);//"1000-";
	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x25>>4)));//0
	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x25&0x0f)));//6
	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%s",p5);//-
	ExGetSigInfo(E_TYPE_SYS, ID_EV_SYS_POWER, &unVal, NULL, BY_E2PROM);
	if (unVal.chtemp[0] == 60)
	{
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x06)));
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));
	}
	else if (unVal.chtemp[0] == 90)
	{
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x09)));
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));
	}
	else if (unVal.chtemp[0] == 30)
	{
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x03)));
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));
	}
	else if (unVal.chtemp[0] == 120)
	{
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x01)));//1
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x02)));//2
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));//0
	}
	else
	{
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x01)));//1
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x05)));//5
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));//0
	}

	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%s",p5);//-
	//for (i = 0; i < 8; i++)
	//{
	//	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii(0x88 >>4));
	//	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",Hex2Ascii(0x88&0x0f));
	//}
	for (i = 0; i < 16; i++)
	{
		u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%c",gEvChargeInfo.u16EVCode[i]);
	}
	//u8Idx += sprintf((char*)gQR_Code + u8Idx, "%s",gu8EVStationNo);//00 00 00 00 00 00 00 10
	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%s",p3);//-01
	u8Idx += sprintf((char*)u8g_1_QR_Code + u8Idx, "%s",p4);//-01

	
	u8Idx = 0;
	i = 0;
	//ZTEV-1000-19-150-0000000000000010-2-2
	u8Idx += sprintf((char*)u8g_2_QR_Code,"%s",p);//"ZTEV-";
	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%s",p2);//"1000-";
	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x25>>4)));//0
	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x25&0x0f)));//6
	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%s",p5);//-

	if (unVal.chtemp[0] == 60)
	{
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x06)));
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));
	}
	else if (unVal.chtemp[0] == 90)
	{
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x09)));
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));
	}
	else if (unVal.chtemp[0] == 30)
	{
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x03)));
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));
	}
	else if (unVal.chtemp[0] == 120)
	{
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x01)));//1
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x02)));//2
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));//0
	}
	else
	{
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x01)));//1
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x05)));//5
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii((0x00)));//0
	}

	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%s",p5);//-
	//for (i = 0; i < 8; i++)
	//{
	//	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii(0x88 >>4));
	//	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c",Hex2Ascii(0x88&0x0f));
	//}
	for (i = 0; i < 16; i++)
	{
		u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%c", gEvChargeInfo.u16EVCode[i]);
	}
	//u8Idx += sprintf((char*)gQR_Code + u8Idx, "%s",gu8EVStationNo);//00 00 00 00 00 00 00 10
	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%s",p8);//-02
	u8Idx += sprintf((char*)u8g_2_QR_Code + u8Idx, "%s",p9);//-02


}

UINT16 u16MdbGetSignal(UINT16 RegAddr)
{
	__XXX_UNION_VALUE unTempVal;
	INT32 iTempVal = 0;
	void *PEInfo;
	static struct tm  tmNow;
	//void *PEInfo_2;
	UNUSED(RegAddr);
	UINT16 u16Value = 0;
	UNUSED(u16Value);
	switch (RegAddr)
	{
	case CMD_0X03_EV_STATUS_1:			//1		充电桩整机	状态-1
		break;

	case MDB_1_A_SET_REAL_VOLT_14:		//14
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattSet_A_Volt * 10);
		break;
	case MDB_1_B_SET_REAL_VOLT_15:		//15
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattSet_B_Volt * 10);
		break;
	case MDB_03_1_K_COEF_16:				//16
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_1CoefA * 100);
		break;
	case MDB_03_1_B_COEF_17:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_1CoefB * 100);
		break;

	case MDB_2_A_SET_REAL_VOLT_18:	//18
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattSet_A_Volt * 10);
		break;
	case MDB_2_B_SET_REAL_VOLT_19:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattSet_B_Volt * 10);
		break;
	case MDB_2_K_COEF_20:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_2CoefA * 100);
		break;
	case MDB_2_B_COEF_21:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_2CoefB * 100);
		break;

/***************************************************************************************************************
				系统告警		50 -- 100
***************************************************************************************************************/
	case MDB_1_AC_OVER_50:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->u8IsACOver);
		if ((((CANCommData_TypeDef*)PEInfo)->u8IsAllCommFail))
		{
			u16Value = 0;
		}
		break;
	case MDB_1_AC_UNDER_51:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->u8IsACUnder);
		if ((((CANCommData_TypeDef*)PEInfo)->u8IsAllCommFail))
		{
			u16Value = 0;
		}
		break;
	case MDB_RECT_COMMFail_52:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->u8IsAllCommFail);
		break;
	case MDB_EPO_53:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8EPOAlm);
		break;
	case MDB_SPD_54:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8SPDAlm);
		break;
	case MDB_NET_COMM_FAIL_55:
		if(gEvChargeInfo.u8NET_Mask == FALSE)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8NetCommAlm);
		}
		break;
	case MDB_HOST_CONNECT_FAIL_58:
		if(gEvChargeInfo.u8NET_Mask == FALSE)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8TelaidianHostCommAlm);
		}
		break;
	
	case MDB_BRUSH_FAIL_56:
		PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
		u16Value = (UINT16)(((BRUSH_INFO*)PEInfo)->uBrushCommAlm);
		break;
	case MDB_DOOR_FAIL_57:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DoorAlm);
		break;

/***************************		1 号枪		充电参数整机数据	************************************************
										100--200
******************************************************************************************************************/
	case MDB_1_SPEAR_RUN_STAT_100:							//充电接口标识	100	U16	1	0x03	一桩一枪时为1，多枪累加
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_A]);

		if((START_CHARGE_VAL == u16Value) && (gEvChargeInfo.RectOwner[GUN_NO_A] == GRP_B))
		{
			u16Value = 2;	//等待另侧释放
		}
		break;	
	case MDB_1_OUT_VOLT_101:								//充电机输出电压	101	U16			0.1V	0x03
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].fOutVolt * 10);
		//if (gEvChargeInfo.RectOwner[GUN_NO_A] == GRP_B)
		//{
		//	u16Value = 0;   //等待另侧释放
		//}
		break;				
	case MDB_1_OUT_CURR_102:								//充电机输出电流	102	U16			0.1A	0x03
		//电表正常时使用电表读数
		if((gWH_Info[GUN_NO_A].u8WH_ALM_Mask == FALSE) && (gWH_Info[GUN_NO_A].u8WHCommAlm == FALSE))
		{
			u16Value = (UINT16)(gWH_Info[GUN_NO_A].fWH_A_Curr * 10);
		}
		else
		{
			PEInfo = GetgEquipInfo(E_RECT_INFO);
			u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].fOutCurr * 10);	
			if (gEvChargeInfo.RectOwner[GUN_NO_B] == GRP_A)
			{
				u16Value += (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].fOutCurr * 10);
			}
		}		
		break;									
	case MDB_1_REQR_VOLT_103:								//BMS需求电压	103	U16			0.1V	0x03
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBMSRequireVolt * 10);
		break;		
	case MDB_1_REQR_CURR_104:								//BMS需求电流	104	U16			0.1A	0x03
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBMSRequireCurr * 10);
		break;										
	case MDB_1_MEASURE_VOLT_105:							//BMS测量电压	105	U16			0.1V	0x03
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fEV_BattVolt_1 * 10);
		if ((((EV_CHARGE_INFO*)PEInfo)->fEV_BattVolt_1 * 10) < 0)
		{
			iTempVal = ABS((int)(((EV_CHARGE_INFO*)PEInfo)->fEV_BattVolt_1 * 10));
			iTempVal = (~(iTempVal-1))&0xffff;
			u16Value = iTempVal;
		}
		break;										
	case MDB_1_BCS_CURR_106:								//BMS测量电流	106	U16			0.1A	0x03	
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MeasureCurr * 10);
		break;																
	case MDB_1_TIME_MIN_107:								//已充电时间-分钟	108	U16			0~59	0x03
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBMS_ChargeTime[GUN_NO_A] * 10);
		break;								
	case MDB_1_SOC_108:										//当前SOC	109	U16			0~100	0x03
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_SOC);
		break;									
	case MDB_1_ENERGY_109:									//本次充电电量	110	U16				0x03		
		u16Value = (UINT16)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_A]*10);
		break;	
	case MDB_1_MONEY_110:
		u16Value = (UINT16)gs_TelaidianCtrl.TelaidianAccountBalace[0];
		break;
	case MDB_1_TEST_MODE_111:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)((EV_CHARGE_INFO*)PEInfo)->u8_A_TestMode;
		break;
	case MDB_1_SYS_TEMP_117:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->fEV_InterTemp >= 0)
		{
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fEV_InterTemp *10);
		}
		else
		{
			iTempVal = ABS((int)(((EV_CHARGE_INFO*)PEInfo)->fEV_InterTemp * 10));
			iTempVal = (~(iTempVal-1))&0xffff;
			u16Value = iTempVal;
		}
		break;
	case MDB_1_RECT_NUM_118:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].nCommRectNum);
		break;
	case MDB_1_WH_REAL_VAL_119:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((UINT32)(((stWHInfo*)PEInfo)->fWHRealValue *10))&0xffff);
		break;
	case MDB_1_WH_REAL_VAL_120:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((UINT32)(((stWHInfo*)PEInfo)->fWHRealValue *10)>>16)&0xffff);
		break;
	case MDB_1_AB_L_VOLT_121:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->RectData[0].fAB_Volt * 10);
		break;
	case MDB_1_BC_L_VOLT_122:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->RectData[0].fBC_Volt * 10);
		break;
	case MDB_1_AC_L_VOLT_123:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->RectData[0].fCA_Volt * 10);
		break;
	case MDB_1_CC1_VAL_124:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fCC1_1VoltVal *10);
		break;
	case MDB_1_ELockStat_125:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8ElecLock_Alm);
		break;
	case MDB_1_WH_CURR_126:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->fWH_A_Curr * 10);
		break;
	case MDB_1_IS_OPT_A_127:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA);
		break;


/******************************	1	号枪	（BMS数据）		******************************************************************
										200--300
*******************************************************************************************************************************/
	case MDB_1_BATT_TYPE_200:						//电池类型	200	U16	见注释1		
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_BattType);
		break;		
	case CMD_0X03_1_BATT_YEAR:						//电池组生产日期：年	201	U16	1985～2235年	
		break;		
	case CMD_0X03_1_BATT_MON:						//电池组生产日期：月	202	U16	1～12月	
		break;											
	case CMD_0X03_1_BATT_DAY:						//电池组生产日期：日	203	U16	1～31日		
		break;										
	case CMD_0X03_1_BATT_FACTORY:					//电池厂商	204	U16	标准ASCII码	;
		break;
	case MDB_1_BATT_ALLOW_MAX_TEMP_205:				//电池最高允许温度	205	U16
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		if (((stBMS_INFO*)PEInfo)->fBCP_MaxAllowTemp >= 0)
		{
			u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_MaxAllowTemp *10);
		}
		else
		{
			iTempVal = ABS((int)(((stBMS_INFO*)PEInfo)->fBCP_MaxAllowTemp * 10));
			iTempVal = (~(iTempVal-1))&0xffff;
			u16Value = iTempVal;
		}
		break;										
	case MDB_1_BATT_ALLOW_MAX_VOLT_206:			//电池最高允许充电电压	206	U16		0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_MaxChargeVolt * 10);
		break;								
	case MDB_1_BATT_ALLOW_MAX_CURR_207:			//最高允许充电电流	207	U16			0.1A
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_MaxAllowCurr * 10);
		break;							
	case MDB_1_BATT_RATED_VOLT_208:				//电池额定总电压	208	U16			0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_RatedVOLT * 10);
		break;						
	case MDB_1_BATT_BCS_VOLT_209:					//电池当前电压	209	U16			0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_CurrBattVolt * 10);
		return u16Value;
		break;								
	case MDB_1_BATT_RATED_CAP_210:					//电池额定容量	210	U16			0.1Ah
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_RatedCAP * 10);
		break;						
	case CMD_0X03_1_BATT_NOMINAL_CAP:				//电池标称能量	211	U16			0.1kWh
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_RatedCAP * 10);
		break;									
	case MDB_1_BATT_SOC_212:						//电池SOC	212	U16		
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_CurrSOC);
		break;		
							
	case MDB_1_BCS_VOLT_213:						//BMS测量电压	218	U16			0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MeasureVolt * 10);
		break;										
	case MDB_1_BCS_CURR_214:						//BMS测量电流	219	U16			0.1A
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MeasureCurr * 10);
		break;									
	case MDB_1_BCS_TIME_215:						//BMS估算剩余时间	220	U16		MIN
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u16BCS_LeaveTime * 10);
		break;									

	case MDB_1_CELL_ALLOW_VOLT_216:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_CellMaxVolt * 10);
		break;
	case MDB_1_CELL_VOLT_217:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MaxCellVolt * 10);
		break;

/*****************************************************************************************************************************
									1号枪 告警 300---400
******************************************************************************************************************************/
	case MDB_1_INSLT_ALM_300:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU1);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8JYMAlmStat[0]);
		break;
	case MDB_1_IMIU_COMM_ALM_301:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU1);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8InsltCommAlm[0]);
		break;
	case MDB_1_WH_COMM_ALM_302:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->u8WHCommAlm);
		break;
	case MDB_1_CC1_ALM_303:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8EV_SpearPst[GUN_NO_A]);
		if (u16Value != SPEAR_ON_LINE)
		{
			u16Value = 1;
		}
		else
		{
			u16Value = 0;
		}
		break;
	case MDB_1_BUS_OVER_ALM_304:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8_Bus_Over_Alm);
		break;
	case MDB_1_CELL_OVER_ALM_305:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8_Cell_Over_Alm);
		break;
	case MDB_1_OVER_CURR_ALM_306:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8_OverCurr_Alm);
		break;
	case MDB_1_POLARITY_ALM_307:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8Polarity_Alm);
		break;
	case MDB_1_NORECT_ALM_308:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		if ((((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].u8IsNoNormalRect))
		{
			u16Value = 1;
		}
		else
		{
			u16Value = 0;
		}
		break;
	case MDB_1_QR_1_400:case 401:
	case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:
	case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:
	case 432:case 433:case 434:case 435:case 436:case 437:case 438:case 439:case 440:case 441:case 442:case 443:case 444:case 445:case 446:
	case 447:
		//u16Value = u8g_1_QR_Code[RegAddr - MDB_1_QR_1_400];
		iTempVal = RegAddr - MDB_1_QR_1_400;
		if(iTempVal < 32)
		{
			u16Value = gs_TelaidianCtrl.Telaidian_QR_Code[0][iTempVal];
		}
		break;


/***************************		2 号枪		充电参数整机数据	*********************************************************
								500--600
*****************************************************************************************************************************/
	case MDB_2_SPEAR_RUN_STAT_500:							//充电接口标识	100	U16	1	0x03	一桩一枪时为1，多枪累加
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_B]);

		if((START_CHARGE_VAL == u16Value) && (gEvChargeInfo.RectOwner[GUN_NO_B] == GRP_A))
		{
			u16Value = 2;   //等待另侧释放
		}
		break;	
	case MDB_2_OUT_VOLT_501:								//充电机输出电压	101	U16			0.1V	0x03	
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].fOutVolt * 10);
		//if (gEvChargeInfo.RectOwner[GUN_NO_B] == GRP_A)
		//{
		//	u16Value = 0;   //等待另侧释放
		//}
		break;				
	case MDB_2_OUT_CURR_502:								//充电机输出电流	102	U16			0.1A	0x03
		//电表正常时使用电表读数
		if((gWH_Info[GUN_NO_B].u8WH_ALM_Mask == FALSE) && (gWH_Info[GUN_NO_B].u8WHCommAlm == FALSE))
		{
			u16Value = (UINT16)(gWH_Info[GUN_NO_B].fWH_A_Curr * 10);
		}
		else
		{
			PEInfo = GetgEquipInfo(E_RECT_INFO);
			u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].fOutCurr * 10);	
			if (gEvChargeInfo.RectOwner[GUN_NO_A] == GRP_B)
			{
				u16Value += (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].fOutCurr * 10);
			}
		}		
		break;									
	case MDB_2_REQR_VOLT_503:								//BMS需求电压	103	U16			0.1V	0x03	
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBMSRequireVolt * 10);
		break;		
	case MDB_2_REQR_CURR_504:								//BMS需求电流	104	U16			0.1A	0x03	
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBMSRequireCurr * 10);
		break;										
	case MDB_2_MEASURE_VOLT_505:							//BMS测量电压	105	U16			0.1V	0x03	
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fEV_BattVolt_2 * 10);
		if ((((EV_CHARGE_INFO*)PEInfo)->fEV_BattVolt_2 * 10) < 0)
		{
			iTempVal = ABS((int)(((EV_CHARGE_INFO*)PEInfo)->fEV_BattVolt_2 * 10));
			iTempVal = (~(iTempVal-1))&0xffff;
			u16Value = iTempVal;
		}
		break;										
	case MDB_2_BCS_CURR_506:								//BMS测量电流	106	U16			0.1A	0x03
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MeasureCurr * 10);
		break;																	
	case MDB_2_TIME_MIN_507:								//已充电时间-分钟	108	U16			0~59	0x03	
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBMS_ChargeTime[GUN_NO_B] * 10);
		break;								
	case MDB_2_SOC_508:									//当前SOC	109	U16			0~100	0x03	
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_SOC);
		break;									
	case MDB_2_ENERGY_509:									//本次充电电量	110	U16				0x03		
		u16Value = (UINT16)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_B]*10);
		break;										
	case MDB_2_MONEY_510:									//本次充电金额	111	U16			0.01元	0x03
		u16Value = (UINT16)gs_TelaidianCtrl.TelaidianAccountBalace[1];
		break;		
	case MDB_2_TEST_MODE_511:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)((EV_CHARGE_INFO*)PEInfo)->u8_B_TestMode;
		break;

	case MDB_2_SYS_TEMP_517:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->fEV_InterTemp >= 0)
		{
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fEV_InterTemp *10);
		}
		else
		{
			iTempVal = ABS((int)(((EV_CHARGE_INFO*)PEInfo)->fEV_InterTemp * 10));
			iTempVal = (~(iTempVal-1))&0xffff;
			u16Value = iTempVal;
		}
		break;
	case MDB_2_RECT_NUM_518:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].nCommRectNum);
		break;
	case MDB_2_WH_REAL_VAL_519:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((UINT32)(((stWHInfo*)PEInfo)->fWHRealValue *10))&0xffff);
		break;
	case MDB_2_WH_REAL_VAL_520:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((UINT32)(((stWHInfo*)PEInfo)->fWHRealValue *10)>>16)&0xffff);
		break;
	case MDB_2_AB_L_VOLT_521:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->RectData[1].fAB_Volt * 10);
		break;
	case MDB_2_BC_L_VOLT_522:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->RectData[1].fBC_Volt * 10);
		break;
	case MDB_2_AC_L_VOLT_523:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		u16Value = (UINT16)(((CANCommData_TypeDef*)PEInfo)->RectData[1].fCA_Volt * 10);
		break;
	case MDB_2_CC1_VAL_524:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fCC1_2VoltVal *10);
		break;
	case MDB_2_ELockStat_525:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8ElecLock_Alm);
		break;
	case MDB_2_WH_CURR_526:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->fWH_A_Curr * 10);
		break;
	case MDB_2_IS_OPT_B_527:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB);
		break;


/******************************	2 号枪		充电参数（BMS数据）		*****************************************************************
									600--700
**************************************************************************************************************************************/
	case MDB_2_BATT_TYPE_600:						//电池类型	200	U16	见注释1		
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_BattType);
		break;		
	case CMD_0X03_2_BATT_YEAR:						//电池组生产日期：年	201	U16	1985～2235年	
		break;		
	case CMD_0X03_2_BATT_MON:						//电池组生产日期：月	202	U16	1～12月	
		break;											
	case CMD_0X03_2_BATT_DAY:						//电池组生产日期：日	203	U16	1～31日		
		break;										
	case CMD_0X03_2_BATT_FACTORY:					//电池厂商	204	U16	标准ASCII码	;
		break;
	case MDB_2_BATT_ALLOW_MAX_TEMP_605:			//电池最高允许温度	205	U16
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		if (((stBMS_INFO*)PEInfo)->fBCP_MaxAllowTemp >= 0)
		{
			u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_MaxAllowTemp *10);
		}
		else
		{
			iTempVal = ABS((int)(((stBMS_INFO*)PEInfo)->fBCP_MaxAllowTemp * 10));
			iTempVal = (~(iTempVal-1))&0xffff;
			u16Value = iTempVal;
		}
		break;										
	case MDB_2_BATT_ALLOW_MAX_VOLT_606:			//电池最高允许充电电压	206	U16		0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_MaxChargeVolt * 10);
		break;								
	case MDB_2_BATT_ALLOW_MAX_CURR_607:			//最高允许充电电流	207	U16			0.1A
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_MaxAllowCurr * 10);
		break;							
	case MDB_2_BATT_RATED_VOLT_608:				//电池额定总电压	208	U16			0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_RatedVOLT * 10);
		break;						
	case MDB_2_BATT_BCS_VOLT_609:					//电池当前电压	209	U16			0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_CurrBattVolt * 10);
		return u16Value;
		break;								
	case MDB_2_BATT_RATED_CAP_610:					//电池额定容量	210	U16			0.1Ah
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_RatedCAP * 10);
		break;						
	case CMD_0X03_2_BATT_NOMINAL_CAP:				//电池标称能量	211	U16			0.1kWh
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8BRM_RatedCAP * 10);
		break;									
	case MDB_2_BATT_SOC_612:						//电池SOC	212	U16		
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_CurrSOC);
		break;		
	case MDB_2_BCS_VOLT_613:						//BMS测量电压	218	U16			0.1V
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MeasureVolt * 10);
		break;										
	case MDB_2_BCS_CURR_614:						//BMS测量电流	219	U16			0.1A
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MeasureCurr * 10);
		break;									
	case MDB_2_BCS_TIME_615:						//BMS估算剩余时间	220	U16		MIN
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u16BCS_LeaveTime * 10);
		break;									
	case MDB_2_CELL_ALLOW_VOLT_616:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCP_CellMaxVolt * 10);
		break;
	case MDB_2_CELL_VOLT_617:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->fBCS_MaxCellVolt * 10);
		break;

/*****************************************************************************************************************************
									1号枪 告警 700---800
******************************************************************************************************************************/
	case MDB_2_INSLT_ALM_700:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU2);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8JYMAlmStat[0]);
		break;
	case MDB_2_IMIU_COMM_ALM_701:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU2);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8InsltCommAlm[0]);
		break;
	case MDB_2_WH_COMM_ALM_702:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->u8WHCommAlm);
		break;
	case MDB_2_CC1_ALM_703:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8EV_SpearPst[GUN_NO_B]);
		if (u16Value != SPEAR_ON_LINE)
		{
			u16Value = 1;
		}
		else
		{
			u16Value = 0;
		}
		break;
	case MDB_2_BUS_OVER_ALM_704:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8_Bus_Over_Alm);
		break;
	case MDB_2_CELL_OVER_ALM_705:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8_Cell_Over_Alm);
		break;
	case MDB_2_OVER_CURR_ALM_706:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8_OverCurr_Alm);
		break;
	case MDB_2_POLARITY_ALM_707:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		u16Value = (UINT16)(((stBMS_INFO*)PEInfo)->u8Polarity_Alm);
		break;
	case MDB_2_NORECT_ALM_708:
		PEInfo = GetgEquipInfo(E_RECT_INFO);
		if ((((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].u8IsNoNormalRect))
		{
			u16Value = 1;
		}
		else
		{
			u16Value = 0;
		}
		break;

	case MDB_2_QR_1_800:case 801:
	case 802:case 803:case 804:case 805:case 806:case 807:case 808:case 809:case 810:case 811:case 812:case 813:case 814:case 815:case 816:
	case 817:case 818:case 819:case 820:case 821:case 822:case 823:case 824:case 825:case 826:case 827:case 828:case 829:case 830:case 831:
	case 832:case 833:case 834:case 835:case 836:case 837:case 838:case 839:case 840:case 841:case 842:case 843:case 844:case 845:case 846:
	case 847:
		//u16Value = u8g_2_QR_Code[RegAddr - MDB_2_QR_1_800];
		iTempVal = RegAddr - MDB_2_QR_1_800;
		if(iTempVal < 32)
		{
			u16Value = gs_TelaidianCtrl.Telaidian_QR_Code[1][iTempVal];
		}
		break;


	case MDB_EV_CODE_900:case 901:
	case 902:case 903:case 904:case 905:case 906:case 907:case 908:case 909:case 910:case 911:case 912:case 913:case 914:case 915:case 916:
	case 917:case 918:case 919:case 920:case 921:case 922:case 923:case 924:case 925:case 926:case 927:case 928:case 929:case 930:case 931:
	case 932:case 933:case 934:case 935:case 936:case 937:case 938:case 939:case 940:case 941:case 942:case 943:case 944:case 945:case 946:
	case 947:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		//u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16EVCode[RegAddr - MDB_EV_CODE_900]);
		if((RegAddr - MDB_EV_CODE_900) < 32)
		{
			ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianStarionID_NUM_1 + RegAddr - MDB_EV_CODE_900, &unTempVal, NULL, BY_E2PROM);
			u16Value = unTempVal.chtemp[0];
		}
		break;

	case MDB_OFFLINE_PSWD_965:case 966:case 967:case 968:case 969:case 970:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16UserPassword[0][RegAddr - MDB_OFFLINE_PSWD_965]);
		break;

	case MDB_CARD_A_INFO_980:case 981:case 982:case 983:case 984:case 985:case 986:case 987:case 988:case 989:
	case 990:case 991:case 992:case 993:case 994:case 995:case 996:case 997:case 998:case 999:
	case 1000:case 1001:case 1002:case 1003:case 1004:case 1005:case 1006:case 1007:case 1008:case 1009:
	case 1011:
		PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
		u16Value = (UINT16)(((BRUSH_INFO*)PEInfo)->u8TempCARDInfo[RegAddr - MDB_CARD_A_INFO_980]);
		break;

	//case MDB_CARD_B_INFO_1020:case 1021:case 1022:case 1023:case 1024:case 1025:case 1026:case 1027:case 1028:case 1029:
	//case 1030:case 1031:case 1032:case 1033:case 1034:case 1035:case 1036:case 1037:case 1038:case 1039:
	//case 1040:case 1041:case 1042:case 1043:case 1044:case 1045:case 1046:case 1047:case 1048:case 1049:
	//case 1050:
	//	PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
	//	u16Value = (UINT16)(((BRUSH_INFO*)PEInfo)->u8CARDInfo[GUN_NO_B][RegAddr - MDB_CARD_B_INFO_1020]);
	//	break;

	case MDB_PAGE87_STOP_CHG_FLAG_1060:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA == TRUE)
		{
			u16Value = (UINT16)gs_TelaidianCtrl.StopRequest[0];
		}
		else if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB == TRUE)
		{
			u16Value = (UINT16)gs_TelaidianCtrl.StopRequest[1];
		}
		break;	

	case MDB_PAGE87_STOP_CHG_CARD_NUMBER_1062:case 1063:case 1064:case 1065:case 1066:case 1067:case 1068:case 1069:case 1070:
		case 1071:case 1072:case 1073:case 1074:case 1075:case 1076:case 1077:
		PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
		u16Value = (UINT16)(((BRUSH_INFO*)PEInfo)->u8TempCARDInfo[RegAddr - MDB_PAGE87_STOP_CHG_CARD_NUMBER_1062]);
		break;
	case MDB_PAGE_NOTIC_PLUG_SPEAR_CARD_NUMBER_1081:case 1082:case 1083:case 1084:case 1085:case 1086:case 1087:case 1088:
		case 1089:case 1090:case 1091:case 1092:case 1093:case 1094:case 1095:case 1096:
		PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
		u16Value = (UINT16)(((BRUSH_INFO*)PEInfo)->u8TempCARDInfo[RegAddr - MDB_PAGE_NOTIC_PLUG_SPEAR_CARD_NUMBER_1081]);
		break;

	case MDB_PAGE_NOTIC_PLUG_SPEAR_1097:
		//PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if(gEvChargeInfo.u8IsOptionB == TRUE)
		{
			if (gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] == SPEAR_ON_LINE)
			{
				u16Value = 0;  //枪已插好
			}
			else
			{
				u16Value = 2;   //提示插入B枪
			}
		}
		else if(gEvChargeInfo.u8IsOptionA == TRUE)
		{
			if (gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] == SPEAR_ON_LINE)
			{
				u16Value = 0;
			}
			else
			{
				u16Value = 1;  //提示插入A枪
			}
		}
		break;	

	case MDB_PAGE_ACCOUNT_CONFIRM_STAT_1098:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA == TRUE)
		{
			u16Value = (UINT16)gs_TelaidianCtrl.AccountRequest[0];
		}
		else if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB == TRUE)
		{
			u16Value = (UINT16)gs_TelaidianCtrl.AccountRequest[1];
		}
		
		break;
	case MDB_PAGE_ACCOUNT_BALANCE_1099:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA == TRUE)
		{
			if(gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_2)
			{
				u16Value = (UINT16)gs_TelaidianCtrl.TelaidianAccountBalace[0];
			}
		}
		else if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB == TRUE)
		{
			if(gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_2)
			{
				u16Value = (UINT16)gs_TelaidianCtrl.TelaidianAccountBalace[1];
			}
		}

		break;

	case MDB_PAGE87_STOP_CHG_TIMEOUT_1061:
	case MDB_PAGE_TIME_OUT_1100:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)((((EV_CHARGE_INFO*)PEInfo)->iPageTimeOut));
		break;

	case MDB_VIEW_PSWD_1101:case (MDB_VIEW_PSWD_1101+1):case (MDB_VIEW_PSWD_1101+2):case (MDB_VIEW_PSWD_1101+3):case (MDB_VIEW_PSWD_1101+4):case (MDB_VIEW_PSWD_1101+5):
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8InputKEY[RegAddr - MDB_VIEW_PSWD_1101]);
		break;
	case MDB_VIEW_PSWD_Result_1107:
		PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
		u16Value = (UINT16)(((BRUSH_INFO*)PEInfo)->u8CARD_Info_Result);
		break;

	case MDB_VIEW_ENERGY_ARGS_1108:
		u16Value =(UINT16)(gTempChargeArgs.fEnergeArgs * 10);
		break;
	case MDB_VIEW_TIME_ARGS_1109:
		u16Value =(UINT16)gTempChargeArgs.fTimeArgs;
		break;	
	case MDB_VIEW_SOC_ARGS_1110:
		u16Value =(UINT16)gTempChargeArgs.fSOCArgs;
		break;

	case MDB_A_RSLT_S_HOUR_1200:
		u16Value = gA_Result.u8Shour;
		break;
	case MDB_A_RSLT_S_MIN_1201:
		u16Value = gA_Result.u8Smin;
		break;
	case MDB_A_RSLT_S_SEC_1202:
		u16Value = gA_Result.u8SSec;
		break;
	case MDB_A_RSLT_E_HOUR_1203:
		u16Value = gA_Result.u8Ehour;
		break;
	case MDB_A_RSLT_E_MIN_1204:
		u16Value = gA_Result.u8Emin;
		break;
	case MDB_A_RSLT_E_SEC_1205:
		u16Value = gA_Result.u8ESec;
		break;
	case MDB_A_RSLT_KEEP_HOUR_1206:
		u16Value = gA_Result.u8Keephour;
		break;
	case MDB_A_RSLT_KEEP_MIN_1207:
		u16Value = gA_Result.u8KeepEmin;
		break;
	case MDB_A_RSLT_KEEP_SEC_1208:
		u16Value = gA_Result.u8KeepESec;
		break;
	case MDB_A_RSLT_S_SOC_1209:
		u16Value = gA_Result.u8S_SOC;
		break;
	case MDB_A_RSLT_E_SOC_1210:
		u16Value = gA_Result.u8E_SOC;
		break;
	case MDB_A_RSLT_ENERGY_1211:
		u16Value = (UINT16)((gEvChargeInfo.fA_WH_EndVal - gEvChargeInfo.fWH_StarVal[GUN_NO_A])*100);
		//u16Value = (UINT16)(gA_Result.fEnergy*100);
		break;
	case MDB_A_RSLT_MONEY_1212:
		u16Value = (UINT16)(gA_Result.fMoney * 100);
		break;
	case MDB_A_RSLT_REASON_3_1213:
		u16Value = gA_Result.u8LastReason;
		break;
	case MDB_A_RSLT_REASON_12_1214:
		u16Value = gA_Result.u16Reason12;
		break;



	case MDB_B_RSLT_S_HOUR_1250:
		u16Value = gB_Result.u8Shour;
		break;
	case MDB_B_RSLT_S_MIN_1251:
		u16Value = gB_Result.u8Smin;
		break;
	case MDB_B_RSLT_S_SEC_1252:
		u16Value = gB_Result.u8SSec;
		break;

	case MDB_B_RSLT_E_HOUR_1253:
		u16Value = gB_Result.u8Ehour;
		break;
	case MDB_B_RSLT_E_MIN_1254:
		 u16Value = gB_Result.u8Emin;
		break;
	case MDB_B_RSLT_E_SEC_1255:
		u16Value = gB_Result.u8ESec;
		break;
	case MDB_B_RSLT_KEEP_HOUR_1256:
		u16Value = gB_Result.u8Keephour;
		break;
	case MDB_B_RSLT_KEEP_MIN_1257:
		u16Value = gB_Result.u8KeepEmin;
		break;
	case MDB_B_RSLT_KEEP_SEC_1258:
		 u16Value = gB_Result.u8KeepESec;
		break;
	case MDB_B_RSLT_S_SOC_1259:
		u16Value = gB_Result.u8S_SOC;
		break;
	case MDB_B_RSLT_E_SOC_1260:
		u16Value = gB_Result.u8E_SOC;
		break;
	case MDB_B_RSLT_ENERGY_1261:
		u16Value = (UINT16)((gEvChargeInfo.fB_WH_EndVal - gEvChargeInfo.fWH_StarVal[GUN_NO_B])*100);
		//u16Value = (UINT16)(gB_Result.fEnergy*100);
		break;
	case MDB_B_RSLT_MONEY_1262:
		u16Value = (UINT16)(gB_Result.fMoney * 100);
		break;
	case MDB_B_RSLT_REASON_3_1263:
		u16Value = (UINT16)(gB_Result.u8LastReason);
		break;
	case MDB_B_RSLT_REASON_12_1264:
		u16Value = (UINT16)(gB_Result.u16Reason12);
		break;

	case MDB_RECORD_NO_1299:
		u16Value = (UINT16)gFlashOptInfo.u32ViewRecordNo;
		break;
	case MDB_RECORD_COST_NO1_1300:
	case MDB_RECORD_COST_NO2_1301:case 1302:case 1303:case 1304:case 1305:case 1306:case 1307:case 1308:case 1309:case 1310:case 1311:case 1312:
	case 1313:case 1314:case 1315:case 1316:case 1317:case 1318:case 1319:case 1320:case 1321:case 1322:case 1323:case 1324:case 1325:case 1326:
	case 1327:case 1328:case 1329:case 1330:case 1331:
		u16Value = (UINT16)ViewCostBuf[RegAddr-MDB_RECORD_COST_NO1_1300];
		break;

	case MDB_RECORD_CARD_NO1_1332:
	case MDB_RECORD_CARD_NO2_1333:case 1334:case 1335:case 1336:case 1337:case 1338:case 1339:case 1340:case 1341:case 1342:case 1343:case 1344:
	case 1345:case 1346:case 1347:case 1348:case 1349:case 1350:case 1351:case 1352:case 1353:case 1354:case 1355:case 1356:case 1357:case 1358:
	case 1359:case 1360:case 1361:case 1362:case 1363:
		u16Value = (UINT16)stViewChargeRecordFlash.u8CARDNo[RegAddr-MDB_RECORD_CARD_NO1_1332];
		break;

	case MDB_RECORD_S_YEAR_1364:
		u16Value = (UINT16)stViewChargeRecordFlash.StartTime.tm_year;
		break;
	case MDB_RECORD_S_MON_1365:
		u16Value = (UINT16)stViewChargeRecordFlash.StartTime.tm_mon;
		break;
	case MDB_RECORD_S_D_1366:
		u16Value = (UINT16)stViewChargeRecordFlash.StartTime.tm_mday;
		break;
	case MDB_RECORD_S_H_1367:
		u16Value = (UINT16)stViewChargeRecordFlash.StartTime.tm_hour;
		break;
	case MDB_RECORD_S_MIN_1368:
		u16Value = (UINT16)stViewChargeRecordFlash.StartTime.tm_min;
		break;

	case MDB_RECORD_E_YEAR_1369:
		u16Value = stViewChargeRecordFlash.StopTime.tm_year;
		break;
	case MDB_RECORD_E_MON_13670:
		u16Value = (UINT16)stViewChargeRecordFlash.StopTime.tm_mon;
		break;
	case MDB_RECORD_E_D_1371:
		u16Value = (UINT16)stViewChargeRecordFlash.StopTime.tm_mday;
		break;
	case MDB_RECORD_E_H_1372:
		u16Value = (UINT16)stViewChargeRecordFlash.StopTime.tm_hour;
		break;
	case MDB_RECORD_E_MIN_1373:
		u16Value = (UINT16)stViewChargeRecordFlash.StopTime.tm_min;
		break;
	case MDB_RECORD_TIME_H_1374:
		u16Value = (UINT16)ViewHour;
		break;
	case MDB_RECORD_TIME_M_1375:
		u16Value = (UINT16)ViewMin;
		break;
	case MDB_RECORD_TIME_S_1376:
		u16Value = (UINT16)ViewSec;
		break;
	case MDB_RECORD_S_WH_1377:
		u16Value = (UINT16)(stViewChargeRecordFlash.fStartWHValue* 100);
		break;
	case MDB_RECORD_E_WH_1378:
		u16Value = (UINT16)(stViewChargeRecordFlash.fStopWHValue* 100);
		break;
	case MDB_RECORD_OUT_ENERGY_1379:
		u16Value = (UINT16)((stViewChargeRecordFlash.fStopWHValue - stViewChargeRecordFlash.fStartWHValue) * 100);
		break;
	case MDB_RECORD_TYPE_1380:
		u16Value = (UINT16)stViewChargeRecordFlash.u8LogInType;
		break;

/************************************	充电机设置信号***************************************************************/
	case MDB_ADDR_fSysSetOverVolt_2500:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fSysSetOverVolt * 10);
		break;
	case MDB_ADDR_fRectLoadStartTime_2501:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fRectLoadStartTime * 10);
		break;		
	case MDB_ADDR_MaxOutCurr_2502:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fMaxOutCurr * 10);
		break;					
	case MDB_ADDR_MaxOutVolt_2503:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fMaxOutVolt * 10);
		break;	
	case MDB_ADDR_fMaxOffLineTime_2504:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fMaxOffLineTime);
		break;
	case MDB_ADDR_u8SetCalcMoneyType_2505:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8SetCalcMoneyType);
		break;	
	case MDB_ADDR_u8PWMPolarityCover_2506:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PWMPolarityCover);
		break;			
	case MDB_ADDR_u8EPOAlmVltLEVEL_2507:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8EPOAlmVltLEVEL);
		break;			
	case MDB_ADDR_u8BrushFactorType_2508:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8BrushFactorType);
		break;				
	case MDB_ADDR_u8EV_StartTemp_2509:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8EV_StartTemp);
		break;				
	case MDB_ADDR_u8EV_StopTemp_2510:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8EV_StopTemp);
		break;	
	case MDB_ADDR_u8NET_OffLine_Charge_Enable_2511:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8NET_OffLine_Charge_Enable);
		break;	
	case MDB_ADDR_u32PowerOnTimes_2512:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u32PowerOnTimes);
		break;	
	case MDB_ADDR_u8NetMask_2494:
		u16Value = (UINT16)(gEvChargeInfo.u8NET_Mask);
		break;
	case MDB_ADDR_SPD_MASK:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8SPDAlmMask);  //SPD告警屏蔽
		break;

	case MDB_ADDR_VERSION:
		//u16Value = (UINT16)(((float)mVERHIGH + (float)mVERLOW / 100)*100);
		u16Value = mVERHIGH*100 + mVERLOW;
		break;

	case MDB_ADDR_SYS_POWER:
		ExGetSigInfo(E_TYPE_SYS, ID_EV_SYS_POWER, &unTempVal, NULL, BY_E2PROM);
		u16Value = unTempVal.chtemp[0];
		break;

	case MDB_ADDR_PARALL_MODE:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->ParallelMode);//
		break;

	case MDB_ADDR_CC1_1Coef_A_2513:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fCC1_1Coef_A * 100);
		break;			
	case MDB_ADDR_CC1_2Coef_A_2540:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fCC1_2Coef_A * 100);
		break;				
	case MDB_ADDR_fBattVolt_1CoefA_2515:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_1CoefA * 100);
		break;				
	case MDB_ADDR_fBattVolt_2CoefA:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_2CoefA * 100);
		break;				
	case MDB_ADDR_u32_A_CTS_DB_T1_2517:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u32_A_CTS_DB_T1);
		break;				
	case MDB_ADDR_u32_A_DB_DB_T2_2518:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u32_A_DB_DB_T2);
		break;			
	case MDB_ADDR_u16SetBCLTimeOutVal_1_2519:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16SetBCLTimeOutVal_1);
		break;		
	case MDB_ADDR_u16SetBRMTimeOutVal_1_2520:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16SetBRMTimeOutVal_1);
		break;			
	case MDB_ADDR_u16SetBCSTimeOutVal_1_2521:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16SetBCSTimeOutVal_1);
		break;		
	case MDB_ADDR_fSetLimitMaxOutCurr_1_2522:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fSetLimitMaxOutCurr_1 * 10);
		break;			
	case MDB_ADDR_u8PolarityAlmMask_1_2523:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PolarityAlmMask_1);
		break;			
	case MDB_ADDR_u8CC1CheckEnable_1_2524:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8CC1CheckEnable_1);
		break;		
	case MDB_ADDR_u8SelfTestEnable_1_2525:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8CC1CheckEnable_1);
		break;			
	case MDB_ADDR_u8MCCBOnCheckEnable_1_2526:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8MCCBOnCheckEnable_1);
		break;		
	case MDB_ADDR_u8OutOverVoltEnable_1_2527:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8OutOverVoltEnable_1);
		break;		
	case MDB_ADDR_u8DoorAlmMask_1_2495:
	case MDB_ADDR_u8DoorAlmMask_1_2528:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DoorAlmMask_1);
		break;				
	case MDB_ADDR_u8HasElec_ALock_1_2529:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8HasElec_ALock);
		break;			
	case MDB_ADDR_u8_A_LOCKType_2530:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8_A_LOCKType);
		break;				
	case MDB_ADDR_u8LOCK_A_DIType_2531:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8LOCK_A_DIType);
		break;			
	case MDB_ADDR_u8_A_ForceVer2011_2532:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8_A_ForceVer2011);
		break;
	case MDB_ADDR1_fACWhCoef_2533:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->fACWhCoef*10);
		break;
	case MDB_ADDR1_u8WH_TYPE_2534:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->u8WH_TYPE);
		break;	
	case MDB_ADDR1_u8WH_ALM_Mask_2535:
		PEInfo = GetgEquipInfo(E_TYPE_WH1);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->u8WH_ALM_Mask);
		break;
	case MDB_ADDR1_u8JYMAlmStatMask_2536:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU1);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8JYMAlmStatMask);
		break;
	case MDB_ADDR1_u8SetJYMCtrlMode_2537:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU1);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8SetJYMCtrlStat[0]);
		break;
	case MDB_ADDR1_u8InsltCheckByVolt_2538:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU1);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8InsltCheckByVolt);
		break;
	case MDB_ADDR1_fSetJYMAlm_Resist_2539:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU1);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->fSetJYMAlm_Resist[0] * 10);
		break;
	case MDB_ADDR_CC1_1Coef_B_2514:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fCC1_1Coef_B * 10);
		break;
	case MDB_ADDR_CC1_2Coef_B_2541:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fCC1_2Coef_B * 10);
		break;
	case MDB_ADDR_fBattVolt_1CoefB_2516:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_1CoefB * 10);
		break;
	case MDB_ADDR_fBattVolt_2CoefB:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fBattVolt_2CoefB * 10);
		break;
	case MDB_ADDR_u32_B_CTS_DB_T1:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u32_B_CTS_DB_T1);
		break;
	case MDB_ADDR_u32_B_DB_DB_T2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u32_B_DB_DB_T2);
		break;
	case MDB_ADDR_u16SetBCLTimeOutVal_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16SetBCLTimeOutVal_2);
		break;
	case MDB_ADDR_u16SetBRMTimeOutVal_2_2547:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16SetBRMTimeOutVal_2);
		break;
	case MDB_ADDR_u16SetBCSTimeOutVal_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16SetBCSTimeOutVal_2);
		break;
	case MDB_ADDR_fSetLimitMaxOutCurr_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->fSetLimitMaxOutCurr_2 * 10);
		break;
	case MDB_ADDR_u8PolarityAlmMask_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PolarityAlmMask_2);
		break;
	case MDB_ADDR_u8CC1CheckEnable_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8CC1CheckEnable_2);
		break;
	case MDB_ADDR_u8SelfTestEnable_2_2552:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8SelfTestEnable_2);
		break;
	case MDB_ADDR_u8MCCBOnCheckEnable_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8MCCBOnCheckEnable_2);
		break;
	case MDB_ADDR_u8OutOverVoltEnable_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8OutOverVoltEnable_2);
		break;
	case MDB_ADDR_u8DoorAlmMask_2:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DoorAlmMask_2);
		break;
	case MDB_ADDR_u8HasElec_ALock_2_2556:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8HasElec_BLock);
		break;		
	case MDB_ADDR_u8_B_LOCKType:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8_B_LOCKType);
		break;				
	case MDB_ADDR_u8LOCK_B_DIType:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8LOCK_B_DIType);
		break;			
	case MDB_ADDR_u8_B_ForceVer2011:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8_B_ForceVer2011);
		break;			
	case MDB_ADDR2_fACWhCoef:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->fACWhCoef * 10);
		break;
	case MDB_ADDR2_u8WH_TYPE:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->u8WH_TYPE);
		break;
	case MDB_ADDR2_u8WH_ALM_Mask:
		PEInfo = GetgEquipInfo(E_TYPE_WH2);
		u16Value = (UINT16)(((stWHInfo*)PEInfo)->u8WH_ALM_Mask);
		break;		
	case MDB_ADDR2_u8JYMAlmStatMask_2563:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU2);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8JYMAlmStatMask);
		break;	
	case MDB_ADDR2_u8SetJYMCtrlMode:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU2);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8SetJYMCtrlStat[0]);
		break;
	case MDB_ADDR2_u8InsltCheckByVolt:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU2);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->u8InsltCheckByVolt);
		break;
	case MDB_ADDR2_fSetJYMAlm_Resist_2566:
		PEInfo = GetgEquipInfo(E_TYPE_IMIU2);
		u16Value = (UINT16)(((stIMIU_Info*)PEInfo)->fSetJYMAlm_Resist[0] * 10);
		break;
	case MDB_ADDR1_BMS_POWER_2567:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8BMS_1_PowerType);
		break;
	case MDB_ADDR2_BMS_POWER_2568:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8BMS_2_PowerType);
		break;

	case MDB_ADDR_DHCP_VAL_2650:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		u16Value = unTempVal.chtemp[0];
		break;
	case MDB_ADDR_MAC1_2651:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[5]);
		break;
	case MDB_ADDR_MAC2_2652:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[4]);
		break;
	case MDB_ADDR_MAC3_2653:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[3]);
		break;
	case MDB_ADDR_MAC4_2654:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[2]);
		break;
	case MDB_ADDR_MAC5_2655:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[1]);
		break;
	case MDB_ADDR_MAC6_2656:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[0]);
		break;
	
	case MDB_ADDR_IP1_2657:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8IP_1);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8IP[0];
		}
		break;
	case MDB_ADDR_IP2_2658:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8IP_2);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8IP[1];
		}
		break;
	case MDB_ADDR_IP3_2659:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8IP_3);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8IP[2];
		}

		break;
	case MDB_ADDR_IP4_2660:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8IP_4);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8IP[3];
		}

		break;
	case MDB_ADDR_MASK1_2661:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Subnetmask[0]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Mask[0];
		}
		break;

	case MDB_ADDR_MASK2_2662:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Subnetmask[1]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Mask[1];
		}
		break;
	case MDB_ADDR_MASK3_2663:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Subnetmask[2]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Mask[2];
		}
		break;

	case MDB_ADDR_MASK4_2664:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Subnetmask[3]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Mask[3];
		}
		break;
	case MDB_ADDR_Gateway1_2665:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Gateway[0]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Gateway[0];
		}
		break;
	case MDB_ADDR_Gateway2_2666:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Gateway[1]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Gateway[1];
		}
		break;

	case MDB_ADDR_Gateway3_2667:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Gateway[2]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Gateway[2];
		}
		break;

	case MDB_ADDR_Gateway4_2668:
		ExGetSigInfo(E_TYPE_SYS, ID_NET_DHCP, &unTempVal, NULL, BY_E2PROM);
		if (unTempVal.chtemp[0] == 0)
		{
			PEInfo = GetgEquipInfo(E_TYPE_SYS);
			u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Gateway[3]);
		}
		else
		{
			u16Value = gEvChargeInfo.stDHCPNetInfo.u8Gateway[3];
		}
		break;

	case MDB_ADDR_PORTNO_2800:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u32NET_PORT);
		break;

	case MDB_ADDR_ServerIp1_2801:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PowerStarIP[0]);
		break;
	case MDB_ADDR_ServerIp2_2802:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PowerStarIP[1]);
		break;
	case MDB_ADDR_ServerIp3_2803:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PowerStarIP[2]);
		break;
	case MDB_ADDR_ServerIp4_2804:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8PowerStarIP[3]);
		break;
	case MDB_ADDR_DNSIp1_2805:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DNSIp[0]);
		break;
	case MDB_ADDR_DNSIp2_2806:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DNSIp[1]);
		break;
	case MDB_ADDR_DNSIp3_2807:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DNSIp[2]);
		break;
	case MDB_ADDR_DNSIp4_2808:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8DNSIp[3]);
		break;
	case MDB_ADDR_RemoteServerIsDomainName_2809:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8RemoteServerIsDomainName);
		break;
	case MDB_ADDR_ServerName_2810:case (MDB_ADDR_ServerName_2810+1):case (MDB_ADDR_ServerName_2810+2):case (MDB_ADDR_ServerName_2810+3):case (MDB_ADDR_ServerName_2810+4):case (MDB_ADDR_ServerName_2810+5):
	case (MDB_ADDR_ServerName_2810+6):case (MDB_ADDR_ServerName_2810+7):case (MDB_ADDR_ServerName_2810+8):case (MDB_ADDR_ServerName_2810+9):case (MDB_ADDR_ServerName_2810+10):case (MDB_ADDR_ServerName_2810+11):
	case (MDB_ADDR_ServerName_2810+12):case (MDB_ADDR_ServerName_2810+13):case (MDB_ADDR_ServerName_2810+14):case (MDB_ADDR_ServerName_2810+15):case (MDB_ADDR_ServerName_2810+16):case (MDB_ADDR_ServerName_2810+17):
	case (MDB_ADDR_ServerName_2810+18):case (MDB_ADDR_ServerName_2810+19):case (MDB_ADDR_ServerName_2810+20):case (MDB_ADDR_ServerName_2810+21):case (MDB_ADDR_ServerName_2810+22):case (MDB_ADDR_ServerName_2810+23):
	case (MDB_ADDR_ServerName_2810+24):case (MDB_ADDR_ServerName_2810+25):case (MDB_ADDR_ServerName_2810+26):case (MDB_ADDR_ServerName_2810+27):case (MDB_ADDR_ServerName_2810+28):case (MDB_ADDR_ServerName_2810+29):
	case (MDB_ADDR_ServerName_2810+30):case (MDB_ADDR_ServerName_2810+31):
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		u16Value = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u16ServerName[RegAddr - MDB_ADDR_ServerName_2810]);
		break;

	case MDB_BASE_PAGE_ADDR_2700:
		u16Value = gHMIData.unPageAddr.u16BasePageAddr;
		break;
	case MDB_A_PAGE_ADDR_2702:
		u16Value = gHMIData.unPageAddr.u16_A_PageAddr;
		break;
	case MDB_B_PAGE_ADDR_2704:
		u16Value = gHMIData.unPageAddr.u16_B_PageAddr;
		break;

	default:

		if((RegAddr >= MDB_ADDR_TelaidianStarionID_3000) && (RegAddr <= MDB_ADDR_TelaidianStarionID_3031))
		{
			ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianStarionID_NUM_1 + RegAddr - MDB_ADDR_TelaidianStarionID_3000, &unTempVal, NULL, BY_E2PROM);
			u16Value = unTempVal.chtemp[0];
		}
		else if((RegAddr >= MDB_ADDR_TelaidianUserName_2900) && (RegAddr <= MDB_ADDR_TelaidianUserName_2931))
		{
			ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianUserName_NUM_1 + RegAddr - MDB_ADDR_TelaidianUserName_2900, &unTempVal, NULL, BY_E2PROM);
			u16Value = unTempVal.chtemp[0];
		}
		else if((RegAddr >= MDB_ADDR_TelaidianPassword_2932) && (RegAddr <= MDB_ADDR_TelaidianPassword_2964))
		{
			ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianPassword_NUM_1 + RegAddr - MDB_ADDR_TelaidianPassword_2932, &unTempVal, NULL, BY_E2PROM);
			u16Value = unTempVal.chtemp[0];
		}
		else if((RegAddr >= MDB_ADDR_StationCode_3032) && (RegAddr <= MDB_ADDR_StationCode_3091))
		{
			ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + RegAddr - MDB_ADDR_StationCode_3032, &unTempVal, NULL, BY_E2PROM);
			u16Value = unTempVal.chtemp[0];
		}
			
		break;
	}
	return u16Value;
}

UINT8 u8CompareLocalPSWD(void)
{
	UINT8 uRst = TRUE;
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);	 //EV_CHARGE_INFO
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	for (UINT8 u8Idx = 0; u8Idx < EV_OFF_LINE_PSWD; u8Idx++)
	{
		if (((EV_CHARGE_INFO*)PChargeInfo)->u16UserPassword[0][u8Idx] != ((EV_CHARGE_INFO*)PChargeInfo)->u8InputKEY[u8Idx])
		{
			uRst = FALSE;
		}
	}
	if (!uRst)
	{
		PBrushInfo->u8CARD_Info_Result = 1;																								//提示密码错误
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void PAGE_76_KeyBord_NETDeal(void)
{
	HMIKeyClean();
	gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
	return;
}

void Page_76_KeBoard_KEYDeal(UINT16 u16KeyVal)
{
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);		//EV_CHARGE_INFO
	//键盘		确定键！
	if (MDB_KEY_ENT_2023 == u16KeyVal)					
	{					
		if (((EV_CHARGE_INFO*)PChargeInfo)->u8NetCommAlm == TRUE)
		{
			if (u8CompareLocalPSWD())
			{
				HMIKeyClean();
				gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
			}
		}
		else
		{
			gEvChargeInfo.u8PSWD_0x70_Flag = TRUE;
		}
	}
	//if (MDB_KEY_CANCEL_GO_A_B_GET_2020 == u16KeyVal)
	//{
	//	gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
	//	RefreshQRCode();
	//	HMIKeyClean();
	//}
}

void Page_77_Business_KEYDeal(UINT16 u16KeyVal)
{
	UINT8 u8SpearIdlNum = 0;
	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_BUSINESS_SELECT_77)
	{
		return;
	}
	if (MDB_KEY_BMS_AUTO_2024 == u16KeyVal)	
	{
		//枪选择
		u8SpearIdlNum = CheckValidSpear();
		if (0 == u8SpearIdlNum)
		{
			//无可用枪！	不操作！
		}
		else if (u8SpearIdlNum > 2)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
		}
		else
		{
			//只有一个！
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪
		}
	}

	if (MDB_KEY_OTHER_MODE_2027 == u16KeyVal)							//其他方式
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_OTHER_CHARGE_MODE_79;
	}

	//if (MDB_KEY_CANCEL_GO_A_B_GET_2020 == u16KeyVal)
	//{
	//	gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
	//	RefreshQRCode();
	//}
}

void  Page_79_SelectChargeMode_KEYDeal(UINT16 u16KeyVal)
{
	UINT8 u8SpearIdlNum = 0;
	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_OTHER_CHARGE_MODE_79)
	{
		return;
	}
	if (MDB_KEY_BMS_TIME_MODE_2028 == u16KeyVal)		//进81界面
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_TIME_ARGS_81;
	}

	if (MDB_KEY_BMS_ENERGE_MODE_2029 == u16KeyVal)		//进80界面
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_ENERGE_ARGS_80;
	}

	if (MDB_KEY_BMS_SOC_MODE_2030 == u16KeyVal)			//进82界面
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_SOC_ARGS_82;
	}
	if (MDB_KEY_BMS_AUTO_2024 == u16KeyVal)	
	{
		//枪选择
		u8SpearIdlNum = CheckValidSpear();
		if (0 == u8SpearIdlNum)
		{
			//无可用枪！	不操作！
		}
		else if (u8SpearIdlNum > 2)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
		}
		else
		{
			//只有一个！
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪
		}
	}
	if (MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
	}
}


void  Page_80_EnergeMode_KEYDeal(UINT16 u16KeyVal)
{
	UINT8 u8SpearIdlNum = 0;
	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_ENERGE_ARGS_80)
	{
		return;
	}
	if (MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
	}
	//		SOC 确定		时间确定		电量确定
	if ((MDB_KEY_SOC_TIME_ENERGE_CONFIRM_2044 == u16KeyVal))			
	{
		//枪选择
		u8SpearIdlNum = CheckValidSpear();
		if (0 == u8SpearIdlNum)
		{
			//无可用枪！	不操作！
		}
		else if (u8SpearIdlNum > 2)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
		}
		else
		{
			//只有一个！
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪
		}
	}
}


void  Page_81_TimeMode_KEYDeal(UINT16 u16KeyVal)
{
	UINT8 u8SpearIdlNum = 0;
	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_TIME_ARGS_81)
	{
		return;
	}
	if (MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
	}
	//		SOC 确定		时间确定		电量确定
	if ((MDB_KEY_SOC_TIME_ENERGE_CONFIRM_2044 == u16KeyVal))			
	{
		//枪选择
		u8SpearIdlNum = CheckValidSpear();
		if (0 == u8SpearIdlNum)
		{
			//无可用枪！	不操作！
		}
		else if (u8SpearIdlNum > 2)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
		}
		else
		{
			//只有一个！
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪
		}
	}
}


void  Page_78_A_B_SpearSelect_KEYDeal(UINT16 u16KeyVal)
{
	//UINT8 u8SpearIdlNum = 0;
	void *PEInfo = GetgEquipInfo(E_TYPE_SYS);

	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_SELECT_SPEAR_78)
	{
		return;
	}
	if (MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_HOME_NO_11;
		((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA = FALSE;
		((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB = FALSE;
	}
	if (MDB_KEY_SPEAR_CONFIRM_2045 == u16KeyVal )
	{
		//因为只有刷卡才有78 这个界面提示。
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
		if(((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA == TRUE)
		{
			gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x01;
			gs_TelaidianCtrl.AccountRequest[0] = 0;
		}
		else
		{
			gEvChargeInfo.u8_B_LogInMother = EV_ENTER_IC_CARD;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x02;
			gs_TelaidianCtrl.AccountRequest[1] = 0;
		}
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪
	}
}


void  Page_82_SocMode_KEYDeal(UINT16 u16KeyVal)
{
	UINT8 u8SpearIdlNum = 0;
	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_SOC_ARGS_82)
	{
		return;
	}
	if (MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
	}
	//		SOC 确定		时间确定		电量确定
	if ((MDB_KEY_SOC_TIME_ENERGE_CONFIRM_2044 == u16KeyVal))			
	{
		//枪选择
		u8SpearIdlNum = CheckValidSpear();
		if (0 == u8SpearIdlNum)
		{
			//无可用枪！	不操作！
		}
		else if (u8SpearIdlNum > 2)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
		}
		else
		{
			//只有一个！
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪
		}
	}
}

void  Page_83_WaitConnectSpear_KEYDeal(UINT16 u16KeyVal)
{
	//UINT8 u8SpearIdlNum = 0;
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);		//EV_CHARGE_INFO
	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_NOTE_PLUG_IN_83)
	{
		return;
	}
	if (MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043 == u16KeyVal)
	{
		//gHMIData.unPageAddr.u16BasePageAddr = PAGE_BUSINESS_SELECT_77;
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;

		if(gEvChargeInfo.u8IsOptionB == TRUE)
		{
			gEvChargeInfo.u8IsOptionB = FALSE;
			//gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
		}
		if(gEvChargeInfo.u8IsOptionA == TRUE)
		{
			gEvChargeInfo.u8IsOptionA = FALSE;
			//gHMIData.unPageAddr.u16_B_PageAddr = PAGE_A_QR_CODE_20;
		}
	}
	
	/*if (PChargeInfo->u8EV_SpearPst[GUN_NO_B] == SPEAR_ON_LINE
		&& gEvChargeInfo.u8IsOptionB == TRUE)
	{
		gEvChargeInfo.u8_B_LogInMother = EV_ENTER_IC_CARD;
		B_SetChargeSTART_Cmd(EV_ENTER_IC_CARD);
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
		gEvChargeInfo.u8IsOptionB = FALSE;
		gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_CHARGE_36;
	}

	if (PChargeInfo->u8EV_SpearPst[GUN_NO_A] == SPEAR_ON_LINE
		&& gEvChargeInfo.u8IsOptionA == TRUE)
	{
		gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
		A_SetChargeSTART_Cmd(EV_ENTER_IC_CARD);
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
		gEvChargeInfo.u8IsOptionA = FALSE;
		gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_CHARGE_21;
	}*/
}

void HMIPageDeal(UINT16 u16KeyVal)
{
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);		//EV_CHARGE_INFO
	//UINT8 u8A_WorkStat;
	//UINT8 u8B_WorkStat;
	//UINT8 u8SpearIdlNum;

	Page_76_KeBoard_KEYDeal(u16KeyVal);

	Page_77_Business_KEYDeal(u16KeyVal);

	Page_79_SelectChargeMode_KEYDeal(u16KeyVal);

	Page_80_EnergeMode_KEYDeal(u16KeyVal);

	Page_81_TimeMode_KEYDeal(u16KeyVal);

	Page_82_SocMode_KEYDeal(u16KeyVal);

	Page_78_A_B_SpearSelect_KEYDeal(u16KeyVal);

	Page_83_WaitConnectSpear_KEYDeal(u16KeyVal);


	if (BUTTONS_MaintenanceLogIn_2709 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_MaintenanceLogIn_75;
	}

	if (BUTTONS_Go_HOME_2710 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_HOME_NO_11;	   //
		RefreshQRCode();
	}
	if (BUTTONS_Go_Sys_PARAM_2711 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_SYS_PARAM_201;
	}
	if (BUTTONS_A_PARAM_2712 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_PARAM_202;
	}
	if (BUTTONS_B_PARAM_2713 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_B_PARAM_203;
	}
	if (BUTTONS_A_ADJUST_BUS_2714 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_BUS_ADJUST_204;
	}
	if (BUTTONS_B_ADJUST_BUS_2715 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_B_BUS_ADJUST_205;
	}

	if (u16KeyVal == BUTTONS_Go_Sys_PARAM_2_2717)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_SYS_PARAM_2_211;
	}

	if (BUTTONS_Go_TestCharge_2716 == u16KeyVal)
	{
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_TEST_CHARGE_200;
	}
}

UINT16 u16Fun05SetSignal(UINT16 RegAddr,UINT16 u16Value)
{
	UNUSED(RegAddr);
	UNUSED(u16Value);
	UINT8 u8Idx = 0;
	INT32 CSecond = 0;
	time_t now;
	time_t now2;
	struct tm timeNow;
	//__XXX_UNION_VALUE unVal;
	void* PEInfo;
	switch (RegAddr)
	{
	case MDB_A_CHARGE_CMD_2000:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL)
		{
			gEvChargeInfo.u8_A_LogInMother = EV_ENTER_admin;
			A_SetChargeSTART_Cmd(EV_ENTER_admin);
		}
		else
		{
			A_SetChargeSTOP_Cmd(LUI_STOP_CMD);
		}
		break;
	case MDB_B_CHARGE_CMD_2001:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
		{
			gEvChargeInfo.u8_B_LogInMother = EV_ENTER_admin;
			B_SetChargeSTART_Cmd(EV_ENTER_admin);
		}
		else
		{
			B_SetChargeSTOP_Cmd(LUI_STOP_CMD);
		}
		break;

	case MDB_A_TEST_MODE_2005:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->u8_A_TestMode == TRUE)
		{
			((EV_CHARGE_INFO*)PEInfo)->u8_A_TestMode = FALSE;
			gs_CANComm.fSetRectCtlStatus(GRP_A, RECT_OFF);
			__A_DC_MCCB_OFF();
		}
		else
		{
			((EV_CHARGE_INFO*)PEInfo)->u8_A_TestMode = TRUE;

			PEInfo = GetgEquipInfo(E_TYPE_BMS1);
			//((stBMS_INFO*)PEInfo)->fBMSRequireCurr = u16Value * 0.1;
			gs_CANComm.fSetRectCtlVoltCurr(GRP_A, ((stBMS_INFO*)PEInfo)->fBMSRequireVolt, ((stBMS_INFO*)PEInfo)->fBMSRequireCurr);
			gs_CANComm.fSetRectCtlStatus(GRP_A, RECT_ON);
			__A_DC_MCCB_ON();
		}
		break;
	case MDB_B_TEST_MODE_2006:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->u8_B_TestMode == TRUE)
		{
			((EV_CHARGE_INFO*)PEInfo)->u8_B_TestMode = FALSE;
			gs_CANComm.fSetRectCtlStatus(GRP_B, RECT_OFF);
			__B_DC_MCCB_OFF();
		}
		else
		{
			((EV_CHARGE_INFO*)PEInfo)->u8_B_TestMode = TRUE;

			PEInfo = GetgEquipInfo(E_TYPE_BMS2);
			//((stBMS_INFO*)PEInfo)->fBMSRequireCurr = u16Value * 0.1;
			gs_CANComm.fSetRectCtlVoltCurr(GRP_B, ((stBMS_INFO*)PEInfo)->fBMSRequireVolt, ((stBMS_INFO*)PEInfo)->fBMSRequireCurr);
			gs_CANComm.fSetRectCtlStatus(GRP_B, RECT_ON);
			__B_DC_MCCB_ON();
		}
		break;

	case MDB_KEY_0_2010:case MDB_KEY_1_2011:case MDB_KEY_2_2012:case MDB_KEY_3_2013:case MDB_KEY_4_2014:case MDB_KEY_5_2015:
	case MDB_KEY_6_2016:case MDB_KEY_7_2017:case MDB_KEY_8_2018:case MDB_KEY_9_2019:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		if (((EV_CHARGE_INFO*)PEInfo)->u8InputKEY_Idx < HMI_KEY_MAX_NUM)
		{
			((EV_CHARGE_INFO*)PEInfo)->u8InputKEY[((EV_CHARGE_INFO*)PEInfo)->u8InputKEY_Idx] = '0' + (RegAddr - MDB_KEY_0_2010);
			((EV_CHARGE_INFO*)PEInfo)->u8InputKEY_Idx++;
		}
		break;

	case MDB_KEY_CANCEL_GO_A_B_GET_2020:
		//HMIPageDeal(MDB_KEY_CANCEL_GO_A_B_GET_2020);
		gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
		RefreshQRCode();
		HMIKeyClean();
		break;
	case MDB_KEY_BACK_2021:
		HMIKeyBack();
		break;
	case MDB_KEY_CLEAN_2022:
		HMIKeyClean();
		break;
	case MDB_KEY_ENT_2023:
		HMIPageDeal(MDB_KEY_ENT_2023);
		break;

	case MDB_KEY_BMS_AUTO_2024:			
		gTempChargeArgs.u8ChargeMode = BMS_AUTO;
		HMIPageDeal(MDB_KEY_BMS_AUTO_2024);
		break;

	case MDB_KEY_A_SELECT_2025:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA = TRUE;
		((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB = FALSE;
		break;

	case MDB_KEY_B_SELECT_2026:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		((EV_CHARGE_INFO*)PEInfo)->u8IsOptionB = TRUE;
		((EV_CHARGE_INFO*)PEInfo)->u8IsOptionA = FALSE;
		break;

	case MDB_KEY_OTHER_MODE_2027:					//进79界面
		gTempChargeArgs.u8ChargeMode = BMS_TIME;
		HMIPageDeal(MDB_KEY_OTHER_MODE_2027);
		break;			
	case MDB_KEY_BMS_TIME_MODE_2028:				//进81界面
		HMIPageDeal(MDB_KEY_BMS_TIME_MODE_2028);
		break;
	case MDB_KEY_BMS_ENERGE_MODE_2029:				//进80界面
		HMIPageDeal(MDB_KEY_BMS_ENERGE_MODE_2029);
		break;
	case MDB_KEY_BMS_SOC_MODE_2030:					//进82界面
		HMIPageDeal(MDB_KEY_BMS_SOC_MODE_2030);
		break;
	case MDB_KEY_BMS_ENERGE_10_2031:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		gTempChargeArgs.fEnergeArgs	 = 10;
		gTempChargeArgs.u8ChargeMode = BMS_ENERGY;
		break;
	case MDB_KEY_BMS_ENERGE_15_2032:
		gTempChargeArgs.fEnergeArgs	 = 15;
		gTempChargeArgs.u8ChargeMode = BMS_ENERGY;
		break;
	case MDB_KEY_BMS_ENERGE_20_2033:
		gTempChargeArgs.fEnergeArgs	 = 20;
		gTempChargeArgs.u8ChargeMode = BMS_ENERGY;
		break;
	case MDB_KEY_BMS_ENERGE_25_2034:
		gTempChargeArgs.fEnergeArgs	 = 25;
		gTempChargeArgs.u8ChargeMode = BMS_ENERGY;
		break;
	case MDB_KEY_BMS_TIME_30_2035:
		gTempChargeArgs.fTimeArgs = 30;
		gTempChargeArgs.u8ChargeMode = BMS_TIME;
		break;
	case MDB_KEY_BMS_TIME_60_2036:
		gTempChargeArgs.fTimeArgs = 60;
		gTempChargeArgs.u8ChargeMode = BMS_TIME;
		break;
	case MDB_KEY_BMS_TIME_120_2037:
		gTempChargeArgs.fTimeArgs = 120;
		gTempChargeArgs.u8ChargeMode = BMS_TIME;
		break;
	case MDB_KEY_BMS_TIME_180_2038:
		gTempChargeArgs.fTimeArgs = 180;
		gTempChargeArgs.u8ChargeMode = BMS_TIME;
		break;
	case MDB_KEY_BMS_SOC_70_2039:
		gTempChargeArgs.fSOCArgs = 70;
		gTempChargeArgs.u8ChargeMode = BMS_SOC;
		break;
	case MDB_KEY_BMS_SOC_80_2040:
		gTempChargeArgs.fSOCArgs = 80;
		gTempChargeArgs.u8ChargeMode = BMS_SOC;
		break;
	case MDB_KEY_BMS_SOC_90_2041:
		gTempChargeArgs.fSOCArgs = 90;
		gTempChargeArgs.u8ChargeMode = BMS_SOC;
		break;
	case MDB_KEY_BMS_SOC_100_2042:
		gTempChargeArgs.fSOCArgs = 100;
		gTempChargeArgs.u8ChargeMode = BMS_SOC;
		break;
	case MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043:
		HMIPageDeal(MDB_KEY_GO_PAGE_BUSINESS_SELECT_2043);
		break;
	case MDB_KEY_SOC_TIME_ENERGE_CONFIRM_2044:
		HMIPageDeal(MDB_KEY_SOC_TIME_ENERGE_CONFIRM_2044);
		break;
	case MDB_KEY_SPEAR_CONFIRM_2045:
		HMIPageDeal(MDB_KEY_SPEAR_CONFIRM_2045);
		break;


	case MDB_TEST_PAGE_ADDR_2706: //2703
		HMIPageDeal(MDB_TEST_PAGE_ADDR_2706);
		break;
	case MDB_TEST_2_PAGE_ADDR_2708:   //2704
		HMIPageDeal(MDB_TEST_2_PAGE_ADDR_2708);
		break;
	case BUTTONS_MaintenanceLogIn_2709:
		HMIPageDeal(BUTTONS_MaintenanceLogIn_2709);
		break;
	case BUTTONS_Go_HOME_2710:
		//gHMIData.unPageAddr.u16BasePageAddr = PAGE_HOME_NO_11;
		//RefreshQRCode();
		HMIPageDeal(BUTTONS_Go_HOME_2710);
		break;
	case BUTTONS_Go_Sys_PARAM_2711:
		HMIPageDeal(BUTTONS_Go_Sys_PARAM_2711);
		break;
	case BUTTONS_A_PARAM_2712:
		HMIPageDeal(BUTTONS_A_PARAM_2712);
		break;
	case BUTTONS_B_PARAM_2713:
		HMIPageDeal(BUTTONS_B_PARAM_2713);
		break;
	case BUTTONS_A_ADJUST_BUS_2714:
		HMIPageDeal(BUTTONS_A_ADJUST_BUS_2714);
		break;
	case BUTTONS_B_ADJUST_BUS_2715:
		HMIPageDeal(BUTTONS_B_ADJUST_BUS_2715);
		break;
	case BUTTONS_Go_TestCharge_2716:
		HMIPageDeal(BUTTONS_Go_TestCharge_2716);
		break;
	case BUTTONS_Go_Sys_PARAM_2_2717:
		HMIPageDeal(BUTTONS_Go_Sys_PARAM_2_2717);
		break;

	case BUTTONS_GET_RECORD_LOG_2718:
		if (gChargeLogTable.u32TotalNum > 1)
		{
			gFlashOptInfo.u32ViewRecordNo = gChargeLogTable.u32TotalNum;
		}
		else
		{
			gFlashOptInfo.u32ViewRecordNo = 1;
		}

		gFlashOptInfo.u32ViewPageIdx = FLASH_GetPageIdx(gFlashOptInfo.u32ViewRecordNo);
		FLASH_ReadRecords(gFlashOptInfo.u32ViewPageIdx*BYTES_NUM_PER_PAGE, &stViewChargeRecordFlash.u8SpearType,sizeof(stViewChargeRecordFlash)/sizeof(UINT8));
		memset(ViewCostBuf, 0, sizeof(ViewCostBuf));
		for (u8Idx = 0; u8Idx < 9; u8Idx++)
		{
			ViewCostBuf[u8Idx*2] = Hex2Ascii(0x0f&(stViewChargeRecordFlash.u8CostNo[u8Idx] >>4));
			ViewCostBuf[u8Idx*2 + 1] = Hex2Ascii(0x0f&(stViewChargeRecordFlash.u8CostNo[u8Idx]));
		}
		
		timeNow.tm_year = stViewChargeRecordFlash.StopTime.tm_year - YEAR0;
		timeNow.tm_mon = stViewChargeRecordFlash.StopTime.tm_mon;
		timeNow.tm_mday = stViewChargeRecordFlash.StopTime.tm_mday;
		timeNow.tm_hour = stViewChargeRecordFlash.StopTime.tm_hour;
		timeNow.tm_min = stViewChargeRecordFlash.StopTime.tm_min;
		timeNow.tm_sec = stViewChargeRecordFlash.StopTime.tm_sec;
		now = mktime_r(&timeNow);
		timeNow.tm_year = stViewChargeRecordFlash.StartTime.tm_year - YEAR0;
		timeNow.tm_mon = stViewChargeRecordFlash.StartTime.tm_mon;
		timeNow.tm_mday = stViewChargeRecordFlash.StartTime.tm_mday;
		timeNow.tm_hour = stViewChargeRecordFlash.StartTime.tm_hour;
		timeNow.tm_min = stViewChargeRecordFlash.StartTime.tm_min;
		timeNow.tm_sec = stViewChargeRecordFlash.StartTime.tm_sec;
		now2 = mktime_r(&timeNow);
		CSecond = now - now2;
		if (CSecond <= 0)
		{
			CSecond = 0;
		}
		ViewHour = CSecond/3600;
		ViewMin = (CSecond - (ViewHour*3600))/60;
		ViewSec = CSecond%60;
		break;
	

	case BUTTONS_GET_NEXT_RECORD_LOG_2719:	//《充电记录查询》111界面 “下一项”

		/*gFlashOptInfo.u32ViewRecordNo++;
		if (gFlashOptInfo.u32ViewRecordNo > (gChargeLogTable.u32TotalNum))
		{
			gFlashOptInfo.u32ViewRecordNo = 1;
		}*/
		gFlashOptInfo.u32ViewRecordNo++;
		if (gFlashOptInfo.u32ViewRecordNo > (gChargeLogTable.u32TotalNum))
		{
			if(gChargeLogTable.u32TotalNum > MAX_RECORD_NUMBER)
			{
				gFlashOptInfo.u32ViewRecordNo = gChargeLogTable.u32TotalNum - MAX_RECORD_NUMBER + 1;
			}
			else
			{
				gFlashOptInfo.u32ViewRecordNo = 1;
			}
		}

		gFlashOptInfo.u32ViewPageIdx = FLASH_GetPageIdx(gFlashOptInfo.u32ViewRecordNo);
		FLASH_ReadRecords(gFlashOptInfo.u32ViewPageIdx*BYTES_NUM_PER_PAGE, &stViewChargeRecordFlash.u8SpearType,sizeof(stViewChargeRecordFlash)/sizeof(UINT8));
		memset(ViewCostBuf, 0, sizeof(ViewCostBuf));
		for (u8Idx = 0; u8Idx < 9; u8Idx++)
		{
			ViewCostBuf[u8Idx*2] = Hex2Ascii(0x0f&(stViewChargeRecordFlash.u8CostNo[u8Idx] >>4));
			ViewCostBuf[u8Idx*2 + 1] = Hex2Ascii(0x0f&(stViewChargeRecordFlash.u8CostNo[u8Idx]));
		}
		timeNow.tm_year = stViewChargeRecordFlash.StopTime.tm_year - YEAR0;
		timeNow.tm_mon = stViewChargeRecordFlash.StopTime.tm_mon;
		timeNow.tm_mday = stViewChargeRecordFlash.StopTime.tm_mday;
		timeNow.tm_hour = stViewChargeRecordFlash.StopTime.tm_hour;
		timeNow.tm_min = stViewChargeRecordFlash.StopTime.tm_min;
		timeNow.tm_sec = stViewChargeRecordFlash.StopTime.tm_sec;
		now = mktime_r(&timeNow);
		timeNow.tm_year = stViewChargeRecordFlash.StartTime.tm_year - YEAR0;
		timeNow.tm_mon = stViewChargeRecordFlash.StartTime.tm_mon;
		timeNow.tm_mday = stViewChargeRecordFlash.StartTime.tm_mday;
		timeNow.tm_hour = stViewChargeRecordFlash.StartTime.tm_hour;
		timeNow.tm_min = stViewChargeRecordFlash.StartTime.tm_min;
		timeNow.tm_sec = stViewChargeRecordFlash.StartTime.tm_sec;
		now2 = mktime_r(&timeNow);
		CSecond = now - now2;
		if (CSecond <= 0)
		{
			CSecond = 0;
		}
		ViewHour = CSecond/3600;
		ViewMin = (CSecond - (ViewHour*3600))/60;
		ViewSec = CSecond%60;
		break;

	case BUTTONS_GET_PRE_RECORD_LOG_2720:												//《充电记录查询》111界面		“上一项”

		/*if (gChargeLogTable.u32TotalNum > 0)
		{
			gFlashOptInfo.u32ViewRecordNo--;
		}

		if (gFlashOptInfo.u32ViewRecordNo < 1)
		{
			if (gChargeLogTable.u32TotalNum > 1)
			{
				gFlashOptInfo.u32ViewRecordNo = gChargeLogTable.u32TotalNum;
			}
			else
			{
				gFlashOptInfo.u32ViewRecordNo = 1;
			}
		}*/

		if (gChargeLogTable.u32TotalNum > 0)
		{
			gFlashOptInfo.u32ViewRecordNo--;
		}

		if(gChargeLogTable.u32TotalNum > MAX_RECORD_NUMBER)
		{
			if(gFlashOptInfo.u32ViewRecordNo <= (gChargeLogTable.u32TotalNum - MAX_RECORD_NUMBER))
			{
				gFlashOptInfo.u32ViewRecordNo = gChargeLogTable.u32TotalNum;
			}
		}
		else if (gFlashOptInfo.u32ViewRecordNo < 1)
		{
			if (gChargeLogTable.u32TotalNum > 1)
			{
				gFlashOptInfo.u32ViewRecordNo = gChargeLogTable.u32TotalNum;
			}
			else
			{
				gFlashOptInfo.u32ViewRecordNo = 1;
			}
		}
	
		gFlashOptInfo.u32ViewPageIdx = FLASH_GetPageIdx(gFlashOptInfo.u32ViewRecordNo);
		FLASH_ReadRecords(gFlashOptInfo.u32ViewPageIdx*BYTES_NUM_PER_PAGE, &stViewChargeRecordFlash.u8SpearType,sizeof(stViewChargeRecordFlash)/sizeof(UINT8));
		memset(ViewCostBuf, 0, sizeof(ViewCostBuf));
		for (u8Idx = 0; u8Idx < 9; u8Idx++)
		{
			ViewCostBuf[u8Idx*2] = Hex2Ascii(0x0f&(stViewChargeRecordFlash.u8CostNo[u8Idx] >>4));
			ViewCostBuf[u8Idx*2 + 1] = Hex2Ascii(0x0f&(stViewChargeRecordFlash.u8CostNo[u8Idx]));
		}
		timeNow.tm_year = stViewChargeRecordFlash.StopTime.tm_year - YEAR0;
		timeNow.tm_mon = stViewChargeRecordFlash.StopTime.tm_mon;
		timeNow.tm_mday = stViewChargeRecordFlash.StopTime.tm_mday;
		timeNow.tm_hour = stViewChargeRecordFlash.StopTime.tm_hour;
		timeNow.tm_min = stViewChargeRecordFlash.StopTime.tm_min;
		timeNow.tm_sec = stViewChargeRecordFlash.StopTime.tm_sec;
		now = mktime_r(&timeNow);
		timeNow.tm_year = stViewChargeRecordFlash.StartTime.tm_year - YEAR0;
		timeNow.tm_mon = stViewChargeRecordFlash.StartTime.tm_mon;
		timeNow.tm_mday = stViewChargeRecordFlash.StartTime.tm_mday;
		timeNow.tm_hour = stViewChargeRecordFlash.StartTime.tm_hour;
		timeNow.tm_min = stViewChargeRecordFlash.StartTime.tm_min;
		timeNow.tm_sec = stViewChargeRecordFlash.StartTime.tm_sec;
		now2 = mktime_r(&timeNow);
		CSecond = now - now2;
		if (CSecond <= 0)
		{
			CSecond = 0;
		}
		ViewHour = CSecond/3600;
		ViewMin = (CSecond - (ViewHour*3600))/60;
		ViewSec = CSecond%60;
		break;

	case BUTTONS_CLR_RECORD_LOG_2721:
		gFlashOptInfo.u8CleanRecordsFlag = TRUE;
		break;

	/*case BUTTONS_TEST_POP_CARD_2799:
		gBrushInfo.u8TempCARDInfo[0] = '1';
		gBrushInfo.u8TempCARDInfo[1] = '2';
		gBrushInfo.u8TempCARDInfo[2] = '3';
		gBrushInfo.u8TempCARDInfo[3] = '4';
		gBrushInfo.u8TempCARDInfo[4] = '5';
		gEvChargeInfo.u8BrushSem = TRUE;
		break;*/

	default:
		break;
	}
	return 0;
}

UINT16 u16MdbSetSignal(UINT16 RegAddr,UINT16 u16Value)
{
	UNUSED(RegAddr);
	UNUSED(u16Value);
	__XXX_UNION_VALUE unVal;
	struct tm  tmNow;
	void* PEInfo;
	//struct tm timeNow;
	switch (RegAddr)
	{
	case MDB_1_A_SET_REAL_VOLT_14:					
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		((EV_CHARGE_INFO*)PEInfo)->fBattSet_A_Volt = u16Value * 0.1;
		((EV_CHARGE_INFO*)PEInfo)->u8Batt_1VoltSet_A_Flag = TRUE;
		break;
	case MDB_1_B_SET_REAL_VOLT_15:					
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		((EV_CHARGE_INFO*)PEInfo)->fBattSet_B_Volt = u16Value * 0.1;
		((EV_CHARGE_INFO*)PEInfo)->u8Batt_1VoltSet_B_Flag = TRUE;
		break;

	case MDB_2_A_SET_REAL_VOLT_18:					
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		((EV_CHARGE_INFO*)PEInfo)->fBattSet_A_Volt = u16Value * 0.1;
		((EV_CHARGE_INFO*)PEInfo)->u8Batt_2VoltSet_A_Flag = TRUE;
		break;
	case MDB_2_B_SET_REAL_VOLT_19:					
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		((EV_CHARGE_INFO*)PEInfo)->fBattSet_B_Volt = u16Value * 0.1;
		((EV_CHARGE_INFO*)PEInfo)->u8Batt_2VoltSet_B_Flag = TRUE;
		break;
	case MDB_1_REQR_VOLT_103:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		((stBMS_INFO*)PEInfo)->fBMSRequireVolt = u16Value * 0.1;
		gs_CANComm.fSetRectCtlVoltCurr(GRP_A, ((stBMS_INFO*)PEInfo)->fBMSRequireVolt, ((stBMS_INFO*)PEInfo)->fBMSRequireCurr);
		break;
	case MDB_2_REQR_VOLT_503:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		((stBMS_INFO*)PEInfo)->fBMSRequireVolt = u16Value * 0.1;
		gs_CANComm.fSetRectCtlVoltCurr(GRP_B, ((stBMS_INFO*)PEInfo)->fBMSRequireVolt, ((stBMS_INFO*)PEInfo)->fBMSRequireCurr);
		break;
	case MDB_1_REQR_CURR_104:
		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		((stBMS_INFO*)PEInfo)->fBMSRequireCurr = u16Value * 0.1;
		gs_CANComm.fSetRectCtlVoltCurr(GRP_A, ((stBMS_INFO*)PEInfo)->fBMSRequireVolt, ((stBMS_INFO*)PEInfo)->fBMSRequireCurr);
		break;
	case MDB_2_REQR_CURR_504:
		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		((stBMS_INFO*)PEInfo)->fBMSRequireCurr = u16Value * 0.1;
		gs_CANComm.fSetRectCtlVoltCurr(GRP_B, ((stBMS_INFO*)PEInfo)->fBMSRequireVolt, ((stBMS_INFO*)PEInfo)->fBMSRequireCurr);
		break;



	case MDB_EV_CODE_900:case 901:
	case 902:case 903:case 904:case 905:case 906:case 907:case 908:case 909:case 910:case 911:case 912:case 913:case 914:case 915:case 916:
	case 917:case 918:case 919:case 920:case 921:case 922:case 923:case 924:case 925:case 926:case 927:case 928:case 929:case 930:case 931:
	case 932:case 933:case 934:case 935:case 936:case 937:case 938:case 939:case 940:case 941:case 942:case 943:case 944:case 945:case 946:
	case 947:case 948:case 949:case 950:case 951:case 952:case 953:case 954:case 955:case 956:case 957:case 958:case 959:
		//0011223344 --> 0030	0030	0031	0031	0032	0032
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		(((EV_CHARGE_INFO*)PEInfo)->u16EVCode[RegAddr - MDB_EV_CODE_900]) = u16Value;
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + (RegAddr - MDB_EV_CODE_900), unVal, NULL, BY_E2PROM);
		break;

	case MDB_OFFLINE_PSWD_965:case 966:case 967:case 968:case 969:case 970:
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		(((EV_CHARGE_INFO*)PEInfo)->u16UserPassword[0][RegAddr - MDB_OFFLINE_PSWD_965]) = u16Value;
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8OffLinePwsd1 + (RegAddr - MDB_OFFLINE_PSWD_965), unVal, NULL, BY_E2PROM);
		break;

	case MDB_VIEW_ENERGY_ARGS_1108:
		gTempChargeArgs.u8ChargeMode = BMS_ENERGY;
		gTempChargeArgs.fEnergeArgs = u16Value * 0.1;
		break;
	case MDB_VIEW_TIME_ARGS_1109:
		gTempChargeArgs.u8ChargeMode = BMS_TIME;
		gTempChargeArgs.fTimeArgs = u16Value;
		break;	
	case MDB_VIEW_SOC_ARGS_1110:
		gTempChargeArgs.u8ChargeMode = BMS_SOC;
		gTempChargeArgs.fSOCArgs = u16Value;
		break;

	case MDB_ADDR_CC1_1Coef_A_2513:
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fCC1_1Coef_A, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_CC1_1Coef_B_2514:
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fCC1_1Coef_B, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_CC1_2Coef_A_2540:
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fCC1_2Coef_A, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_CC1_2Coef_B_2541:
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fCC1_2Coef_B, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_fSysSetOverVolt_2500:	
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_SYS, ID_fSysSetOverVolt, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_fRectLoadStartTime_2501:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_fRectLoadStartTime, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_MaxOutCurr_2502:
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_SYS, ID_fMaxOutCurr, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_MaxOutVolt_2503:
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_SYS, ID_fMaxOutVolt, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_fBattVolt_1CoefA_2515:
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_1CoefA, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_fBattVolt_1CoefB_2516:	
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_1CoefB, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_fBattVolt_2CoefA:
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_2CoefA, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_fBattVolt_2CoefB:	
		unVal.ftemp = u16Value*0.01;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_2CoefB, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u32_B_CTS_DB_T1:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u32_B_CTS_DB_T1, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u32_B_DB_DB_T2:
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u32_B_DB_DB_T2, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u32_A_CTS_DB_T1_2517:
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u32_A_CTS_DB_T1, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u32_A_DB_DB_T2_2518:
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u32_A_DB_DB_T2, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u16SetBCLTimeOutVal_1_2519:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u16SetBCLTimeOutVal_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u16SetBCLTimeOutVal_2:		
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u16SetBCLTimeOutVal_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u16SetBRMTimeOutVal_1_2520:
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u16SetBRMTimeOutVal_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u16SetBRMTimeOutVal_2_2547:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u16SetBRMTimeOutVal_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u16SetBCSTimeOutVal_1_2521:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u16SetBCSTimeOutVal_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u16SetBCSTimeOutVal_2:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u16SetBCSTimeOutVal_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_fSetLimitMaxOutCurr_1_2522:	
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_SYS, ID_fSetLimitMaxOutCurr_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_fSetLimitMaxOutCurr_2:	
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_SYS, ID_fSetLimitMaxOutCurr_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8PolarityAlmMask_1_2523:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PolarityAlmMask_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8PolarityAlmMask_2:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PolarityAlmMask_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_fMaxOffLineTime_2504:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_fMaxOffLineTime, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8SetCalcMoneyType_2505:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8SetCalcMoneyType, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8CC1CheckEnable_1_2524:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8CC1CheckEnable_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8CC1CheckEnable_2:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8CC1CheckEnable_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8SelfTestEnable_1_2525:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8SelfTestEnable_1, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8SelfTestEnable_2_2552:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8SelfTestEnable_2, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8MCCBOnCheckEnable_1_2526:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8MCCBOnCheckEnable_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8MCCBOnCheckEnable_2:		
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8MCCBOnCheckEnable_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8OutOverVoltEnable_1_2527:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8OutOverVoltEnable_1, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8OutOverVoltEnable_2:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8OutOverVoltEnable_2, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8DoorAlmMask_1_2495:
	case MDB_ADDR_u8DoorAlmMask_1_2528:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8DoorAlmMask_1, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8DoorAlmMask_2:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8DoorAlmMask_2, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8HasElec_ALock_1_2529:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8HasElec_ALock, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8HasElec_ALock_2_2556:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8HasElec_BLock, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8PWMPolarityCover_2506:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PWMPolarityCover, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8_A_LOCKType_2530:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8_A_LOCKType, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_u8_B_LOCKType:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8_B_LOCKType, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_u8LOCK_A_DIType_2531:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8LOCK_A_DIType, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8LOCK_B_DIType:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8LOCK_B_DIType, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8EPOAlmVltLEVEL_2507:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8EPOAlmVltLEVEL, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8BrushFactorType_2508:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8BrushFactorType, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8EV_StartTemp_2509:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8EV_StartTemp, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8EV_StopTemp_2510:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8EV_StopTemp, unVal, NULL, BY_E2PROM);
		break;					
	case MDB_ADDR_u8NET_OffLine_Charge_Enable_2511:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8NET_OffLine_Charge_Enable, unVal, NULL, BY_E2PROM);
		break;	
	case MDB_ADDR_u32PowerOnTimes_2512:
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u32PowerOnTimes, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR_u8NetMask_2494:
		if(u16Value < 2)
		{
			gEvChargeInfo.u8NET_Mask = u16Value;
		}
		break;
	case MDB_ADDR_SPD_MASK:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8SPDAlmMask, unVal, NULL, BY_E2PROM);
		break;

	case MDB_ADDR_SYS_POWER:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_EV_SYS_POWER, unVal, NULL, BY_E2PROM);
		break;

	case MDB_ADDR_PARALL_MODE:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_ParallelMode, unVal, NULL, BY_E2PROM);
		break;

	case MDB_ADDR_u8_A_ForceVer2011_2532:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8_A_ForceVer2011, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR_u8_B_ForceVer2011:	
		unVal.u32Value = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8_B_ForceVer2011, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR1_fACWhCoef_2533:
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_WH1, ID_fACWhCoef, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR2_fACWhCoef:
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_WH2, ID_fACWhCoef, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR1_u8WH_TYPE_2534:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_WH1, ID_u8WH_TYPE, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR2_u8WH_TYPE:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_WH2, ID_u8WH_TYPE, unVal, NULL, BY_E2PROM);
		break;			
	case MDB_ADDR1_u8WH_ALM_Mask_2535:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_WH1, ID_u8WH_ALM_Mask, unVal, NULL, BY_E2PROM);
		break;		
	case MDB_ADDR2_u8WH_ALM_Mask:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_WH2, ID_u8WH_ALM_Mask, unVal, NULL, BY_E2PROM);
		break;		
	case MDB_ADDR1_u8JYMAlmStatMask_2536:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_IMIU1, ID_u8JYMAlmStatMask, unVal, NULL, BY_E2PROM);
		break;	
	case MDB_ADDR2_u8JYMAlmStatMask_2563:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_IMIU2, ID_u8JYMAlmStatMask, unVal, NULL, BY_E2PROM);
		break;				
	case MDB_ADDR1_u8SetJYMCtrlMode_2537:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_IMIU1, ID_u8SetJYMCtrlMode, unVal, NULL, BY_E2PROM);
		break;		
	case MDB_ADDR2_u8SetJYMCtrlMode:	
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_IMIU2, ID_u8SetJYMCtrlMode, unVal, NULL, BY_E2PROM);
		break;		
	case MDB_ADDR1_u8InsltCheckByVolt_2538:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_IMIU1, ID_u8InsltCheckByVolt, unVal, NULL, BY_E2PROM);
		break;	
	case MDB_ADDR2_u8InsltCheckByVolt:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_IMIU2, ID_u8InsltCheckByVolt, unVal, NULL, BY_E2PROM);
		break;	
	case MDB_ADDR1_fSetJYMAlm_Resist_2539:
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_IMIU1, ID_fSetJYMAlm_Resist, unVal, NULL, BY_E2PROM);
		break;	
	case MDB_ADDR2_fSetJYMAlm_Resist_2566:
		unVal.ftemp = u16Value*0.1;
		ExSetSigInfo(E_TYPE_IMIU2, ID_fSetJYMAlm_Resist, unVal, NULL, BY_E2PROM);
		break;

	case MDB_ADDR1_BMS_POWER_2567:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_1_BMS_POWER, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR2_BMS_POWER_2568:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_2_BMS_POWER, unVal, NULL, BY_E2PROM);
		break;

	case MDB_ADDR_DHCP_VAL_2650:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_NET_DHCP, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MAC1_2651:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_1, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MAC2_2652:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_2, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MAC3_2653:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_3, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MAC4_2654:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_4, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MAC5_2655:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_5, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MAC6_2656:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_6, unVal, NULL, BY_E2PROM);
		break;	
	case MDB_ADDR_IP1_2657:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8IP_1, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_IP2_2658:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8IP_2, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_IP3_2659:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8IP_3, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_IP4_2660:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8IP_4, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MASK1_2661:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_1, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MASK2_2662:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_2, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MASK3_2663:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_3, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_MASK4_2664:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_4, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_Gateway1_2665:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Gateway_1, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_Gateway2_2666:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Gateway_2, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_Gateway3_2667:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Gateway_3, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_Gateway4_2668:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8Gateway_4, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_PORTNO_2800:
		unVal.u32Value = u16Value;
		gEvChargeInfo.u32NET_PORT = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u32NET_PORT, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_ServerIp1_2801:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_ServerIp2_2802:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_2, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_ServerIp3_2803:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_3, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_ServerIp4_2804:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_4, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_DNSIp1_2805:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_DNSServerIP_NUM_1, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_DNSIp2_2806:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_DNSServerIP_NUM_2, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_DNSIp3_2807:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_DNSServerIP_NUM_3, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_DNSIp4_2808:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_DNSServerIP_NUM_4, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_RemoteServerIsDomainName_2809:
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_RemoteServerIsDomainName_NUM, unVal, NULL, BY_E2PROM);
		break;
	case MDB_ADDR_ServerName_2810:case (MDB_ADDR_ServerName_2810+1):case (MDB_ADDR_ServerName_2810+2):case (MDB_ADDR_ServerName_2810+3):case (MDB_ADDR_ServerName_2810+4):case (MDB_ADDR_ServerName_2810+5):
	case (MDB_ADDR_ServerName_2810+6):case (MDB_ADDR_ServerName_2810+7):case (MDB_ADDR_ServerName_2810+8):case (MDB_ADDR_ServerName_2810+9):case (MDB_ADDR_ServerName_2810+10):case (MDB_ADDR_ServerName_2810+11):
	case (MDB_ADDR_ServerName_2810+12):case (MDB_ADDR_ServerName_2810+13):case (MDB_ADDR_ServerName_2810+14):case (MDB_ADDR_ServerName_2810+15):case (MDB_ADDR_ServerName_2810+16):case (MDB_ADDR_ServerName_2810+17):
	case (MDB_ADDR_ServerName_2810+18):case (MDB_ADDR_ServerName_2810+19):case (MDB_ADDR_ServerName_2810+20):case (MDB_ADDR_ServerName_2810+21):case (MDB_ADDR_ServerName_2810+22):case (MDB_ADDR_ServerName_2810+23):
	case (MDB_ADDR_ServerName_2810+24):case (MDB_ADDR_ServerName_2810+25):case (MDB_ADDR_ServerName_2810+26):case (MDB_ADDR_ServerName_2810+27):case (MDB_ADDR_ServerName_2810+28):case (MDB_ADDR_ServerName_2810+29):
	case (MDB_ADDR_ServerName_2810+30):case (MDB_ADDR_ServerName_2810+31):
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		(((EV_CHARGE_INFO*)PEInfo)->u16ServerName[RegAddr - MDB_ADDR_ServerName_2810]) = u16Value;
		unVal.chtemp[0] = u16Value;
		ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_RemoteServerDomainName_NUM_1 + (RegAddr - MDB_ADDR_ServerName_2810), unVal, NULL, BY_E2PROM);
		break;
	default:
		unVal.chtemp[0] = u16Value;
		if((RegAddr >= MDB_ADDR_TelaidianStarionID_3000) && (RegAddr <= MDB_ADDR_TelaidianStarionID_3031))
		{
			ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianStarionID_NUM_1 + RegAddr - MDB_ADDR_TelaidianStarionID_3000, unVal, NULL, BY_E2PROM);
		}
		else if((RegAddr >= MDB_ADDR_TelaidianUserName_2900) && (RegAddr <= MDB_ADDR_TelaidianUserName_2931))
		{
			ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianUserName_NUM_1 + RegAddr - MDB_ADDR_TelaidianUserName_2900, unVal, NULL, BY_E2PROM);
		}
		else if((RegAddr >= MDB_ADDR_TelaidianPassword_2932) && (RegAddr <= MDB_ADDR_TelaidianPassword_2964))
		{
			ExSetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianPassword_NUM_1 + RegAddr - MDB_ADDR_TelaidianPassword_2932, unVal, NULL, BY_E2PROM);
		}
		else if((RegAddr >= MDB_ADDR_StationCode_3032) && (RegAddr <= MDB_ADDR_StationCode_3091))
		{
			ExSetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + RegAddr - MDB_ADDR_StationCode_3032, unVal, NULL, BY_E2PROM);
		}
		break;
	}
	return 0;
}

UINT16 CRC16_Mdbcheck(UINT8 *fp_CRC, UINT8 count)
{
	UINT8 i;
	UINT16 ModbusCRC;
	i = 0;
	ModbusCRC = 0xffff;
	while(count!=0)
	{
		ModbusCRC^=*fp_CRC; 
		for(i=0;i<8;i++) 
		{
			if((ModbusCRC&0x0001)==0)
			{
				ModbusCRC=ModbusCRC>>1;
			}
			else
			{ 
				ModbusCRC=ModbusCRC>>1; 
				ModbusCRC^=0xa001;
			}
		}
		count-=1;
		fp_CRC++;
	}
	return ModbusCRC;
}


void InitMdbusBuf(void)
{
	dv_Uart6.pData = 0;
	gUart6ArryIdx = 0;
	UINT16 u16Idx = 0;
	for (u16Idx = 0; u16Idx < MDBUS_RCV_ARRY_NUM; u16Idx++)
	{
		gMdbusRcvInfo[u16Idx].uReceivFlag = FALSE;
		memset(gMdbusRcvInfo[u16Idx].uBuf, 0, (MDBUS_BYTES_MAX_NUM - 1));
	}
}

UINT16 MdbusCRC_check(UINT8 *fp_CRC, UINT8 count)
{
	UINT8 i;
	UINT16 ModbusCRC;
	i = 0;
	ModbusCRC = 0xffff;
	while(count!=0)
	{
		ModbusCRC^=*fp_CRC; 
		for(i=0;i<8;i++) 
		{
			if((ModbusCRC&0x0001)==0)
			{
				ModbusCRC=ModbusCRC>>1;
			}
			else
			{ 
				ModbusCRC=ModbusCRC>>1; 
				ModbusCRC^=0xa001;
			}
		}
		count-=1;
		fp_CRC++;
	}
	return ModbusCRC;
}

void CMD_0X03Deal(UINT8* pRcvBuf)
{
	UINT16 u16Value = 0;
	UINT16 u16Idx = 0;
	UINT16 u16RegNum = 0;
	WORD wRegAddr = 0;
	WORD wCRC = 0;
	wRegAddr = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff)) + 1;
	u16RegNum = (((WORD )pRcvBuf[MDB_RCV_BYTE_4_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_5_L] & 0xff));
	UNUSED(wRegAddr);
	UNUSED(u16RegNum);
	UINT16 OffsetPst = 0;
	u8MdbSendbuf[MDB_RCV_BYTE_0_ADDR] = MDB_IMSU_ADDR_VALUE;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD] = UI_MODBUS_CMD_03_ID;
	//总字节数
	u16Value =  u16RegNum * 2;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD + 1] = (u16Value)&0xff;
	//寄存器值 高位  寄存器值 低位
	for (u16Idx = 0; u16Idx < u16RegNum; u16Idx++)
	{
		u16Value = u16MdbGetSignal(wRegAddr + u16Idx);
		u8MdbSendbuf[MDB_RCV_BYTE_1_CMD + 2 + (u16Idx * 2)] = (u16Value >> 8) & 0xff;
		u8MdbSendbuf[MDB_RCV_BYTE_1_CMD + 2 + (u16Idx * 2) + 1] = u16Value & 0xff;
		OffsetPst = MDB_RCV_BYTE_1_CMD + 2 + (u16Idx * 2) + 1;
	}
	OffsetPst += 1;
	wCRC = CRC16_Mdbcheck(u8MdbSendbuf, OffsetPst);
	u8MdbSendbuf[OffsetPst] = EV_DWORD_LO(wCRC);
	u8MdbSendbuf[OffsetPst+1] = EV_DWORD_HI(wCRC);
	OSTimeDly(30);
	dv_Uart6.fSendDataTXY(u8MdbSendbuf, OffsetPst + 2);
}

void CMD_0X06Deal(UINT8* pRcvBuf)
{
	//UINT16 u16Val = 0;
	//WORD wRegAddr = 0;
	//WORD wCRC = 0;
	//wRegAddr = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));
	//u16Val = (((WORD )pRcvBuf[MDB_RCV_BYTE_4_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_5_L] & 0xff));	  //寄存器值 高位  //寄存器值 低位
	//u16SetSignal(wRegAddr+1, u16Val);
	//u8MdbSendbuf[MDB_RCV_BYTE_0_ADDR] = MDB_IMSU_ADDR_VALUE;
	//u8MdbSendbuf[MDB_RCV_BYTE_1_CMD] = UI_MODBUS_CMD_06_ID;
	//u8MdbSendbuf[MDB_RCV_BYTE_2_REG_ADDR_H] = (wRegAddr>>8) &0xff;
	//u8MdbSendbuf[MDB_RCV_BYTE_3_REG_ADDR_L] = (wRegAddr) &0xff;
	//u8MdbSendbuf[MDB_RCV_BYTE_4_H] = (u16Val>>8) &0xff;		//PRESET DATA HI
	//u8MdbSendbuf[MDB_RCV_BYTE_5_L] = (u16Val) &0xff;		//PRESET DATA LO
	//UINT8 OffsetPst = MDB_RCV_BYTE_5_L + 1;
	//wCRC = CRC16_Mdbcheck(u8MdbSendbuf, MDB_RCV_BYTE_5_L + 1);
	//u8MdbSendbuf[OffsetPst] = EV_DWORD_LO(wCRC);
	//u8MdbSendbuf[OffsetPst+1] = EV_DWORD_HI(wCRC);
	//OSTimeDly(30);
	//dv_Uart6.fSendDataTXY(u8MdbSendbuf, OffsetPst + 2);
}

void CMD_0X10Deal(UINT8* pRcvBuf)
{
	UINT8 u8Idx = 0;
	UINT8* pRegVal;
	UINT16 u16RegNum = 0;
	UINT16 u16Val = 0;
	WORD wRegAddr = 0;
	WORD tAddr = 0;
	WORD wCRC = 0;
	wRegAddr = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));		//starting Adrress Hi	starting adress LO
	u16RegNum = (((WORD )pRcvBuf[MDB_RCV_BYTE_4_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_5_L] & 0xff));						//寄存器总数	 No of Registers Hi		No of Registers Lo
	pRegVal = pRcvBuf + MDB_RCV_BYTE_5_L + 1;																				//寄存器  总 字节 数	Byte count
	pRegVal = pRegVal + 1;																									//寄存器值 高位  Data Hi  Data Lo
	//寄存器值 低位
	UNUSED(wRegAddr);
	UNUSED(u16RegNum);
	UNUSED(pRegVal);
	for (u8Idx = 0; u8Idx < u16RegNum && u8Idx < 100; u8Idx++)
	{
		tAddr = wRegAddr + u8Idx;
		u16Val = (pRegVal[0]<<8) + pRegVal[1];
		u16MdbSetSignal(tAddr + 1, u16Val);
		pRegVal += 2;

	}
	u8MdbSendbuf[MDB_RCV_BYTE_0_ADDR] = MDB_IMSU_ADDR_VALUE;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD] = UI_MODBUS_CMD_10_ID;
	u8MdbSendbuf[MDB_RCV_BYTE_2_REG_ADDR_H] = (wRegAddr>>8) &0xff;
	u8MdbSendbuf[MDB_RCV_BYTE_3_REG_ADDR_L] = (wRegAddr) &0xff;
	u8MdbSendbuf[MDB_RCV_BYTE_4_H] = (u16RegNum>>8) &0xff;					//寄存器总数
	u8MdbSendbuf[MDB_RCV_BYTE_5_L] = (u16RegNum) &0xff;
	UINT8 OffsetPst = MDB_RCV_BYTE_5_L+1;
	wCRC = CRC16_Mdbcheck(u8MdbSendbuf, MDB_RCV_BYTE_5_L+1);
	u8MdbSendbuf[OffsetPst] = EV_DWORD_LO(wCRC);
	u8MdbSendbuf[OffsetPst+1] = EV_DWORD_HI(wCRC);
	OSTimeDly(30);
	dv_Uart6.fSendDataTXY(u8MdbSendbuf, OffsetPst + 2);
}




void CMD_0X05Deal(UINT8* pRcvBuf)
{
	//UINT8 u8Idx = 0;
	UINT8* pRegVal = 0;
	UINT16 u16Val = 0;
	WORD wRegAddr = 0;
	WORD wCRC = 0;
	wRegAddr = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));		//starting Adrress Hi	starting adress LO
	u16Val = (((WORD )pRcvBuf[MDB_RCV_BYTE_4_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_5_L] & 0xff));	
	UNUSED(wRegAddr);
	UNUSED(pRegVal);
	//u16SetSignal(wRegAddr + 1, u16Val);
	u16Fun05SetSignal(wRegAddr + 1, u16Val);

	u8MdbSendbuf[MDB_RCV_BYTE_0_ADDR] = MDB_IMSU_ADDR_VALUE;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD] = UI_MODBUS_CMD_05_ID;
	u8MdbSendbuf[MDB_RCV_BYTE_2_REG_ADDR_H] = (wRegAddr>>8) &0xff;
	u8MdbSendbuf[MDB_RCV_BYTE_3_REG_ADDR_L] = (wRegAddr) &0xff;
	u8MdbSendbuf[MDB_RCV_BYTE_4_H] = (u16Val>>8) &0xff;					//寄存器总数
	u8MdbSendbuf[MDB_RCV_BYTE_5_L] = (u16Val) &0xff;
	UINT8 OffsetPst = MDB_RCV_BYTE_5_L+1;
	wCRC = CRC16_Mdbcheck(u8MdbSendbuf, MDB_RCV_BYTE_5_L+1);
	u8MdbSendbuf[OffsetPst] = EV_DWORD_LO(wCRC);
	u8MdbSendbuf[OffsetPst+1] = EV_DWORD_HI(wCRC);
	OSTimeDly(30);
	dv_Uart6.fSendDataTXY(u8MdbSendbuf, OffsetPst + 2);
	OSTimeDly(10);
}


void RPT_Base_A_B_PageDeal(UINT8* pRcvBuf)
{
	UINT16 OffsetPst = 0;
	//UINT16 wRegAddr = 0;
	UINT16 u16RegNum = 0;
	UINT16 u16Value;
	UINT16 u16Idx = 0;
	//UINT16 u16SeqceNo = 0;
	//static UINT8 su8Delay = 0;
	WORD wCRC = 0;
	memset(u8MdbSendbuf, 0, MDBUS_SEND_BYTES_MAX_NUM);
	//wRegAddr = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff)) + 1;
	u16RegNum = (((WORD )pRcvBuf[MDB_RCV_BYTE_4_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_5_L] & 0xff));
	u8MdbSendbuf[MDB_RCV_BYTE_0_ADDR] = MDB_ADDR_VALUE;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD] = UI_MODBUS_CMD_04_ID;
	//总字节数
	u16Value =  u16RegNum * 2;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD + 1] = (u16Value)&0xff;
	//寄存器值 高位  寄存器值 低位
	for (u16Idx = 0; u16Idx < u16RegNum; u16Idx++)
	{
		u16Value = gHMIData.u16Data[u16Idx];
		u8MdbSendbuf[MDB_RCV_BYTE_1_CMD + 2 + (u16Idx * 2)] = (u16Value >> 8) & 0xff;
		u8MdbSendbuf[MDB_RCV_BYTE_1_CMD + 2 + (u16Idx * 2) + 1] = u16Value & 0xff;
		OffsetPst = MDB_RCV_BYTE_1_CMD + 2 + (u16Idx * 2) + 1;
	}
	OffsetPst += 1;
	wCRC = CRC16_Mdbcheck(u8MdbSendbuf, OffsetPst);
	u8MdbSendbuf[OffsetPst] = EV_DWORD_LO(wCRC);
	u8MdbSendbuf[OffsetPst+1] = EV_DWORD_HI(wCRC);
	OSTimeDly(30);
	dv_Uart6.fSendDataTXY(u8MdbSendbuf, OffsetPst + 2);
	OSTimeDly(3);
}

void CMD_0X04Deal(UINT8* pRcvBuf)
{
	//UINT16 u16Value = 0;
	//UINT16 u16Idx = 0;
	UINT16 u16RegNum = 0;
	WORD wRegAddr = 0;
	//WORD wCRC = 0;
	wRegAddr = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff)) + 1;
	u16RegNum = (((WORD )pRcvBuf[MDB_RCV_BYTE_4_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_5_L] & 0xff));
	UNUSED(wRegAddr);
	UNUSED(u16RegNum);
	//UINT16 OffsetPst = 0;
	u8MdbSendbuf[MDB_RCV_BYTE_0_ADDR] = MDB_IMSU_ADDR_VALUE;
	u8MdbSendbuf[MDB_RCV_BYTE_1_CMD] = UI_MODBUS_CMD_04_ID;
	switch(wRegAddr)
	{
	case MDB_BASE_PAGE_ADDR_2700:
		RPT_Base_A_B_PageDeal(pRcvBuf);
		break;
	default:
		break;
	}
}

void MdbusDeal(void)
{
	WORD  wCRC = 0;
	UINT8  byCRCHiCal = 0x00;
	UINT8  byCRCLoCal = 0x00;
	UINT8 byCRCLo = 0;
	UINT8 byCRCHi = 0;
	UINT16 byFrameBytesNum = 0;
	UINT16 u16Idx = 0;
	UINT8 ArryIdx = 0;
	static UINT32 u32CheckUart6Times = 0;

	gSelfTest.fSetTaskID(Task_ID_Console);

	for (ArryIdx = 0; ArryIdx < UART4_RCV_ARRY_NUM; ArryIdx++)
	{
		if (TRUE == gMdbusRcvInfo[ArryIdx].uReceivFlag)
		{
			for (u16Idx = 0; u16Idx < MDBUS_BYTES_MAX_NUM; u16Idx++)
			{
				u8AnalayseRbuf[u16Idx] = gMdbusRcvInfo[ArryIdx].uBuf[u16Idx];
				gMdbusRcvInfo[ArryIdx].uBuf[u16Idx] = 0;
			}
			gMdbusRcvInfo[ArryIdx].uReceivFlag = FALSE;

			byFrameBytesNum = 0;
			if (UI_MODBUS_CMD_03_ID == u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD]
			||UI_MODBUS_CMD_05_ID == u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD]
			||UI_MODBUS_CMD_01_ID == u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD]
			||UI_MODBUS_CMD_02_ID == u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD]
			||UI_MODBUS_CMD_04_ID == u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD])
			{
				byFrameBytesNum = 8;
			}
			else if (UI_MODBUS_CMD_10_ID == u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD])
			{
				//6 寄存器总 字节数;	2 CRC;	+1 字节数自己加进来，见协议文本
				byFrameBytesNum = (MDB_RCV_BYTE6_10H_BYTE_NUM + 1) + 2 + u8AnalayseRbuf[MDB_RCV_BYTE6_10H_BYTE_NUM];
			}
			else
			{
				gSelfTest.fSetTaskID(Task_ID_Console);//容错！
				break;
			}

			if (byFrameBytesNum < UART6_RECV_MAX_NUM)
			{
				//Received CRC
				byCRCLo = u8AnalayseRbuf[byFrameBytesNum - 1];
				byCRCHi = u8AnalayseRbuf[byFrameBytesNum - 2];
				wCRC = MdbusCRC_check(u8AnalayseRbuf, (byFrameBytesNum - 2));		//这种计算方法反过来了
				byCRCHiCal = 0;
				byCRCLoCal = 0;
				byCRCLoCal = 0xff&((wCRC&0xff00) >> 8);
				byCRCHiCal = wCRC & 0xff;
				//CRC is Error
				if ((byCRCHiCal != byCRCHi)
					||(byCRCLoCal != byCRCLo))
				{
					gSelfTest.fSetTaskID(Task_ID_Console);
				}
				else
				{
					u32CheckUart6Times = 0;
					//Uart4WDG = 0;
					gSelfTest.fSetTaskID(Task_ID_Console);
					switch(u8AnalayseRbuf[MDB_RCV_BYTE_1_CMD])
					{
					case UI_MODBUS_CMD_03_ID:
						CMD_0X03Deal(u8AnalayseRbuf);
						break;

					case UI_MODBUS_CMD_04_ID:
						CMD_0X04Deal(u8AnalayseRbuf);
						break;

					case UI_MODBUS_CMD_05_ID:
						CMD_0X05Deal(u8AnalayseRbuf);
						break;

					case UI_MODBUS_CMD_01_ID:
						break;

					case UI_MODBUS_CMD_06_ID:
						CMD_0X06Deal(u8AnalayseRbuf);
						break;

					case UI_MODBUS_CMD_10_ID:
						CMD_0X10Deal(u8AnalayseRbuf);
						break;
					default:
						gSelfTest.fSetTaskID(Task_ID_Console);
						break;
					}
				}
			}
			else
			{
				gSelfTest.fSetTaskID(Task_ID_Console);//容错
			}
		}
		else
		{
		}
	}//END FOR PROCESS 
	u32CheckUart6Times++;
	if (u32CheckUart6Times > 1000)
	{
		u32CheckUart6Times = 0;
		dv_Uart6.fSetUart6BaudRate();   //19200 E 8 1
		dv_Uart6.fEnableUart6Rx();
		OSTimeDly(800);
	}
}

void InitHMIData(void)
{
	gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
	gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
	gHMIData.unPageAddr.u16BasePageAddr = PAGE_HOME_NO_11;					//上电不跳转
}


void SecificPage_83_Deal(void)
{
	UINT8 u83CVal=0;
        UNUSED(u83CVal);
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);						//EV_CHARGE_INFO
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	void *PEInfo = GetgEquipInfo(E_TYPE_SYS);
	if (PAGE_NOTE_PLUG_IN_83 != gHMIData.unPageAddr.u16BasePageAddr)
	{		
		gs_TelaidianCtrl.AccountRequest[0] = 0;
		gs_TelaidianCtrl.AccountRequest[1] = 0;
		return;
	}

	//if (PAGE_NOTE_PLUG_IN_83 == gHMIData.unPageAddr.u16BasePageAddr)
	{
		if (((EV_CHARGE_INFO*)PEInfo)->iPageTimeOut <= 0)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
			if (gEvChargeInfo.u8IsOptionB == TRUE)
			{
				u83CVal = PLUGIN_TIME_OUT_3C + (0x02<<4);
			}
			else
			{
				u83CVal = PLUGIN_TIME_OUT_3C + (0x01<<4);
			}
			gEvChargeInfo.u8IsOptionA = FALSE;
			gEvChargeInfo.u8IsOptionB = FALSE;
			gs_TelaidianCtrl.AccountRequest[0] = 0;
			gs_TelaidianCtrl.AccountRequest[1] = 0;
			return;
		}

		//后台拒绝
		if(gEvChargeInfo.u8IsOptionA == TRUE)
		{
			if(gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_5)
			{
				if (Pop_BrushSem()) //重刷卡，则再发一遍
				{
					gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x01;
					gs_TelaidianCtrl.AccountRequest[0] = 0;					
				}
				//gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;				
				return;
			}
		}
		else if(gEvChargeInfo.u8IsOptionB == TRUE)
		{
			if(gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_5)
			{
				if (Pop_BrushSem())  //重刷卡，则再发一遍
				{
					gEvChargeInfo.u8_B_LogInMother = EV_ENTER_IC_CARD;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x02;
					gs_TelaidianCtrl.AccountRequest[1] = 0;			
				}
				//gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;				
				return;				
			}
		}

		
		
		if(gEvChargeInfo.u8IsOptionB == TRUE)
		{
			//收到账户确认，且枪已插好，发送充电请求
			if ((PChargeInfo->u8EV_SpearPst[GUN_NO_B] == SPEAR_ON_LINE)
				&& ((gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_2)  ////收到账户确认
				  ||(gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_6)) //收到账户确认
				)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].NeedSendGunNo |= 0x02;
				gs_TelaidianCtrl.AccountRequest[1] = TelaidianAccountRequestFlag_7;
			}
			else if(gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_2)  //枪未插好
			{
				gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_6;  //提示插枪
			}

			/*gEvChargeInfo.u8ChargeMode[GUN_NO_B] = gTempChargeArgs.u8ChargeMode;
			B_SetChargeSTART_Cmd(1);
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
			gEvChargeInfo.u8IsOptionB = FALSE;
			gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_CHARGE_36;*/
			return;
		}

		if(gEvChargeInfo.u8IsOptionA == TRUE)
		{
			//收到账户确认，且枪已插好，发送充电请求
			if ((PChargeInfo->u8EV_SpearPst[GUN_NO_A] == SPEAR_ON_LINE)
				&& ((gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_2)  ////收到账户确认
				||(gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_6)) //收到账户确认
				)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].NeedSendGunNo |= 0x01;
				gs_TelaidianCtrl.AccountRequest[0] = TelaidianAccountRequestFlag_7;
			}
			else if(gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_2)  //枪未插好
			{
				gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_6;  //提示插枪
			}

			/*gEvChargeInfo.u8ChargeMode[GUN_NO_A] = gTempChargeArgs.u8ChargeMode;
			A_SetChargeSTART_Cmd(1);
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
			gEvChargeInfo.u8IsOptionA = FALSE;
			gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_CHARGE_21;*/
			return;
		}
	}	
}
void SecificPage_87_Deal(void)
{
	UINT8 u83CVal=0;
        UNUSED(u83CVal);
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);						//EV_CHARGE_INFO
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	void *PEInfo = GetgEquipInfo(E_TYPE_SYS);
	if (PAGE_BRUSH_STOP_CHG_87 != gHMIData.unPageAddr.u16BasePageAddr)
	{		
		return;
	}

	if (Pop_A_StopSem())
	{
		gs_TelaidianCtrl.StopRequest[0] = 0;
		gEvChargeInfo.u8IsOptionA = FALSE;

		gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
		gHMIData.unPageAddr.u16BasePageAddr= PAGE_A_RESULT_84;
		gEvChargeInfo.u16_A_PageResultDelay = PAGE_RESULT_DELAY_SEC;
		return;
	}

	if (Pop_B_StopSem())
	{
		gs_TelaidianCtrl.StopRequest[1] = 0;
		gEvChargeInfo.u8IsOptionB = FALSE;

		gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
		gHMIData.unPageAddr.u16BasePageAddr= PAGE_B_RESULT_85;

		gEvChargeInfo.u16_B_PageResultDelay = PAGE_RESULT_DELAY_SEC;
		return;
	}

	//后台拒绝
	if(gEvChargeInfo.u8IsOptionA == TRUE)
	{
		if((gs_TelaidianCtrl.StopRequest[0] == TelaidianStopRequestFlag_3)
			|| (gs_TelaidianCtrl.StopRequest[0] == TelaidianStopRequestFlag_4))
		{
			if (Pop_BrushSem()) //重刷卡，则再发一遍
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].NeedSendGunNo |= 0x01;
				gs_TelaidianCtrl.StopRequest[0] = TelaidianStopRequestFlag_1;	

				PChargeInfo->iPageTimeOut = 90;
			}
			//gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;				
			return;
		}
	}
	else if(gEvChargeInfo.u8IsOptionB == TRUE)
	{		

		if((gs_TelaidianCtrl.StopRequest[1] == TelaidianStopRequestFlag_3)
			|| (gs_TelaidianCtrl.StopRequest[1] == TelaidianStopRequestFlag_4))
		{
			if (Pop_BrushSem()) //重刷卡，则再发一遍
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].NeedSendGunNo |= 0x02;
				gs_TelaidianCtrl.StopRequest[1] = TelaidianStopRequestFlag_1;	

				PChargeInfo->iPageTimeOut = 90;
			}
			//gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;				
			return;
		}
	}

	//if (PAGE_NOTE_PLUG_IN_83 == gHMIData.unPageAddr.u16BasePageAddr)
	{
		if (((EV_CHARGE_INFO*)PEInfo)->iPageTimeOut <= 0)
		{
			if (gEvChargeInfo.u8IsOptionB == TRUE)
			{
				gs_TelaidianCtrl.StopRequest[1] = TelaidianStopRequestFlag_4;
			}
			else
			{
				gs_TelaidianCtrl.StopRequest[0] = TelaidianStopRequestFlag_4;
			}		
			return;
		}
	}

	
}
void SpecificPage_BRUSHEventDeal(void)
{
	//UINT8 u8ValidNum = 0;
	//u8ValidNum = CheckValidSpear();
	UINT8 u8SpearIdlNum;
	UINT8 u8Idx = 0;
	UINT8 u8Rst = FALSE;
	BRUSH_INFO *PBrushInfo = GetgEquipInfo(E_TYPE_BRUSH);
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);		//EV_CHARGE_INFO

	if (!(gHMIData.unPageAddr.u16BasePageAddr == PAGE_HOME_NO_11
				|| gHMIData.unPageAddr.u16BasePageAddr == PAGE_INVALID_NO_0
				|| gHMIData.unPageAddr.u16BasePageAddr == PAGE_A_B_SPLIT_NO_12))
	{
		return;
	}
	if (Pop_BrushSem())
	{			
		PChargeInfo->iPageTimeOut = 90 ;

		if (PChargeInfo->u8IsOptionA == FALSE && PChargeInfo->u8IsOptionB == FALSE
			&& PChargeInfo->u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL && PChargeInfo->u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
		{
			//gHMIData.unPageAddr.u16BasePageAddr = PAGE_PSWD_KEYBOAD_76;
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;
			HMIKeyClean();
		}
		else if (PChargeInfo->u8IsOptionA == TRUE)  
		{
			//结束充电
			if (PChargeInfo->u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
			{
				//判断一下 卡是否启动的卡号！！
				u8Rst = TRUE;
				for (u8Idx = 0; u8Idx < CARD_NO_BYTES/2; u8Idx++)
				{
					if (gBrushInfo.u8CARDInfo[GUN_NO_A][u8Idx] != gBrushInfo.u8TempCARDInfo[u8Idx])
					{
						u8Rst = FALSE;
						break;
					}
				}

				if (u8Rst)
				{
					/*A_SetChargeSTOP_Cmd(BRUSH_STOP_CMD);
					gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_RESULT_84;
					PChargeInfo->u8IsOptionA = FALSE;*/
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].NeedSendGunNo |= 0x01;

					gHMIData.unPageAddr.u16BasePageAddr = PAGE_BRUSH_STOP_CHG_87;	
					//gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
					gs_TelaidianCtrl.StopRequest[0] = TelaidianStopRequestFlag_1;

					PChargeInfo->iPageTimeOut = 90 ;
				}

				//充电结果
				return;
			}
			//充电！
			if (PChargeInfo->u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL)
			{
				//gHMIData.unPageAddr.u16BasePageAddr = PAGE_PSWD_KEYBOAD_76;

				//枪选择
				u8SpearIdlNum = CheckValidSpear();
				if (0 == u8SpearIdlNum)
				{
					//无可用枪！	不操作！
				}
				else if (u8SpearIdlNum > 2)
				{
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
				}
				else
				{
					//只有一个！
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪

					if(u8SpearIdlNum == 1)
					{
						gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x01;
						gs_TelaidianCtrl.AccountRequest[0] = 0;
					}
					else if(u8SpearIdlNum == 2)
					{
						gEvChargeInfo.u8_B_LogInMother = EV_ENTER_IC_CARD;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x02;
						gs_TelaidianCtrl.AccountRequest[1] = 0;
					}
				}

				return;
			}
			//其他别他娘的烦我！
		}
		else if (PChargeInfo->u8IsOptionB == TRUE)
		{
			
			//结束充电
			if (PChargeInfo->u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
			{
				//判断一下  卡是否为启动时的卡号
				u8Rst = TRUE;
				for (u8Idx = 0; u8Idx < CARD_NO_BYTES/2; u8Idx++)
				{
					if (gBrushInfo.u8CARDInfo[GUN_NO_B][u8Idx] != gBrushInfo.u8TempCARDInfo[u8Idx])
					{
						u8Rst = FALSE;
						break;
					}
				}

				if (u8Rst)
				{
					/*B_SetChargeSTOP_Cmd(BRUSH_STOP_CMD);
					gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_B_RESULT_85;
					PChargeInfo->u8IsOptionB = FALSE;*/

					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].NeedSendGunNo |= 0x02;

					gHMIData.unPageAddr.u16BasePageAddr = PAGE_BRUSH_STOP_CHG_87;	
					//gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;

					gs_TelaidianCtrl.StopRequest[1] = TelaidianStopRequestFlag_1;

					
					PChargeInfo->iPageTimeOut = 90 ;
				}
				
				//充电结果
				return;
			}
			//充电！
			if (PChargeInfo->u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
			{
				//gHMIData.unPageAddr.u16BasePageAddr = PAGE_PSWD_KEYBOAD_76;
				//枪选择
				u8SpearIdlNum = CheckValidSpear();
				if (0 == u8SpearIdlNum)
				{
					//无可用枪！	不操作！
				}
				else if (u8SpearIdlNum > 2)
				{
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_SELECT_SPEAR_78;	//选择枪！
				}
				else
				{
					//只有一个！
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_NOTE_PLUG_IN_83;	//提示插枪

					if(u8SpearIdlNum == 1)
					{
						gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x01;
						gs_TelaidianCtrl.AccountRequest[0] = 0;
					}
					else if(u8SpearIdlNum == 2)
					{
						gEvChargeInfo.u8_B_LogInMother = EV_ENTER_IC_CARD;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo |= 0x02;
						gs_TelaidianCtrl.AccountRequest[1] = 0;
					}
				}
				return;
			}
		}
		else
		{
			//A 和 B 不可能同时为TRUE!!
		}
	}
}
#define PAGE_RESULT_DELAY_SEC		60		//结算页面停留时间 秒
void SecificPage_12_GetA_B_Deal(void)
{
	//static int iErr1Delay = 0;
	//static int iErr2Delay = 0;
	static int iDelay = 0;
	
	//结算页面不能停留太久
	/*if((gHMIData.unPageAddr.u16BasePageAddr == PAGE_A_RESULT_84)
		|| (gHMIData.unPageAddr.u16BasePageAddr == PAGE_B_RESULT_85))
	{
		if((gEvChargeInfo.u16_A_PageResultDelay == 0) && (gEvChargeInfo.u16_B_PageResultDelay == 0))
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
			iDelay = 0;
			return;
		}
	}*/


	if (gHMIData.unPageAddr.u16BasePageAddr != PAGE_A_B_SPLIT_NO_12)
	{
		iDelay = 0;//iErr1Delay = 0;iErr2Delay = 0;
		gEvChargeInfo.i32_A_PageErrDelay = 5 * 60 * OS_TICKS_PER_SEC;
		gEvChargeInfo.i32_B_PageErrDelay = 5 * 60 * OS_TICKS_PER_SEC;
		return;
	}

	if (Pop_A_StopSem())
	{
		gs_TelaidianCtrl.StopRequest[0] = 0;
		gEvChargeInfo.u8IsOptionA = FALSE;

		gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
		gHMIData.unPageAddr.u16BasePageAddr= PAGE_A_RESULT_84;
		iDelay = 0;
		gEvChargeInfo.u16_A_PageResultDelay = PAGE_RESULT_DELAY_SEC;
		return;
	}

	if (Pop_B_StopSem())
	{
		gs_TelaidianCtrl.StopRequest[1] = 0;
		gEvChargeInfo.u8IsOptionB = FALSE;

		gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
		gHMIData.unPageAddr.u16BasePageAddr= PAGE_B_RESULT_85;
		iDelay = 0;

		gEvChargeInfo.u16_B_PageResultDelay = PAGE_RESULT_DELAY_SEC;
		return;
	}

	if (gHMIData.unPageAddr.u16_B_PageAddr == PAGE_B_QR_CODE_35 
            && gHMIData.unPageAddr.u16_A_PageAddr == PAGE_A_QR_CODE_20)
	{
		//iErr1Delay = 0;iErr2Delay = 0;
		gEvChargeInfo.i32_A_PageErrDelay = 5 * 60 * OS_TICKS_PER_SEC;
		gEvChargeInfo.i32_B_PageErrDelay = 5 * 60 * OS_TICKS_PER_SEC;
		iDelay++;
		if (iDelay > 80)
		{
			gHMIData.unPageAddr.u16BasePageAddr= PAGE_HOME_NO_11;
			gEvChargeInfo.u8IsOptionA = FALSE;
			gEvChargeInfo.u8IsOptionB = FALSE;
		}
	}

	//纠错用！如果在GET A B页，但是又没在充电！且A或B 均在充电页面 但是没充电！属与异常状态
	if ((PAGE_A_CHARGE_21 == gHMIData.unPageAddr.u16_A_PageAddr)
		|| (PAGE_A_CHARGE_APP_22 == gHMIData.unPageAddr.u16_A_PageAddr))
	{
		if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL)
		{
			if (gEvChargeInfo.i32_A_PageErrDelay <= 0)
			{
				gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
			}
			
			//iErr1Delay ++;
			//if (iErr1Delay > 3000)
			//{
			//	iErr1Delay = 0;
			//	gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
			//}
		}
		else
		{
			gEvChargeInfo.i32_A_PageErrDelay = 5 * 60 * OS_TICKS_PER_SEC;
			//iErr1Delay = 0;
		}
	}

	if ((PAGE_B_CHARGE_36 == gHMIData.unPageAddr.u16_B_PageAddr)
		|| (PAGE_B_CHARGE_APP_37 == gHMIData.unPageAddr.u16_B_PageAddr))
	{
		if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
		{
			if (gEvChargeInfo.i32_B_PageErrDelay <= 0)
			{
				gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
			}
			//iErr2Delay ++;
			//if (iErr2Delay > 3000)
			//{
			//	iErr2Delay = 0;
			//	gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
			//}
		}
		else
		{
			gEvChargeInfo.i32_B_PageErrDelay = 5 * 60 * OS_TICKS_PER_SEC;
			//iErr2Delay = 0;
		}
	}
}

void SecificPage_11_206_HomeALM_Deal(void)
{
	if (gHMIData.unPageAddr.u16BasePageAddr == PAGE_HOME_NO_11)
	{
		if (gEvChargeInfo.u8SysAlm == TRUE)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_SYS_ALM_206;
		}
	}
	if (gHMIData.unPageAddr.u16BasePageAddr == PAGE_SYS_ALM_206)
	{
		if (gEvChargeInfo.u8SysAlm == FALSE)
		{
			gHMIData.unPageAddr.u16BasePageAddr = PAGE_HOME_NO_11;
		}
	}
}
void SpecificPageDeal(void)
{
	SecificPage_83_Deal();
	SecificPage_87_Deal();
	SpecificPage_BRUSHEventDeal();
	SecificPage_12_GetA_B_Deal();
	SecificPage_11_206_HomeALM_Deal();
}

unsigned long u32Test;
//UART6		J20
void TASK_Uart6Proc (void* pdata)
{
	UNUSED(pdata);
	OSTimeDly(800);
	OSTimeDly(800);
	OSTimeDly(800);
	InitHMIData();
	dv_Uart6.fSetUart6BaudRate();			//19200 E 8 1
	dv_Uart6.fEnableUart6Rx();
	OSTimeDly(800);
	InitMdbusBuf();
	RefreshQRCode();
        
        u32Test = TOPWAY_16BIT_ADDR_GUN_SELECT;
	//TXY
	for(;;)
	{
          u32Test = u32Test;
		OSTimeDly(10);
		gSelfTest.fSetTaskID(Task_ID_Console);
		MdbusDeal();
		SpecificPageDeal();
		OSTimeDly(10);

		Test_API();
	}
}
