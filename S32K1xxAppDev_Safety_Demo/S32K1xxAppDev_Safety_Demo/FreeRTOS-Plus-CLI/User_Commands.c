/*
 * User_Commands.c
 *
 *  Created on: Apr 27, 2019
 *      Author: nxa07657
 */

#include "User_Commands.h"

#include "App_Config.h"

#include "cpu.h"

#include "SafetyLib.h"

#define MAX_INPUT_LENGTH    50
#define MAX_OUTPUT_LENGTH   2048

#define HELP_STATS      "taskstats  Display all FreeRTOS's tasks status\r\n"
#define HELP_LED        "led        Control the on board RGB LED\r\n"
#define HELP_GET	    "get        get system status\r\n"
#define HELP_WDOG	    "wdog       configure watchdog timer in MCU\r\n"
#define HELP_MPU	    "mpu        Configure and test MPU\r\n"
#define HELP_CRC	    "crc        Test CRC module in MCU\r\n"
#define HELP_FAULT	    "fault      Read or clear fault information\r\n"
#define HELP_PMC	    "pmc        configure power monitor module in MCU\r\n"
#define HELP_CMU	    "cmu        configure clock monitor module in MCU\r\n"
#define HELP_STK	    "stk        stack overflow monitor\r\n"
#define HELP_NVIC       "nvic       interrupts error injection\r\n"
#define HELP_REGCHK     "regchk     registers integrity check\r\n"
#define HELP_SAFEBT     "safebt     checksum validation for safe boot\r\n"
#define HELP_SRAM       "sram       SRAM ECC error injection\r\n"
#define HELP_SCST       "scst       Cortex-M4 Structural Core self-test.\r\n"
#define HELP_FLS        "fls        Flash ECC handling and protection\r\n"

#define MAN_STATS   "Display all freerots's tasks status.\r\n"\
					"taskstats <option>:\r\n"\
					"[1]tasklist    Display tasks status\r\n"\
					"[2]runtime     Display tasks runtime information\r\n\r\n"

#define MAN_LED		"Control the RGB LED on EVB.\r\n"\
					"led <option>:\r\n"\
					"[1]r <0-255> set Red Led.\r\n"\
					"[2]g <0-255> set Green Led.\r\n"\
					"[3]b <0-255> set Blue Led.\r\n"\
					"[4]view      view RGB values.\r\n"\
					"[5]on        turn on all 3 Leds.\r\n"\
					"[6]off       turn off all 3 Leds.\r\n\r\n"

#define MAN_WDOG	"configure software watchdog in MCU\r\n"\
					"wdog (<option>)\r\n"\
					"[1]on <para>   turn WDOG on with timeout in mSec (default 100ms)\r\n"\
					"[2]off         turn WDOG off\r\n"\
					"[3]view        view WDOG configurations\r\n"\
					"[4]reset       generate a WDOG reset\r\n"\
					"[5]stop        stop WDOG refresh operation\r\n\r\n"

#define MAN_PMC "configure Power Monitor module in MCU\r\n"\
				"pmc (<option>)\r\n"\
				"[1]on <para>   Enable PMC Monitor\r\n" \
				"               0: Enable LVW interrupt\r\n" \
				"               1: Enable LVD interrupt\r\n" \
				"               2: Enable LVR reset\r\n" \
				"               3: Enable all low voltage detect\r\n"\
				"[2]off <para>  Disable PMC Monitor (0-LVW; 1-LVD; 2-LVR; 3-all\r\n"\
				"[3]view        view Power Monitor configurations\r\n\r\n"\

#define MAN_CMU "configure Clock Monitor module in MCU\r\n"\
				"cmu (<option>)\r\n"\
				"[1]on <p> <t>  Enable Clock Monitor Unit\r\n"\
				"               p=0: OSC CMU;  p=1: PLL CMU;  p=2: OSC & PLL CMU;\r\n"\
				"               t=0: int on error;  t=1: reset on error;\r\n"\
				"[2]off <p>     Disable Clock Monitor Unit\r\n"\
				"               p=0: OSC CMU;  p=1: PLL CMU;  p=2: OSC & PLL CMU;\r\n"\
				"[3]view        view Clock Monitor configurations\r\n\r\n"


#define MAN_CRC "configure CRC module in MCU\r\n"\
				"crc (<option>)\r\n"\
				"[1]sw16  <addr> <size>    Use CPU software to do CRC16.\r\n"\
				"[2]hw16  <addr> <size>    Use CRC module to do CRC16.\r\n"\
				"[3]dma16 <addr> <size>    Use CRC and DMA to do CRC16.\r\n"\
				"[4]sw32  <addr> <size>    Use CPU software to do CRC32.\r\n"\
				"[5]hw32  <addr> <size>    Use CRC module to do CRC32.\r\n"\
				"[6]dma32 <addr> <size>    Use CRC and DMA to do CRC32.\r\n"\
				"[7]set   <seed> <ctrl>    Setup CRC configuration.\r\n"\
				"[8]view                   view CRC configuration.\r\n\r\n"

#define MAN_MPU "configure MPU in MCU\r\n"\
				"mpu (<option>)\r\n"\
				"[1]on          turn on MPU.\r\n"\
				"[2]off         turn off MPU.\r\n"\
				"[3]errors      check MPU errors.\r\n"\
				"[4]view        view MPU Configurations.\r\n"\
				"[5]inj <error> inject MPU errors.\r\n\r\n"

#define MAN_FAULT 	"save or clear faults in EEPROM\r\n"\
					"fault (<option>)\r\n"\
					"[1]count      get faults count.\r\n"\
					"[2]read       print information of all recorded faults.\r\n"\
					"[3]clear      clear faults.\r\n"\
					"[4]rstsrc     read reset source.\r\n\r\n"

#define MAN_STK		"stack view and error injection\r\n"\
					"stk <option>\r\n"\
					"[1]view       print tasks stack address\r\n"\
					"[2]inj        inject stack overflow error\r\n\r\n"

#define MAN_NVIC	"NVIC error injection\r\n"\
					"nvic <option>\r\n"\
					"[1]inj1       inject unknown interrupt\r\n"\
					"[2]inj2       inject false interrupt\r\n\r\n"

#define MAN_REGCHK	"registers integrity check\r\n"\
					"regchk <option>\r\n"\
					"[1]view       view monitored registers CRC value\r\n"\
					"[2]inj        inject errors by modifying PORT register\r\n\r\n"

#define MAN_SAFEBT	"checksum validation for safe boot\r\n"\
					"safebt <option>\r\n"\
					"[1]view       view calculated and saved CRC values\r\n"\
					"[2]check      re-do the CRC of all specified memory blocks\r\n\r\n"

#define MAN_SRAM	"sram ECC error injection\r\n"\
					"sram <option>\r\n"\
					"[1]inj1       inject sram ECC error to SRAM_L\r\n"\
					"[2]inj2       inject sram ECC error to SRAM_U\r\n"\
					"[3]view       view sram ECC status\r\n\r\n"

#define MAN_SCST    "configure M4 core self test in MCU\r\n"\
					"scst (<option>)\r\n"\
					"[1]set <p1> <p2>    Set start and end test numbers (0~43)\r\n"\
					"[2]inj <p>          Inject fault to specified test (0~43)\r\n"\
					"[3]view             View test result\r\n"\
					"[4]on               Turn on SCST\r\n"\
					"[5]off              Turn off SCST\r\n"

#define MAN_FLS "configure flash protection region and test for protection mechanism\r\n"\
	                "fls (<option>)\r\n"\
				    "[1]injprot          inject PFLash protection error in region 31.\r\n"\
					"[2]injforce         inject PFlash force double bit ECC error.\r\n"\
					"[3]injovp           inject PFlash over-program ECC error.\r\n"\
					"[4]view             view flash status.\r\n"\
					"[5]erase            erase the last flash sector.\r\n"

/*buffer used for command console output information*/
char cOutputBuffer[ configCOMMAND_INT_MAX_OUTPUT_SIZE ];

static const char * const pcWelcomeMessage =
  "FreeRTOS command server.\r\nType Help to view a list of registered commands.\r\n";

#define TaskList_String \
"*****************************************************************\r\n \
X: running, B: blocking, R: ready, D: delete, S: suspend\r\n \
*****************************************************************\r\n \
task_name     task_state    priority   stack  task_num\r\n\
_____________________________________________\r\n"

#define TaskRunTime_String \
"task_name        run_count        usage % \r\n\
____________________________________________\r\n"

static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer,
                                          size_t xWriteBufferLen,
                                          const char *pcCommandString );
static CLI_Command_Definition_t xTaskStatsCommand =
{
	"taskstats",
	HELP_STATS,
	prvTaskStatsCommand,
	1
};

