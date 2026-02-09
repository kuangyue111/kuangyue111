/*
 * Safety_Flash.c
 *
 *  Created on: Jun 27, 2019
 *      Author: B33124
 */


#include "SafetyLib.h"
uint32_t flsErrorAddr = 0xFFFFFFFF;
uint32_t flsEccCheckFlag = 0;

/************************************************************************
 * Inject Flash ECC errors and verify Flash ECC fault handler can be invoked.
 ***********************************************************************/
void FTFC_EccSelfTest(void)
{
	flsEccCheckFlag = 0x55AA55AA;
	FLASH_DRV_EnableDoubleBitFaultInterupt();
	FLASH_DRV_ForceDoubleBitFaultDetectCmd(true);
	if(flsEccCheckFlag == 0x33CC33CC)
		selfTestStatus |= SELFTEST_PASS_FLASH_ECC;
	else
		selfTestStatus &= ~SELFTEST_PASS_FLASH_ECC;
	flsEccCheckFlag = 0;
}

/*******************************************************************************
 * FTFC_Fault_IRQHandler
 *******************************************************************************/
void FTFC_Fault_IRQHandler(void)
{
	register uint32_t LR_Reg;
	uint32_t SP_Reg = 0;
	uint32_t *LR_Pointer = NULL;
	__ASM volatile ("mov %0, r14\n" : "=r" (LR_Reg) );

	FLASH_DRV_ForceDoubleBitFaultDetectCmd(false);		//immiedately disable flash Double-bit fault injection

	if( (Re2TM_from_FPS_PSP_to_use_PSP!=LR_Reg) &&  (Re2TM_from_nFPS_PSP_to_use_PSP!=LR_Reg) )
	{
		/*increase stacked MSP by 4/2 to next instruction, for the LDR is 4/2 byte instruction*/
		SP_Reg = __get_MSP();
	}
	else
	{
		/*increase stacked PSP by 4/2 to next instruction, for the LDR is 4/2 byte instruction*/
		SP_Reg = __get_PSP();
	}
	LR_Pointer =(uint32_t *) (SP_Reg+ 0x18);	//this is the PC value after IRQ return. (The address after the instruction causing flash fault IRQ)

	if(flsEccCheckFlag == 0x55AA55AA)
	{
		if(FLASH_DRV_GetDoubleBitFaultFlag())
		{
			flsEccCheckFlag = 0x33CC33CC;
			FLASH_DRV_ClearDoubleBitFaultFlag();
		}
	}
	else
	{
		if(FLASH_DRV_GetDoubleBitFaultFlag())
		{
			FLASH_DRV_ClearDoubleBitFaultFlag();
			faultBuffer.faultType = faultFlashEcc;		//fault type is Flash ECC fault
			faultBuffer.faultPcAddr = *LR_Pointer;		//fault pc address is got from stack
			faultBuffer.faultDataAddr = 0xFFFFFFFF;		//fault data address is unknown
			Safety_FaultManage_SaveFault(&faultBuffer);
			Safety_FlashFault_Callback();	//0xFFFFFFFF represent unknown fault address
		}
		if(FLASH_DRV_GetReadColisionFlag())
		{
			FLASH_DRV_ClearReadColisionFlag();
		}
	}
}

void FTFC_SetErrorAddr(uint32_t errAddr)
{
	flsErrorAddr = errAddr;
}

uint32_t FTFC_GetErrorAddr(void)
{
	return flsErrorAddr;
}
