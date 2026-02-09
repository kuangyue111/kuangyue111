/*
 * Safety_RCM.c
 *
 *  Created on: May 3, 2019
 *      Author: B33124
 */

#include "Safety_RCM.h"

reset_source_t resetSourceVar __attribute__((section (".customerBootData")));
uint32_t resetCountVar __attribute__((section (".customerBootData")));
uint32_t selfTestStatus __attribute__((section (".customerBootData")));

#define RESET_SOURCE_REG	RCM->SSRS
//#define RESET_SOURCE_REG	RCM->SRS

reset_source_t RCM_GetResetSource(void)
{
	reset_source_t tmp;
	if(RESET_SOURCE_REG & RCM_SRS_POR_MASK)
		tmp = ResetSourcePOR;
	else if(RESET_SOURCE_REG & RCM_SRS_LVD_MASK)
		tmp = ResetSourceLVD;
	else if(RESET_SOURCE_REG & RCM_SRS_LOC_MASK)
		tmp = ResetSourceLOC;
	else if(RESET_SOURCE_REG & RCM_SRS_LOL_MASK)
		tmp = ResetSourceLOL;
	else if(RESET_SOURCE_REG & RCM_SRS_CMULOC_MASK)
		tmp = ResetSourceCMULOC;
	else if(RESET_SOURCE_REG & RCM_SRS_MDM_AP_MASK)
		tmp = ResetSourceMDMAP;
	else if(RESET_SOURCE_REG & RCM_SRS_PIN_MASK)
		tmp = ResetSourcePIN;
	else if(RESET_SOURCE_REG & RCM_SRS_JTAG_MASK)
		tmp = ResetSourceJTAG;
	else if(RESET_SOURCE_REG & RCM_SRS_WDOG_MASK)
		tmp = ResetSourceWDOG;
	else if(RESET_SOURCE_REG & RCM_SRS_LOCKUP_MASK)
		tmp = ResetSourceLOCKUP;
	else if(RESET_SOURCE_REG & RCM_SRS_SW_MASK)
		tmp = ResetSourceSoft;
	else if(RESET_SOURCE_REG & RCM_SRS_SACKERR_MASK)
		tmp = ResetSourceSACKERR;
	RESET_SOURCE_REG = RESET_SOURCE_REG;	//clear reset source sticky flag
	return tmp;
}

/**********************************************************************************************
 * Save reset source to SRAM; and increment reset count;
 * This function should be called at the beginning of main() function.
 *********************************************************************************************/
void RCM_SaveResetSource(void)
{
	reset_source_t tmp;
	tmp = RCM_GetResetSource();
	resetSourceVar = tmp;
	//if((ResetSourcePOR == tmp) || (ResetSourceMDMAP == tmp) || (ResetSourcePIN == tmp))	//if it's power on rest or debugger reset, clear the counter
	if( (ResetSourcePOR == tmp) || (ResetSourceMDMAP == tmp) )	//if it's power on rest or debugger reset, clear the counter
	{
		resetCountVar = 0;
		selfTestStatus = 0;
	}
	else
		resetCountVar++;		//if it's not power on reset, increment the counter.
}

reset_source_t RCM_ReadResetSource(void)
{
	if(resetSourceVar > (uint32_t)ResetSourceReserved)
		resetSourceVar = ResetSourceReserved;
	return (reset_source_t)resetSourceVar;
}

uint32_t RCM_GetResetCnt(void)
{
	return resetCountVar;
}

void RCM_EnableInt(void)
{
	RCM->SRIE = RCM_SRIE_SW_MASK | RCM_SRIE_LOCKUP_MASK | RCM_SRIE_GIE_MASK | RCM_SRIE_PIN_MASK | RCM_SRIE_LOL_MASK | RCM_SRIE_LOC_MASK;
	//when functional reset occurred, MCU will first enter the RCM IRQ handler and later reset.
}

void RCM_IRQHandler(void)
{
	SIM->CHIPCTL &= (~(SIM_CHIPCTL_SRAML_RETEN_MASK | SIM_CHIPCTL_SRAMU_RETEN_MASK));	//clear these two bits to forbidden any access to SRAM during reset process
	for(;;);
}
