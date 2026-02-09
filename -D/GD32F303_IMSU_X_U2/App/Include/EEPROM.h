#ifndef __EEPROM_h__
#define __EEPROM_h__

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *  Includes
 */
#include "gd32f30x.h"
#include "configure.h"

/*******************************************************************************
 *  Exported Constants
 */
enum
{
	eState_I2C_Start = 0,
	eState_I2C_SendAddress,
	eState_I2C_ClearAddrFlag,
	eState_I2C_Transmit,
	eState_I2C_Stop,

	eState_I2C_StateMax,
};
	
/*******************************************************************************
 *  Exported Typedef
 */

/*******************************************************************************
 *  Exported Macros
 */

/*******************************************************************************
 *  Exported Variables
 */
extern INT8U EEPROM_ReadBuf[EEPROM_BUFFER_MAX];
extern INT8U EEPROM_WriteBuf[EEPROM_BUFFER_MAX];
/*******************************************************************************
 *  Exported Functions
 */
void EEPROM_write																					(INT8U *WriteBuf, INT16U WriteAddress, INT16U Length);
void EEPROM_load																					(INT8U *ReadBuf,	INT16U ReadAddress,	 INT16U Length);
void EEPROM_set_default_configuration											(void);
void EEPROM_configure_load																(void);





#ifdef __cplusplus
}
#endif

#endif

///:~


