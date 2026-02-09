/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
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
/*
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "dmaController1.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "S32K144.h"
#include "lpspiCom1.h"
#include <stdint.h>
#include <stdbool.h>
#include "wait.h"
#include "FS65_driver/sbc_fs65_map.h"
#include "FS65_driver/sbc_fs65_common.h"
#include "FS65_driver/sbc_fs65_communication.h"
#include "FS65_driver/sbc_fs65.h"
#include "utils/nxp_console.h"
#include "diagnostics.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Red on-board LED. */
#define RED_LED_GPIO_PORT   PTD
#define RED_LED_PIN         15U

/* SBC Reset pin. */
#define SBC_RESET_PORT      PORTD
#define SBC_RESET_PIN       6U

/* SBC INTb pin. */
//#define SBC_INTB_PORT       PORTD
//#define SBC_INTB_GPIO_PORT  PTD
//#define SBC_INTB_PIN        6U

/* SBC FS0b pin */
#define SBC_FS0B_PORT       PORTD
#define SBC_FS0B_PIN        7U

/* Debug pin (used e.g. for oscilloscope monitoring) */
#define SBC_DEBUG_PORT  PTD
#define DEBUG_PIN       4U

/* Settings for EVB SW2 button. */
#define SW2_PORT        PORTC
#define SW2_GPIO_PORT   PTDC
#define SW2_PIN         12U

/* Settings for EVB SW3 button. */
#define SW3_PORT        PORTC
#define SW3_GPIO_PORT   PTDC
#define SW3_PIN         13U

/* SPI */
#define SPI_COMM_TIMEOUT_MS 5

/* LPIT Timers settings */
/* Watchdog timer. */
#define LPIT_CHANNEL_WD     0UL
/* Diagnostic timer. */
#define LPIT_CHANNEL_DIAG   1UL

/* Enable periodic Watchdog refresh. */
#define FS65_WD_ENABLED

#ifdef FS65_WD_ENABLED
/* Watchdog period option.  */
#define WD_PERIOD (FS65_W_FS_WD_WINDOW_512MS)
/* Watchdog window period in us. */
#define WD_PERIOD_US (512000U)
/* Watchdog timer, set to refresh the WD in 3/4 of the period (open window). */
#define WD_TIMER_PERIOD_US ((WD_PERIOD_US / 4) * 3)

/* Start timer for watchdog refresh. */
#define WD_TIMER_START() LPIT_DRV_StartTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL_WD));
/* Stop timer for watchdog refresh. */
#define WD_TIMER_STOP() LPIT_DRV_StopTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL_WD));

#endif

/* Start timer for SBC status refresh. */
#define DIAG_TIMER_START() LPIT_DRV_StartTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL_DIAG));
/* Stop timer for SBC status refresh. */
#define DIAG_TIMER_STOP() LPIT_DRV_StopTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL_DIAG));

/*************************
 * Application settings
 *************************/
/* If defined, the MCU will not wait for the RESET pin to release and
 * INIT_FS phase is skipped. */
#define _FS65_DEBUG_MODE
/* Delay between consequent reports of SBC status in [ms]. */
#define FS65_REPORT_DELAY_MS (2000U)

/* ADC settings */
#define ADC_CHN         2U

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Configuration of the FS65 SBC. */
fs65_user_config_t userConfig = {0};

/* Flag indicating WD refresh is required. */
volatile bool isWdRefreshRequired = false;

/* If true, INTb pin was asserted. */
volatile bool intbPinAsserted = false;

/* If true, low priority communication is enabled.
 * Will be set to true right after WD refresh.
 * Communication should be finished before the "closed"
 * WD window ends. */
volatile bool isCommEnabled = false;

/* If true, RESET pin was released. */
volatile bool isResetReleased = false;

/* If true, SW3 button was pressed. */
volatile bool isSw2Pressed = false;

/* If true, SW4 button was pressed. */
volatile bool isSw3Pressed = false;

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

