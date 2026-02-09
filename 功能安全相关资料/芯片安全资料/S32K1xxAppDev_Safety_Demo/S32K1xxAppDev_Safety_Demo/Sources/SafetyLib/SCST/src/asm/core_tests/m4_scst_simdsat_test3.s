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
* Tests general purpose saturation instructions SSAT16 and USAT16 
* with with all possible saturation conditions as well as their decoding.
* Condition flag Q is checked.
*
* Overall coverage:
* -----------------
* SIMDSAT:
* - General purpose saturation logic.
* 
* DECODER:
* Thumb (32-bit)
*   - Encoding of "Data processing (plain binary immediate)" instructions
******************************************************************************/

#include "m4_scst_compiler.h"

    /* Symbols defined in the current module but to be visible to outside    */
    SCST_EXPORT m4_scst_simdsat_test3
    
    /* Symbols defined outside but used within current module */
    SCST_EXTERN m4_scst_test_tail_end
    SCST_EXTERN m4_scst_clear_flags
    SCST_EXTERN m4_scst_check_q_flag

    /* Local defines */    
SCST_SET(PRESIGNATURE,0x24CA43F3) /* this macro has to be at the beginning of the line */

    SCST_SECTION_EXEC(m4_scst_test_code_unprivileged)
    SCST_THUMB2

    /* The ".type" directive instructs the assembler/linker that the label "m4_scst_simdsat_test3" designates a function.
       In combination with the above specified ".thumb2" directive, this would cause setting
       the least significant bit to '1' within any pointer to this function,
       causing change to Thumb mode whenever this function is called. */
    SCST_TYPE(m4_scst_simdsat_test3, function) 
m4_scst_simdsat_test3:

    PUSH.W  {R1-R12,R14}
    
    LDR     R8,=PRESIGNATURE
    
    /*--------------------------------------------------------------------*/
    /* SSAT16                                                             */
    /*--------------------------------------------------------------------*/
    LDR     R5,=0xAAAAAAAA  
    LDR     R10,=0x55555555
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#16,R5
    LDR     R1,=0xAAAAAAAA
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BNE     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#16,R10
    LDR     R2,=0x55555555        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BNE     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#15,R5
    LDR     R1,=0xC000C000
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#15,R10
    LDR     R2,=0x3FFF3FFF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#14,R5
    LDR     R1,=0xE000E000
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#14,R10
    LDR     R2,=0x1FFF1FFF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#13,R5
    LDR     R1,=0xF000F000
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#13,R10
    LDR     R2,=0x0FFF0FFF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#12,R5
    LDR     R1,=0xF800F800
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    B m4_scst_simdsat_test3_ltorg_jump_1 /* jump over the following LTORG section */
    SCST_ALIGN_BYTES_4
    SCST_LTORG  /* Marks the current location for dumping psuedoinstruction pools (literal pool)
                   (containing numeric values for used symbolic names used within LDR instruction).
                   It is 4-byte aligned, as 2-byte alignment causes incorrect work. */
m4_scst_simdsat_test3_ltorg_jump_1:
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#12,R10
    LDR     R2,=0x07FF07FF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#11,R5
    LDR     R1,=0xFC00FC00
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#11,R10
    LDR     R2,=0x03FF03FF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#10,R5
    LDR     R1,=0xFE00FE00
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#10,R10
    LDR     R2,=0x01FF01FF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#9,R5
    LDR     R1,=0xFF00FF00
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#9,R10
    LDR     R2,=0x00FF00FF        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#8,R5
    LDR     R1,=0xFF80FF80
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#8,R10
    LDR     R2,=0x007F007F        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#7,R5
    LDR     R1,=0xFFC0FFC0
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#7,R10
    LDR     R2,=0x003F003F        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#6,R5
    LDR     R1,=0xFFE0FFE0
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#6,R10
    LDR     R2,=0x001F001F        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#5,R5
    LDR     R1,=0xFFF0FFF0
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#5,R10
    LDR     R2,=0x000F000F        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#4,R5
    LDR     R1,=0xFFF8FFF8
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#4,R10
    LDR     R2,=0x00070007        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#3,R5
    LDR     R1,=0xFFFCFFFC
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#3,R10
    LDR     R2,=0x00030003     
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#2,R5
    LDR     R1,=0xFFFEFFFE
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#2,R10
    LDR     R2,=0x00010001        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R3,#1,R5
    LDR     R1,=0xFFFFFFFF
    CMP     R3,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    SSAT16  R12,#1,R10
    LDR     R2,=0x00000000        
    CMP     R12,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    ADDW    R8,R8,#0xABC
    
    /*--------------------------------------------------------------------*/
    /* USAT16                                                             */
    /*--------------------------------------------------------------------*/
    LDR     R3,=0x55555555
    LDR     R12,=0xAAAAAAAA
    
    BL      m4_scst_clear_flags
    USAT16  R5,#15,R3
    LDR     R1,=0x55555555
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BNE     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R10,#15,R12
    LDR     R2,=0x0
    CMP     R10,R2
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    LDR     R12,=0x55555555  
    USAT16  R10,#14,R12
    LDR     R1,=0x3FFF3FFF
    CMP     R10,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#13,R3
    LDR     R1,=0x1FFF1FFF
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#12,R3
    LDR     R1,=0x0FFF0FFF
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags    
    USAT16  R5,#11,R3
    LDR     R1,=0x07FF07FF
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#10,R3
    LDR     R1,=0x03FF03FF
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#9,R3
    LDR     R1,=0x01FF01FF
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#8,R3
    LDR     R1,=0x00FF00FF
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#7,R3
    LDR     R1,=0x007F007F
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#6,R3
    LDR     R1,=0x003F003F
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#5,R3
    LDR     R1,=0x001F001F
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#4,R3
    LDR     R1,=0x000F000F
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#3,R3
    LDR     R1,=0x00070007
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
                
    BL      m4_scst_clear_flags
    USAT16  R5,#2,R3
    LDR     R1,=0x00030003
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#1,R3
    LDR     R1,=0x00010001
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    BL      m4_scst_clear_flags
    USAT16  R5,#0,R3
    LDR     R1,=0x00000000
    CMP     R5,R1
    BNE     m4_scst_test_tail_end
    BL      m4_scst_check_q_flag
    BEQ     m4_scst_test_tail_end
    
    ADDW    R8,R8,#0xABC
    
    
    MOV     R0,R8       /* Test result is returned in R0 */
    
    B       m4_scst_test_tail_end
    
    
    SCST_ALIGN_BYTES_4
    SCST_LTORG  /* Marks the current location for dumping psuedoinstruction pools (literal pool)
                   (containing numeric values for used symbolic names used within LDR instruction).
                   It is 4-byte aligned, as 2-byte alignment causes incorrect work. */

    SCST_FILE_END
    