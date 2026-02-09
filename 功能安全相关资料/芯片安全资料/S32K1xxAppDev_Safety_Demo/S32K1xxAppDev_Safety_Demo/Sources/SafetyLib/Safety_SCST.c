/*
 * Safety_SCST.c
 *
 *  Created on: Jul 18, 2019
 *      Author: B33124
 */
#include "Safety_SCST.h"
#include "m4_scst_configuration.h"
#include "Safety_FaultSave.h"
#include "Safety_RCM.h"

const uint32_t m4ScstSignature[M4_SCST_TESTS_NUM] = {
	0x000012E0,
	0x00000E7C,
	0x000006EA,
	0x00000D89,
	0x00001A69,
	0x0000205A,
	0x00000C3B,
	0x00000FE3,
	0x0000256C,
	0x00002784,
	0x00002B88,
	0x0000302F,
	0x000047DB,
	0x00001C21,
	0x7E256E6B,
	0x00003A0E,
	0xB91BDE95,
	0xB14D3A0D,
	0xC19005EE,
	0xC63C044C,
	0xAE1D1D83,
	0x5B80787E,
	0x618271EE,
	0xF61A1F74,
	0xE35DF821,
	0x28D6AF31,
	0xC726A3D2,
	0x00001AC7,
	0x0000A828,
	0x00011B11,
	0x00002C09,
	0x0000ADEF,
	0x00003F51,
	0x00008C65,
	0x00006E5A,
	0x00002022,
	0x1C464F74,
	0x78C61FF0,
	0x24CA596B,
	0xC4527367,
	0x38A82153,
	0x53273023,
	0x17A3FF6A,
	0x75DB236C
};

uint32_t scstRslt = 0;
uint32_t desiredSignature = 0;
uint32_t numStart = 0;
uint32_t numEnd = M4_SCST_TESTS_NUM-1;
uint32_t numCurrentTest = 0;
uint32_t bEnableScstRun = 0;
uint32_t cntScstInterrupted = 0;
uint32_t cntScstCompleted = 0;

/**************************************************************************************************
 * Calculate the combined signatures of specified SCST tests
 **************************************************************************************************/
uint32_t SCST_CalcSignature(uint32_t nStart, uint32_t nEnd)
{
	uint32_t i;
	uint32_t return_value = 0;
	if ( (nEnd < nStart) || (nEnd >= M4_SCST_TESTS_NUM) )
	{
		return_value = M4_SCST_WRONG_RANGE;
	}
	else
	{
		for( i=nStart; i<nEnd+1; i++ )
		{
			return_value ^=  m4ScstSignature[i];
		}
	}
	return return_value;
}

/**************************************************************************************************
 * Run a series of tests and compare the results with expected signatures;
 * if test result matches signature, return 1; meaning core no fault;
 * else return 0; meaning core fault detected;
 * This function can be called periodically within FTTI to check core faults.
 * The test series start from numStart and end at numEnd.
 * Call function SCST_SetStartNum() to set the start test number numStart;	0 by default
 * Call function SCST_SetEndNum() to set the end test number numEnd;		43 by default (namely covering all scst tests)
 **************************************************************************************************/
uint32_t SCST_RunTestSeries(void)
{
	scstRslt = 0;

	if ( (numEnd < numStart) || (numEnd >= M4_SCST_TESTS_NUM) )
	{
		scstRslt = M4_SCST_WRONG_RANGE;
		return 0;
	}
	else
	{
		scstRslt = m4_scst_execute_core_tests(numStart, numEnd);
		desiredSignature = SCST_CalcSignature(numStart, numEnd);
		//scstRslt ^= m4_scst_execute_core_tests(14, 15);
		//scstRslt ^= m4_scst_execute_core_tests(17, 43);
		//desiredSignature = SCST_CalcSignature(14, 15);
		//desiredSignature ^= SCST_CalcSignature(17, 43);
		if( scstRslt == desiredSignature )
		{
			cntScstCompleted++;
			return 1;
		}
		else if( scstRslt == M4_SCST_TEST_WAS_INTERRUPTED )
		{
			cntScstInterrupted++;
			return 1;
		}
		else	//test result doesn't match the desired signature
		{
			faultBuffer.faultType = faultCoreSCST;
			faultBuffer.faultPcAddr = m4_scst_last_executed_test_number;
			faultBuffer.faultDataAddr = scstRslt;
			SCST_FaultCallback();
			m4_scst_fault_inject_test_index = 0xFFFFFFFF;
			return 0;
		}
	}
}

