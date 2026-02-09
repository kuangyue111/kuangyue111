/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Src/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   USB host Mass storage demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_main.h"
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
USBH_HandleTypeDef hUSBHost;
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void MSC_InitApplication(void);
extern HCD_HandleTypeDef hhcd;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void MYUSB_HOST_Init(void)
{
  BSP_IntVectSet(BSP_INT_ID_OTG_FS, OTG_FS_IRQHandler);
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);                                    // Init Host Library 
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);                                // Add Supported Class
  USBH_Start(&hUSBHost);                                                        // Start Host Process 
  if(f_mount(&USBH_fatfs, "", 0) != FR_OK)                                      // Register the file system object to the FatFs module 
  {  
 //   LCD_ErrLog("ERROR : Cannot Initialize FatFs! \n");
  }
}

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
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
    break;

  default:
    break; 
  }
}


void MyUsbTask(void)
{
  USBH_Process(&hUSBHost);
  if(Appli_state == APPLICATION_READY)
  {
    MSC_File_Operations();
  }
}
  
void OTG_FS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&hhcd);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
