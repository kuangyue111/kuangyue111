/*==========================================Copyright(c)===========================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information------------------------------------------
** @File Name    : CANUser.h
** @Founder      : SBK326
** @Date         : 2019.11.1
** @Function     : CAN通信包用户接口
** @Instructions :
**===============================================================================================*/
#ifndef _CANUSER_H
#define _CANUSER_H

/*=================================================================================================
** @Header file definition
**===============================================================================================*/
#include "BSPTypeDef.h"

/*=================================================================================================
** @Macro definition
**===============================================================================================*/
/*=======================================通信包配置===============================================*/
/*通信包基本配置*/
#define CAN_DEVTAB_NUM              1                       /*创建设备表数目*/
#define BMU_INDEX_NUM               76                      /*BMU-CAN通信主索引总数(创建设备表的主索引数目)*/
#define CAN_TRTAB_NUM               10                      /*通信收发处理表数目*/

/*路由功能配置*/
#define CAN_ROUTE_NUM               2                       /*路由CAN通道数目*/
#define CAN_ROUTE0_NUM              1                       /*路由口1转发通道数目*/
#define CAN_ROUTE1_NUM              1                       /*路由口2转发通道数目*/
#define CAN_ROUTE2_NUM              0                       /*路由口3转发通道数目*/
#define CAN_ROUTE3_NUM              0                       /*路由口4转发通道数目*/

/*周期发送信息长度配置*/
#define CAN_TX_VER_NUM              0                       /*主动发送校时帧信息配置组数*/
#define CAN_TX_BRE_NUM              3                       /*主动上传心跳帧信息配置组数*/
#define CAN_TX_MUL_NUM              5                       /*主动更新发送长帧配置组数*/
#define CAN_TX_SIN_NUM              25                      /*主动更新发送单帧配置组数*/
#define CAN_TX_CEL_NUM              6                       /*主动上传单体信息配置组数*/
#define CAN_TX_PAR_NUM              5                       /*主动上传参数信息配置组数*/

/*周期发送信息配置总组数*/
#define CAN_TX_ALL_NUM              (CAN_TX_MUL_NUM+CAN_TX_SIN_NUM+CAN_TX_CEL_NUM+CAN_TX_PAR_NUM)

/*参数事件读写长度配置*/
#define CAN_RW_PAR_NUM              40                      /*一次(长帧)最大读写参数字节数*/
#define CAN_RO_RCD_NUM              51                      /*一次(长帧)最大读取数据记录信息字数*/
#define CAN_RO_WAV_NUM              50                      /*一次(长帧)最大读取故障录波信息字数*/

/*=======================================子索引长度配置===========================================*/
#define CANINDEX0_LEN               4                       /*CAN通信主索引0数组长度(校时信息)*/
#define CANINDEX1_LEN               8                       /*CAN通信主索引1数组长度(设备信息)*/
#define CANINDEX2_LEN               23                      /*CAN通信主索引2数组长度(设备序列号信息)*/
#define CANINDEX3_LEN               9                       /*CAN通信主索引3数组长度(CAN通信包基本信息)*/
#define CANINDEX4_LEN               (2+6)                   /*CAN通信主索引4数组长度(CAN通信包路由信息)*/

#define CANINDEX10_LEN              (1+3*CAN_TX_VER_NUM)    /*CAN通信主索引10数组长度(CAN发送校时报文信息)*/
#define CANINDEX11_LEN              (1+3*CAN_TX_BRE_NUM)    /*CAN通信主索引11数组长度(CAN发送心跳报文信息)*/
#define CANINDEX12_LEN              (1+7*CAN_TX_MUL_NUM)    /*CAN通信主索引12数组长度(CAN发送主动更新多帧报文信息)*/
#define CANINDEX13_LEN              (1+7*CAN_TX_SIN_NUM)    /*CAN通信主索引13数组长度(CAN发送上传单帧报文信息)*/
#define CANINDEX14_LEN              (1+8*CAN_TX_CEL_NUM)    /*CAN通信主索引14数组长度(CAN发送上传单体信息单帧报文信息)*/
#define CANINDEX15_LEN              (1+8*CAN_TX_PAR_NUM)    /*CAN通信主索引15数组长度(CAN发送上传参数信息单帧报文信息)*/

