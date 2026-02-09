#include "main.h"
//#include "TelaidianProto.h"
#include "Uart6_new.h"
#include "EtherNet.h"
#include "can_ring.h"
#include "Uart4and5.h"

//V2 版本 优化参数GET SET ，完全没必要再去取拷贝一份变量！！这样大大简化GET SET函数内部的实现！
const __SIG_E2P_ADDR stArgsSigAddr[]=
{
	{E_TYPE_SYS,		ID_fCC1_1Coef_A,					ADDR_CC1_1Coef_A,						IS_FLOAT},
	{E_TYPE_SYS,		ID_fCC1_1Coef_B,					ADDR_CC1_1Coef_B,						IS_FLOAT},
	{E_TYPE_SYS,		ID_fCC1_2Coef_A,					ADDR_CC1_2Coef_A,						IS_FLOAT},
	{E_TYPE_SYS,		ID_fCC1_2Coef_B,					ADDR_CC1_2Coef_B,						IS_FLOAT},
	{E_TYPE_SYS,		ID_fSysSetOverVolt,					ADDR_fSysSetOverVolt,					IS_FLOAT},
	{E_TYPE_SYS,		ID_fRectLoadStartTime,				ADDR_fRectLoadStartTime,				IS_FLOAT},
	{E_TYPE_SYS,		ID_fMaxOutCurr,						ADDR_MaxOutCurr,						IS_FLOAT},
	{E_TYPE_SYS,		ID_fMaxOutVolt,						ADDR_MaxOutVolt,						IS_FLOAT},
	{E_TYPE_SYS,		ID_fSetLimitMaxOutCurr_1,			ADDR_fSetLimitMaxOutCurr_1,				IS_FLOAT},
	{E_TYPE_SYS,		ID_fSetLimitMaxOutCurr_2,			ADDR_fSetLimitMaxOutCurr_2,				IS_FLOAT},


	
	{E_TYPE_SYS,		ID_u32_B_CTS_DB_T1,					ADDR_u32_B_CTS_DB_T1,					IS_ULONG},
	{E_TYPE_SYS,		ID_u32_B_DB_DB_T2,					ADDR_u32_B_DB_DB_T2,					IS_ULONG},
	{E_TYPE_SYS,		ID_u32_A_CTS_DB_T1,					ADDR_u32_A_CTS_DB_T1,					IS_ULONG},
	{E_TYPE_SYS,		ID_u32_A_DB_DB_T2,					ADDR_u32_A_DB_DB_T2,					IS_ULONG},
	{E_TYPE_SYS,		ID_u16SetBCLTimeOutVal_1,			ADDR_u16SetBCLTimeOutVal_1,				IS_ULONG},
	{E_TYPE_SYS,		ID_u16SetBCLTimeOutVal_2,			ADDR_u16SetBCLTimeOutVal_2,				IS_ULONG},
	{E_TYPE_SYS,		ID_u16SetBRMTimeOutVal_1,			ADDR_u16SetBRMTimeOutVal_1,				IS_ULONG},
	{E_TYPE_SYS,		ID_u16SetBRMTimeOutVal_2,			ADDR_u16SetBRMTimeOutVal_2,				IS_ULONG},
	{E_TYPE_SYS,		ID_u16SetBCSTimeOutVal_1,			ADDR_u16SetBCSTimeOutVal_1,				IS_ULONG},
	{E_TYPE_SYS,		ID_u16SetBCSTimeOutVal_2,			ADDR_u16SetBCSTimeOutVal_2,				IS_ULONG},

	{E_TYPE_SYS,		ID_u8PolarityAlmMask_1,				ADDR_u8PolarityAlmMask_1,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8PolarityAlmMask_2,				ADDR_u8PolarityAlmMask_2,				IS_CHAR},

	{E_TYPE_SYS,		ID_u8SetCalcMoneyType,				ADDR_u8SetCalcMoneyType,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8CC1CheckEnable_1,				ADDR_u8CC1CheckEnable_1,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8CC1CheckEnable_2,				ADDR_u8CC1CheckEnable_2,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8SelfTestEnable_1,				ADDR_u8SelfTestEnable_1,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8SelfTestEnable_2,				ADDR_u8SelfTestEnable_2,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8MCCBOnCheckEnable_1,			ADDR_u8MCCBOnCheckEnable_1,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8MCCBOnCheckEnable_2,			ADDR_u8MCCBOnCheckEnable_2,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8OutOverVoltEnable_1,			ADDR_u8OutOverVoltEnable_1,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8OutOverVoltEnable_2,			ADDR_u8OutOverVoltEnable_2,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8DoorAlmMask_1,					ADDR_u8DoorAlmMask_1,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8DoorAlmMask_2,					ADDR_u8DoorAlmMask_2,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8HasElec_ALock,					ADDR_u8HasElec_Lock_1,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8HasElec_BLock,					ADDR_u8HasElec_Lock_2,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PWMPolarityCover,				ADDR_u8PWMPolarityCover,				IS_CHAR},
	{E_TYPE_SYS,		ID_u8_A_LOCKType,					ADDR_u8_A_LOCKType,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8_B_LOCKType,					ADDR_u8_B_LOCKType,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8LOCK_A_DIType,					ADDR_u8LOCK_A_DIType,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8LOCK_B_DIType,					ADDR_u8LOCK_B_DIType,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8EPOAlmVltLEVEL,				ADDR_u8EPOAlmVltLEVEL,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8BrushFactorType,				ADDR_u8BrushFactorType,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8EV_StartTemp,					ADDR_u8EV_StartTemp,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8EV_StopTemp,					ADDR_u8EV_StopTemp,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8NET_OffLine_Charge_Enable,		ADDR_u8NET_OffLine_Charge_Enable,		IS_CHAR},
	{E_TYPE_SYS,		ID_fMaxOffLineTime,					ADDR_fMaxOffLineTime,					IS_ULONG},
	{E_TYPE_SYS,		ID_u32PowerOnTimes,					ADDR_u32PowerOnTimes,					IS_ULONG},
	{E_TYPE_SYS,		ID_u8_A_ForceVer2011,				ADDR_u8_A_ForceVer2011,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8_B_ForceVer2011,				ADDR_u8_B_ForceVer2011,					IS_CHAR},

	//网络屏蔽告警
	{E_TYPE_SYS,		ID_NET_DHCP,						ADDR_DHCP_VALUE,						IS_CHAR},
	{E_TYPE_SYS,		ID_u32NET_PORT,						ADDR_u32_NET_PORT,						IS_ULONG},
	{E_TYPE_SYS,		ID_u8IP_1,							ADDR_u8IP_1,							IS_CHAR},
	{E_TYPE_SYS,		ID_u8IP_2,							ADDR_u8IP_2,							IS_CHAR},		
	{E_TYPE_SYS,		ID_u8IP_3,							ADDR_u8IP_3,							IS_CHAR},
	{E_TYPE_SYS,		ID_u8IP_4,							ADDR_u8IP_4,							IS_CHAR},	
	{E_TYPE_SYS,		ID_u8IP_5,							ADDR_u8IP_5,							IS_CHAR},
	{E_TYPE_SYS,		ID_u8IP_6,							ADDR_u8IP_6,							IS_CHAR},	
	{E_TYPE_SYS,		ID_u8Mac_1,							ADDR_u8Mac_1,							IS_CHAR},
	{E_TYPE_SYS,		ID_u8Mac_2,							ADDR_u8Mac_2,							IS_CHAR},		
	{E_TYPE_SYS,		ID_u8Mac_3,							ADDR_u8Mac_3,							IS_CHAR},
	{E_TYPE_SYS,		ID_u8Mac_4,							ADDR_u8Mac_4,							IS_CHAR},	
	{E_TYPE_SYS,		ID_u8Mac_5,							ADDR_u8Mac_5,							IS_CHAR},
	{E_TYPE_SYS,		ID_u8Mac_6,							ADDR_u8Mac_6,							IS_CHAR},	
	{E_TYPE_SYS,		ID_u8Gateway_1,						ADDR_u8Gateway_1,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8Gateway_2,						ADDR_u8Gateway_2,						IS_CHAR},		
	{E_TYPE_SYS,		ID_u8Gateway_3,						ADDR_u8Gateway_3,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8Gateway_4,						ADDR_u8Gateway_4,						IS_CHAR},	
	{E_TYPE_SYS,		ID_u8Gateway_5,						ADDR_u8Gateway_5,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8Gateway_6,						ADDR_u8Gateway_6,						IS_CHAR},	
	{E_TYPE_SYS,		ID_u8Subnetmask_1,					ADDR_u8Subnetmask_1,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8Subnetmask_2,					ADDR_u8Subnetmask_2,					IS_CHAR},		
	{E_TYPE_SYS,		ID_u8Subnetmask_3,					ADDR_u8Subnetmask_3,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8Subnetmask_4,					ADDR_u8Subnetmask_4,					IS_CHAR},	
	{E_TYPE_SYS,		ID_u8Subnetmask_5,					ADDR_u8Subnetmask_5,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8Subnetmask_6,					ADDR_u8Subnetmask_6,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStarIP_1,					ADDR_u8PowerStarIP_1,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStarIP_2,					ADDR_u8PowerStarIP_2,					IS_CHAR},		
	{E_TYPE_SYS,		ID_u8PowerStarIP_3,					ADDR_u8PowerStarIP_3,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStarIP_4,					ADDR_u8PowerStarIP_4,					IS_CHAR},	
	{E_TYPE_SYS,		ID_u8PowerStarIP_5,					ADDR_u8PowerStarIP_5,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStarIP_6,					ADDR_u8PowerStarIP_6,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8SPDAlmMask,					ADDR_u8SPDAlmMask,						IS_CHAR},

	{E_TYPE_SYS,		ID_ParallelMode,					ADDR_PARALLEL_MODE,						IS_CHAR},
	{E_TYPE_SYS,		ID_EV_SYS_POWER,					ADDR_EV_SYS_POWER,						IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_RemoteServerIsDomainName_NUM,			ADDR_SETDATA_RemoteServerIsDomainName,						IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_DNSServerIP_NUM_1,					ADDR_DNSServerIP_1,					IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_DNSServerIP_NUM_2,					ADDR_DNSServerIP_2,					IS_CHAR},		
	{E_TYPE_SYS,		ID_SETDATA_DNSServerIP_NUM_3,					ADDR_DNSServerIP_3,					IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_DNSServerIP_NUM_4,					ADDR_DNSServerIP_4,					IS_CHAR},	
	{E_TYPE_SYS,		ID_SETDATA_DNSServerIP_NUM_5,					ADDR_DNSServerIP_5,					IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_DNSServerIP_NUM_6,					ADDR_DNSServerIP_6,					IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_RemoteServerDomainName_NUM_1,		ADDR_RemoteServerDomainName_1,						IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+1),	(ADDR_RemoteServerDomainName_1+1),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+2),	(ADDR_RemoteServerDomainName_1+2),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+3),	(ADDR_RemoteServerDomainName_1+3),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+4),	(ADDR_RemoteServerDomainName_1+4),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+5),	(ADDR_RemoteServerDomainName_1+5),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+6),	(ADDR_RemoteServerDomainName_1+6),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+7),	(ADDR_RemoteServerDomainName_1+7),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+8),	(ADDR_RemoteServerDomainName_1+8),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+9),	(ADDR_RemoteServerDomainName_1+9),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+10),	(ADDR_RemoteServerDomainName_1+10),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+11),	(ADDR_RemoteServerDomainName_1+11),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+12),	(ADDR_RemoteServerDomainName_1+12),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+13),	(ADDR_RemoteServerDomainName_1+13),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+14),	(ADDR_RemoteServerDomainName_1+14),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+15),	(ADDR_RemoteServerDomainName_1+15),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+16),	(ADDR_RemoteServerDomainName_1+16),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+17),	(ADDR_RemoteServerDomainName_1+17),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+18),	(ADDR_RemoteServerDomainName_1+18),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+19),	(ADDR_RemoteServerDomainName_1+19),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+20),	(ADDR_RemoteServerDomainName_1+20),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+21),	(ADDR_RemoteServerDomainName_1+21),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+22),	(ADDR_RemoteServerDomainName_1+22),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+23),	(ADDR_RemoteServerDomainName_1+23),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+24),	(ADDR_RemoteServerDomainName_1+24),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+25),	(ADDR_RemoteServerDomainName_1+25),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+26),	(ADDR_RemoteServerDomainName_1+26),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+27),	(ADDR_RemoteServerDomainName_1+27),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+28),	(ADDR_RemoteServerDomainName_1+28),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+29),	(ADDR_RemoteServerDomainName_1+29),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+30),	(ADDR_RemoteServerDomainName_1+30),					IS_CHAR},
	{E_TYPE_SYS,		(ID_SETDATA_RemoteServerDomainName_NUM_1+31),	(ADDR_RemoteServerDomainName_1+31),					IS_CHAR},


	{E_TYPE_SYS,		ID_1_BMS_POWER,					ADDR_1_BMS_POWER,					IS_CHAR},
	{E_TYPE_SYS,		ID_2_BMS_POWER,					ADDR_2_BMS_POWER,						IS_CHAR},

	

	//{E_TYPE_SYS,		ID_A_ChargeCMD,							INVALID_ITEM,						IS_CHAR},
	//{E_TYPE_SYS,		ID_B_ChargeCMD,							INVALID_ITEM,						IS_CHAR},

	{E_TYPE_WH1,		ID_fACWhCoef,						ADDR1_fACWhCoef,			IS_FLOAT},
	{E_TYPE_WH2,		ID_fACWhCoef,						ADDR2_fACWhCoef,			IS_FLOAT},
	{E_TYPE_WH1,		ID_u8WH_TYPE,						ADDR1_u8WH_TYPE,			IS_CHAR},
	{E_TYPE_WH2,		ID_u8WH_TYPE,						ADDR2_u8WH_TYPE,			IS_CHAR},
	{E_TYPE_WH1,		ID_u8WH_ALM_Mask,					ADDR1_u8WH_ALM_Mask,		IS_CHAR},
	{E_TYPE_WH2,		ID_u8WH_ALM_Mask,					ADDR2_u8WH_ALM_Mask,		IS_CHAR},

	{E_TYPE_IMIU1,		ID_u8JYMAlmStatMask,				ADDR1_u8JYMAlmStatMask,		IS_CHAR},
	{E_TYPE_IMIU2,		ID_u8JYMAlmStatMask,				ADDR2_u8JYMAlmStatMask,		IS_CHAR},
	{E_TYPE_IMIU1,		ID_u8SetJYMCtrlMode,				ADDR1_u8SetJYMCtrlMode,		IS_CHAR},
	{E_TYPE_IMIU2,		ID_u8SetJYMCtrlMode,				ADDR2_u8SetJYMCtrlMode,		IS_CHAR},
	{E_TYPE_IMIU1,		ID_u8InsltCheckByVolt,				ADDR1_u8InsltCheckByVolt,	IS_CHAR},
	{E_TYPE_IMIU2,		ID_u8InsltCheckByVolt,				ADDR2_u8InsltCheckByVolt,	IS_CHAR},
	{E_TYPE_IMIU1,		ID_fSetJYMAlm_Resist,				ADDR1_fSetJYMAlm_Resist,	IS_FLOAT},
	{E_TYPE_IMIU2,		ID_fSetJYMAlm_Resist,				ADDR2_fSetJYMAlm_Resist,	IS_FLOAT},

	{E_TYPE_SYS,		ID_u8StationCode_No1,				ADDR_u8EVStationCode1,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No2,				ADDR_u8EVStationCode2,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No3,				ADDR_u8EVStationCode3,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No4,				ADDR_u8EVStationCode4,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No5,				ADDR_u8EVStationCode5,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No6,				ADDR_u8EVStationCode6,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No7,				ADDR_u8EVStationCode7,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No8,				ADDR_u8EVStationCode8,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No9,				ADDR_u8EVStationCode9,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No10,				ADDR_u8EVStationCode10,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No11,				ADDR_u8EVStationCode11,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No12,				ADDR_u8EVStationCode12,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No13,				ADDR_u8EVStationCode13,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No14,				ADDR_u8EVStationCode14,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No15,				ADDR_u8EVStationCode15,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No16,				ADDR_u8EVStationCode16,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No17,				ADDR_u8EVStationCode17,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No18,				ADDR_u8EVStationCode18,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No19,				ADDR_u8EVStationCode19,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No20,				ADDR_u8EVStationCode20,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No21,				ADDR_u8EVStationCode21,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No22,				ADDR_u8EVStationCode22,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No23,				ADDR_u8EVStationCode23,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No24,				ADDR_u8EVStationCode24,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No25,				ADDR_u8EVStationCode25,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No26,				ADDR_u8EVStationCode26,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No27,				ADDR_u8EVStationCode27,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No28,				ADDR_u8EVStationCode28,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No29,				ADDR_u8EVStationCode29,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No30,				ADDR_u8EVStationCode30,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No31,				ADDR_u8EVStationCode31,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No32,				ADDR_u8EVStationCode32,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No33,				ADDR_u8EVStationCode33,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No34,				ADDR_u8EVStationCode34,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No35,				ADDR_u8EVStationCode35,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No36,				ADDR_u8EVStationCode36,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No37,				ADDR_u8EVStationCode37,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No38,				ADDR_u8EVStationCode38,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No39,				ADDR_u8EVStationCode39,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No40,				ADDR_u8EVStationCode40,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No41,				ADDR_u8EVStationCode41,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No42,				ADDR_u8EVStationCode42,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No43,				ADDR_u8EVStationCode43,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No44,				ADDR_u8EVStationCode44,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No45,				ADDR_u8EVStationCode45,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No46,				ADDR_u8EVStationCode46,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No47,				ADDR_u8EVStationCode47,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No48,				ADDR_u8EVStationCode48,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No49,				ADDR_u8EVStationCode49,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No50,				ADDR_u8EVStationCode50,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No51,				ADDR_u8EVStationCode51,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No52,				ADDR_u8EVStationCode52,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No53,				ADDR_u8EVStationCode53,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No54,				ADDR_u8EVStationCode54,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No55,				ADDR_u8EVStationCode55,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No56,				ADDR_u8EVStationCode56,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No57,				ADDR_u8EVStationCode57,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No58,				ADDR_u8EVStationCode58,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No59,				ADDR_u8EVStationCode59,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8StationCode_No60,				ADDR_u8EVStationCode60,		IS_CHAR},

	{E_TYPE_SYS,		ID_u8OffLinePwsd1,				ADDR_u8OffLinePwsd1,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8OffLinePwsd2,				ADDR_u8OffLinePwsd2,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8OffLinePwsd3,				ADDR_u8OffLinePwsd3,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8OffLinePwsd4,				ADDR_u8OffLinePwsd4,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8OffLinePwsd5,				ADDR_u8OffLinePwsd5,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8OffLinePwsd6,				ADDR_u8OffLinePwsd6,		IS_CHAR},

	{E_TYPE_SYS,		ID_SETDATA_TelaidianStarionID_NUM_1,		ADDR_TelaidianStarionID_1,			IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_TelaidianStarionID_NUM_32,		ADDR_TelaidianStarionID_32,			IS_CHAR},

	{E_TYPE_SYS,		ID_SETDATA_TelaidianUserName_NUM_1,		ADDR_TelaidianUserName_1,			IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_TelaidianUserName_NUM_32,		ADDR_TelaidianUserName_32,			IS_CHAR},

	{E_TYPE_SYS,		ID_SETDATA_TelaidianPassword_NUM_1,		ADDR_TelaidianPassword_1,			IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_TelaidianPassword_NUM_32,		ADDR_TelaidianPassword_32,			IS_CHAR},

	{E_TYPE_SYS,		ID_LocalTimeZone,						ADDR_LocalTimeZone,						IS_FLOAT},
	{E_TYPE_SYS,		ID_SETDATA_MASK_NET_NUM,				ADDR_MaskNet,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8BRUSH_ALM_Mask,				ADDR_u8BRUSH_ALM_Mask,						IS_CHAR},

	
	{E_TYPE_SYS,		ID_fTimePrice1,		ADDR_fTimePrice1,						IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+1),	(ADDR_fTimePrice1+1),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+2),	(ADDR_fTimePrice1+2),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+3),	(ADDR_fTimePrice1+3),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+4),	(ADDR_fTimePrice1+4),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+5),	(ADDR_fTimePrice1+5),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+6),	(ADDR_fTimePrice1+6),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+7),	(ADDR_fTimePrice1+7),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+8),	(ADDR_fTimePrice1+8),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+9),	(ADDR_fTimePrice1+9),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+10),	(ADDR_fTimePrice1+10),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+11),	(ADDR_fTimePrice1+11),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+12),	(ADDR_fTimePrice1+12),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+13),	(ADDR_fTimePrice1+13),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+14),	(ADDR_fTimePrice1+14),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+15),	(ADDR_fTimePrice1+15),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+16),	(ADDR_fTimePrice1+16),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+17),	(ADDR_fTimePrice1+17),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+18),	(ADDR_fTimePrice1+18),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+19),	(ADDR_fTimePrice1+19),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+20),	(ADDR_fTimePrice1+20),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+21),	(ADDR_fTimePrice1+21),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+22),	(ADDR_fTimePrice1+22),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+23),	(ADDR_fTimePrice1+23),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+24),	(ADDR_fTimePrice1+24),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+25),	(ADDR_fTimePrice1+25),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+26),	(ADDR_fTimePrice1+26),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+27),	(ADDR_fTimePrice1+27),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+28),	(ADDR_fTimePrice1+28),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+29),	(ADDR_fTimePrice1+29),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+30),	(ADDR_fTimePrice1+30),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+31),	(ADDR_fTimePrice1+31),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+32),	(ADDR_fTimePrice1+32),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+33),	(ADDR_fTimePrice1+33),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+34),	(ADDR_fTimePrice1+34),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+35),	(ADDR_fTimePrice1+35),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+36),	(ADDR_fTimePrice1+36),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+37),	(ADDR_fTimePrice1+37),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+38),	(ADDR_fTimePrice1+38),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+39),	(ADDR_fTimePrice1+39),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+40),	(ADDR_fTimePrice1+40),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+41),	(ADDR_fTimePrice1+41),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+42),	(ADDR_fTimePrice1+42),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+43),	(ADDR_fTimePrice1+43),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+44),	(ADDR_fTimePrice1+44),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+45),	(ADDR_fTimePrice1+45),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+46),	(ADDR_fTimePrice1+46),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimePrice1+47),	(ADDR_fTimePrice1+47),					IS_FLOAT},

	{E_TYPE_SYS,		ID_fTimeServicePrice1,		ADDR_fTimeServicePrice1,			IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+1),	(ADDR_fTimeServicePrice1+1),		IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+2),	(ADDR_fTimeServicePrice1+2),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+3),	(ADDR_fTimeServicePrice1+3),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+4),	(ADDR_fTimeServicePrice1+4),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+5),	(ADDR_fTimeServicePrice1+5),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+6),	(ADDR_fTimeServicePrice1+6),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+7),	(ADDR_fTimeServicePrice1+7),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+8),	(ADDR_fTimeServicePrice1+8),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+9),	(ADDR_fTimeServicePrice1+9),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+10),	(ADDR_fTimeServicePrice1+10),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+11),	(ADDR_fTimeServicePrice1+11),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+12),	(ADDR_fTimeServicePrice1+12),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+13),	(ADDR_fTimeServicePrice1+13),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+14),	(ADDR_fTimeServicePrice1+14),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+15),	(ADDR_fTimeServicePrice1+15),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+16),	(ADDR_fTimeServicePrice1+16),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+17),	(ADDR_fTimeServicePrice1+17),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+18),	(ADDR_fTimeServicePrice1+18),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+19),	(ADDR_fTimeServicePrice1+19),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+20),	(ADDR_fTimeServicePrice1+20),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+21),	(ADDR_fTimeServicePrice1+21),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+22),	(ADDR_fTimeServicePrice1+22),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+23),	(ADDR_fTimeServicePrice1+23),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+24),	(ADDR_fTimeServicePrice1+24),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+25),	(ADDR_fTimeServicePrice1+25),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+26),	(ADDR_fTimeServicePrice1+26),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+27),	(ADDR_fTimeServicePrice1+27),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+28),	(ADDR_fTimeServicePrice1+28),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+29),	(ADDR_fTimeServicePrice1+29),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+30),	(ADDR_fTimeServicePrice1+30),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+31),	(ADDR_fTimeServicePrice1+31),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+32),	(ADDR_fTimeServicePrice1+32),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+33),	(ADDR_fTimeServicePrice1+33),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+34),	(ADDR_fTimeServicePrice1+34),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+35),	(ADDR_fTimeServicePrice1+35),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+36),	(ADDR_fTimeServicePrice1+36),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+37),	(ADDR_fTimeServicePrice1+37),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+38),	(ADDR_fTimeServicePrice1+38),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+39),	(ADDR_fTimeServicePrice1+39),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+40),	(ADDR_fTimeServicePrice1+40),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+41),	(ADDR_fTimeServicePrice1+41),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+42),	(ADDR_fTimeServicePrice1+42),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+43),	(ADDR_fTimeServicePrice1+43),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+44),	(ADDR_fTimeServicePrice1+44),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+45),	(ADDR_fTimeServicePrice1+45),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+46),	(ADDR_fTimeServicePrice1+46),					IS_FLOAT},
	{E_TYPE_SYS,		(ID_fTimeServicePrice1+47),	(ADDR_fTimeServicePrice1+47),					IS_FLOAT},

	
	{E_TYPE_SYS,		ID_SETDATA_FeeMode_NUM,	ADDR_SETDATA_FeeMode_NUM,				IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_ServiceFeeMode_NUM,ADDR_SETDATA_ServiceFeeMode_NUM,	IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_fServicePrice_NUM,	ADDR_SETDATA_fServicePrice_NUM,	IS_FLOAT},
	{E_TYPE_SYS,		ID_SETDATA_fFixPrice_NUM,	ADDR_SETDATA_fFixPrice_NUM,			IS_FLOAT},


	
	{E_TYPE_SYS,		ID_SETDATA_AdjustMode_NUM,	ADDR_SETDATA_AdjustMode_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_Adjust_Volt1,	ADDR_SETDATA_Adjust_Volt1,			IS_FLOAT},
	{E_TYPE_SYS,		ID_SETDATA_Adjust_Volt2,	ADDR_SETDATA_Adjust_Volt2,			IS_FLOAT},	

	{E_TYPE_SYS,		ID_SETDATA_EVChargeCodeOnUI_NUM,ADDR_SETDATA_EVChargeCodeOnUI_NUM,	IS_CHAR},	

	{E_TYPE_SYS,		ID_NetFrameHead_NUM,			ADDR_NetFrameHead_NUM,			IS_ULONG},

	{E_TYPE_SYS,		ID_u8IsRingEnabled,				ADDR_u8IsRingEnabled,			IS_CHAR},
	{E_TYPE_SYS,		ID_fRingStartMergeCurr,			ADDR_fRingStartMergeCurr,		IS_FLOAT},
	{E_TYPE_SYS,		ID_bIsGunInsertMode,			ADDR_bIsGunInsertMode,			IS_CHAR},

	{E_TYPE_SYS,		ID_nGunInsertMode_Start_Hour,	ADDR_nGunInsertMode_Start_Hour,	IS_CHAR},
	{E_TYPE_SYS,		ID_nGunInsertMode_Start_Min,	ADDR_nGunInsertMode_Start_Min,	IS_CHAR},
	{E_TYPE_SYS,		ID_nGunInsertMode_Stop_Hour,	ADDR_nGunInsertMode_Stop_Hour,	IS_CHAR},
	{E_TYPE_SYS,		ID_nGunInsertMode_Stop_Min,		ADDR_nGunInsertMode_Stop_Min,	IS_CHAR},

	{E_TYPE_SYS,		ID_bySelfAddress,				ADDR_bySelfAddress,				IS_CHAR},
	{E_TYPE_SYS,		ID_nTotalRingStub,				ADDR_nTotalRingStub,			IS_CHAR},

	{E_TYPE_SYS,		ID_AllRingsPwrLmt,				ADDR_AllRingsPwrLmt,			IS_FLOAT},
	{E_TYPE_SYS,		ID_RingChgCurrIncSpeed,			ADDR_RingChgCurrIncSpeed,		IS_FLOAT},

	{E_TYPE_SYS,		ID_IsSingleGunMode,				ADDR_IsSingleGunMode,			IS_CHAR},

	{E_TYPE_SYS,		ID_fContactorShortThreshold,	ADDR_fContactorShortThreshold,	IS_FLOAT},

	{E_TYPE_SYS,		ID_EnGunOverTemDi,				ADDR_EnGunOverTemDi,			IS_CHAR},
	{E_TYPE_SYS,		ID_GunOverTemDiAlmStat,			ADDR_GunOverTemDiAlmStat,		IS_CHAR},
	{E_TYPE_SYS,		ID_fGunOverTemLmtCurrRatio,		ADDR_fGunOverTemLmtCurrRatio,	IS_FLOAT},
	{E_TYPE_SYS,		ID_fGunOverTemIntervalTime,		ADDR_fGunOverTemIntervalTime,	IS_FLOAT},

	{E_TYPE_SYS,		ID_u8ShuimuAutoEndSoc,			ADDR_u8ShuimuAutoEndSoc,		IS_CHAR},
	{E_TYPE_SYS,		ID_UserPassword_L2_1,			ADDR_UserPassword_L2_1,		IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L2_1+1),		(ADDR_UserPassword_L2_1+1),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L2_1+2),		(ADDR_UserPassword_L2_1+2),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L2_1+3),		(ADDR_UserPassword_L2_1+3),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L2_1+4),		(ADDR_UserPassword_L2_1+4),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L2_1+5),		(ADDR_UserPassword_L2_1+5),	IS_CHAR},

	{E_TYPE_SYS,		ID_UserPassword_L3_1,			ADDR_UserPassword_L3_1,		IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L3_1+1),		(ADDR_UserPassword_L3_1+1),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L3_1+2),		(ADDR_UserPassword_L3_1+2),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L3_1+3),		(ADDR_UserPassword_L3_1+3),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L3_1+4),		(ADDR_UserPassword_L3_1+4),	IS_CHAR},
	{E_TYPE_SYS,		(ID_UserPassword_L3_1+5),		(ADDR_UserPassword_L3_1+5),	IS_CHAR},

	{E_TYPE_SYS,		ID_StChgByOverVoltOrCurr_Source, ADDR_StChgByOverVoltOrCurr_Source,	IS_CHAR},	
	{E_TYPE_SYS,		ID_fMinOutVolt,					ADDR_MinOutVolt,		IS_FLOAT},

	{E_TYPE_SYS,		ID_u8FanMaxSpeed,			ADDR_u8FanMaxSpeed,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8VinChgEn,			ADDR_u8VinChgEn,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8ButtonChgEn,			ADDR_u8ButtonChgEn,		IS_CHAR},

	{E_TYPE_SYS,		ID_SETDATA_GunASetQrCode_NUM_1,			ADDR_GunASetQrCode_NUM_1,						IS_CHAR},
	{E_TYPE_SYS,		ID_SETDATA_GunBSetQrCode_NUM_1,			ADDR_GunBSetQrCode_NUM_1,						IS_CHAR},

	{E_TYPE_SYS,		ID_fDCMeterRatio,		ADDR_fDCMeterRatio,						IS_FLOAT},
	{E_TYPE_SYS,		ID_u8ChgModeSelEn,		ADDR_u8ChgModeSelEn,		IS_CHAR},
	{E_TYPE_SYS,		ID_fHallVoltK,				ADDR_fHallVoltK,					IS_FLOAT},
	{E_TYPE_SYS,		ID_fInsulationAlmCoeff,		ADDR_fInsulationAlmCoeff,					IS_FLOAT},
	{E_TYPE_SYS,		ID_u8PowerStar2IP_1,					ADDR_u8PowerStar2IP_1,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStar2IP_2,					ADDR_u8PowerStar2IP_2,					IS_CHAR},		
	{E_TYPE_SYS,		ID_u8PowerStar2IP_3,					ADDR_u8PowerStar2IP_3,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStar2IP_4,					ADDR_u8PowerStar2IP_4,					IS_CHAR},	
	{E_TYPE_SYS,		ID_u8PowerStar2IP_5,					ADDR_u8PowerStar2IP_5,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8PowerStar2IP_6,					ADDR_u8PowerStar2IP_6,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8Server2Enable,					ADDR_u8Server2Enable,					IS_CHAR},
	{E_TYPE_SYS,		ID_u32NET_PORT2,						ADDR_u32NET_PORT2,						IS_ULONG},
	{E_TYPE_SYS,		ID_u8ACInputContactorOpenDelay,			ADDR_u8ACInputContactorOpenDelay,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8DoorOpenDisconACInput,			ADDR_u8DoorOpenDisconACInput,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP1_MODULE_NUM,			ADDR_u8GRP1_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP2_MODULE_NUM,			ADDR_u8GRP2_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP3_MODULE_NUM,			ADDR_u8GRP3_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP4_MODULE_NUM,			ADDR_u8GRP4_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP5_MODULE_NUM,			ADDR_u8GRP5_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP6_MODULE_NUM,			ADDR_u8GRP6_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP7_MODULE_NUM,			ADDR_u8GRP7_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP8_MODULE_NUM,			ADDR_u8GRP8_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP9_MODULE_NUM,			ADDR_u8GRP9_MODULE_NUM,		IS_CHAR},
	{E_TYPE_SYS,		ID_u8GRP10_MODULE_NUM,			ADDR_u8GRP10_MODULE_NUM,	IS_CHAR},
	{E_TYPE_SYS,		ID_u8UserSetBMSPower,					ADDR_u8UserSetBMSPower,					IS_CHAR},
	{E_TYPE_SYS,		ID_u8DisplayStopKey,					ADDR_u8DisplayStopKey,						IS_CHAR},
	{E_TYPE_SYS,		ID_u8OrderChargeEnable,					ADDR_u8OrderChargeEnable,					IS_CHAR},

	{E_TYPE_INVALID,	INVALID_ITEM,					INVALID_ITEM,					IS_CHAR},


};
UINT8 CheckBrushSem(void)
{
	return gEvChargeInfo.u8BrushSem;
}

