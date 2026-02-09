/*==========================================Copyright(c)===========================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information------------------------------------------
** @File Name    : GroupMsg.c
** @Founder      : SBK326
** @Date         : 2019.11.1
** @Function     : 电池组主控CAN报文处理模块
** @Instructions : 
**===============================================================================================*/

/*=================================================================================================
** @Header file definition
**===============================================================================================*/
#include "BSPTypeDef.h"
#include "BSPUserAPI.h"
#include "PublicPara.h"
#include "GroupPara.h"
#include "PublicInfo.h"
#include "GroupInfo.h"
#include "Authority.h"
#include "ParaUser.h"
#include "ParaInterface.h"
#include "RcdDisplayUser.h"
#include "CANInterface.h"
#include "TacUser.h"
#include "CanUser.h"
#include "GroupMsg.h"

/*=================================================================================================
** @Global variable definition
**===============================================================================================*/

/*=================================================================================================
** @Static function declaration
**===============================================================================================*/

/*=================================================================================================
** @Interface function definition
**===============================================================================================*/
/*=================================================================================================
** @Name      : void RcvCanDeviceSerialCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组写设备序列号命令CAN报文处理
** @The notes : CAN通信包接收请求修改(2)处理调用
**===============================================================================================*/
void RcvCanDeviceSerialCfgDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_Admin != GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > CANINDEX2_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
    	RcvDeviceSerialParaCfgAPI(i, eAuthP_Can0 + canNum);
    }
}

/*=================================================================================================
** @Name      : void RcvCanDnloadParaCmdDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组参数批量配置读写命令CAN报文处理
** @The notes : CAN通信包接收请求修改(22)处理调用
**===============================================================================================*/
void RcvCanDnloadParaCmdDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_Admin != GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > CANINDEX22_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
    	switch(i)
		{
			//切换参数配置状态
			case 0:
				ParaChangDownloadState();
				break;

			//参数读写命令
			case 1:
				/*执行参数读写命令*/
				ParaDownloadRWCmdHandle();
				break;

			default:
				break;
		}
    }
}

/*=================================================================================================
** @Name      : void RcvCanDnloadParaWRDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组参数批量配置写数据CAN报文处理
** @The notes : CAN通信包接收请求修改(23)处理调用
**===============================================================================================*/
void RcvCanDnloadParaWRDeal(u8 index, u8 len, u8 canNum)
{
    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_Admin != GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > CANINDEX23_LEN) || (0 == len))
    {
        return;
    }

    /*接收到写确认命令时只调用一次即可*/
    ParaDownloadWriteParaHandle();
}

/*=================================================================================================
** @Name      : void RcvCanErrRcdReadInfoDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收查询故障记录详细信息读取CAN报文处理
** @The notes : CAN通信包接收请求查询(31)处理调用
**===============================================================================================*/
void RcvCanErrRcdReadInfoDeal(u8 index, u8 len, u8 canNum)
{
    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > CANINDEX31_LEN) || (0 == len))
    {
        return;
    }

    /*读一条记录到显示区*/
    (void)ErrRcdUpdateReadInfoAPI();
}

/*=================================================================================================
** @Name      : void RcvCanEventRcdReadInfoDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收查询事件记录详细信息读取CAN报文处理
** @The notes : CAN通信包接收请求查询(32)处理调用
**===============================================================================================*/
void RcvCanEventRcdReadInfoDeal(u8 index, u8 len, u8 canNum)
{
    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > CANINDEX32_LEN) || (0 == len))
    {
        return;
    }

    /*读一条记录到显示区*/
    (void)EventRcdUpdateReadInfoAPI();
}

/*=================================================================================================
** @Name      : void RcvCanDataRcdDisplayInfoDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收查询数据记录详细信息显示CAN报文处理
** @The notes : CAN通信包接收请求查询(37)处理调用
**===============================================================================================*/
void RcvCanDataRcdDisplayInfoDeal(u8 index, u8 len, u8 canNum)
{
    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > CANINDEX37_LEN) || (0 == len))
    {
        return;
    }

    /*读一条记录到显示区*/
    (void)DataRcdUpdateDisplayAPI();
}

