/*
 * diagnostics.c
 *
 *  Created on: Oct 26, 2018
 *      Author: nxf44615
 */

#include "diagnostics.h"
#include "FS65_driver/sbc_fs65.h"
#include "FS65_driver/sbc_fs65_communication.h"
#include "utils/nxp_console.h"
#include "utils/nxp_helpers.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Number of registers with status information. Note that this define can be
 * used by user to create array of required size for function FS65_GetStatus(). */
#define FS65_STAT_NUM 23

/* AMUX */
/* Number of channels of MUX_OUT multiplexer. */
#define FS65_AMUX_NUM_OF_CHANNELS 7U
/* Temperature sensor MUX_OUT output voltage (at TJ = 165 °C). */
#define FS65_V_AMUX_TP 2.15f
/* Internal temperature sensor coefficient */
#define FS65_V_TEMP_COEFF 9.9f
/* Internal voltage Reference with 6.0 V < VSUP < 19 V.
 * This voltage is used for the ADC conversion function
 * in this example. */
#define FS65_V_REF 2.5f
/* Divider ratio (wide input voltage range) at VDDIO = 5.0 V */
#define FS65_V_AMUX_WD_5 5U
/* Divider ratio (wide input voltage range) at VDDIO = 3.3 V */
#define FS65_V_AMUX_WD_3P3 7U
/* Divider ratio (tight input voltage range) at VDDIO = 5.0 V */
#define FS65_V_AMUX_TG_5 2U
/* Divider ratio (tight input voltage range) at VDDIO = 3.3 V */
#define FS65_V_AMUX_TG_3P3 3U

/* LDT */
/* LDT after-run value [seconds]. */
#define SBC_LDT_AFTER_RUN_VALUE 5
/* LDT wake-up value [seconds]. */
#define SBC_LDT_WAKE_UP_VALUE 3

/* Pointer to function used for conversion of AMUX measured value. */
typedef float (*conversion_function_t)(float);

/* Structure used for AMUX measurements console print. */
typedef struct
{
    const fs65_amux_selection_t amuxSelection;
    const char* text;
    conversion_function_t conversionFunc;
    char* unit;
    float value;
} amux_meas_t;

/* FS65 supply voltage values. */
typedef enum
{
    fs65SupplyVoltage3V3,
    fs65SupplyVoltage5V,
} fs65_supply_voltage_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* This array contains addresses of registers with status information. */
static const uint8_t g_statAddrMap[] = {
     FS65_M_INIT_VREG_ADDR, FS65_M_LONG_DURATION_TIMER_ADDR, FS65_M_HW_CONFIG_ADDR,
     FS65_M_WU_SOURCE_ADDR, FS65_M_DEVICE_ID_ADDR, FS65_M_IO_INPUT_ADDR,
     FS65_M_DIAG_VPRE_ADDR, FS65_M_DIAG_VCORE_ADDR, FS65_M_DIAG_VCCA_ADDR,
     FS65_M_DIAG_VAUX_ADDR, FS65_M_DIAG_VSUP_VCAN_ADDR, FS65_M_DIAG_CAN_FD_ADDR,
     FS65_M_DIAG_CAN_LIN_ADDR, FS65_M_DIAG_SPI_ADDR,
     FS65_M_MODE_ADDR, FS65_M_REG_MODE_ADDR, FS65_M_CAN_LIN_MODE_ADDR,
     FS65_FS_BIST_ADDR, FS65_FS_RELEASE_FSXB_ADDR, FS65_M_DIAG_SF_IOS_ADDR,
     FS65_M_WD_COUNTER_ADDR, FS65_M_DIAG_SF_ERR_ADDR, FS65_M_DEVICE_ID_FS_ADDR,
};

/* Buffer used for conversion to binary format. */
static char hpBuff[20];

