
#if !defined(__GNUC__) || ( __GNUC__ < 3) || (__GNUC__ == 3 && __GNUC_MINOR__ < 4)
#define checkreturn
#else
#define checkreturn __attribute__((warn_unused_result))
#endif

#include "Telaidian.pb.h"
#include "TelaidianProto.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "MQTTClient.h"
#include "main.h"
#include "interface.h"



#define	PROTOBUF_ENCODE_MESSAGE_BUF_LEN	2900
typedef struct _ProtobufEncodeMessageData
{
	unsigned char MessageBuffer[PROTOBUF_ENCODE_MESSAGE_BUF_LEN];
	unsigned short MessageLength;
}ProtobufEncodeMessageData;
ProtobufEncodeMessageData gs_ProtobufEncodeMessageData;


unsigned char ProtobufEncodeMessageFun(unsigned char u8PubMessageNo, ProtobufEncodeMessageData *pMessageData);

TelaidianCtrl_Def gs_TelaidianCtrl = {0};

/////////////////////////////////////////////////////////
////////////发布消息定义/////////////////////////////////
/////////////////////////////////////////////////////////
///1.upload/state/集控器地址
MQTTMessage	PublicMessage_EventStateUpload = 
{
	1,					//enum QoS qos;
	0,					//unsigned char retained;
	0,					//unsigned char dup;
	0,					//unsigned short id;
	"Topic01Message",	//(void *)PublicTopic01_Payloade, //void *payload;
	0,					//size_t payloadlen;
};
char PublicTopic_EventStateUpload[60];
MQTTString PublicMessage_EventStateUpload_Topic = 
{
	PublicTopic_EventStateUpload, //(char *)PublicTopic01, //char* cstring;	
	{							  //MQTTLenString lenstring;
		0,				//int len;
		0,				//char* data;
	},
};

///2.upload/data/集控地址
MQTTMessage	PublicMessage_EventUpload = 
{
	1,					//enum QoS qos;
	0,					//unsigned char retained;
	0,					//unsigned char dup;
	0,					//unsigned short id;
	"Topic02Message",	//(void *)PublicTopic01_Payloade, //void *payload;
	0,					//size_t payloadlen;
};
char PublicTopic_EventUpload[60];
MQTTString PublicMessage_EventUpload_Topic = 
{
	PublicTopic_EventUpload, //(char *)PublicTopic01, //char* cstring;	
	{						 //MQTTLenString lenstring;
		0,				//int len;
		0,				//char* data;
	},
};

///3.upload/lwt/集控器地址
MQTTMessage	PublicMessage_EventUploadLwt = 
{
	1,					//enum QoS qos;
	0,					//unsigned char retained;
	0,					//unsigned char dup;
	0,					//unsigned short id;
	"Topic03Message",	//(void *)PublicTopic01_Payloade, //void *payload;
	0,					//size_t payloadlen;
};
char PublicTopic_EventUploadLwt[60];
MQTTString PublicMessage_EventUploadLwt_Topic = 
{
	PublicTopic_EventUploadLwt, //(char *)PublicTopic01, //char* cstring;	
	{						 //MQTTLenString lenstring;
		0,				//int len;
			0,				//char* data;
	},
};

///4.request/data/集控地址
MQTTMessage	PublicMessage_EventRequest = 
{
	1,					//enum QoS qos;
	0,					//unsigned char retained;
	0,					//unsigned char dup;
	0,					//unsigned short id;
	"Topic04Message",	//(void *)PublicTopic01_Payloade, //void *payload;
	0,					//size_t payloadlen;
};
char PublicTopic_EventRequest[60];
MQTTString PublicMessage_EventRequest_Topic = 
{
	PublicTopic_EventRequest, //(char *)PublicTopic01, //char* cstring;	
	{						 //MQTTLenString lenstring;
		0,				//int len;
			0,				//char* data;
	},
};

//控制 消息发送
PublicMessage_Ctrl_def gs_PublicMessage_Ctrl[MAX_PUB_NUMBER] = {0};

/////////////////////////////////////////////////////////
////////////订阅消息定义/////////////////////////////////
/////////////////////////////////////////////////////////
void SubTopic01Func_pushdown_data(MessageData * pMessageData);
void SubTopic02Func_pushdown_state(MessageData * pMessageData);
void SubTopic03Func_response_data(MessageData * pMessageData);

messageHandler SubFunction[MAX_MESSAGE_HANDLERS] =
{
	SubTopic01Func_pushdown_data,
	SubTopic02Func_pushdown_state,
	SubTopic03Func_response_data
};

unsigned char g_ucSubTopicxxIsSubed[MAX_MESSAGE_HANDLERS] = {0};

MQTTMessage SubMqttMessageValue;
MessageData gs_SubMessageData[MAX_MESSAGE_HANDLERS] = {0};

MQTTString gs_SubMessageTopic[MAX_MESSAGE_HANDLERS] = {0};

//存放 接收到的uuid
char g_SubMessageUuidBuf[MAX_MESSAGE_HANDLERS][40] = {0};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


//20170923 MQTT
MQTTClient gs_MqttClient;

char SubTopic[MAX_MESSAGE_HANDLERS][60] = {0};


void PrepareSubTopicName(void)
{
	unsigned char i, j;
	//unsigned char StationID[32];
	__XXX_UNION_VALUE unVal;
	unsigned char ucAddrChanged;
	static unsigned char s_u8FirstTime = 1;

	char Password[6];

	ucAddrChanged = 0;

	//站点ID
	//memset(gs_TelaidianCtrl.TelaidianStationID, 0, sizeof(gs_TelaidianCtrl.TelaidianStationID));
	for (i = 0; i < 32; i++)
	{
		ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianStarionID_NUM_1 + i, &unVal, NULL, BY_SRAM);
		if(unVal.chtemp[0] == 0)
		{
			gs_TelaidianCtrl.TelaidianStationID[i] = 0;
			gs_TelaidianCtrl.Telaidian_QR_Code[0][i] = 0;
			gs_TelaidianCtrl.Telaidian_QR_Code[1][i] = 0;
			break;
		}
		if(gs_TelaidianCtrl.TelaidianStationID[i] != unVal.chtemp[0])
		{
			gs_TelaidianCtrl.TelaidianStationID[i] = unVal.chtemp[0];
			ucAddrChanged = 1;
		}
		gs_TelaidianCtrl.Telaidian_QR_Code[0][i] = unVal.chtemp[0];
		gs_TelaidianCtrl.Telaidian_QR_Code[1][i] = unVal.chtemp[0];
	}
	if(i < 31)
	{
		gs_TelaidianCtrl.Telaidian_QR_Code[0][i] = '0';
		gs_TelaidianCtrl.Telaidian_QR_Code[1][i] = '0';
		i++;
		gs_TelaidianCtrl.Telaidian_QR_Code[0][i] = '1';
		gs_TelaidianCtrl.Telaidian_QR_Code[1][i] = '2';
		i++;
		for(; i < 32; i++)
		{
			gs_TelaidianCtrl.Telaidian_QR_Code[0][i] = 0;
			gs_TelaidianCtrl.Telaidian_QR_Code[1][i] = 0;
		}
		
	}

	//memset(gs_TelaidianCtrl.TelaidianPassword, 0, sizeof(gs_TelaidianCtrl.TelaidianPassword));
	memset(Password, 0, sizeof(Password));
	for (i = 0; i < 32; i++)
	{
		ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianPassword_NUM_1 + i, &unVal, NULL, BY_SRAM);
		if(unVal.chtemp[0] == 0)
		{
			gs_TelaidianCtrl.TelaidianPassword[i] = 0;
			break;
		}
		if(gs_TelaidianCtrl.TelaidianPassword[i] != unVal.chtemp[0])
		{
			gs_TelaidianCtrl.TelaidianPassword[i] = unVal.chtemp[0];
			ucAddrChanged = 1;
		}		
	}
	if(i > 4)
	{
		for(j = 0; j < 5; j++)
		{
			Password[j] = gs_TelaidianCtrl.TelaidianPassword[i- 5 + j];
		}
	}


	

	//地址变化
	if(ucAddrChanged || s_u8FirstTime)
	{
		s_u8FirstTime = 0;

		for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
		{
			memset(SubTopic[i], 0, sizeof(SubTopic[i]));
			g_ucSubTopicxxIsSubed[i] = 0;		//需要重新订阅
		}	

		sprintf( SubTopic[SUB_MESSAGE_TOPIC01_pushdown_data], "%s%s/%s", "pushdown/data/", Password, (char *)gs_TelaidianCtrl.TelaidianStationID);
		sprintf( SubTopic[SUB_MESSAGE_TOPIC02_pushdown_state],"%s", "pushdown/state/all");
		sprintf( SubTopic[SUB_MESSAGE_TOPIC03_response_data], "%s%s/%s", "response/data/", Password, (char *)gs_TelaidianCtrl.TelaidianStationID);

		memset(PublicTopic_EventStateUpload, 0, sizeof(PublicTopic_EventStateUpload));
		sprintf( PublicTopic_EventStateUpload, "%s%s/%s", "upload/state/", Password, (char *)gs_TelaidianCtrl.TelaidianStationID);

		memset(PublicTopic_EventUpload, 0, sizeof(PublicTopic_EventUpload));
		sprintf( PublicTopic_EventUpload, "%s%s/%s", "upload/data/", Password, (char *)gs_TelaidianCtrl.TelaidianStationID);

		memset(PublicTopic_EventRequest, 0, sizeof(PublicTopic_EventRequest));
		sprintf( PublicTopic_EventRequest, "%s%s/%s", "request/data/", Password, (char *)gs_TelaidianCtrl.TelaidianStationID);

		memset(PublicTopic_EventUploadLwt, 0, sizeof(PublicTopic_EventUploadLwt));
		sprintf( PublicTopic_EventUploadLwt, "%s%s/%s", "upload/lwt/", Password, (char *)gs_TelaidianCtrl.TelaidianStationID);
	}


	//memset(gs_TelaidianCtrl.TelaidianUserName, 0, sizeof(gs_TelaidianCtrl.TelaidianUserName));
	for (i = 0; i < 32; i++)
	{
		ExGetSigInfo(E_TYPE_SYS, ID_SETDATA_TelaidianUserName_NUM_1 + i, &unVal, NULL, BY_SRAM);
		if(unVal.chtemp[0] == 0)
		{
			gs_TelaidianCtrl.TelaidianUserName[i] = 0;
			break;
		}
		if(gs_TelaidianCtrl.TelaidianUserName[i] != unVal.chtemp[0])
		{
			gs_TelaidianCtrl.TelaidianUserName[i] = unVal.chtemp[0];
		}		
	}
	
	//memset(gs_TelaidianCtrl.TelaidianStationName, 0, sizeof(gs_TelaidianCtrl.TelaidianStationName));
	for (i = 0; i < 60; i++)
	{
		ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + i, &unVal, NULL, BY_SRAM);
		if(unVal.chtemp[0] == 0)
		{
			gs_TelaidianCtrl.TelaidianStationName[i] = 0;
			break;
		}
		if(gs_TelaidianCtrl.TelaidianStationName[i] != unVal.chtemp[0])
		{
			gs_TelaidianCtrl.TelaidianStationName[i] = unVal.chtemp[0];
		}		
	}

}