/**************************************************************************************************
 * This function is almost the same as "SCST_RunTestSeries()".
 * The difference is that it will try again if the result indicates the test is interrupted.
 * It will try 3 times to get a result and compare with desired signature.
 * If they match, the global variable "selfTestStatus" bit 9 is set.
 * Customer can use this function to run test once after MCU Power-up.
 **************************************************************************************************/
void SCST_RunPorTest(void)
{
	uint32_t i;
	uint32_t ret = 0;
	scstRslt = 0;

	if ( (numEnd < numStart) || (numEnd >= M4_SCST_TESTS_NUM) )
	{
		scstRslt = M4_SCST_WRONG_RANGE;
		ret = 0;
	}
	else
	{
		for(i=0; i<3; i++)
		{
			scstRslt = m4_scst_execute_core_tests(numStart, numEnd);
			desiredSignature = SCST_CalcSignature(numStart, numEnd);
			//scstRslt ^= m4_scst_execute_core_tests(14, 15);
			//scstRslt ^= m4_scst_execute_core_tests(17, 43);
			//desiredSignature = SCST_CalcSignature(14, 15);
			//desiredSignature ^= SCST_CalcSignature(17, 43);
			if( scstRslt == desiredSignature )
			{
				ret = 1;
				break;
			}
			else if( scstRslt == M4_SCST_TEST_WAS_INTERRUPTED )
			{
				continue;
			}
			else	//test result doesn't match the desired signature
			{
				ret = 0;
				break;
			}
		}
	}
	if(ret)
		selfTestStatus |= SELFTEST_PASS_SCST;
	else
		selfTestStatus &= ~SELFTEST_PASS_SCST;
}

/**************************************************************************************************
 * Run the scst test specified by "numCurrentTest" and compare the result with expected signature;
 * if test result matches signature, return 1; meaning core no fault;
 * else return 0; meaning core fault detected;
 * This function can be called periodically within FTTI to check core faults.
 * After each call of this function, the current test number "numCurrentTest" increased by 1, targeted for next test
 * Call function SCST_SetCurrentTest() to set the number "numCurrentTest";	0 by default
 **************************************************************************************************/
uint32_t SCST_RunTestSingle(void)
{
	//uint32_t scstRslt1;
	if ( numCurrentTest > (M4_SCST_TESTS_NUM-1) )
	{
		scstRslt = M4_SCST_WRONG_RANGE;
		return 0;
	}
	else
	{
		scstRslt = m4_scst_execute_core_tests(numCurrentTest, numCurrentTest);
		desiredSignature = SCST_CalcSignature(numCurrentTest, numCurrentTest);
		numCurrentTest++;
		if(numCurrentTest >= M4_SCST_WRONG_RANGE)
			numCurrentTest = 0;
		if( scstRslt == desiredSignature )
			return 1;
		else	//test result doesn't match the desired signature
		{
			//m4_scst_last_executed_test_number
			//m4_scst_test_was_interrupted
			faultBuffer.faultType = faultCoreSCST;
			faultBuffer.faultPcAddr = m4_scst_last_executed_test_number;
			faultBuffer.faultDataAddr = scstRslt;
			Safety_FaultManage_SaveFault(&faultBuffer);
			SCST_FaultCallback();
			m4_scst_fault_inject_test_index = 0xFFFFFFFF;
			return 0;
		}
	}
}

/**************************************************************************************************
 * Callback function that is triggered when SCST fault detected
 * This function is an example. It saves fault information and turn off fault injection.
 * Customer need create code to handle this fault according to system requirements.
 **************************************************************************************************/
void SCST_FaultCallback(void)
{

}

/**************************************************************************************************
 * Set start test number
 **************************************************************************************************/
void SCST_SetStartNum(uint32_t nStart)
{
	numStart = nStart;
}

void SCST_SetEndNum(uint32_t nEnd)
{
	numEnd = nEnd;
}

uint32_t SCST_GetStartNum(void)
{
	return numStart;
}

uint32_t SCST_GetEndNum(void)
{
	return numEnd;
}

uint32_t SCST_GetTestResult(void)
{
	return scstRslt;
}

uint32_t SCST_GetSignature(void)
{
	return desiredSignature;
}

void SCST_SetCurrentTest(uint32_t numTest)
{
	numCurrentTest = numTest;
}

void SCST_Enable(void)
{
	bEnableScstRun = 1;
}

void SCST_Disable(void)
{
	bEnableScstRun = 0;
}
