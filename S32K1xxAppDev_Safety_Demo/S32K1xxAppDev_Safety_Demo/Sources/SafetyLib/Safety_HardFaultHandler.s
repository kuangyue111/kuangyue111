.if 0
    .syntax unified
    .arch armv7-m

    .section .text, "ax"
    .thumb
    .thumb_func
    .align 2
    .globl HardFault_Handler
    .type    HardFault_Handler, %function
    .extern HardFault_Handler_C

HardFault_Handler:
    /*tst lr, #4
	ite eq
	;mrseq r0, msp
	;mrseq r0, psp*/
	mrs r0, psp
	ldr r2, =HardFault_Handler_C
	bx r2
	pop {pc}

	.endif
