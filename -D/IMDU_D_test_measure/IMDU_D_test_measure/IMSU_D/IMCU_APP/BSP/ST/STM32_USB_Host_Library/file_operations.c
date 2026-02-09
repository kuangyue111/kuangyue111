/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Src/file_operations.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   Write/read file on the disk.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
FIL MyFile;
FRESULT res;
uint32_t bytesWritten;
uint8_t rtext[200];
uint8_t wtext[] = "USB Host Library : Mass Storage Example";

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Files operations: Read/Write and compare
  * @param  None
  * @retval None
  */
 uint8_t MSC_File_Operations(UCHAR* pBuff,UINT16 length,UCHAR* pFileName)
{
  uint16_t bytesread;
  FRESULT RTN;
  //LCD_UsrLog("INFO : FatFs Initialized \n");
  RTN = f_open(&MyFile,pFileName,FA_READ|FA_OPEN_ALWAYS|FA_WRITE);
  //RTN = f_open(&MyFile,"0:TestLog01.txt",FA_READ|FA_OPEN_ALWAYS|FA_WRITE);
  if( RTN != FR_OK) 
  {
    //LCD_ErrLog("Cannot Open 'USBHost.txt' file \n");
	  return 2;
  }
  else
  {
	  f_lseek(&MyFile,MyFile.fptr+MyFile.fsize);
	  res= f_write (&MyFile, pBuff, length, (void *)&bytesWritten); 
	  f_close(&MyFile);
	  MyFile.fsize = 0;
	  if((bytesWritten == 0) || (res != FR_OK)) 
	  {
		  return 2;
	  }
	  else
	  {
		  return 1;
	  } 
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
