#include "bsp_can.h"
#include "main.h"
CAN_HandleTypeDef    Can_1_Handle;
CAN_HandleTypeDef    Can_2_Handle;
static int CAN_Config(int iCANNo,int iBaudRate,int iFrameType);
int HAL_CAN_Transmit_Flg(CAN_HandleTypeDef* hcan);
canmsgTXY2	gCan2RxMsgBuf[CAN_A_B_BUF_MAX_NUM];
canmsgTXY2	gCan1RxMsgBuf[CAN_A_B_BUF_MAX_NUM];
uint32_t pRecvCAN2Idx = 0;
uint32_t pRecvCAN1Idx = 0;
uint32_t pProcCAN2Buf = 0;
uint32_t pProcCAN1Buf = 0;

/****************************************************************************************************************************************************************
HAL_CAN_RxCpltCallback
dv_CAN1.fCAN1TxData(&TxCAN_3Buffer);		//CANTxData -> BSP_CanTx(canmsg* pCanmsg) --> HAL_CAN_Transmit
dv_CAN1.fCAN1BusInit();						//CANBusInit-->BSP_Can_1_Init();  --> CAN_Config(0)
dv_CAN1.fCAN1RxInit();						//发之前要准备收	CAN1RxInit --> BSP_Can1_RxEnable 
												-->HAL_CAN_Receive_IT(&Can_1_Handle, CAN_FIFO0)
													--->void HAL_CAN_IRQHandler(CAN_HandleTypeDef* hcan)
														---->HAL_CAN_RxCpltCallback
															--->BSP_Can1_RxEnable();
dv_CAN1.fCAN1ReadFrame				//CANReceiveMessage
__CAN2_CLK_ENABLE();
BSP_Can_2_Init();
********************************************************************************************************************************************************************/

int BSP_Can_2_Init(int iBaudRate,int iFrameType)
{
	__CAN2_CLK_ENABLE();		//(RCC->APB1ENR |= (RCC_APB1ENR_CAN2EN))
	CAN_Config(1,iBaudRate,iFrameType);
	BSP_Can2_RxEnable();
	return 1;
}


int BSP_Can_1_Init(int iBaudRate,int iFrameType)
{
	__CAN1_CLK_ENABLE();
	CAN_Config(0,iBaudRate,iFrameType);
	return BSP_Can1_RxEnable();
}

