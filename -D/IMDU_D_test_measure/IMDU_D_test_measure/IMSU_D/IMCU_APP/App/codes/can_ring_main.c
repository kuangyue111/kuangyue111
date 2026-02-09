
#include "main.h"
#include "can_ring.h"
#include "Uart6_new.h"

RING_SALVE	s_RingSlavesBuf[2][MAX_RING_SLAVE_NUM];	//存放动态产生的从机信息

#define R_PRPTO_DEVNO_0C		0x0C //for ring
#define R_PROTO_RING_BROADCAST	0x1F

CanRxMsgTypeDef RxMsg_Ring[CAN_3_MAX_BUFF];

RING_ALL_INFO			g_RingAllInfo; 

static CAN_RING_DATA	s_RingData_A;
static CAN_RING_DATA	s_RingData_B;
CAN_RING_DATA*			g_pRing_A;
CAN_RING_DATA*			g_pRing_B;


//以下是状态机函数/**/
RING_STATE_MACHINE_STATE	OnIdle(void* pArg);
//Master
RING_STATE_MACHINE_STATE	OnPrepare_BeforeCharge(void *pArg);
RING_STATE_MACHINE_STATE	OnWaitMerging(void *pArg);
RING_STATE_MACHINE_STATE	OnCharging(void *pArg);
RING_STATE_MACHINE_STATE	OnWaitCanceling(void *pArg);
RING_STATE_MACHINE_STATE	OnStop(void *pArg);
//Slave
RING_STATE_MACHINE_STATE	OnPrepare_BeforeMerge(void *pArg);
RING_STATE_MACHINE_STATE	OnMerging(void *pArg);
RING_STATE_MACHINE_STATE	OnWaitCmd3(void *pArg);
RING_STATE_MACHINE_STATE	OnMergeCanceling(void *pArg);


/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
time_t time_ring(unsigned char ucflag)
{
	return (time_t)g_RingAllInfo.u32RingTimeSec;
}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void Can_Msg_u32Data_2_u8data(UCHAR *u8CanMsgDataBuf,  canmsg * msg)
{
	//UINT8 Buff[8];
	u8CanMsgDataBuf[0] = (msg->Data[0] >> 24)&0xff;
	u8CanMsgDataBuf[1] = (msg->Data[0] >> 16)&0xff;
	u8CanMsgDataBuf[2] = (msg->Data[0] >> 8)&0xff;
	u8CanMsgDataBuf[3] = (msg->Data[0])&0xff;
	u8CanMsgDataBuf[4] = (msg->Data[1] >> 24)&0xff;
	u8CanMsgDataBuf[5] = (msg->Data[1] >> 16)&0xff;
	u8CanMsgDataBuf[6] = (msg->Data[1] >> 8)&0xff;
	u8CanMsgDataBuf[7] = (msg->Data[1])&0xff;
}
void HalCan_Msg_u32Data_2_u8data(UCHAR *u8CanMsgDataBuf,  CanRxMsgTypeDef *msg)
{
	//UINT8 Buff[8];
	u8CanMsgDataBuf[0] = msg->Data[0];
	u8CanMsgDataBuf[1] = msg->Data[1];
	u8CanMsgDataBuf[2] = msg->Data[2];
	u8CanMsgDataBuf[3] = msg->Data[3];
	u8CanMsgDataBuf[4] = msg->Data[4];
	u8CanMsgDataBuf[5] = msg->Data[5];
	u8CanMsgDataBuf[6] = msg->Data[6];
	u8CanMsgDataBuf[7] = msg->Data[7];
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
float CAN_StringToFloat(BYTE* strInput)
{
	__XXX_UNION_VALUE unTempVal;
	unsigned char	i;	


	for(i = 0; i < 4; i++)
	{
		unTempVal.chtemp[4 - i - 1] = strInput[i];
	}
	return unTempVal.ftemp;
}
//void CAN_FloatToString(float fInput, BYTE* strOutput)
//{
//	unsigned char	i;	
//	__XXX_UNION_VALUE unTempVal;
//
//	unTempVal.fValue = fInput;
//
//	for(i = 0; i < 4; i++)
//	{
//		strOutput[4 - i - 1] = unTempVal.chtemp[i];
//	}
//
//	return;
//}
void CAN_UShortToString(USHORT usInput, BYTE* strOutput)
{
	strOutput[0] = (BYTE)(usInput >> 8);
	strOutput[1] = (BYTE)(usInput);
}


void CAN_UintToString(UINT32 nInput, BYTE* strOutput)
{
	int					i;	
	__XXX_UNION_VALUE		unValue;

	unValue.u32Value = nInput;

	for(i = 0; i < 4; i++)
	{
		strOutput[4 - i - 1] = unValue.chtemp[i];
	}

	return;
}

USHORT CAN_StringToUShort(BYTE* strInput)
{
	USHORT usValue = (((USHORT)(strInput[0])) << 8)
		+ ((USHORT)(strInput[1]));

	return usValue;
}

UINT32 CAN_StringToUint(BYTE* strInput)
{
	UINT32 uiValue = (((UINT32)(strInput[0])) << 24)
		+ (((UINT32)(strInput[1])) << 16)
		+ (((UINT32)(strInput[2])) << 8)
		+ ((UINT32)(strInput[3]));

	return uiValue;
}



/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
//if(Queue_Get(pRing_Rough->hqCANMsgFromRect, &recFrame, FALSE, MAX_WAIT_RING_QUEUE) == ERR_QUEUE_OK)
static void ClenRingRxBuff(void)
{
	for (UINT8 u8Idx = 0; u8Idx < CAN_3_MAX_BUFF; u8Idx++)
	{
		RxMsg_Ring[u8Idx].ExtId = 0;
	}
}
UINT8  GetRingDataFromCAN3(void)
{
	UINT8 uFrameNum = 0;
	
	ClenRingRxBuff();

	uFrameNum = MCP2515_Read_Ring(RxMsg_Ring, CAN_3_MAX_BUFF);

	/*if (uFrameNum > 0)
	{
		
	}*/

	return uFrameNum;
}

void Parse_RingCmd1(BYTE bySrcAddr, BYTE* pbyData)
{
	unsigned short u16temp;

	if(bySrcAddr == s_RingData_A.pNodeMe->byNodeAddress)
	{
		//本桩发出去的报文，不可能自己还收到了，因此是ID冲突/**/
		s_RingData_A.bSelfIDConflict = TRUE;
		return; //不解析本桩内的CMD1/**/
	}
	else if((bySrcAddr == s_RingData_B.pNodeMe->byNodeAddress)&&(gEvChargeInfo.u8IsSingleGunMode == FALSE)) //双枪模式才判断
	{
		//本桩发出去的报文，不可能自己还收到了，因此是ID冲突/**/
		s_RingData_B.bSelfIDConflict = TRUE;
		return; //不解析本桩内的CMD1/**/
	}

	int n;
	RING_NODE* pNode = NULL;
	for(n = 0;n < g_RingAllInfo.nNodeTotal; n++)
	{
		if(g_RingAllInfo.allNodes[n].byNodeAddress == bySrcAddr)
		{
			pNode = &g_RingAllInfo.allNodes[n];
			break;
		}		
	}
	if(pNode == NULL)
	{
		return;
	}
	pNode->bIsExist = TRUE;
	pNode->tmLastRefresh = time_ring(NULL);
	pNode->bCommFail = FALSE;

	u16temp = ((UINT16)pbyData[0] << 8) + (UINT16)pbyData[1];
	pNode->fOutVolt = (float)u16temp * 0.1;
	u16temp = ((UINT16)pbyData[2] << 8) + (UINT16)pbyData[3];
	pNode->fOutCurr = (float)u16temp * 0.1;
	pNode->fMaxMergeCurr = (float)(pbyData[4]);
	BYTE byAvailableStatus  = pbyData[5];
	if(byAvailableStatus == 0)
	{
		pNode->emLockState = _LOCK_NO_LOCK;
	}
	else if((byAvailableStatus >= 1) && (byAvailableStatus <= 32))
	{
		pNode->emLockState = _LOCK_AS_SLAVE;
		pNode->nLockedMasterID = byAvailableStatus;
	}
	else if(byAvailableStatus == 99)
	{
		pNode->emLockState = _LOCK_AS_MASTER;
	}
	else //if(byAvailableStatus == 255)
	{
		pNode->emLockState = _LOCK_NOT_AVAILABLE;
	}
	//TRACE("---------Now received CMD1 from Node%d, byAvailableStatus = %d-------------\n", bySrcAddr, byAvailableStatus);
	BYTE byMergeStatus = (pbyData[6] & 0x0F);
	switch (byMergeStatus)
	{
		case 0x0A:
			pNode->emMergeStatus = STAT_PENDING;
			break;
		case 0x0B:
			pNode->emMergeStatus = STAT_MERGING;
			break;
		case 0x01:
			pNode->emMergeStatus = STAT_NOMERGE_GUNIN;
			pNode->bIsCC1_GunInCar = TRUE;
			break;

		case 0x00:
		default:
			pNode->emMergeStatus = STAT_NO_MERGE;
			pNode->bIsCC1_GunInCar = FALSE;
			break;
	}
	BYTE byExtStatus = (pbyData[7] & 0x0F);
	pNode->bIsNeedExtCurr = (byExtStatus > 0) ? TRUE : FALSE;
}

void Prase_RingCmd234(CAN_RING_DATA* pRing_Rough, BYTE bySrcAddr, BYTE byCmdNo, BYTE* pbyData)
{
	unsigned short u16temp;
	int n;
	//__XXX_UNION_VALUE unTempVal;
	
	RING_NODE* pNodeFrom = pRing_Rough->pNodeMe;
	time_t tmNow = time_ring(NULL);
	do 
	{
		pNodeFrom = pNodeFrom->right;
		if(pNodeFrom == NULL)
		{
			break;
		}
		else if(pNodeFrom->byNodeAddress == bySrcAddr)
		{
			break;
		}
	} while (pNodeFrom != pRing_Rough->pNodeMe);

	if((pNodeFrom == pRing_Rough->pNodeMe) ||
		(pNodeFrom == NULL)) //本组发出去的，不理会。一般是不可能的/**/
	{
		//TRACEX("----------Ring%d, Received invalide CMD234 from %d-----------\n", pRing_Rough->pNodeMe->byNodeAddress, bySrcAddr);
		return;
	}
	//TRACEX("---------Ring%d WW received CMD%d from Node%d-------------\n", pRing_Rough->pNodeMe->byNodeAddress,  byCmdNo, bySrcAddr);
	if(byCmdNo == RING_CMD2)
	{
		//float fReqVolt= (float)(CAN_StringToUShort(&pbyData[0])) * 0.1;
		u16temp = ((UINT16)pbyData[0] << 8) + (UINT16)pbyData[1];
		float fReqVolt = (float)u16temp * 0.1;

		//float fReqCurr= (float)(CAN_StringToUShort(&pbyData[2])) * 0.1;
		u16temp = ((UINT16)pbyData[2] << 8) + (UINT16)pbyData[3];
		float fReqCurr = (float)u16temp * 0.1;


		//float fBusRealVolt= (float)(CAN_StringToUShort(&pbyData[4])) * 0.1;
		u16temp = ((UINT16)pbyData[4] << 8) + (UINT16)pbyData[5];
		float fBusRealVolt = (float)u16temp * 0.1;

		BYTE byLock = pbyData[6];
		if(byLock == 0xAA)
		{
			//判断是否可以锁定从机/**/
			if((pRing_Rough->pCurrState->emState != RING_STATE_IDLE) && (pRing_Rough->pCurrState->emState != RING_STATE_PREPARE_BEFOR_MERGE))
			{
				//TRACEX("----------Ring%d Found invalide CMD2-AA from %d-----------\n",pRing_Rough->pNodeMe->byNodeAddress,  bySrcAddr);
				return;
			}
			pRing_Rough->slave.pMaster = pNodeFrom;
			pRing_Rough->slave.fBusRealVolt = fBusRealVolt;
			pRing_Rough->slave.fReqVolt = fReqVolt;
			pRing_Rough->slave.fReqCurr = fReqCurr;
			pRing_Rough->slave.bIsCmd2_AA_Ready = TRUE; //通知StateMachine开始锁定从机/**/
			pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster = tmNow;
		}
		else if(byLock == 0xCC)
		{
			//判断是否可以锁定从机/**/
			if((pRing_Rough->pCurrState->emState != RING_STATE_IDLE) && (pRing_Rough->pCurrState->emState != RING_STATE_PREPARE_BEFOR_MERGE))
			{
				//TRACEX("----------Ring%d Found invalide CMD2-AA from %d-----------\n",pRing_Rough->pNodeMe->byNodeAddress,  bySrcAddr);
				return;
			}
			else if(!pRing_Rough->pNodeMe->bIsCC1_GunInCar)
			{
				return;
			}
			pRing_Rough->slave.pMaster = pNodeFrom;
			pRing_Rough->slave.fBusRealVolt = fBusRealVolt;
			pRing_Rough->slave.fReqVolt = fReqVolt;
			pRing_Rough->slave.fReqCurr = fReqCurr;
			pRing_Rough->slave.bIsCmd2_CC_Ready = TRUE; //通知StateMachine开始锁定从机/**/
			pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster = tmNow;
		}
		else
		{
			//判断是否充电中/**/
			if((pRing_Rough->pNodeMe->emLockState != _LOCK_AS_SLAVE) ||
				(pNodeFrom != pRing_Rough->slave.pMaster))
			{
				//TRACEX("----------Ring%d Found invalide CMD2-00 from %d-----------\n", pRing_Rough->pNodeMe->byNodeAddress, bySrcAddr);
				return;
			}
			pRing_Rough->slave.fBusRealVolt = fBusRealVolt;
			pRing_Rough->slave.fReqVolt = fReqVolt;
			pRing_Rough->slave.fReqCurr = fReqCurr;
			pRing_Rough->slave.emSetRelayOnOff = (byLock == 0x01) ? 1 : 0; //Ctrl the relay on/off
			pRing_Rough->slave.bIsCmd2_00_Ready= TRUE;
			pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster = tmNow;
		}
	}
	else if(byCmdNo == RING_CMD3)
	{
		//判断是否从机/**/
		if((pRing_Rough->pNodeMe->emLockState != _LOCK_AS_SLAVE) ||
			(pNodeFrom != pRing_Rough->slave.pMaster))
		{
			return;
		}
		pRing_Rough->slave.bIsCmd3_Ready= TRUE;
		pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster = tmNow;
	}
	else if(byCmdNo == RING_CMD4)
	{
		//判断是否主机/**/
		if(pRing_Rough->pNodeMe->emLockState != _LOCK_AS_MASTER)
		{
			return;
		}
		//以下查找是那个Slave过来的		
		for(n = 0;n < pRing_Rough->master.nSlaveTotal; n++)
		{
			if(pRing_Rough->master.slaves[n] == NULL)
			{
				continue;
			}
			if(pRing_Rough->master.slaves[n]->pNode->byNodeAddress == bySrcAddr)
			{
				pRing_Rough->master.slaves[n]->bIsCmd4Ready = TRUE;
				break;
			}
		}
	}
	else if(byCmdNo == RING_CMD5)
	{
		/*if((pRing_Rough->pNodeMe->left->byNodeAddress == bySrcAddr) ||
			(pRing_Rough->pNodeMe->right->byNodeAddress == bySrcAddr))
		{
			pRing_Rough->master.fSlave1_kWhReading = CAN_StringToFloat(pbyData);
		}
		else
		{
			pRing_Rough->master.fSlave2_kWhReading = CAN_StringToFloat(pbyData);
		}*/

		//以下查找是那个Slave过来的		
		for(n = 0;n < pRing_Rough->master.nSlaveTotal; n++)
		{
			if(pRing_Rough->master.slaves[n] == NULL)
			{
				continue;
			}
			if(pRing_Rough->master.slaves[n]->pNode->byNodeAddress == bySrcAddr)
			{
				if(pRing_Rough->master.u8WaitSlaveKwhReading[n] == 0)
				{
					pRing_Rough->master.fSlaveStart_kWhReading[n] = CAN_StringToFloat(pbyData);
					pRing_Rough->master.fSlaveEnd_kWhReading[n] = pRing_Rough->master.fSlaveStart_kWhReading[n];
					pRing_Rough->master.u8WaitSlaveKwhReading[n] = 1;
				}
				else
				{
					pRing_Rough->master.fSlaveEnd_kWhReading[n] = CAN_StringToFloat(pbyData);
				}
				break;
			}
		}
	}
}


static void Parse_RingCmd6(BYTE bySrcAddr, BYTE* pbyData)
{
	if(bySrcAddr == s_RingData_A.pNodeMe->byNodeAddress)
	{
		//本桩发出去的报文，不可能自己还收到了，因此是ID冲突/**/
		s_RingData_A.bSelfIDConflict = TRUE;
		return; //不解析本桩内的CMD1/**/
	}
	else if((bySrcAddr == s_RingData_B.pNodeMe->byNodeAddress)&&(gEvChargeInfo.u8IsSingleGunMode == FALSE)) //双枪模式才判断
	{
		//本桩发出去的报文，不可能自己还收到了，因此是ID冲突/**/
		s_RingData_B.bSelfIDConflict = TRUE;
		return; //不解析本桩内的CMD1/**/
	}
	int n;
	RING_NODE* pNode = NULL;
	for(n = 0;n < g_RingAllInfo.nNodeTotal; n++)
	{
		if(g_RingAllInfo.allNodes[n].byNodeAddress == bySrcAddr)
		{
			pNode = &g_RingAllInfo.allNodes[n];
		}
	}
	if(pNode == NULL)
	{
		return;
	}
	pNode->fBMSReqMaxPower = (float)(CAN_StringToUShort(&pbyData[0]));
	pNode->fCalcLeftMaxPower = (float)(CAN_StringToUShort(&pbyData[2]));
	pNode->tmStartAsMaster = (time_t)(CAN_StringToUint(&pbyData[4]));
}

void Refresh_SelfNode(CAN_RING_DATA* pRing_Rough)
{
	double f64temp;

	BOOL bIsGunA = (pRing_Rough->bIsGunA) ? TRUE : FALSE;
	pRing_Rough->pNodeMe->fMaxMergeCurr = bIsGunA ? gs_CANComm.GrpData[GRP_A].fMax_OutCurr_ring : 
		gs_CANComm.GrpData[GRP_B].fMax_OutCurr_ring;
	pRing_Rough->pNodeMe->fOutVolt = bIsGunA ? gs_CANComm.GrpData[GRP_A].fOutVolt : 
		gs_CANComm.GrpData[GRP_B].fOutVolt;
	pRing_Rough->pNodeMe->fOutCurr = bIsGunA ? gs_CANComm.GrpData[GRP_A].fOutCurr : 
		gs_CANComm.GrpData[GRP_B].fOutCurr;

	//Get CC1 status
	if(pRing_Rough->bIsGunA)
	{
		if (gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] == SPEAR_ON_LINE)
		{
			pRing_Rough->pNodeMe->bIsCC1_GunInCar = TRUE;
		}
		else
		{
			pRing_Rough->pNodeMe->bIsCC1_GunInCar = FALSE;
		}
	}
	else
	{
		if (gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] == SPEAR_ON_LINE)
		{
			pRing_Rough->pNodeMe->bIsCC1_GunInCar = TRUE;
		}
		else
		{
			pRing_Rough->pNodeMe->bIsCC1_GunInCar = FALSE;
		}
	}
	
	if(((pRing_Rough->pNodeMe->emMergeStatus == STAT_NO_MERGE) || (pRing_Rough->pNodeMe->emMergeStatus == STAT_NOMERGE_GUNIN)) 
		&& (g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN))
	{
		pRing_Rough->pNodeMe->emMergeStatus = ((pRing_Rough->pNodeMe->bIsCC1_GunInCar) ? STAT_NOMERGE_GUNIN : STAT_NO_MERGE);
	}
	else if(((pRing_Rough->pNodeMe->emMergeStatus == STAT_MERGING) || (pRing_Rough->pNodeMe->emMergeStatus == STAT_PENDING)) 
		&& (g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN) 
		&& (pRing_Rough->pNodeMe->emLockState == _LOCK_AS_SLAVE)
		&& (pRing_Rough->pNodeMe->bIsCC1_GunInCar))
	{
		//Get kWh Reading
		if(pRing_Rough->bIsGunA)
		{
			pRing_Rough->pNodeMe->fkWhReading = gWH_Info[GUN_NO_A].fWHRealValue;
		}
		else
		{
			pRing_Rough->pNodeMe->fkWhReading = gWH_Info[GUN_NO_B].fWHRealValue;
		}
		Send_RingCmd(pRing_Rough, RING_CMD5, NULL);
	}


	pRing_Rough->pNodeMe->tmLastRefresh = time_ring(NULL);
	pRing_Rough->pNodeMe->bCommFail = FALSE;
	
	//calculate master real output
	if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
	{
		float fMaxVolt = pRing_Rough->pNodeMe->fOutVolt;
		float fSumCurr = pRing_Rough->pNodeMe->fOutCurr;

		int n;		
		for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
		{
			if(pRing_Rough->master.slaves[n] == NULL)
			{
				continue;
			}
			if((pRing_Rough->master.slaves[n]->pNode->fOutVolt <= (gEvChargeInfo.fMaxOutVolt))
				&& (pRing_Rough->master.slaves[n]->pNode->fOutVolt > fMaxVolt))
			{
				fMaxVolt = pRing_Rough->master.slaves[n]->pNode->fOutVolt;
			}			
			fSumCurr += pRing_Rough->master.slaves[n]->pNode->fOutCurr;			
		}

		f64temp = 0;
		for(n = 0; n < MAX_RING_SLAVE_NUM; n++)
		{
			if(pRing_Rough->master.u8WaitSlaveKwhReading[n] == 1)
			{				
				f64temp += (pRing_Rough->master.fSlaveEnd_kWhReading[n] - pRing_Rough->master.fSlaveStart_kWhReading[n]);
			}
		}
		pRing_Rough->master.fSlaveTotalOutKwhReading = f64temp;
		pRing_Rough->master.fTotalOutVolt = fMaxVolt;
		pRing_Rough->master.fTotalOutCurr = fSumCurr;

		

		Send_RingCmd(pRing_Rough, RING_CMD6, NULL);

	}

	//将状态CMD1发出去/**/
	Send_RingCmd(pRing_Rough, RING_CMD1, NULL);
}

