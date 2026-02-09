/*==========================================Copyright(c)===========================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information------------------------------------------
** @File Name    : EventUser.c
** @Founder      : SBK326
** @Date         : 2019.11.1
** @Function     : 电池事件记录用户处理模块
** @Instructions : 
**===============================================================================================*/

/*=================================================================================================
** @Header file definition
**===============================================================================================*/
#include "BSPTypeDef.h"
#include "BSPUserAPI.h"
#include "PublicInfo.h"
#include "GroupInfo.h"
#include "GroupMsg.h"
#include "RecordInterface.h"
#include "EventUser.h"

/*=================================================================================================
** @Global variable definition
**===============================================================================================*/
extern u16 gCANVerTime_0[];                                 /*校时信息*/

extern u16 gGHisEventInfo_46[];	                            /*历史事件记录信息查询数组*/
extern u16 gGUserCtrl_87[];	    	                        /*用户控制命令信息数组*/

/*=================================================================================================
** @Static function declaration
**===============================================================================================*/

/*=================================================================================================
** @Interface function definition
**===============================================================================================*/
/*=================================================================================================
** @Name      : void EventRcdDisplayInit(void)
** @Input     : void
** @Output    : void
** @Function  : 事件记录显示信息初始化
** @The notes : 读取初始显示的历史事件,用户任务单次调用(避免占用太长系统初始化时间)
**===============================================================================================*/
void EventRcdDisplayInit(void)
{
    t_RcdStoreInfo info = {0};

	//读一条事件记录到查询显示区
	if(TRUE == RcdDisplayOneEventRcdUserAPI(0, &info))
	{
		//事件主要信息
		gGHisEventInfo_46[0] = info.codeH & 0x00ff;
		gGHisEventInfo_46[1] = info.codeL;
		gGHisEventInfo_46[2] = info.year;
		gGHisEventInfo_46[3] = info.MD;
		gGHisEventInfo_46[4] = info.HM;
		gGHisEventInfo_46[5] = info.sec;

		//事件附件信息
		gGHisEventInfo_46[6] = info.info1;
		gGHisEventInfo_46[7] = info.info2;
		gGHisEventInfo_46[8] = info.info3;
		gGHisEventInfo_46[9] = info.info4;
		gGHisEventInfo_46[10] = info.info5;
	}
}

/*=================================================================================================
** @Name      : void EventRecordUserTask(void)
** @Input     : void
** @Output    : void
** @Function  : 事件记录用户任务(定周期检查变化事件)
** @The notes : 100ms周期
**===============================================================================================*/
void EventRecordUserTask(void)
{
	static u8 sFlag = 0;
	static u8 sHisProState = 0;
	static u8 sHisChgState = 0;
	static u8 sHisModeState = 0;
	static u8 sHisWorkState = 0;
	static u16 sHisInPorState = 0;
	static u16 sHisOutPorState = 0;

	/*记录显示未初始化*/
    if(0 == sFlag)
	{
    	/*记录存储模块未初始化*/
        if(0 == RcdGetEventRcdEnStateAPI())
    	{
        	/*记录存储读写指针初始化*/
        	RcdStoreStateInit();
    	}
        else
        {
        	/*记录显示初始化*/
        	EventRcdDisplayInit();
        	sFlag = 1;
        }
    	return;
	}

	/*非上电初始化状态*/
	if(eWORK_INIT != GetGWorkStateAPI())
	{
		/*输入IO口变化*/
		if(sHisInPorState != GetGInPortStateAPI())
		{
			/*产生事件*/
			EventRcdDisplayUserAPI(eEVN_RCDSORT_INPUT, 0, GetGSampOutCurrAPI(), sHisInPorState, GetGInPortStateAPI(), GetGWorkModeAPI());
		}

		/*输出IO口变化*/
		if(sHisOutPorState != (GetGOutPortStateAPI() & EVN_RCD_OUTIO_BIT))
		{
			/*产生事件*/
			EventRcdDisplayUserAPI(eEVN_RCDSORT_OUTPUT, 0, GetGSampOutCurrAPI(), sHisOutPorState, GetGOutPortStateAPI(), GetGWorkModeAPI());
		}
	}

	/*保护状态变化*/
	if(sHisProState != GetGProtectStateAPI())
	{
		/*产生事件*/
		EventRcdDisplayUserAPI(eEVN_RCDSORT_STATE, 0, GetGSampOutCurrAPI(), sHisProState, GetGSampSumVoltAPI(), GetGSampPreVoltAPI());
	}

	/*工作状态变化*/
	if(sHisWorkState != GetGWorkStateAPI())
	{
		/*产生事件*/
		EventRcdDisplayUserAPI(eEVN_RCDSORT_STATE, 1, GetGSampOutCurrAPI(), sHisWorkState, GetGSampSumVoltAPI(), GetGSampPreVoltAPI());
	}

	/*充放电状态变化*/
	if(sHisChgState != GetGChgDhgStateAPI())
	{
		/*产生事件*/
		EventRcdDisplayUserAPI(eEVN_RCDSORT_STATE, 2, GetGSampOutCurrAPI(), sHisChgState, GetGChgDhgStateAPI(), GetGSampPreVoltAPI());
	}

	/*工作模式状态变化*/
	if(sHisModeState != GetGWorkModeAPI())
	{
		/*产生事件*/
		EventRcdDisplayUserAPI(eEVN_RCDSORT_STATE, 3, GetGSampOutCurrAPI(), sHisModeState, GetGWorkModeAPI(), GetGSampPreVoltAPI());
	}

	sHisProState = GetGProtectStateAPI();
	sHisWorkState = GetGWorkStateAPI();
	sHisChgState = GetGChgDhgStateAPI();
	sHisModeState = GetGWorkModeAPI();
	sHisInPorState = GetGInPortStateAPI();
	sHisOutPorState = GetGOutPortStateAPI() & EVN_RCD_OUTIO_BIT;
}

