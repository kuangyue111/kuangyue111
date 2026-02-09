#include "main.h"
#include "can_ring.h"
#include "Uart6_new.h"

CANCommData_TypeDef gs_CANComm;
canmsg TxCAN_3Buffer;
UINT8 RxCAN3Buff(UCHAR u8Num, CanRxMsgTypeDef* pRcvMsg);

UINT32 u32Mask_1 = 0x0380FF00;
UINT32 u32Mask_2 = 0x0380E000;
UINT32 u32RcvID_1 = 0x0280F000;
UINT32 u32RcvID_2 = 0x03001010;
UCHAR	DSTADDR = 0;

#define SAMP_3_TIMES		3
#define SAMP_ALWAYS		    255

#define R_PRPTO_DEVNO_0A	0x0A  //监控与单个模块通信
#define R_PRPTO_DEVNO_0B	0x0B  //监控与组模块通信

#define R_PROTO_BROADCAST		0x3F
#define R_PROTO_SELF_ADDR		gs_CANComm.bySelfAddress

//模块回复的错误码定义
typedef enum tagRECT_RET_ERR_Code
{
	RECT_RET_ERR_Code_NO_ERR = 0x00,
	RECT_RET_ERR_Code_WRONG_ADDR = 0x01,
	RECT_RET_ERR_Code_WRONG_CMD = 0x02,
	RECT_RET_ERR_Code_WRONG_DATA = 0x03,
	RECT_RET_ERR_Code_IN_STARTING = 0x07, //地址重排未完成/**/
}RECT_RET_ERR_Code;


NOINIT UINT8 u8Can3_S_Buff[CAN_FRAME_SIZE];
CanRxMsgTypeDef RxMsg[CAN_3_MAX_BUFF];

static float s_fGrpRateCurrBuf;
static float s_fGrpRatePowerBuf;

void CAN3_GetRectCtlVoltCurr(UCHAR  i, float* fVal1, float* fVal2);
UINT8 CAN3_GetRectCtlStatus(UCHAR i);
void CAN3_SetRectCtlVoltCurr(UCHAR i, float fVal1, float fVal2);
UCHAR CAN3_MutexInit(void);
void CAN3_SetRectCtlStatus(UCHAR i,UINT8 u8OnOff);
void CAN3_InitRectData(UCHAR isFirstFlag);

void NoSampDeal(void);
float String2Float(UINT8* pData);
int CAN3_Send(canmsg *pMsg, UINT8 DataL);
void Init_CmdInfos(void);
static void ClenRBuff(void);





static void RefreshCAN3Time(void)
{
	struct tm  tmNow;
	static UCHAR  Sec = 0;
	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)/*1秒钟*/
	{
		Sec = (UCHAR)tmNow.tm_sec;
		gs_CANComm.u16AllCommFailResetDelay++;
	}
}


void CAN3_InitRectData(UCHAR isFirstFlag)
{
	UINT8 u8Idx = 0;
	//gEvChargeInfo.fMaxOutCurr = 240;
	//gEvChargeInfo.fMaxOutVolt = 750;

	s_fGrpRateCurrBuf = 0;
	s_fGrpRatePowerBuf = 0;

	//LKF 只初始化时执行一次，以后重配置时不能再执行
	if(isFirstFlag)
	{
		gs_CANComm.u8IsACOver = FALSE;
		gs_CANComm.u8IsACUnder = FALSE;
		gs_CANComm.u8IsAllCommFail = FALSE;
		gs_CANComm.u16AllCommFailResetDelay = 0;
		gs_CANComm.u16RectCommPeriodResetDelay = 0;
		gs_CANComm.u8GroupRectOnOff[GRP_A] = RECT_OFF;
		gs_CANComm.u8GroupRectOnOff[GRP_B] = RECT_OFF;

		gs_CANComm.fRect_GRP_NeedOutVolt[GRP_A] = 100;
		gs_CANComm.fRect_GRP_NeedOutCurr[GRP_A] = 1;
		gs_CANComm.fRect_GRP_NeedOutVolt[GRP_B] = 100;
		gs_CANComm.fRect_GRP_NeedOutCurr[GRP_B] = 1;
	}

	for (u8Idx= 0; u8Idx < mRECTNUM; u8Idx++)
	{		
		gs_CANComm.AddrInfo[u8Idx] = u8Idx;
		gs_CANComm.iCount[u8Idx] = 0;
		gs_CANComm.RectData[u8Idx].bIsExisted = FALSE;
		gs_CANComm.RectData[u8Idx].nBelongedGrp = 0xff;
		gs_CANComm.RectData[u8Idx].bCommFail = TRUE;
		gs_CANComm.RectData[u8Idx].nCommFailTimes = 0;
		gs_CANComm.RectData[u8Idx].fRealTime_Volt = 0;
		gs_CANComm.RectData[u8Idx].fRealTime_Curr = 0;
		gs_CANComm.RectData[u8Idx].fRealTime_Temp = 0;
		gs_CANComm.RectData[u8Idx].fDC_Ver = 0;
		gs_CANComm.RectData[u8Idx].fAC_Ver = 0;
		gs_CANComm.RectData[u8Idx].fHW_Ver = 0;
		gs_CANComm.RectData[u8Idx].fMax_OutVolt = 0;
		gs_CANComm.RectData[u8Idx].fMin_OutVolt = 0;
		gs_CANComm.RectData[u8Idx].fMax_OutCurr= 0;
		gs_CANComm.RectData[u8Idx].fRatedPower= 0;
		gs_CANComm.RectData[u8Idx].bAC_Off = 0;
		gs_CANComm.RectData[u8Idx].bAC_OverVolt = 0;
		gs_CANComm.RectData[u8Idx].bDC_LowVolt = 0;
		gs_CANComm.RectData[u8Idx].bAC_NoBalance = 0;
		gs_CANComm.RectData[u8Idx].bAC_PhaseLack = 0;
		gs_CANComm.RectData[u8Idx].bCurr_NoBalace = 0;
		gs_CANComm.RectData[u8Idx].bID_Conflict = 0;
		gs_CANComm.RectData[u8Idx].bPowerLimited = 0;
		//gs_CANComm.RectData[u8Idx].bRectCommFail = 0;
		gs_CANComm.RectData[u8Idx].bWalkIn_Enabled = 1;
		gs_CANComm.RectData[u8Idx].bDC_OverVolt = 0;
		gs_CANComm.RectData[u8Idx].bOverTemp = 0;
		gs_CANComm.RectData[u8Idx].bFanFault = 0;
		gs_CANComm.RectData[u8Idx].bProtected = 0;
		gs_CANComm.RectData[u8Idx].bRectFail = 0;
		gs_CANComm.RectData[u8Idx].bDC_Off = 0;
		gs_CANComm.RectData[u8Idx].bDisCh_Err = 0;
		gs_CANComm.RectData[u8Idx].bOn_Sleep = 0;
		gs_CANComm.RectData[u8Idx].bPFC_Err = 0;
		gs_CANComm.RectData[u8Idx].bInternalCommErr = 0;
		gs_CANComm.RectData[u8Idx].bInternalDC_LowVolt = 0;
		gs_CANComm.RectData[u8Idx].bDC_Shorted = 0;
		gs_CANComm.RectData[u8Idx].nCommFailTimes = 0;
		gs_CANComm.RectData[u8Idx].fAB_Volt = 411;
		gs_CANComm.RectData[u8Idx].fBC_Volt = 412;
		gs_CANComm.RectData[u8Idx].fCA_Volt = 413;
		
		gs_CANComm.RectData[u8Idx].bStatCode[0] = 0;
		gs_CANComm.RectData[u8Idx].bStatCode[1] = 0;
		gs_CANComm.RectData[u8Idx].bStatCode[2] = 0;
	}

	for (u8Idx= 0; u8Idx < _MAX_GRP_NUM; u8Idx++)
	{
		gs_CANComm.GrpData[u8Idx].nCommRectNum = 0;
		gs_CANComm.GrpData[u8Idx].bAllRect_Off = TRUE;
		gs_CANComm.GrpData[u8Idx].fOutVolt = 0;
		gs_CANComm.GrpData[u8Idx].fOutCurr = 0;
		gs_CANComm.GrpData[u8Idx].bAllRectProtected = 0;
		gs_CANComm.GrpData[u8Idx].bAllRectFailed = 0;
		gs_CANComm.GrpData[u8Idx].bAllRect_Off = 0;		//0 系统关机
		gs_CANComm.GrpData[u8Idx].fMax_OutVolt = 770;
		gs_CANComm.GrpData[u8Idx].fMin_OutVolt = 100;
		gs_CANComm.GrpData[u8Idx].fMax_OutCurr = 240;
		gs_CANComm.GrpData[u8Idx].fRatedMaxCurr = 0;
		gs_CANComm.GrpData[u8Idx].fMax_OutCurr_ring = 240;
		gs_CANComm.GrpData[u8Idx].fRatedPower = 0;
		gs_CANComm.GrpData[u8Idx].bIsAnyRectAlarm = 0;

		//LKF 只初始化时执行一次，以后重配置时不能再执行
		if(isFirstFlag)
		{
			gs_CANComm.GrpData[u8Idx].u8IsNoNormalRect = FALSE;
		}
	}

	/*环网*/
	gs_CANComm.nScanedRectNum = 0; //The rect num after Reconfigure done
	gs_CANComm.nCommFailedNum = 0;
	//LKF 只初始化时执行一次，以后重配置时不能再执行
	if(isFirstFlag)
	{
		gs_CANComm.bySelfAddress = 0xF0; //本桩地址/**/
		gs_CANComm.bNeedReConfig = TRUE;
	}
	
	gs_CANComm.nOnlineTotalGrp = 0; //實際在線的各種模塊的組數,必须为0,1,2/**/
	for (u8Idx= 0; u8Idx < _MAX_GRP_NUM; u8Idx++)
	{		
		gs_CANComm.bIsGrpExist[u8Idx] = 0;//该组存在状态/**/
		gs_CANComm.nNotAccessbleCount[u8Idx] = 0;
		gs_CANComm.bIsGrp_NotAvailable[u8Idx] = 0;

		//LKF 只初始化时执行一次，以后重配置时不能再执行
		if(isFirstFlag)
		{
			gs_CANComm.byGrpIdx[u8Idx] = u8Idx+1;//真实的组号，根据监控地址固定写死1~8/**/
			gs_CANComm.bIsInitClosed[u8Idx] = 0; //是否初始化关闭了该组模块/**/  
		}		
	}	
	/*环网 end*/
	
}
void CAN3_GetRectCtlVoltCurr(UCHAR  i, float* fVal1, float* fVal2)
{
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
		if (GRP_A == i)
		{
			*fVal1 = gs_CANComm.fRect_GRP_NeedOutVolt[GRP_A];
			*fVal2 = gs_CANComm.fRect_GRP_NeedOutCurr[GRP_A];
		}
		else if (GRP_B == i)
		{
			*fVal1 = gs_CANComm.fRect_GRP_NeedOutVolt[GRP_B];
			*fVal2 = gs_CANComm.fRect_GRP_NeedOutCurr[GRP_B];
		}
	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
	return ;
}

