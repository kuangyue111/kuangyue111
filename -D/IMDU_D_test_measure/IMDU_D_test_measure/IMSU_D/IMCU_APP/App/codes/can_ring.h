
#ifndef __CAN_RING_MAIN_H
#define __CAN_RING_MAIN_H

#include "main.h"
//#include "can_rect_sampler_main.h"


typedef enum tagRingMergeStat
{
	STAT_NO_MERGE = 0,
	STAT_PENDING,
	STAT_MERGING,
	STAT_NOMERGE_GUNIN,
}RINGNODE_MERGE_STAT;

typedef enum tagRingLockStat
{
	_LOCK_NO_LOCK = 0,
	_LOCK_AS_MASTER,
	_LOCK_AS_SLAVE, //1~32枪Idx来区分作为哪个枪的从机/**/
	_LOCK_NOT_AVAILABLE,
}RINGNODE_LOCK_STAT;

typedef struct _RING_NODE RING_NODE;
struct _RING_NODE
{
	//Basic info(from CMD1)
	BYTE						byNodeAddress; //该枪地址/**/
	float						fOutVolt;
	float						fOutCurr;
	float						fMaxMergeCurr;
	RINGNODE_MERGE_STAT		emMergeStatus; //0--No merge, 1--Pending, 2--Merging
	RINGNODE_LOCK_STAT		emLockState;
	BYTE						nLockedMasterID; //当作为从机时，此id表示被那把枪锁定/**/
	BOOL						bIsNeedExtCurr;//当作为从机或主机时，根据限流点判断，该枪还是否需要旁边的枪来并机，以此优化和决定并机和取消并机的时机/**/

	time_t					tmLastRefresh; //上次收到CMD1刷新时间/**/
	BOOL						bCommFail; //10s时间没收到CMD1就算CommFail

	BOOL						bIsExist; //是否存在，收到过数据即认为存在 0-不存在 1-存在

	float						fkWhReading; //for multiple-gun mode
	BOOL						bIsCC1_GunInCar; //for one-car-multi-gun mode

	//以下是作为主机时，功率分配信息/**/
	float						fBMSReqMaxPower; //BMS需求的最大功率,根据电池电压*需求电流计算/**/
	float						fCalcLeftMaxPower; //计算公式: [本级设置的总功率限制] - [所有比自己先加入环网的主机的需求最大功率之和] /**/
	time_t						tmStartAsMaster; //以开设锁定主机的时间作为开始时间/**/


	//for convince of searching
	RING_NODE*				left;
	RING_NODE*				right;
};

typedef enum tagRingMergeMode
{
	RING_MODE_DUE_CURRRENT = 0,
	RING_MODE_INSERT,
	RING_MODE_ONECAR_MULTIGUN
}RING_MERGE_MODE;

#define MAX_SUPPORTED_RING_NODE	32
typedef struct tagRingList
{
	unsigned long u32RingTimeSec;		  //环网时间，用于延时处理 单位：秒

	/*环网配置信息--注意：以下参数在系统启动时由参数恢复时进行初始化，不要另外去初始化*/
	//BOOL				bIsGunInsertMode; //是否插枪切换/**/
	RING_MERGE_MODE		emRingMergeMode;

	UCHAR			nGunInsertMode_Start[2]; //插枪切换开始和结束时间, 如果是插枪切换模式的话  H时-M分
	UCHAR				nGunInsertMode_Stop[2];  //H时-M分
	//float				fRingMaxAllowedCurr;	
	float				fRingStartMergeCurr; //开启环网电流/**/
	float				fAllRingsPwrLmt;//环网总功率限制/**/
	float				fRingCurrIncSpeed;//环网充电电流上升速率/**/
	/*END: 环网配置信息*/	

	int				nNodeTotal;//总接入节点数, 根据配置确定，一旦改变必须重新建链表，故需要重启/**/
	int				nOnlineTotal; //实际在线的节点数目/**/
	BOOL				bIsRingCfgErr;
	RING_NODE		allNodes[MAX_SUPPORTED_RING_NODE];
}RING_ALL_INFO; //只有一份，两把枪共享/**/

typedef struct _RING_SALVE RING_SALVE;
struct _RING_SALVE
{
	RING_NODE*		pNode;
	//slave data
	time_t			tm_LastStartMerging;
	time_t			tm_LastCancelMerging;
	BOOL				bIsCmd4Ready; //是否收到CMD4申请停机/**/
	BOOL				bIsOnRight;		//1-表示本枪在主枪右侧  0-左侧

	float				fVolt2Sent;//需要发给它多大电压，由主机计算得到/**/
	float				fCurr2Sent;//需要发给它多大电流，由主机计算得到/**/
	BYTE				emSetRelayOnOff; //0--off,1--on

	//for continence of searching
	RING_SALVE*		left;
	RING_SALVE*		right;
};