void Put_BrushSem(void)
{
	gEvChargeInfo.u8BrushSem = TRUE;
}

UINT8 Pop_BrushSem(void)
{
	if (gEvChargeInfo.u8BrushSem)
	{
		gEvChargeInfo.u8BrushSem = FALSE;
		return TRUE;
	}
	return FALSE;
}

void Put_A_StopSem(void)
{
	gEvChargeInfo.u8A_NoCardStopSem = TRUE;
}

void Put_B_StopSem(void)
{
	gEvChargeInfo.u8B_NoCardStopSem = TRUE;
}

UINT8 Pop_A_StopSem(void)
{
	if (gEvChargeInfo.u8A_NoCardStopSem)
	{
		gEvChargeInfo.u8A_NoCardStopSem = FALSE;
		return TRUE;
	}
	return FALSE;
}

UINT8 Pop_B_StopSem(void)
{
	if (gEvChargeInfo.u8B_NoCardStopSem)
	{
		gEvChargeInfo.u8B_NoCardStopSem = FALSE;
		return TRUE;
	}
	return FALSE;
}
void Put_Ether_3CSem(UINT8 u8SemVal)
{
	UINT8 uIdx = 0;
	for (uIdx = 0; uIdx < ETHER_SEM_NUM; uIdx++)
	{
		if (u8SemVal == gEvChargeInfo.u8EherSem[uIdx])
		{
			return;										//重复的也不用压了！！
		}
	}

	for (uIdx = 0; uIdx < ETHER_SEM_NUM; uIdx++)
	{
		if (ETHER_SEM_INVALID == gEvChargeInfo.u8EherSem[uIdx])
		{
			gEvChargeInfo.u8EherSem[uIdx] = u8SemVal;
			return;
		}
	}
}