BaseType_t prvLED_Ctrl_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );
static const CLI_Command_Definition_t xLED_CtrlCommand =
{
    "led",
	HELP_LED,
    //"\r\n LED_Ctrl <LED> <Action>:\r\n Control the on board RGB <LED> according to <Action>\r\n <LED> can be RED/BLUE/GREEN\r\n <Action> can be ON/OFF \r\n",
    prvLED_Ctrl_Command,
    2
};

static BaseType_t prvWdog_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xWdogCommand =
{
    "wdog",
	HELP_WDOG,
	prvWdog_Command,
    2
};

static BaseType_t prvPmc_Command( char *pcWriteBuffer,
        size_t xWriteBufferLen,
        const char *pcCommandString );

static const CLI_Command_Definition_t xPMCMonitorCommand =
{
    "pmc",
    HELP_PMC,
    prvPmc_Command,
    2
};

static BaseType_t prvClkMonitor_Command( char *pcWriteBuffer,
        size_t xWriteBufferLen,
        const char *pcCommandString );

static const CLI_Command_Definition_t xClkMonitorCommand =
{
    "cmu",
    HELP_CMU,
    prvClkMonitor_Command,
    3
};


static BaseType_t prvMpu_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xMpuCommand =
{
    "mpu",
	HELP_MPU,
	prvMpu_Command,
    2
};

static BaseType_t prvFault_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xFaultCommand =
{
    "fault",
	HELP_FAULT,
	prvFault_Command,
    1
};

static BaseType_t prvCrc_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xCrcCommand =
{
    "crc",
	HELP_CRC,
	prvCrc_Command,
    3
};


static BaseType_t prvStk_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xStkCommand =
{
    "stk",
	HELP_STK,
	prvStk_Command,
    3
};


static BaseType_t prvNvic_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xNvicCommand =
{
    "nvic",
	HELP_NVIC,
	prvNvic_Command,
    2
};


static BaseType_t prvRegchk_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xRegchkCommand =
{
    "regchk",
	HELP_REGCHK,
	prvRegchk_Command,
    2
};


static BaseType_t prvSafebt_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xSafebtCommand =
{
    "safebt",
	HELP_SAFEBT,
	prvSafebt_Command,
    2
};


static BaseType_t prvSram_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xSramCommand =
{
    "sram",
	HELP_SRAM,
	prvSram_Command,
    2
};


static BaseType_t prvSCST_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xSCSTCommand =
{
    "scst",
	HELP_SCST,
	prvSCST_Command,
    3
};

static BaseType_t prvFls_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString );

static const CLI_Command_Definition_t xFlsprotCommand =
{
    "fls",
	HELP_FLS,
	prvFls_Command,
    1
};

uint8_t  testPgmData[8] = {0,1,2,3,4,5,6,7};
//flash_ssd_config_t pSSDConfig;

/*************************************************************************************
 * the task for UART command console
 * if you want o use command console, please create the task in AppTasks_Create()
 * Notice: ensure the console peripheral(LPUART) is initialized before it start to run
 **************************************************************************************/
void vCommandConsoleTask( void *pvParameters )
{
	uint8_t i = 0;
	BaseType_t xMoreDataToFollow;
	static uint32_t rxByteCnt;
	/* The input and output buffers are declared static to keep them off the stack. */
	static uint8_t pcOutputString[ MAX_OUTPUT_LENGTH ], pcInputString[ MAX_INPUT_LENGTH ];

    /* This code assumes the peripheral being used as the console has already
    been opened and configured, and is passed into the task as the task
    parameter.  Cast the task parameter to the correct type. */

	/*the BSP commands register, uncomment to remove it if needed*/
	FreeRTOS_CLIRegisterCommand( &xTaskStatsCommand );

	/*
	 * register the user commands here before using it
	 */
	FreeRTOS_CLIRegisterCommand( &xLED_CtrlCommand );
	FreeRTOS_CLIRegisterCommand( &xWdogCommand );
    FreeRTOS_CLIRegisterCommand( &xPMCMonitorCommand );
    FreeRTOS_CLIRegisterCommand( &xClkMonitorCommand );
	FreeRTOS_CLIRegisterCommand( &xMpuCommand );
	FreeRTOS_CLIRegisterCommand( &xFaultCommand );
	FreeRTOS_CLIRegisterCommand( &xCrcCommand );
	FreeRTOS_CLIRegisterCommand( &xStkCommand );
	FreeRTOS_CLIRegisterCommand( &xNvicCommand );
	FreeRTOS_CLIRegisterCommand( &xRegchkCommand );
	FreeRTOS_CLIRegisterCommand( &xSafebtCommand );
	FreeRTOS_CLIRegisterCommand( &xSramCommand );
	FreeRTOS_CLIRegisterCommand( &xSCSTCommand );
	FreeRTOS_CLIRegisterCommand( &xFlsprotCommand );

	/* send out the welcome message at the beginning as user prompt */
    LPUART_DRV_SendDataBlocking(INST_LPUART1, (const uint8_t *)pcWelcomeMessage, strlen( pcWelcomeMessage ), 100);

    rxByteCnt = 0;
    memset( pcInputString, 0x00, MAX_INPUT_LENGTH );
    for( ;; )
    {
        /* receive the command from the console terminal */
        LPUART_DRV_ReceiveDataBlocking(INST_LPUART1, (&pcInputString[0]) + rxByteCnt, MAX_INPUT_LENGTH - rxByteCnt, 20);
        for(i=0; i<MAX_INPUT_LENGTH-1; i++)
        	if(pcInputString[i] == '\0')
        		break;
        rxByteCnt = i;
        if(rxByteCnt > MAX_INPUT_LENGTH - 5)
        	rxByteCnt = 0;

        /*search the input string to find the command*/
        for(i=0; i<(MAX_INPUT_LENGTH-1); i++)
        {
        	if(pcInputString[i]=='\r'&&pcInputString[i+1]=='\n')
        	{
        		/*send back*/
        		LPUART_DRV_SendDataBlocking(INST_LPUART1,pcInputString, i+2, 10);

        		pcInputString[i] = '\0';/*end of the line*/

        		do
				{
        			pcOutputString[0] = '\0';/*end of the line in case no output*/

					/* Send the command string to the command interpreter.  Any
					output generated by the command interpreter will be placed in the
					pcOutputString buffer. */
					xMoreDataToFollow = FreeRTOS_CLIProcessCommand
								  (
									  (char *)pcInputString,   /* The command string.*/
									  (char *)pcOutputString,  /* The output buffer. */
									  MAX_OUTPUT_LENGTH/* The size of the output buffer. */
								  );

					/* Write the output generated by the command interpreter to the console. */
					if(strlen( (char *)pcOutputString )>0)
						LPUART_DRV_SendDataBlocking(INST_LPUART1, pcOutputString, strlen( (char *)pcOutputString ), 100);

				} while( xMoreDataToFollow != pdFALSE );

        		/*
        		 * reset the command receive string buffer
        		 */
        		memset( pcInputString, 0x00, MAX_INPUT_LENGTH );
        		rxByteCnt = 0;

        		break;
        	}
        }

    }
}