#define MAX_RING_SLAVE_NUM		10
typedef struct tagRingMaster
{
//#define MAX_RING_SLAVE_NUM		10
	RING_SALVE*		slaves[MAX_RING_SLAVE_NUM]; //最多支持10个slave同时， 指向s_RingSlavesBuf[x]
	UCHAR           u8RingSlavesBufNo[MAX_RING_SLAVE_NUM];  //指向s_RingSlavesBuf[x]
	UCHAR			u8RingSlavesBuf_IsUsed[MAX_RING_SLAVE_NUM];	//s_RingSlavesBuf 占用情况
	UCHAR			u8RingSlavesBuf_InHandling;             //指示s_RingSlavesBuf[x]
	int				nSlaveTotal;

	BYTE			u8WaitSlaveKwhReading[MAX_RING_SLAVE_NUM]; //1-有效
	float			fSlaveStart_kWhReading[MAX_RING_SLAVE_NUM];
	float			fSlaveEnd_kWhReading[MAX_RING_SLAVE_NUM];

	RING_SALVE*		pLeftEdge;
	RING_SALVE*		pRightEdge;

	RING_SALVE*		pSlave_InHandling;//正在处理并机或者并机取消请求的Node/**/
	
	//主机信息/**/
	BOOL				bIsReceivedLock; //收到BMS锁定主机请求/**/
	float				fTotalReqVolt;
	float				fTotalReqCurr; //BMS需求/**/
	float				fTotalReqCurr_FromBms; //BMS需求/**/
	BOOL				bIsReceivedAdj; //是否收到BMS调节电压电流命令/**/
	BOOL				bIsRecievedStop; //是否收到停机命令/**/
	BOOL				bIsReceivedStoppOK;//是否收到BMS通知停机OK的命令, 要确保BMS那边先停机OK才行/**/

	float				fTotalOutVolt;
	float				fTotalOutCurr; //实际/**/

	float				fSlaveTotalOutKwhReading; //实际/**/

	float				fSlave1_kWhReading;
	float				fSlave2_kWhReading;

	//time_t			tm_LastStartMerging;
	time_t			tm_LastCancelMerging;

	time_t			tmlastCtrlDO;

	time_t			tmlastCtrlChgCurr;		//上一次控制调整充电电流

	time_t			tm_FirstEnterCharging;	//刚进入环网Charing状态的时间

	time_t			tm_CancelMergeDelay;

}RING_AS_MASTER;

typedef struct tagRingAsSlave
{
	RING_NODE		*pMaster;

	//作为从机时，相关信息/**/
	BOOL				bReceivedStop;//收到BMS那边通知的主动申请退出环网命令/**/
	time_t			tm_LasReceive_Cmd_FromMaster;//上次收到命令时间，当10s内没收到主机命令时自动申请断网/**/
	BOOL				bIsCmd2_AA_Ready;
	BOOL				bIsCmd2_00_Ready;
	BOOL				bIsCmd2_CC_Ready;
	BOOL				bIsCmd3_Ready;
	float				fBusRealVolt; //用于判断母排电压跟当前输出是否一致/**/
	float				fReqVolt;
	float				fReqCurr;
	BYTE				emSetRelayOnOff; //0--off,1--on

	time_t			tmlastCtrlDO;

	time_t			tmLastCancel; //记录上次从机取消的时间/**/

}RING_AS_SLAVE;

typedef enum tagRingState
{
	RING_STATE_IDLE = 0,
	//Master
	RING_STATE_PREPARE_BEFRE_CHARGE,
	RING_STATE_CHARGING,
	RING_STATE_WAITING_MERGING,
	RING_STATE_WAITING_CANCELING, //手动CMD4或者电流减小，都需要逐步取消并机/**/
	RING_STATE_STOPING, //BMS通知停机/**/
	//Slave
	RING_STATE_PREPARE_BEFOR_MERGE,
	RING_STATE_MERGING,
	RING_STATE_WAITING_CMD3,
	RING_STATE_MERGE_CANCELING,

	RING_MAX_STM_STATE
}RING_STATE_MACHINE_STATE;

typedef RING_STATE_MACHINE_STATE (*ON_RING_STATE_PROC)(void* pArg);

typedef struct tagStateMachineItem
{
//	char						szStateName[16];
	RING_STATE_MACHINE_STATE	emState;
	ON_RING_STATE_PROC		pfnAction;
	time_t					tm_Enter;// the first time that enter this state
}RING_STM_ITEM;


