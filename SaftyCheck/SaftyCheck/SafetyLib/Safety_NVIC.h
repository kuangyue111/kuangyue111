/*
 * Safety_NVIC.h
 *
 *  Created on: 2021Äê9ÔÂ14ÈÕ
 *      Author: zsy342
 */

#ifndef SAFTYCHECK_SAFETYLIB_SAFETY_NVIC_H_
#define SAFTYCHECK_SAFETYLIB_SAFETY_NVIC_H_

uint8_t GetNvicSelfTestStatus(void);

void SetNvicSelfTestStatus(uint8_t flag);

#endif /* SAFTYCHECK_SAFETYLIB_SAFETY_NVIC_H_ */