#if 0
BaseType_t prvLED_Ctrl_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString )
{
	const char *pcParameter1, *pcParameter2;
	BaseType_t xParameter1StringLength, xParameter2StringLength, xResult;

	 uint8_t Led, Action;

    /* Obtain the name of the RGB LED, and the length of its name, from the command string. */
    pcParameter1 = FreeRTOS_CLIGetParameter
                        ( pcCommandString,/* The command string itself. */
                          1,/* Return the first parameter. */
                          /* Store the parameter string length. */
                          &xParameter1StringLength
                        );

    /* Obtain the name of the action, and the length of its name. */
    pcParameter2 = FreeRTOS_CLIGetParameter( pcCommandString,
                                             2,
                                             &xParameter2StringLength );
    xResult = pdPASS;

    /* find the LED */
    if( strncmp( pcParameter1, "RED", xParameter1StringLength ) == 0 )
    	Led = RED;
    else if( strncmp( pcParameter1, "BLUE", xParameter1StringLength ) == 0 )
        Led = BLUE;
    else if( strncmp( pcParameter1, "GREEN", xParameter1StringLength ) == 0 )
        Led = GREEN;
    else
    	xResult = pdFAIL;

    /* set the output state*/
    if( strncmp( pcParameter2, "ON", xParameter2StringLength ) == 0 )
    	Action = 0;
    else if( strncmp( pcParameter2, "OFF", xParameter2StringLength ) == 0 )
    	Action = 1;
    else
    	xResult = pdFAIL;

    if( xResult == pdPASS )
    {
    	/* Perform the LED GPIO control operation itself. */
    	RGB_LED_Ctrl( Led, Action);
        /* The command was executed successful. There is nothing to output. */
        *pcWriteBuffer = NULL;
    }
    else
    {
        /* The command was not executed due to errors  Inform the users. */
        snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
    }

    /* There is only a single line of output produced in all cases.  pdFALSE is
    returned because there is no more output to be generated. */
    return pdFALSE;
}
#endif
BaseType_t prvLED_Ctrl_Command( char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString )
{
	const char *pcParameter1, *pcParameter2;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	int tmp;
	xResult = pdFALSE;

    /* Obtain the name of the RGB LED, and the length of its name, from the command string. */
	pcParameter1 = FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)
	{
		strcpy(pcWriteBuffer, MAN_LED);
		xResult = pdFALSE;
	}
	else
	{
		/* Obtain the name of the action, and the length of its name. */
		pcParameter2 = FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)
		{
			if( ( strncmp( pcParameter1, "view", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "4", xP1StrLen ) == 0))
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "R: %d; G: %d; B: %d.\r\n", (int)ledRGBValue[0]/2, (int)ledRGBValue[1]/2, (int)ledRGBValue[2]/2);
			}

			else if( ( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "5", xP1StrLen ) == 0) )
			{
				ledRGBValue[0] = 255 * 2;
				ledRGBValue[1] = 255 * 2;
				ledRGBValue[2] = 255 * 2;
				snprintf(pcWriteBuffer, xWriteBufferLen, "R: %d; G: %d; B: %d.\r\n", (int)ledRGBValue[0]/2, (int)ledRGBValue[1]/2, (int)ledRGBValue[2]/2);
				INT_SYS_DisableIRQ(LPTMR0_IRQn);
			}
			else if( ( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "6", xP1StrLen ) == 0) )
			{
				ledRGBValue[0] = 0;
				ledRGBValue[1] = 0;
				ledRGBValue[2] = 0;
				snprintf(pcWriteBuffer, xWriteBufferLen, "R: %d; G: %d; B: %d.\r\n", (int)ledRGBValue[0]/2, (int)ledRGBValue[1]/2, (int)ledRGBValue[2]/2);
				//Add following LPTMR init code for test purpose: generate 100uS interrupt to see whether SCST can run normally
				LPTMR0->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TFC_MASK;
				LPTMR0->CMR = 400;
				LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
				INT_SYS_EnableIRQ(LPTMR0_IRQn);
				cntScstCompleted = 0;
				cntScstInterrupted = 0;
			}
			else
			{
				/* The command was not executed due to errors  Inform the users. */
				snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
			}
		}
		else	//two parameters
		{
			sscanf(pcParameter2, "%d", &tmp);
			if(tmp < 0)
				tmp = 0;
			if(tmp > 255)
				tmp = 255;
			if( ( strncmp( pcParameter1, "r", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0) )
			{
				ledRGBValue[0] = tmp * 2;
				snprintf(pcWriteBuffer, xWriteBufferLen, "R: %d; G: %d; B: %d.\r\n", (int)ledRGBValue[0]/2, (int)ledRGBValue[1]/2, (int)ledRGBValue[2]/2);
			}
			else if( ( strncmp( pcParameter1, "g", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0) )
			{
				ledRGBValue[1] = tmp * 2;
				snprintf(pcWriteBuffer, xWriteBufferLen, "R: %d; G: %d; B: %d.\r\n", (int)ledRGBValue[0]/2, (int)ledRGBValue[1]/2, (int)ledRGBValue[2]/2);
			}
			else if( ( strncmp( pcParameter1, "b", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "3", xP1StrLen ) == 0) )
			{
				ledRGBValue[2] = tmp * 2;
				snprintf(pcWriteBuffer, xWriteBufferLen, "R: %d; G: %d; B: %d.\r\n", (int)ledRGBValue[0]/2, (int)ledRGBValue[1]/2, (int)ledRGBValue[2]/2);
			}
			else
			{
				snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
			}
		}
	}
    return xResult;
}

/* This command is used to list the FreeRTOS task status, it's useful for debug */
static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer,
                                          size_t xWriteBufferLen,
                                          const char *pcCommandString )
{
	char *pcParameter1;
	BaseType_t xParameter1StringLength;
	BaseType_t xResult = pdPASS;

	/* Obtain the command option, and its length, from the command string. */
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter
						( pcCommandString,/* The command string itself. */
						  1,/* Return the first parameter. */
						  /* Store the parameter string length. */
						  &xParameter1StringLength
						);

	if(pcParameter1 == NULL)
	{
		strcpy(pcWriteBuffer, MAN_STATS);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		/* Parse the command parameters */
		if( ( strncmp( pcParameter1, "tasklist", xParameter1StringLength ) == 0 ) || (strncmp( pcParameter1, "1", xParameter1StringLength ) == 0))
		{
			/* pcWriteBuffer is used directly as the vTaskList() parameter, so the table
			generated by executing vTaskList() is written directly into the output
			buffer. */
			vTaskList( pcWriteBuffer );
			/*send out a info prompt*/
			LPUART_DRV_SendDataBlocking(INST_LPUART1, (const uint8_t *)TaskList_String, strlen( TaskList_String ), 50);
		}

		else if( ( strncmp( pcParameter1, "runtime", xParameter1StringLength ) == 0 ) || (strncmp( pcParameter1, "2", xParameter1StringLength ) == 0) )
		{
			/* pcWriteBuffer is used directly as the vTaskGetRunTimeStats() parameter, so the table
			generated by executing vTaskGetRunTimeStats() is written directly into the output
			buffer. */
			vTaskGetRunTimeStats(pcWriteBuffer);
			/*send out a info prompt*/
			LPUART_DRV_SendDataBlocking(INST_LPUART1, (const uint8_t *)TaskRunTime_String, strlen(TaskRunTime_String), 50);
		}
		else
		{
			/* The command was not executed due to errors  Inform the users. */
			snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
		}
	}

    /* The entire table was written directly to the output buffer.  Execution
    of this command is complete, so return pdFALSE. */
    return xResult;
}


/********************************************************************************************
 *
 ********************************************************************************************/
BaseType_t prvWdog_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	int Num;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)		//if there is no parameter
	{
		strcpy(pcWriteBuffer, MAN_WDOG);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	//if there is only one parameter
		{
			if ( ( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 ) )
			{
				WDOG_DRV_Init(INST_WATCHDOG1, &watchdog1_Config0);
				tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "WDOG_CS_EN = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_EN_MASK)>>WDOG_CS_EN_SHIFT);
				snprintf(pcWriteBuffer+tmp, xWriteBufferLen, "WDOG_TOVAL = %u.\r\n", (unsigned int)(WDOG->TOVAL));
			}
			else if( ( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 ) )
			{
				WDOG_Disable();
				tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "WDOG_CS_EN = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_EN_MASK)>>WDOG_CS_EN_SHIFT);
				snprintf(pcWriteBuffer+tmp, xWriteBufferLen, "WDOG_TOVAL = %u.\r\n", (unsigned int)(WDOG->TOVAL));
			}
			else if( ( strncmp( pcParameter1, "view", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "3", xP1StrLen ) == 0 ) )
			{
				tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "WDOG_CS_EN = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_EN_MASK)>>WDOG_CS_EN_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "WDOG_CS_UPDATE = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_UPDATE_MASK)>>WDOG_CS_UPDATE_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "WDOG_CS_WIN = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_WIN_MASK)>>WDOG_CS_WIN_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "WDOG_CS_CLK = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_CLK_MASK)>>WDOG_CS_CLK_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "WDOG_CS_INT = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_INT_MASK)>>WDOG_CS_INT_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "WDOG_CS_TST = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_TST_MASK)>>WDOG_CS_TST_SHIFT);
				snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "WDOG_TOVAL = %u.\r\n", (unsigned int)(WDOG->TOVAL));
			}
			else if( ( strncmp( pcParameter1, "reset", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "4", xP1StrLen ) == 0 ) )
			{
				WDOG_TrigReset();
			}
			else if( ( strncmp( pcParameter1, "stop", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "5", xP1StrLen ) == 0 ) )
			{
				wdogRefreshDisable = 1;
			}
			else
			{
				snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
			}
		}
		else						//if there are two parameters
		{
			sscanf(pcParameter2, "%d", &Num);
			if ( ( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 ) )
			{
				WDOG_ConfigTimeout(Num); //configure SWT with timeout value
				tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "WDOG_CS_EN = %u.\r\n", (unsigned int)(WDOG->CS & WDOG_CS_EN_MASK)>>WDOG_CS_EN_SHIFT);	//print WDOG configurations
				snprintf(pcWriteBuffer+tmp, xWriteBufferLen, "WDOG_TOVAL = %u.\r\n", (unsigned int)(WDOG->TOVAL));
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
	}
	return xResult;

}

/********************************************************************************************
 * This function is for processing command "pmc" power management controller
 ********************************************************************************************/