UINT8 CAN3_GetRectCtlStatus(UCHAR i)
{
	UCHAR ucRTN = 0xFF;
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
	if (GRP_A == i)
	{
		ucRTN = gs_CANComm.u8GroupRectOnOff[GRP_A];
	}
	else if (GRP_B == i)
	{
		ucRTN = gs_CANComm.u8GroupRectOnOff[GRP_B];
	}
	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
	return ucRTN ;
}
UINT8 CAN3_GetRectCtlMode(UCHAR i)
{
	UCHAR ucRTN = 0xFF;
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
	if (GRP_A == i)
	{
		ucRTN = gs_CANComm.u8GRP_WorkModeSet[GRP_A];
		
		
	}
	else if (GRP_B == i)
	{
		ucRTN = gs_CANComm.u8GRP_WorkModeSet[GRP_B];
	}
	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
	return ucRTN ;
}

void CAN3_SetRectCtlVoltCurr(UCHAR i, float fVal1, float fVal2)
{
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
	if (GRP_A == i)
	{
		gs_CANComm.fRect_GRP_NeedOutVolt[GRP_A] = fVal1;
		gs_CANComm.fRect_GRP_NeedOutCurr[GRP_A] = fVal2;
	}
	else if (GRP_B == i)
	{
		gs_CANComm.fRect_GRP_NeedOutVolt[GRP_B] = fVal1;
		gs_CANComm.fRect_GRP_NeedOutCurr[GRP_B] = fVal2;
	}
	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
}
static void CtrlRectFunction(UCHAR ucGunNo,UCHAR ucRectMode,UCHAR ucRectOn,float fVolt,float fCurr)
{
	UCHAR ucOtherGunNo,ucNumber = 1;
	float ftemp;
	if(ucGunNo == GUN_NO_A)
	{
		ucOtherGunNo = GUN_NO_B;
	}
	else
	{
		ucOtherGunNo = GUN_NO_A;
	}
	if(gChargeCtrl.ucParallelMode[ucOtherGunNo] >=Parallel_Mode) //如果已经被并机，不允许控制模块
	{
		return;
	}
	if(gChargeCtrl.ucParallelMode[ucGunNo] >=Parallel_Mode)//并机模式可以控制别的模块
	{
		ucNumber = 2;
		ftemp = fCurr/ucNumber;
		gs_CANComm.fSetRectCtlMode(ucOtherGunNo,ucRectMode);
		gs_CANComm.fSetRectCtlVoltCurr(ucOtherGunNo,fVolt,ftemp);
	}
	ftemp = fCurr/ucNumber;
	gs_CANComm.fSetRectCtlMode(ucGunNo,ucRectMode);
	gs_CANComm.fSetRectCtlVoltCurr(ucGunNo,fVolt,ftemp);

	gs_CANComm.fSetRectCtlStatus(ucGunNo,ucRectOn);
	if(gChargeCtrl.ucParallelMode[ucGunNo] >=Parallel_Mode)//并机模式可以控制别的模块
	{
		gs_CANComm.fSetRectCtlStatus(ucOtherGunNo,ucRectOn);
	}

}

UCHAR CAN3_MutexInit(void)
{
	gs_CANComm.hMutexReadWriteCANCommData = MutexCreate(UPPER_PRIO_READ_WRITE_CANCOMM_DATA);
	if(gs_CANComm.hMutexReadWriteCANCommData == NULL)
	{
		return FALSE;
	}
	return  TRUE;
}

void CAN3_SetRectCtlStatus(UCHAR i,UINT8 u8OnOff)
{
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
	if (GRP_A == i)
	{
		gs_CANComm.u8GroupRectOnOff[GRP_A] = u8OnOff;
	}
	else if (GRP_B == i)
	{
		gs_CANComm.u8GroupRectOnOff[GRP_B] = u8OnOff;
	}
	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
}
void CAN3_SetRectCtlMode(UCHAR i,UINT8 u8Mode)
{
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
	if (GRP_A == i)
	{
		gs_CANComm.u8GRP_WorkModeSet[GRP_A] = u8Mode;
	}
	else if (GRP_B == i)
	{
		gs_CANComm.u8GRP_WorkModeSet[GRP_B] = u8Mode;
	}
	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
}

static UCHAR u8GetRectDigital(UCHAR mode, UCHAR i)
{
	UCHAR  ctemp = 0;

	if(i > mRECTNUM - 1)
	{
		return  0;
	}
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);


	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);

	return  ctemp;
}

static UCHAR CAN3_get_rect_barcode(UINT16 nID, void *pDataBuf)
{
	//UCHAR i;

	if(nID > mRECTNUM - 1)
	{
		return  FALSE;
	}
	else
	{
		return  TRUE;
	}
}

