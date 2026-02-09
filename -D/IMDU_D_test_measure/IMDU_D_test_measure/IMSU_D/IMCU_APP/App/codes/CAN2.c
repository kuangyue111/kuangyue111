#include "main.h"
#include "EtherNet.h"
#include "CAN_BMS.h"
#include "Measure.h"
#include "Uart4and5.h"
#include "Uart6.h"
#include "CAN_2_Common.h"

#define ABS(a)	(((a) < 0) ? (-(a)) : (a))
NOINIT stBMS_Comm_Data_Info gBmsCommDataInfo[ALL_GUN_NUM];
NOINIT CanRxMsgTypeDef CanRxMsgBuff[ALL_GUN_NUM];
NOINIT MACHINE_INFO BMSMachineIfo[ALL_GUN_NUM][MACHINE_ID_MAX_NUM];
UINT32 u32CAN_2_PerioCount[9] = {0,0,0,0,0,0,0,0,0};
static void RefreshCAN2Time(void)
{
	float fCurr;
	struct tm  tmNow;
	static UCHAR  Sec = 0,ucMin = 0;
	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)/*1秒钟*/
	{
		Sec = (UCHAR)tmNow.tm_sec;
		if((gChargeCtrl.ucStopDelayCount[GUN_NO_B]>0)&&(gChargeCtrl.ucStopDelayCount[GUN_NO_B]<0XFF))
        {
			gChargeCtrl.ucStopDelayCount[GUN_NO_B]--;
        }
        if((gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]>0)&&(gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]<180))
                {
                    gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]++;
                }
		if((gChargeCtrl.ucVoltCurrOKCount[GUN_NO_B]>0)&&(gChargeCtrl.ucVoltCurrOKCount[GUN_NO_B]<180))
		{
			gChargeCtrl.ucVoltCurrOKCount[GUN_NO_B]++;
		}
		if(gChargeCtrl.u16AgingFailCount[GUN_NO_B] >0)
			gChargeCtrl.u16AgingFailCount[GUN_NO_B]--;
		if((gChargeCtrl.u16AgingCount[GUN_NO_B]>0)&&(gChargeCtrl.u16AgingCount[GUN_NO_B]<65535))
			gChargeCtrl.u16AgingCount[GUN_NO_B]--;
		if((gChargeCtrl.u16StartCount[GUN_NO_B]>0)&&(gChargeCtrl.u16StartCount[GUN_NO_B]<250))
		{
			gChargeCtrl.u16StartCount[GUN_NO_B]++;
		}
		if((gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]>0)&&(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]<250))
		{
			gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]++;
		}
		if((gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay >0)&&(gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay<250))
			gBmsCommDataInfo[GUN_NO_B].CHAdeMOChargeEnableBitDelay++;
		if((gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay >0)&&(gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay<250))
			gBmsCommDataInfo[GUN_NO_B].CHAdeMORectModeChageDelay++;
		
	}
	if((UCHAR)tmNow.tm_min != ucMin)/*1分钟*/
	{
		ucMin = (UCHAR)tmNow.tm_min;
		fCurr = GetBusData(GUN_NO_B,BUS_CURR);
		if((fCurr >1)&&(gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease < 100)
			&&(gBmsCommDataInfo[GUN_NO_B].ucMachineState == TEST_CHARGE_CHECK_CCS))
		{
			gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease += 1;
		}
	}
	
}
static void GUN_B_InitGunAllData(void)
{
	UCHAR i;
	for(i =0;i<ALL_SEND_CMD_STATE;i++)
	{
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[i] = CAN_DATA_STOP;
	}
	for(i=0;i<ALL_GET_CMD_COUNT_ID;i++)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[i] = CAN_GET_CMD_COUNT_STOP;
	}
	gBmsCommDataInfo[GUN_NO_B].ucMachineState = TEST_HANDSHAKE_CHECK_CHM;
	gChargeCtrl.fEVOutVolt[GUN_NO_B] = 0;
	gChargeCtrl.fEVOutCurr[GUN_NO_B] = 0;
	gChargeCtrl.u16EVCMDGet[GUN_NO_B] = 0;
	gChargeCtrl.u16BMSCMDSend[GUN_NO_B] = 0;
	gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease = 0;
}
UINT8 GUN_B_SendFrame(UINT8 u8CMDID,UINT8 u8Len)
{
	if (gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[u8CMDID] == CAN_DATA_SEND)
	{
		BSP_Can_2_Tx(&gBmsCommDataInfo[GUN_NO_B].CMDMsg[u8CMDID], u8Len,CAN_ID_EXT);
		gChargeCtrl.u16BMSCMDSend[GUN_NO_B] =(UCHAR)((gBmsCommDataInfo[GUN_NO_B].CMDMsg[u8CMDID].Id&0x00FF0000)>>16);
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

UINT8 GUN_B_SendMultiFrame(void)
{
	if (gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE] == CAN_DATA_SEND)
	{
		BSP_Can_2_Tx(&gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_RST_STATE], 8,CAN_ID_EXT);
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE] = CAN_DATA_ACK;
		gChargeCtrl.u16BMSCMDSend[GUN_NO_B] = XXX_EC;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_200MS_COUNT;
		return TRUE;
	}
	return FALSE;
}
UINT8 GUN_B_SendMultiData(void)
{
	UINT8 u8Temp,u8PackageNum;

	u8PackageNum = (UINT8)(gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_RST_STATE].Data[0]&0x000000FF);
    u8Temp = gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE];
	if((u8Temp  >= CAN_DATA_FRAME1)&&(u8Temp <CAN_DATA_END))
	{
		
		BSP_Can_2_Tx(&gBmsCommDataInfo[GUN_NO_B].DataMsg[u8Temp-CAN_DATA_FRAME1], 8,CAN_ID_EXT);
		gChargeCtrl.u16BMSCMDSend[GUN_NO_B] = XXX_EB;
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE] = u8Temp+1;
		if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]-CAN_DATA_FRAME1) >= u8PackageNum)
		{
			gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE] = CAN_DATA_END ;
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_200MS_COUNT;
		}
		return TRUE;
	}
	return FALSE;
}
void CAN_2_SendCallBack(void)
{
	UCHAR i;
	for(i=0;i<ALL_GET_CMD_COUNT_ID;i++)
	{
		if((gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[i] >0)&&((gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[i] != CAN_GET_CMD_COUNT_STOP)))
		{
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[i]--;
		}
	}
	
	if(dv_Uart6.u8TimeOutFlag == TRUE)
	{
		dv_Uart6.u8TimeOutCount++;
		if(dv_Uart6.u8TimeOutCount>10)
		{
			dv_Uart6.u8TimeOutCount=0;
			dv_Uart6.pData = 0;
		}
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval >9000)
	{
		gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval -= 9000;
		if (GUN_B_SendMultiFrame())return;
	}
	if(GUN_B_SendFrame(SEND_CMD_TWO_GUN,8))return;
	for(i=0;i<9;i++)
	{
		u32CAN_2_PerioCount[i]++;		
	}
	
	if ((u32CAN_2_PerioCount[0]%TIME5_10MS_COUNT) == 0)
	{
		if (GUN_B_SendMultiData())return;
        if (GUN_B_SendFrame(SEND_CMD_BST_STATE,2))return;
	}
	if (u32CAN_2_PerioCount[1] >=TIME5_50MS_COUNT)
	{
		u32CAN_2_PerioCount[1] -=TIME5_50MS_COUNT;
		if (GUN_B_SendFrame(SEND_CMD_BCL_STATE,5))return;
	}
	
	if (u32CAN_2_PerioCount[2] >=TIME5_250MS_COUNT)
	{
		u32CAN_2_PerioCount[2] -=TIME5_250MS_COUNT;
		if (GUN_B_SendFrame(SEND_CMD_BHM_STATE,2))return;
		if (GUN_B_SendFrame(SEND_CMD_BRO_STATE,1))return;
		if (GUN_B_SendFrame(SEND_CMD_BSM_STATE,7))return;
		if (GUN_B_SendFrame(SEND_CMD_BSD_STATE,7))return;
		if (GUN_B_SendFrame(SEND_CMD_BEM_STATE,4))return;
		
	}
	
	if (u32CAN_2_PerioCount[3]>= TIME5_250MS_COUNT)
	{
		u32CAN_2_PerioCount[3] -=TIME5_250MS_COUNT;
		if(gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval == 250)
		{
			if (GUN_B_SendMultiFrame())return;
		}

	}
	
	if (u32CAN_2_PerioCount[4] >= TIME5_500MS_COUNT)
	{
		u32CAN_2_PerioCount[4] -=TIME5_500MS_COUNT;
		if(gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval == 500)
		{
			if (GUN_B_SendMultiFrame())return;
		}
	}
	if (u32CAN_2_PerioCount[5] >= TIME5_100MS_COUNT)
	{
		u32CAN_2_PerioCount[5] -=TIME5_100MS_COUNT;
		
		if (GUN_B_Send_Standard_Frame(SEND_CMD_100_STATE,8))return;
		
	}
	if (u32CAN_2_PerioCount[6] >= TIME5_100MS_COUNT)
	{
		u32CAN_2_PerioCount[6] -=TIME5_100MS_COUNT;

		if (GUN_B_Send_Standard_Frame(SEND_CMD_101_STATE,8))return;

	}
	if (u32CAN_2_PerioCount[7] >= TIME5_100MS_COUNT)
	{
		u32CAN_2_PerioCount[7] -=TIME5_100MS_COUNT;

		if (GUN_B_Send_Standard_Frame(SEND_CMD_102_STATE,8))return;

	}
	if (u32CAN_2_PerioCount[8] >= TIME5_100MS_COUNT)
	{
		u32CAN_2_PerioCount[8] -=TIME5_100MS_COUNT;

		if (GUN_B_Send_Standard_Frame(SEND_CMD_110_STATE,8))return;

	}
}
UCHAR CAN2_Read_AllBuf(UINT8 u8ReadPF,UINT8 u8Priority)
{
	UCHAR ucRTN = FALSE,iIdx = 0;
	for (iIdx = 0;iIdx < CAN_A_B_BUF_MAX_NUM; iIdx++)
	{
		if (u8ReadPF == 0xFF)
		{
			gCan2RxMsgBuf[iIdx].iFlag = FALSE;
			gCan2RxMsgBuf[iIdx].RxMsg.ExtId = 0;
			continue;
		}

		if ((u8ReadPF == (0xFF&(gCan2RxMsgBuf[iIdx].RxMsg.ExtId >> 16)))
			&&(u8Priority == (0xFF&(gCan2RxMsgBuf[iIdx].RxMsg.ExtId >> 26))))
		{
			ucRTN = TRUE;
			CanRxMsgBuff[GUN_NO_B] = gCan2RxMsgBuf[iIdx].RxMsg;
			gCan2RxMsgBuf[iIdx].iFlag = FALSE;
			gCan2RxMsgBuf[iIdx].RxMsg.ExtId = 0;
			gChargeCtrl.u16EVCMDGet[GUN_NO_B] = u8ReadPF; 
			break;
		}
	}
	return ucRTN;
}
static void CAN2_RefreshBHMFrameData(void)
{
	float ftemp;
	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BHM_STATE].Id = (((UINT32)PRIORITY_6)<<26) + ((UINT32)XXX_BHM << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	data.u16data[0] = 3210;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BHM_STATE].Data[0] = ((UINT32)data.ucdata[0] << 24) + ((UINT32)data.ucdata[1] << 16) ;
}
static void CAN2_RefreshBROFrameData(UCHAR ucdata)
{

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BRO_STATE].Id = (((UINT32)PRIORITY_4)<<26) + ((UINT32)XXX_BR0 << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BRO_STATE].Data[0] = ((UINT32)ucdata << 24) ;
}
static void CAN2_RefreshBCLFrameData(void)
{

	UINT32 u32temp1,u32temp2;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BCL_STATE].Id = (((UINT32)PRIORITY_6)<<26) + ((UINT32)XXX_BCL << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	u32temp1 =(UINT32)(gChargeCtrl.fBMSRequireVolt[GUN_NO_B]*10);
	u32temp2 =(UINT32)((400-gChargeCtrl.fBMSRequireCurr[GUN_NO_B])*10);
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BCL_STATE].Data[0] 
	       = ((0xff&u32temp1) << 24) + ((UINT32)(0xff&(u32temp1>>8)) << 16) + ((u32temp2&0xff)<<8) + ((u32temp2>>8)&0xff);
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BCL_STATE].Data[1] = 0x01000000;
}
static void CAN2_RefreshBSMFrameData(void)
{

	UINT32 u32BSMData1,u32BSMData2;
	
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BSM_STATE].Id = (((UINT32)PRIORITY_6)<<26) + ((UINT32)XXX_BSM << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	
	u32BSMData1 = (UINT32)gBmsCommDataInfo[GUN_NO_B].fBSM_MaxCellTemp+50;
	u32BSMData1 = (u32BSMData1<<16)+0x01003C02;
	u32BSMData2 = 0x00001000;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BSM_STATE].Data[0] = u32BSMData1;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BSM_STATE].Data[1] = u32BSMData2;
}
static void CAN2_RefreshBSTFrameData(void)
{

	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BST_STATE].Id = (((UINT32)PRIORITY_4)<<26) + ((UINT32)XXX_BST << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	data.u32data = 0;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BST_STATE].Data[0] = data.u32data;
	data.u32data = 0x10;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BST_STATE].Data[1] =  data.u32data;
}
static void CAN2_RefreshBSDFrameData(void)
{

	union
	{
		UCHAR ucdata[4];
		UINT16 u16data[2];
		UINT32 u32data;
		float  fdata;
	}data;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BSD_STATE].Id = (((UINT32)PRIORITY_6)<<26) + ((UINT32)XXX_BSD << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	data.u32data = 0;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BSD_STATE].Data[0] = data.u32data;
	data.u32data = 0x10;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BSD_STATE].Data[1] =  data.u32data;
}
static void CAN2_RefreshBEMFrameData(void)
{
	UINT32 u32BEMData1,u32BEMData2;

	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BEM_STATE].Id = (((UINT32)PRIORITY_2)<<26) + ((UINT32)XXX_BEM << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	u32BEMData1 = 0xFCF0F0F0;
	if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CRM_00_COUNT_ID)
	{
		u32BEMData1 |=0x00000001;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CRM_AA_COUNT_ID)
	{
		u32BEMData1 |=0x00000004;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CML_COUNT_ID)
	{
		u32BEMData1 |=0x00000100;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CTS_COUNT_ID)
	{
		u32BEMData1 |=0x00000100;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CRO_COUNT_ID)
	{
		u32BEMData1 |=0x00000400;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CCS_COUNT_ID)
	{
		u32BEMData1 |=0x00010000;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CST_COUNT_ID)
	{
		u32BEMData1 |=0x00040000;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag == GET_CMD_CSD_COUNT_ID)
	{
		u32BEMData1 |=0x01000000;
	}
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BEM_STATE].Data[0] = u32BEMData1;
	u32BEMData2 = 0;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_BEM_STATE].Data[1] =  u32BEMData2;
}
static void CAN2_RefreshMultiFrame_RST(UINT8 uPGN,UINT8 uByteNum,UINT8 uPak)
{

	UINT8 byEVbuf[8];
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_RST_STATE].Id = (((UINT32)PRIORITY_7)<<26) + ((UINT32)XXX_EC << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	byEVbuf[0] = 0x10;
	byEVbuf[1] = uByteNum;
	byEVbuf[2] = 0;
	byEVbuf[3] = uPak;
	byEVbuf[4] = 0xff;
	byEVbuf[5] = 0;
	byEVbuf[6] = uPGN;
	byEVbuf[7] = 0;
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_RST_STATE].Data[0] = ((UINT32)byEVbuf[0] << 24) + ((UINT32)byEVbuf[1] << 16) +  ((UINT32)byEVbuf[2] << 8) + byEVbuf[3];
	gBmsCommDataInfo[GUN_NO_B].CMDMsg[SEND_CMD_RST_STATE].Data[1] = ((UINT32)byEVbuf[4] << 24) + ((UINT32)byEVbuf[5] << 16) +  ((UINT32)byEVbuf[6] << 8) + byEVbuf[7];
}
static void CAN2_StuffMultiFrame_Data(UINT8 Num,UINT32 data1,UINT32 data2)
{
	gBmsCommDataInfo[GUN_NO_B].DataMsg[Num-1].Id = (((UINT32)PRIORITY_7)<<26) + ((UINT32)XXX_EB << 16) + ((UINT32)EV_ADDR<<8) + BMS_ADDR;
	gBmsCommDataInfo[GUN_NO_B].DataMsg[Num-1].Data[0] = (((UINT32)Num)<<24)+(data1>>8);
	gBmsCommDataInfo[GUN_NO_B].DataMsg[Num-1].Data[1] = (data1<<24)+(data2>>8);
}
static void CAN2_RefreshBRMFrameData(void)
{
	UINT32 u32temp1,u32temp2;
	if(gBmsCommDataInfo[GUN_NO_B].u8GBT_BMS_ProtocolType == BMS_VERSION_IS_GBT2015)
	{
		u32temp1 = 0x01010006;
	}
	else
	{
		u32temp1 = 0x00010006;
	}
	u32temp2 = 0xB80BEE00;
	CAN2_StuffMultiFrame_Data(1,u32temp1,u32temp2);

	u32temp1 = 0x02434154;
	u32temp2 = 0x4C000000;
	CAN2_StuffMultiFrame_Data(2,u32temp1,u32temp2);
	u32temp1 = 0xFFFFFFFF;
	u32temp2 = 0xFFFFFFFF;
	CAN2_StuffMultiFrame_Data(3,u32temp1,u32temp2);
	u32temp1 = 0xFFFFFFFF&gChargeCtrl.ucVIN[GUN_NO_B][0];
	u32temp2 = 0x000000FF&gChargeCtrl.ucVIN[GUN_NO_B][1];
	u32temp2 = u32temp2<<8;
	u32temp2 |=gChargeCtrl.ucVIN[GUN_NO_B][2];
	u32temp2 = u32temp2<<8;
	u32temp2 |=gChargeCtrl.ucVIN[GUN_NO_B][3];
	u32temp2 = u32temp2<<8;
	CAN2_StuffMultiFrame_Data(4,u32temp1,u32temp2);


	u32temp1 =0x000000FF&gChargeCtrl.ucVIN[GUN_NO_B][4];
	u32temp1 = u32temp2<<8;
	u32temp1 |=gChargeCtrl.ucVIN[GUN_NO_B][5];
	u32temp1 = u32temp2<<8;
	u32temp1 |=gChargeCtrl.ucVIN[GUN_NO_B][6];
	u32temp1 = u32temp2<<8;
	u32temp1 |=gChargeCtrl.ucVIN[GUN_NO_B][7];

	u32temp2 =0x000000FF&gChargeCtrl.ucVIN[GUN_NO_B][8];
	u32temp2 = u32temp2<<8;
	u32temp2 |=gChargeCtrl.ucVIN[GUN_NO_B][9];
	u32temp2 = u32temp2<<8;
	u32temp2 |=gChargeCtrl.ucVIN[GUN_NO_B][10];
	u32temp2 = u32temp2<<8;
	CAN2_StuffMultiFrame_Data(5,u32temp1,u32temp2);


	u32temp1=0x000000FF&gChargeCtrl.ucVIN[GUN_NO_B][11];
	u32temp1 = u32temp2<<8;
	u32temp1 |=gChargeCtrl.ucVIN[GUN_NO_B][12];
	u32temp1 = u32temp2<<8;
	u32temp1 |=gChargeCtrl.ucVIN[GUN_NO_B][13];
	u32temp1 = u32temp2<<8;
	u32temp1 |=gChargeCtrl.ucVIN[GUN_NO_B][14];

	u32temp2 =0x000000FF&gChargeCtrl.ucVIN[GUN_NO_B][15];
	u32temp2 = u32temp2<<8;
	u32temp2 |=gChargeCtrl.ucVIN[GUN_NO_B][16];
	u32temp2 = u32temp2<<8;
	if(gBmsCommDataInfo[GUN_NO_B].u8GBT_BMS_ProtocolType == BMS_VERSION_IS_GBT2015)
	{
		u32temp2 |=0x000010FF;
		CAN2_StuffMultiFrame_Data(6,u32temp1,u32temp2);
		CAN2_StuffMultiFrame_Data(7,0x0A0B07DF,0xFFFFFFFF);
	}
	else
	{
		u32temp2 |=0x0000FFFF;
		CAN2_StuffMultiFrame_Data(6,u32temp1,u32temp2);
	}
	CAN2_StuffMultiFrame_Data(4,u32temp1,u32temp2);
	u32temp1 = 0;
	u32temp2 = 0;
	CAN2_StuffMultiFrame_Data(5,u32temp1,u32temp2);

	
	if(gBmsCommDataInfo[GUN_NO_B].u8GBT_BMS_ProtocolType == BMS_VERSION_IS_GBT2015)
	{
		u32temp1 = 0;
		u32temp2 |=0x000010FF;
		CAN2_StuffMultiFrame_Data(6,u32temp1,u32temp2);
		CAN2_StuffMultiFrame_Data(7,0x0A0B07DF,0xFFFFFFFF);
	}
	else
	{
		u32temp1 = 0;
		u32temp2 |=0x0000FFFF;
		CAN2_StuffMultiFrame_Data(6,u32temp1,u32temp2);
	}
}
static void CAN2_RefreshBCPFrameData(void)
{
	UINT32 u32temp1,u32temp2,u32BCPValue1,u32BCPValue2;
	float ftemp;
	UCHAR uctemp;



	u32temp1 = (UINT32)(gBmsCommDataInfo[GUN_NO_B].fBCP_MaxPermitCellVolt* 100);
	u32temp2 = (UINT32)(10*(gBmsCommDataInfo[GUN_NO_B].fBCP_MaxPermitCurr+400));			
	u32BCPValue1 = ((0xff&u32temp1) << 24) + ((UINT32)(0xff&(u32temp1>>8)) << 16) + ((u32temp2&0xff)<<8) + ((u32temp2>>8)&0xff);

	u32temp1 = 300 *10;	
	u32temp2 =(UINT32) gBmsCommDataInfo[GUN_NO_B].fBCP_MaxVolt*10;			
	u32BCPValue2 = ((0xff&u32temp1) << 24) + ((UINT32)(0xff&(u32temp1>>8)) << 16) + ((u32temp2&0xff)<<8) + ((u32temp2>>8)&0xff);
	CAN2_StuffMultiFrame_Data(1,u32BCPValue1,u32BCPValue2);

	uctemp = (UCHAR)gBmsCommDataInfo[GUN_NO_B].fBCP_MaxPermitTemp;
	ftemp = gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartSOC;
	u32temp1 = ((0xff&u32BCPValue2)<<24) + ((uctemp + 50)<<16) + (((UINT16)(ftemp * 10))<<8) + (((UINT16)(ftemp * 10)>>8)&0x00ff);
	ftemp=gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartVolt;
	u32temp2 = (((UINT32)(ftemp*10)&0xff)<<24) + ((( (UINT32)(ftemp*10) >> 8)&0xff)<<16);

	u32BCPValue1 = u32temp1;
	u32BCPValue2 = u32temp2;
	CAN2_StuffMultiFrame_Data(2,u32BCPValue1,u32BCPValue2);
}
static void CAN2_RefreshBCSFrameData(void)
{
	UINT32 u32temp1,u32temp2,u32BCSValue1,u32BCSValue2;
	UCHAR uctemp;
	float ftemp;
	ftemp = GetBusData(GUN_NO_B,BUS_VOLT);
	u32temp1 = (UINT32)(ftemp*10);
	ftemp = GetBusData(GUN_NO_B,BUS_CURR);
	u32temp2 = (UINT32)(gIMIUData[GUN_NO_B].fCurr*10);
	u32temp2 = u32temp2+4000;
	u32BCSValue1 = ((0xff&u32temp1) << 24) + ((UINT32)(0xff&(u32temp1>>8)) << 16) + ((u32temp2&0xff)<<8) + ((u32temp2>>8)&0xff);
	
	u32temp1 = ((UINT16)(gBmsCommDataInfo[GUN_NO_B].fBCS_MaxCellVolt*100))+(3<<14);
	uctemp = (UCHAR)(gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartSOC +gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease);
	if(uctemp >100)
	{
		uctemp =100;
	}
	u32temp2 = uctemp;
	
	u32BCSValue2 = ((0xff&u32temp1) << 24) + ((UINT32)(0xff&(u32temp1>>8)) << 16) + ((u32temp2&0xff)<<8) + ((u32temp2>>8)&0xff);
	CAN2_StuffMultiFrame_Data(1,u32BCSValue1,u32BCSValue2);

	if(gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease <100)
	{
		uctemp = (UINT8)(100 - gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease);
	}
	else
	{
		uctemp = 0;
	}
	u32temp1 = uctemp;
	u32temp1 = u32temp1<<24;
	u32temp2 = 0;
	CAN2_StuffMultiFrame_Data(2,u32temp1,u32temp2);
}
static void GUN_B_ChargerDataInit()
{
	gChargeCtrl.fEVOutVolt[GUN_NO_B] = 0;
	gChargeCtrl.fEVOutCurr[GUN_NO_B] = 0;
	gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease = 0;
}
static UCHAR CAN2_HandShake_Check_CHM(void)
{
	UCHAR ucRTN;
	if(gChargeCtrl.ucCtrlCmdState[GUN_NO_B]== START_CHARGE_VAL)
	{
		ucRTN = CAN2_Read_AllBuf(XXX_CRM,PRIORITY_6);
		if((ucRTN == TRUE)&&(0x00 == CanRxMsgBuff[GUN_NO_B].Data[0]))
		{
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_00_COUNT_ID] = CAN_GET_CMD_5S_COUNT;  
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CHM_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BHM_STATE);
			return  TEST_HANDSHAKE_CHECK_CRM;
		}
		ucRTN = CAN2_Read_AllBuf(XXX_CHM,PRIORITY_6);
		if(ucRTN == TRUE)
		{
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CHM_COUNT_ID] = CAN_GET_CMD_5S_COUNT;  
			_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BHM_STATE);//准备发送
		}
		if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CHM_COUNT_ID] == 0)
		{
			gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CHM_COUNT_ID;
			return TEST_TIMEOUT_BEM;
		}
		if (gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BHM_STATE]  == CAN_DATA_WAIT)                     
		{
			CAN2_RefreshBHMFrameData();//更新BHM数据
			_START_SEND_CMD(GUN_NO_B,SEND_CMD_BHM_STATE);
			gBmsCommDataInfo[GUN_NO_B].u8GBT_BMS_ProtocolType = BMS_VERSION_IS_GBT2015;
		}
	}
	gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
	GUN_B_ChargerDataInit();
        return TEST_HANDSHAKE_CHECK_CHM;
}


