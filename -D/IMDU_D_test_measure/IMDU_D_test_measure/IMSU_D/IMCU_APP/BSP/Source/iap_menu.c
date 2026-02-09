
#include "main.h"
#include "ff.h"
#include "usbh_msc.h"
#include "bsp_usb.h"
#include <ucos_ii.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define DEMO_INIT       ((uint8_t)0x00)
//#define DEMO_IAP        ((uint8_t)0x01)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//DIR dir;
//FILINFO fno;
//static uint8_t Demo_State = DEMO_INIT;
//FATFS USBH_fatfs;
/* Private function prototypes -----------------------------------------------*/
//extern uint8_t MSC_File_Operations(UCHAR* pBuff,UINT16 length);
/* Private functions ---------------------------------------------------------*/
/*void FW_UPGRADE_Process(UCHAR* pBuff,UINT16 length)
{
	UCHAR temp;
	switch(Demo_State)
	{
	case DEMO_INIT:
		if(f_mount(&USBH_fatfs, "", 0 ) == FR_OK )
		{
			Demo_State = DEMO_IAP;
		}
		break;

	case DEMO_IAP:
		if(USBH_MSC_IsReady(&hUSBHost))
		{  
			if(Appli_state == APPLICATION_READY)
			{
				temp = MSC_File_Operations(pBuff,length);
				if(temp ==2)
					dv_USB.ucUSBFlag +=2;
				else
					dv_USB.ucUSBFlag =temp;
				dv_USB.ucLoadflag  = 0;
			}
		}
		else
		{
			dv_USB.ucUSBFlag +=2;
			dv_USB.ucLoadflag  = 1;
		}
		break;

	default:
		break;
	}
	if(Appli_state == APPLICATION_DISCONNECT)
	{
		OSTimeDly(100);
	}
}/*

/************************ (C) COPYRIGHT InfyPOWER *****END OF FILE****/
