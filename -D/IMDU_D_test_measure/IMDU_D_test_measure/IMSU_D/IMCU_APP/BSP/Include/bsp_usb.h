/**
  ******************************************************************************
  * @file    bsp_usb.h 
  * @author  
  * @version V1.0.0
  * @date    15-Nov-2014
  * @brief   Header for bsp_usb.c module
  ******************************************************************************

  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_USB_H
#define __BSP_USB_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_msc.h" 
#include "ff.h"
#include "stm324xg_eval.h" 
//#include "flash_if.h"
//#include "command.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_READY, 
  APPLICATION_CONNECT,  
  APPLICATION_DISCONNECT,
}FW_ApplicationTypeDef;

extern USBH_HandleTypeDef hUSBHost;
extern FATFS USBH_fatfs;
extern FW_ApplicationTypeDef Appli_state;


/* Exported constants --------------------------------------------------------*/  
#define BUFFER_SIZE        ((uint16_t)512*64) 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USB_Log_Process(UCHAR* pBuff,UINT16 length);
void USB_Rect_U1_Copy_Process(void);
void USB_Rect_U2_Copy_Process(void);
void Fail_Handler(void);
void FatFs_Fail_Handler(void);
void Erase_Fail_Handler(void);
void BSP_UsbSoftwareUpdata(void);
void BSP_USBH_Process(void);
void vUsbInit(void);
void FW_UPGRADE_Process(void);
#endif /* __BSP_USB_H */ 

/************************ (C) COPYRIGHT EMERSON NETWORK POWER *****END OF FILE****/