UINT8 Pop_Ether_3CSem(void)
{
	UINT8 uIdx = 0;
	UINT8 uRst = 0;
	for (uIdx = 0; uIdx < ETHER_SEM_NUM; uIdx++)
	{
		if (ETHER_SEM_INVALID != gEvChargeInfo.u8EherSem[uIdx])
		{
			uRst = gEvChargeInfo.u8EherSem[uIdx];
			gEvChargeInfo.u8EherSem[uIdx] = ETHER_SEM_INVALID;
			return uRst;
		}
	}
	return ETHER_SEM_INVALID;
}

void Init_EtherSendRecordSem(void)
{
	gEvChargeInfo.u8SendRecordSem[0] = ETHER_SEM_INVALID;
	gEvChargeInfo.u8SendRecordSem[1] = ETHER_SEM_INVALID;
}

void Put_EtherSendRecordSem(UINT16 u16Idx)
{
	if (gEvChargeInfo.u8SendRecordSem[0] == ETHER_SEM_INVALID)
	{
		gEvChargeInfo.u8SendRecordSem[0] = TRUE;
		gEvChargeInfo.u16SendRecordIdx[0] = (UINT16)(MAX(0.1,(u16Idx-1)));
	}

	if (gEvChargeInfo.u8SendRecordSem[1] == ETHER_SEM_INVALID)
	{
		gEvChargeInfo.u8SendRecordSem[1] = TRUE;
		gEvChargeInfo.u16SendRecordIdx[1] = (UINT16)(MAX(0.1,(u16Idx-1)));
	}
}