int BSP_Can2_RxEnable(void)
{
	if(HAL_CAN_Receive_IT(&Can_2_Handle, CAN_FIFO0) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

int BSP_Can1_RxEnable(void)
{
	if(HAL_CAN_Receive_IT(&Can_1_Handle, CAN_FIFO0) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

int BSP_Can_2_Tx(canmsg* pCanmsg,int uDLC,uint32_t IDE )
{
	//static uint8_t u8Tmp=0;
	//++u8Tmp;
	Can_2_Handle.pTxMsg->IDE = IDE;//CAN_ID_STD
	if(IDE ==CAN_ID_EXT)
		Can_2_Handle.pTxMsg->ExtId = pCanmsg->Id;
	else
		Can_2_Handle.pTxMsg->StdId = pCanmsg->Id;
	
	

	Can_2_Handle.pTxMsg->Data[0] = 0xff&(pCanmsg->Data[0]>>24);
	Can_2_Handle.pTxMsg->Data[1] = 0xff&(pCanmsg->Data[0]>>16);
	Can_2_Handle.pTxMsg->Data[2] = 0xff&(pCanmsg->Data[0]>>8);
	Can_2_Handle.pTxMsg->Data[3] = 0xff&(pCanmsg->Data[0]);
	Can_2_Handle.pTxMsg->Data[4] = 0xff&(pCanmsg->Data[1]>>24);
	Can_2_Handle.pTxMsg->Data[5] = 0xff&(pCanmsg->Data[1]>>16);
	Can_2_Handle.pTxMsg->Data[6] = 0xff&(pCanmsg->Data[1]>>8);
	Can_2_Handle.pTxMsg->Data[7] = 0xff&(pCanmsg->Data[1]);
	Can_2_Handle.pTxMsg->DLC = uDLC;
	UINT16 u16Idx = 0;
	for (u16Idx = 0; u16Idx < 100; u16Idx++)
	{
		if(HAL_CAN_Transmit(&Can_2_Handle, 10) == HAL_OK)
		{
			return HAL_OK;
		}
	}
        if(IDE ==CAN_ID_EXT)
            BSP_Can_2_Init(250,CAN_EXTEND);	
        else
            BSP_Can_2_Init(500,CAN_STAND);
	return HAL_ERROR;
}

int BSP_Can_1_Tx(canmsg* pCanmsg,int uDLC)
{
	Can_1_Handle.pTxMsg->ExtId = pCanmsg->Id;
	Can_1_Handle.pTxMsg->Data[0] = 0xff&(pCanmsg->Data[0]>>24);
	Can_1_Handle.pTxMsg->Data[1] = 0xff&(pCanmsg->Data[0]>>16);
	Can_1_Handle.pTxMsg->Data[2] = 0xff&(pCanmsg->Data[0]>>8);
	Can_1_Handle.pTxMsg->Data[3] = 0xff&(pCanmsg->Data[0]);
	Can_1_Handle.pTxMsg->Data[4] = 0xff&(pCanmsg->Data[1]>>24);
	Can_1_Handle.pTxMsg->Data[5] = 0xff&(pCanmsg->Data[1]>>16);
	Can_1_Handle.pTxMsg->Data[6] = 0xff&(pCanmsg->Data[1]>>8);
	Can_1_Handle.pTxMsg->Data[7] = 0xff&(pCanmsg->Data[1]);
	Can_1_Handle.pTxMsg->DLC = uDLC;
	UINT16 u16Idx = 0;
	for (u16Idx = 0; u16Idx < 100; u16Idx++)
	{
		if(HAL_CAN_Transmit(&Can_1_Handle, 10) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	BSP_Can_1_Init(250,CAN_EXTEND);	
	return HAL_ERROR;
}

void BSP_CanCheck(int iNo)//0 CAN1  1CAN2
{
#define HAL_CAN_STATE_ERROR_1	0x04
#define HAL_CAN_STATE_RESET_1	0x00

	if (0 != iNo)
	{
		if (HAL_CAN_STATE_ERROR_1 == HAL_CAN_GetState(&Can_2_Handle))
		{
			BSP_Can_2_Init(250,CAN_EXTEND);
		}
	}
	else
	{
		if (HAL_CAN_STATE_ERROR_1 == HAL_CAN_GetState(&Can_1_Handle))
		{
			BSP_Can_1_Init(250,CAN_EXTEND);
		}
	}
	
}

int BSP_CanTx(canmsg* pCanmsg, int uDLC)
{
	static uint8_t u8Tmp=0;
	++u8Tmp;
	Can_1_Handle.pTxMsg->ExtId = pCanmsg->Id;
	Can_1_Handle.pTxMsg->Data[0] = (pCanmsg->Data[0]>>24)&0x000000FF;
	Can_1_Handle.pTxMsg->Data[1] = (pCanmsg->Data[0]>>16)&0x000000FF;
	Can_1_Handle.pTxMsg->Data[2] = (pCanmsg->Data[0]>>8)&0x000000FF;
	Can_1_Handle.pTxMsg->Data[3] =  pCanmsg->Data[0]&0x000000FF;
	Can_1_Handle.pTxMsg->Data[4] = (pCanmsg->Data[1]>>24)&0x000000FF;
	Can_1_Handle.pTxMsg->Data[5] = (pCanmsg->Data[1]>>16)&0x000000FF;
	Can_1_Handle.pTxMsg->Data[6] = (pCanmsg->Data[1]>>8)&0x000000FF;
	Can_1_Handle.pTxMsg->Data[7] = pCanmsg->Data[1]&0x000000FF;  
	Can_1_Handle.pTxMsg->DLC = uDLC;
	UINT8 u8TempIdx = 0;
	for (u8TempIdx = 0; u8TempIdx < 10; u8TempIdx++)
	{
		if(HAL_CAN_Transmit(&Can_1_Handle, 10) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}
int BSP_GetCanTxFlg(void)
{
    return HAL_CAN_Transmit_Flg(&Can_1_Handle);
}

int HAL_CAN_Transmit_Flg(CAN_HandleTypeDef* hcan)
{
    __HAL_LOCK(hcan);

    if(__HAL_CAN_TRANSMIT_STATUS(hcan, 0) !=0)
    {
      if(hcan->State == HAL_CAN_STATE_BUSY_TX_RX) 
      {
        hcan->State = HAL_CAN_STATE_BUSY_RX;
      }
      else
      {
        hcan->State = HAL_CAN_STATE_READY;
      }
      __HAL_UNLOCK(hcan);
      return HAL_OK;
    }
    else
    {
       hcan->State = HAL_CAN_STATE_TIMEOUT;
       __HAL_UNLOCK(hcan);
       return HAL_TIMEOUT;
    }
}
#define IDE_VAL		1	//扩展帧
#define RTR_VAL		0	//数据帧
static int CAN_Config(int iCANNo,int iBaudRate,int iFrameType)
{
			UINT32 u32Value;// = (0x44 << 11) | (0xF4 << 3) | (IDE_VAL << 2) | (RTR_VAL << 1);
	CAN_FilterConfTypeDef		sFilterConfig;
	static CanTxMsgTypeDef		TxMessage;
	static CanRxMsgTypeDef		RxMessage;
	static CanTxMsgTypeDef		TxMessage2;
	static CanRxMsgTypeDef		RxMessage2;
	if (0 == iCANNo)
	{
		Can_1_Handle.State = HAL_CAN_STATE_RESET;
		Can_1_Handle.Instance = CAN1;
		Can_1_Handle.pTxMsg = &TxMessage;
		Can_1_Handle.pRxMsg = &RxMessage;

		Can_1_Handle.Init.TTCM = 0;
		Can_1_Handle.Init.ABOM = 0;
		Can_1_Handle.Init.AWUM = 0;
		Can_1_Handle.Init.NART = 0;
		Can_1_Handle.Init.RFLM = 0;
		Can_1_Handle.Init.TXFP = 0;
		Can_1_Handle.Init.Mode = 0;
		Can_1_Handle.Init.SJW = 0;
		Can_1_Handle.Init.BS1 = 0x030000;
		Can_1_Handle.Init.BS2 = 0x200000;

		if(HAL_CAN_Init(&Can_1_Handle,iFrameType) != HAL_OK)
		{
			return HAL_ERROR;
		}

		sFilterConfig.FilterNumber = 0;
		sFilterConfig.FilterMode =0;
		sFilterConfig.FilterScale = 1;

		//u32Value = (0xF4<< 11) | (0x56 << 3) | (IDE_VAL << 2) | (RTR_VAL << 1);
		//sFilterConfig.FilterIdHigh = 0xffff & (u32Value>>16);				//0 显性电平
		//sFilterConfig.FilterIdLow = u32Value & 0xffff;						//1 隐形电平
		//sFilterConfig.FilterMaskIdHigh = 0xffff & (u32Value>>16);			//0 无关	不使用此位进行比较
		//sFilterConfig.FilterMaskIdLow = u32Value & 0xffff;;					//1 必须匹配
		sFilterConfig.FilterIdHigh = 0;				//0 显性电平
		sFilterConfig.FilterIdLow = 0;						//1 隐形电平
		sFilterConfig.FilterMaskIdHigh = 0x0000; //0 无关	不使用此位进行比较
		sFilterConfig.FilterMaskIdLow  = 0x0000; //1 必须匹配

		sFilterConfig.FilterFIFOAssignment = 0;
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.BankNumber = 14;

		if(HAL_CAN_ConfigFilter(&Can_1_Handle, &sFilterConfig) != HAL_OK)
		{
			return HAL_ERROR;
		}	
		Can_1_Handle.pTxMsg->RTR = 0;
		Can_1_Handle.pTxMsg->IDE = 4;
		Can_1_Handle.pTxMsg->DLC = 8;
		return HAL_OK;
	}
	else
	{
		Can_2_Handle.State = HAL_CAN_STATE_RESET;
		Can_2_Handle.Instance = CAN2;
		Can_2_Handle.pTxMsg = &TxMessage2;
		Can_2_Handle.pRxMsg = &RxMessage2;

		Can_2_Handle.Init.TTCM = 0;
		Can_2_Handle.Init.ABOM = 0;
		Can_2_Handle.Init.AWUM = 0;
		Can_2_Handle.Init.NART = 0;
		Can_2_Handle.Init.RFLM = 1;
		Can_2_Handle.Init.TXFP = 0;
		Can_2_Handle.Init.Mode = 0;
		Can_2_Handle.Init.SJW = 0;
		if(iBaudRate == 250)
		{
			Can_2_Handle.Init.BS1 = 0x030000;
			Can_2_Handle.Init.BS2 = 0x200000;
		}
		else if(iBaudRate == 500)
		{
			Can_2_Handle.Init.BS1 = 0x0F0000;//0x030000;
			Can_2_Handle.Init.BS2 = 0x300000;//0x200000;
		}

		if(HAL_CAN_Init(&Can_2_Handle,iFrameType) != HAL_OK)
		{
			return HAL_ERROR;
		}

		sFilterConfig.FilterNumber = 14;
		sFilterConfig.FilterMode =0;
		sFilterConfig.FilterScale = 1;

		//56F4 变为 E5F4 所以只能找他们相同1的地方
		//u32Value = (0xF4 << 11) | (0x56 << 3) | (IDE_VAL << 2) | (RTR_VAL << 1);
		//sFilterConfig.FilterIdHigh = 0xffff & (u32Value>>16);				//0 显性电平
		//sFilterConfig.FilterIdLow = u32Value & 0xffff;						//1 隐形电平
		//sFilterConfig.FilterMaskIdHigh = 0xffff & (u32Value>>16);			//0 无关	不使用此位进行比较
		//sFilterConfig.FilterMaskIdLow = u32Value & 0xffff;;					//1 必须匹配
		sFilterConfig.FilterIdHigh = 0;				//0 显性电平
		sFilterConfig.FilterIdLow = 0;						//1 隐形电平
		sFilterConfig.FilterMaskIdHigh = 0x0000; //0 无关	不使用此位进行比较
		sFilterConfig.FilterMaskIdLow  = 0x0000; //1 必须匹配


		sFilterConfig.FilterFIFOAssignment = 0;			//FIFO0
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.BankNumber = 14;

		if(HAL_CAN_ConfigFilter(&Can_2_Handle, &sFilterConfig) != HAL_OK)
		{
			return HAL_ERROR;
		}	
		Can_2_Handle.pTxMsg->RTR = 0;
		if(iFrameType == 1)
		{
			Can_2_Handle.pTxMsg->IDE = CAN_ID_EXT ;
		}
		else
		{
			Can_2_Handle.pTxMsg->IDE = CAN_ID_STD ;
		}
		
		Can_2_Handle.pTxMsg->DLC = 8;
		return HAL_OK;
	}
}


void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle)
{
}

void bsp_CANGetData(uint32_t* Data,uint8_t ID)
{
	//uint8_t  i; 
	//if(ID >CAN_MAX_BUFF)
	//	return;
	//Data[0] = CanRxMsgBuff_1[ID].ExtId;
	//Data[1] = CanRxMsgBuff_1[ID].DLC; 
	//for(i=0;i<8;i++)
	//{
	//	Data[i+2] = CanRxMsgBuff_1[ID].Data[i];
	//}
}