void Judge_NodeCommFail(CAN_RING_DATA* pRing_Rough)
{
	int n;
	time_t tmNow = time_ring(NULL);
	BOOL bAllOK = TRUE;
	g_RingAllInfo.nOnlineTotal = 0;
	for(n = 0; n < g_RingAllInfo.nNodeTotal; n++)
	{
		//5s没收到报文，则认为是通信中断/**/
		if(tmNow - g_RingAllInfo.allNodes[n].tmLastRefresh > 5)
		{
			//TRACE("------------Node[%d] Comm Failed!!!!!!!-----\n", g_RingAllInfo.allNodes[n].byNodeAddress);
			g_RingAllInfo.allNodes[n].bCommFail = TRUE;
			if(g_RingAllInfo.allNodes[n].emLockState == _LOCK_NO_LOCK)
			{
				g_RingAllInfo.allNodes[n].emLockState = _LOCK_NOT_AVAILABLE;
			}
			bAllOK = FALSE;
		}
		else
		{
			g_RingAllInfo.nOnlineTotal++;
		}
	}
	if(bAllOK)
	{
		pRing_Rough->bSelfIDConflict = FALSE;
	}
}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/

void Send_RingCmd(CAN_RING_DATA* pRing_Rough, BYTE byCmdNo, void* pWhere2Send)
{
	unsigned short u16temp1, u16temp2;
	__XXX_UNION_VALUE untemp;
	__16BITS_UNION_VALUE untemp_16bits;
	UINT8 u8Rst = FALSE;
	UNUSED(u8Rst);
	canmsg SendMsg;
	UCHAR u8CanMsgDataBuf[8];

	//BYTE pbySendBuf[CAN_FRAME_LEN];
	//memset(pbySendBuf, 0 , CAN_FRAME_LEN);
	//BYTE bySendCmd = (RING_CMD2_AA == byCmdNo) ? 0x02 : byCmdNo;
	BYTE bySendCmd = ((RING_CMD2_AA == byCmdNo) || (RING_CMD2_CC == byCmdNo)) ? 0x02 : byCmdNo;

	//BYTE* byData = pbySendBuf + 5;
	CAN_RING_DATA *pOtherRIng_Rough = (pRing_Rough == g_pRing_A) ? g_pRing_B : g_pRing_A; //如果发给本桩另一把枪的命令，不用走CAN，直接处理即可/**/

	if(byCmdNo == RING_CMD1)
	{
		SendMsg.Id = ((UINT32)R_PRPTO_DEVNO_0C << 22) + ((UINT32)bySendCmd << 16) + ((UINT32)R_PROTO_RING_BROADCAST << 8) + (UINT32)(pRing_Rough->pNodeMe->byNodeAddress);	

		u16temp1 = (USHORT)(pRing_Rough->pNodeMe->fOutVolt * 10);
		u16temp2 = (USHORT)(pRing_Rough->pNodeMe->fOutCurr * 10);
		SendMsg.Data[0] = ((UINT32)u16temp1 << 16) + ((UINT32)u16temp2);
		untemp.chtemp[3] = (BYTE)(pRing_Rough->pNodeMe->fMaxMergeCurr);
		if(pRing_Rough->pNodeMe->emLockState == _LOCK_NO_LOCK)
		{
			untemp.chtemp[2] = 0;
		}
		else if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
		{
			untemp.chtemp[2] = 99;
		}
		else if(pRing_Rough->pNodeMe->emLockState == _LOCK_NOT_AVAILABLE)
		{
			untemp.chtemp[2] = 255;
		}
		else //1~32
		{
			untemp.chtemp[2] = (BYTE)pRing_Rough->pNodeMe->nLockedMasterID;
		}	
		switch(pRing_Rough->pNodeMe->emMergeStatus)
		{
		case STAT_PENDING:
			untemp.chtemp[1] = 0x0A;
			break;
		case STAT_MERGING:
			untemp.chtemp[1] = 0x0B;
			break;
		case STAT_NOMERGE_GUNIN:
			untemp.chtemp[1] = 0x01;
			break;

		default:
			untemp.chtemp[1] = 0x00;
			break;
		}
		untemp.chtemp[0] = (pRing_Rough->pNodeMe->bIsNeedExtCurr) ? 0x01 : 0x00;
		SendMsg.Data[1] = untemp.u32Value;


	}
	//else if((byCmdNo == RING_CMD2_AA) || (RING_CMD2 == byCmdNo))
	else if((byCmdNo == RING_CMD2_AA) || (RING_CMD2 == byCmdNo) || (RING_CMD2_CC == byCmdNo))
	{		
		SendMsg.Id = ((UINT32)R_PRPTO_DEVNO_0C << 22) + ((UINT32)bySendCmd << 16) + ((UINT32)((RING_SALVE*)pWhere2Send)->pNode->byNodeAddress << 8) + (UINT32)(pRing_Rough->pNodeMe->byNodeAddress);	

		u16temp1 = (USHORT)(((RING_SALVE*)pWhere2Send)->fVolt2Sent * 10);
		u16temp2 = (USHORT)(((RING_SALVE*)pWhere2Send)->fCurr2Sent* 10);
		SendMsg.Data[0] = ((UINT32)u16temp1 << 16) + ((UINT32)u16temp2);

		u16temp1 = (USHORT)(pRing_Rough->master.fTotalOutVolt * 10);
		BOOL bNeedSetRelay = (((RING_SALVE*)pWhere2Send)->emSetRelayOnOff > 0) ? TRUE : FALSE;

		/*untemp_16bits.chtemp[1] = (byCmdNo == RING_CMD2_AA) ? 0xAA : (bNeedSetRelay ? 0x01 : 0x00);
		untemp_16bits.chtemp[0] = 0;*/		
		if(RING_CMD2_CC == byCmdNo)
		{
			untemp_16bits.chtemp[1] = 0xCC;
			untemp_16bits.chtemp[0] = 0;
		}
		else
		{
			untemp_16bits.chtemp[1] = (byCmdNo == RING_CMD2_AA) ? 0xAA : (bNeedSetRelay ? 0x01 : 0x00);
			untemp_16bits.chtemp[0] = 0;			
		}
		SendMsg.Data[1] = ((UINT32)u16temp1 << 16) + ((UINT32)untemp_16bits.u16Temp);

		RING_SALVE*pSlave = (RING_SALVE*)pWhere2Send;

		if(gEvChargeInfo.u8IsSingleGunMode == FALSE) //双枪模式才判断
		{
			if(pSlave->pNode->byNodeAddress == pOtherRIng_Rough->pNodeMe->byNodeAddress) //another gun in th
			{
				//TRACE("----------Now Send CMD02 to the Other gun within this Charger, emRelay = %d---------\n", bNeedSetRelay);
				//直接让另一组来解析/**/
				Can_Msg_u32Data_2_u8data(u8CanMsgDataBuf, &SendMsg);  //s_u8CanMsgDataBuf
				Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, RING_CMD2, u8CanMsgDataBuf);
				return;
			}
		}


	}
	else if(RING_CMD3 == byCmdNo)
	{
		SendMsg.Id = ((UINT32)R_PRPTO_DEVNO_0C << 22) + ((UINT32)bySendCmd << 16) + ((UINT32)((RING_SALVE*)pWhere2Send)->pNode->byNodeAddress << 8) + (UINT32)(pRing_Rough->pNodeMe->byNodeAddress);	
		untemp.chtemp[3] = 0x01;
		untemp.chtemp[2] = 0x00;
		untemp.chtemp[1] = 0x00;
		untemp.chtemp[0] = 0x00;
		SendMsg.Data[0] = untemp.u32Value;
		SendMsg.Data[1] = 0;

		RING_SALVE*pSlave = (RING_SALVE*)pWhere2Send;

		if(gEvChargeInfo.u8IsSingleGunMode == FALSE) //双枪模式才判断
		{
			if(pSlave->pNode == pOtherRIng_Rough->pNodeMe) //another gun in th
			{
				//直接让另一组来解析/**/
				Can_Msg_u32Data_2_u8data(u8CanMsgDataBuf, &SendMsg);  //s_u8CanMsgDataBuf
				Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, byCmdNo, u8CanMsgDataBuf);
				return;
			}
		}
	}
	else if(RING_CMD4 == byCmdNo)
	{
		SendMsg.Id = ((UINT32)R_PRPTO_DEVNO_0C << 22) + ((UINT32)bySendCmd << 16) + ((UINT32)pRing_Rough->slave.pMaster->byNodeAddress << 8) + (UINT32)(pRing_Rough->pNodeMe->byNodeAddress);	
		untemp.chtemp[3] = 0x01;
		untemp.chtemp[2] = 0x00;
		untemp.chtemp[1] = 0x00;
		untemp.chtemp[0] = 0x00;
		SendMsg.Data[0] = untemp.u32Value;
		SendMsg.Data[1] = 0;

		if(gEvChargeInfo.u8IsSingleGunMode == FALSE) //双枪模式才判断
		{
			if(pRing_Rough->slave.pMaster == pOtherRIng_Rough->pNodeMe) //another gun in th
			{
				//直接让另一组来解析/**/
				Can_Msg_u32Data_2_u8data(u8CanMsgDataBuf, &SendMsg);  //s_u8CanMsgDataBuf
				Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, byCmdNo, u8CanMsgDataBuf);
				return;
			}
		}
	}
	else if(RING_CMD5 == byCmdNo)
	{
		SendMsg.Id = ((UINT32)R_PRPTO_DEVNO_0C << 22) + ((UINT32)bySendCmd << 16) + ((UINT32)pRing_Rough->slave.pMaster->byNodeAddress << 8) + (UINT32)(pRing_Rough->pNodeMe->byNodeAddress);	
		
		untemp.ftemp = pRing_Rough->pNodeMe->fkWhReading;		
		SendMsg.Data[0] = untemp.u32Value;
		SendMsg.Data[1] = 0;
		
		if(gEvChargeInfo.u8IsSingleGunMode == FALSE) //双枪模式才判断
		{
			if(pRing_Rough->slave.pMaster == pOtherRIng_Rough->pNodeMe) //another gun in th
			{
				//直接让另一组来解析/**/
				Can_Msg_u32Data_2_u8data(u8CanMsgDataBuf, &SendMsg);  //s_u8CanMsgDataBuf
				Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, byCmdNo, u8CanMsgDataBuf);
				return;
			}
		}
	}
	else if(byCmdNo == RING_CMD6)
	{
		//Stuff first 29 bits in the send frame
		//CAN_StuffHead(R_PRPTO_DEVNO_0C, bySendCmd, R_PROTO_RING_BROADCAST, pRing_Rough->pNodeMe->byNodeAddress,
		//	pbySendBuf);
		/*CAN_UShortToString((USHORT)(pRing_Rough->pNodeMe->fBMSReqMaxPower + 0.5), byData);
		CAN_UShortToString((USHORT)(pRing_Rough->pNodeMe->fCalcLeftMaxPower + 0.5), byData + 2);
		CAN_UintToString((UINT)pRing_Rough->pNodeMe->tmStartAsMaster, byData + 4);*/

		SendMsg.Id = ((UINT32)R_PRPTO_DEVNO_0C << 22) + ((UINT32)bySendCmd << 16) + ((UINT32)R_PROTO_RING_BROADCAST << 8) + (UINT32)(pRing_Rough->pNodeMe->byNodeAddress);	

		//Now gather data
		u16temp1 = (USHORT)(pRing_Rough->pNodeMe->fBMSReqMaxPower + 0.5);
		u16temp2 = (USHORT)(pRing_Rough->pNodeMe->fCalcLeftMaxPower + 0.5);
		SendMsg.Data[0] = ((UINT32)u16temp1 << 16) + ((UINT32)u16temp2);
		SendMsg.Data[1] = pRing_Rough->pNodeMe->tmStartAsMaster;		
	}


	//Write_CAN3(s_pRect_Rough->iCanDevHanle, (void *)pbySendBuf, CAN_FRAME_LEN);
	//write(s_pRect_Rough->iCanDevHanle, (void *)pbySendBuf, CAN_FRAME_LEN); //20171229, Jimmy发现，环网上会发生mutex竞争现象，导致通信异常/**/

	//gSelfTest.fSetTaskID(TASK_ID_CAN3);
	//ClenRBuff();
	u8Rst = gs_CANComm.fTxCAN3Buff(&SendMsg, 8);
	OSTimeDly(5);
}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void Ctrl_RighDO(CAN_RING_DATA* pRing_Rough, BOOL bOn)
{
	//if(bOn && (pRing_Rough->pNodeMe->right->bIsCC1_GunInCar||left->bIsCC1_GunInCar) &&
	//	g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN) //if right gun is inserted in car, do not merge!!!
	//{
	//	bOn = FALSE;
	//}
	if(pRing_Rough->bIsGunA)
	{
		//CtrlDO(MERGER_CTRL_DO, (bOn ? 0: 1));//并机接触器是反逻辑/**/
		if(bOn)
		{
			__PARALLEL_ON();
		}
		else
		{
			__PARALLEL_OFF();
		}
	}
	else
	{
		//SIG_POINTER			sigP;	
		//VAR_VALUE			varVal;
		////用A枪IMIU的DO/**/
		//MAKE_SIGP(sigP, 301, SIG_TYPE_CTRL, 2);
		//varVal.enumValue = bOn;
		//DXI_Internal_SentCtrl(sigP, varVal, FALSE);
		if(bOn)
		{
			gEvChargeInfo.u8CTRLInterDOCMD = 0x01;
		}
		else
		{
			gEvChargeInfo.u8CTRLInterDOCMD = 0x00;
		}

		gEvChargeInfo.u8CTRLInterDOCMDFlag = TRUE;
	}
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void Ctrl_SlaveOutDO(CAN_RING_DATA* pRing_Rough, BOOL bOn)
{
	//int nSampID = (pRing_Rough->bIsGunA) ? 2 : 3;
	//CtrlDO(OUTPUT_RELAY_DO(nSampID), bOn);
	if(pRing_Rough->bIsGunA)
	{
		if(bOn)
		{
			__A_DC_MCCB_ON();
		}
		else
		{
			__A_DC_MCCB_OFF();
		}
	}
	else
	{
		if(bOn)
		{
			__B_DC_MCCB_ON();
		}
		else
		{
			__B_DC_MCCB_OFF();
		}
	}
}

/************************************************/
//函数名称:										
//函数功能:		由AdjAll_VoltCurr_4Ring() 及 Slave调用								
//更改日期:      								
/************************************************/
void AdjSelf_VoltCurr(CAN_RING_DATA* pRing_Rough, float fTargetVolt, float fTargetCurr)
{
	if(pRing_Rough->bIsGunA)
	{				
		fTargetCurr = MIN(gs_CANComm.GrpData[GRP_A].fMax_OutCurr_ring, fTargetCurr);
		fTargetCurr = MIN(fTargetCurr, gEvChargeInfo.fSetLimitMaxOutCurr_1);
		__A_RECT_ADJST_VLT_CURR(fTargetVolt, fTargetCurr);
		//gs_CANComm.cmdInfos[_CAN_R_CMD_ID_G_1B].pfnCmdHandler(GRP_A, &gs_CANComm, &gs_CANComm);
	}
	else
	{
		fTargetCurr = MIN(gs_CANComm.GrpData[GRP_B].fMax_OutCurr_ring, fTargetCurr);
		fTargetCurr = MIN(fTargetCurr, gEvChargeInfo.fSetLimitMaxOutCurr_2);
		__B_RECT_ADJST_VLT_CURR(fTargetVolt, fTargetCurr);
		//gs_CANComm.cmdInfos[_CAN_R_CMD_ID_G_1B].pfnCmdHandler(GRP_B, &gs_CANComm, &gs_CANComm);
	}

}
/************************************************/
//函数名称:										
//函数功能:		由AdjAll_VoltCurr_4Ring() 及 Slave调用								
//更改日期:      								
/************************************************/
void Set_SelfRectOnOff(CAN_RING_DATA* pRing_Rough, BOOL bOn)
{
	UINT8 u8OnOff;

	if(bOn == TRUE)
	{
		u8OnOff = RECT_ON;
	}
	else
	{
		u8OnOff = RECT_OFF;
	}

	if(pRing_Rough->bIsGunA)
	{				
		gs_CANComm.fSetRectCtlStatus(GRP_A, u8OnOff);
		//gs_CANComm.cmdInfos[_CAN_R_CMD_ID_G_1A].pfnCmdHandler(GRP_A, &gs_CANComm, &gs_CANComm);
	}
	else
	{
		gs_CANComm.fSetRectCtlStatus(GRP_B, u8OnOff);
		//gs_CANComm.cmdInfos[_CAN_R_CMD_ID_G_1A].pfnCmdHandler(GRP_B, &gs_CANComm, &gs_CANComm);
	}
}
/************************************************/
//函数名称:										
//函数功能:		由AdjAll_VoltCurr_4Ring() 及 Slave调用								
//更改日期:      								
/************************************************/
void Stop_AllSlaveRects(CAN_RING_DATA* pRing_Rough)
{
	//First stop myself/**/
	Set_SelfRectOnOff(pRing_Rough, FALSE);

	//then stop all slaves/**/
	int n;
	for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
	{
		if(pRing_Rough->master.slaves[n] == NULL)
		{
			continue;
		}
		SetGrp_OnOff_4RingSlaveGrp( pRing_Rough->master.slaves[n]->pNode->byNodeAddress, RECT_OFF);
	}
	//also stop the to-be-handling slave
	if(pRing_Rough->master.pSlave_InHandling != NULL)
	{
		SetGrp_OnOff_4RingSlaveGrp( pRing_Rough->master.pSlave_InHandling->pNode->byNodeAddress, RECT_OFF);
	}
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
//enum _RING_CTRL_CMD_def
//{
//	RING_CTRL_CMD_NONE = 0,	
//	RING_CTRL_CMD_LOCK_AS_MASTER,		//Lock gun as master
//	RING_CTRL_CMD_RQ_ST_SLAVE,			//Request for Stopping Slave
//	RING_CTRL_CMD_ST_MASTER,			//Stop Master Charging
//	//RING_CTRL_CMD_VOLT_ADJ,				//Ring DC voltage control
//	//RING_CTRL_CMD_CURR_ADJ,				//Ring Adjust Rect Total Curr
//	RING_CTRL_CMD_ADJ_VOLT_CURR,		//Ring Adjust Rect Total Curr and Voltage 
//	RING_CTRL_CMD_EndOfStop,			//Notice End of Stop(Internal)
//};
int Ring_Ctrl(UINT8 u8GunNo, UINT8 u8CmdNo, VAR_VALUE *pVarVal1, VAR_VALUE *pVarVal2)
{
	struct tm  tmNow;
	unsigned short u16StartTime, u16StopTime, u16CurrTime;

	CAN_RING_DATA* pRing_Rough = (u8GunNo == GRP_A) ? g_pRing_A : g_pRing_B;

	if(u8CmdNo == RING_CTRL_CMD_LOCK_AS_MASTER)   //Lock gun as master
	{
		if(pRing_Rough->pCurrState->emState == RING_STATE_IDLE)
		{
			pRing_Rough->master.bIsReceivedLock = TRUE;
		}
		else if((g_RingAllInfo.emRingMergeMode == RING_MODE_INSERT) 
			&& (pRing_Rough->pCurrState->emState == RING_STATE_MERGING)) //插枪模式下可以发送给从机/**/
		{
			gRTCData.fGetLocalTime(&tmNow);

			u16StartTime = (unsigned short)g_RingAllInfo.nGunInsertMode_Start[0] * 60 +  (unsigned short)g_RingAllInfo.nGunInsertMode_Start[1];
			u16StopTime = (unsigned short)g_RingAllInfo.nGunInsertMode_Stop[0] * 60 +  (unsigned short)g_RingAllInfo.nGunInsertMode_Stop[1];
			u16CurrTime = (unsigned short)tmNow.tm_hour * 60 + (unsigned short)tmNow.tm_min;

			if(u16StartTime < u16StopTime)
			{
				if((u16CurrTime >= u16StartTime) && (u16StartTime <= u16StopTime))
				{
					pRing_Rough->master.bIsReceivedLock = TRUE;
				}
			}
			else if(u16StartTime > u16StopTime)
			{
				if((u16CurrTime <= u16StopTime) || (u16StartTime >= u16StopTime))
				{
					pRing_Rough->master.bIsReceivedLock = TRUE;
				}
			}
			else   //起始时间相同，表示全天都支持插枪切换？
			{
				pRing_Rough->master.bIsReceivedLock = TRUE;
			}			
		}
	}
	else if(u8CmdNo == RING_CTRL_CMD_ST_MASTER)			//10  Stop Master Charging
	{
		if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER) 
			&& (!pRing_Rough->master.bIsRecievedStop))
		{
			Stop_AllSlaveRects(pRing_Rough);
			pRing_Rough->master.bIsRecievedStop = TRUE;
		}
	}
	else if(u8CmdNo == RING_CTRL_CMD_EndOfStop)		//11  Notice End of Stop(Internal)
	{
		if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
		{
			//TRACE("-----Received BMS stoppped OK cmd-------------\n");
			pRing_Rough->master.bIsReceivedStoppOK = TRUE;
		}
	}
	//else if(u8CmdNo == RING_CTRL_CMD_CURR_ADJ)			//24   Ring Adjust Rect Total Curr
	else if(u8CmdNo == RING_CTRL_CMD_ADJ_VOLT_CURR)
	{
		pRing_Rough->master.fTotalReqVolt = pVarVal1->ftemp;

		//gEvChargeInfo.fMaxOutCurr  / fSetLimitMaxOutCurr_1 /  fSetLimitMaxOutCurr_2
		pRing_Rough->master.fTotalReqCurr_FromBms  = MIN(pVarVal2->ftemp, gEvChargeInfo.fMaxOutCurr);
		//pRing_Rough->master.fTotalReqCurr = MIN(pVarVal2->ftemp, gEvChargeInfo.fMaxOutCurr);
		//pRing_Rough->master.fTotalReqCurr = MIN(pRing_Rough->master.fTotalReqCurr , g_RingAllInfo.fRingMaxAllowedCurr);//BMS那边已经比较过/**/
		//TRACE("----------BMS Received fReqVolt = %.1f, fReqCurr = %.1f, fMaxCurLmt = %.1f-----------\n", varVal.fValue, pVarVal->fValue, s_pRect_Rough->fMaxCurrLimit);
		pRing_Rough->master.bIsReceivedAdj = TRUE;
	}
	else if(u8CmdNo == RING_CTRL_CMD_RQ_ST_SLAVE)			//2   Request for Stopping Slave
	{
		pRing_Rough->slave.bReceivedStop = TRUE;
	}

	return TRUE;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
static BOOL CreateRingNodes(void)
{	
	//g_RingAllInfo.nNodeTotal = 2 * gEvChargeInfo.nTotalRingStub;
	g_RingAllInfo.nNodeTotal = gEvChargeInfo.nTotalRingStub;
	if((g_RingAllInfo.nNodeTotal < 2) || (g_RingAllInfo.nNodeTotal > MAX_SUPPORTED_RING_NODE)) //最少需要2个桩/**/
	{
		g_RingAllInfo.bIsRingCfgErr = TRUE;
		return FALSE;
	}

	g_RingAllInfo.nOnlineTotal = 0;
	int n;
	for(n = 0; n < g_RingAllInfo.nNodeTotal; n++)
	{
		g_RingAllInfo.allNodes[n].byNodeAddress = (BYTE)(n + 1);
		g_RingAllInfo.allNodes[n].emLockState = _LOCK_NO_LOCK;
		g_RingAllInfo.allNodes[n].emMergeStatus = STAT_NO_MERGE;
		g_RingAllInfo.allNodes[n].tmLastRefresh = 0;
		g_RingAllInfo.allNodes[n].bIsExist = FALSE;
		//建立双向循环链表/**/
		if(n < g_RingAllInfo.nNodeTotal - 1)
		{
			g_RingAllInfo.allNodes[n].right = &g_RingAllInfo.allNodes[n + 1];
		}
		else
		{
			g_RingAllInfo.allNodes[n].right = &g_RingAllInfo.allNodes[0];
		}

		if(n > 0)
		{
			g_RingAllInfo.allNodes[n].left = &g_RingAllInfo.allNodes[n - 1];
		}
		else
		{
			g_RingAllInfo.allNodes[n].left = &g_RingAllInfo.allNodes[g_RingAllInfo.nNodeTotal - 1];
		}
	}
	//其余节点进行初始化
	for(; n < MAX_SUPPORTED_RING_NODE; n++)
	{
		g_RingAllInfo.allNodes[n].byNodeAddress = (BYTE)(n + 1);
		g_RingAllInfo.allNodes[n].emLockState = _LOCK_NOT_AVAILABLE;
		g_RingAllInfo.allNodes[n].emMergeStatus = STAT_NO_MERGE;
		g_RingAllInfo.allNodes[n].tmLastRefresh = 0;
		g_RingAllInfo.allNodes[n].bIsExist = FALSE;

		g_RingAllInfo.allNodes[n].right = NULL;
		g_RingAllInfo.allNodes[n].left = NULL;
	}

	

	//找自己/**/
	//g_RingAllInfo.bIsRingCfgErr = FALSE;
	
	{
		BOOL bFindA = FALSE;
		BOOL bFindB = FALSE;
		for(n = 0; n < g_RingAllInfo.nNodeTotal; n++)
		{
			if(g_RingAllInfo.allNodes[n].byNodeAddress == (BYTE)(gs_CANComm.byGrpIdx[GRP_A]))
			{
				bFindA = TRUE;
				s_RingData_A.pNodeMe = &g_RingAllInfo.allNodes[n];
				s_RingData_A.pNodeMe->bIsExist = TRUE;
			}
			else if(g_RingAllInfo.allNodes[n].byNodeAddress == (BYTE)(gs_CANComm.byGrpIdx[GRP_B]))
			{
				bFindB = TRUE;
				s_RingData_B.pNodeMe = &g_RingAllInfo.allNodes[n];
				s_RingData_B.pNodeMe->bIsExist = TRUE;
			}
		}
		//if((!bFindB) || (!bFindA))
		if((!bFindA) 
			|| ((!bFindB) && (gEvChargeInfo.u8IsSingleGunMode == FALSE)))  //双枪模式才判断
		{
			g_RingAllInfo.bIsRingCfgErr = TRUE;
		}
		else
		{
			g_RingAllInfo.bIsRingCfgErr = FALSE;
		}
	}

	return TRUE;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
static void Restore_Default(unsigned char isPowerOnInit, CAN_RING_DATA *pRing_Rough)
{
	//上电初始化，不需要判断下面条件
	if(isPowerOnInit == 1)
	{
	}
	else
	{
		if(g_RingAllInfo.bIsRingCfgErr)
		{
			return;
		}

		if((pRing_Rough->pNodeMe->emLockState == _LOCK_NO_LOCK) && 
			(pRing_Rough->pNodeMe->emMergeStatus == STAT_NOMERGE_GUNIN) &&
			g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)
		{
			return;
		}
	}

	//Clean me
	pRing_Rough->pNodeMe->bIsCC1_GunInCar = FALSE;
	pRing_Rough->pNodeMe->emLockState = _LOCK_NO_LOCK;
	pRing_Rough->pNodeMe->emMergeStatus = STAT_NO_MERGE;
	pRing_Rough->pNodeMe->bIsNeedExtCurr = FALSE;
	pRing_Rough->pNodeMe->fkWhReading = 0.0;

	pRing_Rough->pNodeMe->fBMSReqMaxPower = 0.0;
	pRing_Rough->pNodeMe->fCalcLeftMaxPower = g_RingAllInfo.fAllRingsPwrLmt;
	pRing_Rough->pNodeMe->tmStartAsMaster = 0;


	//Clean master
	pRing_Rough->master.bIsReceivedAdj = FALSE;
	pRing_Rough->master.bIsReceivedLock = FALSE;
	pRing_Rough->master.bIsRecievedStop = FALSE;
	pRing_Rough->master.bIsReceivedStoppOK = FALSE;
	pRing_Rough->master.fTotalReqVolt = 0;
	pRing_Rough->master.fTotalReqCurr = 0;
	pRing_Rough->master.tm_CancelMergeDelay = 0;
	int n;
	for(n = 0; n < MAX_RING_SLAVE_NUM; n++)
	{
		//SAFELY_DELETE(pRing_Rough->master.slaves[n]);
		pRing_Rough->master.slaves[n] = NULL;
		pRing_Rough->master.u8RingSlavesBufNo[n] = 0;
		pRing_Rough->master.u8RingSlavesBuf_IsUsed[n] = 0;
	}
	pRing_Rough->master.u8RingSlavesBuf_InHandling = 0;	
	pRing_Rough->master.nSlaveTotal = 0;
	pRing_Rough->master.pLeftEdge = NULL;
	pRing_Rough->master.pRightEdge = NULL;

	pRing_Rough->master.fSlave1_kWhReading = 0.0;
	pRing_Rough->master.fSlave2_kWhReading = 0.0;

	//Clean slave
	pRing_Rough->slave.bIsCmd2_00_Ready = FALSE;
	pRing_Rough->slave.bIsCmd2_AA_Ready = FALSE;
	pRing_Rough->slave.bIsCmd2_CC_Ready = FALSE;
	pRing_Rough->slave.bIsCmd3_Ready = FALSE;
	pRing_Rough->slave.pMaster = NULL;
	pRing_Rough->slave.bReceivedStop = FALSE;
	pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster = 0;
	pRing_Rough->slave.tmLastCancel = 0;
	
	pRing_Rough->bSelfIDConflict = FALSE;

	Set_SelfRectOnOff(pRing_Rough, FALSE);

	Ctrl_RighDO(pRing_Rough, FALSE);
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
BOOL Ring_Initialize(void)
{	
	unsigned char i;
	CAN_RING_DATA *pRingData;

	g_pRing_A = &s_RingData_A;
	g_pRing_A->bIsGunA = TRUE;
	g_pRing_A->pNodeMe = NULL;

	g_pRing_B = &s_RingData_B;
	g_pRing_B->bIsGunA = FALSE;
	g_pRing_B->pNodeMe = NULL;

	g_RingAllInfo.u32RingTimeSec = 0;

	s_RingData_A.bIsGunMergable = FALSE;
	s_RingData_B.bIsGunMergable = FALSE;

	s_RingData_A.hMutex = MutexCreate(UPPER_PRIO_CAN_RING_A);
	s_RingData_B.hMutex = MutexCreate(UPPER_PRIO_CAN_RING_B);


	g_RingAllInfo.nNodeTotal = 0;//总接入节点数, 根据配置确定，一旦改变必须重新建链表，故需要重启/**/
	g_RingAllInfo.nOnlineTotal = 0; //实际在线的节点数目/**/
	g_RingAllInfo.bIsRingCfgErr = TRUE;


	//Init the state machine
	//不要改变顺序，因为以顺序来索引/**/
	for(i = 0; i < 2; i++)
	{
		if(i == 0)
			pRingData = g_pRing_A;
		else
			pRingData = g_pRing_B;
				
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_IDLE, OnIdle);

		//Master		
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_PREPARE_BEFRE_CHARGE, OnPrepare_BeforeCharge);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_CHARGING, OnCharging);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_WAITING_MERGING, OnWaitMerging);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_WAITING_CANCELING, OnWaitCanceling);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_STOPING, OnStop);

		//Slave
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_PREPARE_BEFOR_MERGE,	OnPrepare_BeforeMerge);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_MERGING,	OnMerging);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_WAITING_CMD3,	OnWaitCmd3);
		INIT_RING_STATE_MACHINE(pRingData, RING_STATE_MERGE_CANCELING, OnMergeCanceling	);
	}

	
	//time_t tmNow = time_ring(NULL);
	s_RingData_A.pCurrState = &s_RingData_A.stateMachineLoop[RING_STATE_IDLE];
	s_RingData_A.pCurrState->tm_Enter = time_ring(NULL);

	s_RingData_B.pCurrState = &s_RingData_B.stateMachineLoop[RING_STATE_IDLE];
	s_RingData_B.pCurrState->tm_Enter = time_ring(NULL);

	//CreateRingNodes();
	Restore_Default(1, g_pRing_A);
	//双枪模式才处理
	if(gEvChargeInfo.u8IsSingleGunMode == FALSE)
	{
		Restore_Default(1, g_pRing_B);
	}

	for(i = 0; i < MAX_RING_SLAVE_NUM; i++)
	{
		s_RingData_A.master.slaves[i] = NULL;
		s_RingData_B.master.slaves[i] = NULL;
	}
	s_RingData_A.master.nSlaveTotal = 0;
	s_RingData_B.master.nSlaveTotal = 0;


	//节点进行初始化
	for(i = 0; i < MAX_SUPPORTED_RING_NODE; i++)
	{
		g_RingAllInfo.allNodes[i].byNodeAddress = (BYTE)(i + 1);
		g_RingAllInfo.allNodes[i].emLockState = _LOCK_NOT_AVAILABLE;
		g_RingAllInfo.allNodes[i].emMergeStatus = STAT_NO_MERGE;
		g_RingAllInfo.allNodes[i].tmLastRefresh = 0;
		g_RingAllInfo.allNodes[i].bCommFail = FALSE;
		g_RingAllInfo.allNodes[i].bIsExist = FALSE;

		g_RingAllInfo.allNodes[i].right = NULL;
		g_RingAllInfo.allNodes[i].left = NULL;
	}
	
	/*s_RingData_A.hMutex = Mutex_Create(TRUE);
	s_RingData_B.hMutex = Mutex_Create(TRUE);*/
	if((s_RingData_A.hMutex == NULL) || (s_RingData_B.hMutex == NULL))
	{
		return FALSE;
	}
	
		
	return TRUE;
}


