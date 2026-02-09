/**
  ******************************************************************************
  * @file    usbh_conf.c
  * @author  
  * @version V1.0.0
  * @date    15-Nov-2014
  * @brief   USB Host configuration file.
  ******************************************************************************

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_hal.h"
//#include "usbh_core.h"
//#include "includes.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_hcd.h"
#include "usbh_core.h"
#include <ucos_ii.h>

HCD_HandleTypeDef hhcd;

void HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC->AHB1ENR |= 1;
    GPIO_InitStruct.Pin = 0x1800;
    GPIO_InitStruct.Mode = 2;
    GPIO_InitStruct.Pull = 0;
    GPIO_InitStruct.Speed = 3;
    GPIO_InitStruct.Alternate = 10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
    RCC->AHB2ENR |= 0x80;
    RCC->APB2ENR |= 0x4000;
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)
{ 
    RCC->AHB2ENR &= ~(0x00000080);
    RCC->APB2ENR &= ~(0x00004000);
}

void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_IncTimer (hhcd->pData);
}

void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_Connect(hhcd->pData);
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_Disconnect(hhcd->pData);
} 

void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
  /* To be used with OS to sync URB state with the global state machine */
}

USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{  
  hhcd.Instance = USB_OTG_FS;
  hhcd.Init.Host_channels = 11; 
  hhcd.Init.dma_enable = 0;
  hhcd.Init.low_power_enable = 0;
  hhcd.Init.phy_itface = 2; 
  hhcd.Init.Sof_enable = 0;
  hhcd.Init.speed = 3;
  hhcd.Init.vbus_sensing_enable = 0;
  hhcd.pData = phost;
  phost->pData = &hhcd;

  HAL_HCD_Init(&hhcd);
  USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&hhcd));
  return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
  HAL_HCD_DeInit(phost->pData);
  return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Start(phost->pData);
  return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Stop(phost->pData);
  return USBH_OK; 
}

USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
  USBH_SpeedTypeDef speed = USBH_SPEED_FULL;
    
  switch (HAL_HCD_GetCurrentSpeed(phost->pData))
  {
  case 0: 
    speed = USBH_SPEED_HIGH;
    break;

  case 1: 
    speed = USBH_SPEED_FULL;    
    break;
    
  case 2: 
    speed = USBH_SPEED_LOW;   
    break;
    
   default:  
    speed = USBH_SPEED_FULL;    
    break;    
  }
  return speed;
}


USBH_StatusTypeDef USBH_LL_ResetPort (USBH_HandleTypeDef *phost) 
{
  HAL_HCD_ResetPort(phost->pData);
  return USBH_OK; 
}

uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)  
{
  return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, 
                                    uint8_t pipe,
                                    uint8_t epnum,                                      
                                    uint8_t dev_address,
                                    uint8_t speed,
                                    uint8_t ep_type,
                                    uint16_t mps)
{
  HAL_HCD_HC_Init(phost->pData,
                  pipe,
                  epnum,
                  dev_address,
                  speed,
                  ep_type,
                  mps);
  return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)   
{
  HAL_HCD_HC_Halt(phost->pData, pipe);
  return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, 
                                     uint8_t pipe, 
                                     uint8_t direction,
                                     uint8_t ep_type,  
                                     uint8_t token, 
                                     uint8_t* pbuff, 
                                     uint16_t length,
                                     uint8_t do_ping) 
{
  HAL_HCD_HC_SubmitRequest(phost->pData,
                           pipe, 
                           direction,
                           ep_type,  
                           token, 
                           pbuff, 
                           length,
                           do_ping);
  return USBH_OK;   
}

USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe) 
{
  return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState (phost->pData, pipe);
}


USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
  return USBH_OK;  
}


USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)   
{
  if(hhcd.hc[pipe].ep_is_in)
  {
    hhcd.hc[pipe].toggle_in = toggle;
  }
  else
  {
    hhcd.hc[pipe].toggle_out = toggle;
  }
  return USBH_OK; 
}

uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)   
{
  uint8_t toggle = 0;
  
  if(hhcd.hc[pipe].ep_is_in)
  {
    toggle = hhcd.hc[pipe].toggle_in;
  }
  else
  {
    toggle = hhcd.hc[pipe].toggle_out;
  }
  return toggle; 
}

void USBH_Delay(uint32_t Delay)
{
  //HAL_Delay(Delay);  
  OSTimeDly(Delay);
}

/************************ (C) COPYRIGHT EMERSON NETWORK POWER *****END OF FILE****/