static UCHAR CAN2_HandShake_Check_CRM(void)
{
	UCHAR ucRTN;
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_STOP)
	{
            gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval = 0;
            _WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]   == CAN_DATA_WAIT)
	{
		if(gBmsCommDataInfo[GUN_NO_B].u8GBT_BMS_ProtocolType  == BMS_VERSION_IS_GBT2015)
		{
			CAN2_RefreshMultiFrame_RST(XXX_BRM, 49, 7);						//RTS 请求链接
		}
		else
		{
			CAN2_RefreshMultiFrame_RST(XXX_BRM, 41, 6);						//RTS 请求链接
		}
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
        if(gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval ==0)
        {
          gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval = 9250;
        }
	}
	
	ucRTN = CAN2_Read_AllBuf(XXX_EC,PRIORITY_7);
	if((ucRTN == TRUE)&&(0x11 == CanRxMsgBuff[GUN_NO_B].Data[0])&&(XXX_BRM == CanRxMsgBuff[GUN_NO_B].Data[6]))//CTS ok
	{
		CAN2_RefreshBRMFrameData();
		_START_SEND_DATA(GUN_NO_B,SEND_CMD_RST_STATE,CAN_DATA_FRAME1);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	else if(((ucRTN == TRUE) && (0x13 ==CanRxMsgBuff[GUN_NO_B].Data[0]))
		||((ucRTN == TRUE) && (0xFF ==CanRxMsgBuff[GUN_NO_B].Data[0])))		
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] == 0)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}

	ucRTN = CAN2_Read_AllBuf(XXX_CRM,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		if(0xAA == CanRxMsgBuff[GUN_NO_B].Data[0])
		{
			gBmsCommDataInfo[GUN_NO_B].u8GetCRMAAFlag = TRUE;
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_00_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_AA_COUNT_ID] = CAN_GET_CMD_5S_COUNT;
		}
		if(0x00 == CanRxMsgBuff[GUN_NO_B].Data[0])
		{
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_00_COUNT_ID] = CAN_GET_CMD_5S_COUNT;
		}
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_00_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CRM_00_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8GetCRMAAFlag == TRUE)
	{
		if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_SEND)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_WAIT)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_ACK)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_END))
		{
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		}
		if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_STOP)
		{
			gBmsCommDataInfo[GUN_NO_B].u8GetCRMAAFlag = FALSE;
			return  TEST_CONFIG_CHECK_CML_CTS;
		}
	}
	return  TEST_HANDSHAKE_CHECK_CRM;
}



