/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : CANManag.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : CAN信息管理函数模块
** @Instructions :
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "BSPUserAPI.h"
#include "SystemInfo.h"
#include "LVMsgSamp.h"
#include "EepData.h"
#include "SysState.h"
#include "SysCtrl.h"
#include "RcvCfgHandle.h"
#include "CellDNB110xSamp.h"
#include "CellAFE79616Samp.h"
#include "CANManag.h"
//#include "SEGGER_RTT.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
extern u16 gAlarmInfo_53[MAX_DASIY_NUM][INDEX_53_LEN];//告警详细信息
extern u16 gFaultInfo_54[];                 //故障信息
//extern s16 gLVSampInfo_61[];              //板级采集信息

extern u16 gLVSampInfo_68[];                //板级采集信息
extern u16 gHvOrFGInfo_67[];                //外部检测通道信息
extern s16 gPtOrWatInfo_65[];               //外部模拟量采集信息

extern u16 gDevInfo_1[];                    //设备信息
extern u8 gCANSerialWrData_2[INDEX_2_LEN];  //设备序列号
extern u8 gCANSerialRdData_2[INDEX_2_LEN];
extern u16 gCellVolt_90[][INDEX_90_LEN];    //单体电压信息
extern u16 gCellBState_91[][INDEX_91_LEN];  //单节均衡状态信息
extern s8 gCellTemp_92[INDEX_92_LEN];       //单体温度信息
//extern s8 gCellBTemp_93[][INDEX_93_LEN];  //均衡板上温度信息
extern u16 gAersolSenInfo_69[];             //BSU02U1 溶胶传感器信息
extern u16 gPtOrWatSelfInfo_66[];           //BSU02U1 PT000和水浸电路自检信息
//extern u16 gSysCtrl_70[];                 //系统控制命令
//#if(1 == AFE_TYPE_CHOICE)
extern u16 gCellZm_96[INDEX_96_LEN];
//#endif
extern u16 gEepNorPara_102[];               //设备基本信息配置参数

extern u16 gModeState_50[];                	//工作模式


extern s16 gCellBState_62[INDEX_62_LEN];    //单体特征信息

extern u16 gVolInfo_63[INDEX_63_LEN];       //电压特征信息

extern s16 gTemInfo_64[INDEX_64_LEN];       //温度特征信息
extern u16 gNorParaCfg_107[INDEX_107_LEN];

u8 sParaTxTime = 0;                         //连续发送当前参数值的时间(帧数)

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=================================================================================================
** @Name      : void CalcVoltAndTempCharaMsgTask(void)
** @Input     : void
** @Output    : void
** @Function  : 计算电压温度最值信息任务
** @The notes : 100ms周期
**===============================================================================================*/
void CalcVoltAndTempCharaMsgTask(void)
{
 	u16 i = 0;
    u8 packId = 0;              //电池包序号
    u8 cellId = 0;              //电池包内电压序号
    u8 packNum = 0;             //电池包数
    u8 packVNum = 0;            //电池包电压数
    
    u16 MaxVoltage = 0;         //最大单体电压
	u16 MaxVolNum = 1;          //最大单体电压序号
	u16 MinVoltage = 0xFFFF;    //最小单体电压
	u16 MinVolNum = 1;          //最小单体电压序号
	u32 TotalVoltage = 0;       //总电压
    
    s16 MaxTemp = -41;          //最大单体温度
	u16 MaxTempNum = 1;         //最大单体温度序号
	s16 MinTemp = 126;          //最小单体温度
	u16 MinTempNum = 1;         //最小单体温度序号
	s32 TotalTemp = 0;          //累加温度(求平均温度)
	
    //单节电压温度第一遍未采样完成
	if((1 != GetCellVSampFinFlagAPI())
       || (1 != GetCellTSampFinFlagAPI()))
    {
        return;
    }
	
    packNum = GetDaisyAFENum();  
    packVNum = GetVoltSampNum();
	    
    //电压节数不少于温度数目
	for(i = 0; i < GetTotalVoltNum(); i++)
	{
        //计算电压顺序号对应电池箱序号和电池箱内电池序号
        packId = i / packVNum;
        cellId = i % packVNum;
        //电池箱序号或电池箱内电池序号正常
        if((packId < packNum) && (cellId < packVNum))
        {
            //计算总电压
			TotalVoltage += gCellVolt_90[packId][cellId];
            
            //计算电压最值
            if(MaxVoltage < gCellVolt_90[packId][cellId])
			{
				MaxVoltage = gCellVolt_90[packId][cellId];
				MaxVolNum = i + 1;
			}
			if(MinVoltage > gCellVolt_90[packId][cellId])
			{
				MinVoltage = gCellVolt_90[packId][cellId];
				MinVolNum = i + 1;
			}
        }
        
        //温度序号范围内
        if(i < GetTotalTempNum())
        {
            TotalTemp += gCellTemp_92[i];

            if(MaxTemp < gCellTemp_92[i])
            {
                MaxTemp = gCellTemp_92[i];
                MaxTempNum = i + 1;
            }
            if(MinTemp > gCellTemp_92[i])
            {
                MinTemp = gCellTemp_92[i];
                MinTempNum = i + 1;
            }
        }
    }
	
	gCellBState_62[e62BCell_TotalV] = TotalVoltage / 100;  //0.1V
	gCellBState_62[e62BCell_AvgV] = TotalVoltage / GetTotalVoltNum();
	gCellBState_62[e62BCell_AvgT] = TotalTemp / GetTotalTempNum();
    
    gVolInfo_63[e63Vol_MaxNum] = MaxVolNum;
	gVolInfo_63[e63Vol_MaxV] = MaxVoltage;
	gVolInfo_63[e63Vol_MinNum] = MinVolNum;
	gVolInfo_63[e63Vol_MinV] = MinVoltage;
    
    gTemInfo_64[e64Tem_MaxNum] = MaxTempNum;
	gTemInfo_64[e64Tem_MaxT] = MaxTemp;
	gTemInfo_64[e64Tem_MinNum] = MinTempNum;
	gTemInfo_64[e64Tem_MinT] = MinTemp;
    
    //PT1000自检完成
	if(2 == GetPtOrWatCheckSelfFlagAPI(eCheckSelfType_PT1000))
	{
        //环境温度选择两路PT1000的最大值
        if(gPtOrWatInfo_65[e65PtOrWatInfo_PT1] >= gPtOrWatInfo_65[e65PtOrWatInfo_PT2])
        {
            gCellBState_62[e62BCell_EnvT] = gPtOrWatInfo_65[e65PtOrWatInfo_PT1];
        }
        else
        {
            gCellBState_62[e62BCell_EnvT] = gPtOrWatInfo_65[e65PtOrWatInfo_PT2];
        }
	}
    else
    {
        gCellBState_62[e62BCell_EnvT] = -41;
    }
}

