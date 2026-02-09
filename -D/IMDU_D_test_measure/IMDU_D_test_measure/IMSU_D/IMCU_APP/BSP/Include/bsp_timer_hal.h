

/*
  *****************************************************************************
  *
  * Name:bsp_timer_hal.h
  * Function: 	         
  * Version: 					                     
  * Update:					                           
  * Date: 			                       
  * Designer:                             			
  * Modified Date:                            	
  * Modified Engineer:				                  
  *
  *****************************************************************************
*/

#ifndef __BSP_TIMER_HAL_H
#define __BSP_TIMER_HAL_H

// ================* Include Files ************************
#include "includes.h"

// ================* Function Prototypes ******************
void bsp_InitGenTimer(void);
void bsp_GenTimer_Enable(void);
void bsp_GenTimer_Disable(void);
void bsp_Gen_Timer_ISR(void);
void bsp_CfgGenTimerISR(void);

#endif // __BSP_TIMER_H
// End of file