extern uint32_t u32Vddmv;
BaseType_t prvPmc_Command( char *pcWriteBuffer,
                                  size_t xWriteBufferLen,
                                  const char *pcCommandString )
{
    char *pcParameter1, *pcParameter2;
    int32_t tmp;
    BaseType_t xP1StrLen, xP2StrLen, xResult;
    xResult = pdPASS;

    tmp = 0;
    pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
    if(pcParameter1 == NULL)		//if there is no parameter
    {
        strcpy(pcWriteBuffer, MAN_PMC);
        xResult = pdFALSE;
    }
    else
    {
        xResult = pdFALSE;
        pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
        if(pcParameter2 == NULL)	//if there is only one parameter
        {
            if( ( strncmp( pcParameter1, "view", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "3", xP1StrLen ) == 0 ) )
            {
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVWIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC2 & PMC_LVDSC2_LVWIE_MASK)>>PMC_LVDSC2_LVWIE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDIE_MASK)>>PMC_LVDSC1_LVDIE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PMCMonitor_LVDRE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDRE_MASK)>>PMC_LVDSC1_LVDRE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "VDD supply = %umV.\r\n", (unsigned int)u32Vddmv);
            }
            else
            {
                snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
            }
        }
        else //if there are two parameters
        {
            if ( (( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 )) &&
                    (strncmp( pcParameter2, "0", xP2StrLen ) == 0 ))
            {
                PMC_LVWIE_Config(STD_ON);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVWIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC2 & PMC_LVDSC2_LVWIE_MASK)>>PMC_LVDSC2_LVWIE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 )) &&
                     (strncmp( pcParameter2, "1", xP2StrLen ) == 0 ))
            {
                PMC_LVDIE_Config(STD_ON);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDIE_MASK)>>PMC_LVDSC1_LVDIE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 )) &&
                     (strncmp( pcParameter2, "2", xP2StrLen ) == 0 ))
            {
                PMC_LVDRE_Config(STD_ON);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDRE_MASK)>>PMC_LVDSC1_LVDRE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 )) &&
                     (strncmp( pcParameter2, "3", xP2StrLen ) == 0 ))
            {
                PMC_LVWIE_Config(STD_ON);
                PMC_LVDIE_Config(STD_ON);
                PMC_LVDRE_Config(STD_ON);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVWIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC2 & PMC_LVDSC2_LVWIE_MASK)>>PMC_LVDSC2_LVWIE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDIE_MASK)>>PMC_LVDSC1_LVDIE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PMCMonitor_LVDRE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDRE_MASK)>>PMC_LVDSC1_LVDRE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                     (strncmp( pcParameter2, "0", xP2StrLen ) == 0 ))
            {
                PMC_LVWIE_Config(STD_OFF);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVWIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC2 & PMC_LVDSC2_LVWIE_MASK)>>PMC_LVDSC2_LVWIE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                     (strncmp( pcParameter2, "1", xP2StrLen ) == 0 ))
            {
                PMC_LVDIE_Config(STD_OFF);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDIE_MASK)>>PMC_LVDSC1_LVDIE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                     (strncmp( pcParameter2, "2", xP2StrLen ) == 0 ))
            {
                PMC_LVDRE_Config(STD_OFF);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDRE_MASK)>>PMC_LVDSC1_LVDRE_SHIFT);
            }
            else if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                     (strncmp( pcParameter2, "3", xP2StrLen ) == 0 ))
            {
                PMC_LVWIE_Config(STD_OFF);
                PMC_LVDIE_Config(STD_OFF);
                PMC_LVDRE_Config(STD_OFF);
                /* print Por Monitor configurations */
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "PMCMonitor_LVWIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC2 & PMC_LVDSC2_LVWIE_MASK)>>PMC_LVDSC2_LVWIE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PMCMonitor_LVDIE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDIE_MASK)>>PMC_LVDSC1_LVDIE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PMCMonitor_LVDRE_EN = %u.\r\n", (unsigned int)(PMC->LVDSC1 & PMC_LVDSC1_LVDRE_MASK)>>PMC_LVDSC1_LVDRE_SHIFT);
            }
            else
            {
                snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
            }
        }
    }

    return xResult;
}



/********************************************************************************************
 * This function is for processing command "cmu" clock monitor unit
 ********************************************************************************************/
BaseType_t prvClkMonitor_Command( char *pcWriteBuffer,
                                  size_t xWriteBufferLen,
                                  const char *pcCommandString )
{
    char *pcParameter1, *pcParameter2, *pcParameter3;
    int32_t tmp;
    BaseType_t xP1StrLen, xP2StrLen, xP3StrLen, xResult;
    xResult = pdPASS;

    tmp = 0;
    pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
    if(pcParameter1 == NULL)        //if there is no parameter
    {
        strcpy(pcWriteBuffer, MAN_CMU);
        xResult = pdFALSE;
    }
    else
    {
        xResult = pdFALSE;
        pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
        if(pcParameter2 == NULL)    //if there is only one parameter
        {
            if( ( strncmp( pcParameter1, "view", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "3", xP1StrLen ) == 0 ) )
            {
                tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_OSC_IERE_EN = %u.(0 IE_EN; 1 RE_EN)\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCMRE_MASK)>>SCG_SOSCCSR_SOSCCMRE_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_IERE_EN = %u.(0 IE_EN; 1 RE_ENh)\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCMRE_MASK)>>SCG_SPLLCSR_SPLLCMRE_SHIFT);
            }
            else
            {
                snprintf( pcWriteBuffer, xWriteBufferLen, "parameters error\r\n\r\n" );
            }
        }
        else //if there are two parameters
        {
            xResult = pdFALSE;
            pcParameter3 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 3, &xP3StrLen);
            if(pcParameter3 == NULL)
            {
                if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                     (strncmp( pcParameter2, "0", xP2StrLen ) == 0 ))
                {
                    CMU_OSC_Config(STD_OFF);
                    /* print clock Monitor configurations */
                    tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                }
                else if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                         (strncmp( pcParameter2, "1", xP2StrLen ) == 0 ))
                {
                    CMU_PLL_Config(STD_OFF);
                    /* print clock Monitor configurations */
                    tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                }
                else if( (( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 )) &&\
                         (strncmp( pcParameter2, "2", xP2StrLen ) == 0 ))
                {
                    CMU_OSC_Config(STD_OFF);
                    CMU_PLL_Config(STD_OFF);
                    /* print clock Monitor configurations */
                    tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                    tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                }
                else
                {
                    snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
                }
            }
            else
            {
                if( ( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "1", xP1StrLen ) == 0 ) )
                {
                    if(strncmp( pcParameter2, "0", xP2StrLen ) == 0 && strncmp( pcParameter3, "0", xP3StrLen ) == 0)
                    {
                        CMU_OSC_Config(STD_ON);
                        CMU_OSC_IERE_Config(CMU_Interrupt_On_Error);
                        /* print Clock Monitor configurations */
                        tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_OSC_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCMRE_MASK)>>SCG_SOSCCSR_SOSCCMRE_SHIFT);
                    }
                    else if( strncmp( pcParameter2, "0", xP2StrLen ) == 0 && strncmp( pcParameter3, "1", xP3StrLen ) == 0)
                    {
                        CMU_OSC_Config(STD_ON);
                        CMU_OSC_IERE_Config(CMU_Reset_On_Error);
                        /* print Por Monitor configurations */
                        tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_OSC_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCMRE_MASK)>>SCG_SOSCCSR_SOSCCMRE_SHIFT);
                    }
                    else if( strncmp( pcParameter2, "1", xP2StrLen ) == 0 && strncmp( pcParameter3, "0", xP3StrLen ) == 0)
                    {
                        CMU_PLL_Config(STD_ON);
                        CMU_PLL_IERE_Config(CMU_Interrupt_On_Error);
                        /* print Por Monitor configurations */
                        tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCMRE_MASK)>>SCG_SPLLCSR_SPLLCMRE_SHIFT);
                    }
                    else if( strncmp( pcParameter2, "1", xP2StrLen ) == 0 && strncmp( pcParameter3, "1", xP3StrLen ) == 0)
                    {
                        CMU_PLL_Config(STD_ON);
                        CMU_PLL_IERE_Config(CMU_Reset_On_Error);
                        /* print Por Monitor configurations */
                        tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCMRE_MASK)>>SCG_SPLLCSR_SPLLCMRE_SHIFT);
                    }
                    else if( strncmp( pcParameter2, "2", xP2StrLen ) == 0 && strncmp( pcParameter3, "0", xP3StrLen ) == 0)
                    {
                        CMU_OSC_Config(STD_ON);
                        CMU_PLL_Config(STD_ON);
                        CMU_OSC_IERE_Config(CMU_Interrupt_On_Error);
                        CMU_PLL_IERE_Config(CMU_Interrupt_On_Error);
                        /* print Por Monitor configurations */
                        tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_OSC_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCMRE_MASK)>>SCG_SOSCCSR_SOSCCMRE_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCMRE_MASK)>>SCG_SPLLCSR_SPLLCMRE_SHIFT);
                    }
                    else if( strncmp( pcParameter2, "2", xP2StrLen ) == 0 && strncmp( pcParameter3, "1", xP3StrLen ) == 0)
                    {
                        CMU_OSC_Config(STD_ON);
                        CMU_PLL_Config(STD_ON);
                        CMU_OSC_IERE_Config(CMU_Reset_On_Error);
                        CMU_PLL_IERE_Config(CMU_Reset_On_Error);
                        /* print Por Monitor configurations */
                        /* print Por Monitor configurations */
                        tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "CLKMonitor_OSC_EN = %u.\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCM_MASK) >> SCG_SOSCCSR_SOSCCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_OSC_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCCMRE_MASK)>>SCG_SOSCCSR_SOSCCMRE_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_EN = %u.\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCM_MASK) >> SCG_SPLLCSR_SPLLCM_SHIFT);
                        tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CLKMonitor_PLL_IERE_EN = %u (0 IE_EN; 1 RE_EN).\r\n", (unsigned int)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLCMRE_MASK)>>SCG_SPLLCSR_SPLLCMRE_SHIFT);
                    }
                    else
                    {
                        snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
                    }
                }
                else
                {
                    snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
                }
            }
        }
    }

    return xResult;
}


