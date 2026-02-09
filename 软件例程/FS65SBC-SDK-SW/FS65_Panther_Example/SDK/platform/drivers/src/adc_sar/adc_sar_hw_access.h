/*
 * Copyright 2017 NXP
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

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * Function-like macros are used to calculate bit positions based on channel index
 *
 */

#ifndef ADC_SAR_HW_ACCESS_H
#define ADC_SAR_HW_ACCESS_H

/*! @file */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macro defines a bitmask used to access status flags.
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "device_registers.h"

/*!
 * @defgroup adc_sar_group ADC SAR Hardware Access
 * @brief ADC SAR Group
 * @{
 */

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Define the hardware register size when calculating bit positions */
#define ADC_HW_REG_SIZE (32u)

/* Definitions to compute bit positions from channel index */
#define CHAN_2_VECT(CHNIDX) ((CHNIDX) / ADC_HW_REG_SIZE)
#define CHAN_2_BIT(CHNIDX)  ((CHNIDX) % ADC_HW_REG_SIZE)

/* Define ADC Channel Groups */
#define ADC_GROUP_0 (0u)
#define ADC_GROUP_1  (1u)
#if ((ADC_CIMR_COUNT > 2u) || (ADC_CTR_COUNT > 2u))
/* Define the group only if it exists in hardware */
#define ADC_GROUP_2  (2u)
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline void ADC_Powerup(ADC_Type * const base)
{
    REG_BIT_CLEAR32(&(base->MCR), ADC_MCR_PWDN(1u));
}


static inline void ADC_Powerdown(ADC_Type * const base)
{
    REG_BIT_SET32(&(base->MCR), ADC_MCR_PWDN(1u));
}

/*FUNCTION*********************************************************************
 *
 * Function Name : ADC_WriteThresholds
 * Description   :
 *
 *END*************************************************************************/
static inline void ADC_WriteThresholds(ADC_Type * const base, uint8_t registerNumber, uint32_t value)
{
	switch (registerNumber)
	{
		case 0:
			base->THRHLR0 = value;
			break;
		case 1:
			base->THRHLR1 = value;
			break;
		case 2:
			base->THRHLR2 = value;
			break;
		case 3:
			base->THRHLR3 = value;
			break;
		case 4:
			base->THRHLR4 = value;
			break;
		case 5:
			base->THRHLR5 = value;
			break;
#if (ADC_THRHLR_COUNT > 6u)
		case 6:
			base->THRHLR6 = value;
			break;
		case 7:
			base->THRHLR7 = value;
			break;
		case 8:
			base->THRHLR8 = value;
			break;
		case 9:
			base->THRHLR9 = value;
			break;
		case 10:
			base->THRHLR10 = value;
			break;
		case 11:
			base->THRHLR11 = value;
			break;
		case 12:
			base->THRHLR12 = value;
			break;
		case 13:
			base->THRHLR13 = value;
			break;
		case 14:
			base->THRHLR14 = value;
			break;
		case 15:
			base->THRHLR15 = value;
			break;
#endif
		default:
			/* no-op */
		    break;
	}
}

/*FUNCTION*********************************************************************
 *
 * Function Name : ADC_WriteChannelMapping
 * Description   :
 *
 *END*************************************************************************/
static inline void ADC_WriteChannelMapping(ADC_Type * const base, uint32_t registerNumber,uint32_t fieldPosition, uint32_t value)
{
	switch (fieldPosition)
	{
		case 0:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH0_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH0(value);
			break;
		case 1:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH1_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH1(value);
			break;
		case 2:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH2_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH2(value);
			break;
		case 3:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH3_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH3(value);
			break;
		case 4:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH4_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH4(value);
			break;
		case 5:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH5_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH5(value);
			break;
		case 6:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH6_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH6(value);
			break;
		case 7:
			base->CWSELR[registerNumber] &= ~ADC_CWSELR_WSEL_CH7_MASK;
			base->CWSELR[registerNumber] |= ADC_CWSELR_WSEL_CH7(value);
			break;
		default:
			/* no-op */
			break;
	}
}

#if defined (__cplusplus)
extern "C" {
#endif

#if defined (__cplusplus)
}
#endif

/*! @} */

#endif /* ADC_SAR_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