/*=================================================================================================
** @Name      : void RcvCanHisRecordRWDateDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收历史记录读写日期处理
** @The notes : CAN通信包接收请求修改(39)处理调用
**===============================================================================================*/
void RcvCanHisRecordRWDateDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*设置命令索引错误*/
    if(((index + len) > CANINDEX39_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        switch(i)
        {
            //设置读取数据记录日期
            case 0:
            	/*CAN口运维权限未开启*/
                if((canNum >= eCANNUM)
                    || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
            	{
            		return;
            	}

            	/*记录下发日期*/
            	DataRcdSetReadDateAPI();
				break;

            //设置清除数据记录日期
            case 1:
            	/*CAN口高级权限未开启*/
                if((canNum >= eCANNUM)
                    || (eAuthL_Service > GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
            	{
            		return;
            	}

            	/*清除指定当天数据记录*/
            	DataRcdClrDateInfoAPI();
                break;

			//设置清除故障记录日期
			case 2:
				/*CAN口高级权限未开启*/
				if((canNum >= eCANNUM)
					|| (eAuthL_Service > GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
				{
					return;
				}

				/*清除指定当天故障记录*/
				ErrRcdClrDateInfoAPI();
				break;

			//设置清除事件记录日期
			case 3:
				/*CAN口高级权限未开启*/
				if((canNum >= eCANNUM)
					|| (eAuthL_Service > GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
				{
					return;
				}

				/*清除指定当天事件记录*/
				EventRcdClrDateInfoAPI();
				break;

			//设置清除故障录波日期
			case 4:
				/*CAN口高级权限未开启*/
				if((canNum >= eCANNUM)
					|| (eAuthL_Service > GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
				{
					return;
				}

				/*清除指定当天事件记录*/
				WaveRcdClrDateInfoAPI();
				break;

			//设置清除DOD记录命令
			case 5:
				/*CAN口高级权限未开启*/
				if((canNum >= eCANNUM)
					|| (eAuthL_Service > GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
				{
					return;
				}

				/*清除全部DOD记录*/
				DodRcdClrAllInfoAPI();
				break;

            default:
                break;
        }
    }
}

/*=================================================================================================
** @Name      : void RcvCanGroupPasswordDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组权限密码命令CAN报文处理
** @The notes : CAN通信包接收请求修改(85)处理调用
**===============================================================================================*/
void RcvCanGroupPasswordDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*控制命令索引错误*/
    if(((index + len) > GINDEX85_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        switch(i)
        {
            //权限密码高位
            case 0:
				break;

            //权限密码低位
            case 1:
            	/*执行密码检测*/
            	RcvSysAuthorityCmdAPI(eAuthP_Can0 + canNum);
            	SendCanGroupAuthorityInfoMsg();
                break;

            default:
                break;
        }
    }
}

/*=================================================================================================
** @Name      : void RcvCanGroupCtrlCmdDeal(u8 index, u8 len)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组控制命令CAN报文处理
** @The notes : CAN通信包接收请求修改(86)处理调用
**===============================================================================================*/
void RcvCanGroupCtrlCmdDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口控制权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > GINDEX86_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        switch(i)
        {
            //工作状态控制
            case eSysCmdCtrl86_State:
            	RcvGroupWorkStateCmdAPI(eAuthP_Can0 + canNum);
                break;

            //工作模式切换
            case eSysCmdCtrl86_Mode:
            	RcvGroupWorkModeCmdAPI(eAuthP_Can0 + canNum);
                break;

            //系统清除命令
            case eSysCmdCtrl86_ClrCmd:
            	RcvGroupSysClrCmdAPI(eAuthP_Can0 + canNum);
                break;

            default:
                break;
        }
    }
}

/*=================================================================================================
** @Name      : void RcvCanUserCtrlCmdDeal(u8 index, u8 len)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收用户控制命令CAN报文处理
** @The notes : CAN通信包接收请求修改(87)处理调用
**===============================================================================================*/
void RcvCanUserCtrlCmdDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*参数错误*/
    if(canNum >= eCANNUM)
    {
    	return;
    }

    /*控制命令索引错误*/
    if(((index + len) > GINDEX87_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        switch(i)
        {
            //单体信息触发
            case eUserCtrl87_CellTrig:
            	RcvGroupTrigInfoCmdAPI(eAuthP_Can0 + canNum);
                break;

            //内部报文转发命令
            case eUserCtrl87_InTran:
            	RcvGroupTransInfoCmdAPI(eAuthP_Can0 + canNum);
                break;

            //历史故障切换命令
            case eUserCtrl87_HisErrSw:
            	RcvGroupDispHisErrCmdAPI(eAuthP_Can0 + canNum);
                break;

			//历史事件切换命令
			case eUserCtrl87_HisEvnSw:
				RcvGroupDispHisEventCmdAPI(eAuthP_Can0 + canNum);
                break;

			//故障录波切换命令
			case eUserCtrl87_HisWavSw:
				RcvGroupDispHisWaveRcdCmdAPI(eAuthP_Can0 + canNum);
				break;

			//DOD记录换命令
			case eUserCtrl87_HisDodSw:
				RcvGroupDispHisDodCmdAPI(eAuthP_Can0 + canNum);
				break;

            default:
                break;
        }
    }
}

/*=================================================================================================
** @Name      : void RcvCanGroupDebugCmdDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组调试命令CAN报文处理
** @The notes : CAN通信包接收请求修改(88)处理调用
**===============================================================================================*/
void RcvCanGroupDebugCmdDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口调试权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_Admin != GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*调试命令索引错误*/
    if(((index + len) > GINDEX88_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
    	//调试命令设置
        if(i < eDbgCmd88_Affirm)
        {
            RcvGroupDebugOperCmdAPI(i);
        }
        //调试确认
        else if(i == eDbgCmd88_Affirm)
        {
            RcvGroupDebugAffirmCmdAPI(eAuthP_Can0 + canNum);
        }
    }
}

/*=================================================================================================
** @Name      : void RcvCanSystemGenParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收系统通用参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(105)处理调用
**===============================================================================================*/
void RcvCanSystemGenParaCfgDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口调试权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*设置命令索引错误*/
    if(((index + len) > SINDEX105_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        /*写入参数处理*/
        RcvSystemGenParaCfgAPI(i, eAuthP_Can0 + canNum);
    }
}

/*=================================================================================================
** @Name      : void RcvCanSystemHWParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收系统硬件参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(106)处理调用
**===============================================================================================*/
void RcvCanSystemHWParaCfgDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口调试权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_Admin != GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*设置命令索引错误*/
    if(((index + len) > SINDEX106_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        /*写入参数处理*/
        RcvSystemHWParaCfgAPI(i, eAuthP_Can0 + canNum);
    }
}

/*=================================================================================================
** @Name      : void RcvCanGroupGenParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控通用参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(107)处理调用
**===============================================================================================*/
void RcvCanGroupGenParaCfgDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口调试权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*设置命令索引错误*/
    if(((index + len) > GINDEX107_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        /*写入参数处理*/
        RcvGroupGenParaCfgAPI(i, eAuthP_Can0 + canNum);
    }
}

/*=================================================================================================
** @Name      : void RcvCanGroupHigParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控高级参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(108)处理调用
**===============================================================================================*/
void RcvCanGroupHigParaCfgDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口调试权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_User == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum)))
    	|| (eAuthL_Operater == GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*设置命令索引错误*/
    if(((index + len) > GINDEX108_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        /*写入参数处理*/
        RcvGroupHigParaCfgAPI(i, eAuthP_Can0 + canNum);
    }
}

/*=================================================================================================
** @Name      : void RcvCanGroupHWParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控高级参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(109)处理调用
**===============================================================================================*/
void RcvCanGroupHWParaCfgDeal(u8 index, u8 len, u8 canNum)
{
    u8 i = 0;

    /*无CAN口调试权限*/
    if((canNum >= eCANNUM)
        || (eAuthL_Admin != GetSysNowAuthorityAPI((e_AuthOpenPort)(eAuthP_Can0 + canNum))))
    {
        /*权限未开启*/
    	return;
    }

    /*设置命令索引错误*/
    if(((index + len) > GINDEX109_LEN) || (0 == len))
    {
        return;
    }

    for(i = index; i < (index + len); i++)
    {
        /*写入参数处理*/
        RcvGroupHWParaCfgAPI(i, eAuthP_Can0 + canNum);
    }
}

/*=================================================================================================
** @Name      : void SendCanGroupSystemStateInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送系统整体状态信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupSystemStateInfoMsg(void)
{
	/*发送单帧*/
	(void)CANBuildSendUpsMsg_API(0xff, 0, 50, 0, GINDEX50_LEN, eCAN0);
}

/*=================================================================================================
** @Name      : void SendCanGroupPortStateInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送输入输出端口状态信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupPortStateInfoMsg(void)
{
	/*发送单帧*/
	(void)CANBuildSendUpsMsg_API(0xff, 0, 56, 0, GINDEX56_LEN, eCAN0);
}

/*=================================================================================================
** @Name      : void SendCanGroupAuthorityInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送当前权限状态信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupAuthorityInfoMsg(void)
{
	/*发送单帧*/
	(void)CANBuildSendUpsMsg_API(0xff, 0, 80, 0, GINDEX80_LEN, eCAN0);
}

/*=================================================================================================
** @Name      : void SendCanGroupHisErrInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史故障信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisErrInfoMsg(void)
{
	/*发送长帧*/
	(void)CANBuildSendUpmMsg_API(0xff, 0, 45, 0, GINDEX45_LEN, eCAN0);
}

/*=================================================================================================
** @Name      : void SendCanGroupHisEventInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史事件信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisEventInfoMsg(void)
{
	/*发送长帧*/
	(void)CANBuildSendUpmMsg_API(0xff, 0, 46, 0, GINDEX46_LEN, eCAN0);
}

/*=================================================================================================
** @Name      : void SendCanGroupHisWaveInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史故障录波信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisWaveInfoMsg(void)
{
	/*发送长帧*/
	(void)CANBuildSendUpmMsg_API(0xff, 0, 47, 0, GINDEX47_LEN, eCAN0);
}

/*=================================================================================================
** @Name      : void SendCanGroupHisDodInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史DOD记录信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisDodInfoMsg(void)
{
	/*发送长帧*/
	(void)CANBuildSendUpmMsg_API(0xff, 0, 48, 0, GINDEX48_LEN, eCAN0);
}

/*=================================================================================================
** @Static function definition
**===============================================================================================*/
