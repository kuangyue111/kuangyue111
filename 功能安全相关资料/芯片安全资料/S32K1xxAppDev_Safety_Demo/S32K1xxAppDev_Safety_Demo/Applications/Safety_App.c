/*
 * Safety_App.c
 *
 *  Created on: May 8, 2019
 *      Author: B33124
 */

/**************************************************************************************
 * For safety features demo, the following modfications are made to this project:
 * 1) Add WDOG_PorTest to beginning of SystemInit();
 * 2) Add Reset source reading asm code to skip SRAM init code for some resets;
 * 3) Modify linker file to add user data section in SRAM and EEPROM data section;
 ***************************************************************************************/

#include <stdio.h>
#include "Safety_App.h"
#include "BSP_Config.h"

void cmdPrintInitInfo(void);

uint32_t wdogRefreshDisable = 0;

char *resetSourceString[] = {
	"ResetSourceUnknown\0",
	"ResetSourceLVD\0",
	"ResetSourceLOC\0",
	"ResetSourceLOL\0",
	"ResetSourceCMULOC\0",
	"ResetSourceWDOG\0",
	"ResetSourcePIN\0",
	"ResetSourcePOR\0",
	"ResetSourceJTAG\0",
	"ResetSourceLOCKUP\0",
	"ResetSourceSoft\0",
	"ResetSourceMDMAP\0",
	"ResetSourceWDOGTest\0",
	"ResetSourceSACKERR\0",
	"ResetSourceReserved\0"
};

char *faultSourceString[] = {
	"faultPOR\0",
	"faultLVD\0",
	"faultWDOG\0",
	"faultSafetyBoot\0",
	"faultLOC\0",
	"faultLOL\0",
	"faultCMULOC\0",
	"faultResetPIN\0",
	"faultLockup\0",
	"faultFlashEcc\0",
	"faultFlashProtection\0",
	"faultSramEcc\0",
	"faultSmpu\0",
	"faultHVD\0",
	"faultNVIC\0",
	"faultCoreSCST\0",
	"faultStackOverflow\0",
	"faultSelfTest\0",
	"faultRegCheck\0",
	"faultUnknown\0",
	"faultHardFault\0",
	"faultClockConfig\0",
	"faultAdcDiagnostic\0"
};

mutex_t mutexCrc;

/*******************************************************************************************
 * the ADC test application task
 * trigger the ADC sample and convert, then read the result and send to PC
 *******************************************************************************************/
void Task_SafetyDiagnostic(void *pvParameters)
{
	SAFETY_InitialCheck();
	cmdPrintInitInfo();
	OSIF_MutexCreate(&mutexCrc);	//create a mutex to lock/unlock CRC module

	while(1)
	{
		PINS_DRV_TogglePins(PTE, (1<<10));		//use PTE10 toggle to measure SCST execution time
		//if(wdogRefreshDisable == 0)
		//	WDOG_DRV_Trigger(INST_WATCHDOG1);

		PMC_Monitor_Task();
		if( OSIF_MutexLock(&mutexCrc, 0) == STATUS_SUCCESS)
		{
			RegCheck_Compare(regPortPcrCrcBaseValue);
			OSIF_MutexUnlock(&mutexCrc);
		}

		if(bEnableScstRun)
			SCST_RunTestSeries();
		PINS_DRV_TogglePins(PTE, (1<<10));


		OSIF_TimeDelay(100);/*delay 100ms*/
	}

}

void cmdPrintInitInfo(void)
{
	uint32_t u32Tmp;
	char tmpBuf[70];

	/* Print reset source */
	UART_Print("\r\n\r\nMCU booting...\r\n");
	u32Tmp = (uint32_t)RCM_ReadResetSource();
	snprintf(tmpBuf, 70, "Reset source is %s, Reset count is %d.\r\n", resetSourceString[u32Tmp], (int)resetCountVar);
	UART_Print(tmpBuf);
	//printf("Reset source is %s.\r\n", ptrStr);
	//UART_Print("Reset source is ");	UART_Print(ptrStr);	UART_Print("\r\n");

	snprintf(tmpBuf, 70, "Watchdog enable is %d, test mode is %d.\r\n",
			(int)(WDOG->CS & WDOG_CS_EN_MASK)>>WDOG_CS_EN_SHIFT,
			(int)(WDOG->CS & WDOG_CS_TST_MASK)>>WDOG_CS_TST_SHIFT);
	UART_Print(tmpBuf);
	snprintf(tmpBuf, 70, "EEPROM ready = %d; EEESIZE = 0x%x, DEPART = 0x%x.\r\n",
			(int)faultEepromReady,
			(unsigned int)(SIM->FCFG1 & SIM_FCFG1_EEERAMSIZE_MASK)>>SIM_FCFG1_EEERAMSIZE_SHIFT,
			(unsigned int)(SIM->FCFG1 & SIM_FCFG1_DEPART_MASK)>>SIM_FCFG1_DEPART_SHIFT);
	UART_Print(tmpBuf);
	snprintf(tmpBuf, 50, "Number of fault: %d; Fault Bitmap: 0x%08x.\r\n", (int)Safety_FaultManage_GetFaultCount(), (int)Safety_FaultManage_GetFaultBitmap());
	UART_Print(tmpBuf);
	snprintf(tmpBuf, 50, "Self-test status flag: 0x%04X.\r\n\r\n", (unsigned int)selfTestStatus);
	UART_Print(tmpBuf);
}


/*******************************************************************************
 * This callback function is entered when FreeRTOS detected the data 0xA5A5A5A5
 * at stack start address was modified.
 ******************************************************************************/
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	faultBuffer.faultType = faultStackOverflow;
	faultBuffer.faultPcAddr = 0;
	Safety_FaultManage_SaveFault(&faultBuffer);
	//UART_Print("stack overflow detected. Reboot......\r\n");
	SystemSoftwareReset();
	return;
}