void initMqttClient(void)
{
	unsigned char i;

	for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
	{
		gs_MqttClient.messageHandlers[i].topicFilter = 0;

		gs_SubMessageData[i].message = &SubMqttMessageValue;
		gs_SubMessageData[i].topicName = &gs_SubMessageTopic[i];
		gs_SubMessageTopic[i].cstring = SubTopic[i];
	}

	gs_MqttClient.keepAliveInterval = 45000;

	gs_MqttClient.command_timeout_ms = 15000;

	//gs_MqttClient.buf = gNET_TCP_Info.u8SendBuf.u8Buf;
	//gs_MqttClient.buf_size = NET_R_DATA_L;
	//gs_MqttClient.readbuf = gNetR_Buf;
	//gs_MqttClient.readbuf_size = NET_MAX_BUF;

	gs_MqttClient.buf = gNetS_Buf;
	gs_MqttClient.buf_size = NET_MAX_BUF;
	gs_MqttClient.readbuf = gNetR_Buf;
	gs_MqttClient.readbuf_size = NET_MAX_BUF;

	gs_MqttClient.isconnected = 0;
	gs_MqttClient.cleansession = 0;
	gs_MqttClient.ping_outstanding = 0;
	gs_MqttClient.defaultMessageHandler = NULL;
	gs_MqttClient.next_packetid = 1;

	TimerInit(&gs_MqttClient.last_sent);
	TimerInit(&gs_MqttClient.last_received);

#if defined(MQTT_TASK)
	MutexInit(&c->mutex);
#endif

}


bool Write_String(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	char *str = *arg;
	if (!pb_encode_tag_for_field(stream, field))
		return false;

	return pb_encode_string(stream, (uint8_t*)str, strlen(str));
}

typedef struct Telaidian_bytes_array_ {
	pb_size_t size;
	pb_byte_t *bytes;
}Telaidian_bytes_array_Def;
bool Write_bytes(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	Telaidian_bytes_array_Def *bytes = NULL;
	bytes = *arg;

	if (!pb_encode_tag_for_field(stream, field))
		return false;
	
	return pb_encode_string(stream, bytes->bytes, bytes->size);
}

typedef struct Telaidian_bytes_array_repeat {
	pb_size_t size1;
	pb_byte_t *bytes1;
	pb_size_t size2;
	pb_byte_t *bytes2;
}Telaidian_bytes_array_repeat_Def;
bool Write_bytes_repeat(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	Telaidian_bytes_array_repeat_Def *bytes = NULL;
	bytes = *arg;

	if (!pb_encode_tag_for_field(stream, field))
		return false;

	if(pb_encode_string(stream, bytes->bytes1, bytes->size1) == false)
	{
		return false;
	}

	pb_encode_tag_for_field(stream, field);
	return pb_encode_string(stream, bytes->bytes2, bytes->size2);
}

//static bool checkreturn pb_dec_string(pb_istream_t *stream, const pb_field_t *field, void *dest)
bool read_ints(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	unsigned char *str = *arg;
	while (stream->bytes_left)
	{
		uint64_t value;
		if (!pb_decode_varint(stream, &value))
			return false;

		*str++ = (unsigned char)value;
		//printf("%lld\n", value);
	}
	return true;
}
extern bool checkreturn pb_readbyte(pb_istream_t *stream, pb_byte_t *buf);
bool read_ints_beanbuf(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	pb_byte_t byte;
	unsigned char *str = *arg;
	while (stream->bytes_left)
	{
		if (!pb_readbyte(stream, &byte))
			return false;		

		*str++ = byte;
		//printf("%lld\n", value);
	}
	return true;
}

unsigned char TelaidianFaltCodeTransToStopReason(unsigned char FaltCode)
{
	unsigned char StopReason = 0;

	switch(FaltCode)
	{
		case 0x3D:  //充电模块输入过压
			StopReason = 5;  //充电机故障
			break;

		case 0x3E:  //充电模块输入欠压
			StopReason = 5;  //充电机故障
			break;

		case 0x4C:  //充电模块通信故障
			StopReason = 59;  //DC模块不响应
			break;
		case 0x52:  //刷卡器通信故障
			StopReason = 5;  //充电机故障
			break;

		case 0x55:  //EPO
			StopReason = 8;  //强制开关
			break;
		case 0x35:  //防雷故障
			StopReason = 5;  //充电机故障
			break;

		case 0x53:  //绝缘故障
			StopReason = 58;  
			break;
		
		case 0x50:  //电表通信故障
			StopReason = 17;  //电表通信故障
			break;

		default:
			break;
	}

	return StopReason;
}

void TelaidianEndReason(unsigned char GunNo, unsigned char EndReason)
{
	unsigned char TelaidianReason = 0;
	unsigned char StopChgReportMode;

	stBMS_INFO *pInfo;

	if(GunNo == 1)
	{
		pInfo = &gEVBms_2_Info;		
	}
	else
	{
		pInfo = &gEVBms_1_Info;
	}
	StopChgReportMode = TelaidianStopChgReportMode_3;  //3-因故障结束

	switch(EndReason)
	{
		case CEM_CRM00_TIMEOUT:
		case CEM_RCV_BCP_OR_S_CRMAA_TIMEOUT:
		case CEM_SEND_CML_RCV_BRO_TIMEOUT:
		case CEM_RCV_BCS_OR_S_CRO_TIMEOUT:
		case CEM_CST_BST_TIMEOUT:
		case CEM_BSD_CSD_TIMEOUT:
		case CEM_OTHER_TIMEOUT:
			TelaidianReason = 54;  //BMS通信超时
			break;

		case CRM00_LOCK_ERR:
			TelaidianReason = 11;  //电子锁异常
			break;

		case SELF_TEST_ERR:
			TelaidianReason = 58;  //绝缘异常
			break;

		case CRO_BUS_VOLT_ERR:
			TelaidianReason = 5;  //充电机故障
			break;

		case CHARGE_RECEIVE_BST:
			if((pInfo->u8BST_StopReason & 0x03) == 0x01)
			{
				StopChgReportMode = TelaidianStopChgReportMode_2;  //2-成功结束
				TelaidianReason = 66;  //SOC达到目标值
			}
			else if((pInfo->u8BST_StopReason & 0x0C) == 0x04)
			{
				StopChgReportMode = TelaidianStopChgReportMode_2;  //2-成功结束
				TelaidianReason = 67;  //电压达到目标值
			}
			else if((pInfo->u8BST_StopReason & 0x30) == 0x10)
			{
				StopChgReportMode = TelaidianStopChgReportMode_2;  //2-成功结束
				TelaidianReason = 68;  //单体电压达到目标值
			}
			else if((pInfo->u16BST_FaultReason & 0x03) == 0x01)
			{
				TelaidianReason = 69;  //绝缘故障
			}
			else if((pInfo->u16BST_FaultReason & 0x0C) == 0x04)
			{
				TelaidianReason = 70;  //连接器过温
			}
			else if((pInfo->u16BST_FaultReason & 0x30) == 0x10)
			{
				TelaidianReason = 71;  //BMS元件过温、连接器过温
			}
			else if((pInfo->u16BST_FaultReason & 0xC0) == 0x40)
			{
				TelaidianReason = 72;  //连接器故障 
			}
			else if((pInfo->u8BST_ErrorReason & 0x03) == 0x01)
			{
				TelaidianReason = 73;  //过温
			}
			else if((pInfo->u8BST_ErrorReason & 0x0C) == 0x04)
			{
				TelaidianReason = 74;  //高压继电器故障
			}
			else if((pInfo->u8BST_ErrorReason & 0x30) == 0x10)
			{
				TelaidianReason = 75;  //cc2电压异常
			}
			else if((pInfo->u8BST_ErrorReason & 0xC0) == 0x40)
			{
				TelaidianReason = 76;  //车辆其它故障
			}
			else
			{	
				TelaidianReason = 79;  //BMS中止
			}
			break;

		case CHARGE_BSM_ERR:  //BSM  报文有异常
			if((pInfo->byEV_BSM_byte6 & 0x03) != 0x00) 
			{
				TelaidianReason = 78;  //单体电压异常
			}
			else if((pInfo->byEV_BSM_byte6 & 0x30) == 0x10) 
			{
				TelaidianReason = 77;  //电流过大
			}
			else if((pInfo->byEV_BSM_byte6 & 0xC0) == 0x40) 
			{
				TelaidianReason = 73;  //过温
			}
			else if((pInfo->byEV_BSM_byte7 & 0x03) == 0x01) 
			{
				TelaidianReason = 69;  //BMS 绝缘故障
			}
			else if((pInfo->byEV_BSM_byte7 & 0x0C) == 0x04) 
			{
				TelaidianReason = 72;  //连接器故障
			}
			else 
			{
				TelaidianReason = 76;  //车辆其它故障
			}
			break;

		case BMS_SOC_100:
			TelaidianReason = 19;  //BMS满 中止
			StopChgReportMode = TelaidianStopChgReportMode_2;  //2-成功结束
			break;

		case RECEIVE_STOP_CMD_BY_NET:
			TelaidianReason = 2;  //平台中止
			StopChgReportMode = TelaidianStopChgReportMode_1;  //1-后台主动下发
			break;

		case LUI_STOP_CMD:
			TelaidianReason = 4;  //屏点击中止
			StopChgReportMode = TelaidianStopChgReportMode_4;  //4-本地屏点击终止
			break;
		case BRUSH_STOP_CMD:
			TelaidianReason = 92;  //离线刷卡中止?
			break;
		case CHARGE_DOOR_ALM_BY_BMS:
			TelaidianReason = 5;  //充电机故障?
			break;

		case CHARGE_ELE_LOCK_ALM_BY_BMS:
			TelaidianReason = 11;  //电子锁
			break;
		case CHARGE_SPEAR_CONNCT_ALM_BY_BMS:
			TelaidianReason = 6;  //充点枪连接异常
			break;

		case CHARGE_EPO_ALM_BY_BMS:
			TelaidianReason = 8;  //紧急关机
			break;
		case START_CHARGE_CC1_ALM:
			TelaidianReason = 7;  //连接器拔除?
			break;
		case BATT_VOLT_IS_NEGATIVE:
			TelaidianReason = 5;  //充电机故障
			break;

		case CHARGE_SPD_ALM:
			TelaidianReason = 5;  //充电机故障
			break;
		case CHARGE_WH_COMM_ALM:
			TelaidianReason = 17;  //电表通信故障
			break;
		case CHARGE_CELL_VOLT_ALM:  //节点压过高
			TelaidianReason = 78;  //单体电压异常？？？
			break;
		case CHARGE_OVER_CURR_ALM:  //输出过流（超过充电机最大电流）
			TelaidianReason = 13;  //充电电流异常
			break;

		case NET_BREAK_STOP:		//网络 APP告警停机
			TelaidianReason = 18;  //后台通信终止
			break; 
		default:
			break;
	}

	if(GunNo == 1)
	{
		gs_TelaidianCtrl.B_StopChgReport = StopChgReportMode; 
		gs_TelaidianCtrl.EndReason[1] = TelaidianReason;
	}
	else
	{
		gs_TelaidianCtrl.A_StopChgReport = StopChgReportMode; 
		gs_TelaidianCtrl.EndReason[0] = TelaidianReason;
	}	 
	
}

uint64_t GetLocalTimeStamp(void)
{
	
	uint64_t u64temp;
	unsigned long u32temp;

	/*if(gRTCData.localtimezone < 0)
	{
		u32temp = (-gRTCData.localtimezone*MIN_SECONDS);
		u64temp = gRTCData.RTCGMTSec - u32temp;
	}
	else
	{
		u32temp = (gRTCData.localtimezone*MIN_SECONDS);
		u64temp = gRTCData.RTCGMTSec + u32temp;
	}*/
	u64temp = gRTCData.RTCGMTSec;
	u64temp *= 1000;
	return u64temp;
}

////////////////////////////////////////////////////////////////////////////////////////
///SUB相关函数
////////////////////////////////////////////////////////////////////////////////////////
unsigned char BillOfStopCmdIsAllZero(unsigned char *pBill, unsigned char lenth)
{
	unsigned char i;
	for(i = 0; i < lenth; i++)
	{
		if(*pBill != '0')
		{
			return FALSE;
		}
	}
	return TRUE;
}