//以下是状态机函数

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/

void JudgeIfGunMergeable(CAN_RING_DATA *pRingData)
{
	BOOL bMergable = TRUE;

	if(pRingData->bIsGunA)
	{
		/*if(gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
		{
			bMergable = FALSE;
		}
		else */
		if((gEvChargeInfo.u8SysAlm == TRUE) || (gEvChargeInfo.u8Gun_Alm[GUN_NO_A] == TRUE))
		{
			bMergable = FALSE;
		}
		//如果小于0,则认为不可用
		else if(g_pRing_A->pNodeMe->fMaxMergeCurr < 1.0)
		{
			bMergable = FALSE;
		}		
	}
	else
	{
		/*if(gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
		{
			bMergable = FALSE;
		}
		else */
		if((gEvChargeInfo.u8SysAlm == TRUE) || (gEvChargeInfo.u8Gun_Alm[GUN_NO_B] == TRUE))
		{
			bMergable = FALSE;
		}
		//如果小于0,则认为不可用
		else if(g_pRing_B->pNodeMe->fMaxMergeCurr < 1.0)
		{
			bMergable = FALSE;
		}
	}

	pRingData->bIsGunMergable = bMergable;
	
	//if((pBMSRough->emSysChargeState >= SYS_STATE_HANDSHAKE) && 
	//	(pBMSRough->emSysChargeState <= SYS_STATE_CHARGE_PAUSED) )
	//{
	//	bMergable = FALSE;
	//}
	//else if((pBMSRough->bChargeHasStarted) && (pBMSRough->emSysChargeState != SYS_STATE_WAITING))
	//{
	//	bMergable = FALSE;
	//}
	//else if((pBMSRough->bChargeHasStarted) && (pBMSRough->nFailTimes > 0)) //重试的时候不能并机/**/
	//{
	//	bMergable = FALSE;
	//}
	//else if(pBMSRough->emSysChargeState == SYS_STATE_WAITING)
	//{
	//	//预约开始之后不能并机, 其余等待状态也不可并机/**/
	//	if((!g_bIsJoinedRing) && (pBMSRough->cfgParams.chargeMode != CHARGE_MODE_BY_TIMER))
	//	{
	//		bMergable = FALSE;
	//	}
	//	else if((time_ring(NULL) >= pBMSRough->cfgParams.tm_TimerStartCharge) && (pBMSRough->cfgParams.chargeMode == CHARGE_MODE_BY_TIMER))
	//	{
	//		bMergable = FALSE;
	//	}
	//}
	//else if(pBMSRough->emSysChargeState == SYS_STATE_NOT_USEABLE)
	//{
	//	switch(pBMSRough->emSysFailReason)
	//	{
	//	case SYS_FAIL_REASON_DOOR_OPEN:
	//	case SYS_FAIL_REASON_EPO:
	//	case SYS_FAIL_REASON_SPD:
	//	case SYS_FAIL_REASON_RECT_COMM_FAIL:
	//	case SYS_FAIL_REASON_OUT_RELAY_SHORTED:
	//	case SYS_FAIL_REASON_AC_INPUT_INVALID:
	//	case SYS_FAIL_REASON_LOCKED:
	//		bMergable = FALSE;
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//pBMSRough->bIsGunMergable = bMergable;
	//return bMergable;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void Calculate_LeftPower_4Me(CAN_RING_DATA *pRing_Rough)
{
	int n;
	//UCHAR ucPreChgOkFlag; //,ucFoundFreeGun;
	float fBMSReqMaxPower, ftemp, fTotalReqCurr_FromBms;
	time_t tmNow = time_ring(NULL);
	float fGetMinPower = 9999.0;
	float fUsedPower = 0.0;
	for(n = 0; n < g_RingAllInfo.nNodeTotal; n++)
	{
		if(pRing_Rough->pNodeMe == (&g_RingAllInfo.allNodes[n]))
		{
			continue;
		}
		if(g_RingAllInfo.allNodes[n].emLockState == _LOCK_AS_MASTER)
		{
			if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
			{
				if(g_RingAllInfo.allNodes[n].tmStartAsMaster > pRing_Rough->pNodeMe->tmStartAsMaster)
				{
					continue;
				}
			}
			fGetMinPower = MIN(fGetMinPower, g_RingAllInfo.allNodes[n].fCalcLeftMaxPower);
			fUsedPower += g_RingAllInfo.allNodes[n].fBMSReqMaxPower;
		}
	}
	float fLeftPwr = MIN(g_RingAllInfo.fAllRingsPwrLmt - fUsedPower, fGetMinPower);
	if(fLeftPwr < 0.5)
	{
		fLeftPwr= 0.0;
	}
	if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
	{
		ftemp = pRing_Rough->master.fTotalOutVolt;

		fTotalReqCurr_FromBms = pRing_Rough->master.fTotalReqCurr_FromBms;
	
		fBMSReqMaxPower = ftemp * fTotalReqCurr_FromBms * 1.02/ 1000.0; //乘以2%的余量，防止未限住/**/

		if(fBMSReqMaxPower > fLeftPwr)
		{
			//TRACE("-----------OVVVVVVVVVVVVVVVVVV------------\n");
			pRing_Rough->pNodeMe->fBMSReqMaxPower = fLeftPwr;
			//根据功率显示，来计算实际可用的最大需求电流/**/
			float fCalcCurrReq = fLeftPwr * 1000.0 / pRing_Rough->master.fTotalOutVolt;
			pRing_Rough->master.fTotalReqCurr = MIN(fCalcCurrReq, fTotalReqCurr_FromBms);
			pRing_Rough->master.fTotalReqCurr = MAX(3.0, pRing_Rough->master.fTotalReqCurr);
			pRing_Rough->master.bIsReceivedAdj = TRUE; //需要限流/**/
		}
		else
		{
			pRing_Rough->pNodeMe->fBMSReqMaxPower = fBMSReqMaxPower;
			pRing_Rough->master.fTotalReqCurr = fTotalReqCurr_FromBms;
		}
	}
	else
	{
		pRing_Rough->pNodeMe->fBMSReqMaxPower = 0.0;
	}

	pRing_Rough->pNodeMe->fCalcLeftMaxPower = fLeftPwr - pRing_Rough->pNodeMe->fBMSReqMaxPower;
	//TRACE("Master[%d], fBMSReq = %.1f, fLeftPwr_Before = %.1f, fLeftPwr_After = %.1f, fCalcReqCurr = %.0f\n", 
	//	pRing_Rough->pNodeMe->byNodeAddress, pRing_Rough->pNodeMe->fBMSReqMaxPower, 
	//	fLeftPwr, pRing_Rough->pNodeMe->fCalcLeftMaxPower, pRing_Rough->master.fTotalReqCurr);
}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE OnIdle(void* pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;

	if(pRing_Rough->bIsGunMergable == FALSE)
	{
		if(pRing_Rough->pNodeMe->emLockState != _LOCK_NOT_AVAILABLE)
		{
			Restore_Default(0, pRing_Rough);
			pRing_Rough->pNodeMe->emLockState = _LOCK_NOT_AVAILABLE;
			return RING_STATE_IDLE;
		}
	}
	else if((pRing_Rough->pNodeMe->emLockState != _LOCK_NO_LOCK) 
		||	((pRing_Rough->pNodeMe->emMergeStatus != STAT_NO_MERGE) && (pRing_Rough->pNodeMe->emMergeStatus != STAT_NOMERGE_GUNIN)))

	{
		if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_SLAVE) 
			&& (time_ring(NULL) - pRing_Rough->slave.tmLastCancel < 10))
		{
			//当从机取消时，要等10s再释放资源，不然如果立即开充，会报108错误/**/
			return RING_STATE_IDLE;
		}
		Restore_Default(0, pRing_Rough);
		return RING_STATE_IDLE;
	}

	//优先判断是否能锁定主机/**/
	if(pRing_Rough->master.bIsReceivedLock)
	{
		Calculate_LeftPower_4Me(pRing_Rough);
		if(pRing_Rough->pNodeMe->fCalcLeftMaxPower < 1.0)
		{
			return RING_STATE_IDLE;
		}
		
		//TRACE("-----------Now goto MASTER MODE-----------\n");
		pRing_Rough->pNodeMe->tmStartAsMaster = dv_RTC.RTC_Counter;  //time_ring(NULL);

		return RING_STATE_PREPARE_BEFRE_CHARGE;
	}
	else if(pRing_Rough->slave.bIsCmd2_AA_Ready)
	{
		if(pRing_Rough->pNodeMe->emLockState != _LOCK_NOT_AVAILABLE)
		{
			//TRACE("-----------Now goto SLAVE MODE-----------\n");
			return RING_STATE_PREPARE_BEFOR_MERGE;
		}
		else
		{
			pRing_Rough->slave.bIsCmd2_AA_Ready = FALSE;
		}
	}
	else if(pRing_Rough->slave.bIsCmd2_CC_Ready)
	{
		if(pRing_Rough->pNodeMe->emLockState != _LOCK_NOT_AVAILABLE)
		{
			//TRACE("-----------Now goto SLAVE MODE-----------\n");
			return RING_STATE_PREPARE_BEFOR_MERGE;
		}
		else
		{
			pRing_Rough->slave.bIsCmd2_CC_Ready = FALSE;
		}
	}

	return RING_STATE_IDLE;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
