/*
 * includes.h
 *
 *  Created on: 2021.08.31
 *      Author: xry344
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include "can.h"
#include "BSP.h"
#include "I2C.h"
#include "EEPROM.h"
#include "Calibration.h"
#include "gd32f30x.h"

extern hal_data_t *const	hal_data;
extern INT32U VoltSampleCnt[2];

INT8U coTimeout					(co_time *timep, co_time ticks);
INT8U co_expired				(co_time *timep, co_time ticks);
void* co_memcpy					(void* dst, void* src, int bytes);
int		co_memcmp					(void* mb1, void *mb2, int bytes);
int		co_strcpy					(char *dst, char *src, int n);
void* co_memset					(void* dst, char ch, int bytes);
INT8U fabs							(float fNum1, float fNum2);


void	co_delay_ms				(co_long ms);


#endif /* INCLUDES_H_ */

