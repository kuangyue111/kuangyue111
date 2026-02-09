#include "main.h"
#include "resolv.h"
#include "MQTTClient.h"
#include "TelaidianProto.h"

Timer MqttPeriodTimer;


//#define NET_MAX_BUF						1500
#define EV_VERSION_BYTES_NUM			64
#define ZTE_SIGN_RESULT_ERR				0x00
#define ZTE_SIGN_RESULT_OK				0x01
#define ZTE_RSP_AUTO_MODE				0
#define ZTE_SQC_VOLUNTARY 				0						//主动上报
#define BUF_NO_DATA						-1
#define FIND_SPECIFIED_CMD				0
#define BUF_HAS_DADA					1
#define STATION_RETAD_POWER				60
#define ABS(a)	(((a) < 0) ? (-(a)) : (a))
#define Z_SOI_PST					0
#define Z_LENGH_PST					2
#define Z_VERSION_PST					4		//版本 0x11
#define Z_FACTORY_PST					5		//厂家
#define Z_STATION_CODE_PST				6		//桩编码	8字节
#define Z_SEQUENCE_PST					14		//序列号域 1字节
#define Z_CMD_PST						15		//命令域
#define Z_DATA_PST						16		//数据域
#define Z_FRAME_MIN_LEN		(Z_DATA_PST - Z_SOI_PST + 1)
#define Z_SOI_VALUE					0xf5fa
#define Z_VERSION_VALUE					0x11
#define Z_SQC_CMD_CHECK_L				3
#define Z_PACKET_SPLICING_NUM			        5		//粘包数量
#define Z_FACTORY_INFY					0x25		//厂家
#define Z_CMD_TIME_SYNC					0x42		//时间同步
#define Z_CMD_HEARDBEAT					0x48		//心跳
#define Z_CMD_HEARDBEAT_INFO			        0x49		//心跳查询 应答
#define Z_CMD_COMM_MODE					0xE0		//通讯模式
#define Z_CMD_EV_STATUS					0x2c		//设置/查询状态
#define Z_CMD_SIGN_IN					0x01		//签到 查询/应答
#define Z_CMD_BMS_INFO					0x22		//BMS 查询/应答
#define Z_CMD_RECT_INFO					0x23		//RCT 查询/应答
#define Z_CMD_CARD_INFO					0x60		//服务器下发卡信息
#define Z_CMD_STEP_KWH_ACK				0x67		//服务器应答 步进电量
#define Z_CMD_CHARGE_RECORD				0x63		//充电记录应答
#define Z_CMD_REMOTE_START				0x06		//远程充电
#define Z_CMD_REMOTE_STOP				0x09		//远程终止充电
#define Z_CMD_CHARGE_INFO				0xE1		//充电状态查询
#define Z_CMD_SPEAR_STATUS				0x2B 		//充电枪 状态查询/应答
#define Z_CMD_TRADE					0x2A		//交易号
#define I_CMD_APPROVE_CHARGE			        0x3A
#define I_CMD_TIME_SYNC					0x52
#define I_CMD_HEARDBEAT					0x58
#define I_CMD_HEARDBEAT_INFO			        0x59
#define I_CMD_COMM_MODE					0xF0
#define I_CMD_EV_STATUS					0x3C
#define I_CMD_SIGN_IN					0x10
#define I_CMD_BMS_INFO					0x32
#define I_CMD_RECT_INFO					0x39
#define I_CMD_CARD_INFO					0x70
#define I_CMD_CHARGE_RECORD				0x78
#define I_CMD_STEP_KWH					0x77
#define I_CMD_REMOTE_START				0x16
#define I_CMD_REMOTE_STOP				0x19
#define I_CMD_CHARGE_STATUS				0xF1
#define I_CMD_SPEAR_STATUS				0x3B

#define ZTEV_EV_STATION			                0
#define ZTEV_EV_SPEAR_1			                1
#define ZTEV_EV_SPEAR_2			                2



NOINIT UINT8 gNetR_Buf[NET_MAX_BUF];
NOINIT UINT8 gNetS_Buf[NET_MAX_BUF];
NOINIT UINT8 gNetBuf[NET_MAX_BUF];
NOINIT UINT8 gu8EVVersion[EV_VERSION_BYTES_NUM];
NOINIT UINT8 g8Mac[64];
NOINIT UINT8 gu8EVStationNo[64];
NOINIT __NET_TCP_INFO gNET_TCP_Info;
void NetPipeCheckWHDOG(void);
void NetPipeWHDOG(void);
static INT8 PipeIsOk(void);
void AnalayseNetData(void);
UINT8 GetOneFrame(UINT16 u16TotalLen, 
				  UINT16* pNextFramePST, 
				  UINT16* FrameLen, 
				  UINT8* InBuf, 
				  UINT8* OutBuf);
void UnPackXXXFrame(UINT8* pData, UINT16 u16Len);
void InitNetData(void);

void PackHeardBeat_0X58(UINT8 SqcId);
void NET_HeardBeat_0X58Deal(void);
void NET_Sign_0X10Deal(void);
void PackSign_In_0X10(UINT8 SqcId);
void NET_HeardBeat_0X58Deal(void);
void NET_EvStat_0X3CDeal(void);
void PackEv_Stat_0X3C(UINT8 SqcId,UINT8 u8RptType, UINT8 u8No);
void NET_Approve_0X3ADeal(void);
void PackIApprove_Charge_0X3A(UINT8 SqcId, UINT8 u8SpearNo, UINT8 u8RspType, UINT8 u8StartType);
void PackBms_Info_0X32(UINT8 SqcId, UINT8 u8MaxNum);
void PackRect_Info_0X39(UINT8 SqcId, UINT8 u8MaxRectNum);
void NET_Bms_Rct_0X39_0X32Deal(void);
void PackComm_Mode_0XF0(UINT8 SqcId);
void UnPackComm_Mode_0XE0(UINT8* InBuf);
void UnPackHeardbeatInfo_0X49(UINT8* InBuf);
void PackHeardbeatInfo_0X59(UINT8 SqcId);
void UnPackHeardbeat_0X48(UINT8* InBuf);
void UnPackSyncTime_0X42(UINT8* InBuf);
void PackSyncTime_0X52(UINT8 SqcId);
void UnPackSign_In_0X01(UINT8* InBuf);
void UnPackBms_Info_0X22(UINT8* InBuf);
void UnPackRect_Info_0X23(UINT8* InBuf);
void PackSpear_Status_0X3B(UINT8 SqcId, UINT8 Result, UINT8 u8SpearNo);
void UnPackCharge_Info_0XE1(UINT8* InBuf);
void PackCharge_Info_0XF1(UINT8 SqcId, UINT8 u8ChargeNo, UINT8 *pCustomerNo);
void UnPackSpear_Status_0X2B(UINT8* InBuf);
void UnPackTrade_0X2A(UINT8* InBuf);


