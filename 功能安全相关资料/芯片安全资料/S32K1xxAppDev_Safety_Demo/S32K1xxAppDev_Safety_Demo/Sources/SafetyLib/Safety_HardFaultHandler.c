/*
 * Safety_HardFaultHandler.c
 *
 *  Created on: May 7, 2019
 *      Author: B33124
 */

#include "SafetyLib.h"

uint32_t HardFault_Source_Addr;
void HardFault_Handler(void)
{
    register uint32_t LR_Reg;
    uint32_t SP_Reg = 0;
    uint32_t *LR_Pointer = NULL;
    uint32_t i, j;
    __ASM volatile ("mov %0, r14\n" : "=r" (LR_Reg) );
    EIM->EIMCR = 0;	//disable SRAM ECC error injection;	must do this at the beginning of the HardFault_Handler, otherwise the data in sram will be wrong

    if( (Re2TM_from_FPS_PSP_to_use_PSP!=LR_Reg) &&  (Re2TM_from_nFPS_PSP_to_use_PSP!=LR_Reg) )
        SP_Reg = __get_MSP();
    else
        SP_Reg = __get_PSP();
    LR_Pointer =(uint32_t *) (SP_Reg+ 0x18);
    *LR_Pointer += LDR_INST_WIDTH;
    faultBuffer.faultPcAddr = *LR_Pointer - LDR_INST_WIDTH;

    //there is no S32_SCB->HFSR and S32_SCB->BFAR register on CM0+ core, cannot get the exact HardFault ECC address
    #if defined(MCU_S32K116)||defined(MCU_S32K118)
    HardFault_Source_Addr = 0xDEADBEEF;/*used "dead beef" instead*/
    #else
    if(S32_SCB->HFSR)
    {
        S32_SCB->HFSR = S32_SCB->HFSR;/*clean the HardFault flags*/
    }
    HardFault_Source_Addr = S32_SCB->BFAR;
	#endif

    if(MPU->CESR & 0xF8000000)	//if there are MPU errors
	{
    	for(i=0, j=0x80000000; i<5; i++)	//find out the slave port that caused MPU error, only slave port 0~3 for S32K144.
		{
			if(MPU->CESR & j)
				break;
			j = j>>1;
		}
    	MPU_DRV_GetDetailErrorAccessInfo(MEMPROTECT1, i, &mpuErrorInfo);	//read the error information of the specified slave port
    	if(mpuErrorCheckFlag == 0x55AA55AA)		//this flag indicate whether it's an MPU error injection test or it's a real MPU error
    	{
    		mpuErrorCheckFlag = 0x33CC33CC;
    	}
    	else	//save fault information, and call MPU Fault Callback function
    	{
    		faultBuffer.faultType = faultSmpu;
			faultBuffer.faultDataAddr = mpuErrorInfo.addr;
			Safety_FaultManage_SaveFault(&faultBuffer);
			Safety_MPUFault_Callback();
    	}
    	if(mpuErrorInfo.accessType == MPU_ERR_TYPE_WRITE)
    		*LR_Pointer -= LDR_INST_WIDTH;
	}
    else if( ERM->SR0 & (ERM_SR0_NCE0_MASK | ERM_SR0_NCE1_MASK) )	//if single bit error or multi bit error occurred in SRAM_L or SRAM_U
	{
		EIM->EIMCR = 0;	//disable error injection;
		faultBuffer.faultType = faultSramEcc;
		if(ERM->SR0 & ERM_SR0_NCE0_MASK)
			faultBuffer.faultDataAddr = ERM->EARn[0].EAR;
		else if(ERM->SR0 & ERM_SR0_NCE1_MASK)
			faultBuffer.faultDataAddr = ERM->EARn[1].EAR;
		ERM->SR0 = ERM_SR0_NCE0_MASK | ERM_SR0_NCE1_MASK;	//clear multi-bit ECC error flag
		Safety_FaultManage_SaveFault(&faultBuffer);
	}
    else if( FLASH_DRV_GetDoubleBitFaultFlag() )
	{
		FLASH_DRV_ClearDoubleBitFaultFlag();
		FTFC_SetErrorAddr(HardFault_Source_Addr);
		faultBuffer.faultType = faultFlashEcc;
		faultBuffer.faultDataAddr = HardFault_Source_Addr;
		Safety_FaultManage_SaveFault(&faultBuffer);
	}
    /*else if( S32_SCB->BFSR & 0x06)
    {
    	S32_SCB->BFSR = 0x06;	//clear fault flags
    }*/
    else
    {
    	faultBuffer.faultType = faultHardFault;
    	faultBuffer.faultDataAddr = HardFault_Source_Addr;
    	Safety_FaultManage_SaveFault(&faultBuffer);
    	SystemSoftwareReset();
    }
	return;
}

void BusFault_Handler(void)
{
	faultBuffer.faultDataAddr = S32_SCB->BFAR;
	faultBuffer.faultType = faultUnknown;
	//S32_SCB->BFSR = 0x06;	//clear fault flags
	Safety_FaultManage_SaveFault(&faultBuffer);
}