#define PROTOBUF_DECODE_MESSAGE_BUF_LEN		400
unsigned char g_Protobuf_Decode_Buf[PROTOBUF_DECODE_MESSAGE_BUF_LEN] = {0};
unsigned char beanBuf[200], billId[60];
void SubTopic01Func_pushdown_data(MessageData * pMessageData)
{
	bool status;
	unsigned char StationID[40];
	UINT8 u8Buf[6], i, *pBuf;
	struct tm  tmNow, tmNow_2;
	void* PEInfo;
	UINT8 uctemp, uctemp2, u8A_ChargeStat, u8B_ChargeStat;

	memset(g_Protobuf_Decode_Buf, 0, PROTOBUF_DECODE_MESSAGE_BUF_LEN);
	memcpy(g_Protobuf_Decode_Buf, pMessageData->message->payload, pMessageData->message->payloadlen);

	memset(StationID, 0, sizeof(StationID));
	memset(g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data], 0, sizeof(g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data]));
	memset(beanBuf, 0, sizeof(beanBuf));


	EventPushdown message = EventPushdown_init_zero;

	/* Create a stream that reads from the buffer. */
	pb_istream_t stream = pb_istream_from_buffer(g_Protobuf_Decode_Buf, PROTOBUF_DECODE_MESSAGE_BUF_LEN);

	message.stationId.funcs.decode = read_ints;
	message.stationId.arg = StationID;

	message.uuid.funcs.decode = read_ints;
	message.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];
	message.bean.funcs.decode = read_ints_beanbuf;
	message.bean.arg = beanBuf;

	/* Now we are ready to decode the message. */
	status = pb_decode(&stream, EventPushdown_fields, &message);

	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return;
	}

	//站点名称比较
	if(strcmp(StationID, gs_TelaidianCtrl.TelaidianStationID) != 0)
	{
		return;
	}

	//创建流
	stream = pb_istream_from_buffer(beanBuf, sizeof(beanBuf));

	//StartChargeOrderPush
	if(message.key == 1)
	{
		StartChargeOrderPush message1 = StartChargeOrderPush_init_zero;

		memset(billId, 0, sizeof(billId));
		message1.billId.funcs.decode = read_ints;
		message1.billId.arg = billId;

		
		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, StartChargeOrderPush_fields, &message1);

		if (!status)
		{			
			return;
		}

		u8A_ChargeStat = u8Get_A_SpearRectStat();
		u8B_ChargeStat = u8Get_B_SpearRectStat();

		
		if(message1.canId == A_GUN_CANID)
		{					
			if(strlen(billId) == 0)
			{
				//空订单，不做处理
			}
			else 
			{
				gs_TelaidianCtrl.StartCmd[0] = message1.chargeMode;  //开始充电1 经济充电2(预留) 开始放电3(预留)

				gs_TelaidianCtrl.StopRequest[0] = 0;

				gs_TelaidianCtrl.EndReason[0] = 0;				
				
				if(gs_TelaidianCtrl.SysAlmStat != 0)
				{
					gs_TelaidianCtrl.EndReason[0] = TelaidianFaltCodeTransToStopReason(gs_TelaidianCtrl.SysAlmStat);
					gs_TelaidianCtrl.Result[0] = 1;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
					gEvChargeInfo.u8IsOptionA = FALSE;
					gs_TelaidianCtrl.AccountRequest[0] = 0;
					memset(gs_TelaidianCtrl.TelaidianBillId[0], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[0]));
					strncpy(gs_TelaidianCtrl.TelaidianBillId[0], billId, strlen(billId));
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x01;
				}
				else if(gs_TelaidianCtrl.A_AlmStat != 0)
				{
					gs_TelaidianCtrl.EndReason[0] = TelaidianFaltCodeTransToStopReason(gs_TelaidianCtrl.A_AlmStat);
					gs_TelaidianCtrl.Result[0] = 1;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
					gEvChargeInfo.u8IsOptionA = FALSE;
					gs_TelaidianCtrl.AccountRequest[0] = 0;
					memset(gs_TelaidianCtrl.TelaidianBillId[0], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[0]));
					strncpy(gs_TelaidianCtrl.TelaidianBillId[0], billId, strlen(billId));
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x01;
				}
				//20171208 增加轮充模式和电流模式下的支持
				else if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL) 
					&& (((gEvChargeInfo.ParallelMode == PARALLEL_CURR_MODE) && (u8A_ChargeStat != RECT_OFF_SPEAR_IDL))  //电流模式，枪必须是空闲才能启动
						|| ((gEvChargeInfo.ParallelMode == PARALLEL_ROTATION_MODE) 
							&& ((u8A_ChargeStat != RECT_OFF_SPEAR_IDL) || (u8B_ChargeStat != RECT_OFF_SPEAR_IDL))))) //轮充模式，AB枪必须是空闲才能启动
				{
					gs_TelaidianCtrl.EndReason[0] = 20;  //系统模式类型转换中？
					gs_TelaidianCtrl.Result[0] = 1;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
					gEvChargeInfo.u8IsOptionA = FALSE;
					gs_TelaidianCtrl.AccountRequest[0] = 0;
					memset(gs_TelaidianCtrl.TelaidianBillId[0], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[0]));
					strncpy(gs_TelaidianCtrl.TelaidianBillId[0], billId, strlen(billId));
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x01;
				}
				else
				{
					gs_TelaidianCtrl.Result[0] = 255;
					PEInfo = GetgEquipInfo(E_TYPE_SYS);
					if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL)
					{
						memset(gs_TelaidianCtrl.TelaidianBillId[0], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[0]));
						strncpy(gs_TelaidianCtrl.TelaidianBillId[0], billId, strlen(billId));
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x01;

						if(gs_TelaidianCtrl.AccountRequest[0] >= TelaidianAccountRequestFlag_2)
						//	|| gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_7)
						{
							gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
							gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_CHARGE_21;
							gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[0] = 0;
						}
						else
						{
							gEvChargeInfo.u8_A_LogInMother = EV_ENTER_APP;
							gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_CHARGE_APP_22;
							gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[0] = 1;
						}
						A_SetChargeSTART_Cmd(gEvChargeInfo.u8_A_LogInMother);
						gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
						gEvChargeInfo.u8IsOptionA = FALSE;
						gs_TelaidianCtrl.AccountRequest[0] = 0;						

						for (i = 0; i < 16; i++)
						{
						   gEvChargeInfo.u8_CustomerNo[GUN_NO_A][i] = TwoAscii2Hex(gs_TelaidianCtrl.TelaidianBillId[0][i*2], gs_TelaidianCtrl.TelaidianBillId[0][i*2+1]);
						}

						//禁止弹上次结算页面
						if(Pop_A_StopSem())
						{
							gs_TelaidianCtrl.StopRequest[0] = 0;
						}
					}
					else if(strcmp(gs_TelaidianCtrl.TelaidianBillId[0], billId) == 0)  //订单一致，则再次返回结果
					{
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x01;
					}
					else  //订单号不一致，停止本次充电
					{
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x01;
						gs_TelaidianCtrl.StopCmd[0] = 1;  //停止充电1
						gs_TelaidianCtrl.StopRequest[0] = 0;
						A_SetChargeSTOP_Cmd(RECEIVE_STOP_CMD_BY_NET);
						gEvChargeInfo.u8IsOptionA = FALSE;	
					}
				}
			}
			
		}
		else if(message1.canId == B_GUN_CANID)
		{		
			if(strlen(billId) == 0)
			{
				//空订单，不做处理
			}
			else 
			{
				gs_TelaidianCtrl.StartCmd[1] = message1.chargeMode;  //开始充电1 经济充电2(预留) 开始放电3(预留)

				gs_TelaidianCtrl.StopRequest[1] = 0;

				gs_TelaidianCtrl.EndReason[1] = 0;


				if(gs_TelaidianCtrl.SysAlmStat != 0)
				{
					gs_TelaidianCtrl.EndReason[1] = TelaidianFaltCodeTransToStopReason(gs_TelaidianCtrl.SysAlmStat);
					gs_TelaidianCtrl.Result[1] = 1;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
					gEvChargeInfo.u8IsOptionB = FALSE;
					gs_TelaidianCtrl.AccountRequest[1] = 0;
					memset(gs_TelaidianCtrl.TelaidianBillId[1], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[1]));
					strncpy(gs_TelaidianCtrl.TelaidianBillId[1], billId, strlen(billId));
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x02;
				}
				else if(gs_TelaidianCtrl.B_AlmStat != 0)
				{
					gs_TelaidianCtrl.EndReason[1] = TelaidianFaltCodeTransToStopReason(gs_TelaidianCtrl.B_AlmStat);
					gs_TelaidianCtrl.Result[1] = 1;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
					gEvChargeInfo.u8IsOptionB = FALSE;
					gs_TelaidianCtrl.AccountRequest[1] = 0;
					memset(gs_TelaidianCtrl.TelaidianBillId[1], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[1]));
					strncpy(gs_TelaidianCtrl.TelaidianBillId[1], billId, strlen(billId));
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x02;
				}
				//20171208 增加轮充模式和电流模式下的支持
				else if ((gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL) 
					&& (((gEvChargeInfo.ParallelMode == PARALLEL_CURR_MODE) && (u8B_ChargeStat != RECT_OFF_SPEAR_IDL))  //电流模式，枪必须是空闲才能启动
					|| ((gEvChargeInfo.ParallelMode == PARALLEL_ROTATION_MODE) 
					&& ((u8A_ChargeStat != RECT_OFF_SPEAR_IDL) || (u8B_ChargeStat != RECT_OFF_SPEAR_IDL))))) //轮充模式，AB枪必须是空闲才能启动
				{
					gs_TelaidianCtrl.EndReason[1] =  20;  //系统模式类型转换中？
					gs_TelaidianCtrl.Result[1] = 1;
					gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
					gEvChargeInfo.u8IsOptionB = FALSE;
					gs_TelaidianCtrl.AccountRequest[1] = 0;
					memset(gs_TelaidianCtrl.TelaidianBillId[1], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[1]));
					strncpy(gs_TelaidianCtrl.TelaidianBillId[1], billId, strlen(billId));
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
					gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x02;
				}
				else
				{
					gs_TelaidianCtrl.Result[1] = 255;
					PEInfo = GetgEquipInfo(E_TYPE_SYS);
					if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
					{
						memset(gs_TelaidianCtrl.TelaidianBillId[1], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[1]));
						strncpy(gs_TelaidianCtrl.TelaidianBillId[1], billId, strlen(billId));
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x02;

						if(gs_TelaidianCtrl.AccountRequest[1] >= TelaidianAccountRequestFlag_2)
						//	|| gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_7)
						{
							gEvChargeInfo.u8_B_LogInMother = EV_ENTER_IC_CARD;
							gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_CHARGE_36;
							gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[1] = 0;
						}
						else
						{
							gEvChargeInfo.u8_B_LogInMother = EV_ENTER_APP;
							gHMIData.unPageAddr.u16_B_PageAddr = PAGE_B_CHARGE_APP_37;
							gs_TelaidianCtrl.AfterRcvStartRptTelesignal90SecCnt[1] = 1;
						}
						B_SetChargeSTART_Cmd(gEvChargeInfo.u8_B_LogInMother);					
						gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
						gEvChargeInfo.u8IsOptionB = FALSE;
						gs_TelaidianCtrl.AccountRequest[1] = 0;
						
						for (i = 0; i < 9; i++)
						{
						   gEvChargeInfo.u8_CustomerNo[GUN_NO_B][i] = TwoAscii2Hex(gs_TelaidianCtrl.TelaidianBillId[1][i*2], gs_TelaidianCtrl.TelaidianBillId[1][i*2+1]);
						}
						//禁止弹上次结算页面
						if(Pop_B_StopSem())
						{
							gs_TelaidianCtrl.StopRequest[1] = 0;
						}
					}
					else if(strcmp(gs_TelaidianCtrl.TelaidianBillId[1], billId) == 0)  //订单一致，则再次返回结果
					{
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x02;
					}
					else  //订单号不一致，停止本次充电
					{
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
						gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x02;
						gs_TelaidianCtrl.StopCmd[1] = 1;  //停止充电1
						gs_TelaidianCtrl.StopRequest[1] = 0;
						B_SetChargeSTOP_Cmd(RECEIVE_STOP_CMD_BY_NET);
						gEvChargeInfo.u8IsOptionB = FALSE;					
					}
				}
			}
		}
	}
	//StopChargeOrderPush
	else if(message.key == 2)
	{
		StopChargeOrderPush message1 = StopChargeOrderPush_init_zero;

		message1.billId.funcs.decode = read_ints;
		message1.billId.arg = billId;


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, StopChargeOrderPush_fields, &message1);

		if (!status)
		{			
			return;
		}

		if(message1.canId == A_GUN_CANID)
		{
			uctemp = strlen(billId);
			uctemp2 = strlen(gs_TelaidianCtrl.TelaidianBillId[0]);
			
			if(uctemp == 0)
			{
				//空订单，不做处理
			}
			//因会出现订单号未全0的情况，特来电陈帅的意见是也停止充电
			else if((strcmp(gs_TelaidianCtrl.TelaidianBillId[0], billId) == 0)  //订单一致
				|| ((uctemp == uctemp2) && (BillOfStopCmdIsAllZero(billId, uctemp) == TRUE))
				)
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x01;

				gs_TelaidianCtrl.StopCmd[0] = message1.chargeMode;  //停止充电1

				gs_TelaidianCtrl.StopRequest[0] = 0;

				//gs_TelaidianCtrl.EndReason[0] = 2;
				//gs_TelaidianCtrl.Result[0] = 255;	//成功
				
				PEInfo = GetgEquipInfo(E_TYPE_SYS);
				if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
				{
					if(gs_TelaidianCtrl.StopRequest[0] >= TelaidianStopRequestFlag_1)				
					{
						uctemp = BRUSH_STOP_CMD;  //认为是刷卡停机
					}
					else
					{
						uctemp = RECEIVE_STOP_CMD_BY_NET;
					}
					A_SetChargeSTOP_Cmd(uctemp);
					gEvChargeInfo.u8IsOptionA = FALSE;					
				}

				//20171116lkf 收到停止充电命令，马上同时上报电量数据和5秒后发遥信信号
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x01;  //A枪 
				if(gs_TelaidianCtrl.A_AfterMeterUpToSendTelesignalDelay == 0)
					gs_TelaidianCtrl.A_AfterMeterUpToSendTelesignalDelay = 1;
			}
		}
		else if(message1.canId == B_GUN_CANID)
		{
			uctemp = strlen(billId);
			uctemp2 = strlen(gs_TelaidianCtrl.TelaidianBillId[1]);

			if(uctemp == 0)
			{
				//空订单，不做处理
			}
			//因会出现订单号未全0的情况，特来电陈帅的意见是也停止充电
			else if((strcmp(gs_TelaidianCtrl.TelaidianBillId[1], billId) == 0)  //订单一致
				|| ((uctemp == uctemp2) && (BillOfStopCmdIsAllZero(billId, uctemp) == TRUE))
				)
			{
				//订单号如何处理?
				/*memset(gs_TelaidianCtrl.TelaidianBillId[1], 0, sizeof(gs_TelaidianCtrl.TelaidianBillId[1]));
				strncpy(gs_TelaidianCtrl.TelaidianBillId[1], billId, strlen(billId));*/
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo |= 0x02;
				gs_TelaidianCtrl.StopCmd[1] = message1.chargeMode;  //停止充电1

				gs_TelaidianCtrl.StopRequest[1] = 0;

				//gs_TelaidianCtrl.EndReason[1] = 2;
				//gs_TelaidianCtrl.Result[1] = 255;	//成功

				PEInfo = GetgEquipInfo(E_TYPE_SYS);
				if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
				{
					if(gs_TelaidianCtrl.StopRequest[1] >= TelaidianStopRequestFlag_1)				
					{
						uctemp = BRUSH_STOP_CMD;  //认为是刷卡停机
					}
					else
					{
						uctemp = RECEIVE_STOP_CMD_BY_NET;
					}
					B_SetChargeSTOP_Cmd(uctemp);
					gEvChargeInfo.u8IsOptionB = FALSE;					
				}

				//20171116lkf 收到停止充电命令，马上同时上报电量数据和5秒后发遥信信号
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x02;  //B枪
				if(gs_TelaidianCtrl.B_AfterMeterUpToSendTelesignalDelay == 0)
					gs_TelaidianCtrl.B_AfterMeterUpToSendTelesignalDelay = 1;
			}
		}
	}
	//CallAllDataPush
	else if(message.key == 3)
	{
		CallAllDataPush message1 = CallAllDataPush_init_zero;
		
		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, CallAllDataPush_fields, &message1);

		if (!status)
		{			
			return;
		}
		
		//需要上传遥信1, 不需要2
		if(message1.telesData == 1) 
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo = 0x03;  //AB枪都要 
		}

		//需要上传电量1, 不需要2
		if(message1.telemData == 1) 
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].NeedSendGunNo = 0x03;  //AB枪都要 
		}

		//需要上传遥测1, 不需要2
		if(message1.meterData == 1) 
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo = 0x03;  //AB枪都要 
		}

	}
	//CallPileDataPush
	else if(message.key == 4)
	{
		CallPileDataPush message1 = CallPileDataPush_init_zero;

		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, CallPileDataPush_fields, &message1);

		if (!status)
		{			
			return;
		}

		if(message1.canId == A_GUN_CANID)
		{
			//需要上传遥信1, 不需要2
			if(message1.telesData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 0x01;  
			}

			//需要上传电量1, 不需要2
			if(message1.telemData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].NeedSendGunNo = 0x01;  
			}

			//需要上传遥测1, 不需要2
			if(message1.meterData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x01;  
			}

			//需要上传bms 1, 不需要2
			if(message1.bmsData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].NeedSendGunNo |= 0x01;  
			}
		}
		else if(message1.canId == B_GUN_CANID)
		{		
			//需要上传遥信1, 不需要2
			if(message1.telesData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo |= 0x02;  
			}

			//需要上传电量1, 不需要2
			if(message1.telemData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].NeedSendGunNo |= 0x02;  
			}

			//需要上传遥测1, 不需要2
			if(message1.meterData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x02;  
			}

			//需要上传bms 1, 不需要2
			if(message1.bmsData == 1) 
			{
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].NeedSendGunNo |= 0x02;  
			}
		}
	}
	//DataAckPush
	else if(message.key == 5)
	{
		DataAckPush message1 = DataAckPush_init_zero;

		message1.accountId.funcs.decode = read_ints;
		message1.accountId.arg = billId;


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, DataAckPush_fields, &message1);

		if (!status)
		{			
			return;
		}

		if(message1.canId == A_GUN_CANID)
		{		
			if(message1.result == 255)
			{
				gs_TelaidianCtrl.A_WaitHostJiesuanCmdDelay = 0;  //收到确认
	
				////上报电量
				//gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				//gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x01;  //A枪 
				//if(gs_TelaidianCtrl.A_AfterMeterUpToSendTelesignalDelay == 0)
				//	gs_TelaidianCtrl.A_AfterMeterUpToSendTelesignalDelay = 1;
			}
		}
		else if(message1.canId == B_GUN_CANID)
		{		
			if(message1.result == 255)
			{
				gs_TelaidianCtrl.B_WaitHostJiesuanCmdDelay = 0;	//收到确认

				//上报电量
				//gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].IsSend = 1;
				//gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo |= 0x02;  //B枪
				//if(gs_TelaidianCtrl.B_AfterMeterUpToSendTelesignalDelay == 0)
				//	gs_TelaidianCtrl.B_AfterMeterUpToSendTelesignalDelay = 1;
			}
		}


	}
	//ClockSynPush
	else if(message.key == 6)
	{
		ClockSynPush message1 = ClockSynPush_init_zero;

		message1.bcdTimeStamp.funcs.decode = read_ints;
		message1.bcdTimeStamp.arg = billId;


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, ClockSynPush_fields, &message1);

		if (!status)
		{			
			return;
		}
		
		pBuf = billId;
		//年
		for(i = 0; i < 4; i++)
		{
			u8Buf[i] = Ascii2Hex(*pBuf++);
		}
		tmNow.tm_year = u8Buf[0]*1000 + u8Buf[1]*100 + u8Buf[2]*10 + u8Buf[3];
		//月
		for(i = 0; i < 2; i++)
		{
			u8Buf[i] = Ascii2Hex(*pBuf++);
		}
		tmNow.tm_mon = u8Buf[0]*10 + u8Buf[1];
		//日
		for(i = 0; i < 2; i++)
		{
			u8Buf[i] = Ascii2Hex(*pBuf++);
		}
		tmNow.tm_mday = u8Buf[0]*10 + u8Buf[1];
		//时
		for(i = 0; i < 2; i++)
		{
			u8Buf[i] = Ascii2Hex(*pBuf++);
		}
		tmNow.tm_hour = u8Buf[0]*10 + u8Buf[1];
		//分
		for(i = 0; i < 2; i++)
		{
			u8Buf[i] = Ascii2Hex(*pBuf++);
		}
		tmNow.tm_min = u8Buf[0]*10 + u8Buf[1];
		//秒
		for(i = 0; i < 2; i++)
		{
			u8Buf[i] = Ascii2Hex(*pBuf++);
		}
		tmNow.tm_sec = u8Buf[0]*10 + u8Buf[1];

		gRTCData.fGetLocalTime(&tmNow_2);
		
		if((tmNow_2.tm_year != tmNow.tm_year)
			|| (tmNow_2.tm_mon != tmNow.tm_mon)
			|| (tmNow_2.tm_mday != tmNow.tm_mday)
			|| (tmNow_2.tm_hour != tmNow.tm_hour)
			|| (tmNow_2.tm_min != tmNow.tm_min)
			|| (tmNow_2.tm_sec != tmNow.tm_sec)
			)
		{
			gRTCData.fSetLocalTime(&tmNow);
		}
		
		
	}

}
void SubTopic02Func_pushdown_state(MessageData * pMessageData)
{
	bool status;
	unsigned char StationID[40];

	memset(g_Protobuf_Decode_Buf, 0, PROTOBUF_DECODE_MESSAGE_BUF_LEN);
	memcpy(g_Protobuf_Decode_Buf, pMessageData->message->payload, pMessageData->message->payloadlen);

	memset(StationID, 0, sizeof(StationID));
	memset(g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC02_pushdown_state], 0, sizeof(g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC02_pushdown_state]));
	memset(beanBuf, 0, sizeof(beanBuf));


	EventStatePushdown message = EventStatePushdown_init_zero;

	/* Create a stream that reads from the buffer. */
	pb_istream_t stream = pb_istream_from_buffer(g_Protobuf_Decode_Buf, PROTOBUF_DECODE_MESSAGE_BUF_LEN);

	message.uuid.funcs.decode = read_ints;
	message.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC02_pushdown_state];
	message.bean.funcs.decode = read_ints_beanbuf;
	message.bean.arg = beanBuf;

	/* Now we are ready to decode the message. */
	status = pb_decode(&stream, EventStatePushdown_fields, &message);

	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return;
	}

	//创建流
	stream = pb_istream_from_buffer(beanBuf, sizeof(beanBuf));

	//CallAllStatePush
	if(message.key == 1)
	{

		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventStateUpload].IsSend = 1;

		CallAllStatePush message1 = CallAllStatePush_init_zero;

		message1.resFieldOne.funcs.decode = read_ints;
		message1.resFieldOne.arg = billId;


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, CallAllStatePush_fields, &message1);

		if (!status)
		{			
			return;
		}

	}
	//CallControllerStatePush
	else if(message.key == 2)
	{
		CallControllerStatePush message1 = CallControllerStatePush_init_zero;

		message1.stationId.funcs.decode = read_ints;
		message1.stationId.arg = StationID;

		/* 预留，不解析
		message1.resFieldOne.funcs.decode = read_ints;
		message1.resFieldOne.arg = billId;

		message1.resFieldTwo.funcs.decode = read_ints;
		message1.resFieldTwo.arg = billId;*/


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, CallControllerStatePush_fields, &message1);

		if (!status)
		{			
			return;
		}

		//站点名称比较
		if(strcmp(StationID, gs_TelaidianCtrl.TelaidianStationID) == 0)
		{
			gs_PublicMessage_Ctrl[PUB_MESSAGE_EventStateUpload].IsSend = 1;
		}
	}
	
}
unsigned char CardAccount[40];
unsigned int GetPowerPer10(unsigned char numer)
{
	unsigned char i;
	unsigned int PowerValue;
	
	PowerValue = 1;
	for(i = 0; i < numer; i++)
	{
		PowerValue *= 10;
	}
	return PowerValue;
}
void SubTopic03Func_response_data(MessageData * pMessageData)
{
	bool status;
	unsigned char StationID[40];
	unsigned char BalanceNumber[10];
	char *pChar;
	unsigned char i, uctemp,BalanceLenth, ucFlag, j;
	INT32 i32Balance;

	memset(g_Protobuf_Decode_Buf, 0, PROTOBUF_DECODE_MESSAGE_BUF_LEN);
	memcpy(g_Protobuf_Decode_Buf, pMessageData->message->payload, pMessageData->message->payloadlen);

	memset(CardAccount, 0, sizeof(CardAccount));
	memset(StationID, 0, sizeof(StationID));
	memset(g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC03_response_data], 0, sizeof(g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC03_response_data]));
	memset(beanBuf, 0, sizeof(beanBuf));


	EventResponse message = EventResponse_init_zero;

	/* Create a stream that reads from the buffer. */
	pb_istream_t stream = pb_istream_from_buffer(g_Protobuf_Decode_Buf, PROTOBUF_DECODE_MESSAGE_BUF_LEN);

	message.stationId.funcs.decode = read_ints;
	message.stationId.arg = StationID;

	message.uuid.funcs.decode = read_ints;
	message.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC03_response_data];
	message.bean.funcs.decode = read_ints_beanbuf;
	message.bean.arg = beanBuf;

	/* Now we are ready to decode the message. */
	status = pb_decode(&stream, EventResponse_fields, &message);

	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return;
	}

	//站点名称比较
	if(strcmp(StationID, gs_TelaidianCtrl.TelaidianStationID) != 0)
	{
		return;
	}

	//创建流
	stream = pb_istream_from_buffer(beanBuf, sizeof(beanBuf));

	//AccountResponse
	if(message.key == 1)   //账户信息应答
	{
		AccountResponse message1 = AccountResponse_init_zero;

		message1.accountId.funcs.decode = read_ints;
		message1.accountId.arg = CardAccount;

		message1.itemDetail.funcs.decode = read_ints;
		message1.itemDetail.arg = billId;
		


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, AccountResponse_fields, &message1);

		if (!status)
		{			
			return;
		}

		BalanceLenth = strlen(billId);
		pChar = strstr(billId, "balance");
		if((BalanceLenth > 0) && (pChar != NULL))
		{	
			memset(BalanceNumber, 0, sizeof(BalanceNumber));
			pChar += 9;
			BalanceLenth = BalanceLenth - (pChar-billId);
			for(i = 0, j = 0, ucFlag = 0; i < BalanceLenth; i++)
			{
				if(( *pChar >= '0') && (*pChar <= '9')) 
				{
					ucFlag = 1;
					BalanceNumber[j++] = *pChar;
				}
				else if(ucFlag)
				{
					break;
				}
				pChar++;
			}
			//找到余额
			if(ucFlag)
			{				
				i32Balance = 0;
				for(i = 0, i32Balance = 0; i < j; i++)
				{
					i32Balance += ((BalanceNumber[i]-'0') * GetPowerPer10(j-i-1));
				}

				if(message1.canId == A_GUN_CANID)
				{
					gs_TelaidianCtrl.TelaidianAccountBalace[0] = i32Balance;
					if(gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_1)
						gs_TelaidianCtrl.AccountRequest[0] = TelaidianAccountRequestFlag_2;
				}
				else if(message1.canId == B_GUN_CANID)
				{
					gs_TelaidianCtrl.TelaidianAccountBalace[1] = i32Balance;
					if(gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_1)
						gs_TelaidianCtrl.AccountRequest[1] = TelaidianAccountRequestFlag_2;
				}
			}
		}

		
		

	}
	//ChargeAckResponse
	else if(message.key == 2)  ////刷卡申请开始充电/刷卡申请结束充电/VIN申请开始/车牌号申请开始响应结果
	{
		ChargeAckResponse message1 = ChargeAckResponse_init_zero;

		message1.accountId.funcs.decode = read_ints;
		message1.accountId.arg = billId;


		/* Now we are ready to decode the message. */
		status = pb_decode(&stream, ChargeAckResponse_fields, &message1);

		if (!status)
		{			
			return;
		}

		if(message1.canId == A_GUN_CANID)
		{		
			if(message1.result == 255)
			{
				/*gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].IsSend = 1;
				gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo |= 0x01;
				gs_TelaidianCtrl.StartCmd[0] = 1;  
				gs_TelaidianCtrl.EndReason[0] = 0;
				if(gs_TelaidianCtrl.SysAlmStat != 0)
				{
					gs_TelaidianCtrl.EndReason[0] = TelaidianFaltCodeTransToStopReason(gs_TelaidianCtrl.SysAlmStat);
					gs_TelaidianCtrl.Result[0] = 1;
				}
				else if(gs_TelaidianCtrl.A_AlmStat != 0)
				{
					gs_TelaidianCtrl.EndReason[0] = TelaidianFaltCodeTransToStopReason(gs_TelaidianCtrl.A_AlmStat);
					gs_TelaidianCtrl.Result[0] = 1;
				}
				else
				{
					gs_TelaidianCtrl.Result[0] = 255;
					PEInfo = GetgEquipInfo(E_TYPE_SYS);
					if (((EV_CHARGE_INFO*)PEInfo)->u8_Charge_CMD[GUN_NO_A] == STOP_CHARGE_VAL)
					{
						gEvChargeInfo.u8_A_LogInMother = EV_ENTER_IC_CARD;
						A_SetChargeSTART_Cmd(EV_ENTER_IC_CARD);
						gHMIData.unPageAddr.u16BasePageAddr = PAGE_A_B_SPLIT_NO_12;
						gEvChargeInfo.u8IsOptionA = FALSE;
						gHMIData.unPageAddr.u16_A_PageAddr = PAGE_A_CHARGE_21;
					}
				}*/
			}
			else if(gs_TelaidianCtrl.AccountRequest[0] == TelaidianAccountRequestFlag_1)
			{
				gs_TelaidianCtrl.AccountRequest[0] = TelaidianAccountRequestFlag_5;
			}
			else if(gs_TelaidianCtrl.StopRequest[0] == TelaidianStopRequestFlag_1)
			{
				gs_TelaidianCtrl.StopRequest[0] = TelaidianStopRequestFlag_3;
			}
		}
		else if(message1.canId == B_GUN_CANID)
		{		
			if(message1.result == 255)
			{
			}
			else if(gs_TelaidianCtrl.AccountRequest[1] == TelaidianAccountRequestFlag_1)
			{
				gs_TelaidianCtrl.AccountRequest[1] = TelaidianAccountRequestFlag_5;
			}
			else if(gs_TelaidianCtrl.StopRequest[1] == TelaidianStopRequestFlag_1)
			{
				gs_TelaidianCtrl.StopRequest[1] = TelaidianStopRequestFlag_3;
			}
		}
		
	}
}