#define APP_START_CMD_OVERTIME	85		//秒，用于gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt
char TelaidianClientID[100];
static unsigned char RegistCMD0A(void)
{
	unsigned char i;
	int rc;
	static unsigned char s_ucFirstConnected = 1;
	static unsigned char s_ucSpearPstBak[2] = {0};

	MQTTPacket_connectData options;
	MQTTString PublicMessage_EventUploadLwt_Payload;

	MessageData MqttMessageValue;
	//MQTTConnackData data;
	options = (MQTTPacket_connectData)MQTTPacket_connectData_initializer;

	PrepareSubTopicName();


	if (gs_MqttClient.isconnected)
	{
		gEvChargeInfo.u8TelaidianHostCommAlm = FALSE;

		//离网充电允许，重连上网后先上报电量数据
		if(gs_TelaidianCtrl.OffLineChgReportMeterData[0] == 1)
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x01;
			PackPublicMessage(PUB_MESSAGE_EventUpload_MeterDataUp, &MqttMessageValue);
			rc = MQTTPublish(&gs_MqttClient, MqttMessageValue.topicName->cstring, MqttMessageValue.message );
			gs_TelaidianCtrl.OffLineChgReportMeterData[0] = 0;
		}
		if(gs_TelaidianCtrl.OffLineChgReportMeterData[1] == 1)
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x02;
			PackPublicMessage(PUB_MESSAGE_EventUpload_MeterDataUp, &MqttMessageValue);
			rc = MQTTPublish(&gs_MqttClient, MqttMessageValue.topicName->cstring, MqttMessageValue.message );
			gs_TelaidianCtrl.OffLineChgReportMeterData[1] = 0;
		}


		keepalive(&gs_MqttClient);

		//接收服务器下发数据
		DealServerCmdData(&gs_MqttClient);

		//设备在线状态上报
		if((gs_PublicMessage_Ctrl[PUB_MESSAGE_EventStateUpload].IsSend == 1)||(s_ucFirstConnected == 1))
		{			
			s_ucFirstConnected = 0;

			PackPublicMessage(PUB_MESSAGE_EventStateUpload, &MqttMessageValue);

			rc = MQTTPublish(&gs_MqttClient, MqttMessageValue.topicName->cstring, MqttMessageValue.message );

			if(rc == SUCCESS_MQTT)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventStateUpload].IsSend = 0;	
			}
		}

		//订阅消息
		for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
		{
			if(g_ucSubTopicxxIsSubed[i] == 0)  //是否已订阅过
			{
				if(MQTTSubscribe(&gs_MqttClient, gs_SubMessageData[i].topicName->cstring, QOS1, SubFunction[i]) == SUCCESS_MQTT)
				{
					g_ucSubTopicxxIsSubed[i] = 1;
				}
			}
		}

		//定期发送
		//if(TimerIsExpired(&MqttPeriodTimer) || (s_ucFirstConnected == 1))
		//if(s_ucFirstConnected == 1)
		//{			
		//	s_ucFirstConnected = 0;

		//	PackPublicMessage(PUB_MESSAGE_EventStateUpload, &MqttMessageValue);

		//	rc = MQTTPublish(&gs_MqttClient, MqttMessageValue.topicName->cstring, MqttMessageValue.message );

		//	if(rc == SUCCESS_MQTT)
		//	{

		//	}

		//	//TimerCountdown(&MqttPeriodTimer, 5000);
		//}


		//拔插枪突发上报事件
		if(((gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] == SPEAR_ON_LINE) 
			&& (s_ucSpearPstBak[0] == SPEAR_NO_ON_LINE))
			|| 	((gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] == SPEAR_NO_ON_LINE) 
			&& (s_ucSpearPstBak[0] == SPEAR_ON_LINE)))
		{
			s_ucSpearPstBak[0] = gEvChargeInfo.u8EV_SpearPst[GUN_NO_A];
			gs_TelaidianCtrl.A_burstDataFlag = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 1;
		}
		if(((gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] == SPEAR_ON_LINE) 
			&& (s_ucSpearPstBak[1] == SPEAR_NO_ON_LINE))
			|| 	((gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] == SPEAR_NO_ON_LINE) 
			&& (s_ucSpearPstBak[1] == SPEAR_ON_LINE)))
		{
			s_ucSpearPstBak[1] = gEvChargeInfo.u8EV_SpearPst[GUN_NO_B];
			gs_TelaidianCtrl.B_burstDataFlag = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 2;
		}

		//启动充电后需要突发上报遥信信号
		if(gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[0])
		{
			//超过XX时间还没进入充电阶段，则直接停止，也不上报其它信息
			if(gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[0] > APP_START_CMD_OVERTIME)
			{
				if(gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
				{					
					A_SetChargeSTOP_Cmd(APP_CMD_ENTER_CHG_OVERTIME);
					//gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_QR_CODE_20;
					//gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_RESULT_84;
					gEvChargeInfo.u8IsOptionA = FALSE;	
					gs_TelaidianCtrl.A_StopChgReport = 0; //不上报信息
				}
				gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[0] = 0;
			}
			else if (CHARGING_RCV_BCL_SEND_CCS == gBms_1_CommInfo.i8_CMD_Phase)
			{				
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 0x01;

				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x01;  //电量也上报

				gs_TelaidianCtrl.A_burstDataFlag = 1;
				gs_TelaidianCtrl.PileState[0] = 1;
				gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[0] = 0;
				gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[0] = 0;
			}
		}
		if(gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[1])
		{
			//超过XX时间还没进入充电阶段，则直接停止，也不上报其它信息
			if(gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[1] > APP_START_CMD_OVERTIME)
			{
				if(gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
				{					
					B_SetChargeSTOP_Cmd(APP_CMD_ENTER_CHG_OVERTIME);
					//gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_QR_CODE_35;
					//gHMIData.unPageAddr.u16BasePageAddr = PAGE_B_RESULT_85;
					gEvChargeInfo.u8IsOptionB = FALSE;	
					gs_TelaidianCtrl.B_StopChgReport = 0; //不上报信息
				}
				gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[1] = 0;
			}
			else if (CHARGING_RCV_BCL_SEND_CCS == gBms_2_CommInfo.i8_CMD_Phase)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 0x02;

				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x02;  //电量也上报

				gs_TelaidianCtrl.B_burstDataFlag = 1;
				gs_TelaidianCtrl.PileState[1] = 1;
				gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[1] = 0;
				gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[1] = 0;
			}
		}


		//A枪充电结束上报
		if(gs_TelaidianCtrl.A_StopChgReport > 0)
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x01;
			gs_TelaidianCtrl.StopCmd[0] = 1;
			gs_TelaidianCtrl.Result[0] = 255;	//成功
			if(gs_TelaidianCtrl.A_StopChgReport == TelaidianStopChgReportMode_2)  //成功结束
			{
				gs_TelaidianCtrl.EndReason[0] = 0xFF;
			}
			gs_TelaidianCtrl.A_StopChgReport = 0;
			gs_TelaidianCtrl.A_WaitHostJiesuanCmdDelay = 0;
			gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[0] = 0;
			gs_TelaidianCtrl.PileState[0] = 0;
			gs_TelaidianCtrl.WaitHostJiesuanCmdDelay_ReportCount[0] = 0;
		} 
		else if(gs_TelaidianCtrl.A_WaitHostJiesuanCmdDelay > 60)  //60秒没收到确认，需重发，最多重发2000次
		{
			gs_TelaidianCtrl.WaitHostJiesuanCmdDelay_ReportCount[0]++;

			if(gs_TelaidianCtrl.WaitHostJiesuanCmdDelay_ReportCount[0] < 2000)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x01;
			}
			gs_TelaidianCtrl.A_WaitHostJiesuanCmdDelay = 0;
		}
		if(gs_TelaidianCtrl.A_AfterMeterUpToSendTelesignalDelay > 5)  //电量上报5秒后上报遥信数据
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 0x01;
			gs_TelaidianCtrl.A_AfterMeterUpToSendTelesignalDelay = 0;
			gs_TelaidianCtrl.A_burstDataFlag = 1;
		}
		//B枪充电结束上报	
		if(gs_TelaidianCtrl.B_StopChgReport > 0)
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x02;
			gs_TelaidianCtrl.Result[1] = 255;	//成功
			gs_TelaidianCtrl.StopCmd[1] = 1;
			if(gs_TelaidianCtrl.B_StopChgReport == TelaidianStopChgReportMode_2)  //成功结束
			{
				gs_TelaidianCtrl.EndReason[1] = 0xFF;
			}
			gs_TelaidianCtrl.B_StopChgReport = 0;
			gs_TelaidianCtrl.B_WaitHostJiesuanCmdDelay = 0;
			gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[1] = 0;
			gs_TelaidianCtrl.PileState[1] = 0;
			gs_TelaidianCtrl.WaitHostJiesuanCmdDelay_ReportCount[1] = 0;
		} 
		else if(gs_TelaidianCtrl.B_WaitHostJiesuanCmdDelay > 60)  //60秒没收到确认，需重发，最多重发2000次
		{
			gs_TelaidianCtrl.WaitHostJiesuanCmdDelay_ReportCount[1]++;

			if(gs_TelaidianCtrl.WaitHostJiesuanCmdDelay_ReportCount[1] < 2000)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x02;
			}
			gs_TelaidianCtrl.B_WaitHostJiesuanCmdDelay = 0;			
		}
		if(gs_TelaidianCtrl.B_AfterMeterUpToSendTelesignalDelay > 5)	 //电量上报5秒后上报遥信数据
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 0x02;
			gs_TelaidianCtrl.B_AfterMeterUpToSendTelesignalDelay = 0;
			gs_TelaidianCtrl.B_burstDataFlag = 1;
		}

		

		//A B 枪需要单独上报的处理
		for(i = PUB_MESSAGE_A_B_GUN_CMD_START; i <= PUB_MESSAGE_A_B_GUN_CMD_END; i++)
		{
			if(gs_PublicMessage_Ctrl[i].IsSend == 1)
			{

				PackPublicMessage(i, &MqttMessageValue);

				rc = MQTTPublish(&gs_MqttClient, MqttMessageValue.topicName->cstring, MqttMessageValue.message );

				if(rc == SUCCESS_MQTT)
				{
					if(gs_PublicMessage_Ctrl[i].JustSentGunNo == 1)
					{
						gs_PublicMessage_Ctrl[i].NeedSendGunNo &= 0xFE;
	
						//stop命令需要启动接收后台结算回复的计时器
						if(i == PUB_MESSAGE_EventUpload_StopChargeResultUp)
						{
							gs_TelaidianCtrl.A_WaitHostJiesuanCmdDelay = 1;
						}
					}
					else if(gs_PublicMessage_Ctrl[i].JustSentGunNo == 2)
					{
						gs_PublicMessage_Ctrl[i].NeedSendGunNo &= 0xFD;

						//stop命令需要启动接收后台结算回复的计时器
						if(i == PUB_MESSAGE_EventUpload_StopChargeResultUp)
						{
							gs_TelaidianCtrl.B_WaitHostJiesuanCmdDelay = 1;
						}
					}
					else if(gs_PublicMessage_Ctrl[i].JustSentGunNo == 3)
					{
						gs_PublicMessage_Ctrl[i].NeedSendGunNo &= 0xFC;
					}

					if((gs_PublicMessage_Ctrl[i].NeedSendGunNo & 0x03) == 0)
					{
						gs_PublicMessage_Ctrl[i].IsSend = 0;
					}
				}
			}

		}	
	}  //end:  if (gs_MqttClient.isconnected)
	else
	{
		s_ucFirstConnected = 1;

		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventStateUpload].IsSend = 1;


		memset(TelaidianClientID, 0, sizeof(TelaidianClientID));
		sprintf( TelaidianClientID, "%s/%s", (char *)gs_TelaidianCtrl.TelaidianStationID,  (char *)gs_TelaidianCtrl.TelaidianStationName);
		options.clientID.cstring = TelaidianClientID;
		options.username.cstring = gs_TelaidianCtrl.TelaidianUserName;
		options.password.cstring = gs_TelaidianCtrl.TelaidianPassword;
		options.keepAliveInterval = 10;
		options.cleansession = 1;
		options.willFlag = 1;
		PackPublicMessage(PUB_MESSAGE_EventStateUpload_Lwt, &MqttMessageValue);
		options.will.topicName = PublicMessage_EventUploadLwt_Topic;

		PublicMessage_EventUploadLwt_Payload.cstring = NULL;
		PublicMessage_EventUploadLwt_Payload.lenstring.len = PublicMessage_EventUploadLwt.payloadlen;
		PublicMessage_EventUploadLwt_Payload.lenstring.data = PublicMessage_EventUploadLwt.payload;
		options.will.message = PublicMessage_EventUploadLwt_Payload;
		
		MQTTConnect(&gs_MqttClient, &options);

		if (gs_MqttClient.isconnected)
		{
			gEvChargeInfo.u8TelaidianHostCommAlm = FALSE;
		}
		else
		{
			gEvChargeInfo.u8TelaidianHostCommAlm = TRUE;
		}
	}

}

/***************************************************************************************************************
	增加该函数的原因为，发现有SOCKET建立OK，但是应用程序就收不到数据，
	而底层还能收到>0的数值，所以要在APP端加个狗！
*****************************************************************************************************************/
void NetPipeCheckWHDOG(void)
{
	//网口 软看门口。	如果EN久没有收到网络数据则 需要复位网络 重新链接！！！
	gEvChargeInfo.i32APP_PipeWTDOG++;
	if (gEvChargeInfo.i32APP_PipeWTDOG >= 80000)
	{
		gNET_TCP_Info.iNetCreateSocketFlag = TRUE;
		gEvChargeInfo.i32APP_PipeWTDOG = 0;
		CLR_CONN_STAT(gNET_TCP_Info.u8MachineStat);
		CLR_SEND_STAT(gNET_TCP_Info.u8MachineStat);
	}
	return;
}

/***************************************************************************************************************
	增加该函数的原因为，发现有SOCKET建立OK，但是应用程序就收不到数据，
	而底层还能收到>0的数值，所以要在APP端加个狗！
*****************************************************************************************************************/
void NetPipeWHDOG(void)
{
	gEvChargeInfo.i32APP_PipeWTDOG = 0;
	return;
}

static INT8 PipeIsOk(void)
{
	static UINT16 su8Delay = 0;
	if (!CHK_CONN_STAT(gNET_TCP_Info.u8MachineStat))
	{
		gEvChargeInfo.u8Sign_Result = ZTE_SIGN_RESULT_ERR;											//SOCKET都需要重新建立了，当然要进行重新登录了。
		//gEvChargeInfo.i32Flash_UnPackRecordDelay = 1 * 60 * 1000;									// 4分钟后再判断FLASH 中未上传的数据。
		gNET_TCP_Info.u8NetIsBreakFlag = TRUE;
		Put_Ether_3CSem(POWER_ON_3C);
		//if ( (gEvChargeInfo.i32OffLineTimeCount <= 0 && (gEvChargeInfo.u8CARD_OffLineFlag != TRUE))	//进来的时候是网络正常 刷卡进来
		//	&& (gEvChargeInfo.u8NET_OffLine_Charge_Enable == FALSE))								//离线充电使能的情况下可以继续充电！！
		//{
		//	if (START_CHARGE_VAL == gEvChargeInfo.uUIXXXCmd
		//		&& 2 == gEvChargeInfo.uRunPhase
		//		&& (gEvChargeInfo.u8ChargeLogInMode == EV_ENTER_APP
		//		|| gEvChargeInfo.u8ChargeLogInMode == EV_ENTER_IC_CARD))
		//	{
		//		SetChargeSTOP_Cmd(NET_BREAK_STOP);												//网络中断 停机
		//		gEvChargeInfo.u8EV_Charge_RecordTimes = 0;
		//		gEvChargeInfo.u8SendChargeRecordFlag = FALSE;									//断网
		//		gEvChargeInfo.u8SendStepKwh0X77Ok = FALSE;
		//	}
		//}
		//离线禁止充电处理
		if(gEvChargeInfo.u8NET_OffLine_Charge_Enable == FALSE)
		{
			if(gEvChargeInfo.u32OffLineTimeCount == 0)
			{
				gEvChargeInfo.u32OffLineTimeCount = 1;
			}
			else if(gEvChargeInfo.u32OffLineTimeCount > gEvChargeInfo.fMaxOffLineTime)
			{
				if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
					&& (gEvChargeInfo.u8_StartReason[GUN_NO_A] == EV_ENTER_APP || gEvChargeInfo.u8_StartReason[GUN_NO_A] == EV_ENTER_IC_CARD_NET))
				{
					A_SetChargeSTOP_Cmd(NET_BREAK_STOP);
				}

				if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
					&& (gEvChargeInfo.u8_StartReason[GUN_NO_B] == EV_ENTER_APP || gEvChargeInfo.u8_StartReason[GUN_NO_B] == EV_ENTER_IC_CARD_NET))
				{
					B_SetChargeSTOP_Cmd(NET_BREAK_STOP);
				}
			}
		}
		else
		{	
			//下次恢复网络时要先发送电量数据
			/*if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
			{
				gs_TelaidianCtrl.OffLineChgReportMeterData[0] = 1;
			}
			if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
			{
				gs_TelaidianCtrl.OffLineChgReportMeterData[1] = 1;
			}*/
		}
		gEvChargeInfo.u8NetCommAlm = TRUE;
		gEvChargeInfo.u8TelaidianHostCommAlm = TRUE;

		su8Delay++;
		if (su8Delay > 100)
		{
			//NetPipeWHDOG();
			NET_Clean_SBuf();
			gNET_TCP_Info.iNetCreateSocketFlag = TRUE;	//需要重置，让底层线程重新建立链接！
			g_ucRemoteServerIpIsOk = 0;
			resolv_init();
			su8Delay = 0;
			
		}
		//网络心跳 周期 相关参数
		gEvChargeInfo.u8RcvRunModeFlag = FALSE;

		gEvChargeInfo.i32HeardBeat_Count = 0 * TICKS_COUT_UNINT;
		//gEvChargeInfo.i32FixedPeriodCount = 200 * 1000;
		gEvChargeInfo.i32HeardBeat_TimeOutCount = gEvChargeInfo.u8HeardBeatTimes * gEvChargeInfo.i32HeardBeat_Period * TICKS_COUT_UNINT;
		//gEvChargeInfo.u8EVStat_0x3C_PowerOnFlag = TRUE;//断网重连下发
		OSTimeDly(100);

		return FALSE;
	}
	else
	{
		gEvChargeInfo.u8NetCommAlm = FALSE;
		gEvChargeInfo.u32OffLineTimeCount = 0;
		OSTimeDly(2);
		su8Delay = 0;
		return TRUE;
	}
}