UINT8 u8Pop_EtherSendRecordSem(UINT16* u16Idx)
{
	if (gEvChargeInfo.u8SendRecordSem[0] == TRUE)
	{
		gEvChargeInfo.u8SendRecordSem[0] = ETHER_SEM_INVALID;
		*u16Idx = gEvChargeInfo.u16SendRecordIdx[0];
		return TRUE;
	}

	if (gEvChargeInfo.u8SendRecordSem[1] == TRUE)
	{
		gEvChargeInfo.u8SendRecordSem[1] = ETHER_SEM_INVALID;
		*u16Idx = gEvChargeInfo.u16SendRecordIdx[1];
		return TRUE;
	}
	return FALSE;
}

void RecordTime(unsigned char *pTime)
{
	struct tm  tmNow;

	gRTCData.fGetLocalTime(&tmNow);
	*pTime++ = tmNow.tm_year - 2000;				/*年*/
	*pTime++ = tmNow.tm_mon;						/*月*/
	*pTime++ = tmNow.tm_mday;						/*日*/
	*pTime++ = tmNow.tm_hour;						/*时*/
	*pTime++ = tmNow.tm_min;						/*分*/
	*pTime   = tmNow.tm_sec;						/*秒*/
}

void Trap2(void)
{
	Trap1();
}

void Trap1(void)
{
	Trap();
}

void Trap(void)
{
	gSelfTest.fSetStopFeedDog();
}

void Delay(UINT32 delay_count)
{
	UINT32 i,j;
	j = (UINT32)(delay_count*3.5);
	for (i=0; i<j; i++) {}
}

unsigned char fabs(float fNum1, float fNum2)
{
	fNum1 = fNum1 - fNum2;
	if (fNum1 < 0)
		fNum1 = -fNum1;
	if (fNum1 > 0.0001)
		return TRUE;
	return FALSE;	
}

