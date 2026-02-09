/*==========================================================================*
 *    Copyright(c) 2017,InfyPower Co., Ltd.
 *                     ALL RIGHTS RESERVED
 * 
 *  FILENAME : 
 *  CREATOR  : Jimmy Wu                   DATE: 2017-08-22
 *  VERSION  : V1.00
 *  PURPOSE  : 
 *
 *  HISTORY  :
 *==========================================================================*/


#include "can_ring.h"

static CAN_RECT_SAMPLER_DATA	*s_pRect_Rough = NULL;
RING_ALL_INFO			g_RingAllInfo; //两个线程公用数据集/**/

static CAN_RING_DATA			s_RingData_A;
static CAN_RING_DATA			s_RingData_B;

//for rect sample
CAN_RING_DATA*			g_pRing_A;
CAN_RING_DATA*			g_pRing_B;



static BOOL CreateRingNodes(void)
{
	g_RingAllInfo.nNodeTotal = 2*s_pRect_Rough->nTotalRingStub;
	if((g_RingAllInfo.nNodeTotal < 4) || (g_RingAllInfo.nNodeTotal > MAX_SUPPORTED_RING_NODE)) //最少需要2个桩/**/
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

	//找自己/**/
	g_RingAllInfo.bIsRingCfgErr = FALSE;
	TRACE("------bySelfAddress = %d, nTotalRingStub = %d----------\n", s_pRect_Rough->bySelfAddress, s_pRect_Rough->nTotalRingStub);
	if((s_pRect_Rough->bySelfAddress > s_pRect_Rough->nTotalRingStub) || (s_pRect_Rough->bySelfAddress < 1))
	{
		g_RingAllInfo.bIsRingCfgErr = TRUE;
		//return FALSE;
	}
	else
	{
		BOOL bFindA = FALSE;
		BOOL bFindB = FALSE;
		for(n = 0; n < g_RingAllInfo.nNodeTotal; n++)
		{
			if(g_RingAllInfo.allNodes[n].byNodeAddress == (BYTE)(2*s_pRect_Rough->bySelfAddress - 1))
			{
				bFindA = TRUE;
				s_RingData_A.pNodeMe = &g_RingAllInfo.allNodes[n];
			}
			else if(g_RingAllInfo.allNodes[n].byNodeAddress == (BYTE)(2*s_pRect_Rough->bySelfAddress))
			{
				bFindB = TRUE;
				s_RingData_B.pNodeMe = &g_RingAllInfo.allNodes[n];
			}
		}
		if((!bFindB) || (!bFindA))
		{
			g_RingAllInfo.bIsRingCfgErr = TRUE;
		}
	}

	return TRUE;
}

static void Parse_RingCmd1(BYTE bySrcAddr, BYTE* pbyData)
{
	if(bySrcAddr == s_RingData_A.pNodeMe->byNodeAddress)
	{
		//本桩发出去的报文，不可能自己还收到了，因此是ID冲突/**/
		s_RingData_A.bSelfIDConflict = TRUE;
		return; //不解析本桩内的CMD1/**/
	}
	else if(bySrcAddr == s_RingData_B.pNodeMe->byNodeAddress)
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
	pNode->tmLastRefresh = time(NULL);
	pNode->bCommFail = FALSE;

	pNode->fOutVolt = (float)(CAN_StringToUShort(&pbyData[0])) * 0.1;
	pNode->fOutCurr = (float)(CAN_StringToUShort(&pbyData[2])) * 0.1;
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
	case 0x00:
	default:
		pNode->emMergeStatus = STAT_NO_MERGE;
		break;
	}
	BYTE byExtStatus = (pbyData[7] & 0x0F);
	pNode->bIsNeedExtCurr = (byExtStatus > 0) ? TRUE : FALSE;
}

