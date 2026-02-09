
#ifndef __CAN_2_COMMON
#define __CAN_2_COMMON

#define STATE_INIT_DATA_BEFORE_CHARGE	0
#define STATE_Wait_SelfTest				1
#define STATE_CHARGING					2
#define STATE_STOP_CHARGING				3
#define TEST_CHARGE_CHECK_CCS		4
#define TEST_STOP_CHECK_CST			5
#define TEST_STOP_CHECK_CSD			6
#define TEST_TIMEOUT_BEM			7
#define MACHINE_ID_MAX_NUM			8

#define CHAdeMO_MACHINE_ID_MAX_NUM			4

UCHAR CAN2_Read_AllBuf(UINT8 u8ReadPF,UINT8 u8Priority);
UINT8 GUN_B_Send_Standard_Frame(UINT8 u8CMDID,UINT8 u8Len);
void GUN_B_CHAdeMOProtocolTestProcess(void);
void InitCHAdeMO_BMS_Machine(void);
void InitCHAdeMOCMDdata(void);
UCHAR GUN_B_AgingModeProcess(void);
void GUN_A_TestRecordProcess(void);
void GUN_B_TestRecordProcess(void);
void GUN_B_TestRecordStartProcess(void);
#endif