//只找包，不做包有效性判断 TRUE 找到!
UINT8 GetOneFrame(UINT16 u16TotalLen, UINT16* pNextFramePST, UINT16* FrameLen, UINT8* InBuf, UINT8* OutBuf)
{
	UINT8 u8Rst = FALSE;
	UINT16 u16Idx = 0;
	UINT16 FramePst = 0;											//该帧的起始位置
	UINT16 u16FrameLen = 0;
	if (u16TotalLen == 24)
	{
		FramePst = 0;
	}
	//Find SOI
	for (u16Idx = (*pNextFramePST); u16Idx < u16TotalLen && u16Idx < NET_MAX_BUF; u16Idx++)
	{
		if ((InBuf[u16Idx] == (Z_SOI_VALUE & 0xff))
			&& (InBuf[u16Idx + 1] == (0xff & (Z_SOI_VALUE >> 8))))
		{
			FramePst = u16Idx;									//本包在Buff中的起始位置！！
			u8Rst = TRUE;
			break;
		}
	}

	if (TRUE == u8Rst)
	{
		u16FrameLen = InBuf[FramePst + Z_LENGH_PST] + (InBuf[FramePst + Z_LENGH_PST + 1] << 8) + Z_SEQUENCE_PST;	//见协议格式
		for (u16Idx = 0; u16Idx < u16FrameLen && u16Idx < NET_MAX_BUF; u16Idx++)
		{
			OutBuf[u16Idx] = InBuf[FramePst + u16Idx];
		}
	}

	*FrameLen = u16FrameLen;									//本包帧长度
	(*pNextFramePST) = FramePst + u16FrameLen + 1;				//下一包位置！ +1 目的指向下1包位置
	return u8Rst;
}

void PackComm_Mode_0XF0(UINT8 SqcId)
{
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;

	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;						//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_COMM_MODE;
	u32CheckSum += I_CMD_COMM_MODE;

	//数据域
	u16DataLen = 0;
	gNetS_Buf[Z_DATA_PST] = 0;				//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;			//用户ID
	gNetS_Buf[Z_DATA_PST + 2] = 0;			//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;			//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;

	//通信模式
	gNetS_Buf[Z_DATA_PST + 4] = gEvChargeInfo.u8CommMode;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 4];
	u16DataLen++;
	//通信上报间隔
	gNetS_Buf[Z_DATA_PST + 5] = gEvChargeInfo.u8FixedPeriod;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 5];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 6] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含 从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (Z_DATA_PST + 6 + 1), 100);
	OSTimeDly(500);
}


void UnPackComm_Mode_0XE0(UINT8* InBuf)
{
	UINT8 u8TempVal= 0;
	UINT8 u8Type;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	pData = InBuf + Z_DATA_PST;							//Point 2 Data
	//用户序ID  指令序号
	pData += 2;
	pData += 2;
	u8Type = pData[0];
	pData += 1;

	//是否网管主动查询  0主动设置	1主动查询
	if (u8Type == 0)
	{
		gEvChargeInfo.u8CommMode = pData[0];			//0应答模式  1上报模式
		pData += 1;
		u8TempVal = pData[0];							//定时上报间隔 15---255
		if (gEvChargeInfo.u8FixedPeriod != u8TempVal)
		{
			gEvChargeInfo.u8FixedPeriod = u8TempVal;
			//gEvChargeInfo.u8CharParamUpDataCount++;
		}
		gEvChargeInfo.u8FixedPeriod = MAX(gEvChargeInfo.u8FixedPeriod, 15);
		gEvChargeInfo.i32FixedPeriodCount = gEvChargeInfo.u8FixedPeriod * 1000;
		pData += 1;
		//NET_PRINTF("CMD=%x ModeSet\n",InBuf[Z_CMD_PST]);
		PackComm_Mode_0XF0(u8SequenceId);
	}
	else
	{
		//NET_PRINTF("CMD=%x ModeGet\n",InBuf[Z_CMD_PST]);
		PackComm_Mode_0XF0(u8SequenceId);
	}
}

void PackHeardbeatInfo_0X59(UINT8 SqcId)
{
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;

	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_HEARDBEAT_INFO;
	u32CheckSum += I_CMD_HEARDBEAT_INFO;

	//数据域
	u16DataLen = 0;
	gNetS_Buf[Z_DATA_PST] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 4] = gEvChargeInfo.u8HeartBeatIntervalParam;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 4];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 5] = gEvChargeInfo.u8HeartBeatTimeOutParam;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 5];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 6] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (Z_DATA_PST + 6 + 1), 100);
	OSTimeDly(200);
}

void UnPackHeardbeatInfo_0X49(UINT8* InBuf)
{
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	pData = InBuf + Z_DATA_PST;//Point 2 Data

	//用户序ID  指令序号  
	pData += 2;
	pData += 2;
	//是否网管主动查询  0设置	1查询
	if (pData[0] == 0)
	{
		pData += 1;
		gEvChargeInfo.u8HeartBeatIntervalParam = pData[0];		//到这里转换为S
		pData += 1;
		gEvChargeInfo.u8HeartBeatTimeOutParam = pData[0];
		PackHeardbeatInfo_0X59(u8SequenceId);
		gEvChargeInfo.u8RcvRunModeFlag = TRUE;
	}
	else
	{
		PackHeardbeatInfo_0X59(u8SequenceId);
	}
}


void UnPackHeardbeat_0X48(UINT8* InBuf)
{
	INT32 Rcv_Counter,Sys_Counter;
	struct tm  tmNow,RcvtmNow;
	UINT8 *pData = NULL;
	pData = InBuf;
	pData = InBuf + Z_DATA_PST;//Point 2 Data
	//用户序ID  指令序号  心跳应答缺省0
	pData += 2;
	pData += 2;
	pData += 2;

	RcvtmNow.tm_year = (pData[0] + (pData[1] << 8));
	pData += 2;
	RcvtmNow.tm_year = RcvtmNow.tm_year - YEAR0;

	RcvtmNow.tm_mon = pData[0];
	pData += 1;
	RcvtmNow.tm_mday = pData[0];
	pData += 1;
	RcvtmNow.tm_hour = pData[0];
	pData += 1;
	RcvtmNow.tm_min = pData[0];
	pData += 1;
	RcvtmNow.tm_sec = pData[0];
	pData += 1;
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	gRTCData.fGetLocalTime(&tmNow);
	tmNow.tm_year = tmNow.tm_year - YEAR0;
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	gNET_TCP_Info.u8NetIsBreakFlag = FALSE;
	Sys_Counter = mktime_r(&tmNow);
	Rcv_Counter = mktime_r(&RcvtmNow);

	if (ABS(Rcv_Counter - Sys_Counter) > 30)
	{
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		RcvtmNow.tm_year =  RcvtmNow.tm_year + YEAR0;
		gRTCData.fSetLocalTime(&RcvtmNow);
		//NET_PRINTF("\n CMD52 =%d %d %d %d %d %d\n",RcvtmNow.tm_year,RcvtmNow.tm_mon,RcvtmNow.tm_mday,RcvtmNow.tm_hour,RcvtmNow.tm_min,RcvtmNow.tm_sec);
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		//OSTimeDly(50);
	}
	gEvChargeInfo.i32HeardBeat_TimeOutCount = gEvChargeInfo.u8HeardBeatTimes * gEvChargeInfo.i32HeardBeat_Period * TICKS_COUT_UNINT;
	//NET_PRINTF("CMD=%x TimeSET0X48\n",InBuf[Z_CMD_PST]);
}

void PackSyncTime_0X52(UINT8 SqcId)
{
	struct tm  tmNow;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;

	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_TIME_SYNC;
	u32CheckSum += I_CMD_TIME_SYNC;

	//数据域
	u16DataLen = 0;
	gNetS_Buf[Z_DATA_PST] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;

	gRTCData.fGetLocalTime(&tmNow);

	gNetS_Buf[Z_DATA_PST + 4] = (tmNow.tm_year) & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 4];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 5] = ((tmNow.tm_year)>>8) & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 5];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 6] = tmNow.tm_mon & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 6];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 7] = tmNow.tm_mday & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 7];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 8] = tmNow.tm_hour & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 8];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 9] = tmNow.tm_min & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 9];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 10] = tmNow.tm_sec & 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 10];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 11] = 0xff;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 11];
	u16DataLen++;
	//NET_PRINTF("\n CMD52 =%d %d %d %d %d %d\n",tmNow.tm_year,tmNow.tm_mon,tmNow.tm_mday,tmNow.tm_hour,tmNow.tm_min,tmNow.tm_sec);
	gNetS_Buf[Z_DATA_PST + 12] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;//包含从序列号域到校验和域的所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;


	NET_Send(gNetS_Buf, (Z_DATA_PST + 12 + 1), 100);
	OSTimeDly(300);
}

void UnPackSyncTime_0X42(UINT8* InBuf)
{
	struct tm  tmNow;
	UINT8 u8SequenceId = 0;
	UINT8 *pData = NULL;
	pData = InBuf;

	u8SequenceId = pData[Z_SEQUENCE_PST];

	pData = InBuf + Z_DATA_PST;//Point 2 Data
	//用户序ID  指令序号
	pData += 2;
	pData += 2;

	tmNow.tm_year = (pData[0] + (pData[1] << 8));
	pData += 2;

	tmNow.tm_mon = pData[0];
	pData += 1;

	tmNow.tm_mday = pData[0];
	pData += 1;

	tmNow.tm_hour = pData[0];
	pData += 1;

	tmNow.tm_min = pData[0];
	pData += 1;

	tmNow.tm_sec = pData[0];
	pData += 1;

	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	gRTCData.fSetLocalTime(&tmNow);
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	OSTimeDly(2);
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	//NET_PRINTF("\n CMD42=%d %d %d %d %d %d\n",tmNow.tm_year,tmNow.tm_mon,tmNow.tm_mday,tmNow.tm_hour,tmNow.tm_min,tmNow.tm_sec);
	PackSyncTime_0X52(u8SequenceId);
}

