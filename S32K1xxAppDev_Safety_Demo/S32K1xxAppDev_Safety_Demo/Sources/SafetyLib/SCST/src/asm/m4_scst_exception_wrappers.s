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
    SCST_EXPORT m4_scst_ISR_dispatcher
    SCST_EXPORT m4_scst_ISR_address
    SCST_EXPORT m4_scst_user_VTOR
    SCST_EXPORT m4_scst_pass_control_to_user_interrupt
    SCST_EXPORT m4_scst_set_flag_test_was_interrupted
    
    
    /* Symbols defined outside but used within current module */
    SCST_EXTERN m4_scst_exception_test_tail
    SCST_EXTERN m4_scst_restore_dedicated_registers_content
    SCST_EXTERN m4_scst_special_condition_flag
    SCST_EXTERN m4_scst_test_was_interrupted
    SCST_EXTERN m4_scst_isr_return_address
    
    
    SCST_SECTION_DATA(m4_scst_ram_data)
m4_scst_user_VTOR:
    SCST_ALLOC_BYTES(4)
m4_scst_ISR_address:
    SCST_ALLOC_BYTES(4)


    SCST_SECTION_EXEC_ALIGN(m4_scst_exception_wrappers, SCST_BYTES_4)
    SCST_THUMB2

    /* The ".type" directive instructs the assembler/linker that the label m4_scst_ISR_dispatcher
       designates a function. In combination with the above specified ".thumb" directive,
       this would cause setting the least significant bit to '1' within any pointer
       (mainly within vector table) to this function,
       causing change to Thumb mode whenever this function is called. */

    SCST_TYPE(m4_scst_ISR_dispatcher, function)