#define CANINDEX20_LEN              3                       /*CAN通信主索引20数组长度(参数批量下载状态)*/
#define CANINDEX21_LEN              (5+CAN_RW_PAR_NUM)      /*CAN通信主索引21数组长度(读取批量参数信息)*/
#define CANINDEX22_LEN              5                       /*CAN通信主索引22数组长度(参数批量下载控制命令)*/
#define CANINDEX23_LEN              (5+CAN_RW_PAR_NUM)      /*CAN通信主索引23数组长度(写入批量参数信息)*/

#define CANINDEX30_LEN              4                       /*CAN通信主索引30数组长度(故障/事件/录波记录数目)*/
#define CANINDEX31_LEN              12                      /*CAN通信主索引31数组长度(故障记录监控信息)*/
#define CANINDEX32_LEN              12                      /*CAN通信主索引32数组长度(事件记录监控信息)*/
#define CANINDEX33_LEN              (6+CAN_RO_WAV_NUM)      /*CAN通信主索引33数组长度(故障录波记录监控信息)*/
#define CANINDEX35_LEN              3                       /*CAN通信主索引35数组长度(运行数据记录总数目)*/
#define CANINDEX36_LEN              32                      /*CAN通信主索引36数组长度(运行数据记录日数目)*/
#define CANINDEX37_LEN              (5+CAN_RO_RCD_NUM)      /*CAN通信主索引37数组长度(运行数据记录查询信息)*/
#define CANINDEX39_LEN              6                       /*CAN通信主索引39数组长度(历史记录读写操作命令)*/

/*=======================================主索引映射配置===========================================*/
#define CANMAP90_INDEX              0                       /*CAN通信主索引90映射起始主索引(单体充电均衡状态)*/
#define CANMAP91_INDEX              10                      /*CAN通信主索引91映射起始主索引(单体放电均衡状态)*/
#define CANMAP92_INDEX              50                      /*CAN通信主索引92映射起始主索引(单体电压信息1)*/
#define CANMAP93_INDEX              110                     /*CAN通信主索引93映射起始主索引(单体电压信息2)*/
#define CANMAP94_INDEX              150                     /*CAN通信主索引94映射起始主索引(单体温度信息1)*/
#define CANMAP95_INDEX              180                     /*CAN通信主索引94映射起始主索引(单体温度信息2)*/

#define CANMAP100_INDEX             0                       /*CAN通信主索引100映射起始主索引(系统通用参数)*/
#define CANMAP101_INDEX             25                      /*CAN通信主索引101映射起始主索引(系统硬件参数)*/
#define CANMAP102_INDEX             50                      /*CAN通信主索引102映射起始主索引(设备通用参数)*/
#define CANMAP103_INDEX             75                      /*CAN通信主索引103映射起始主索引(设备高级参数)*/
#define CANMAP104_INDEX             100                     /*CAN通信主索引104映射起始主索引(设备硬件参数)*/

/*=======================================通信数据类型定义=========================================*/
#define CAN_DT_U0                   0                       /*空类型*/
#define CAN_DT_U8                   1                       /*U8类型*/
#define CAN_DT_U16                  2                       /*U16类型*/
#define CAN_DT_U32                  4                       /*U32类型*/
#define CAN_DT_S8                   1                       /*S8类型*/
#define CAN_DT_S16                  2                       /*S16类型*/
#define CAN_DT_S32                  4                       /*S32类型*/

/*=======================================通信口读写方式定义=======================================*/
#define CAN_READONLY                0                       /*CAN口只读*/
#define CAN_UPWRITE                 0x05                    /*CAN口上层可写(CAN0/CAN2)*/
#define CAN_DNWRITE                 0x02                    /*CAN口下层可写(CAN1)*/

