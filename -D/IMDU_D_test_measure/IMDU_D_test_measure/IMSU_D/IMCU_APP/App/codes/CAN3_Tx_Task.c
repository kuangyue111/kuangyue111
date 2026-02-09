#include "main.h"
#include "can_ring.h"
#include "Uart6_new.h"

//CANCommData_TypeDef gs_CANComm;
//canmsg TxCAN_3Buffer;



tagCAN3TxDeal_def gs_Can3TxMng;

void WriteTxCAN3Buff(canmsg * pRcvMsg, UINT8 u8Len)
{
	MutexLock(gs_CANComm.hMutexWriteCAN3Port);

	gs_Can3TxMng.TxCanMsgBuf[gs_Can3TxMng.CanTxBuffWrPos].Data[0] = pRcvMsg->Data[0];
	gs_Can3TxMng.TxCanMsgBuf[gs_Can3TxMng.CanTxBuffWrPos].Data[1] = pRcvMsg->Data[1];
	gs_Can3TxMng.TxCanMsgBuf[gs_Can3TxMng.CanTxBuffWrPos].Id = pRcvMsg->Id;

	gs_Can3TxMng.CanTxBuffWrPos++;
	if(gs_Can3TxMng.CanTxBuffWrPos >= CAN3_TX_MSG_BUF_NUM)
	{
		gs_Can3TxMng.CanTxBuffWrPos = 0; 
	}

	MutexUnlock(gs_CANComm.hMutexWriteCAN3Port);
}


void InitTaskOfCan3Tx(void)
{
	gs_Can3TxMng.CanTxBuffRdPos = 0;
	gs_Can3TxMng.CanTxBuffWrPos = 0;
	gs_Can3TxMng.ucClearBufFlag = 0;
}
/************************************************/
//函数名称:										
//函数功能:										
//更改日期:      								
/************************************************/
void TASK_CAN3_Tx(void* pdata)
{
	unsigned char i;
	UNUSED(pdata);
	
	canmsg TxMsg;

	//UINT32 u32Delay = 0;

	InitTaskOfCan3Tx();

	for(;;)
	{
		gSelfTest.fSetTaskID(TASK_ID_CAN3_TX);


		/*if (gs_CANComm.u8IsAllCommFail == TRUE)
		{
			u32Delay++;
			if (u32Delay > 1000)
			{
				u32Delay = 0;
				BSP_InitSPI3();
				OSTimeDly(500);
				InitCan3Port();
				OSTimeDly(500);
				gSelfTest.fSetTaskID(TASK_ID_CAN3_TX);
			}
		}
		else
		{
			u32Delay = 0;
		}*/

		if(gs_Can3TxMng.ucClearBufFlag == 1)
		{
			MutexLock(gs_CANComm.hMutexWriteCAN3Port);
			gs_Can3TxMng.CanTxBuffRdPos = 0;
			gs_Can3TxMng.CanTxBuffWrPos = 0;
			gs_Can3TxMng.ucClearBufFlag = 0;
			MutexUnlock(gs_CANComm.hMutexWriteCAN3Port);
		}


		/* 配置参数 更新 */
		for(i = 0; i < CAN3_TX_MSG_BUF_NUM; i++)
		{                   
			if(gs_Can3TxMng.CanTxBuffWrPos == gs_Can3TxMng.CanTxBuffRdPos)        
			{
				break;
			}
			else
			{           
				TxMsg.Data[0] = gs_Can3TxMng.TxCanMsgBuf[gs_Can3TxMng.CanTxBuffRdPos].Data[0];   
				TxMsg.Data[1] = gs_Can3TxMng.TxCanMsgBuf[gs_Can3TxMng.CanTxBuffRdPos].Data[1];   
				TxMsg.Id = gs_Can3TxMng.TxCanMsgBuf[gs_Can3TxMng.CanTxBuffRdPos].Id;   
				gs_Can3TxMng.CanTxBuffRdPos++;                    
				if(gs_Can3TxMng.CanTxBuffRdPos >= CAN3_TX_MSG_BUF_NUM)
				{
					gs_Can3TxMng.CanTxBuffRdPos = 0;
				}	

				if(gs_Can3TxMng.ucClearBufFlag == 0)
				{
					CAN3_Send(&TxMsg, 8);
				}
                                
                 gSelfTest.fSetTaskID(TASK_ID_CAN3_TX);
				 OSTimeDly(3);
			}
		}

		OSTimeDly(5);	
	}

}	