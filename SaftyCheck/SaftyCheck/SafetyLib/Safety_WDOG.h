#ifndef __WDOG_TEST_H__
#define __WDOG_TEST_H__

#include "cpu.h"

//counter value used for WDOG test (set to max value)
#define WDOG_TEST_TO_VALUE      0xFFFF
//this is a counter value used in WDOG test software to indicate a timeout event
#define WDOG_TEST_MAXCNT        40000

//Deisred WDOG Time-Out value for normal application
#define WDOG_NORMAL_TO_VALUE    0xF8FE

//Clock source for WDOG, might be from internal bus, LPO RC, OSC, Slow IRC.
#define WDOG_CLK_SRC_BUS   0x00
#define WDOG_CLK_SRC_LPO   0x01
#define WDOG_CLK_SRC_OSC   0x02
#define WDOG_CLK_SRC_SIRC  0x03

//clock source selection for WDOG test
#define WDOG_TEST_CLK_SRC       WDOG_CLK_SRC_SIRC
//clock source selection for normal application
#define WDOG_NORMAL_CLK_SRC     WDOG_CLK_SRC_SIRC

#define WDOG_TEST_RESULT_GOOD   1
#define WDOG_TEST_RESULT_BAD    0

#define WDOG_WAIT_COUNT		10000

//Write sequence for refreshing watchdog
#define WDOG_REFRESH()          {WDOG->CNT = 0xB480A602;}
//Write sequence for unlocking watchdog config register
#define WDOG_UNLOCK()           {WDOG->CNT = 0xD928C520;}

//this function will unlock WDOG registers and disable watchdog
void WDOG_Disable(void);

void WDOG_Enable(void);

void WDOG_ConfigTimeout(uint32_t nTimeoutMsec);


//This function should be placed in system_init() to conduct the Watchdog test after a POR reset.
void WDOG_PorTest(void);

//Get the test result of WDOG test.
void WDOG_GetTestResult(void);

void WDOG_TrigReset(void);

void WDOG_SDK_Test(void);

uint8_t GetWdogSelfTestStatus(void);

#endif