void UnPackSign_In_0X01(UINT8* InBuf)
{
	UINT8 u8Result = 0;
	UINT8 u8Type = 0;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	UNUSED(u8Result);
	UNUSED(u8Type);
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	pData = InBuf + Z_DATA_PST;//Point 2 Data
	//用户序ID  指令序号  
	pData += 2;
	pData += 2;

	u8Type = pData[0];
	pData += 1;
	//主动/被动   0服务器被动反馈		1服务器主动查询
	if (u8Type == 0) //服务器  被动反馈 结果！
	{
		//桩工作类型  00禁止本桩工作	01允许本桩工作	02网络平台在线本桩正常工作
		gEvChargeInfo.u8Sign_Result = pData[0];
		pData += 1;

		gNET_TCP_Info.u8NetIsBreakFlag = FALSE;
		if (0!=gEvChargeInfo.u8Sign_Result)
		{
			gEvChargeInfo.u8NetLogInFails = FALSE;			//登录成功
		}
		else
		{
			gEvChargeInfo.u8NetLogInFails = TRUE;			//登录失败
		}
		//设置桩编号 保留
		pData += 1;
	}
	else
	{
		//NET_PRINTF("CMD=%x SignGet \n",InBuf[Z_CMD_PST]);
		//主动查询 后边两个参数失效！
		PackSign_In_0X10(u8SequenceId);
	}
}


void UnPackBms_Info_0X22(UINT8* InBuf)
{
	UINT8 u8RectNum = 1;
	UINT8 u8RectNo = 1;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	UNUSED(u8RectNum);
	UNUSED(u8RectNo);
	UNUSED(u8SequenceId);
	pData = InBuf + Z_DATA_PST;							//Point 2 Data
	//用户序ID		指令序号
	pData += 2;
	pData += 2;
	if (1 == gEvChargeInfo.u8CommMode)					//上报模式
	{
		//收到数据全扔掉！！
	}
	else
	{
		u8RectNum = pData[0];
		pData += 1;
		u8RectNo = pData[0];
		pData += 1;
	}
}

void UnPackRect_Info_0X23(UINT8* InBuf)
{
	UINT8 u8RectNum = 1;
	UINT8 u8RectNo = 1;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	UNUSED(u8RectNum);
	UNUSED(u8RectNo);
	UNUSED(u8SequenceId);
	pData = InBuf + Z_DATA_PST;//Point 2 Data
	//用户序ID  指令序号  
	pData += 2;
	pData += 2;
	if (1 == gEvChargeInfo.u8CommMode)//上报模式
	{
		//收到数据全扔掉！！
	}
	else
	{
		//数量
		u8RectNum = pData[0];
		pData += 1;
		//充电枪位置编号
		u8RectNo = pData[0];
		pData += 1;
	}
}

void PackSpear_Status_0X3B(UINT8 SqcId, UINT8 Result, UINT8 u8SpearNo)
{
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;

	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_SPEAR_STATUS;
	u32CheckSum += I_CMD_SPEAR_STATUS;

	//数据域
	u16DataLen = 0;
	gNetS_Buf[Z_DATA_PST] = 0;		//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;
	u16DataLen++;

	//充电枪编号 0代表所有枪 
	gNetS_Buf[Z_DATA_PST + 4] = u8SpearNo;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 4];
	u16DataLen++;
	//设置结果
	gNetS_Buf[Z_DATA_PST + 5] = Result;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 5];
	u16DataLen++;

	//是否工作		 暂时不做功能，一切都是工作！！后边有需求再改！
	gNetS_Buf[Z_DATA_PST + 6] = 0x01;	//gEvChargeInfo.u8SpearWorkStat;
	u32CheckSum += gNetS_Buf[Z_DATA_PST + 6];
	u16DataLen++;

	gNetS_Buf[Z_DATA_PST + 7] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (Z_DATA_PST + 7 + 1), 100);
}

void UnPackSpear_Status_0X2B(UINT8* InBuf)
{
	UINT8 u8SpearNo = 0;
	UINT8 u8Result;
	UINT8 u8Type;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	pData = InBuf + Z_DATA_PST;//Point 2 Data
	//用户序ID  指令序号  
	pData += 2;
	pData += 2;

	u8Type = pData[0];
	pData += 1;
	//是否网管主动查询  0 主动设置	1 主动查询
	if (u8Type == 0)									//0主动设置
	{
		u8SpearNo = pData[0];
		pData += 1;										//充电枪编号  单枪不考虑了

		//u8Result = SetSpearStatus(pData[0]);			// 设置
		pData += 1;
		PackSpear_Status_0X3B(u8SequenceId, u8Result, u8SpearNo);
	}
	else
	{
		//GetSpearStatus();
		u8Result = 0x01;//如果查询 此数值为1
		PackSpear_Status_0X3B(u8SequenceId, u8Result,0);
	}
}

