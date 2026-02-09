
#ifndef __CAN_BMS_H
#define __CAN_BMS_H

#include "main.h"

#define INT_STATE_MACHINE(_P, _MachineId, _fnProc) ((_P)->MachineIdx = (_MachineId), (_P)->fproc = (_fnProc))

#define BMS_VERSION_IS_GBT2011	0
#define BMS_VERSION_IS_GBT2015	1

/*************************************************************************************************
		BMS 相关联的宏定义！
***************************************************************************************************/
//状态机！
#define TEST_HANDSHAKE_CHECK_CHM	0
#define TEST_HANDSHAKE_CHECK_CRM	1
#define TEST_CONFIG_CHECK_CML_CTS	2
#define TEST_CONFIG_CHECK_CRO_AA	3
#define TEST_CHARGE_CHECK_CCS		4
#define TEST_STOP_CHECK_CST			5
#define TEST_STOP_CHECK_CSD			6
#define TEST_TIMEOUT_BEM			7
#define MACHINE_ID_MAX_NUM			8

#define XXX_DM3  0x22
#define XXX_DM1  0x20
#define XXX_CHM  0x26  
#define XXX_BHM  0x27
#define XXX_CRM  0x01
#define XXX_BRM  0x02
#define XXX_EC   0xEC
#define XXX_BCP  0x06
#define XXX_CTS  0x07
#define XXX_CML  0x08
#define XXX_BR0  0x09
#define XXX_CR0  0x0A
#define XXX_CCS  0x12
#define XXX_BCL  0x10
#define XXX_BCS  0x11
#define XXX_CST  0x1a
#define XXX_BST  0x19
#define XXX_BSD  0x1C
#define XXX_CSD  0x1D
#define XXX_CEM  0x1F
#define XXX_BSM  0x13
#define XXX_BMT  0x15   
#define XXX_BMV  0x16
#define XXX_BSP  0x17
#define XXX_BEM  0x1E
#define XXX_EB	 0xEB
#define BMS_ADDR 0xf4
#define EV_ADDR  0x56

#define TWO_GUN_IDENTIFY 0x55

#define PRIORITY_2	2
#define PRIORITY_4	4
#define PRIORITY_6	6
#define PRIORITY_7	7
#define PRIORITY_0	0

#define CAN_GET_CMD_COUNT_STOP 0xFFFF
#define CAN_GET_CMD_5S_COUNT 501
#define CAN_GET_CMD_1S_COUNT 101
#define CAN_GET_CMD_200MS_COUNT 21

#define CAN_DATA_STOP			0 //未启动
#define CAN_DATA_SEND			1 //发送
#define CAN_DATA_WAIT			2 //周期等待
#define CAN_DATA_ACK			3 //周期等待

#define CAN_DATA_FRAME1			5 //发送
#define CAN_DATA_FRAME2			6 //发送
#define CAN_DATA_FRAME3			7 //发送
#define CAN_DATA_FRAME4			8 //发送
#define CAN_DATA_FRAME5			9 //发送
#define CAN_DATA_FRAME6			10 //发送
#define CAN_DATA_FRAME7			11 //发送
#define CAN_DATA_END			12 //发送

#define NOT_AGING_STATE 	0
#define START_AGING_STATE 	1
#define AGING_STATE 		2
#define AGING_OK_STATE 		3
#define AGING_FAIL_STATE 	4

#define _WAIT_SEND_CMD(GunNo,CMDID) do {\
	gBmsCommDataInfo[GunNo].u8Send_Flag[CMDID] = CAN_DATA_WAIT;\
} while (0);
#define _START_SEND_CMD(GunNo,CMDID) do {\
	gBmsCommDataInfo[GunNo].u8Send_Flag[CMDID] = CAN_DATA_SEND;\
} while (0);
#define _STOP_SEND_CMD(GunNo,CMDID) do {\
	gBmsCommDataInfo[GunNo].u8Send_Flag[CMDID]  = CAN_DATA_STOP;\
} while (0);
#define _START_SEND_DATA(GunNo,CMDID,STEP) do {\
	gBmsCommDataInfo[GunNo].u8Send_Flag[CMDID] = STEP;\
} while (0);

#define _CLEAR_SEND_DATA(GunNo,CMDID) do {\
	gBmsCommDataInfo[GunNo].u8Send_Flag[CMDID] = CAN_DATA_STOP;\
} while (0);


struct _MachineInfo
{
	int MachineIdx;
	ON_STATE_MACH_PROC fproc;
};
enum
{
	SEND_CMD_BHM_STATE = 0,
	SEND_CMD_RST_STATE ,
	SEND_CMD_BRO_STATE,
	SEND_CMD_BCL_STATE,
	SEND_CMD_BSM_STATE,
	SEND_CMD_BST_STATE,
	SEND_CMD_BSD_STATE,
	SEND_CMD_BEM_STATE,
	SEND_CMD_100_STATE,
	SEND_CMD_101_STATE,
	SEND_CMD_102_STATE,
	SEND_CMD_110_STATE,
	SEND_CMD_TWO_GUN,
	ALL_SEND_CMD_STATE

};
enum
{
	GET_CMD_CHM_COUNT_ID = 0,
	GET_CMD_CRM_00_COUNT_ID ,
	GET_CMD_CRM_AA_COUNT_ID ,
	GET_CMD_CML_COUNT_ID,
	GET_CMD_CTS_COUNT_ID,
	GET_CMD_CRO_COUNT_ID,
	
	GET_CMD_CCS_COUNT_ID,
	GET_CMD_CST_COUNT_ID,
	GET_CMD_CSD_COUNT_ID,

