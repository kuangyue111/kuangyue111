#ifndef ETHERNET_H
#define ETHERNET_H

#define NEED_CREAD_SOCKET 1
#define NEED_NOT_CREAD_SOCKET 0
#define BUF_NO_DATA	-1
#define FIND_SPECIFIED_CMD 0

#define Y_SOI			gs_EthAppCommMng.u16NetFrameHead
#define Y_VERSION		0x13

#define Get_Parameters  0
#define Set_Parameters  1

#define Y_SOI_PST		0
#define Y_LENGH_PST		2
#define Y_VERSION_PST	4
#define Y_SEQ_NO_PST	5
#define Y_CMD_PST		6
#define Y_DATA_PST		8
#define MIN_FRAME_LENGTH		9

#define CMD_SET_INT_PRG_1				1		//(CMD=1)后台服务器下发充电桩整形工作参数
#define CMD_SET_INT_PRG_2_ACK			2		//(CMD=2)后台服务器下发充电桩整形工作参数
#define CMD_SET_CHAR_PRG_3				3		//(CMD=3)后台服务器下发充电桩字符型工作参数
#define CMD_SET_CHAR_PRG_4_ACK			4		//(CMD=4)后台服务器下发充电桩字符型工作参数
#define CMD_CTRL_5						5		//(CMD=5)后台服务器下发充电桩控制命令
#define CMD_CTRL_6_ACK					6		//(CMD=6)后台服务器下发充电桩控制命令
#define CMD_START_CHARGE				7		//(CMD=7)后台服务器下发充电桩开启充电控制命令
#define CMD_START_CHARGE_ACK			8		//(CMD=8)后台服务器下发充电桩开启充电控制命令
#define CMD_HEARDBEAT_ACK				101		//(CMD=101)充电桩上传心跳包信息
#define CMD_HEARDBEAT					102		//(CMD=102)充电桩上传心跳包信息
#define CMD_EV_STATE_ACK				103		//(CMD=103)充电桩状态信息包上报
#define CMD_EV_STATE					104		//(CMD=104)充电桩状态信息包上报
#define CMD_REGIST_ACK					105		//签到回复
#define CMD_REGIST						106		//(CMD=106)充电桩签到信息上报
#define CMD_END_INFO_ACK				201
#define CMD_END_INFO					202		//(CMD=202)充电桩上报最新一次充电信息
#define CMD_ACCOUNT_IDENTIFY_ACK		203	
#define CMD_ACCOUNT_IDENTIFY			204			//发送命令 204
#define CMD_GET_EXCHANGE_CODE_ACK		205
#define CMD_GET_EXCHANGE_CODE			206
#define CMD_BATT_DATA_ACK				301		//(CMD=302)充电桩上报BMS信息
#define CMD_BATT_DATA					302		//(CMD=302)充电桩上报BMS信息
#define CMD_HIS_END_INFO_ACK			401
#define CMD_HIS_END_INFO				402		//充电桩上报历史的充电记录
#define CMD_SET_FIX_PRICE				1301		//CMD=1301）后台下发固定电费计价策略信息
#define CMD_SET_FIX_PRICE_ACK			1302		//CMD=1301）后台下发固定电费计价策略信息
#define CMD_SET_TIME_PRICE				1303		//（CMD=1303）后台下发峰平谷电费计价策略信息
#define CMD_SET_TIME_PRICE_ACK			1304		//（CMD=1304）后台下发峰平谷电费计价策略信息
#define CMD_SET_FIX_SERVICE_PRICE		1305		//（CMD=1305）后台下发当前充电用户充电服务费信息
#define CMD_SET_FIX_SERVICE_PRICE_ACK	1306	//（CMD=1306）充电桩响应后台下发当前充电用户充电服务费信息
#define CMD_SET_TIME_SERVICE_PRICE		1307		//（CMD=1307）后台下发当前充电用户充电服务费信息
#define CMD_SET_TIME_SERVICE_PRICE_ACK	1308	//（CMD=1308）充电桩响应后台下发当前充电用户充电服务费信息











#define RSP_MODE_IS_ACK		1
#define RSP_MODE_IS_AUTO	2		//自动应答







#define USER_Card_Type			0x00
#define USER_VIN_Type			0x01


//后台服务器下发充电桩字符型工作参数	具体信号！！！
#define CMD3_PRG_EV_SerialNum	1
#define CMD3_PRG_Tick		2
#define CMD3_PRG_AdminPSWD1	3
#define CMD3_PRG_AdminPSWD2	4	//操作员密码
#define CMD3_PRG_MACReserve	5
#define CMD3_PRG_BoxNumber	6	//箱变编码
#define CMD3_PRG_2BarCode	7	//二维码
#define CMD3_PRG_RESERVE1	8
#define CMD3_PRG_RESERVE2	9
#define CMD3_PRG_Cost2BarCode	10

#define CMD5_SIG_MAX_NUM	16

