/*
c * Safety_CMU.c (Clock Monitor Unit)
 *
 *  Created on: 2019Äê5ÔÂ20ÈÕ
 *      Author: nxf47391
 *
 */
#include "Safety_CMU.h"

/********************************************************************
 * System OSC Clock Monitor Enable / Disable
 **********************************************************************/
void CMU_OSC_Config(STD_Type On_Off)
{
	/* System OSC Valid */
	if(1 == ((SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) >> SCG_SOSCCSR_SOSCVLD_SHIFT))
	{
		if(STD_ON == On_Off)
		{
			SCG->SOSCCSR |= SCG_SOSCCSR_SOSCCM_MASK;
		}
		else
		{
			SCG->SOSCCSR &= (~SCG_SOSCCSR_SOSCCM_MASK);
		}
	}
	else
	{
		SCG->SOSCCSR &= (~SCG_SOSCCSR_SOSCCM_MASK);
	}
}

/*
  * Clock Monitor generates Interrupt or Reset when error detected
  *    *
  *    */
void CMU_OSC_IERE_Config(CMU_INT_RST_Type IE_RE)
{
	if(CMU_Interrupt_On_Error == IE_RE)
	{
		SCG->SOSCCSR &= (~SCG_SOSCCSR_SOSCCMRE_MASK);
	}
	else
	{
		SCG->SOSCCSR |= SCG_SOSCCSR_SOSCCMRE_MASK;
	}
}

/*
  * System OSC Clock Monitor Enable / Disable
  *    *
  *    */
void CMU_PLL_Config(STD_Type On_Off)
{
	/* System OSC Valid */
	if(1 == ((SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK) >> SCG_SPLLCSR_SPLLVLD_SHIFT))
	{
		if(STD_ON == On_Off)
		{
			SCG->SPLLCSR |= SCG_SPLLCSR_SPLLCM_MASK;
		}
		else
		{
			SCG->SPLLCSR &= (~SCG_SPLLCSR_SPLLCM_MASK);
		}
	}
	else
	{
		SCG->SPLLCSR &= (~SCG_SPLLCSR_SPLLCM_MASK);
	}
}

/*
  * Clock Monitor generates Interrupt or Reset when error detected
  *    *
  *    */
void CMU_PLL_IERE_Config(CMU_INT_RST_Type IE_RE)
{
	if(CMU_Interrupt_On_Error == IE_RE)
	{
		SCG->SPLLCSR &= (~SCG_SPLLCSR_SPLLCMRE_MASK);
	}
	else
	{
		SCG->SPLLCSR |= SCG_SPLLCSR_SPLLCMRE_MASK;
	}
}

/*
 * Write 1 to clear SOSCERR.
*/
void CMU_SOSC_Error_Clear(void)
{
	SCG->SOSCCSR |= SCG_SOSCCSR_SOSCERR_MASK;
}

/*
 * Write 1 to clear SOSCERR.
*/
void CMU_SPLL_Error_Clear(void)
{
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLERR_MASK;
}

/************************************************************************************
 * (Recommended settings "OSC monitor enable, and reset on error", "PLL monitor enable, and reset on error".
 * If SCG_SPLLCSR_SPLLCMRE == 0 and SCG_SOSCCSR_SOSCCMRE == 0,
 * 1) The disconnection between XOSC and MCU pin will trigger the SCG_IRQHandler.
 * 2) The short between XTAL and EXTAL will trigger MCU Lockup reset.
 **************************************************************************************/
void SCG_IRQHandler(void)
{

	/* Code will be blocking until clock recover */
	INT_SYS_DisableIRQGlobal();

	/* Customer can add code here to handle loss of clock interrupt if clock monitor is configured to generate interrupt instead of reset. */
	while(SCG_SOSCCSR_SOSCERR_MASK == (SCG->SOSCCSR & SCG_SOSCCSR_SOSCERR_MASK) ||\
		  SCG_SPLLCSR_SPLLERR_MASK == (SCG->SPLLCSR & SCG_SPLLCSR_SPLLERR_MASK))
	{
		CMU_SOSC_Error_Clear();
		CMU_SPLL_Error_Clear();
	}
}

void LPIT0_Ch3_IRQHandler(void)
{
	LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT1, 1<<3);
	WDOG_DRV_Trigger(INST_WATCHDOG1);
}

