#include "Safety_WDOG.h"
//#include "SafetyLib.h"
//#include "watchdog1.h"
#include <stdio.h>

//uint32_t wdogTestRslt __attribute__((section (".customerBootData")));

uint8_t WdogSelfTestStatus = 0;/*0:Õý³£   1£ºÒì³£*/

/**********************************************************************************************************
The purpose of this project is to demonstrate how to implement the Watchdog test feature.

For safety requirement in some application, the MCU internal Watchdog timer need be tested before being used.
After power-on reset, the WDOG reset is tested to verify the watchdog (WDOG) timer and reset functionality are working well.
The WDOG timer is 16bit, then the test is splitted into low byte test and high byte test.

This test is placed at the beginning of system_init() function and will trigger the MCU to reset twice purposely.
So this test will increase the application startup time by 0.4ms~4ms, depending on the clock source selected for WDOG.

Please check the Waveform folder for the reset signal waveforms captured by oscilloscope during the WDOG test process.

What does user need do to implement WDOG test feature in their application?
1) Call WDOG_POR_Test() at the beginning of function system_init();
2) Call WDOG_Get_Test_Result() in application to get the WDOG test result;
**********************************************************************************************************/

/**********************************************************************************************************
Function: WDOG_Disable
          unlock watchdog config registers and disable watchdog.
**********************************************************************************************************/
void WDOG_Disable(void)
{
	WDOG->CNT = 0xD928C520;                // unlock watchdog
    WDOG->TOVAL = 0x0000FEFE;            // maximum timeout value
    WDOG->CS |= WDOG_CS_UPDATE_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1);
}

void WDOG_Enable(void)
{
	WDOG->CNT = 0xD928C520;                // unlock watchdog
	WDOG->TOVAL = WDOG->TOVAL;            // keep TOVAL unchanged
	WDOG->CS |= WDOG_CS_EN_MASK;           // disable watchdog, update allowed
	//WDOG->CS = WDOG_CS_EN_MASK | WDOG_CS_UPDATE_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1) | WDOG_CS_TST(0x01);
}

/**********************************************************************************************************
This function disable WDOG and put it into Test Mode indicated by nTest
nTest = 0;	test mode disabled;
nTest = 1;	user mode enabled;
nTest = 2;	test mode lower byte
nTest = 3;	test mode higher byte
**********************************************************************************************************/
void WDOG_TestMode(uint32_t nTest)
{
	WDOG->CNT = 0xD928C520;                // unlock watchdog
    WDOG->TOVAL = 0x0000FEFE;            // maximum timeout value
    WDOG->CS = WDOG_CS_UPDATE_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1) | WDOG_CS_TST(nTest);
}

/**********************************************************************************************************
Function: WDOG_Disable
          unlock watchdog config registers, set the watchdog timeout value, and enable the watchdog.
Input: nTimeoutMsec, in msec, this function calculates the timeout count value assuming the watchdog clock is 128KHz IRC.
**********************************************************************************************************/
void WDOG_ConfigTimeout(uint32_t nTimeoutMsec)
{
	DISABLE_INTERRUPTS();
	WDOG->CNT = 0xD928C520;

	if(nTimeoutMsec*128 > 0x0000FFFF)
		WDOG->TOVAL = 0x0000FFFF;		//max TOVAL value is 0xFFFF;
	else if(nTimeoutMsec == 0)
		WDOG->TOVAL = 0x00000010;		//set a very short timeout period;
	else
		WDOG->TOVAL = nTimeoutMsec*128;	//128KHhz LPO as clock source for watchdog.

	//WDOG->CS =  WDOG_CS_EN_MASK | WDOG_CS_UPDATE_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1) | WDOG_CS_TST(0x01) ;				//watchdog enabled
	WDOG->CS = WDOG->CS | WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK;

	ENABLE_INTERRUPTS();
}


