/*
 * Safety_HardFaultHandler.h
 *
 *  Created on: May 7, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_HARDFAULTHANDLER_H_
#define SAFETYLIB_SAFETY_HARDFAULTHANDLER_H_

#include "cpu.h"

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

#if defined(MCU_S32K116)||defined(MCU_S32K118)
    #define LDR_INST_WIDTH 2 /*S32K11x uses CM0+, LDR is a 2 Byte(16-bit) instruction*/
#else
    #define LDR_INST_WIDTH 2 /*S32K14x uses CM4F, LDR is a 4 Byte(32-bit) instruction*/
#endif

/** \brief  Get Main Stack Pointer

    This function returns the current value of the Main Stack Pointer (MSP).

    \return               MSP Register value
 */
#define __ASM            __asm
#define __STATIC_INLINE  static inline
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t __get_MSP(void)
{
  register uint32_t result;

  __ASM volatile ("MRS %0, msp\n" : "=r" (result) );
  return(result);
}

/** \brief  Get Process Stack Pointer

    This function returns the current value of the Process Stack Pointer (PSP).

    \return               PSP Register value
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t __get_PSP(void)
{
  register uint32_t result;

  __ASM volatile ("MRS %0, psp\n"  : "=r" (result) );
  return(result);
}

void HardFault_Handler(void);
//void HardFault_Handler_C(uint32_t *hardFaultArgs);

#endif /* SAFETYLIB_SAFETY_HARDFAULTHANDLER_H_ */