/*=================================================================================================
** @Name      : void EventRcdDisplayUserAPI(eEvent_RcdSort sort, u8 id, u16 info2, u16 info3, u16 info4, u16 info5)
** @Input     : sort:事件类型 id:事件码ID info2-5:附加信息2-5
** @Output    : void
** @Function  : 事件记录与显示用户接口
** @The notes : 用户需产生事件的地方调用,事件显示到显示区并记录存储
**===============================================================================================*/
void EventRcdDisplayUserAPI(eEvent_RcdSort sort, u8 id, u16 info2, u16 info3, u16 info4, u16 info5)
{
	t_SysClock sysClock = {0};
    t_RcdCreateInfo rcdInfo = {0};
    static u16 sHisInfo[6] = {0};

    /*不记录连续重复的事件*/
    if((sort == sHisInfo[0]) && (id == sHisInfo[1]) && (info2 == sHisInfo[2])
    	&& (info3 == sHisInfo[3]) && (info4 == sHisInfo[4]) && (info5 == sHisInfo[5]))
    {
    	return;
    }

    /*更新上次记录信息*/
    sHisInfo[0] = sort;
    sHisInfo[1] = id;
    sHisInfo[2] = info2;
    sHisInfo[3] = info3;
    sHisInfo[4] = info4;
    sHisInfo[5] = info5;

    /*正在显示当天事件信息才更新显示内容*/
    if(0 == gGUserCtrl_87[eUserCtrl87_HisEvnSw])
    {
		//记录新历史事件记录
		//记录类型
		gGHisEventInfo_46[0] = sort;

		//输入IO类型
		if(eEVN_RCDSORT_INPUT == sort)
		{
			gGHisEventInfo_46[1] = EVN_RCD_INIOCODE_FST + id;     	//事件记录码
		}
		//输出IO类型
		else if(eEVN_RCDSORT_OUTPUT == sort)
		{
			gGHisEventInfo_46[1] = EVN_RCD_OUTIOCODE_FST + id;     	//事件记录码
		}
		//状态类型
		else if(eEVN_RCDSORT_STATE == sort)
		{
			gGHisEventInfo_46[1] = EVN_RCD_STATECODE_FST + id;     	//事件记录码
		}
		//控制命令类型
		else if(eEVN_RCDSORT_CMD == sort)
		{
			gGHisEventInfo_46[1] = EVN_RCD_CMDCODE_FST + id;    	//事件记录码
		}
		//调试命令类型
		else if(eEVN_RCDSORT_DEBUG == sort)
		{
			gGHisEventInfo_46[1] = EVN_RCD_DBGCODE_FST + id;    	//事件记录码
		}
		//参数配置类型
		else if(eEVN_RCDSORT_CFG == sort)
		{
			gGHisEventInfo_46[1] = EVN_RCD_CFGCODE_FST + id;    	//事件记录码
		}
		//用户自定义类型
		else
		{
			gGHisEventInfo_46[1] = id;                          	//事件记录码
		}

		/*记录时间信息*/
		sysClock = GetSysDateClockAPI();

		gGHisEventInfo_46[2] = sysClock.year;                                 //记录时间-年
		gGHisEventInfo_46[3] = ((u16)sysClock.month << 8) | sysClock.day;     //记录时间-月日
		gGHisEventInfo_46[4] = ((u16)sysClock.hour << 8) | sysClock.minute;   //记录时间-时分
		gGHisEventInfo_46[5] = sysClock.second;                               //记录时间-秒

		//事件附件信息
		gGHisEventInfo_46[6] = ((u16)GetGProtectStateAPI() << 8) | (GetGWorkStateAPI() & 0x00ff);
		gGHisEventInfo_46[7] = info2;
		gGHisEventInfo_46[8] = info3;
		gGHisEventInfo_46[9] = info4;
		gGHisEventInfo_46[10] = info5;

		/*触发发送历史事件信息*/
		SendCanGroupHisEventInfoMsg();
    }

    /*取到相应事件记录附加信息*/
	rcdInfo.type = sort;

    /*附加信息1:当前保护状态和工作状态*/
	rcdInfo.info1 = ((u16)GetGProtectStateAPI() << 8) | (GetGWorkStateAPI() & 0x00ff);

    /*附加信息2-5:用户定义*/
	rcdInfo.info2 = info2;
	rcdInfo.info3 = info3;
	rcdInfo.info4 = info4;
	rcdInfo.info5 = info5;

	//输入IO类型
	if(eEVN_RCDSORT_INPUT == sort)
	{
		rcdInfo.code = EVN_RCD_INIOCODE_FST + id;     	//事件记录码
	}
	//输出IO类型
	else if(eEVN_RCDSORT_OUTPUT == sort)
	{
		rcdInfo.code = EVN_RCD_OUTIOCODE_FST + id;     	//事件记录码
	}
	//状态类型
	else if(eEVN_RCDSORT_STATE == sort)
	{
		rcdInfo.code = EVN_RCD_STATECODE_FST + id;     	//事件记录码
	}
	//控制命令类型
	else if(eEVN_RCDSORT_CMD == sort)
	{
		rcdInfo.code = EVN_RCD_CMDCODE_FST + id;    	//事件记录码
	}
	//调试命令类型
	else if(eEVN_RCDSORT_DEBUG == sort)
	{
		rcdInfo.code = EVN_RCD_DBGCODE_FST + id;    	//事件记录码
	}
	//参数配置类型
	else if(eEVN_RCDSORT_CFG == sort)
	{
		rcdInfo.code = EVN_RCD_CFGCODE_FST + id;    	//事件记录码
	}
	//用户自定义类型
	else
	{
		rcdInfo.code = id;                              //事件记录码
	}

	//事件记录
	RcdCreateEventRcdInfoAPI(&rcdInfo);
}

