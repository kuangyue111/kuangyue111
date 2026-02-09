#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "stm32f4xx_hal.h"
/* Definition for USARTx's NVIC */
#define USART6_IRQHandler                USART6_IRQHandler
#define UART4_IRQHandler                 UART4_IRQHandler
#define UART5_IRQHandler                 UART5_IRQHandler

#define RS485_CTRL_TX     GPIO_PIN_RESET
#define RS485_CTRL_RX     GPIO_PIN_SET
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

int8_t BSP_Uart2Init(uint8_t u8BaudRateOffset);
int8_t BSP_Uart1Init(uint8_t u8BaudRateOffset);
int8_t BSP_Uart4Init(uint8_t u8BaudRateOffset,uint8_t ParityCase);
int8_t BSP_Uart5Init(uint8_t u8BaudRateOffset,uint8_t ParityCase);
int8_t BSP_Uart6Init(uint8_t u8BaudRate);
int8_t BSP_Uart4Tx(uint8_t * pbuffer ,uint16_t Size);
void BSP_Uart6RxEnable(void);
void BSP_Uart2RxEnable(void);
void BSP_Uart1RxEnable(void);
void BSP_Uart4RxEnable(void);
void BSP_Uart5RxEnable(void);
int8_t BSP_Uart4Rx(uint8_t *pbuffer ,uint16_t Size);
int8_t BSP_Uart5Tx(uint8_t * pbuffer ,uint16_t Size);
int8_t BSP_Uart5Rx(uint8_t *pbuffer ,uint16_t Size);
int8_t BSP_Uart6Tx(uint8_t *pbuffer ,uint16_t Size);
int8_t BSP_Uart6Rx(uint8_t *pbuffer ,uint16_t Size);
void  BSP_Realy1_Ctrl (GPIO_PinState u8CtrlStatus);
void  BSP_Realy2_Ctrl (GPIO_PinState u8CtrlStatus);
void BSP_Uart4_RS485_1_Ctrl (GPIO_PinState u8CtrlStatus);
void BSP_Uart5_RS485_2_Ctrl (GPIO_PinState u8CtrlStatus);
void BSP_Uart6_RS485_3_Ctrl (GPIO_PinState u8CtrlStatus);
void BSP_Uart6_RS485_3_Init(void);
int BSP_GetUart4State(void);
int BSP_GetUart5State(void);
int BSP_GetUart6State(void);
void BSP_Uart6_RTS_CTS_Init(void);
void Uart6_RTS_Ctrl (GPIO_PinState u8CtrlStatus);
void BSP_Uart6_RTS_Ctrl (GPIO_PinState u8CtrlStatus);
uint8_t BSP_GetUart6_CTS (void);
#endif
