#include "includes.h"
#include "configure.h"

co_time coSystemTicksClock = 0u;
co_time LEDTimer;

hal_data_t L_hal_data;
hal_data_t  *const hal_data = &L_hal_data;

void BSP_data_init							(void);


void main(void)
{
	co_dword dword;
	
	BSP_Init();
	BSP_Addr_Task();
	BSP_CAN_Init(CFG_CAN_Baudrate);//Set can filter if we get U2 addr

	co_delay_ms(mSec(1));//wait eeprom hardware init

	//init ram data
	BSP_data_init();

	//Read EEPROM Configure Version
	EEPROM_load(EEPROM_ReadBuf, 0u, 2);
	dword.b[1] = EEPROM_ReadBuf[0];
	dword.b[0] = EEPROM_ReadBuf[1];

	if (dword.w[0] != CFG_EEPROM_VERSION)
	{
		EEPROM_set_default_configuration();
	}

	EEPROM_configure_load();
	
	while(coWaitForever)
	{
		//ADCSample_Task();

		BSP_Task();

		Can_Task();

		Calibration_Task();
		
#if (CFG_DEBUG == 0)
		fwdgt_counter_reload();
#endif

		if (coTimeout(&LEDTimer, mSec(1u)))
		{
			BSP_LED_TOGGLE();
		}
	}
}

INT8U coTimeout(co_time *timep, co_time ticks)
{
	INT8U		ret = CO_FALSE;
	co_time now = HalTimeGet_ms();

  if (ticks == 0u)
  {
    *timep = now;
  }
  else if ((now - *timep) >= ticks)
  {
    *timep += ticks;
    ret = !CO_FALSE;
  }

  return ret;
}

INT8U co_expired(co_time *timep, co_time ticks)
{
	INT8U		ret = CO_FALSE;
	co_time now = HalTimeGet_ms();

  if (ticks == 0)
  {
    if (now >= *timep)
    {
      ret = !CO_FALSE;
    }
  }
  else
  {
    *timep = now + ticks;
  }

  return ret;
}

void* co_memcpy(void* dst, void* src, int bytes)
{
  char  *p = (char*)dst;
  char  *q = (char*)src;

  for ( ; bytes > 0; bytes--)
  {
    *p++ = *q++;
  }

  return p;
}

int co_memcmp(void* mb1, void *mb2, int bytes)
{
  char *p = (char*)mb1;
  char *q = (char*)mb2;

  for ( ; bytes > 0; bytes--)
  {
    if (*p++ != *q++)
    {
      break;
    }
  }

  return bytes;
}

int co_strcpy(char *dst, char *src, int n)
{
  char *p = dst;
  char *q = src;
  int   i = 0;

  for (i = 0; n > 0; n--, i++)
  {
    *p++ = *q++;

    if (*q == '\0')
    {
      break;
    }
  }

  return i;
}

void* co_memset(void* dst, char ch, int bytes)
{
  char  *p = (char*)dst;

  for ( ; bytes > 0; bytes--)
  {
    *p++ = ch;
  }

  return p;
}

void co_delay_ms(co_long ms)
{
	co_long time = HalTimeGet_ms();
	while ((HalTimeGet_ms() - time) < ms);
}

INT8U fabs(float fNum1, float fNum2)
{
	fNum1 = fNum1 - fNum2;
	
	if (fNum1 < 0)
	{
		fNum1 = -fNum1;
	}
	
	if (fNum1 > 0.0001)
	{
		return TRUE;
	}
		
	return FALSE;	
}


void BSP_data_init(void)
{
	INT8U i = 0u;
	hal_data->VoltFreq[0] = 0u;
	hal_data->VoltFreq[1] = 0u;
	hal_data->CanInfo.transmit_mode = 0u;

	for (i = 0u; i < 2; i++)
	{
		hal_data->CalibrationInfo[i].NeedReport = CO_FALSE;
		hal_data->CalibrationInfo[i].Result		= CO_FALSE;
		hal_data->CalibrationInfo[i].Working	= CO_FALSE;
		hal_data->CalibrationInfo[i].step		= 0u;
	}
	
	BSP_I2C_WP_SET();//Open EEPROM write protect
}