	GET_CMD_EC_CTS_COUNT_ID,

	GET_CMD_108_COUNT_ID,
	GET_CMD_109_COUNT_ID,
	ALL_GET_CMD_COUNT_ID

};
struct __stBattFault_Flag
{
	UINT8 bitBattOverVoltage : 1;
	UINT8 bitBattUnderVoltage : 1;
	UINT8 bitCurrDeviationError : 1;
	UINT8 bitHighBattTemp : 1;
	UINT8 bitVoltDeviationError : 1;
	UINT8 bitReserve : 3;
};
typedef struct __stBattFault_Flag  _BitBattFaultFlag;

union __BattFault_Flag
{
	UINT8 u8Fault_flag;
	_BitBattFaultFlag bFaultFlag;
};
typedef union __BattFault_Flag   __unBattFault_FLAG;

struct __stVehicleStat_Flag
{
	UINT8 bit_0VehicleChargingEnable : 1;
	UINT8 bit_1VehicleShiftPost : 1;
	UINT8 bit_2ChargingSysFault : 1;
	UINT8 bit_3VehicleStatus : 1;						
	UINT8 bit_4NormalStopRequest : 1;					
	UINT8 bitReserve : 2;
	UINT8 bit_7VehicleDisChargingFlag : 1;
};
typedef struct __stVehicleStat_Flag  _BitVehicleFlag;

union __VehicleStat_Flag
{
	UINT8 u8VehicleSTAT_flag;
	_BitVehicleFlag bSTATFlag;
};
typedef union __VehicleStat_Flag   __unVehicleSTAT_FLAG;

struct __stEVFault_Flag
{
	UINT8 bit_0ChargeStatus : 1;
	UINT8 bit_1ChargeMalfunction : 1;
	UINT8 bit_2ChargingConnectorLock : 1;
	UINT8 bit_3BattIncompatibility : 1;
	UINT8 bit_4ChargeSysMalfunction : 1;
	UINT8 bit_5ChargeStopControl : 1;
	UINT8 bitReserve : 3;
};
typedef struct __stEVFault_Flag  _BitEVFaultFlag;

union __EVFault_Flag
{
	UINT8 u8EVFault_flag;
	_BitEVFaultFlag bFaultFlag;
};
typedef union __EVFault_Flag   __unEVFALT_FLAG;

struct __BMS_CHADEMO_COM_DATA_INFO
{
	UCHAR ucCMD_100_MinCurr;
	UINT16 u16CMD_100_MaxBattVolt;
	UCHAR ucCMD_100_ChargedRate;
	UCHAR ucCMD_101_MaxChargeTimeSec;
	UCHAR ucCMD_101_MaxChargeTimeMin;
	UCHAR ucCMD_101_EstimatedTime;
	float fCMD_101_BattaryCapacity;

	UCHAR  ucCMD_102_ProtocolNum;
	UINT16 u16CMD_102_TargetBatteryVoltage;						
	UCHAR  ucCMD_102_RequireCurrent;
	__unBattFault_FLAG unCMD_102_Fault_Flag;
	__unVehicleSTAT_FLAG unCMD_102_Stat_Flag;
	UCHAR ucCMD_102_ChargeRate;

	__unEVFALT_FLAG unCMD_109_Fault_Flag;
};
typedef struct __BMS_CHADEMO_COM_DATA_INFO	stBMS_CHADEMO_Comm_Data_Info;

struct __BMS_COM_DATA_INFO
{
	UINT8 u8GBT_BMS_ProtocolType;
	UINT8 ucMachineState;
	UINT8 ucCHAdeMOMachineState;
	UINT8 u8Send_Flag[ALL_SEND_CMD_STATE];
	UINT16 u16MultiFrameInterval;
	canmsg CMDMsg[ALL_SEND_CMD_STATE];
	canmsg DataMsg[8];
	UINT16 u16GetCmdCount[ALL_GET_CMD_COUNT_ID];
	UCHAR  ucGetCmdTimeOutFlag;
	UCHAR  ucACSwitchCtrlState;

	UINT8 u8GetCRMAAFlag;
	UINT8 u8GetCMLFlag;
	UINT8 u8GetCTSFlag;
	UINT8 u8OpenRectStep;
	UINT8 u8GetCSTFlag;
	UINT8 u8GetCSDFlag;
	UINT8 u8SendBSTFlag;
	UINT8 u8SendBEMFlag;

	float fBCP_MaxPermitCellVolt;
	float fBCP_MaxPermitCurr;
	float fBCP_MaxVolt;
	float fBCP_MaxPermitTemp;
	float fBCP_BattStartSOC;
	float fBCP_BattStartVolt;

	float fBCS_MaxCellVolt;
	float fBSM_MaxCellTemp;	

	float fBattSOCIncrease;


	stBMS_CHADEMO_Comm_Data_Info CHAdeMODataInfo;
	UCHAR CHAdeMOChargeEnableBitDelay;
	UCHAR CHAdeMORectModeChageDelay;
	

};
typedef struct __BMS_COM_DATA_INFO	stBMS_Comm_Data_Info;

typedef struct _MachineInfo MACHINE_INFO;


extern NOINIT stBMS_Comm_Data_Info gBmsCommDataInfo[ALL_GUN_NUM];
extern NOINIT CanRxMsgTypeDef CanRxMsgBuff[ALL_GUN_NUM];
extern NOINIT MACHINE_INFO BMSMachineIfo[ALL_GUN_NUM][MACHINE_ID_MAX_NUM];
#endif
