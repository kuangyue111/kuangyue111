#include "main.h"
#include "EtherNet.h"
#include "CAN_BMS.h"
#include "Measure.h"
#include "Uart4and5.h"
#include "Uart6.h"
#include "CAN_2_Common.h"
#define ABS(a)	(((a) < 0) ? (-(a)) : (a))

NOINIT MACHINE_INFO CHAdeMO_BMSMachineIfo[CHAdeMO_MACHINE_ID_MAX_NUM];
UINT8 GUN_B_Send_Standard_Frame(UINT8 u8CMDID,UINT8 u8Len)
{
	if (gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[u8CMDID] == CAN_DATA_SEND)
	{
		BSP_Can_2_Tx(&gBmsCommDataInfo[GUN_NO_B].CMDMsg[u8CMDID], u8Len,CAN_ID_STD);
		gChargeCtrl.u16BMSCMDSend[GUN_NO_B] =(UINT16)(gBmsCommDataInfo[GUN_NO_B].CMDMsg[u8CMDID].Id&0x00000FFF);
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[u8CMDID] = CAN_DATA_WAIT;
		if(u8CMDID == SEND_CMD_BST_STATE)
		{
			gBmsCommDataInfo[GUN_NO_B].u8SendBSTFlag = TRUE;
		}
		if((u8CMDID == SEND_CMD_BEM_STATE)&&(gBmsCommDataInfo[GUN_NO_B].u8SendBEMFlag<255))
		{
			gBmsCommDataInfo[GUN_NO_B].u8SendBEMFlag++;
		}
		return TRUE;
	}
	return FALSE;
}

UCHAR CAN2_Read_StandFrame(UINT16 u16ReadPF)
{
	UCHAR ucRTN = FALSE,iIdx = 0;
	for (iIdx = 0;iIdx < CAN_A_B_BUF_MAX_NUM; iIdx++)
	{
		if (u16ReadPF == 0xFFFF)
		{
			gCan2RxMsgBuf[iIdx].iFlag = FALSE;
			gCan2RxMsgBuf[iIdx].RxMsg.StdId = 0;
			continue;
		}

		if(u16ReadPF == (0x1FF&gCan2RxMsgBuf[iIdx].RxMsg.StdId))
		{
			ucRTN = TRUE;
			CanRxMsgBuff[GUN_NO_B] = gCan2RxMsgBuf[iIdx].RxMsg;
			gCan2RxMsgBuf[iIdx].iFlag = FALSE;
			gCan2RxMsgBuf[iIdx].RxMsg.StdId = 0;
			gChargeCtrl.u16EVCMDGet[GUN_NO_B] =u16ReadPF; 
			break;
		}
	}
	return ucRTN;
}
static void CAN2_Refresh_CHAdeMO_CMD_100(void)
{

	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_100_STATE].Id = 0x100;
	data.ucdata[0]= gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_100_MinCurr;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_100_STATE].Data[0] = ((UINT32)data.ucdata[0] << 24) ;

	data.u16data[0] = gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.u16CMD_100_MaxBattVolt;
	data.ucdata[2] = gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_100_ChargedRate;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_100_STATE].Data[1] = ((UINT32)data.ucdata[0] << 24) + ((UINT32)data.ucdata[1] << 16)+((UINT32)data.ucdata[2] << 8) ;
}
static void CAN2_Refresh_CHAdeMO_CMD_101(void)
{

	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_101_STATE].Id = 0x101;
	data.ucdata[3]=0;
	data.ucdata[2]= (UCHAR)gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_101_MaxChargeTimeSec;
	data.ucdata[1]= (UCHAR)gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_101_MaxChargeTimeMin;
	data.ucdata[0]= (UCHAR)gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_101_EstimatedTime;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_101_STATE].Data[0] = data.u32data ;

	data.u16data[0] = (UINT16)(gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.fCMD_101_BattaryCapacity*10);
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_101_STATE].Data[1] = ((UINT32)data.ucdata[0] << 16) + ((UINT32)data.ucdata[1] << 8);
}

