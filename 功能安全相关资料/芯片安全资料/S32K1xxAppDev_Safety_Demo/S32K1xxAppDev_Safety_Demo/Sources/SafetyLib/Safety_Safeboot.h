/*
 * Safety_Safeboot.h
 *
 *  Created on: Jun 26, 2019
 *      Author: B33124
 */

#ifndef SAFETYLIB_SAFETY_SAFEBOOT_H_
#define SAFETYLIB_SAFETY_SAFEBOOT_H_

#include "cpu.h"

#define NUM_OF_SAFE_BLOCKS		4
#define SAFE_BLOCK0_START_ADDR	0x00000000
#define SAFE_BLOCK0_SIZE		0x00000400
#define SAFE_BLOCK1_START_ADDR	0x00000420
#define SAFE_BLOCK1_SIZE		0x00010000
#define SAFE_BLOCK2_START_ADDR	0x00000420
#define SAFE_BLOCK2_SIZE		0x00000000
#define SAFE_BLOCK3_START_ADDR	0x00000420
#define SAFE_BLOCK3_SIZE		0x00000000

//#define CRC32_SAVE_ADDR			0x00000410
extern const uint32_t safeCrcRefValue;

extern uint32_t safeBootNewMcu;
extern uint32_t safeBootCheckSum[2];
extern uint32_t safeBootSize[NUM_OF_SAFE_BLOCKS];
extern uint32_t safeBootStartAddr[NUM_OF_SAFE_BLOCKS];

uint32_t Safeboot_CrcCalc(void);
void Safeboot_Init(void);
uint32_t Safeboot_StartCrcDma(void);


#endif /* SAFETYLIB_SAFETY_SAFEBOOT_H_ */