static UCHAR CAN2_Config_Check_CML_CTS(void)
{
	UCHAR ucRTN;
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_STOP)
	{
		gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval =0;
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]   == CAN_DATA_WAIT)
	{
		
		CAN2_RefreshMultiFrame_RST(XXX_BCP,13,2);						//RTS 请求链接
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		if(gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval ==0)
		{
			gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval = 9500;
		}
	}

	ucRTN = CAN2_Read_AllBuf(XXX_EC,PRIORITY_7);
	if((ucRTN == TRUE)&&(0x11 == CanRxMsgBuff[GUN_NO_B].Data[0])&&(XXX_BCP == CanRxMsgBuff[GUN_NO_B].Data[6]))//CTS ok
	{
		CAN2_RefreshBCPFrameData();
		_START_SEND_DATA(GUN_NO_B,SEND_CMD_RST_STATE,CAN_DATA_FRAME1);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_AA_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	else if(((ucRTN == TRUE) && (0x13 ==CanRxMsgBuff[GUN_NO_B].Data[0]))
		||((ucRTN == TRUE) && (0xFF ==CanRxMsgBuff[GUN_NO_B].Data[0])))		
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] == 0)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}

	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRM_AA_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CRM_AA_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CML,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CML_COUNT_ID] = CAN_GET_CMD_5S_COUNT;
		gBmsCommDataInfo[GUN_NO_B].u8GetCMLFlag = TRUE;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CML_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CML_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CTS,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CTS_COUNT_ID] = CAN_GET_CMD_5S_COUNT;
		gBmsCommDataInfo[GUN_NO_B].u8GetCTSFlag = TRUE;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CTS_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CTS_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8GetCMLFlag == TRUE)
		&&(gBmsCommDataInfo[GUN_NO_B].u8GetCTSFlag== TRUE))
	{
		if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_SEND)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_WAIT)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_ACK)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_END))
		{
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		}
		if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_STOP)
		{
			gBmsCommDataInfo[GUN_NO_B].u8GetCMLFlag = FALSE;
			gBmsCommDataInfo[GUN_NO_B].u8GetCTSFlag = FALSE;
			return  TEST_CONFIG_CHECK_CRO_AA;
		}
	}
	return  TEST_CONFIG_CHECK_CML_CTS;
}

