/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/* ###################################################################
 **     Filename    : main.c
 **     Processor   : MPC574xP
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.00
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* If defined, output to serial console is enabled.
 * Console output is functional just with the DEVKIT-MPC5744P board.
 * It should be disabled for boards without the UART interface. */
#define CONSOLE_ENABLED

/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

#include <stdio.h>

#include "Cpu.h"
#include "dspi1.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "dmaController1.h"

#include "FS65_driver/sbc_fs65.h"
#include "FS65_driver/sbc_fs65_communication.h"

#include "diagnostics.h"

#ifdef CONSOLE_ENABLED
#include "utils/nxp_console.h"
#endif

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* If enabled, initialization of the SBC is skipped. */
#define _FS65_DEBUG_MODE_ENABLED

#ifdef CONSOLE_ENABLED
/* The LPUART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#else
#define PRINTF(s, ...) ((void)0)
#endif

/* Delay between consequent reports of SBC status in [ms]. */
#define FS65_REPORT_DELAY_MS (2000U)

/* SPI */
#define SPI_COMM_TIMEOUT_MS 5

/* ADC1_AN1 corresponding to PB[14] */
#define ADC_MUX_OUT_CHAN_NUM 1U

/*!
 * @name Watchdog settings
 * @{
 */
/*! Enable periodic Watchdog refresh. */
#define FS65_WD_ENABLED

#ifdef FS65_WD_ENABLED
/*! Watchdog period option.  */
#define WD_PERIOD (FS65_W_FS_WD_WINDOW_512MS)
/*! Watchdog window period in us. */
#define WD_PERIOD_US (512000U)
/*! Watchog timer, set to refresh the WD in 3/4 of the period (open window). */
#define WD_TIMER_PERIOD_US ((WD_PERIOD_US / 4) * 3)

/* Start timer for SBC status refresh */
#define DIAG_TIMER_START() PIT_DRV_StartChannel(INST_PIT1, pit1_ChnConfig1.hwChannel)
#define DIAG_TIMER_STOP() PIT_DRV_StopChannel(INST_PIT1, pit1_ChnConfig1.hwChannel)

#define WD_TIMER_START() PIT_DRV_StartChannel(INST_PIT1, pit1_ChnConfig0.hwChannel)
#define WD_TIMER_STOP() PIT_DRV_StopChannel(INST_PIT1, pit1_ChnConfig0.hwChannel)

#endif
/*! @} */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* This structure holds information about configurable registers.
 * See comments of the fs65_user_config_t structure for details. */
fs65_user_config_t userConfig = {0};

/* Flag indicating WD refresh is required. */
volatile bool isWdRefreshRequired = false;

/* If true, low priority communication is enabled.
 * Will be set to true right after WD refresh.
 * Communication should be finished before the "closed"
 * WD window ends. */
volatile bool isCommEnabled = false;

/* Flag used to store if an ADC PAL conversion group has finished executing. */
volatile bool groupConvDone = false;

/* Flag for the user button SW1. */
volatile bool isSw1Pressed = false;

/* Flag for the user button SW2. */
volatile bool isSw2Pressed = false;

/* Flag for the LDT test status. */
volatile bool isLdtTestStarted = false;

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

static status_t mcuInit(void);
static status_t startApp(void);
static void loadUserConfig(fs65_user_config_t* config);

/*******************************************************************************
 * FS65 configuration structures
 *
 * See description for the fs65_user_config_t structure for more information.
 *
 ******************************************************************************/