static void Prase_RingCmd234(CAN_RING_DATA* pRing_Rough, BYTE bySrcAddr, BYTE byCmdNo, BYTE* pbyData)
{
	RING_NODE* pNodeFrom = pRing_Rough->pNodeMe;
	time_t tmNow = time(NULL);
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
	TRACEX("---------Ring%d WW received CMD%d from Node%d-------------\n", pRing_Rough->pNodeMe->byNodeAddress,  byCmdNo, bySrcAddr);
	if(byCmdNo == RING_CMD2)
	{
		float fReqVolt= (float)(CAN_StringToUShort(&pbyData[0])) * 0.1;
		float fReqCurr= (float)(CAN_StringToUShort(&pbyData[2])) * 0.1;
		float fBusRealVolt= (float)(CAN_StringToUShort(&pbyData[4])) * 0.1;
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
		int n;
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
}

void Set_SelfRectOnOff(CAN_RING_DATA* pRing_Rough, BOOL bOn)
{
	SIG_ENUM emOn = bOn ? 1 : 0;
	SetGrp_OnOff_4Ring(pRing_Rough->pNodeMe->byNodeAddress, s_pRect_Rough, &emOn);
	//if(pRing_Rough->bIsGunA)
	//{
	//	SetGrp_OnOff(GRP_A, s_pRect_Rough, &emOn);
	//}
	//else
	//{
	//	SetGrp_OnOff(GRP_B, s_pRect_Rough, &emOn);
	//}
}

void AdjSelf_VoltCurr(CAN_RING_DATA* pRing_Rough, float fTargetVolt, float fTargetCurr)
{
	if(pRing_Rough->bIsGunA)
	{
		s_pRect_Rough->fGrp1_VoltSet = fTargetVolt;
		fTargetCurr = MIN(s_pRect_Rough->rawDataGrp[GRP_A].fMax_OutCurr, fTargetCurr);
		SetGrp_AdjVoltCurr_4Ring(GRP_A, s_pRect_Rough, (void*)&fTargetCurr);
		//if(s_pRect_Rough->rawDataGrp[GRP_A].fOutVolt < 50.0)
		//{
		//	SIG_ENUM emOn = 1;
		//	SetGrp_OnOff(GRP_A, s_pRect_Rough, &emOn);
		//}
	}
	else
	{
		s_pRect_Rough->fGrp2_VoltSet = fTargetVolt;
		fTargetCurr = MIN(s_pRect_Rough->rawDataGrp[GRP_B].fMax_OutCurr, fTargetCurr);
		SetGrp_AdjVoltCurr_4Ring(GRP_B, s_pRect_Rough, (void*)&fTargetCurr);
		//if(s_pRect_Rough->rawDataGrp[GRP_B].fOutVolt < 50.0)
		//{
		//	SIG_ENUM emOn = 1;
		//	SetGrp_OnOff(GRP_B, s_pRect_Rough, &emOn);
		//}
	}

}

void Stop_AllSlaveRects(CAN_RING_DATA* pRing_Rough)
{
	SIG_ENUM emOn = 0;
	//First stop myself/**/
	SetGrp_OnOff_4Ring(pRing_Rough->pNodeMe->byNodeAddress, s_pRect_Rough, &emOn);
	//then stop all slaves/**/
	int n;
	for(n = 0; n < pRing_Rough->master.nSlaveTotal; n++)
	{
		if(pRing_Rough->master.slaves[n] == NULL)
		{
			continue;
		}
		SetGrp_OnOff_4Ring( pRing_Rough->master.slaves[n]->pNode->byNodeAddress, s_pRect_Rough, &emOn);
	}
	//also stop the to-be-handling slave
	if(pRing_Rough->master.pSlave_InHandling != NULL)
	{
		SetGrp_OnOff_4Ring( pRing_Rough->master.pSlave_InHandling->pNode->byNodeAddress, s_pRect_Rough, &emOn);
	}
}

void Send_RingCmd(CAN_RING_DATA* pRing_Rough, BYTE byCmdNo, void* pWhere2Send)
{
	BYTE pbySendBuf[CAN_FRAME_LEN];
	memset(pbySendBuf, 0 , CAN_FRAME_LEN);
	BYTE bySendCmd = (RING_CMD2_AA == byCmdNo) ? 0x02 : byCmdNo;
	BYTE* byData = pbySendBuf + 5;
	CAN_RING_DATA *pOtherRIng_Rough = (pRing_Rough == g_pRing_A) ? g_pRing_B : g_pRing_A; //如果发给本桩另一把枪的命令，不用走CAN，直接处理即可/**/

	if(byCmdNo == RING_CMD1)
	{
		//Stuff first 29 bits in the send frame
		CAN_StuffHead(R_PRPTO_DEVNO_0C, bySendCmd, R_PROTO_RING_BROADCAST, pRing_Rough->pNodeMe->byNodeAddress,
			pbySendBuf);
		//Now gather data
		CAN_UShortToString((USHORT)(pRing_Rough->pNodeMe->fOutVolt * 10), byData);
		CAN_UShortToString((USHORT)(pRing_Rough->pNodeMe->fOutCurr * 10), byData + 2);
		byData[4] = (BYTE)(pRing_Rough->pNodeMe->fMaxMergeCurr);
		if(pRing_Rough->pNodeMe->emLockState == _LOCK_NO_LOCK)
		{
			byData[5] = 0;
		}
		else if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
		{
			byData[5] = 99;
		}
		else if(pRing_Rough->pNodeMe->emLockState == _LOCK_NOT_AVAILABLE)
		{
			byData[5] = 255;
		}
		else //1~32
		{
			byData[5] = (BYTE)pRing_Rough->pNodeMe->nLockedMasterID;
		}	
		switch(pRing_Rough->pNodeMe->emMergeStatus)
		{
		case STAT_PENDING:
			byData[6] = 0x0A;
			break;
		case STAT_MERGING:
			byData[6] = 0x0B;
			break;
		default:
			byData[6] = 0x00;
			break;
		}
		byData[7] = (pRing_Rough->pNodeMe->bIsNeedExtCurr) ? 0x01 : 0x00;
		TRACE("-----------Ring%d, send CMD1[%02X %02X %02X %02X %02X %02X %02X %02X], emLock = %d----------------\n",
			pRing_Rough->pNodeMe->byNodeAddress, byData[0], byData[1], byData[2], byData[3],
			byData[4], byData[5], byData[6], byData[7], pRing_Rough->pNodeMe->emLockState);
	}
	else if((byCmdNo == RING_CMD2_AA) || (RING_CMD2 == byCmdNo))
	{
		//Stuff first 29 bits in the send frame
		CAN_StuffHead(R_PRPTO_DEVNO_0C, bySendCmd, ((RING_SALVE*)pWhere2Send)->pNode->byNodeAddress, pRing_Rough->pNodeMe->byNodeAddress,
			pbySendBuf);
		//Now gather data
		CAN_UShortToString((USHORT)(((RING_SALVE*)pWhere2Send)->fVolt2Sent * 10), byData);
		CAN_UShortToString((USHORT)(((RING_SALVE*)pWhere2Send)->fCurr2Sent* 10), byData + 2);
		CAN_UShortToString((USHORT)(pRing_Rough->master.fTotalOutVolt * 10), byData + 4);
		BOOL bNeedSetRelay = (((RING_SALVE*)pWhere2Send)->emSetRelayOnOff > 0) ? TRUE : FALSE;
		byData[6] = (byCmdNo == RING_CMD2_AA) ? 0xAA : (bNeedSetRelay ? 0x01 : 0x00);
		RING_SALVE*pSlave = (RING_SALVE*)pWhere2Send;
	
		TRACE("-----------Now send CMD02[%02X]------>slave.Node=%d, other gun node=%d---------------\n", byData[6],
			pSlave->pNode->byNodeAddress, pOtherRIng_Rough->pNodeMe->byNodeAddress);

		if(pSlave->pNode->byNodeAddress == pOtherRIng_Rough->pNodeMe->byNodeAddress) //another gun in th
		{
			TRACE("----------Now Send CMD02 to the Other gun within this Charger, emRelay = %d---------\n", bNeedSetRelay);
			//直接让另一组来解析/**/
			Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, RING_CMD2, byData);
			return;
		}
		else
		{
			TRACE("----------Now Node[%d]--->Write CMD02 to Gun[%d], oTher[%d]---------\n", pRing_Rough->pNodeMe->byNodeAddress, 
				pOtherRIng_Rough->pNodeMe->byNodeAddress, pSlave->pNode->byNodeAddress);
		}
	}
	else if(RING_CMD3 == byCmdNo)
	{
		//Stuff first 29 bits in the send frame
		CAN_StuffHead(R_PRPTO_DEVNO_0C, bySendCmd, ((RING_SALVE*)pWhere2Send)->pNode->byNodeAddress, pRing_Rough->pNodeMe->byNodeAddress,
			pbySendBuf);
		//Now gather data
		byData[0] = 0x01;
		RING_SALVE*pSlave = (RING_SALVE*)pWhere2Send;
		if(pSlave->pNode == pOtherRIng_Rough->pNodeMe) //another gun in th
		{
			//直接让另一组来解析/**/
			Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, byCmdNo, byData);
			return;
		}
	}
	else //CMD4
	{
		//Stuff first 29 bits in the send frame
		CAN_StuffHead(R_PRPTO_DEVNO_0C, bySendCmd, pRing_Rough->slave.pMaster->byNodeAddress, pRing_Rough->pNodeMe->byNodeAddress,
			pbySendBuf);
		//Now gather data
		byData[0] = 0x01;
		if(pRing_Rough->slave.pMaster == pOtherRIng_Rough->pNodeMe) //another gun in th
		{
			//直接让另一组来解析/**/
			Prase_RingCmd234(pOtherRIng_Rough, pRing_Rough->pNodeMe->byNodeAddress, byCmdNo, byData);
			return;
		}
	}
	
	//Write_CAN3(s_pRect_Rough->iCanDevHanle, (void *)pbySendBuf, CAN_FRAME_LEN);
	write(s_pRect_Rough->iCanDevHanle, (void *)pbySendBuf, CAN_FRAME_LEN); //20171229, Jimmy发现，环网上会发生mutex竞争现象，导致通信异常/**/

}


