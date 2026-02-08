/*==========================================Copyright(c)===========================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information------------------------------------------
** @File Name    : GroupMsg.h
** @Founder      : SBK326
** @Date         : 2019.11.1
** @Function     : 电池组主控CAN报文处理模块
** @Instructions : 
**===============================================================================================*/
#ifndef _GEOUPMSG_H
#define _GROUPMSG_H

/*=================================================================================================
** @Header file definition
**===============================================================================================*/
#include "BSPTypeDef.h"

/*=================================================================================================
** @Macro definition
**===============================================================================================*/

/*=================================================================================================
** @Data type definition
**===============================================================================================*/

/*=================================================================================================
** @Interface function declaration
**===============================================================================================*/
/*=================================================================================================
** @Name      : void RcvCanDeviceSerialCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收设备序列号配置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(2)处理调用
**===============================================================================================*/
void RcvCanDeviceSerialCfgDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanDnloadParaCmdDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组参数批量配置读写命令CAN报文处理
** @The notes : CAN通信包接收请求修改(22)处理调用
**===============================================================================================*/
void RcvCanDnloadParaCmdDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanDnloadParaWRDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组参数批量配置写数据CAN报文处理
** @The notes : CAN通信包接收请求修改(23)处理调用
**===============================================================================================*/
void RcvCanDnloadParaWRDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanErrRcdReadInfoDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收查询故障记录详细信息读取CAN报文处理
** @The notes : CAN通信包接收请求查询(31)处理调用
**===============================================================================================*/
void RcvCanErrRcdReadInfoDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanEventRcdReadInfoDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收查询事件记录详细信息读取CAN报文处理
** @The notes : CAN通信包接收请求查询(32)处理调用
**===============================================================================================*/
void RcvCanEventRcdReadInfoDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanDataRcdDisplayInfoDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收查询数据记录详细信息显示CAN报文处理
** @The notes : CAN通信包接收请求查询(37)处理调用
**===============================================================================================*/
void RcvCanDataRcdDisplayInfoDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanHisRecordRWDateDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收历史记录读写日期处理
** @The notes : CAN通信包接收请求修改(39)处理调用
**===============================================================================================*/
void RcvCanHisRecordRWDateDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupPasswordDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组权限密码命令CAN报文处理
** @The notes : CAN通信包接收请求修改(85)处理调用
**===============================================================================================*/
void RcvCanGroupPasswordDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupCtrlCmdDeal(u8 index, u8 len)
** @Input     : index:索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组控制命令CAN报文处理
** @The notes : CAN通信包接收请求修改(86)处理调用
**===============================================================================================*/
void RcvCanGroupCtrlCmdDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanUserCtrlCmdDeal(u8 index, u8 len)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收用户控制命令CAN报文处理
** @The notes : CAN通信包接收请求修改(87)处理调用
**===============================================================================================*/
void RcvCanUserCtrlCmdDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupDebugCmdDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收电池组调试命令CAN报文处理
** @The notes : CAN通信包接收请求修改(88)处理调用
**===============================================================================================*/
void RcvCanGroupDebugCmdDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanSystemGenParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收系统通用参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(105)处理调用
**===============================================================================================*/
void RcvCanSystemGenParaCfgDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanSystemHWParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收系统硬件参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(106)处理调用
**===============================================================================================*/
void RcvCanSystemHWParaCfgDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupGenParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控通用参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(107)处理调用
**===============================================================================================*/
void RcvCanGroupGenParaCfgDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupHigParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控高级参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(108)处理调用
**===============================================================================================*/
void RcvCanGroupHigParaCfgDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupHWParaCfgDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:控制索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控高级参数设置命令CAN报文处理
** @The notes : CAN通信包接收请求修改(109)处理调用
**===============================================================================================*/
void RcvCanGroupHWParaCfgDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void RcvCanGroupReadSysClockDeal(u8 index, u8 len, u8 canNum)
** @Input     : index:子索引号 len:查询索引长度 canNum:CAN口
** @Output    : void
** @Function  : 接收主控请求查询系统时钟信息CAN报文处理
** @The notes : CAN通信包接收请求查询处理调用
**===============================================================================================*/
void RcvCanGroupReadSysClockDeal(u8 index, u8 len, u8 canNum);

/*=================================================================================================
** @Name      : void SendCanGroupSystemStateInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送系统整体状态信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupSystemStateInfoMsg(void);

/*=================================================================================================
** @Name      : void SendCanGroupPortStateInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送输入输出端口状态信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupPortStateInfoMsg(void);

/*=================================================================================================
** @Name      : void SendCanGroupAuthorityInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送当前权限状态信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupAuthorityInfoMsg(void);

/*=================================================================================================
** @Name      : void SendCanGroupHisErrInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史故障信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisErrInfoMsg(void);

/*=================================================================================================
** @Name      : void SendCanGroupHisEventInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史事件信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisEventInfoMsg(void);

/*=================================================================================================
** @Name      : void SendCanGroupHisWaveInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史故障录波信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisWaveInfoMsg(void);

/*=================================================================================================
** @Name      : void SendCanGroupHisDodInfoMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 用户自行发送历史DOD记录信息CAN报文
** @The notes : 用户需要发送时调用
**===============================================================================================*/
void SendCanGroupHisDodInfoMsg(void);

#endif