static fs65_reg_config_value_t initMainRegs[] =
{
        {
                FS65_M_INIT_VREG_ADDR,
                FS65_RW_M_VAUX_TRK_EN_NO_TRACKING | FS65_RW_M_TAUX_LIM_OFF_50_MS |
                FS65_RW_M_VCAN_OV_MON_OFF | FS65_RW_M_IPFF_DIS_ENABLED | FS65_RW_M_TCCA_LIM_OFF_50_MS |
                FS65_RW_M_ICCA_LIM_ICCA_LIM_OUT,
                FS65_RW_M_VAUX_TRK_EN_MASK | FS65_RW_M_TAUX_LIM_OFF_MASK | FS65_RW_M_VCAN_OV_MON_MASK |
                FS65_RW_M_IPFF_DIS_MASK | FS65_RW_M_TCCA_LIM_OFF_MASK | FS65_RW_M_ICCA_LIM_MASK,
                false
        },
        {
                FS65_M_INIT_WU1_ADDR,
                FS65_RW_M_WU_IO4_NO_WAKEUP | FS65_RW_M_WU_IO3_NO_WAKEUP | FS65_RW_M_WU_IO2_NO_WAKEUP |
                FS65_RW_M_WU_IO0_RISING_EDGE,
                0xFFU,
                false
        },
        {
                FS65_M_INIT_WU2_ADDR,
                FS65_RW_M_LIN_SR_20KBITS | FS65_RW_M_LIN_J2602_DIS_COMPLIANT | FS65_RW_M_CAN_WU_TO_120US |
                FS65_RW_M_CAN_DIS_CFG_RX_ONLY | FS65_RW_M_WU_IO5_NO_WAKEUP,
                0xFFU,
                false
        },
        {
                FS65_M_INIT_INH_INT_ADDR,
                FS65_RW_M_INT_INH_0_NOT_MASKED | FS65_RW_M_INT_INH_2_MASKED | FS65_RW_M_INT_INH_3_MASKED |
                FS65_RW_M_INT_INH_4_NOT_MASKED | FS65_RW_M_INT_INH_5_NOT_MASKED,
                FS65_RW_M_INT_INH_0_MASK | FS65_RW_M_INT_INH_2_MASK | FS65_RW_M_INT_INH_3_MASK |
                FS65_RW_M_INT_INH_4_MASK | FS65_RW_M_INT_INH_5_MASK,
                false
        }
};

static fs65_reg_config_value_t initFailSafeRegs[] =
{
        {
                FS65_FS_INIT_FS1B_TIMING_ADDR,
                FS65_R_FS_FS1B_TIME_106_848MS,
                FS65_R_FS_FS1B_TIME_MASK,
                true
        },
        {
                FS65_FS_INIT_SUPERVISOR_ADDR,
                FS65_R_FS_FS1B_TIME_RANGE_X1 | FS65_R_FS_VAUX_5D_NORMAL | FS65_R_FS_VCCA_5D_NORMAL |
                FS65_R_FS_VCORE_5D_NORMAL,
                0x0FU,
                true
        },
        {
                FS65_FS_INIT_FAULT_ADDR,
                FS65_R_FS_FLT_ERR_IMP_RSTB | FS65_R_FS_FLT_ERR_FS_INT3_FIN6, /* FS65_R_FS_FS1B_CAN_IMPACT_RX_ONLY cannot be set in the device which has no FS1B. */
                0x0FU,
                true
        },
        {
                FS65_FS_INIT_FSSM_ADDR,
                FS65_R_FS_RSTB_DURATION_10MS | FS65_R_FS_PS_HIGH | FS65_R_FS_IO_23_FS_NOT_SAFETY |
                FS65_R_FS_IO_45_FS_NOT_SAFETY,
                0x0FU,
                true
        },
        {
                FS65_FS_INIT_SF_IMPACT_ADDR,
                FS65_R_FS_WD_IMPACT_RSTB | FS65_R_FS_DIS_8S_ENABLED | FS65_R_FS_TDLY_TDUR_DELAY,
                0x0FU,
                true
        },
        {
                FS65_FS_WD_WINDOW_ADDR,
                FS65_R_FS_WD_WINDOW_512MS,
                0x0FU,
                true
        },
        {
                FS65_FS_WD_LFSR_ADDR,
                FS65_WD_SEED_DEFAULT,
                0xFFU,
                false
        },
        {
                FS65_FS_INIT_WD_CNT_ADDR,
                FS65_R_FS_WD_CNT_RFR_6 | FS65_R_FS_WD_CNT_ERR_6,
                0x0FU,
                true
        },
        {
                FS65_FS_INIT_VCORE_OVUV_IMPACT_ADDR,
                FS65_R_FS_VCORE_FS_UV_FS0B | FS65_R_FS_VCORE_FS_OV_RSTB_FS0B,
                0x0FU,
                true
        },
        {
                FS65_FS_INIT_VCCA_OVUV_IMPACT_ADDR,
                FS65_R_FS_VCCA_FS_UV_FS0B | FS65_R_FS_VCCA_FS_OV_RSTB_FS0B,
                0x0FU,
                true
        },
        {
                FS65_FS_INIT_VAUX_OVUV_IMPACT_ADDR,
                FS65_R_FS_VAUX_FS_UV_FS0B | FS65_R_FS_VAUX_FS_OV_RSTB_FS0B,
                0x0FU,
                true
        }
};