static void Refresh_SelfNode(CAN_RING_DATA* pRing_Rough)
{
	BOOL bIsGunA = (pRing_Rough == &s_RingData_A) ? TRUE : FALSE;
	pRing_Rough->pNodeMe->fMaxMergeCurr = bIsGunA ? gs_CANComm.GrpData[GRP_A].fMax_OutCurr : 
		gs_CANComm.GrpData[GRP_B].fMax_OutCurr;
	pRing_Rough->pNodeMe->fOutVolt = bIsGunA ? gs_CANComm.GrpData[GRP_A].fOutVolt : 
		gs_CANComm.GrpData[GRP_B].fOutVolt;
	pRing_Rough->pNodeMe->fOutCurr = bIsGunA ? gs_CANComm.GrpData[GRP_A].fOutCurr : 
		gs_CANComm.GrpData[GRP_B].fOutCurr;

	pRing_Rough->pNodeMe->tmLastRefresh = time_ring(NULL);
	pRing_Rough->pNodeMe->bCommFail = FALSE;

	//将状态CMD1发出去/**/
	Send_RingCmd(pRing_Rough, RING_CMD1, NULL);

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
			if(pRing_Rough->master.slaves[n]->pNode->fOutVolt > fMaxVolt)
			{
				fMaxVolt = pRing_Rough->master.slaves[n]->pNode->fOutVolt;
			}
			fSumCurr += pRing_Rough->master.slaves[n]->pNode->fOutCurr;
		}
		pRing_Rough->master.fTotalOutVolt = fMaxVolt;
		pRing_Rough->master.fTotalOutCurr = fSumCurr;
	}
}

