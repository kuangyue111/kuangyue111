
#ifndef TELAIDIANPROTO_H
#define	TELAIDIANPROTO_H

#include "MQTTClient.h"
#include "main.h"



enum PUB_MESSAGE_TOPIC_DEF
{
	PUB_MESSAGE_EventStateUpload = 0,	
	PUB_MESSAGE_EventStateUpload_Lwt,

	//Start: A/B枪都要分别上报 的消息 放一起，方便批处理
	PUB_MESSAGE_EventUpload_StartChargeResultUp,
	PUB_MESSAGE_EventUpload_StopChargeResultUp,
	PUB_MESSAGE_EventUpload_TelesignallingDataUp,
	PUB_MESSAGE_EventUpload_TelemetryDataUp,
	PUB_MESSAGE_EventUpload_MeterDataUp,	
	PUB_MESSAGE_EventUpload_BMSDataUp,
	PUB_MESSAGE_EventRequest_StartChargeRequest,
	PUB_MESSAGE_EventRequest_StopChargeRequest,
	PUB_MESSAGE_EventRequest_AccountRequest,
	//End	

	MAX_PUB_NUMBER
};
#define PUB_MESSAGE_A_B_GUN_CMD_START	PUB_MESSAGE_EventUpload_StartChargeResultUp
#define PUB_MESSAGE_A_B_GUN_CMD_END		PUB_MESSAGE_EventRequest_AccountRequest

extern void PackPublicMessage(unsigned char u8PubMessageNo, MessageData * pMessageData);



typedef struct PublicMessage_Ctrl_
{
	unsigned char IsSend;	//1-需要发送 0-不需要
	unsigned char NeedSendGunNo;	//当前哪只枪需要发 1-A枪 2-B枪 3-AB枪
	unsigned char JustSentGunNo;  //刚发送的 枪号 1-枪 2-B枪
	char *pUuid;
}PublicMessage_Ctrl_def;
extern PublicMessage_Ctrl_def gs_PublicMessage_Ctrl[MAX_PUB_NUMBER];

void PrepareSubTopicName(void);
void initMqttClient(void);

//extern void SubTopic01Func(MessageData * pMessageData);
//extern void SubTopic02Func(MessageData * pMessageData);
//extern void SubTopic03Func(MessageData * pMessageData);
extern messageHandler SubFunction[MAX_MESSAGE_HANDLERS];

extern char g_SubMessageUuidBuf[MAX_MESSAGE_HANDLERS][40];

//AccountRequest[2];  //账号请求标志 0-初始态 1-已发送账号请求 2-获取到了账号余额  3-未获取到账号余额 4-未收到回复 5-收到9.2拒绝命令
                      //6-收到账户余额后，若枪未插入，等待枪插入   7-收到账户余额且枪也插好后，发送充电请求  
enum TelaidianAccountRequestFlag_
{
	TelaidianAccountRequestFlag_0 = 0,
	TelaidianAccountRequestFlag_1,  //1-已发送账号请求
	TelaidianAccountRequestFlag_2,  //2-获取到了账号余额
	TelaidianAccountRequestFlag_3,
	TelaidianAccountRequestFlag_4,
	TelaidianAccountRequestFlag_5,	//收到9.2拒绝命令
	TelaidianAccountRequestFlag_6,
	TelaidianAccountRequestFlag_7
};
//StopRequest[2];  //账号请求标志 0-初始态 1-已发送账号请求 2-获取到了停电回复  3-收到9.2拒绝命令 4-未收到回复 5-
enum TelaidianStopRequestFlag_
{
	TelaidianStopRequestFlag_0 = 0,
	TelaidianStopRequestFlag_1,  //1-已发送账号请求
	TelaidianStopRequestFlag_2,  //2-获取到了停电回复
	TelaidianStopRequestFlag_3, //3 收到9.2拒绝命令
	TelaidianStopRequestFlag_4, //未收到回复
};

