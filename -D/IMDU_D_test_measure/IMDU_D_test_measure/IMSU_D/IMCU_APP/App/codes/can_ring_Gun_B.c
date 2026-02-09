
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