static status_t mcuInit(void);
static fs65_status_t startApp(void);
static void loadUserConfig(fs65_user_config_t* config);
void LPIT_ISR_0(void);
void LPIT_ISR_1(void);

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
        FS65_R_FS_FLT_ERR_IMP_RSTB | FS65_R_FS_FS1B_CAN_IMPACT_RX_ONLY | FS65_R_FS_FLT_ERR_FS_INT3_FIN6,
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
    },
    {
        FS65_M_CAN_LIN_MODE_ADDR,
        FS65_RW_M_LIN_AUTO_DIS_RESET | FS65_RW_M_LIN_MODE_NORMAL | FS65_RW_M_CAN_AUTO_DIS_RESET |
        FS65_RW_M_CAN_MODE_NORMAL,
        FS65_RW_M_LIN_AUTO_DIS_MASK | FS65_RW_M_LIN_MODE_MASK | FS65_RW_M_CAN_AUTO_DIS_MASK |
        FS65_RW_M_CAN_MODE_MASK,
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

    DEV_ASSERT(txFrame != NULL);

    /* Call of blocking driver function. */
    status = LPSPI_DRV_MasterTransferBlocking(LPSPICOM1, txFrame, rxFrame,
            FS65_COMM_FRAME_SIZE_BYTES, SPI_COMM_TIMEOUT_MS);
    if (status != STATUS_SUCCESS)
    {
        return fs65StatusError;
    }

    return fs65StatusOk;
}

/* Implementation of driver function for blocking wait. */
void MCU_WaitUs(uint16_t delay)
{
    S32_WaitUs(delay);
}


/*******************************************************************************
 * Dignostics extern Functions Implementation
 ******************************************************************************/

/* Gets S32 actual ADC resolution. */
status_t getAdcResolution(uint16_t* resolution)
{
    /* Get ADC max value from the resolution */
    switch (adConv1_ConvConfig0.resolution)
    {
        case ADC_RESOLUTION_8BIT:
            *resolution = 255;
            break;

        case ADC_RESOLUTION_10BIT:
            *resolution = 1023;
            break;

        case ADC_RESOLUTION_12BIT:
            *resolution = 4095;
            break;

        default:
            return STATUS_ERROR;
    }
    return STATUS_SUCCESS;
}

/* Helper function for ADC measurements of the AMUX pin. */
void runAmuxConversion(uint16_t* adcRawValue)
{
    /* Configure ADC channel and software trigger a conversion */
    ADC_DRV_ConfigChan(INST_ADCONV1, 0U, &adConv1_ChnConfig0);
    /* Wait for the conversion to be done */
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    /* Store the channel result into a local variable */
    ADC_DRV_GetChanResult(INST_ADCONV1, 0U, adcRawValue);
}

/*******************************************************************************
 * Interrupt handlers
 ******************************************************************************/

/* LPIT interrupt handler used for the Watchdog refresh timing. */
void LPIT_ISR_0()
{
    /* Clear LPIT channel flag */
    LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL_WD));

    /* Enable WD refresh */
    isWdRefreshRequired = true;
}

/* LPIT interrupt handler used for the diagnostics and report timing. */
void LPIT_ISR_1()
{
    /* Clear LPIT channel flag */
    LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL_DIAG));

    /* Enable diagnostics communication. */
    isCommEnabled = true;
}

/* PORTC interrupt handler. Used for buttons SW2 and SW3 detection. */
void PORTC_IRQHandler(void)
{
    /* Button SW2 pushed. */
    if (PINS_DRV_GetPortIntFlag(SW2_PORT) & (1 << SW2_PIN))
    {
        PINS_DRV_ClearPinIntFlagCmd(SW2_PORT, SW2_PIN);
        isSw2Pressed = true;
    }
    /* Button SW3 pushed. */
    if (PINS_DRV_GetPortIntFlag(SW3_PORT) & (1 << SW3_PIN))
    {
        PINS_DRV_ClearPinIntFlagCmd(SW3_PORT, SW3_PIN);
        isSw3Pressed = true;
    }
}

/* Debug port interrupt handler */
void PORTD_IRQHandler(void)
{
    uint8_t portStatus;

    /* RSTB pin raising edge detected */
    if (PINS_DRV_GetPortIntFlag(SBC_RESET_PORT) & (1 << SBC_RESET_PIN))
    {
        PINS_DRV_ClearPinIntFlagCmd(SBC_RESET_PORT, SBC_RESET_PIN);
        portStatus = PINS_DRV_ReadPins(SBC_DEBUG_PORT);

        if ((portStatus & (1 << SBC_RESET_PIN)) == 0)
        {
            isResetReleased = false;
        }
        else
        {
            isResetReleased = true;
        }
    }
}

/*******************************************************************************
 * Functions
 ******************************************************************************/