#define Data_HI(w)  ((UINT8)((UINT16)(w) >> 8))
#define Data_LO(w)  ((UINT8)((UINT16)(w) &0xff))

#define NET_MAX_BUF  1500




struct _EthAppCommMng
{
	UCHAR ucCtrlSocketFlag;   // 0:None  1：Connect  2：Disconnect
	UCHAR ucNeedSendFrame[3];
	UCHAR ucConnectIntervalCount;



	UINT8  u8NetSeqNo;                  //报文序号
	UINT16 u16APP_PipeWTDOG;			//防止SOCKET挂死！
	UINT8  u8RegistResult;				//登录结果
	UINT16 u16RegistCount;				//未收到校时指令后，每隔15S，登录一下。
	UINT16 u16RegistInterval;			//签到间隔 单位分钟
	UINT8  u8HeartBeatCount;			//心跳计时
	UINT8  u8HeartBeatInterval;			//心跳周期
	UINT8  u8HeartBeatTimeOutTimes;		//心跳超时次数
	UINT8  u8HeartBeatAckCount;			//心跳回复计时
	UINT8  u8HeartBeatAckErr;			//心跳超时次数

	UINT8 u8WorkStat[ALL_GUN_NUM];
	UINT16 u16EVStateInterval;
	UINT16 u16EVStateCount[ALL_GUN_NUM];

	UINT8 u8TickFlag;				//校时标识	TRUE 收到校时	
	UINT8 u8RcvFlagOfCodeByHost[ALL_GUN_NUM];	//屏显示二维码 标识	TRUE 收到
	UINT8 u8RcvFlagOfIdentifyID[ALL_GUN_NUM];	//屏显示识别码 标识	TRUE 收到
	UINT8  ucSetCodeByHost[ALL_GUN_NUM][128];
	UINT8  ucSetIdentify[ALL_GUN_NUM][16];

	UINT8  u8CMD1Flag;
	UINT8  u8CMD1Type;
	UINT32 u32CMD1StartAddress;
	UINT8  u8CMD1ParaNum;
	UINT8  u8CMD1Result;

	UINT8  u8CMD3Flag;
	UINT8  u8CMD3Type;
	UINT32 u32CMD3StartAddress;
	UINT8  u8CMD3Result;

	UINT8  u8CMD5Flag;
	UINT8  u8CMD5GunNo; 
	UINT32 u32CMD5StartAddress;
	UINT8  u8CMD5ParaNum;
	UINT8  u8CMD5Result;

	UINT8  u8CMD7Flag;
	UINT8  u8CMD7GunNo; 
	UINT8  u8CMD7Result;
	UINT8  u8ExchangeCode[ALL_GUN_NUM][COST_NO_BYTES];
	UINT8  u8GetExchangecodeFlag[ALL_GUN_NUM];
	UINT8  u8GetExchangecodeCount[ALL_GUN_NUM];
	
	UINT32 u16OrderTimeOutCount[ALL_GUN_NUM];
	UINT16 u16OrderTimeOut[ALL_GUN_NUM];				//预约超时时间	用于回传 不用于记数
	UINT8  u8OrderChargeCMD[ALL_GUN_NUM];			//0-无预约（无效）	1-预约有效
	UINT8  u8OrderSetTime[ALL_GUN_NUM][8];		//预约开始时间	

	UINT8 u8APPInfo[ALL_GUN_NUM][32];    //预定号/APP账号

	UINT32 u32AlmInfo[ALL_GUN_NUM];
	
	UINT16 u16BattDataCount[ALL_GUN_NUM];
	UINT16 u16BattDataCMDSeqNo[ALL_GUN_NUM];

	UINT8 u8EndInfoFlag[ALL_GUN_NUM];
	UINT16 u16EndInfoCount[ALL_GUN_NUM];			//结算信息上传间隔计时

	UINT16 u16HisEndInfoCount;

	UINT16 ui16DHCPReconfigTimeOUT;	
	UINT8  u8CardIdentifyCount;
	UINT8  u8VINIdentifyCount[ALL_GUN_NUM];
	UINT8  u8VINIdentifyDelay[ALL_GUN_NUM];
	UINT8  u8AccountType[ALL_GUN_NUM];

	UINT8  u8CMD1301Flag;
	UINT8  u8CMD1303Flag;
	UINT8  u8CMD1305Flag;
	UINT8  u8CMD1307Flag;

	UINT16 u16NetFrameHead;


	UCHAR  ucEndStateUpdataCount[2];  //状态报文中，结束状态需要传三次
	UINT8 u8OffLineFlag;			//0-不允许 1-允许
	float fOffLineAllowPower;


	struct tm  LastChargeStartTime;
	struct tm  PowerOnTime;			//最后一次启动时间

};
typedef struct _EthAppCommMng EthAppCommMng;
extern EthAppCommMng gs_EthAppCommMng;


#define CMD202			0
#define CMD402			1

#endif

