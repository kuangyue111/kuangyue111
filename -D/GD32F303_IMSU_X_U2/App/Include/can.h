#ifndef __can_h__
#define __can_h__

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *  Includes
 */
#include "gd32f30x.h"
#include "configure.h"

/*******************************************************************************
 *  Exported Constants
 */
 
#define CAN_BUFFER_LENGTH					5	/*!< CAN Tx or Rx buffer max length */
#define CAN_TX_BUFFER_LENGTH			30	/*!< CAN Tx or Rx buffer max length */
#define CAN_RX_BUFFER_LENGTH			30	/*!< CAN Tx or Rx buffer max length */

/*******************************************************************************
 *  Exported Typedef
 */
 
//CAN数据结构体
typedef struct
{
	INT8U	Len;
	INT8U Data[8];
	
	INT32U ID; 
}t_CANMSG, *t_CANMSGPtr;

/*******************************************************************************
 *  Exported Macros
 */

/*******************************************************************************
 *  Exported Variables
 */
extern co_time Calibration_SumTimer[2];
extern co_time Calibration_Timeout[2];

/*******************************************************************************
 *  Exported Functions
 */
void Can_Task													(void);
void BSP_CAN_Init 										(INT16U BaudRate);
void Can_SendCalibrationResult				(INT8U ch, INT8U Result);


#ifdef __cplusplus
}
#endif

#endif

///:~

