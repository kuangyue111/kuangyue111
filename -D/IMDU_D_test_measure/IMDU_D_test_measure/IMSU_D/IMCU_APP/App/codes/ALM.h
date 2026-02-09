#ifndef ALM_H
#define ALM_H

typedef enum SystemAlarmType
{
	SYS_ALM_SUMMARY = 0,
	SYS_ALM_AllRectCommFail,
	SYS_ALM_ACOVER,
	SYS_ALM_ACLOW,
	SYS_ALM_CardReaderCommFail,
	SYS_ALM_EPO,
	SYS_ALM_SPD,
	SYS_ALM_DOOR,
	SYS_ALM_NET_BREAK,
	ALL_SYS_ALM_NUM
}SYS_ALARM_TYPE;
typedef enum GunAlarmType
{
	GUN_ALM_SUMMARY= 0,
	GUN_ALM_Insulation,
	GUN_ALM_InsulationCommFail,
	GUN_ALM_NoNormalRect,
	GUN_ALM_WHCommAlm,
	GUN_ALM_GunOverTemAlm,
	ALL_GUN_ALM_NUM
}GUN_ALARM_TYPE;
	
struct _AlarmData
{
	UINT8  u8ALMState;
	UINT8  u8ALMFlag;
};
typedef struct _AlarmData ST_AlarmData;

struct _AlarmMng
{
	ST_AlarmData SystemAlarm[ALL_SYS_ALM_NUM];
	ST_AlarmData GunAlarm[ALL_GUN_NUM][ALL_GUN_ALM_NUM];
};
typedef struct _AlarmMng ST_AlarmMng;

extern NOINIT ST_AlarmMng  gs_AlarmMng;
#endif