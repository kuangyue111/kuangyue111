/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : GroupInfo.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     :
** @Instructions :
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "GroupPara.h"
#include "PublicInfo.h"
#include "HigAFESamp.h"
#include "LowADCSamp.h"
#include "HigVoltCorr.h"
#include "OutCurrCorr.h"
#include "DiagInterface.h"
#include "GroupInfo.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
extern u16 gGBmuHigLevPara_103[]; 			/*主控高级参数值*/

u16 gGHisErrInfo_45[GINDEX45_LEN] =			/*历史故障记录信息查询数组*/
{
	0,		/*历史故障记录故障类型		0:无效 1:告警产生 2:告警消除 3:故障参数 4:故障消除*/
	0,		/*历史故障记录故障代码*/
	0,		/*历史故障记录时间-年*/
	0,		/*历史故障记录时间-月/日	"高8位:月;低8位:日."*/
	0,		/*历史故障记录时间-时/分	"高8位:时;低8位:分."*/
	0,		/*历史故障记录时间-秒*/
	0,		/*历史故障记录附件信息1(保护状态)*/
	0,		/*历史故障记录附件信息2(工作状态)*/
	0,		/*历史故障记录附件信息3(输出电流)*/
	0,		/*历史故障记录附加信息4*/
	0		/*历史故障记录附加信息5*/
};

u16 gGHisEventInfo_46[GINDEX46_LEN] =		/*历史事件记录信息查询数组*/
{
	0,		/*历史事件记录故障类型		0:无效 1:告警产生 2:告警消除 3:故障参数 4:故障消除*/
	0,		/*历史事件记录故障代码*/
	0,		/*历史事件记录时间-年*/
	0,		/*历史事件记录时间-月/日	"高8位:月;低8位:日."*/
	0,		/*历史事件记录时间-时/分	"高8位:时;低8位:分."*/
	0,		/*历史事件记录时间-秒*/
	0,		/*历史事件记录附件信息1(保护状态)*/
	0,		/*历史事件记录附件信息2(工作状态)*/
	0,		/*历史事件记录附件信息3(输出电流)*/
	0,		/*历史事件记录附加信息4*/
	0		/*历史事件记录附加信息5*/
};

u16 gGHisWaveInfo_47[GINDEX47_LEN] =		/*历史故障录波信息查询数组*/
{
	0,      /*故障录波记录序号*/
	0,      /*故障录波记录类型码 0:无效记录 1:单体二级过压,2:单体一级过压,3:单体二级欠压,4:单体一级欠压;5:总压二级过压,6:总压一级过压,7:总压二级欠压,8:总压一级欠压;9:充电二级过温;10:充电一级过温;11:充电二级低温;12:充电一级低温;13:放电二级过温;14:放电一级过温;15:放电二级低温;16:放电一级低温;17:充电二级过流;18:充电一级过流;19:放电二级过流;20:放电一级过流;21:充电超限值;22:放电超限值*/
	0,      /*故障录波记录时间-年*/
	0,      /*故障录波记录时间-月/日	"高8位:月;低8位:日."*/
	0,      /*故障录波记录时间-时/分	"高8位:时;低8位:分."*/
	0,      /*故障录波记录时间-秒*/
	0,      /*故障录波记录值1*/
	0       /*故障录波记录值50*/
};

u16 gGHisDodInfo_48[GINDEX48_LEN] =			/*历史DOD记录信息查询数组*/
{
	0,		/*历史DOD记录序号*/
	0,		/*当前DOD记录有效数目*/
	0,		/*历史DOD记录时间-年*/
	0,		/*历史DOD记录时间-月/日	"高8位:月;低8位:日."*/
	0,		/*历史DOD记录时间-时/分	"高8位:时;低8位:分."*/
	0,		/*历史DOD记录时间-秒*/
	0,		/*历史DOD记录值1*/
	0		/*历史DOD记录值50*/
};

u16 gGSysStateInfo_50[GINDEX50_LEN] =		/*系统状态信息数组*/
{
	0,		/*系统保护状态[0,5]		0:未就绪 1:正常 2:充电截止 3:放电截止 4:充放电限制 5:充放电保护*/
	0,		/*系统工作状态[0,6]		0:开机,1:待命,2:启动,3:运行,4:停机,5:故障 6:关机*/
	0,		/*电池充放电状态[0,2]	0:空闲 1:充电 2:放电*/
	0		/*系统工作模式[0,5]		0:自动 1:手动 2:紧急 5:调试*/
};

u16 gGLimCPInfo_51[GINDEX51_LEN] =			/*电流功率限值数组*/
{
	0,		/*最大允许充电电流限值(PCS充电输入最大电流)[0,10000]0.1A*/
	0,		/*最大允许放电电流限值(PCS放电输出最大电流)[0,10000]0.1A*/
	0,		/*最大允许充电功率限值(PCS充电输入最大功率)[0,10000]0.1kW*/
	0,		/*最大允许放电功率限值(PCS放电输出最大功率)[0,10000]0.1kW*/
};

u16 gGLimVInfo_52[GINDEX52_LEN] =			/*电压限值信息数组*/
{
	0,		/*最大允许充电电压限值(PCS充电截止电压)[0,10000]0.1V*/
	0		/*最大允许放电电压限值(PCS放电截止电压)[0,10000]0.1V*/
};

u16 gGAlarmInfo_53[GINDEX53_LEN] =			/*告警信息数组*/
{
	0,		/*电池告警信息1*/
	0,		/*电池告警信息2*/
	0		/*电池告警信息3*/
};

u16 gGFaultInfo_54[GINDEX54_LEN] = 			/*故障信息数组*/
{
	0,		/*电池故障信息1*/
	0,		/*电池故障信息2*/
	0		/*电池故障信息3*/
};

u16 gGErrDetail_55[GINDEX55_LEN] = 			/*故障定位信息数组*/
{
	0,		/*定位故障码*/
	0		/*定位故障详情*/
};

u16 gGPortStateInfo_56[GINDEX56_LEN] =		/*输入输出口状态信息数组*/
{
	0,		/*输入端口回检状态*/
	0,		/*输出端口控制状态*/
	0,		/*内部输入回检状态*/
	0		/*内部输出控制状态*/
};

s16 gGSysMainInfo_60[GINDEX60_LEN] =		/*系统整体信息数组*/
{
	0,		/*总电压采样值[0,10000]0.1V*/
	0,		/*总电流采样值[-30000,30000]0.1A*/
	0,		/*输出功率值[-30000,30000]0.1kW*/
	0		/*预充电压值[0,10000]0.1V*/
};

u16 gGSysCapInfo_61[GINDEX61_LEN] =			/*系统电量信息数组*/
{
	0,		/*电池SOC值[0,1000]0.1%*/
	0,		/*电池SOE值[0,1000]0.1%*/
	0,		/*电池SOH值[0,1000]0.1%*/
	0		/*电池组总容量[0,1000]0.1%*/
};

u16 gGSysCellInfo_62[GINDEX62_LEN] = 		/*系统单体信息数组*/
{
	0,		/*单体总电压值[0,10000]0.1V*/
	0,		/*单体平均电压值[0,5000]1mV*/
	0,		/*单体平均温度值[-40,105]1℃*/
	0		/*系统环境温度值[-40,105]1℃*/
};

u16 gGVoltCharInfo_63[GINDEX63_LEN] = 		/*电压特征信息数组*/
{
	0,		/*单体最大电压序号[1,400]*/
	0,		/*单体最大电压值[0,5000]1mV*/
	0,		/*单体最小电压序号[1,400]*/
	0		/*单体最小电压值[0,5000]1mV*/
};

s16 gGTempCharInfo_64[GINDEX64_LEN] = 		/*温度特征信息数组*/
{
	0,		/*单体最高温度序号[1,400]*/
	0,		/*单体最高温度值[-40,105]1℃*/
	0,		/*单体最低温度序号[1,400]*/
	0		/*单体最低温度值[-40,105]1℃*/
};

s16 gGDcCTInfo_65[GINDEX65_LEN] =			/*直流电流温度信息数组*/
{
	0,		/*霍尔原始采样电压值[0,5000]1mV*/
	0,		/*分流器原始采样电压值[-7500,7500]0.01mV*/
	0,		/*分流器温度采样值[-40,180]1℃*/
	0		/*电源侧温度采样值[-40,126]1℃*/
};

u16 gGLVSampInfo_66[GINDEX66_LEN] =			/*低压采样信息数组*/
{
	0,		/*负极电压采样值[0,10000]0.1V*/
	0,		/*低压电源电压采样值[0,3300]0.01V*/
	0,		/*激活信号ACC电压采样值[0,3300]0.01V*/
	0		/*霍尔电源电压采样值[0,500]0.01V*/
};

u16 gGFstLeakInfo_67[GINDEX67_LEN] = 		/*直流电路信息数组*/
{
	0,		/*充电枪KC供入电压采样值[0,3300]0.01V*/
	0,		/*充电枪CC2信号电压采样值[0,500]0.01V*/
	0,		/*正极对地绝缘阻值[0,32767]1kΩ*/
	0		/*负极对地绝缘阻值[0,32767]1kΩ*/
};

s16 gGHsCurrInfo_68[GINDEX68_LEN] =			/*高边驱动电流信息数组1*/
{
	0,		/*HS1通道1驱动电流采样值[-5000,5000] 1mA*/
	0,		/*HS1通道2驱动电流采样值[-5000,5000] 1mA*/
	0,		/*HS2通道1驱动电流采样值[-5000,5000] 1mA*/
	0		/*HS2通道2驱动电流采样值[-5000,5000] 1mA*/
};

s16 gGHsCurrInfo_69[GINDEX69_LEN] =			/*高边驱动电流信息数组2*/
{
	0,		/*HS3通道1驱动电流采样值[-5000,5000] 1mA*/
	0,		/*HS3通道2驱动电流采样值[-5000,5000] 1mA*/
	0,		/*HS4通道1驱动电流采样值[-5000,5000] 1mA*/
	0		/*HS4通道2驱动电流采样值[-5000,5000] 1mA*/
};