UINT8 u8A_SpearLOCKDeal_Ring(UINT8 u8IsLock)
{
	__XXX_UNION_VALUE unVal;
	ExGetSigInfo(E_TYPE_SYS, ID_u8HasElec_ALock, &unVal,NULL, BY_SRAM);
	if (unVal.chtemp[0] == 0)
	{
		return TRUE;
	}
	
	if (TRUE == u8IsLock)
	{
		__A_SPEAR_LOCK_Ring();
		__A_SPEAR_LOCK_STABLE();
	}
	else
	{
		__A_SPEAR_UNLOCK_Ring();
		__A_SPEAR_UNLOCK_STABLE();
	}
	return FALSE;
}
UINT8 u8B_SpearLOCKDeal_Ring(UINT8 u8IsLock)
{
	__XXX_UNION_VALUE unVal;

	ExGetSigInfo(E_TYPE_SYS, ID_u8HasElec_BLock, &unVal,NULL, BY_SRAM);
	if (unVal.chtemp[0] == 0)
	{
		gEVBms_2_Info.u8ElecLock_Alm = FALSE;
		return TRUE;
	}

	
	if (TRUE == u8IsLock)
	{
		__B_SPEAR_LOCK_Ring();
		__B_SPEAR_LOCK_STABLE();
	}
	else
	{
		__B_SPEAR_UNLOCK_Ring();
		__B_SPEAR_UNLOCK_STABLE();
	}
	return FALSE;
}
void Send_LockGunMsg2BMS(CAN_RING_DATA *pRing_Rough,BOOL bLock)
{
	UCHAR ucLockCmd;

	if(pRing_Rough->pNodeMe->bIsCC1_GunInCar && g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)
	{
		//TRACE("--- SendLockGunMsgf2BMS !!! bLock = %d pRing_Rough->bIsGunA = %d ---\n",bLock,pRing_Rough->bIsGunA);
		/*SIG_POINTER sigP;
		VAR_VALUE    varVal;
		varVal.enumValue = bLock;
		MAKE_SIGP(sigP, ((pRing_Rough->bIsGunA) ? 101 : 102), SIG_TYPE_CTRL, 9);
		DXI_Internal_SentCtrl(sigP, varVal, FALSE);*/

		if(bLock)
		{
			ucLockCmd = IS_LOCK;
		}
		else
		{
			ucLockCmd = IS_UN_LOCK;
		}

		if(pRing_Rough->bIsGunA)
		{

			u8A_SpearLOCKDeal_Ring(ucLockCmd);
		}
		else
		{
			u8B_SpearLOCKDeal_Ring(ucLockCmd);
		}
	}
	return;
}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnPrepare_BeforeMerge(void *pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;
	time_t tmNow = time_ring(NULL);
	if(pRing_Rough->pNodeMe->emLockState != _LOCK_AS_SLAVE)
	{
		Ctrl_RighDO(pRing_Rough, FALSE);//断开右侧接触器/**/
		pRing_Rough->pCurrState->tm_Enter = tmNow;
	}
	pRing_Rough->pNodeMe->emLockState = _LOCK_AS_SLAVE;
	pRing_Rough->pNodeMe->nLockedMasterID = pRing_Rough->slave.pMaster->byNodeAddress;
	pRing_Rough->pNodeMe->emMergeStatus = STAT_PENDING;

	//将模块电压电流调到很小，然后再开，然后判断准备好没/**/
	float fTargetVolt = pRing_Rough->slave.fBusRealVolt; //跳到跟母排一致电压，再并就比较好了/**/
	if(pRing_Rough->slave.bIsCmd2_AA_Ready)
	{
		if(tmNow - pRing_Rough->pCurrState->tm_Enter >= 3) //增加一点延时
		{
			//TRACE("----------Now adj slave volt, targetVolt = %.1f, now outVolt = %.1f-------------\n", fTargetVolt, pRing_Rough->pNodeMe->fOutVolt);
			pRing_Rough->slave.bIsCmd2_AA_Ready = FALSE;
			AdjSelf_VoltCurr(pRing_Rough, fTargetVolt, 3.0);
			Set_SelfRectOnOff(pRing_Rough, TRUE);
		}
	}
	else if(pRing_Rough->slave.bIsCmd2_CC_Ready)
	{
		if(tmNow - pRing_Rough->pCurrState->tm_Enter >= 3) //增加一点延时
		{
			//TRACE("----------Now SSSSSL adj slave volt, targetVolt = %.1f, now outVolt = %.1f-------------\n", fTargetVolt, pRing_Rough->pNodeMe->fOutVolt);
			pRing_Rough->slave.bIsCmd2_CC_Ready = FALSE;
			Ctrl_SlaveOutDO(pRing_Rough, TRUE);
			Send_LockGunMsg2BMS(pRing_Rough, TRUE);
			AdjSelf_VoltCurr(pRing_Rough, fTargetVolt, 3.0);
			Set_SelfRectOnOff(pRing_Rough, TRUE);
		}
	}


	//模塊採樣比較慢，因此等電壓抬上來差20V就行了/**/
	if((tmNow - pRing_Rough->pCurrState->tm_Enter >= 3) 
		&& (ABS(pRing_Rough->pNodeMe->fOutVolt - fTargetVolt) < 20.0))
	{
		//TRACE("-----------Waited %d sec for preparing slave merging!----------\n", tmNow - pRing_Rough->pCurrState->tm_Enter);
		pRing_Rough->slave.bIsCmd2_AA_Ready = FALSE;
		pRing_Rough->slave.bIsCmd2_00_Ready = FALSE;
		pRing_Rough->slave.bIsCmd2_CC_Ready = FALSE;
		pRing_Rough->slave.bIsCmd3_Ready = FALSE;
		pRing_Rough->slave.bReceivedStop = FALSE;
		pRing_Rough->slave.tmlastCtrlDO = 0;
		return RING_STATE_MERGING;
	}
	else
	{
		//10s仍不行，则不再并了，说明有问题/**/
		if(tmNow - pRing_Rough->pCurrState->tm_Enter >= 12) 
		{
			Send_LockGunMsg2BMS(pRing_Rough, FALSE);
			return RING_STATE_IDLE;
		}
		else
		{
			return RING_STATE_PREPARE_BEFOR_MERGE;
		}
	}
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnMerging(void *pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;
	time_t tmNow = time_ring(NULL);
	pRing_Rough->pNodeMe->emMergeStatus = STAT_MERGING;
	//先判断是否收到CMD3，需要关机/**/
	if(pRing_Rough->slave.bIsCmd3_Ready)
	{
		return RING_STATE_MERGE_CANCELING;
	}
	else if(pRing_Rough->slave.bReceivedStop) //收到BMS那边发过来的请求停机，需要申请退出先/**/
	{
		return RING_STATE_WAITING_CMD3;
	}
	else if(pRing_Rough->master.bIsReceivedLock)
	{
		//收到插抢模式的抢占命令，故要申请退出/**/
		return RING_STATE_WAITING_CMD3;
	}
	else if(pRing_Rough->slave.bIsCmd2_00_Ready)
	{
		pRing_Rough->slave.bIsCmd2_00_Ready = FALSE;
		AdjSelf_VoltCurr(pRing_Rough, pRing_Rough->slave.fReqVolt, 
			pRing_Rough->slave.fReqCurr);
		pRing_Rough->pNodeMe->bIsNeedExtCurr = (pRing_Rough->slave.fReqCurr > pRing_Rough->pNodeMe->fMaxMergeCurr) ? TRUE : FALSE;
		//每5s控制一次/**/
		if(tmNow - pRing_Rough->slave.tmlastCtrlDO > 5)
		{
			pRing_Rough->slave.tmlastCtrlDO = tmNow;
			Set_SelfRectOnOff(pRing_Rough, TRUE);
			//TRACE("-------------Now Ctrl Slave RingDO--->%d-----\n", pRing_Rough->slave.emSetRelayOnOff);
			Ctrl_RighDO(pRing_Rough, (pRing_Rough->slave.emSetRelayOnOff > 0 ? TRUE : FALSE));
			
			/*if(pRing_Rough->pNodeMe->bIsCC1_GunInCar && g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)
			{
				Ctrl_SlaveOutDO(pRing_Rough, TRUE);
			}*/
		}
	}	
	else
	{
		//10s没收到主机命令，则认为通信中断，主动退出/**/
		if((tmNow - pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster > 10) || (pRing_Rough->slave.pMaster->bCommFail))
		{
			//TRACE("---------tmInterv = %d, bCommFail = %d, so quit slave mode now--------\n", 
				//tmNow - pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster, pRing_Rough->slave.pMaster->bCommFail);
			//Set_SelfRectOnOff(pRing_Rough, FALSE);
			//return RING_STATE_MERGE_CANCELING;
			return RING_STATE_WAITING_CMD3;
		}
	}
	return RING_STATE_MERGING;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnPrepare_BeforeCharge(void* pArg )
{
	unsigned char i;

	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA *)pArg;

	
	for(i = 0; i < MAX_RING_SLAVE_NUM; i++)
	{
		pRing_Rough->master.slaves[i] = NULL;
		pRing_Rough->master.u8WaitSlaveKwhReading[i] = 0;
		pRing_Rough->master.fSlaveStart_kWhReading[i] = 0;
		pRing_Rough->master.fSlaveEnd_kWhReading[i] = 0;

		pRing_Rough->master.u8RingSlavesBuf_IsUsed[i] = 0;
		pRing_Rough->master.u8RingSlavesBuf_InHandling = 0;
		pRing_Rough->master.u8RingSlavesBufNo[i] = 0;
	}
	pRing_Rough->master.u8RingSlavesBuf_InHandling = 0;	
	
	pRing_Rough->master.fTotalReqCurr = 0;  //先初始化成0
	pRing_Rough->master.tm_CancelMergeDelay = 0;
	pRing_Rough->master.nSlaveTotal = 0;
	pRing_Rough->master.pLeftEdge = NULL;
	pRing_Rough->master.pRightEdge = NULL;
	pRing_Rough->master.pSlave_InHandling = NULL;
	pRing_Rough->master.tmlastCtrlDO = 0;
	pRing_Rough->master.tm_LastCancelMerging = 0;
	pRing_Rough->master.fSlaveTotalOutKwhReading = 0;
	if(pRing_Rough->pNodeMe->emLockState != _LOCK_AS_MASTER)
	{
		Ctrl_RighDO(pRing_Rough, FALSE);//断开右侧接触器/**/
	}
	pRing_Rough->pNodeMe->emLockState = _LOCK_AS_MASTER;
	pRing_Rough->pNodeMe->emMergeStatus = STAT_NO_MERGE;
	pRing_Rough->master.bIsReceivedLock = FALSE;
	if(pRing_Rough->master.bIsReceivedAdj)
	{
		//pRing_Rough->pNodeMe->tmStartAsMaster = dv_RTC.RTC_Counter;  //time_ring(NULL);
		pRing_Rough->master.tm_FirstEnterCharging = time_ring(NULL);
		return RING_STATE_CHARGING;
	}
	//还没开始充电，故这里在等BMS那边发指令/**/
	return RING_STATE_PREPARE_BEFRE_CHARGE;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
static float GetNodeAvgCurr_2Give(CAN_RING_DATA *pRing_Rough, RING_NODE* pNode)
{
	float fNeed = pRing_Rough->master.fTotalReqCurr;
	float fMaxCurr_2Give = pRing_Rough->pNodeMe->fMaxMergeCurr;
	int n;
	for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
	{
		if(pRing_Rough->master.slaves[n] == NULL)
		{
			continue;
		}
		fMaxCurr_2Give += pRing_Rough->master.slaves[n]->pNode->fMaxMergeCurr;
	}
	//計算這個節點站比，然後就按比例分配就是了/**/
	float fCalcCurr_2Give = fNeed * (pNode->fMaxMergeCurr / fMaxCurr_2Give);

	return fCalcCurr_2Give;
}


/************************************************/
//函数名称:										
//函数功能:		由Master调用								
//更改日期:      								
/************************************************/
static void AdjAll_VoltCurr_4Ring(CAN_RING_DATA *pRing_Rough, BOOL bPreAdj)
{
	time_t tmNow = time_ring(NULL);
	int n;

	//计算主机限功率/**/
	Calculate_LeftPower_4Me(pRing_Rough);

	if(pRing_Rough->master.bIsReceivedAdj || bPreAdj)
	{
		if(pRing_Rough->master.bIsReceivedAdj)
		{
			pRing_Rough->master.bIsReceivedAdj = FALSE;
			if((bPreAdj) && (pRing_Rough->master.fTotalReqCurr > 3.001))
			{
				pRing_Rough->master.fTotalReqCurr -= 3.0; //往回限流一点，防止过流/**/
			}
		}

		if(pRing_Rough->master.nSlaveTotal == 0)
		{
			//TRACE("---------Ring%d, Now no slave, the fReqVolt = %.1f, fReqCurr = %.1f------------\n", pRing_Rough->pNodeMe->byNodeAddress,
			//	pRing_Rough->master.fTotalReqVolt, pRing_Rough->pNodeMe->fMaxMergeCurr);
			pRing_Rough->pNodeMe->emMergeStatus = STAT_NO_MERGE;
			AdjSelf_VoltCurr(pRing_Rough, pRing_Rough->master.fTotalReqVolt, 
				MIN(pRing_Rough->pNodeMe->fMaxMergeCurr, pRing_Rough->master.fTotalReqCurr));
			if((tmNow - pRing_Rough->master.tmlastCtrlDO > 5) || (pRing_Rough->pCurrState != pRing_Rough->pLastState)) //每5s控制一次，本组右侧继电器/**/
			{
				pRing_Rough->master.tmlastCtrlDO = tmNow;
				Ctrl_RighDO(pRing_Rough, FALSE);
				Set_SelfRectOnOff(pRing_Rough, TRUE);
			}
		}
		else
		{
			pRing_Rough->pNodeMe->emMergeStatus = STAT_MERGING;
			//必須按比例給電流，不然可能模塊數不一樣/**/
			//float fSetAvgCurr = //pRing_Rough->master.fTotalReqCurr / (pRing_Rough->master.nSlaveTotal + 1);
			float fSetAvgCurr = GetNodeAvgCurr_2Give(pRing_Rough, pRing_Rough->pNodeMe);
			AdjSelf_VoltCurr(pRing_Rough, pRing_Rough->master.fTotalReqVolt, fSetAvgCurr);
			if((tmNow - pRing_Rough->master.tmlastCtrlDO > 5) || (pRing_Rough->pCurrState != pRing_Rough->pLastState)) //每5s控制一次，本组右侧继电器/**/
			{
				pRing_Rough->master.tmlastCtrlDO = tmNow;

				if(pRing_Rough->master.pRightEdge != NULL) //如果右侧有slave, 则需要吸合继电器/**/
				{
					if((g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)
						&& (pRing_Rough->pNodeMe->right->bIsCC1_GunInCar))
					{
						Ctrl_RighDO(pRing_Rough, FALSE);
					}
					else
					{
						Ctrl_RighDO(pRing_Rough, TRUE);
					}
				}
				else
				{
					Ctrl_RighDO(pRing_Rough, FALSE);//断开右侧接触器/**/
				}
				Set_SelfRectOnOff(pRing_Rough, TRUE);
			}
			for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
			{
				if(pRing_Rough->master.slaves[n] == NULL)
				{
					continue;
				}
				fSetAvgCurr = GetNodeAvgCurr_2Give(pRing_Rough, pRing_Rough->master.slaves[n]->pNode);
				pRing_Rough->master.slaves[n]->fCurr2Sent = fSetAvgCurr;
				pRing_Rough->master.slaves[n]->fVolt2Sent = pRing_Rough->master.fTotalReqVolt;
				if(pRing_Rough->master.slaves[n] == pRing_Rough->master.pRightEdge) //右边沿的不需要控制接触器/**/
				{
					pRing_Rough->master.slaves[n]->emSetRelayOnOff = 0;
				}
				else 
				{
					if(g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)
					{
						if(pRing_Rough->master.slaves[n]->bIsOnRight)
						{
							if(pRing_Rough->master.slaves[n]->pNode->right->bIsCC1_GunInCar)
							{
								pRing_Rough->master.slaves[n]->emSetRelayOnOff = 0;
							}
							else
							{
								pRing_Rough->master.slaves[n]->emSetRelayOnOff = 1;
							}
						}
						else
						{
							if(pRing_Rough->master.slaves[n]->pNode->bIsCC1_GunInCar)
							{
								pRing_Rough->master.slaves[n]->emSetRelayOnOff = 0;
							}
							else
							{
								pRing_Rough->master.slaves[n]->emSetRelayOnOff = 1;
							}
						}					
					}
					else
					{
						pRing_Rough->master.slaves[n]->emSetRelayOnOff = 1;
					}
				}

				Send_RingCmd(pRing_Rough, RING_CMD2, (void*)pRing_Rough->master.slaves[n]);

				
			}
			
		}
	}
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_SALVE* FindEdge_2Cancel(CAN_RING_DATA *pRing_Rough, RING_SALVE* pSlave2Quit)
{
	RING_SALVE* pFoundSlave = NULL;
	if((pSlave2Quit == pRing_Rough->master.pLeftEdge) || 
		(pSlave2Quit == pRing_Rough->master.pRightEdge))
	{
		//来自边上的，还好办点,/**/
		pFoundSlave = pSlave2Quit;
	}
	else
	{
		//在中间的，需要找是左边还是右边, 然后从边沿停起/**/
		RING_SALVE* pSlave = NULL;
		if(pRing_Rough->master.pLeftEdge != NULL)
		{
			pSlave = pRing_Rough->master.pLeftEdge;
			do 
			{
				pSlave = pSlave->right;
				if(pSlave == pSlave2Quit)
				{
					break;
				}
			} while (pSlave != NULL);
		}
		//如果没找到，再找右边/**/
		if(pSlave != NULL)
		{
			pFoundSlave = pRing_Rough->master.pLeftEdge;
		}
		else
		{
			if(pRing_Rough->master.pRightEdge != NULL)
			{
				pSlave = pRing_Rough->master.pRightEdge;
				do 
				{
					pSlave = pSlave->left;
					if(pSlave == pSlave2Quit)
					{
						break;
					}
				} while (pSlave != NULL);
			}
			if(pSlave != NULL)
			{
				pFoundSlave = pRing_Rough->master.pRightEdge;
			}
		}
	}
	//如果没找到，则停自己，一般不可能/**/
	return (pFoundSlave == NULL) ? pSlave2Quit : pFoundSlave;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_SALVE* FindSlave_2Cancel(CAN_RING_DATA *pRing_Rough)
{
	time_t tmNow = time_ring(NULL);
	//看是否有必要释放，因为释放后可能反复，为避免震荡，需要确保真的是需要释放/**/
	RING_SALVE* pSlave = NULL;

	RING_NODE* pOtherGun = (pRing_Rough->bIsGunA) ? 
		pRing_Rough->pNodeMe->right : pRing_Rough->pNodeMe->left;

	//单枪模式
	if(gEvChargeInfo.u8IsSingleGunMode == TRUE)
	{
		pOtherGun = NULL;
	}

	//先看只有左右一边的情况，比较容易/**/
	if((pRing_Rough->master.pLeftEdge == NULL) || (pRing_Rough->master.pRightEdge == NULL))
	{
		//TRACE("---------Only hassssssss one edge--------\n");
		pSlave = (pRing_Rough->master.pLeftEdge == NULL) ?
			pRing_Rough->master.pRightEdge : pRing_Rough->master.pLeftEdge;
	}
	else //再看左右都有的情况，取消并机电流小的/**/
	{
		//TRACE("---------has both egdge, so need to consider which--------\n");
		/*pSlave = (pRing_Rough->master.pRightEdge->pNode->fMaxMergeCurr > pRing_Rough->master.pLeftEdge->pNode->fMaxMergeCurr) ?
			pRing_Rough->master.pLeftEdge : pRing_Rough->master.pRightEdge;*/
		if(pRing_Rough->master.pRightEdge->pNode->fMaxMergeCurr > pRing_Rough->master.pLeftEdge->pNode->fMaxMergeCurr + 5.0)
		{
			pSlave = pRing_Rough->master.pLeftEdge;
		}
		else if(pRing_Rough->master.pLeftEdge->pNode->fMaxMergeCurr > pRing_Rough->master.pRightEdge->pNode->fMaxMergeCurr + 5.0)
		{
			pSlave = pRing_Rough->master.pRightEdge;
		}
		else
		{
			//先取消旁边有并机需求的从机/**/
			if((pRing_Rough->master.pRightEdge->pNode->right->bIsNeedExtCurr))
			{
				pSlave = pRing_Rough->master.pRightEdge;
			}
			else if((pRing_Rough->master.pLeftEdge->pNode->left->bIsNeedExtCurr))
			{
				pSlave = pRing_Rough->master.pLeftEdge;
			}
			else
			{
				//差不多，则遵循中心聚拢法则/**/
				int nIdxxx_Left = ABS(pRing_Rough->master.pLeftEdge->pNode->byNodeAddress - pRing_Rough->pNodeMe->byNodeAddress);
				int nIdxxx_Right = ABS(pRing_Rough->master.pRightEdge->pNode->byNodeAddress - pRing_Rough->pNodeMe->byNodeAddress);

				if(nIdxxx_Left == nIdxxx_Right)
				{
					if(pRing_Rough->master.pLeftEdge->pNode == pOtherGun)
					{
						pSlave = pRing_Rough->master.pRightEdge;
					}
					else
					{
						pSlave = pRing_Rough->master.pLeftEdge;
					}
				}
				else if(nIdxxx_Left > nIdxxx_Right)
				{
					pSlave = pRing_Rough->master.pLeftEdge;
				}
				else
				{
					pSlave = pRing_Rough->master.pRightEdge;
				}
			}
		}
	}

	if(pSlave == NULL)
	{
		return NULL;
	}

	if(tmNow - pSlave->tm_LastStartMerging < 30) //at least merge for 30 sec/**/
	{
		//TRACE("------Cannot cancel ,reason1111111111111111\n");
		return NULL;
	}

	return pSlave;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_SALVE*  FindFreeSlaveBuf(CAN_RING_DATA *pRing_Rough)
{
	unsigned char i;
	RING_SALVE* pRingSlaveBuf = NULL;

	//UCHAR *pRingSlavesBuf_IsUsed;
	for(i = 0; i < MAX_RING_SLAVE_NUM; i++)
	{
		/*if(pRing_Rough->master.slaves[i] == NULL)
		{
			if(pRing_Rough->bIsGunA == TRUE)
			{
				pRingSlaveBuf = &s_RingSlavesBuf[0][i];
			}
			else
			{
				pRingSlaveBuf = &s_RingSlavesBuf[1][i];
			}
			
			break;
		}*/

		if(pRing_Rough->master.u8RingSlavesBuf_IsUsed[i] == 0)
		{
			pRing_Rough->master.u8RingSlavesBuf_InHandling = i;

			if(pRing_Rough->bIsGunA == TRUE)
			{
				pRingSlaveBuf = &s_RingSlavesBuf[0][i];
			}
			else
			{
				pRingSlaveBuf = &s_RingSlavesBuf[1][i];
			}	
			break;
		}
	}
	return pRingSlaveBuf;	
}

BOOL FindSlave_2Merge(CAN_RING_DATA *pRing_Rough)
{
	
	BOOL bIsOnRight = FALSE;

	RING_NODE* pSalve_2Lock = NULL;
	RING_NODE* pOtherGun = (pRing_Rough->bIsGunA) ? 
		pRing_Rough->pNodeMe->right : pRing_Rough->pNodeMe->left;
	//先找潜在的左右可用节点，然后看哪边比较适合并机/**/
	RING_NODE* pPreLeft = (pRing_Rough->master.pLeftEdge == NULL) ? 
		pRing_Rough->pNodeMe->left : pRing_Rough->master.pLeftEdge->pNode->left;
	RING_NODE* pPreRight = (pRing_Rough->master.pRightEdge == NULL) ? 
		pRing_Rough->pNodeMe->right : pRing_Rough->master.pRightEdge->pNode->right;

	//单枪模式
	if(gEvChargeInfo.u8IsSingleGunMode == TRUE)
	{
		pOtherGun = NULL;
	}
	
	if(pPreLeft->emLockState != _LOCK_NO_LOCK)
	{
		pSalve_2Lock = (pPreRight->emLockState == _LOCK_NO_LOCK) ? pPreRight : NULL;
		bIsOnRight = TRUE;
	}
	else if(pPreRight->emLockState != _LOCK_NO_LOCK)
	{
		pSalve_2Lock = pPreLeft;
	}
	else
	{
		//单桩-双枪
		if((g_RingAllInfo.nNodeTotal == 2)&&(gEvChargeInfo.u8IsSingleGunMode == FALSE))
		{
			if(pRing_Rough->bIsGunA) 
			{
				pSalve_2Lock = pPreRight;
				bIsOnRight = TRUE;
			}
			else
			{
				pSalve_2Lock = pPreLeft;
			}
			
		}
		//同一枪，已到环边界
		else if(pPreLeft == pPreRight)  
		{
			//避免远端并机/**/
			if(pPreLeft->byNodeAddress > pRing_Rough->pNodeMe->byNodeAddress)
			{
				pSalve_2Lock = pPreRight;
				bIsOnRight = TRUE;
			}
			else
			{
				pSalve_2Lock = pPreLeft;
			}
		}
		//两者都空闲，则看下其各自边上有没有需要并机的，如果需要，则优先使用另一边的/**/
		else if(pPreLeft->left->bIsNeedExtCurr)
		{
			pSalve_2Lock = pPreRight;
			bIsOnRight = TRUE;
		}
		else if(pPreRight->right->bIsNeedExtCurr)
		{
			pSalve_2Lock = pPreLeft;
		}
		else
		{
			//两边各自都空闲的话，优先使用本桩内的/**/			
			if(pPreRight == pOtherGun)
			{
				pSalve_2Lock = pPreRight;
				bIsOnRight = TRUE;
			}
			else if(pPreLeft == pOtherGun)
			{
				pSalve_2Lock = pPreLeft;
			}
			else
			{
				//原则1：避免远端并机/**/
				if(pPreLeft->byNodeAddress > pRing_Rough->pNodeMe->byNodeAddress)
				{
					pSalve_2Lock = pPreRight;
					bIsOnRight = TRUE;
				}
				else if(pPreRight->byNodeAddress < pRing_Rough->pNodeMe->byNodeAddress)
				{
					pSalve_2Lock = pPreLeft;
				}
				else
				{
					//原则2：向中心聚拢法则/**/
					int nIdxxx_Left = ABS(pPreLeft->byNodeAddress- pRing_Rough->pNodeMe->byNodeAddress);
					int nIdxxx_Right = ABS(pPreRight->byNodeAddress - pRing_Rough->pNodeMe->byNodeAddress);
					if(nIdxxx_Left >= nIdxxx_Right)
					{
						pSalve_2Lock = pPreRight;
						bIsOnRight = TRUE;
					}
					else
					{
						pSalve_2Lock = pPreLeft;
					}
				}
			}
		}
	}
	if(pSalve_2Lock == NULL)
	{
		return FALSE;
	}
	else if(pSalve_2Lock->bCommFail)
	{
		//防止并过机后，该枪没置位，一直尝试并机/**/
		pSalve_2Lock->emLockState = _LOCK_NOT_AVAILABLE;
		return FALSE;
	}

	//float fSetAvgCurr = pRing_Rough->master.fTotalReqCurr / (pRing_Rough->master.nSlaveTotal + 2);
	//pRing_Rough->master.pSlave_InHandling = NEW(RING_SALVE, 1);//&pRing_Rough->master.slaves[pRing_Rough->master.nSlaveTotal];
	pRing_Rough->master.pSlave_InHandling = FindFreeSlaveBuf(pRing_Rough);
	
	if(pRing_Rough->master.pSlave_InHandling == NULL)
	{
		return FALSE;
	}

	pRing_Rough->master.pSlave_InHandling->pNode = pSalve_2Lock;
	pRing_Rough->master.pSlave_InHandling->bIsOnRight =  bIsOnRight;
	
	return TRUE;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnCharging(void* pArg)
{
	int n;	
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;

	pRing_Rough->master.pSlave_InHandling = NULL;
	time_t tmNow = time_ring(NULL);
	
	//先调压限流/**/
	AdjAll_VoltCurr_4Ring(pRing_Rough, 
		(BOOL)(pRing_Rough->master.fTotalOutCurr > (pRing_Rough->master.fTotalReqCurr * 1.05))); //增加了過流的處理，過流立即限流/**/

	//等3秒后再判断是否并机
	if((tmNow - pRing_Rough->master.tm_FirstEnterCharging) < 3)
	{
		return RING_STATE_CHARGING;
	}

	//判断是否通信中断或者收到CMD4取消并机命令，这个处理就比较复杂，需要逐步取消/**/
	float fExistedMaxCurr = 0.0; //计算当前已经并机的电流总数/**/
	fExistedMaxCurr = pRing_Rough->pNodeMe->fMaxMergeCurr;
	for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
	{
		if(pRing_Rough->master.slaves[n] == NULL)
		{
			continue;
		}
		fExistedMaxCurr += pRing_Rough->master.slaves[n]->pNode->fMaxMergeCurr;

		if((pRing_Rough->master.slaves[n]->bIsCmd4Ready) 
			|| (pRing_Rough->master.slaves[n]->pNode->bCommFail)
			|| ((pRing_Rough->master.slaves[n]->pNode->emLockState != _LOCK_AS_SLAVE)   //该从机自己返回到了IDLE态且通讯恢复正常了
				&& (pRing_Rough->master.slaves[n]->pNode->emMergeStatus == STAT_NO_MERGE
					|| pRing_Rough->master.slaves[n]->pNode->emMergeStatus == STAT_NOMERGE_GUNIN)))
		{  
			//TRACE("----------Nowwwwwwwwwwwww, bFail = %d------------\n", pRing_Rough->master.slaves[n]->pNode->bCommFail);
			RING_SALVE* pSlave = pRing_Rough->master.slaves[n];
			pRing_Rough->master.pSlave_InHandling = FindEdge_2Cancel(pRing_Rough, pSlave);
			Send_RingCmd(pRing_Rough, RING_CMD3, (void*)pRing_Rough->master.pSlave_InHandling);
			pRing_Rough->master.pSlave_InHandling->tm_LastCancelMerging = time_ring(NULL);
			return RING_STATE_WAITING_CANCELING;
		}
	}

	//再判断是否还需要并机和取消并机/**/
	if(pRing_Rough->master.fTotalReqCurr >= fExistedMaxCurr + g_RingAllInfo.fRingStartMergeCurr)
	{
		pRing_Rough->pNodeMe->bIsNeedExtCurr = TRUE;
		//判断有没有环回来，则不需要继续找/**/
		if(pRing_Rough->master.nSlaveTotal + 1 == g_RingAllInfo.nOnlineTotal)
		{
			return RING_STATE_CHARGING;
		}
		else if(time_ring(NULL) - pRing_Rough->master.tm_LastCancelMerging < 30) //上次有Node取消，不能马上又并，这里是为了防止震荡/**/
		{
			return RING_STATE_CHARGING;
		}
		
		//return (FindSlave_2Merge(pRing_Rough)) ? RING_STATE_WAITING_MERGING : RING_STATE_CHARGING;		
		if(FindSlave_2Merge(pRing_Rough))
		{
			pRing_Rough->master.pSlave_InHandling->fVolt2Sent = pRing_Rough->master.fTotalReqVolt;
			pRing_Rough->master.pSlave_InHandling->fCurr2Sent = 3.0;//fSetAvgCurr;
			//Send_RingCmd(pRing_Rough, RING_CMD2_AA, (void*)pRing_Rough->master.pSlave_InHandling);
			Send_RingCmd(pRing_Rough, ((pRing_Rough->master.pSlave_InHandling->pNode->bIsCC1_GunInCar)&&(g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)) ? RING_CMD2_CC : RING_CMD2_AA,
				(void*)pRing_Rough->master.pSlave_InHandling);
			pRing_Rough->master.pSlave_InHandling->tm_LastStartMerging = time_ring(NULL);
			pRing_Rough->master.tm_CancelMergeDelay = time_ring(NULL);
			return RING_STATE_WAITING_MERGING;
		}
		
	}
	else if(pRing_Rough->master.fTotalReqCurr < fExistedMaxCurr)
	{
		//20190423 如果刚加一个从机，禁止马上取消
		if((time_ring(NULL) - pRing_Rough->master.tm_CancelMergeDelay) < 60) 
		{
			//
		}
		else
		{
		pRing_Rough->pNodeMe->bIsNeedExtCurr = FALSE;
		//TRACE("----------Now reqCurr decreaded %.1f-->%.1f, nToal = %d----------\n",
		//	pRing_Rough->master.fTotalReqCurr, fExistedMaxCurr, pRing_Rough->master.nSlaveTotal);
		//需求降低，则找到一个释放并机/**/
		if(pRing_Rough->master.nSlaveTotal == 0)
		{
			return RING_STATE_CHARGING;
		}
		else
		{
			RING_SALVE* pSlave = FindSlave_2Cancel(pRing_Rough);
			if(pSlave == NULL)
			{
				return RING_STATE_CHARGING;
			}
			else
			{
				float fIncreasedMergeCurr = fExistedMaxCurr - pSlave->pNode->fMaxMergeCurr;
				if(pRing_Rough->master.fTotalReqCurr > fIncreasedMergeCurr) //取消并机后仍然需要再并，故不取消/**/
				{
					//TRACE("------Cannot cancel ,reason22222222222, fIncreasedMergeCurr = %.1f, me = %.1f \n",
					//	fIncreasedMergeCurr, pSlave->pNode->fMaxMergeCurr);
					return RING_STATE_CHARGING;
				}
			}
			pRing_Rough->master.pSlave_InHandling = pSlave;
			Send_RingCmd(pRing_Rough, RING_CMD3, (void*)pRing_Rough->master.pSlave_InHandling);
			pRing_Rough->master.pSlave_InHandling->tm_LastCancelMerging = tmNow;
			return RING_STATE_WAITING_CANCELING;
			}
		}
	}

	return RING_STATE_CHARGING;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
static void Add_ASlave(CAN_RING_DATA *pRing_Rough)
{
	pRing_Rough->master.pSlave_InHandling->left = NULL;
	pRing_Rough->master.pSlave_InHandling->right = NULL;	
	pRing_Rough->master.pSlave_InHandling->bIsCmd4Ready = FALSE;
	pRing_Rough->master.pSlave_InHandling->tm_LastCancelMerging = 0;
	//判断左右/**/
	if(pRing_Rough->master.pSlave_InHandling->bIsOnRight)
	{
		if(pRing_Rough->master.pRightEdge != NULL)
		{
			if(pRing_Rough->master.pRightEdge->pNode->right == pRing_Rough->master.pSlave_InHandling->pNode)
			{
				pRing_Rough->master.pRightEdge->right = pRing_Rough->master.pSlave_InHandling;
				pRing_Rough->master.pSlave_InHandling->left = pRing_Rough->master.pRightEdge;
				pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
			}
			else if(pRing_Rough->master.pSlave_InHandling->pNode->right == pRing_Rough->pNodeMe)
			{
				//TRACE("-------------Found Node --> pLeftEdge---------------\n");
				pRing_Rough->master.pLeftEdge = pRing_Rough->master.pSlave_InHandling;
			}
		}
		else if(pRing_Rough->master.pSlave_InHandling->pNode->left == pRing_Rough->pNodeMe)
		{
			//TRACE("-------------Found Node --> pRightEdge---------------\n");
			pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
		}
	}
	else
	{
		if(pRing_Rough->master.pLeftEdge != NULL)
		{
			if(pRing_Rough->master.pLeftEdge->pNode->left == pRing_Rough->master.pSlave_InHandling->pNode)
			{
				pRing_Rough->master.pLeftEdge->left = pRing_Rough->master.pSlave_InHandling;
				pRing_Rough->master.pSlave_InHandling->right = pRing_Rough->master.pLeftEdge;
				pRing_Rough->master.pLeftEdge = pRing_Rough->master.pSlave_InHandling;
			}
			else if(pRing_Rough->master.pRightEdge != NULL)
			{
				if(pRing_Rough->master.pRightEdge->pNode->right == pRing_Rough->master.pSlave_InHandling->pNode)
				{
					pRing_Rough->master.pRightEdge->right = pRing_Rough->master.pSlave_InHandling;
					pRing_Rough->master.pSlave_InHandling->left = pRing_Rough->master.pRightEdge;
					pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
				}
			}
			else if(pRing_Rough->master.pSlave_InHandling->pNode->left == pRing_Rough->pNodeMe)
			{
				//TRACE("-------------Found Node --> pRightEdge---------------\n");
				pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
			}
		}
		else if(pRing_Rough->master.pSlave_InHandling->pNode->right == pRing_Rough->pNodeMe)
		{
			//TRACE("-------------Found Node --> pLeftEdge---------------\n");
			pRing_Rough->master.pLeftEdge = pRing_Rough->master.pSlave_InHandling;
		}
	}

	//if(pRing_Rough->master.pLeftEdge != NULL)
	//{
	//	if(pRing_Rough->master.pLeftEdge->pNode->left == pRing_Rough->master.pSlave_InHandling->pNode)
	//	{
	//		pRing_Rough->master.pLeftEdge->left = pRing_Rough->master.pSlave_InHandling;
	//		pRing_Rough->master.pSlave_InHandling->right = pRing_Rough->master.pLeftEdge;
	//		pRing_Rough->master.pLeftEdge = pRing_Rough->master.pSlave_InHandling;
	//	}
	//	else if(pRing_Rough->master.pRightEdge != NULL)
	//	{
	//		if(pRing_Rough->master.pRightEdge->pNode->right == pRing_Rough->master.pSlave_InHandling->pNode)
	//		{
	//			pRing_Rough->master.pRightEdge->right = pRing_Rough->master.pSlave_InHandling;
	//			pRing_Rough->master.pSlave_InHandling->left = pRing_Rough->master.pRightEdge;
	//			pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
	//		}
	//	}
	//	else if(pRing_Rough->master.pSlave_InHandling->pNode->left == pRing_Rough->pNodeMe)
	//	{
	//		//TRACE("-------------Found Node --> pRightEdge---------------\n");
	//		pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
	//	}
	//}
	//else if(pRing_Rough->master.pRightEdge != NULL)
	//{
	//	if(pRing_Rough->master.pRightEdge->pNode->right == pRing_Rough->master.pSlave_InHandling->pNode)
	//	{
	//		pRing_Rough->master.pRightEdge->right = pRing_Rough->master.pSlave_InHandling;
	//		pRing_Rough->master.pSlave_InHandling->left = pRing_Rough->master.pRightEdge;
	//		pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
	//	}
	//	else if(pRing_Rough->master.pSlave_InHandling->pNode->right == pRing_Rough->pNodeMe)
	//	{
	//		//TRACE("-------------Found Node --> pLeftEdge---------------\n");
	//		pRing_Rough->master.pLeftEdge = pRing_Rough->master.pSlave_InHandling;
	//	}
	//}
	//else //左右都沒有/**/
	//{
	//	if(pRing_Rough->master.pSlave_InHandling->pNode->left == pRing_Rough->pNodeMe)
	//	{
	//		//TRACE("-------------Found Node --> pRightEdge---------------\n");
	//		pRing_Rough->master.pRightEdge = pRing_Rough->master.pSlave_InHandling;
	//	}
	//	else if(pRing_Rough->master.pSlave_InHandling->pNode->right == pRing_Rough->pNodeMe)
	//	{
	//		//TRACE("-------------Found Node --> pLeftEdge---------------\n");
	//		pRing_Rough->master.pLeftEdge = pRing_Rough->master.pSlave_InHandling;
	//	}
	//}

	//memmove(&pRing_Rough->master.slaves[pRing_Rough->master.nSlaveTotal], pRing_Rough->master.pSlave_InHandling, sizeof(RING_SALVE));
	//SAFELY_DELETE(pRing_Rough->master.pSlave_InHandling);
	pRing_Rough->master.slaves[pRing_Rough->master.nSlaveTotal] = pRing_Rough->master.pSlave_InHandling;

	if(pRing_Rough->master.u8RingSlavesBuf_InHandling < MAX_RING_SLAVE_NUM)
	{
		pRing_Rough->master.u8RingSlavesBufNo[pRing_Rough->master.nSlaveTotal] = pRing_Rough->master.u8RingSlavesBuf_InHandling;
		pRing_Rough->master.u8RingSlavesBuf_IsUsed[pRing_Rough->master.u8RingSlavesBuf_InHandling] = TRUE;
	}	

	pRing_Rough->master.pSlave_InHandling = NULL;
	pRing_Rough->master.nSlaveTotal++;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnWaitMerging(void* pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;

	pRing_Rough->pNodeMe->emMergeStatus = STAT_PENDING;
	time_t tmNow = time_ring(NULL);
	RING_NODE* pNodeToMerge = pRing_Rough->master.pSlave_InHandling->pNode;
	if(pNodeToMerge->emLockState == _LOCK_NO_LOCK)
	{
		//Send_RingCmd(pRing_Rough, RING_CMD2_AA, (void*)pRing_Rough->master.pSlave_InHandling);
		Send_RingCmd(pRing_Rough, ((pNodeToMerge->bIsCC1_GunInCar) && (g_RingAllInfo.emRingMergeMode == RING_MODE_ONECAR_MULTIGUN)) ? RING_CMD2_CC : RING_CMD2_AA, 
			(void*)pRing_Rough->master.pSlave_InHandling);

		if(tmNow - pRing_Rough->master.pSlave_InHandling->tm_LastStartMerging >= 5)
		{
			return RING_STATE_CHARGING; //说明这个组没准备好，则继续充电/**/
		}
		else
		{
			AdjAll_VoltCurr_4Ring(pRing_Rough, FALSE);
		}
	}
	else if((pNodeToMerge->emLockState == _LOCK_AS_SLAVE) 
		&& (pNodeToMerge->nLockedMasterID == pRing_Rough->pNodeMe->byNodeAddress)) //被本桩锁定/**/
	{
		if(pNodeToMerge->emMergeStatus != STAT_MERGING)
		{
			if(tmNow - pRing_Rough->master.pSlave_InHandling->tm_LastStartMerging < 17)
			{
				AdjAll_VoltCurr_4Ring(pRing_Rough, TRUE);
				return RING_STATE_WAITING_MERGING;//继续等直到该桩准备好为止/**/
			}
			else
			{
				//5s仍未准备就绪，则直接放弃/**/
				/*Send_RingCmd(pRing_Rough, RING_CMD3, (void*)pRing_Rough->master.pSlave_InHandling);
				pRing_Rough->master.pSlave_InHandling->tm_LastCancelMerging = time_ring(NULL);
				return RING_STATE_WAITING_CANCELING;*/
				//不用发停止命令了，slave那边会自己超时停止/**/
				return RING_STATE_CHARGING;
			}
		}
		else
		{
			Add_ASlave(pRing_Rough);
			if(pRing_Rough->master.nSlaveTotal == 1) //只有一个时，需要控制下本枪继电器/**/
			{
				pRing_Rough->master.tmlastCtrlDO = 0;
			}
			return RING_STATE_CHARGING; //准备好了，直接进充电，发送调压限流即可/**/
		}
	}
	else
	{
		return RING_STATE_CHARGING; //说明这个组没准备好，则继续充电/**/
	}

	return RING_STATE_WAITING_MERGING;
}

static void RemoveASlave(CAN_RING_DATA *pRing_Rough)
{
	if(pRing_Rough->master.pSlave_InHandling == pRing_Rough->master.pLeftEdge)
	{
		pRing_Rough->master.pLeftEdge = pRing_Rough->master.pLeftEdge->right;
		if(pRing_Rough->master.pLeftEdge != NULL)
		{
			pRing_Rough->master.pLeftEdge->left = NULL;
		}
	}
	else if(pRing_Rough->master.pSlave_InHandling == pRing_Rough->master.pRightEdge)
	{
		pRing_Rough->master.pRightEdge = pRing_Rough->master.pRightEdge->left;
		if(pRing_Rough->master.pRightEdge != NULL)
		{
			pRing_Rough->master.pRightEdge->right = NULL;
		}
	}
	int n = 0;
	int nFoundIdx = 0;
	for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
	{
		if(pRing_Rough->master.pSlave_InHandling == pRing_Rough->master.slaves[n])
		{
			nFoundIdx = n;
			if(pRing_Rough->master.pSlave_InHandling->pNode->bCommFail) //通信中断时给它复位/**/
			{
				pRing_Rough->master.pSlave_InHandling->pNode->emLockState = _LOCK_NO_LOCK;
				pRing_Rough->master.pSlave_InHandling->pNode->emMergeStatus = STAT_NO_MERGE;
			}
			//SAFELY_DELETE(pRing_Rough->master.pSlave_InHandling);
			pRing_Rough->master.slaves[n] = NULL;
			pRing_Rough->master.u8RingSlavesBuf_IsUsed[pRing_Rough->master.u8RingSlavesBufNo[n]] = 0; //释放
			break;
		}
	}
	if(pRing_Rough->master.nSlaveTotal > 0)
	{
		if(nFoundIdx < pRing_Rough->master.nSlaveTotal - 1)
		{
			for(n = nFoundIdx; n < pRing_Rough->master.nSlaveTotal - 1; n++)
			{				
				pRing_Rough->master.slaves[n] = pRing_Rough->master.slaves[n+1]; 
				pRing_Rough->master.u8RingSlavesBufNo[n] = pRing_Rough->master.u8RingSlavesBufNo[n+1];
			}
			pRing_Rough->master.slaves[pRing_Rough->master.nSlaveTotal - 1] = NULL;
			pRing_Rough->master.u8RingSlavesBufNo[pRing_Rough->master.nSlaveTotal - 1] = 0;
		}
		pRing_Rough->master.nSlaveTotal--;
	}
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnWaitCmd3(void* pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;
	time_t tmNow = time_ring(NULL);
	pRing_Rough->slave.bReceivedStop = FALSE;
	pRing_Rough->pNodeMe->emMergeStatus = STAT_PENDING;
	Send_RingCmd(pRing_Rough, RING_CMD4, NULL);
	if(pRing_Rough->slave.bIsCmd3_Ready)
	{
		if(pRing_Rough->slave.bIsCmd2_00_Ready)
		{
			pRing_Rough->slave.bIsCmd2_00_Ready = FALSE;
			/*AdjSelf_VoltCurr(pRing_Rough, pRing_Rough->slave.fReqVolt, 
				pRing_Rough->slave.fReqCurr);*/
			Set_SelfRectOnOff(pRing_Rough, FALSE);
		}
		return RING_STATE_MERGE_CANCELING;
	}
	else
	{
		//15s 仍没等到，可能通信中断了,不管它，直接退出/**/
		if((tmNow - pRing_Rough->pCurrState->tm_Enter > 15) || (pRing_Rough->slave.pMaster->bCommFail))
		{
			Set_SelfRectOnOff(pRing_Rough, FALSE);
			return RING_STATE_MERGE_CANCELING;
		}
	}
	return RING_STATE_WAITING_CMD3;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnMergeCanceling(void* pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;
	time_t tmNow = time_ring(NULL);
	pRing_Rough->pNodeMe->emMergeStatus = STAT_PENDING;

	if(pRing_Rough->slave.bIsCmd2_00_Ready)
	{
		pRing_Rough->slave.bIsCmd2_00_Ready = FALSE;
		/*AdjSelf_VoltCurr(pRing_Rough, pRing_Rough->slave.fReqVolt, 
			pRing_Rough->slave.fReqCurr);*/
		Set_SelfRectOnOff(pRing_Rough, FALSE);
	}

	if(pRing_Rough->slave.bIsCmd3_Ready)
	{
		pRing_Rough->slave.bIsCmd3_Ready = FALSE;
		Set_SelfRectOnOff(pRing_Rough, FALSE);
	}

	if((pRing_Rough->pNodeMe->fOutCurr < 5.0))
	{
		Ctrl_RighDO(pRing_Rough, FALSE);
		Ctrl_SlaveOutDO(pRing_Rough, FALSE);
		Send_LockGunMsg2BMS(pRing_Rough, FALSE);

		pRing_Rough->slave.tmLastCancel = tmNow;
		return RING_STATE_IDLE;
	}
	else if(tmNow - pRing_Rough->pCurrState->tm_Enter >= 3) //3s仍停不下来，则不管它直接退出/**/
	{
		Set_SelfRectOnOff(pRing_Rough, FALSE);
		Ctrl_RighDO(pRing_Rough, FALSE);
		if(pRing_Rough->slave.pMaster->bCommFail) //通信中断时给它复位/**/
		{
			pRing_Rough->slave.pMaster->emLockState = _LOCK_NO_LOCK;
			pRing_Rough->slave.pMaster->emMergeStatus = STAT_NO_MERGE;
		}
		if(pRing_Rough->pNodeMe->bIsCC1_GunInCar)
		{
			Ctrl_SlaveOutDO(pRing_Rough, FALSE);
			Send_LockGunMsg2BMS(pRing_Rough, FALSE);
		}

		pRing_Rough->slave.tmLastCancel = tmNow;
		return RING_STATE_IDLE;
	}
	return RING_STATE_MERGE_CANCELING;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnWaitCanceling(void* pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;
	pRing_Rough->pNodeMe->emMergeStatus = STAT_PENDING;
	pRing_Rough->master.pSlave_InHandling->bIsCmd4Ready = FALSE;
	time_t tmNow = time_ring(NULL);
	RING_NODE* pNodeToMerge = pRing_Rough->master.pSlave_InHandling->pNode;
	//TRACE("-----------Now waiting Node[%d] to cancel Merge..............\n", pNodeToMerge->byNodeAddress);
	if((pNodeToMerge->emLockState == _LOCK_AS_SLAVE) && 
		(pNodeToMerge->nLockedMasterID == pRing_Rough->pNodeMe->byNodeAddress)) //被本桩锁定/**/
	{
		if(tmNow - pRing_Rough->master.pSlave_InHandling->tm_LastCancelMerging < 3)
		{
			Send_RingCmd(pRing_Rough, RING_CMD3, (void*)pRing_Rough->master.pSlave_InHandling);
			AdjAll_VoltCurr_4Ring(pRing_Rough, FALSE);
			return RING_STATE_WAITING_CANCELING;//继续等直到该桩安全断开为止/**/
		}
		else
		{
			//Do Emergency stop???
			RemoveASlave(pRing_Rough);
			pRing_Rough->master.tm_LastCancelMerging = time_ring(NULL);
			return RING_STATE_CHARGING;
		}
	}
	else
	{
		RemoveASlave(pRing_Rough);
		pRing_Rough->master.tm_LastCancelMerging = time_ring(NULL);
		return RING_STATE_CHARGING;
	}
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
static BOOL Handle_PendingSlave(CAN_RING_DATA *pRing_Rough)
{
	BOOL bHandSlaveStopped = FALSE;
	if(pRing_Rough->master.pSlave_InHandling != NULL) //如果它之前正等待并机，则需要删除/**/
	{
		int n;
		for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
		{
			if(pRing_Rough->master.slaves[n] == pRing_Rough->master.pSlave_InHandling)
			{
				bHandSlaveStopped = TRUE;
				break;
			}
		}
		if(!bHandSlaveStopped)
		{
			if((pRing_Rough->master.pSlave_InHandling->pNode->emLockState == _LOCK_AS_SLAVE) && 
				(pRing_Rough->master.pSlave_InHandling->pNode->nLockedMasterID == pRing_Rough->pNodeMe->byNodeAddress))
			{
				if(time_ring(NULL) - pRing_Rough->pCurrState->tm_Enter < 5)
				{
					Send_RingCmd(pRing_Rough, RING_CMD3, (void*)pRing_Rough->master.pSlave_InHandling);
				}
				else
				{
					//SAFELY_DELETE(pRing_Rough->master.pSlave_InHandling);
					pRing_Rough->master.pSlave_InHandling = NULL;
					bHandSlaveStopped = TRUE;
				}
			}
			else
			{
				//SAFELY_DELETE(pRing_Rough->master.pSlave_InHandling);
				pRing_Rough->master.pSlave_InHandling = NULL;
				bHandSlaveStopped = TRUE;
			}
		}
	}
	else
	{
		bHandSlaveStopped = TRUE;
	}
	return bHandSlaveStopped;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
RING_STATE_MACHINE_STATE	OnStop(void* pArg)
{
	CAN_RING_DATA *pRing_Rough = (CAN_RING_DATA*)pArg;
	time_t tmNow = time_ring(NULL);
	pRing_Rough->pNodeMe->emMergeStatus = STAT_PENDING;
	if((pRing_Rough->pNodeMe->fOutCurr > 5.0) || 
		(pRing_Rough->pNodeMe->fOutVolt > 50.0))
	{
		Set_SelfRectOnOff(pRing_Rough, FALSE); //关自己, 别人关了一遍就够了，不用发多次/**/
	}
	//如果右侧桩已退出，则里面关掉自己的借继电器/**/
	if((pRing_Rough->pNodeMe->right->emLockState == _LOCK_AS_SLAVE) &&
		(pRing_Rough->pNodeMe->right->nLockedMasterID == pRing_Rough->pNodeMe->byNodeAddress))
	{
		//do nothing
	}
	else
	{
		static time_t tmLastCtrl = 0;
		if(tmNow - tmLastCtrl >= 2)
		{
			tmLastCtrl = tmNow;
			Ctrl_RighDO(pRing_Rough, FALSE);
		}
	}

	//if(pRing_Rough->master.bIsRecievedStop)
	//{
	//	pRing_Rough->master.bIsRecievedStop = FALSE;
	//	Stop_AllSlaveRects(pRing_Rough);
	//}
	//send CMD3 to all slaves
	int n;
	int nStoppedSlaves = 0;
	for( n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
	{
		if((pRing_Rough->master.slaves[n]->pNode->emLockState == _LOCK_AS_SLAVE) && 
			(pRing_Rough->master.slaves[n]->pNode->nLockedMasterID == pRing_Rough->pNodeMe->byNodeAddress))
		{
			Send_RingCmd(pRing_Rough, RING_CMD3, (void*)pRing_Rough->master.slaves[n]);
		}
		else
		{
			nStoppedSlaves++;
		}
	}
	//有可能停止时队列还有从机等待并机/**/
	BOOL bHandedPending = Handle_PendingSlave(pRing_Rough);

	//所有都已经停掉/**/
	if((nStoppedSlaves == pRing_Rough->master.nSlaveTotal) && (bHandedPending) && (pRing_Rough->master.bIsReceivedStoppOK))
	{
		pRing_Rough->master.bIsRecievedStop = FALSE;
		return RING_STATE_IDLE;
	}
	else
	{
		if(tmNow - pRing_Rough->pCurrState->tm_Enter >= 10) //10s仍不能断的话，那就不管了/**/
		{
			pRing_Rough->master.bIsRecievedStop = FALSE;
			return RING_STATE_IDLE;
		}
	}
	return RING_STATE_STOPING;
}

/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void Task_Ring_Proc(void* pdata)
{
	UNUSED(pdata);

	RING_STATE_MACHINE_STATE emNewState;
	CAN_RING_DATA* pRing_Rough = NULL;	


	UINT8 u8Buf[8];
	int nMaxReadTimes;
	UINT8 u8Idx;
	BYTE bySrcAddr, byCmdNo, byDestAddr;

	gSelfTest.fSetTaskID(TASK_ID_RING);
	OSTimeDly(500);
	gSelfTest.fSetTaskID(TASK_ID_RING);
	OSTimeDly(500);
	gSelfTest.fSetTaskID(TASK_ID_RING);
	OSTimeDly(500);
	gSelfTest.fSetTaskID(TASK_ID_RING);

	Ring_Initialize();

	for(;;)
	{	
		gSelfTest.fSetTaskID(TASK_ID_RING);

		if ((gEvChargeInfo.u8TEST_MODE_FLAG == TRUE)
			||(gEvChargeInfo.u8TestMode[GUN_NO_A] == TRUE)
			|| (gEvChargeInfo.u8TestMode[GUN_NO_B] == TRUE))
		{
			g_RingAllInfo.bIsRingCfgErr = TRUE;
			OSTimeDly(500);
			continue;
		}		

		//桩序号设置不满足要求，所以不启动环网
		if(gs_CANComm.bIsRingEnabled == FALSE)
		{
			OSTimeDly(500);
			continue;
		}

		//20190807
		if((gEvChargeInfo.u8ACInputContactorCtrlStat == DO_OFF)	
			|| (gEvChargeInfo.u8ACInputContactorCloseAlmDelayFlag == TRUE))
		{
			OSTimeDly(50);
			continue;
		}

		//模块方面还不满足，所以不启动环网
		if((gs_CANComm.bNeedReConfig == TRUE)
			|| (gs_CANComm.nScanedRectNum == 0)
			|| (gs_CANComm.nOnlineTotalGrp == 0))
		{
			g_RingAllInfo.bIsRingCfgErr = TRUE;
			OSTimeDly(500);
			continue;
		}


		if(g_RingAllInfo.bIsRingCfgErr)
		{			
			//TRACE("------------Now get Ring Cfg Err, do need Samp after 5 sec------------\n");
			CreateRingNodes();

			//环网配置成功,初始化处理
			if(g_RingAllInfo.bIsRingCfgErr == FALSE)
			{
				GetRingDataFromCAN3();  //清空环网CAN消息
				g_pRing_A->pCurrState = &s_RingData_A.stateMachineLoop[RING_STATE_IDLE];
				g_pRing_A->pCurrState->emState = RING_STATE_IDLE;
				g_pRing_A->pCurrState->tm_Enter = time_ring(NULL);

				g_pRing_B->pCurrState = &s_RingData_B.stateMachineLoop[RING_STATE_IDLE];
				g_pRing_B->pCurrState->emState = RING_STATE_IDLE;
				g_pRing_B->pCurrState->tm_Enter = time_ring(NULL);

				Restore_Default(1, g_pRing_A);

				//双枪模式才处理
				if(gEvChargeInfo.u8IsSingleGunMode == FALSE)
				{
					Restore_Default(1, g_pRing_B);
				}
			}

			OSTimeDly(500);
			continue;
		}

		
		//Get cmd or data from CAN bus periodly
		nMaxReadTimes = GetRingDataFromCAN3();		
		if((nMaxReadTimes > 0) && (nMaxReadTimes <= CAN_3_MAX_BUFF))
		{
			//for (u8Idx = 0; u8Idx < CAN_3_MAX_BUFF; u8Idx++)
			for (u8Idx = 0; u8Idx < nMaxReadTimes; u8Idx++)
			{
				if(RxMsg_Ring[u8Idx].ExtId != 0)
				{
					bySrcAddr = (BYTE)(RxMsg_Ring[u8Idx].ExtId & 0xFF);
					byCmdNo = (BYTE)((RxMsg_Ring[u8Idx].ExtId >> 16) & 0x3F);
					byDestAddr = (BYTE)((RxMsg_Ring[u8Idx].ExtId >> 8) & 0xFF);

					if(byCmdNo == RING_CMD1)
					{
						HalCan_Msg_u32Data_2_u8data(u8Buf, &RxMsg_Ring[u8Idx] );  //s_u8CanMsgDataBuf
						Parse_RingCmd1(bySrcAddr, u8Buf);
					}
					else if((R_PROTO_RING_BROADCAST == byDestAddr) && (RING_CMD6 == byCmdNo))
					{
						HalCan_Msg_u32Data_2_u8data(u8Buf, &RxMsg_Ring[u8Idx]);
						Parse_RingCmd6(bySrcAddr, u8Buf);
					}
					else if(byDestAddr == g_pRing_A->pNodeMe->byNodeAddress)
					{
						HalCan_Msg_u32Data_2_u8data(u8Buf, &RxMsg_Ring[u8Idx]);
						Prase_RingCmd234(g_pRing_A, bySrcAddr, byCmdNo, u8Buf);
					}
					else if((byDestAddr == g_pRing_B->pNodeMe->byNodeAddress)&&(gEvChargeInfo.u8IsSingleGunMode == FALSE)) //双枪模式才处理
					{
						HalCan_Msg_u32Data_2_u8data(u8Buf, &RxMsg_Ring[u8Idx]);
						Prase_RingCmd234(g_pRing_B, bySrcAddr, byCmdNo, u8Buf);
					}

					RxMsg_Ring[u8Idx].ExtId = 0;
				}
			}

		}

		//TRACEX("--------Ring[%d], get msg in the queue[%d]-----------\n", pRing_Rough->pNodeMe->byNodeAddress, nMaxReadTimes);

		for (u8Idx = 0; u8Idx < 2; u8Idx++)
		{
			if(u8Idx == 1)
			{
				pRing_Rough = g_pRing_B;
				if(gEvChargeInfo.u8IsSingleGunMode == TRUE) //单枪模式不处理
				{
					continue;
				}
			}
			else
			{
				pRing_Rough = g_pRing_A;
			}

			//Ring_Sample(pRing_Rough);
			//JudgeIfGunMergeable(pRing_Rough);
			Refresh_SelfNode(pRing_Rough);
			Judge_NodeCommFail(pRing_Rough);

			//状态机循环/**/
			//需要停时在这里紧急处理/**/
			if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER) && 
				(pRing_Rough->master.bIsRecievedStop))
			{
				pRing_Rough->pCurrState = &pRing_Rough->stateMachineLoop[RING_STATE_STOPING];
				if(pRing_Rough->pCurrState != pRing_Rough->pLastState)
				{
					pRing_Rough->pCurrState->tm_Enter = time_ring(NULL);
				}
			}
			else if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_SLAVE) && 
				(pRing_Rough->slave.bIsCmd3_Ready))
			{
				pRing_Rough->pCurrState = &pRing_Rough->stateMachineLoop[RING_STATE_MERGE_CANCELING];
				if(pRing_Rough->pCurrState != pRing_Rough->pLastState)
				{
					pRing_Rough->pCurrState->tm_Enter = time_ring(NULL);
				}
			}

			pRing_Rough->pLastState = pRing_Rough->pCurrState;

			//MutexLock(pRing_Rough->hMutex);

			emNewState = pRing_Rough->pCurrState->pfnAction(pRing_Rough);

			//MutexUnlock(pRing_Rough->hMutex);

			pRing_Rough->pCurrState = &pRing_Rough->stateMachineLoop[emNewState];

			//TRACE("-----------Ring[%d], now State:%s-->%s-----\n",pRing_Rough->pNodeMe->byNodeAddress, 
			//	pRing_Rough->pLastState->szStateName, pRing_Rough->pCurrState->szStateName);
			if(pRing_Rough->pCurrState != pRing_Rough->pLastState)
			{
				pRing_Rough->pCurrState->tm_Enter = time_ring(NULL);
			}

		} // END: for (u8Idx = 0; u8Idx < 2; u8Idx++)

		//300ms循环一次应该够了/**/
		OSTimeDly(150);
	}
}