const int _ytab[2][12] =
{
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

long _timezone = 0;
long _dstbias = 0;

void SetE2prmValue(UINT8 Etype, UINT16 Idx, __XXX_UNION_VALUE unVal)
{
	float fVal = 0;
	UINT8 u8Val = 0;
	UINT16 u16Idx = 0;
	UINT32 u32Addr = INVALID_ITEM;
	UINT16 IdxOffset;

	IdxOffset = 0;
	if(Etype == E_TYPE_SYS)
	{
		if((Idx >= ID_SETDATA_TelaidianStarionID_NUM_1) && (Idx <= ID_SETDATA_TelaidianStarionID_NUM_32))
		{
			IdxOffset = Idx - ID_SETDATA_TelaidianStarionID_NUM_1;
			Idx = ID_SETDATA_TelaidianStarionID_NUM_1;
		}
		else if((Idx >= ID_SETDATA_TelaidianUserName_NUM_1) && (Idx <= ID_SETDATA_TelaidianUserName_NUM_32))
		{
			IdxOffset = Idx - ID_SETDATA_TelaidianUserName_NUM_1;
			Idx = ID_SETDATA_TelaidianUserName_NUM_1;
		}
		else if((Idx >= ID_SETDATA_TelaidianPassword_NUM_1) && (Idx <= ID_SETDATA_TelaidianPassword_NUM_32))
		{
			IdxOffset = Idx - ID_SETDATA_TelaidianPassword_NUM_1;
			Idx = ID_SETDATA_TelaidianPassword_NUM_1;
		}
		else if((Idx >= ID_SETDATA_GunASetQrCode_NUM_1) && (Idx <= ID_SETDATA_GunASetQrCode_NUM_128))
		{
			IdxOffset = Idx - ID_SETDATA_GunASetQrCode_NUM_1;
			Idx = ID_SETDATA_GunASetQrCode_NUM_1;
		}
		else if((Idx >= ID_SETDATA_GunBSetQrCode_NUM_1) && (Idx <= ID_SETDATA_GunBSetQrCode_NUM_128))
		{
			IdxOffset = Idx - ID_SETDATA_GunBSetQrCode_NUM_1;
			Idx = ID_SETDATA_GunBSetQrCode_NUM_1;
		}
	}

	for (u16Idx = 0; u16Idx < (sizeof(stArgsSigAddr))/sizeof(stArgsSigAddr[0]); u16Idx++)
	{
		if (stArgsSigAddr[u16Idx].u16SigIdx == Idx
			&& stArgsSigAddr[u16Idx].u8EType == Etype)
		{
			u32Addr = stArgsSigAddr[u16Idx].u32E2pAddr;
			break;
		}
	}

	if(stArgsSigAddr[u16Idx].u8SigType == IS_CHAR)
	{
		u32Addr += IdxOffset;
	}

	if (INVALID_ITEM != u32Addr)
	{
		if (stArgsSigAddr[u16Idx].u8SigType == IS_FLOAT)
		{
			if (u32Addr < FLOAT_U32_ARGS_MAX_ADDR)
			{
				fVal = unVal.ftemp;
				SetFloatSetData(u32Addr, &fVal);
			}
		}
		
		if (stArgsSigAddr[u16Idx].u8SigType == IS_CHAR)
		{
			if (u32Addr < CHAR_ARGS_MAX_ADDR)
			{
				u8Val = unVal.chtemp[0];
				SetCharSetData(u32Addr, &u8Val);
			}
		}
		
		if (stArgsSigAddr[u16Idx].u8SigType == IS_ULONG)
		{
			if (u32Addr < FLOAT_U32_ARGS_MAX_ADDR)
			{
				fVal = unVal.ftemp;
				SetFloatSetData(u32Addr, &fVal);
			}
		}
	}
}

// 把各个设备的 各变量也用 信号ID的方式标记，目的是不让全局变量各个文件飞！
// 后续可以定义个缓冲池，用malloc方式进行分配，使用共用体，取信号只用设备ID和信号ID。
// 压根不去操作结构体元素!,	这需要更进一步优化，往810G上靠拢！
void ExSetSigInfo(UINT8 Etype, UINT16 Idx, __XXX_UNION_VALUE unVal,void* p,UINT8 We2p)
{
	//float fVal = 0;
	//UINT8 u8Val = 0;
	void* pEqpInfo = NULL;
	switch (Etype)
	{
	case E_TYPE_SYS:
		pEqpInfo = (EV_CHARGE_INFO*)(&gEvChargeInfo);
		break;
	default:
		return;
		break;
	}

	if (pEqpInfo == NULL)
	{
		return;
	}

	if (BY_E2PROM == We2p)
	{
		SetE2prmValue(Etype, Idx, unVal);
	}

	if (E_TYPE_IMIU1 == Etype || E_TYPE_IMIU2 == Etype)
	{
		switch (Idx)
		{	

		case ID_u8QuickSmpIMIUFlag:
			((stIMIU_Info*)pEqpInfo)->u8QuickSmpIMIUFlag = unVal.chtemp[0];
			break;
		case ID_u8InsltCheckByVolt:
			((stIMIU_Info*)pEqpInfo)->u8InsltCheckByVolt = unVal.chtemp[0];
			break;
		case ID_u8JYMAlmStatMask:
			((stIMIU_Info*)pEqpInfo)->u8JYMAlmStatMask = unVal.chtemp[0];
			break;
		case ID_fSetJYMAlm_Resist:						//	绝缘告警阀值
			((stIMIU_Info*)pEqpInfo)->fSetJYMAlm_Resist[0] = unVal.ftemp;
			break;
		case ID_u8SetJYMCtrlMode:						//	绝缘检测控制状态 
			((stIMIU_Info*)pEqpInfo)->u8SetJYMCtrlStat[0] = unVal.chtemp[0];
			break;
		case ID_u8CTRLInsulationCMDFlag:
			((stIMIU_Info*)pEqpInfo)->u8CTRLInsulationCMDFlag[0] = unVal.chtemp[0];
			break;
		case ID_u8CTRLInsulationCMD:							//0x02待机，0x01检测，0x00脱离
			((stIMIU_Info*)pEqpInfo)->u8CTRLInsulationCMD[0] = unVal.chtemp[0];
			break;
		case ID_u8InsltCommAlm:									//绝缘检测板通信状态  TRUE 告警
			((stIMIU_Info*)pEqpInfo)->u8InsltCommAlm[0] = unVal.chtemp[0];
			break;
		default:
			break;
		}
	}
	else if (E_TYPE_WH1 == Etype || E_TYPE_WH2 == Etype)
	{
		switch (Idx)
		{
		case ID_WH_Reserved:
			break;
		case ID_u8WH_TYPE:
			((stWHInfo*)pEqpInfo)->u8WH_TYPE = unVal.chtemp[0];
			break;
		case ID_u8WH_ALM_Mask:
			((stWHInfo*)pEqpInfo)->u8WH_ALM_Mask = unVal.chtemp[0];
			break;
		case ID_u8WHCommAlm:
			((stWHInfo*)pEqpInfo)->u8WHCommAlm = unVal.chtemp[0];
			break;
		case ID_fWHRealValue:
			((stWHInfo*)pEqpInfo)->fWHRealValue = unVal.ftemp;
			break;
		case ID_fWH_A_Volt:
			((stWHInfo*)pEqpInfo)->fWH_A_Volt = unVal.ftemp;
			break;
		case ID_fWH_B_Volt:
			((stWHInfo*)pEqpInfo)->fWH_B_Volt = unVal.ftemp;
			break;
		case ID_fWH_C_Volt:
			((stWHInfo*)pEqpInfo)->fWH_C_Volt = unVal.ftemp;
			break;
		case ID_fWH_A_Curr:
			((stWHInfo*)pEqpInfo)->fWH_A_Curr = unVal.ftemp;
			break;
		case ID_fWH_B_Curr:
			((stWHInfo*)pEqpInfo)->fWH_B_Curr = unVal.ftemp;
			break;
		case ID_fWH_C_Curr:
			((stWHInfo*)pEqpInfo)->fWH_C_Curr = unVal.ftemp;
			break;
		case ID_fWHStartValue:
			((stWHInfo*)pEqpInfo)->fWHStartValue = unVal.ftemp;
			break;
		case ID_fWHStartTempValue:
			((stWHInfo*)pEqpInfo)->fWHStartTempValue = unVal.ftemp;
			break;
		case ID_fACWhCoef:
			((stWHInfo*)pEqpInfo)->fACWhCoef = unVal.ftemp;
			break;
		default:
			break;
		}
	}
	else if (E_TYPE_BMS1 == Etype || E_TYPE_BMS2 == Etype)
	{
		
		
	}
	else if (E_TYPE_BRUSH == Etype)
	{
		
	}
	else if (E_TYPE_SYS == Etype)
	{
		if((Idx >= ID_SETDATA_GunASetQrCode_NUM_1) && (Idx <= ID_SETDATA_GunASetQrCode_NUM_128))
		{
			gEvChargeInfo.u8GunASetQrCode[Idx-ID_SETDATA_GunASetQrCode_NUM_1] = unVal.chtemp[0];
			return;
		}
		else if((Idx >= ID_SETDATA_GunBSetQrCode_NUM_1) && (Idx <= ID_SETDATA_GunBSetQrCode_NUM_128))
		{
			gEvChargeInfo.u8GunBSetQrCode[Idx-ID_SETDATA_GunBSetQrCode_NUM_1] = unVal.chtemp[0];
			return;
		}

		switch (Idx)
		{
		//浮点数
		case ID_fCC1_1Coef_A:
			((EV_CHARGE_INFO*)pEqpInfo)->fCC1_1Coef_A = unVal.ftemp;
			break;
		case ID_fCC1_1Coef_B:
			((EV_CHARGE_INFO*)pEqpInfo)->fCC1_1Coef_B = unVal.ftemp;
			break;
		case ID_fCC1_2Coef_A:
			((EV_CHARGE_INFO*)pEqpInfo)->fCC1_2Coef_A = unVal.ftemp;
			break;
		case ID_fCC1_2Coef_B:
			((EV_CHARGE_INFO*)pEqpInfo)->fCC1_2Coef_B = unVal.ftemp;
			break;
		case ID_fSysSetOverVolt:
			((EV_CHARGE_INFO*)pEqpInfo)->fSysSetOverVolt = unVal.ftemp;
			break;
		case ID_fRectLoadStartTime:
			((EV_CHARGE_INFO*)pEqpInfo)->fRectLoadStartTime = unVal.ftemp;
			break;
		case ID_fMaxOutCurr:
			((EV_CHARGE_INFO*)pEqpInfo)->fMaxOutCurr = unVal.ftemp;
			break;
		case ID_fMaxOutVolt:
			((EV_CHARGE_INFO*)pEqpInfo)->fMaxOutVolt = unVal.ftemp;
			break;
		case ID_fMinOutVolt:
			((EV_CHARGE_INFO*)pEqpInfo)->fMinOutVolt = unVal.ftemp;
			break;
		
		case ID_fSetLimitMaxOutCurr_1:
			((EV_CHARGE_INFO*)pEqpInfo)->fSetLimitMaxOutCurr_1 = unVal.ftemp;
			break;		
		case ID_fSetLimitMaxOutCurr_2:
			((EV_CHARGE_INFO*)pEqpInfo)->fSetLimitMaxOutCurr_2 = unVal.ftemp;
			break;

		//4字节型
		case ID_u32PowerOnTimes:
			((EV_CHARGE_INFO*)pEqpInfo)->u32PowerOnTimes = unVal.u32Value;
			break;
		case ID_u32_B_CTS_DB_T1:
			((EV_CHARGE_INFO*)pEqpInfo)->u32_B_CTS_DB_T1 = unVal.u32Value;
			break;
		case ID_u32_B_DB_DB_T2:
			((EV_CHARGE_INFO*)pEqpInfo)->u32_B_DB_DB_T2 = unVal.u32Value;
			break;
		case ID_u32_A_CTS_DB_T1:
			((EV_CHARGE_INFO*)pEqpInfo)->u32_A_CTS_DB_T1 = unVal.u32Value;
			break;
		case ID_u32_A_DB_DB_T2:
			((EV_CHARGE_INFO*)pEqpInfo)->u32_A_DB_DB_T2 = unVal.u32Value;
			break;
		case ID_u16SetBCLTimeOutVal_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u16SetBCLTimeOutVal_1 = unVal.u32Value;
			break;
		case ID_u16SetBCLTimeOutVal_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u16SetBCLTimeOutVal_2 = unVal.u32Value;
			break;
		case ID_u16SetBRMTimeOutVal_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u16SetBRMTimeOutVal_1 = unVal.u32Value;
			break;
		case ID_u16SetBRMTimeOutVal_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u16SetBRMTimeOutVal_2 = unVal.u32Value;
			break;		
		case ID_u16SetBCSTimeOutVal_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u16SetBCSTimeOutVal_1 = unVal.u32Value;
			break;		
		case ID_u16SetBCSTimeOutVal_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u16SetBCSTimeOutVal_2 = unVal.u32Value;
			break;	
		case ID_u32NET_PORT:
			((EV_CHARGE_INFO*)pEqpInfo)->u32NET_PORT = unVal.u32Value;
			break;
		case ID_u32NET_PORT2:
			((EV_CHARGE_INFO*)pEqpInfo)->u32NET_PORT2 = unVal.u32Value;
			break;
		case ID_NetFrameHead_NUM:
			gs_EthAppCommMng.u16NetFrameHead = unVal.u32Value;
			break;
		case ID_NET_DHCP:		// NULL!!
			break;


		//字节型
		case ID_u8PolarityAlmMask_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PolarityAlmMask_1 = unVal.chtemp[0];
			break;			
		case ID_u8PolarityAlmMask_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PolarityAlmMask_2 = unVal.chtemp[0];
			break;

		case ID_fMaxOffLineTime:
			((EV_CHARGE_INFO*)pEqpInfo)->fMaxOffLineTime = unVal.ftemp;
			break;

		case ID_u8SetCalcMoneyType:
			((EV_CHARGE_INFO*)pEqpInfo)->u8SetCalcMoneyType = unVal.chtemp[0];
			break;

		case ID_u8CC1CheckEnable_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8CC1CheckEnable_1 = unVal.chtemp[0];
			break;

		case ID_u8CC1CheckEnable_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8CC1CheckEnable_2 = unVal.chtemp[0];
			break;

		case ID_u8SelfTestEnable_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8SelfTestEnable_1 = unVal.chtemp[0];
			break;

		case ID_u8SelfTestEnable_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8SelfTestEnable_2 = unVal.chtemp[0];
			break;

		case ID_u8MCCBOnCheckEnable_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8MCCBOnCheckEnable_1 = unVal.chtemp[0];
			break;		
		case ID_u8MCCBOnCheckEnable_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8MCCBOnCheckEnable_2 = unVal.chtemp[0];
			break;		
		case ID_u8OutOverVoltEnable_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8OutOverVoltEnable_1 = unVal.chtemp[0];
			break;		

		case ID_u8OutOverVoltEnable_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8OutOverVoltEnable_2 = unVal.chtemp[0];
			break;		
		case ID_u8DoorAlmMask_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DoorAlmMask_1 = unVal.chtemp[0];
			break;				
		case ID_u8DoorAlmMask_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DoorAlmMask_2 = unVal.chtemp[0];
			break;				
		case ID_u8HasElec_ALock:
			((EV_CHARGE_INFO*)pEqpInfo)->u8HasElec_ALock = unVal.chtemp[0];
			break;			
		case ID_u8HasElec_BLock:
			((EV_CHARGE_INFO*)pEqpInfo)->u8HasElec_BLock = unVal.chtemp[0];
			break;		
		case ID_u8PWMPolarityCover:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PWMPolarityCover = unVal.chtemp[0];
			break;
		case ID_u8_A_LOCKType:
			((EV_CHARGE_INFO*)pEqpInfo)->u8_A_LOCKType = unVal.chtemp[0];
			break;			
		case ID_u8_B_LOCKType:
			((EV_CHARGE_INFO*)pEqpInfo)->u8_B_LOCKType = unVal.chtemp[0];
			break;
		case ID_u8LOCK_A_DIType:
			((EV_CHARGE_INFO*)pEqpInfo)->u8LOCK_A_DIType = unVal.chtemp[0];
			break;				
		case ID_u8LOCK_B_DIType:
			((EV_CHARGE_INFO*)pEqpInfo)->u8LOCK_B_DIType = unVal.chtemp[0];
			break;				
		case ID_u8EPOAlmVltLEVEL:
			((EV_CHARGE_INFO*)pEqpInfo)->u8EPOAlmVltLEVEL = unVal.chtemp[0];
			break;			
		case ID_u8BrushFactorType:
			((EV_CHARGE_INFO*)pEqpInfo)->u8BrushFactorType = unVal.chtemp[0];
			break;		
		case ID_u8EV_StartTemp:
			((EV_CHARGE_INFO*)pEqpInfo)->u8EV_StartTemp = unVal.chtemp[0];
			break;			
		case ID_u8EV_StopTemp:
			((EV_CHARGE_INFO*)pEqpInfo)->u8EV_StopTemp = unVal.chtemp[0];
			break;				
		case ID_u8NET_OffLine_Charge_Enable:
			((EV_CHARGE_INFO*)pEqpInfo)->u8NET_OffLine_Charge_Enable = unVal.chtemp[0];
			break;
		case ID_A_ChargeCMD:
			((EV_CHARGE_INFO*)pEqpInfo)->u8_Charge_CMD[GUN_NO_A] = unVal.chtemp[0];
			break;
		case ID_B_ChargeCMD:
			((EV_CHARGE_INFO*)pEqpInfo)->u8_Charge_CMD[GUN_NO_B] = unVal.chtemp[0];
			break;
		case ID_u8_A_ForceVer2011:
			((EV_CHARGE_INFO*)pEqpInfo)->u8_A_ForceVer2011 = unVal.chtemp[0];
			break;
		case ID_u8_B_ForceVer2011:
			((EV_CHARGE_INFO*)pEqpInfo)->u8_B_ForceVer2011 = unVal.chtemp[0];
			break;
		case ID_u8IP_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8IP_1 = unVal.chtemp[0];
			break;
		case ID_u8IP_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8IP_2 = unVal.chtemp[0];
			break;
		case ID_u8IP_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8IP_3 = unVal.chtemp[0];
			break;
		case ID_u8IP_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8IP_4 = unVal.chtemp[0];
			break;
		case ID_u8IP_5:
			((EV_CHARGE_INFO*)pEqpInfo)->u8IP_5 = unVal.chtemp[0];
			break;
		case ID_u8IP_6:
			((EV_CHARGE_INFO*)pEqpInfo)->u8IP_6 = unVal.chtemp[0];
			break;
		case ID_u8Mac_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Mac[0] = unVal.chtemp[0];
			break;
		case ID_u8Mac_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Mac[1] = unVal.chtemp[0];
			break;			
		case ID_u8Mac_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Mac[2] = unVal.chtemp[0];
			break;			
		case ID_u8Mac_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Mac[3] = unVal.chtemp[0];
			break;			
		case ID_u8Mac_5:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Mac[4] = unVal.chtemp[0];
			break;			
		case ID_u8Mac_6:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Mac[5] = unVal.chtemp[0];
			break;			
		case ID_u8Gateway_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Gateway[0] = unVal.chtemp[0];
			break;		
		case ID_u8Gateway_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Gateway[1] = unVal.chtemp[0];
			break;		
		case ID_u8Gateway_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Gateway[2] = unVal.chtemp[0];
			break;		
		case ID_u8Gateway_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Gateway[3] = unVal.chtemp[0];
			break;		
		case ID_u8Gateway_5:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Gateway[4] = unVal.chtemp[0];
			break;		
		case ID_u8Gateway_6:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Gateway[5] = unVal.chtemp[0];
			break;		
		case ID_u8Subnetmask_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Subnetmask[0] = unVal.chtemp[0];
			break;	
		case ID_u8Subnetmask_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Subnetmask[1] = unVal.chtemp[0];
			break;	
		case ID_u8Subnetmask_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Subnetmask[2] = unVal.chtemp[0];
			break;	
		case ID_u8Subnetmask_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Subnetmask[3] = unVal.chtemp[0];
			break;	
		case ID_u8Subnetmask_5:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Subnetmask[4] = unVal.chtemp[0];
			break;	
		case ID_u8Subnetmask_6:
			((EV_CHARGE_INFO*)pEqpInfo)->u8Subnetmask[5] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStarIP_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStarIP[0] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStarIP_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStarIP[1] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStarIP_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStarIP[2] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStarIP_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStarIP[3] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStarIP_5:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStarIP[4] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStarIP_6:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStarIP[5] = unVal.chtemp[0];
			break;
		case ID_u8SPDAlmMask:
			((EV_CHARGE_INFO*)pEqpInfo)->u8SPDAlmMask = unVal.chtemp[0];
			break;
		case ID_SETDATA_MASK_NET_NUM:
			((EV_CHARGE_INFO*)pEqpInfo)->u8NET_Mask = unVal.chtemp[0];
			break;
		case ID_u8Server2Enable:
			((EV_CHARGE_INFO*)pEqpInfo)->u8NET2_Enable = unVal.chtemp[0];
			break;

		case ID_ParallelMode:
			((EV_CHARGE_INFO*)pEqpInfo)->ParallelMode = unVal.chtemp[0];
			break;

		case ID_u8BRUSH_ALM_Mask:
			((EV_CHARGE_INFO*)pEqpInfo)->u8BRUSH_ALM_Mask = unVal.chtemp[0];
			break;


		case ID_u8StationCode_No1:case ID_u8StationCode_No2:case ID_u8StationCode_No3:case ID_u8StationCode_No4:case ID_u8StationCode_No5:case ID_u8StationCode_No6:case ID_u8StationCode_No7:
		case ID_u8StationCode_No8:case ID_u8StationCode_No9:case ID_u8StationCode_No10:case ID_u8StationCode_No11:case ID_u8StationCode_No12:case ID_u8StationCode_No13:case ID_u8StationCode_No14:
		case ID_u8StationCode_No15:case ID_u8StationCode_No16:case ID_u8StationCode_No17:case ID_u8StationCode_No18:case ID_u8StationCode_No19:case ID_u8StationCode_No20:case ID_u8StationCode_No21:
		case ID_u8StationCode_No22:case ID_u8StationCode_No23:case ID_u8StationCode_No24:case ID_u8StationCode_No25:case ID_u8StationCode_No26:case ID_u8StationCode_No27:case ID_u8StationCode_No28:
		case ID_u8StationCode_No29:case ID_u8StationCode_No30:case ID_u8StationCode_No31:case ID_u8StationCode_No32:case ID_u8StationCode_No33:case ID_u8StationCode_No34:case ID_u8StationCode_No35:
		case ID_u8StationCode_No36:case ID_u8StationCode_No37:case ID_u8StationCode_No38:case ID_u8StationCode_No39:case ID_u8StationCode_No40:case ID_u8StationCode_No41:case ID_u8StationCode_No42:
		case ID_u8StationCode_No43:case ID_u8StationCode_No44:case ID_u8StationCode_No45:case ID_u8StationCode_No46:case ID_u8StationCode_No47:case ID_u8StationCode_No48:case ID_u8StationCode_No49:
		case ID_u8StationCode_No50:case ID_u8StationCode_No51:case ID_u8StationCode_No52:case ID_u8StationCode_No53:case ID_u8StationCode_No54:case ID_u8StationCode_No55:case ID_u8StationCode_No56:
		case ID_u8StationCode_No57:case ID_u8StationCode_No58:case ID_u8StationCode_No59:case ID_u8StationCode_No60:

			((EV_CHARGE_INFO*)pEqpInfo)->u8EVCode[Idx-ID_u8StationCode_No1] = unVal.chtemp[0];
			break;

		case ID_u8OffLinePwsd1:case ID_u8OffLinePwsd2:case ID_u8OffLinePwsd3:case ID_u8OffLinePwsd4:case ID_u8OffLinePwsd5:case ID_u8OffLinePwsd6:
			((EV_CHARGE_INFO*)pEqpInfo)->u16UserPassword[0][Idx-ID_u8OffLinePwsd1] = unVal.chtemp[0];
			break;

		case ID_UserPassword_L2_1:case (ID_UserPassword_L2_1+1):case (ID_UserPassword_L2_1+2):case (ID_UserPassword_L2_1+3):case (ID_UserPassword_L2_1+4):case (ID_UserPassword_L2_1+5):
			((EV_CHARGE_INFO*)pEqpInfo)->u16UserPassword[1][Idx-ID_UserPassword_L2_1] = unVal.chtemp[0];
			break;
		case ID_UserPassword_L3_1:case (ID_UserPassword_L3_1+1):case (ID_UserPassword_L3_1+2):case (ID_UserPassword_L3_1+3):case (ID_UserPassword_L3_1+4):case (ID_UserPassword_L3_1+5):
			((EV_CHARGE_INFO*)pEqpInfo)->u16UserPassword[2][Idx-ID_UserPassword_L3_1] = unVal.chtemp[0];
			break;			

		case ID_1_BMS_POWER:
			((EV_CHARGE_INFO*)pEqpInfo)->u8BMSPowerType[GUN_NO_A] = unVal.chtemp[0];
			break;
		case ID_2_BMS_POWER:
			((EV_CHARGE_INFO*)pEqpInfo)->u8BMSPowerType[GUN_NO_B] = unVal.chtemp[0];
			break;

		case ID_SETDATA_DNSServerIP_NUM_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DNSIp[0] = unVal.chtemp[0];
			break;	
		case ID_SETDATA_DNSServerIP_NUM_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DNSIp[1] = unVal.chtemp[0];
			break;	
		case ID_SETDATA_DNSServerIP_NUM_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DNSIp[2] = unVal.chtemp[0];
			break;	
		case ID_SETDATA_DNSServerIP_NUM_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DNSIp[3] = unVal.chtemp[0];
			break;	

		case ID_SETDATA_RemoteServerIsDomainName_NUM:
			((EV_CHARGE_INFO*)pEqpInfo)->u8RemoteServerIsDomainName = unVal.chtemp[0];
			break;		
		case ID_SETDATA_RemoteServerDomainName_NUM_1:case (ID_SETDATA_RemoteServerDomainName_NUM_1+1):case (ID_SETDATA_RemoteServerDomainName_NUM_1+2):case (ID_SETDATA_RemoteServerDomainName_NUM_1+3):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+4):case (ID_SETDATA_RemoteServerDomainName_NUM_1+5):case (ID_SETDATA_RemoteServerDomainName_NUM_1+6):case (ID_SETDATA_RemoteServerDomainName_NUM_1+7):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+8):case (ID_SETDATA_RemoteServerDomainName_NUM_1+9):case (ID_SETDATA_RemoteServerDomainName_NUM_1+10):case (ID_SETDATA_RemoteServerDomainName_NUM_1+11):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+12):case (ID_SETDATA_RemoteServerDomainName_NUM_1+13):case (ID_SETDATA_RemoteServerDomainName_NUM_1+14):case (ID_SETDATA_RemoteServerDomainName_NUM_1+15):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+16):case (ID_SETDATA_RemoteServerDomainName_NUM_1+17):case (ID_SETDATA_RemoteServerDomainName_NUM_1+18):case (ID_SETDATA_RemoteServerDomainName_NUM_1+19):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+20):case (ID_SETDATA_RemoteServerDomainName_NUM_1+21):case (ID_SETDATA_RemoteServerDomainName_NUM_1+22):case (ID_SETDATA_RemoteServerDomainName_NUM_1+23):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+24):case (ID_SETDATA_RemoteServerDomainName_NUM_1+25):case (ID_SETDATA_RemoteServerDomainName_NUM_1+26):case (ID_SETDATA_RemoteServerDomainName_NUM_1+27):
		case (ID_SETDATA_RemoteServerDomainName_NUM_1+28):case (ID_SETDATA_RemoteServerDomainName_NUM_1+29):case (ID_SETDATA_RemoteServerDomainName_NUM_1+30):case (ID_SETDATA_RemoteServerDomainName_NUM_1+31):
			((EV_CHARGE_INFO*)pEqpInfo)->u16ServerName[Idx-ID_SETDATA_RemoteServerDomainName_NUM_1] = unVal.chtemp[0];
			break;

		case ID_u8IsRingEnabled:
			gEvChargeInfo.u8IsRingEnabled = unVal.chtemp[0];
			break;	
		case ID_bySelfAddress:
			gEvChargeInfo.bySelfAddress = unVal.chtemp[0];
			break;
		case ID_nTotalRingStub:
			gEvChargeInfo.nTotalRingStub = unVal.chtemp[0];
			break;
		case ID_IsSingleGunMode:
			gEvChargeInfo.u8IsSingleGunMode = unVal.chtemp[0];			
			break;


		case ID_EnGunOverTemDi:
			gEvChargeInfo.u8EnGunOverTemDi = unVal.chtemp[0];	
			break;
		case ID_GunOverTemDiAlmStat:
			gEvChargeInfo.u8GunOverTemDiAlmStatCfg = unVal.chtemp[0];	
			break;

		case ID_fContactorShortThreshold:
			gEvChargeInfo.fContactorShortThreshold  = unVal.ftemp;
			break;		
		case ID_fGunOverTemLmtCurrRatio:
			gEvChargeInfo.fGunOverTemLmtCurrRatio  = unVal.ftemp;
			break;		
		case ID_fGunOverTemIntervalTime:
			gEvChargeInfo.fGunOverTemIntervalTime  = unVal.ftemp;
			break;		
		case ID_StChgByOverVoltOrCurr_Source:
			gEvChargeInfo.u8StChgByOverVoltOrCurr_Source = unVal.chtemp[0];
			break;
		case ID_u8ShuimuAutoEndSoc:
			gEvChargeInfo.u8ShuimuAutoEndSoc = unVal.chtemp[0];			
			break;

		case ID_u8FanMaxSpeed:
			gEvChargeInfo.u8FanMaxSpeed = unVal.chtemp[0];			
			break;
		case ID_u8VinChgEn:
			gEvChargeInfo.u8VinChgEn = unVal.chtemp[0];			
			break;
		case ID_u8ButtonChgEn:
			gEvChargeInfo.u8ButtonChgEn = unVal.chtemp[0];			
			break;
		case ID_fDCMeterRatio:
			gEvChargeInfo.fDCMeterRatio = unVal.ftemp;
			break;
		case ID_u8ChgModeSelEn:
			gEvChargeInfo.u8ChgModeSelEn = unVal.chtemp[0];			
			break;	
		case ID_fHallVoltK:
			gEvChargeInfo.fHallVoltK = unVal.ftemp;
			break;		
		case ID_fInsulationAlmCoeff:
			gEvChargeInfo.fInsulationAlmCoeff = unVal.ftemp;
			break;		
		case ID_u8PowerStar2IP_1:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStar2IP[0] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStar2IP_2:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStar2IP[1] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStar2IP_3:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStar2IP[2] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStar2IP_4:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStar2IP[3] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStar2IP_5:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStar2IP[4] = unVal.chtemp[0];
			break;	
		case ID_u8PowerStar2IP_6:
			((EV_CHARGE_INFO*)pEqpInfo)->u8PowerStar2IP[5] = unVal.chtemp[0];
			break;
		case ID_u8ACInputContactorOpenDelay:
			((EV_CHARGE_INFO*)pEqpInfo)->u8ACInputContactorOpenDelayCfg = unVal.chtemp[0];
			break;	
		case ID_u8DoorOpenDisconACInput:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DoorOpenDisconACInput = unVal.chtemp[0];
			break;	

		case ID_u8UserSetBMSPower:
			((EV_CHARGE_INFO*)pEqpInfo)->u8UserSetBMSPower = unVal.chtemp[0];
			break;	

		case ID_u8DisplayStopKey:
			((EV_CHARGE_INFO*)pEqpInfo)->u8DisplayStopKey = unVal.chtemp[0];
			break;	

		case ID_u8OrderChargeEnable:
			((EV_CHARGE_INFO*)pEqpInfo)->u8OrderChargeEnable = unVal.chtemp[0];
			break;	

		default:
			break;
		}
	}
	else
	{
		
	}
	return;
}