static UCHAR GetRectLocation(UCHAR uclocation)
{
      return 0;
}

static UCHAR CAN3_GetRectID(UCHAR ucShelfNo)
{
	UCHAR ctemp = mDISABLE;
	//GetCharSetData(SETDATA_RectACPhaseEnable_NUM, &ctemp);
	if(ctemp == mDISABLE)
	{										
		return ucShelfNo;
	}
	else
	{
		ctemp = GetRectLocation(ucShelfNo);
		if(ctemp == mRECTNUM)
			return ucShelfNo;
		else
			return ctemp;
	}
}
static float CAN3_GetRectAnalog(UCHAR uGrpNo, UCHAR RectId, UCHAR SigID)
{
	MutexLock(gs_CANComm.hMutexReadWriteCANCommData);
	if (GRP_A == uGrpNo || GRP_B == uGrpNo)
	{
		CANCommData_TypeDef* PData = (CANCommData_TypeDef*)(&gs_CANComm);
		switch (SigID)
		{
		case ID_RECT_OUT_VOLT:
			MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
			return PData->GrpData[uGrpNo].fOutVolt;
			break;
		case ID_RECT_OUT_CURR:
			MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
			return PData->GrpData[uGrpNo].fOutCurr;
			break;
		default:
			break;
		}
	}
	else//RECT
	{

	}

	MutexUnlock(gs_CANComm.hMutexReadWriteCANCommData);
	return 0;
}

UINT8 RxCAN3Buff(UCHAR u8Num, CanRxMsgTypeDef* pRcvMsg)
{
	UINT16 uFrameNum = 0;
	u8Num = MIN(u8Num,CAN_3_MAX_BUFF);
	uFrameNum = MCP2515_Read(pRcvMsg, u8Num);
	return uFrameNum;
}

int CAN3_Send(canmsg *pMsg, UINT8 DataL)
{
	return MCP2515_Send_2(pMsg, DataL);
}

static void ClenRBuff(void)
{
	for (UINT8 u8Idx = 0; u8Idx < CAN_3_MAX_BUFF; u8Idx++)
	{
		RxMsg[u8Idx].ExtId = 0;
	}
}
static void u8PackSendMsg(UINT8 u8DevNo,UINT8 u8Cmd, UINT8 u8GSNo,UINT32 Dh,UINT32 Dl)
{
	UINT8 u8Rst = FALSE;
	UNUSED(u8Rst);
	canmsg SendMsg;
	if (u8GSNo >= mRECTNUM && R_PRPTO_DEVNO_0A == u8DevNo)
	{
		return ;
	}
	if (u8GSNo >= _MAX_GRP_NUM && R_PRPTO_DEVNO_0B == u8DevNo)
	{
		return ;
	}
	if (!(R_PRPTO_DEVNO_0A == u8DevNo || R_PRPTO_DEVNO_0B == u8DevNo))
	{
		return ;
	}
		
	if (u8DevNo == R_PRPTO_DEVNO_0A)
	{
		SendMsg.Id = (R_PRPTO_DEVNO_0A << 22) + (u8Cmd << 16) + ((gs_CANComm.AddrInfo[u8GSNo]) << 8) + R_PROTO_SELF_ADDR;			//模块地址 0 1 2 3
	}
	else
	{
		SendMsg.Id = (R_PRPTO_DEVNO_0B << 22) + (u8Cmd << 16) + (gs_CANComm.byGrpIdx[u8GSNo] << 8) + R_PROTO_SELF_ADDR;			//组 1 2 
	}
	SendMsg.Data[0] = Dh;
	SendMsg.Data[1] = Dl;
	CAN3_Send(&SendMsg, 8);
}