u16 gGStatistTime_70[GINDEX70_LEN] = 		/*时间统计信息数组*/
{
	0,		/*系统运行时长[0,65535]1Hour*/
	0,		/*可充电时间[0,65535]1Min*/
	0,		/*可放电时间[0,65535]1Min*/
	0		/*充放电循环次数[0,65535]1次*/
};

u16 gGStatistErr_71[GINDEX71_LEN] = 		/*故障统计信息数组*/
{
	0,		/*单月告警数目[0,65535]*/
	0,		/*单月故障数目[0,65535]*/
	0,		/*累计告警数目[0,65535]*/
	0		/*累计故障数目[0,65535]*/
};

u16 gGStatistCapEner_72[GINDEX72_LEN] =		/*当前能量统计信息数组*/
{
	0,		/*电池组剩余电能[0,65535]0.1kWh*/
	0,		/*电池组总电能[0,65535]0.1kWh*/
	0,		/*电池组单次充电电量[0,65535]1AH*/
	0		/*电池组单次放电电量[0,655351]AH*/
};

u32 gGStatistHisCap_73[GINDEX73_LEN] =		/*累计电量统计信息数组*/
{
	0,		/*电池组累计充电电量[0,65535]1AH*/
	0		/*电池组累计放电电量[0,65535]1AH*/
};

u32 gGStatistHisEner_74[GINDEX74_LEN] =		/*累计电能统计信息数组*/
{
	0,		/*电池组累计充电电能[0,65535]0.1kWh*/
	0		/*电池组累计放电电能[0,65535]0.1kWh*/
};

u16 gGStatistDodInfo_75[GINDEX75_LEN] =		/*放电深度统计信息数组*/
{
	0,		/*最新放电深度DOD(单次)[0,1000]0.1%*/
	0,		/*近日放电深度DOD(两次平均)[0,1000]0.1%*/
	0,		/*最大放电深度DOD(单次)[0,1000]0.1%*/
	0		/*平均放电深度DOD(多次平均)[0,1000]0.1%*/
};

u16 gGStatistSosInfo_76[GINDEX76_LEN] =		/*安全等级SOS统计信息数组*/
{
	0,		/*当前安全等级SOS[0,4](等级越低越安全)*/
	0,		/*预测安全等级SOS[0,4](等级越低越安全)*/
	0,		/*当前SOS等级异常原因[按bit位表示]*/
	0		/*预测SOS等级异常原因[按bit位表示]*/
};

u16 gGAuthorInfo_80[GINDEX80_LEN] = 		/*权限级别状态信息数组*/
{
	0,		/*当前密码权限H		用户级别:0:普通用户,1:运维用户, 2:高级用户 3:超级用户*/
	0		/*当前密码权限L		启用可写端口:0-4:CAN口 5-9:RS485*/
};


u16 gGDebugResult_82[GINDEX82_LEN] =		/*调试结果状态信息数组*/
{
	0,		/*调试类型	0:从控均衡调试 1:从控接触器调试 2:从控输出干接点调试 3:从控EEPROM读写调试 4:主控内控输出端口调试 5:主控接触器调试 6:主控输出干接点调试 7:主控EEPROM读写调试*/
	0,		/*调试主控编号	0:所有主控 [1,16]:主控编号*/
	0,		/*调试从控编号	0:所有从控 [1,32]:从控编号*/
	0,		/*调试端口号/均衡类型/EEPROM地址	"输出IO/接触器/LED调试:0:所有端口 [1,6]:端口编号;均衡调试:0:禁止均衡 1:主动均衡 2:被动均衡;EEPROM读写调试:[0,65535]:EEPROM地址"*/
	0,		/*调试状态/EEPROM数据类型	"输出IO/接触器/LED调试:0:分 1:合;均衡调试:0:关闭均衡 1:充电均衡 2:放电均衡;EEPROM读写调试:0:8位数据 1:16位数据 2:32位数据"*/
	0,		/*调试写EEPROM参数值低16位	"均衡调试:按位表示开均衡电池;EEPROM读写调试:32位数据低16位"*/
	0,		/*调试写EEPROM参数值高16位	32位数据高16位*/
	0,		/*调试读EEPROM参数值低16位	32位数据低16位*/
	0,		/*调试读EEPROM参数值高16位	32位数据高16位*/
	0		/*调试结果	0:未调试 1:调试成功 2:非调试模式 3:调试参数错误 4：通讯失败*/
};

u16 gGCalibrResult_83[GINDEX83_LEN] =		/*校准结果状态信息数组*/
{
	0,		/*采样校准类型	0:电流零飘手动校准 1:充电电流单点校准 2:放电电流单点校准 3:充电电流多点校准 4:放电电流多点校准 5:高压单点校准 6:高压多点校准*/
	0,		/*采样校准状态	0:未开始 [1,99]:校准过程 100:校准成功 101:校准失败*/
	0		/*采样校准结果	按位表示,每bit为1个校准类型[0:未校准,1:已校准] bit0:电流零飘校准 bit1:充电电流校准 bit2:放电电流校准 bit3:高压单点校准*/
};

u16 gGAuthorCtrl_85[GINDEX85_LEN] =			/*权限密码登录信息数组*/
{
	0,		/*权限密码H		用户级别*/
	0		/*权限密码L		密码校对*/
};

u16 gGSysCmdCtrl_86[GINDEX86_LEN] =			/*系统控制命令信息数组*/
{
	0,		/*工作状态控制	0:正常停止 1:正常预充 2:正常启动 3:充电启动 4:放电启动 5:强制停机 6:紧急停机 7:下电关机 8:上电开机*/
	0,		/*工作模式控制	0:自动 1:手动 2:紧急 5:调试*/
	0		/*系统清除命令	1:清除当前故障 2:清除历史故障 3:清除故障数目 4:清除运行计时 5:清除累计电量 6:清除故障记录 7:清除事件记录  8:清除故障录波 9:清除数据记录 10:恢复出厂设置*/
};

u16 gGUserCtrl_87[GINDEX87_LEN] =			/*用户控制命令信息数组*/
{
	0,		/*单体信息触发命令	"每bit(0-退出,1-触发):bit0:单体电压信息触发;bit1:单体温度信息触发;bit2:单体均衡状态信息触发."*/
	0,		/*内部报文转发命令	0-不触发不转发,1-触发内部协议 2-转发内部报文 3-全部触发转发*/
	0,		/*历史故障切换命令	0-最新记录,[1,30]-指定日期记录,90-首页记录,91-下一页记录*/
	0,		/*历史事件切换命令	0-最新记录,[1,30]-指定日期记录,90-首页记录,91-下一页记录*/
	0,		/*故障录波切换命令	0-最新记录,[1,30]-指定日期记录,90-首条记录,91-下一条记录*/
	0		/*DOD记录切换命令	0-最新记录,90-页条记录,91-下一页记录*/
};

u16 gGDebugCtrl_88[GINDEX88_LEN] =			/*系统调试命令信息数组*/
{
	0,		/*调试类型	0:从控均衡调试 1:从控接触器调试 2:从控输出干接点调试 3:从控EEPROM读写调试 4:主控内控输出端口调试 5:主控接触器调试 6:主控输出干接点调试 7:主控EEPROM读写调试*/
	0,		/*调试主控编号	0:所有主控 [1,16]:主控编号*/
	0,		/*调试从控编号	0:所有从控 [1,32]:从控编号*/
	0,		/*调试端口号/均衡类型/EEPROM地址	"输出口/接触器调试:0:所有端口 [1,11]:端口编号;均衡调试:0:禁止均衡 1:主动均衡 2:被动均衡;EEPROM读写调试:[0,65535]:EEPROM地址"*/
	0,		/*调试状态/EEPROM数据类型	"输出口/接触器调试:0:分 1:合;均衡调试:0:关闭均衡 1:充电均衡 2:放电均衡;EEPROM读写调试:0:8位数据 1:16位数据 2:32位数据"*/
	0,		/*调试写EEPROM参数值低16位	32位数据低16位*/
	0,		/*调试写EEPROM参数值高16位	32位数据高16位*/
	0		/*调试确认命令	1:确认调试/确认读EEPROM参数 2:确认写EEPROM参数*/
};

u16 gGCalibrCtrl_89[GINDEX89_LEN] =			/*采样校准命令信息数组*/
{
	0,		/*采样校准类型	0:电流零飘手动校准 1:充电电流单点校准 2:放电电流单点校准 3:充电电流多点校准 4:放电电流多点校准 5:高压单点校准 6:高压多点校准 255:终止校准*/
	0		/*校准点基准值	基准电压 0.1V /基准电流 0.01A*/
};

u16 gGChgBalanInfo_90[GINDEX90_LEN] =		/*充电均衡状态信息数组*/
{
	0		/*连续Bit位表示电池号(非1个索引表示1个从控)[0:空闲    1:均充]*/
};

u16 gGDhgBalanInfo_91[GINDEX91_LEN] =		/*放电均衡状态信息数组*/
{
	0		/*连续Bit位表示电池号(非1个索引表示1个从控)[0:空闲    1:均放]*/
};

u16 gGCellVoltInfo_92[MAX_CELL_NUM] =		/*电池单体电压信息数组*/
{
	0		/*单体电压值[0,5000]1mV*/
};

s8 gGCellTempInfo_94[MAX_CELL_NUM] =		/*电池单体温度信息数组*/
{
	0		/*单体温度值[-40,105]1℃*/
};

s16 gGReferSampInfo_200[GINDEX200_LEN] =	/*参考采集信息*/
{
	0,		/*MCU供电电源电压采样值[0,500]0.01V*/
	0,		/*SBC芯片5V输出AUX通道采样值[0,500]0.01V*/
	0,		/*SBC芯片MUX通道电压采集[0,500]0.01V*/
	0,		/*PWM电压ADC通道采样值[0,500]0.01V*/
	0,		/*PWM输入捕获通道电压值[0,500]0.01V*/
	0,		/*LTC2949芯片基准源,基准回采[0,300]0.01V*/
	0,		/*LTC2949芯片高压侧5V电压采集[0,500]0.01V*/
	0,		/*电池总电压采样,BATT电压采集[0,10000]0.1V*/
	0,		/*预充电压采样,外端电压采集[0,10000]0.01V*/
	0,		/*link+电压采样,预充电压正端电压采集[0,5000]1mV*/
	0,		/*link-电压采样,预充电压负端电压采集[0,5000]1mV*/
	0		/*负载电压原始采样值,V7电压采集[0,5000]1mV*/
};