__XXX_UNION_VALUE unionGetE2pVal(UINT8 Etype, UINT16 Idx, unsigned char* p)
{
	__XXX_UNION_VALUE unVal;
        unVal.chtemp[0] = 0xff;
	float fdata = 0;
	UINT8 u8data = 0;
	UINT16 u16Idx = 0;
	UINT32 u32Addr = INVALID_ITEM;
	UINT16 IdxOffset;

	IdxOffset = 0;
	if(Etype == E_TYPE_SYS)
	{
		if((Idx >= ID_SETDATA_TelaidianStarionID_NUM_1) && (Idx <= ID_SETDATA_TelaidianStarionID_NUM_32))
		{
			IdxOffset = Idx - ID_SETDATA_TelaidianStarionID_NUM_1;
			Idx = ID_SETDATA_TelaidianStarionID_NUM_1;			
		}
		else if((Idx >= ID_SETDATA_TelaidianUserName_NUM_1) && (Idx <= ID_SETDATA_TelaidianUserName_NUM_32))
		{
			IdxOffset = Idx - ID_SETDATA_TelaidianUserName_NUM_1;
			Idx = ID_SETDATA_TelaidianUserName_NUM_1;
		}
		else if((Idx >= ID_SETDATA_TelaidianPassword_NUM_1) && (Idx <= ID_SETDATA_TelaidianPassword_NUM_32))
		{
			IdxOffset = Idx - ID_SETDATA_TelaidianPassword_NUM_1;
			Idx = ID_SETDATA_TelaidianPassword_NUM_1;
		}
		else if((Idx >= ID_SETDATA_GunASetQrCode_NUM_1) && (Idx <= ID_SETDATA_GunASetQrCode_NUM_128))
		{
			IdxOffset = Idx - ID_SETDATA_GunASetQrCode_NUM_1;
			Idx = ID_SETDATA_GunASetQrCode_NUM_1;
		}
		else if((Idx >= ID_SETDATA_GunBSetQrCode_NUM_1) && (Idx <= ID_SETDATA_GunBSetQrCode_NUM_128))
		{
			IdxOffset = Idx - ID_SETDATA_GunBSetQrCode_NUM_1;
			Idx = ID_SETDATA_GunBSetQrCode_NUM_1;
		}
	}

	for (u16Idx = 0; u16Idx < ((sizeof(stArgsSigAddr))/sizeof(stArgsSigAddr[0])); u16Idx++)
	{
		if (stArgsSigAddr[u16Idx].u16SigIdx == Idx
			&& stArgsSigAddr[u16Idx].u8EType == Etype)
		{
			u32Addr = stArgsSigAddr[u16Idx].u32E2pAddr;
			break;
		}
	}

	//获取数据信号类型 
        if(p != NULL)
	{
          *p = stArgsSigAddr[u16Idx].u8SigType;
        }
	
	if(stArgsSigAddr[u16Idx].u8SigType == IS_CHAR)
	{
		u32Addr += IdxOffset;
	}

	if (stArgsSigAddr[u16Idx].u8SigType == IS_FLOAT)
	{
		if (INVALID_ITEM != u32Addr && u32Addr < FLOAT_U32_ARGS_MAX_ADDR)
		{
			GetFloatSetData(u32Addr, &fdata);
			unVal.ftemp = fdata;
		}
	}
	else if (stArgsSigAddr[u16Idx].u8SigType == IS_CHAR)
	{
		if (INVALID_ITEM != u32Addr && u32Addr < CHAR_ARGS_MAX_ADDR)
		{
			GetCharSetData(u32Addr, &u8data);
			unVal.chtemp[0] = u8data;
		}
	}
	else if (stArgsSigAddr[u16Idx].u8SigType == IS_ULONG)
	{
		if (INVALID_ITEM != u32Addr && u32Addr < FLOAT_U32_ARGS_MAX_ADDR)
		{
			GetFloatSetData(u32Addr, &fdata);
			unVal.ftemp = fdata;				//因为存的时候是按4个字节FLOAT 方式存的，所以出去一转换即可。
		}
	}

	return unVal;
}

