
#ifndef __BSP_CAN_H
#define __BSP_CAN_H
#include "stm32f4xx_hal.h"
#define  CAN_MAX_BUFF   20

#define CANx_RX_IRQn                   CAN1_RX0_IRQn
#define CANx_RX_IRQHandler             CAN1_RX0_IRQHandler

#define CAN2_RX_IRQn                   CAN2_RX0_IRQn
#define CAN2_RX_IRQHandler             CAN2_RX0_IRQHandler

typedef struct
{
	uint32_t Id;
	uint32_t Data[2];
} canmsg;


int BSP_Can_2_Init(int iBaudRate,int iFrameType);
int BSP_Can_1_Init(int iBaudRate,int iFrameType);
void BSP_CanCheck(int iNo);
int BSP_Can2_RxEnable(void);
int BSP_Can1_RxEnable(void);
int BSP_CanTx(canmsg* pCanmsg, int uDLC);
int BSP_Can_2_Tx(canmsg* pCanmsg,int uDLC,uint32_t IDE);
int BSP_Can_1_Tx(canmsg* pCanmsg,int uDLC);
int BSP_GetCanTxFlg(void);
void bsp_CANGetData(uint32_t* Data,uint8_t ID);
#endif