/********************************************************************************************
 * This function is for processing command "mpu" memory protection unit
 ********************************************************************************************/
BaseType_t prvMpu_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	int32_t i=0;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)
	{
		strcpy(pcWriteBuffer, MAN_MPU);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)
		{
			if ((strncmp(pcParameter1, "on", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{
				MPU_GlobalEnable();//MPU_Config(); //enable MPU
				snprintf(pcWriteBuffer, xWriteBufferLen, "MPU enabled. MPU.CESR = 0x%x\r\n", (unsigned int)(MPU->CESR));
			}
			else if ((strncmp(pcParameter1, "off", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{
				MPU_GlobalDisable();
				snprintf(pcWriteBuffer, xWriteBufferLen, "MPU disabled. MPU.CESR = 0x%x\r\n", (unsigned int)(MPU->CESR));
			}
			else if ((strncmp(pcParameter1, "errors", xP1StrLen) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0))
			{
				tmp = 0;
				for(i = 0; i < 4; i++)
				{
					if(MPU->CESR & (MPU_CESR_SPERR0_MASK >> i))
					{
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Slave Port%d: Error Addr 0x%x, Details 0x%x.\r\n", (unsigned int)i, (unsigned int)(MPU->EAR_EDR[i].EAR), (unsigned int)(MPU->EAR_EDR[i].EDR));
					}
				}
				if((MPU->CESR & (MPU_CESR_SPERR0_MASK|MPU_CESR_SPERR1_MASK|MPU_CESR_SPERR2_MASK|MPU_CESR_SPERR3_MASK)) == 0)
				{
					snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "No MPU Errors.\r\n");
				}
			}
			else if ((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "4", xP1StrLen) == 0))
			{
				for(i = 0; i < 8; i++)
				{
					if(MPU->RGD[i].WORD3 & MPU_RGD_WORD3_VLD_MASK)	//if this region is valid
					{
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Region %d: Start 0x%08X, End 0x%08X, Access 0x%08X;\r\n", (unsigned int)i, (unsigned int)MPU->RGD[i].WORD0, (unsigned int)MPU->RGD[i].WORD1, (unsigned int)MPU->RGD[i].WORD2);
					}
				}
			}
			else if ((strncmp(pcParameter1, "inj", xP1StrLen) == 0)||(strncmp(pcParameter1, "5", xP1StrLen) == 0))
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Inject MPU Errors, write to Flash address 0x0100.\r\n");
				//MPU_InjErr(1);
				*(uint32_t *)(0x0100) = 0x0000;		//write to flash address 0x0100, this is a read-only area in MPU region settings.
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else	/*parameter2 is available*/
		{
			if ((strncmp(pcParameter1, "inj", xP1StrLen) == 0)||(strncmp(pcParameter1, "5", xP1StrLen) == 0))
			{
				if((strncmp(pcParameter2, "1", xP2StrLen) == 0))
				{
					tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Inject MPU Errors, write to Flash address 0x0100.\r\n");
					//MPU_InjErr(1);
					*(uint32_t *)(0x0100) = 0x0000;		//write to flash address 0x0100, this is a read-only area in MPU region settings.
					if(mpuErrorInfo.accessType == MPU_ERR_TYPE_READ)
						snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Error Addr 0x%08X, Master %d, Read Error.\r\n", (unsigned int)mpuErrorInfo.addr, (unsigned int)mpuErrorInfo.master);
					else
						snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Error Addr 0x%08X, Master %d, Write Error.\r\n", (unsigned int)mpuErrorInfo.addr, (unsigned int)mpuErrorInfo.master);
				}
				else if((strncmp(pcParameter2, "2", xP2StrLen) == 0))
				{
					tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Inject MPU Errors, read reserved address 0x00080000.\r\n");
					//MPU_InjErr(2);
					i += *(int32_t *)(0x00080000);		//read address 0x80000, it is a reserved area, no write or read access allowed.
					if(mpuErrorInfo.accessType == MPU_ERR_TYPE_READ)
						snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Error Addr 0x%08X, Master %d, Read Error.\r\n", (unsigned int)mpuErrorInfo.addr, (unsigned int)mpuErrorInfo.master);
					else
						snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Error Addr 0x%08X, Master %d, Write Error.\r\n", (unsigned int)mpuErrorInfo.addr, (unsigned int)mpuErrorInfo.master);
				}
				else
					snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
			else
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
	}
	return xResult;
}

/********************************************************************************************
 * This function is for processing command "fault" faults information read/clear
 ********************************************************************************************/
BaseType_t prvFault_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2, *pcParameter3;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xP3StrLen, xResult;
	int32_t i;
	uint32_t tmpcnt;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)
	{
		strcpy(pcWriteBuffer, MAN_FAULT);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)
		{
			tmpcnt = Safety_FaultManage_GetFaultCount();
			if ((strncmp(pcParameter1, "count", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{	//print fault count
				snprintf(pcWriteBuffer, xWriteBufferLen, "Number of fault: %d; Fault Bitmap: 0x%08X.\r\n", (int)tmpcnt, (unsigned int)Safety_FaultManage_GetFaultBitmap());
			}
			else if ((strncmp(pcParameter1, "read", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{	//print fault information
				if(tmpcnt > 0)
				{
					if(tmpcnt > 15)	//pcWriteBuffer is not big enough to hold more fault information for UART output
						i = tmpcnt - 15;
					else
						i = 0;
					for(; i < tmpcnt; i++)
					{
						//cmd_printf("Fault %d is %d.\r\n", (unsigned int)(i+1), (unsigned int)(*(uint32_t *)(FAULT_LIST_START_ADDR + i*16)), );
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Fault %d is %d %s, PC addr 0x%08X, Data addr 0x%08X.\r\n", (unsigned int)(i+1), (int)faultList[i].faultType, faultSourceString[(uint32_t)(faultList[i].faultType)], (int)faultList[i].faultPcAddr, (int)faultList[i].faultDataAddr);
					}
				}
				else
				{
					snprintf(pcWriteBuffer, xWriteBufferLen, "No Fault recorded.\r\n");
				}
			}
			else if ((strncmp(pcParameter1, "clear", xP1StrLen) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0))
			{
				Safety_FaultManage_ClearAllFault();
				snprintf(pcWriteBuffer, xWriteBufferLen, "Faults are cleared.\r\n");
			}
			else if ((strncmp(pcParameter1, "rstsrc", xP1StrLen) == 0)||(strncmp(pcParameter1, "4", xP1StrLen) == 0))
			{
				i = (uint32_t)RCM_ReadResetSource();
				snprintf(pcWriteBuffer, xWriteBufferLen, "Reset source is %s.\r\n", resetSourceString[i]);
			}
			else
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
		else
		{
			pcParameter3 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 3, &xP3StrLen);
			if(pcParameter3 == NULL)	/* if parameter number is 2 */
			{
				sscanf((char *)pcParameter2, "%d", (int *)&tmpcnt);	//tmpcnt is the faultType to be cleared
				if ((strncmp(pcParameter1, "clear", xP1StrLen) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0))
				{
					Safety_FaultManage_ClearSingleFault(tmpcnt);
					snprintf(pcWriteBuffer, xWriteBufferLen, "Fault type %d is cleared; Fault Bitmap: 0x%08X.\r\n", (int)tmpcnt, (unsigned int)Safety_FaultManage_GetFaultBitmap());
				}
				else
					snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
			else						/* if parameter number is 3 */
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
	}
	return xResult;
}

/********************************************************************************************
 * This function is for processing command "crc" test CRC calculation
 ********************************************************************************************/
extern mutex_t mutexCrc;
BaseType_t prvCrc_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2, *pcParameter3;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xP3StrLen, xResult;
	uint32_t u32TmpCrcRslt;
	unsigned int pSrcAddr;
	unsigned int u32Size;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_CRC);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "8", xP1StrLen) == 0))
			{
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "CRC_GPOLY = 0x%x.\r\n", (unsigned int)(CRC->GPOLY));	//print CRC polynomial value
				if(CRC->CTRL & CRC_CTRL_TCRC_MASK)
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Data width is 32bit.\r\n");
				else
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Data width is 16bit.\r\n");
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC_CTRL_TOT = %u.\r\n", (unsigned int)(CRC->CTRL & CRC_CTRL_TOT_MASK)>>CRC_CTRL_TOT_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC_CTRL_TOTR = %u.\r\n", (unsigned int)(CRC->CTRL & CRC_CTRL_TOTR_MASK)>>CRC_CTRL_TOTR_SHIFT);
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			pcParameter3 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 3, &xP3StrLen);
			if(pcParameter3 == NULL)	/* if parameter number is 2 */
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
			else						/* if parameter number is 3 */
			{
				sscanf((char *)pcParameter2, "%x", &pSrcAddr);	//pSrcAddr = strtol(argv[2], NULL, 16);
				sscanf((char *)pcParameter3, "%x", &u32Size);	//u32Size = strtol(argv[3], NULL, 16);
				OSIF_MutexLock(&mutexCrc, 50);
				if ((strncmp(pcParameter1, "sw16", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
				{
					u32TmpCrcRslt = CRC_Crc16SoftSlow((uint8_t *)pSrcAddr, u32Size, 0xFFFF);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC16 polynomial = 0x%x.\r\n", (unsigned int)(MY_CRC16_POLY));
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC16 result = 0x%x.\r\n", (unsigned int)(u32TmpCrcRslt));
				}
				else if ((strncmp(pcParameter1, "hw16", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0)) {
					u32TmpCrcRslt = CRC_Crc16Hw((uint8_t *)pSrcAddr, u32Size, &crc1_InitConfig0);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC16 polynomial = 0x%x, seed = 0x%x.\r\n", (unsigned int)(CRC->GPOLY), (unsigned int)crc1_InitConfig0.seed);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC16 result = 0x%x.\r\n", (unsigned int)(u32TmpCrcRslt));
				}
				else if ((strncmp(pcParameter1, "dma16", xP1StrLen) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0)) {
					CRC_Crc16HwDma((uint8_t *)pSrcAddr, u32Size, &crc1_InitConfig0, EDMA_CHN1_NUMBER);
					while(u32CrcDmaRslt == 0);
					u32TmpCrcRslt = u32CrcDmaRslt;
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC16 polynomial = 0x%x, seed = 0x%x.\r\n", (unsigned int)(CRC->GPOLY), (unsigned int)crc1_InitConfig0.seed);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC16 result = 0x%x.\r\n", (unsigned int)(u32TmpCrcRslt));
				}
				else if ((strncmp(pcParameter1, "sw32", xP1StrLen) == 0)||(strncmp(pcParameter1, "4", xP1StrLen) == 0)) {
					u32TmpCrcRslt = CRC_Crc32SoftSlow((uint8_t *)pSrcAddr, u32Size, 0xFFFFFFFF);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC32 polynomial = 0x%x.\r\n", (unsigned int)(MY_CRC32_POLY));
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC32 result = 0x%x.\r\n", (unsigned int)(u32TmpCrcRslt));
				}
				else if ((strncmp(pcParameter1, "hw32", xP1StrLen) == 0)||(strncmp(pcParameter1, "5", xP1StrLen) == 0)) {
					u32TmpCrcRslt = CRC_Crc32Hw((uint8_t *)pSrcAddr, u32Size, &crc1_InitConfig2);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC32 polynomial = 0x%x, seed = 0x%x.\r\n", (unsigned int)(CRC->GPOLY), (unsigned int)crc1_InitConfig1.seed);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC32 result = 0x%x.\r\n", (unsigned int)(u32TmpCrcRslt));
				}
				else if ((strncmp(pcParameter1, "dma32", xP1StrLen) == 0)||(strncmp(pcParameter1, "6", xP1StrLen) == 0)) {
					CRC_Crc32HwDma((uint8_t *)pSrcAddr, u32Size, &crc1_InitConfig2, EDMA_CHN1_NUMBER);
					while(u32CrcDmaRslt == 0);
					u32TmpCrcRslt = u32CrcDmaRslt;
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC32 polynomial = 0x%x, seed = 0x%x.\r\n", (unsigned int)(CRC->GPOLY), (unsigned int)crc1_InitConfig1.seed);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC32 result = 0x%x.\r\n", (unsigned int)(u32TmpCrcRslt));
				}
				else if((strncmp(pcParameter1, "set", xP1StrLen) == 0)||(strncmp(pcParameter1, "6", xP1StrLen) == 0))
				{
					CRC->CTRL |= CRC_CTRL_WAS_MASK;	//enable seed write
					CRC->DATAu.DATA = pSrcAddr;	//pSrcAddr is the seed value;
					CRC->CTRL  = u32Size;	//u32Size  is the CTRL reg value;
					CRC->CTRL &= ~CRC_CTRL_WAS_MASK;
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Configure CRC DATA and CTRL registers.\r\n");
					if(CRC->CTRL & CRC_CTRL_TCRC_MASK)
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Data width is 32bit.\r\n");
					else
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Data width is 16bit.\r\n");
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC_CTRL_TOT = %u.\r\n", (unsigned int)(CRC->CTRL & CRC_CTRL_TOT_MASK)>>CRC_CTRL_TOT_SHIFT);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC_CTRL_TOTR = %u.\r\n", (unsigned int)(CRC->CTRL & CRC_CTRL_TOTR_MASK)>>CRC_CTRL_TOTR_SHIFT);
				}
				else
				{
					snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
				}
				OSIF_MutexUnlock(&mutexCrc);
			}
		}
	}

	return xResult;
}


