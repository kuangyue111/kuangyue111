/*
 * Safety_RCM.h
 *
 *  Created on: May 3, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_RCM_H_
#define SAFETYLIB_SAFETY_RCM_H_

#include "cpu.h"

#define RCM_SRS_CMULOC_MASK	0x10

#define	SELFTEST_PASS_WDOG		0x00000001
#define	SELFTEST_PASS_FLASH_ECC	0x00000002
#define	SELFTEST_PASS_SRAML_ECC	0x00000004
#define SELFTEST_PASS_SRAMU_ECC	0x00000008
#define	SELFTEST_PASS_MPU_INJ	0x00000010
#define	SELFTEST_PASS_XOSC		0x00000020
#define	SELFTEST_PASS_SPLL		0x00000040
#define	SELFTEST_PASS_ADC0		0x00000080
#define	SELFTEST_PASS_ADC1		0x00000100
#define	SELFTEST_PASS_CRC		0x00000200
#define	SELFTEST_PASS_SCST		0x00000400

#define SELFTEST_PASS_ALL		0x000007FF


typedef enum reset_source_type
{
	ResetSourceLVD = 1,
	ResetSourceLOC,
	ResetSourceLOL,
	ResetSourceCMULOC,
	ResetSourceWDOG,
	ResetSourcePIN,
	ResetSourcePOR,
	ResetSourceJTAG,
	ResetSourceLOCKUP,
	ResetSourceSoft,
	ResetSourceMDMAP,
	ResetSourceWDOGTest,
	ResetSourceSACKERR,
	ResetSourceReserved		//=14
} reset_source_t;



extern reset_source_t resetSourceVar;
extern uint32_t resetCountVar;
extern uint32_t selfTestStatus;	//each bit represents a self-test status - WDOG, Flash, SRAM, MPU, XOSC, SPLL, ADC0, ADC1

reset_source_t RCM_GetResetSrouce(void);
void RCM_SaveResetSource(void);
reset_source_t RCM_ReadResetSource(void);
uint32_t RCM_GetResetCnt(void);
void RCM_EnableInt(void);

#endif /* SAFETYLIB_SAFETY_RCM_H_ */