// 把各个设备的 各变量也用 信号ID的方式标记，目的是不让全局变量各个文件飞！
// 后续可以定义个缓冲池，用malloc方式进行分配，使用共用体，取信号只用设备ID和信号ID。
// 压根不去操作结构体元素!,	这需要更进一步优化，往810G上靠拢！
void ExGetSigInfo(UINT8 Etype, UINT16 Idx, __XXX_UNION_VALUE* unVal,void* p,UINT8 Re2p)
{
	__XXX_UNION_VALUE unTemp;
	void *pEqpInfo = NULL;
	__XXX_UNION_VALUE* PunVal = NULL;
	//p = NULL;
	unsigned char *pSigType = NULL;
	//UNUSED(p);
	PunVal = unVal;
	pSigType = p;

	switch (Etype)
	{
	case E_TYPE_SYS:
		pEqpInfo = (EV_CHARGE_INFO*)(&gEvChargeInfo);
		break;
	default:
		return;
		break;
	}

	if (pEqpInfo == NULL)
	{
		return;
	}

	unTemp = unionGetE2pVal(Etype, Idx, pSigType);
	*PunVal = unTemp;
	return;
}


struct tm *gmtime_r(const time_t *timer, struct tm *tmbuf)
{
	time_t time = *timer;
	unsigned long dayclock, dayno;
	int year = EPOCH_YR;

	dayclock = (unsigned long) time % SECS_DAY;
	dayno = (unsigned long) time / SECS_DAY;

	tmbuf->tm_sec = dayclock % 60;
	tmbuf->tm_min = (dayclock % 3600) / 60;
	tmbuf->tm_hour = dayclock / 3600;
	tmbuf->tm_wday = (dayno + 4) % 7; // Day 0 was a thursday
	while (dayno >= (unsigned long) YEARSIZE(year))
	{
		dayno -= YEARSIZE(year);
		year++;
	}
	tmbuf->tm_year = year - YEAR0;
	tmbuf->tm_yday = dayno;
	tmbuf->tm_mon = 0;
	while (dayno >= (unsigned long) _ytab[LEAPYEAR(year)][tmbuf->tm_mon])
	{
		dayno -= _ytab[LEAPYEAR(year)][tmbuf->tm_mon];
		tmbuf->tm_mon++;
	}
	tmbuf->tm_mday = dayno + 1;
	tmbuf->tm_isdst = 0;

	return tmbuf;
}

time_t Getsec(INT16 Index)
{
	UCHAR Mon, Day, Hour, i;
	UINT16 NumDay = 0;
	time_t sec;
	GetCharSetData(Index, &Mon);
	GetCharSetData(Index+1, &Day);
	GetCharSetData(Index+2, &Hour);

	for(i = 0; i < (Mon-1); i++)
	{
		NumDay += _ytab[0][i];
	}
	sec = (NumDay + Day - 1) * DAY_SECONDS + Hour * HOUR_SECONDS + GMT_TIME_MIN+31622400;
	return sec;
}

time_t mktime_r(struct tm *tmbuf)
{
	long day, year;
	int tm_year;
	int yday, month;
	long seconds;
	int overflow;
	long dst;

	tmbuf->tm_min += tmbuf->tm_sec / 60;
	tmbuf->tm_sec %= 60;
	if (tmbuf->tm_sec < 0)
	{
		tmbuf->tm_sec += 60;
		tmbuf->tm_min--;
	}
	tmbuf->tm_hour += tmbuf->tm_min / 60;
	tmbuf->tm_min = tmbuf->tm_min % 60;
	if (tmbuf->tm_min < 0)
	{
		tmbuf->tm_min += 60;
		tmbuf->tm_hour--;
	}
	day = tmbuf->tm_hour / 24;
	tmbuf->tm_hour= tmbuf->tm_hour % 24;
	if (tmbuf->tm_hour < 0)
	{
		tmbuf->tm_hour += 24;
		day--;
	}
	tmbuf->tm_year += tmbuf->tm_mon / 12;
	tmbuf->tm_mon %= 12;
	if (tmbuf->tm_mon < 0)
	{
		tmbuf->tm_mon += 12;
		tmbuf->tm_year--;
	}
	day += (tmbuf->tm_mday - 1);
	while (day < 0)
	{
		if(--tmbuf->tm_mon < 0)
		{
			tmbuf->tm_year--;
			tmbuf->tm_mon = 11;
		}
		day += _ytab[LEAPYEAR(YEAR0 + tmbuf->tm_year)][tmbuf->tm_mon];
	}
	while (day >= _ytab[LEAPYEAR(YEAR0 + tmbuf->tm_year)][tmbuf->tm_mon])
	{
		day -= _ytab[LEAPYEAR(YEAR0 + tmbuf->tm_year)][tmbuf->tm_mon];
		if (++(tmbuf->tm_mon) == 12)
		{
			tmbuf->tm_mon = 0;
			tmbuf->tm_year++;
		}
	}
	tmbuf->tm_mday = day + 1;
	year = EPOCH_YR;
	if (tmbuf->tm_year < year - YEAR0) return (time_t) -1;
		seconds = 0;
	day = 0;
	overflow = 0;

	tm_year = tmbuf->tm_year + YEAR0;

	if (TIME_MAX / 365 < tm_year - year) overflow++;
	day = (tm_year - year) * 365;
	if (TIME_MAX - day < (tm_year - year) / 4 + 1) overflow++;
	day += (tm_year - year) / 4 + ((tm_year % 4) && tm_year % 4 < year % 4);
	day -= (tm_year - year) / 100 + ((tm_year % 100) && tm_year % 100 < year % 100);
	day += (tm_year - year) / 400 + ((tm_year % 400) && tm_year % 400 < year % 400);

	yday = month = 0;
	while (month < tmbuf->tm_mon)
	{
		yday += _ytab[LEAPYEAR(tm_year)][month];
		month++;
	}
	yday += (tmbuf->tm_mday - 1);
	if (day + yday < 0) overflow++;
	day += yday;

	tmbuf->tm_yday = yday;
	tmbuf->tm_wday = (day + 4) % 7;               // Day 0 was thursday (4)
	seconds = ((tmbuf->tm_hour * 60L) + tmbuf->tm_min) * 60L + tmbuf->tm_sec;
	if ((TIME_MAX - seconds) / SECS_DAY < day) overflow++;
	seconds += day * SECS_DAY;

	// Now adjust according to timezone and daylight saving time
	if (((_timezone > 0) && (TIME_MAX - _timezone < seconds))
		|| ((_timezone < 0) && (seconds < -_timezone)))
		overflow++;
	seconds += _timezone;

	if (tmbuf->tm_isdst)
		dst = _dstbias;
	else
		dst = 0;

	if (dst > seconds) overflow++;        // dst is always non-negative
		seconds -= dst;

	if (overflow) return (time_t) -1;
	if ((time_t) seconds != seconds) return (time_t) -1;
	return (time_t) seconds;
}


