/*
 * Safety_FaultSave.c
 *
 *  Created on: May 4, 2019
 *      Author: B33124
 */

#include "Safety_FaultSave.h"
#include "Safety_RCM.h"

fault_info_t faultBuffer;

flash_ssd_config_t mySSDConfig;

uint32_t faultEepromReady = 0;

/********************************************************************************
 * S32DS PE Debugger cannot handle EEPROM section, may have error during download.
 * So changed to MACRO definition so that debugger needn't write data to EEPROM area.
 ********************************************************************************/
//uint32_t faultCountEeprom __attribute__((section (".eeprom_section")));
#define faultCountEeprom (*(uint32_t *)0x14000000)
//It records the number of faults occurred on this MCU

//uint32_t faultBitMap __attribute__((section (".eeprom_section")));
#define faultBitMap (*(uint32_t *)0x14000004)
//Each bit of this variable indicates one fault

//fault_info_t faultList[FAULT_MAX_COUNT] __attribute__((section (".eeprom_section")));
//#ifndef faultList
//#define faultList ((fault_info_t *)0x14000008)
//#endif
//each element of this array contains information of one fault

/********************************************************************************************
 *
 * Function description:
 * 1) Wait till EEPROM is ready and previous command is completed;
 * 2) If EEPROM is not initialized, execute the Flash partition command for EEPROM;
 * 2.1) after EEPROM initialization, set faultCountEeprom to 0 in EEPROM;
 * 3) if EEPROM is already initialized, read out the EEPROM variable and store at faultCount;
 * 4) Check reset source, and save the fault information if the previous reset indicate a fault occurred;
 ********************************************************************************************/
status_t Safety_FaultManage_Init(void)
{
	status_t ret;

	ret = FLASH_DRV_Init(&Flash1_InitConfig0, &mySSDConfig);
	if(ret != STATUS_SUCCESS)
		return ret;
	if(mySSDConfig.EEESize == 0)	//EEPROM is not initialized on this MCU
	{
		INT_SYS_DisableIRQGlobal();
		ret = FLASH_DRV_DEFlashPartition(&mySSDConfig,
									2,		//uEEEDataSizeCode=2, means 4K FlexRAM for EEPROM
									8, 		//uDEPartitionCode=4, means 64K dataflash all used for EEPROM, no pure dataflash.
									0, 		//uCSEcKeySize=0, no cSEC key
									0, 		//uSFE
									1);		//flexRamEnableLoadEEEData
		INT_SYS_EnableIRQGlobal();
		Safety_FaultManage_ClearAllFault();
	}
	if((0U != (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK)) && (0U != (FTFx_FCNFG & FTFx_FCNFG_EEERDY_MASK)))
		faultEepromReady = 1;		//EEPROM is ready for fault recording
	else
		faultEepromReady = 0;		//EEPROM not ready for fault recording

	if( faultEepromReady == 1)
	{
		if(Safety_FaultManage_GetFaultCount() > FAULT_MAX_COUNT)
		{
			Safety_FaultManage_ClearAllFault();
		}
	}

	return ret;
}

/********************************************************************************
 * Check reset source, determine whether it's a fault caused reset;
 *
 ********************************************************************************/
status_t Safety_FaultManage_ProcessResetFault(void)
{
	reset_source_t resetSourceTmp;
	fault_t faultTmp;
	status_t ret = STATUS_SUCCESS;
	resetSourceTmp = RCM_ReadResetSource();

	//convert reset source to fault type
	switch(resetSourceTmp)
	{
		case ResetSourceLVD:
			faultTmp = faultLVD;
			break;
		case ResetSourceLOC:
			faultTmp = faultLOC;
			break;
		case ResetSourceLOL:
			faultTmp = faultLOL;
			break;
		case ResetSourceCMULOC:
			faultTmp = faultCMULOC;
			break;
		case ResetSourceWDOG:
			faultTmp = faultWDOG;
			break;
		case ResetSourcePIN:
			faultTmp = faultResetPIN;
			break;
		case ResetSourceLOCKUP:
			faultTmp = faultLockup;
			break;
		case ResetSourcePOR:
			faultTmp = faultPOR;
			break;
		default:
			faultTmp = faultPOR;	//means this reset is not a fault
	}

	//customer can decide what faults need be saved into EEPROM
	if ( (faultTmp != faultPOR ) && ( faultTmp != faultResetPIN ))
	{
		//if( ((1 << (uint32_t)faultTmp) & faultBitMap) == 0 ) //if this type of fault is already recorded in fault bitmap, and it's not cleared yet, don't add the same fault again.
		{
			faultBuffer.faultType = faultTmp;
			faultBuffer.faultPcAddr = 0;
			faultBuffer.faultDataAddr = 0;
			ret = Safety_FaultManage_SaveFault(&faultBuffer);
			if(ret != STATUS_SUCCESS)
				return ret;
		}
	}
	return ret;
}

/*
 * Description:
 * This function saves fault information into EEPROM;
 * And increment the fault count if it's less than the MAX value;
 * If the faults list in EEPROM is full, this function will over-write the last fault;
 * */