/*=================================================================================================
** @Data type definition
**===============================================================================================*/
/*CAN通信校时信息索引定义*/
typedef enum
{
    eCANTim0_Y = 0,                     //校时信息-年
	eCANTim0_MD = 1,                    //校时信息-月日
	eCANTim0_HM = 2,                    //校时信息-时分
	eCANTim0_MS = 3                     //校时信息-毫秒
}e_CANTimeInfo0;

/*CAN通信设备信息索引定义*/
typedef enum
{
    eCANDev1_ID = 0,                    //设备识别码
	eCANDev1_Addr = 1,                  //设备地址
	eCANDev1_HW = 2,                    //硬件版本号
	eCANDev1_SWL = 3,                   //软件版本号L
	eCANDev1_SWH = 4,                   //软件版本号H
	eCANDev1_BSP = 5,                   //BSP版本号
	eCANDev1_CAN = 6,                   //CAN通信包版本号
	eCANDev1_Mdb = 6                    //Modbus通信包版本号
}e_CANDevInfo1;

/*CAN通信包基本配置信息索引定义*/
typedef enum
{
    eCANRBas3_DevNum = 0,               /*通信包设备表数目*/
	eCANRBas3_IndexNum = 1,             /*主控主索引数目*/
	eCANRBas3_TRTabNum = 2,             /*收发通信表数目*/
	eCANRBas3_TxVerNum = 3,             /*主动发送校时帧信息配置组数*/
	eCANRBas3_TxBreNum = 4,             /*主动上传心跳帧信息配置组数*/
	eCANRBas3_TxMulNum = 5,             /*主动更新发送长帧配置组数*/
	eCANRBas3_TxSinNum = 6,             /*主动更新发送单帧配置组数*/
	eCANRBas3_TxCelNum = 7,             /*主动上传单体信息配置组数*/
	eCANRBas3_TxParNum = 8              /*主动上传参数信息配置组数*/
}e_CANBaseInfo3;

/*CAN通信路由信息索引定义*/
typedef enum
{
    eCANRout4_En = 0,                   /*路由使能*/
    eCANRout4_Num = 1,                  /*路由组数*/
}e_CANRoutInfo4;

/*=================================================================================================
** @Interface function declaration
**===============================================================================================*/
/*=================================================================================================
** @Name      : void CANDeviceTableInit(void)
** @Input     : void
** @Output    : void
** @Function  : CAN通信设备表初始化
** @The notes : CAN通信包总初始化调用
**===============================================================================================*/
void CANDeviceTableInit(void);

/*=================================================================================================
** @Name      : void CANCommuPortUserInit(void)
** @Input     : void
** @Output    : void
** @Function  : CAN通信口用户初始化
** @The notes : CAN通信包总初始化调用
**===============================================================================================*/
void CANCommuPortUserInit(void);

/*=================================================================================================
** @Name      : void CANSetMsgRouteTransEnAPI(u8 state)
** @Input     : state:0禁止转发 1使能转发
** @Output    : void
** @Function  : 设置CAN通信包路由转发功能使能状态
** @The notes :
**===============================================================================================*/
void CANSetMsgRouteTransEnAPI(u8 state);

/*=================================================================================================
** @Name      : void CANSetMsgCycleTxEnAPI(u8 state)
** @Input     : state:0禁止发送 1使能发送
** @Output    : void
** @Function  : 设置CAN通信包周期上传所有信息使能状态
** @The notes :
**===============================================================================================*/
void CANSetMsgCycleTxEnAPI(u8 state);

/*=================================================================================================
** @Name      : void CANSetCellCycleTxEnAPI(u8 state)
** @Input     : state:0禁止发送 1使能发送
** @Output    : void
** @Function  : 设置CAN通信包周期上传单体信息使能状态
** @The notes :
**===============================================================================================*/
void CANSetCellCycleTxEnAPI(u8 state);

/*=================================================================================================
** @Name      : void CANSetParaCycleTxEnAPI(u8 state)
** @Input     : state:0禁止发送 1使能发送
** @Output    : void
** @Function  : 设置CAN通信包周期上传参数信息使能状态
** @The notes :
**===============================================================================================*/
void CANSetParaCycleTxEnAPI(u8 state);

#endif
