/******************************************************************************
*
* Freescale Confidential Proprietary
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* (c) Copyright 2001-2004 Motorola, Inc.
* ALL RIGHTS RESERVED.
* @remarks Implements DES_ICST_A010
******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "m4_scst_typedefs.h"
#include "m4_scst_test_shell.h"
#include "m4_scst_data.h"
#include "m4_scst_configuration.h"
#include "m4_scst_compiler.h"

/******************************************************************************
* External objects
******************************************************************************/
extern const m4_scst_atomic_test_icst_DESC_t m4_scst_atomic_test_desc[M4_SCST_TESTS_NUM];

/******************************************************************************
* Global variables
******************************************************************************/
/* This variable contains the number of the most recently executed atomic test */
SCST_PLACE_IN_SECTION(m4_scst_test_shell_data) m4_scst_uint32_t m4_scst_last_executed_test_number = 0xFFFFFFFFUL;
SCST_PLACE_IN_SECTION(m4_scst_test_shell_data) m4_scst_uint32_t m4_scst_fault_inject_value = 0x00000000UL;
SCST_PLACE_IN_SECTION(m4_scst_test_shell_data) m4_scst_vuint32_t m4_scst_test_was_interrupted = 0x00000000UL;
SCST_PLACE_IN_SECTION(m4_scst_test_shell_data) m4_scst_uint32_t m4_scst_fault_inject_test_index = M4_SCST_DEFAULT_FAULT_INJECT_TEST_INDEX;

/******************************************************************************
* Function implementation
******************************************************************************/
SCST_PLACE_IN_SECTION(m4_scst_test_shell_code)

m4_scst_uint32_t m4_scst_execute_core_tests(m4_scst_uint32_t start, m4_scst_uint32_t end)
{
m4_scst_uint32_t i;
m4_scst_uint32_t atomic_test_result;
m4_scst_uint32_t converted_atomic_test_result;
m4_scst_uint32_t (* atomic_test_start_address) (void);
volatile m4_scst_uint32_t icst_cfm;
m4_scst_uint32_t return_value;

    icst_cfm = 0xD68B3EC4U;
    
    m4_scst_fault_inject_value = 0U;
    m4_scst_accumulated_signature = 0U;
    m4_scst_test_was_interrupted = 0U;
    
    return_value = 0U;

    if ((end < start) || (end >= M4_SCST_TESTS_NUM))
    {
        return_value = M4_SCST_WRONG_RANGE;
    }
    else
    {
        i = start;

        while (i <= end)
        {
            m4_scst_last_executed_test_number = i;

            atomic_test_start_address = m4_scst_atomic_test_desc[i].atomic_test_start_address;

            if (icst_cfm != 0xD68B3EC4U)
            {
                __asm("UDF #0x0");
            }
            else
            {
                if (i == m4_scst_fault_inject_test_index)
                {
                    m4_scst_fault_inject_value = M4_SCST_FAULT_INJECT;
                    m4_scst_fault_inject_test_index = M4_SCST_DEFAULT_FAULT_INJECT_TEST_INDEX;
                }
                /* Execute the current core test and retrieve its result... */
                atomic_test_result = atomic_test_start_address();
                /* Check whether the test was not interrupted...
                (Some of the test do not depend on their interruption. When they are executed, the user interrupt vector table
                is not replaced with the core self test specific one, so the scst_test_was_interrupted flag will remain 0 for those tests. */

                if (m4_scst_test_was_interrupted != 0U)
                {
                    return_value = M4_SCST_TEST_WAS_INTERRUPTED;
                    m4_scst_accumulated_signature = M4_SCST_TEST_WAS_INTERRUPTED;                    
                    break;
                }

                /* Check whether the result of the executed test is correct or not... */

                if ((atomic_test_result & 0x80000000U) != 0U)
                {
                    converted_atomic_test_result = ((atomic_test_result << 1U) | 0x00000001U);
                }
                else
                {
                    converted_atomic_test_result = (atomic_test_result << 1U);
                }
                converted_atomic_test_result ^= atomic_test_result;

                if (converted_atomic_test_result != m4_scst_atomic_test_desc[i].expected_signature)
                {
                    return_value = atomic_test_result;
                    break;
                }
                else
                {
                    return_value ^= atomic_test_result;
                }

                i ++;

            } /* else / if (icst_cfm != 0xD68B3EC4U) */

        } /* while (i <= end) */

    } /* else / if ((end < start) || (end >= SCST_TESTS_NUM)) */


    icst_cfm = 0x0U;

    return return_value;
}