static fs65_reg_config_value_t nonInitRegs[] =
{
        {
                FS65_M_MODE_ADDR,
                FS65_RW_M_VKAM_EN_DISABLED,
                FS65_RW_M_VKAM_EN_MASK,
                true
        },
        {
                FS65_M_REG_MODE_ADDR,
                FS65_R_M_VCAN_EN_ENABLED | FS65_R_M_VAUX_EN_ENABLED | FS65_R_M_VCCA_EN_ENABLED |
                FS65_R_M_VCORE_EN_ENABLED,
                FS65_R_M_VCAN_EN_MASK | FS65_R_M_VAUX_EN_MASK | FS65_R_M_VCCA_EN_MASK |
                FS65_R_M_VCORE_EN_MASK,
                true
        },
        {
                FS65_M_IO_OUT_AMUX_ADDR,
                FS65_RW_M_AMUX_VREF | FS65_RW_M_IO_OUT_4_LOW | FS65_RW_M_IO_OUT_4_EN_Z,
                FS65_RW_M_AMUX_MASK | FS65_RW_M_IO_OUT_4_MASK | FS65_RW_M_IO_OUT_4_EN_MASK,
                false
        }
};

/*******************************************************************************
 * FS65 driver extern Functions Implementation
 ******************************************************************************/

/* Implementation of driver function for SPI communication. */
fs65_status_t MCU_SPI_TransferData(uint8_t* txFrame, uint8_t* rxFrame)
{
    status_t status;
    uint8_t tx[2];
    uint8_t rx[2];

    tx[0] = txFrame[1];
    tx[1] = txFrame[0];

    /* Start the blocking transfer */
    status = DSPI_MasterTransferBlocking(INST_DSPI1, tx, rx, 1, SPI_COMM_TIMEOUT_MS);
    if (status != STATUS_SUCCESS)
    {
        return fs65StatusError;
    }

    if (rxFrame != NULL)
    {
        rxFrame[0] = rx[1];
        rxFrame[1] = rx[0];
    }

    return fs65StatusOk;
}

/* Wait at least delay us. */
void MCU_WaitUs(uint16_t delay)
{
    /* OSIF wait function has resolution 1 ms. As in this example
     * the watchdog period is 512 ms, this resolution is sufficient.
     * For time critical application usage, the function should be
     * implemented with real 1 us resolution. */
    OSIF_TimeDelay(delay / 1000 + 1);
}

/*******************************************************************************
 * Dignostics extern Functions Implementation
 ******************************************************************************/

/* Helper function for ADC measurements of the AMUX pin. */
void runAmuxConversion(uint16_t* adcRawValue)
{
    uint16_t adcResult;

    /* Read MUX_OUT input. */
    ADC_DRV_StartConversion(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL);
    while(!(ADC_DRV_GetStatusFlags(INST_ADCONV1) & ADC_FLAG_NORMAL_ENDCHAIN));
    uint32_t len = ADC_DRV_GetConvResultsToArray(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, &adcResult, 1u);
    if (len == 1U)
    {
        *adcRawValue = adcResult;
    }
    else
    {
        *adcRawValue = 0;
        PRINTF(">>> ADC reading error.\n\r");
    }
    ADC_DRV_ClearStatusFlags(INST_ADCONV1, ADC_FLAG_NORMAL_ENDCHAIN);
}

/* Gets actual ADC resolution. */
status_t getAdcResolution(uint16_t* resolution)
{
    /* MPC5744P has ADC with 12-bit resolution. */
    *resolution = 4095;
    return STATUS_SUCCESS;
}

/*******************************************************************************
 * Interrupt handlers
 ******************************************************************************/

/* LPIT interrupt handler used for the Watchdog refresh timing. */
void PIT_Ch0_IRQHandler()
{
    /* Clear channel 0 interrupt flag */
    PIT_DRV_ClearStatusFlags(INST_PIT1, pit1_ChnConfig0.hwChannel);

    /* Enable WD refresh */
    isWdRefreshRequired = true;
}

/* LPIT interrupt handler used for the diagnostics and report timing. */
void PIT_Ch1_IRQHandler()
{
    /* Clear channel 1 interrupt flag */
    PIT_DRV_ClearStatusFlags(INST_PIT1, pit1_ChnConfig1.hwChannel);

    /* Enable diagnostics communication. */
    isCommEnabled = true;
}

