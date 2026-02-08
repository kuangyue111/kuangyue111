/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : McuSelfCheck.h
** @Founder      : ZSY342
** @Date         : 2019.10.1
** @Function     : MCU自检模块
** @Instructions :
**===========================================================================================*/
#ifndef _MCUSELFCHECK_H
#define _MCUSELFCHECK_H

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"

/*=============================================================================================
** @Macro definition
**===========================================================================================*/
#define RCM_SRS_CMULOC_MASK	0x10

#define	SELFTEST_PASS_WDOG		0x00000001
#define	SELFTEST_PASS_FLASH_ECC	0x00000002
#define	SELFTEST_PASS_SRAML_ECC	0x00000004
#define SELFTEST_PASS_SRAMU_ECC	0x00000008
#define	SELFTEST_PASS_MPU_INJ	0x00000010
#define	SELFTEST_PASS_XOSC		0x00000020
#define	SELFTEST_PASS_SPLL		0x00000040
#define	SELFTEST_PASS_ADC0		0x00000080
#define	SELFTEST_PASS_ADC1		0x00000100
#define	SELFTEST_PASS_CRC		0x00000200
#define	SELFTEST_PASS_SCST		0x00000400

#define SELFTEST_PASS_ALL		0x000007FF

/*****************************************************************************************************************
 * the linker register(R14/LR) is the EXC_RETURN[31:0] to decide interrupt/exception return state machine
 * the following macro definitions are used for HardFault handle
 ****************************************************************************************************************/
/*return to Thread mode, exception return use float-point state from PSP and execution uses PSP after return*/
#define Re2TM_from_FPS_PSP_to_use_PSP  0xFFFFFFED

/*return to Thread mode, exception return use non-float-point state from PSP and execution uses PSP after return*/
#define Re2TM_from_nFPS_PSP_to_use_PSP 0xFFFFFFFD

/*return to Handler mode, exception return use float-point state from MSP and execution uses MSP after return*/
#define Re2HM_from_FPS_MSP_to_use_MSP  0xFFFFFFE1

/*return to Thread mode, exception return use float-point state from MSP and execution uses MSP after return*/
#define Re2TM_from_FPS_MSP_to_use_MSP  0xFFFFFFE9

/*return to Handler mode, exception return use non-float-point state from MSP and execution uses MSP after return*/
#define Re2HM_from_nFPS_MSP_to_use_MSP 0xFFFFFFF1

/*return to Thread mode, exception return use non-float-point state from MSP and execution uses MSP after return*/
#define Re2TM_from_nFPS_MSP_to_use_MSP 0xFFFFFFF9

#define LDR_INST_WIDTH 2 /*S32K14x uses CM4F, LDR is a 4 Byte(32-bit) instruction*/

/*=============================================================================================
** @Data type definition
**===========================================================================================*/
typedef enum
{
	STD_OFF = 0,
	STD_ON,
}STD_Type;

/** \brief  Get Main Stack Pointer

    This function returns the current value of the Main Stack Pointer (MSP).

    \return               MSP Register value
 */
#define __ASM            __asm
#define __STATIC_INLINE  static inline
__attribute__( ( always_inline ) ) __STATIC_INLINE u32 __get_MSP(void)
{
  register u32 result;

  __ASM volatile ("MRS %0, msp\n" : "=r" (result) );
  return(result);
}

/** \brief  Get Process Stack Pointer

    This function returns the current value of the Process Stack Pointer (PSP).

    \return               PSP Register value
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE u32 __get_PSP(void)
{
  register u32 result;

  __ASM volatile ("MRS %0, psp\n"  : "=r" (result) );
  return(result);
}

typedef enum
{
	MCU_SELF_CHECK_Reg 		= 0,
	MCU_SELF_CHECK_Clock 	= 1,
	MCU_SELF_CHECK_Nvic 	= 2,
	MCU_SELF_CHECK_Wdog 	= 3,/*使用外部硬件狗*/
	MCU_SELF_CHECK_Adc 		= 4,
	MCU_SELF_CHECK_Flash 	= 5,
	MCU_SELF_CHECK_Ram 		= 6,
	MCU_SELF_CHECK_Gpio 	= 7,
	MCU_SELF_CHECK_Comm 	= 8,/*原理同GPIO*/
	MCU_SELF_CHECK_Crc 		= 9,

	MCU_SELF_CHECK_NUM,
}McuSelfCheckResult;


/*=============================================================================================
** @Interface function declaration
**===========================================================================================*/
/*=============================================================================================
** @Name      : void McuSelfCheckInit(void)
** @Input     : void
** @Output    : void
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
void McuSelfCheckInit(void);

/*=============================================================================================
** @Name      : void TestMcuFaultTask(void)
** @Input     : void
** @Output    : MCU自检检测任务
** @Function  :
** @The notes : 上电只检测一次
**===========================================================================================*/
void TestMcuFaultTask(void);

/*=============================================================================================
** @Name      : u8 GetMcuSelfCheckFaultFlagAPI(void)
** @Input     : void
** @Output    : MCU自检检测结果
** @Function  : 获取MCU自检检测结果
** @The notes :
**===========================================================================================*/
u8 GetMcuSelfCheckFaultFlagAPI(void);

#endif
