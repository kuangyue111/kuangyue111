/*
 * Safety_ADC.h
 *
 *  Created on: 2021Äê9ÔÂ7ÈÕ
 *      Author: zsy342
 */

#ifndef SAFTYCHECK_SAFETYLIB_SAFETY_ADC_H_
#define SAFTYCHECK_SAFETYLIB_SAFETY_ADC_H_

/**************************************************************************************************
 * Sample ADC internal reference channels and external reference channel to validate ADC operation.
 * Conduct the following 5 samples and compare the results with expected value:
 * VREFL
 * external channel
 * VREFH
 * external channel
 * VREFL
 **************************************************************************************************/
void ADC_PorSelfTest(void);

uint8_t GetAdcSelfTestStatus(void);

#endif /* SAFTYCHECK_SAFETYLIB_SAFETY_ADC_H_ */