s8 gGCellBalanTInfo_201[GINDEX201_LEN] =	/*均衡温度特征信息*/
{
	0,			/*单体均衡温度组内最大值[-40,125]1℃*/
	0,			/*单体均衡温度最大值从控号[0,239]*/
	0,			/*单体均衡温度1#从控内最大值[-40,125]1℃*/
	0,			/*单体均衡温度2#从控内最大值[-40,125]1℃*/
	0 			/*单体均衡温度n#从控内最大值[-40,125]1℃*/
};

u16 gGFanCtrlInfo_202[GINDEX202_LEN] = 		/*IMLU辅助板风扇控制信息*/
{
	0,			/*风扇alarm回检信号(4个回检通道代表16路风扇,1为异常)*/
	0,			/*水浸传感器采样值(频率),1hz*/
	0,			/*#1风扇控制转速,PWM值*/
	0,			/*#2风扇控制转速,PWM值*/
	0,			/*#3风扇控制转速,PWM值*/
	0,			/*#4风扇控制转速,PWM值*/
	0,			/*#5风扇控制转速,PWM值*/
	0,			/*#6风扇控制转速,PWM值*/
	0,			/*#7风扇控制转速,PWM值*/
	0,			/*#8风扇控制转速,PWM值*/
	0,			/*#9风扇控制转速,PWM值*/
	0,			/*#10风扇控制转速,PWM值*/
	0,			/*#11风扇控制转速,PWM值*/
	0,			/*#12风扇控制转速,PWM值*/
	0,			/*#13风扇控制转速,PWM值*/
	0,			/*#14风扇控制转速,PWM值*/
	0,			/*#15风扇控制转速,PWM值*/
	0			/*#16风扇控制转速,PWM值*/
};

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/
/*=================================================================================================
** @Name      : u16 ChangSlaveVidToGroupVid(u8 slave, u8 cell)
** @Input     : slave:从控号[0,31] cell:从控内电池号[0,17]
** @Output    : 从控内电压序号对应组内电压序号[0,399],0xffff:错误序号
** @Function  : 将从控内电压序号转换成电池组内电压序号
** @The notes : 每个从控电压数目不一样的需要特殊处理
**===============================================================================================*/
u16 ChangSlaveVidToGroupVid(u8 slave, u8 cell);

/*=================================================================================================
** @Name      : u16 ChangSlaveTidToGroupVid(u8 slave, u8 cell)
** @Input     : slave:从控号[0,31] cell:从控内温度号[0,8]
** @Output    : 从控内温度序号对应组内温度序号[0,199],0xffff:错误序号
** @Function  : 将从控内温度序号转换成电池组内温度序号
** @The notes : 每个从控温度数目不一样的需要特殊处理
**===============================================================================================*/
u16 ChangSlaveTidToGroupTid(u8 slave, u8 cell);

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
#if(0)
/*=============================================================================================
** @Name      : void InputBsuBalanceState(u8 ecu, u16 cell, u8 state)
** @Input     : ecu:从控节点号[0,31] cell:从控电池号[0,15] state:均衡状态值[0关闭 1:均充 2均放]
** @Output    : void
** @Function  : 输入接收到从控均衡状态值
** @The notes : 按连续Bit位表示电池顺序(非一个数组索引表示1个从控)
**===========================================================================================*/
void InputBsuBalanceState(u8 ecu, u16 cell, u8 state)
{
	u8 index = 0;
	u8 cellBit = 0;

    if(ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])      /*节点号错误*/
    {
        return;
    }

    index = (ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum] + cell) / 16;
    cellBit = (ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum] + cell) % 16;

	/*索引未越界*/
	if((index < GINDEX90_LEN) && (index < GINDEX91_LEN))
	{
		/*关闭均衡*/
		if(0 == state)
		{
			BitClr(gGChgBalanInfo_90[index], cellBit);
			BitClr(gGDhgBalanInfo_91[index], cellBit);
		}
		/*均衡充电*/
		else if(1 == state)
		{
			BitSet(gGChgBalanInfo_90[index], cellBit);
			BitClr(gGDhgBalanInfo_91[index], cellBit);
		}
		/*均衡放电*/
		else if(2 == state)
		{
			BitClr(gGChgBalanInfo_90[index], cellBit);
			BitSet(gGDhgBalanInfo_91[index], cellBit);
		}
	}
}

/*=============================================================================================
** @Name      : void InputBsuChgBalanState(u8 ecu, u16 cellState)
** @Input     : ecu:从控节点号[0,31] cellState:从控各电池均衡状态值(按bit位表示电池[0关闭 1:均充]
** @Output    : void
** @Function  : 输入接收到从控充电均衡的电池
** @The notes : [暂未使用] 按连续Bit位表示电池顺序(非一个数组索引表示1个从控)
**===========================================================================================*/
void InputBsuChgBalanState(u8 ecu, u16 cellState)
{
	u8 i = 0;
	u8 index = 0;
	u8 cellBit = 0;

    if(ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])      /*节点号错误*/
    {
        return;
    }

    index = (ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]) / 16;
    cellBit = (ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]) % 16;

	/*索引未越界*/
	if(index < GINDEX90_LEN)
	{
		/*起始位*/
		if(0 == cellBit)
		{
			gGChgBalanInfo_90[index] = cellState & (0xffff >> (16 - gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]));
		}
		/*非起始位*/
		else
		{

			/*按bit位逐位设置*/
			for(i = 0; i < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]; i++)
			{
				/*不超出原有索引*/
				if((cellBit + i) < 16)
				{
					if(1 == BitGet(cellState, i))
					{
						BitSet(gGChgBalanInfo_90[index], (cellBit + i));
					}
					else
					{
						BitClr(gGChgBalanInfo_90[index], (cellBit + i));
					}
				}
				/*超出原有索引*/
				else
				{
					/*下一索引未越界*/
					if((index + 1) < GINDEX90_LEN)
					{
						if(1 == BitGet(cellState, i))
						{
							BitSet(gGChgBalanInfo_90[index + 1], (cellBit + i - 16));
						}
						else
						{
							BitClr(gGChgBalanInfo_90[index + 1], (cellBit + i - 16));
						}
					}
				}
			}
		}
	}
}

/*=============================================================================================
** @Name      : void InputBsuDhgBalanState(u8 ecu, u16 cellState)
** @Input     : ecu:从控节点号[0,31] cellState:从控各电池放电均衡状态值(按bit位表示电池[0关闭 1:均放]
** @Output    : void
** @Function  : 输入接收到从控放电均衡的电池
** @The notes : [暂未使用] 按连续Bit位表示电池顺序(非一个数组索引表示1个从控)
**===========================================================================================*/
void InputBsuDhgBalanState(u8 ecu, u16 cellState)
{
	u8 i = 0;
	u8 index = 0;
	u8 cellBit = 0;

    if(ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])      /*节点号错误*/
    {
        return;
    }

    index = (ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]) / 16;
    cellBit = (ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]) % 16;

	/*索引未越界*/
	if(index < GINDEX91_LEN)
	{
		/*起始位*/
		if(0 == cellBit)
		{
			gGDhgBalanInfo_91[index] = cellState & (0xffff >> (16 - gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]));
		}
		/*非起始位*/
		else
		{

			/*按bit位逐位设置*/
			for(i = 0; i < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]; i++)
			{
				/*不超出原有索引*/
				if((cellBit + i) < 16)
				{
					if(1 == BitGet(cellState, i))
					{
						BitSet(gGDhgBalanInfo_91[index], (cellBit + i));
					}
					else
					{
						BitClr(gGDhgBalanInfo_91[index], (cellBit + i));
					}
				}
				/*超出原有索引*/
				else
				{
					/*下一索引未越界*/
					if((index + 1) < GINDEX91_LEN)
					{
						if(1 == BitGet(cellState, i))
						{
							BitSet(gGDhgBalanInfo_91[index + 1], (cellBit + i - 16));
						}
						else
						{
							BitClr(gGDhgBalanInfo_91[index + 1], (cellBit + i - 16));
						}
					}
				}
			}
		}
	}
}
#endif

/*=============================================================================================
** @Name      : void InputBsuCellVolt(u8 ecu, u8 cell, u16 volt)
** @Input     : ecu:从控节点号[0,31] cell:从控内电池号[0,15] volt:电压值 1mv
** @Output    : void
** @Function  : 输入接收到从控单体电压值
** @The notes :
**===========================================================================================*/
void InputBsuCellVolt(u8 ecu, u8 cell, u16 volt)
{
    u16 index = 0;

    if((ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])       /*节点号错误*/
        || (cell >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]))/*一个BSU电池节数错误*/
    {
        return;
    }

	//从控内序号转换成组内序号
    index = ChangSlaveVidToGroupVid(ecu, cell);

    if(index < gGBmuHigLevPara_103[eBmuHigLevPara103_VSumNum])      /*电池组总节数正确*/
    {
        gGCellVoltInfo_92[index] = volt;

        if(volt > 0)                                                /*非电压清零*/
        {
            DiagRcvCellMissCheckAPI(0, index);                      /*电压信息缺失检测*/
        }
    }
}