//gs_TelaidianCtrl.A_StopChgReport/B_StopChgReport
//枪停止充电后，需要上报后台， 0-无  1-后台主动下发  2-成功结束  3-因故障结束  4-本地屏点击终止
enum TelaidianStopChgReportMode_
{
	TelaidianStopChgReportMode_0 = 0,
	TelaidianStopChgReportMode_1,
	TelaidianStopChgReportMode_2,
	TelaidianStopChgReportMode_3,
	TelaidianStopChgReportMode_4
};
typedef struct TelaidianCtrl_
{
	unsigned char TelaidianStationName[60];
	unsigned char TelaidianUserName[32];
	unsigned char TelaidianPassword[32];
	unsigned char TelaidianStationID[32];
	unsigned char TelaidianBillId[2][60];  //双枪对应的订单号

	unsigned char Telaidian_QR_Code[2][32];  //二维码
	
	unsigned char StartCmd[2];		//开始充电1 经济充电2(预留) 开始放电3(预留)
	unsigned char StopCmd[2];		//停止充电1 

	unsigned char Result[2];	//结果 1失败 255成功
	unsigned char EndReason[2];   //失败原因 充电成功填-1
	unsigned char PileState[2];		 //未知0, 充电中1 

	unsigned char A_burstDataFlag;	//A枪突发数据 1-表示突发 0-非
	unsigned char B_burstDataFlag;	//B枪突发数据 1-表示突发 0-非

	unsigned char A_AlmStat;	//A枪故障内容
	unsigned char B_AlmStat;	//B枪故障内容
	unsigned char SysAlmStat;	//系统故障


	unsigned char A_StopChgReport;	//A枪停止充电后，需要上报后台， 0-无  1-后台主动下发  2-成功结束  3-因故障结束 4-屏点击停止
	unsigned char B_StopChgReport;	//B枪停止充电后，需要上报后台， 0-无  1-后台主动下发  2-成功结束  3-因故障结束 4-屏点击停止

	unsigned char A_WaitHostJiesuanCmdDelay;	//A枪上报结束充电结果后，需要等候后台下发结算命令
	unsigned char B_WaitHostJiesuanCmdDelay;	//B枪上报结束充电结果后，需要等候后台下发结算命令

	unsigned char A_AfterMeterUpToSendTelesignalDelay;	//A枪上报结束充电结果且收到后台结算命令，然后上报电量，间隔5秒后再发送遥信数据
	unsigned char B_AfterMeterUpToSendTelesignalDelay;	//B枪上报结束充电结果且收到后台结算命令，然后上报电量，间隔5秒后再发送遥信数据

	unsigned char OffLineChgReportMeterData[2];//允许离线充电情况下，下次上线后，先发电量数据（作为离线充电结束后的电表值）

	unsigned char AfterAppStartChgSendTelesignalFlag[2];//在APP启动充电且设备回复成功后，待进入CCS充电阶段后，需要上报遥信信号


	unsigned char TelaidianAccount[2][16];  //刷卡卡号
	int TelaidianAccountBalace[2];  //刷卡卡号余额
	unsigned char  AccountRequest[2];  //账号请求标志 0-初始态 1-已发送账号请求 2-获取到了账号余额  3-未获取到账号余额 4-未收到回复 5-收到9.2拒绝命令
	unsigned char  StopRequest[2];  //请求停止标志 0-初始态 1-已发送账号请求  2-收到9.2拒绝命令

	unsigned char  AfterRcvStartRptTelesignal90SecCnt[2];  //收到后台启动充电命令后，90内要进入充电阶段并上报遥信信号，否则终止充电？

	unsigned short WaitHostJiesuanCmdDelay_ReportCount[2];  //枪上报结束充电结果后，需要等候后台下发结算命令,超过60秒重发，重发次数2000
}TelaidianCtrl_Def;
extern TelaidianCtrl_Def gs_TelaidianCtrl;

extern MQTTString PublicMessage_EventUploadLwt_Topic;
extern MQTTMessage	PublicMessage_EventUploadLwt;

void TelaidianEndReason(unsigned char GunNo, unsigned char EndReason);

//AB枪的CAN ID 
#define A_GUN_CANID		181
#define B_GUN_CANID		182

#define TELD_FREEZE_METER_NUM	48		//冻结电量最大数量
typedef struct TelaidianStopReportFreezeMeter_
{
	unsigned char u8FreezeRecordNum;	//当前记录数量
	struct tm FreezeTime[TELD_FREEZE_METER_NUM];
	int i32FreezeWhVal[TELD_FREEZE_METER_NUM];	
}TelaidianStopReportFreezeMeter_Def;
extern TelaidianStopReportFreezeMeter_Def gs_TelaidianStopReportFreezeMeter_A;
extern TelaidianStopReportFreezeMeter_Def gs_TelaidianStopReportFreezeMeter_B;
#endif