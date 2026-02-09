/*
 * Safety_Callback.c
 *
 *  Created on: Jun 20, 2019
 *      Author: B33124
 */
#include "SafetyLib.h"

/*!
 * @brief PMC faults callback function.
 *
 * This callback function is invoked when any PMC faults detected:
 * 1) LVD: low voltage detected; (VDD < 2.9V, determined by MCU internal hardware)
 * 2) LVW: low voltage warning; (VDD < 4.305V, determined by MCU internal hardware)
 * 3) HVD: high voltage detected; (VDD > 5.5V, determined by software)
 * Customer can add more code to handle the LVD/LVW/HVD event;
 *
 * @param pmcFault - PMC fault type, PMC_LVD, PMC_LVW or PMC_HVD.
 * @return none.
 */
void Safety_PmcFault_Callback(PMC_Fault_Type pmcFault)
{

}


/*******************************************************************************
 * If an interrupt is not enabled or not configured, but the interrupt occurred.
 * This default ISR will be triggered.
 ******************************************************************************/
void DefaultISR(void)
{
	uint16_t nIntNum = 0;
	nIntNum = * (uint16_t *)0x1FFFFFF8;
	nIntNum |= (0x001FF & S32_SCB->ICSR) - 16;
	faultBuffer.faultType = faultNVIC;
	faultBuffer.faultPcAddr = nIntNum;
	faultBuffer.faultDataAddr = nIntNum;
	Safety_FaultManage_SaveFault(&faultBuffer);		//save fault, PcAddr means the error interrupt number
	//UART_Print("un-configured interrupt occurred. Reboot......\r\n");
	SystemSoftwareReset();
}

/*******************************************************************************
 * This function is called if specified flash area CRC doesn't match the pre-saved CRC value
 * This function is called in "Safeboot_Init()"
 *******************************************************************************/
void Safety_Safeboot_Callback(void)
{

}

/*******************************************************************************
 * This function is called if register check fault detected
 * This function is called in "RegCheck_Compare()"
 *******************************************************************************/
void Safety_RegCheck_Callback(void)
{

}

/*********************************************************************************
 * This function is called when MPU fault detected.
 * This function is called in "HardFault_Handler_C".
 * return 0 - read access error
 * return 1 - write access error
 ********************************************************************************/
void Safety_MPUFault_Callback(void)
{

}

/*********************************************************************************
 * This function is called when Flash double bit error detected.
 * This function is called from FTFC_Fault_IRQHandler.
 * ******************************************************************************/
void Safety_FlashFault_Callback(void)
{

}

/***********************************************************************************
 * This function is called if Flash protection violation flag is found to be set.
 ***********************************************************************************/
void Safety_FlashProtection_Callback(void)
{

}