static UCHAR CAN2_CONFIG_CHECK_CRO_AA(void)
{
	UCHAR uctemp,ucRTN;
	float ftemp,fVolt;
	ftemp=gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartVolt;
	gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = ftemp;
	if((ftemp<50)||(ftemp>750))
	{
		ftemp = 50;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep < 12)
	{
		gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep++;
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],10);
	}
	else 
	{
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],10);
	}
	
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BRO_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BRO_STATE);
	}
	
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BRO_STATE]   == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BRO_STATE]   == CAN_DATA_SEND))
	{
		fVolt =GetBusData(GUN_NO_B,BUS_VOLT); 
		if(fVolt >= (ftemp-5))
		{
			uctemp = 0xAA;
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CML_COUNT_ID]= CAN_GET_CMD_COUNT_STOP ;
			gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CTS_COUNT_ID]= CAN_GET_CMD_COUNT_STOP ;
		}
		else
		{
			uctemp = 0x00;
		}
		CAN2_RefreshBROFrameData(uctemp);//更新BHM数据
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_BRO_STATE);
	}
	

	ucRTN = CAN2_Read_AllBuf(XXX_CML,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CML_COUNT_ID]=CAN_GET_CMD_5S_COUNT ;
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CTS,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CTS_COUNT_ID]=CAN_GET_CMD_5S_COUNT ;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CML_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CML_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CTS_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CTS_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CR0,PRIORITY_4);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRO_COUNT_ID] = CAN_GET_CMD_5S_COUNT ;
		if(0xAA == CanRxMsgBuff[GUN_NO_B].Data[0])
		{
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BRO_STATE)
			gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep = 0;
			return TEST_CHARGE_CHECK_CCS ;
		}
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRO_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CRO_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	return  TEST_CONFIG_CHECK_CRO_AA;
}
static void CAN2_UnpackCCSFrame(void)
{
	UINT16 u16temp;
	float ftemp;
	u16temp = CanRxMsgBuff[GUN_NO_B].Data[0];
	u16temp += CanRxMsgBuff[GUN_NO_B].Data[1]*256;
	gChargeCtrl.fEVOutVolt[GUN_NO_B] = ((float)u16temp)*0.1;
	u16temp = CanRxMsgBuff[GUN_NO_B].Data[2];
	u16temp += CanRxMsgBuff[GUN_NO_B].Data[3]*256;
	
	ftemp = ((float)u16temp)*0.1-400;
	if(ftemp<0)
		ftemp = -ftemp;
	gChargeCtrl.fEVOutCurr[GUN_NO_B]= ftemp ;
}
static UCHAR CAN2_CHARGE_CHECK_CCS(void)
{
	UCHAR ucRTN,ucRectOutputMode;
	static UCHAR ucRectOutputModeBack = GRP_WORKMODE_INVERTER;
	float fVolt;
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
		if(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep >= 1)
		{
			gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep  = 1;
		}
	}
	
	ucRTN = CAN2_Read_AllBuf(XXX_CR0,PRIORITY_4);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRO_COUNT_ID] = CAN_GET_CMD_5S_COUNT ;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRO_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CRO_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BCL_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BCL_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BCL_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BCL_STATE] == CAN_DATA_SEND))
	{
		CAN2_RefreshBCLFrameData();//更新BCL数据
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_BCL_STATE);
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval = 0;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]   == CAN_DATA_WAIT)
	{

		CAN2_RefreshMultiFrame_RST(XXX_BCS,9,2);						//RTS 请求链接
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		if(gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval ==0)
		{
			gBmsCommDataInfo[GUN_NO_B].u16MultiFrameInterval = 9250;
		}

	}

	ucRTN = CAN2_Read_AllBuf(XXX_EC,PRIORITY_7);
	if((ucRTN == TRUE)&&(0x11 == CanRxMsgBuff[GUN_NO_B].Data[0])&&(XXX_BCS == CanRxMsgBuff[GUN_NO_B].Data[6]))//CTS ok
	{
		CAN2_RefreshBCSFrameData();
		_START_SEND_DATA(GUN_NO_B,SEND_CMD_RST_STATE,CAN_DATA_FRAME1);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	else if(((ucRTN == TRUE) && (0x13 == CanRxMsgBuff[GUN_NO_B].Data[0]))
		||((ucRTN == TRUE) && (0xFF == CanRxMsgBuff[GUN_NO_B].Data[0])))		
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	else if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] == 0)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_EC_CTS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}

	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BSM_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BSM_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BSM_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BSM_STATE] == CAN_DATA_SEND))
	{
		CAN2_RefreshBSMFrameData();//更新BCL数据
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_BSM_STATE);
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CCS,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CRO_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CCS_COUNT_ID] = CAN_GET_CMD_1S_COUNT ;
		CAN2_UnpackCCSFrame();
		fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
		if((gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep == 0)&&(fVolt >= (gChargeCtrl.fBMSRequireVolt[GUN_NO_B]-10)))
		{
			if(ucRectOutputMode == GRP_WORKMODE_INVERTER)
			{
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
				gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep = 1;
			}
			else
			{
				gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep = 40;
			}
		}
		else if((gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep>0)&&(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep < 20))
		{
			if(ucRectOutputMode == GRP_WORKMODE_INVERTER)
			{
				gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep++;
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,0,0);
			}
			else
			{
				gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep = 40;
			}
		}
		else if((gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep>=20)&&(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep < 40))
		{
            gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep++;
			if(ucRectOutputMode == GRP_WORKMODE_INVERTER)
			{
				gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,gChargeCtrl.fBMSRequireVolt[GUN_NO_B]-30,5);
			}
			else
			{
				gChargeCtrl.fBMSRequireVolt[GUN_NO_B] = gChargeCtrl.fUserSetVolt[GUN_NO_B];
				gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],gChargeCtrl.fUserSetLoadCurr[GUN_NO_B]);
			}
		}
		else if((gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep>=40)&&(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep < 100))
		{
			if(gChargeCtrl.fUserSetLoadCurr[GUN_NO_B] >0)
			{
				gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep++;
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

			}
		}
		else if(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep > 0)
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
		}

	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CCS_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CCS_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CST,PRIORITY_4);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u8GetCSTFlag = TRUE;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CCS_COUNT_ID] = CAN_GET_CMD_COUNT_STOP ;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] = CAN_GET_CMD_5S_COUNT ;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8GetCSTFlag == TRUE)
        {
            _STOP_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
            _STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BCL_STATE);
            _STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BSM_STATE);
            gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep =0;
			gBmsCommDataInfo[GUN_NO_B].u8SendBSTFlag = FALSE;
			gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,0,0);
            return  TEST_STOP_CHECK_CST;
        }
    if(gChargeCtrl.ucCtrlCmdState[GUN_NO_B] == STOP_CHARGE_VAL)
	{
		if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_SEND)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_WAIT)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_ACK)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_END))
		{
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_RST_STATE);
		}
		if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_RST_STATE]  == CAN_DATA_STOP)
		{
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BCL_STATE);
			_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BSM_STATE);
            gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep =0;
			gBmsCommDataInfo[GUN_NO_B].u8SendBSTFlag = FALSE;
			return  TEST_STOP_CHECK_CST;
		}
	}
	 if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] == 0)
	 {
		 gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CST_COUNT_ID;
		 return TEST_TIMEOUT_BEM;
	 }
	return TEST_CHARGE_CHECK_CCS ;
}

