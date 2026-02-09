/*
 * Safety_SRAM.c
 *
 *  Created on: Jun 27, 2019
 *      Author: B33124
 */
#include "McuSelfCheck.h"
#include "Safety_SRAM.h"

uint32_t sramEccCheckFlag = 0;
uint32_t sramEccCheckRslt = 0;

uint8_t SRamSelfTestStatus = 0;/*0:Õý³£   1£ºÒì³£*/

/*****************************************************************************************
 * Initialize ERM CR0 register to enable SRAM ECC error reporting
 ****************************************************************************************/
void SRAM_ERM_Init(void)
{
	ERM->SR0 = ERM_SR0_SBC0_MASK | ERM_SR0_NCE0_MASK | ERM_SR0_SBC1_MASK | ERM_SR0_NCE1_MASK;	//clear error flags during initialization
	ERM->CR0 = ERM_CR0_ENCIE0_MASK | ERM_CR0_ENCIE1_MASK;	//enable SRAM multi-bit ECC error reporting in memory 0 (SRAM_L) and 1 (SRAM_U)
}

uint32_t  SRamtimerCounter1 = 0;
uint32_t  SRamtimerCounter2 = 0;
/***************************************************************
 * Inject SRAM ECC errors by EIM module.
 * when EIM enabled, it cause ECC errors by any read access to any addresses of
 * the SRAM upper region or SRAM lower region.
 * This will usually cause HardFault or lockup reset.
 * So disable Global IRQ before EIM is on.
 * And enable Global IRQ after EIM is off.
 ****************************************************************/
void SRAM_EccEimTest(void)
{
	INT_SYS_EnableIRQ(ERM_double_fault_IRQn);
	sramEccCheckFlag = 0x55AA55AA;		//signal to SRAM ECC ISR that this is an ECC injection test
	sramEccCheckRslt = 0;
	SRAM_EccInject(0);					//inject ECC error to SRAM_L region
	if(sramEccCheckRslt == 0x33CC33CC)
	{
//		selfTestStatus |= SELFTEST_PASS_SRAML_ECC;
		SRamSelfTestStatus = 0;
		SRamtimerCounter1++;
		if(SRamtimerCounter1 > 20)
		{
			SRamtimerCounter1 = 30;
			SRamSelfTestStatus = 0;
		}
	}
	else
	{
//		selfTestStatus &= ~SELFTEST_PASS_SRAML_ECC;
		SRamSelfTestStatus = 1;
	}

	sramEccCheckFlag = 0x55AA55AA;		//signal to SRAM ECC ISR that this is an ECC injection test
	sramEccCheckRslt = 0;
	SRAM_EccInject(1);					//inject ECC error to SRAM_L region
	if(sramEccCheckRslt == 0x33CC33CC)
	{
//		selfTestStatus |= SELFTEST_PASS_SRAMU_ECC;
		SRamSelfTestStatus = 0;
		SRamtimerCounter2++;
		if(SRamtimerCounter2 > 20)
		{
			SRamtimerCounter2 = 30;
			SRamSelfTestStatus = 0;
		}
	}
	else
	{
//		selfTestStatus &= ~SELFTEST_PASS_SRAMU_ECC;
		SRamSelfTestStatus = 1;
	}

	sramEccCheckFlag = 0;				//clear flags so that real SRAM ECC errors can be handled
	sramEccCheckRslt = 0;
}

/*******************************************************************************************
 * Inject ECC error to SRAM_L (channel 0) or SRAM_U (channel 1) with EIM module.
 *******************************************************************************************/
void SRAM_EccInject(uint32_t nChan)
{
	if(nChan == 0)
	{
		EIM->EICHDn[0].WORD0 = 0xC0000000;		//check bits error, only upper 7 bits used;
		EIM->EICHDn[0].WORD1 = 0x00000000;		//data bits error, all 32 bits used;
		INT_SYS_DisableIRQGlobal();
		EIM->EICHEN = EIM_EICHEN_EICH0EN_MASK;	//enable channel 0 error injection (SRAM_L region)
		EIM->EIMCR = EIM_EIMCR_GEIEN_MASK;		//enable error injection
		EIM->EICHDn[0].WORD1 = *(uint32_t *)0x1FFFFF10;	//read SRAM_L region to trigger SRAM ECC error;
		EIM->EIMCR = 0;							//disable error injection
		INT_SYS_EnableIRQGlobal();
	}
	else if(nChan == 1)
	{
		EIM->EICHDn[1].WORD0 = 0x00000000;		//check bits error, only upper 7 bits used;
		EIM->EICHDn[1].WORD1 = 0xC0000000;		//data bits error, all 32 bits used;
		INT_SYS_DisableIRQGlobal();
		EIM->EICHEN = EIM_EICHEN_EICH1EN_MASK;	//enable channel 0 error injection (SRAM_L region)
		EIM->EIMCR = EIM_EIMCR_GEIEN_MASK;		//enable error injection
		EIM->EICHDn[1].WORD1 = *(uint32_t *)0x20000010;	//read SRAM_L region to trigger SRAM ECC error;
		EIM->EIMCR = 0;							//disable error injection
		INT_SYS_EnableIRQGlobal();
	}
}

