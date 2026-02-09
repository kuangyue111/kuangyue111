/*
 * Safety_MPU.c
 *
 *  Created on: Apr 18, 2019
 *      Author: B33124
 */
#include "Safety_MPU.h"
#include "Safety_FaultSave.h"
#include "Safety_RCM.h"

mpu_access_err_info_t mpuErrorInfo;
uint32_t mpuErrorCheckFlag = 0;

void MPU_Config(void)
{
	MPU_DRV_Init(MEMPROTECT1, MPU_NUM_OF_REGION_CFG0, memProtect1_UserConfig0);
}

/*********************************************************************
 * Write 1 to MPU CESR VLD bit to enable MPU globally.
 *********************************************************************/
void MPU_GlobalEnable(void)
{
	MPU->CESR = MPU_CESR_VLD_MASK;
}

/*********************************************************************
 * Write 0 to MPU CESR VLD bit to disable MPU globally. Other bits in CESR can be written with 0 without other effects.
 *********************************************************************/
void MPU_GlobalDisable(void)
{
	MPU->CESR = 0;
}

/**********************************************************************************
 * Afte POR reset, inject MPU errors to verify its functionality
 **********************************************************************************/
void MPU_ErrorInjectionTest(void)
{
	mpuErrorCheckFlag = 0x55AA55AA;
	*(uint32_t *)(0x0100) = mpuErrorCheckFlag;		//MPU_InjectError(); this address 0x0100 is read only region
	if(mpuErrorCheckFlag == 0x33CC33CC)				//if MPU error detection logic check failed
		selfTestStatus |= SELFTEST_PASS_MPU_INJ;
	else
		selfTestStatus &= ~SELFTEST_PASS_MPU_INJ;
	mpuErrorCheckFlag = 0;
}
