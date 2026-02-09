#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H
#include "stm32f4xx_hal.h"
//#ifdef __cplusplus
// extern "C" {
//#endif
#define EEPROM_PAGESIZE			128
#define EEPROM_MAX_SIZE			0x10000 
#define EEPROM_READ_TIMEOUT		((uint32_t)(1000))
#define EEPROM_WRITE_TIMEOUT		((uint32_t)(1000))
#define EEPROM_MAX_TRIALS		3000

#define EEPROM_OK			0
#define EEPROM_FAIL			1
#define EEPROM_TIMEOUT			2
#define I2C_TIMEOUT			100 
#define EEPROM_WP_ENABLE		GPIO_PIN_RESET
#define EEPROM_WP_DISENABLE		GPIO_PIN_SET

/*RTC*/
#define   SECONDSREG		0x10
#define   MINUTESREG		0x11
#define   HOURSREG		0x12
#define   WEEKDAYSREG		0x13	 
#define   DAYSREG		0x14
#define   MONTHSREG		0x15
#define   YEARSREG		0x16

	 
int BSP_EepromInit (void);
void  EEPROM_IO_Init(void);
void BSP_Eeprom_WP_Ctrl (GPIO_PinState u8CtrlStatus);
/* after write opreation Wait for EEPROM standby state */
uint32_t BSP_EEPROM_WaitEepromStandbyState(void);
uint32_t BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
uint32_t BSP_EEPROM_WriteBuffer(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint8_t BSP_WriteRtcReg(uint8_t Buffer, uint16_t WriteAddr);
uint8_t BSP_ReadRtcReg(uint8_t* pBuffer, uint16_t ReadAddr);
void I2C3_Init(void);
uint8_t BSP_WriteI2C3Reg(uint16_t DevAddress,uint8_t* pBuffer, uint16_t WriteAddr);
uint8_t BSP_ReadI2C3Reg(uint16_t DevAddress,uint8_t* pBuffer, uint16_t ReadAddr);
//#ifdef __cplusplus
//}
//#endif

#endif