static void Judge_NodeCommFail(CAN_RING_DATA* pRing_Rough)
{
	int n;
	time_t tmNow = time(NULL);
	BOOL bAllOK = TRUE;
	g_RingAllInfo.nOnlineTotal = 0;
	for(n = 0; n < g_RingAllInfo.nNodeTotal; n++)
	{
		//5s没收到报文，则认为是通信中断/**/
		if(tmNow - g_RingAllInfo.allNodes[n].tmLastRefresh > 5)
		{
			TRACE("------------Node[%d] Comm Failed!!!!!!!-----\n", g_RingAllInfo.allNodes[n].byNodeAddress);
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

static ULONG MakeLockCode(RING_NODE *pNode)
{
	ULONG nCode = 0;
	switch (pNode->emLockState)
	{
	case _LOCK_NO_LOCK:
		nCode = 0;
		break;
	case _LOCK_AS_MASTER:
		nCode = 99;
		break;
	case _LOCK_NOT_AVAILABLE:
		nCode = 255;
		break;
	case _LOCK_AS_SLAVE:
	default:
		nCode = (ULONG)pNode->nLockedMasterID;
		break;
	}
	return nCode;
}

static void Ring_StuffChn(CAN_RING_DATA* pRing_Rough)
{
	if(s_pRect_Rough->enumProc == NULL)
	{
		return;
	}
	
	SAMPLER_ENUM_PROC stuffProc = s_pRect_Rough->enumProc;
	VAR_VALUE varRawVal;
	int nIdx = (pRing_Rough->bIsGunA) ? (s_pRect_Rough->nMaxRectNumToSample) : (s_pRect_Rough->nMaxRectNumToSample + 1);
	int n;
	int kk;

	TRACE("----------Now Begin Stuff Ring Data For[%d]---------\n", s_pRect_Rough->pAddrInfo[nIdx].iDevUnitID);

	//SelfInfo
	ULONG nLockCode = MakeLockCode(pRing_Rough->pNodeMe);
	RECT_SAMPVAL_MAP sigPubMp[] = 
	{
		{(void*)&pRing_Rough->pNodeMe->byNodeAddress,	RECT_RAW_BYTE,		1},
		{(void*)&g_RingAllInfo.nNodeTotal,				RECT_RAW_INT,		2},
		{(void*)&g_RingAllInfo.nOnlineTotal,				RECT_RAW_INT,		3},
		{(void*)&nLockCode,							RECT_RAW_ENUM,		4},
		{(void*)&pRing_Rough->pNodeMe->emMergeStatus,	RECT_RAW_ENUM,		5},
		{(void*)&pRing_Rough->pNodeMe->fOutVolt,		RECT_RAW_FLOAT,		6},
		{(void*)&pRing_Rough->pNodeMe->fOutCurr,		RECT_RAW_FLOAT,		7},
		{(void*)&pRing_Rough->master.fTotalOutVolt,		RECT_RAW_FLOAT,		8},
		{(void*)&pRing_Rough->master.fTotalOutCurr,		RECT_RAW_FLOAT,		9},
		{(void*)&pRing_Rough->bSelfIDConflict,			RECT_RAW_BOOL,		10},
	};
	for(n = 0; n < ITEM_OF(sigPubMp); n++)
	{
		stuffProc(Get_FVal_FromMapItem(&sigPubMp[n]),
			s_pRect_Rough->nSamplerId, s_pRect_Rough->pAddrInfo[nIdx].iRelevLstIdx,
			sigPubMp[n].iSigID);
	}
	//Other Node info
	for(n = 0; n < MAX_SUPPORTED_RING_NODE; n++)
	{
		nLockCode = MakeLockCode(&g_RingAllInfo.allNodes[n]);
		RECT_SAMPVAL_MAP sigSingMp[] = 
		{
			{(void*)&nLockCode,							RECT_RAW_ENUM,		11 + n*10},
			{(void*)&g_RingAllInfo.allNodes[n].emMergeStatus,	RECT_RAW_ENUM,		12 + n*10},
			{(void*)&g_RingAllInfo.allNodes[n].fMaxMergeCurr,	RECT_RAW_FLOAT,		13 + n*10},
			{(void*)&g_RingAllInfo.allNodes[n].fOutVolt,		RECT_RAW_FLOAT,		14 + n*10},
			{(void*)&g_RingAllInfo.allNodes[n].fOutCurr,		RECT_RAW_FLOAT,		15 + n*10},
		};
		for(kk = 0; kk < ITEM_OF(sigSingMp); kk++)
		{
			stuffProc(Get_FVal_FromMapItem(&sigSingMp[kk]),
				s_pRect_Rough->nSamplerId, s_pRect_Rough->pAddrInfo[nIdx].iRelevLstIdx,
				sigSingMp[kk].iSigID);
		}
	}
}

#define MAX_WAIT_RING_INTERV	20
static int Ring_Sample(CAN_RING_DATA* pRing_Rough)
{
	if(Mutex_Lock(s_pRect_Rough->hSamplerLock, MAX_WAIT_RING_INTERV) == ERR_MUTEX_OK)
	{
		int nReadLen = 0;
		int n = 0;
		//refresh other node from CAN msg
		BYTE *byRecBuff = s_pRect_Rough->abyRcvBuf;
		memset(byRecBuff, 0, MAX_READ_BUFF_SIZE * CAN_FRAME_LEN);

		nReadLen = read(s_pRect_Rough->iCanDevHanle, (void *)byRecBuff, (ssize_t)(CAN_FRAME_LEN * MAX_READ_BUFF_SIZE));
		if(nReadLen > CAN_FRAME_LEN * MAX_READ_BUFF_SIZE)
		{
			nReadLen = CAN_FRAME_LEN * MAX_READ_BUFF_SIZE;
		}
		TRACEX("----------Ring%d, now I read %d CANmsg---------\n", pRing_Rough->pNodeMe->byNodeAddress, nReadLen);
		if(nReadLen >= CAN_FRAME_LEN)
		{
			CAN_RING_DATA* pOtherRing = (pRing_Rough == &s_RingData_A) ? (&s_RingData_B) : (&s_RingData_A);
			for(n = 0;n < nReadLen / CAN_FRAME_LEN;n++)
			{
				A_CAN_RECT_FRAME recFrame;
				memcpy(recFrame.byRawData, byRecBuff + n*CAN_FRAME_LEN, CAN_FRAME_LEN);
				CAN_ParseHead(&recFrame);
				//判断地址和命令号就行/**/
				if(recFrame.byDevNo == R_PRPTO_DEVNO_0C)
				{
					if((R_PROTO_RING_BROADCAST == recFrame.byDestAddr) && (RING_CMD1 == recFrame.byCmdNo))
					{
						Parse_RingCmd1(recFrame.bySrcAddr, recFrame.byData);
					}
					else if(recFrame.byDestAddr == pRing_Rough->pNodeMe->byNodeAddress)
					{
						Prase_RingCmd234(pRing_Rough, recFrame.bySrcAddr, recFrame.byCmdNo, recFrame.byData);
					}
					else if(recFrame.byDestAddr == pOtherRing->pNodeMe->byNodeAddress)
					{
						//需要将数据放进队列里/**/
						Queue_Put(pOtherRing->hqCANMsgFromRect, &recFrame, FALSE);
					}
				}
			}
		}
		Mutex_Unlock(s_pRect_Rough->hSamplerLock);
	}

	//Now refresh me
	Refresh_SelfNode(pRing_Rough);

	Judge_NodeCommFail(pRing_Rough);

	Ring_StuffChn(pRing_Rough);

	return ERR_OK;
}
#define RING_MUTEX_WAIT	100
static void __ThreadEntry_OfRing(void* pArgs)
{
#define MAX_WAIT_RING_QUEUE		10
	CAN_RING_DATA* pRing_Rough = (CAN_RING_DATA*)pArgs;	
	SIG_POINTER sigP;
	VAR_VALUE    varVal;
	A_CAN_RECT_FRAME recFrame;
	int nMaxReadTimes = 0;
	//while(1) //这里还没想到办法如何优雅退出/**/
	while(!DXI_GetAppExitFlag())
	{
		//RUN_THREAD_HEARTBEAT();
		RunThread_Heartbeat(pRing_Rough->hThread);
		GetRingCfg();
		if(g_RingAllInfo.bIsRingCfgErr)
		{
			TRACE("------------Now get Ring Cfg Err, do need Samp after 5 sec------------\n");
			//GetRingCfg();
			CreateRingNodes();
			//设置配置错误/**/
			MAKE_SIGP(sigP, 1, SIG_TYPE_SAMP, 404);
			varVal.enumValue = 1; 
			DXI_Internal_SentCtrl(sigP, varVal, FALSE);
			//Queue_Empty(s_RingData_A.hqCANMsgFromRect);
			//Queue_Empty(s_RingData_B.hqCANMsgFromRect);
			Sleep(5000);
			continue;
		}

		//取消配置错误/**/
		MAKE_SIGP(sigP, 1, SIG_TYPE_SAMP, 404);
		varVal.enumValue = 0; 
		DXI_Internal_SentCtrl(sigP, varVal, FALSE);

		//Get cmd or data from CAN bus periodly
		nMaxReadTimes = 0;
		do 
		{
			if(Queue_Get(pRing_Rough->hqCANMsgFromRect, &recFrame, FALSE, MAX_WAIT_RING_QUEUE) == ERR_QUEUE_OK)
			{
				if(RING_CMD1 == recFrame.byCmdNo)
				{
					Parse_RingCmd1(recFrame.bySrcAddr, recFrame.byData);
				}
				else
				{
					Prase_RingCmd234(pRing_Rough, recFrame.bySrcAddr, recFrame.byCmdNo, recFrame.byData);
				}
			}
			else
			{
				break;
			}
			nMaxReadTimes++;
		} while (nMaxReadTimes < MAX_CANMSG_FROM_RECT);

		TRACEX("--------Ring[%d], get msg in the queue[%d]-----------\n", pRing_Rough->pNodeMe->byNodeAddress, nMaxReadTimes);

		Ring_Sample(pRing_Rough);
		//状态机循环/**/
		//需要停时在这里紧急处理/**/
		if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER) && 
			(pRing_Rough->master.bIsRecievedStop))
		{
			pRing_Rough->pCurrState = &pRing_Rough->stateMachineLoop[RING_STATE_STOPING];
			if(pRing_Rough->pCurrState != pRing_Rough->pLastState)
			{
				pRing_Rough->pCurrState->tm_Enter = time(NULL);
			}
		}
		else if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_SLAVE) && 
			(pRing_Rough->slave.bIsCmd3_Ready))
		{
			pRing_Rough->pCurrState = &pRing_Rough->stateMachineLoop[RING_STATE_MERGE_CANCELING];
			if(pRing_Rough->pCurrState != pRing_Rough->pLastState)
			{
				pRing_Rough->pCurrState->tm_Enter = time(NULL);
			}
		}

		pRing_Rough->pLastState = pRing_Rough->pCurrState;
		if (Mutex_Lock(pRing_Rough->hMutex, RING_MUTEX_WAIT) != ERR_MUTEX_OK)
		{
			continue;
		}
		RING_STATE_MACHINE_STATE emNewState = pRing_Rough->pCurrState->pfnAction(pRing_Rough);
		Mutex_Unlock(pRing_Rough->hMutex);
		pRing_Rough->pCurrState = &pRing_Rough->stateMachineLoop[emNewState];

		TRACE("-----------Ring[%d], now State:%s-->%s-----\n",pRing_Rough->pNodeMe->byNodeAddress, 
			pRing_Rough->pLastState->szStateName, pRing_Rough->pCurrState->szStateName);
		if(pRing_Rough->pCurrState != pRing_Rough->pLastState)
		{
			pRing_Rough->pCurrState->tm_Enter = time(NULL);
		}

		//300ms循环一次应该够了/**/
		Sleep(300);
	}
}