void  floattoascii(float fin, unsigned char * pDataBuf)
{
  union
  {
	  float	ftemp;						          /*浮点数*/
	  unsigned long	Ltemp;
	  unsigned char	chtemp[4];					    /*字节型数*/
  }unTempVal;

  unTempVal.ftemp = fin;/*取要转换的浮点数*/
#ifdef _LITTLE_ENDIAN_TYPE
  *pDataBuf       = unTempVal.chtemp[0];/*转换浮点数尾数3,2,1,阶码*/
  *(pDataBuf + 1) = unTempVal.chtemp[1];
  *(pDataBuf + 2) = unTempVal.chtemp[2];
  *(pDataBuf + 3) = unTempVal.chtemp[3];
#else
  *pDataBuf       = unTempVal.chtemp[3];/*转换浮点数阶码,尾数1,2,3*/
  *(pDataBuf + 1) = unTempVal.chtemp[2];
  *(pDataBuf + 2) = unTempVal.chtemp[1];
  *(pDataBuf + 3) = unTempVal.chtemp[0];
#endif
}

void  FourByte2float(unsigned char * pDataBuf, float * pfout)
{
  union
  {
	  float			ftemp;						          /*浮点数*/
	  unsigned long	Ltemp;
	  unsigned char	chtemp[4];					    /*字节型数*/
  }unTempVal;

#ifdef _LITTLE_ENDIAN_TYPE
  unTempVal.chtemp[0] = *pDataBuf;/*转换浮点数尾数3,2,1,阶码*/
  unTempVal.chtemp[1] = *(pDataBuf + 1);
  unTempVal.chtemp[2] = *(pDataBuf + 2);
  unTempVal.chtemp[3] = *(pDataBuf + 3);
#else
  unTempVal.chtemp[3] = *pDataBuf;/*转换浮点数阶码,尾数1,2,3*/
  unTempVal.chtemp[2] = *(pDataBuf + 1);
  unTempVal.chtemp[1] = *(pDataBuf + 2);
  unTempVal.chtemp[0] = *(pDataBuf + 3);
#endif
  *pfout = unTempVal.ftemp;/*取要转换的浮点数*/
}

unsigned char	BCDToHex(unsigned char	Input)
{
	return	(Input & 0x0f) + (Input >> 4) * 10;
}


unsigned char	RTCWeekToHex (unsigned char Input)
{
  UCHAR out=0,i;
  for(i=0;i<6;i++)
  {
    out += ((Input>>i)&0x01)*(i+1);
  }
	return	out;
}


unsigned char	HexToRTCWeek(unsigned char	Input)
{
    UCHAR out=0,i;
    for(i=0;i<6;i++)
    {
        if(Input>(i+1))
        {
            out |=0x01<<i;
            Input = Input-i-1;
        }
        else
        {
            break;
        } 
    }
	return	out;
}

unsigned char	HexToBCD(unsigned char	Input)
{
	return	(Input % 10) + (Input / 10) * 16;
}


char *strncpyz(char *pDst, const char *pSrc, int nDstLen)
{
  char *p = pDst;

  if (p == NULL)
	{
    return NULL;
	}

	if (pSrc != NULL)
	{
		for( nDstLen -= 1; *pSrc && (nDstLen > 0); nDstLen -- )
		{
			*p ++ = *pSrc ++;
		}
	}

  *p = '\0';   /* append a string end flag '\0'    */

  return pDst;
}

float CharToFloat(volatile UINT32 data)
{
	/*浮点数和字节型数交换变量*/
	union
	{
		FLOAT32 f32Temp;/*浮点数*/
		UINT32 u32Temp;
	}unTempVal;
	unTempVal.u32Temp = data;
	return unTempVal.f32Temp;
}

unsigned char Hex2Ascii(unsigned char Input)
{
	if(Input > 9)
	{
		return Input + 0x37; /*A - F*/
	}
	else
	{
		return Input + 0x30; /*0 - 9*/
	}
}
unsigned char Ascii2Hex(unsigned char Input)
{
	if(Input > 0x39)
	{
		return Input - 0x37;					/*'A' - 'F'*/
	}
	else
	{
		return Input - 0x30;					/*'0' - '9'*/
	}
}
unsigned char TwoAscii2Hex(unsigned char InputH, unsigned char InputL)
{
	return	((Ascii2Hex(InputH)) << 4) + Ascii2Hex(InputL);
}



//这里没考虑 是不是第1个字节就是0x7e,后边可以优化！！
UINT16 CalcRcvCheckSum(UINT8 UartNo)
{
	UINT16 i = 0;
	UINT16 IICRC = 0;
	if (UART5_SEQUECE_NO == UartNo)
	{
		for (i = 0; i < (dv_Uart5.pData - 6) && (i < RS485_UART5_RX_MAX_NUM); i++)
		{
			IICRC += gUart5RecvBuff[YDN23_VER_PST + i];
		}
	}
	else if (UART4_SEQUECE_NO == UartNo)
	{
		for (i = 0; i < (dv_Uart4.pData - 6) && (i < RS485_UART4_RX_MAX_NUM); i++)
		{
			IICRC += gUart4RecvBuff[YDN23_VER_PST + i];
		}
	}
	else
	{
		return 0;
	}

	IICRC = IICRC%65536;
	return ((~IICRC)+1);
}




void * GetgEquipInfo(UINT8 u8Etype)
{
	switch (u8Etype)
	{
	case E_TYPE_SYS:
		return &gEvChargeInfo;
		break;
	
	default:
		if(gSelfTest.StopFeedDogReason == 0)
		{
			gSelfTest.u32DogBit_Bak = u8Etype;
			gSelfTest.StopFeedDogReason = 1;
			gs_E2PROM.fSetHistoryEventUpdate();
		}
		Trap1();
                break;
	}
		
	dv_Output.fOpenBeep();
	OSTimeDly(700);
	dv_Output.fCloseBeep();
	OSTimeDly(700);
	dv_Output.fOpenBeep();
	OSTimeDly(700);
	dv_Output.fCloseBeep();
	OSTimeDly(700);
	dv_Output.fOpenBeep();
	OSTimeDly(700);
	dv_Output.fCloseBeep();
	OSTimeDly(700);
	dv_Output.fOpenBeep();
	OSTimeDly(700);
	dv_Output.fCloseBeep();
	OSTimeDly(700);
	Trap1();
	return NULL;
}










/*==========================================================================*
* 函数名称 : RecordChargeRecord
* 函数功能 : 写充电记录
* 入口参数 :  enum RECORD_CHG_EVENT_DEF
* 出口参数 : 
* 说明     :                  
* 作者     :          
* 日期     : 
* 修改记录 :
*==========================================================================*/

ChargeRecordMng_TypeDef gs_ChargeRecordMng[2] = {0};


void RecordChargeRecord(unsigned char GunNo, unsigned char j)
{
	if(GunNo >1)
	{
		return;
	}

	gs_ChargeRecordMng[GunNo].ChargeRecord[gs_ChargeRecordMng[GunNo].CurrChargeRecord].u16EndReason = j;
	gs_ChargeRecordMng[GunNo].ChargeRecord[gs_ChargeRecordMng[GunNo].CurrChargeRecord].u16ChgDuration = 0;
	gs_ChargeRecordMng[GunNo].ChargeRecord[gs_ChargeRecordMng[GunNo].CurrChargeRecord].u16ChgEnegy = 0;

	gs_ChargeRecordMng[GunNo].CurrChargeRecord++;
	if(gs_ChargeRecordMng[GunNo].CurrChargeRecord >= CHARGE_RECORD_NUM)
	{
		gs_ChargeRecordMng[GunNo].CurrChargeRecord = 0;     
		gs_ChargeRecordMng[GunNo].ChargeRecordFull = 1;      
	}     

	if(gs_ChargeRecordMng[GunNo].ChargeRecordFull == 1)
		gs_ChargeRecordMng[GunNo].ucChargeRecordNum = CHARGE_RECORD_NUM;
	else
		gs_ChargeRecordMng[GunNo].ucChargeRecordNum = gs_ChargeRecordMng[GunNo].CurrChargeRecord;

	if(gs_ChargeRecordMng[GunNo].WriteChargeRecordFlag < 255)
		gs_ChargeRecordMng[GunNo].WriteChargeRecordFlag++;
}

/*==========================================================================*
* 函数名称 : init_ChargeRecordMng
* 函数功能 : 初始化
* 入口参数 : 
* 出口参数 : 
* 说明     :                  
* 作者     :          
* 日期     : 
* 修改记录 :
*==========================================================================*/
void init_ChargeRecordMng(unsigned char GunNo)
{
	if(GunNo >1)
	{
		return;
	}

	memset((unsigned char *)&gs_ChargeRecordMng[GunNo], 0, sizeof(ChargeRecordMng_TypeDef));
}

/*==========================================================================*
* 函数名称 : GetChargeRecordIndex
* 函数功能 : 获取充电记录索引
* 入口参数 : 
* 出口参数 : 
* 说明     :                  
* 作者     :          
* 日期     : 
* 修改记录 :
*==========================================================================*/
unsigned char GetChargeRecordIndex(unsigned char GunNo, unsigned char nID)
{
	unsigned char  idx;

	if(GunNo >1)
	{
		return 0;
	}

	if (nID > CHARGE_RECORD_NUM - 1)
	{    
		return  0;
	}
	else if (gs_ChargeRecordMng[GunNo].ucChargeRecordNum == 0)
	{    
		return  0;
	}
	else
	{
		if(gs_ChargeRecordMng[GunNo].CurrChargeRecord == 0)
		{
			idx = CHARGE_RECORD_NUM - 1;
		}
		else
		{
			idx = gs_ChargeRecordMng[GunNo].CurrChargeRecord - 1;
		}
		while(nID > 0)
		{
			if (idx > 0)
			{
				idx--;
			}
			else
			{
				idx = CHARGE_RECORD_NUM - 1;
			}
			nID--;
		}
		return  idx;
	}
}

void SetIMEUDOState(UCHAR ucIMEUNo,UCHAR ucDoNo,UCHAR ucState)
{
	if(gIMEUData[ucIMEUNo].ucDoCtrlState[ucDoNo]!=ucState)
	{
		gIMEUData[ucIMEUNo].ucDoCtrlState[ucDoNo] = ucState;
		gIMEUData[ucIMEUNo].ucDoCtrlFlag[ucDoNo] = TRUE;
	}

}
float GetBusData(UCHAR ucGunNo, UCHAR ucDataID)
{
	UCHAR uctemp;
	float ftemp = 0;
	UCHAR ucOtherGunNo;
	if(ucGunNo>=ALL_GUN_NUM)
	{
		return ftemp;
	}
	if(ucGunNo == GUN_NO_A)
	{
		ucOtherGunNo = GUN_NO_B;
	}
	else
	{
		ucOtherGunNo = GUN_NO_A;
	}
	GetCharSetData(SETDATA_ucLoadDataSource_NUM, &uctemp);
	if(uctemp == FROM_RECT)
	{
		if(ucDataID == BUS_VOLT)
		{
			ftemp = gs_CANComm.GrpData[ucGunNo].fOutVolt;
		}
		else if(ucDataID == BUS_CURR)
		{
			ftemp = gs_CANComm.GrpData[ucGunNo].fOutCurr;
			if(gChargeCtrl.ucParallelMode[ucGunNo] == Parallel_Mode)
			{
				ftemp += gs_CANComm.GrpData[ucOtherGunNo].fOutCurr; 
			}
		}
		
	}
	else
	{
		if(ucDataID == BUS_VOLT)
		{
			ftemp = gIMIUData[ucGunNo].fBusVolt;
		}
		else if(ucDataID == BUS_CURR)
		{
			ftemp = gIMIUData[ucGunNo].fCurr;
		}
	}
	return ftemp;
}

UCHAR u8Is1000VHardwareA00(void)
{
	return TRUE;
	if(HARDWARE_IS_1000V == gEvChargeInfo.u8Is_1000V_Hardware)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}