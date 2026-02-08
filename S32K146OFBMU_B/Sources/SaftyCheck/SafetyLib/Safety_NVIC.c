/*
 * Safety_NVIC.c
 *
 *  Created on: 2021年9月14日
 *      Author: zsy342
 */
#include "BSPTypeDef.h"
#include "S32K146.h"
#include "Safety_NVIC.h"

uint8_t NvicSelfTestStatus = 0;

/*******************************************************************************
 * If an interrupt is not enabled or not configured, but the interrupt occurred.
 * This default ISR will be triggered.
 ******************************************************************************/
void DefaultISR(void)
{
	u16 nIntNum = 0;
	nIntNum = * (u16 *)0x1FFFFFF8;
	nIntNum |= (0x001FF & S32_SCB->ICSR) - 16;
//	faultBuffer.faultType = faultNVIC;
//	faultBuffer.faultPcAddr = nIntNum;
//	faultBuffer.faultDataAddr = nIntNum;
//	Safety_FaultManage_SaveFault(&faultBuffer);		//save fault, PcAddr means the error interrupt number
//	//UART_Print("un-configured interrupt occurred. Reboot......\r\n");
//	SystemSoftwareReset();
	SetNvicSelfTestStatus(1);/*产生意外中断*/
}
uint32_t  NvictimerCounter = 0;
uint8_t GetNvicSelfTestStatus(void)
{
	NvictimerCounter++;
	if(NvictimerCounter > 20)
	{
		NvictimerCounter = 30;
		NvicSelfTestStatus = 0;
	}
	return(NvicSelfTestStatus);
}

void SetNvicSelfTestStatus(uint8_t flag)
{
	if(flag > 1)
	{
		return;
	}

	NvicSelfTestStatus = flag;
}