typedef struct stCanRingData
{
	//环网信息/**/
	RING_NODE			*pNodeMe; //以me为节点建立双向链表，便于管理/**/
	RING_AS_MASTER		master; //作为主机时用/**/
	RING_AS_SLAVE		slave; //作为从机时用/**/

	BOOL					bIsGunA; //本组数据是否来自A枪/**/

#define MAX_CANMSG_FROM_RECT	100
	HANDLE				hqCANMsgFromRect;//模块解析到的报文暂存在这里/**/

	HANDLE				hThread;

	HANDLE				hMutex;

	RING_STM_ITEM		stateMachineLoop[RING_MAX_STM_STATE];
	RING_STM_ITEM		*pLastState;// Last状态/**/
	RING_STM_ITEM		*pCurrState;// 当前状态/**/	

	BOOL					bSelfIDConflict; //自己的ID与其它装ID冲突/**/

	BOOL				bIsGunMergable;


}CAN_RING_DATA;

#define INIT_RING_STATE_MACHINE(_P, _MachineId, _fnProc) ((_P)->stateMachineLoop[_MachineId].emState = (_MachineId), (_P)->stateMachineLoop[_MachineId].pfnAction = (_fnProc))

#define RING_CMD1		(0x01)
#define RING_CMD2		(0x02)
#define RING_CMD3		(0x03)
#define RING_CMD4		(0x04)
#define RING_CMD2_AA	(0xF2)
#define RING_CMD2_CC	(0xF3)
#define RING_CMD5		(0x05)
#define RING_CMD6		(0x06)

void Set_SelfRectOnOff(CAN_RING_DATA* pRing_Rough, BOOL bOn);
void AdjSelf_VoltCurr(CAN_RING_DATA* pRing_Rough, float fTargetVolt, float fTargetCurr);
void Send_RingCmd(CAN_RING_DATA* pRing_Rough, BYTE byCmdNo, void* pWhere2Send);
//控制我右边的DO/**/
void Ctrl_RighDO(CAN_RING_DATA* pRing_Rough, BOOL bOn);
//直接关所有模块/**/
void Stop_AllSlaveRects(CAN_RING_DATA* pRing_Rough);

void Ctrl_SlaveOutDO(CAN_RING_DATA* pRing_Rough, BOOL bOn);

time_t time_ring(unsigned char ucflag);

BOOL Ring_Initialize(void);

UINT8  GetRingDataFromCAN3(void);
void Parse_RingCmd1(BYTE bySrcAddr, BYTE* pbyData);
void Prase_RingCmd234(CAN_RING_DATA* pRing_Rough, BYTE bySrcAddr, BYTE byCmdNo, BYTE* pbyData);
void Refresh_SelfNode(CAN_RING_DATA* pRing_Rough);
void Judge_NodeCommFail(CAN_RING_DATA* pRing_Rough);

void Send_LockGunMsg2BMS(CAN_RING_DATA *pRing_Rough,BOOL bLock);
void JudgeIfGunMergeable(CAN_RING_DATA *pRingData);

enum _RING_CTRL_CMD_def
{
	RING_CTRL_CMD_NONE = 0,	
	RING_CTRL_CMD_LOCK_AS_MASTER,		//Lock gun as master
	RING_CTRL_CMD_RQ_ST_SLAVE,			//Request for Stopping Slave
	RING_CTRL_CMD_ST_MASTER,			//Stop Master Charging
	//RING_CTRL_CMD_VOLT_ADJ,				//Ring DC voltage control
	//RING_CTRL_CMD_CURR_ADJ,				//Ring Adjust Rect Total Curr
	RING_CTRL_CMD_ADJ_VOLT_CURR,		//Ring Adjust Rect Total Curr and Voltage 
	RING_CTRL_CMD_EndOfStop,			//Notice End of Stop(Internal)
};
int Ring_Ctrl(UINT8 u8GunNo, UINT8 u8CmdNo, VAR_VALUE *pVarVal1, VAR_VALUE *pVarVal2); //u8CmdNo: enum _RING_CTRL_CMD_def

void Calculate_LeftPower_4Me(CAN_RING_DATA *pRing_Rough);

////以下是状态机函数/**/
//RING_STATE_MACHINE_STATE	OnIdle(CAN_RING_DATA *pRing_Rough);
////Master
//RING_STATE_MACHINE_STATE	OnPrepare_BeforeCharge(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnWaitMerging(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnCharging(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnWaitCanceling(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnStop(CAN_RING_DATA *pRing_Rough);
////Slave
//RING_STATE_MACHINE_STATE	OnPrepare_BeforeMerge(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnMerging(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnWaitCmd3(CAN_RING_DATA *pRing_Rough);
//RING_STATE_MACHINE_STATE	OnMergeCanceling(CAN_RING_DATA *pRing_Rough);

extern RING_ALL_INFO		g_RingAllInfo;
extern CAN_RING_DATA*		g_pRing_A;
extern CAN_RING_DATA*		g_pRing_B;

extern CanRxMsgTypeDef RxMsg_Ring[CAN_3_MAX_BUFF];
//#pragma pack()

#endif