/********************************************************************************************
 * This function is for processing command "stk" stack monitor: inject stack overflow error
 ********************************************************************************************/
extern TaskHandle_t xTaskLed;
extern TaskHandle_t xTaskCmd;
extern TaskHandle_t xTaskSafety;
BaseType_t prvStk_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	unsigned int nStackStart, nStackEnd;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_STK);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{
				nStackStart = *(uint32_t *)(((uint32_t)xTaskLed) + 0x30);
				nStackEnd   = *(uint32_t *)(((uint32_t)xTaskLed) + 0x00);
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "TaskLed stack from 0x%08x to 0x%08x.\r\n", nStackStart, nStackEnd);
				nStackStart = *(uint32_t *)(((uint32_t)xTaskCmd) + 0x30);
				nStackEnd   = *(uint32_t *)(((uint32_t)xTaskCmd) + 0x00);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "TaskCmd stack from 0x%08x to 0x%08x.\r\n", nStackStart, nStackEnd);
				nStackStart = *(uint32_t *)(((uint32_t)xTaskSafety) + 0x30);
				nStackEnd   = *(uint32_t *)(((uint32_t)xTaskSafety) + 0x00);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "TaskSafety stack from 0x%08x to 0x%08x.\r\n", nStackStart, nStackEnd);
			}
			else if((strncmp(pcParameter1, "inj", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{
				nStackStart = *(uint32_t *)(((uint32_t)xTaskLed) + 0x30);
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Writing data to stack address 0x%08x.\r\n", (unsigned int)nStackStart);

				*((uint32_t *)nStackStart) = 0x11111111;		//overwrite the data at the stack bottom
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
	}

	return xResult;
}


/********************************************************************************************
 * function for NVIC command: inject interrupt error
 * inject unknown interrupt
 * inject false interrupt
 ********************************************************************************************/
extern void LVD_LVW_IRQHandler(void);
BaseType_t prvNvic_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_NVIC);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "inj1", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Generate an interrupt to trigger DefaultISR.\r\n");
				//the following code is to enable LPIT0CH3 and generate an interrupt.
				if( 0 == (PCC->PCCn[PCC_LPIT_INDEX] & PCC_PCCn_CGC_MASK) )
				{
					if( 0 == (PCC->PCCn[PCC_LPIT_INDEX] & PCC_PCCn_PCS_MASK) )
						PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_CGC_MASK | PCC_PCCn_PCS(6);
					else
						PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
				}
				if( 0 == (LPIT0->MCR & LPIT_MCR_M_CEN_MASK) )
					LPIT0->MCR |= (LPIT_MCR_M_CEN_MASK | LPIT_MCR_DBG_EN_MASK | LPIT_MCR_DOZE_EN_MASK);
				LPIT0->TMR[3].TVAL = 160000;
				LPIT0->TMR[3].TCTRL = LPIT_TMR_TCTRL_TSOI_MASK;
				LPIT0->MIER |= LPIT_MIER_TIE3_MASK;	//enable LPIT0_CH3 interrupt
				LPIT0->TMR[3].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;	//start timer 3
				INT_SYS_EnableIRQ(LPIT0_Ch3_IRQn);
			}
			else if((strncmp(pcParameter1, "inj2", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Jump to an ISR without interrupt occurred.\r\n");
				LVD_LVW_IRQHandler();
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
	}

	return xResult;
}


/********************************************************************************************
 * function for REGCHK command: modify protected registers values to inject fault
 * view calculated crc value and saved crc value;
 * inject error by modifying one PORT PCR register;
 ********************************************************************************************/
BaseType_t prvRegchk_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_REGCHK);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Init CRC value of all PORT PCR registers is 0x%04X.\r\n", regPortPcrCrcBaseValue);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Runtime CRC value of all PORT PCR registers is 0x%04X.\r\n", regPortPcrCrcRuntimeValue);
			}
			else if((strncmp(pcParameter1, "inj", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{
				tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Modify PORT registers to inject error.\r\n");
				PORTA->PCR[0] = 0x01;
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
	}

	return xResult;
}


/********************************************************************************************
 * function for "safebt" command:
 * safeboot feature calculates the CRC value of specified memory ranges.
 * view calculated crc value and saved crc value of the specified flash memory range;
 ********************************************************************************************/
extern uint32_t safeBootStartAddr[];
extern uint32_t safeBootSize[];
BaseType_t prvSafebt_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	uint32_t tmpRslt;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_SAFEBT);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{
				/*
				if( OSIF_MutexLock(&mutexCrc, 0) == STATUS_SUCCESS)
				{
					WDOG_Disable();
					INT_SYS_DisableIRQGlobal();
					safeBootCheckSum[0] = Safeboot_CrcCalc();
					INT_SYS_EnableIRQGlobal();
					WDOG_Enable();
					OSIF_MutexUnlock(&mutexCrc);
				}
				*/
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC calculation block 1: 0x%08X ~ 0x%08X.\r\n", (unsigned int)safeBootStartAddr[0], (unsigned int)(safeBootStartAddr[0] + safeBootSize[0]));
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CRC calculation block 2: 0x%08X ~ 0x%08X.\r\n", (unsigned int)safeBootStartAddr[1], (unsigned int)(safeBootStartAddr[1] + safeBootSize[1]));
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Saved CRC value of the memory is 0x%08X.\r\n", (unsigned int)safeCrcRefValue);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Calculated CRC value of the memory is 0x%08X.\r\n", (unsigned int)safeBootCheckSum[0]);
				//Safeboot_StartCrcDma();
			}
			else if ((strncmp(pcParameter1, "check", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{
				WDOG_Disable();
				OSIF_MutexLock(&mutexCrc, 50);
				tmpRslt = Safeboot_CrcCalc();
				OSIF_MutexUnlock(&mutexCrc);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Calculated CRC value of all specified blocks is 0x%08X.\r\n", (unsigned int)tmpRslt);
				WDOG_Enable();
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
	}

	return xResult;
}


/********************************************************************************************
 * function for processing command"sram": SRAM ECC error injection:
 * inject ECC error to SRAM_L
 * inject ECC error to SRAM_U
 ********************************************************************************************/
extern uint32_t __RAM_END;
extern uint32_t STACK_SIZE;
BaseType_t prvSram_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	//uint32_t tmp2 = 0;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_SRAM);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "inj1", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
			{
				ERM->SR0 = 0xFFFFFFFF;	//clear all SRAM ECC error flags
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Use EIM to inject ECC error to SRAM_L.\r\n");
				SRAM_EccInject(0);
				/*
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Read addr 0x1FFFFFF0 to trigger SRAM_L ECC error.\r\n");
				tmp2 += *(uint32_t *)0x1FFFFFF0;		//read the SRAM area with ECC errors
				tmp2 += *(uint32_t *)(0x1FFFFFF0 + 4);
				tmp2 += *(uint32_t *)(0x1FFFFFF0 + 8);
				tmp2 += *(uint32_t *)(0x1FFFFFF0 + 12);
				if( ERM->SR0 & ERM_SR0_SBC0_MASK )
				{
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SRAM_L single bit error detected.\r\n");
					ERM->SR0 = ERM_SR0_SBC0_MASK;	//clear SRAM_L single bit error flag
				}
				else
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "No SRAM_L single bit error.\r\n");
				*/
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Use \"fault read\" command to check if multiple bit error detected.\r\n");
			}
			else if((strncmp(pcParameter1, "inj2", xP1StrLen) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))
			{
				ERM->SR0 = 0xFFFFFFFF;	//clear all SRAM ECC error flags
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Use EIM to inject ECC error to SRAM_U.\r\n");
				SRAM_EccInject(1);
				/*
				tmp2 += *(uint32_t *)( 0x20000000 );		//read the SRAM area with ECC errors
				tmp2 += *(uint32_t *)( 0x20000000 + 4 );
				tmp2 += *(uint32_t *)( 0x20000000 + 8 );
				tmp2 += *(uint32_t *)( 0x20000000 + 12 );
				if( ERM->SR0 & ERM_SR0_SBC1_MASK )
				{
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SRAM_U single bit error detected.\r\n");
					ERM->SR0 = ERM_SR0_SBC1_MASK;	//clear SRAM_L single bit error flag
				}
				else
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "No SRAM_U single bit error.\r\n");
				*/
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Use \"fault read\" command to check if multiple bit error detected.\r\n");

			}
			else if((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0))
			{
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SRAM_L: Single bit SBC0=%u, multi-bit NCE0=%u;\r\n", (unsigned int)(ERM->SR0&ERM_SR0_SBC0_MASK)>>ERM_SR0_SBC0_SHIFT, (unsigned int)(ERM->SR0&ERM_SR0_NCE0_MASK)>>ERM_SR0_NCE0_SHIFT);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SRAM_L: Single bit SBC1=%u, multi-bit NCE1=%u;\r\n", (unsigned int)(ERM->SR0&ERM_SR0_SBC1_MASK)>>ERM_SR0_SBC1_SHIFT, (unsigned int)(ERM->SR0&ERM_SR0_NCE1_MASK)>>ERM_SR0_NCE1_SHIFT);
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
	}

	return xResult;
}


