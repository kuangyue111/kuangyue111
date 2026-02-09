/*==========================================Copyright(c)===========================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information------------------------------------------
** @File Name    : ParaDnload.h
** @Date         : 2019.11.1
** @Function     : 参数存储模块参数批量下载
** @Instructions : 
**===============================================================================================*/
#ifndef _PARADNLOAD_H
#define _PARADNLOAD_H

/*=================================================================================================
** @Header file definition
**===============================================================================================*/
#include "BSPTypeDef.h"

/*=================================================================================================
** @Macro definition
**===============================================================================================*/
#define PARA_DNLOAD_MAX_ADDR            (4095)              /*读写EEPROM最大地址*/
#define PARA_DNLOAD_MAX_BYTE            (50)                /*读写一段参数最大字节数*/

/*=================================================================================================
** @Data type definition
**===============================================================================================*/
/*参数批量下载状态*/
typedef enum
{
	eParaDnSta_NowState = 0,		    					/*[0]:当前参数配置状态(0:非配置状态 1:配置状态)*/
	eParaDnSta_CfgResult = 1,		    					/*[1]:参数配置结果(0:未操作 1:正在读 2:读成功 3:读失败 4:等待写 5:写成功 6:写失败 7:备份成功 8:备份失败)*/
	eParaDnSta_ErrInfo = 2,		        					/*[2]:参数配置异常信息(0:无异常 其它:异常码)*/

	eParaDnSta_Fin
}e_ParaDnStaInfo;

/*参数配置读取信息 */
typedef enum
{
	eParaDnRd_AddrL = 0,		    						/*[[0]:读参数起始EEPROM地址L)*/
	eParaDnRd_AddrH = 1,		    						/*[1]:读参数起始EEPROM地址H*/
	eParaDnRd_ByteNum = 2,		        					/*[2]:读参数总字节数*/
	eParaDnRd_Data = 3,		      							/*[3]…[n]:读到的参数值*/

	eParaDnRd_Fin
}e_ParaDnReadInfo;

/*参数配置控制命令    [3]读写参数起始EEPROM地址H*/
typedef enum
{
	eParaDnCmd_CfgState = 0,		    					/*[0]:切换参数配置状态(0:退出 1:进入)*/
	eParaDnCmd_RdOrWt = 1,									/*[1]:读写命令(0:读参数 1:写参数 2:备份参数))*/
	eParaDnCmd_AddrL = 2,		        					/*[2]:读参数起始EEPROM地址L*/
	eParaDnCmd_AddrH = 3,		        					/*[3]:读参数起始EEPROM地址H*/
	eParaDnCmd_ByteNum = 3,		        					/*[4]:读参数字节数*/

	eParaDnCmd_Fin
}e_ParaDnCtrlCmd;

/*参数配置写入信息 */
typedef enum
{
	eParaDnWt_AddrL = 0,		    						/*[0]:写参数起始EEPROM地址L*/
	eParaDnWt_AddrH = 1,		    						/*[1]:写参数起始EEPROM地址H*/
	eParaDnWt_ByteNum = 2,		        					/*[2]:写参数总字节数*/
	eParaDnWt_Data = 3,		      							/*[3]…[n]:写入的参数值*/

	eParaDnWt_Fin
}e_ParaDnWriteInfo;

/*参数配置异常信息 */
typedef enum
{
	eParaDnErr_Normal = 0,		    						/*正常无故障*/
	eParaDnErr_CmdExp = 1,		    						/*命令异常*/
	eParaDnErr_NotRWSta = 2,		    					/*非配置状态*/
	eParaDnErr_NotWWait = 3,		    					/*非写等待状态*/
	eParaDnErr_AddrLenErr = 4,		        				/*读写地址长度错误*/
	eParaDnErr_CommuErr = 5,		        				/*通信传输错误*/
	eParaDnErr_RWEepErr = 6,		      					/*读写EEPROM错误*/

	eParaDnErr_Fin
}e_ParaDnErrInfo;

/*=================================================================================================
** @Interface function declaration
**===============================================================================================*/
/*=================================================================================================
** @Name      : void ParaDownloadInit(void
** @Input     : void
** @Output    : void
** @Function  : 参数批量下载功能初始化
** @The notes :
**===============================================================================================*/
void ParaDownloadInit(void);

/*=================================================================================================
** @Name      : void ParaChangDownloadState(void)
** @Input     : void
** @Output    : void
** @Function  : 切换参数批量下载状态
** @The notes : 接收到配置状态切换命令后调用
**===============================================================================================*/
void ParaChangDownloadState(void);

/*=================================================================================================
** @Name      : void ParaDownloadRWCmdHandle(void)
** @Input     : void
** @Output    : void
** @Function  : 参数批量下载读写命令处理
** @The notes : 接收到读写命令和地址信息后调用
**===============================================================================================*/
void ParaDownloadRWCmdHandle(void);

/*=================================================================================================
** @Name      : void ParaDownloadWriteParaHandle(void)
** @Input     : void
** @Output    : void
** @Function  : 参数批量下载写入参数处理
** @The notes : 接收完写入参数信息后调用
**===============================================================================================*/
void ParaDownloadWriteParaHandle(void);

#endif