m4_scst_ISR_dispatcher:
    /* 
       4 NOPs are used to clean up possibly active IT-block queue.
       This may happen if the dispatcher routines are entered in thread mode
       during interrupt "tail" processing.
       Valid for: 
           m4_scst_ISR_dispatcher
    */
    
    NOP
    NOP
    NOP
    NOP
    
    /*********************************************************************************************/
    /* We have to check whether there is an active interrupt !! */
    /*********************************************************************************************/
    MRS     R3,IPSR         /* IPSR contains IRQ number of the current interrupt */
    CMP     R3,#0           /* Check whether we are in the dispatcher but there is no interrupt pending */
    BEQ     m4_scst_exception_test_tail     /* No active interrupt => EXIT dispatcher !! */
    /*********************************************************************************************/
    
    /*********************************************************************************************/
    /* !! There is an active interrupt !! */
    /*********************************************************************************************/
    /* !!! We have to restore all critical registers or stacked values which were destroyed by a test !!! */
    ORR     LR,LR,#1        /* Correct the LR register bit[0] which could be destroyed by a test */
                            /* In normal situation this command has no effect */
                            
    LDR     R3,[SP,#28]     /* Load xPSR value from stack */
    ORR     R3,R3,#(1<<24)  /* Set Thumb bit which could be destroyed by a test */
    STR     R3,[SP,#28]     /* Rewrite xPSR on stack. In normal situation this command has no effect */
    /*********************************************************************************************/
    
    /*********************************************************************************************/
    /* We have to check whether we are interrupted for the first time */
    /*********************************************************************************************/
    LDR     R3,=m4_scst_test_was_interrupted 
    LDR     R1,[R3]                          
    CMP     R1,#0                            
    BNE     m4_scst_exit_dispatcher /* -> Exit dispatcher, we were already interrupted !! */
    
    /*********************************************************************************************/
    /* !!! We have to find SCST exception !!!                                                    */
    /*********************************************************************************************/
    /* Check ISR address */
    LDR     R1,[SP,#0]      /* Load stacked R0 register !!! */
    LDR     R3,[SP,#24]     /* Load stacked return address */
    SUB     R3,R3,R1
    CMP     R3,#4
    BGT     m4_scst_pass_control_to_alien_ISR
    
    /* Check ISR vector */
    LDR     R1,[SP,#8]      /* Load stacked R2 register */
    MRS     R3,IPSR         /* Load IPSR register */
    CMP     R1,R3
    BNE     m4_scst_pass_control_to_alien_ISR
    
    /* !!! SCST exception was recognized !!! */
    /* We have to update stacked R2 value */
    LDR     R1,[SP,#4]
    STR     R1,[SP,#8]
    
    /* We have to pass control to SCST ISR */
m4_scst_pass_control_to_scst_ISR:
    LDR     R1,=m4_scst_ISR_address
    LDR     R3,[R1]
    /*********************************************************************************************/
    BX      R3      /* -> Pass control to SCST ISR !! */
    /*********************************************************************************************/
    
    
    /*********************************************************************************************/
    /* We have to exit dispatcher since the test was already interrupted !! */
    /*********************************************************************************************/
m4_scst_exit_dispatcher:
    /*********************************************************************************************
       !! Caution !! 
       The SCST interrupt can be chained with an user interrupt, we have to find it 
       to avoid situation that SCST interrupt is serviced by the user interrupt handler !!
     *********************************************************************************************/
    /* Check ISR vector */
    MRS     R3,IPSR         /* Load IPSR register */
    CMP     R3,#0
    BEQ     m4_scst_exception_test_tail     /* -> Leave dispatcher, no active interrupt !! */
    
    /* There is still active interrupt !!*/
    LDR     R1,[SP,#8]      /* Load stacked R2 register */
    CMP     R1,R3
    /* We do not need to restore registers !! */
    BNE     m4_scst_pass_control_to_user_interrupt
    
    
    /* Check ISR address on stack */
    LDR     R1,[SP,#0]      /* Load stacked R0 register !!! */
    LDR     R3,=m4_scst_isr_return_address
    LDR     R3,[R3]
    SUB     R3,R3,R1
    CMP     R3,#4
    /* We do not need to restore registers !! */
    BGT     m4_scst_pass_control_to_user_interrupt
    
    
    /* !! We have recognized SCST interrupt !! */
    /* Change return address */
    LDR     R3,=m4_scst_exit_dispatcher
    ORR     R3,R3,#1
    STR     R3,[SP,#24]
    
    /* Change stacked R2 value */
    LDR     R1,[SP,#12] /* Load stacked R3 value */
    STR     R1,[SP,#8]  /* Rewrite stacked R2 value */
    DSB     /* Ensures that stacked value is updated before exception return */
    
    BX      LR  /* -> EXIT interrupt */
    
    
    /*********************************************************************************************/
    /* Alien interrupt was detected !!! */
    /*********************************************************************************************/
m4_scst_pass_control_to_alien_ISR:
    /* Restore registers */
    LDR     R3,=m4_scst_restore_dedicated_registers_content
    BX      R3
    
    SCST_TYPE(m4_scst_set_flag_test_was_interrupted, function)
m4_scst_set_flag_test_was_interrupted:
    /* Set flag to a non-zero value indicating that current core test was interrupted */
    /* This flag signals that registers were restored */
    LDR     R3,=m4_scst_test_was_interrupted
    STR     R3,[R3]
    
    /* Save ISR Return address from stack to variable !! */
    LDR     R1,=m4_scst_isr_return_address
    LDR     R3,[SP,#24]                   
    STR     R3,[R1]
    
    /* Update Stack ISR Return address -> m4_scst_ISR_dispatcher */
    ADR.W   R3,m4_scst_ISR_dispatcher
    ORR     R3,R3,#1    /* Set bit[0] */
    STR     R3,[SP,#24]

    SCST_TYPE(m4_scst_pass_control_to_user_interrupt, function)
m4_scst_pass_control_to_user_interrupt:
    LDR     R3,=m4_scst_user_VTOR
    LDR     R1,[R3]     /* Load R1 with content of user_VTOR variable */
    MRS     R3,IPSR     /* IPSR contains IRQ number of the current interrupt */
    LSL     R3,R3,#2    /* It has to be multiplied by 4 to get an offset address of the corresponding ISR. */
    ADD     R1,R3,R1    /* Add this offset to the base address of the user vector table. */
    LDR     R3,[R1]     /* Load the address of the corresponding user ISR... */
    
    BX      R3          /* Pass control to user interrupt and exit test */
    
    
    SCST_ALIGN_BYTES_4
    SCST_LTORG  /* Marks the current location for dumping psuedoinstruction pools (literal pool)
                   (containing numeric values for used symbolic names used within LDR instruction).
                   It is 4-byte aligned, as 2-byte alignment causes incorrect work. */
    SCST_FILE_END
    