static void UnPackData(UINT8 u8RcvMsgNum)
{
	UINT8 u8RcvSrcAddr,u8RcvCmd, u8RcvDevNo,u8RcvErrCode, u8RcvDstAddr, u8GSNo;
	UINT8 u8Idx = 0;
	float  fMax_OutCurr, fRatedPower, ftemp;
	CANCommData_TypeDef* PData;
	UINT8 u8Buf[8];
	PData = &gs_CANComm;
	if(u8RcvMsgNum > CAN_3_MAX_BUFF)
	{
		return ;
	}

	for (u8Idx = 0; u8Idx < u8RcvMsgNum; u8Idx++)
	{
		u8RcvErrCode = ((RxMsg[u8Idx].ExtId>>26)&0x07);	
		u8RcvCmd = ((RxMsg[u8Idx].ExtId>>16)&0x3f);	
		u8RcvDevNo = ((RxMsg[u8Idx].ExtId>>22)&0x0f);
		u8RcvDstAddr = ((RxMsg[u8Idx].ExtId>>8)&0xff);	
		u8RcvSrcAddr = (RxMsg[u8Idx].ExtId & 0xff);
		//地址重排
		if(u8RcvErrCode == RECT_RET_ERR_Code_IN_STARTING)
		{
			gs_CANComm.bNeedReConfig = TRUE;
			return ;
		}

		if (u8RcvErrCode != RECT_RET_ERR_Code_NO_ERR) 
		{
			continue;
		}

		if (u8RcvDstAddr == R_PROTO_SELF_ADDR)
		{
			u8Buf[0] = RxMsg[u8Idx].Data[0]; u8Buf[1] = RxMsg[u8Idx].Data[1];
			u8Buf[2] = RxMsg[u8Idx].Data[2]; u8Buf[3] = RxMsg[u8Idx].Data[3];
			u8Buf[4] = RxMsg[u8Idx].Data[4]; u8Buf[5] = RxMsg[u8Idx].Data[5];
			u8Buf[6] = RxMsg[u8Idx].Data[6]; u8Buf[7] = RxMsg[u8Idx].Data[7];
			
			if (R_PRPTO_DEVNO_0B == u8RcvDevNo)   //组命令
			{
				if(u8RcvSrcAddr == 0x01)
				{
					u8GSNo = GRP_A;
				}
				else if(u8RcvSrcAddr == 0x02)
				{
					u8GSNo = GRP_B;
				}
				else
				{
					continue;
				}
				switch(u8RcvCmd)
				{
				case 0x01:
					PData->GrpData[u8GSNo].fOutVolt = String2Float(u8Buf);					//1或2组 系统电压
					PData->GrpData[u8GSNo].fOutCurr = String2Float(u8Buf + 4);				//1或2组 系统电压
					break;
				case 0x02:
					PData->GrpData[u8GSNo].nCommRectNum = (int)RxMsg[u8Idx].Data[2];
					PData->GrpData[u8GSNo].bAllRectProtected = (RxMsg[u8Idx].Data[1] >> 2)&0x01;
					PData->GrpData[u8GSNo].bAllRectFailed = (RxMsg[u8Idx].Data[1] >> 1)&0x01;
					PData->GrpData[u8GSNo].bAllRect_Off = RxMsg[u8Idx].Data[1] & 0x01;
					PData->bIsGrpExist[u8GSNo] = TRUE;
					break;
				case 0x13:
					break;
				case 0x1A://广播
					break;
				case 0x1B://调节电压电流
					break;
				case 0x0A:					
					ftemp = (float)(RxMsg[u8Idx].Data[0] << 8) + RxMsg[u8Idx].Data[1];
					PData->GrpData[u8GSNo].fMax_OutVolt = (ftemp >= 50.0) ? ftemp : 1000.0;

					ftemp = (float)(RxMsg[u8Idx].Data[2] << 8) + RxMsg[u8Idx].Data[3];
					PData->GrpData[u8GSNo].fMin_OutVolt = MAX(50.0, ftemp);
	
					fMax_OutCurr = (float)(RxMsg[u8Idx].Data[4] << 8) + RxMsg[u8Idx].Data[5];
					fMax_OutCurr *= 0.1;

					//增加额定功率/**/
					fRatedPower = (float)(RxMsg[u8Idx].Data[6] << 8) + RxMsg[u8Idx].Data[7];
					fRatedPower *= 0.01;

					if((fMax_OutCurr < 10.0) || (fMax_OutCurr > 300.0))
					{
						fRatedPower = 15.0;
						fMax_OutCurr = 30.0;
					}
					else if(fRatedPower < 10.0)
					{
						fRatedPower = (fMax_OutCurr > 31.0) ? 20.0 : 15.0;
					}

					//20190618 单模块的功率，最大电流
					if(u8RcvSrcAddr < mRECTNUM)
					{
						gs_CANComm.RectData[u8RcvSrcAddr].fMax_OutCurr = fMax_OutCurr;
						gs_CANComm.RectData[u8RcvSrcAddr].fRatedPower = fRatedPower;
					}

					s_fGrpRateCurrBuf += fMax_OutCurr;
					s_fGrpRatePowerBuf += fRatedPower;

					//PData->GrpData[u8GSNo].fRatedPower = fRatedPower * PData->GrpData[u8GSNo].nCommRectNum;//模块是单个回复的/**/
					//PData->GrpData[u8GSNo].fMax_OutCurr = fMax_OutCurr * PData->GrpData[u8GSNo].nCommRectNum;//模块是单个回复的/**/
					//PData->GrpData[u8GSNo].fRatedMaxCurr = PData->GrpData[u8GSNo].fMax_OutCurr;						
					break;
				default:
					break;
				}
			}
			else  //单个模块命令
			{		
				if(u8RcvSrcAddr<mRECTNUM)
				{
					u8GSNo = u8RcvSrcAddr;
				
					PData->RectData[u8GSNo].bCommFail = FALSE;
					PData->RectData[u8GSNo].nCommFailTimes = 0;

					switch(u8RcvCmd)
					{
					case 0x03:
						PData->RectData[u8GSNo].fRealTime_Volt = String2Float(u8Buf);
						PData->RectData[u8GSNo].fRealTime_Curr = String2Float(u8Buf + 4);
						break;

					case 0x04:
						PData->RectData[u8GSNo].nBelongedGrp = (int)RxMsg[u8Idx].Data[2];
						PData->RectData[u8GSNo].fRealTime_Temp = (float)RxMsg[u8Idx].Data[4];
						//以下解析状态字/**/
						PData->RectData[u8GSNo].bStatCode[2] = RxMsg[u8Idx].Data[5];
						PData->RectData[u8GSNo].bAC_Off = (RxMsg[u8Idx].Data[5] >> 7) & 0x01;
						PData->RectData[u8GSNo].bAC_OverVolt = (RxMsg[u8Idx].Data[5] >> 6) & 0x01;
						PData->RectData[u8GSNo].bDC_LowVolt = (RxMsg[u8Idx].Data[5] >> 5) & 0x01;
						PData->RectData[u8GSNo].bAC_NoBalance = (RxMsg[u8Idx].Data[5] >> 4) & 0x01;
						PData->RectData[u8GSNo].bAC_PhaseLack = (RxMsg[u8Idx].Data[5] >> 3) & 0x01;
						PData->RectData[u8GSNo].bCurr_NoBalace = (RxMsg[u8Idx].Data[5] >> 2) & 0x01;
						PData->RectData[u8GSNo].bID_Conflict = (RxMsg[u8Idx].Data[5] >> 1) & 0x01;
						PData->RectData[u8GSNo].bPowerLimited = (RxMsg[u8Idx].Data[5]) & 0x01;

						PData->RectData[u8GSNo].bStatCode[1] = RxMsg[u8Idx].Data[6];
						//pRect_Rough->pRawRects[nIdx].bRectCommFail = (pRect_Rough->frmReply.byData[6] >> 7) & 0x01;
						PData->RectData[u8GSNo].bWalkIn_Enabled = (RxMsg[u8Idx].Data[6] >> 6) & 0x01;
						PData->RectData[u8GSNo].bDC_OverVolt = (RxMsg[u8Idx].Data[6] >> 5) & 0x01;
						PData->RectData[u8GSNo].bOverTemp = (RxMsg[u8Idx].Data[6] >> 4) & 0x01;
						PData->RectData[u8GSNo].bFanFault = (RxMsg[u8Idx].Data[6] >> 3) & 0x01;
						PData->RectData[u8GSNo].bProtected = (RxMsg[u8Idx].Data[6] >> 2) & 0x01;
						PData->RectData[u8GSNo].bRectFail = (RxMsg[u8Idx].Data[6] >> 1) & 0x01;
						PData->RectData[u8GSNo].bDC_Off = (RxMsg[u8Idx].Data[6]) & 0x01;

						PData->RectData[u8GSNo].bStatCode[0] = RxMsg[u8Idx].Data[7];
						PData->RectData[u8GSNo].bDisCh_Err = (RxMsg[u8Idx].Data[7] >> 5) & 0x01;
						PData->RectData[u8GSNo].bOn_Sleep = (RxMsg[u8Idx].Data[7] >> 4) & 0x01;
						PData->RectData[u8GSNo].bPFC_Err = (RxMsg[u8Idx].Data[7] >> 3) & 0x01;
						PData->RectData[u8GSNo].bInternalCommErr = (RxMsg[u8Idx].Data[7] >> 2) & 0x01;
						PData->RectData[u8GSNo].bInternalDC_LowVolt = (RxMsg[u8Idx].Data[7] >> 1) & 0x01;
						PData->RectData[u8GSNo].bDC_Shorted = (RxMsg[u8Idx].Data[7]) & 0x01;
						break;
					case 0x06:
						PData->RectData[u8GSNo].fAB_Volt = 0.1*((float)(RxMsg[u8Idx].Data[0] << 8) + RxMsg[u8Idx].Data[1]);
						PData->RectData[u8GSNo].fBC_Volt = 0.1*((float)(RxMsg[u8Idx].Data[2] << 8) + RxMsg[u8Idx].Data[3]);
						PData->RectData[u8GSNo].fCA_Volt = 0.1*((float)(RxMsg[u8Idx].Data[4] << 8) + RxMsg[u8Idx].Data[5]);
						break;
					case 0x07:
						PData->RectData[u8GSNo].fDC_Ver  = (float)(RxMsg[u8Idx].Data[0]) + (float)(RxMsg[u8Idx].Data[1] >> 4) * 0.1 + (float)(RxMsg[u8Idx].Data[1] & 0x0F) * 0.01;
						PData->RectData[u8GSNo].fAC_Ver  = (float)(RxMsg[u8Idx].Data[2]) + (float)(RxMsg[u8Idx].Data[3] >> 4) * 0.1 + (float)(RxMsg[u8Idx].Data[3] & 0x0F) * 0.01;
						PData->RectData[u8GSNo].fHW_Ver = (float)(RxMsg[u8Idx].Data[4]) + (float)(RxMsg[u8Idx].Data[5] >> 4) * 0.1 + (float)(RxMsg[u8Idx].Data[5] & 0x0F) * 0.01;
						break;
					case 0x1A:
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

static void GetSingle_VoltCurr(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x03;	 
	u8PackSendMsg(R_PRPTO_DEVNO_0A,u8CMD, SGNo, 0, 0);
}

static void GetSingle_GrpAndStatus(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x04;
	u8PackSendMsg(R_PRPTO_DEVNO_0A,u8CMD, SGNo, 0, 0);
}

static void GetSingle_PhaseVolts(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x06;	 
	u8PackSendMsg(R_PRPTO_DEVNO_0A,u8CMD, SGNo, 0, 0);
}

static void GetSingle_VerInfo(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x07;	 
	u8PackSendMsg(R_PRPTO_DEVNO_0A,u8CMD, SGNo, 0, 0);
}




static void GetGrp_VoltCurr(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x01;
	u8PackSendMsg(R_PRPTO_DEVNO_0B,u8CMD, SGNo, 0, 0);
}

static void GetGrp_NumAndStatus(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x02;
	u8PackSendMsg(R_PRPTO_DEVNO_0B,u8CMD, SGNo, 0, 0);
}
static void SetGrp_WorkMode (int SGNo, void* pRawData,void* pSetData)
{
	UINT32 u32DataVal1 =0x21100000, u32DataVal2 =0x00000000 ;
	UINT8 u8CMD = 0x24;			
	CANCommData_TypeDef* PData = (CANCommData_TypeDef*)pRawData;
	if ((GRP_A == SGNo)||(GRP_B == SGNo))
	{
		u32DataVal2 |= PData->u8GRP_WorkModeSet[SGNo];
	}
	else
	{
		return ;
	}
	u8PackSendMsg(R_PRPTO_DEVNO_0B,u8CMD, SGNo, u32DataVal1, u32DataVal2);
	OSTimeDly(5);
    if(PData->u8GRP_WorkModeSet[SGNo] == GRP_WORKMODE_INVERTER)
    {
      u32DataVal1 =0x21110000;
      u32DataVal2 =0x000000A1;
      u8CMD = 0x24;			
      u8PackSendMsg(R_PRPTO_DEVNO_0B,u8CMD, SGNo, u32DataVal1, u32DataVal2);
      OSTimeDly(5);
    }
}
static void SetGrp_OnOff(int SGNo, void* pRawData,void* pSetData)
{
	UINT32 u32Val = 0;
	UINT8 u8CMD = 0x1A;			//1A 控制模块开关！！广播指令无回复！	设备号为A时，3F 广播		设备号为B时,	目标地址为组号
	CANCommData_TypeDef* PData = (CANCommData_TypeDef*)pRawData;
	if (GRP_A == SGNo)
	{
		u32Val = (PData->u8GroupRectOnOff[GRP_A]<<24);
	}
	else if (GRP_B == SGNo)
	{
		u32Val = (PData->u8GroupRectOnOff[GRP_B]<<24);
	}
	else
	{
		return;
	}
	u8PackSendMsg(R_PRPTO_DEVNO_0B,u8CMD, SGNo, u32Val, 0);
}



static void SetGrp_WalkIn(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x13;									//设置walkin
	__XXX_UNION_VALUE untemp;
	__16BITS_UNION_VALUE untemp_16bits;
	UINT32 uiDH,uiDL;

	if (SGNo >= _MAX_GRP_NUM)
	{
		return ;
	}

	untemp.u32Value = 0;
	untemp.chtemp[3] = 1;   //使能
	uiDH = untemp.u32Value;

	untemp_16bits.u16Temp = (UINT16)gEvChargeInfo.fRectLoadStartTime;

	untemp.u32Value = 0;
	untemp.chtemp[1] = untemp_16bits.chtemp[1];
	untemp.chtemp[0] = untemp_16bits.chtemp[0];
	uiDL = untemp.u32Value;
	u8PackSendMsg(R_PRPTO_DEVNO_0B, u8CMD, SGNo, uiDH, uiDL);
}

static void SetGrp_AdjVoltCurr(int SGNo, void* pRawData,void* pSetData)
{
	UINT8 u8CMD = 0x1B;									//设置电压	电流
	CANCommData_TypeDef* PData = (CANCommData_TypeDef*)pRawData;
	UINT32 uiSetVolt,uiSetCurr,u32DataVal1,u32DataVal2;
	if (SGNo >= _MAX_GRP_NUM)
	{
		return ;
	}
	
	if((SGNo == GRP_A)||(SGNo == GRP_B))
	{
		uiSetVolt = (UINT32)(PData->fRect_GRP_NeedOutVolt[SGNo] * 1000);
		uiSetCurr = (UINT32)(PData->fRect_GRP_NeedOutCurr[SGNo] * 1000);
	}
	else
	{
		return ;
	}
	u8PackSendMsg(R_PRPTO_DEVNO_0B, u8CMD, SGNo, uiSetVolt, uiSetCurr);
	OSTimeDly(5);
	if(PData->u8GRP_WorkModeSet[SGNo] == GRP_WORKMODE_INVERTER)
	{
		u32DataVal1 =0x11320000;
		u32DataVal2 =(UINT32)(PData->fRect_GRP_NeedOutVolt[SGNo] * 1000);
		u8CMD = 0x24;			
		u8PackSendMsg(R_PRPTO_DEVNO_0B,u8CMD, SGNo, u32DataVal1, u32DataVal2);
		OSTimeDly(5);
	}
}

float String2Float(UINT8* pData)
{
	__XXX_UNION_VALUE unVal;

	for(UINT8 i = 0; i < 4; i++)
	{
		unVal.chtemp[4 - i - 1] = pData[i];
	}
	return unVal.ftemp;
}




void InitCan3Port(void)
{
	UINT32 u32Mask, u32RcvID;

	MCP2515_init(CAN_125KBPS);

	//Rec Buff[0] / [1] 屏蔽码设置
	u32Mask = 0x0380FF00;
	MCP2515_Set_Recmask(0, u32Mask);
	MCP2515_Set_Recmask(1, u32Mask);

	//Rec Buff[0] 接收 0A/0B设备号 消息, 目的地址需要满足本桩地址
	u32RcvID = 0x02800000 + ((UINT32)gs_CANComm.bySelfAddress<<8);
	MCP2515_Set_RecID(0, u32RcvID);
	MCP2515_Set_RecID(1, u32RcvID);

	//Rec Buff[1] 接收 0C设备号 消息, 目的地址必须等于本桩的组号或广播地址
	u32RcvID = 0x03000000  + ((UINT32)gs_CANComm.byGrpIdx[GRP_A]<<8);
	MCP2515_Set_RecID(2, u32RcvID);
	u32RcvID = 0x03000000  + ((UINT32)gs_CANComm.byGrpIdx[GRP_B]<<8);
	MCP2515_Set_RecID(3, u32RcvID);
	u32RcvID = 0x03000000 + ((UINT32)0x1f<<8);		//
	MCP2515_Set_RecID(4, u32RcvID);
	MCP2515_Set_RecID(5, u32RcvID);
	
	
	OSTimeDly(4);

	
}
//每次调用，AB组各发一个命令
static void SampGInfo(void)
{
	UINT8 iGIdx = 0;
	UINT8 iCmdIdx = 0;

	for (iGIdx = 0; iGIdx < _MAX_GRP_NUM; iGIdx++)
	{
		if((gs_CANComm.u8GroupRectCMDIndex[iGIdx]>=_CAN_R_CMD_ID_G_01)&&(gs_CANComm.u8GroupRectCMDIndex[iGIdx]<_MAX_CAN_R_CMD))
		{
			iCmdIdx = gs_CANComm.u8GroupRectCMDIndex[iGIdx]++;
			if(gs_CANComm.cmdInfos[iCmdIdx].u8SampCount<gs_CANComm.cmdInfos[iCmdIdx].u8SampTimes) 
			{
				gs_CANComm.cmdInfos[iCmdIdx].pfnCmdHandler(iGIdx, &gs_CANComm, &gs_CANComm);
				if(gs_CANComm.cmdInfos[iCmdIdx].u8SampTimes == SAMP_ALWAYS)
				{
					gs_CANComm.cmdInfos[iCmdIdx].u8SampCount = 0; 
				}
				else if(gs_CANComm.cmdInfos[iCmdIdx].u8SampCount <255)
				{
					gs_CANComm.cmdInfos[iCmdIdx].u8SampCount++;
				}
			}
		}
		if(gs_CANComm.u8GroupRectCMDIndex[iGIdx]>=_MAX_CAN_R_CMD)
		{
			gs_CANComm.u8GroupRectCMDIndex[iGIdx]=_CAN_R_CMD_ID_G_01;
		}
	}
}

static void SampSInfo(void)
{
	UINT8 iCmdIdx = 0;
	static UINT8 su8LastRectNum = 0;
	UINT8 u8RectNum = gs_CANComm.GrpData[GRP_A].nCommRectNum + gs_CANComm.GrpData[GRP_B].nCommRectNum;
	if (u8RectNum >= su8LastRectNum)
	{
		su8LastRectNum = u8RectNum;
	}
	if((gs_CANComm.u8RectIndex< mRECTNUM)&&(gs_CANComm.u8RectIndex<su8LastRectNum))
	{
		for (iCmdIdx= _CAN_R_CMD_ID_S_03; iCmdIdx <_CAN_R_CMD_ID_G_01; iCmdIdx++)
		{
			if(gs_CANComm.cmdInfos[iCmdIdx].u8SampCount<gs_CANComm.cmdInfos[iCmdIdx].u8SampTimes) 
			{
				gs_CANComm.cmdInfos[iCmdIdx].pfnCmdHandler(gs_CANComm.u8RectIndex, &gs_CANComm, &gs_CANComm);
				if(gs_CANComm.cmdInfos[iCmdIdx].u8SampTimes == SAMP_ALWAYS)
				{
					gs_CANComm.cmdInfos[iCmdIdx].u8SampCount = 0; 
				}
				else if(gs_CANComm.cmdInfos[iCmdIdx].u8SampCount <255)
				{
					gs_CANComm.cmdInfos[iCmdIdx].u8SampCount++;
				}
			}
		}
		gs_CANComm.u8RectIndex++;
		if((gs_CANComm.u8RectIndex>= mRECTNUM)||(gs_CANComm.u8RectIndex>=su8LastRectNum))
		{
			gs_CANComm.u8RectIndex=0;
		}
	}
}

void NoSampDeal(void)
{
	UINT8 u8Idx = 0;
	UINT8 u8IsFail = TRUE;
	UINT8 u8IsFail_2 = TRUE;
	UINT8 u8AcIsUnderFail = TRUE;
	UINT8 u8AcIsOverFail = TRUE;
	UINT8 u8IsAnyRectAlm_A, u8IsAnyRectAlm_B;

	UINT8 u8RectNum = gs_CANComm.GrpData[GRP_A].nCommRectNum + gs_CANComm.GrpData[GRP_B].nCommRectNum;
	static UINT8 su8LastRectNum = 0;
	//20190807
	if(gEvChargeInfo.u8ACInputContactorCloseAlmDelayFlag == TRUE)
	{
		return;
	}
	
	if (u8RectNum >= su8LastRectNum)
	{
		su8LastRectNum = u8RectNum;
	}

	for (u8Idx= su8LastRectNum; u8Idx < mRECTNUM; u8Idx++)
	{
		gs_CANComm.RectData[u8Idx].bIsExisted = FALSE;
		gs_CANComm.RectData[u8Idx].bCommFail = TRUE;
	}

	u8IsFail = TRUE;
	u8IsFail_2 = TRUE;
	u8IsAnyRectAlm_A = FALSE;
	u8IsAnyRectAlm_B = FALSE;
	for (u8Idx= 0; u8Idx < mRECTNUM; u8Idx++)
	{

		if (TRUE == gs_CANComm.RectData[u8Idx].bIsExisted)
		{
			//判断是否不能充电
			if (FALSE == gs_CANComm.RectData[u8Idx].bCommFail
				&& FALSE == gs_CANComm.RectData[u8Idx].bOverTemp
				&& FALSE == gs_CANComm.RectData[u8Idx].bFanFault
				&& FALSE == gs_CANComm.RectData[u8Idx].bProtected
				&& FALSE == gs_CANComm.RectData[u8Idx].bRectFail)
			{
				if ((GRP_A+1) == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsFail = FALSE;
				}

				if ((GRP_B+1) == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsFail_2 = FALSE;
				}
			}

			//判断是否存在模块告警
			if ((TRUE == gs_CANComm.RectData[u8Idx].bCommFail)
				|| ((gs_CANComm.RectData[u8Idx].bStatCode[2] & RECT_STAT_CODE_2_ALM_BITs) != 0)
				|| ((gs_CANComm.RectData[u8Idx].bStatCode[1] & RECT_STAT_CODE_1_ALM_BITs) != 0)
				|| ((gs_CANComm.RectData[u8Idx].bStatCode[0] & RECT_STAT_CODE_0_ALM_BITs) != 0))
			{
				if ((GRP_A+1) == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsAnyRectAlm_A = TRUE;
				}

				if ((GRP_B+1) == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsAnyRectAlm_B = TRUE;
				}
			}
		}
	}

	//判断A组是否无正常模块，不能充电
	if (TRUE == u8IsFail)
	{
		gs_CANComm.GrpData[GRP_A].u8IsNoNormalRect = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_A].u8IsNoNormalRect = FALSE;
	}
	//判断A组是否存在模块告警
	if (TRUE == u8IsAnyRectAlm_A)
	{
		gs_CANComm.GrpData[GRP_A].bIsAnyRectAlarm = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_A].bIsAnyRectAlarm = FALSE;
	}

	//单枪模式屏蔽B枪告警
	if(gEvChargeInfo.u8IsSingleGunMode == TRUE)
	{
		u8IsFail_2 = FALSE;
		u8IsAnyRectAlm_B = FALSE;
	}
	//判断B组是否无正常模块，不能充电
	if (TRUE == u8IsFail_2)
	{
		gs_CANComm.GrpData[GRP_B].u8IsNoNormalRect = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_B].u8IsNoNormalRect = FALSE;
	}
	//判断B组是否存在模块告警
	if (TRUE == u8IsAnyRectAlm_B)
	{
		gs_CANComm.GrpData[GRP_B].bIsAnyRectAlarm = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_B].bIsAnyRectAlarm = FALSE;
	}
		
	for (u8Idx= 0; u8Idx < mRECTNUM; u8Idx++)
	{
		if (gs_CANComm.RectData[u8Idx].bIsExisted == TRUE)
		{
			if (gs_CANComm.RectData[u8Idx].fAB_Volt > 330
				|| gs_CANComm.RectData[u8Idx].fBC_Volt > 330
				|| gs_CANComm.RectData[u8Idx].fCA_Volt > 330)
			{
				u8AcIsUnderFail = FALSE;
			}
			if (gs_CANComm.RectData[u8Idx].fAB_Volt < 510
				|| gs_CANComm.RectData[u8Idx].fBC_Volt < 510
				|| gs_CANComm.RectData[u8Idx].fCA_Volt < 510)
			{
				u8AcIsOverFail = FALSE;
			}
		}
	}
	if (TRUE == u8AcIsUnderFail)
	{
		gs_CANComm.u8IsACUnder = TRUE;
	}
	else
	{
		gs_CANComm.u8IsACUnder = FALSE;
	}

	if (u8AcIsOverFail == TRUE)
	{
		gs_CANComm.u8IsACOver = TRUE;
	}
	else
	{
		gs_CANComm.u8IsACOver = FALSE;
	}


	//模块不正常，清输出电压、电流
	for (u8Idx = 0; u8Idx < _MAX_GRP_NUM; u8Idx++)
	{			
		if((!gs_CANComm.bIsGrpExist[u8Idx])
			|| (gs_CANComm.bIsGrp_NotAvailable[u8Idx] == TRUE))
		{
			//模块不正常，清输出电压、电流
			gs_CANComm.GrpData[u8Idx].fOutVolt = 0;					
			gs_CANComm.GrpData[u8Idx].fOutCurr = 0; 
			gs_CANComm.GrpData[u8Idx].fRatedPower = 0;
			gs_CANComm.GrpData[u8Idx].fMax_OutCurr = 0;
			gs_CANComm.GrpData[u8Idx].fRatedMaxCurr = 0;
			gs_CANComm.GrpData[u8Idx].nCommRectNum = 0;
		}
	}


	//判断所有模块通信中断
	u8IsFail = TRUE;
	for (u8Idx = 0; u8Idx < mRECTNUM; u8Idx++)
	{
		if (FALSE == gs_CANComm.RectData[u8Idx].bCommFail)
		{
			u8IsFail = FALSE;
			break;
		}
	}
	if ((u8IsFail == TRUE)
		|| ((gs_CANComm.GrpData[GRP_A].nCommRectNum == 0) && (gs_CANComm.GrpData[GRP_B].nCommRectNum == 0)))
	{
		gs_CANComm.u8IsAllCommFail = TRUE;
	}
	else 
	{
		gs_CANComm.u8IsAllCommFail = FALSE;
	}
}

/************************************************/
//函数名称:										
//函数功能:		
//         当用户修桩数量时和桩序号时，需要重新配置CAN屏蔽码，以及环网节点配置
//更改日期:      								
/************************************************/
void Reflash_CAN_SelftAddr(void)
{
	gs_CANComm.bySelfAddress = 0xF0;
	gs_CANComm.byGrpIdx[GRP_A] = 1;
	gs_CANComm.byGrpIdx[GRP_B] = 2;			
	InitCan3Port();
}


static void Calculate_RTMaxOutCurrent(CANCommData_TypeDef *pRect_Rough)
{
	float fRatedMaxCurr = 0.0;
	float fCalcMax = 0.0;
	int n;
	for(n = 0; n < _MAX_GRP_NUM; n++)
	{
		if((pRect_Rough->GrpData[n].nCommRectNum > 0) &&
			(pRect_Rough->GrpData[n].fOutVolt > 50.0) &&
			(pRect_Rough->GrpData[n].fRatedPower > 1.0))
		{
			fRatedMaxCurr = pRect_Rough->GrpData[n].fRatedMaxCurr;
			fCalcMax = pRect_Rough->GrpData[n].fRatedPower * 1000 / pRect_Rough->GrpData[n].fOutVolt;
			fCalcMax = (fCalcMax > fRatedMaxCurr) ? fRatedMaxCurr : fCalcMax;
			//pRect_Rough->GrpData[n].fMax_OutCurr = fCalcMax;
			pRect_Rough->GrpData[n].fMax_OutCurr_ring  = fCalcMax;
		}
		else
		{
			//pRect_Rough->GrpData[n].fMax_OutCurr = pRect_Rough->GrpData[n].fRatedMaxCurr;
			pRect_Rough->GrpData[n].fMax_OutCurr_ring  = pRect_Rough->GrpData[n].fRatedMaxCurr;
		}
		//Set CurrLmt
		/*fCalcMax = pRect_Rough->fMaxSingleRectCurrLmt * pRect_Rough->GrpData[n].nCommRectNum;
		if(fCalcMax >= 20.0)
		{
			pRect_Rough->GrpData[n].fMax_OutCurr = MIN(pRect_Rough->GrpData[n].fMax_OutCurr, fCalcMax);
		}*/
	}
}

#define AC_3_PHASE_LOW_VOLT		320
#define AC_3_PHASE_OVER_VOLT	440
void RT_Alm_Deal(void)
{
	UINT8 u8Idx = 0;
	UINT8 u8IsFail = TRUE;
	UINT8 u8IsFail_2 = TRUE;
	UINT8 u8AcIsUnderFail = FALSE;
	UINT8 u8AcIsOverFail = FALSE;
	UINT8 u8IsAnyRectAlm_A, u8IsAnyRectAlm_B;
	//UINT8 u8RectNum = gs_CANComm.GrpData[GRP_A].nCommRectNum + gs_CANComm.GrpData[GRP_B].nCommRectNum;
	//static UINT8 su8LastRectNum = 0;
	//20190807
	if(gEvChargeInfo.u8ACInputContactorCloseAlmDelayFlag == TRUE)
	{
		return;
	}
	
	u8IsFail = TRUE;
	u8IsFail_2 = TRUE;
	u8IsAnyRectAlm_A = FALSE;
	u8IsAnyRectAlm_B = FALSE;
	for(u8Idx = 0; u8Idx < gs_CANComm.nScanedRectNum; u8Idx++)
	{
		/*if (TRUE == gs_CANComm.RectData[u8Idx].bIsExisted 
			&& FALSE == gs_CANComm.RectData[u8Idx].bCommFail
			&& FALSE == gs_CANComm.RectData[u8Idx].bOverTemp
			&& FALSE == gs_CANComm.RectData[u8Idx].bFanFault
			&& FALSE == gs_CANComm.RectData[u8Idx].bProtected
			&& FALSE == gs_CANComm.RectData[u8Idx].bRectFail)
		{
			if (gs_CANComm.byGrpIdx[GRP_A] == gs_CANComm.RectData[u8Idx].nBelongedGrp)
			{
				u8IsFail = FALSE;
			}

			if (gs_CANComm.byGrpIdx[GRP_B] == gs_CANComm.RectData[u8Idx].nBelongedGrp)
			{
				u8IsFail_2 = FALSE;
			}
		}*/
		if (TRUE == gs_CANComm.RectData[u8Idx].bIsExisted)
		{
			//判断是否不能充电
			if (FALSE == gs_CANComm.RectData[u8Idx].bCommFail
				&& FALSE == gs_CANComm.RectData[u8Idx].bOverTemp
				&& FALSE == gs_CANComm.RectData[u8Idx].bFanFault
				&& FALSE == gs_CANComm.RectData[u8Idx].bProtected
				&& FALSE == gs_CANComm.RectData[u8Idx].bRectFail)
			{
				if (gs_CANComm.byGrpIdx[GRP_A] == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsFail = FALSE;
				}

				if (gs_CANComm.byGrpIdx[GRP_B] == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsFail_2 = FALSE;
				}
			}

			//判断是否存在模块告警
			if ((TRUE == gs_CANComm.RectData[u8Idx].bCommFail)
				|| ((gs_CANComm.RectData[u8Idx].bStatCode[2] & RECT_STAT_CODE_2_ALM_BITs) != 0)
				|| ((gs_CANComm.RectData[u8Idx].bStatCode[1] & RECT_STAT_CODE_1_ALM_BITs) != 0)
				|| ((gs_CANComm.RectData[u8Idx].bStatCode[0] & RECT_STAT_CODE_0_ALM_BITs) != 0))
			{
				if (gs_CANComm.byGrpIdx[GRP_A] == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsAnyRectAlm_A = TRUE;
				}

				if (gs_CANComm.byGrpIdx[GRP_B] == gs_CANComm.RectData[u8Idx].nBelongedGrp)
				{
					u8IsAnyRectAlm_B = TRUE;
				}
			}
		}

	}
	//判断A组是否无正常模块，不能充电
	if (TRUE == u8IsFail)
	{
		gs_CANComm.GrpData[GRP_A].u8IsNoNormalRect = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_A].u8IsNoNormalRect = FALSE;
	}
	//判断A组是否存在模块告警
	if (TRUE == u8IsAnyRectAlm_A)
	{
		gs_CANComm.GrpData[GRP_A].bIsAnyRectAlarm = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_A].bIsAnyRectAlarm = FALSE;
	}

	//单枪模式屏蔽B枪告警
	if(gEvChargeInfo.u8IsSingleGunMode == TRUE)
	{
		u8IsFail_2 = FALSE;
		u8IsAnyRectAlm_B = FALSE;
	}
	//判断B组是否无正常模块，不能充电
	if (TRUE == u8IsFail_2)
	{
		gs_CANComm.GrpData[GRP_B].u8IsNoNormalRect = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_B].u8IsNoNormalRect = FALSE;
	}
	//判断B组是否存在模块告警
	if (TRUE == u8IsAnyRectAlm_B)
	{
		gs_CANComm.GrpData[GRP_B].bIsAnyRectAlarm = TRUE;
	}
	else
	{
		gs_CANComm.GrpData[GRP_B].bIsAnyRectAlarm = FALSE;
	}


	//判断所有模块通信中断
	u8IsFail = TRUE;
	//for (u8Idx = 0; u8Idx < mRECTNUM; u8Idx++)
	for(u8Idx = 0; u8Idx < gs_CANComm.nScanedRectNum; u8Idx++)
	{
		if (FALSE == gs_CANComm.RectData[u8Idx].bCommFail)
		{
			u8IsFail = FALSE;
			break;
		}
	}
	if ((u8IsFail == TRUE)
		|| ((gs_CANComm.GrpData[GRP_A].nCommRectNum == 0) && (gs_CANComm.GrpData[GRP_B].nCommRectNum == 0)))
	{
		gs_CANComm.u8IsAllCommFail = TRUE;
	}
	else
	{
		gs_CANComm.u8IsAllCommFail = FALSE;
	}

	//for (u8Idx= 0; u8Idx < mRECTNUM; u8Idx++)
	u8AcIsUnderFail = TRUE;
	u8AcIsOverFail = TRUE;
	for(u8Idx = 0; u8Idx < gs_CANComm.nScanedRectNum; u8Idx++)
	{
		if (gs_CANComm.RectData[u8Idx].bIsExisted == TRUE)
		{
			if (gs_CANComm.RectData[u8Idx].fAB_Volt > AC_3_PHASE_LOW_VOLT
				|| gs_CANComm.RectData[u8Idx].fBC_Volt > AC_3_PHASE_LOW_VOLT
				|| gs_CANComm.RectData[u8Idx].fCA_Volt > AC_3_PHASE_LOW_VOLT)
			{
				u8AcIsUnderFail = FALSE;
			}
			if (gs_CANComm.RectData[u8Idx].fAB_Volt < AC_3_PHASE_OVER_VOLT
				|| gs_CANComm.RectData[u8Idx].fBC_Volt < AC_3_PHASE_OVER_VOLT
				|| gs_CANComm.RectData[u8Idx].fCA_Volt < AC_3_PHASE_OVER_VOLT)
			{
				u8AcIsOverFail = FALSE;
			}			
		}
	}
	if (TRUE == u8AcIsUnderFail)
	{
		gs_CANComm.u8IsACUnder = TRUE;
	}
	else
	{
		gs_CANComm.u8IsACUnder = FALSE;
	}

	if (u8AcIsOverFail == TRUE)
	{
		gs_CANComm.u8IsACOver = TRUE;
	}
	else
	{
		gs_CANComm.u8IsACOver = FALSE;
	}
}

