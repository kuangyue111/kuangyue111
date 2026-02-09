/******************************************************************************
*  
*
* (c) Copyright 2001-2004 Motorola, Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
*
******************************************************************************/


#include "m4_scst_configuration.h"
#include "m4_scst_compiler.h"


    /* Symbols defined in the current module but to be visible to outside */
    SCST_EXPORT m4_scst_exception_test_tail_end 
    SCST_EXPORT m4_scst_exception_test_tail
    SCST_EXPORT m4_scst_store_registers_content
    SCST_EXPORT m4_scst_restore_dedicated_registers_content
    SCST_EXPORT m4_scst_restore_registers_content
    SCST_EXPORT m4_scst_set_scst_interrupt_vector_table
    
    /* Symbols defined outside but used within current module */
    SCST_EXTERN m4_scst_user_VTOR
    SCST_EXTERN m4_scst_accumulated_signature
    SCST_EXTERN m4_scst_fault_inject_value
    SCST_EXTERN m4_scst_pass_control_to_user_interrupt
    SCST_EXTERN m4_scst_set_flag_test_was_interrupted
    SCST_EXTERN m4_scst_spr_registers_dump
    SCST_EXTERN m4_scst_scb_registers_dump
    SCST_EXTERN m4_scst_VTABLE
    
    
    SCST_SECTION_EXEC_ALIGN(m4_scst_test_code,SCST_BYTES_4)
    SCST_THUMB2

/*--------------------------------------------------------------------*/
    SCST_TYPE(m4_scst_store_registers_content, function)