/*=================================================================================================
** @Name      : void CanRcvRequestWriteUserHook(u8 mindex, u8 sindex, u8 len)
** @Input     : mindex:主索引 sindex:起始子索引 len:子索引数目
** @Output    : void
** @Function  : 接收完请求修改后的用户处理
** @The notes : 接收完请求修改信息后调用
**===============================================================================================*/
void CanRcvRequestWriteUserHook(u8 mindex, u8 sindex, u8 len)
{
  u8 i = 0;
	//根据主索引号区别处理
	switch(mindex)
	{
		//设置序列号
		case 2:
			//控制子索引越界
			if((sindex + len) > INDEX_2_LEN)
			{
				return;
			}
            //更改设备序列号
			RcvSerialNumCfgDeal();
            
            //修改回复
			BSU02U1SendUpReplyMsgSerial(1);
			break;
            
		/*//接收系统控制命令
		case 85:
			//控制子索引越界
			if((sindex + len) > INDEX_50_LEN)
			{
				return;
			}
			SetSysWorkMode(gModeState_50[e50Mode_Work]);
			
            BSU02U1SendUpReplyMsgWorkMode(1);//修改回复(模式变化会自动上传,不需要回复)
			break;*/
            
		//接收常规参数配置命令  
		case 107:
            //上电前3s禁止设置参数
            if(BSPGetCosTimer() < 3000)
            {
                return;
            }
                        
			//参数子索引越界
			if(((sindex + len) > e102CfgDataFin) || ((sindex + len) > INDEX_107_LEN))
			{
				return;
			}
            //解析每个参数
            for(i = 0; i < len; i++)
            {
                RcvNorParaCfgDeal(sindex + i);
            }
			//应答更新后的参数
			BSU02U1SendUpReplyMsgParaCfg(1, sindex, len);
			break;
            
		default:
			return;
	}
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgWorkModeTask(void)
** @Input     : Auto
** @Output    : void
** @Function  : 周期上传单帧信息任务:系统工作模式
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgWorkModeTask(void)
{
    t_CANMSG msg;
    static u8 sTime = 10;
    
    //调试模式连续发送
    if(eMODE_DEBUG == GetSysWorkMode())
    {
        sTime = 10;
    }
    //非调试模式发送10s
    else if(0 == sTime)
    {
      return;
    }
    else
    {
        sTime--;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x32=50:系统工作模式信息
    msg.ID = 0x1432FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    
    msg.Len = 8;

    msg.Data[0] = gModeState_50[e50Mode_Work] & 0xff;
    msg.Data[1] = (gModeState_50[e50Mode_Work] >> 8) & 0xff;

    msg.Data[2] = 0x00;
    msg.Data[3] = 0x00;
    msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;
    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgMainCharaTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:单体总体特征信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgMainCharaTask(void)
{
	t_CANMSG msg;
    static u8 sTime = 10;
    
    //上电前5s或单节电压温度第一遍未采样完成
	if((BSPGetCosTimer() < 5000)
       || (1 != GetCellVSampFinFlagAPI())
       || (1 != GetCellTSampFinFlagAPI()))
    {
        return;
    }
	
    //PT1000未自检完成
	if(2 != GetPtOrWatCheckSelfFlagAPI(eCheckSelfType_PT1000))
	{
		//return;
	}
    
    //调试模式连续发送
    if(eMODE_DEBUG == GetSysWorkMode())
    {
        sTime = 10;
    }
    //非调试模式发送10s
    else if(0 == sTime)
    {
      return;
    }
    else
    {
        sTime--;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x3E=62:单体总体特征信息
	msg.ID = 0x143EFF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    
    msg.Len = 8;
    
	msg.Data[0] = gCellBState_62[e62BCell_TotalV] & 0xff;   //总电压
   	msg.Data[1] = (gCellBState_62[e62BCell_TotalV] >> 8) & 0xff;
	
	msg.Data[2] = gCellBState_62[e62BCell_AvgV] & 0xff;     //平均电压
   	msg.Data[3] = (gCellBState_62[e62BCell_AvgV] >> 8) & 0xff;
	
	msg.Data[4] = gCellBState_62[e62BCell_AvgT] & 0xff;     //平均温度
   	msg.Data[5] = (gCellBState_62[e62BCell_AvgT] >> 8) & 0xff;
    
	msg.Data[6] = gCellBState_62[e62BCell_EnvT] & 0xFF;     //环境温度
    msg.Data[7] = (gCellBState_62[e62BCell_EnvT] >> 8) & 0xFF;
	
	(void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgVoltCharaTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:单体电压特征信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgVoltCharaTask(void)
{
	t_CANMSG msg;
    static u8 sTime = 10;
    
    //上电前5s或单节电压第一遍未采样完成
	if((BSPGetCosTimer() < 5000)
       || (1 != GetCellVSampFinFlagAPI()))
    {
        return;
    }
    
    //调试模式连续发送
    if(eMODE_DEBUG == GetSysWorkMode())
    {
        sTime = 10;
    }
    //非调试模式发送10s
    else if(0 == sTime)
    {
      return;
    }
    else
    {
        sTime--;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x3F=63:单体电压特征信息
	msg.ID = 0x143FFF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
	msg.Data[0] = gVolInfo_63[e63Vol_MaxNum] & 0xff;        //最高电压电池号
   	msg.Data[1] = (gVolInfo_63[e63Vol_MaxNum] >> 8) & 0xff;
	
	msg.Data[2] = gVolInfo_63[e63Vol_MaxV] & 0xff;          //最高电压值
   	msg.Data[3] = (gVolInfo_63[e63Vol_MaxV] >> 8) & 0xff;
	
	msg.Data[4] = gVolInfo_63[e63Vol_MinNum] & 0xff;        //最低电压电池号
   	msg.Data[5] = (gVolInfo_63[e63Vol_MinNum] >> 8) & 0xff;
	
	msg.Data[6] = gVolInfo_63[e63Vol_MinV] & 0xFF;          //最低电压值
    msg.Data[7] = (gVolInfo_63[e63Vol_MinV] >> 8) & 0xFF;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgTempCharaTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:单体温度特征信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgTempCharaTask(void)
{
	t_CANMSG msg;
    static u8 sTime = 10;
    
    //上电前5s或单节温度第一遍未采样完成
	if((BSPGetCosTimer() < 5000)
       || (1 != GetCellTSampFinFlagAPI()))
    {
        return;
    }
    
    //调试模式连续发送
    if(eMODE_DEBUG == GetSysWorkMode())
    {
        sTime = 10;
    }
    //非调试模式发送10s
    else if(0 == sTime)
    {
        return;
    }
    else
    {
        sTime--;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x40=64:单体温度特征信息
	msg.ID = 0x1440FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    
	msg.Len = 8;
    
	msg.Data[0] = gTemInfo_64[e64Tem_MaxNum] & 0xff;        //最高温度电池号
   	msg.Data[1] = (gTemInfo_64[e64Tem_MaxNum] >> 8) & 0xff;
	
	msg.Data[2] = gTemInfo_64[e64Tem_MaxT] & 0xff;          //最高温度值
   	msg.Data[3] = (gTemInfo_64[e64Tem_MaxT] >> 8) & 0xff;
	
	msg.Data[4] = gTemInfo_64[e64Tem_MinNum] & 0xff;        //最低温度电池号
   	msg.Data[5] = (gTemInfo_64[e64Tem_MinNum] >> 8) & 0xff;
    
	msg.Data[6] = gTemInfo_64[e64Tem_MinT] & 0xFF;          //最低温度值
    msg.Data[7] = (gTemInfo_64[e64Tem_MinT] >> 8) & 0xFF;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgAlarmInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:告警信息(故障定位)
** @The notes : 400ms
**===============================================================================================*/
void BSU02U1SendUpsMsgAlarmInfoTask(void)
{
    u8 i = 0;
    u8 index = 0;       //首个故障索引
	t_CANMSG msg;
	
    //0x14=10<<1:发送单帧信息
    //0x35=53:告警信息
    msg.ID = 0x1435FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
    //查找第一个异常的电池PACK
    for(i = 0; i < GetDaisyAFENum(); i++)
    {
        //有故障定位信息
        if((gAlarmInfo_53[i][0] > 0)
           || (gAlarmInfo_53[i][1] > 0)
           || (gAlarmInfo_53[i][2] > 0))
        {
            index = i + 1;
            break;
        }
    }
    
    //有故障定位信息
    if(index > 0)
    {
        msg.Data[0] = index & 0xff;
        msg.Data[1] = (index >> 8) & 0xff;
        msg.Data[2] = gAlarmInfo_53[index - 1][0] & 0xff;
        msg.Data[3] = (gAlarmInfo_53[index - 1][0] >> 8) & 0xff;

        msg.Data[4] = gAlarmInfo_53[index - 1][1] & 0xff;
        msg.Data[5] = (gAlarmInfo_53[index - 1][1] >> 8) & 0xff;
            
        msg.Data[6] = gAlarmInfo_53[index - 1][2] & 0xff;
        msg.Data[7] = (gAlarmInfo_53[index - 1][2] >> 8) & 0xff;
    }
    //无定位信息
    else
    {
        msg.Data[0] = 0;
        msg.Data[1] = 0;
        msg.Data[2] = 0;
        msg.Data[3] = 0;
        msg.Data[4] = 0;
        msg.Data[5] = 0;
        msg.Data[6] = 0;
        msg.Data[7] = 0;
    }
    
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgFaultInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:故障信息
** @The notes : 400ms
**===============================================================================================*/
void BSU02U1SendUpsMsgFaultInfoTask(void)
{
    t_CANMSG msg;
    
    //0x14=10<<1:发送单帧信息
    //0x36=54:故障信息
    msg.ID = 0x1436FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;

    msg.Data[0] = gFaultInfo_54[0] & 0xff;
    msg.Data[1] = (gFaultInfo_54[0] >> 8) & 0xff;

    msg.Data[2] = gFaultInfo_54[1] & 0xff;
    msg.Data[3] = (gFaultInfo_54[1] >> 8) & 0xff;

    msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;
    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgPtWatSampInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:PT1000温度水浸监测信息
** @The notes : 500ms
**===============================================================================================*/
void BSU02U1SendUpsMsgPtWatSampInfoTask(void)
{
	t_CANMSG msg;
    
    //低压采样未完成
    if(1 != GetLVSampFinishFlag())
    {
        return;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x41=65:PT1000温度水浸监测信息
	msg.ID = 0x1441FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
    msg.Data[0] = (gPtOrWatInfo_65[e65PtOrWatInfo_PT1]) & 0xff;
    msg.Data[1] = ((gPtOrWatInfo_65[e65PtOrWatInfo_PT1]) >> 8) & 0xff;

    msg.Data[2] = (gPtOrWatInfo_65[e65PtOrWatInfo_PT2]) & 0xff;
    msg.Data[3] = ((gPtOrWatInfo_65[e65PtOrWatInfo_PT2]) >> 8) & 0xff;
    
    msg.Data[4] = gPtOrWatInfo_65[e65PtOrWatInfo_WaterSen1] & 0xff;
    msg.Data[5] = (gPtOrWatInfo_65[e65PtOrWatInfo_WaterSen1] >> 8) & 0xff;

    msg.Data[6] = gPtOrWatInfo_65[e65PtOrWatInfo_WaterSen2] & 0xff;
    msg.Data[7] = (gPtOrWatInfo_65[e65PtOrWatInfo_WaterSen2] >> 8) & 0xff;

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgPtWatSelfInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:PT1000温度水浸自检信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgPtWatSelfInfoTask(void)
{
	t_CANMSG msg;
    
    //传感器自检未完成
    if(1 != GetWaterSenCheckFlagAPI())
    {
        return;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x42=66:PT1000温度水浸自检信息
	msg.ID = 0x1442FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
    msg.Data[0] = (gPtOrWatSelfInfo_66[eCheckSelfInfo_PT1]) & 0xff;
    msg.Data[1] = ((gPtOrWatSelfInfo_66[eCheckSelfInfo_PT1]) >> 8) & 0xff;

    msg.Data[2] = (gPtOrWatSelfInfo_66[eCheckSelfInfo_PT2])& 0xff;
    msg.Data[3] = ((gPtOrWatSelfInfo_66[eCheckSelfInfo_PT2]) >> 8) & 0xff;

    msg.Data[4] = gPtOrWatSelfInfo_66[eCheckSelfInfo_WaterSen1] & 0xff;
    msg.Data[5] = (gPtOrWatSelfInfo_66[eCheckSelfInfo_WaterSen1] >> 8) & 0xff;

    msg.Data[6] = gPtOrWatSelfInfo_66[eCheckSelfInfo_WaterSen2] & 0xff;
    msg.Data[7] = (gPtOrWatSelfInfo_66[eCheckSelfInfo_WaterSen2] >> 8) & 0xff;

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgHvFgSampInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:高压互锁和FG监测信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgHvFgSampInfoTask(void)
{
    t_CANMSG msg;
    
    //低压采样未完成
    if(1 != GetLVSampFinishFlag())
    {
        return;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x43=67:高压互锁和FG监测信息
    msg.ID = 0x1443FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;

    msg.Data[0] = gHvOrFGInfo_67[e67HvOrFG_HLVIC] & 0xff;
    msg.Data[1] = (gHvOrFGInfo_67[e67HvOrFG_HLVIC] >> 8) & 0xff;

    msg.Data[2] = gHvOrFGInfo_67[e67HvOrFG_HLVPWM_DUTY] & 0xff;
    msg.Data[3] = (gHvOrFGInfo_67[e67HvOrFG_HLVPWM_DUTY] >> 8) & 0xff;

    msg.Data[4] = gHvOrFGInfo_67[e67HvOrFG_FGFreq] & 0xff;
    msg.Data[5] = (gHvOrFGInfo_67[e67HvOrFG_FGFreq] >> 8) & 0xff;

    msg.Data[6] = gHvOrFGInfo_67[e67HvOrFG_FGDuty] & 0xff;
    msg.Data[7] = (gHvOrFGInfo_67[e67HvOrFG_FGDuty] >> 8) & 0xff;

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsg5V12VSelfInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:5V12V自检信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsg5V12VSelfInfoTask(void)
{
    t_CANMSG msg;
    
    //低压采样未完成
    if(1 != GetLVSampFinishFlag())
    {
        return;
    }
    
    //0x14=10<<1:发送单帧信息
    //0x44=68:5V12V自检信息
	msg.ID = 0x1444FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
    msg.Data[0] = gLVSampInfo_68[e68LV_PT5V] & 0xff;
    msg.Data[1] = (gLVSampInfo_68[e68LV_PT5V] >> 8) & 0xff;

    msg.Data[2] = gLVSampInfo_68[e68LV_OUT12V] & 0xff;
    msg.Data[3] = (gLVSampInfo_68[e68LV_OUT12V] >> 8) & 0xff;

    msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;
    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpsMsgAeroSenInfoTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单帧信息任务:气溶胶传感器信息
** @The notes : 1000ms
**===============================================================================================*/
void BSU02U1SendUpsMsgAeroSenInfoTask(void)
{
    t_CANMSG msg;
    
    //0x14=10<<1:发送单帧信息
    //0x45=69:气溶胶传感器信息
    msg.ID = 0x1445FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;

    msg.Data[0] = gAersolSenInfo_69[eAeroSenInfo_Concent] & 0xff;
    msg.Data[3] = (gAersolSenInfo_69[eAeroSenInfo_Concent] >> 8) & 0xff;

    msg.Data[4] = gAersolSenInfo_69[eAeroSenInfo_WakeUpData] & 0xff;
    msg.Data[5] = (gAersolSenInfo_69[eAeroSenInfo_WakeUpData] >> 8) & 0xff;

    msg.Data[6] = gAersolSenInfo_69[eAeroSenInfo_State] & 0xff;
    msg.Data[7] = (gAersolSenInfo_69[eAeroSenInfo_State] >> 8) & 0xff;

    msg.Data[2] = gAersolSenInfo_69[eAeroSenInfo_RcAndCrc] & 0xff;
    msg.Data[3] = (gAersolSenInfo_69[eAeroSenInfo_RcAndCrc] >> 8) & 0xff;

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpcMsgCellVTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单体信息帧任务:电池单体电压值信息
** @The notes : 10ms周期
**===============================================================================================*/
void BSU02U1SendUpcMsgCellVTask(void)
{
    u8 i = 0;
    u8 packId = 0;              //电池包序号
    u8 cellId = 0;              //电池包内电压序号
    u8 packNum = 0;             //电池包数
    u8 packVNum = 0;            //电池包电压数
    u16 sumVNum = 0;            //所有电压数
    
    t_CANMSG msg;
    static u8 sIndex = 0;    //发送电压信息帧序号
    
    //单节电压第一遍未采样完成
    if(1 != GetCellVSampFinFlagAPI())
    {
        return;
    }
    
    packNum = GetDaisyAFENum();
    packVNum = GetVoltSampNum();
    sumVNum = GetTotalVoltNum();
    
    //先确认帧序号没越界
    if((sIndex * 4) >= sumVNum)
    {
        sIndex = 0;
    }
    
    //0x16=10<<1:发送单体信息帧
    //单体电压ID索引从50开始
    msg.ID = 0x1600FF00 | ((u32)((u32)sIndex+LONG_VMSG_BASE) << 16) | (u32)gEepNorPara_102[e102Dev_Ecuid];
    
    msg.Len  = 8;
    
    //每帧发送4个数据
    for(i = 0; i < 4; i++)
    {
        //在总数范围内
        if(((sIndex * 4) + i) < sumVNum)
        {
            //计算帧索引对应电池箱序号和电池箱内电池序号
            packId = ((sIndex * 4) + i) / packVNum;
            cellId = ((sIndex * 4) + i) % packVNum;
            //电池箱序号或电池箱内电池序号正常
            if((packId < packNum) && (cellId < packVNum))
            {
                //取电压值发送
                msg.Data[i * 2] = gCellVolt_90[packId][cellId] & 0xff;
                msg.Data[i * 2 + 1] = (gCellVolt_90[packId][cellId] >> 8) & 0xff;
            }
            //电池箱序号或电池箱内电池序号越界
            else
            {
                //剩余数据域补0
                msg.Data[i * 2] = 0;
                msg.Data[i * 2 + 1] = 0;
            }
        }
        else
        {
            //剩余数据域补0
            msg.Data[i * 2] = 0;
            msg.Data[i * 2 + 1] = 0;
        }
    }    
    
    sIndex++;
    //帧序号达到最大电压数目
    if((sIndex * 4) >= sumVNum)
    {
        sIndex = 0;
    }
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpcMsgCellTTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单体信息帧任务:电池单体温度值信息
** @The notes : 100ms周期
**===============================================================================================*/
void BSU02U1SendUpcMsgCellTTask(void)
{
    u8 i = 0;
    u16 sumTNum = 0;          //所有温度数
    
    t_CANMSG msg;
    static u8 sIndex = 0;    //发送温度信息帧序号
    
    //单节温度第一遍未采样完成
    if(1 != GetCellTSampFinFlagAPI())
    {
        return;
    }
    
    sumTNum = GetTotalTempNum();
    //有均衡温度的从控
	if((AFE_TYPE_BYD == GetDaisyAFEType()) || (AFE_TYPE_ADI == GetDaisyAFEType()))
    {
        sumTNum += (DEFAULT_BTNUM * GetDaisyAFENum());
    }
    
    //先确认帧序号没越界
    if((sIndex * 8) >= sumTNum)
    {
        sIndex = 0;
    }

    //0x16=10<<1:发送单体信息帧
    //单体电压ID索引从150开始
    msg.ID = 0x1600FF00 | ((u32)((u32)sIndex+LONG_TMSG_BASE) << 16) | (u32)gEepNorPara_102[e102Dev_Ecuid];
    
    msg.Len = 8;
    
    //每帧发送8个数据
    for(i = 0; i < 8; i++)
    {
        //总数范围内
        if(((sIndex * 8) + i) < sumTNum)
        {
            //取温度值发送
            msg.Data[i] = gCellTemp_92[(sIndex * 8) + i] & 0xff;
        }
        else
        {
            //剩余数据域补0
            msg.Data[i] = 0;
        }
    }
                
    sIndex++;
    //帧序号达到最大温度数目
    if((sIndex * 8) >= sumTNum)
    {
        sIndex = 0;
    }
    
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpcMsgDhgBalanTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传单体信息帧任务:电池单体均衡放电状态信息
** @The notes : 400ms周期
**===============================================================================================*/
void BSU02U1SendUpcMsgDhgBalanTask(void)
{
    u8 i = 0;
    u8 sumIndex = 0;           //总索引个数
    
    t_CANMSG msg;
    static u8 sIndex = 0;    //发送均衡信息帧序号
    
    //单节电压第一遍未采样完成
    if(1 != GetCellVSampFinFlagAPI())
    {
        return;
    }
    
    //电池连续放满16BIT(非一个PACK一个索引)
    sumIndex = (GetTotalVoltNum() + 15) / 16;
    
    //先确认帧序号没越界
    if((sIndex * 4) >= sumIndex)
    {
        sIndex = 0;
    }
    
    //0x16=10<<1:发送单体信息帧
    //放电均衡状态ID索引从25开始
    msg.ID = 0x1600FF00 | ((u32)((u32)sIndex+LONG_BMSG_BASE) << 16) | (u32)gEepNorPara_102[e102Dev_Ecuid];
    msg.Len  = 8;  
    
    //每帧发送4个数据
    for(i = 0; i < 4; i++)
    {
        if(((sIndex * 4) + i) < sumIndex)
        {
            msg.Data[i * 2] = gCellBState_91[sIndex * 4 + i][e91BState_PasDhg] & 0xff;
            msg.Data[i * 2 + 1] = (gCellBState_91[sIndex * 4 + i][e91BState_PasDhg] >> 8) & 0xff;
        }
        else
        {
            //剩余数据域补0
            msg.Data[i * 2] = 0;
            msg.Data[i * 2 + 1] = 0;
        }
	}
    
    //下一帧
    sIndex++;
    
    //帧序号达到最大电池箱数目
    if((sIndex * 4) >= sumIndex)
    {
        sIndex = 0;
    }
    
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

//#if(1 == AFE_TYPE_CHOICE)
/*=================================================================================================
** @Name      : void BSU02U1SendUppMsgCellRTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传主动更新信息帧任务:电池单体内阻信息
** @The notes : 100ms周期,按长帧格式上传
**===============================================================================================*/
void BSU02U1SendUppMsgCellRTask(void)
{
    u8 i = 0;
    u16 txIndex = 0;          //当前发送主索引
    u16 txRNum = 0;           //需发送主索引数
    u16 sumRNum = 0;          //所有阻抗值索引数目
    
    t_CANMSG msg;
    static u16 sIndex = 0;    //正在发送的索引
    
    //非大唐NXP前端或阻抗第一遍未采样完成
    if((AFE_TYPE_NXP != GetDaisyAFEType())
      || (1 != GetCellZSampFinFlagAPI()))
    {
        return;
    }
    
    //大唐NXP内阻和电压数目一样
    sumRNum = GetTotalVoltNum();
    
    //超过索引长度
    if(sIndex >= sumRNum)
    {
        sIndex = 0;
    }
    
    //前240节
    if(sIndex < 240)
    {
        //0x10=8<<1:发送主动更新信息帧
        //0x60=96:单体内阻信息主索引96/97
        msg.ID = 0x1060FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
        txIndex = sIndex;
        if(sumRNum > 240)
        {
            txRNum = 240;
        }
        else
        {
            txRNum = sumRNum;
        }
    }
    //超过240节
    else
    {
        //0x10=8<<1:发送主动更新信息帧
        //0x61=97:单体内阻信息主索引96/97
        msg.ID = 0x1061FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
        txIndex = sIndex - 240;
        txRNum = sumRNum - 240;
    }
    msg.Len = 8;
    
    //长帧第一帧
    if(0 == txIndex)
    {
        //长帧多帧起始帧
        if(txRNum > 3)
        {
            msg.Data[0] = 0x43;
        }
        //长帧仅单帧
        else
        {
            msg.Data[0] = 0xC0 | txRNum;
        }
    }
    //长帧多帧的最后一帧
    else if((txIndex + 3) >= txRNum)
    {
        msg.Data[0] = 0x80 | (txRNum - txIndex);
    }
    //长帧多帧的中间帧
    else
    {
        msg.Data[0] = 0x03;
    }
    
    //本帧发送起始索引
    msg.Data[1] = txIndex;

    //每帧发送3个数据
    for(i = 0; i < 3; i++)
    {
        if((i < (msg.Data[0] & 0x0f)) && ((txIndex + i) < txRNum))
        {
            msg.Data[i * 2 + 2] = gCellZm_96[sIndex + i] & 0xff;
            msg.Data[i * 2 + 3] = (gCellZm_96[sIndex + i] >> 8) & 0xff;
        }
        else
        {
            msg.Data[i * 2 + 2] = 0;
            msg.Data[i * 2 + 3] = 0;
        }
    }
    
    //下一帧后移3个索引
    sIndex += 3;
    
    //本次为最后一帧
    if(sIndex >= sumRNum)
    {
        sIndex = 0;
    }

    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}
//#endif

/*=================================================================================================
** @Name      : void BSU02U1SendUppMsgNowParaTask(void)
** @Input     : void
** @Output    : void
** @Function  : 周期上传参数信息帧任务:常规参数当前值
** @The notes : 1000ms周期,按参数帧格式上传
**===============================================================================================*/
void BSU02U1SendUppMsgNowParaTask(void)
{
    u8 i = 0;
    u8 index = 0;
	t_CANMSG msg;
    static u16 sIndex = 0;
    
    //上电前3s不发送
	if(BSPGetCosTimer() < 3000)
    {
        //上电后发送10次
        sParaTxTime = 10;
        return;
    }
    
    //需要时发送10s后停止
    if(0 == sParaTxTime)
    {
        return;
    }
    //非最后1次发送(确保发送到参数最后一帧)
    else if(sParaTxTime >= 2)
    {
        sParaTxTime--;
    }
    
    //帧序号转化为参数索引
    index = sIndex * 4;
    
    //超过索引长度
    if(index >= e102CfgDataFin)
    {
        index = 0;
        sIndex = 0;
    }
    
    //0x18=12<<1:发送参数信息帧
    //常规参数102帧ID索引从0x32=50开始
	msg.ID = 0x1800FF00 | (((u32)sIndex+0x32) << 16) | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
    //每帧发4个参数
    for(i = 0; i < 4; i++)
    {
        if((index + i) < e102CfgDataFin)
        {
            msg.Data[i*2] = gEepNorPara_102[index + i] & 0xff;
            msg.Data[i*2+1] = (gEepNorPara_102[index + i] >> 8) & 0xff;
        }
        else
        {
            msg.Data[i*2] = 0;
            msg.Data[i*2+1] = 0;
        }
    }
    
    //帧计数
    sIndex++;
    
    //本次为最后一帧
    if((sIndex * 4) >= e102CfgDataFin)
    {
        sIndex = 0;
        
        //最后1次发送
        if(1 == sParaTxTime)
        {
            //停止发送
            sParaTxTime = 0;
        }
    }
    
   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgDevInfo(void)
** @Input     : void
** @Output    : void
** @Function  : 读写应答回复:设备信息
** @The notes : 查询回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgDevInfo(void)
{
    t_CANMSG msg;
    
    //0x0c=6<<1,查询回复
    //0x01=1,查询索引1
    msg.ID = 0x0C01FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;

    msg.Data[0] = 0x43;     //长帧起始帧       第一包数据
    msg.Data[1] = 0x00;     //起始子索引

    msg.Data[2] = gDevInfo_1[e1Dev_ID] & 0xff;
    msg.Data[3] = (gDevInfo_1[e1Dev_ID] >> 8) & 0xff;

    msg.Data[4] = gDevInfo_1[e1Dev_ECU] & 0xff;
    msg.Data[5] = (gDevInfo_1[e1Dev_ECU] >> 8) & 0xff;

    msg.Data[6] = gDevInfo_1[e1Dev_HW] & 0xff;
    msg.Data[7] = (gDevInfo_1[e1Dev_HW] >> 8) & 0xff;

    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_ID] = %d\r\n",gDevInfo_1[e1Dev_ID]);
    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_ECU] = %d\r\n",gDevInfo_1[e1Dev_ECU]);
    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_HW] = %d\r\n",gDevInfo_1[e1Dev_HW]);
   
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
   
    msg.Len = 8;
   	msg.Data[0] = 0x03;     //长帧中间帧       第二包数据
    msg.Data[1] = 0x03;     //起始子索引

    msg.Data[2] = gDevInfo_1[e1Dev_SW_L] & 0xff;
    msg.Data[3] = (gDevInfo_1[e1Dev_SW_L] >> 8) & 0xff;

    msg.Data[4] = gDevInfo_1[e1Dev_SW_H] & 0xff;
    msg.Data[5] = (gDevInfo_1[e1Dev_SW_H] >> 8) & 0xff;

    msg.Data[6] = gDevInfo_1[e1Dev_BSP] & 0xff;
    msg.Data[7] = (gDevInfo_1[e1Dev_BSP] >> 8) & 0xff;

    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_SW_L] = %d\r\n",gDevInfo_1[e1Dev_SW_L]);
    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_SW_H] = %d\r\n",gDevInfo_1[e1Dev_SW_H]);
    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_BSP] = %d\r\n",gDevInfo_1[e1Dev_BSP]);

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
   
    msg.Len = 8;
   	msg.Data[0] = 0x82;     //长帧结束帧       第三包数据
    msg.Data[1] = 0x06;     //起始子索引

    msg.Data[2] = gDevInfo_1[e1Dev_CAN] & 0xff;
    msg.Data[3] = (gDevInfo_1[e1Dev_CAN] >> 8) & 0xff;

    msg.Data[4] = gDevInfo_1[e1Dev_UART] & 0xff;;
    msg.Data[5] = (gDevInfo_1[e1Dev_UART] >> 8) & 0xff;

    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;

    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_CAN] = %d\r\n",gDevInfo_1[e1Dev_CAN]);
    //SEGGER_RTT_printf(0,"gDevInfo_1[e1Dev_UART] = %d\r\n",gDevInfo_1[e1Dev_UART]);
   
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgSerial(u8 replyType)
** @Input     : replyType:应答类型[0:查询回复 1:修改回复]
** @Output    : void
** @Function  : 读写应答回复:设备序列号
** @The notes : 查询或修改回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgSerial(u8 replyType)
{
    t_CANMSG msg;
    
    if(replyType > 1)
    {
        return;
    }
    
    //修改回复
    if(1 == replyType)
    {
        //0x06=3<<1,修改回复
        //0x02=2,修改索引2
        msg.ID = 0x0602FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    }
    //查询回复
    else
    {
        //0x0c=6<<1,查询回复
        //0x020=2,查询索引2
        msg.ID = 0x0C02FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    }
    
    msg.Len = 8;

    msg.Data[0] = 0x46;     //长帧起始帧       第一包数据
    msg.Data[1] = 0x00;     //起始子索引

    msg.Data[2] = gCANSerialRdData_2[0] & 0xff;
    msg.Data[3] = gCANSerialRdData_2[1] & 0xff;

    msg.Data[4] = gCANSerialRdData_2[2] & 0xff;
    msg.Data[5] = gCANSerialRdData_2[3] & 0xff;

    msg.Data[6] = gCANSerialRdData_2[4] & 0xff;
    msg.Data[7] = gCANSerialRdData_2[5] & 0xff;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
   
   	msg.Data[0] = 0x06;     //长帧中间帧       第二包数据
    msg.Data[1] = 0x06;     //起始子索引

    msg.Data[2] = gCANSerialRdData_2[6] & 0xff;
    msg.Data[3] = gCANSerialRdData_2[7] & 0xff;

    msg.Data[4] = gCANSerialRdData_2[8] & 0xff;
    msg.Data[5] = gCANSerialRdData_2[9] & 0xff;

    msg.Data[6] = gCANSerialRdData_2[10] & 0xff;
    msg.Data[7] = gCANSerialRdData_2[11] & 0xff;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
   
   	msg.Data[0] = 0x06;     //长帧中间帧       第三包数据
    msg.Data[1] = 0x0C;     //起始子索引

    msg.Data[2] = gCANSerialRdData_2[12] & 0xff;
    msg.Data[3] = gCANSerialRdData_2[13] & 0xff;

    msg.Data[4] = gCANSerialRdData_2[14] & 0xff;
    msg.Data[5] = gCANSerialRdData_2[15] & 0xff;

    msg.Data[6] = gCANSerialRdData_2[16] & 0xff;
    msg.Data[7] = gCANSerialRdData_2[17] & 0xff;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
   
   	msg.Data[0] = 0x85;     //长帧结束帧       第四包数据
    msg.Data[1] = 0x12;     //起始子索引

    msg.Data[2] = gCANSerialRdData_2[18] & 0xff;;
    msg.Data[3] = gCANSerialRdData_2[19] & 0xff;

    msg.Data[4] = gCANSerialRdData_2[20] & 0xff;;
    msg.Data[5] = gCANSerialRdData_2[21] & 0xff;

    msg.Data[6] = gCANSerialRdData_2[22] & 0xff;;
    msg.Data[7] = 0x00;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgWorkMode(u8 replyType)
** @Input     : replyType:应答类型[0:查询回复 1:修改回复]
** @Output    : void
** @Function  : 读写应答回复:系统工作模式
** @The notes : 查询或修改回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgWorkMode(u8 replyType)
{
	t_CANMSG msg;
	
	if(replyType > 1)
    {
        return;
    }
    
    //修改回复
	if(1 == replyType)
    {
        //0x06=3<<1,修改回复
        //0x55=85,修改索引85
		msg.ID = 0x0655FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    }
    //查询回复
	else
    {
        //0x0c=6<<1,查询回复
        //0x30=50,查询索引50
	    msg.ID = 0x0C32FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    }

    msg.Len = 8;
	
	msg.Data[0] = 0xc1;     //长帧仅单帧
    msg.Data[1] = 0x00;     //起始子索引

    msg.Data[2] = gModeState_50[e50Mode_Work] & 0xff;
    msg.Data[3] = (gModeState_50[e50Mode_Work] >> 8) & 0xff;

    msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;

    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;

   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgMainChara(void)
** @Input     : void
** @Output    : void
** @Function  : 读写应答回复:整体特征信息
** @The notes : 查询回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgMainChara(void)
{
	t_CANMSG msg;
	
    //单节电压温度第一遍未采样完成
	if((1 != GetCellVSampFinFlagAPI())
       || (1 != GetCellTSampFinFlagAPI()))
    {
        //return;
    }
	
    //PT1000未自检完成
	if(2 != GetPtOrWatCheckSelfFlagAPI(eCheckSelfType_PT1000))
	{
		//return;
	}
    
    //0x0c=6<<1,查询回复
    //0x3E=62,查询索引62
	msg.ID = 0x0C3EFF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
    
	msg.Data[0] = 0x43;     //长帧起始帧       第一包数据
    msg.Data[1] = 0x00;     //起始子索引
	
	msg.Data[2] = gCellBState_62[e62BCell_TotalV] & 0xff;   //总电压
   	msg.Data[3] = (gCellBState_62[e62BCell_TotalV] >> 8) & 0xff;
	
	msg.Data[4] = gCellBState_62[e62BCell_AvgV] & 0xff;     //平均电压
   	msg.Data[5] = (gCellBState_62[e62BCell_AvgV] >> 8) & 0xff;
	
	msg.Data[6] = gCellBState_62[e62BCell_AvgT] & 0xff;     //平均温度
   	msg.Data[7] = (gCellBState_62[e62BCell_AvgT] >> 8) & 0xff;
	
	(void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
	
	msg.Data[0] = 0x81;     //长帧结束帧        第二包数据
    msg.Data[1] = 0x03;     //起始子索引
		
	msg.Data[2] = gCellBState_62[e62BCell_EnvT] & 0xFF;     //环境温度
    msg.Data[3] = (gCellBState_62[e62BCell_EnvT] >> 8) & 0xFF;

	msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;
		
    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;
	
   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgVoltChara(void)
** @Input     : void
** @Output    : void
** @Function  : 读写应答回复:单体电压特征信息
** @The notes : 查询回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgVoltChara(void)
{
	t_CANMSG msg;

	//单节电压第一遍未采样完成
	if(1 != GetCellVSampFinFlagAPI())
    {
        //return;
    }
    
    //0x0c=6<<1,查询回复
    //0x3F=63,查询索引63
	msg.ID = 0x0C3FFF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];

    msg.Len = 8;
	
	msg.Data[0] = 0x43;     //长帧起始帧
    msg.Data[1] = 0x00;     //起始子索引
	
	msg.Data[2] = gVolInfo_63[e63Vol_MaxNum] & 0xff;        //最高电压电池号
   	msg.Data[3] = (gVolInfo_63[e63Vol_MaxNum] >> 8) & 0xff;
	
	msg.Data[4] = gVolInfo_63[e63Vol_MaxV] & 0xff;          //最高电压值
   	msg.Data[5] = (gVolInfo_63[e63Vol_MaxV] >> 8) & 0xff;
	
	msg.Data[6] = gVolInfo_63[e63Vol_MinNum] & 0xff;        //最低电压电池号
   	msg.Data[7] = (gVolInfo_63[e63Vol_MinNum] >> 8) & 0xff;
		
	(void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
	
	msg.Data[0] = 0x81;     //长帧结束帧
    msg.Data[1] = 0x03;     //起始子索引
		
	msg.Data[2] = gVolInfo_63[e63Vol_MinV] & 0xFF;          //最低电压值
    msg.Data[3] = (gVolInfo_63[e63Vol_MinV] >> 8) & 0xFF;

	msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;
		
    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;
	
   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgTempChara(void)
** @Input     : void
** @Output    : void
** @Function  : 读写应答回复:单体温度特征信息
** @The notes : 查询回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgTempChara(void)
{
	t_CANMSG msg;

	//单节温度第一遍未采样完成
	if(1 != GetCellTSampFinFlagAPI())
    {
        //return;
    }
    
    //0x0c=6<<1,查询回复
    //0x40=64,查询索引64
	msg.ID = 0x0C40FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
		
	msg.Len = 8;
	msg.Data[0] = 0x43;     //长帧起始帧
    msg.Data[1] = 0x00;     //起始子索引
	
	msg.Data[2] = gTemInfo_64[e64Tem_MaxNum] & 0xff;        //最高温度电池号
   	msg.Data[3] = (gTemInfo_64[e64Tem_MaxNum] >> 8) & 0xff;
	
	msg.Data[4] = gTemInfo_64[e64Tem_MaxT] & 0xff;          //最高温度值
   	msg.Data[5] = (gTemInfo_64[e64Tem_MaxT] >> 8) & 0xff;
	
	msg.Data[6] = gTemInfo_64[e64Tem_MinNum] & 0xff;        //最低温度电池号
   	msg.Data[7] = (gTemInfo_64[e64Tem_MinNum] >> 8) & 0xff;
    
    (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送

	msg.Data[0] = 0x81;     //长帧结束帧
    msg.Data[1] = 0x03;     //起始子索引
	
	msg.Data[2] = gTemInfo_64[e64Tem_MinT] & 0xFF;          //最低温度值
    msg.Data[3] = (gTemInfo_64[e64Tem_MinT] >> 8) & 0xFF;

	msg.Data[4] = 0x00;
    msg.Data[5] = 0x00;
	
    msg.Data[6] = 0x00;
    msg.Data[7] = 0x00;
   (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
}

/*=================================================================================================
** @Name      : void BSU02U1SendUpReplyMsgParaCfg(u8 replyType, u8 sindex, u8 len)
** @Input     : replyType:应答类型[0:查询回复 1:修改回复] sindex:参数起始索引 len:参数个数
** @Output    : void
** @Function  : 读写应答回复:常规参数当前值
** @The notes : 查询或修改回复
**===============================================================================================*/
void BSU02U1SendUpReplyMsgParaCfg(u8 replyType, u8 sindex, u8 len)
{
    u8 i = 0;
	t_CANMSG msg;
    u8 offIndex = 0;
    
	if((replyType > 1) || ((sindex + len) > e102CfgDataFin))
	{
		return;
	}
    
    //修改回复
	if(1 == replyType)
    {
        //0x06=3<<1,修改回复
        //0x6b=107,修改索引107
		//msg.ID = 0x066BFF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
      
        //通过定周期上传回复(连续收发CAN可能异常)
        sParaTxTime = 10;
        return;
    }
    //查询回复
	else
    {
        //0x0c=6<<1,查询回复
        //0x66=102,查询索引102
	    msg.ID = 0x0C66FF00 | (u32)gEepNorPara_102[e102Dev_Ecuid];
    }
    
    msg.Len = 8;

    //所有参数
    if(0 == len)
    {
        sindex = 0;
        len = e102CfgDataFin;
    }
    
    //参数个数计算能发的帧数
    for(i = 0; i < ((len + 2) / 3); i++)
    {
        //偏移子索引数
        offIndex = i * 3;
      
        //长帧第一帧
        if(0 == i)
        {
            //长帧多帧起始帧
            if(len > 3)
            {
                msg.Data[0] = 0x43;
            }
            //长帧仅单帧
            else
            {
                msg.Data[0] = 0xC0 | len;
            }
        }
        //长帧多帧的最后一帧
        else if((offIndex + 3) >= len)
        {
            msg.Data[0] = 0x80 | (len - offIndex);
        }
        //长帧多帧的中间帧
        else
        {
            msg.Data[0] = 0x03;
        }
      
        //起始子索引
        msg.Data[1] = sindex + offIndex;
        
        //发送第一个参数
        msg.Data[2] = gEepNorPara_102[sindex + offIndex] & 0xff;
        msg.Data[3] = (gEepNorPara_102[sindex + offIndex] >> 8) & 0xff;

        //下一个参数索引未超查询长度
        if((offIndex + 1) < len)
        {
            msg.Data[4] = gEepNorPara_102[sindex + offIndex + 1] & 0xff;
            msg.Data[5] = (gEepNorPara_102[sindex + offIndex + 1] >> 8) & 0xff;
        }
        else
        {
            msg.Data[4] = 0x00;
            msg.Data[5] = 0x00;
        }

        //下一个参数索引未超查询长度
        if((offIndex + 2) < len)
        {
            msg.Data[6] = gEepNorPara_102[sindex + offIndex + 2] & 0xff;
            msg.Data[7] = (gEepNorPara_102[sindex + offIndex + 2] >> 8) & 0xff;
        }
        else
        {
            msg.Data[6] = 0x00;
            msg.Data[7] = 0x00; 
        }

        (void)BSPCANSendQin(eCAN0, &msg);		//向CAN0通道发送
    }
}

/*=================================================================================================
** @Static function definition
**===============================================================================================*/
