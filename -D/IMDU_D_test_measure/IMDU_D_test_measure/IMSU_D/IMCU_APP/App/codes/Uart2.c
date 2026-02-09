
#include "main.h"


NOINIT UINT8 gUart2RecvBuf[UART2_RECV_MAX_NUM];
NOINIT stKey_Data gKey_Data;

void Uart2TestMode(UINT8 u8IsTestMode)
{
	if (u8IsTestMode)
	{
		BSP_Uart2Init(m19200);
		dv_Uart2.fUart2EnableRx();
	}
	else
	{
		dv_Uart2.fInitUart2();//9600
		dv_Uart2.fUart2EnableRx();
	}

}
