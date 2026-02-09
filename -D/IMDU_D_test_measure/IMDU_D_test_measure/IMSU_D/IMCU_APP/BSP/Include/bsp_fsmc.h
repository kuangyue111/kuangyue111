#ifndef __BSP_FSMC_H
#define __BSP_FSMC_H
#include "stm32f4xx_hal.h"

#define DOBASEADDRESS 0x64000000
#define CSKEY_OFFSET  0x00
#define CSLCD_OFFSET  0x01
#define CSLED_OFFSET  0x02
#define CSDO1_OFFSET  0x03
#define CSDO2_OFFSET  0x04
#define LCD_RW_OFFSET 0x11
#define LED_Data         CSLED
#define KEY_Data         CSKEY
#define LCD_CMD_Data     CSLCD
#define LCD_RW_Data    (*(__IO uint8_t*) (DOBASEADDRESS + LCD_RW_OFFSET))
#define SRAM_TIMEOUT     ((uint32_t)0xFFFF)

#define CSKEY         (*(__IO uint8_t*) (DOBASEADDRESS+CSKEY_OFFSET))
#define CSLCD         (*(__IO uint8_t*) (DOBASEADDRESS+CSLCD_OFFSET))
#define CSLED         (*(__IO uint8_t*) (DOBASEADDRESS+CSLED_OFFSET))
#define CSDO1         (*(__IO uint8_t*) (DOBASEADDRESS+CSDO1_OFFSET))
#define CSDO2         (*(__IO uint8_t*) (DOBASEADDRESS+CSDO2_OFFSET))



int BSP_FsmcInit(void);
#endif