static UCHAR CAN2_STOP_CHECK_CST(void)
{
	UCHAR ucRTN,ucRectOutputMode;
	if(gChargeCtrl.ucRectWorkMode[GUN_NO_A] == 0)
	{
		ucRectOutputMode = GRP_WORKMODE_INVERTER;
	}
	else
	{
		ucRectOutputMode = GRP_WORKMODE_RECTIFIER;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BST_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BST_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BST_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BST_STATE] == CAN_DATA_SEND))
	{
		CAN2_RefreshBSTFrameData();//更新BST数据
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_BST_STATE);
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,0,0);
	}
	ucRTN = CAN2_Read_AllBuf(XXX_CST,PRIORITY_4);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u8GetCSTFlag = TRUE;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] = CAN_GET_CMD_5S_COUNT ;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CST_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8GetCSTFlag == TRUE)&&(gBmsCommDataInfo[GUN_NO_B].u8SendBSTFlag== TRUE))
	{
		gBmsCommDataInfo[GUN_NO_B].u8GetCSTFlag = FALSE;
		return  TEST_STOP_CHECK_CSD;
	}
	return  TEST_STOP_CHECK_CST;
}
static UCHAR CAN2_STOP_CHECK_CSD(void)
{
	UCHAR ucRTN;
	if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BSD_STATE]  == CAN_DATA_STOP)
	{
		_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BSD_STATE);
	}
	if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BSD_STATE] == CAN_DATA_WAIT)
		||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BSD_STATE] == CAN_DATA_SEND))
	{
		CAN2_RefreshBSDFrameData();//更新BCL数据
		_START_SEND_CMD(GUN_NO_B,SEND_CMD_BSD_STATE);
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CSD_COUNT_ID] = CAN_GET_CMD_5S_COUNT ;
	}

	ucRTN = CAN2_Read_AllBuf(XXX_CST,PRIORITY_4);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] = CAN_GET_CMD_5S_COUNT ;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CST_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}

	ucRTN = CAN2_Read_AllBuf(XXX_CSD,PRIORITY_6);
	if(ucRTN == TRUE)
	{
		gBmsCommDataInfo[GUN_NO_B].u8GetCSDFlag = TRUE;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CST_COUNT_ID] = CAN_GET_CMD_COUNT_STOP ;
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CSD_COUNT_ID] = CAN_GET_CMD_COUNT_STOP;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[GET_CMD_CSD_COUNT_ID] == 0)
	{
		gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = GET_CMD_CSD_COUNT_ID;
		return TEST_TIMEOUT_BEM;
	}
	if(gBmsCommDataInfo[GUN_NO_B].u8GetCSDFlag == TRUE)
	{
		_STOP_SEND_CMD(GUN_NO_B,SEND_CMD_BSD_STATE);
		gBmsCommDataInfo[GUN_NO_B].u8GetCSDFlag = FALSE;
		
		return  TEST_HANDSHAKE_CHECK_CHM;
	}
	return  TEST_STOP_CHECK_CSD;
}
static UCHAR CAN2_TIMEOUT_BEM(void)
{
	UCHAR ucRectOutputMode;
	if(gChargeCtrl.ucRectWorkMode[GUN_NO_A] == 0)
	{
		ucRectOutputMode = GRP_WORKMODE_INVERTER;
	}
	else
	{
		ucRectOutputMode = GRP_WORKMODE_RECTIFIER;
	}
		if(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BEM_STATE]  == CAN_DATA_STOP)
		{
			gBmsCommDataInfo[GUN_NO_B].u8SendBEMFlag = 1;
			_WAIT_SEND_CMD(GUN_NO_B,SEND_CMD_BEM_STATE);
		}
		if((gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BEM_STATE] == CAN_DATA_WAIT)
			||(gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[SEND_CMD_BEM_STATE] == CAN_DATA_SEND))
		{
			CAN2_RefreshBEMFrameData();//更新BCL数据
			_START_SEND_CMD(GUN_NO_B,SEND_CMD_BEM_STATE);
		}
		CAN2_Read_AllBuf(XXX_CEM,PRIORITY_2);
		if(gBmsCommDataInfo[GUN_NO_B].u8SendBEMFlag < 5)
		{
			return  TEST_TIMEOUT_BEM;
		}
		else
		{
			gBmsCommDataInfo[GUN_NO_B].u8SendBEMFlag = 0;
			GUN_B_InitGunAllData();
			gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,0,0);
			return  TEST_HANDSHAKE_CHECK_CHM;
		}
	

	
	
	
}
static void GUN_B_AgingRecordStartProcess(void)
{
	struct tm  stTimeNow;
	UCHAR i;
	gRTCData.fGetLocalTime(&stTimeNow);
	
	if(gChargeCtrl.ucAgingRecordState[GUN_NO_B] == 0)
	{
		gChargeCtrl.ucAgingRecordState[GUN_NO_B] =1;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8GunNum = gChargeCtrl.ucTestGunNum[GUN_NO_B];
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8Reserve[0] = 0;	
                gChargeCtrl.stAgingRecord[GUN_NO_B].u8Reserve[1] = 0;	
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[0] = (UCHAR)(stTimeNow.tm_year/100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[1] = (UCHAR)(stTimeNow.tm_year%100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[2] = (UCHAR)stTimeNow.tm_mon;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[3] = (UCHAR)stTimeNow.tm_mday;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[4] = (UCHAR)stTimeNow.tm_hour;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[5] = (UCHAR)stTimeNow.tm_min;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[6] = (UCHAR)stTimeNow.tm_sec;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8GunType = gChargeCtrl.ucBMSProtocolType[GUN_NO_B];
		gChargeCtrl.stAgingRecord[GUN_NO_B].fUserSetVolt = gChargeCtrl.fBMSRequireVolt[GUN_NO_B];				
		gChargeCtrl.stAgingRecord[GUN_NO_B].fUserSetCurr = gChargeCtrl.fBMSRequireCurr[GUN_NO_B];	
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16UserSetTime = gChargeCtrl.u16AgingTime[GUN_NO_B];
		
		for(i =0;i<22;i++)
		{
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8EVSerialNum[i] = gChargeCtrl.ucEVCode[GUN_NO_B][i];
		}
		for(i =22;i<32;i++)
		{
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8EVSerialNum[i] =0;
		}

		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxVolt = 0;				//7 桩上报的充电电压最大值	4		49
		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinVolt = 10000;             //8 桩上报的充电电压最小值	4		53
		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxCurr = 0;				//9 桩上报的充电电流最大值	4		57
		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinCurr =10000;				//10桩上报的充电电流最小值	4		61
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxVolt = 0;				//11 工装测量最大电压		4		65
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinVolt = 10000;				//12 工装测量最小电压		4		69
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxCurr = 0;				//13 工装测量最大电流		4		73
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinCurr = 10000;				//14 工装测量最小电流		4		77
		gChargeCtrl.stAgingRecord[GUN_NO_B].fCC2Volt = 0;						//15 工装测量的CC2电压		4		81
		gChargeCtrl.stAgingRecord[GUN_NO_B].fBMSVolt = 0;						//16工装测量的辅助源电压	4		85
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16TestTime = 0 ;					//17 老化实际测试时间		2		87
		for(i =0;i<7;i++)
		{
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[i] = 0;
		}
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestResult = 0 ;					//20 老化测试结果			1		102
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestFailReason = 0;             //21 老化测试失败原因		1		103	
	}
} 
void GUN_B_TestRecordStartProcess(void)
{
	struct tm  stTimeNow;
	UCHAR i;
	gRTCData.fGetLocalTime(&stTimeNow);

	if(gChargeCtrl.ucTestRecordState[GUN_NO_B] == 0)
	{
		gChargeCtrl.ucTestRecordState[GUN_NO_B] =1;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8GunNum = gChargeCtrl.ucTestGunNum[GUN_NO_B];
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8Reserve[0] = 0;	
                gChargeCtrl.stAgingRecord[GUN_NO_B].u8Reserve[1] = 0;		
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[0] = (UCHAR)(stTimeNow.tm_year/100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[1] = (UCHAR)(stTimeNow.tm_year%100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[2] = (UCHAR)stTimeNow.tm_mon;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[3] = (UCHAR)stTimeNow.tm_mday;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[4] = (UCHAR)stTimeNow.tm_hour;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[5] = (UCHAR)stTimeNow.tm_min;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8StartTime[6] = (UCHAR)stTimeNow.tm_sec;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8GunType = gChargeCtrl.ucBMSProtocolType[GUN_NO_B];
		gChargeCtrl.stAgingRecord[GUN_NO_B].fUserSetVolt = gChargeCtrl.fBMSRequireVolt[GUN_NO_B];				
		gChargeCtrl.stAgingRecord[GUN_NO_B].fUserSetCurr = gChargeCtrl.fBMSRequireCurr[GUN_NO_B];	
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16UserSetTime = 0;//gChargeCtrl.u16AgingTime[GUN_NO_B];

		for(i =0;i<22;i++)
		{
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8EVSerialNum[i] = gChargeCtrl.ucEVCode[GUN_NO_B][i];
		}
		for(i =22;i<32;i++)
		{
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8EVSerialNum[i] =0;
		}

		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxVolt = 0;				//7 桩上报的充电电压最大值	4		49
		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinVolt = 10000;             //8 桩上报的充电电压最小值	4		53
		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxCurr = 0;				//9 桩上报的充电电流最大值	4		57
		gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinCurr =10000;				//10桩上报的充电电流最小值	4		61
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxVolt = 0;				//11 工装测量最大电压		4		65
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinVolt = 10000;				//12 工装测量最小电压		4		69
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxCurr = 0;				//13 工装测量最大电流		4		73
		gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinCurr = 10000;				//14 工装测量最小电流		4		77
		gChargeCtrl.stAgingRecord[GUN_NO_B].fCC2Volt = 0;						//15 工装测量的CC2电压		4		81
		gChargeCtrl.stAgingRecord[GUN_NO_B].fBMSVolt = 0;						//16工装测量的辅助源电压	4		85
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16TestTime = 0 ;					//17 老化实际测试时间		2		87
		for(i =0;i<7;i++)
		{
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[i] = 0;
		}
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestResult = 0 ;					//20 老化测试结果			1		102
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestFailReason = 0;             //21 老化测试失败原因		1		103	
	}
} 
void GUN_B_TestRecordProcess(void)
{
	struct tm  stTimeNow;
	float fVolt,fCurr;
	gRTCData.fGetLocalTime(&stTimeNow);
	if(gChargeCtrl.ucTestRecordState[GUN_NO_B] == 1)
	{
		if(gChargeCtrl.fEVOutVolt[GUN_NO_B] > gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxVolt)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxVolt =gChargeCtrl.fEVOutVolt[GUN_NO_B];
		if(gChargeCtrl.fEVOutVolt[GUN_NO_B]<gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinVolt )
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinVolt = gChargeCtrl.fEVOutVolt[GUN_NO_B];
		fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
		if(fVolt> gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxVolt)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxVolt = fVolt;
		if(fVolt < gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinVolt)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinVolt = fVolt;
		if(gChargeCtrl.fEVOutCurr[GUN_NO_B] > gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxCurr)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxCurr =gChargeCtrl.fEVOutCurr[GUN_NO_B];
		if(gChargeCtrl.fEVOutCurr[GUN_NO_B]<gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinCurr )
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinCurr = gChargeCtrl.fEVOutCurr[GUN_NO_B];
		fCurr = GetBusData(GUN_NO_B,BUS_CURR);
		if(fCurr > gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxCurr)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxCurr = fCurr;
		if(fCurr < gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinCurr)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinCurr = fCurr;
		gChargeCtrl.stAgingRecord[GUN_NO_B].fCC2Volt = gMeasureInfo.fCC1Volt[GUN_NO_B];	
		gChargeCtrl.stAgingRecord[GUN_NO_B].fBMSVolt = gMeasureInfo.fBattVolt[GUN_NO_B];
	}
	else if(gChargeCtrl.ucTestRecordState[GUN_NO_B] == 2)
	{
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16TestTime = 0;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[0] = (UCHAR)(stTimeNow.tm_year/100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[1] = (UCHAR)(stTimeNow.tm_year%100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[2] = (UCHAR)stTimeNow.tm_mon;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[3] = (UCHAR)stTimeNow.tm_mday;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[4] = (UCHAR)stTimeNow.tm_hour;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[5] = (UCHAR)stTimeNow.tm_min;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[6] = (UCHAR)stTimeNow.tm_sec;
		if(gChargeCtrl.ucStepRTN[GUN_NO_B] == 0)
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestResult = 3;	
		else
			gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestResult = 2;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestFailReason = gChargeCtrl.u16FaultStep[GUN_NO_B] ;
		gChargeCtrl.ucTestRecordState[GUN_NO_B] = 3;
	}
}
static void GUN_B_AgingRecordProcess(void)
{
	struct tm  stTimeNow;
	float fVolt,fCurr;
	gRTCData.fGetLocalTime(&stTimeNow);
	if(gChargeCtrl.ucAgingRecordState[GUN_NO_B] != 1)
	{
		return;
	}
	if(gChargeCtrl.ucAgingState[GUN_NO_B] == AGING_STATE)
	{
		if(gChargeCtrl.fEVOutVolt[GUN_NO_B] > gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxVolt)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxVolt =gChargeCtrl.fEVOutVolt[GUN_NO_B];
		if(gChargeCtrl.fEVOutVolt[GUN_NO_B]<gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinVolt )
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinVolt = gChargeCtrl.fEVOutVolt[GUN_NO_B];
		if(gChargeCtrl.fEVOutCurr[GUN_NO_B] > gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxCurr)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMaxCurr =gChargeCtrl.fEVOutCurr[GUN_NO_B];
		if(gChargeCtrl.fEVOutCurr[GUN_NO_B]<gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinCurr )
			gChargeCtrl.stAgingRecord[GUN_NO_B].fEVOutputMinCurr = gChargeCtrl.fEVOutCurr[GUN_NO_B];
		fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
		if(fVolt > gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxVolt)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxVolt = fVolt;
		if(fVolt < gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinVolt)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinVolt = fVolt;
		fCurr = GetBusData(GUN_NO_B,BUS_CURR);
		if(fCurr > gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxCurr)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMaxCurr = fCurr;
		if(fCurr < gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinCurr)
			gChargeCtrl.stAgingRecord[GUN_NO_B].fMeasuerMinCurr = fCurr;
		gChargeCtrl.stAgingRecord[GUN_NO_B].fCC2Volt = gMeasureInfo.fCC1Volt[GUN_NO_B];	
		gChargeCtrl.stAgingRecord[GUN_NO_B].fBMSVolt = gMeasureInfo.fBattVolt[GUN_NO_B];
	}
	else if(gChargeCtrl.ucAgingState[GUN_NO_B] == AGING_OK_STATE)
	{
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16TestTime = gChargeCtrl.u16AgingTime[GUN_NO_B] - gChargeCtrl.u16AgingCount[GUN_NO_B];
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[0] = (UCHAR)(stTimeNow.tm_year/100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[1] = (UCHAR)(stTimeNow.tm_year%100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[2] = (UCHAR)stTimeNow.tm_mon;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[3] = (UCHAR)stTimeNow.tm_mday;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[4] = (UCHAR)stTimeNow.tm_hour;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[5] = (UCHAR)stTimeNow.tm_min;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[6] = (UCHAR)stTimeNow.tm_sec;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestResult = 0;					
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestFailReason =0 ;
		gChargeCtrl.ucAgingRecordState[GUN_NO_B] = 2;
	}
	else if(gChargeCtrl.ucAgingState[GUN_NO_B] == AGING_FAIL_STATE)
	{
		gChargeCtrl.stAgingRecord[GUN_NO_B].u16TestTime = gChargeCtrl.u16AgingTime[GUN_NO_B] - gChargeCtrl.u16AgingCount[GUN_NO_B];
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[0] = (UCHAR)(stTimeNow.tm_year/100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[1] = (UCHAR)(stTimeNow.tm_year%100);
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[2] = (UCHAR)stTimeNow.tm_mon;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[3] = (UCHAR)stTimeNow.tm_mday;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[4] = (UCHAR)stTimeNow.tm_hour;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[5] = (UCHAR)stTimeNow.tm_min;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8EndTime[6] = (UCHAR)stTimeNow.tm_sec;
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestResult = 1;					
		gChargeCtrl.stAgingRecord[GUN_NO_B].u8TestFailReason =0 ; 
		gChargeCtrl.ucAgingRecordState[GUN_NO_B] = 2;
	}
}
UCHAR GUN_B_AgingModeProcess(void)
{
	UCHAR ucRTN = FALSE;
	float fCurr;
	fCurr = GetBusData(GUN_NO_B,BUS_CURR);
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] == BUTTOM_START_AGING)
	{
		if(gChargeCtrl.ucAgingState[GUN_NO_B] ==NOT_AGING_STATE)
		{
			gChargeCtrl.ucAgingState[GUN_NO_B] = START_AGING_STATE; //进入老化状态
			gChargeCtrl.u16AgingFailCount[GUN_NO_B] =0;
			gChargeCtrl.u16AgingCount[GUN_NO_B] = 0;
			ucRTN = TRUE;
		}
		else if((gChargeCtrl.ucAgingState[GUN_NO_B] ==START_AGING_STATE)
			&&(fCurr >5))
			//&&(ABS(gIMIUData[GUN_NO_B].fCurr - gChargeCtrl.fBMSRequireCurr[GUN_NO_B])<3))
		{
			gChargeCtrl.ucAgingState[GUN_NO_B] = AGING_STATE; //进入老化状态
			gChargeCtrl.u16AgingCount[GUN_NO_B] = 65535; //进入老化状态
			if(gChargeCtrl.u16AgingTime[GUN_NO_B] <60)
				gChargeCtrl.u16AgingFailCount[GUN_NO_B] = 60;
			else
				gChargeCtrl.u16AgingFailCount[GUN_NO_B] =gChargeCtrl.u16AgingTime[GUN_NO_B]-5;
			ucRTN = TRUE;
		}
		else if((gChargeCtrl.ucAgingState[GUN_NO_B] ==AGING_STATE)
			&&(fCurr >5)
			&&(ABS(fCurr - gChargeCtrl.fUserSetLoadCurr[GUN_NO_B])<3)
			&&(gChargeCtrl.u16AgingFailCount[GUN_NO_B] > 0)
			&&(gChargeCtrl.u16AgingCount[GUN_NO_B] > 0))
		{
			if(gChargeCtrl.u16AgingCount[GUN_NO_B]==65535)
			{
				gChargeCtrl.u16AgingCount[GUN_NO_B] =gChargeCtrl.u16AgingTime[GUN_NO_B];
			}
			if(gChargeCtrl.u16AgingTime[GUN_NO_B] <60)
				gChargeCtrl.u16AgingFailCount[GUN_NO_B] = 60;
			else
				gChargeCtrl.u16AgingFailCount[GUN_NO_B] =gChargeCtrl.u16AgingTime[GUN_NO_B]-5;
			GUN_B_AgingRecordStartProcess();
			ucRTN = TRUE;
		}
		else if((gChargeCtrl.ucAgingState[GUN_NO_B] ==AGING_STATE)
			&&(gChargeCtrl.u16AgingFailCount[GUN_NO_B] == 0))
		{
			gChargeCtrl.ucAgingState[GUN_NO_B] = AGING_FAIL_STATE ; //进入老化状态 = 180;
			ucRTN = FALSE;
		}
		else if((gChargeCtrl.ucAgingState[GUN_NO_B] ==AGING_STATE)
			&&(gChargeCtrl.u16AgingCount[GUN_NO_B] == 0))
		{
			gChargeCtrl.ucAgingState[GUN_NO_B] = AGING_OK_STATE ; //进入老化状态 = 180;
			gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] = NO_BUTTOM_START;
			ucRTN = FALSE;
		}
		else
		{
			ucRTN = TRUE;
		}
	}
	else
	{
		ucRTN = FALSE;
	}
	GUN_B_AgingRecordProcess();
	return ucRTN;
}
 static UCHAR GUN_B_InsulationCheckStart(UCHAR ucDelay)
{
	float fVolt;
	fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] ==  BUTTOM_INSUL_START)//开始测试
	{
		if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)!=0x01)
		{
			gIMIUData[GUN_NO_B].ucPutResistance = 0x01;//投入电阻
		}
		if(fVolt  > 100)
		{
			gChargeCtrl.ucInsulationVoltCheckFlag[GUN_NO_B] = TRUE;
		}
		if(gChargeCtrl.u16PutResOnBusCount[GUN_NO_B] >0)
		{
			GUN_B_TestRecordStartProcess();
		}
		if((gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]>3)&&((gChargeCtrl.u16PutResOnBusCount[GUN_NO_B]<ucDelay)))
		{
			if((gBmsCommDataInfo[GUN_NO_B].ucMachineState >=TEST_HANDSHAKE_CHECK_CRM)&&(gBmsCommDataInfo[GUN_NO_B].ucMachineState <=TEST_STOP_CHECK_CSD	))
			{
				gChargeCtrl.u16InsulState[GUN_NO_B] = 2;
				gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] = NO_BUTTOM_START;
				if((gIMIUData[GUN_NO_B].ucPutResistance&0x01)==0x01)
				{
					gIMIUData[GUN_NO_B].ucPutResistance = 0x00;//去掉电阻	
				}
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
static UCHAR GUN_B_Charge1Check(void)
{
	float fVolt,fCurr;
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] ==  BUTTOM_START_1_CHARGE)//开始测试
	{
		if(gBmsCommDataInfo[GUN_NO_B].ucMachineState == TEST_CHARGE_CHECK_CCS)
		{
			gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGING_STATE ;    //充电状态
			if(gBmsCommDataInfo[GUN_NO_A].u8OpenRectStep>=35)
			{
				GUN_B_TestRecordProcess();
			}
			if((gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep >35)&&(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep<40))
			{
				if(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]==0)
				{
					gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]=1;
				}
				if(gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B]<15)
				{
					gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep =35;
				}
				else
				{
                                        
					gChargeCtrl.fEVOutVoltBack[GUN_NO_B]   = gChargeCtrl.fEVOutVolt[GUN_NO_B];
					fVolt = GetBusData(GUN_NO_B,BUS_VOLT);
					gChargeCtrl.fActualVoltBack[GUN_NO_B] = fVolt;
					gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep =41;
                    gChargeCtrl.u8GetVoltCurrCount[GUN_NO_B] = 0;
				}
				
			}
			if(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep >=100)
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
static UCHAR GUN_B_Charge2Check(void)
{
	if(gChargeCtrl.ucUIStartChargeCMD[GUN_NO_B] ==  BUTTOM_START_2_CHARGE)//开始测试
	{
		if(gBmsCommDataInfo[GUN_NO_B].ucMachineState == TEST_CHARGE_CHECK_CCS)
		{
			gChargeCtrl.u8ChargeState[GUN_NO_B] = CHARGING_STATE ;    //充电状态
			if(gBmsCommDataInfo[GUN_NO_B].u8OpenRectStep >=100)
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
static UCHAR GUN_B_StartModeProcess(void)
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
		uctemp = GUN_B_InsulationCheckStart(30);
		if(uctemp == FALSE)
		{
			return FALSE;
		}
		uctemp = GUN_B_Charge1Check();
		if(uctemp == FALSE)
		{
			return FALSE;
		}
                uctemp = GUN_B_Charge2Check();
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
static UCHAR GUN_B_BMSPowerCheck(void)
{
	float fVlot;
	UCHAR ucAuxPowerType;
	fVlot = GetBusData(GUN_NO_B,BUS_VOLT);
	GetCharSetData(SETDATA_GUN2_ucAuxPowerType_NUM,&ucAuxPowerType);
	if(ucAuxPowerType == mBMS_POWER_12V)
	{
		if((gMeasureInfo.fBattVolt[GUN_NO_B]<10)||(gMeasureInfo.fBattVolt[GUN_NO_B]>14))
		{
			GUN_B_InitGunAllData();
			if(fVlot>45)
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_12V_Fail;
			}
			gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
			return FALSE;
		}
		else
		{
			if(fVlot>45)
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_12V_OK;
			}
			return TRUE;
		}
	}
	else if(ucAuxPowerType == mBMS_POWER_24V)
	{
		if((gMeasureInfo.fBattVolt[GUN_NO_B]<22)||(gMeasureInfo.fBattVolt[GUN_NO_B]>26))
		{
			gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
			if(fVlot>45)
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_24V_Fail;
			}
			GUN_B_InitGunAllData();
			return FALSE;
		}
		else
		{
			if(fVlot>45)
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_24V_OK;
			}
			return TRUE;
		}
	}
	else
	{
		if((gChargeCtrl.u8BMSPowerStep[GUN_NO_B] == 0)&&(fVlot>45))
		{
			if((gMeasureInfo.fBattVolt[GUN_NO_B]<10)||(gMeasureInfo.fBattVolt[GUN_NO_B]>14))
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_12V_Fail;
			}
			else
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_12V_OK;

			}
		}
		if((gChargeCtrl.u8BMSPowerStep[GUN_NO_B] == 1)&&(fVlot>45))
		{
			if((gMeasureInfo.fBattVolt[GUN_NO_B]<22)||(gMeasureInfo.fBattVolt[GUN_NO_B]>26))
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_24V_Fail;
			}
			else
			{
				gChargeCtrl.u8BMSPoweState[GUN_NO_B] = mBMS_POWER_24V_OK;

			}
		}
		  return TRUE;
	}
}
static void GUN_B_GBT_ProtocolTestProcess(void)
{
	UCHAR uctemp,ucRTN;
	if(gMeasureInfo.ucGunState[GUN_NO_B] == GUN_DISCONNECT)//未插枪，不做启动
	{
		GUN_B_InitGunAllData();
	}
	else
	{
		ucRTN = GUN_B_StartModeProcess();
		if((ucRTN==TRUE)&&(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0xFF))//非停止充电阶段，可以启动
		{
			uctemp = GUN_B_BMSPowerCheck();
			if(uctemp == FALSE)
			{
				return ;
			}	
			gChargeCtrl.ucCtrlCmdState[GUN_NO_B]= START_CHARGE_VAL;
		}
		else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B] >0)
		{
			if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0xFF) 
			{
				gChargeCtrl.ucStopDelayCount[GUN_NO_B] = 5;
			}
			gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
			gChargeCtrl.ucCtrlCmdState[GUN_NO_B]= STOP_CHARGE_VAL;
			gChargeCtrl.ucGBTProtocolType[GUN_NO_B] = BMS_VERSION_IS_GBT2011;
		}
		else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0)
		{
			GUN_B_InitGunAllData();
			gChargeCtrl.ucStopDelayCount[GUN_NO_B]= 0xFF;
		}
		gBmsCommDataInfo[GUN_NO_B].ucMachineState = BMSMachineIfo[GUN_NO_B][gBmsCommDataInfo[GUN_NO_B].ucMachineState].fproc();
	}
}
static UCHAR GUN_B_CCS2_StartModeProcess(void)
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
		uctemp = GUN_B_InsulationCheckStart(40);
		if(uctemp == FALSE)
		{
			return FALSE;
		}
		uctemp = GUN_B_Charge1Check();
		if(uctemp == FALSE)
		{
			return FALSE;
		}
		uctemp = GUN_B_Charge2Check();
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
static void GUN_B_CCS2_ProtocolTestProcess(void)
{
    UCHAR ucRTN;	
	UCHAR ucAuxPowerType;
	GetCharSetData(SETDATA_GUN2_ucAuxPowerType_NUM,&ucAuxPowerType);
    ucRTN = GUN_B_CCS2_StartModeProcess();
    if((ucRTN==TRUE)&&(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0xFF))//非停止充电阶段，可以启动
    {
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
            gChargeCtrl.ucCtrlCmdState[GUN_NO_B]= START_CHARGE_VAL;
    }
    else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B] >0)
    {
            if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0xFF) 
            {
                    gChargeCtrl.ucStopDelayCount[GUN_NO_B] = 5;
            }
            gs_CANComm.fCtrlRectFunction(GUN_NO_B,GRP_WORKMODE_RECTIFIER,RECT_OFF,0,0);
            gChargeCtrl.ucCtrlCmdState[GUN_NO_B]= STOP_CHARGE_VAL;
            gChargeCtrl.ucGBTProtocolType[GUN_NO_B] = BMS_VERSION_IS_GBT2011;
    }
    else if(gChargeCtrl.ucStopDelayCount[GUN_NO_B]==0)
    {
            GUN_B_InitGunAllData();
            gChargeCtrl.ucStopDelayCount[GUN_NO_B]= 0xFF;
    }
    gBmsCommDataInfo[GUN_NO_B].ucMachineState = BMSMachineIfo[GUN_NO_B][gBmsCommDataInfo[GUN_NO_B].ucMachineState].fproc();
	
}
static void B_TwoGunIndentifyFrameProcess(void)
{
	UCHAR ucRTN;
	union
	{
		UCHAR uctemp[4];
		UINT32 u32temp;
	}temp;
	if(gChargeCtrl.ucParallelMode[GUN_NO_B] == TwoGun_Mode)
	{
		ucRTN =CAN2_Read_AllBuf(TWO_GUN_IDENTIFY,PRIORITY_0);
		if(ucRTN == TRUE)
		{
			if(gBmsCommDataInfo[GUN_NO_A].u8Send_Flag[SEND_CMD_TWO_GUN]  == CAN_DATA_STOP)
			{
				_WAIT_SEND_CMD(GUN_NO_A,SEND_CMD_TWO_GUN);
			}
			if((gBmsCommDataInfo[GUN_NO_A].u8Send_Flag[SEND_CMD_TWO_GUN] == CAN_DATA_WAIT)
				||(gBmsCommDataInfo[GUN_NO_A].u8Send_Flag[SEND_CMD_TWO_GUN] == CAN_DATA_SEND))
			{
				gBmsCommDataInfo[GUN_NO_A].CMDMsg[SEND_CMD_TWO_GUN].Id = CanRxMsgBuff[GUN_NO_B].ExtId; 
				temp.uctemp[0] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[3];
				temp.uctemp[1] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[2];
				temp.uctemp[2] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[1];
				temp.uctemp[3] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[0];
				gBmsCommDataInfo[GUN_NO_A].CMDMsg[SEND_CMD_TWO_GUN].Data[0] = temp.u32temp;
				temp.uctemp[0] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[7];
				temp.uctemp[1] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[6];
				temp.uctemp[2] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[5];
				temp.uctemp[3] =(UCHAR)CanRxMsgBuff[GUN_NO_B].Data[4];
				gBmsCommDataInfo[GUN_NO_A].CMDMsg[SEND_CMD_TWO_GUN].Data[1] = temp.u32temp;
				_START_SEND_CMD(GUN_NO_A,SEND_CMD_TWO_GUN);;
			}
		}
	}
	else
	{
		gBmsCommDataInfo[GUN_NO_A].u8Send_Flag[SEND_CMD_TWO_GUN]  = CAN_DATA_STOP;
	}
}
static void InitBMS_2Machine(void)
{
	gBmsCommDataInfo[GUN_NO_B].ucMachineState = TEST_HANDSHAKE_CHECK_CHM;

	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][0],
		TEST_HANDSHAKE_CHECK_CHM, 
		CAN2_HandShake_Check_CHM);	
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][1],
		TEST_HANDSHAKE_CHECK_CRM, 
		CAN2_HandShake_Check_CRM);
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][2],
		TEST_CONFIG_CHECK_CML_CTS, 
		CAN2_Config_Check_CML_CTS);
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][3],
		TEST_CONFIG_CHECK_CRO_AA, 
		CAN2_CONFIG_CHECK_CRO_AA);
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][4],
		TEST_CHARGE_CHECK_CCS, 
		CAN2_CHARGE_CHECK_CCS);
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][5],
		TEST_STOP_CHECK_CST, 
		CAN2_STOP_CHECK_CST);
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][6],
		TEST_STOP_CHECK_CSD, 
		CAN2_STOP_CHECK_CSD);
	INT_STATE_MACHINE(&BMSMachineIfo[GUN_NO_B][7],
		TEST_TIMEOUT_BEM, 
		CAN2_TIMEOUT_BEM);


}
static void InitBms_2CommInfo(void)
{

	UCHAR i;
	for(i =0;i<ALL_SEND_CMD_STATE;i++)
	{
		gBmsCommDataInfo[GUN_NO_B].u8Send_Flag[i] = CAN_DATA_STOP;
	}
	for(i=0;i<ALL_GET_CMD_COUNT_ID;i++)
	{
		gBmsCommDataInfo[GUN_NO_B].u16GetCmdCount[i] = CAN_GET_CMD_COUNT_STOP;
	}
	gBmsCommDataInfo[GUN_NO_B].u8GBT_BMS_ProtocolType = BMS_VERSION_IS_GBT2015;
	gBmsCommDataInfo[GUN_NO_B].ucGetCmdTimeOutFlag = ALL_GET_CMD_COUNT_ID;
	gBmsCommDataInfo[GUN_NO_B].u8GetCSTFlag = FALSE;
	gChargeCtrl.fEVOutVolt[GUN_NO_B] = 0;
	gChargeCtrl.fEVOutCurr[GUN_NO_B] = 0;
	
	gBmsCommDataInfo[GUN_NO_B].fBCP_MaxPermitCellVolt = 4.8;
	gBmsCommDataInfo[GUN_NO_B].fBCP_MaxPermitCurr = 250;
	gBmsCommDataInfo[GUN_NO_B].fBCP_MaxVolt =750;
	gBmsCommDataInfo[GUN_NO_B].fBCP_MaxPermitTemp = 50;
	gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartSOC = 20;
	gBmsCommDataInfo[GUN_NO_B].fBCP_BattStartVolt =350;

	gBmsCommDataInfo[GUN_NO_B].fBCS_MaxCellVolt = 4;
	gBmsCommDataInfo[GUN_NO_B].fBSM_MaxCellTemp = 35;	
	gChargeCtrl.fUserSetVolt[GUN_NO_B] = 500;
	gChargeCtrl.fBMSRequireCurr[GUN_NO_B] = 10;
	gBmsCommDataInfo[GUN_NO_B].fBattSOCIncrease = 0;
    gChargeCtrl.ucStopDelayCount[GUN_NO_B]= 0xFF;

}
static UCHAR Set_CAN2_Para(UCHAR ProtocolType)
{
	static UCHAR ucProtocolTypeBack = 0xFF;
	if (ProtocolType  != ucProtocolTypeBack)
	{
		if(ProtocolType  == PROTOCOL_TYPE_CHADEMO)
		{
			BSP_Can_2_Init(500,CAN_STAND);
		}
		else 
		{
			BSP_Can_2_Init(250,CAN_EXTEND);  
		}
        ucProtocolTypeBack  = ProtocolType;
		return TRUE;
	}
	return FALSE;
}
static void GUN_B_SourceCtrlProcess(void)
{
	UCHAR ucRectOutputMode = GRP_WORKMODE_INVERTER;;
	SetIMEUDOState(IMEU_1_ID,OUTSIDE_GUN2_GREEN_CTRL,TRUE);
	gBmsCommDataInfo[GUN_NO_B].ucACSwitchCtrlState = TRUE;
	if(gChargeCtrl.ucRectWorkMode[GUN_NO_B] == 0)
	{
		ucRectOutputMode = GRP_WORKMODE_INVERTER;
	}
	else
	{
		ucRectOutputMode = GRP_WORKMODE_RECTIFIER;
	}
	if(gChargeCtrl.ucSourceCtrlCmd[GUN_NO_B] == TRUE)
	{	
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_ON,gChargeCtrl.fBMSRequireVolt[GUN_NO_B],gChargeCtrl.fBMSRequireCurr[GUN_NO_B]);
	}
	else
	{
		gs_CANComm.fCtrlRectFunction(GUN_NO_B,ucRectOutputMode,RECT_OFF,0,0);
	}
}
void TASK_CAN2Proc(void* pdata)
{
    UCHAR uctemp,ucRTN;
	UINT16 u16temp;
    gChargeCtrl.ucAgingRecordState[GUN_NO_B] = 0;
	BSP_Can_2_Init(250,CAN_EXTEND);
	BSP_Can2_RxEnable();
	BSP_CanCheck(1);
    InitBMS_2Machine();
    InitCHAdeMO_BMS_Machine();
	InitBms_2CommInfo();
	for(;;)
	{
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
		OSTimeDly(5);
		ucRTN = CAN2_Read_AllBuf(XXX_CHM,PRIORITY_6);
		if(ucRTN == TRUE)
		{
			u16temp = CanRxMsgBuff[GUN_NO_B].Data[0];
			u16temp += CanRxMsgBuff[GUN_NO_B].Data[1]*256;
			CAN2_RefreshBHMFrameData();//更新BHM数据
			_START_SEND_CMD(GUN_NO_B,SEND_CMD_BHM_STATE);
		}
	}
}
	
	
	