/* MCU initialization. */
static status_t mcuInit()
{
    status_t status;

    /* Initialize and configure clocks
     *     -   see clock manager component for details
     */
    status = CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
            g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    status |= CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    status |= PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    PINS_DRV_SetPinsDirection(SBC_DEBUG_PORT, (0 << SBC_RESET_PIN));
    PINS_DRV_SetPinsDirection(SBC_DEBUG_PORT, (1 << DEBUG_PIN));

    /* Initialize LPSPI0 (Send)*/
    status |= LPSPI_DRV_MasterInit(LPSPICOM1, &lpspiCom1State, &lpspiCom1_MasterConfig0);
    status = LPSPI_DRV_MasterSetDelay(LPSPICOM1, 4, 1, 1);

    /* Initialize LPIT instance 0
     *  -   Reset and enable peripheral */
    LPIT_DRV_Init(INST_LPIT1, &lpit1_InitConfig);
#ifdef FS65_WD_ENABLED
    /* Initialize LPIT channel 0 and configure it as a periodic counter. */
    lpit1_ChnConfig0.period = WD_TIMER_PERIOD_US;
    status |= LPIT_DRV_InitChannel(INST_LPIT1, LPIT_CHANNEL_WD, &lpit1_ChnConfig0);

    /* Install LPIT_ISR as LPIT interrupt handler */
    INT_SYS_InstallHandler(LPIT0_Ch0_IRQn, &LPIT_ISR_0, (isr_t *)0);
#endif

    /* Initialize LPIT channel 1 and configure it as a periodic counter
     * which is used to enable diagnostic communication during the WD closed
     * window. */
    lpit1_ChnConfig1.period = FS65_REPORT_DELAY_MS * 1000;
    status |= LPIT_DRV_InitChannel(INST_LPIT1, LPIT_CHANNEL_DIAG, &lpit1_ChnConfig1);
    INT_SYS_InstallHandler(LPIT0_Ch1_IRQn, &LPIT_ISR_1, (isr_t *)0);

    /* Initialize debug console / LPUART instance */
    status |= DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_TYPE, \
            &lpuart1_State, &lpuart1_InitConfig0);

    /* Configure the ADC converter */
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig0);
    DEV_ASSERT(adConv1_ChnConfig0.channel == ADC_CHN);

    /* Clear interrupt flag and enable interrupts. */
    INT_SYS_ClearPending(PORTE_IRQn);

    if (status != STATUS_SUCCESS)
    {
        /* Initialization failed. */
        PINS_DRV_ClearPins(RED_LED_GPIO_PORT, 1U << RED_LED_PIN);
    }

    /* Enable interrupts on ports */
    INT_SYS_EnableIRQ(PORTC_IRQn);
    INT_SYS_EnableIRQ(PORTD_IRQn);
    INT_SYS_EnableIRQGlobal();

    return status;
}

/* Stop all timers and wait for the FS65 RESET pin release. */
static void waitForResetRelease()
{
    WD_TIMER_STOP();
    DIAG_TIMER_STOP();

    do
    {
        OSIF_TimeDelay(10);
    }
    while (!isResetReleased);

    PINS_DRV_TogglePins(SBC_DEBUG_PORT, (1 << DEBUG_PIN));
}

