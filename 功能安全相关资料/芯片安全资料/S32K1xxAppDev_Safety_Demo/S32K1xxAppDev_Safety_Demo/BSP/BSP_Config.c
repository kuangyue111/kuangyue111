/*******************************************************************************
*
* Copyright 2006-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
****************************************************************************//*!
*
* @file     peripherals_config.c
*
* @date     March-28-2017
*
* @brief    MCU Peripherals Configuration
*
*******************************************************************************/

/*******************************************************************************
* Includes
*******************************************************************************/
#include <BSP_Config.h>

 mutex_t UART_Print_Mutex;/*the mutex for UART print peripheral lock*/

/*******************************************************************************
*
* Function: 	void McuClockConfig(void)
*
* Description:  This function installs the pre-defined clock configuration table
* 				to the clock manager. For more details see configuration
* 				in Processor Expert.
*
*******************************************************************************/
void McuClockConfig(void)
{
	/* Clock configuration for MCU and MCU's peripherals */
	CLOCK_SYS_Init(g_clockManConfigsArr,
	               CLOCK_MANAGER_CONFIG_CNT,
	               g_clockManCallbacksArr,
	               CLOCK_MANAGER_CALLBACK_CNT);

	/* Clock configuration update */
	CLOCK_SYS_UpdateConfiguration(0, CLOCK_MANAGER_POLICY_FORCIBLE);
}


/*******************************************************************************
*
* Function: 	status_t UART_Print(uint8_t *string)
*
* Description:  send out the user string via the EVB UART
*
*******************************************************************************/
#ifdef DEBUG_PRINT_ENABLE

status_t UART_Print(char *string)
{
	status_t returnCode = STATUS_SUCCESS;

	/*get to lock the LPUART resource*/
	if(STATUS_SUCCESS==OSIF_MutexLock(&UART_Print_Mutex,0))
	{
		/* send out the string */
		returnCode = LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)string, strlen(string), 1000);

		OSIF_MutexUnlock(&UART_Print_Mutex);/*unlock the LPUART resource*/
	}

	return returnCode;
}

#else
/*empty the UART_Print function to disable the debug print*/
status_t UART_Print(uint8_t *string)
{
	return STATUS_SUCCESS;
}
#endif



/*******************************************************************************************
* basic clock and pin function initialization
* it should be called before any other MCU peripheral initialization and task creation
*******************************************************************************************/
void MCU_SystemInit(void)
{
	/*
	* initialize the system clock(SPLL, SysCLK, CoreCLK) and peripheral clock(function clock and clock gate enable) as clock_manager component configured
	*/
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0,CLOCK_MANAGER_POLICY_FORCIBLE);

	/*
	* configure the PORT and GPIO(PIN) function as Pin_Settings component configured:
	* interrupt, filter, direction, and other properties(.eg Pull up/down/enable, drive strength, etc. )
	*/
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

	/*
	 * initialize the MCU power mode and switch to RUN mode
	 */
	McuPowerConfig();
	/*
	 * enable the CPU cache to ensure the max. CPU performance
	 */
	McuCacheConfig();

	/*
	 * initialize the eDMA module
	 * user needs to enable the exact DMA channel for some peripheral driver in app
	 */
	McuDmaConfig();

	McuIntConfig();

}

/*******************************************************************************
*
* Function: 	void McuPowerConfig(void)
*
* Description:  This function configures the Power manager for operation.
* 				For more details see configuration in Processor Expert.
*
*******************************************************************************/
void McuPowerConfig(void)
{
	/* initialize the MCU power mode configuration */
	POWER_SYS_Init(&powerConfigsArr, 0, &powerStaticCallbacksConfigsArr,0);
	/* Set MCU power mode to RUN mode */
	POWER_SYS_SetMode(RUN,POWER_MANAGER_POLICY_AGREEMENT);
}

/*******************************************************************************
*
* Function: 	void McuIntConfig(void)
*
* Description:  Enables an interrupt for a given IRQ number.
* 				For more details see configuration in Processor Expert.
*
*******************************************************************************/
void McuIntConfig(void)
{
	INT_SYS_EnableIRQ(WDOG_EWM_IRQn);
	INT_SYS_EnableIRQ(DMA1_IRQn);	//DMA channel 1 interrupt enabled for CRC data calculation
	INT_SYS_EnableIRQ(ERM_double_fault_IRQn);
}

/*******************************************************************************
*
* Function: 	void McuSimConfig(void)
*
* Description:  This function configures SIM module
*
*******************************************************************************/
void McuSimConfig(void)
{
	/* Enable interleaved mode for ADC0_SE5 and ADC1_SE15 channels on PTB1 pin */
	//SIM_HAL_SetAdcInterleaveSel(SIM, 0b0010);
	SIM->CHIPCTL |= SIM_CHIPCTL_ADC_INTERLEAVE_EN(0b0010);
}


/*******************************************************************************
*
* Function: 	void McuCacheConfig(void)
*
* Description:  This function enables Cache memory
*
*******************************************************************************/
void McuCacheConfig(void)
{
    // Enable Cache !
    // Flush and enable I cache and write buffer

    //LMEM->PCCCR = LMEM_PCCCR_ENCACHE_MASK | LMEM_PCCCR_INVW1_MASK
    //		    | LMEM_PCCCR_INVW0_MASK   | LMEM_PCCCR_GO_MASK;
}

/*******************************************************************************
*
* Function: 	void McuDmaConfig(void)
*
* Description:  This function eDMA module
* Notice: user needs to enable the exact DMA channel for some peripheral driver in app
*
*******************************************************************************/
void McuDmaConfig(void)
{
	EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, \
			edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);

}

/*******************************************************************************
*
* Function: 	void McuFlashPrefetchConfig(void)
*
* Description:  This function disable the Flash data and instruction prefetch
* Notice: this is necessary for Flash program option
*
*******************************************************************************/
void McuFlashPrefetchConfig(void)
{
    /* Disable cache to ensure that all flash operations will take effect instantly,
     * this is device dependent and necessary for Flash program option*/
#ifdef S32K144_SERIES
    MSCM->OCMDR[0u] |= MSCM_OCMDR_OCM1(0xFu);
    MSCM->OCMDR[1u] |= MSCM_OCMDR_OCM1(0xFu);
    MSCM->OCMDR[2u] |= MSCM_OCMDR_OCM1(0xFu);
#endif /* S32K144_SERIES */
}
/* End of file */
