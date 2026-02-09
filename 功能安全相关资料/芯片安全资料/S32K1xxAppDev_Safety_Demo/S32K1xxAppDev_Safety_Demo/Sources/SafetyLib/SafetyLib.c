/*
 * SafetyLib.c
 *
 *  Created on: Mar 13, 2019
 *      Author: B33124
 */

#include "Cpu.h"
#include "SafetyLib.h"

/**********************************************************************************
 * This function includes all safety-relevant initial check functions.
 *********************************************************************************/
void SAFETY_InitialCheck(void)
{
	LMEM_CacheDisable();
	RCM_SaveResetSource();
	MPU_DRV_Init(MEMPROTECT1, MPU_NUM_OF_REGION_CFG0, memProtect1_UserConfig0);				//MPU enable
	WDOG_DRV_Init(INST_WATCHDOG1, &watchdog1_Config0);
	CRC_DRV_Init(INST_CRC1, &crc1_InitConfig0);
	Safety_FaultManage_Init();
	Safety_FaultManage_ProcessResetFault();
	PMC_Init();					//power monitor init
	CMU_Init();					//clock monitor init, enalbe, and read clock errors;
	ERM_DRV_Init(INST_ERM1, ERM_CHANNEL_COUNT0, erm1_InitConfig0);	//SRAM ERM init
	RegCheck_Init();			//registers CRC values init

	if(RCM_ReadResetSource() == ResetSourcePOR)
	{
		CMU_ClockFreqCheck();		//check XOSC frequency, and SPLL frequency
		CRC_SelfTest();				//CRC self-test
		WDOG_GetTestResult();		//WDOG_PorTest() is called in startup.s, here only need get test result
		ADC_PorSelfTest();			//ADC0 and ADC1 self-test after power-up
		MPU_ErrorInjectionTest();	//MPU error injection and verification
		SRAM_EccEimTest();			//SRAM ECC test using EIM;
		FTFC_EccSelfTest();			//flash ECC injection and verification, it should not run during safe boot Flash CRC calculation
		Safeboot_StartCrcDma();		//start CRC calculation of specified PFlash areas;
		SCST_RunPorTest();			//SCST core self-test
	}
	else
	{
		//CMU is disabled after each reset, so it's required to initialize for each reset;
		//Another reason is this function also enable LPIT0 ch3 for ISR to refresh WDOG;
		CMU_ClockFreqCheck();

		//if it's non-POR reset, and the code flash reference CRC value is not generated yte, run safeboot function
		if( safeCrcRefValue == 0xFFFFFFFF )
			Safeboot_StartCrcDma();

	}
	//SAFETY_SelfTestResultRead();

	WDOG_DRV_Trigger(INST_WATCHDOG1);	//refresh watchdog
	SCST_Enable();
	LMEM_CacheEnable();
	while(bCrcInUse);		//wait till Safe boot finish Flash CRC calculation.
	if(safeBootNewMcu)		//if it's a new MCU
		Safety_FaultManage_ClearAllFault();
	//PINS_DRV_ClearPins(PTE, (1<<11));
}

/*******************************************************************************
 * Read self-test result and record a fault if the result indicate at least one self-test is not passed.
 ********************************************************************************/
void SAFETY_SelfTestResultRead(void)
{
	if(selfTestStatus != SELFTEST_PASS_ALL)
	{
		if( (Safety_FaultManage_GetFaultBitmap() & (1<<faultSelfTest)) ==  0)
		{
			faultBuffer.faultType = faultSelfTest;
			faultBuffer.faultPcAddr = selfTestStatus;
			faultBuffer.faultDataAddr = 0;
			Safety_FaultManage_SaveFault(&faultBuffer);
		}
	}
}
