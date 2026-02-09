#include "main.h"
#include "EtherNet.h"
#include "Uart6_new.h"
#include "resolv.h"
#include "can_ring.h"
#include "ethcomm.h"
#include "ALM.h"

NOINIT UINT8 gNetBuf[NET_MAX_BUF];
NOINIT UINT8 gNetR_Buf[NET_MAX_BUF];
NOINIT UINT8 gNetS_Buf[NET_MAX_BUF];

extern NOINIT __NET_TCP_INFO gNET_TCP_Info;
extern NOINIT ChargeLogTable   gChargeLogTable;
ChargeLogTable UnRptLogTable;//未上送记录序号记录
EthAppCommMng gs_EthAppCommMng;

static void RefreshEtherNetTime(void)
{
	struct tm  tmNow;
	static UCHAR  Sec = 0;

	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)//秒钟
	{
		Sec = (UCHAR)tmNow.tm_sec;
	}
}
static void initSendBuffer(void)
{
      UINT16 i;
      for (i = 0; i< NET_BUF_S_DATA_L; i++)
      {
              gNetS_Buf[i] = '\0';
      }
}
static void InitNetData(void)
{
  UINT8 i,j;
  struct tm tmNow;		//最后一次启动时间
	gs_EthAppCommMng.u8NetSeqNo = 0;
	gs_EthAppCommMng.u16APP_PipeWTDOG = 0;			//防止SOCKET挂死！
	gs_EthAppCommMng.u8RegistResult = FALSE;				//登录结果
	gs_EthAppCommMng.u16RegistCount = 0;				//未收到校时指令后，每隔15S，登录一下。
	gs_EthAppCommMng.u16RegistInterval = 30;			//签到间隔 单位分钟
	gs_EthAppCommMng.u8HeartBeatCount = 0;			//心跳计时
	gs_EthAppCommMng.u8HeartBeatInterval = 30;			//心跳周期
	gs_EthAppCommMng.u8HeartBeatTimeOutTimes = 3;		//心跳超时次数
	gs_EthAppCommMng.u8HeartBeatAckCount = 0;			//心跳回复计时
	gs_EthAppCommMng.u8HeartBeatAckErr = 3;			//心跳超时次数
	gs_EthAppCommMng.u16EVStateInterval = 60;
	gs_EthAppCommMng.u8TickFlag = FALSE;
	gs_EthAppCommMng.u8CMD1Flag = FALSE;
	gs_EthAppCommMng.u8CMD3Flag = FALSE;
	gs_EthAppCommMng.u8CMD5Flag = FALSE;
	gs_EthAppCommMng.u8CMD7Flag = FALSE;
	for(i=0;i<ALL_GUN_NUM;i++)
	{
		for(j=0;j<COST_NO_BYTES;j++)
		{
		  gs_EthAppCommMng.u8ExchangeCode[i][j] = 0;
		  gs_EthAppCommMng.u8APPInfo[i][j] = 0;
		}
		for(j=0;j<128;j++)
		{
		  gs_EthAppCommMng.ucSetCodeByHost[i][j] = 0;
		}
		for(j=0;j<16;j++)
		{
		  gs_EthAppCommMng.ucSetIdentify[i][j] = 0;
		}
		for(j=0;j<8;j++)
		{
		  gs_EthAppCommMng.u8OrderSetTime[i][j] = 0;
		}
		gs_EthAppCommMng.u8GetExchangecodeFlag[i] = FALSE;
		gs_EthAppCommMng.u8GetExchangecodeCount[i] = 0;
		gs_EthAppCommMng.u8WorkStat[i] = EV_STAT_IDL;
		gs_EthAppCommMng.u16EVStateCount[i]= 0;
		gs_EthAppCommMng.u8RcvFlagOfCodeByHost[i] = FALSE;
		gs_EthAppCommMng.u8RcvFlagOfIdentifyID[i] = FALSE;
		gs_EthAppCommMng.u16OrderTimeOutCount[i] = 0;
		gs_EthAppCommMng.u16OrderTimeOut[i] = 0;				//预约超时时间	用于回传 不用于记数
		gs_EthAppCommMng.u8OrderChargeCMD[i] = FALSE;			//0-无预约（无效）	1-预约有效
		gs_EthAppCommMng.u32AlmInfo[i] = 0;
		gs_EthAppCommMng.u16BattDataCount[i] = 0;
		gs_EthAppCommMng.u16BattDataCMDSeqNo[i] = 0;
		gs_EthAppCommMng.u8EndInfoFlag[i]= FALSE;
		gs_EthAppCommMng.u16EndInfoCount[i] =0;	
		gs_EthAppCommMng.u8VINIdentifyCount[i]=0;
		gs_EthAppCommMng.u8VINIdentifyDelay[i] = 0;
		gs_EthAppCommMng.u8AccountType[i] = USER_Card_Type;
		gs_EthAppCommMng.ucEndStateUpdataCount[i] = 0;  //状态报文中，结束状态需要传三次
		gs_EthAppCommMng.u8OffLineFlag = TRUE;			//0-不允许 1-允许
		gs_EthAppCommMng.fOffLineAllowPower = 500;
	}
	gs_EthAppCommMng.u16HisEndInfoCount = 0;
	gs_EthAppCommMng.ui16DHCPReconfigTimeOUT = 0;	
	gs_EthAppCommMng.u8CardIdentifyCount = 0;
	gs_EthAppCommMng.u8CMD1301Flag = FALSE;
	gs_EthAppCommMng.u8CMD1303Flag = FALSE;
	gs_EthAppCommMng.u8CMD1305Flag = FALSE;
	gs_EthAppCommMng.u8CMD1307Flag = FALSE;

	gRTCData.fGetLocalTime(&tmNow);
	gs_EthAppCommMng.PowerOnTime = tmNow;
	
}

static UCHAR CheckSendDataFlag(void)
{
	UCHAR i;
	for(i=0;i<3;i++)
	{
		if(gs_EthAppCommMng.ucNeedSendFrame[i] == TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
}
static void ConnectServer(void)
{
	if(gs_EthAppCommMng.ucConnectIntervalCount == 0)
	{
		gNET_TCP_Info.iNetCreateSocketFlag = TRUE;
		gs_EthAppCommMng.ucConnectIntervalCount = 30;
	}
}
static void DisConnectServer (void)
{

}
void TASK_NET_ProtocolProc(void* pdata)
{
	UCHAR ucNetEnable,RTN;
	gNET_TCP_Info.u8DHCPResult = FALSE;
	OSTimeDly(1000);
	InitNetData();
	for(;;)
	{
		gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
		RefreshEtherNetTime();
		OSTimeDly(100);

    }
}
