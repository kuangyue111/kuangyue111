/******************************************************************************
*  
*
* (c) Copyright 2001-2004 Motorola, Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
*
******************************************************************************/


#include "m4_scst_compiler.h"

    /* Symbols defined outside but used within current module */
    SCST_EXTERN m4_scst_fault_inject_value
    
    /* Symbols defined in the current module but to be visible to outside */
    SCST_EXPORT SCST_RAM_TARGET0
    SCST_EXPORT SCST_RAM_TARGET1
    SCST_EXPORT SCST_RAM_FETCH_TEST_CODE
    SCST_EXPORT m4_scst_spr_registers_dump
    SCST_EXPORT m4_scst_link_register_dump
    SCST_EXPORT m4_scst_scb_registers_dump
    SCST_EXPORT m4_scst_special_condition_flag
    SCST_EXPORT m4_scst_accumulated_signature
    SCST_EXPORT m4_scst_test_tail_end
    SCST_EXPORT m4_scst_clear_flags
    SCST_EXPORT m4_scst_check_flags_cleared
    SCST_EXPORT m4_scst_check_q_flag
    SCST_EXPORT m4_scst_isr_return_address

    SCST_EXPORT VAL1
    SCST_EXPORT VAL2
    SCST_EXPORT VAL3
    SCST_EXPORT VAL4
    SCST_EXPORT VAL5
    SCST_EXPORT VAL6
    SCST_EXPORT VAL7
    SCST_EXPORT VAL8
    SCST_EXPORT VAL9
    SCST_EXPORT VAL10
    SCST_EXPORT VAL11
    SCST_EXPORT VAL12

    /* Data patterns definition for short fault detection within load/store module */

SCST_DEFINE(VAL1, 0x55555555)  /* <data_to_store>: Short fault pattern #1 */
SCST_DEFINE(VAL2, 0xAAAAAAAA)  /* <data_to_store>: Short fault pattern #1 */
SCST_DEFINE(VAL3, 0xCCCCCCCC)  /* <data_to_store>: Short fault pattern #2 */
SCST_DEFINE(VAL4, 0x33333333)  /* <data_to_store>: Short fault pattern #2 */
SCST_DEFINE(VAL5, 0x99999999)  /* <data_to_store>: Short fault pattern #3 */
SCST_DEFINE(VAL6, 0x66666666)  /* <data_to_store>: Short fault pattern #3 */
SCST_DEFINE(VAL7, 0x77777777)  /* <data_to_store>: Short fault pattern #4 */
SCST_DEFINE(VAL8, 0xEEEEEEEE)  /* <data_to_store>: Short fault pattern #5 */
SCST_DEFINE(VAL9, 0x13579BDF)  /* <data_to_store>: Short fault pattern #6 */
SCST_DEFINE(VAL10, 0xFDB97531)  /* <data_to_store>: Short fault pattern #7 */
SCST_DEFINE(VAL11, 0x2468ACE0)  /* <data_to_store>: Short fault pattern #8 */
SCST_DEFINE(VAL12, 0x0ECA8642)  /* <data_to_store>: Short fault pattern #9 */

    SCST_SECTION_DATA_UNINIT_ALIGN(m4_scst_ram_data_target0, SCST_BYTES_4)
SCST_RAM_TARGET0:
    SCST_ALLOC_BYTES(52)

    SCST_SECTION_DATA_UNINIT_ALIGN(m4_scst_ram_data_target1, SCST_BYTES_4)
SCST_RAM_TARGET1:
    SCST_ALLOC_BYTES(52)
    
    SCST_SECTION_EXEC_W_ALIGN(m4_scst_ram_test_code, SCST_BYTES_4)
SCST_RAM_FETCH_TEST_CODE:
    SCST_ALLOC_BYTES(24)


    SCST_SECTION_DATA(m4_scst_ram_data)
m4_scst_spr_registers_dump:
    SCST_ALLOC_BYTES(28)
        /* 7 registers (PSP(or MSP),R9, R10, R11, R12, CONTROL and MSP) x 4 bytes */
        
m4_scst_scb_registers_dump:
    SCST_ALLOC_BYTES(24)
        /* 3 registers (SHPR1, SHPR2, SHPR3) x 4 bytes
               + SHCSR register
               + CCR register
               + BASEPRI */
               
m4_scst_special_condition_flag:
    SCST_ALLOC_BYTES(4)
    
m4_scst_link_register_dump:
    SCST_ALLOC_BYTES(4)
    
m4_scst_isr_return_address:
    SCST_ALLOC_BYTES(4)
    
    SCST_SECTION_DATA(m4_scst_test_shell_data)
m4_scst_accumulated_signature:
    SCST_ALLOC_BYTES(4)
    
    
    SCST_SECTION_EXEC_ALIGN(m4_scst_test_code_unprivileged, SCST_BYTES_4)
    SCST_THUMB2

/*--------------------------------------------------------------------*/
    SCST_TYPE(m4_scst_clear_flags, function)
    SCST_TYPE(m4_scst_check_flags_cleared, function)
    SCST_TYPE(m4_scst_check_q_flag, function)

m4_scst_clear_flags:
    MOV     R4,#0x0
    MSR     APSR_nzcvq,R4     /* Clear flags */
    ISB
m4_scst_check_flags_cleared:
    MRS     R4,APSR     /* Load flags again */
    LSR     R4,R4,#27   /* We need to shift because GE bits are not cleared */
    CBNZ    R4,m4_scst_flags_not_cleared
    BX      LR
m4_scst_flags_not_cleared:
    B       m4_scst_test_tail_end
    
m4_scst_check_q_flag:
    MRS    R4,APSR
    TST    R4,#(1<<27)
    BX     LR
    
    SCST_TYPE(m4_scst_test_tail_end, function)
m4_scst_test_tail_end:
    
    /* Apply the fault injection */
    LDR     R1,=m4_scst_fault_inject_value
    LDR     R3,[R1]
    EOR     R0,R0,R3
    
    /* Accumulate test result into global variable */
    LDR     R1,=m4_scst_accumulated_signature
    LDR     R3,[R1]
    EOR     R3,R3,R0
    STR     R3,[R1]
    
    POP.W   {R1-R12,R14}
    /* Return from test */
    BX      LR

    SCST_ALIGN_BYTES_4
    SCST_LTORG  /* Marks the current location for dumping psuedoinstruction pools (literal pool)
                   (containing numeric values for used symbolic names used within LDR instruction).
                   It is 4-byte aligned, as 2-byte alignment causes incorrect work. */
    SCST_FILE_END