void PackCharge_Info_0XF1(UINT8 SqcId, UINT8 u8ChargeNo, UINT8 *pCustomerNo)
{
	UNUSED(pCustomerNo);	//当前充电信息，所以暂时用不到
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	UINT32 u32Value = 0;
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_CHARGE_STATUS;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 用户ID
	gNetS_Buf[Z_DATA_PST] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;	//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//2 指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//3 模块个数
	u16BufIdx = Z_DATA_PST + 4;
	gNetS_Buf[u16BufIdx] = 2;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	float fRctOutVolt = gs_CANComm.fGetRectAnalog(GRP_A, 0, ID_RECT_OUT_VOLT);
	float fRctOutCurr = gs_CANComm.fGetRectAnalog(GRP_A, 0, ID_RECT_OUT_CURR);

	u16BufIdx += 1;//指向枪的信息！

	if (0 == u8ChargeNo || 1 == u8ChargeNo)		//0 获取所有 1获取1枪  2获取2枪
	{
		//4 充电枪位置1
		gNetS_Buf[u16BufIdx] = 1;				//1 
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//5 交易号
		u16BufIdx += 1;
		for (u16Idx = 0; u16Idx < 16; u16Idx++)
		{
			gNetS_Buf[u16BufIdx + u16Idx] = 0;											//gEvChargeInfo.u8CustomerNo[0][u16Idx];
			u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
			u16DataLen++;
		}

		//6 充点枪位置1 SOC  %1
		u16BufIdx += 16;
		gNetS_Buf[u16BufIdx] = (UINT8)gEVBms_1_Info.fBCS_SOC;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = 0;
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//7 充电枪1 充电时间 1S
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)((UINT16)(gEvChargeInfo.fBMS_ChargeTime[GUN_NO_A] * 60) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT16)((((UINT16)gEvChargeInfo.fBMS_ChargeTime[GUN_NO_A] * 60)>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//8 充电枪位置1剩余时间  2 
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT8)((gEVBms_1_Info.u16BCS_LeaveTime * 60) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)(((gEVBms_1_Info.u16BCS_LeaveTime * 60)>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//9 当前温度 2 温度，单位：1℃，收到数据的基础上-50
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT8)(((UINT16)gEVBms_1_Info.fBSM_MaxBattTemp + 50) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((((UINT16)gEVBms_1_Info.fBSM_MaxBattTemp + 50)>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//10 当前功率 2 当前功率，单位：0.01kw
		u16BufIdx += 2;
		u32Value = (UINT16)(fRctOutVolt*fRctOutCurr * 100);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//11 当前充电电压 2 当前充电电压，单位：0.1V 
		u16BufIdx += 2;
		u32Value = (UINT16)(fRctOutVolt * 10);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//12 当前充电电流 2 电流偏移3200 之后除以 100
		u16BufIdx += 2;
		u32Value = (UINT16)(fRctOutCurr * 100) + 3200;
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//13 最大单体电压 2 最大单体电压，单位：0.1V
		u16BufIdx += 2;
		u32Value = (UINT16)(gEVBms_1_Info.fBCS_MaxCellVolt * 10);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//14 最大单体温度 2 最大单体温度，单位：1℃，收到数据的基础上-50
		u16BufIdx += 2;
		u32Value = (UINT16)(gEVBms_1_Info.fBSM_MaxBattTemp+50);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//15 当前充电电量 4 充电电量，单位0.01kwh 
		u16BufIdx += 2;
		u32Value = ((UINT32)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_A] * 1000))/10;  //0xF1数据，暂时靠CAN1线程来刷新！
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 2] = (UINT8)((u32Value>>16) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 3] = (UINT8)((u32Value>>24) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		u16BufIdx += 4;
	}

	if (0 == u8ChargeNo || 2 == u8ChargeNo)		//0 获取所有 1获取1枪  2获取2枪
	{
		//4 充电枪位置1
		gNetS_Buf[u16BufIdx] = 2;				//1 
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//5 交易号
		u16BufIdx += 1;
		for (u16Idx = 0; u16Idx < 16; u16Idx++)
		{
			gNetS_Buf[u16BufIdx + u16Idx] = 0;//gEvChargeInfo.u8CustomerNo[0][u16Idx];
			u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
			u16DataLen++;
		}

		//6 充点枪位置1 SOC  %1
		u16BufIdx += 16;
		gNetS_Buf[u16BufIdx] = (UINT8)gEVBms_2_Info.fBCS_SOC;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = 0;
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//7 充电枪1 充电时间 1S
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)((UINT16)(gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B] * 60) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT16)((((UINT16)gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B] * 60)>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//8 充电枪位置1剩余时间  2 
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT8)((gEVBms_2_Info.u16BCS_LeaveTime * 60) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)(((gEVBms_2_Info.u16BCS_LeaveTime * 60)>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//9 当前温度 2 温度，单位：1℃，收到数据的基础上-50
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT8)(((UINT16)gEVBms_2_Info.fBSM_MaxBattTemp + 50) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((((UINT16)gEVBms_2_Info.fBSM_MaxBattTemp + 50)>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		fRctOutVolt = gs_CANComm.fGetRectAnalog(GRP_B, 0, ID_RECT_OUT_VOLT);
		fRctOutCurr = gs_CANComm.fGetRectAnalog(GRP_B, 0, ID_RECT_OUT_CURR);

		//10 当前功率 2 当前功率，单位：0.01kw
		u16BufIdx += 2;
		u32Value = (UINT16)(fRctOutVolt*fRctOutCurr * 100);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//11 当前充电电压 2 当前充电电压，单位：0.1V 
		u16BufIdx += 2;
		u32Value = (UINT16)(fRctOutVolt * 10);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//12 当前充电电流 2 电流偏移3200 之后除以 100
		u16BufIdx += 2;
		u32Value = (UINT16)(fRctOutCurr * 100) + 3200;
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//13 最大单体电压 2 最大单体电压，单位：0.1V
		u16BufIdx += 2;
		u32Value = (UINT16)(gEVBms_2_Info.fBCS_MaxCellVolt * 10);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//14 最大单体温度 2 最大单体温度，单位：1℃，收到数据的基础上-50
		u16BufIdx += 2;
		u32Value = (UINT16)(gEVBms_2_Info.fBSM_MaxBattTemp+50);
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		//15 当前充电电量 4 充电电量，单位0.01kwh 
		u16BufIdx += 2;
		u32Value = ((UINT32)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_B] * 1000))/10;  //0xF1数据，暂时靠CAN1线程来刷新！
		gNetS_Buf[u16BufIdx] = (UINT8)(u32Value & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = (UINT8)((u32Value>>8) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 2] = (UINT8)((u32Value>>16) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 3] = (UINT8)((u32Value>>24) & 0xff);
		u32CheckSum += gNetS_Buf[u16BufIdx + 1];
		u16DataLen++;

		u16BufIdx += 4;
	}


	//校验和
	//u16BufIdx += 4;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
}

void UnPackCharge_Info_0XE1(UINT8* InBuf)
{
	UINT8 u8ChargeNo = 0;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	u8SequenceId = pData[Z_SEQUENCE_PST];
	pData = InBuf + Z_DATA_PST;							//Point 2 Data
	//用户序ID  指令序号
	pData += 2;
	pData += 2;

	//充电桩编号 0表示所有充电枪  1--255获取单个充电枪数据
	u8ChargeNo = pData[0];
	pData += 1;
	PackCharge_Info_0XF1(u8SequenceId, u8ChargeNo, pData);
}

void UnPackTrade_0X2A(UINT8* InBuf)
{
	UINT16 u16Idx = 0;
	//UINT8 u8Result;
	UINT8 u8Type;
	UINT8 EVRectNo = 0;
	UINT8 SpearNo = 0;
	UINT8 *pData = NULL;
	UINT8 u8SequenceId = 0;
	pData = InBuf;
	UNUSED(SpearNo);
	u8SequenceId = pData[Z_SEQUENCE_PST];
	pData = InBuf + Z_DATA_PST;									//Point 2 Data
	//用户序ID  指令序号
	pData += 2;
	pData += 2;

	//0 网关平台被动回馈		1 网关平台主动查询
	u8Type = pData[0];
	if (1 == u8Type)											//查询
	{
		//3 充电模块编号
		pData += 1;
		EVRectNo = pData[0];

		//4 充电枪编号
		pData += 1;
		SpearNo = pData[0];
		//查询算了		不做了！！！
		//u8SetApproveChargeVal(TRUE, 0xff);
		//PackIApprove_Charge_0X3A(u8SequenceId, EVRectNo, 1, gEvChargeInfo.u8ChargeLogInMode);//1平台查询
	}
	else
	{
		//3 充电模块编号
		pData += 1;
		EVRectNo = pData[0];

		//4 充电枪编号
		pData += 1;
		SpearNo = pData[0];

		//5 交易号
		pData += 1;
		for (u16Idx = 0; u16Idx < 16; u16Idx++)
		{
			if (1 == SpearNo)
			{
				gEvChargeInfo.u8_CustomerNo[GUN_NO_A][u16Idx] = pData[u16Idx];
			}
			else
			{
				gEvChargeInfo.u8_CustomerNo[GUN_NO_B][u16Idx] = pData[u16Idx];
			}
		}

		//6 交易号是否有效	0 无效		 1 有效
		pData += 16;
		if (pData[0] == 1)
		{
			//if (gEvChargeInfo.u8ChargeLogInMode == EV_ENTER_IC_CARD)
			//{
			//	gEvChargeInfo.u8PlcCtlJumpNo = MENU_124_BRUSH_CHARGE;
			//}
			//if (gEvChargeInfo.u8ChargeLogInMode == EV_ENTER_APP)
			//{
			//	gEvChargeInfo.u8PlcCtlJumpNo = MENU_123_APP_CHARGE;
			//}
		}
		else
		{
			//交易号无效！
			__STOP_CHARGE_CMD();//交易号  无效 停止 不计入充电记录
		}

		Put_Ether_3CSem((TRADE_3C | (SpearNo<<4)));
		//gEvChargeInfo.u8EVStat_0x3C_RcvTradeNoFlag = TRUE;				//需要上报充电机状态

		//7 最大充电离线时长
		pData += 1;
		//float fMaxOffLineTime = (float)(pData[0] + (pData[1]<<8) + (pData[2]<<16) + (pData[3]<<24));
		//????
		//if (ABS(gEvChargeInfo.fMaxOffLineTime - fMaxOffLineTime) > 0.03)
		//{
		//	gEvChargeInfo.fMaxOffLineTime = fMaxOffLineTime;
		//	gEvChargeInfo.u8FloatParamUpDataCount++;
		//}
	}
}


void UnPackXXXFrame(UINT8* pData, UINT16 u16Len)
{
	UINT32 u32CheckS = 0;
	UINT8 u8CmdId = 0;
	//UINT8 u8SequenceId = 0;
	UINT16 u16Idx = 0;

	if (u16Len >= NET_MAX_BUF)
	{
		OSTimeDly(2);
		return;				//内存泄漏！
	}

	if ((pData[Z_SOI_PST] != (Z_SOI_VALUE & 0xff))
		|| (pData[Z_SOI_PST + 1] != (Z_SOI_VALUE >> 8) & 0xff))
	{
		OSTimeDly(2);
		return;
	}

	//if (pData[Z_VERSION_PST] != 0x11)
	//{
	//	return;
	//}

	if (Z_FACTORY_INFY != pData[Z_FACTORY_PST])
	{
		OSTimeDly(2);
		return;
	}

	//充电桩编码
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		if (pData[u16Idx + Z_STATION_CODE_PST] != gu8EVStationNo[u16Idx])
		{
			OSTimeDly(2);
			return;
		}
	}
	//CheckSum
	for (u16Idx = Z_CMD_PST; u16Idx < (u16Len - 1) && u16Idx < NET_MAX_BUF; u16Idx++)
	{
		u32CheckS += pData[u16Idx];
	}

	if ((u32CheckS & 0xff) != pData[u16Len - 1])
	{
		OSTimeDly(2);
		return;
	}
	//来任何数据 都清0
	gNET_TCP_Info.u32BreakCount =0;

	//u8SequenceId = pData[Z_SEQUENCE_PST];	//如果回包，必须一致！
	u8CmdId = pData[Z_CMD_PST];
	//NET_PRINTF("CMD=%x Type=%x \n",u8CmdId, pData[Z_DATA_PST + 4]);
	switch (u8CmdId)
	{
	case Z_CMD_TIME_SYNC:			//0x42  时间同步设置
		UnPackSyncTime_0X42(pData);
		break;
	case Z_CMD_HEARDBEAT:			//0x48  注册心跳应答
		UnPackHeardbeat_0X48(pData);
		break;
	case Z_CMD_HEARDBEAT_INFO:		//0x49	心跳信息设置/查询
		UnPackHeardbeatInfo_0X49(pData);
		break;
	case Z_CMD_COMM_MODE:			//0xE0	运行模式设置
		UnPackComm_Mode_0XE0(pData);
		gEvChargeInfo.u8RcvRunModeFlag = TRUE;
		break;
	case Z_CMD_EV_STATUS:			//0x2C	充电机/枪运行状态
		//UnPackEV_Status_0X2C(pData);
		break;
	case Z_CMD_SIGN_IN:			//0x01	登录应答/查询
		UnPackSign_In_0X01(pData);
		break;
	case Z_CMD_BMS_INFO:			//0x22
		UnPackBms_Info_0X22(pData);
		break;
	case Z_CMD_RECT_INFO:			//0x23
		UnPackRect_Info_0X23(pData);
		break;
	case Z_CMD_CARD_INFO:			//0x60
		//UnPackCARD_Info_0X60(pData);
		break;
	case Z_CMD_CHARGE_RECORD:		//0x63		78
		//UnPackCharge_Record_0X63(pData);
		break;
	case Z_CMD_STEP_KWH_ACK:
		//UnPackStep_KWH_0X62(pData);
		break;
	case Z_CMD_REMOTE_START:		//0x06		远端APP开始从点
		//UnPackRemote_Start_0X06(pData);	
		break;
	case Z_CMD_REMOTE_STOP:			//0x09		远端APP停止充电
		//UnPackRemote_Stop_0X09(pData);
		break;
	case Z_CMD_CHARGE_INFO:			//0xE1		查询当前充电信息
		UnPackCharge_Info_0XE1(pData);
		break;
	case Z_CMD_SPEAR_STATUS:		//0x2B		没有太多用，用于 维护
		UnPackSpear_Status_0X2B(pData);
		break;
	case Z_CMD_TRADE:			//0x2A 下传交易号
		UnPackTrade_0X2A(pData);
		break;
	default:
		break;
	}
}

void AnalayseNetData(void)
{
	UINT8 u8TryTimes = 0;
	UINT16 u16NextFramePst = 0;
	UINT16 u16FrameLen = 0;
	UINT16 u16TotalRLen = 0;
	UINT8 u8Idx = 0;
	for (u8Idx = 0; u8Idx < 20; u8Idx++)
	{
		u16TotalRLen = NET_Read(&gNetR_Buf);
		u16NextFramePst = 0;
		u16FrameLen = 0;
		if (u16TotalRLen >= Z_FRAME_MIN_LEN)
		{
			//gEvChargeInfo.i32OffLineTimeCount = (INT32)(gEvChargeInfo.fMaxOffLineTime * 1000);
			NetPipeWHDOG();
			for (u8TryTimes = 0; u8TryTimes < Z_PACKET_SPLICING_NUM && u8TryTimes < 255; u8TryTimes++)
			{
				if (TRUE == GetOneFrame(u16TotalRLen, &u16NextFramePst, &u16FrameLen, gNetR_Buf, gNetBuf))
				{
					//UnPack!!
					UnPackXXXFrame(gNetBuf, u16FrameLen);
				}
				else
				{
					break;
				}
			}
		}
		OSTimeDly(3);
	}
}


void PackSign_In_0X10(UINT8 SqcId)
{
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	float fRetadPower = 0;
	UINT16 u16Value = 0;
	UINT16 uIdx = 0;
	UINT8 u8TempValue, u8TempValue2;
	__XXX_UNION_VALUE unVal;
	for (uIdx = 0; uIdx < 16; uIdx++)
	{
		//0011223344 --> 0030	0030	0031	0031	0032	0032
		ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + (uIdx * 2), &unVal, NULL, BY_SRAM);
		u8TempValue = unVal.chtemp[0];
		ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + (uIdx * 2) + 1, &unVal, NULL, BY_SRAM);
		u8TempValue2 = unVal.chtemp[0];;
		gu8EVStationNo[uIdx]= TwoAscii2Hex(u8TempValue,u8TempValue2);
	}

	/***********************************************************
		faf5c200110600000000000000100010000000000000000000000010000000000000000000000000000000000000000044435353563130303b413030
		000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
		ffffffffffffffffffffffffffffffffffffffff01701700000000000000000000000000000000000000000000000000000000000000000000000000
		00000000000000000000000000000000000000000000000000000084
	************************************************************/
	UNUSED(fRetadPower);
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;


	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_SIGN_IN;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 用户ID
	gNetS_Buf[Z_DATA_PST] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;	//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//2 指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	u16BufIdx = Z_DATA_PST + 4;
	//3 充电桩编号  8字节
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = gu8EVStationNo[u16Idx];
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	u16BufIdx += 8;
	//4 充电桩设备资产编码	20字节 可选 直接填 '\0'
	for (u16Idx = 0; u16Idx < 20; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = '\0';
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//5 充电桩版本	64字节
	u16BufIdx += 20;
	for (u16Idx = 0; u16Idx < EV_VERSION_BYTES_NUM; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = gu8EVVersion[u16Idx];
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;

	}

	//6 额定电压  未知 则默认为0  4字节  0.1
	u16BufIdx += EV_VERSION_BYTES_NUM;
	for (u16Idx = 0; u16Idx < 4; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//7 辅助电压  未知 则填0 4字节
	u16BufIdx += 4;
	for (u16Idx = 0; u16Idx < 4; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//8 充电桩软件 已经持续运行时间 （可选） 默认0xff 4字节
	u16BufIdx += 4;
	for (u16Idx = 0; u16Idx < 4; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//9 充电桩 最近一次启动时间 （可选） 默认0xff  8字节
	u16BufIdx += 4;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//10 充电桩 最近一次签到时间 （可选） 默认0xff  8字节
	u16BufIdx += 8;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//11 本桩充电枪 枪数 必填
	u16BufIdx += 8;
	gNetS_Buf[u16BufIdx] = 0x01;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;


	//12 本桩功率 2 字节
	u16BufIdx += 1;	
	fRetadPower = STATION_RETAD_POWER * 100;	//0.01kw
	u16Value = (UINT16)fRetadPower;
	gNetS_Buf[u16BufIdx] = u16Value & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 1] = (u16Value>>8) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx + 1];
	u16DataLen++;

	//13 SIM卡 内置 ICCID 32字节 无则填充\0  32字节
	u16BufIdx += 2;
	for (u16Idx = 0; u16Idx < 32; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = '\0';
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//14 内置模块IMSI 32字节 无则填充\0
	u16BufIdx += 32;
	for (u16Idx = 0; u16Idx < 32; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = '\0';
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//校验和
	u16BufIdx += 32;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
}



void NET_Sign_0X10Deal(void)
{
	UINT16 u16Idx = 0;
	AnalayseNetData();
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	if (gEvChargeInfo.u8Sign_Result == ZTE_SIGN_RESULT_ERR)
	{
		//gEvChargeInfo.i32_0X3A_Check_Stat = FALSE;
		//gEvChargeInfo.u82Approve_0X3A_Send_Flag = FALSE;					//上报允许成功！
		if (gEvChargeInfo.i32Sign_Interval <= 0)
		{
			gEvChargeInfo.i32Sign_Interval = 5 * TICKS_COUT_UNINT;
			PackSign_In_0X10(0);											//登录
		}
		for (u16Idx = 0; u16Idx < 500; u16Idx++)
		{
			AnalayseNetData();
			if (gEvChargeInfo.u8Sign_Result == ZTE_SIGN_RESULT_OK)
			{
				break;
			}
			OSTimeDly(10);
		}
		gEvChargeInfo.i32HeardBeat_Count = gEvChargeInfo.i32HeardBeat_Period * TICKS_COUT_UNINT;
		gEvChargeInfo.i32HeardBeat_TimeOutCount = (gEvChargeInfo.i32HeardBeat_Period * gEvChargeInfo.u8HeardBeatTimes) * TICKS_COUT_UNINT;
	}
	return;
}

void InitNetData(void)
{
	__XXX_UNION_VALUE unTempVal,unVal;
	UINT8 u8TempValue = 0;
	UINT8 u8TempValue2 = 0;
	UINT16 uIdx = 0;
	INT32 iTempVal = 0;
	void *PEInfo;
	UINT8 u8Idx = 0;
	struct tm  tmNow;
	gRTCData.fGetLocalTime(&tmNow);
	gEvChargeInfo.PowerOnTime = tmNow;

	gNET_TCP_Info.u8NetIsBreakFlag = TRUE;
	//gEvChargeInfo.i16HeardBeatCount = 0;					//3000;		//30S 注册一下心跳
	//RefreshQRCode();
	//for (u8Idx = 0; u8Idx < EV_VERSION_BYTES_NUM; u8Idx++)
	//{
	//	gu8EVStationNo[u8Idx]= '\0';
	//}
	for (u8Idx = 0; u8Idx < 64; u8Idx++)
	{
		gu8EVVersion[u8Idx] = '\0';
	}
	gu8EVVersion[0] = 'D';
	gu8EVVersion[1] = 'C';
	gu8EVVersion[2] = 'D';
	gu8EVVersion[3] = 'S';
	gu8EVVersion[4] = 'V';
	gu8EVVersion[5] = '1';
	gu8EVVersion[6] = '0';
	gu8EVVersion[7] = '0';
	gu8EVVersion[8] = ';';
	gu8EVVersion[9] = 'A';
	gu8EVVersion[10] = '0';
	gu8EVVersion[11] = '0';
	for (u8Idx = 0; u8Idx < 32; u8Idx++)
	{
		g8Mac[u8Idx]= '\0';
	}
	g8Mac[0] = '0';
	g8Mac[1] = '0';
	g8Mac[2] = '0';
	g8Mac[3] = '9';
	g8Mac[4] = 'f';
	g8Mac[5] = '5';
	g8Mac[6] = '1';
	g8Mac[7] = '2';
	g8Mac[8] = '8';
	g8Mac[9] = 'a';
	g8Mac[10] = 'b';
	g8Mac[11] = 'c';
	g8Mac[12] = '\0';
	for(u8Idx = 0; u8Idx < 6; u8Idx++)
	{
		PEInfo = GetgEquipInfo(E_TYPE_SYS);
		g8Mac[u8Idx] = (UINT16)(((EV_CHARGE_INFO*)PEInfo)->u8Mac[u8Idx]);
	}

	gEvChargeInfo.u8NetLogInFails = FALSE;
	gEvChargeInfo.u8RcvRunModeFlag = FALSE;
	gEvChargeInfo.u8Sign_Result = ZTE_SIGN_RESULT_ERR;
	gEvChargeInfo.i32HeardBeat_Period = 15;																							//30S 注册心跳周期
	gEvChargeInfo.i32HeardBeat_Count = 0;
	gEvChargeInfo.u8HeardBeatTimes = 5;

	gNET_TCP_Info.u8DHCPResult = FALSE;

	for (uIdx = 0; uIdx < 16; uIdx++)
	{
		//0011223344 --> 0030	0030	0031	0031	0032	0032
		ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + (uIdx * 2), &unVal, NULL, BY_SRAM);
		u8TempValue = unVal.chtemp[0];
		ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + (uIdx * 2) + 1, &unVal, NULL, BY_SRAM);
		u8TempValue2 = unVal.chtemp[0];
		gu8EVStationNo[uIdx]= TwoAscii2Hex(u8TempValue, u8TempValue2);
	}

	for (uIdx = 0; uIdx < ETHER_SEM_NUM; uIdx++)
	{
		gEvChargeInfo.u8EherSem[uIdx] = ETHER_SEM_INVALID;
	}
	Put_Ether_3CSem(POWER_ON_3C);
}

void PackHeardBeat_0X58(UINT8 SqcId)
{
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	//float fRetadPower = 0;
	UINT16 u16Value = 0;
	UNUSED(u16Value);
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_HEARDBEAT;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 2用户ID
	gNetS_Buf[Z_DATA_PST] = 0;	//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;	//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//3 4指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	u16BufIdx = Z_DATA_PST + 4;
	// 5 6 心跳应答 ！
	gNetS_Buf[u16BufIdx] = 0;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 1] = 0;
	u32CheckSum += gNetS_Buf[u16BufIdx + 1];
	u16DataLen++;

	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
}

void NET_HeardBeat_0X58Deal(void)
{
	AnalayseNetData();

	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	//if (gEvChargeInfo.u8RcvRunModeFlag == FALSE)
	//{
	//	return;
	//}

	//if (gEvChargeInfo.u8RcvRunModeFlag == FALSE)
	//{
	//	gEvChargeInfo.i32HeardBeat_TimeOutCount = 500 * TICKS_COUT_UNINT;
	//	return;
	//}

	if (gEvChargeInfo.u8Sign_Result == ZTE_SIGN_RESULT_OK)
	{
		if (gEvChargeInfo.i32HeardBeat_Count <= 0)
		{
			gEvChargeInfo.i32HeardBeat_Count = gEvChargeInfo.i32HeardBeat_Period * TICKS_COUT_UNINT;
			PackHeardBeat_0X58(ZTE_SQC_VOLUNTARY);//
		}

		if (gEvChargeInfo.i32HeardBeat_TimeOutCount <= 0)
		{
			//说明需要重新登录
			//gEvChargeInfo.u8Sign_Result = ZTE_SIGN_RESULT_ERR;
		}
	}
}
UINT8 GetEV_StatusCode(UINT8 u8SpearNo);
//00 （桩或枪）空闲	01（桩或枪）正在使用中  02（桩或枪）出错	03（枪或桩）不可用	04（枪或桩）预约状态	05（桩）紧急停止
UINT8 GetEV_StatusCode(UINT8 u8SpearNo)
{
	UINT8 u8Rst = 0;
	if (ZTEV_EV_STATION == u8SpearNo)
	{
		if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL) || (STOP_CHARGE_VAL == gEvChargeInfo.u8_Charge_CMD[GUN_NO_B]))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	if (ZTEV_EV_SPEAR_1 == u8SpearNo)
	{
		if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL))
		{
			return 0;														//0 空闲
		}
		else
		{
			return 1;														//1 枪正在使用
		}
	}
	else
	{
		if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL))
		{
			return 0;														//0 空闲
		}
		else
		{
			return 1;														//1 枪正在使用
		}
	}


	return 0;//空闲
}
//u8RptType 主动被动  u8No几号枪信息
void PackEv_Stat_0X3C(UINT8 SqcId,UINT8 u8RptType, UINT8 u8No)
{
	struct tm  tmNow;
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	UINT16 u16Value = 0;
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;													//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_EV_STATUS;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 用户ID
	gNetS_Buf[Z_DATA_PST] = 0;											//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;										//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//2 指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//3 主动、被动
	u16BufIdx = Z_DATA_PST + 4;
	gNetS_Buf[u16BufIdx] = u8RptType;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//4充电枪 充电桩 编号
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = u8No;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//5 状态码 1
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = 0;	//GetEV_StatusCode();
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//6 插枪状态 1
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = 0;//GetEV_SpearConnctStat();
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//6 错误码 1
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = 0;	//GetEV_ErrorCode();
	//NET_PRINTF("	ErrCode=%d\n",gEvChargeInfo.u8ErrCode);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//7 时间
	//年
	u16BufIdx+=1;
	gRTCData.fGetLocalTime(&tmNow);
	u16Value = tmNow.tm_year;
	gNetS_Buf[u16BufIdx] = u16Value & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 1] = (u16Value>>8) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx + 1];
	u16DataLen++;
	//月 
	u16BufIdx+=2;
	gNetS_Buf[u16BufIdx] = tmNow.tm_mon & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//日
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = tmNow.tm_mday & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//时
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = tmNow.tm_hour & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//分
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = tmNow.tm_min & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//秒
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = tmNow.tm_sec & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//时间 保留
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//校验和
	u16BufIdx+=1;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
}