////////////////////////////////////////////////////////////////////////////////////////
///PUBLIC相关函数
////////////////////////////////////////////////////////////////////////////////////////
unsigned char KeyXbuf[300];
Telaidian_bytes_array_Def pBytesArray;

unsigned char ProtobufEncodeMessage_EventStateUpload(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;
	/*unsigned char i, StationID[32];
	__XXX_UNION_VALUE unVal;*/

	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	StateDataUp message = StateDataUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	/* Fill in the message */
	message.type = 1;	//在线状态
	message.timestamp = GetLocalTimeStamp();

	//站点名字
	/*memset(StationID, 0, sizeof(StationID));
	for (i = 0; i < 32; i++)
	{
		ExGetSigInfo(E_TYPE_SYS, ID_u8StationCode_No1 + i, &unVal, NULL, BY_SRAM);
		if(unVal.chtemp[0] == 0)
		{
			break;
		}
		StationID[i] = unVal.chtemp[0];
	}*/
	message.authen.funcs.encode = Write_String;
	message.authen.arg = gs_TelaidianCtrl.TelaidianStationName;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, StateDataUp_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventStateUpload message1 = EventStateUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 1;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC02_pushdown_state];

	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventStateUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

unsigned char ProtobufEncodeMessage_EventStateUpload_lwt(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;
	/*unsigned char i, StationID[32];
	__XXX_UNION_VALUE unVal;*/

	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	StateDataUp message = StateDataUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	/* Fill in the message */
	message.type = 2;	//离线状态
	message.timestamp = GetLocalTimeStamp();

	//站点名字
	message.authen.funcs.encode = Write_String;
	message.authen.arg = gs_TelaidianCtrl.TelaidianStationName;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, StateDataUp_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventStateUpload message1 = EventStateUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 1;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	/*message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC02_pushdown_state];*/

	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventStateUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}


