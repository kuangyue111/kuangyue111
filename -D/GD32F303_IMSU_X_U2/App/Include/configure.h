#ifndef __config_h__
#define __config_h__

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *  Includes
 */

/*******************************************************************************
 *  Exported Constants
 */
enum
{
	e_Timer0 = 0u,
	e_Timer1,
	e_Timer2,
	e_Timer3,
	e_Timer4,
	e_Timer5,
	e_Timer6,
	e_Timer7,
	e_Timer8,
	e_Timer9,
	e_Timer10,
	e_Timer11,
	e_Timer12,
	e_Timer13,
};
	
/*******************************************************************************
 *  Exported Typedef
 */
 
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned char           co_byte;
typedef unsigned short int      co_word;
typedef unsigned long           co_long;
typedef void *                  co_vptr;

typedef unsigned int            co_regs;
typedef volatile int            co_regv;

typedef unsigned long           co_bits;
typedef unsigned long long      co_time;

typedef unsigned int            co_size;

typedef union TDWORD
{
	unsigned long						v;
	long										l;
	unsigned short int			w[2];
	signed int							s[2];
	unsigned char						b[4];
	char										c[4];
	float										f;
}
co_dword;

/*******************************************************************************
 *  Exported Macros
 */

#define CO_FALSE						(0u)
#define coWaitForever				(1u)


#define cVersion						100
#define CFG_EEPROM_VERSION	0x9900

#define CFG_DEBUG						0	//If in debugging, set 1 to close FWDOG, Normal state set 0 to open FWDOG

#define HalTimeGet_ms()			coSystemTicksClock

#define coINLINE						inline
#define coASM								__asm

#define mSec(n)							((n)*1000u)
#define mMin(n)							((n)*60000u)

#define Freq_RatioSample(x)		(x * 0.001f * 2.50625f - 1253.125f)

#define DisableInterrupts __disable_irq()
#define EnableInterrupts  __enable_irq()

#define CalibrationFactor_K1	(1.0003f)
#define CalibrationFactor_B1	(-1.7768f)
#define CalibrationFactor_K2	(0.9995f)
#define CalibrationFactor_B2	(8.9595f)


/************* CAN Config **************/
#define CFG_Baudrate_125K 	125
#define CFG_Baudrate_250K		250
#define CFG_Baudrate_500K		500
#define CFG_CAN_Baudrate		CFG_Baudrate_125K

#define CFG_CAN_MASK_BITMAP		0x3C0FF00//DecviceNo. + Destination address  (0x0D << 22 | 0xDx << 16)
#define CFG_DEFAULT_CAN_ADDR	0xD0
#define CFG_U2_DEVICENUM		0x0C
/************* CAN Config **************/

/************* EEPROM Config **************/
#define EEPROM_PAGE_SIZE				128
#define EEPROM_BUFFER_MAX				512
#define EEPROM_DEVICE_ADDR			0xA0

#define I2C_TIMEOUT							1000u
#define I2C1_SPEED							400000u
#define I2C1_SLAVE_ADDRESS			0xA0
/************* EEPROM Config **************/

/********************* EEPROM ADDRESS *********************/
enum
{
	EEPROM_cVersion												= 0x00,
	EEPROM_ADDR_4x_sample_ratio_K0								= 0x10,
	EEPROM_ADDR_4x_sample_ratio_K1								= 0x14,
	EEPROM_ADDR_4x_sample_ratio_B0								= 0x18,
	EEPROM_ADDR_4x_sample_ratio_B1								= 0x1C,

	EEPROM_ADDR_can_transmit_mode								= 0x20,
	EEPROM_ADDR_4x_can_send_period								= 0x21,
	
};

enum
{
	eState_null = 0u,
	eState_succeed,
	eState_failed,
};

/********************* EEPROM ADDRESS *********************/


/*******************************************************************************
 *  Exported Variables
 */

/*******************************************************************************
 *  Exported Functions
 */

#ifdef __cplusplus
}
#endif

#endif

///:~