/*=============================================================================================
** @Name      : void InputBsuCellTemp(u8 ecu, u8 num, s8 temp)
** @Input     : ecu:从控节点号[0,31] num:从控内单体温度序号[0,3] temp:温度值(-40~126℃)
** @Output    : void
** @Function  : 输入接收到BSU单体温度值
** @The notes : 不包括均衡板温
**===========================================================================================*/
void InputBsuCellTemp(u8 ecu, u8 num, s8 temp)
{
    u16 index = 0;

    /*节点号错误*/
    if(ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])
    {
        return;
    }

    /*电池温序号正确*/
    if(num < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum])
    {
    	//从控内序号转换成组内序号
    	index = ChangSlaveTidToGroupTid(ecu, num);

	    /*电池温度序号越界*/
		if(index >= gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum])
		{
			return;
		}
    }
    else
    {
    	return;
    }

	if(temp < -41)
	{
		temp = -41;
	}
	else
	{
	    /*非单体温度清零才进行单体温度缺失检测*/
		DiagRcvCellMissCheckAPI(1, index);
	}

	gGCellTempInfo_94[index] = temp;
}

/*=============================================================================================
** @Name      : void InputBsuBalanTemp(u8 ecu, u8 num, s8 temp)
** @Input     : ecu:BSU节点号[0,31] num:从控内均衡板温序号[0,1] temp:温度值(-40~126℃)
** @Output    : void
** @Function  : 输入接收到BSU均衡板温温度值
** @The notes : 放在全部单体温度数组之后
**===========================================================================================*/
void InputBsuBalanTemp(u8 ecu, u8 num, s8 temp)
{
    u16 index = 0;

    /*节点号错误*/
    if(ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])
    {
        return;
    }

    /*均衡板温序号正确*/
    if(num < gGBmuHigLevPara_103[eBmuHigLevPara103_BalTNum])
    {
    	/*放在全部单体温度数组之后*/
    	index =  gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum] + ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BalTNum] + num;

	    /*均衡温度序号越界*/
		if(index >= (gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum] + gGBmuHigLevPara_103[eBmuHigLevPara103_BTSumNum]))
		{
			return;
		}
    }
    else
    {
    	return;
    }

	if(temp < -41)
	{
		temp = -41;
	}

	gGCellTempInfo_94[index] = temp;
}

/*=============================================================================================
** @Name      : void InputGroupHVSideData(e_HAfeSampChan index, s32 data)
** @Input     : index:高压端信息索引 data:高压端信息值
** @Output    : void
** @Function  : 输入高压端采样信息值
** @The notes :
**===========================================================================================*/
void InputGroupHVSideData(e_HAfeSampChan index, s32 data)
{
	s16 moment = 0;

	switch(index)
	{
		/*分流器电流采样值 0.01mV*/
		case HIG_AFE_Shunt:
			moment = CorrRealShuCurrByKB(data);
			//变化值在0.03mV—0.50mV之间
			if((ABS(gGDcCTInfo_65[eDcCTInfo65_ShutSignV], moment) >= 3)
				&& (ABS(gGDcCTInfo_65[eDcCTInfo65_ShutSignV], moment) <= 50))
			{
				gGDcCTInfo_65[eDcCTInfo65_ShutSignV] = (((s32)gGDcCTInfo_65[eDcCTInfo65_ShutSignV] * 20) + ((s32)moment * 12)) / 32;
			}
			else
			{
				gGDcCTInfo_65[eDcCTInfo65_ShutSignV] = moment;
			}
			break;

		/*电池总电压采样 0.1V*/
		case HIG_AFE_BatV:
			if(data < 0)
			{
				data = 0;
			}

			/*0.01V转0.1V*/
			moment = CorrRealSumVoltByKB(data);
			//变化值在0.3V—5.0V之间
			if((ABS(gGReferSampInfo_200[eReferSampInfo200_BattHV], moment) >= 3)
				&& (ABS(gGReferSampInfo_200[eReferSampInfo200_BattHV], moment) <= 50))
			{
				gGReferSampInfo_200[eReferSampInfo200_BattHV] = (((s32)gGReferSampInfo_200[eReferSampInfo200_BattHV] * 20) + ((s32)moment * 12)) / 32;
			}
			else
			{
				gGReferSampInfo_200[eReferSampInfo200_BattHV] = moment;
			}
			break;

		/*预充电压采样 0.1V*/
		case HIG_AFE_PreV:
			//if(data < 0)
			if(data < 3000)/*低于30V不显示*/
			{
				data = 0;
			}

			/*0.01V转0.1V*/
			moment = CorrRealPreVoltByKB(data);
			//变化值在0.3V—5.0V之间
			if((ABS(gGReferSampInfo_200[eReferSampInfo200_PChgHV], moment) >= 3)
				&& (ABS(gGReferSampInfo_200[eReferSampInfo200_PChgHV], moment) <= 50))
			{
				gGReferSampInfo_200[eReferSampInfo200_PChgHV] = (((s32)gGReferSampInfo_200[eReferSampInfo200_PChgHV] * 20) + ((s32)moment * 12)) / 32;
			}
			else
			{
				gGReferSampInfo_200[eReferSampInfo200_PChgHV] = moment;
			}
			break;

		/*负载电压采样 0.1V*/
		case HIG_AFE_LoadV:
			//if(data < 0)
			if(data < 5000)//低于50V不显示
			{
				data = 0;
			}

			/*0.01V转0.1V*/
			moment = CorrRealLoaVoltByKB(data);
			//变化值在0.3V—5.0V之间
			if((ABS(gGReferSampInfo_200[eReferSampInfo200_PChgHV], moment) >= 3)
				&& (ABS(gGReferSampInfo_200[eReferSampInfo200_PChgHV], moment) <= 50))
			{
				gGReferSampInfo_200[eReferSampInfo200_PChgHV] = (((s32)gGReferSampInfo_200[eReferSampInfo200_PChgHV] * 20) + ((s32)moment * 12)) / 32;
			}
			else
			{
				gGReferSampInfo_200[eReferSampInfo200_PChgHV] = moment;
			}
			break;

		/*link+电压采样,外端电压(预充电压)正极电压采集 1mV*/
		case HIG_AFE_Link1V:
			if(data < 0)
			{
				data = 0;
			}

			/*1mV*/
			moment = CorrRealLinkPosVoltByKB(data);
			//变化值在3mV—50mV之间
			if((ABS(gGReferSampInfo_200[eReferSampInfo200_Link1], moment) >= 3)
				&& (ABS(gGReferSampInfo_200[eReferSampInfo200_Link1], moment) <= 50))
			{
				gGReferSampInfo_200[eReferSampInfo200_Link1] = (((s32)gGReferSampInfo_200[eReferSampInfo200_Link1] * 20) + ((s32)moment * 12)) / 32;
			}
			else
			{
				gGReferSampInfo_200[eReferSampInfo200_Link1] = moment;
			}
			break;

		/*link-电压采样,外端电压(预充电压)负极电压采集 1mV*/
		case HIG_AFE_Link2V:
			if(data < 0)
			{
				data = 0;
			}

			/*1mV*/
			moment = CorrRealLinkNegVoltByKB(data);
			//变化值在3mV—50mV之间
			if((ABS(gGReferSampInfo_200[eReferSampInfo200_Link2], moment) >= 3)
				&& (ABS(gGReferSampInfo_200[eReferSampInfo200_Link2], moment) <= 50))
			{
				//gGReferSampInfo_200[eReferSampInfo200_Link2] = (((s32)gGReferSampInfo_200[eReferSampInfo200_Link2] * 20) + ((s32)moment * 12)) / 32;
				gGReferSampInfo_200[eReferSampInfo200_Link2] = (((s32)gGReferSampInfo_200[eReferSampInfo200_Link2] * 12) + ((s32)moment * 20)) / 32;
			}
			else
			{
				gGReferSampInfo_200[eReferSampInfo200_Link2] = moment;
			}
			break;

		/*PGND电压采样,机壳相对对负母线电压 0.1V*/
		case HIG_AFE_PgndV:
			if(data < 0)
			{
				data = 0;
			}
			gGLVSampInfo_66[eLVSamp66_NegV] = data;
			break;

		/*分流器温度采样 1℃*/
		case HIG_AFE_Temp:
			gGDcCTInfo_65[eDcCTInfo65_ShutT] = data;
			break;

		/*采样LTC2949芯片基准源,基准回采 0.01V*/
		case HIG_AFE_Vref:
			gGReferSampInfo_200[eReferSampInfo200_Ltc2949Vref] = data;
			break;

		/*采样LTC2949芯片5V输出电压采样,高压侧5V电压采集 0.01V*/
		case HIG_AFE_Out5V:
			gGReferSampInfo_200[eReferSampInfo200_Ltc2949_5V] = data;
			break;

		/*采样负载电压V7原始采样值,1mV*/
		case HIG_AFE_LoadV7:
			gGReferSampInfo_200[eReferSampInfo200_LoadV7] = data;
			break;

		default:
			break;
	}
}

/*=============================================================================================
** @Name      : void InputGroupDaisyPwmData(u32 freq)
** @Input     : freq:PWM频率
** @Output    : void
** @Function  : 输入PWM频率采样值
** @The notes :
**===========================================================================================*/
void InputGroupDaisyPwmData(u32 freq)
{
	gGReferSampInfo_200[eReferSampInfo200_PwmCycle] = freq;
}