unsigned char ProtobufEncodeMessage_EventUpload_StartChargeResultUp(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;
	
	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	StartChargeResultUp message = StartChargeResultUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].JustSentGunNo = 0;

	/* Fill in the message */
	if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo & 0x01)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].JustSentGunNo = 1;
		message.canId = A_GUN_CANID;
		message.billId.funcs.encode = Write_String;
		message.billId.arg = gs_TelaidianCtrl.TelaidianBillId[0];

		message.chargeMode = gs_TelaidianCtrl.StartCmd[0];  //开始充电1

		message.result = gs_TelaidianCtrl.Result[0];   //结果 1失败 255成功

		if(gs_TelaidianCtrl.Result[0] == 255)
		{
			message.endReason = -1;
			gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[0] = 1;
			gs_TelaidianCtrl.PileState[0] = 1;
		}
		else
		{
			message.endReason = gs_TelaidianCtrl.EndReason[0];  //失败原因 充电成功填-1
			gs_TelaidianCtrl.PileState[0] = 0;
		}
		message.pileState = gs_TelaidianCtrl.PileState[0];  //未知0, 充电中1 
	}
	else if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].NeedSendGunNo & 0x02)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StartChargeResultUp].JustSentGunNo = 2;
		message.canId = B_GUN_CANID;
		message.billId.funcs.encode = Write_String;
		message.billId.arg = gs_TelaidianCtrl.TelaidianBillId[1];

		message.chargeMode = gs_TelaidianCtrl.StartCmd[1];  //开始充电1

		message.result = gs_TelaidianCtrl.Result[1];   //结果 1失败 255成功

		if(gs_TelaidianCtrl.Result[1] == 255)
		{
			message.endReason = -1;
			gs_TelaidianCtrl.AfterAppStartChgSendTelesignalFlag[1] = 1;
			gs_TelaidianCtrl.PileState[1] = 1;
		}
		else
		{
			message.endReason = gs_TelaidianCtrl.EndReason[1];  //失败原因 充电成功填-1
			gs_TelaidianCtrl.PileState[1] = 0;
		}
		message.pileState = gs_TelaidianCtrl.PileState[1];  //未知0, 充电中1 
	}
	else
	{
		return 0;
	}


	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, StartChargeResultUp_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventUpload message1 = EventUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 1;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];

	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

TelaidianStopReportFreezeMeter_Def gs_TelaidianStopReportFreezeMeter_A = {0};
TelaidianStopReportFreezeMeter_Def gs_TelaidianStopReportFreezeMeter_B = {0};

unsigned char FreezeMeterBuf[2710];
unsigned short u16FreezeMeterBufLenth;

