/*
 * Safety_FaultSave.h
 *
 *  Created on: May 4, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_FAULTSAVE_H_
#define SAFETYLIB_SAFETY_FAULTSAVE_H_

#include "cpu.h"

typedef enum fault_type
{
	faultPOR 		= 0,
	faultLVD 		= 1,
	faultWDOG		= 2,
	faultSafetyBoot = 3,
	faultLOC		= 4,
	faultLOL		= 5,
	faultCMULOC		= 6,
	faultResetPIN	= 7,
	faultLockup		= 8,
	faultFlashEcc	= 9,
	faultFlashProt	= 10,
	faultSramEcc	= 11,
	faultSmpu		= 12,
	faultHVD		= 13,
	faultNVIC		= 14,
	faultCoreSCST 	= 15,
	faultStackOverflow	= 16,
	faultSelfTest		= 17,
	faultRegCheck 		= 18,
	faultAdcDiagnostic	= 19,
	faultHardFault 		= 20,
	faultClockConfig	= 21,
	faultUnknown		= 22
}fault_t;

typedef struct fault_info_type
{
	fault_t faultType;
	uint32_t faultPcAddr;
	uint32_t faultDataAddr;
}fault_info_t;

#define FAULT_POR_MASK			0x00000001
#define FAULT_LVD_MASK			0x00000002
#define FAULT_WDOG_MASK			0x00000004
#define FAULT_SAFETYBOOT_MASK	0x00000008
#define FAULT_LOC_MASK			0x00000010
#define FAULT_LOL_MASK			0x00000020
#define FAULT_CMULOC_MASK		0x00000040
#define FAULT_RESETPIN_MASK		0x00000080
#define FAULT_LOCKUP_MASK		0x00000100
#define FAULT_FLASHECC_MASK		0x00000200
#define FAULT_FLASHPROT_MASK	0x00000400
#define FAULT_SRAMECC_MASK		0x00000800
#define FAULT_SMPU_MASK			0x00001000
#define FAULT_HVD_MASK			0x00002000
#define FAULT_NVIC_MASK			0x00004000
#define FAULT_CORESCST_MASK		0x00008000
#define FAULT_STACKOVER_MASK	0x00010000
#define FAULT_STACKUNDER_MASK	0x00020000
#define FAULT_ADCDIAG_MASK		0x00040000
#define FAULT_REGPROT_MASK		0x00080000
#define FAULT_UNKNOWN_MASK		0x00100000
#define FAULT_SRAMECCSELFTEST_MASK		0x00200000
#define FAULT_FLASHECCSELFTEST_MASK		0x00400000
#define FAULT_HARDFAULT_MASK			0x00800000
#define FAULT_CLOCKFREQ_MASK			0x01000000

#define FAULT_MAX_COUNT				20

extern uint32_t faultEepromReady;
extern fault_info_t faultBuffer;
//extern fault_info_t faultList[FAULT_MAX_COUNT];
#ifndef faultList
#define faultList ((fault_info_t *)0x14000008)
#endif
extern flash_ssd_config_t mySSDConfig;

status_t Safety_FaultManage_Init(void);
status_t Safety_FaultManage_SaveFault(fault_info_t * pFaultInfo);
status_t Safety_FaultManage_ClearAllFault(void);
uint32_t Safety_FaultManage_GetFaultCount(void);
uint32_t Safety_FaultManage_GetFaultBitmap(void);
status_t Safety_FaultManage_GetFaultInfo(fault_info_t *pFault, uint32_t numFault);
status_t Safety_FaultManage_ProcessResetFault(void);
status_t Safety_FaultManage_ClearSingleFault(fault_t faultToClear);

#endif /* SAFETYLIB_SAFETY_FAULTSAVE_H_ */