/******************************************************************************************************************************************
		1) 签到成功发送
		2）3A 充电成功后 上报充电桩和充电枪状态
		3）充电桩和充电枪发生故障后 立即上报
		4）故障恢复和排除后
		5）网管下发查询
		6）插枪拔枪
********************************************************************************************************************************************/
void NET_EvStat_0X3CDeal(void)
{
	AnalayseNetData();
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	if (gEvChargeInfo.u8RcvRunModeFlag == FALSE)
	{
		return;
	}

	if (Pop_Ether_3CSem() != ETHER_SEM_INVALID)
	{
		PackEv_Stat_0X3C(ZTE_SQC_VOLUNTARY, ZTE_RSP_AUTO_MODE, ZTEV_EV_STATION);
		OSTimeDly(100);
		PackEv_Stat_0X3C(ZTE_SQC_VOLUNTARY, ZTE_RSP_AUTO_MODE, ZTEV_EV_SPEAR_1);
		OSTimeDly(100);
		PackEv_Stat_0X3C(ZTE_SQC_VOLUNTARY, ZTE_RSP_AUTO_MODE, ZTEV_EV_SPEAR_2);
		OSTimeDly(100);
	}
}

UINT8 u8GetChargeMode(UINT8 u8SpearNo);
UINT8 u8GetChargeMode(UINT8 u8SpearNo)
{
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);						//EV_CHARGE_INFO
	if (1 == u8SpearNo)
	{
		return ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_A];
	}
	else
	{
		return ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_B];
	}
}

