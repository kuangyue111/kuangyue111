/*
 * Safety_RegCheck.c
 *
 *  Created on: Jun 26, 2019
 *      Author: B33124
 */


#include "crc1.h"
#include "crc_driver.h"
#include "Safety_CRC.h"
#include "Safety_RegCheck.h"
uint16_t regPortPcrCrcBaseValue = 0;		//base crc value is calculated after PORT initialization
uint16_t regPortPcrCrcRuntimeValue = 0;		//runtime crc value is calculated periodically in safety diagnostic task
//uint16_t regPortFaultDetected = 0;			//bool var to indicate PORT registers modification detected

uint8_t PortSelfTestStatus = 0;    /*1:Õý³£   0£ºÒì³£*/
/***********************************************************************************
 * Calculate the CRC16 value of specified registers.
 * This demo includes only the PORT PCR registers.
 * Customer can add more registers per safety requirements: SCG, PCC, ADC, FTM, and so on.
 *
 ***********************************************************************************/
uint16_t RegCheck_CrcCalc(void)
{
	uint16_t i;
	uint16_t *ptr;
	uint32_t ret;

	if(bCrcInUse)		//check whether CRC is in use, to avoid conflicts.
		return 0;
	else
		bCrcInUse = 1;

	CRC_DRV_Init(INST_CRC1, &crc1_InitConfig0);	//calculate CRC16 for PORT PCR registers

	ptr = (uint16_t *)(&PORTA->PCR[0]);		//get PORTA PCR[0] register address
	for(i = 0; i < 32; i++)
	{
		CRC_DRV_WriteData(INST_CRC1, (uint8_t *)ptr, 2);
		ptr += 2;
	}

	ptr = (uint16_t *)(&PORTB->PCR[0]);		//get PORTB PCR[0] register address
	for(i = 0; i < 32; i++)
	{
		CRC_DRV_WriteData(INST_CRC1, (uint8_t *)ptr, 2);
		ptr += 2;
	}

	ptr = (uint16_t *)(&PORTC->PCR[0]);		//get PORTC PCR[0] register address
	for(i = 0; i < 32; i++)
	{
		CRC_DRV_WriteData(INST_CRC1, (uint8_t *)ptr, 2);
		ptr += 2;
	}

	ptr = (uint16_t *)(&PORTD->PCR[0]);		//get PORTD PCR[0] register address
	for(i = 0; i < 32; i++)
	{
		CRC_DRV_WriteData(INST_CRC1, (uint8_t *)ptr, 2);
		ptr += 2;
	}

	ptr = (uint16_t *)(&PORTE->PCR[0]);		//get PORTE PCR[0] register address
	for(i = 0; i < 32; i++)
	{
		CRC_DRV_WriteData(INST_CRC1, (uint8_t *)ptr, 2);
		ptr += 2;
	}
	ret = CRC_DRV_GetCrcResult(INST_CRC1);
	bCrcInUse = 0;
	return ret;
}

/***********************************************************************************
 * Calculate PORT PCR registers (lower 16bit) CRC value and store into specified variable as reference value.
 * The reference value is from registers that are just initialized by software.
 ***********************************************************************************/
void RegCheck_Init(void)
{
	if(regPortPcrCrcBaseValue == 0)		//if reg crc value is not initialized, run initialization.
		regPortPcrCrcBaseValue = RegCheck_CrcCalc();
}
uint32_t  PorttimerCounter = 0;
/***********************************************************************************
 * Calculate PORT PCR registers (lower 16bit) CRC value and compare with the reference value.
 * Input parameter u16CrcValue is the reference CRC16 value.
 ***********************************************************************************/
void RegCheck_Compare(uint16_t u16CrcValue)
{
	regPortPcrCrcRuntimeValue = RegCheck_CrcCalc();
	if(regPortPcrCrcRuntimeValue == u16CrcValue)	//CRC value matches
	{

		PortSelfTestStatus = 0;
		PorttimerCounter++;
		if(PorttimerCounter > 20)
		{
			PorttimerCounter = 30;
			PortSelfTestStatus = 0;
		}
	}
	else
	{
//		//if( 0 == regPortFaultDetected )
//		if( (Safety_FaultManage_GetFaultBitmap() & (1<<faultRegCheck)) == 0 )
//		{
//			faultBuffer.faultType = faultRegCheck;
//			faultBuffer.faultPcAddr = 0;
//			faultBuffer.faultDataAddr = 0;
//			Safety_FaultManage_SaveFault(&faultBuffer);
//			Safety_RegCheck_Callback();
//		}
		PortSelfTestStatus = 1;			//CRC value doesn't match, register value error detected
	}
}

uint8_t GetPortSelfTestStatus(void)
{
	return(PortSelfTestStatus);
}

/*******************************************************************************************
 * Lock all registers that have read-only capability.
 * It's recommended to do this in Processor Expert Component settings
 *******************************************************************************************/
void RegCheck_EnableHardLock(void)
{
	//WDOG no update;
	//PORT registers lock;
	//Clock registers lock;
	//FTM
}

/*******************************************************************************************
 * Change the DBG bit of the peripherals registers so that the modules continue to run in debug mode
 * It's recommended to do this in Processor Expert Component settings
 *******************************************************************************************/
void RegCheck_DisableDebugMode(void)
{
	//WDOG
	//LPIT timer
	//LPTMR timer
	//FTM
	//CAN
	//LPSPI
	//LPI2C
}