static void Restore_Default(CAN_RING_DATA *pRing_Rough)
{
	if(g_RingAllInfo.bIsRingCfgErr)
	{
		return;
	}
	//Clean me
	pRing_Rough->pNodeMe->emLockState = _LOCK_NO_LOCK;
	pRing_Rough->pNodeMe->emMergeStatus = STAT_NO_MERGE;
	pRing_Rough->pNodeMe->bIsNeedExtCurr = FALSE;
	//Clean master
	pRing_Rough->master.bIsReceivedAdj = FALSE;
	pRing_Rough->master.bIsReceivedLock = FALSE;
	pRing_Rough->master.bIsRecievedStop = FALSE;
	pRing_Rough->master.bIsReceivedStoppOK = FALSE;
	int n;
	for(n = 0; n < MAX_RING_SLAVE_NUM; n++)
	{
		SAFELY_DELETE(pRing_Rough->master.slaves[n]);
	}
	pRing_Rough->master.nSlaveTotal = 0;
	pRing_Rough->master.pLeftEdge = NULL;
	pRing_Rough->master.pRightEdge = NULL;

	//Clean slave
	pRing_Rough->slave.bIsCmd2_00_Ready = FALSE;
	pRing_Rough->slave.bIsCmd2_AA_Ready = FALSE;
	pRing_Rough->slave.bIsCmd3_Ready = FALSE;
	pRing_Rough->slave.pMaster = NULL;
	pRing_Rough->slave.bReceivedStop = FALSE;
	pRing_Rough->slave.tm_LasReceive_Cmd_FromMaster = 0;
	pRing_Rough->slave.tmLastCancel = 0;
	
	pRing_Rough->bSelfIDConflict = FALSE;

	Set_SelfRectOnOff(pRing_Rough, FALSE);
	Ctrl_RighDO(pRing_Rough, FALSE);
}