float fGetChargeArgs(UINT8 u8SpearNo);
float fGetChargeArgs(UINT8 u8SpearNo)
{
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);						//EV_CHARGE_INFO
	if (u8SpearNo == 1)
	{
		if (BMS_AUTO == ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_A])						//无含义
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_A];
		}
		else if (BMS_TIME == ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_A])
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_A];
		}
		else if (BMS_ENERGY == ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_A])
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_A];
		}
		else //BMS_SOC
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_A];
		}
	}
	else
	{
		if (BMS_AUTO == ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_B])						//无含义
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_B];
		}
		else if (BMS_TIME == ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_B])
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_B];
		}
		else if (BMS_ENERGY == ((EV_CHARGE_INFO*)PChargeInfo)->u8ChargeMode[GUN_NO_B])
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_B];
		}
		else //BMS_SOC
		{
			return ((EV_CHARGE_INFO*)PChargeInfo)->fChargeParam[GUN_NO_B];
		}
	}
	return 0;
}

float fGetStartKwh(UINT8 u8SpearNo);
float fGetStartKwh(UINT8 u8SpearNo)
{
	if (1 == u8SpearNo)
	{
		return gEvChargeInfo.fWH_StarVal[GUN_NO_A];
	}
	else
	{
		return gEvChargeInfo.fWH_StarVal[GUN_NO_B];
	}
}

void PackIApprove_Charge_0X3A(UINT8 SqcId, UINT8 u8SpearNo, UINT8 u8RspType, UINT8 u8StartType)
{
	struct tm  tmNow;
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	float fChargeArgs = 0;
	float fStartKwh = 0;
	EV_CHARGE_INFO *PChargeInfo = GetgEquipInfo(E_TYPE_SYS);						//EV_CHARGE_INFO
	//UINT32 u32Value = 0;
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_APPROVE_CHARGE;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 用户ID
	gNetS_Buf[Z_DATA_PST] = 0;			//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;		//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//2 指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;	//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;	//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//3 模块位置编号
	u16BufIdx = Z_DATA_PST + 4;
	gNetS_Buf[u16BufIdx] = u8SpearNo;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//4 充电枪编号
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = u8SpearNo;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//5 卡号 或 网管平台下发信息 用户信息
	u16BufIdx += 1;
	void *PEInfo = GetgEquipInfo(E_TYPE_BRUSH);
	for (u16Idx = 0; u16Idx < CARD_NO_BYTES; u16Idx++)
	{
		if (1 == u8SpearNo)
		{
			gNetS_Buf[u16BufIdx + u16Idx] = ((BRUSH_INFO*)PEInfo)->u8CARDInfo[GUN_NO_A][u16Idx];
		}
		else
		{
			gNetS_Buf[u16BufIdx + u16Idx] = ((BRUSH_INFO*)PEInfo)->u8CARDInfo[GUN_NO_B][u16Idx];
		}
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//6 是否允许充电
	u16BufIdx += 64;
	gNetS_Buf[u16BufIdx] = 0;						//u8GetApproveChargeVal()| (u8RspType<<6);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//7 当前时间
	u16BufIdx += 1;
	gRTCData.fGetLocalTime(&tmNow);
	gNetS_Buf[u16BufIdx] = tmNow.tm_year & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 1] = (tmNow.tm_year>>8) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 2] = tmNow.tm_mon;
	u32CheckSum += gNetS_Buf[u16BufIdx+2];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 3] = tmNow.tm_mday;
	u32CheckSum += gNetS_Buf[u16BufIdx+3];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 4] = tmNow.tm_hour;
	u32CheckSum += gNetS_Buf[u16BufIdx+4];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 5] = tmNow.tm_min;
	u32CheckSum += gNetS_Buf[u16BufIdx+5];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 6] = tmNow.tm_sec;
	u32CheckSum += gNetS_Buf[u16BufIdx+6];
	u16DataLen++;
	gNetS_Buf[u16BufIdx + 7] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+7];
	u16DataLen++;

	//8 预估时间
	u16BufIdx += 8;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//9 启动类型 1刷卡  2APP启动
	u16BufIdx += 8;
	gNetS_Buf[u16BufIdx] = u8StartType;//1 刷卡 2APP
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//10 充电策略
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = u8GetChargeMode(u8SpearNo);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//11 充电策略参数
	fChargeArgs = fGetChargeArgs(u8SpearNo);
	u16BufIdx += 1;
	if (0 == fChargeArgs)						//无含义	说明BMS 自动充满方式
	{
		gNetS_Buf[u16BufIdx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 2] =0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+2];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 3] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+3];
		u16DataLen++;
	}
	else //秒		SOC		金额	电量
	{
		gNetS_Buf[u16BufIdx] = ((UINT32)(fChargeArgs))&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 1] = ((UINT32)(fChargeArgs)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 2] = ((UINT32)(fChargeArgs)>>16)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+2];
		u16DataLen++;
		gNetS_Buf[u16BufIdx + 3] = ((UINT32)(fChargeArgs)>>24)&0xff;;
		u32CheckSum += gNetS_Buf[u16BufIdx+3];
		u16DataLen++;
	}

	//12 失败原因
	u16BufIdx += 4;
	gNetS_Buf[u16BufIdx] = gEvChargeInfo.u32ChargeFailtReason;					//0X3A充电失败原因！！！
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//13 充电模式
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = 1;										//单枪！！
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//14 起始电表读数 4 字节 精度0.01
	fStartKwh = fGetStartKwh(u8SpearNo);
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = ((UINT32)(fStartKwh * 100)) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = (((UINT32)(fStartKwh * 100)>>8)) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+2] = (((UINT32)(fStartKwh * 100)>>16)) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+2];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+3] = (((UINT32)(fStartKwh * 100)>>24)) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+3];
	u16DataLen++;

	//15 预留  4字节 
	u16BufIdx += 4;
	gNetS_Buf[u16BufIdx] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+2] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+2];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+3] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+3];
	u16DataLen++;

	//校验和
	u16BufIdx += 4;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
}




void NET_Approve_0X3ADeal(void)
{
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	if (gEvChargeInfo.u8RcvRunModeFlag == FALSE)
	{
		return;
	}
}

void PackBms_Info_0X32(UINT8 SqcId, UINT8 u8MaxNum)
{
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	//UINT32 u32Value = 0;
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_BMS_INFO;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 用户ID
	gNetS_Buf[Z_DATA_PST] = 0;			//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;		//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//2 指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;		//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;		//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//3 充电枪个数
	u16BufIdx = Z_DATA_PST + 4;
	gNetS_Buf[u16BufIdx] = u8MaxNum;				//2	个 充电枪
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;


	//4  充电枪数据是否有效		1有效 0无效
	u16BufIdx += 1;
	if (CHARGING_RCV_BCL_SEND_CCS == gBms_1_CommInfo.i8_CMD_Phase)
	{
		gNetS_Buf[u16BufIdx] = 1;
	}
	else
	{
		gNetS_Buf[u16BufIdx] = 0;//无效
	}

	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//5  充电枪位置编号		1
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = 1;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//6 充电枪在位状态	1在	0空
	u16BufIdx += 1;
	if (gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] != SPEAR_ON_LINE)
	{
		gNetS_Buf[u16BufIdx] = 0;
	}
	else
	{
		gNetS_Buf[u16BufIdx] = 1;
	}
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//7 BMS编码可选
	u16BufIdx += 1;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[u16BufIdx + u16Idx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
		u16DataLen++;
	}

	//8 电池总电压  2字节 0.1 若无0XFF
	u16BufIdx += 8;
	gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_1_Info.fBCS_MeasureVolt * 10)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_1_Info.fBCS_MeasureVolt * 10)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//9 电池总电流  精度0.01
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_1_Info.fBCS_MeasureCurr * 100 + 3200)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_1_Info.fBCS_MeasureCurr * 100 + 3200)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//10 最高单体电压  精度0.01
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_1_Info.fBCS_MaxCellVolt * 100)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_1_Info.fBCS_MaxCellVolt * 100)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//11 最低单体电压 精度0.01
	u16BufIdx += 2;
	if (gEVBms_1_Info.fBSD_MinCellVolt <= 0)
	{
		gNetS_Buf[u16BufIdx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;
	}
	else
	{
		gNetS_Buf[u16BufIdx] =(UINT16)(gEVBms_1_Info.fBSD_MinCellVolt * 100)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_1_Info.fBSD_MinCellVolt * 100)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;
	}


	//12 温度最高测点值
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_1_Info.fBSM_MaxBattTemp+50);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//13 温度最低测点值
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_1_Info.fBSM_MinBattTemp + 50);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//14 最高单体电压位置 2字节
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_1_Info.u8BSM_MaxCellVoltGrpNo)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_1_Info.u8BSM_MaxCellVoltGrpNo)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//15 最低单体电压位置 2字节
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//16 温度测点最高值位置1
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = gEVBms_1_Info.u8BSM_MaxCellVoltGrpNo;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//17 温度测点最低值位置1
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//18 SOH1
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_1_Info.fBCP_CurrSOC);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//19 SOC 
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_1_Info.fBCS_SOC);
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//20 充电枪工作状态	1充电 2静止 3休眠 4故障
	u16BufIdx += 1;
	if (CHARGING_RCV_BCL_SEND_CCS == gBms_1_CommInfo.i8_CMD_Phase)
	{
		gNetS_Buf[u16BufIdx] = 1;
	}
	else
	{
		gNetS_Buf[u16BufIdx] = 2;
	}
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	/**********************************************************************************************************************************
			2枪
	***********************************************************************************************************************************/
	if (u8MaxNum > 1)
	{
		u16BufIdx += 1;
		if (CHARGING_RCV_BCL_SEND_CCS == gBms_2_CommInfo.i8_CMD_Phase)
		{
			gNetS_Buf[u16BufIdx] = 1;
		}
		else
		{
			gNetS_Buf[u16BufIdx] = 0;//无效
		}
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//5  充电枪位置编号		1
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = 2;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//6 充电枪在位状态	1在		0空
		u16BufIdx += 1;
		if (gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] != SPEAR_ON_LINE)
		{
			gNetS_Buf[u16BufIdx] = 0;
		}
		else
		{
			gNetS_Buf[u16BufIdx] = 1;
		}
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//7 BMS编码可选
		u16BufIdx += 1;
		for (u16Idx = 0; u16Idx < 8; u16Idx++)
		{
			gNetS_Buf[u16BufIdx + u16Idx] = 0xff;
			u32CheckSum += gNetS_Buf[u16BufIdx + u16Idx];
			u16DataLen++;
		}

		//8 电池总电压  2字节 0.1 若无0XFF
		u16BufIdx += 8;
		gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_2_Info.fBCS_MeasureVolt * 10)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_2_Info.fBCS_MeasureVolt * 10)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//9 电池总电流  精度0.01
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_2_Info.fBCS_MeasureCurr * 100 + 3200)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_2_Info.fBCS_MeasureCurr * 100 + 3200)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//10 最高单体电压  精度0.01
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_2_Info.fBCS_MaxCellVolt * 100)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_2_Info.fBCS_MaxCellVolt * 100)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//11 最低单体电压 精度0.01
		//gEvChargeInfo.fBSD_MinCellVolt
		u16BufIdx += 2;
		if (gEVBms_2_Info.fBSD_MinCellVolt <= 0)
		{
			gNetS_Buf[u16BufIdx] = 0xff;
			u32CheckSum += gNetS_Buf[u16BufIdx];
			u16DataLen++;
			gNetS_Buf[u16BufIdx+1] = 0xff;
			u32CheckSum += gNetS_Buf[u16BufIdx+1];
			u16DataLen++;
		}
		else
		{
			gNetS_Buf[u16BufIdx] =(UINT16)(gEVBms_2_Info.fBSD_MinCellVolt * 100)&0xff;
			u32CheckSum += gNetS_Buf[u16BufIdx];
			u16DataLen++;
			gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_2_Info.fBSD_MinCellVolt * 100)>>8)&0xff;
			u32CheckSum += gNetS_Buf[u16BufIdx+1];
			u16DataLen++;
		}


		//12 温度最高测点值
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_2_Info.fBSM_MaxBattTemp+50);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//13 温度最低测点值
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_2_Info.fBSM_MinBattTemp + 50);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//14 最高单体电压位置 2字节
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (UINT16)(gEVBms_2_Info.u8BSM_MaxCellVoltGrpNo)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(gEVBms_2_Info.u8BSM_MaxCellVoltGrpNo)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//15 最低单体电压位置 2字节
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//16 温度测点最高值位置1
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = gEVBms_2_Info.u8BSM_MaxCellVoltGrpNo;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//17 温度测点最低值位置1
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//18 SOH1
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_2_Info.fBCP_CurrSOC);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//19 SOC 
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (UINT8)(gEVBms_2_Info.fBCS_SOC);
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//20 充电枪工作状态	1充电 2静止 3休眠 4故障
		u16BufIdx += 1;
		if (CHARGING_RCV_BCL_SEND_CCS == gBms_2_CommInfo.i8_CMD_Phase)
		{
			gNetS_Buf[u16BufIdx] = 1;
		}
		else
		{
			gNetS_Buf[u16BufIdx] = 2;
		}
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
	}

	//校验和
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
	OSTimeDly(400);
}