/* Main testing application. */
static fs65_status_t startApp()
{
    fs65_status_t sbcStatus = fs65StatusOk; /* SBC return status code. */
    status_t status = STATUS_SUCCESS;       /* Return status code. */
    pins_channel_type_t portStatus;         /* Status of MCU GPIO port. */
    uint8_t errorCounter = 0xFF;            /* Fault Error Counter value. */

    for(;;)
    {
        if (!isResetReleased)
        {
            PRINTF("RSTB pin asserted.\n\r");
            PRINTF("Waiting for SBC RSTB pin release...\n\r");
            /* Wait for SBC to power-up. As this example is for testing with MCU and SBC on two separate boards,
             * GPIO pin on MCU is used for synchronization with the SBC after power up. The input GPIO pin on MCU
             * should be connected to the RSTB pin of the SBC. It is assumed that MCU has its own power source.
             * This function can be removed if the SBC is used as power source for the MCU. */
            waitForResetRelease();

            PRINTF("SBC Initialization started...\n\r");
            sbcStatus = FS65_Init(&userConfig);
            if (sbcStatus != fs65StatusOk)
            {
                PRINTF("FS65 initialization failed.\n\r");
                return sbcStatus;
            }
            PRINTF("OK\n\r");
#ifndef FS65_DEBUG_MODE
            /* Start timers for WD refresh and diagnostics */
            WD_TIMER_START();
#endif
            DIAG_TIMER_START();

            /* Example of runtime settings */
            sbcStatus = FS65_SetRegulatorState(fs65Vkam, true);
            sbcStatus |= FS65_CAN_SetMode(fs65CanModeListenOnly, true);
            sbcStatus |= FS65_LIN_SetMode(fs65LinModeSleepNoWakeup, false);

            if (sbcStatus != fs65StatusOk)
            {
                PRINTF("ERROR: Unable to set SBC parameters.\n\r");
                return sbcStatus;
            }
        }

        /* Test of the LPOFF mode, activated by the SW2 push (on S32K144EVB).
         * Following conditions must be met before LPOFF activation:
         * - SBC in NORMAL mode.
         * - IO_0 (key input) connected to Vsup3.
         *
         * To transition back from the LPOFF mode to normal mode, connect the IO_0 input to GND. */
        if (isSw2Pressed)
        {
            PRINTF("Go to the LPOFF mode.\n\r");
            sbcStatus = FS65_SetLowPowerMode(false);
            if (sbcStatus != fs65StatusOk)
            {
                return sbcStatus;
            }
            isSw2Pressed = false;

            continue;
        }

        /* Test of the LDT timer, activated by the SW3 push (on S32K144EVB).
         * Conditions for LPOFF activation mentioned in previous comment.
         * The LDT timer is set to count for SBC_LDT_AFTER_RUN_VALUE, then SBC transitions to LPOFF mode,
         * waits for the SBC_LDT_WAKE_UP_VALUE period and wakes up (LDT Function 3).
         */
        if (isSw3Pressed)
        {
            PRINTF("Start of the LDT timer.\n\r");
            status = sbcTestLdt();
            if (status != STATUS_SUCCESS)
            {
                return sbcStatus;
            }
            isSw3Pressed = false;
        }

        /* Refresh WD. Timing handled by the MCU timer. */
        if (isWdRefreshRequired)
        {
            sbcStatus = FS65_WD_Refresh();
            isWdRefreshRequired = false;
            /* Debug pin toggle, used for monitoring with oscilloscope. */
            PINS_DRV_TogglePins(SBC_DEBUG_PORT, (1 << DEBUG_PIN));
        }

        /* Diagnostic communication is enabled just once after the WD is refreshed
         * (during the WD CLOSED window). */
        if (isCommEnabled && !isWdRefreshRequired)
        {
            /* Read the state of the debug port. */
            portStatus = PINS_DRV_ReadPins(SBC_DEBUG_PORT);

            sbcStatus = FS65_GetFaultErrorCounterValue(&errorCounter);
            if (sbcStatus != fs65StatusOk)
            {
                return sbcStatus;
            }

            if (errorCounter)
            {
                PRINTF("Fault Error Counter: %d\n\r", errorCounter);
            }
            /* FS0b pin asserted, start release procedure. */
            else if ((portStatus & (1 << SBC_FS0B_PIN)) == 0)
            {
                PRINTF("FSxb pins release started...\n\r");

                /* Close S1 switch between VPRE and VPU_FS if FS1B backup delay was engaged (FS1B_DLY_DRV bit = 1). */
                sbcStatus = FS65_WriteRegister(FS65_FS_SF_OUTPUT_REQUEST_ADDR, 0, NULL);

                /* Try to release the FS0b pin. */
                sbcStatus |= FS65_ReleaseFSx(fs65ReleaseFs0bFs1b);

                if (sbcStatus != fs65StatusOk)
                {
                    PRINTF("FS65_ReleaseFSx() failed.\r\n");
                }
                else
                {
                    PRINTF("FSx outputs were released.\r\n");
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
            isCommEnabled = false;
        }
    }
    return sbcStatus;
}

/* Loads used configuration structure. */
static void loadUserConfig(fs65_user_config_t* config)
{
    config->initMainRegs = initMainRegs;
    config->initMainRegsCount = 4;

    config->initFailSafeRegs = initFailSafeRegs;
    config->initFailSafeRegsCount = 11;

    config->nonInitRegs = nonInitRegs;
    config->nonInitRegsCount = 4;

    config->initIntReg = 0;
}

/*
 * The main function for the project.
 * The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void)
{
    /* Write your local variable definition here */
    status_t error;
    fs65_status_t fsError = fs65StatusOk;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                 /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    loadUserConfig(&userConfig);

    /* Initialize MCU peripherals. */
    error = mcuInit();

    if (error == STATUS_SUCCESS)
    {
        PRINTF("------------- BEGIN ---------------\r\n");

        if ((fsError = startApp()) != fs65StatusOk)
        {
            PRINTF("An error occurred (0x%04x)\r\n)", fsError);
            printSbcStatus();
        }
        PRINTF("-------------- END ----------------\r\n");
    }
    else
    {
        PRINTF("An error occurred during MCU initialization.\r\n");
        PRINTF("Error code: (0x%04x)\r\n", error);
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
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
