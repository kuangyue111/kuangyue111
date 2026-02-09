
/*
  *****************************************************************************
  *
  * Name:bsp_uart_hal.h
  * Function: 	         
  * Version: 					                     
  * Update:					                           
  * Date: 			                       
  * Designer:                             			
  * Modified Date:                            	
  * Modified Engineer:				                  
  *
  *****************************************************************************
*/

#ifndef __BSP_UART_HAL_H
#define __BSP_UART_HAL_H

// ================* Include Files ************************
#include "includes.h"
#include <STM32F4xx_HAL_RCC_EX.H>

// ================* Definitions and Marcos ***************

#define UART_BUF_RX_SIZE   256
#define UART_BUF_TX_SIZE   256

typedef enum 
{
  USART_RS232,
  UART_485A,
  UART_485B,
  USART_DBG,
  NUM_USARTS
}usart_peripherals_e;

typedef  struct 
{
  USART_TypeDef*        usart_peri;
  IRQn_Type             irq;
  GPIO_TypeDef*         tx_port;
  GPIO_TypeDef*         rx_port;
  GPIO_TypeDef*         rts_port;
  GPIO_TypeDef*         cts_port;
  uint16_t              alt_func;
  uint16_t              tx_pin;
  uint16_t              rx_pin;
  uint16_t              cts_pin;
  uint16_t              rts_pin;
  UART_InitTypeDef      uart_setting;
//  void                  (*fp)(uint8_t);
}usart_conf_s;

typedef enum
{
  UART_1200_BAUD,
  UART_9600_BAUD,
  UART_19200_BAUD,
  UART_57600_BAUD,
  UART_115200_BAUD,
  UART_NUM_BUAD
}uart_baud_e;

typedef enum  {
  UART_BUF_STATUS_FREE,
  UART_BUF_STATUS_INUSE,
  UART_BUF_STATUS_DISABLED,
  UART_BUF_NUM_STATUS
} uart_buf_status_e;

typedef enum  {
  UART_STATUS_BUSY,
  UART_STATUS_FREE,
  UART_STATUS_ERROR,
  UART_NUM_STATUS
} uart_status_e;


typedef struct
{
  uint8_t au8RxBuf[UART_BUF_RX_SIZE];
  uint8_t au8TxBuf[UART_BUF_TX_SIZE];
  uint16_t u16RxBufCnt;
  uint16_t u16TxBufCnt;
  uint16_t u16ISRTxCnt;
  uint8_t  u8TxComplete;
  uint8_t  u8Error;
  uint8_t  u8RxStatus;
  uint8_t  u8TxStatus;
}uart_data_s;

// ================* Variables ****************************
extern const usart_conf_s usart_conf[NUM_USARTS];
extern uart_data_s uart_data_485A, uart_data_485B, uart_data_232, uart_data_dbg;

extern UART_HandleTypeDef UartRS232Handle;
extern UART_HandleTypeDef Uart485AHandle;
extern UART_HandleTypeDef Uart485BHandle;
extern UART_HandleTypeDef UartDBGHandle;

// ================* Function Prototypes ******************
// Function Prototypes
void bsp_InitUart(void);
void bsp_uart_Enable(uint8_t uart_no);
void bsp_uart_Disable(uint8_t uart_no);
void bsp_uart_485A_ISR(void);
void bsp_uart_485B_ISR(void);
void bsp_uart_232_ISR(void);
void bsp_uart_dbg_ISR(void);
void uart_data_init(uint8_t uart_no);

// Interface
void bsp_uart_DataRecv(void);
void bsp_uart_DataSend_Blocking(uint8_t uart, uint8_t *pu8Buffer, uint16_t ulCount);
void bsp_uart_DataSend_NonBlocking(uint8_t uart, uint8_t *pu8Buffer, uint16_t ulCount);
uint8_t bsp_uart_IsDataSendComplete(uint8_t uart);
void bsp_CfgUartISR(void);
#endif // __BSP_UART_232_H
// End of file