#define  INIT_RING_STATE_MACHINE(p, _name, _emState, _pfnOnAct) \
	(memcpy((p)->szStateName, (_name), sizeof((_name))), \
	(p)->emState = (_emState), \
	(p)->pfnAction = (_pfnOnAct), \
	(p)++)

BOOL Ring_Initialize(CAN_RECT_SAMPLER_DATA *pRect_Rough)
{
	s_pRect_Rough = pRect_Rough;

	g_pRing_A = &s_RingData_A;
	g_pRing_A->bIsGunA = TRUE;

	g_pRing_B = &s_RingData_B;
	g_pRing_B->bIsGunA = FALSE;

	g_pRing_A->pNodeMe = NULL;
	g_pRing_B->pNodeMe = NULL;

	GetRingCfg();

	

	s_RingData_A.hqCANMsgFromRect = Queue_Create(MAX_CANMSG_FROM_RECT,sizeof(A_CAN_RECT_FRAME), 0);//0 means can not be auto increased
	s_RingData_B.hqCANMsgFromRect = Queue_Create(MAX_CANMSG_FROM_RECT,sizeof(A_CAN_RECT_FRAME), 0);//0 means can not be auto increased

	if((s_RingData_A.hqCANMsgFromRect == NULL) || (s_RingData_B.hqCANMsgFromRect == NULL))
	{
		return FALSE;
	}

	//Init the state machine
	//不要改变顺序，因为以顺序来索引/**/
	int n;
	for(n = 0; n < 2; n++)
	{
		RING_STM_ITEM *p = (n == 1) ? (&s_RingData_A.stateMachineLoop[0]) : (&s_RingData_B.stateMachineLoop[0]);
		INIT_RING_STATE_MACHINE(p,"IDLE",				RING_STATE_IDLE,						OnIdle				);
		//Master
		INIT_RING_STATE_MACHINE(p,"PRE_BEFORE_CHG",			RING_STATE_PREPARE_BEFRE_CHARGE,		OnPrepare_BeforeCharge	);
		INIT_RING_STATE_MACHINE(p,"IN_CHARGING",				RING_STATE_CHARGING,						OnCharging						);
		INIT_RING_STATE_MACHINE(p,"WAIT_MERGING",			RING_STATE_WAITING_MERGING,			OnWaitMerging					);		
		INIT_RING_STATE_MACHINE(p,"WAIT_CANCEL_MERGE",	RING_STATE_WAITING_CANCELING,			OnWaitCanceling				);
		INIT_RING_STATE_MACHINE(p,"STOPPING",					RING_STATE_STOPING,							OnStop							);
		//Slave
		INIT_RING_STATE_MACHINE(p,"PRE_BEFORE_MERGE",		RING_STATE_PREPARE_BEFOR_MERGE,		OnPrepare_BeforeMerge		);
		INIT_RING_STATE_MACHINE(p,"MERGING",						RING_STATE_MERGING,							OnMerging						);
		INIT_RING_STATE_MACHINE(p,"WAIT_CMD3",				RING_STATE_WAITING_CMD3,					OnWaitCmd3					);
		INIT_RING_STATE_MACHINE(p,"MERGE_CANCELING",		RING_STATE_MERGE_CANCELING,			OnMergeCanceling			);
	}

	time_t tmNow = time(NULL);
	s_RingData_A.pCurrState = &s_RingData_A.stateMachineLoop[RING_STATE_IDLE];
	s_RingData_A.pCurrState->tm_Enter = tmNow;
	s_RingData_B.pCurrState = &s_RingData_B.stateMachineLoop[RING_STATE_IDLE];
	s_RingData_B.pCurrState->tm_Enter = tmNow;
	
	CreateRingNodes();

	for(n = 0; n < MAX_RING_SLAVE_NUM; n++)
	{
		s_RingData_A.master.slaves[n] = NULL;
		s_RingData_B.master.slaves[n] = NULL;
	}
	s_RingData_A.master.nSlaveTotal = 0;
	s_RingData_B.master.nSlaveTotal = 0;
	
	s_RingData_A.hMutex = Mutex_Create(TRUE);
	s_RingData_B.hMutex = Mutex_Create(TRUE);
	if((s_RingData_A.hMutex == NULL) || (s_RingData_B.hMutex == NULL))
	{
		return FALSE;
	}

	Restore_Default(g_pRing_A);
	Restore_Default(g_pRing_B);

	s_RingData_A.hThread = RunThread_Create("RING_A",
		(RUN_THREAD_START_PROC)__ThreadEntry_OfRing,
		(void *)(&s_RingData_A),
		NULL,
		0);//no msg queue

	s_RingData_B.hThread = RunThread_Create("RING_B",
		(RUN_THREAD_START_PROC)__ThreadEntry_OfRing,
		(void *)(&s_RingData_B),
		NULL,
		0);//no msg queue

	if((s_RingData_A.hThread == NULL) || (s_RingData_B.hThread == NULL))
	{
		return FALSE;
	}

	TRACE("---------Now intialize Ring OKKKKK!------\n");
	return TRUE;
}