/*=============================================================================================
** @Name      : void InputGroupLVSideData(e_BMULVSampADC index, s32 data)
** @Input     : index:低压端信息索引 data:低压端信息值
** @Output    : void
** @Function  : 输入低压端AD采样信息值
** @The notes :
**===========================================================================================*/
void InputGroupLVSideData(e_BMULVSampADC index, s32 data)
{
	s32 moment = 0;

	switch(index)
	{
		/*霍尔电流信号采集 1mV*/
		case eBMU_ADC_HalSIG:
			moment = CorrRealHalCurrByKB(data);
			//0.01mV转1mV
			moment = (moment + 50) / 100;
			//变化值在3mV—50mV之间
			if((ABS(gGDcCTInfo_65[eDcCTInfo65_HallSignV], moment) >= 3)
				&& (ABS(gGDcCTInfo_65[eDcCTInfo65_HallSignV], moment) <= 50))
			{
				gGDcCTInfo_65[eDcCTInfo65_HallSignV] = (((s32)gGDcCTInfo_65[eDcCTInfo65_HallSignV] * 12) + (moment * 20)) / 32;
			}
			else
			{
				gGDcCTInfo_65[eDcCTInfo65_HallSignV] = moment;
			}
			break;

		/*低压电源电压采样*/
		case eBMU_ADC_PwrV:
			gGLVSampInfo_66[eLVSamp66_PwrV] = data;
			break;

		/*激活信号ACC电压采样*/
		case eBMU_ADC_ACCV:
			gGLVSampInfo_66[eLVSamp66_AccV] = data;
	    	break;

		/*霍尔供电电压采样(5V)*/
		case eBMU_ADC_HalV:
			gGLVSampInfo_66[eLVSamp66_Hall5V] = data;
	    	break;

		/*充电枪KC供入电压采样*/
		case eBMU_ADC_CdcV:
			gGFstLeakInfo_67[eFstLeakInfo67_FcV] = data;
	    	break;

		/*充电枪CC2信号电压采样*/
		case eBMU_ADC_CC2V:
			gGFstLeakInfo_67[eFstLeakInfo67_CC2V] = data;
	    	break;

		/*MCU5V电源电压采样*/
		case eBMU_ADC_MCU5V:
			gGReferSampInfo_200[eReferSampInfo200_McuPwr5V] = data;
			break;

		/*电源管理芯片6503输出信号AUX(5V)*/
		case eBMU_ADC_Aux5V:
			gGReferSampInfo_200[eReferSampInfo200_SbcAux5V] = data;
			break;

		/*电源管理芯片MUX_OUT检测*/
		case eBMU_ADC_MUXV:
			gGReferSampInfo_200[eReferSampInfo200_SbcMuxV] = data;
			break;

		/*PWM电压ADC通道检测*/
		case eBMU_ADC_PWMV:
			gGReferSampInfo_200[eReferSampInfo200_PwmAdcV] = data;
			break;

		/*环境温度(系统环境温度)*/
		case eBMU_ADC_ENVIT:
			gGSysCellInfo_62[eSysCellInfo62_EnvT] = data;
			break;

		/*板上温度(低压电源侧温度)*/
		case eBMU_ADC_LPWRT:
			gGDcCTInfo_65[eDcCTInfo65_PowrT] = data;
			break;

		/*K1高边驱动电流采样*/
		case eBMU_ADC_HS0C:
			gGHsCurrInfo_68[eHsCurrInfo68_MHS0C] = data;
	    	break;

		/*K2高边驱动电流采样*/
		case eBMU_ADC_HS1C:
			gGHsCurrInfo_68[eHsCurrInfo68_MHS1C] = data;
	    	break;

		/*K3高边驱动电流采样*/
		case eBMU_ADC_HS2C:
			gGHsCurrInfo_68[eHsCurrInfo68_MHS2C] = data;
	    	break;

		/*K4高边驱动电流采样*/
		case eBMU_ADC_HS3C:
			gGHsCurrInfo_68[eHsCurrInfo68_MHS3C] = data;
	    	break;

		/*K5高边驱动电流采样*/
		case eBMU_ADC_HS4C:
			gGHsCurrInfo_69[eHsCurrInfo69_MHS4C] = data;
	    	break;

		/*K6高边驱动电流采样*/
		case eBMU_ADC_HS5C:
			gGHsCurrInfo_69[eHsCurrInfo69_MHS5C] = data;
	    	break;

		/*K7高边驱动电流采样*/
		case eBMU_ADC_HS6C:
			gGHsCurrInfo_69[eHsCurrInfo69_MHS6C] = data;
	    	break;

		 /*K8高边驱动电流采样*/
		case eBMU_ADC_HS7C:
			gGHsCurrInfo_69[eHsCurrInfo69_MHS7C] = data;
	    	break;

		default:
			break;
	}
}

/*=============================================================================================
** @Name      : void InputGPerImlu1SignAPI(e_IMLU1Info202 index, u16 info)
** @Input     : index:信号信息索引 data:信号信息值
** @Output    : void
** @Function  : 输入IMLU1信号信息值
** @The notes :
**===========================================================================================*/
void InputGPerImlu1SignAPI(e_IMLU1Info202 index, u16 info)
{
	if(index < eIMLU1Info202_Len)
	{
		/*更新值*/
		gGFanCtrlInfo_202[index] = info;
	}
}

/*=============================================================================================
** @Name      : u16 GetBsuCellVolt(u8 ecu, u8 cell)
** @Input     : ecu:从控节点号[0,31] cell:从控内电池号[0,15]
** @Output    : 从控单体当前电压值:1mV
** @Function  : 获取指定从控内部单体当前电压值
** @The notes :
**===========================================================================================*/
u16 GetBsuCellVolt(u8 ecu, u8 cell)
{
    u16 index = 0;

    if((ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])       /*节点号错误*/
        || (cell >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]))/*一个BSU电池节数错误*/
    {
        return(0);
    }

    /*从控内序号转换成组内序号*/
    index = ChangSlaveVidToGroupVid(ecu, cell);

    /*电池组总节数正确*/
    if(index < gGBmuHigLevPara_103[eBmuHigLevPara103_VSumNum])
    {
        return(gGCellVoltInfo_92[index]);
    }
    else
    {
        return(0);
    }
}

/*=============================================================================================
** @Name      : s8 GetBsuCellTemp(u8 ecu, u8 num)
** @Input     : ecu:从控节点号[0,31] num:从控内单体温度序号[0,3]
** @Output    : 从控单体当前温度值:1℃
** @Function  : 获取指定从控内部单体当前温度值
** @The notes :
**===========================================================================================*/
s8 GetBsuCellTemp(u8 ecu, u8 num)
{
    u16 index = 0;

    if((ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])       /*节点号错误*/
        || (num >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum])) /*一个从控单体温度号错误*/
    {
        return(-40);
    }

    /*从控内序号转换成组内序号*/
    index = ChangSlaveTidToGroupTid(ecu, num);

    /*电池组总单体温度数正确*/
    if(index < gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum])
    {
        return(gGCellTempInfo_94[index]);
    }
    else
    {
        return(-40);
    }
}

/*=============================================================================================
** @Name      : s8 GetBsuBalanTemp(u8 ecu, u8 num)
** @Input     : ecu:从控节点号[0,31] num:从控内均衡板温序号[0,1]
** @Output    : 从控均衡板温当前温度值:1℃
** @Function  : 获取指定从控内部均衡板温当前温度值
** @The notes :
**===========================================================================================*/
s8 GetBsuBalanTemp(u8 ecu, u8 num)
{
    u16 index = 0;

    if((ecu >= gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])       /*节点号错误*/
        || (num >= gGBmuHigLevPara_103[eBmuHigLevPara103_BalTNum])) /*一个从控单体温度号错误*/
    {
        return(-40);
    }

    /*计算电池组内总温度序号*/
    index = gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum] + ecu * gGBmuHigLevPara_103[eBmuHigLevPara103_BalTNum] + num;

    /*电池组总温度数正确*/
    if(num < (gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum] + gGBmuHigLevPara_103[eBmuHigLevPara103_BTSumNum]))
    {
        return(gGCellTempInfo_94[index]);
    }
    else
    {
        return(25);
    }
}

/*=================================================================================================
** @Name      : s16 GetGDcSideSampRealInfoAPI(e_ReferSampInfo200 index)
** @Input     : index:直流侧采样信息序号
** @Output    : 直流侧实际采样信息值
** @Function  : 获取电池组直流侧实际采样信息值
** @The notes : 采样中间参考值
**===============================================================================================*/
s16 GetGDcSideSampRealInfoAPI(e_ReferSampInfo200 index)
{
	if(index < eReferSampInfo200_Len)
	{
        return(gGReferSampInfo_200[index]);
	}
	else
	{
		return(0);
	}
}

/*=================================================================================================
** @Name      : u16 GetGPerImlu1SignInfoAPI(e_IMLU1Info202 index)
** @Input     : index:IMLU1信号信息序号
** @Output    : IMLU1信号信息值
** @Function  : 获取电池组IMLU1信号信息值
** @The notes : 前2个是IMLU1反馈值,其他是判断值
**===============================================================================================*/
u16 GetGPerImlu1SignInfoAPI(e_IMLU1Info202 index)
{
	if(index < eIMLU1Info202_Len)
	{
        return(gGFanCtrlInfo_202[index]);
	}
	else
	{
		return(0);
	}
}

/*=================================================================================================
** @Name      : u16 GetGProtectStateAPI(void)
** @Input     : void
** @Output    : 电池组保护状态[0,5]
** @Function  : 获取电池组当前保护状态
** @The notes : e_ProState
**===============================================================================================*/
u16 GetGProtectStateAPI(void)
{
    return(gGSysStateInfo_50[eSysStateInfo50_SysPro]);
}

/*=================================================================================================
** @Name      : u16 GetGWorkStateAPI(void)
** @Input     : void
** @Output    : 电池组工作状态[0,6]
** @Function  : 获取电池组当前工作状态
** @The notes : e_WorkState
**===============================================================================================*/
u16 GetGWorkStateAPI(void)
{
    return(gGSysStateInfo_50[eSysStateInfo50_SysWor]);
}

/*=================================================================================================
** @Name      : u16 GetGChgDhgStateAPI(void)
** @Input     : void
** @Output    : 电池组充放电状态[0,2]
** @Function  : 获取电池组当前充放电状态
** @The notes : e_CurrState
**===============================================================================================*/
u16 GetGChgDhgStateAPI(void)
{
    return(gGSysStateInfo_50[eSysStateInfo50_SysChg]);
}

/*=================================================================================================
** @Name      : u16 GetGWorkModeAPI(void)
** @Input     : void
** @Output    : 电池组工作模式[0,5]
** @Function  : 获取电池组当前工作模式
** @The notes : e_WorkMode
**===============================================================================================*/
u16 GetGWorkModeAPI(void)
{
    return(gGSysStateInfo_50[eSysStateInfo50_SysMod]);
}