/*=================================================================================================
** @Name      : void EventChangHisRcdDisplayAPI(u8 date, u8 home)
** @Input     : date:历史事件日期[0,30]:0-30天前记录 home:读首页标志 0:读当前记录 1:读当日首页记录
** @Output    : void
** @Function  : 切换历史事件显示信息
** @The notes : 按照日期读取指定时间的历史事件显示(读30条)
**===============================================================================================*/
void EventChangHisRcdDisplayAPI(u8 date, u8 home)
{
    static u8 sHisDate = 0;
    t_RcdStoreInfo info = {0};

    /*不切换日期且都首页记录则跳转切换首页*/
    if((sHisDate == date) && (1 == home))
    {
    	/*切换首页失败*/
    	if(FALSE == RcdJumpDispHomeEventRcdUserAPI())
    	{
    		/*不更新显示*/
    		return;
    	}
    }

	/*读取一条记录成功*/
	if(TRUE == RcdDisplayOneEventRcdUserAPI(date, &info))
	{
		//事件主要信息
		gGHisEventInfo_46[0] = info.codeH & 0x00ff;
		gGHisEventInfo_46[1] = info.codeL;
		gGHisEventInfo_46[2] = info.year;
		gGHisEventInfo_46[3] = info.MD;
		gGHisEventInfo_46[4] = info.HM;
		gGHisEventInfo_46[5] = info.sec;

		//事件附件信息
		gGHisEventInfo_46[6] = info.info1;
		gGHisEventInfo_46[7] = info.info2;
		gGHisEventInfo_46[8] = info.info3;
		gGHisEventInfo_46[9] = info.info4;
		gGHisEventInfo_46[10] = info.info5;
	}
	/*读取已失败*/
	else
	{
		/*切换日期未读取到记录*/
		if(sHisDate != date)
		{
			/*事件记录查询显示区清零*/
			gGHisEventInfo_46[0] = 0;
			gGHisEventInfo_46[1] = 0;
			gGHisEventInfo_46[2] = 0;
			gGHisEventInfo_46[3] = 0;
			gGHisEventInfo_46[4] = 0;
			gGHisEventInfo_46[5] = 0;

			gGHisEventInfo_46[6] = 0;
			gGHisEventInfo_46[7] = 0;
			gGHisEventInfo_46[8] = 0;
			gGHisEventInfo_46[9] = 0;
			gGHisEventInfo_46[10] = 0;
		}
	}
    sHisDate = date;

	/*触发发送历史事件查询显示信息*/
	SendCanGroupHisEventInfoMsg();
}

/*=================================================================================================
** @Name      : void EventClrHisRcdDisplayAPI(void)
** @Input     : void
** @Output    : void
** @Function  : 清除换历史事件显示信息
** @The notes : 显示区域清零
**===============================================================================================*/
void EventClrHisRcdDisplayAPI(void)
{
	/*事件记录查询显示区清零*/
	gGHisEventInfo_46[0] = 0;
	gGHisEventInfo_46[1] = 0;
	gGHisEventInfo_46[2] = 0;
	gGHisEventInfo_46[3] = 0;
	gGHisEventInfo_46[4] = 0;
	gGHisEventInfo_46[5] = 0;

	gGHisEventInfo_46[6] = 0;
	gGHisEventInfo_46[7] = 0;
	gGHisEventInfo_46[8] = 0;
	gGHisEventInfo_46[9] = 0;
	gGHisEventInfo_46[10] = 0;

	/*触发发送历史事件查询显示信息*/
	SendCanGroupHisEventInfoMsg();
}

/*=================================================================================================
** @Static function definition
**===============================================================================================*/