/********************************************************************************************
 * function for processing command"scst": enable/disable SCST core self-test, and specify test items
 ********************************************************************************************/
BaseType_t prvSCST_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2, *pcParameter3, *pcParameter4;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xP3StrLen, xResult;
	int32_t u32startnum, u32endnum, u32injnum;
	int32_t u32Rslt, u32Signature;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)	/* if parameter number is 0 */
	{
		strcpy(pcWriteBuffer, MAN_SCST);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)	/* if parameter number is 1 */
		{
			if ((strncmp(pcParameter1, "view", xP1StrLen) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0))
			{
				u32Rslt = SCST_GetTestResult();
				u32Signature = SCST_GetSignature();
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SCST test number is from %u to %u.\r\n", (unsigned int)SCST_GetStartNum(), (unsigned int)SCST_GetEndNum());
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SCST result is    0x%08X.\r\nSCST signature is 0x%08X.\r\n", (unsigned int)u32Rslt, (unsigned int)u32Signature);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SCST was interrupted %d times.\r\n", (unsigned int)cntScstInterrupted);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "SCST was completed   %d times.\r\n", (unsigned int)cntScstCompleted);
				//tmp += snprintf(pcWriteBuffer, xWriteBufferLen, "Request execution of ALU tests include atomic function 30 31 32 33 34 35 and 13.\r\nRight signature is 0x608b.\r\nCalculation signature = 0x%x.\r\n", (m4_scst_uint32_t)(u32tempresult1 ^ u32tempresult2 ));	//print ALU test value
			}
			else if(( strncmp( pcParameter1, "on", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "4", xP1StrLen ) == 0 ))
			{
				bEnableScstRun = 1;
				tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "Turn on SCST execution.\r\n");
			}
			else if(( strncmp( pcParameter1, "off", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "5", xP1StrLen ) == 0 ))
			{
				bEnableScstRun = 0;
				tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "Turn off SCST execution.\r\n");
			}
			else
			{
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
			}
		}
		else
		{
			pcParameter3 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 3, &xP3StrLen);
			if(pcParameter3 == NULL)	/* if parameter number is 2 */
			{
				if(( strncmp( pcParameter1, "inj", xP1StrLen ) == 0 ) || (strncmp( pcParameter1, "2", xP1StrLen ) == 0 ))
				{
					sscanf((char *)pcParameter2, "%d", (int *)(&u32injnum));
					m4_scst_fault_inject_test_index = u32injnum;
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Inject SCST fault to test %u.\r\n", (unsigned int)u32injnum);
					tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Use \"fault read\" command to check if SCST fault recorded.\r\n");
				}
				else
				{
					snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
				}
			}
			else						/* if parameter number is 3 */
			{
				pcParameter4 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 4, &xP3StrLen);
				if(pcParameter4 == NULL)	/* if parameter number is 3 */
				{
					sscanf((char *)pcParameter2, "%d", (int *)(&u32startnum));
					sscanf((char *)pcParameter3, "%d", (int *)(&u32endnum));
					if ((strncmp(pcParameter1, "set", xP1StrLen) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))
					{
						SCST_SetStartNum(u32startnum);
						SCST_SetEndNum(u32endnum);
						tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "Periodic SCST test is from %d to %d.\r\n", (unsigned int)u32startnum, (unsigned int)u32endnum);
					}
					else
						snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
				}
				else
				{
					snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
				}
			}
		}
	}

	return xResult;
}