/*=================================================================================================
** @Name      : u16 GetGChgLimCurrAPI(void)
** @Input     : void
** @Output    : 实时充电电流限值 0.1A
** @Function  : 获取电池组当前充电电流限值
** @The notes : 最大充电电流请求值
**===============================================================================================*/
u16 GetGChgLimCurrAPI(void)
{
    return(gGLimCPInfo_51[eLimCPInfo51_ChgC]);
}

/*=================================================================================================
** @Name      : u16 GetGDhgLimCurrAPI(void)
** @Input     : void
** @Output    : 实时放电电流限值 0.1A
** @Function  : 获取电池组当前放电电流限值
** @The notes : 最大放电电流请求值
**===============================================================================================*/
u16 GetGDhgLimCurrAPI(void)
{
    return(gGLimCPInfo_51[eLimCPInfo51_DhgC]);
}

/*=============================================================================================
** @Name      : u16 GetGChgLimPowerAPI(void)
** @Input     : void
** @Output    : 充电功率限值 0.1kW
** @Function  : 返回充电功率限值
** @The notes : 最大充电功率请求值
**===========================================================================================*/
u16 GetGChgLimPowerAPI(void)
{
    return(gGLimCPInfo_51[eLimCPInfo51_ChgP]);
}

/*=============================================================================================
** @Name      : u16 GetGDhgLimPowerAPI(void)
** @Input     : void
** @Output    : 放电功率限值 0.1kW
** @Function  : 返回放电功率限值
** @The notes : 最大放电功率请求值
**===========================================================================================*/
u16 GetGDhgLimPowerAPI(void)
{
    return(gGLimCPInfo_51[eLimCPInfo51_DhgP]);
}

/*=============================================================================================
** @Name      : u16 GetGChgLimSumVAPI(void)
** @Input     : void
** @Output    : 充电总压限值 0.1V
** @Function  : 返回充电总压限值
** @The notes :
**===========================================================================================*/
u16 GetGChgLimSumVAPI(void)
{
    return(gGLimVInfo_52[eLimVInfo52_ChgV]);
}

/*=============================================================================================
** @Name      : u16 GetGDhgLimSumVAPI(void)
** @Input     : void
** @Output    : 放电总压限值 0.1V
** @Function  : 返回放电总压限值
** @The notes :
**===========================================================================================*/
u16 GetGDhgLimSumVAPI(void)
{
    return(gGLimVInfo_52[eLimVInfo52_DhgV]);
}

/*=============================================================================================
** @Name      : u16 GetGDhgLimSumVAPI(void)
** @Input     : index:告警信息索引号[0,2]
** @Output    : 电池组告警信息
** @Function  : 返回电池组告警信息
** @The notes : 16位告警码
**===========================================================================================*/
u16 GetGAlarmInfoAPI(u8 index)
{
	if(index < GINDEX53_LEN)
	{
		return(gGAlarmInfo_53[index]);
	}
	else
	{
		return(0);
	}
}

/*=============================================================================================
** @Name      : u16 GetGFaultInfoAPI(void)
** @Input     : index:故障信息索引号[0,2]
** @Output    : 电池组故障信息
** @Function  : 返回电池组故障信息
** @The notes : 16位故障码
**===========================================================================================*/
u16 GetGFaultInfoAPI(u8 index)
{
	if(index < GINDEX54_LEN)
	{
		return(gGFaultInfo_54[index]);
	}
	else
	{
		return(0);
	}
}

/*=============================================================================================
** @Name      : u16 GetGInPortStateAPI(void)
** @Input     : void
** @Output    : 各输出IO端口当前状态值(按bit位表示 0分 1合)
** @Function  : 返回输出IO端口当前状态值
** @The notes :
**===========================================================================================*/
u16 GetGInPortStateAPI(void)
{
    return(gGPortStateInfo_56[ePortStateInfo56_Input]);
}

/*=============================================================================================
** @Name      : u16 GetGOutPortStateAPI(void)
** @Input     : void
** @Output    : 各输出IO端口当前状态值(按bit位表示 0分 1合)
** @Function  : 返回各输出IO端口当前状态值
** @The notes :
**===========================================================================================*/
u16 GetGOutPortStateAPI(void)
{
    return(gGPortStateInfo_56[ePortStateInfo56_Output]);
}

/*=================================================================================================
** @Name      : u16 GetGSampSumVoltAPI(void)
** @Input     : void
** @Output    : 电池组采样总电压 0.1V
** @Function  : 获取电池组采样总电压
** @The notes : 采样值
**===============================================================================================*/
u16 GetGSampSumVoltAPI(void)
{
    return((u16)gGSysMainInfo_60[eSysMainInfo60_SumV]);
}

/*=================================================================================================
** @Name      : s16 GetGSampOutCurrAPI(void)
** @Input     : void
** @Output    : 电池组采样输出电流 0.1A
** @Function  : 获取电池组采样输出电流
** @The notes : 负号为充电
**===============================================================================================*/
s16 GetGSampOutCurrAPI(void)
{
    return(gGSysMainInfo_60[eSysMainInfo60_OutC]);
}

/*=================================================================================================
** @Name      : s16 GetGSampOutPowerAPI(void)
** @Input     : void
** @Output    : 电池组输出功率 0.1kW
** @Function  : 获取电池组输出功率值
** @The notes : 负号为充电
**===============================================================================================*/
s16 GetGSampOutPowerAPI(void)
{
    return(gGSysMainInfo_60[eSysMainInfo60_OutP]);
}

/*=================================================================================================
** @Name      : u16 GetGSampPreVoltAPI(void)
** @Input     : void
** @Output    : 电池组外端预充电压 0.1V
** @Function  : 获取电池组外端预充电压值
** @The notes : 采样值
**===============================================================================================*/
u16 GetGSampPreVoltAPI(void)
{
    return(gGSysMainInfo_60[eSysMainInfo60_PreV]);
}

/*=================================================================================================
** @Name      : u16 GetGBattSocAPI(void)
** @Input     : void
** @Output    : 电池组SOC 0.1%
** @Function  : 获取电池组SOC值
** @The notes :
**===============================================================================================*/
u16 GetGBattSocAPI(void)
{
    return(gGSysCapInfo_61[eSysCapInfo61_SOC]);
}

/*=================================================================================================
** @Name      : u16 GetGBattSoeAPI(void)
** @Input     : void
** @Output    : 电池组SOE 0.1%
** @Function  : 获取电池组SOE值
** @The notes :
**===============================================================================================*/
u16 GetGBattSoeAPI(void)
{
    return(gGSysCapInfo_61[eSysCapInfo61_SOE]);
}

/*=================================================================================================
** @Name      : u16 GetGBattSoeAPI(void)
** @Input     : void
** @Output    : 电池组SOH 0.1%
** @Function  : 获取电池组SOH值
** @The notes :
**===============================================================================================*/
u16 GetGBattSohAPI(void)
{
    return(gGSysCapInfo_61[eSysCapInfo61_SOH]);
}

/*=================================================================================================
** @Name      : u16 GetGBattAllCapAPI(void)
** @Input     : void
** @Output    : 电池组总容量 0.1AH
** @Function  : 获取电池组总容量值
** @The notes : 实测值
**===============================================================================================*/
u16 GetGBattAllCapAPI(void)
{
    return(gGSysCapInfo_61[eSysCapInfo61_AllCap]);
}

/*=================================================================================================
** @Name      : u16 GetGCellSumVoltAPI(void)
** @Input     : void
** @Output    : 电池组求和总电压 0.1V
** @Function  : 获取电池组求和总电压
** @The notes : 单体累加值
**===============================================================================================*/
u16 GetGCellSumVoltAPI(void)
{
    return((u16)gGSysCellInfo_62[eSysCellInfo62_SumV]);
}

/*=================================================================================================
** @Name      : u16 GetGCellAvgVoltAPI(void)
** @Input     : void
** @Output    : 电池组单体平均电压 1mV
** @Function  : 获取电池组单体平均电压
** @The notes : 单体累加平均值
**===============================================================================================*/
u16 GetGCellAvgVoltAPI(void)
{
    return((u16)gGSysCellInfo_62[eSysCellInfo62_AvgV]);
}

/*=================================================================================================
** @Name      : s16 GetGCellAvgTempAPI(void)
** @Input     : void
** @Output    : 电池组单体平均温度 1℃
** @Function  : 获取电池组单体平均温度
** @The notes : 单体累加平均值
**===============================================================================================*/
s16 GetGCellAvgTempAPI(void)
{
    return(gGSysCellInfo_62[eSysCellInfo62_AvgT]);
}

/*=================================================================================================
** @Name      : s16 GetGEnvirTempAPI(void)
** @Input     : void
** @Output    : 电池组环境温度 1℃
** @Function  : 获取电池组环境温度
** @The notes : 第1路外围温度
**===============================================================================================*/
s16 GetGEnvirTempAPI(void)
{
    return(gGSysCellInfo_62[eSysCellInfo62_EnvT]);
}

/*=================================================================================================
** @Name      : u16 GetGCellMaxVNumAPI(void)
** @Input     : void
** @Output    : 电池组单体最大电压电池号[1,400]
** @Function  : 获取电池组单体最大电压电池号
** @The notes :
**===============================================================================================*/
u16 GetGCellMaxVNumAPI(void)
{
    return(gGVoltCharInfo_63[eVoltCharInfo63_MaxVNum]);
}

/*=================================================================================================
** @Name      : u16 GetGCellMaxVoltAPI(void)
** @Input     : void
** @Output    : 电池组单体最大电压值 1mV
** @Function  : 获取电池组单体最大电压值
** @The notes :
**===============================================================================================*/
u16 GetGCellMaxVoltAPI(void)
{
    return(gGVoltCharInfo_63[eVoltCharInfo63_MaxVolt]);
}

/*=================================================================================================
** @Name      : u16 GetGCellMaxVNumAPI(void)
** @Input     : void
** @Output    : 电池组单体最小电压电池号[1,400]
** @Function  : 获取电池组单体最小电压电池号
** @The notes :
**===============================================================================================*/
u16 GetGCellMinVNumAPI(void)
{
    return(gGVoltCharInfo_63[eVoltCharInfo63_MinVNum]);
}