/******************************************************************
 * When SRAM ECC error detected, this function will be invoked.
 * 1) It get PSP or MSP.
 * 2) And get error PC address in the stack.
 * 3) Increment the error PC address to skip the instruction causing ECC error.
 * 4) Read and save error data address and error PC address.
 * 5) Clear fault flags.
 * Normally SRAM ECC error will trigger this ISR, but will not trigger HardFault_Handler.
 * Unless the SRAM access of fetching vector table or fetching ISR instructions also caused SRAM ECC.
 ******************************************************************/
void ERM_double_fault_IRQHandler(void)
{
	register uint32_t LR_Reg;
	uint32_t SP_Reg = 0;
	uint32_t *LR_Pointer = NULL;
	__ASM volatile ("mov %0, r14\n" : "=r" (LR_Reg) );
	EIM->EIMCR = 0;	//disable SRAM ECC error injection;	must do this at the beginning of the HardFault_Handler, otherwise the data in sram will be wrong

	if( (Re2TM_from_FPS_PSP_to_use_PSP!=LR_Reg) &&  (Re2TM_from_nFPS_PSP_to_use_PSP!=LR_Reg) )
		SP_Reg = __get_MSP();
	else
		SP_Reg = __get_PSP();

	/*increase stacked MSP by 4/2 to next instruction, for the LDR is 4/2 byte instruction*/
	LR_Pointer =(uint32_t *) (SP_Reg+ 0x18);
	*LR_Pointer += LDR_INST_WIDTH;

	if(sramEccCheckFlag == 0x55AA55AA)	//this means the ECC error is injected during MCU init for self-test.
	{
		if( ERM->SR0 & (ERM_SR0_NCE0_MASK | ERM_SR0_SBC0_MASK) )
		{
			//if( (ERM->EARn[0].EAR >= SRAM_L_ECC_ADDR) && (ERM->EARn[0].EAR < (SRAM_L_ECC_ADDR + 0x10)) )
			{
				sramEccCheckRslt = 0x33CC33CC;
			}
		}
		else if(  ERM->SR0 & (ERM_SR0_NCE1_MASK | ERM_SR0_SBC1_MASK) )
		{
			//if( (ERM->EARn[1].EAR >= SRAM_U_ECC_ADDR) && (ERM->EARn[1].EAR < (SRAM_U_ECC_ADDR + 0x10)) )
			{
				sramEccCheckRslt = 0x33CC33CC;
			}
		}
	}
	else
	{
//		faultBuffer.faultType = faultSramEcc;
//		faultBuffer.faultPcAddr = *LR_Pointer - LDR_INST_WIDTH;
//		if(ERM->SR0 & ERM_SR0_NCE0_MASK)
//		{
//			faultBuffer.faultDataAddr = ERM->EARn[0].EAR;
//		}
//		else if(ERM->SR0 & ERM_SR0_NCE1_MASK)
//		{
//			faultBuffer.faultDataAddr = ERM->EARn[1].EAR;
//		}
//		else if(ERM->SR0 & ERM_SR0_SBC0_MASK)
//		{
//			faultBuffer.faultDataAddr = ERM->EARn[0].EAR;
//		}
//		else if(ERM->SR0 & ERM_SR0_SBC1_MASK)
//		{
//			faultBuffer.faultDataAddr = ERM->EARn[1].EAR;
//		}
//		//ERM->SR0 = ERM_SR0_SBC0_MASK | ERM_SR0_SBC1_MASK | ERM_SR0_NCE0_MASK | ERM_SR0_NCE1_MASK;	//clear all error flags
//		Safety_FaultManage_SaveFault(&faultBuffer);
	}
	ERM->SR0 = ERM_SR0_SBC0_MASK | ERM_SR0_SBC1_MASK | ERM_SR0_NCE0_MASK | ERM_SR0_NCE1_MASK;	//clear all error flags
}


/**************************************************************************************
 * It is required to disable Cache during safety initial check.
 ***************************************************************************************/
void LMEM_CacheDisable(void)
{
	LMEM->PCCCR = LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_GO_MASK;
}

/**************************************************************************************
 * Cache can be enabled when all safety-relevant initial check is done.
 ***************************************************************************************/
void LMEM_CacheEnable(void)
{
	LMEM->PCCCR = LMEM_PCCCR_ENCACHE_MASK | LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_GO_MASK;
}

uint8_t GetSRamSelfTestStatus(void)
{
	return(SRamSelfTestStatus);
}
