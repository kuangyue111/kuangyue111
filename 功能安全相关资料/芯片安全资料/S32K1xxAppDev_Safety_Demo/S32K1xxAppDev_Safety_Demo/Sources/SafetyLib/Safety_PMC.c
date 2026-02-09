/*
 * Safety_PMCMonitor.c
 *
 *  Created on: 2019Äê5ÔÂ20ÈÕ
 *      Author: nxf47391
 */

#include "SafetyLib.h"
#include "ADC_App.h"

#define BANDGAP_MV      1000
#define S32K1XX_VDDMAX  5500

/**********************************************************************************
* Low Voltage Detect Interrupt enables/disables
***********************************************************************************/
void PMC_LVDIE_Config(STD_Type On_Off)
{
    if(STD_ON == On_Off)
    {
        PMC->LVDSC1 |= PMC_LVDSC1_LVDIE_MASK;
    }
    else
    {
        PMC->LVDSC1 &= (~PMC_LVDSC1_LVDIE_MASK);
    }
}

/**********************************************************************************
* Low Voltage Detect Reset enables/disables
***********************************************************************************/
void PMC_LVDRE_Config(STD_Type On_Off)
{
    if(STD_ON == On_Off)
    {
        PMC->LVDSC1 |= PMC_LVDSC1_LVDRE_MASK;
    }
    else
    {
        PMC->LVDSC1 &= (~PMC_LVDSC1_LVDRE_MASK);
    }
}

/**********************************************************************************
* Low Voltage Detect Warning Interrupt enables/disables
**********************************************************************************/
void PMC_LVWIE_Config(STD_Type On_Off)
{
    if(STD_ON == On_Off)
    {
        PMC->LVDSC2 |= PMC_LVDSC2_LVWIE_MASK;
    }
    else
    {
        PMC->LVDSC2 &= (~PMC_LVDSC2_LVWIE_MASK);
    }
}

/**********************************************************************************
* Write 1 to clear LVDF.
**********************************************************************************/
void PMC_LVDF_Clear(void)
{
    PMC->LVDSC1 |= PMC_LVDSC1_LVDACK_MASK;
}

/**********************************************************************************
* Write 1 to clear LVWF.
**********************************************************************************/
void PMC_LVWF_Clear(void)
{
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
}

/**********************************************************************************
* LVD LVW interrupt handler, entered when LVDIE or LVWIE is set to 1.
**********************************************************************************/
void LVD_LVW_IRQHandler(void)
{
    uint8_t num = PMC_NONE;

    //low voltage detection interrupt, keep CPU running in this loop until LVD fault disappear
    if(PMC_LVDSC1_LVDF_MASK == (PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK))
    {
        num = PMC_LVD;
        INT_SYS_DisableIRQGlobal();
       	Safety_PmcFault_Callback(num);
        while(PMC_LVDSC1_LVDF_MASK == (PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK))
		{
			PMC_LVDF_Clear();
			WDOG_REFRESH();
		}
		INT_SYS_EnableIRQGlobal();
    }
    //low voltage warning interrupt, keep CPU running in this loop until LVW fault disappear
    else if(PMC_LVDSC2_LVWF_MASK == (PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK))
    {
        num = PMC_LVW;
        INT_SYS_DisableIRQGlobal();
        Safety_PmcFault_Callback(num);
		while(PMC_LVDSC2_LVWF_MASK == (PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK))
		{
			PMC_LVWF_Clear();
			WDOG_REFRESH();
		}
		INT_SYS_EnableIRQGlobal();
    }
    else	//no LVD/LVW interrupt flag set
    {
    	SystemSoftwareReset();
    }


}

/**********************************************************************************
* Initialize Low Voltage Detect
**********************************************************************************/
void PMC_Init(void)
{
	PMC_LVWIE_Config(STD_OFF);
	PMC_LVDIE_Config(STD_ON);
	PMC_LVDRE_Config(STD_ON);

    /* Install hander for LVD_LVW ISR */
    INT_SYS_InstallHandler(LVD_LVW_IRQn, LVD_LVW_IRQHandler, (isr_t *)0);
    /* Enable LVD_LVW IRQ */
    INT_SYS_EnableIRQ(LVD_LVW_IRQn);
}

/***********************************************************************
 * Sample the MCU internal ADC channel - VDD and Bandgap - to calculate the actual power supply voltage.
 * This calculation is to detect high voltage on VDD, and will trap the CPU in case VDD higher than S32K1XX_VDDMAX.
 * 1) ADC result adc_pal1_Results00[1] is Bandgap sampling value;
 * 2) ADC result adc_pal1_Results00[2] is VDD u32Vddmvsampling value;
 ***********************************************************************/
uint32_t u32Vddmv = 0u;
void PMC_Monitor_Task(void)
{
    uint32_t Vref_mV = 0u;

    ADC_StartGroupConversion(&adc_pal1_instance, 0/* groupIdx */);
    if(STATUS_SUCCESS == OSIF_SemaWait(ADC0_ConvertComplete_Sema,10))
    {
        Vref_mV = (BANDGAP_MV << 12) / adc_pal1_Results00[1];
        u32Vddmv = (adc_pal1_Results00[2] * Vref_mV) >> 12;
    }

    if(u32Vddmv > S32K1XX_VDDMAX)	//if HVD detected, save fault and trigger MCU reset
    {
    	faultBuffer.faultType = faultHVD;
    	faultBuffer.faultPcAddr = u32Vddmv;
    	faultBuffer.faultDataAddr = S32K1XX_VDDMAX;
    	Safety_FaultManage_SaveFault(&faultBuffer);
    	Safety_PmcFault_Callback(PMC_HVD);
    	SystemSoftwareReset();
    }
}