/* LPIT interrupt handler used for the push-buttons debouncing. */
void PIT_Ch2_IRQHandler()
{
    pins_channel_type_t portF;
    static uint16_t sw1Counter = 0;
    static uint16_t sw2Counter = 0;

    /* Clear channel 1 interrupt flag */
    PIT_DRV_ClearStatusFlags(INST_PIT1, pit1_ChnConfig2.hwChannel);

    /* Read status of the PORTF as the DEVKIT push-buttons are connected to it. */
    portF = PINS_DRV_ReadPins(PTF);

    if ((portF & (1 << 12)) > 0)
    {
        sw1Counter++;
    }
    else
    {
        sw1Counter = 0;
    }

    if ((portF & (1 << 13)) > 0)
    {
        sw2Counter++;
    }
    else
    {
        sw2Counter = 0;
    }

    isSw1Pressed = sw1Counter > 10;
    isSw2Pressed = sw2Counter > 10;
}

/*******************************************************************************
 * Functions
 ******************************************************************************/

/* Loads used configuration structure. */
static void loadUserConfig(fs65_user_config_t* config)
{
    config->initMainRegs = initMainRegs;
    config->initMainRegsCount = 4;

    config->initFailSafeRegs = initFailSafeRegs;
    config->initFailSafeRegsCount = 11;

    config->nonInitRegs = nonInitRegs;
    config->nonInitRegsCount = 3;

    config->initIntReg = 0;
}

/* MCU initialization. */
static status_t mcuInit()
{
    status_t status = STATUS_SUCCESS;

    /* Initialize and configure clocks
     *     -   see clock manager component for details
     */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    status |= PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* SPI master configuration: clock speed: 500 kHz, 8 bits/frame, MSB first */
    status |= DSPI_MasterInit(INST_DSPI1, &dspi1State, &dspi1_MasterInitConfig0);

    /* Configure delay between transfer (4 us),
     * delay between SCK and PCS and
     * delay between PCS and SCK */
    status |= DSPI_MasterSetDelay(INST_DSPI1, 12, 3, 3);

    /* Initialize PIT instance 0 */
    PIT_DRV_Init(INST_PIT1, &pit1_InitConfig);

#ifdef FS65_WD_ENABLED
    /* Initialize LPIT channel 0 and configure it as a periodic counter. */
    pit1_ChnConfig0.period = WD_TIMER_PERIOD_US;
    status |= PIT_DRV_InitChannel(INST_PIT1, &pit1_ChnConfig0);
    PIT_DRV_EnableChannelInterrupt(INST_PIT1, pit1_ChnConfig0.hwChannel);
    INT_SYS_EnableIRQ(PIT_Ch0_IRQn);
#endif

    /* Initialize LPIT channel 1 and configure it as a periodic counter
     * which is used to enable diagnostic communication during the WD closed
     * window. */
    pit1_ChnConfig1.period = FS65_REPORT_DELAY_MS * 1000;
    status |= PIT_DRV_InitChannel(INST_PIT1, &pit1_ChnConfig1);
    PIT_DRV_EnableChannelInterrupt(INST_PIT1, pit1_ChnConfig1.hwChannel);
    INT_SYS_EnableIRQ(PIT_Ch1_IRQn);

    status |= PIT_DRV_InitChannel(INST_PIT1, &pit1_ChnConfig2);
    PIT_DRV_EnableChannelInterrupt(INST_PIT1, pit1_ChnConfig2.hwChannel);
    INT_SYS_EnableIRQ(PIT_Ch2_IRQn);

#ifdef CONSOLE_ENABLED
    /* Initialize debug console / LPUART instance */
    status |= DbgConsole_Init(BOARD_DEBUG_UART_TYPE, &uart_pal1_instance, &uart_pal1_Config0);
#endif

    /* Initialize ADC to read MUX_OUT. */
    ADC_DRV_Reset(INST_ADCONV1);
    ADC_DRV_DoCalibration(INST_ADCONV1);
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvCfg0);
    ADC_DRV_EnableChannel(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, ADC_MUX_OUT_CHAN_NUM);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    return status;
}

