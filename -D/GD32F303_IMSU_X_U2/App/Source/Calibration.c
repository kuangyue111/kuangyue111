/*******************************************************************************
 *  Includes
 */
#include "includes.h"

/*******************************************************************************
 *  Constants
 */

/*******************************************************************************
 *  Typedef
 */

/*******************************************************************************
 *  Macros
 */

/*******************************************************************************
 *  Global Variables
 */

/*******************************************************************************
 *  External Variables
 */
co_time Calibration_SumTimer[2];
co_time Calibration_Timeout[2];

/*******************************************************************************
 *  External Functions
 */

/*******************************************************************************
 *  Local Variables
 */

/*******************************************************************************
 *  Functions Prototypes Declaration
 */

/*******************************************************************************
 *  Local Functions Declaration
 */
void Calibration_Volt											(INT8U ch);

/*******************************************************************************
 *  Public Functions Definition
 */
void Calibration_Task(void)
{
	Calibration_Volt(0);
	Calibration_Volt(1);
}
/*******************************************************************************
 *  Local Functions Definition
 */
void Calibration_Volt(INT8U ch)
{
	static INT8U steplast[2] = {0u, 0u};
	static INT16U Cnt[2] = {0u, 0u};
	static float NumSample[2]  = {0.0f, 0.0f};
	static float NumComm[2]  = {0.0f, 0.0f};
	
	if (hal_data->CalibrationInfo[ch].Working == CO_FALSE)
	{
		Cnt[ch] = 0u;
		NumSample[ch] = 0.0f;
		NumComm[ch]		= 0.0f;
		hal_data->CalibrationInfo[ch].RealVolt_1 = 0;
		hal_data->CalibrationInfo[ch].RealVolt_2 = 0;
		hal_data->CalibrationInfo[ch].step			 = 0u;
		hal_data->CalibrationInfo[ch].NeedReport = CO_FALSE;
		hal_data->CalibrationInfo[ch].Result		 = eState_null;
		return;
	}

	if (coTimeout(&Calibration_Timeout[1], mSec(15u)))
	{
		hal_data->CalibrationInfo[ch].Working = CO_FALSE;
		return;
	}

	switch (hal_data->CalibrationInfo[ch].step)
	{
		case 0:
			{
				if (coTimeout(&Calibration_SumTimer[ch], mSec(1u)))
				{
					if (Cnt[ch] < 5)
					{
						NumSample[ch] += hal_data->VoltSample_Value[ch];
						NumComm[ch]		+= hal_data->CalibrationInfo[ch].Volt_Comm1;
						Cnt[ch]++;
					}
				}

				steplast[ch] = 0u;
			}
			break;

		case 1:
			{
				if (steplast[ch] != hal_data->CalibrationInfo[ch].step)
				{
					hal_data->CalibrationInfo[ch].RealVolt_1 = NumSample[ch] / (float)Cnt[ch];
					hal_data->CalibrationInfo[ch].Volt_Comm1 = NumComm[ch]	 / (float)Cnt[ch];
					Cnt[ch]				= 0u;
					NumSample[ch] = 0.0f;
					NumComm[ch]		= 0.0f;
					steplast[ch]	= hal_data->CalibrationInfo[ch].step;
					coTimeout(&Calibration_SumTimer[ch], 0u);
				}
				
				if (coTimeout(&Calibration_SumTimer[ch], mSec(1u)))
				{
					NumSample[ch] += hal_data->VoltSample_Value[ch];
					NumComm[ch]		+= hal_data->CalibrationInfo[ch].Volt_Comm2;
					Cnt[ch]++;
				}

				if (Cnt[ch] > 4)
				{
					hal_data->CalibrationInfo[ch].RealVolt_2 = NumSample[ch] / (float)Cnt[ch];
					hal_data->CalibrationInfo[ch].Volt_Comm2 = NumComm[ch]	 / (float)Cnt[ch];
					Cnt[ch]				= 0u;
					NumSample[ch] = 0.0f;
					NumComm[ch]		= 0.0f;
					hal_data->CalibrationInfo[ch].step++;
				}
			}
			break;

		case 2:
			{
				float k = 0.0f;
				co_dword dword;

				hal_data->CalibrationInfo[ch].Result = eState_failed;
				if (hal_data->CalibrationInfo[ch].Volt_Comm2 > hal_data->CalibrationInfo[ch].Volt_Comm1)
				{
					k = (hal_data->CalibrationInfo[ch].Volt_Comm2 - hal_data->CalibrationInfo[ch].Volt_Comm1) / \
					(hal_data->CalibrationInfo[ch].RealVolt_2 - hal_data->CalibrationInfo[ch].RealVolt_1);

					if ((k > 0.9) && (k < 1.3))
					{
						hal_data->CalibrationInfo[ch].Ratio_K = k;
						hal_data->CalibrationInfo[ch].Ratio_B = hal_data->CalibrationInfo[ch].Volt_Comm2 - (k * hal_data->CalibrationInfo[ch].RealVolt_2);

						if (ch == 0u)
						{
							dword.f = hal_data->CalibrationInfo[ch].Ratio_K;
							EEPROM_WriteBuf[0] = dword.b[3];
							EEPROM_WriteBuf[1] = dword.b[2];
							EEPROM_WriteBuf[2] = dword.b[1];
							EEPROM_WriteBuf[3] = dword.b[0];
							EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_4x_sample_ratio_K0, 4u);

							dword.f = hal_data->CalibrationInfo[ch].Ratio_B;
							EEPROM_WriteBuf[0] = dword.b[3];
							EEPROM_WriteBuf[1] = dword.b[2];
							EEPROM_WriteBuf[2] = dword.b[1];
							EEPROM_WriteBuf[3] = dword.b[0];
							EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_4x_sample_ratio_B0, 4u);
						}
						else if (ch == 1u)
						{
							dword.f = hal_data->CalibrationInfo[ch].Ratio_K;
							EEPROM_WriteBuf[0] = dword.b[3];
							EEPROM_WriteBuf[1] = dword.b[2];
							EEPROM_WriteBuf[2] = dword.b[1];
							EEPROM_WriteBuf[3] = dword.b[0];
							EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_4x_sample_ratio_K1, 4u);

							dword.f = hal_data->CalibrationInfo[ch].Ratio_B;
							EEPROM_WriteBuf[0] = dword.b[3];
							EEPROM_WriteBuf[1] = dword.b[2];
							EEPROM_WriteBuf[2] = dword.b[1];
							EEPROM_WriteBuf[3] = dword.b[0];
							EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_4x_sample_ratio_B1, 4u);
						}

						hal_data->CalibrationInfo[ch].Result = eState_succeed;
					}
				}

				hal_data->CalibrationInfo[ch].NeedReport = !CO_FALSE;
				
			}
			break;
	}	

	if (hal_data->CalibrationInfo[ch].NeedReport == !CO_FALSE)
	{		
		Can_SendCalibrationResult(ch, hal_data->CalibrationInfo[ch].Result);
		Can_SendCalibrationResult(ch, hal_data->CalibrationInfo[ch].Result);
		Can_SendCalibrationResult(ch, hal_data->CalibrationInfo[ch].Result);
		hal_data->CalibrationInfo[ch].Working = CO_FALSE;
		return;
	}
}