m4_scst_store_registers_content:
    
    /* If unexpected interrupt will be taken, the m4_scst_ISR_dispatcher() would */
    /* try to restore content of SP,R9-R12. */
    LDR     R1,=m4_scst_spr_registers_dump
    MOV     R2,SP
    STMIA   R1,{R2,R9-R12}
    
    /* If unexpected interrupt will be taken, the m4_scst_ISR_dispatcher would */
    /* try to restore content of SCB registers which could be modified by this test. */
    LDR     R1,=m4_scst_scb_registers_dump
    
    LDR     R3,=M4_CCR_REG  /* Load CCR(SCB) register address */
    LDR     R4,[R3]         /* Load current CCR register value */
    STR     R4,[R1,#16]     /* Dump content of CCR register to the buffer */
    
    LDR     R3,=M4_SHCSR_REG    /* Load SHCSR(SCB) register address */
    LDR     R4,[R3]         /* Load current SHCSR register value */
    STR     R4,[R1,#12]     /* Dump content of SHCSR register to the buffer */
    
    /* Save current content of SHPR1 register and clear their content to unmask
       exceptions with configurable priorities possibly masked by the BASEPRI value.
       The BASEPRI register is not cleared! It prevents activation of all alien
       interrupts with the same or lower priority level. */
    
    LDR     R3,=M4_SHPR1_REG
    LDR     R4,[R3]         /* Load SHPR1 register */
    STR     R4,[R1]         /* Dump SHPR1 register to the buffer */
    LDR     R4,[R3,#4]      /* Load SHPR2 register */
    STR     R4,[R1,#4]      /* Dump SHPR2 register to the buffer */
    LDR     R4,[R3,#8]      /* Load SHPR3 register */
    STR     R4,[R1,#8]      /* Dump SHPR3 register to the buffer */
    
    MRS     R4,BASEPRI
    STR     R4,[R1,#20]
    
    BX      LR
    
/*--------------------------------------------------------------------*/
    SCST_TYPE(m4_scst_set_scst_interrupt_vector_table, function)
m4_scst_set_scst_interrupt_vector_table:
    /* Disable interrupts */
    CPSID   i
    MRS     R4,CONTROL
    /* Switch stack to MSP */ 
    MSR     CONTROL,R2  /* R2 is always initialized to 0 at the beginning of test - this ensures that MSP will be used */
    LDR     R3,=m4_scst_spr_registers_dump
    STR     R4,[R3,#24]
    MOV     R4,SP
    STR     R4,[R3,#20] /* Store MSP */
    
    /* Replace user interrupt vector table with the SCST-specific one */
    LDR     R3,=M4_VTOR_REG
    LDR     R7,[R3]         /* Load current (user) content of VTOR to R7 */
    LDR     R4,=m4_scst_user_VTOR
    STR     R7,[R4]         /* Store R7 to m4_scst_user_VTOR variable */
    LDR     R7,=m4_scst_VTABLE
    STR     R7,[R3]         /* Store m4_scst_VTABLE address to the VTOR register */
    /* Restore interrupts from R1 which contains original content of the PRIMASK register !! */
    MSR     PRIMASK,R1

    BX      LR
    
/*--------------------------------------------------------------------*/
    SCST_TYPE(m4_scst_restore_dedicated_registers_content, function)
m4_scst_restore_dedicated_registers_content:
    /* Restore registers R9-R12. */
    LDR     R3,=m4_scst_spr_registers_dump
    LDR     R9,[R3,#4]
    LDR     R10,[R3,#8]
    LDR     R11,[R3,#12]
    LDR     R12,[R3,#16]
    
    LDR     R1,=m4_scst_scb_registers_dump
    
    LDR     R3,=M4_CCR_REG      /* Restore CCR */
    LDR     R2,[R1,#16]         /* Load CCR register value from the buffer */
    STR     R2,[R3]             /* Restore user content of CCR from R2 */    
    
    LDR     R3,=M4_SHPR1_REG    /* Restore SHPR1, SHPR2, SHPR3 */
    LDR     R2,[R1]             /* Load SHPR1 register value from the buffer */
    STR     R2,[R3]             /* Restore user content of SHPR1 from R2 */
    LDR     R2,[R1,#4]          /* Load SHPR2 register from the buffer */
    STR     R2,[R3,#4]          /* Restore SHPR2 register */
    LDR     R2,[R1,#8]          /* Load SHPR3 register from the buffer */
    STR     R2,[R3,#8]          /* Restore SHPR3 register */
    
    LDR     R2,[R1,#20]
    MSR     BASEPRI,R2          /* Restore initial content of BASEPRI from buffer */
    
    MOV     R3,#0x0
    MSR     CONTROL,R3          /* Ensure privileged mode */
    
    LDR     R3,=m4_scst_set_flag_test_was_interrupted
    BX      R3
    
/*--------------------------------------------------------------------*/
    SCST_TYPE(m4_scst_restore_registers_content, function)
m4_scst_restore_registers_content:
    /* Restore registers SP,R9-R12. */
    LDR     R3,=m4_scst_spr_registers_dump
    LDR     R9,[R3,#4]
    LDR     R10,[R3,#8]
    LDR     R11,[R3,#12]
    LDR     R12,[R3,#16]
    
    LDR     R7,=m4_scst_scb_registers_dump
    LDR     R3,=M4_CCR_REG      /* Restore CCR */
    LDR     R4,[R7,#16]         /* Load CCR register value from the buffer */
    STR     R4,[R3]             /* Restore user content of CCR from R4 */
    
    LDR     R3,=M4_SHPR1_REG    /* Restore SHPR1, SHPR2, SHPR3 */
    LDR     R4,[R7]             /* Load SHPR1 register value from the buffer */
    STR     R4,[R3]             /* Restore user content of SHPR1 from R4 */
    LDR     R4,[R7,#4]          /* Load SHPR2 register from the buffer */
    STR     R4,[R3,#4]          /* Restore SHPR2 register */
    LDR     R4,[R7,#8]          /* Load SHPR3 register from the buffer */
    STR     R4,[R3,#8]          /* Restore SHPR3 register */
    
    LDR     R4,[R7,#20]
    MSR     BASEPRI,R4          /* Restore initial content of BASEPRI from buffer */
    
    LDR     R3,=M4_SHCSR_REG    /* Restore SHCSR */
    LDR     R4,[R7,#12]         /* Load SHCSR register value from the buffer */
    LDR     R7,[R3]             /* Load current SHCSR value */
    BFI     R4,R7,#0,#16        /* Ensure current active status is not cleared */
    STR     R4,[R3]             /* Restore user content of SHCR from R4 */
    
    /* Restore user interrupt vector table */
    CPSID   i
    LDR     R3,=m4_scst_spr_registers_dump
    LDR     R13,[R3,#20]    /* Restore MSP */
    LDR     R4,[R3,#24]
    MSR     CONTROL,R4  /* Restore CONTROL register */
    
    LDR     R3,=m4_scst_user_VTOR
    LDR     R4,[R3]             /* Load R4 with content of user_VTOR variable */
    LDR     R3,=M4_VTOR_REG
    STR     R4,[R3]             /* Restore user content of VTOR from R4 */
    
    BX      LR
    
    
/*--------------------------------------------------------------------*/
    SCST_TYPE(m4_scst_exception_test_tail_end, function)
    SCST_TYPE(m4_scst_exception_test_tail, function)
m4_scst_exception_test_tail:
    /* Restore SHCSR REGISTER */
    LDR     R4,=m4_scst_scb_registers_dump
    LDR     R3,=M4_SHCSR_REG   
    LDR	    R2,[R4,#12]         /* Load SHCSR register value from the buffer */
    LDR     R4,[R3]             /* Load current SHCSR value */
    BFI     R2,R4,#0,#16        /* Ensure current active status is not cleared */
    STR     R2,[R3]             /* Restore user content of SHCR from R2 */

    /* Restore user interrupt vector table */
    CPSID   i
    LDR     R3,=m4_scst_spr_registers_dump
    LDR     R13,[R3,#20]    /* Restore MSP */
    LDR     R4,[R3,#24]
    MSR     CONTROL,R4      /* Restore CONTROL register */
    
    LDR     R3,=m4_scst_user_VTOR  
    LDR     R4,[R3]                /* Load R4 with content of user_VTOR variable */
    LDR     R3,=M4_VTOR_REG        
    STR     R4,[R3]                /* Restore user content of VTOR from R4 */
        
m4_scst_exception_test_tail_end:    
    /* Clear flags MMFSR, BFSR, UFSR flags */
    LDR     R3,=M4_CFSR_REG
    LDR     R4,[R3]
    STR     R4,[R3]

    MSR     PRIMASK,R1      /* Restore initial content of PRIMASK from R1 */
    MSR     FAULTMASK,R6    /* Restore initial content of FAULTMASK from R6 */
    
    /* Restore stack */
    LDR     R3,=m4_scst_spr_registers_dump
    LDR     R13,[R3]
    
    /* Apply the fault injection */
    LDR     R1,=m4_scst_fault_inject_value
    LDR     R3,[R1]
    EOR     R0,R0,R3
    
    /* Accumulate test result into global variable */
    LDR     R1,=m4_scst_accumulated_signature
    LDR     R3,[R1]
    EOR     R3,R3,R0
    STR     R3,[R1]
    
    POP.W   {R1-R8,R14}
    /* Return from test */
    BX      LR

    SCST_ALIGN_BYTES_4
    SCST_LTORG  /* Marks the current location for dumping psuedoinstruction pools (literal pool) 
                   (containing numeric values for used symbolic names used within LDR instruction).
                   It is 4-byte aligned, as 2-byte alignment causes incorrect work. */
    SCST_FILE_END