/********************************************************************************************
 * function for processing command"fls": inject flash protection error, flash ECC error, flash over-program errors.
 ********************************************************************************************/
BaseType_t prvFls_Command( char *pcWriteBuffer,
								 size_t xWriteBufferLen,
								 const char *pcCommandString )
{
	char *pcParameter1, *pcParameter2;
	int32_t tmp;
	BaseType_t xP1StrLen, xP2StrLen, xResult;
	uint32_t dstAddr = 0x0007F000;
	uint32_t flsProt;
	uint32_t tmpData;
	uint8_t Res=1u;
	xResult = pdPASS;

	tmp = 0;
	pcParameter1 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 1, &xP1StrLen);
	if(pcParameter1 == NULL)
	{
		strcpy(pcWriteBuffer, MAN_FLS);
		xResult = pdFALSE;
	}
	else
	{
		xResult = pdFALSE;
		pcParameter2 = (char *)FreeRTOS_CLIGetParameter( pcCommandString, 2, &xP2StrLen);
		if(pcParameter2 == NULL)
		{
			if ((strncmp(pcParameter1, "injprot", 7) == 0)||(strncmp(pcParameter1, "1", xP1StrLen) == 0))	//protect region 31 and program it (at 0x7F000) to trigger flash protection violation error
			{
				//enable PFlash protection in region 31 and program this region to verify the protection
				dstAddr = 0x7F000;
				//Res = FLASH_DRV_EraseSector(&mySSDConfig, dstAddr, 0x1000);
				FLASH_DRV_SetPFlashProtection(0x7FFFFFFF);	//MSB=0 to protect region 31: 0x7C000~0x7FFFF;
				if(FTFC->FSTAT & FTFC_FSTAT_FPVIOL_MASK)
					FTFC->FSTAT = FTFC_FSTAT_FPVIOL_MASK;	//clear flash protection violation flag;
				INT_SYS_DisableIRQGlobal();
				Res = FLASH_DRV_Program(&mySSDConfig, dstAddr, 8u, testPgmData);	//program 8 bytes to 0x7F800
				INT_SYS_EnableIRQGlobal();
				if(Res != STATUS_SUCCESS)
				{
					if(FTFC->FSTAT & FTFC_FSTAT_FPVIOL_MASK)
					{
						FTFC->FSTAT = FTFC_FSTAT_FPVIOL_MASK;	//clear flash protection violation flag;
						FTFC->FSTAT = 0x10;		//clear Flash fault flags
						faultBuffer.faultType = faultFlashProt;
						faultBuffer.faultPcAddr = 0;
						faultBuffer.faultDataAddr = 0xFFFFFFFF;
						Safety_FaultManage_SaveFault(&faultBuffer);
						Safety_FlashProtection_Callback();	//flash protection violation fault address is unknown
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Program PFLASH address 0x%08X failed, FTFC_FSTAT_FPVIOL = %d\r\n", (unsigned int)dstAddr, (FTFC->FSTAT & FTFC_FSTAT_FPVIOL_MASK)>>FTFC_FSTAT_FPVIOL_SHIFT);
						//FLASH_DRV_GetPFlashProtection(&dstAddr);
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "PFLASH protection registers values 0x%02X 0x%02X 0x%02X 0x%02X\r\n", FTFC->FPROT[0], FTFC->FPROT[1], FTFC->FPROT[2], FTFC->FPROT[3]);
					}
					else
					{
						tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Program PFLASH address 0x%08X failed. FTFC_FSTAT = 0x%X\r\n", (unsigned int)dstAddr, FTFC->FSTAT);
					}
				}
				else
				{
					tmp = snprintf(pcWriteBuffer, xWriteBufferLen, "Program PFLASH address 0x%08X successful.\r\n", (unsigned int)dstAddr);
				}
			}
			else if ((strncmp(pcParameter1, "injforce", 8) == 0)||(strncmp(pcParameter1, "2", xP1StrLen) == 0))	//inject Flash ECC error by forced double bit error
			{
				if(FTFC->FSTAT & FTFC_FSTAT_FPVIOL_MASK)
					FTFC->FSTAT = FTFC_FSTAT_FPVIOL_MASK;	//clear flash protection violation flag;
				snprintf(pcWriteBuffer, xWriteBufferLen, "PFlash force double bit error.\r\nUse \"fault read\" command to view faults information.\r\n");
				FLASH_DRV_EnableDoubleBitFaultInterupt();
				FLASH_DRV_ForceDoubleBitFaultDetectCmd(true);
			}
			else if ((strncmp(pcParameter1, "injovp", 6) == 0)||(strncmp(pcParameter1, "3", xP1StrLen) == 0))	//inject Flash ECC error by over-program 0x7F000
			{
				//dstAddr = 0x7F000;
				FLASH_DRV_GetPFlashProtection(&flsProt);
				if(flsProt != 0xFFFFFFFF)
				{
					snprintf(pcWriteBuffer, xWriteBufferLen, "Please reset to remove PFlash protection and retry this command.\r\n");
				}
				else
				{
					INT_SYS_DisableIRQGlobal();
					Res = FLASH_DRV_Program(&mySSDConfig, dstAddr, 8u, testPgmData);	//program 8 bytes to 0x7F800
					testPgmData[0]+=3; testPgmData[1]+=3; testPgmData[2]+=3; testPgmData[3]+=3;
					INT_SYS_EnableIRQGlobal();
					tmpData = *(uint32_t *)dstAddr;
					snprintf(pcWriteBuffer, xWriteBufferLen, "Program the Flash address 0x%X to inject PFlash ECC error.\r\n", (unsigned int)dstAddr);
				}
			}
			else if ((strncmp(pcParameter1, "view", 4) == 0)||(strncmp(pcParameter1, "4", xP1StrLen) == 0))	//read flash specified address and status register
			{
				tmpData = *(uint32_t *)dstAddr;	//read error address;
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "Read address 0x%X value is 0x%X.\r\n", (unsigned int)dstAddr, (unsigned int)tmpData);
				tmp += snprintf(pcWriteBuffer + tmp, xWriteBufferLen, "CCIF=%d FPVIOL=%d FPROT[0]=0x%X DFDIF=%d.\r\n", (FTFC->FSTAT&0x80)>>7, (FTFC->FSTAT&0x10)>>4, *(unsigned int *)(&(FTFC->FPROT[0])), (FTFC->FERSTAT&0x2)>>1);
			}
			else if ((strncmp(pcParameter1, "erase", 5) == 0)||(strncmp(pcParameter1, "5", xP1StrLen) == 0))	//erase the flash sector at 0x7F000;
			{
				if(FTFC->FSTAT & FTFC_FSTAT_FPVIOL_MASK)
					FTFC->FSTAT = FTFC_FSTAT_FPVIOL_MASK;	//clear flash protection violation flag;
				if(FTFC->FPROT[3] & 0x80)
				{
					INT_SYS_DisableIRQGlobal();
					WDOG_Disable();
					Res = FLASH_DRV_EraseSector(&mySSDConfig, dstAddr, 0x1000);	//erase the last PFlash sector
					WDOG_Enable();
					INT_SYS_EnableIRQGlobal();
					snprintf(pcWriteBuffer, xWriteBufferLen, "Flash sector at 0x%08X is erased.\r\n", (unsigned int)dstAddr);
				}
			}
			else
				snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
		}
		else
			snprintf(pcWriteBuffer, xWriteBufferLen, "Parameters error.\r\n");
	}
	return xResult;
}
/*end of the file*/
