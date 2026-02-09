
#ifndef __CAN_MULTI_GUN_H
#define __CAN_MULTI_GUN_H

#include "main.h"


typedef enum tagSlaveGunStat
{
	SlaveGun_Stat_ = 0,

}SLAVE_GUN_STAT;



typedef struct _SLAVE_GUN_NODE SLAVE_GUN_NODE;
struct _SLAVE_GUN_NODE
{
	SLAVE_GUN_STAT				SlaveGunNodeStat;

	BYTE						byNodeAddress;		//该枪地址
	float						fOutVolt;
	float						fOutCurr;
	float						fMaxGunAllowCurr;	//枪最大限流点(枪线缆容量)

	float						fNeedVolt;
	float						fNeedCurr;

	BYTE						nLockedMasterID;	//当作为从机时，此id表示被那把枪锁定
	time_t						tmLastRefresh;		//上次收到CMD1刷新时间
	BOOL						bCommFail;			//10s时间没收到CMD1就算CommFail

	float						fkWhReading;		//for multiple-gun mode
	BOOL						bIsCC1_GunInCar;	//for one-car-multi-gun mode

};

#define MAX_GUN_NUMBER	5
typedef struct tagSlaveGunList
{
	unsigned long u32SlaveGunTimeSec;		  //环网时间，用于延时处理 单位：秒

	//RING_MERGE_MODE		emRingMergeMode;

	int				nOnlineTotal; //实际在线的节点数目/**/

	SLAVE_GUN_NODE		allSlaveGunNodes[MAX_GUN_NUMBER];
}SLAVE_GUN_ALL_INFO;


typedef struct tagMultiGunMng
{
	unsigned long u32SlaveGunTimeSec;		  //环网时间，用于延时处理 单位：秒

	//作为主机
	

	//作为从机
	

	int					nOnlineSlaveGunTotal; //实际在线的节点数目
	SLAVE_GUN_NODE		allSlaveGunNodes[MAX_GUN_NUMBER];
}tagMultiGunMngDef;

#endif