void Ctrl_RighDO(CAN_RING_DATA* pRing_Rough, BOOL bOn)
{
	if(pRing_Rough->bIsGunA)
	{
		CtrlDO(MERGER_CTRL_DO, (bOn ? 0: 1));//并机接触器是反逻辑/**/
	}
	else
	{
		SIG_POINTER			sigP;	
		VAR_VALUE			varVal;
		//用A枪IMIU的DO/**/
		MAKE_SIGP(sigP, 301, SIG_TYPE_CTRL, 2);
		varVal.enumValue = bOn;
		DXI_Internal_SentCtrl(sigP, varVal, FALSE);
	}
}

int Ring_Ctrl(SIG_POINTER *pSigPointer, VAR_VALUE *pVarVal)
{
	if(s_pRect_Rough == NULL) //发错命令了，环网未初始化/**/
	{
		return ERR_INVALID_ARGS;
	}
	CAN_RING_DATA* pRing_Rough = (pSigPointer->iDevUnitID == 3) ? g_pRing_A : g_pRing_B;
	SIG_POINTER			sigP;
	VAR_VALUE			varVal;
	if (Mutex_Lock(pRing_Rough->hMutex, RING_MUTEX_WAIT) != ERR_MUTEX_OK)
	{
		return ERR_INVALID_ARGS;
	}
	TRACE("---------RinCtrl, Received Cmd[%d,%d]!!-------\n", pSigPointer->iDevUnitID, pSigPointer->iSigID);
	if(pSigPointer->iSigType == SIG_TYPE_CTRL)
	{
		if(pSigPointer->iSigID == 1) //Lock gun as master
		{
			if(pRing_Rough->pCurrState->emState == RING_STATE_IDLE)
			{
				pRing_Rough->master.bIsReceivedLock = TRUE;
			}
			else if((g_RingAllInfo.bIsGunInsertMode) && (pRing_Rough->pCurrState->emState == RING_STATE_MERGING)) //插枪模式下可以发送给从机/**/
			{
				time_t tmNow = time(NULL);
				struct tm gmTime;
				gmtime_r(&tmNow, &gmTime);
				gmTime.tm_min = 0;
				gmTime.tm_hour = 0;
				gmTime.tm_sec = 0; 
				time_t tmZero = mktime(&gmTime); //以今天0点作为基准/**/
				time_t tmStart = tmZero + g_RingAllInfo.nGunInsertMode_Start * 3600;
				time_t tmStop;
				if(g_RingAllInfo.nGunInsertMode_Start >= g_RingAllInfo.nGunInsertMode_Stop)
				{
					tmStop = tmZero + g_RingAllInfo.nGunInsertMode_Stop * 3600 + SEC_PER_DAY;
				}
				else
				{
					tmStop = tmZero + g_RingAllInfo.nGunInsertMode_Stop * 3600;
				}
				TRACE("-----------Now-->%d, tmStart-->%d, tmStop-->%d---------\n", tmNow, tmStart, tmStop);
				if((tmNow > tmStart) && (tmNow < tmStop))
				{
					pRing_Rough->master.bIsReceivedLock = TRUE;
				}
			}
		}
		else if(pSigPointer->iSigID == 10)
		{
			if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER) &&
				(!pRing_Rough->master.bIsRecievedStop))
			{
				Stop_AllSlaveRects(pRing_Rough);
				pRing_Rough->master.bIsRecievedStop = TRUE;
			}
		}
		else if(pSigPointer->iSigID == 11)
		{
			if(pRing_Rough->pNodeMe->emLockState == _LOCK_AS_MASTER)
			{
				TRACE("-----Received BMS stoppped OK cmd-------------\n");
				pRing_Rough->master.bIsReceivedStoppOK = TRUE;
			}
		}
		else if(pSigPointer->iSigID == 24)
		{
			MAKE_SIGP(sigP, ((pRing_Rough->bIsGunA) ? 3 : 4), SIG_TYPE_CTRL, 23);
			DXI_External_GetSigVal_BySigPointer(&sigP, &varVal);
			pRing_Rough->master.fTotalReqVolt = varVal.fValue;
			pRing_Rough->master.fTotalReqCurr = MIN(pVarVal->fValue, s_pRect_Rough->fMaxCurrLimit);
			//pRing_Rough->master.fTotalReqCurr = MIN(pRing_Rough->master.fTotalReqCurr , g_RingAllInfo.fRingMaxAllowedCurr);//BMS那边已经比较过/**/
			TRACE("----------BMS Received fReqVolt = %.1f, fReqCurr = %.1f, fMaxCurLmt = %.1f-----------\n", varVal.fValue, pVarVal->fValue, s_pRect_Rough->fMaxCurrLimit);
			pRing_Rough->master.bIsReceivedAdj = TRUE;
		}
		else if(pSigPointer->iSigID == 2)
		{
			pRing_Rough->slave.bReceivedStop = TRUE;
		}
	}
	Mutex_Unlock(pRing_Rough->hMutex);
	return ERR_OK;
}

