/*
 * Safety_SCST.c
 *
 *  Created on: 2021年9月27日
 *      Author: zsy342
 */
#include "BSPTypeDef.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "m4_scst_typedefs.h"
#include "m4_scst_configuration.h"
#include "m4_scst_test_shell.h"
#include "Safety_SCST.h"

///* Linker defined variables used to copy initialized data from ROM to RAM */
extern uint32_t __SCST_DATA_ROM[];
extern uint32_t __SCST_DATA_SIZE[];
extern uint32_t __scst_data_start__[];

uint8_t ScstSelfTestStatus = 0;    /*1:正常   0：异常*/

/* Define Test ID for test where will be injected error */
#define INJECT_ERROR_TO_TEST_NUM 40U
#define ACCUMULATED_SIGNATURE_ID0_43 0x929940F1U

uint32_t  ScsttimerCounter = 0;
void SCST_SelfTest(void)
{
	u32 result = 0;
	u8 errorNum = 0;

	/* Copy sCST initialized data from ROM to RAM */
	memcpy(__scst_data_start__, __SCST_DATA_ROM, (uint32_t)__SCST_DATA_SIZE);

	/* Execute all tests */
	result = m4_scst_execute_core_tests(24, 24);/*m4_scst_regbank_test1*/

	if(result != m4_scst_regbank_test1)
	{
		errorNum++;
	}

	result = m4_scst_execute_core_tests(25, 25);/*m4_scst_regbank_test2*/

	if(result != m4_scst_regbank_test2)
	{
		errorNum++;
	}

	result = m4_scst_execute_core_tests(26, 26);/*m4_scst_regbank_test3*/

	if(result != m4_scst_regbank_test3)
	{
		errorNum++;
	}

//  .S文件中，大部分代码被注释。原因不清楚
//	result = m4_scst_execute_core_tests(12, 12);/*m4_scst_regbank_test4*/
//
//	if(result != m4_scst_regbank_test4)
//	{
//		errorNum++;
//	}

	result = m4_scst_execute_core_tests(27, 27);/*m4_scst_regbank_test5*/

	if(result != m4_scst_regbank_test5)
	{
		errorNum++;
	}

	result = m4_scst_execute_core_tests(16, 16);/*m4_scst_regbank_test6*/

	if(result != m4_scst_regbank_test6)
	{
		errorNum++;
	}

	if(errorNum > 0)/*0:正常  1:异常*/
	{
		ScstSelfTestStatus = 1;
	}
	else
	{
		ScstSelfTestStatus = 0;
		ScsttimerCounter++;
		if(ScsttimerCounter > 20)
		{
			ScsttimerCounter = 30;
			ScstSelfTestStatus = 0;
		}
	}
}

uint8_t GetScstSelfTestStatus(void)
{
	return(ScstSelfTestStatus);
}











