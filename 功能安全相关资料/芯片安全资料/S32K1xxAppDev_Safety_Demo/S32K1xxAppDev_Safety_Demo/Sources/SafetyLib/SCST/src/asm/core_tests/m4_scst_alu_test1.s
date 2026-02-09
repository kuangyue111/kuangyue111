/******************************************************************************
*
* Freescale Confidential Proprietary                              
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* (c) Copyright 2001-2004 Motorola, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/

/******************************************************************************
* Test summary:
* -------------
* Test ALU module - ALU unit control bus
*
* Overall coverage:
* -----------------
* ALU control bus:
*   - use carry
*   - adder input 2 valid
*   - adder input 1 valid
*   - invert ALU input 2
*   - invert ALU input 1
*   - ALU operation type:
*       - no operation
*       - MOV(normal)
*
* DECODER:
* Thumb (32-bit)
*   - Encoding of "Data processing (modified immediate)" instructions
*   - Encoding of "Data processing (shifted register)" instructions
******************************************************************************/

#include "m4_scst_configuration.h"
#include "m4_scst_compiler.h"
    
    /* Symbols defined in the current module but to be visible to outside */        
    SCST_EXPORT m4_scst_alu_test1
    
    /* Symbols defined outside but used within current module */    
    SCST_EXTERN m4_scst_test_tail_end
    SCST_EXTERN m4_scst_clear_flags
    SCST_EXTERN m4_scst_check_flags_cleared
    
    
    SCST_SECTION_EXEC(m4_scst_test_code_unprivileged)        
    SCST_THUMB2
    
    /* The ".type" directive instructs the assembler/linker that the label "m4_scst_alu_test1" designates a function.
       In combination with the above specified ".thumb2" directive, this would cause setting
       the least significant bit to '1' within any pointer to this function,
       causing change to Thumb mode whenever this function is called. */
    SCST_TYPE(m4_scst_alu_test1, function)