void PrepareFreezeMeterData(unsigned char GunNo)
{
	//char cTimebuf[50];
	unsigned short u16Idx = 0;
	stWHInfo* PWH_1Info;
	unsigned char *pBuf = FreezeMeterBuf;
	struct tm *StartTime, *ENDTime;
	INT32 i32StarWhVal, i32EndWhVal;
	TelaidianStopReportFreezeMeter_Def *pFreezeMeter;
	unsigned char i;

	if(GunNo == 1)
	{
		//PWH_1Info = GetgEquipInfo(E_TYPE_WH2);
		StartTime = &gEvChargeInfo.StartTime[GUN_NO_B];
		ENDTime = &gEvChargeInfo.ENDTime[GUN_NO_B];
		i32StarWhVal = (gEvChargeInfo.fWH_StarVal[GUN_NO_B]*100);
		pFreezeMeter = &gs_TelaidianStopReportFreezeMeter_B;

		//i32EndWhVal = (gEvChargeInfo.fB_WH_EndVal*100);
	}
	else
	{
		//PWH_1Info = GetgEquipInfo(E_TYPE_WH1);
		StartTime = &gEvChargeInfo.StartTime[GUN_NO_A];
		ENDTime = &gEvChargeInfo.ENDTime[GUN_NO_A];
		i32StarWhVal = (gEvChargeInfo.fWH_StarVal[GUN_NO_A]*100);
		pFreezeMeter = &gs_TelaidianStopReportFreezeMeter_A;

		//i32EndWhVal = (gEvChargeInfo.fA_WH_EndVal*100);
	}
	//i32EndWhVal = (PWH_1Info->fWHRealValue*100);

	memset(FreezeMeterBuf, 0, sizeof(FreezeMeterBuf));
	//memset(cTimebuf, 0, sizeof(cTimebuf));
	u16FreezeMeterBufLenth = 0;

	/*u16Idx += sprintf(pBuf+u16Idx,"%s", "[\n" );

	u16Idx += sprintf(pBuf+u16Idx,"%s", "    {\n" );
	sprintf(cTimebuf,"\"Time\": \"%04d-%02d-%02d %02d:%02d:%02d.000\"", StartTime->tm_year, StartTime->tm_mon, StartTime->tm_mday, StartTime->tm_hour,  StartTime->tm_min,  StartTime->tm_sec);
	u16Idx += sprintf(pBuf+u16Idx,"        %s,\n", cTimebuf);
	u16Idx += sprintf(pBuf+u16Idx,"        \"Power\": %1.17g\n", fStarWhVal);
	u16Idx += sprintf(pBuf+u16Idx,"%s", "    },\n" );

	u16Idx += sprintf(pBuf+u16Idx,"%s", "    {\n" );
	sprintf(cTimebuf,"\"Time\": \"%04d-%02d-%02d %02d:%02d:%02d.000\"", ENDTime->tm_year, ENDTime->tm_mon, ENDTime->tm_mday, ENDTime->tm_hour, ENDTime->tm_min,  ENDTime->tm_sec);
	u16Idx += sprintf(pBuf+u16Idx,"        %s,\n", cTimebuf);
	u16Idx += sprintf(pBuf+u16Idx,"        \"Power\": %1.17g\n", PWH_1Info->fWHRealValue);
	u16Idx += sprintf(pBuf+u16Idx,"%s", "    }\n" );

	u16Idx += sprintf(pBuf+u16Idx,"%s", "]\n" );*/

	u16Idx += sprintf(pBuf+u16Idx,"%s", "[{" );
	u16Idx += sprintf(pBuf+u16Idx,"\"time\":\"%04d-%02d-%02d %02d:%02d:%02d.000\",", StartTime->tm_year, StartTime->tm_mon, StartTime->tm_mday, StartTime->tm_hour,  StartTime->tm_min,  StartTime->tm_sec);
	u16Idx += sprintf(pBuf+u16Idx,"\"power\":%d", i32StarWhVal);
	u16Idx += sprintf(pBuf+u16Idx,"%s", "}," );

	for(i = 0; i < pFreezeMeter->u8FreezeRecordNum;i++)
	{
		/*u16Idx += sprintf(pBuf+u16Idx,"%s", "{" );
		u16Idx += sprintf(pBuf+u16Idx,"\"time\":\"%04d-%02d-%02d %02d:%02d:%02d.000\",", pFreezeMeter->FreezeTime[i].tm_year, pFreezeMeter->FreezeTime[i].tm_mon, pFreezeMeter->FreezeTime[i].tm_mday, pFreezeMeter->FreezeTime[i].tm_hour, pFreezeMeter->FreezeTime[i].tm_min,  pFreezeMeter->FreezeTime[i].tm_sec);
		u16Idx += sprintf(pBuf+u16Idx,"\"power\":%d", pFreezeMeter->i32FreezeWhVal[i]);
		u16Idx += sprintf(pBuf+u16Idx,"%s", "}," );*/

		u16Idx += sprintf(pBuf+u16Idx,"%s", "{" );
		u16Idx += sprintf(pBuf+u16Idx,"\"time\":\"%04d-%02d-%02d %02d:%02d:%02d.000\",", StartTime->tm_year, StartTime->tm_mon, StartTime->tm_mday, StartTime->tm_hour,  StartTime->tm_min,  StartTime->tm_sec);
		u16Idx += sprintf(pBuf+u16Idx,"\"power\":%d", i32StarWhVal);
		u16Idx += sprintf(pBuf+u16Idx,"%s", "}," );
	}

	u16Idx += sprintf(pBuf+u16Idx,"%s", "{" );
	u16Idx += sprintf(pBuf+u16Idx,"\"time\":\"%04d-%02d-%02d %02d:%02d:%02d.000\",", ENDTime->tm_year, ENDTime->tm_mon, ENDTime->tm_mday, ENDTime->tm_hour,  ENDTime->tm_min,  ENDTime->tm_sec);
	u16Idx += sprintf(pBuf+u16Idx,"\"power\":%d", i32EndWhVal);
	u16Idx += sprintf(pBuf+u16Idx,"%s", "}]" );

	u16FreezeMeterBufLenth = u16Idx;

	//dv_Uart6.fSendDataTXY(FreezeMeterBuf, u16FreezeMeterBufLenth);

}
unsigned char StopReportFreezeMeterBuf[2880];
unsigned char ProtobufEncodeMessage_EventUpload_StopChargeResultUp(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;

	//Key message
	memset(StopReportFreezeMeterBuf, 0, sizeof(StopReportFreezeMeterBuf));

	StopChargeResultUp message = StopChargeResultUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(StopReportFreezeMeterBuf, sizeof(StopReportFreezeMeterBuf));

	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].JustSentGunNo = 0;

	/* Fill in the message */
	if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo & 0x01)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].JustSentGunNo = 1;

		/* Fill in the message */
		message.canId = A_GUN_CANID;
		message.billId.funcs.encode = Write_String;
		message.billId.arg = gs_TelaidianCtrl.TelaidianBillId[0];

		message.chargeMode = gs_TelaidianCtrl.StopCmd[0];
		message.result = gs_TelaidianCtrl.Result[0];   //结果 1失败 255成功

		if(gs_TelaidianCtrl.EndReason[0] == 0xFF)
		{
			message.endReason = -1;
		}
		else
		{
			message.endReason = gs_TelaidianCtrl.EndReason[0];  //失败原因 充电成功填-1
		}

		PrepareFreezeMeterData(0);

		message.freezeMeters.funcs.encode = Write_String;
		message.freezeMeters.arg = FreezeMeterBuf;
	}
	else if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].NeedSendGunNo & 0x02)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_StopChargeResultUp].JustSentGunNo = 2;

		/* Fill in the message */
		message.canId = B_GUN_CANID;
		message.billId.funcs.encode = Write_String;
		message.billId.arg = gs_TelaidianCtrl.TelaidianBillId[1];

		message.chargeMode = gs_TelaidianCtrl.StopCmd[1];
		message.result = gs_TelaidianCtrl.Result[1];   //结果 1失败 255成功

		if(gs_TelaidianCtrl.EndReason[1] == 0xFF)
		{
			message.endReason = -1;
		}
		else
		{
			message.endReason = gs_TelaidianCtrl.EndReason[1];  //失败原因 充电成功填-1
		}

		//gs_TelaidianStopReportFreezeMeter_B.u8FreezeRecordNum = 48;
		PrepareFreezeMeterData(1);

		message.freezeMeters.funcs.encode = Write_String;
		message.freezeMeters.arg = FreezeMeterBuf;
	}
	else
	{
		return 0;
	}

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, StopChargeResultUp_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventUpload message1 = EventUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 2;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];

	message1.timestamp = GetLocalTimeStamp();


	pBytesArray.bytes = (pb_byte_t *)&StopReportFreezeMeterBuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

//typedef struct pb_field_s pb_field_t;
//struct pb_field_s {
//    pb_size_t tag;
//    pb_type_t type;
//    pb_size_t data_offset; /* Offset of field data, relative to previous field. */
//    pb_ssize_t size_offset; /* Offset of array size or has-boolean, relative to data */
//    pb_size_t data_size; /* Data size in bytes for a single item */
//    pb_size_t array_size; /* Maximum number of entries in array */
//    
//    /* Field definitions for submessage
//     * OR default value for all other non-array, non-callback types
//     * If null, then field will zeroed. */
//    const void *ptr;
//} pb_packed;
//
//{1, PB_ATYPE_CALLBACK | PB_HTYPE_REPEATED | PB_LTYPE_MAP_MESSAGE, PB_DATAOFFSET_FIRST(TelesignallingDataUp, teleSigns, teleSigns), 0, 
//pb_membersize(TelesignallingDataUp, teleSigns), 0, &TelesignallingDataUp_Telesignalling_fields}
unsigned char KeyRepeatBuf[400];
unsigned char KeyXbuf_2[300];

Telaidian_bytes_array_repeat_Def pBytesArray_repeat;