/* Names of status registers of SBC. */
static const char* sbcStatNameStr[FS65_STAT_NUM] = {
    "INIT_VREG",
    "LONG_DURATION_TIMER",
    "HW_CONFIG",
    "WU_SOURCE",
    "DEVICE_ID",
    "IO_INPUT",
    "DIAG_VPRE",
    "DIAG_VCORE",
    "DIAG_VCCA",
    "DIAG_VAUX",
    "DIAG_VSUP_VCAN",
    "DIAG_CAN_FD",
    "DIAG_CAN_LIN",
    "DIAG_SPI",
    "MODE",
    "REG_MODE",
    "CAN_LIN_MODE",
    "BIST",
    "RELEASE_FSXB",
    "DIAG_SF_IOS",
    "WD_COUNTER",
    "DIAG_SF_ERR",
    "DEVICE_ID_FS"
};

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
status_t sbcGetStatus(fs65_rx_data_t* sbcStatusData);
static status_t adcCalibration(uint16_t adcResolution, float* lsbSize);
static status_t getSupplyVoltage(fs65_supply_voltage_t* voltage);

/* This function reads content of registers of the SBC with status
 * values (wake-up sources, diagnostics, device ID, etc.)
 * Note that it is up to user to evaluate obtained data. In order to
 * facilitate this task, user is advice to use masks in register map. */
status_t sbcGetStatus(fs65_rx_data_t* sbcStatusData)
{
    fs65_status_t    err;   /* Status variable. */
    uint8_t     i;          /* Index to array. */

    DEV_ASSERT(sbcStatusData != NULL);

    for (i = 0U; i < FS65_STAT_NUM; i++)
    {
        err = FS65_ReadRegister(g_statAddrMap[i], &sbcStatusData[i]);
        if (err != fs65StatusOk)
        {
            return STATUS_ERROR;
        }
    }

    return STATUS_SUCCESS;
}

/* This function prints content of all status registers. */
void printSbcStatus()
{
    fs65_status_t status = fs65StatusOk;
    fs65_rx_data_t sbcStatus[FS65_STAT_NUM];
    uint8_t i;

    status = sbcGetStatus(sbcStatus);
    if (status != fs65StatusOk)
    {
        PRINTF("FS65_GetStatus() failed with error code %d.\r\n", status);
    }
    else
    {
        for (i = 0; i < FS65_STAT_NUM; i++)
        {
            PRINTF(" %-16s\t0x%02X\t[%s]\r\n", sbcStatNameStr[i], sbcStatus[i].readData,
                    uint8ToBinary(hpBuff, sbcStatus[i].readData));
        }
        PRINTF("\r\n");
    }
}

/* Uses FS65 voltage reference for MCU ADC calibration. */
static status_t adcCalibration(uint16_t adcResolution, float* lsbSize)
{
    fs65_status_t status;
    uint16_t adcRawValue;

    DEV_ASSERT(lsbSize != NULL);

    status = FS65_SwitchAMUXchannel(fs65AmuxVref);
    if (status != fs65StatusOk)
    {
        PRINTF("\n\r>>> Unable to switch AMUX channel.\n\r");
        return STATUS_ERROR;
    }
    /* Settling time (from 10 % to 90 % of VDDIO). */
    MCU_WaitUs(10);
    runAmuxConversion(&adcRawValue);
    *lsbSize = FS65_V_REF / (float) adcRawValue;

    return STATUS_SUCCESS;
}

/* Get FS65 supply voltage (3.3V or 5V). */
static status_t getSupplyVoltage(fs65_supply_voltage_t* voltage)
{
    fs65_status_t status;
    fs65_rx_data_t response;

    DEV_ASSERT(voltage != NULL);

    /* Read-out HW_CONFIG register to get reference voltage. */
    status = FS65_ReadRegister(FS65_M_HW_CONFIG_ADDR, &response);
    if (status != fs65StatusOk)
    {
        return STATUS_ERROR;
    }

    if (response.readData & FS65_R_M_VCCA_HW_MASK) /* VCCA is 5V. */
    {
        *voltage = fs65SupplyVoltage5V;
    }
    else
    {
        *voltage = fs65SupplyVoltage3V3;
    }
    return STATUS_SUCCESS;
}

/* Conversion function for *wide values read from the FS65 AMUX pin.
 * Note: Wide voltage range is from 3.0 to 19.0 V. */
