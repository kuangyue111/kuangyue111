#ifndef ALM_H
#define ALM_H

#define GUN_CONNECT		0x01
#define GUN_DISCONNECT	0x00
enum
{
	PT1000_1_VALUE = 0,
	PT1000_2_VALUE ,
        ALL_PT1000_NUM =2,
	BUSVOLT_1_VALUE =2 ,
	BUSVOLT_2_VALUE ,
	ALL_Data_Type_NUM,
};
typedef struct tagMeasureData
{
	HANDLE			hMutexReadWriteMeasureData;
	UCHAR			ucIsMeasureOneLoop;
	UCHAR	        ucSlowMeasureStep[2];
	UCHAR			ucDIState[2];
	UCHAR			ucGunState[ALL_GUN_NUM];
	float			fCC1Volt[ALL_GUN_NUM];
	float			fRawBattVolt[ALL_GUN_NUM];
	float			fBattVolt[ALL_GUN_NUM];
	float			fHallVolt;
	float			fAC_CC;
	float			fSystemTemp;
	float			fPT1000Temp[ALL_PT1000_NUM];
	float			fPressureSensorVolt;
	float			fAC_CC_Volt;
	float			fAC_CC_Resistant;
	OUTUCHAR_INTERFACE_PROC		fInitMutex;
	OUTUCHAR_INTERFACE_PROC		fIsMeasureOneLoop;
	VOID_INTERFACE_PROC			fClearMeasureOneLoop;
	INUCHAR_OUTUCHAR_INTERFACE_PROC	fGetDIState;


}MeasureData_TypeDef;
extern MeasureData_TypeDef gMeasureInfo;
#endif