m4_scst_alu_test1:

    PUSH.W  {R1-R12,R14}
    
    /* R9 is used as intermediate result */
    MOV     R9,#0x0
    
    /***************************************************************************************************
    * ALU control bus:
    * - use adder input1
    * - use adder input2 (used together with input1)
    * - ALU operation type:
    *   - no operation
    *
    * Note: Linked with ADD, ADDW instructions
    *
    ***************************************************************************************************/
    /* ADD */
    MOV     R10,#0x0
    /* ADD(immediate) Encoding T3 32bit */
    ADDS.W  R5,R10,#0x55555555   /* Set flag bit to verify it is not stuck-at "1" in next call */
    CMP     R5,#0x55555555
    BNE     m4_scst_test_tail_end
    
    /* ADD(immediate) Encoding T3 32bit */
    MOV     R5,#0x0
    BL      m4_scst_clear_flags /* Clear flags */
    ADD.W   R10,R5,#0xAAAAAAAA   /* If flag bit set then N flag is updated !! */
    BL      m4_scst_check_flags_cleared /* Check flags were not updated !!! encoding of flag bit is not stuck-at "1" */
    CMP     R10,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
        
    LDR     R10,=0x55555555
    MOV     R5,#0x0
    /* ADD(register) Encoding T3 32bit */
    ADDS.W  R5,R10,R5   /* Set flag bit to verify it is not stuck-at "1" in next call */
    CMP     R5,#0x55555555
    BNE     m4_scst_test_tail_end
        
    LDR     R5,=0xAAAAAAAA
    MOV     R10,#0x0
    /* ADD(register) Encoding T3 32bit */
    BL      m4_scst_clear_flags /* Clear flags */
    ADD.W   R10,R5,R10    /* If flag bit set then N flag is updated !! */
    BL      m4_scst_check_flags_cleared /* Check flags were not updated !!! encoding of flag bit is not stuck-at "1" */
    CMP     R10,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    MOV     R5,#0x0    
    LDR     R10,=0x55555555
    /* ADD(register) Encoding T3 32bit */
    ADD.W   R5,R5,R10
    CMP     R5,#0x55555555
    BNE     m4_scst_test_tail_end
    
    MOV     R10,#0x0    
    LDR     R5,=0xAAAAAAAA
    /* ADD(register) Encoding T3 32bit */
    ADD.W   R10,R10,R5
    CMP     R10,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    /* ADDW */    
    LDR     R2,=0x55555000
    /* ADD(immediate) Encoding T4 32bit */
    ADDW    R3,R2,#0x555   /* Use imm12 value */
    CMP     R3,#0x55555555
    BNE     m4_scst_test_tail_end
        
    LDR     R2,=0xAAAAA000
    /* ADD(immediate) Encoding T4 32bit */
    ADDW    R3,R2,#0xAAA   /* Use immed12 value */
    CMP     R3,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    ADDW    R9,R9,#0x0A46
    
    
    /***************************************************************************************************
    * ALU control bus:
    * - invert ALU input1
    * - use adder input1
    * - use adder input2
    * - ALU operation type:
    *   - no operation
    *
    *   
    *   Note: Linked with RSB instruction
    ***************************************************************************************************/    
    LDR     R12,=0x55555555
    /* RSB(immediate) Encoding T2 32bit */
    RSB.W   R3,R12,#0x0
    LDR     R6,=0xAAAAAAAB /* - 0x55555555 */
    CMP     R3,R6
    BNE     m4_scst_test_tail_end
    
    LDR     R3,=0xAAAAAAAB /* -0x55555555 */
    /* RSB(immediate) Encoding T2 32bit */
    RSB.W   R12,R3,#0x0
    LDR     R6,=0x55555555
    CMP     R12,R6
    BNE     m4_scst_test_tail_end
    
    LDR     R3,=0xAAAAAAAA /* -0x55555556 */
    MOV     R12,#0
    /* RSB(register) Encoding T1 32bit */
    RSB.W   R3,R3,R12
    LDR     R6,=0x55555556
    CMP     R3,R6
    BNE     m4_scst_test_tail_end
    
    LDR     R12,=0x55555556
    MOV     R3,#0
    /* RSB(register) Encoding T1 32bit */
    RSB.W   R12,R12,R3
    LDR     R6,=0xAAAAAAAA /* -0x55555556 */
    CMP     R12,R6
    BNE     m4_scst_test_tail_end
    
    ADDW    R9,R9,#0x08E7
    
    
    /***************************************************************************************************
    * ALU control bus:
    * - invert ALU input 2
    * - use adder input1
    * - use adder input2
    * - ALU operation type:
    *   - no operation
    *
    * Note: Linked with SUB, SUBW instructions
    *
    ***************************************************************************************************/
    /* SUB */
    MOV     R7,#0x0
    /* SUB(immediate) Encoding T3 32bit */
    SUB.W   R8,R7,#0xAAAAAAAB   /* Use immediate value */
    CMP     R8,#0x55555555
    BNE     m4_scst_test_tail_end
    
    MOV     R8,#0x0
    /* SUB(immediate) Encoding T3 32bit */
    SUB.W   R7,R8,#0x55555556   /* Use immediate value */
    CMP     R7,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    LDR     R8,=0x55555555
    LDR     R7,=0xAAAAAAAA
    /* SUB(register) Encoding T2 32bit */
    SUB.W   R8,R8,R7
    CMP     R8,#0xAAAAAAAB
    BNE     m4_scst_test_tail_end
    
    LDR     R7,=0xAAAAAAAA
    MOV     R8,#0x55555555
    /* SUB(register) Encoding T2 32bit */
    SUB.W   R7,R7,R8
    CMP     R7,#0x55555555
    BNE     m4_scst_test_tail_end
    
    MOV     R8,#0x0
    LDR     R7,=0x55555556
    /* SUB(register) Encoding T2 32bit */
    SUB.W   R7,R8,R7
    CMP     R7,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    /* SUBW */
    LDR     R2,=0x55555FFF
    /* SUB(immediate) Encoding T4 32bit */
    SUBW    R3,R2,#0xAAA
    CMP     R3,#0x55555555
    BNE     m4_scst_test_tail_end
    
    LDR     R2,=0xAAAAAFFF
    /* SUB(immediate) Encoding T4 32bit */
    SUBW    R3,R2,#0x555
    CMP     R3,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    ADDW    R9,R9,#0x0365
    
    
    /***************************************************************************************************
    * ALU control bus:
    * - use carry
    * - use adder input1
    * - use adder input2
    * - ALU operation type:
    *   - no operation
    *
    * Note: Linked with ADC instruction
    *
    ***************************************************************************************************/
    BL      m4_scst_clear_flags
    /* ADC */
    LDR     R4,=0x55555555
    /* ADC(immediate) Encoding T1 32bit */
    ADC.W   R11,R4,#0x55555555   /* Use immediate value */
    CMP     R11,#0xAAAAAAAA      /* Carry flag is not added */
    BNE     m4_scst_test_tail_end
    
    /* Set Carry flag */
    LDR     R4,=0x20000000
    MSR     APSR_nzcvq,R4
    
    LDR     R11,=0xAAAAAAAA
    /* ADC(immediate) Encoding T1 32bit */
    ADC.W   R4,R11,#0xAAAAAAAA   /* Use immediate value */
    CMP     R4,#0x55555555      /* Carry flag is added */
    BNE     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    
    LDR     R4,=0x55555555
    LDR     R11,=0x55555555
    /* ADC(register) Encoding T2 32bit */
    ADC.W   R11,R4,R11
    CMP     R11,#0xAAAAAAAA      /* Carry flag is not added */
    BNE     m4_scst_test_tail_end
    
    /* Set Carry flag */
    LDR     R4,=0x20000000
    MSR     APSR_nzcvq,R4
    
    LDR     R11,=0xAAAAAAAA
    LDR     R4,=0xAAAAAAAA
    /* ADC(register) Encoding T2 32bit */
    ADC.W   R4,R11,R4
    CMP     R4,#0x55555555      /* Carry flag is added */
    BNE     m4_scst_test_tail_end
    
    ADDW    R9,R9,#0x089E
    
    
    /***************************************************************************************************
    * ALU control bus:
    * - use carry
    * - invert ALU input 2
    * - use adder input1
    * - use adder input2
    * - ALU operation type:
    *   - no operation
    *
    * Note: Linked with SBC instruction
    *
    ***************************************************************************************************/
    /* SBC - invert ALU input 2*/
    BL      m4_scst_clear_flags
    
    LDR     R2,=0x55555555
    /* SBC(immediate) Encoding T1 32bit */
    SBC.W   R3,R2,#0xAAAAAAAA
    CMP     R3,#0xAAAAAAAA      /* Carry flag is not added */
    BNE     m4_scst_test_tail_end
    
    /* Set Carry flag */
    LDR     R4,=0x20000000
    MSR     APSR_nzcvq,R4
    
    LDR     R3,=0xAAAAAAAA
    /* SBC(immediate) Encoding T1 32bit */
    SBC.W   R2,R3,#0x55555555   /* Use immediate value */
    CMP     R2,#0x55555555      /* Carry flag is added */
    BNE     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    
    LDR     R2,=0x55555555
    LDR     R1,=0xAAAAAAAA
    /* SBC(register) Encoding T2 32bit */
    SBC.W   R3,R2,R1
    CMP     R3,#0xAAAAAAAA      /* Carry flag is not added */
    BNE     m4_scst_test_tail_end
    
    /* Set Carry flag */
    LDR     R4,=0x20000000
    MSR     APSR_nzcvq,R4
    
    LDR     R2,=0xAAAAAAAA
    LDR     R1,=0x55555555
    /* SBC(register) Encoding T2 32bit */
    SBC.W   R3,R2,R1
    CMP     R3,#0x55555555      /* Carry flag is added */
    BNE     m4_scst_test_tail_end
    
    ADDW    R9,R9,#0x02EF
    
    
    /***************************************************************************************************
    * ALU control bus:
    * - invert ALU input2
    * - use adder input2
    * - ALU operation type:
    *   - MOV(normal)
    *   
    *   Note: Linked with MVN instruction
    ***************************************************************************************************/
    /* MVN(immediate) Encoding T1 32bit */
    MVN.W   R3,#0x55555555
    CMP     R3,#0xAAAAAAAA
    BNE     m4_scst_test_tail_end
    
    LDR     R2,=0xAAAAAAAA
    /* MVN(register) Encoding T2 32bit */
    MVN.W   R3,R2
    CMP     R3,#0x55555555
    BNE     m4_scst_test_tail_end
    
    ADDW    R9,R9,#0x09EA
    
    
    MOV     R0,R9          /* Test result is returned in R0, according to the conventions */
    
    B       m4_scst_test_tail_end
    
    
    SCST_ALIGN_BYTES_4
    SCST_LTORG  /* Marks the current location for dumping psuedoinstruction pools (literal pool)
                   (containing numeric values for used symbolic names used within LDR instruction).
                   It is 4-byte aligned, as 2-byte alignment causes incorrect work. */
    
    SCST_FILE_END
    