unsigned char ProtobufEncodeMessage_EventUpload_TelesignallingDataUp(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length, message_length_2;
	bool status;
	unsigned char uctemp;
		

	//1.第一把枪的信息
	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	TelesignallingDataUp_Telesignalling message = TelesignallingDataUp_Telesignalling_init_zero;
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));
	/* Fill in the message */
	message.canIndex = A_GUN_CANID;

	if(gEvChargeInfo.u8EV_SpearPst[GUN_NO_A] == SPEAR_ON_LINE)
	{
		message.linkStatus = 1;
	}
	else
	{
		message.linkStatus = 0;
	}
	//message.linkStatus = 1;
	if(gEvChargeInfo.u8_Charge_CMD[GUN_NO_A] == START_CHARGE_VAL)
	{
		message.chargerStatus = 1;
	}
	else
	{
		message.chargerStatus = 0;
	}
	if (CHARGING_RCV_BCL_SEND_CCS == gBms_1_CommInfo.i8_CMD_Phase)
	{
		message.relayState = 1;
	}
	else
	{
		message.relayState = 0;
	}
	message.burstData = gs_TelaidianCtrl.A_burstDataFlag;
	gs_TelaidianCtrl.A_burstDataFlag = 0;

	if(gs_TelaidianCtrl.EndReason[0] == 0xFF)
	{
		message.stopReason = 0;
	}
	else
	{
		message.stopReason = gs_TelaidianCtrl.EndReason[0];
	}

	if(gs_TelaidianCtrl.SysAlmStat != 0)
		message.faultStatus = gs_TelaidianCtrl.SysAlmStat;
	else
		message.faultStatus = gs_TelaidianCtrl.A_AlmStat;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, TelesignallingDataUp_Telesignalling_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//2.第二把枪的信息
	//Key message
	memset(KeyXbuf_2, 0, sizeof(KeyXbuf_2));

	message = (TelesignallingDataUp_Telesignalling)TelesignallingDataUp_Telesignalling_init_zero;

	stream = pb_ostream_from_buffer(KeyXbuf_2, sizeof(KeyXbuf_2));
	/* Fill in the message */
	message.canIndex = B_GUN_CANID;
	if(gEvChargeInfo.u8EV_SpearPst[GUN_NO_B] == SPEAR_ON_LINE)
	{
		message.linkStatus = 1;
	}
	else
	{
		message.linkStatus = 0;
	}

	if(gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == START_CHARGE_VAL)
	{
		message.chargerStatus = 1;
	}
	else
	{
		message.chargerStatus = 0;
	}
	if (CHARGING_RCV_BCL_SEND_CCS == gBms_2_CommInfo.i8_CMD_Phase)
	{
		message.relayState = 1;
	}
	else
	{
		message.relayState = 0;
	}
	message.burstData = gs_TelaidianCtrl.B_burstDataFlag;
	gs_TelaidianCtrl.B_burstDataFlag = 0;

	if(gs_TelaidianCtrl.EndReason[1] == 0xFF)
	{
		message.stopReason = 0;
	}
	else
	{
		message.stopReason = gs_TelaidianCtrl.EndReason[1];
	}

	if(gs_TelaidianCtrl.SysAlmStat != 0)
		message.faultStatus = gs_TelaidianCtrl.SysAlmStat;
	else
		message.faultStatus = gs_TelaidianCtrl.B_AlmStat;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, TelesignallingDataUp_Telesignalling_fields, &message);
	message_length_2 = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//3.把repeat内容存入同一个缓冲中
	memset(KeyRepeatBuf, 0, sizeof(KeyRepeatBuf));

	TelesignallingDataUp messageRepeat = TelesignallingDataUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(KeyRepeatBuf, sizeof(KeyRepeatBuf));

	/* Fill in the message */
	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].JustSentGunNo = 0;

	/* Fill in the message */
	uctemp = gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].NeedSendGunNo & 0x03;
	if(uctemp == 0x03)
	{
		pBytesArray_repeat.bytes1 = (pb_byte_t *)&KeyXbuf;
		pBytesArray_repeat.size1 = message_length;
		pBytesArray_repeat.bytes2 = (pb_byte_t *)&KeyXbuf_2;
		pBytesArray_repeat.size2 = message_length_2;
		messageRepeat.teleSigns.funcs.encode = Write_bytes_repeat;
		messageRepeat.teleSigns.arg = &pBytesArray_repeat;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].JustSentGunNo = 3;
	}
	else if(uctemp == 0x01)
	{
		pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
		pBytesArray.size = message_length;
		messageRepeat.teleSigns.funcs.encode = Write_bytes;
		messageRepeat.teleSigns.arg = &pBytesArray;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].JustSentGunNo = 1;
	}
	else if(uctemp == 0x02)
	{
		pBytesArray.bytes = (pb_byte_t *)&KeyXbuf_2;
		pBytesArray.size = message_length_2;
		messageRepeat.teleSigns.funcs.encode = Write_bytes;
		messageRepeat.teleSigns.arg = &pBytesArray;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelesignallingDataUp].JustSentGunNo = 2;
	}


	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, TelesignallingDataUp_fields, &messageRepeat);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//4.主消息上报

	//main message
	EventUpload message1 = EventUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 3;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];

	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyRepeatBuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

unsigned char ProtobufEncodeMessage_EventUpload_TelemetryDataUp(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length, message_length_2;
	bool status;
	unsigned char uctemp;	;
	void *PEInfo;


	//1.第一把枪的信息
	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	TelemetryDataUp_Telemetry message = TelemetryDataUp_Telemetry_init_zero;
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));
	/* Fill in the message */
	message.canIndex = A_GUN_CANID;

	PEInfo = GetgEquipInfo(E_RECT_INFO);
	message.aVoltage = (((CANCommData_TypeDef*)PEInfo)->RectData[0].fAB_Volt * 10);
	message.bVoltage = (((CANCommData_TypeDef*)PEInfo)->RectData[0].fBC_Volt * 10);
	message.cVoltage = (((CANCommData_TypeDef*)PEInfo)->RectData[0].fCA_Volt * 10);
	message.directVoltage = (((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].fOutVolt * 10);
	message.directCurrent = (((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_A].fOutCurr * 10);	

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, TelemetryDataUp_Telemetry_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//2.第二把枪的信息
	//Key message
	memset(KeyXbuf_2, 0, sizeof(KeyXbuf_2));

	message = (TelemetryDataUp_Telemetry)TelemetryDataUp_Telemetry_init_zero;

	stream = pb_ostream_from_buffer(KeyXbuf_2, sizeof(KeyXbuf_2));
	/* Fill in the message */
	message.canIndex = B_GUN_CANID;
	message.aVoltage = (((CANCommData_TypeDef*)PEInfo)->RectData[1].fAB_Volt * 10);
	message.bVoltage = (((CANCommData_TypeDef*)PEInfo)->RectData[1].fBC_Volt * 10);
	message.cVoltage = (((CANCommData_TypeDef*)PEInfo)->RectData[1].fCA_Volt * 10);

	message.directVoltage = (((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].fOutVolt * 10);
	message.directCurrent = (((CANCommData_TypeDef*)PEInfo)->GrpData[GRP_B].fOutCurr * 10);	

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, TelemetryDataUp_Telemetry_fields, &message);
	message_length_2 = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//3.把repeat内容存入同一个缓冲中
	memset(KeyRepeatBuf, 0, sizeof(KeyRepeatBuf));

	TelemetryDataUp messageRepeat = TelemetryDataUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(KeyRepeatBuf, sizeof(KeyRepeatBuf));

	/* Fill in the message */
	uctemp = gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].NeedSendGunNo & 0x03;
	if(uctemp == 0x03)
	{
		pBytesArray_repeat.bytes1 = (pb_byte_t *)&KeyXbuf;
		pBytesArray_repeat.size1 = message_length;
		pBytesArray_repeat.bytes2 = (pb_byte_t *)&KeyXbuf_2;
		pBytesArray_repeat.size2 = message_length_2;
		messageRepeat.telemetry.funcs.encode = Write_bytes_repeat;
		messageRepeat.telemetry.arg = &pBytesArray_repeat;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].JustSentGunNo = 3;
	}
	else if(uctemp == 0x01)
	{
		pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
		pBytesArray.size = message_length;
		messageRepeat.telemetry.funcs.encode = Write_bytes;
		messageRepeat.telemetry.arg = &pBytesArray;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].JustSentGunNo = 1;
	}
	else if(uctemp == 0x02)
	{
		pBytesArray.bytes = (pb_byte_t *)&KeyXbuf_2;
		pBytesArray.size = message_length_2;
		messageRepeat.telemetry.funcs.encode = Write_bytes;
		messageRepeat.telemetry.arg = &pBytesArray;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_TelemetryDataUp].JustSentGunNo = 2;
	}


	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, TelemetryDataUp_fields, &messageRepeat);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//4.主消息上报

	//main message
	EventUpload message1 = EventUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 4;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];

	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyRepeatBuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

unsigned char ProtobufEncodeMessage_EventUpload_MeterDataUp(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length, message_length_2;
	bool status;
	unsigned char uctemp;

	//1.第一把枪的信息
	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	MeterDataUp_Meter message = MeterDataUp_Meter_init_zero;
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));
	/* Fill in the message */
	message.canIndex = A_GUN_CANID;

	message.positiveActiveEnergy = (gWH_Info[GUN_NO_A].fWHRealValue*100);

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, MeterDataUp_Meter_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//2.第二把枪的信息
	//Key message
	memset(KeyXbuf_2, 0, sizeof(KeyXbuf_2));

	message = (MeterDataUp_Meter)MeterDataUp_Meter_init_zero;

	stream = pb_ostream_from_buffer(KeyXbuf_2, sizeof(KeyXbuf_2));
	/* Fill in the message */
	message.canIndex = B_GUN_CANID;

	message.positiveActiveEnergy = (gWH_Info[GUN_NO_B].fWHRealValue*100);

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, MeterDataUp_Meter_fields, &message);
	message_length_2 = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//3.把repeat内容存入同一个缓冲中
	memset(KeyRepeatBuf, 0, sizeof(KeyRepeatBuf));

	MeterDataUp messageRepeat = MeterDataUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(KeyRepeatBuf, sizeof(KeyRepeatBuf));

	/* Fill in the message */
	uctemp = gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].NeedSendGunNo & 0x03;
	if(uctemp == 0x03)
	{
		pBytesArray_repeat.bytes1 = (pb_byte_t *)&KeyXbuf;
		pBytesArray_repeat.size1 = message_length;
		pBytesArray_repeat.bytes2 = (pb_byte_t *)&KeyXbuf_2;
		pBytesArray_repeat.size2 = message_length_2;
		messageRepeat.meter.funcs.encode = Write_bytes_repeat;
		messageRepeat.meter.arg = &pBytesArray_repeat;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].JustSentGunNo = 3;
	}
	else if(uctemp == 0x01)
	{
		pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
		pBytesArray.size = message_length;
		messageRepeat.meter.funcs.encode = Write_bytes;
		messageRepeat.meter.arg = &pBytesArray;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].JustSentGunNo = 1;
	}
	else if(uctemp == 0x02)
	{
		pBytesArray.bytes = (pb_byte_t *)&KeyXbuf_2;
		pBytesArray.size = message_length_2;
		messageRepeat.meter.funcs.encode = Write_bytes;
		messageRepeat.meter.arg = &pBytesArray;
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_MeterDataUp].JustSentGunNo = 2;
	}


	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, MeterDataUp_fields, &messageRepeat);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//4.主消息上报

	//main message
	EventUpload message1 = EventUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 5;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];

	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyRepeatBuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}


unsigned char ProtobufEncodeMessage_EventUpload_BMSDataUp(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;
	void *PEInfo;

	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	BMSDataUp message = BMSDataUp_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].JustSentGunNo = 0;

	/* Fill in the message */
	if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].NeedSendGunNo & 0x01)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].JustSentGunNo = 1;
		message.canIndex = A_GUN_CANID;

		PEInfo = GetgEquipInfo(E_TYPE_BMS1);
		message.soc = (((stBMS_INFO*)PEInfo)->fBCS_SOC);
		message.demandVoltage = (((stBMS_INFO*)PEInfo)->fBMSRequireVolt * 100);
		message.demandCurrent = (((stBMS_INFO*)PEInfo)->fBMSRequireCurr * 100);
		//message.highestTemp	= (((stBMS_INFO*)PEInfo)->fBSM_MaxBattTemp * 100);
		message.highestVoltage	= (((stBMS_INFO*)PEInfo)->fBCS_MaxCellVolt * 100);
		//message.lowestTemp	= (((stBMS_INFO*)PEInfo)->fBSM_MinBattTemp * 100);
		//message.lowestVoltage = (((stBMS_INFO*)PEInfo)->fBSM_MinBattTemp * 10);		
	}
	else if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].NeedSendGunNo & 0x02)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventUpload_BMSDataUp].JustSentGunNo = 2;
		message.canIndex = B_GUN_CANID;

		PEInfo = GetgEquipInfo(E_TYPE_BMS2);
		message.soc = (((stBMS_INFO*)PEInfo)->fBCS_SOC);
		message.demandVoltage = (((stBMS_INFO*)PEInfo)->fBMSRequireVolt * 100);
		message.demandCurrent = (((stBMS_INFO*)PEInfo)->fBMSRequireCurr * 100);
		//message.highestTemp	= (((stBMS_INFO*)PEInfo)->fBSM_MaxBattTemp * 100);
		message.highestVoltage	= (((stBMS_INFO*)PEInfo)->fBCS_MaxCellVolt * 100);
		//message.lowestTemp	= (((stBMS_INFO*)PEInfo)->fBSM_MinBattTemp * 100);
		//message.lowestVoltage = (((stBMS_INFO*)PEInfo)->fBSM_MinBattTemp * 10);	
	}
	else
	{
		return 0;
	}

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, BMSDataUp_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventUpload message1 = EventUpload_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 6;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];

	message1.timestamp = GetLocalTimeStamp();


	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventUpload_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}