//这里没有做区分！直接要不传1枪  要不传12枪  没做单独传2枪
void PackRect_Info_0X39(UINT8 SqcId, UINT8 u8MaxRectNum)
{
	UINT16 u16BufIdx = 0;
	UINT16 u16Idx = 0;
	UINT32 u32CheckSum = 0;
	UINT16 u16DataLen = 0;
	UINT32 u32Value = 0;
	gNetS_Buf[Z_SOI_PST] = Z_SOI_VALUE & 0xff;
	gNetS_Buf[Z_SOI_PST + 1] = (Z_SOI_VALUE>>8) & 0xff;
	//gNetS_Buf[Z_LENGH_PST]
	//gNetS_Buf[Z_LENGH_PST + 1]
	gNetS_Buf[Z_VERSION_PST] = Z_VERSION_VALUE;
	gNetS_Buf[Z_FACTORY_PST] = Z_FACTORY_INFY;
	for (u16Idx = 0; u16Idx < 8; u16Idx++)
	{
		gNetS_Buf[Z_STATION_CODE_PST + u16Idx] = gu8EVStationNo[u16Idx];
	}

	gNetS_Buf[Z_SEQUENCE_PST] = SqcId;

	u32CheckSum = 0;				//从命令代码开始
	gNetS_Buf[Z_CMD_PST] = I_CMD_RECT_INFO;
	u32CheckSum += gNetS_Buf[Z_CMD_PST];

	//数据域 
	u16DataLen = 0;
	//1 用户ID
	gNetS_Buf[Z_DATA_PST] = 0;					//用户ID
	gNetS_Buf[Z_DATA_PST + 1] = 0;				//用户ID
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//2 指令序列
	gNetS_Buf[Z_DATA_PST + 2] = 0;				//指令序列
	gNetS_Buf[Z_DATA_PST + 3] = 0;				//指令序列
	u32CheckSum += 0;
	u32CheckSum += 0;
	u16DataLen++;
	u16DataLen++;

	//3 模块个数  即	充电枪个数
	u16BufIdx = Z_DATA_PST + 4;
	gNetS_Buf[u16BufIdx] = u8MaxRectNum;		// 充电枪个数
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;


	//4  充电模块 数据是否有效		1有效 0无效
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = 1;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//5  充电模块1 编号		1
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = 1;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//6 充电枪位置1
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = 1;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//7 充电枪位置1  累计充电 2字节  KWH	   0x39 上报模块充电信息，就先使用这个命令吧
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (((UINT16)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_A] * 1000))/10) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = (((((UINT16)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_A] * 1000))/10)>>8)) & 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//8 充电枪位置1  累计充电 2字节 AH 若无FF
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = 0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//9 充电枪位置1  累计充电 时间 S
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT16)((gEvChargeInfo.fBMS_ChargeTime[GUN_NO_A] * 60))&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = (((UINT16)(gEvChargeInfo.fBMS_ChargeTime[GUN_NO_A] * 60)) >> 8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//10 充电枪位置1  充电起始SOC
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT8)gEVBms_1_Info.fBCP_CurrSOC;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//11 充电枪位置1  当前SOC
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (UINT8)gEVBms_1_Info.fBCS_SOC;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;

	//12 充电枪位置1  电镀表读数 精度0.01
	u16BufIdx += 1;
	gNetS_Buf[u16BufIdx] = (UINT32)(gWH_Info[GUN_NO_A].fWHRealValue * 100)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT32)(gWH_Info[GUN_NO_A].fWHRealValue * 100)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+2] = ((UINT32)(gWH_Info[GUN_NO_A].fWHRealValue * 100)>>16)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+2];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+3] = ((UINT32)(gWH_Info[GUN_NO_A].fWHRealValue * 100)>>24)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+3];
	u16DataLen++;

	//13 当前功率 0.01		2字节
	u16BufIdx += 4;
	float fRctOutVolt = gs_CANComm.fGetRectAnalog(GRP_A, 0, ID_RECT_OUT_VOLT);
	float fRctOutCurr = gs_CANComm.fGetRectAnalog(GRP_A, 0, ID_RECT_OUT_CURR);
	u32Value = (UINT32)(100 * ((double)(fRctOutVolt * fRctOutCurr)/1000.00));
	gNetS_Buf[u16BufIdx] = u32Value&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = (u32Value>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//14 当前电流	0.01	2字节
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT16)(fRctOutCurr * 100 + 3200)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT16)(fRctOutCurr * 100 + 3200)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;

	//15 当前电压		0.1		2字节
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = (UINT16)(fRctOutVolt * 10)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx];
	u16DataLen++;
	gNetS_Buf[u16BufIdx+1] = ((UINT16)(fRctOutVolt * 10)>>8)&0xff;
	u32CheckSum += gNetS_Buf[u16BufIdx+1];
	u16DataLen++;


	/**************************************************************************************
								2222222
	*************************************************************************************/
	if (u8MaxRectNum > 1)
	{
		//4  充电模块  数据是否有效		1有效  0无效
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = 1;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//5  充电模块2 编号		1
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = 2;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//6 充电枪位置2
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = 2;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//7 充电枪位置2  累计充电 2字节  KWH	   0x39 上报模块充电信息，就先使用这个命令吧
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (((UINT16)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_B] * 1000))/10) & 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = (((((UINT16)(gEvChargeInfo.fOutEnergyByWH[GUN_NO_B] * 1000))/10)>>8)) & 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//8 充电枪位置2 累计充电 2字节 AH 若无FF
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = 0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//9 充电枪位置2  累计充电 时间 S
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)((gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B] * 60))&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = (((UINT16)(gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B] * 60)) >> 8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//10 充电枪位置2  充电起始SOC
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT8)gEVBms_2_Info.fBCP_CurrSOC;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//11 充电枪位置2 当前SOC
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (UINT8)gEVBms_2_Info.fBCS_SOC;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;

		//12 充电枪位置2  电镀表读数 精度0.01
		u16BufIdx += 1;
		gNetS_Buf[u16BufIdx] = (UINT32)(gWH_Info[GUN_NO_B].fWHRealValue * 100)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT32)(gWH_Info[GUN_NO_B].fWHRealValue * 100)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+2] = ((UINT32)(gWH_Info[GUN_NO_B].fWHRealValue * 100)>>16)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+2];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+3] = ((UINT32)(gWH_Info[GUN_NO_B].fWHRealValue * 100)>>24)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+3];
		u16DataLen++;

		//13 当前功率 0.01  2字节
		u16BufIdx += 4;
		fRctOutVolt = gs_CANComm.fGetRectAnalog(GRP_B, 0, ID_RECT_OUT_VOLT);
		fRctOutCurr = gs_CANComm.fGetRectAnalog(GRP_B, 0, ID_RECT_OUT_CURR);
		u32Value = (UINT32)(100 * ((double)(fRctOutVolt * fRctOutCurr)/1000.00));
		gNetS_Buf[u16BufIdx] = u32Value&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = (u32Value>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//14 当前电流	0.01	2字节
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)(fRctOutCurr * 100 + 3200)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(fRctOutCurr * 100 + 3200)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;

		//15 当前电压		0.1		2字节
		u16BufIdx += 2;
		gNetS_Buf[u16BufIdx] = (UINT16)(fRctOutVolt * 10)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx];
		u16DataLen++;
		gNetS_Buf[u16BufIdx+1] = ((UINT16)(fRctOutVolt * 10)>>8)&0xff;
		u32CheckSum += gNetS_Buf[u16BufIdx+1];
		u16DataLen++;
	}

	//校验和
	u16BufIdx += 2;
	gNetS_Buf[u16BufIdx] = u32CheckSum & 0xff;

	u16DataLen += Z_SQC_CMD_CHECK_L;		//包含从序列号域到校验和域的 所有字节长度
	gNetS_Buf[Z_LENGH_PST] = u16DataLen & 0xff;
	gNetS_Buf[Z_LENGH_PST + 1] = (u16DataLen >> 8)&0xff;

	NET_Send(gNetS_Buf, (u16BufIdx + 1), 100);
}

void NET_Bms_Rct_0X39_0X32Deal(void)
{
	UINT8 u8Idx = 0;
	gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL && gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
	{
		return;
	}
	AnalayseNetData();
	if (gEvChargeInfo.u8RcvRunModeFlag == FALSE)
	{
		return;
	}

	//模块
	if (gEvChargeInfo.i32FixedPeriodCount <= 0)
	{
		gEvChargeInfo.i32FixedPeriodCount = 15 * 1000;
		OSTimeDly(100);
		PackBms_Info_0X32(ZTE_SQC_VOLUNTARY, 2);
		OSTimeDly(800);
		for (u8Idx = 0; u8Idx < 50; u8Idx++)
		{
			AnalayseNetData();
			OSTimeDly(10);
		}
		OSTimeDly(100);
		PackRect_Info_0X39(ZTE_SQC_VOLUNTARY,2);
		OSTimeDly(800);
		for (u8Idx = 0; u8Idx < 50; u8Idx++)
		{
			AnalayseNetData();
			OSTimeDly(10);
		}
	}
}

void TASK_NET_ZhixinProc(void* pdata)
{
	OSTimeDly(500);
	OSTimeDly(500);
	OSTimeDly(500);
	InitNetData();

	initMqttClient();

	for(;;)
	{
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		OSTimeDly(10);
		NetPipeCheckWHDOG();
		if (gEvChargeInfo.u8NET_Mask == TRUE)
		{
			OSTimeDly(100);
			continue;
		}
		if (!PipeIsOk())													//管道是否建立OK
		{
			continue;
		}

		AnalayseNetData();
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		NET_Sign_0X10Deal();
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		NET_HeardBeat_0X58Deal();
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		NET_EvStat_0X3CDeal();
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		NET_Approve_0X3ADeal();
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		NET_Bms_Rct_0X39_0X32Deal();
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
	}
}