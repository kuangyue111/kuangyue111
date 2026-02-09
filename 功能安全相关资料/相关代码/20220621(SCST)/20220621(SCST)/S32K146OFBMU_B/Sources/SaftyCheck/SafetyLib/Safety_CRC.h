#ifndef SAFETYLIB_SAFETY_CRC_H__
#define SAFETYLIB_SAFETY_CRC_H__

#include "cpu.h"
#include "crc1.h"

#define MY_CRC16_POLY           0x1021
#define MY_CRC32_POLY           0x04C11DB7

#define MY_CRC_INIT_16BIT       0xFFFF
#define MY_CRC_INIT_32BIT       0xFFFFFFFF

#define CRC_TYPE_CRC16          1
#define CRC_TYPE_CRC32          2

#define CRC_INPUT_SIZE_BYTE           1
#define CRC_INPUT_SIZE_HALFWORD       2
#define CRC_INPUT_SIZE_WORD           4

#define CRC_CALC_STATUS_IDLE    0
#define CRC_CALC_STATUS_BUSY    1
#define CRC_CALC_STATUS_DONE    2

#define CRC_ERROR_STATUS_NO_ERROR       0
#define CRC_ERROR_STATUS_HW_BUSY        1
#define CRC_ERROR_STATUS_WRONG_PARA     2

extern uint32_t u32CrcDmaRslt;
extern uint32_t bCrcInUse;

uint16_t CRC_Crc16SoftSlow(uint8_t *pDataSrc, uint32_t u32Size, uint16_t u16InitData);
uint16_t CRC_Crc16Hw(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg);
uint32_t CRC_Crc16HwDma(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg, uint32_t u32DmaChNo);
void DMA_Ch1Callback(void *parameter, edma_chn_status_t status);

uint32_t CRC_Crc32SoftSlow(uint8_t *pDataSrc, uint32_t u32Size, uint32_t u32InitData);
uint32_t CRC_Crc32Hw(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg);
uint32_t CRC_Crc32HwDma(uint8_t *pDataSrc, uint32_t u32Size, const crc_user_config_t *pCrcUsrCfg, uint32_t u32DmaChNo);

void CRC_SelfTest(void);

uint8_t GetCrcSelfTestStatus(void);

#endif