status_t Safety_FaultManage_SaveFault(fault_info_t * pFaultInfo)
{
	uint32_t tmp;
	status_t ret = STATUS_SUCCESS;
	if(((uint32_t)pFaultInfo->faultType) < 32)		//max 32 types of faults can be recorded
	{
		//tmp = (1<<((uint32_t)pFaultInfo->faultType))|faultBitMap;
		tmp = 1<<((uint32_t)pFaultInfo->faultType);
		//if((tmp & faultBitMap) == 0)		//if this fault is not recorded yet in faultBitMap, then record it.
		{
			tmp = tmp |faultBitMap;
			ret = FLASH_DRV_EEEWrite(&mySSDConfig,
								(uint32_t)&faultBitMap, //FAULT_COUNT_EEPROM_ADDR, 	//dest address
								4, 		//data size
								(uint8_t *)(&tmp));	//source data pointer
			tmp = Safety_FaultManage_GetFaultCount();
			if(tmp < FAULT_MAX_COUNT)
			{
				FLASH_DRV_EEEWrite(&mySSDConfig,
						(uint32_t)(&(faultList[tmp])),//FAULT_LIST_START_ADDR + faultCount * sizeof(faultBuffer), 	//dest address
							sizeof(fault_info_t), 		//data size
							(uint8_t *)(pFaultInfo));	//source data pointer

				tmp++;
				FLASH_DRV_EEEWrite(&mySSDConfig,
							(uint32_t)&faultCountEeprom, //FAULT_COUNT_EEPROM_ADDR, 	//dest address
							4, 		//data size
							(uint8_t *)(&tmp));	//source data pointer
			}
			else if(FAULT_MAX_COUNT == tmp)		//if already max number of faults are saved in EEPROM, over-write the last fault;
			{
				FLASH_DRV_EEEWrite(&mySSDConfig,
							(uint32_t)(&(faultList[tmp-1])),//FAULT_LIST_START_ADDR + faultCount * sizeof(faultBuffer), 	//dest address
							sizeof(fault_info_t), 		//data size
							(uint8_t *)(pFaultInfo));	//source data pointer
			}
			else	//if going to this branch, means fault counter is not correct, need clear the fault counter;
			{
				Safety_FaultManage_ClearAllFault();
			}
		}
	}
	return ret;
}

/************************************************************************************
 * Description:
 * This function clear the fault count to zero and save in EEPROM;
 * This function clear the fault bitmap to zero and save in EEPROM;
 **************************************************************************************/
status_t Safety_FaultManage_ClearAllFault(void)
{
	uint32_t tmp = 0;
	status_t ret = STATUS_SUCCESS;
	//clear fault count to zero
	ret = FLASH_DRV_EEEWrite(&mySSDConfig,
						(uint32_t)&faultCountEeprom,	//FAULT_COUNT_EEPROM_ADDR, 	//dest address
						4, 		//data size
						(uint8_t *)(&tmp));	//source data pointer
	//clear fault bitmap to zero
	ret = FLASH_DRV_EEEWrite(&mySSDConfig,
						(uint32_t)&faultBitMap,	//FAULT_COUNT_EEPROM_ADDR, 	//dest address
						4, 		//data size
						(uint8_t *)(&tmp));	//source data pointer
	return ret;
}

/************************************************************************************
 * Description:
 * This function clear the specified bit in faultBitMap to allow this type of fault be recorded in future.
 **************************************************************************************/
status_t Safety_FaultManage_ClearSingleFault(fault_t faultToClear)
{
	uint32_t tmp;
	status_t ret = STATUS_SUCCESS;
	tmp = (uint32_t)faultToClear;
	if(tmp>=32)
		return STATUS_ERROR;
	tmp = 1<<(tmp);
	if(tmp & faultBitMap)	//if this fault is set in faultBitMap
	{
		tmp = (~tmp)&faultBitMap;
		ret = FLASH_DRV_EEEWrite(&mySSDConfig,
							(uint32_t)&faultBitMap, //FAULT_COUNT_EEPROM_ADDR, 	//dest address
							4, 		//data size
							(uint8_t *)(&tmp));	//source data pointer
	}
	return ret;
}

/****************************************************************************************
 * Return current fault count saved in EEPROM
 ****************************************************************************************/
uint32_t Safety_FaultManage_GetFaultCount(void)
{
	return faultCountEeprom;	//(*(uint32_t *)FAULT_COUNT_EEPROM_ADDR);
}

/****************************************************************************************
 * Return current fault bitmap saved in EEPROM
 ****************************************************************************************/
uint32_t Safety_FaultManage_GetFaultBitmap(void)
{
	return faultBitMap;	//(*(uint32_t *)FAULT_COUNT_EEPROM_ADDR);
}

/*****************************************************************************************
 * Get specified fault (struct faultList[numFaunt] in EEPROM) info and save in buffer pointed by pFault
 ****************************************************************************************/
status_t Safety_FaultManage_GetFaultInfo(fault_info_t *pFault, uint32_t numFault)
{
	uint32_t tmpcnt;
	tmpcnt = Safety_FaultManage_GetFaultCount();
	if(tmpcnt > FAULT_MAX_COUNT)
		return STATUS_ERROR;
	if(numFault > tmpcnt)
		return STATUS_ERROR;
	pFault->faultType = faultList[numFault].faultType;
	pFault->faultPcAddr = faultList[numFault].faultPcAddr;
	pFault->faultDataAddr = faultList[numFault].faultDataAddr;
	return STATUS_SUCCESS;
}