/*=================================================================================================
** @Name      : u16 GetGCellMinVoltAPI(void)
** @Input     : void
** @Output    : 电池组单体最小电压值 1mV
** @Function  : 获取电池组单体最小电压值
** @The notes :
**===============================================================================================*/
u16 GetGCellMinVoltAPI(void)
{
    return(gGVoltCharInfo_63[eVoltCharInfo63_MinVolt]);
}

/*=================================================================================================
** @Name      : u16 GetGCellMaxTNumAPI(void)
** @Input     : void
** @Output    : 电池组单体最高温度序号[1,400]
** @Function  : 获取电池组单体最高温度序号
** @The notes :
**===============================================================================================*/
u16 GetGCellMaxTNumAPI(void)
{
    return((u16)gGTempCharInfo_64[eTempCharInfo64_MaxTNum]);
}

/*=================================================================================================
** @Name      : s16 GetGCellMaxTempAPI(void)
** @Input     : void
** @Output    : 电池组单体最高温度值 1℃
** @Function  : 获取电池组单体最高温度值
** @The notes :
**===============================================================================================*/
s16 GetGCellMaxTempAPI(void)
{
    return(gGTempCharInfo_64[eTempCharInfo64_MaxTemp]);
}

/*=================================================================================================
** @Name      : u16 GetGCellMinTNumAPI(void)
** @Input     : void
** @Output    : 电池组单体最低温度序号[1,400]
** @Function  : 获取电池组单体最低温度序号
** @The notes :
**===============================================================================================*/
u16 GetGCellMinTNumAPI(void)
{
    return((u16)gGTempCharInfo_64[eTempCharInfo64_MinTNum]);
}

/*=================================================================================================
** @Name      : s16 GetGCellMinTempAPI(void)
** @Input     : void
** @Output    : 电池组单体最低温度值 1℃
** @Function  : 获取电池组单体最低温度值
** @The notes :
**===============================================================================================*/
s16 GetGCellMinTempAPI(void)
{
    return(gGTempCharInfo_64[eTempCharInfo64_MinTemp]);
}

/*=================================================================================================
** @Name      : s16 GetGCellBoxMaxTempAPI(u8 slave)
** @Input     : slave:从控号
** @Output    : 电池组一个电池箱中最大温度值 1℃
** @Function  : 获取电池组一个电池箱中最大温度值
** @The notes : for循环查找
**===============================================================================================*/
s16 GetGCellBoxMaxTempAPI(u8 slave)
{
	u8 i = 0;
	u16 index = 0;
	s16 maxTemp = -41;

	index = slave * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum];

	//轮询一个电池箱单体温度
	for(i = 0; i < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum]; i++)
	{
		if(gGCellTempInfo_94[index + i] > maxTemp)
		{
			maxTemp = gGCellTempInfo_94[index + i];
		}
	}

    return(maxTemp);
}

/*=================================================================================================
** @Name      : u16 GetGHallSignVoltAPI(void)
** @Input     : void
** @Output    : 电池组霍尔电流源信号电压采样值 1mV
** @Function  : 获取电池组霍尔电流源信号电压采样值
** @The notes :
**===============================================================================================*/
u16 GetGHallSignVoltAPI(void)
{
    return(gGDcCTInfo_65[eDcCTInfo65_HallSignV]);
}

/*=================================================================================================
** @Name      : s16 GetGShuntSampCurrAPI(void)
** @Input     : void
** @Output    : 电池组分流器电流源采样值 0.01A
** @Function  : 获取电池组分流器电流源采样值
** @The notes :
**===============================================================================================*/
s16 GetGShuntSampCurrAPI(void)
{
    return(gGDcCTInfo_65[eDcCTInfo65_ShutSignV]);
}

/*=================================================================================================
** @Name      : s16 GetGShuntSampTempAPI(void)
** @Input     : void
** @Output    : 电池组分流器温度采样值 1℃
** @Function  : 获取电池组分流器温度采样值
** @The notes :
**===============================================================================================*/
s16 GetGShuntSampTempAPI(void)
{
    return(gGDcCTInfo_65[eDcCTInfo65_ShutT]);
}

/*=================================================================================================
** @Name      : s16 GetGDcPwrSampTempAPI(void)
** @Input     : void
** @Output    : 电池组主控低压电源温度采样值 1℃
** @Function  : 获取电池组主控低压电源温度采样值
** @The notes :
**===============================================================================================*/
s16 GetGDcPwrSampTempAPI(void)
{
    return(gGDcCTInfo_65[eDcCTInfo65_PowrT]);
}

/*=================================================================================================
** @Name      : u16 GetGPgndNegVoltAPI(void)
** @Input     : void
** @Output    : 电池组负母线电压值[0,5000]0.1V
** @Function  : 获取电池组负母线电压值
** @The notes :
**===============================================================================================*/
u16 GetGPgndNegVoltAPI(void)
{
    return((u16)gGLVSampInfo_66[eLVSamp66_NegV]);
}

/*=================================================================================================
** @Name      : u16 GetGLowAccVoltAPI(void)
** @Input     : void
** @Output    : 电池组激活信号电压值[0,3600]0.01V
** @Function  : 获取电池组激活信号电压值
** @The notes :
**===============================================================================================*/
u16 GetGLowAccVoltAPI(void)
{
    return((u16)gGLVSampInfo_66[eLVSamp66_AccV]);
}

/*=================================================================================================
** @Name      : u16 GetGLowHall5VVoltAPI(void)
** @Input     : void
** @Output    : 电池组霍尔5V基准电压值[0,500]0.01V
** @Function  : 获取电池组霍尔5V基准电压采样值
** @The notes :
**===============================================================================================*/
u16 GetGLowHall5VVoltAPI(void)
{
    return((u16)gGLVSampInfo_66[eLVSamp66_Hall5V]);
}

/*=================================================================================================
** @Name      : u16 GetGFstChgFCVoltAPI(void)
** @Input     : void
** @Output    : 电池组快充枪接入电压值[0,3600]0.01V
** @Function  : 获取电池组快充枪接入电压值
** @The notes :
**===============================================================================================*/
u16 GetGFstChgFCVoltAPI(void)
{
    return(gGFstLeakInfo_67[eFstLeakInfo67_FcV]);
}

/*=================================================================================================
** @Name      : u16 GetGFstChgCC2VoltAPI(void)
** @Input     : void
** @Output    : 电池组快充枪CC2信号电压值[0,3600]0.01V
** @Function  : 获取电池组快充枪CC2信号电压值
** @The notes :
**===============================================================================================*/
u16 GetGFstChgCC2VoltAPI(void)
{
    return(gGFstLeakInfo_67[eFstLeakInfo67_CC2V]);
}

/*=================================================================================================
** @Name      : u16 GetGPasInsuResistAPI(void)
** @Input     : void
** @Output    : 电池组正极绝缘值[0,30000]1kΩ
** @Function  : 获取电池组电池正极绝缘值
** @The notes :
**===============================================================================================*/
u16 GetGPasInsuResistAPI(void)
{
    return(gGFstLeakInfo_67[eFstLeakInfo67_PasInsu]);
}

/*=================================================================================================
** @Name      : u16 GetGNegInsuResistAPI(void)
** @Input     : void
** @Output    : 电池组负极绝缘值[0,30000]1kΩ
** @Function  : 获取电池组电池负极绝缘值
** @The notes :
**===============================================================================================*/
u16 GetGNegInsuResistAPI(void)
{
    return(gGFstLeakInfo_67[eFstLeakInfo67_NegInsu]);
}

/*=================================================================================================
** @Name      : u16 GetGNegInsulationAPI(void)
** @Input     : channel:高边开关通道号[0,7]
** @Output    : 电池组高边开关驱动电流值[-1000,1000] 1mA
** @Function  : 获取电池组高边开关驱动电流值
** @The notes :
**===============================================================================================*/
s16 GetGHSDriverCurrAPI(u8 channel)
{
	/*通道0-3*/
	if(channel < 4)
	{
        return(gGHsCurrInfo_68[eHsCurrInfo68_MHS0C + channel]);
	}
	/*通道4-7*/
	else if(channel < 7)
	{
        return(gGHsCurrInfo_69[eHsCurrInfo69_MHS4C + channel - 4]);
	}
	/*无效通道*/
	else
	{
		return(0);
	}
}

/*=================================================================================================
** @Name      : u16 GetGCanChgTimeAPI(void)
** @Input     : void
** @Output    : 电池组可充电时间 1min
** @Function  : 获取电池组可充电时间
** @The notes :
**===============================================================================================*/
u16 GetGCanChgTimeAPI(void)
{
    return(gGStatistTime_70[eTime70_CanChg]);
}

/*=================================================================================================
** @Name      : u16 GetGCanDhgTimeAPI(void)
** @Input     : void
** @Output    : 电池组可放电时间 1min
** @Function  : 获取电池组可放电时间
** @The notes :
**===============================================================================================*/
u16 GetGCanDhgTimeAPI(void)
{
    return(gGStatistTime_70[eTime70_CanDhg]);
}

/*=================================================================================================
** @Name      : u16 GetGChgCycleTimesAPI(void)
** @Input     : void
** @Output    : 电池组充放电循环次数 1次
** @Function  : 获取电池组充放电循环次数
** @The notes :
**===============================================================================================*/
u16 GetGChgCycleTimesAPI(void)
{
    return(gGStatistTime_70[eTime70_ChgCycle]);
}

/*=================================================================================================
** @Name      : u32 GetGHisErrSumNumAPI(void)
** @Input     : void
** @Output    : 电池组累计故障总数目 1个
** @Function  : 获取电池组累计故障总数目
** @The notes : 包括告警数目和故障数目
**===============================================================================================*/
u32 GetGHisErrSumNumAPI(void)
{
    return(gGStatistErr_71[eErr71_HisWarNum] + gGStatistErr_71[eErr71_HisErrNum]);
}

