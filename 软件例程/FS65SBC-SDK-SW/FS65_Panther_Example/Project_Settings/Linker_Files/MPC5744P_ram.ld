/*
** ###################################################################
**     Processor:           MPC5744P with 384 KB SRAM
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2017-2018 NXP
**     All rights reserved.
**
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**
** ###################################################################
*/


/* Entry Point */
ENTRY(_start)

/* define heap and stack size */
__HEAP_SIZE = DEFINED(__heap_size__)  ? __heap_size__  : 0x00000000;
__STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x00001000;


SRAM_SIZE =  256K;
/* Define SRAM Base Address */
SRAM_BASE_ADDR = 0x40000000;
LOCALDMEM_SIZE = 64K;
LOCALDMEM_BASE_ADDR = 0x50800000;

MEMORY
{
    SRAM        :    org = 0x40000000,   len = 384K
    local_dmem  :    org = 0x50800000,   len = 64K
}


SECTIONS
{

     /*========================== Text ==========================*/
    .startup : ALIGN(0x400)
    {
        __start = . ;
        *(.startup)
    } > SRAM

    .core_exceptions_table   : ALIGN(4096)
    {
        __IVPR_VALUE = . ;
        *(.core_exceptions_table)
    } > SRAM

    .intc_vector_table   : ALIGN(4096)
    {
        __VECTOR_TABLE = .;
        __VECTOR_RAM = .;               /* No copy */
        . = ALIGN(4);
        __interrupts_start__ = .;
        KEEP(*(.intc_vector_table))     /* Startup code */
        __interrupts_end__ = .;
    } > SRAM
    __RAM_VECTOR_TABLE_SIZE = 0x0;

    .text :
    {
        *(.text.startup)
        *(.text)
        *(.text.*)
        *(.sdata2)
        *(.sdata2.*)
        KEEP(*(.init))
        KEEP(*(.fini))
        /* discard boot header in RAM target.*/
        *(.rchw)
        *(.cpu0_reset_vector)
        . = ALIGN(16);
    } > SRAM

/* migration to version v1.2
   define section PREINIT_ARRAY */

.preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > SRAM


/* end section PREINIT_ARRAY */
/*  migration to version v1.2
    define section INIT_ARRAY*/

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array ))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > SRAM


/* end section INIT_ARRAY */

/* migration to version v1.2
   define section FINI_ARRAY */


  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array ))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > SRAM



/* end  section FINI_ARRAY */
/* migration to version v1.2
   define section DTORS */

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } > SRAM

/* end section DTORS */
/* migration to version v1.2
   define section CTORS */

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } > SRAM

/* end section CTORS */


    /* Section for storing functions that needs to execute from RAM */
    .code_ram :
    {
        . = ALIGN(4);
        __CODE_RAM = .;
        __code_ram_start__ = .;
        KEEP(*(.code_ram))               /* Custom section for storing code in RAM */
        __code_ram_end__ = .;
        __CODE_ROM = .;            /* Symbol is used by start-up for data initialization. */
        __CODE_END = .;            /* No copy */
        . = ALIGN(4);
    } > SRAM

    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    } > SRAM

    .eh_frame_hdr : { *(.eh_frame_hdr) } > SRAM
    .eh_frame     : { KEEP (*(.eh_frame)) } > SRAM

    __TEXT_END = .;                 /* Define a global symbol at end of code. */
    __DATA_ROM = .;                 /* Symbol is used by startup for data initialization. */
    __DATA_END = __DATA_ROM;        /* No copy */

    /*========================== DATA ==========================*/
    .customSectionBlock :
    {
        . = ALIGN(4);
        __customSectionStart = .;
        __customSection_start__ = .;
        KEEP(*(.customSection))  /* Keep section even if not referenced. */
        __customSection_end__ = .;
        __CUSTOM_ROM = .;
        __CUSTOM_END = .;
    } > SRAM
    .data   :
    {
        . = ALIGN(4);
        __DATA_RAM = .;
        __data_start__ = .;         /* Create a global symbol at data start. */
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        . = ALIGN(4);
        __data_end__ = .;           /* Define a global symbol at data end. */
        __sdata_end__ = .;
    }  > SRAM

    .bss   (NOLOAD)  :
    {
        __BSS_START = .;
        __bss_start__ = .;
        *(.sbss)
        *(.sbss.*)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __BSS_END = .;
        __bss_end__ = .;
    } > SRAM

    .stack (NOLOAD) : ALIGN(16)
    {
        __HEAP = . ;
        __HEAP_START = .;
        PROVIDE (_end = . );
        PROVIDE (end = . );
        . += __HEAP_SIZE ;
        __HEAP_END = . ;
        _stack_end = . ;
        . +=  __STACK_SIZE ;
        _stack_addr = . ;
        __SP_INIT = . ;
        . += 4;
    } > local_dmem

/*-------- LABELS USED IN CODE -------------------------------*/

/* Labels Used for Initialising SRAM ECC */
__SRAM_SIZE = SRAM_SIZE - (__sdata_end__ - SRAM_BASE_ADDR);
/* Skip .sdata section*/
__SRAM_BASE_ADDR = __sdata_end__;

__LOCAL_DMEM_SIZE = LOCALDMEM_SIZE;
__LOCAL_DMEM_BASE_ADDR = LOCALDMEM_BASE_ADDR;

__BSS_SIZE    = __BSS_END - __BSS_START;

}

