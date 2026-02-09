#include "main.h"
#include "Uart6_new.h"
//#define DEBUT_BRUSH_NO_ALM 0

#include "bsp_dido.h"

#define ABS(a)	(((a) < 0) ? (-(a)) : (a))
NOINIT UINT8 gUART1RecvData[UART1_RECV_MAX_NUM];

void TACK_Uart1Proc (void* pdata)
{
	UCHAR i;
	OSTimeDly(500);
	OSTimeDly(500);
	dv_Uart1.fInitUart1();					//19200
	dv_Uart1.fUart1EnableRx();
	for(;;)
	{

		if(dv_Uart1.ReceiveFlag == TRUE)
		{
			if(gChargeCtrl.ucScanEVCodeNumber[GUN_NO_A]== TRUE)
			{
				for(i=0;i<22;i++)
				{
					if(gUART1RecvData[i] ==0xD)
					{
						break;
					}
					gChargeCtrl.ucEVCode[GUN_NO_A][i] = gUART1RecvData[i];
				}
                                gChargeCtrl.ucScanEVCodeNumber[GUN_NO_A] = FALSE;
			}
			if(gChargeCtrl.ucScanEVCodeNumber[GUN_NO_B]== TRUE)
			{
				for(i=0;i<22;i++)
				{
					if(gUART1RecvData[i] ==0xD)
					{
						break;
					}
					gChargeCtrl.ucEVCode[GUN_NO_B][i] = gUART1RecvData[i];
				}
                                gChargeCtrl.ucScanEVCodeNumber[GUN_NO_B] = FALSE;
			}
			for(i=0;i<22;i++)
			{
				gUART1RecvData[i]= 0;
			}
			dv_Uart1.pData = 0;
			dv_Uart1.ReceiveFlag = FALSE;
			
		}
		gSelfTest.fSetTaskID(Task_ID_Uart1);
		OSTimeDly(100);
		
		
	}//END FOR!
}