static void CAN2_Refresh_CHAdeMO_CMD_102(void)
{
	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;
	union
	{
		UCHAR uctemp[2];
		UINT16 u16temp;
	}temp;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_102_STATE].Id = 0x102;

	data.ucdata[3]= gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_102_ProtocolNum;
	temp.u16temp  = (UINT16)gChargeCtrl.fUserSetVolt[GUN_NO_B];//gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.u16CMD_102_TargetBatteryVoltage;
	data.ucdata[2]= temp.uctemp[0];
	data.ucdata[1]= temp.uctemp[1];
	if(gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState!=STATE_STOP_CHARGING)
		data.ucdata[0]= (UCHAR)gChargeCtrl.fUserSetLoadCurr[GUN_NO_B];//gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_102_RequireCurrent;
	else
		data.ucdata[0]= 0;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_102_STATE].Data[0] = data.u32data ;

	data.ucdata[3]= gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Fault_Flag.u8Fault_flag;
	data.ucdata[2]= gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Stat_Flag.u8VehicleSTAT_flag;
	data.ucdata[1]= gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_102_ChargeRate;
	data.ucdata[0]= 0;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_102_STATE].Data[1] = data.u32data;
}

static void CAN2_Refresh_CHAdeMO_CMD_110(void)
{
	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_110_STATE].Id = 0x110;

	data.u32data= 0x01000000;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_110_STATE].Data[0] = data.u32data ;
	data.u32data= 0x00000000;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_110_STATE].Data[1] = data.u32data;
}
static void UnpackCMD108Frame(void)
{
	UINT16 u16temp;
	u16temp = CanRxMsgBuff[GUN_NO_B].Data[1];
	u16temp += CanRxMsgBuff[GUN_NO_B].Data[2]*256;
	gChargeCtrl.fEVMaxVolt[GUN_NO_B] = (float)u16temp;
	u16temp = CanRxMsgBuff[GUN_NO_B].Data[3];
	gChargeCtrl.fEVMaxCurr[GUN_NO_B]= (float)u16temp;
}
static void UnpackCMD109Frame(void)
{
	UINT16 u16temp;
	u16temp = CanRxMsgBuff[GUN_NO_B].Data[1];
	u16temp += CanRxMsgBuff[GUN_NO_B].Data[2]*256;
	gChargeCtrl.fEVOutVolt[GUN_NO_B] = (float)u16temp;
	u16temp = CanRxMsgBuff[GUN_NO_B].Data[3];
	gChargeCtrl.fEVOutCurr[GUN_NO_B]= (float)u16temp;

	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_109_Fault_Flag.u8EVFault_flag  = CanRxMsgBuff[GUN_NO_B].Data[5];
}
static void CAN2_CHAdeMO_ProtocolProcess(void)
{
	UCHAR ucRTN;
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_100_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_100_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_100_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_100_STATE] == CAN_DATA_SEND))
	{
		CAN2_Refresh_CHAdeMO_CMD_100();
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_100_STATE);
	}
	//CMD101 SendProcess
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_101_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_101_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_101_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_101_STATE] == CAN_DATA_SEND))
	{
		CAN2_Refresh_CHAdeMO_CMD_101();
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_101_STATE);
	}
	//CMD102 SendProcess
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_102_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_102_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_102_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_102_STATE] == CAN_DATA_SEND))
	{
		CAN2_Refresh_CHAdeMO_CMD_102();
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_102_STATE);
	}
	//CMD102 SendProcess
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_110_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_110_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_110_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_110_STATE] == CAN_DATA_SEND))
	{
		CAN2_Refresh_CHAdeMO_CMD_110();
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_110_STATE);
	}
	ucRTN = CAN2_Read_StandFrame(0x108);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_108_COUNT_ID] = CAN_GET_CMD_1S_COUNT;  
		UnpackCMD108Frame();
	}
	ucRTN = CAN2_Read_StandFrame(0x109);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_109_COUNT_ID] = CAN_GET_CMD_1S_COUNT;  
		UnpackCMD109Frame();
	}
	 CAN2_Read_StandFrame(0x118);
}
static UCHAR CAN2_CHAdeMO_Init_Data_Before_Charge(void)
{

	if(gChargeCtrl.ucCtrlCmdState[GUN_NO_B]== START_CHARGE_VAL)
	{
		if(gMeasureInfo.fGetDIState(DI_7_ID) == TRUE)
		{
			CAN2_CHAdeMO_ProtocolProcess();//日标协议收发处理
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_109_Fault_Flag.bFaultFlag.bit_3BattIncompatibility == 0 )
			{
				__CHADEMO_CP3_ON();
                if(gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay ==0)
                {
                    gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay = 1;
                }
			}
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay == 3)
			{
                gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay = 0;
				gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Stat_Flag.bSTATFlag.bit_0VehicleChargingEnable = 1;
				gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay = 1;
				return STATE_Wait_SelfTest; 
			}
		}
        else
        {
            gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay =0;
            gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay = 0;
            gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
			if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] !=  BUTTOM_INSUL_START)
			{
				__B_CHAdeMO_MCCB_OFF();
			}
			
			gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_100_STATE]  = CAN_DATA_STOP;
			gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_101_STATE]  = CAN_DATA_STOP;
			gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_102_STATE]  = CAN_DATA_STOP;
			gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_110_STATE]  = CAN_DATA_STOP;
        }
	}
	else
	{
		gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay =0;
		gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay = 0;
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
		if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] !=  BUTTOM_INSUL_START)
		{
			__B_CHAdeMO_MCCB_OFF();
		}
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_100_STATE]  = CAN_DATA_STOP;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_101_STATE]  = CAN_DATA_STOP;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_102_STATE]  = CAN_DATA_STOP;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_110_STATE]  = CAN_DATA_STOP;
	}
	return STATE_INIT_DATA_BEFORE_CHARGE;
}
static UCHAR CAN2_CHAdeMO_Wait_Selftest(void)
{
  float ftemp;
	if(gMeasureInfo.fGetDIState(DI_7_ID) == TRUE)
	{
		CAN2_CHAdeMO_ProtocolProcess();//日标协议收发处理
		if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] !=  BUTTOM_INSUL_START)
		{
			ftemp=gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartVolt;
			gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = ftemp;
			if((ftemp<310)||(ftemp>750))
			{
				ftemp = 310;
			}
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay <2)
			{
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],10);
			}
			else
			{
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],10);
			}
		}
		if(gMeasureInfo.fGetDIState(DI_8_ID) == TRUE)
		{
			return STATE_CHARGING;
		}
		return STATE_Wait_SelfTest;	
	}
	else
	{
		return STATE_INIT_DATA_BEFORE_CHARGE;
	}
}
static UCHAR CAN2_CHAdeMO_Charging(void)
{
	UCHAR ucRectOutputMode;
	static UCHAR ucRectOutputModeBack = GRP_WORKMODE_INVERTER;
	if(gChargeCtrl.ucRectWorkMode[GUN_NO_B] == 0)
	{
		ucRectOutputMode = GRP_WORKMODE_INVERTER;
	}
	else
	{
		ucRectOutputMode = GRP_WORKMODE_RECTIFIER;
	}
	if(ucRectOutputModeBack != ucRectOutputMode)
	{
		ucRectOutputModeBack = ucRectOutputMode;
		if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay >= 25)
		{
			gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay  = 21;
		}
	}

	if((gMeasureInfo.fGetDIState(DI_7_ID) == TRUE)&&(gMeasureInfo.fGetDIState(DI_8_ID) == TRUE))
	{
		CAN2_CHAdeMO_ProtocolProcess();//日标协议收发处理
		__B_CHAdeMO_MCCB_ON();
		 if((gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay>20)&&(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay<25))
		{
			gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
		}
		else if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay<30)
		{
			if(ucRectOutputMode == GRP_WORKMODE_INVERTER)
			{
				gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,gChargeCtrl.fBMSRequireVolt[GUN_NO_B]-30,10);
			}
			else
			{
				gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],gChargeCtrl.fUserSetLoadCurr[GUN_NO_B]);
			}
		}
		else if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay< 35)
		{
			if(gChargeCtrl.fUserSetLoadCurr[GUN_NO_B] >0)
			{
				if(ucRectOutputMode == GRP_WORKMODE_INVERTER)
				{
					gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
					gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B]-30,10);
				}
				else
				{
					gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
					gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],gChargeCtrl.fUserSetLoadCurr[GUN_NO_B]);
				}
				gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay = 35;
			}
		}
		else 
		{

			if(ucRectOutputMode == GRP_WORKMODE_INVERTER)
			{
				gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B]-30,gChargeCtrl.fUserSetLoadCurr[GUN_NO_B]);
			}
			else
			{
				gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],gChargeCtrl.fUserSetLoadCurr[GUN_NO_B]);
			}
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_109_Fault_Flag.bFaultFlag.bit_5ChargeStopControl == 1)
			{
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
				gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay = 0;
				return STATE_STOP_CHARGING;
			}
		}
		if(gChargeCtrl.ucCtrlCmdState[GUN_NO_B]== STOP_CHARGE_VAL)
		{
			gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Stat_Flag.bSTATFlag.bit_0VehicleChargingEnable = 0;
			__CHADEMO_CP3_OFF();
			return STATE_STOP_CHARGING;
		}
		
		return STATE_CHARGING;	
	}
	else
	{
		gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay = 0;
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
		return STATE_STOP_CHARGING;
	}
}
static UCHAR CAN2_CHAdeMO_Stop_Charging(void)
{
	
	__B_CHAdeMO_MCCB_OFF();
	__CHADEMO_CP3_OFF();
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Stat_Flag.bSTATFlag.bit_0VehicleChargingEnable = 0;
	CAN2_CHAdeMO_ProtocolProcess();//日标协议收发处理
	gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
	if(gMeasureInfo.fGetDIState(DI_7_ID) == FALSE)
	{
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_100_STATE]  = CAN_DATA_STOP;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_101_STATE]  = CAN_DATA_STOP;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_102_STATE]  = CAN_DATA_STOP;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_110_STATE]  = CAN_DATA_STOP;
		return STATE_INIT_DATA_BEFORE_CHARGE;
	}
	else
		return STATE_STOP_CHARGING;
}
void InitCHAdeMO_BMS_Machine(void)
{
	gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState = TEST_HANDSHAKE_CHECK_CHM;

	INT_STATE_MACHINE(&CHAdeMO_BMSMachineIfo[0],
		STATE_INIT_DATA_BEFORE_CHARGE, 
		CAN2_CHAdeMO_Init_Data_Before_Charge);	
	INT_STATE_MACHINE(&CHAdeMO_BMSMachineIfo[1],
		STATE_Wait_SelfTest, 
		CAN2_CHAdeMO_Wait_Selftest);
	INT_STATE_MACHINE(&CHAdeMO_BMSMachineIfo[2],
		STATE_CHARGING, 
		CAN2_CHAdeMO_Charging);
	INT_STATE_MACHINE(&CHAdeMO_BMSMachineIfo[3],
		STATE_STOP_CHARGING, 
		CAN2_CHAdeMO_Stop_Charging);
}
void InitCHAdeMOCMDdata(void)
{
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_100_MinCurr = 0;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.u16CMD_100_MaxBattVolt = (UINT16)gBmsCommDataInfo[GUN_NO_B].fBCP_MaxVolt;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_100_ChargedRate = 100;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_101_MaxChargeTimeSec =0xFF;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_101_MaxChargeTimeMin =225;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_101_EstimatedTime =100;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.fCMD_101_BattaryCapacity = 300;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_102_ProtocolNum = 1;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.u16CMD_102_TargetBatteryVoltage = 500;						
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_102_RequireCurrent = 10;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Fault_Flag.u8Fault_flag = 0;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.unCMD_102_Stat_Flag.u8VehicleSTAT_flag = 0x80;
	gBmsCommDataInfo[GUN_NO_B].CHAdeMODataInfo.ucCMD_102_ChargeRate =20;
}
static UCHAR GUN_B_CHAdeMO_InsulationCheckStart(UCHAR ucDelay)
{
	float fVolt;
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] ==  BUTTOM_INSUL_START)//开始测试
	{
		if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)!=0x01)
		{
			gIMIUData[GUN_NO_B].ucPutResistance = 0x01;//投入电阻
			__B_CHAdeMO_MCCB_ON();
		}
		fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
		if(fVolt > 100)
		{
			gChargeCtrl.ucInsulationVoltCheckFlag[GUN_NO_B] = TRUE;
		}
		if(gChargeCtrl.u16PutResOnBusCount[GUN_NO_B] >0)
		{
			GUN_B_TestRecordStartProcess();
		}
		if((gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]>3)&&((gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]<ucDelay)))
		{
			if(gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState ==STATE_CHARGING)
			{
				if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)==0x01)
				{
					gIMIUData[GUN_NO_B].ucPutResistance = 0x00;//去掉电阻	
				}
				__B_CHAdeMO_MCCB_OFF();
				gChargeCtrl.u16InsulState[GUN_NO_B] = 2;
				gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] = NO_BUTTOM_START;
				
				return FALSE;
			}

		}
		else if(gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]>=ucDelay)
		{
			if(gChargeCtrl.ucInsulationVoltCheckFlag[GUN_NO_B] == TRUE)
			{
				gChargeCtrl.u16InsulState[GUN_NO_B] = 1;
			}
			else
			{
				gChargeCtrl.u16InsulState[GUN_NO_B] = 2;
			}
			gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] = NO_BUTTOM_START;
			if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)==0x01)
			{
				gIMIUData[GUN_NO_B].ucPutResistance = 0x00;//去掉电阻	
			}
			__B_CHAdeMO_MCCB_OFF();
			return FALSE;
		}
	}
	else
	{
		gChargeCtrl.u16PutResOnBusCount[GUN_NO_B] =0;
		if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)==0x01)
		{
			gIMIUData[GUN_NO_B].ucPutResistance = 0x00;//去掉电阻	
		}
	}
	return TRUE;
}
static UCHAR GUN_B_CHAdeMO_Charge1Check(void)
{
	float fVolt,fCurr;
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] ==  BUTTOM_START_1_CHARGE)//开始测试
	{
		if(gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState == STATE_CHARGING)
		{
			gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGING_STATE ;    //充电状态
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay>=25)
			{
				GUN_B_TestRecordProcess();
			}

			if((gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay >25)&&(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay<30))
			{
				if(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]==0)
				{
					gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]=1;
				}
				if(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]<15)
				{
					gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay =25;
				}
				else
				{

					gChargeCtrl.fEVOutVoltBack[GUN_NO_B]   = gChargeCtrl.fEVOutVolt[GUN_NO_B];
					fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
					gChargeCtrl.fActualVoltBack[GUN_NO_B] = fVolt;
					gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay =30;
					gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B] = 0;
				}

			}
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay >=70)
			{
				if(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]==0)
				{
					gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]=1;
				}
				if(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]>15)
				{
					gChargeCtrl.fEVOutCurrBack[GUN_NO_B]   = gChargeCtrl.fEVOutCurr[GUN_NO_B];
					fCurr = GetBusData(GUN_NO_B,BUS_CURR);
					gChargeCtrl.fActualCurrBack[GUN_NO_B] = fCurr;
					gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGE_OK_STATE ;    //充电状态
					return FALSE;
				}
			}
			if(gChargeCtrl.u16StartCount[GUN_NO_B]>180)
			{
				gChargeCtrl.fEVOutVoltBack[GUN_NO_B]	= 0;
				gChargeCtrl.fActualVoltBack[GUN_NO_B]	= 0;
				gChargeCtrl.fEVOutCurrBack[GUN_NO_B]	= 0;
				gChargeCtrl.fActualCurrBack[GUN_NO_B]	= 0;
				gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGE_FAIL_STATE ;    //充电状态
				return FALSE;
			}
		}
		else
		{
			gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]=0;
			if(gChargeCtrl.u16StartCount[GUN_NO_B]>180)
			{
				gChargeCtrl.fEVOutVoltBack[GUN_NO_B]	= 0;
				gChargeCtrl.fActualVoltBack[GUN_NO_B]	= 0;
				gChargeCtrl.fEVOutCurrBack[GUN_NO_B]	= 0;
				gChargeCtrl.fActualCurrBack[GUN_NO_B]	= 0;
				gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGE_FAIL_STATE ;    //充电状态
				return FALSE;
			}
		}

	}
	return TRUE;
}
static UCHAR GUN_B_CHAdeMO_Charge2Check(void)
{
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] ==  BUTTOM_START_2_CHARGE)//开始测试
	{
		if(gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState == STATE_CHARGING)
		{
			gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGING_STATE ;    //充电状态
			if(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay >=70)
			{
				gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGE_OK_STATE ;    //充电状态
				if(gChargeCtrl.ucVoltCurrOKCount[GUN_NO_B]==0)
				{
					gChargeCtrl.ucVoltCurrOKCount[GUN_NO_B]=1;
				}
			}
		}
		else
		{

			if(gChargeCtrl.u16StartCount[GUN_NO_B]>180)
			{
				gChargeCtrl.fEVOutVoltBack[GUN_NO_B]	= 0;
				gChargeCtrl.fActualVoltBack[GUN_NO_B]	= 0;
				gChargeCtrl.fEVOutCurrBack[GUN_NO_B]	= 0;
				gChargeCtrl.fActualCurrBack[GUN_NO_B]	= 0;
				gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGE_FAIL_STATE ;    //充电状态
				return FALSE;
			}
		}
	}
	else
	{
		gChargeCtrl.ucVoltCurrOKCount[GUN_NO_B]=0;
	}
	return TRUE;
}
static UCHAR GUN_B_CHAdeMO_StartModeProcess(void)
{
	UCHAR uctemp;
	if(gChargeCtrl.ucUserSelectMode[GUN_NO_B] != NO_USER_MODE)
	{
		SetIMEUDOState(IMEU_1_ID,OUTSIDE_GUN2_GREEN_CTRL,TRUE);
		gBmsCommDataInfo[GUN_NO_B].ucACSwitchCtrlState = TRUE;
	}
	else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0)
	{
		SetIMEUDOState(IMEU_1_ID,OUTSIDE_GUN2_GREEN_CTRL,FALSE);
		gBmsCommDataInfo[GUN_NO_B].ucACSwitchCtrlState = FALSE;
	}
	if(gChargeCtrl.ucUserSelectMode[GUN_NO_B] == USER_TEST_MODE)//手动测试模式
	{
		uctemp = GUN_B_CHAdeMO_InsulationCheckStart(30);
		if(uctemp == FALSE)
		{
			return FALSE;
		}
		uctemp = GUN_B_CHAdeMO_Charge1Check();
		if(uctemp == FALSE)
		{
			return FALSE;
		}
		uctemp = GUN_B_CHAdeMO_Charge2Check();
		if(uctemp == FALSE)
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)==0x01)
		{
			gIMIUData[GUN_NO_B].ucPutResistance = 0x00;//去掉电阻	
		}
		gChargeCtrl.u16InsulState[GUN_NO_B] = 0;
	}
	if((gChargeCtrl.ucUserSelectMode[GUN_NO_B] == USER_BMS_MODE)
		&&(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] == BUTTOM_BMS_START))
	{
		return TRUE;
	}
	if(gChargeCtrl.ucUserSelectMode[GUN_NO_B] == USER_AGING_MODE)
	{
		uctemp = GUN_B_AgingModeProcess();
		return uctemp;	
	}	
	else
	{
		gChargeCtrl.ucAgingState[GUN_NO_B] =  NOT_AGING_STATE;
	}
	return FALSE;
}
void GUN_B_CHAdeMOProtocolTestProcess(void) 
{
	UCHAR ucRTN;
	UCHAR ucAuxPowerType;
	GetCharSetData(SETDATA_GUN2_ucAuxPowerType_NUM,&ucAuxPowerType);
	ucRTN = GUN_B_CHAdeMO_StartModeProcess();
	if((ucRTN==TRUE)&&(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0xFF))//非停止充电阶段，可以启动
	{
		gChargeCtrl.ucCtrlCmdState[GUN_NO_B]= START_CHARGE_VAL;
		if(ucAuxPowerType == mBMS_POWER_12V)
		{
			gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_12V_OK;
		}
		else if(ucAuxPowerType == mBMS_POWER_24V)
		{
			gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_24V_OK;
		}
		else
		{
			if(gChargeCtrl.u8BMSPowerStep[GUN_NO_B] == 0)
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_12V_OK;
			}
			if(gChargeCtrl.u8BMSPowerStep[GUN_NO_B] == 1)
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_24V_OK;
			}
		}
	}
	else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B] >0)
	{
		if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0xFF) 
		{
			gChargeCtrl.ucStopDelayCount[GUN_NO_B] = 5;
		}
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
		gChargeCtrl.ucCtrlCmdState[GUN_NO_B]= STOP_CHARGE_VAL;
	}
	else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0)
	{
		gChargeCtrl.ucStopDelayCount[GUN_NO_B]= 0xFF;
	}
	gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState = CHAdeMO_BMSMachineIfo[gBmsCommDataInfo[GUN_NO_B].ucCHAdeMOMachineState].fproc();
}
	
