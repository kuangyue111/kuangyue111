#include "bsp_fsmc.h"

SRAM_HandleTypeDef hsram;
FSMC_NORSRAM_TimingTypeDef SRAM_Timing;

int BSP_FsmcInit(void)
{    
  hsram.Instance  =FSMC_NORSRAM_DEVICE;
  hsram.Extended  =FSMC_NORSRAM_EXTENDED_DEVICE;
  
  SRAM_Timing.AddressSetupTime       = 4;
  SRAM_Timing.AddressHoldTime        = 4;
  SRAM_Timing.DataSetupTime          = 15;
  SRAM_Timing.BusTurnAroundDuration  = 15;
  SRAM_Timing.CLKDivision            = 2;
  SRAM_Timing.DataLatency            = 2;
  SRAM_Timing.AccessMode             = 0;
  
  hsram.Init.NSBank             = 2;
  hsram.Init.DataAddressMux     = 0;
  hsram.Init.MemoryType         = 0;
  hsram.Init.MemoryDataWidth    = 0;
  hsram.Init.BurstAccessMode    = 0;
  hsram.Init.WaitSignalPolarity = 0;
  hsram.Init.WrapMode           = 0;
  hsram.Init.WaitSignalActive   = 0;
  hsram.Init.WriteOperation     = 0x1000;
  hsram.Init.WaitSignal         = 0;
  hsram.Init.ExtendedMode       = 0;
  hsram.Init.AsynchronousWait   = 0;
  hsram.Init.WriteBurst         = 0;

  if(HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  hsram.Init.NSBank= 0;  
  hsram.Init.MemoryDataWidth= 0x10;
  if(HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing) != HAL_OK)
  {
    return HAL_ERROR;
  }  

  return HAL_OK;
}
