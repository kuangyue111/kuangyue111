/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : BSPUserHook.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : BSP用户服务函数模块
** @Instructions : 用户完善
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "BSPConfig.h"
#include "DEVGPIO.h"
#include "DEVADC.h"
#include "SocInterface.h"
#include "CHGInterface.h"
#include "CANInterface.h"
#include "MbInterface.h"
#include "CurrSample.h"
#include "ModbusUser.h"
#include "FrockMsg.h"
#include "CatlMsg.h"
#include "LoadApp.h"
#include "BSPUserHook.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : void CANSendData_Hook(e_CANNUM CANNum, t_CANMSG msg)
** @Input     : CANNum:CAN通道 msg:报文信息
** @Output    : void
** @Function  : CAN发送任务嵌入函数
** @The notes :
**===========================================================================================*/
void CANSendData_Hook(e_CANNUM CANNum, t_CANMSG msg)
{
    (void)CANNum;
    (void)msg;
}

/*=============================================================================================
** @Name      : void CANRcvMsgHandle_Hook(e_CANNUM CANNum, t_CANMSG *pWriteMsg)
** @Input     : CANNum:CAN通道 pWriteMsg:CAN报文信息结构指针
** @Output    : void
** @Function  : CAN接收信息处理接口
** @The notes : 该函数已经在读CAN接收缓冲区调用
**===========================================================================================*/
void CANRcvMsgHandle_Hook(e_CANNUM CANNum, t_CANMSG *pWriteMsg)
{
    if(CANNum == eCAN0)
    {
        //CAN通讯包接收处理
        (void)CANLibRcvMsgHandle_API(pWriteMsg->ID, pWriteMsg->Data, pWriteMsg->Len, (u8)CANNum);

        #if(1 == PILE_MANAG_EN)
        //电池堆接收上级上PCS信息
        PileRcvUpMsgHandle_API(pWriteMsg->ID, pWriteMsg->Data, pWriteMsg->Len, (u8)CANNum);
        
        //电池堆接收下级主控信息
        PileRcvDnMsgHandle_API(pWriteMsg->ID, pWriteMsg->Data, pWriteMsg->Len, (u8)CANNum);
        #endif
    }
    else if(CANNum == eCAN1)
    {
        //BMU接收BSU信息处理
        //CanRcvBsuMsgHandle_API(pWriteMsg->ID, pWriteMsg->Data, pWriteMsg->Len, (u8)CANNum);
    }
    else
    {
    	;
    }
    
    /*多组并机SOCSOE同步CAN接收处理*/
    SocSoeSyncRcvGMsgHandle_API(pWriteMsg->ID, pWriteMsg->Data, pWriteMsg->Len, (u8)CANNum);

    /*快充通信包CAN接收处理*/
    CHGLibRcvMsgHandleAPI(pWriteMsg->ID, pWriteMsg->Data, pWriteMsg->Len, (u8)CANNum);

    /*兼容CATL电池协议CAN接收处理*/
    #if(1 == BMU_CATL_CAN_EN)
    CanRcvCatlMsgHandleAPI(pWriteMsg, CANNum);
	#endif

    /*在线升级CAN接收处理*/
    CanRcvLoadMsgHandleAPI(pWriteMsg, CANNum);

    /*测试工装CAN接收处理*/
	#if(1 == BMU_FROCK_CAN_EN)
    CanRcvFrockMsgHandleAPI(pWriteMsg, CANNum);
	#endif
}

/*=============================================================================================
** @Name      : void SCIRcvMsgHandle_Hook(e_SCINUM SCINum, u8 rcvData)
** @Input     : SCINum:SCI通道 rcvMsg:接收到的数据
** @Output    : void
** @Function  : SCI接收信息处理接口
** @The notes :
**===========================================================================================*/
void SCIRcvMsgHandle_Hook(e_SCINUM SCINum, u8 rcvData)
{
	MbLibRcvDataHandleAPI((u8)SCINum, rcvData);
}