unsigned char ProtobufEncodeMessage_EventRequest_StartChargeRequest(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;

	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	StartChargeRequest message = StartChargeRequest_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].JustSentGunNo = 0;

	/* Fill in the message */
	if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].NeedSendGunNo & 0x01)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].JustSentGunNo = 1;

		/* Fill in the message */
		message.startChargeMode = 1;  //启动充电方式.刷卡1,VIN2,车牌号3 

		/* Fill in the message */
		message.canId = A_GUN_CANID;

		//string accountId = 3;		//VIN, 车牌号, 卡号
		//int32 chargeType = 4; 		//充电类型 按电量1,时间2,金额3,充满为止4
		//int32 value = 5;			//按电量充(度), 时间(分钟),金额(分),充满-100
		message.chargeType = 4;
		message.value = 100;

		message.accountId.funcs.encode = Write_String;
		message.accountId.arg = gBrushInfo.u8TempCARDInfo;
		
	}
	else if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].NeedSendGunNo & 0x02)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StartChargeRequest].JustSentGunNo = 2;

		/* Fill in the message */
		message.startChargeMode = 1;  //启动充电方式.刷卡1,VIN2,车牌号3 

		/* Fill in the message */
		message.canId = B_GUN_CANID;

		//string accountId = 3;		//VIN, 车牌号, 卡号
		//int32 chargeType = 4; 		//充电类型 按电量1,时间2,金额3,充满为止4
		//int32 value = 5;			//按电量充(度), 时间(分钟),金额(分),充满-100
		message.chargeType = 4;
		message.value = 100;

		message.accountId.funcs.encode = Write_String;
		message.accountId.arg = gBrushInfo.u8TempCARDInfo;

	}	

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, StartChargeRequest_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventRequest message1 = EventRequest_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 1;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	//暂时不用
	/*message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];*/

	message1.timestamp = GetLocalTimeStamp();


	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventRequest_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

unsigned char ProtobufEncodeMessage_EventRequest_StopChargeRequest(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;

	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	StopChargeRequest message = StopChargeRequest_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].JustSentGunNo = 0;

	/* Fill in the message */
	if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].NeedSendGunNo & 0x01)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].JustSentGunNo = 1;

		/* Fill in the message */
		message.stopChargeMode = 1;  //启动充电方式.刷卡1,VIN2,车牌号3 

		/* Fill in the message */
		message.canId = A_GUN_CANID;

		//string accountId = 3;		//VIN, 车牌号, 卡号
		message.accountId.funcs.encode = Write_String;
		message.accountId.arg = gBrushInfo.u8TempCARDInfo;
	}
	else if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].NeedSendGunNo & 0x02)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_StopChargeRequest].JustSentGunNo = 2;

		/* Fill in the message */
		message.stopChargeMode = 1;  //启动充电方式.刷卡1,VIN2,车牌号3 

		/* Fill in the message */
		message.canId = B_GUN_CANID;

		//string accountId = 3;		//VIN, 车牌号, 卡号
		message.accountId.funcs.encode = Write_String;
		message.accountId.arg = gBrushInfo.u8TempCARDInfo;
	}	

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, StopChargeRequest_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventRequest message1 = EventRequest_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 2;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	//暂时不用
	/*message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];*/
	
	message1.timestamp = GetLocalTimeStamp();

	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventRequest_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

unsigned char ProtobufEncodeMessage_EventRequest_AccountRequest(ProtobufEncodeMessageData *pMessageData)
{
	size_t message_length;
	bool status;

	//Key message
	memset(KeyXbuf, 0, sizeof(KeyXbuf));

	AccountRequest message = AccountRequest_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	pb_ostream_t stream = pb_ostream_from_buffer(KeyXbuf, sizeof(KeyXbuf));

	gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].JustSentGunNo = 0;

	/* Fill in the message */
	if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo & 0x01)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].JustSentGunNo = 1;

		/* Fill in the message */
		message.reqType = 3;  //请求类型,3:获取账户列表, 4:获取收费策略

		/* Fill in the message */
		message.canId = A_GUN_CANID;

		//string accountId = 3;		//VIN, 车牌号, 卡号
		message.accountId.funcs.encode = Write_String;
		message.accountId.arg = gBrushInfo.u8TempCARDInfo;

		gs_TelaidianCtrl.AccountRequest[0] = TelaidianAccountRequestFlag_1;
	}
	else if(gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].NeedSendGunNo & 0x02)
	{
		gs_PublicMessage_Ctrl[PUB_MESSAGE_EventRequest_AccountRequest].JustSentGunNo = 2;

		/* Fill in the message */
		message.reqType = 3;  //请求类型,3:获取账户列表, 4:获取收费策略

		/* Fill in the message */
		message.canId = B_GUN_CANID;

		//string accountId = 3;		//VIN, 车牌号, 卡号
		message.accountId.funcs.encode = Write_String;
		message.accountId.arg = gBrushInfo.u8TempCARDInfo;

		gs_TelaidianCtrl.AccountRequest[1] = TelaidianAccountRequestFlag_1;
	}	

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, AccountRequest_fields, &message);
	message_length = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	//main message
	EventRequest message1 = EventRequest_init_zero;

	/* Create a stream that will write to our buffer. */
	//pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	stream = pb_ostream_from_buffer(pMessageData->MessageBuffer, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);

	/* Fill in the message */
	message1.key = 3;
	message1.stationId.funcs.encode = Write_String;
	message1.stationId.arg = gs_TelaidianCtrl.TelaidianStationID;

	//暂时不用
	/*message1.uuid.funcs.encode = Write_String;
	message1.uuid.arg = g_SubMessageUuidBuf[SUB_MESSAGE_TOPIC01_pushdown_data];*/

	message1.timestamp = GetLocalTimeStamp();


	pBytesArray.bytes = (pb_byte_t *)&KeyXbuf;
	pBytesArray.size = message_length;
	message1.bean.funcs.encode = Write_bytes;
	message1.bean.arg = &pBytesArray;

	/* Now we are ready to encode the message! */
	status = pb_encode(&stream, EventRequest_fields, &message1);
	pMessageData->MessageLength = stream.bytes_written;

	/* Then just check for any errors.. */
	if (!status)
	{
		//printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 0;
	}

	return 1;
}

//准备待发布消息的内容（按protobuf编码）
unsigned char ProtobufEncodeMessageFun(unsigned char u8PubMessageNo, ProtobufEncodeMessageData *pMessageData)
{	
	//size_t message_length;
    bool status;

	memset( pMessageData->MessageBuffer, 0, PROTOBUF_ENCODE_MESSAGE_BUF_LEN);
	status = 0;

	switch(u8PubMessageNo)
	{
		case PUB_MESSAGE_EventStateUpload:			
			status = ProtobufEncodeMessage_EventStateUpload(pMessageData);
			break;

		case PUB_MESSAGE_EventStateUpload_Lwt:
			status = ProtobufEncodeMessage_EventStateUpload_lwt(pMessageData);
			break;

		case PUB_MESSAGE_EventUpload_StartChargeResultUp:
			status = ProtobufEncodeMessage_EventUpload_StartChargeResultUp(pMessageData);
			break;

		case PUB_MESSAGE_EventUpload_StopChargeResultUp:
			status = ProtobufEncodeMessage_EventUpload_StopChargeResultUp(pMessageData);
			break;

		case PUB_MESSAGE_EventUpload_TelesignallingDataUp:
			status = ProtobufEncodeMessage_EventUpload_TelesignallingDataUp(pMessageData);			
			break;

		case PUB_MESSAGE_EventUpload_TelemetryDataUp:
			status = ProtobufEncodeMessage_EventUpload_TelemetryDataUp(pMessageData);		
			break;

		case PUB_MESSAGE_EventUpload_MeterDataUp:
			status = ProtobufEncodeMessage_EventUpload_MeterDataUp(pMessageData);		
			break;

		case PUB_MESSAGE_EventUpload_BMSDataUp:
			status = ProtobufEncodeMessage_EventUpload_BMSDataUp(pMessageData);			
			break;

		case PUB_MESSAGE_EventRequest_StartChargeRequest:
			status = ProtobufEncodeMessage_EventRequest_StartChargeRequest(pMessageData);			
			break;

		case PUB_MESSAGE_EventRequest_StopChargeRequest:
			status = ProtobufEncodeMessage_EventRequest_StopChargeRequest(pMessageData);			
			break;

		case PUB_MESSAGE_EventRequest_AccountRequest:
			status = ProtobufEncodeMessage_EventRequest_AccountRequest(pMessageData);			
			break;

		default:
			break;
	}
	return status;
}

//准备发布消息
void PackPublicMessage(unsigned char u8PubMessageNo, MessageData * pMessageData)
{
	switch(u8PubMessageNo)
	{
	  case PUB_MESSAGE_EventStateUpload:
		if(ProtobufEncodeMessageFun(u8PubMessageNo, &gs_ProtobufEncodeMessageData))
		{
			pMessageData->message = &PublicMessage_EventStateUpload;
			pMessageData->message->payload = gs_ProtobufEncodeMessageData.MessageBuffer;			
			pMessageData->message->payloadlen = gs_ProtobufEncodeMessageData.MessageLength;
		}
		else 
		{
			pMessageData->message = &PublicMessage_EventStateUpload;
			pMessageData->message->payloadlen = strlen(pMessageData->message->payload);
		}
		pMessageData->topicName = &PublicMessage_EventStateUpload_Topic;
		break;

	  case PUB_MESSAGE_EventStateUpload_Lwt:
		  if(ProtobufEncodeMessageFun(u8PubMessageNo, &gs_ProtobufEncodeMessageData))
		  {
			  pMessageData->message = &PublicMessage_EventUploadLwt;
			  pMessageData->message->payload = gs_ProtobufEncodeMessageData.MessageBuffer;			
			  pMessageData->message->payloadlen = gs_ProtobufEncodeMessageData.MessageLength;
		  }
		  else 
		  {
			  pMessageData->message = &PublicMessage_EventUploadLwt;
			  pMessageData->message->payloadlen = strlen(pMessageData->message->payload);
		  }
		  pMessageData->topicName = &PublicMessage_EventUploadLwt_Topic;
		  break;

	  case PUB_MESSAGE_EventUpload_StartChargeResultUp:
	  case PUB_MESSAGE_EventUpload_StopChargeResultUp:
	  case PUB_MESSAGE_EventUpload_TelesignallingDataUp:
	  case PUB_MESSAGE_EventUpload_TelemetryDataUp:
	  case PUB_MESSAGE_EventUpload_MeterDataUp:
	  case PUB_MESSAGE_EventUpload_BMSDataUp:
		  if(ProtobufEncodeMessageFun(u8PubMessageNo, &gs_ProtobufEncodeMessageData))
		  {
			  pMessageData->message = &PublicMessage_EventUpload;
			  pMessageData->message->payload = gs_ProtobufEncodeMessageData.MessageBuffer;			
			  pMessageData->message->payloadlen = gs_ProtobufEncodeMessageData.MessageLength;
		  }
		  else 
		  {
			  pMessageData->message = &PublicMessage_EventUpload;
			  pMessageData->message->payloadlen = strlen(pMessageData->message->payload);
		  }
		  pMessageData->topicName = &PublicMessage_EventUpload_Topic;
		  break;

	  case PUB_MESSAGE_EventRequest_StartChargeRequest:
	  case PUB_MESSAGE_EventRequest_StopChargeRequest:
	  case PUB_MESSAGE_EventRequest_AccountRequest:
		  if(ProtobufEncodeMessageFun(u8PubMessageNo, &gs_ProtobufEncodeMessageData))
		  {
			  pMessageData->message = &PublicMessage_EventRequest;
			  pMessageData->message->payload = gs_ProtobufEncodeMessageData.MessageBuffer;			
			  pMessageData->message->payloadlen = gs_ProtobufEncodeMessageData.MessageLength;
		  }
		  else 
		  {
			  pMessageData->message = &PublicMessage_EventRequest;
			  pMessageData->message->payloadlen = strlen(pMessageData->message->payload);
		  }
		  pMessageData->topicName = &PublicMessage_EventRequest_Topic;
		  break;
	default:
		break;
	}
}