/**********************************************************************************************************
Function: WDOG_PorTest
          Call this function as early as possible - immediately after SRAM ECC init and stack pointer register init.
          See souce code in startup_S32K144.S
          It will trigger twice WDOG resets to test the watchdog timer high byte and low byte.
**********************************************************************************************************/
void WDOG_PorTest(void)
{
	//uint32_t u32TmpCnt = WDOG_TEST_MAXCNT;
	if((RCM->SRS & RCM_SRS_POR_MASK)     //POR reset need WDOG test
	 || (RCM->SRS & RCM_SRS_MDM_AP_MASK)            //Debug Reset need WDOG test
	 //|| (RCM->SRS & RCM_SRS_PIN_MASK)  //external PIN reset need WDOG test, customer can remove this line if it's not needed
	)
	{
		WDOG->CNT = 0xD928C520;					//unlock watchdog registers
		WDOG->TOVAL = WDOG_TEST_TO_VALUE;       //watchdog timeout value for doing test
		WDOG->CS = WDOG_CS_EN_MASK |            //watchdog enable
              WDOG_CS_UPDATE_MASK |             //watchdog update allowed
			  WDOG_CS_CMD32EN_MASK  |           //enable 32bit write Watchdog refresh
              WDOG_CS_CLK(WDOG_TEST_CLK_SRC) |  //clock source is bus clock, to make the test fast
              WDOG_CS_TST(0x02);                //test watchdog timer low byte
    
		//for(u32TmpCnt = WDOG_TEST_MAXCNT; u32TmpCnt > 0 ; u32TmpCnt--){}       //wait for wdog reset, use u32TmpCnt as a timeout counter
	}
	else if(RCM->SRS & RCM_SRS_WDOG_MASK)       //if it's Watchdog reset, check whether it's in the middle of the WDOG test
	{
		if((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(0x02))       // if low byte test done,
		{
			WDOG->CNT = 0xD928C520;				//unlock watchdog registers
			WDOG->TOVAL = WDOG_TEST_TO_VALUE;   //watchdog timeout value for doing test
			WDOG->CS = WDOG_CS_EN_MASK |		//watchdog enable
					WDOG_CS_UPDATE_MASK |       //watchdog update allowed
					WDOG_CS_CMD32EN_MASK  |     //enable Watchdog 32bit refresh
					WDOG_CS_CLK(WDOG_TEST_CLK_SRC) |        //clock source is bus clock
					WDOG_CS_TST(0x03);		    //test watchdog timer high byte
			//for(u32TmpCnt = WDOG_TEST_MAXCNT; u32TmpCnt > 0 ; u32TmpCnt--){}	//wait for wdog reset, use u32TmpCnt as a timeout counter
		}
		else if((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(0x03))  //if high byte test done, set to normal user mode
		{

			WDOG->CNT = 0xD928C520;				//unlock watchdog registers
			WDOG->TOVAL = WDOG_NORMAL_TO_VALUE;	//Set watchdog time-out value for normal application
      
			//This is the Watchdog Configuration for normal application
			WDOG->CS = //WDOG_CS_EN_MASK |      //remove this line because need disable watchdog. Let user application to initialize/enable the WDOG
                WDOG_CS_UPDATE_MASK |       	//watchdog update allowed
				WDOG_CS_CMD32EN_MASK  |         //enable Watchdog refresh
                WDOG_CS_CLK(WDOG_NORMAL_CLK_SRC) |        //clock source is bus clock
                WDOG_CS_TST(0x01);          	//watchdog test completed, user mode
		}
		else if((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(0x01))
		{
			WDOG_Disable();						//if normal user mode and WDOG reset occurred, disable WDOG, user application should be noticed and re-enable WDOG
		}
		else                                    //if it's WDOG reset and WDOG isn't tested yet, do nothing.
		{
			WDOG_Disable();						//disable WDOG with TST field unchanged
		}
	}
	else
		WDOG_Disable();							//disable WDOG with TST field unchanged
  
	//Software continues run after this function, and WDOG will occur within short time if it's in
	//if(((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(2)) || ((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(3)))
	/*if((WDOG->CS & WDOG_CS_TST_MASK) != WDOG_CS_TST(1))
	{
		WDOG_TestMode(0);
	}*/
}

/**********************************************************************************************************
Function: WDOG_GetTestResult
          Call this function in user application to get the WDOG test result.
          This function will read the WDOG_CS_TST bit.
          WDOG_CS_TST = 00      WDOG is not tested, or is with problem;
          WDOG_CS_TST = 01      WDOG is tested and is in good state;
Return Value: 0 - WDOG is not tested or is with problem;
              1 - WDOG is tested and is without problem;
**********************************************************************************************************/
//void WDOG_GetTestResult(void)
//{
//    if((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(1))		// if in user software, WDOG_CS_TST is 0/2/3, means WDOG test not passed
////    	selfTestStatus |= SELFTEST_PASS_WDOG;				// if in user software, WDOG_CS_TST is 1, means WDOG test passed
//    	WdogSelfTestStatus = 1;
//    else
////    	selfTestStatus &= ~SELFTEST_PASS_WDOG;
//    	WdogSelfTestStatus = 0;
//}

uint8_t GetWdogSelfTestStatus(void)
{
    if((WDOG->CS & WDOG_CS_TST_MASK) == WDOG_CS_TST(1))		// if in user software, WDOG_CS_TST is 0/2/3, means WDOG test not passed
    {
//    	selfTestStatus |= SELFTEST_PASS_WDOG;				// if in user software, WDOG_CS_TST is 1, means WDOG test passed
    	WdogSelfTestStatus = 0;
    }
    else
    {
//    	selfTestStatus &= ~SELFTEST_PASS_WDOG;
    	WdogSelfTestStatus = 1;
    }

	return(WdogSelfTestStatus);
}

/**********************************************************************************************************
Function: WDOG_TrigReset
          Enable the watchdog with a short timeout value, and wait for watchdog reset to occur.
**********************************************************************************************************/
void WDOG_TrigReset(void)
{
	uint32_t tmp;
	WDOG_ConfigTimeout(0);
	WDOG_Enable();
	for (tmp = 0; tmp < 1000; tmp++);
}

void WDOG_EWM_IRQHandler(void)
{
	if(WDOG->CS & WDOG_CS_FLG_MASK)	//it's WDOG interrupt
	{
		//PTD->PTOR = 1;
		//printf("WDOG Interrupt!\r\n");
	}
	else							//it's EWM interrupt
	{

	}

}

