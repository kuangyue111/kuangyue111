#ifndef __BSP_h__
#define __BSP_h__

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *  Includes
 */
#include "gd32f30x.h"
#include "configure.h"

/*******************************************************************************
 *  Exported Constants
 */
#define BSP_SW1_VALUE()					(gpio_input_bit_get(GPIOB, GPIO_PIN_12))
#define BSP_SW2_VALUE()					(gpio_input_bit_get(GPIOB, GPIO_PIN_13))
#define BSP_SW3_VALUE()					(gpio_input_bit_get(GPIOB, GPIO_PIN_14))
#define BSP_SW4_VALUE()					(gpio_input_bit_get(GPIOB, GPIO_PIN_15))

#define BSP_VRE0_VALUE() 				(gpio_input_bit_get(GPIOC, GPIO_PIN_13))
#define BSP_VRE1_VALUE() 				(gpio_input_bit_get(GPIOC, GPIO_PIN_14))

#define BSP_LED_ON()					(gpio_bit_reset(GPIOC, GPIO_PIN_7))
#define BSP_LED_OFF()					(gpio_bit_set(GPIOC, GPIO_PIN_7))
#define BSP_LED_TOGGLE()				(gpio_bit_write(GPIOC, GPIO_PIN_7, (bit_status)(1-gpio_output_bit_get(GPIOC, GPIO_PIN_7))))

#define BSP_I2C_WP_RESET()			    (gpio_bit_reset(GPIOA, GPIO_PIN_5))
#define BSP_I2C_WP_SET()				(gpio_bit_set(GPIOA, GPIO_PIN_5))
     
#define VOLT_INSO_VFIT_TIMES            (25)

/*******************************************************************************
 *  Exported Typedef
 */
typedef struct TCalibrationInfo
{
	INT8U step;
	INT8U Working;
	INT8U NeedReport;
	INT8U Result;

	float Ratio_K;
	float Ratio_B;

	float RealVolt_1;
	float RealVolt_2;

	float Volt_Comm1;
	float Volt_Comm2;
}
CalibrationInfo_t;

typedef struct TCanInfo
{
	INT8U transmit_mode;

	INT32U period;
}
CanInfo_t;


typedef struct THAL_data
{
	INT8U Addr;
	INT8U Hardware_Version;
	INT8U HostCanAddress;

	INT32U VoltFreq[2];

	float VoltSample_Value[2];
	float VoltSample_Value_jiaozhun[2];

	CalibrationInfo_t CalibrationInfo[2];
	CanInfo_t					CanInfo;
}
hal_data_t;

/*******************************************************************************
 *  Exported Macros
 */

/*******************************************************************************
 *  Exported Variables
 */

/*******************************************************************************
 *  Exported Functions
 */
void BSP_Init										(void);
void BSP_Task										(void);
void BSP_Addr_Task							(void);


#ifdef __cplusplus
}
#endif

#endif

///:~