static BOOL bLastCommOK[mRECTNUM];
static BOOL bLastWorkOK[mRECTNUM];

//检查是否需要重新初始化CAN3口
void ReInitRectCanCommPort(void)
{
	if (gs_CANComm.u8IsAllCommFail == FALSE)
	{
		gs_CANComm.u16AllCommFailResetDelay = 0;
	}	

	if(gs_CANComm.u16AllCommFailResetDelay > 60)
	{	
		gs_CANComm.u16AllCommFailResetDelay = 0;
		BSP_InitSPI3();
		OSTimeDly(500);
		InitCan3Port();
		OSTimeDly(500);
		gSelfTest.fSetTaskID(TASK_ID_CAN3);
	}
}
void SendAddressingCmd(void)
{
	canmsg SendMsg;

	SendMsg.Id = 0x757f803;

	SendMsg.Data[0] = 0x04000001;
	SendMsg.Data[1] = 0x00;
	CAN3_Send(&SendMsg, 8);
}
static void CAN3_RectDataFrameProcess(void)
{
	UINT8 u8RcvMsgNum;
	u8RcvMsgNum = RxCAN3Buff(CAN_3_MAX_BUFF, RxMsg);
	if (u8RcvMsgNum > 0)
	{
		UnPackData(u8RcvMsgNum);
			
	}
	
}
void Init_CmdInfos(void)
{
	CAN_RECT_FRAME_INFO* p = gs_CANComm.cmdInfos;
	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_S_03,	GetSingle_VoltCurr);
	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_S_04,	GetSingle_GrpAndStatus);
	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_S_06,	GetSingle_PhaseVolts);
	INIT_RECT_CMD(p, SAMP_3_TIMES,0, _CAN_R_CMD_ID_S_07,GetSingle_VerInfo);

	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_G_24,	SetGrp_WorkMode);
	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_G_1A,	SetGrp_OnOff);
	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_G_01,	GetGrp_VoltCurr);
	INIT_RECT_CMD(p, SAMP_ALWAYS,0, _CAN_R_CMD_ID_G_02,	GetGrp_NumAndStatus);
	INIT_RECT_CMD(p, SAMP_3_TIMES,0, _CAN_R_CMD_ID_G_13,SetGrp_WalkIn);
	INIT_RECT_CMD(p, SAMP_ALWAYS, 0,_CAN_R_CMD_ID_G_1B,	SetGrp_AdjVoltCurr);
}
//如果想删本线程代码，围绕这个结构体删除，别的线程调用的API可以做空函数或自行处理！
void InitgCANComm(void)
{
	Init_CmdInfos();
	gs_CANComm.fInitRectData = CAN3_InitRectData;
	gs_CANComm.fMutexInit = CAN3_MutexInit;						//互斥锁

	gs_CANComm.fGetRectCtlVoltCurr = CAN3_GetRectCtlVoltCurr;	//当前控制电压电流
	gs_CANComm.fSetRectCtlVoltCurr = CAN3_SetRectCtlVoltCurr;	//控制电压电流
	gs_CANComm.fGetRectCtlStatus = CAN3_GetRectCtlStatus;		//当前控制状态
	gs_CANComm.fSetRectCtlStatus = CAN3_SetRectCtlStatus;		//设置状态		
	gs_CANComm.fGetRectCtlMode = CAN3_GetRectCtlMode;		//当前控制状态
	gs_CANComm.fSetRectCtlMode = CAN3_SetRectCtlMode;		//设置状态		
	gs_CANComm.u8GetRectDigital = u8GetRectDigital;
	gs_CANComm.fGetRectAnalog = CAN3_GetRectAnalog;
	gs_CANComm.fGetRectBarcode = CAN3_get_rect_barcode;
	gs_CANComm.fGetRectIDForShelf = CAN3_GetRectID;
	gs_CANComm.fCtrlRectFunction = CtrlRectFunction;

	gs_CANComm.hMutexWriteCAN3Port = MutexCreate(UPPER_PRIO_WRITE_CAN3);	
	if (gs_CANComm.hMutexWriteCAN3Port == NULL)
	{
		gs_CANComm.hMutexWriteCAN3Port = MutexCreate(UPPER_PRIO_WRITE_CAN3);
	}
	gs_CANComm.u8GroupRectCMDIndex[GRP_A]=_CAN_R_CMD_ID_G_01;
	gs_CANComm.u8GroupRectCMDIndex[GRP_B]=_CAN_R_CMD_ID_G_01;
        gs_CANComm.u8GRP_WorkModeSet[GRP_A] = GRP_WORKMODE_RECTIFIER;
        gs_CANComm.u8GRP_WorkModeSet[GRP_B] = GRP_WORKMODE_RECTIFIER;

}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void TASK_CAN3_Proc(void* pdata)
{

	BSP_InitSPI3();
	OSTimeDly(500);	
	InitgCANComm();
	gs_CANComm.fInitRectData(1);
	Reflash_CAN_SelftAddr();	

	//TEST
	UINT8 tmp = mcp2515_ReadRegister(MCP_CANSTAT);					//读控制寄存器
	tmp = mcp2515_ReadRegister(MCP_CANCTRL);
	tmp = mcp2515_ReadRegister(MCP_CANSTAT);
	tmp = mcp2515_ReadRegister(MCP_CNF1);
	tmp = mcp2515_ReadRegister(MCP_CNF2);
	tmp = mcp2515_ReadRegister(MCP_CNF3);

	OSTimeDly(4);
	SendAddressingCmd();
	OSTimeDly(4);
	//UINT32 u32Delay = 0;
	for(;;)
	{
		//发数据 并延时
		gSelfTest.fSetTaskID(TASK_ID_CAN3);
	
		//检查是否需要重新初始化CAN3口
		ReInitRectCanCommPort();
		
        SampGInfo();
		OSTimeDly(10);
		CAN3_RectDataFrameProcess();

        SampSInfo();
        OSTimeDly(10);
		CAN3_RectDataFrameProcess();
        NoSampDeal();
		
	} //END: for(;;)
}