static float convertWide(float amuxVoltage)
{
    fs65_supply_voltage_t voltage;

    voltage = getSupplyVoltage(&voltage);

    switch (voltage)
    {
        case (fs65SupplyVoltage5V):
            return amuxVoltage * FS65_V_AMUX_WD_5;
            break;

        case (fs65SupplyVoltage3V3):
           return amuxVoltage * FS65_V_AMUX_WD_3P3;
            break;
    }
    return -1;
}

/* Conversion function for *tight values read from the FS65 AMUX pin.
 * Note: Tight voltage range is from 3.0 to 9.0 V. */
static float convertTight(float amuxVoltage)
{
    fs65_supply_voltage_t voltage;

    voltage = getSupplyVoltage(&voltage);

    switch (voltage)
    {
        case (fs65SupplyVoltage5V):
            return amuxVoltage * FS65_V_AMUX_TG_5;
            break;

        case (fs65SupplyVoltage3V3):
            return amuxVoltage * FS65_V_AMUX_TG_3P3;
            break;
    }
    return -1;
}

/* Conversion function for FS65 die temperature calculation. */
static float convertTemp(float number)
{
    return ((number - FS65_V_AMUX_TP) * 1000 / FS65_V_TEMP_COEFF) + 165;
}

/* Prints AMUX measurements in human readable form to serial console. */
void printAmuxMeasurement()
{
    fs65_status_t status;
    uint16_t i;
    amux_meas_t* actualMeas;
    float lsbSize;
    uint16_t adcResolution;
    uint16_t adcRawValue;

    static amux_meas_t measurements[FS65_AMUX_NUM_OF_CHANNELS] =
    {
        {fs65AmuxVsnsWide, "Vsns wide", convertWide},
        {fs65AmuxIO_0Wide,"IO_0 wide", convertWide},
        {fs65AmuxIO_5Wide,"IO_5 wide", convertWide},
        {fs65AmuxVsnsTight,"Vsns tight", convertTight},
        {fs65AmuxIO_0Tight,"IO_0 tight", convertTight},
        {fs65AmuxIO_5TightDivVkam,"IO_5 tight / Vkam", convertTight},
        {fs65AmuxDieTempSensor,"Die Temperature", convertTemp, "degC"},
    };

    status = getAdcResolution(&adcResolution);
    status |= adcCalibration(adcResolution, &lsbSize);
    if (status != fs65StatusOk)
    {
        PRINTF("\n\r*** ADC calibration error.\n\r");
        return;
    }

    PRINTF("\n\r**************** AMUX measurements ****************\n\r");

    for (i = 0; i < FS65_AMUX_NUM_OF_CHANNELS; i++)
    {
        actualMeas = &(measurements[i]);
        status = FS65_SwitchAMUXchannel(actualMeas->amuxSelection);
        if (status != fs65StatusOk)
        {
            PRINTF("\n\r*** Unable to switch AMUX channel.\n\r");
            return;
        }
        /* Read AMUX pin value. */
        runAmuxConversion(&adcRawValue);
        /* Process the result to get the value in volts */
        actualMeas->value = lsbSize * (float) adcRawValue;

        /* Apply conversion function if defined. */
        if (actualMeas->conversionFunc != NULL)
        {
            actualMeas->value = actualMeas->conversionFunc(actualMeas->value);
        }
        if (actualMeas->unit == NULL)
        {
            actualMeas->unit = "V";
        }
        PRINTF("%-10s: %5.3f %s\n\r", actualMeas->text, actualMeas->value, actualMeas->unit);
    }
    PRINTF("\n\r**************************************************\n\r");
}

/* Test of the LDT timer. Uses LDT Function 3
 * (sleep and wake-up). */
status_t sbcTestLdt(void)
{
    fs65_status_t status;

    status = FS65_LDT_RunCounter(false);
    status |= FS65_LDT_SetTimerOperation(fs65LdtFunc3);
    status |= FS65_LDT_SetTimerMode(fs65LdtModeNormal);
    status |= FS65_LDT_SetAfterRunValue(SBC_LDT_AFTER_RUN_VALUE);
    status |= FS65_LDT_SetWakeUpValue(SBC_LDT_WAKE_UP_VALUE);

    status |= FS65_LDT_RunCounter(true);

    if (status != fs65StatusOk)
    {
        return STATUS_ERROR;
    }
    return STATUS_SUCCESS;
}