/* Main testing application. */
static status_t startApp(void)
{
    fs65_status_t sbcStatus;
    status_t status = STATUS_SUCCESS;       /* Return status code. */
    uint8_t errorCounter = 0xFF;            /* Fault Error Counter value. */
    fs65_rx_data_t rxData;

#ifndef FS65_DEBUG_MODE_ENABLED
    PRINTF("SBC Initialization started... ");
    sbcStatus = FS65_Init(&userConfig);
    if (sbcStatus != fs65StatusOk)
    {
        PRINTF("FAILED\n\r");
        return sbcStatus;
    }
    PRINTF("OK\n\r");
#endif

    /* Start timers for WD refresh and diagnostics */
#ifdef FS65_WD_ENABLED
    WD_TIMER_START();
#endif
    DIAG_TIMER_START();
    PIT_DRV_StartChannel(INST_PIT1, pit1_ChnConfig2.hwChannel);

    /* Example of runtime settings */
    /*
    sbcStatus = FS65_SetRegulatorState(fs65Vkam, true);
    sbcStatus |= FS65_CAN_SetMode(fs65CanModeListenOnly, true);
    sbcStatus |= FS65_LIN_SetMode(fs65LinModeSleepNoWakeup, false);
    */

    while (true)
    {
        /* Test of the LPOFF mode, activated by the SW1 push-button (on DEVKIT-MPC5744P).
         * Following conditions must be met before LPOFF activation:
         * - SBC in NORMAL mode.
         * - IO_0 (key input) connected to Vsup3.
         *
         * To transition back from the LPOFF mode to normal mode, connect the IO_0 input to GND. */
        if (isSw1Pressed)
        {
            PRINTF("Go to the LPOFF mode.\n\r");
            sbcStatus = FS65_SetLowPowerMode(false);
            if (sbcStatus != fs65StatusOk)
            {
                return sbcStatus;
            }
            continue;
        }

        /* Test of the LDT timer, activated by the SW2 push-button (on DEVKIT-MPC5744P).
         * Conditions for LPOFF activation mentioned in previous comment.
         * The LDT timer is set to count for SBC_LDT_AFTER_RUN_VALUE, then SBC transitions to LPOFF mode,
         * waits for the SBC_LDT_WAKE_UP_VALUE period and wakes up (LDT Function 3).
         */
        if (!isLdtTestStarted && isSw2Pressed)
        {
            isLdtTestStarted = true;
            PRINTF("Start of the LDT timer.\n\r");
            status = sbcTestLdt();
            if (status != STATUS_SUCCESS)
            {
                return sbcStatus;
            }
        }
#ifdef FS65_WD_ENABLED
        /* Refresh WD. Timing handled by the MCU timer. */
        if (isWdRefreshRequired)
        {
            sbcStatus = FS65_WD_Refresh();
            if (sbcStatus == fs65StatusOk)
            {
                isWdRefreshRequired = false;
            }
        }
#endif
        /* Diagnostic communication is enabled just once after the WD is refreshed
         * (during the WD CLOSED window). */
        if (isCommEnabled && !isWdRefreshRequired)
        {
            isCommEnabled = false;

            sbcStatus = FS65_GetFaultErrorCounterValue(&errorCounter);
            sbcStatus |= FS65_ReadRegister(FS65_FS_RELEASE_FSXB_ADDR, &rxData);
            if (sbcStatus != fs65StatusOk)
            {
                return sbcStatus;
            }

            /* FS0b pin asserted, start release procedure. */
            if (errorCounter == 0 &&
                    (rxData.readData & FS65_R_FS_FS0B_SNS_MASK) == FS65_R_FS_FS0B_SNS_LOW)
            {
                PRINTF("FSxb pins release started... ");

                /* Close S1 switch between VPRE and VPU_FS if FS1B backup delay was engaged (FS1B_DLY_DRV bit = 1). */
                sbcStatus = FS65_WriteRegister(FS65_FS_SF_OUTPUT_REQUEST_ADDR, 0, NULL);

                /* Try to release the FS0b pin. */
                sbcStatus |= FS65_ReleaseFSx(fs65ReleaseFs0bFs1b);

                if (sbcStatus != fs65StatusOk)
                {
                    PRINTF("FAILED\r\n");
                }
                else
                {
                    PRINTF("OK\r\n");
                }
            }

            PRINTF("\n\r***************** Diagnostic report **********************\n\r");

            if (errorCounter)
            {
                PRINTF(">>> Fault Error Counter: %d\n\r", errorCounter);
            }

            printSbcStatus();
            printAmuxMeasurement();

            PRINTF("\n\r***************** Diagnostic report end *******************\n\r");
        }
    }

    return sbcStatus;
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void)
{
    status_t status;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize MCU peripherals. */
    if ((status = mcuInit()) == STATUS_SUCCESS)
    {
        loadUserConfig(&userConfig);

        PRINTF("\n\r------------- BEGIN ---------------\r\n");

        if (startApp() != STATUS_SUCCESS)
        {
            PRINTF("An error occurred.\n\r");
            printSbcStatus();
        }
        PRINTF("-------------- END ----------------\r\n");
    }
    else
    {
        PRINTF("An error occurred during MCU initialization.\r\n");
        PRINTF("Error code: (0x%04x)\r\n", status);
    }

    /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.1 [05.21]
 **     for the NXP C55 series of microcontrollers.
 **
 ** ###################################################################
 */