/*=================================================================================================
** @Name      : u32 GetGHisChgCapAPI(void)
** @Input     : void
** @Output    : 电池组累计充电电量 1Ah
** @Function  : 获取电池组累计充电电量
** @The notes :
**===============================================================================================*/
u32 GetGHisChgCapAPI(void)
{
    return(gGStatistHisCap_73[eHisCap73_HisChgCap]);
}

/*=================================================================================================
** @Name      : u32 GetGHisDhgCapAPI(void)
** @Input     : void
** @Output    : 电池组累计放电电量 1Ah
** @Function  : 获取电池组累计放电电量
** @The notes :
**===============================================================================================*/
u32 GetGHisDhgCapAPI(void)
{
    return(gGStatistHisCap_73[eHisCap73_HisDhgCap]);
}

/*=================================================================================================
** @Name      : u32 GetGHisChgEnerAPI(void)
** @Input     : void
** @Output    : 电池组累计充电电能 0.1kWh
** @Function  : 获取电池组累计充电电能
** @The notes :
**===============================================================================================*/
u32 GetGHisChgEnerAPI(void)
{
    return(gGStatistHisEner_74[eHisEner74_HisChgEner]);
}

/*=================================================================================================
** @Name      : u32 GetGHisDhgEnerAPI(void)
** @Input     : void
** @Output    : 电池组累计放电电能 0.1kWh
** @Function  : 获取电池组累计放电电能
** @The notes :
**===============================================================================================*/
u32 GetGHisDhgEnerAPI(void)
{
    return(gGStatistHisEner_74[eHisEner74_HisDhgEner]);
}

/*=================================================================================================
** @Name      : u16 GetGStatistNowDodAPI(void)
** @Input     : void
** @Output    : 电池组当前放电深度DOD 0.1%
** @Function  : 获取电池组当前放电深度DOD
** @The notes :
**===============================================================================================*/
u16 GetGStatistNowDodAPI(void)
{
    return(gGStatistDodInfo_75[eDodStat75_NowDod]);
}

/*=================================================================================================
** @Name      : u16 GetGStatistNowDodAPI(void)
** @Input     : void
** @Output    : 电池组最大放电深度DOD 0.1%
** @Function  : 获取电池组最大放电深度DOD
** @The notes :
**===============================================================================================*/
u16 GetGStatistMaxDodAPI(void)
{
    return(gGStatistDodInfo_75[eDodStat75_MaxDod]);
}

/*=================================================================================================
** @Name      : u16 GetGSbcAux5VVoltAPI(void)
** @Input     : void
** @Output    : 电池组SBC5V基准电压采样值 0.01V
** @Function  : 获取电池组SBC5V基准电压采样值
** @The notes :
**===============================================================================================*/
u16 GetGSbcAux5VVoltAPI(void)
{
    return((u16)gGReferSampInfo_200[eReferSampInfo200_SbcAux5V]);
}

/*=================================================================================================
** @Name      : s8 GetGBalanMaxTempAPI(void)
** @Input     : void
** @Output    : 电池组均衡板温最大值 1℃
** @Function  : 获取电池组均衡板温最大值
** @The notes :
**===============================================================================================*/
s8 GetGBalanMaxTempAPI(void)
{
    return(gGCellBalanTInfo_201[eCellBalanT201_GMaxTemp]);
}

/*=================================================================================================
** @Name      : u8 GetGBalanMaxTSlaveAPI(void)
** @Input     : void
** @Output    : 电池组最大均衡板温从控号[1,400]
** @Function  : 获取电池组最大均衡板温从控
** @The notes :
**===============================================================================================*/
u8 GetGBalanMaxTSlaveAPI(void)
{
    return((u8)gGCellBalanTInfo_201[eCellBalanT201_GMaxTSlav]);
}

/*=================================================================================================
** @Name      : u8 GetGHigPGNDSwhStateAPI(void)
** @Input     : void
** @Output    : 电池组负极PGND继电器状态 0:断开 1:闭合
** @Function  : 获取电池组负极PGND继电器状态
** @The notes :
**===============================================================================================*/
u8 GetGHigPGNDSwhStateAPI(void)
{
    return(BitGet(gGPortStateInfo_56[ePortStateInfo56_Output], 15));
}

/*=================================================================================================
** @Name      : u16 GetGChgBalanStateAPI(u16 index)
** @Input     : index:索引号[0,24]
** @Output    : 电池组单体充电均衡状态(按bit位表示 0关闭 1均充)
** @Function  : 获取电池组单体充电均衡状态
** @The notes : 指定索引非从控序号
**===============================================================================================*/
u16 GetGChgBalanStateAPI(u16 index)
{
	if(index < GINDEX90_LEN)
	{
        return(gGChgBalanInfo_90[index]);
	}

	return(0);
}

/*=================================================================================================
** @Name      : u16 GetGDhgBalanStateAPI(u16 index)
** @Input     : index:索引号[0,24]
** @Output    : 电池组单体放电均衡状态(按bit位表示 0关闭 1均放)
** @Function  : 获取电池组单体放电均衡状态
** @The notes : 指定索引非从控序号
**===============================================================================================*/
u16 GetGDhgBalanStateAPI(u16 index)
{
	if(index < GINDEX91_LEN)
	{
        return(gGDhgBalanInfo_91[index]);
	}

	return(0);
}

/*=================================================================================================
** @Name      : u16 GetGCellVoltAPI(void)
** @Input     : cell:电池号[0,399]
** @Output    : 电池组单体电压值 1mV
** @Function  : 获取电池组单体电压值
** @The notes :
**===============================================================================================*/
u16 GetGCellVoltAPI(u16 cell)
{
	if(cell < gGBmuHigLevPara_103[eBmuHigLevPara103_VSumNum])
	{
        return(gGCellVoltInfo_92[cell]);
	}

	return(0);
}

/*=================================================================================================
** @Name      : s8 GetGCellTempAPI(u16 cell)
** @Input     : cell:电池号[0,399]
** @Output    : 电池组单体温度值 1℃
** @Function  : 获取电池组单体温度值
** @The notes :
**===============================================================================================*/
s8 GetGCellTempAPI(u16 cell)
{
	if(cell < gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum])
	{
        return(gGCellTempInfo_94[cell]);
	}

	return(0);
}

/*=================================================================================================
** @Name      : s8 GetGBalanTempAPI(u16 cell)
** @Input     : cell:电池号[0,63]
** @Output    : 电池组单体均衡温度值 1℃
** @Function  : 获取电池组单体均衡温度值
** @The notes :
**===============================================================================================*/
s8 GetGBalanTempAPI(u16 cell)
{
	if(cell < gGBmuHigLevPara_103[eBmuHigLevPara103_BTSumNum])
	{
        return(gGCellTempInfo_94[gGBmuHigLevPara_103[eBmuHigLevPara103_TSumNum] + cell]);
	}

	return(0);
}

/*=================================================================================================
** @Name      : s8 GetGBalanSlavMaxTAPI(u8 slave)
** @Input     : slave:从控号[0,31]
** @Output    : 电池组单体均衡温度从控内部最大温度值 1℃
** @Function  : 获取电池组单体均衡温度从控内部最大温度值
** @The notes :
**===============================================================================================*/
s8 GetGBalanSlavMaxTAPI(u8 slave)
{
	if(slave < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuNum])
	{
        return(gGCellBalanTInfo_201[eCellBalanT201_SMaxTemp0 + slave]);
	}

	return(0);
}

/*=================================================================================================
** @Static function definition
**===============================================================================================*/
/*=================================================================================================
** @Name      : u16 ChangSlaveVidToGroupVid(u8 slave, u8 cell)
** @Input     : slave:从控号[0,31] cell:从控内电池号[0,17]
** @Output    : 从控内电压序号对应组内电压序号[0,399],0xffff:错误序号
** @Function  : 将从控内电压序号转换成电池组内电压序号
** @The notes : 每个从控电压数目不一样的需要特殊处理
**===============================================================================================*/
u16 ChangSlaveVidToGroupVid(u8 slave, u8 cell)
{
	u16 index = 0;

	//兼容CATL液冷PACK电池52串解析(16+18+18)
	#if(5 == PRJ_PARA_NUM_INFO)
	{
		//每3个第一个只有16节,越界非法
		if((0 == (slave % 3)) && (cell >= 16))
		{
			//错误序号
			index = 0xffff;
		}
		//每3个从控首个少2节(即第2个起每3个从控(52S)前移2S)
		else if(slave > 0)
		{
			//index = (slave - 1) * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum];
			index = (slave - 1) * 18;
			index = index - (index / 54 * 2) + cell + 16;
		}
		else
		{
			//index = slave * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum] + cell;
			index = cell;
		}
	}
	#else
	{
		index = slave * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum] + cell;
	}
    #endif

	return(index);
}

/*=================================================================================================
** @Name      : u16 ChangSlaveTidToGroupVid(u8 slave, u8 cell)
** @Input     : slave:从控号[0,31] cell:从控内温度号[0,8]
** @Output    : 从控内温度序号对应组内温度序号[0,199],0xffff:错误序号
** @Function  : 将从控内温度序号转换成电池组内温度序号
** @The notes : 每个从控温度数目不一样的需要特殊处理
**===============================================================================================*/
u16 ChangSlaveTidToGroupTid(u8 slave, u8 cell)
{
	u16 index = 0;

	//兼容CATL液冷PACK电池8个温度解析(2+3+3)
	#if(5 == PRJ_PARA_NUM_INFO)
	{
		//每3个第一个只有2节,越界非法
		if((0 == (slave % 3)) && (cell >= 2))
		{
			//错误序号
			index = 0xffff;
		}
		//每3个从控首个少1个温度(即第2个起每3个从控(8个温度)前移1节)
		else if(slave > 0)
		{
			//index = (slave - 1) * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum];
			index = (slave - 1) * 3;
			//index = index - (index / 9 * 1) + cell + 2;
			index = index - (index / 9) + cell + 2;
		}
		else
		{
			//index = slave * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum] + cell;
			index = cell;
		}
	}
	#else
	{
	    index = slave * gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum] + cell;
	}
    #endif

	return(index);
}
