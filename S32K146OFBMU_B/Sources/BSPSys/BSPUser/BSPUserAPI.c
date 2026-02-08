/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : BSPUserAPI.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : BSP用户接口函数模块
** @Instructions : 用户调用
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "BSPConfig.h"
#include "osif.h"
#include "wait.h"
#include "DEVGPIO.h"
#include "DEVADC.h"
#include "InsuCheck.h"
#include "CurrSample.h"
#include "HigAFESamp.h"
#include "CellAFESamp.h"
#include "ParaInterface.h"
#include "CANInterface.h"
#include "MbInterface.h"
#include "TacInterface.h"
#include "DiagInterface.h"
#include "CalcInterface.h"
#include "EnerInterface.h"
#include "StaInterface.h"

#include "SocUserAPI.h"
#include "OutCurrCorr.h"
#include "HigVoltCorr.h"
#include "Authority.h"
#include "ModbusUser.h"
#include "LoadApp.h"
#include "BSPUserAPI.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : void BSPDisIRQUserInit(void)
** @Input     : void
** @Output    : void
** @Function  : 禁止中断后的用户初始化
** @The notes : 用户添加硬件和任务初始化等
**===========================================================================================*/
void BSPDisIRQUserInit(void)
{
	ParaStoreParaInfoInit();            /*参数配置值初始化*/
	ParaDownloadInit();                 /*参数批量配置初始化*/

	CellLTC6813Init();                  /*单体采样LTC6813初始化*/
	//HVSampLTC2949Init();              /*高压采样LTC2949初始化*/
	CurrSampleInit();                   /*电流采样初始化*/
	InsuAdhesMeasurInit();              /*绝缘和接触器粘连检测初始化*/

	CANLibAllInit_API();                /*CAN通信包初始化*/
	ModbusUserInit();                   /*Modbus通信包初始化*/

	AuthorityInit();                    /*权限管理初始化*/
	SysPublicInfoInit();                /*系统公用信息初始化*/
	GroupDataInfoInit();                /*电池组信息初始化*/
	DiagErrFilterInit();                /*故障诊断初始化*/
}

/*=============================================================================================
** @Name      : void BSPEnIRQUserInit(void)
** @Input     : void
** @Output    : void
** @Function  : 使能中断后的用户初始化
** @The notes : 用户添加功能初始化等
**===========================================================================================*/
void BSPEnIRQUserInit(void)
{
	BootLoaderInit();                   /*在线升级初始化*/
	OutCurrSampCorrInit();              /*电流采样校准初始化*/
	HigVoltSampCorrInit();              /*高压采样校准初始化*/

	EnerCapCalcAllInit();               /*电池组电能量计算初始化*/
	GroupStatisAllInit();               /*电池组统计模块初始化*/

	GroupStateInfoInit();               /*电池组状态检测初始化*/
	GroupModeInfoInit();                /*电池组模式检测初始化*/
	GroupProtectInit();                 /*电池组故障保护初始化*/
    DiagGroupSosInit();                 /*电池组安全等级SOS诊断初始化*/

	SocSoeLibAllInit();                 /*SOC/SOE算法包初始化*/
}

/*=============================================================================================
** @Name      : u32 BSPGetOSMillSecTimeAPI(void)
** @Input     : void
** @Output    : RTOS毫秒计时时间 1ms
** @Function  : 获取RTOS毫秒计时时间
** @The notes :
**===========================================================================================*/
u32 BSPGetOSMillSecTimeAPI(void)
{
	return(OSIF_GetMilliseconds());
}

/*=============================================================================================
** @Name      : void BSPSetOSMicrosecDelayAPI(u32 time)
** @Input     : time:RTOS延时 1微秒
** @Output    : void
** @Function  : 设置RTOS等待延时
** @The notes :
**===========================================================================================*/
void BSPSetOSMicrosecDelayAPI(u32 time)
{
	S32_WaitUs(time);
}
