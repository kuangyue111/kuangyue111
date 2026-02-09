/**
  ******************************************************************************
  * @file    bsp_usb.c
  * @author  
  * @version V1.0.0
  * @date    15-Nov-2014
  * @brief   USB host Firmware Upgrade file
  ******************************************************************************
 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_usb.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBH_HandleTypeDef hUSBHost;
FW_ApplicationTypeDef Appli_state = APPLICATION_DISCONNECT;

/* Private function prototypes -----------------------------------------------*/
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/* Private functions ---------------------------------------------------------*/
void vUsbInit(void)
{
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
  USBH_Start(&hUSBHost);
}
void BSP_USBH_Process(void)
{
	USBH_Process(&hUSBHost);
}

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch(id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_READY;
    break;
    
  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_CONNECT;    
    break;
     
  default:
    break; 
  }
}


/************************ (C) COPYRIGHT EMERSON NETWORK POWER *****END OF FILE****/
