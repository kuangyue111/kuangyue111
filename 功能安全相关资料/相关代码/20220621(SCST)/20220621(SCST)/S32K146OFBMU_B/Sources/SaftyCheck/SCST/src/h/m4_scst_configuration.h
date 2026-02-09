/******************************************************************************
*  
*
* (c) Copyright 2001-2004 Motorola, Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
*
******************************************************************************/


#ifndef M4_SCST_CONFIGURATION_H
#define M4_SCST_CONFIGURATION_H

/******************************************************************************
*  Definitions modifiable by a user
******************************************************************************/
/* Define whether SysTick timer is running or not, where:
   1U - SysTick timer is used by user application or operating system
   0U - SysTick timer is not running
*/
#define M4_SCST_SYSTICK_IS_RUNNING          1U

/**********************************************************************************
* Definitions related to single-core / multi-core execution mode
**********************************************************************************/

#define M4_SCST_TESTS_NUM   44U

/* Nested Vector Interrupt (NVIC) registers */
#define M4_ICTR_REG     0xE000E004
#define M4_NVIC_ISER_REG    0xE000E100
#define M4_NVIC_ICER_REG    0xE000E180

/* System Control Block (SCB) registers  */
#define M4_ICSR_REG     0xE000ED04
#define M4_VTOR_REG     0xE000ED08
#define M4_CCR_REG      0xE000ED14
#define M4_SHPR1_REG    0xE000ED18
#define M4_SHPR2_REG    0xE000ED1C
#define M4_SHPR3_REG    0xE000ED20
#define M4_SHCSR_REG    0xE000ED24
#define M4_CFSR_REG     0xE000ED28
#define M4_MMFSR_REG    0xE000ED28
#define M4_BFSR_REG     0xE000ED29
#define M4_UFSR_REG     0xE000ED2A
#define M4_HFSR_REG     0xE000ED2C
#define M4_BFAR_REG     0xE000ED38


#endif /* M4_SCST_CONFIGURATION_H */
