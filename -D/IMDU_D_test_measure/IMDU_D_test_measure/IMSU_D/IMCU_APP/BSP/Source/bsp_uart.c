#include "bsp_uart.h"
#include "main.h"

/* UART handler declaration */
UART_HandleTypeDef Uart1Handle;
UART_HandleTypeDef Uart2Handle;
UART_HandleTypeDef Uart4Handle;
UART_HandleTypeDef Uart5Handle;
UART_HandleTypeDef Uart6Handle;

/* Buffer used for transmission */

/* Buffer used for reception */

const uint32_t  u32BaudRate[7]={9600,4800,2400,1200,19200,38400,115200};

//static GPIO_InitTypeDef  GPIO_InitStruct;

int8_t BSP_Uart1Init(uint8_t u8BaudRateOffset)
{	
	if(u8BaudRateOffset > m115200)
	{
		u8BaudRateOffset= m115200;
	}
	Uart1Handle.Instance        = USART1;
	Uart1Handle.Init.BaudRate   = u32BaudRate[u8BaudRateOffset];
	Uart1Handle.Init.WordLength = 0x00;
	Uart1Handle.Init.StopBits   = 0x00;
	Uart1Handle.Init.Parity     = 0x00;
	Uart1Handle.Init.HwFlowCtl  = 0x00;
	Uart1Handle.Init.Mode       = 0x0C;

	if(HAL_UART_Init(&Uart1Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

int8_t BSP_Uart2Init(uint8_t u8BaudRateOffset)
{	
	if(u8BaudRateOffset > m115200)
	{
		u8BaudRateOffset = m115200;
	}
	Uart2Handle.Instance        = USART2;
	Uart2Handle.Init.BaudRate   = u32BaudRate[u8BaudRateOffset];
	Uart2Handle.Init.WordLength = 0x00;
	Uart2Handle.Init.StopBits   = 0x00;
	Uart2Handle.Init.Parity     = 0x00;
	Uart2Handle.Init.HwFlowCtl  = 0x00;
	Uart2Handle.Init.Mode       = 0x0004;//#define UART_MODE_TX_RX	((uint32_t)(USART_CR1_TE |USART_CR1_RE))

	if(HAL_UART_Init(&Uart2Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

int8_t BSP_Uart4Init(uint8_t u8BaudRateOffset,uint8_t ParityCase)
{	
	Uart4Handle.State = HAL_UART_STATE_RESET;

	if(u8BaudRateOffset > m115200)
	{
		u8BaudRateOffset= m115200;
	}
	Uart4Handle.Instance        = UART4;
	Uart4Handle.Init.BaudRate   = u32BaudRate[u8BaudRateOffset];

	Uart4Handle.Init.StopBits   = 0x00;						//1个停止位
	if (UARTX_PARITY_EVEN == ParityCase)
	{
		Uart4Handle.Init.WordLength = 0x1000;					//USART_WordLength_9b;//0x00;
		Uart4Handle.Init.Parity     = UART_PARITY_EVEN;				//SERIAL_PARITY_EVEN;//0x00;
	}
	else
	{
		Uart4Handle.Init.WordLength = 0;					//USART_WordLength_9b;//0x00;
		Uart4Handle.Init.Parity     = UART_PARITY_NONE;
	}

	Uart4Handle.Init.HwFlowCtl  = 0x00;
	Uart4Handle.Init.Mode       = 0x0C;

	if(HAL_UART_Init(&Uart4Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	BSP_Uart4_RS485_1_Ctrl(RS485_CTRL_RX);

	//if(u8BaudRateOffset > m115200)
	//{
	//	u8BaudRateOffset= m115200;
	//}
	//Uart4Handle.Instance        = UART4;
	//Uart4Handle.Init.BaudRate   = u32BaudRate[u8BaudRateOffset];
	//Uart4Handle.Init.WordLength = 0x00;
	//Uart4Handle.Init.StopBits   = 0x00;
	//Uart4Handle.Init.Parity     = 0x00;
	//Uart4Handle.Init.HwFlowCtl  = 0x00;
	//Uart4Handle.Init.Mode       = 0x0C;
	//
	//if(HAL_UART_Init(&Uart4Handle) != HAL_OK)
	//{
	//	return HAL_ERROR;
	//}
	//BSP_Uart4_RS485_1_Ctrl(RS485_CTRL_RX);
	return HAL_OK;
}	

int8_t BSP_Uart5Init(uint8_t u8BaudRateOffset, uint8_t ParityCase)
{
	Uart5Handle.State = HAL_UART_STATE_RESET;

	if(u8BaudRateOffset > m115200)
	{
		u8BaudRateOffset= m115200;
	}
	Uart5Handle.Instance        = UART5;
	Uart5Handle.Init.BaudRate   = u32BaudRate[u8BaudRateOffset];
	
	Uart5Handle.Init.StopBits   = 0x00;						//1个停止位
	if (UARTX_PARITY_EVEN == ParityCase)
	{
		Uart5Handle.Init.WordLength = 0x1000;					//USART_WordLength_9b;//0x00;
		Uart5Handle.Init.Parity     = UART_PARITY_EVEN;				//SERIAL_PARITY_EVEN;//0x00;
	}
	else
	{
		Uart5Handle.Init.WordLength = 0;					//USART_WordLength_9b;//0x00;
		Uart5Handle.Init.Parity     = UART_PARITY_NONE;
	}
	
	Uart5Handle.Init.HwFlowCtl  = 0x00;
	Uart5Handle.Init.Mode       = 0x0C;
	
	if(HAL_UART_Init(&Uart5Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	BSP_Uart5_RS485_2_Ctrl(RS485_CTRL_RX);
	return HAL_OK;
}
int8_t BSP_Uart6Init(uint8_t u8BaudRateOffset)
{	
	if(u8BaudRateOffset > m115200)
	{
		u8BaudRateOffset= m115200;
	}
	Uart6Handle.Instance        = USART6;
	Uart6Handle.Init.BaudRate   = u32BaudRate[u8BaudRateOffset];

	Uart6Handle.Init.StopBits   = 0x00;

	//Uart6Handle.Init.WordLength = 0x1000;						//USART_WordLength_9b;//0x00;
	//Uart6Handle.Init.Parity     = UART_PARITY_EVEN;				//SERIAL_PARITY_EVEN;//0x00;
	Uart6Handle.Init.WordLength = 0x00;
	Uart6Handle.Init.Parity     = UART_PARITY_NONE;

	Uart6Handle.Init.HwFlowCtl  = 0x00;
	Uart6Handle.Init.Mode       = 0x0C;
	
	if(HAL_UART_Init(&Uart6Handle) != HAL_OK)
	{
		return HAL_ERROR;
	}
	//BSP_Uart6_RTS_Ctrl(GPIO_PIN_SET);
	//BSP_Uart6_RS485_3_Ctrl(RS485_CTRL_RX);
	return HAL_OK;
}	

void BSP_Uart5RxEnable(void)
{
	TXY_UART_Receive_Enable(&Uart5Handle);
}

void BSP_Uart4RxEnable(void)
{
	TXY_UART_Receive_Enable(&Uart4Handle);
}

void BSP_Uart1RxEnable(void)
{
	TXY_UART_Receive_Enable(&Uart1Handle);
}

void BSP_Uart2RxEnable(void)
{
	TXY_UART_Receive_Enable(&Uart2Handle);
}


void BSP_Uart6RxEnable(void)
{
	TXY_UART_Receive_Enable(&Uart6Handle);
}

int8_t BSP_Uart4Tx(uint8_t * pbuffer ,uint16_t Size)
{
    if(HAL_UART_Transmit_IT(&Uart4Handle,  pbuffer, Size)!= HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;  
} 

int8_t BSP_Uart5Tx(uint8_t * pbuffer ,uint16_t Size)
{
  if(HAL_UART_Transmit_IT(&Uart5Handle, pbuffer, Size)!= HAL_OK)
  //if(HAL_UART_Transmit_IT(&Uart5Handle, (uint8_t *)aTxStartMessage, Size)!= HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;  
} 
int8_t BSP_Uart6Tx(uint8_t * pbuffer ,uint16_t Size)
{
  if(HAL_UART_Transmit_IT(&Uart6Handle, pbuffer, Size)!= HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;  
}  
int8_t BSP_Uart4Rx(uint8_t *pbuffer ,uint16_t Size)
{
    if(HAL_UART_Receive_IT(&Uart4Handle, pbuffer, Size) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;    
}
int8_t BSP_Uart5Rx(uint8_t *pbuffer ,uint16_t Size)
{
  //if(HAL_UART_Receive_IT(&Uart5Handle, (uint8_t *)gs_Uart5.RxBuffer, UART_RX_BUFFER_SIZE) != HAL_OK)
  if(HAL_UART_Receive_IT(&Uart5Handle, pbuffer, Size) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;    
}
int8_t BSP_Uart6Rx(uint8_t *pbuffer ,uint16_t Size)
{
  if(HAL_UART_Receive_IT(&Uart6Handle, pbuffer, Size) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;    
}


void BSP_Uart6_RTS_Ctrl (GPIO_PinState u8CtrlStatus)
{
	//HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8,u8CtrlStatus);//0x0100
	return;
}

uint8_t BSP_GetUart6_CTS (void)
{
  //return HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_15);//0x8000
	return 0;
}
void BSP_Uart4_RS485_1_Ctrl (GPIO_PinState u8CtrlStatus)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,u8CtrlStatus);//0x8 维持不变
}
void BSP_Uart5_RS485_2_Ctrl (GPIO_PinState u8CtrlStatus)
{
  //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,u8CtrlStatus);//0x40 PD6用于 UART-RX了
   HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,u8CtrlStatus);//PF10在新板中用于UART5了
}
void  BSP_Realy1_Ctrl (GPIO_PinState u8CtrlStatus)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,u8CtrlStatus);//0x8 维持不变
}
void BSP_Realy2_Ctrl (GPIO_PinState u8CtrlStatus)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,u8CtrlStatus);//PF10在新板中用于UART5了
}
void BSP_Uart6_RS485_3_Ctrl (GPIO_PinState u8CtrlStatus)
{
  //HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,u8CtrlStatus);PF10在新板中用于UART5了
}


int BSP_GetUart4State(void)
{
  //HAL_UART_STATE_READY is TxRx end
  return HAL_UART_GetState(&Uart4Handle);
}
int BSP_GetUart5State(void)
{
  //HAL_UART_STATE_READY is TxRx end
  return HAL_UART_GetState(&Uart5Handle);
}
int BSP_GetUart6State(void)
{
  //HAL_UART_STATE_READY is TxRx end
  return HAL_UART_GetState(&Uart6Handle);
}


//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *Uart6Handle)
//{
//
//}
//
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *Uart6Handle)
//{
//
//}
//
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *Uart6Handle)
//{
//
//}