/********************************************************************************************
   * CMU related settings is already initialized in SDK function.
   * It's required to check the clock related settings are correct:
   * 1) XOSC is valid and CMU is enabled;
   * 2) SPLL is valid and CMU is enabled;
   * 3) SPLL is system clock;
   * 4)
   * These settings are configured in "clockMan1" component.
*********************************************************************************************/
void CMU_Init(void)
{
	uint32_t ret = 0;

	CMU_Enable();

	//SPLL is system clock;
	if( (SCG->CSR & SCG_CSR_SCS_MASK) != SCG_CSR_SCS(6) )
		ret ++;
	//XOSC is valid and CMU is enabled, reset on error;
	if( (SCG->SOSCCSR & (SCG_SOSCCSR_SOSCEN_MASK | SCG_SOSCCSR_SOSCVLD_MASK | SCG_SOSCCSR_SOSCCMRE_MASK | SCG_SOSCCSR_SOSCCM_MASK)) != \
			(SCG_SOSCCSR_SOSCEN_MASK | SCG_SOSCCSR_SOSCVLD_MASK | SCG_SOSCCSR_SOSCCMRE_MASK | SCG_SOSCCSR_SOSCCM_MASK) )
		ret ++;
	//SPLL is valid and CMU is enabled, reset on error;
	if( (SCG->SPLLCSR & (SCG_SPLLCSR_SPLLEN_MASK | SCG_SPLLCSR_SPLLVLD_MASK | SCG_SPLLCSR_SPLLCMRE_MASK | SCG_SPLLCSR_SPLLCM_MASK)) != \
			(SCG_SPLLCSR_SPLLEN_MASK | SCG_SPLLCSR_SPLLVLD_MASK | SCG_SPLLCSR_SPLLCMRE_MASK | SCG_SPLLCSR_SPLLCM_MASK) )
		ret ++;
	if(ret > 0)
	{
		faultBuffer.faultType = faultClockConfig;
		faultBuffer.faultPcAddr = ret;
		faultBuffer.faultDataAddr = 0;
		Safety_FaultManage_SaveFault(&faultBuffer);
	}
	//check SCG error flags
	if( (SCG->SOSCCSR & SCG_SOSCCSR_SOSCERR_MASK) && (SCG->SPLLCSR & SCG_SPLLCSR_SPLLERR_MASK) )
	{
		//loss of clock errors;
		faultBuffer.faultType = faultCMULOC;
		faultBuffer.faultPcAddr = 0;
		faultBuffer.faultDataAddr = 0;
		Safety_FaultManage_SaveFault(&faultBuffer);
		SCG->SOSCCSR |= SCG_SOSCCSR_SOSCERR_MASK;	//clear loss of SOSC clock error flag
		SCG->SPLLCSR |= SCG_SPLLCSR_SPLLERR_MASK;	//clear loss of SPLL clock error flag
	}
    INT_SYS_EnableIRQ(SCG_IRQn);
}

/****************************************************************************************
 * This function disables CMU. It is required when MCU is going into sleep modes.
 ****************************************************************************************/
void CMU_Enable(void)
{
	SCG->SOSCCSR |= SCG_SOSCCSR_SOSCCMRE_MASK|SCG_SOSCCSR_SOSCCM_MASK;	//Turn on SOSC monitor
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLCMRE_MASK|SCG_SPLLCSR_SPLLCM_MASK;	//Turn on SPLL monitor
}

/****************************************************************************************
 * This function disables CMU. It is required when MCU is going into sleep modes.
 ****************************************************************************************/
void CMU_Disable(void)
{
	SCG->SOSCCSR &= ~SCG_SOSCCSR_SOSCCM_MASK;	//Turn off SOSC monitor
	SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLCM_MASK;	//Turn off SPLL monitor
}


/*******************************************************************************************
 * Configure LPTMR using XOSC, LPIT using SPLL.
 * Compare the two timers' values to assure SPLL frequency accuracy.
 *******************************************************************************************/
uint32_t cntLpit = 0;
uint32_t cntLptmr = 0;
uint32_t cntWdog = 0;
void CMU_ClockFreqCheck(void)
{
	LPIT_DRV_Init(INST_LPIT1, &lpit1_InitConfig);			//init LPIT, clocked by SPLL/2=40MHz
	LPIT_DRV_InitChannel(INST_LPIT1, 3, &lpit1_ChnConfig0);	//configure LPIT channel 3 timer
	LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_config0, false);	//start free running 16-bit timer LPTMR, clocked by XOSC 8MHz/4 = 2MHz, prescaler 4
	cntWdog = WDOG_DRV_GetCounter(INST_WATCHDOG1);			//get WDOG start timer, clocked by LPO 128KHz
	cntLpit = LPIT_DRV_GetTimerPeriodByCount(INST_LPIT1, 3);
	LPTMR_DRV_StartCounter(INST_LPTMR1);
	LPIT_DRV_StartTimerChannels(INST_LPIT1, 1<<3);			//start LPIT channel 3 timer
	INT_SYS_EnableIRQ(LPIT0_Ch3_IRQn);
	while( cntLptmr < 200 )									//wait for 1mS
	{
		cntLptmr = (uint32_t) LPTMR_DRV_GetCounterValueByCount(INST_LPTMR1);
	}
	cntLpit = cntLpit - LPIT_DRV_GetCurrentTimerCount(INST_LPIT1, 3);
	cntWdog = WDOG_DRV_GetCounter(INST_WATCHDOG1) - cntWdog;
	if( (cntWdog > 11) && (cntWdog < 15) )		// expected value is 32 = 200 / (2MHz/128KHz) = 12.8;
		selfTestStatus |= SELFTEST_PASS_XOSC;
	else
		selfTestStatus &= ~SELFTEST_PASS_XOSC;
	if( (cntLpit > cntLptmr*20-200) && (cntLpit < cntLptmr*20+200) )
		selfTestStatus |= SELFTEST_PASS_SPLL;
	else
		selfTestStatus &= ~SELFTEST_PASS_SPLL;
	LPTMR_DRV_StopCounter(INST_LPTMR1);
}