static BOOL JudgeNode_NotMergerble(CAN_RING_DATA *pRing_Rough)
{
	SIG_POINTER			sigP;
	VAR_VALUE			varVal;
	//int nSigID = (pRing_Rough->bIsGunA) ? 61 : 12;
	//MAKE_SIGP(sigP, 1, SIG_TYPE_SAMP, nSigID);
	//DXI_External_GetSigVal_BySigPointer(&sigP, &varVal);
	//
	//return (varVal.enumValue == 8);
	int nSigID = (pRing_Rough->bIsGunA) ? 405 : 406;
	MAKE_SIGP(sigP, 1, SIG_TYPE_SAMP, nSigID);
	DXI_External_GetSigVal_BySigPointer(&sigP, &varVal);
	
	return (varVal.enumValue == 0);
}

RING_STATE_MACHINE_STATE OnIdle(CAN_RING_DATA *pRing_Rough)
{
	TRACE("------------Ring[%d],  On Idle-------------\n", pRing_Rough->pNodeMe->byNodeAddress);
	if(JudgeNode_NotMergerble(pRing_Rough))
	{
		if(pRing_Rough->pNodeMe->emLockState != _LOCK_NOT_AVAILABLE)
		{
			TRACE("------Node[%d] Not accessable, OldLock = %d!----------\n", pRing_Rough->pNodeMe->byNodeAddress,
				pRing_Rough->pNodeMe->emLockState);

			Restore_Default(pRing_Rough);
			pRing_Rough->pNodeMe->emLockState = _LOCK_NOT_AVAILABLE;
			return RING_STATE_IDLE;
		}
	}
	else if((pRing_Rough->pNodeMe->emLockState != _LOCK_NO_LOCK) ||
		(pRing_Rough->pNodeMe->emMergeStatus != STAT_NO_MERGE))
	{
		TRACE("------Node[%d] not idle, OldLock = %d!!----------\n", pRing_Rough->pNodeMe->byNodeAddress,
			pRing_Rough->pNodeMe->emLockState);
		if((pRing_Rough->pNodeMe->emLockState == _LOCK_AS_SLAVE) && 
			(time(NULL) - pRing_Rough->slave.tmLastCancel < 10))
		{
			//当从机取消时，要等10s再释放资源，不然如果立即开充，会报108错误/**/
			return RING_STATE_IDLE;
		}
		Restore_Default(pRing_Rough);
		return RING_STATE_IDLE;
	}

	//优先判断是否能锁定主机/**/
	if(pRing_Rough->master.bIsReceivedLock)
	{
		TRACE("-----------Now goto MASTER MODE-----------\n");
		return RING_STATE_PREPARE_BEFRE_CHARGE;
	}
	else if(pRing_Rough->slave.bIsCmd2_AA_Ready)
	{
		if(pRing_Rough->pNodeMe->emLockState != _LOCK_NOT_AVAILABLE)
		{
			TRACE("-----------Now goto SLAVE MODE-----------\n");
			return RING_STATE_PREPARE_BEFOR_MERGE;
		}
		else
		{
			pRing_Rough->slave.bIsCmd2_AA_Ready = FALSE;
		}
	}
	return RING_STATE_IDLE;
}

