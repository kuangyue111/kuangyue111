/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : CellAFESamp.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : 单体电压采样模块
** @Instructions :
**===========================================================================================*/
/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "BSPUserAPI.h"
#include "BSPCOS.h"
#include "BSPSPI.h"
#include "DEVGPIO.h"
#include "SysState.h"
#include "DiagCheck.h"
#include "DiagInterface.h"
#include "BalanCtrl.h"
#include "EepData.h"
#include "BSPUART.h"
//#include "BQ79616.h"
#include "BQ79600.h"
#include "bq79616_spi.h"
#include "SystemInfo.h"
#include "CellAFE79616Samp.h"

#define BALANCE_TEST      (1) //均衡测试，置1表示均衡指令一个一个发送
/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
extern u16 gGBmuGenPara_102[];                                      /*主控通用参数值*/
extern u16 gGBmuHigLevPara_103[];                                   /*主控高级参数值*/
extern u8 UartRcvDataBuff[];                                        /*串口接收数据buff*/

extern u16 autoaddr_response_frame[];                               //response frame for auto-addressing sequence

#if(0 == PORT_COMMTYPE_CHOICE)
static u8 response_frame[(16*2+6)*TOTALBOARDS];                     //BQ79616 hold all 16 vcell*_hi/lo values
#else
static u16 response_frame[(128+6)*TOTALBOARDS];                     //store 128 bytes + 6 header bytes for each board
#endif
#if(1 == BQ79616_BUSBAR_EN)
static u16 Bq79616BusBarVolt[16][1] = {0};                          //busbar电压值
#endif
static u16 Bq79616CellVolt[16][16] = {0};                           //电池电压值
static u16 Bq79616GpioVolt[16][16] = {0};                           //GPIO电压值
//static u8 BQ79616SampAlarmFlag = 0;                                //IC采样异常标志
static u8 BQ79616SampVAlarmFlag[MAX_DASIY_NUM] = {0};               //IC采样电压异常标志
static u8 BQ79616SampTAlarmFlag[MAX_DASIY_NUM] = {0};               //IC采样温度异常标志

static u8 BQ79616SumIcNum = 0;                                      //不修改底层接口，改用参数传送
uint64_t Data1 = 0;
uint64_t Data2 = 0;


static u8 BQ79616BalanceCmd[15] = {0};
/*根据项目选择单体温感对应温度表*/
#if(1 == PRJ_PARA_NUM_INFO)
/*亿纬电池项目*/
static const u32 sNTCTempTabR[] =                                   //亿纬电池NTC(10K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	221500,//-41℃
	204700, 193400, 182700, 172700, 163300, 154400, 146100, 138400, 131000, 124100,
	117600, 111500, 105800, 100400, 95250, 90440, 85900, 81610, 77570, 73750,
	70150, 66740, 63520, 60480,57600, 54870,52290, 49850, 47540, 45350,
	43270, 41300, 39440, 37670, 35990, 34390, 32880, 31440, 30070, 28770,
	27530, 26360, 25240, 24180, 23160, 22200, 21280, 20410, 19570, 18780,
	18020, 17300, 16610, 15950, 15320, 14720, 14150, 13600, 13080, 12580,
	12100, 11640, 11200, 10780, 10380, 10000, 9633, 9281, 8944, 8621,
	8311, 8015, 7730, 7457, 7195, 6944, 6703, 6471, 6249, 6035,
	5830, 5633, 5443, 5261, 5086, 4918, 4756, 4600, 4450, 4305,
	4166, 4033, 3904, 3780, 3660, 3545, 3434, 3327, 3224, 3124,
	3028, 2935, 2846, 2760, 2677, 2596, 2519, 2444, 2371, 2301,
	2234, 2169, 2106, 2045, 1986, 1929, 1874, 1820, 1769, 1719,
	1671, 1624, 1579, 1535, 1492, 1451, 1412, 1373, 1336, 1300,
	1265, 1231, 1198, 1166, 1135, 1105, 1076, 1048, 1020, 994,
	968, 943, 919, 895, 872, 850, 828, 808, 787, 768,
	748, 730, 712, 694, 677, 660, 644, 629, 613, 598,
	584, 570, 556, 543, 530, 518
};
#elif((2 == PRJ_PARA_NUM_INFO) || (5 == PRJ_PARA_NUM_INFO) || (7 == PRJ_PARA_NUM_INFO))
/*CATL240AH电池项目/CATL280AH液冷PACK项目/CATL280AH标准PACK项目*/
static const u32 sNTCTempTabR[] =                                   //CATL电池NTC(10K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	356000,//-41℃
	333562, 312315, 292550, 274155, 257027, 241071, 226202, 212340, 199410, 187344,
	176081, 165562, 155735, 146549, 137960, 129925, 122406, 115366, 108774, 102597,
	96807, 91378, 86286, 81507, 77021, 72808, 68851, 65131, 61635, 58346,
	55252, 52340, 49599, 47017, 44584, 42292, 40130, 38092, 36168, 34353,
	32639, 31021, 29492, 28047, 26682, 25390, 24168, 23012, 21918, 20882,
	19901, 18972, 18091, 17256, 16464, 15713, 15000, 14324, 13682, 13072,
	12493, 11943, 11419, 10922, 10449, 10000, 9572, 9164, 8776, 8407,
	8055, 7720, 7401, 7097, 6806, 6530, 6265, 6013, 5773, 5543,
	5324, 5115, 4915, 4723, 4541, 4366, 4100, 4039, 3886, 3740,
	3600, 3466, 3338, 3215, 3097, 2984, 2876, 2772, 2673, 2578,
	2486, 2398, 2314, 2233, 2156, 2081, 2010, 1941, 1875, 1812,
	1751, 1692, 1636, 1581, 1529, 1479, 1431, 1384, 1340, 1297,
	1255, 1215, 1177, 1140, 1104, 1070, 1036, 1004, 974, 944,
	915, 887, 861, 835, 810, 786, 763, 740, 719, 698,
	678, 658, 639, 621, 603, 586, 570, 554, 538, 523,
	509, 495, 482, 468, 456, 443, 432, 420, 409, 398,
	388, 377, 368, 358, 349, 340
};
#elif((3 == PRJ_PARA_NUM_INFO) || (4 == PRJ_PARA_NUM_INFO))
/*标准PACK项目*/
static const u32 sNTCTempTabR[] =                                   //REPT电池NTC(10K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	204050,//-41℃
	193160, 182910, 173260, 164160, 155590, 147510, 139890, 132700, 125920, 119520,
	113480, 107780, 102400, 97312, 92507, 87964, 83670, 79609, 75767, 72132,
	68691, 65433, 62348, 59425, 56656, 54031, 51542, 49181, 46942, 44817,
	42800, 40885, 39066, 37339, 35697, 34136, 32652, 31241, 29899, 28621,
	27405, 26248, 25145, 24095, 23095, 22141, 21232, 20365, 19538, 18750,
	17997, 17279, 16593, 15938, 15312, 14714, 14143, 13597, 13075, 12575,
	12098, 11641, 11203, 10785, 10384, 10000, 9632, 9280, 8943, 8619,
	8309, 8012, 7727, 7453, 7191, 6939, 6697, 6465, 6242, 6028,
	5822, 5625, 5435, 5253, 5077, 4908, 4746, 4590, 4440, 4296,
	4157, 4023, 3894, 3770, 3650, 3535, 3424, 3317, 3214, 3115,
	3019, 2927, 2838, 2752, 2669, 2589, 2511, 2437, 2365, 2295,
	2228, 2163, 2101, 2040, 1981, 1925, 1870, 1817, 1766, 1716,
	1669, 1622, 1577, 1534, 1492, 1451, 1412, 1374, 1337, 1301,
	1266, 1233, 1200, 1169, 1138, 1109, 1080, 1052, 1025, 999,
	973, 949, 925, 902, 879, 857, 836, 816, 796, 776,
	758, 739, 722, 705, 688, 672, 656, 641, 626, 612,
	598, 585, 572, 559, 547, 535
};
#elif(6 == PRJ_PARA_NUM_INFO)
/*CATL240AH电池百城改造项目*/
static const u32 sNTCTempTabR[] =                                   //CATL电池NTC(100K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	4422000,//-41℃
    4123445, 3846898, 3590500, 3352675, 3131979, 2927087, 2736784, 2559953, 2395568, 2242688,
    2100446, 1968136, 1844753, 1729894, 1622847, 1523039, 1429942, 1343069, 1261972,1186235,
    1115477, 1049344, 987508, 929669, 875547, 824883, 777438, 732992, 691338, 652287,
    615662, 581301, 549051, 518771, 490332, 463611, 438497, 414883, 392673, 371776,
    352107, 333588, 316146, 299713, 284225, 269624, 255852, 242861, 230600, 219026,
    208096, 197771, 188016, 178794, 170076, 161829, 154028, 146645, 139655, 133037,
    126768, 120828, 115199, 109862, 104800, 100000, 95444, 91121, 87016, 83118,
    79415, 75897, 72553, 69375, 66352, 63477, 60741, 58138, 55660, 53300,
    51053, 48912, 46872, 44928, 43074, 41307, 39621, 38012, 36477, 35012,
    33614, 32278, 31002, 29783, 28618, 27505, 26441, 25423, 24449, 23518,
    22627, 21774, 20957, 20175, 19426, 18709, 18022, 17363, 16732, 16127,
    15546, 14990, 14456, 13943, 13452, 12980, 12527, 12092, 11674, 11272,
    10887, 10516, 10160, 9817, 9488, 9172, 8867, 8574, 8292, 8021,
    7760, 7508, 7266, 7033, 6808, 6592, 6384, 6183, 5989, 5803,
    5623, 5449, 5282, 5120, 4964, 4814
};
#else
/*其他默认CATL240AH电池*/
static const u32 sNTCTempTabR[] =                                   //CATL电池NTC(10K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	356000,//-41℃
	333562, 312315, 292550, 274155, 257027, 241071, 226202, 212340, 199410, 187344,
	176081, 165562, 155735, 146549, 137960, 129925, 122406, 115366, 108774, 102597,
	96807, 91378, 86286, 81507, 77021, 72808, 68851, 65131, 61635, 58346,
	55252, 52340, 49599, 47017, 44584, 42292, 40130, 38092, 36168, 34353,
	32639, 31021, 29492, 28047, 26682, 25390, 24168, 23012, 21918, 20882,
	19901, 18972, 18091, 17256, 16464, 15713, 15000, 14324, 13682, 13072,
	12493, 11943, 11419, 10922, 10449, 10000, 9572, 9164, 8776, 8407,
	8055, 7720, 7401, 7097, 6806, 6530, 6265, 6013, 5773, 5543,
	5324, 5115, 4915, 4723, 4541, 4366, 4100, 4039, 3886, 3740,
	3600, 3466, 3338, 3215, 3097, 2984, 2876, 2772, 2673, 2578,
	2486, 2398, 2314, 2233, 2156, 2081, 2010, 1941, 1875, 1812,
	1751, 1692, 1636, 1581, 1529, 1479, 1431, 1384, 1340, 1297,
	1255, 1215, 1177, 1140, 1104, 1070, 1036, 1004, 974, 944,
	915, 887, 861, 835, 810, 786, 763, 740, 719, 698,
	678, 658, 639, 621, 603, 586, 570, 554, 538, 523,
	509, 495, 482, 468, 456, 443, 432, 420, 409, 398,
	388, 377, 368, 358, 349, 340
};

/*CATL电池100K温感电阻
static const u32 sNTCTempTabR[] =                                   //CATL电池NTC(100K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	4422000,//-41℃
    4123445, 3846898, 3590500, 3352675, 3131979, 2927087, 2736784, 2559953, 2395568, 2242688,
    2100446, 1968136, 1844753, 1729894, 1622847, 1523039, 1429942, 1343069, 1261972,1186235,
    1115477, 1049344, 987508, 929669, 875547, 824883, 777438, 732992, 691338, 652287,
    615662, 581301, 549051, 518771, 490332, 463611, 438497, 414883, 392673, 371776,
    352107, 333588, 316146, 299713, 284225, 269624, 255852, 242861, 230600, 219026,
    208096, 197771, 188016, 178794, 170076, 161829, 154028, 146645, 139655, 133037,
    126768, 120828, 115199, 109862, 104800, 100000, 95444, 91121, 87016, 83118,
    79415, 75897, 72553, 69375, 66352, 63477, 60741, 58138, 55660, 53300,
    51053, 48912, 46872, 44928, 43074, 41307, 39621, 38012, 36477, 35012,
    33614, 32278, 31002, 29783, 28618, 27505, 26441, 25423, 24449, 23518,
    22627, 21774, 20957, 20175, 19426, 18709, 18022, 17363, 16732, 16127,
    15546, 14990, 14456, 13943, 13452, 12980, 12527, 12092, 11674, 11272,
    10887, 10516, 10160, 9817, 9488, 9172, 8867, 8574, 8292, 8021,
    7760, 7508, 7266, 7033, 6808, 6592, 6384, 6183, 5989, 5803,
    5623, 5449, 5282, 5120, 4964, 4814
};*/

/*其他项目电池10K温感电阻
static const u32 sNTCTempTabR[] =                                   //电池NTC(10K)温度阻值取值表[CEL_T_TAB_LEN],[-41~125]
{
	234500,//-41℃
    220889, 208085, 196117, 184924, 174453, 164650, 155468, 146865, 138799, 131234,
    124136, 117471, 111211, 105329, 99799, 94598, 89704, 85097, 80759, 76671,
    72818, 69185, 65758, 62524, 59471, 56587, 53863, 51288, 48852, 46549,
    44369, 42306, 40352, 38501, 36747, 35085, 33508, 32012, 30592, 29245,
    27965, 26749, 25594, 24496, 23452, 22459, 21514, 20615, 19759, 18944,
    18167, 17427, 16721, 16048, 15407, 14795, 14211, 13653, 13121, 12612,
    12126, 11662, 11218, 10794, 10388, 10000, 9629, 9273, 8933, 8607,
    8295, 7996, 7709, 7434, 7171, 6918, 6676, 6443, 6220, 6006,
    5801, 5603, 5414, 5231, 5056, 4888, 4726, 4571, 4421, 4277,
    4139, 4005, 3877, 3754, 3635, 3520, 3410, 3304, 3201, 3102,
    3007, 2916, 2827, 2742, 2659, 2580, 2503, 2429, 2358, 2289,
    2222, 2158, 2095, 2035, 1977, 1921, 1867, 1814, 1763, 1714,
    1667, 1621, 1576, 1533, 1492, 1451, 1412, 1375, 1338, 1302,
    1268, 1235, 1202, 1171, 1141, 1111, 1083, 1055, 1028, 1002,
    977, 953, 929, 906, 883, 862
};*/
#endif

/*均衡板温温度传感器,阻值47K电阻(旧规格)
static const u32 sNTCBalTTabR[] =                                   //板温NTC(47K)温度阻值取值表[BAL_T_TAB_LEN],[-41~125]
{
	2643000,//-41℃
	2277000, 2110000, 1957000, 1816000, 1687000, 1567000, 1457000, 1355000, 1262000, 1175000,
	1095000, 1021000, 952800, 889500, 830800, 776500, 726000, 679300, 635900, 595500,
	558000, 523200, 490700, 460600, 432400, 406200, 381800, 359000, 337700, 317900,
	299300, 282000, 265700, 250600, 236400, 223000, 210600, 198900, 187900, 177700,
	168000, 159000, 150500, 142500, 134900, 127900, 121200, 114900, 109000, 103500,
	98250, 93310, 88650, 84260, 80110, 76190, 72490, 68990, 65680, 62550,
	59590, 56790, 54140, 51630, 49250, 47000, 44860, 42840, 40910, 39090,
	37360, 35710, 34150, 32670, 31260, 29920, 28640, 27430, 26270, 25180,
	24130, 23130, 22180, 21280, 20420, 19590, 18810, 18060, 17340, 16660,
	16010, 15390, 14800, 14230, 13680, 13170, 12670, 12190, 11740, 11310,
	10890, 10490, 10110, 9744, 9394, 9058, 8736, 8427, 8131, 7847,
	7575, 7313, 7062, 6821, 6589, 6367, 6153, 5948, 5751, 5561,
	5378, 5203, 5034, 4871, 4715, 4564, 4420, 4280, 4146, 4016,
	3891, 3771, 3655, 3544, 3436, 3332, 3232, 3135, 3042, 2952,
	2865, 2781, 2700, 2622, 2546, 2473, 2403, 2334, 2268, 2205,
	2143, 2084, 2026, 1970, 1916, 1864, 1813, 1765, 1717, 1671,
	1627, 1584, 1542, 1502, 1463, 1425
};*/

/*均衡板温温度传感器,阻值47K电阻(新规格)
static const u32 sNTCBalTTabR[] =                                   //板温NTC(47K)温度阻值取值表[BAL_T_TAB_LEN],[-41~125]
{
	2154500,//-41℃
	1888700, 1756900, 1635300, 1523000, 1419300, 1323400, 1234700, 1152700, 1076700, 1006300,
	941000, 880400, 824200, 771900, 723400, 678300, 636300, 597200, 560800, 526900,
	495310, 465810, 438290, 412580, 388570, 366120, 345130, 325490, 307100, 289890,
	273750, 258630, 244450, 231140, 218650, 206920, 195900, 185540, 175800, 166640,
	158020, 149900, 142250, 135040, 128250, 121850, 115800, 110100, 104710, 99630,
	94820, 90280, 85980, 81920, 78070, 74430, 70990, 67720, 64630, 61700,
	58920, 56280, 53770, 51400, 49140, 47000, 44964, 43029, 41189, 39439,
	37775, 36190, 34683, 33247, 31879, 30576, 29334, 28150, 27021, 25944,
	24917, 23936, 23000, 22106, 21252, 20436, 19657, 18911, 18198, 17517,
	16864, 16240, 15643, 15071, 14523, 13999, 13496, 13014, 12552, 12110,
	11685, 11278, 10887, 10512, 10152, 9806, 9474, 9155, 8849, 8554,
	8271, 7999, 7738, 7486, 7244, 7011, 6787, 6571, 6363, 6163,
	5971, 5785, 5606, 5434, 5268, 5107, 4953, 4804, 4660, 4522,
	4388, 4259, 4134, 4014, 3898, 3785, 3677, 3572, 3471, 3373,
	3278, 3187, 3099, 3013, 2930, 2850, 2773, 2698, 2625, 2555,
	2487, 2421, 2357, 2296, 2236, 2178, 2122, 2067, 2014, 1963,
	1913, 1865, 1819, 1773, 1729, 1687
};*/

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/
#if(0)
/*=============================================================================================
** @Name      : static void ClrAfeSampVoltTemp(u8 ic_Num)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体电压和温度采样值为0
** @The notes : AFE单体电压温度采样失效时调用,设置采样一遍完成
**===========================================================================================*/
static void ClrAfeSampVoltTemp(u8 ic_Num);
#endif

/*=============================================================================================
** @Name      : static void ClrAfeSampCellVolt(u8 ic_Num)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体电压采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellVolt(u8 ic_Num);

/*=============================================================================================
** @Name      : static void ClrAfeSampCellTemp(u8 ic_Num)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体温度和均衡板温温度采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellTemp(u8 ic_Num);

/*=============================================================================================
** @Name      : static u32 ChangeMcuTempVoltToR(u16 Vdata)
** @Input     : ADdata:温度采样电压值
** @Output    : 温度采样NTC阻值 1Ω
** @Function  : 将温度电压值转换为NTC阻值
** @The notes :
**===========================================================================================*/
static u32 ChangeMcuTempVoltToR(u16 Vdata);

/*=============================================================================================
** @Name      : static u8 SearchCellTempTabByR(u32 data)
** @Input     : data:要查表的数据阻值 1Ω
** @Output    : 应温度下标值
** @Function  : 通过NTC阻值查询单体温度表得到对应温度值
** @The notes :
**===========================================================================================*/
static u8 SearchCellTempTabByR(u32 data);

/*=============================================================================================
** @Name      : static s16 ChangeBalADtoTemp(u16 ADdata)
** @Input     : ADdata:均衡温度采样AD值
** @Output    : 均衡温度值 ℃
** @Function  : 将均衡温度AD值转换为实际温度值
** @The notes : AFE采到的AD值已经是电压值
**===========================================================================================*/
//static s8 ChangeBalADtoTemp(u16 ADdata);

///*=============================================================================================
//** @Name      : static u32 ChangeBalTempVoltToR(u8 channel, u16 Vdata)
//** @Input     : channel:均衡温度采样GPIO通道[0, 1] Vdata:均衡温度采样电压值 1mV
//** @Output    : 均衡温度值 ℃
//** @Function  : 将均衡温度AD值转换为实际温度值
//** @The notes : AFE采到的AD值已经是电压值
//**===========================================================================================*/
//static u32 ChangeBalTempVoltToR(u8 channel, u16 Vdata);
//
///*=============================================================================================
//** @Name      : static u8 SearchBalTempTabByR(u32 data)
//** @Input     : data:要查表的数据阻值 1Ω
//** @Output    : 应温度下标值
//** @Function  : 通过NTC阻值查询均衡板温温度表得到对应温度值
//** @The notes :
//**===========================================================================================*/
//static u8 SearchBalTempTabByR(u32 data);

/*=============================================================================================
** @Name      : static u8 CorrectBalTempByKB(u8 channel, u8 temp)
** @Input     : channel:均衡温度采样GPIO通道[0, 1] temp:查表均衡温度[0, 166] 1℃
** @Output    : 修正后的均衡温度值
** @Function  : 通过KB值修正均衡板温温度值
** @The notes : KB值由实际测定
**===========================================================================================*/
static u8 CorrectBalTempByKB(u8 channel, u8 temp);

/*=============================================================================================
** @Name      : SetLv4051TempSampChan(e_BqTempSampStep Num)
** @Input     : void
** @Output    : void
** @Function  : 温度采集通道选择
** @The notes : 
**===========================================================================================*/
static void SetLv4051TempSampChan(e_BqTempSampStep Num);

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : void CellAfe79616SampInit(void)
** @Input     : void
** @Output    : void
** @Function  : 初始化模拟前端配置寄存器所需的变量
** @The notes :
**===========================================================================================*/
void CellAfe79616SampInit(void)
{
    ;
}

u8 watchNum = 0;
u8 watchBSUNum = 0;
//u8 SciSendData[7] = {0x80, 0x01, 0x03 ,0x08 ,0x00 ,0xD1 ,0x84};
//u16 wCRCTest = 0;
static u8 AutoaddrrFlag = 0;

/*=============================================================================================
** @Name      : void CellInfoBQSampleTask(void *p)
** @Input     : *p:形参,任务函数参数需要
** @Output    : void
** @Function  : 单体电压、温度采样任务
** @The notes : 该任务目前只支持单线采样(暂不实现双线)
**===========================================================================================*/
void CellInfoBQSampleTask(void *p)
{
    static u16 sState = 0xaa;        //先初始化
    static u8 sWakeStimer = 0;       //唤醒次数
    static u8 sTempSampChan = 0;    //温度采集通道选择
    static u8 errIcNum = 0;         //IC异常计数
    static u8 noVSampCounter[MAX_BF8915A_NUM] = {0};     /*电压采样失效次数*/
    static u8 noTSampCounter[MAX_BF8915A_NUM] = {0};     /*温度采样失效次数*/
//	static u8 cellTempGpioCh = 0; 				         /* GPIO 温度采集通道选择 ， 0：GPIO7  1:GPIO8 */
	static u8 indexIc = 0; 				             /* 用于GPIO 温度采集通道切换*/
	static u8 boardIc = 1; 				             /* 用于电芯电压IC切换*/
	static u8 rcvTempMsgNum = 0;
    static u8 BQ79616BalancePack = 1;                    //均衡pack序号
    u8 cell_Ic = 0;  								     /* IC counter */
    u8 cell_Num = 0;  								     /* CELL counter */
	u8 cellNum_Gpio = 0; 							     /* GPIO 温度采集通道寄存器存放位置 */
//	s16 cellTemp = 0;
	s16 balTemp = 0;
	u32 tempR = 0;
	u16 bsuNum = 0;		/*组内从控并联串数 */
	//u16 bsuVNum = 0;	/*一个从控电压采样节数*/
	//u16 bsuTNum = 0;	/*一个从控温度采样节数*/
	//u16 balTNum = 0;	/*一个从控板温采样节数*/
//	u16 anaVNum = 0;
	u8 rcvMsgNum = 0;
	u8 i = 0;
    p = p;

	bsuNum = GetDaisyAFENum();		/*组内从控并联串数 */
	//bsuNum = 15;		/*组内从控并联串数 */
	//bsuVNum = GetVoltSampNum();	    /*一个从控电压采样节数*/
    //bsuTNum = gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum];	/*一个从控温度采样节数*/
    BQ79616SumIcNum = bsuNum;
    
    switch(sState)
    {
        /*通讯唤醒，UART->RX拉低*/
        case 0xaa:
            #if(0 == PORT_COMMTYPE_CHOICE)
            BSPUartRestRcvPin();
            #else
            BSPSpiRestRcvPin();  
            DEVGPIOSetPortState(ePOut_BQ79616_CS, ePSTATE_ON);
            #endif
            DEVGPIOSetPortState(ePOut_BQ79616_WAKE, ePSTATE_ON);
            sState = 0xbb;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
            
        case 0xbb:      
            #if(1 == PORT_COMMTYPE_CHOICE) 
            DEVGPIOSetPortState(ePOut_BQ79616_CS, ePSTATE_OFF);
            #endif
            DEVGPIOSetPortState(ePOut_BQ79616_WAKE, ePSTATE_OFF);//BQ79600的RX拉低2ms，唤醒BQ79600
            sState = 0xcc;
            BSPCosStart(COS_ID_CELL_SAMP, (2));
            break;
            
        /*通讯管脚复位后，RX重新初始化*/    
        case 0xcc:
            sWakeStimer++;
            DEVGPIOSetPortState(ePOut_BQ79616_WAKE, ePSTATE_ON);//
            #if(1 == PORT_COMMTYPE_CHOICE) 
            DEVGPIOSetPortState(ePOut_BQ79616_CS, ePSTATE_ON);//
            #endif
            
            if(sWakeStimer >= 2)//必须唤醒两次，保证链路从件全部唤醒
            {
                sWakeStimer = 0;
                sState = 0xdd;
                BSPCosStart(COS_ID_CELL_SAMP, 2);
            }
            else
            {
                sState = 0xbb;
                BSPCosStart(COS_ID_CELL_SAMP, 2);
            }
            break; //上述唤醒过程需要的时间未经允许不能更改
            
        case 0xdd:
            SpiCommClear79600();            //通讯清除
            #if(0 == PORT_COMMTYPE_CHOICE)
            DEVSCIResetInit(eSCI0, 1000000);                             //初始化UART，准备硬件
            #else
            BSPSPIInit(eSPI2);     
            #endif
            sState = 0;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;  
          
        //INITIALIZE BQ79616-Q1 STACK 
        case 0: 
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, CONTROL1, 0x20, 1, FRMWRT_SGL_W); //send wake tone to stack devices
            #else 
            SpiWriteReg(0, CONTROL1, 0x20, 1, FRMWRT_SGL_W);     //send wake tone to stack devices       
            #endif
            sState = 1;
//            SetBq79616SampChangeDirection(eBq79616_DireCommL);//测试
//            sState = 2;//测试
            //BSPCosStart(COS_ID_CELL_SAMP, 12*TOTALBOARDS); //wake tone duration is ~1.6ms per board + 10ms per board for each device to wake up from shutdown = 11.6ms per 616 board
            BSPCosStart(COS_ID_CELL_SAMP, 50); //wake tone duration is ~1.6ms per board + 10ms per board for each device to wake up from shutdown = 11.6ms per 616 board
            break;
#if 1           
        /*设置BQ79616自动编址*/
        case 1:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);            
            #endif
            sState = 0x11;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x11:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);    
            #endif
        
            sState = 0x21;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x21:  
            #if(0 == PORT_COMMTYPE_CHOICE)  
            Bq79616WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);            
            #endif
        
            sState = 0x31;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x31:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);            
            #endif
        
            sState = 0x41;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x41:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);            
            #endif
        
            sState = 0x51;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x51:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);            
            #endif
        
            sState = 0x61;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x61:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);            
            #endif
        
            sState = 0x71;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x71:    
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);  
            #else
            SpiWriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);              
            #endif
        
            sState = 0x81;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x81: 
            #if(0 == PORT_COMMTYPE_CHOICE)   
            //ENABLE AUTO ADDRESSING MODE
            Bq79616WriteReg(0, CONTROL1, 0X01, 1, FRMWRT_ALL_W);
            #else
            SpiWriteReg(0, CONTROL1, 0X01, 1, FRMWRT_ALL_W);            
            #endif
        
            sState = 0x91;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0x91:    

            //SET ADDRESSES FOR EVERY BOARD
            for(i = 0; i < (bsuNum + BRIDGEDEVICE); i++)
            {
                #if(0 == PORT_COMMTYPE_CHOICE)
                Bq79616WriteReg(0, DIR0_ADDR, i, 1, FRMWRT_ALL_W);
                #else
                SpiWriteReg(0, DIR0_ADDR, i, 1, FRMWRT_ALL_W);        
                #endif
            }
        
            sState = 0xA1;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
        case 0xA1: 
            #if(0 == PORT_COMMTYPE_CHOICE)   
            //BROADCAST WRITE TO SET ALL DEVICES AS STACK DEVICE
            Bq79616WriteReg(0, COMM_CTRL, 0x02, 1, FRMWRT_ALL_W);
            #else
            SpiWriteReg(0, COMM_CTRL, 0x02, 1, FRMWRT_ALL_W);            
            #endif
        
            sState = 0xb2;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
                 
        case 0xb2:  
            #if(0 == PORT_COMMTYPE_CHOICE)  
            //SET THE HIGHEST DEVICE IN THE STACK AS BOTH STACK AND TOP OF STACK
            Bq79616WriteReg(bsuNum + BRIDGEDEVICE - 1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W); 
            #else
            SpiWriteReg(bsuNum + BRIDGEDEVICE - 1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W);             
            #endif 
        
            sState = 0xc1;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
                       
         //查询地址   
        case 0xc1:
            #if(0 == PORT_COMMTYPE_CHOICE)
            //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
            Bq79616ReadReg(0, OTP_ECC_DATAIN1, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN2, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN3, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN4, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN5, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN6, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN7, response_frame, 1, 0, FRMWRT_STK_R);
            Bq79616ReadReg(0, OTP_ECC_DATAIN8, response_frame, 1, 0, FRMWRT_STK_R);

            //OPTIONAL: read back all device addresses
            for(cell_Ic=0; cell_Ic<bsuNum; cell_Ic++)
            {
                Bq79616ReadReg(cell_Ic, DIR0_ADDR, response_frame, 1, 0, FRMWRT_SGL_R);
            }

            //OPTIONAL: read register address 0x2001 and verify that the value is 0x14
            Bq79616ReadReg(0, 0x2001, response_frame, 1, 0, FRMWRT_SGL_R);
            #else
            //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
            SpiReadReg(0, OTP_ECC_DATAIN1, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN2, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN3, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN4, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN5, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN6, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN7, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            SpiReadReg(0, OTP_ECC_DATAIN8, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);

            //OPTIONAL: read back all device addresses
			//for(cell_Ic=0; cell_Ic<TOTALBOARDS; cell_Ic++)
            for(cell_Ic=0; cell_Ic<(GetBQ79616BQ79616SumIcNumAPI() + BRIDGEDEVICE); cell_Ic++)
            {
                SpiReadReg(cell_Ic, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
            }
            
            //OPTIONAL: read register address 0x2001 and verify that the value is 0x14
            SpiReadReg(0, 0x2001, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);

            #endif
            sState = 2;
            BSPCosStart(COS_ID_CELL_SAMP, 5);
            break;            
#else
            
        case 1:
            SpiWriteReg(0, Bridge_DEV_CONF1, 0x14, 0, FRMWRT_SGL_W);
            sState = 0x11;
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;
        case 0x11:
            SpiWriteReg(0, OTP_ECC_TEST, 0X00, 1, FRMWRT_ALL_W);
            sState = 0x81;
            BSPCosStart(COS_ID_CELL_SAMP, 1);      
            break;
            
        case 0x81:  
            //ENABLE AUTO ADDRESSING MODE
            SpiWriteReg(0, CONTROL1, 0X01, 1, FRMWRT_ALL_W); 
        
            sState = 0x91;
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;
            
        case 0x91:    
            //SET ADDRESSES FOR EVERY BOARD
            if(0 == AutoaddrrFlag)
            {
                //for(i = 0; i < (bsuNum + BRIDGEDEVICE); i++)
                for(i = 0; i <= 35; i++)
                {
                    SpiWriteReg(0, DIR0_ADDR, i, 1, FRMWRT_ALL_W);  
                }
        
                sState = 0xA1;
            }
            else
            {
                for(i = 0; i < (watchBSUNum+1); i++)
                {
                    SpiWriteReg(0, DIR0_ADDR, i, 1, FRMWRT_ALL_W);  
                } 
        
                sState = 0xC2;
            }
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
            
            
        case 0xA1:            
            //OPTIONAL: read back all device addresses
            //for(cell_Ic=0; cell_Ic<TOTALBOARDS; cell_Ic++)
            {
                //SpiReadReg(cell_Ic, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
                SpiReadReg(boardIc, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
            }
            sState = 0xB1;
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
            
        case 0xB1:
            if(boardIc != (u8)autoaddr_response_frame[1])
            {  
                AutoaddrrFlag = 1;
                sState = 0xaa;//重新初始化
                watchBSUNum = boardIc - 1;//记录当前没有响应的616
            }
            else
            {
                if(boardIc >= bsuNum)
                {
                    watchBSUNum = boardIc;
                    boardIc = 1;
                    sState = 0xC2;
                }
                else
                {
                    boardIc++;
                    sState = 0xA1;
                }
            }
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;
         
//        case 0xC1:
//            for(i = 0; i < (watchBSUNum+1); i++)
//            {
//                SpiWriteReg(0, DIR0_ADDR, i, 1, FRMWRT_ALL_W);  
//            }
//            sState = 0xC2;
//            BSPCosStart(COS_ID_CELL_SAMP, 2);  
//            break;
         
        case 0xC2:
            SpiWriteReg(watchBSUNum, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W);   //设置没有响应的前一个地址为START_DEV and TOP_START
            sState = 0xD3;
            BSPCosStart(COS_ID_CELL_SAMP, 2);  
            break;
         
        case 0xD3:
            SpiReadReg(watchBSUNum, COMM_CTRL, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);   //测试
            sState = 0xD2;
            BSPCosStart(COS_ID_CELL_SAMP, 1);  
            break;
         
        case 0xD2:
            SpiReadReg(watchBSUNum, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);   //测试
            sState = 0xD1;
            BSPCosStart(COS_ID_CELL_SAMP, 1);  
            break;
            
        case 0xD1:
            //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
            SpiReadReg(0, OTP_ECC_TEST, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            sState = 0xE1;
            BSPCosStart(COS_ID_CELL_SAMP, 1);  
            break;
            
        case 0xE1:
        SpiCommClear79600();            //通讯清除
            //OPTIONAL: read register address 0x2001 and verify that the value is 0x14
            SpiReadReg(0, Bridge_DEV_CONF1, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
            sState = 0xF1;
            BSPCosStart(COS_ID_CELL_SAMP, 1);  
            break;
            
        case 0xF1:
            SpiReadReg(0, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
            sState = 2;
            BSPCosStart(COS_ID_CELL_SAMP, 1);  
            break;  
            
//        case 0xF3:
//            SpiWriteReg(0, FAULT_RST2, 0x03, 1, FRMWRT_STK_W); 
//            SpiWriteReg(0, COMM_TIMEOUT_CONF, 0x00, 1, FRMWRT_STK_W); 
//            //SpiWriteReg(0, COMM_TIMEOUT, 0x00, 1, FRMWRT_STK_W); 
//            sState = 3;
//            BSPCosStart(COS_ID_CELL_SAMP, 1);  
//            break;       
            
#endif          
         
        //清除BQ79616所有faults
        case 2:   
            #if(0 == PORT_COMMTYPE_CHOICE)
            //RESET ANY COMM FAULT CONDITIONS FROM STARTUP
            Bq79616WriteReg(0, FAULT_RST1, 0xFF, 1, FRMWRT_STK_W); //Reset faults on stacked devices
            Bq79616WriteReg(0, FAULT_RST2, 0xFF, 1, FRMWRT_STK_W); //Reset faults on stacked devices
            Bq79616WriteReg(0, Bridge_FAULT_RST, 0x22, 1, FRMWRT_SGL_W); //Reset FAULT_COMM and FAULT_SYS on bridge device  
            #else
            //RESET ANY COMM FAULT CONDITIONS FROM STARTUP
            SpiWriteReg(0, FAULT_RST1, 0xFF, 1, FRMWRT_STK_W); //Reset faults on stacked devices
            SpiWriteReg(0, FAULT_RST2, 0xFF, 1, FRMWRT_STK_W); //Reset faults on stacked devices
            SpiWriteReg(0, Bridge_FAULT_RST, 0x22, 1, FRMWRT_SGL_W); //Reset FAULT_COMM and FAULT_SYS on bridge device        
            #endif
            sState = 3;
            BSPCosStart(COS_ID_CELL_SAMP, 5);
            break;
           
        //init Main ADC
        case 3: 
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, ACTIVE_CELL, 0x0A, 1, FRMWRT_STK_W); //set all cells to active 
            #else
            SpiWriteReg(0, ACTIVE_CELL, 0x0A, 1, FRMWRT_STK_W); //set all cells to active             
            #endif             
            sState = 0x23;
            BSPCosStart(COS_ID_CELL_SAMP, 2);    
            break; 
//        case 0x13: 
//            #if(0 == PORT_COMMTYPE_CHOICE)
//            Bq79616WriteReg(0, ADC_CONF1, 0x02, 1, FRMWRT_ALL_W);      //26Hz LPF_Vcell (38ms average)  
//            #else
//            SpiWriteReg(0, ADC_CONF1, 0x02, 1, FRMWRT_ALL_W);      //26Hz LPF_Vcell (38ms average)             
//            #endif            
//            sState = 0x23;
//            BSPCosStart(COS_ID_CELL_SAMP, 2);    
//            break; 
        case 0x23: 
            #if(0 == PORT_COMMTYPE_CHOICE)
            Bq79616WriteReg(0, ADC_CTRL1, 0x06, 1, FRMWRT_STK_W);   //continuous run and MAIN_GO      
            #else
            SpiWriteReg(0, ADC_CTRL1, 0x06, 1, FRMWRT_STK_W);   //continuous run and MAIN_GO                   
            SpiWriteReg(0, CONTROL2, 0x01, 1, FRMWRT_STK_W);       //TSREF 使能            
            #endif          
            sState = 0x24;
            BSPCosStart(COS_ID_CELL_SAMP, 2);    
            break;  
			
		/*关闭均衡*/	
		case 0x24: 
            BQ79616BalancePack = 1;
            sState = 0x25;
            BSPCosStart(COS_ID_CELL_SAMP, 2);  
            break; 
            
        case 0x25: 
            BQ79616PasEqualCloseAllPasEqual(BQ79616BalancePack);
            BQ79616BalancePack++;
            if(BQ79616BalancePack > BQ79616SumIcNum)
            {
               sState = 4; 
               BSPCosStart(COS_ID_CELL_SAMP, 10);  
            }
            else
            {
                sState = 0x25;
                BSPCosStart(COS_ID_CELL_SAMP, 2); 
            }  
            break; 
              
        /*读取BQ79616电压数据*/    
        case 4:
            #if(0 == PORT_COMMTYPE_CHOICE)
            BSPUartClrRcvData();//清除串口数据
            Bq79616ReadReg(0, VCELL16_HI, response_frame, 16*2, 0, FRMWRT_STK_R);   
            #else
            //SpiReadReg(cell_Ic, VCELL16_HI, response_frame, 16*2, 0, FRMWRT_STK_R);  
            SpiReadReg(boardIc, VCELL16_HI, response_frame, 16*2, 0, FRMWRT_SGL_R);             
            #endif  
            sState = 5;
            //BSPCosStart(COS_ID_CELL_SAMP, 10);  
            BSPCosStart(COS_ID_CELL_SAMP, 10);            
            break;
                        
        case 5: 
            //for(cell_Ic = 0; cell_Ic < bsuNum; cell_Ic++)
            {
                //go through each byte in the current board (32 bytes = 16 cells * 2 bytes each)
                for(i = 0; i < 32; i+=2)
                {
                    //each board responds with 32 data bytes + 6 header bytes
                    //so need to find the start of each board by doing that * cell_Ic
                    //int boardByteStart = (16*2+6)*cell_Ic;

                    //convert the two individual bytes of each cell into a single 16 bit data item (by bit shifting)
                    #if(0 == PORT_COMMTYPE_CHOICE)
                    Bq79616CellVolt[boardIc - 1][(15- i/2)] = ((u16)UartRcvDataBuff[boardByteStart+i+4] << 8) | UartRcvDataBuff[boardByteStart+i+5];     
                    #else
                    Bq79616CellVolt[boardIc - 1][(15- i/2)] = ((u16)response_frame[/*boardByteStart+*/i+4] << 8) | response_frame[/*boardByteStart+*/i+5];  
                    #endif
                    //Bq79616CellVolt[cell_Ic][(15- i/2)] = (u16)((((u16)UartRcvDataBuff[boardByteStart+i+4] << 8) | UartRcvDataBuff[boardByteStart+i+5]) * 0.19073);                                        
                }
            }
            //BSPUartClrRcvData();//清除串口数据
            if(boardIc >= bsuNum)
            {
                boardIc = 1;
                #if(1 == BQ79616_BUSBAR_EN) 
                sState = 6;
                #else
                sState = 7; 
                #endif
            }
            else
            {
                boardIc++;
                sState = 4; 
            }
            BSPCosStart(COS_ID_CELL_SAMP, 2);
            break;
            
        #if(1 == BQ79616_BUSBAR_EN)                       
        //读取BQ79616  BBP/BBN电压数据
        case 6:
            #if(0 == PORT_COMMTYPE_CHOICE)
            BSPUartClrRcvData();//清除串口数据
            Bq79616ReadReg(0, BUSBAR_HI, response_frame, 1*2, 0, FRMWRT_ALL_R); 
            #else
            SpiReadReg(0, BUSBAR_HI, response_frame, 1*2, 0, FRMWRT_ALL_R);            
            #endif      
            sState = 7;
            BSPCosStart(COS_ID_CELL_SAMP, 5);           
            break;
        #endif 
            
        //电压数据处理   
        case 7:       
            for(cell_Ic = 0; cell_Ic < bsuNum; cell_Ic++)
            {
                #if(1 == BQ79616_BUSBAR_EN)   
                //convert the two individual bytes of each cell into a single 16 bit data item (by bit shifting)
                    #if(0 == PORT_COMMTYPE_CHOICE)
                    Bq79616BusBarVolt[cell_Ic][0] = ((u16)UartRcvDataBuff[4] << 8) | UartRcvDataBuff[5];  
                    #else
                    Bq79616BusBarVolt[cell_Ic][0] = (u16)((((u16)response_frame[4] << 8) | response_frame[5]) * 0.03052);  
                    #endif
                #endif
                for(cell_Num = 0; cell_Num < GetVoltSampNum(); cell_Num++)
                {
                    //if(rawData < 0x8000)//采集数据正常
                    //if(Bq79616CellVolt[cell_Ic][cell_Num] < 32500)//采集数据正常
                    if(Bq79616CellVolt[cell_Ic][cell_Num] < 65000)//采集数据正常
                    {                    
                        SetBQ79616SampVAlarmFlagAPI(cell_Ic, 0);//清除异常标志
                        noVSampCounter[cell_Ic] = 0;
                        if(Bq79616CellVolt[cell_Ic][cell_Num] > 32768)//负数
                        {
                            Bq79616CellVolt[cell_Ic][cell_Num] = 0;
                        }
                        /*采样电压值判断断线*/
                        DiagCheckCellVOffLFlagAPI(cell_Ic, cell_Num, (u16)(Bq79616CellVolt[cell_Ic][cell_Num] * 0.19073));/*电压采样线断线检测*/
                        
                        /*输出采样电压值*/
                        #if(1 == BQ79616_BUSBAR_EN)
                        if(8 == cell_Num)
                        {
//                            if(Bq79616CellVolt[cell_Ic][cell_Num] > Bq79616BusBarVolt[cell_Ic][0])
//                            {
//                                WriteSampCellVoltHook(cell_Ic, cell_Num, ((u16)(Bq79616CellVolt[cell_Ic][cell_Num] * 0.19073) - Bq79616BusBarVolt[cell_Ic][0]));  /*输出电压值*/
//                            }
//                            else
//                            {
//                                WriteSampCellVoltHook(cell_Ic, cell_Num, (u16)(Bq79616CellVolt[cell_Ic][cell_Num] * 0.19073));  /*输出电压值*/
//                            }
                            WriteSampCellVoltHook(cell_Ic, cell_Num, (u16)(Bq79616CellVolt[cell_Ic][cell_Num] * 0.19073));  /*输出电压值*/
                        }
                        else
                        {
                            WriteSampCellVoltHook(cell_Ic, cell_Num, (u16)(Bq79616CellVolt[cell_Ic][cell_Num] * 0.19073));  /*输出电压值*/
                        }
                        #else
                        WriteSampCellVoltHook(cell_Ic, cell_Num, (u16)(Bq79616CellVolt[cell_Ic][cell_Num] * 0.19073));  /*输出电压值*/
                        #endif
                    }
                    else
                    {
                        /*该从控采样异常计数*/
                        noVSampCounter[cell_Ic]++;
                        /*达到异常次数则记录故障并清除该从控数据*/
                        if(noVSampCounter[cell_Ic] >= SAMPE_ERR_TIMES)
                        {
//                            /*记录该从控采样异常*/
//                            DiagRecordAFESampErrFlagAPI(cell_Ic, 1);
                            SetBQ79616SampVAlarmFlagAPI(cell_Ic, 1);
                            noVSampCounter[cell_Ic] = SAMPE_ERR_TIMES;

                            /*采样失效清除单体电压信息(输出单体电压值为0)*/
                            ClrAfeSampCellVolt(cell_Ic);
                            //InputBsuCellVolt(cell_Ic, cell_Num, 0);
                            
                            errIcNum++;//异常的IC计数
                        }
                        else
                        {
//                            sState = 0xaa;
//                            BSPCosStart(COS_ID_CELL_SAMP, 1);
//                            return; 
                            //重新初始化
                            //#if(1 == AFE_TYPE_CHOICE)
                            //CellAfe79616SampInit();
                            //#endif
                        }
                        break;
                    } 
                }

                /*该从控采样正常或已确认采样异常*/
                if((0 == noVSampCounter[cell_Ic])
                    || (noVSampCounter[cell_Ic] >= SAMPE_ERR_TIMES))
                {
                    /*累计已采样从控数目*/
                    rcvMsgNum++;
                }
                
            }

            /*每个从控都有数据或已确认异常*/
            if(rcvMsgNum >= bsuNum)
            {
                /*记录完成一遍电压采样*/
                //WriteCellVSampFinFlagHook(1);
                WriteVSampFinFlag(1);
            }
            
            //全部无回复(支持通讯热拨插)
            if(errIcNum >= bsuNum)
            {
                //直接清除单体温度信息
                for(cell_Ic = 0; cell_Ic < bsuNum; cell_Ic++)
                {
                    /*采样失效清除单体温度信息*/
                    ClrAfeSampCellTemp(cell_Ic);
                }
                
                //直接记录温度采样完成
                //WriteCellTSampFinFlagHook(1);
                WriteTSampFinFlag(1);
              
                //返回重新初始化
                errIcNum = 0;
                SpiCommClear79600();
                sState = 0xaa;
                BSPCosStart(COS_ID_CELL_SAMP, 1);
                return;
            }
    
            //BSPUartClrRcvData();//清除串口数据
            sState = 8;
            BSPCosStart(COS_ID_CELL_SAMP, 8);//时间不能太短
            break;
/*            
        case 0xf1:
            #if(0 == PORT_COMMTYPE_CHOICE)
            //cellTempGpioCh = 1;//测试
            Bq79616WriteReg(0, CONTROL2, 0x01, 1, FRMWRT_STK_W);       //TSREF 使能
            #else
            SpiWriteReg(0, CONTROL2, 0x01, 1, FRMWRT_STK_W);       //TSREF 使能            
            #endif
            sState = 8;
            BSPCosStart(COS_ID_CELL_SAMP, 5);
            break;
*/          
        case 8:
//            if(0 == cellTempGpioCh)
//            {
//                #if(0 == PORT_COMMTYPE_CHOICE)
//                Bq79616WriteReg(0, GPIO_CONF4, 0x02, 1, FRMWRT_STK_W);      //GPIO7作为ADC 
//                #else
//                SpiWriteReg(0, GPIO_CONF4, 0x02, 1, FRMWRT_STK_W);      //GPIO7作为ADC         
//                #endif
//            }
//            else
//            {
//                #if(0 == PORT_COMMTYPE_CHOICE)
//                Bq79616WriteReg(0, GPIO_CONF4, 0x10, 1, FRMWRT_STK_W);       //GPIO8作为ADC 
//                #else
//                SpiWriteReg(0, GPIO_CONF4, 0x10, 1, FRMWRT_STK_W);       //GPIO8作为ADC         
//                #endif
//            }
            SpiWriteReg(0, GPIO_CONF4, 0x12, 1, FRMWRT_STK_W);       //GPIO8/GPIO7作为ADC   
            sState = 9;
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;
            
        case 9:
//            if(0 == cellTempGpioCh)
//            {
//                #if(0 == PORT_COMMTYPE_CHOICE)
//                Bq79616WriteReg(0, ADC_CTRL3, 0x3E, 1, FRMWRT_STK_W);   //continuous run and AUX_GO  GPIO7
//                #else
//                SpiWriteReg(0, ADC_CTRL3, 0x3E, 1, FRMWRT_STK_W);   //continuous run and AUX_GO  GPIO7        
//                #endif
//            }
//            else
//            {
//                #if(0 == PORT_COMMTYPE_CHOICE)
//                Bq79616WriteReg(0, ADC_CTRL3, 0x46, 1, FRMWRT_STK_W);   //continuous run and AUX_GO   GPIO8  
//                #else
//                SpiWriteReg(0, ADC_CTRL3, 0x46, 1, FRMWRT_STK_W);   //continuous run and AUX_GO   GPIO8         
//                #endif              
//            }
            SpiWriteReg(0, ADC_CTRL1, 0x06, 1, FRMWRT_STK_W);   //continuous run and MAIN_GO  
            sState = 11;
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;         
/*            
        case 10:
            Bq79616ReadReg(1, ADC_CTRL3, response_frame, 1, 0, FRMWRT_SGL_R);
            //Bq79616WriteReg(0, ADC_CTRL3, 0x06, 1, FRMWRT_STK_W);   //continuous run and AUX_GO 
            BSPUartClrRcvData();//清除串口数据
            sState = 11;
            BSPCosStart(COS_ID_CELL_SAMP, 10);
            break;
*/            
        /*温度采集通道选择*/    
        case 11:
            //sTempSampChan = eBqTemp_1And7;//测试
            SetLv4051TempSampChan((e_BqTempSampStep)(eBqTemp_1And7 + sTempSampChan));
            sState = 12;      
            BSPCosStart(COS_ID_CELL_SAMP, 2);//4051切换之后需要等待时间，具体以测量为准
            break;
            
        /*读取BQ79616温度数据*/    
        case 12:
            #if(0 == PORT_COMMTYPE_CHOICE)
            BSPUartClrRcvData();//清除串口数据
            Bq79616ReadReg(0, AUX_GPIO_HI, response_frame, 1*2, 0, FRMWRT_STK_R);  
            #else
            //SpiReadReg(0, AUX_GPIO_HI, response_frame, 1*2, 0, FRMWRT_STK_R); 
            //SpiReadReg(0, GPIO7_HI, response_frame, 2*2, 0, FRMWRT_STK_R); 
            SpiReadReg(boardIc, GPIO7_HI, response_frame, 2*2, 0, FRMWRT_SGL_R); 
            #endif
            
            sState = 13;
            BSPCosStart(COS_ID_CELL_SAMP, 5);//
            break;  
            
        case 13:
            //for(indexIc = 0; indexIc < bsuNum; /*indexIc++*/)
            {
                //go through each byte in the current board (2 bytes = 1 temps * 2 bytes each)
                #if(1 == BQ79616_TEMPNUM)
                for(i = 0; i < 4; i+=2)
                #else 
                for(i = 0; i < 2; i+=2)
                #endif
                {
                    //each board responds with 2 data bytes + 6 header bytes
                    //so need to find the start of each board by doing that * cell_Ic
                    //int boardByteStart = (2*2+6)*indexIc;

                    //convert the two individual bytes of each temp into a single 16 bit data item (by bit shifting)
                    //rawData = (response_frame[boardByteStart+i+4] << 8) | response_frame[boardByteStart+i+5];
                    //Bq79616GpioVolt[cell_Ic][cellNum_Gpio] = (response_frame[4] << 8) | response_frame[5];
                   
                    //计算GPIO7 / GPIO8 选择存放位置(没有规律可循)
                    if(eBqTemp_1And7 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 0;//cellNum_Gpio7 = 0; 
                        }
                        else
                        {
                            cellNum_Gpio = 6;//cellNum_Gpio8 = 6; 
                        }
                    }
                    else if(eBqTemp_5And8 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 4;//cellNum_Gpio7 = 4; 
                        }
                        else
                        {
                            cellNum_Gpio = 7;//cellNum_Gpio8 = 7;  
                        }
                    }
                    else if(eBqTemp_10And9 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 9;//cellNum_Gpio7 = 9; 
                        }
                        else
                        {
                            cellNum_Gpio = 8;//cellNum_Gpio8 = 8; 
                        }
                    }
                    else if(eBqTemp_14And11 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 13;//cellNum_Gpio7 = 13; 
                        }
                        else
                        {
                            cellNum_Gpio = 10;//cellNum_Gpio8 = 10; 
                        }
                    }
                    #if(1 == BQ79616_TEMPNUM)
                    else if(eBqTemp_2And12 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 1;//cellNum_Gpio7 = 1; 
                        }
                        else
                        {
                            cellNum_Gpio = 11;//cellNum_Gpio8 = 11; 
                        }
                    }
                    else if(eBqTemp_3And13 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 2;//cellNum_Gpio7 = 2; 
                        }
                        else
                        {
                            cellNum_Gpio = 12;//cellNum_Gpio8 = 12; 
                        }
                    }
                    else if(eBqTemp_4And15 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 3;//cellNum_Gpio7 = 3; 
                        }
                        else
                        {
                            cellNum_Gpio = 14;//cellNum_Gpio8 = 14; 
                        }
                    }
                    else if(eBqTemp_6And16 == sTempSampChan)
                    {
                        //if(0 == cellTempGpioCh)
                        if(0 == i)
                        {
                            cellNum_Gpio = 5;//cellNum_Gpio7 = 5; 
                        }
                        else
                        {
                            cellNum_Gpio = 15;//cellNum_Gpio8 = 15; 
                        }
                    }
                    #endif
                   
                    #if(0 == PORT_COMMTYPE_CHOICE)
                    Bq79616GpioVolt[indexIc][cellNum_Gpio] = ((u16)UartRcvDataBuff[4] << 8) | UartRcvDataBuff[5];
                    #else
                    //Bq79616GpioVolt[indexIc][cellNum_Gpio] = ((u16)response_frame[4] << 8) | response_frame[5];  
                    Bq79616GpioVolt[boardIc - 1][cellNum_Gpio] = ((u16)response_frame[/*boardByteStart+*/i+4] << 8) | response_frame[/*boardByteStart+*/i+5];                                      
                    #endif
                    
                    //if(rawData < 0x8000)//采集数据正常
                    //if(Bq79616GpioVolt[boardIc - 1][cellNum_Gpio] < 32500)//采集数据正常
                    if(Bq79616GpioVolt[boardIc - 1][cellNum_Gpio] < 65000)//采集数据正常
                    {   
                        SetBQ79616SampTAlarmFlagAPI(boardIc - 1, 0);
                        noTSampCounter[boardIc - 1] = 0;
                        
                        if(Bq79616GpioVolt[boardIc - 1][cellNum_Gpio] > 32768)//负数
                        {
                            Bq79616GpioVolt[boardIc - 1][cellNum_Gpio] = 0;
                        }
                        
                        /*GPIO7~GPIO8计算实际温度值*/
                        tempR = ChangeMcuTempVoltToR((u16)(Bq79616GpioVolt[boardIc - 1][cellNum_Gpio]*0.15259));

                        balTemp = SearchCellTempTabByR(tempR);
                        balTemp = CorrectBalTempByKB(cellNum_Gpio, balTemp);
                        
                        #if(1 == BQ79616_TEMPNUM)
                        /*采样电压值判断断线*/
                        DiagCheckCellTOffLFlagAPI(boardIc - 1, cellNum_Gpio, balTemp - 41);/*温度采样线断线检测*/
                        /*输出采样电压值*/
                        WriteSampCellTempHook(boardIc - 1, cellNum_Gpio, balTemp - 41);  /*输出温度值*/
                        #else
                        if((0 == cellNum_Gpio) && (0 == i))
                        {
                            cellNum_Gpio = 0;
                            //balTemp = 60;
                        }
                        else if((4 == cellNum_Gpio) && (0 == i))
                        {
                            cellNum_Gpio = 1;
                            //balTemp = 75;
                        }
                        else if((9 == cellNum_Gpio) && (0 == i))
                        {
                            cellNum_Gpio = 2;
                            //balTemp = 62;
                        }
                        else if((13 == cellNum_Gpio) && (0 == i))
                        {
                            cellNum_Gpio = 3;
                            //balTemp = 65;
                        } 
                        else
                        {
                            //continue;
                        }
                        /*采样电压值判断断线*/
                        DiagCheckCellTOffLFlagAPI(boardIc - 1, cellNum_Gpio, balTemp - 41);/*温度采样线断线检测*/
                        /*输出采样电压值*/
                        WriteSampCellTempHook(boardIc - 1, cellNum_Gpio, balTemp - 41);  /*输出温度值*/                
                        #endif
                    }
                    else
                    {
                        /*该从控采样异常计数*/
                        noTSampCounter[boardIc - 1]++;
                        /*达到异常次数则记录故障并清除该从控数据*/
                        if(noTSampCounter[boardIc - 1] >= SAMPE_ERR_TIMES)
                        {
//							/*记录该从控采样异常*/
//							DiagRecordAFESampErrFlagAPI(indexIc, 3);
                            SetBQ79616SampTAlarmFlagAPI(boardIc - 1, 1);
                            noTSampCounter[boardIc - 1] = SAMPE_ERR_TIMES;

                            /*采样失效清除单体温度信息*/
                            ClrAfeSampCellTemp(boardIc - 1);
                            //InputBsuCellTemp(boardIc - 1, cellNum_Gpio, - 41);
                        }
                        else
                        {
                            sState = 0xaa;
                            BSPCosStart(COS_ID_CELL_SAMP, 1);
                            return; 
                        }
                    }
                }
                /*该从控采样正常或已确认采样异常*/
                if((0 == noTSampCounter[boardIc - 1])
                    || (noTSampCounter[boardIc - 1] >= SAMPE_ERR_TIMES))
                {
                    /*累计已采样从控数目*/
                    //rcvTempMsgNum++;
                    rcvTempMsgNum = boardIc;
                }
            }
            /*每个从控都有数据*/
            if(rcvTempMsgNum >= bsuNum)
            {
                rcvTempMsgNum = 0;
                /*记录温度采样一遍完成*/
                //WriteCellTSampFinFlagHook(1);
                WriteTSampFinFlag(1);
            }
           
            #if(1 == BQ79616_TEMPNUM)
            if(sTempSampChan >= eBqTemp_6And16)//采集完成
            #else
            if(sTempSampChan >= eBqTemp_14And11)//采集完成                
            #endif
            {
                if(boardIc >= bsuNum)
                {
                    boardIc = 1;
                    sState = 14;
                }
                else
                {
                    boardIc++;//下一个IC
                    sState = 11;
                }
                sTempSampChan = eBqTemp_1And7;//下一个温度采集周期
                BSPCosStart(COS_ID_CELL_SAMP, 1);
            }
            //切换下一个通道
            else
            {
                sTempSampChan++;//切换通道，4051切换一次有两路temp接入，AFE每次只能采集一路，
                sState = 11;
                BSPCosStart(COS_ID_CELL_SAMP, 1);
            }
            
            #if(0 == PORT_COMMTYPE_CHOICE)
            BSPUartClrRcvData();//清除串口数据
            #endif
            break;  
        
        //设置均衡
        case 14:
            UpdateBattBalanCtrlMsg();				            /*更新均衡信息(正常执行均衡)*/
            CtrlBattBalanSetState(1);                           /*开启均衡需求(奇偶数节)*/
            BQ79616BalancePack = 1;
            sState = 15;
            BSPCosStart(COS_ID_CELL_SAMP, 2);//均衡控制时间，用户可以修改
         break;
          case 15:
                if(BQ79616BalancePack > BQ79616SumIcNum)  //关闭完毕
                {
                      sState = 0x24;
                      BQ79616BalancePack = 1;
                }
                else
                {
                    BQ79616PasEqualCmd(BQ79616BalancePack);
                    BQ79616BalancePack++;
                    if(BQ79616BalancePack > BQ79616SumIcNum)
                    {
                      sState = 0x24;
                      BSPCosStart(COS_ID_CELL_SAMP, 100);//均衡控制时间，用户可以修改
                    }
                    else
                    {
                        sState = 15;
                        BSPCosStart(COS_ID_CELL_SAMP, 2);//均衡控制时间，用户可以修改
                    }
                }
            break;
            
        default:
            sState = 0;
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;
    }
}

//u16 tempCmd2 = 0;
/*=============================================================================================
** @Name      : void BQ79616PasEqual(u16* commande, u8 total_ic)
** @Input     : commande:均衡命令   total_ic:从件个数
** @Output    : void
** @Function  : BQ79616的被动均衡执行函数
** @The notes : 支持16S均衡(目前串口一次性发多个指令可能会失效，所以此函数只支持SPI菊花链通信方式)
**===========================================================================================*/
void BQ79616PasEqual(u16* commande, u8 total_ic)
{
    u8 cell_ic = 1;
    for(cell_ic = 1; cell_ic <= total_ic; cell_ic++)
    {
        BQ79616BalanceCmd[cell_ic - 1] = commande[cell_ic - 1];
    }
}
void BQ79616PasEqualCmd(u8 cellIc)
{
    static u8 oesel_Flag = 0;//奇偶数节切换标志  0：奇数  1：偶数
    u8 i = 0;
    if((cellIc < 1) || (cellIc > GetDaisyAFENum()))
      return; 
    Data1 = 0x0000000000000000;
    Data2 = 0x0000000000000000;
    //for(cell_ic = 1; cell_ic <= total_ic; cell_ic++)
    //{
        for(i = 0;i<8;i++)
        {
             if((BQ79616BalanceCmd[cellIc - 1] & (0x01 << i)) != 0)
             {
                  Data1 |= (uint64_t)0x02 << (8 * i);
             }
             else
             {
                  Data1 &= (uint64_t)(~((uint64_t)0x03 << (8 * i)));
             }
        }
        for(i = 8;i<16;i++)
        {
             if((BQ79616BalanceCmd[cellIc - 1] & (0x0001 << i)) != 0)
             {
                  Data2 |= (uint64_t)0x02 << (8 * (i - 8));
             }
             else
             {
                  Data2 &= (uint64_t)(~((uint64_t)0x03 << (8 * (i - 8))));
             }
        }
        if(oesel_Flag == 0)
        {
            Data1 &= (uint64_t)(0x0200020002000200);
            Data2 &= (uint64_t)(0x0200020002000200);
            oesel_Flag = 1;
        }
        else
        {          
            Data1 &= (uint64_t)(0x0002000200020002);
            Data2 &= (uint64_t)(0x0002000200020002);
            oesel_Flag = 0; 
        }
        SpiWriteReg(cellIc, CB_CELL16_CTRL, Data2, 8, FRMWRT_SGL_W);   //cell 16-9 (8 byte max write)

        SpiWriteReg(cellIc, CB_CELL8_CTRL, Data1, 8, FRMWRT_SGL_W);    //cell 8-1
        
        SpiWriteReg(cellIc, BAL_CTRL1, 0x02, 1, FRMWRT_SGL_W); //30s duty cycle（设置均衡时间）
        SpiWriteReg(cellIc, BAL_CTRL2, 0x02, 1, FRMWRT_SGL_W); //auto balance and BAL_GO（手动开始均
}

/*=============================================================================================
** @Name      :void BQ79616PasEqualCloseAllPasEqual(u8 cellIc))
** @Input     : void
** @Output    : void
** @Function  : 关闭所有BQ79616PasEqual的被动均衡
** @The notes : 支持16S均衡
**===========================================================================================*/
void BQ79616PasEqualCloseAllPasEqual(u8 cellIc)
{
      if((cellIc < 1) || (cellIc > GetDaisyAFENum()))
        return; 
        SpiWriteReg(cellIc, CB_CELL16_CTRL, 0x00, 8, FRMWRT_SGL_W);   //cell 16-9 (8 byte max write)

        SpiWriteReg(cellIc, CB_CELL8_CTRL, 0x00, 8, FRMWRT_SGL_W);    //cell 8-1
        
        SpiWriteReg(cellIc, BAL_CTRL1, 0x02, 1, FRMWRT_SGL_W); //30s duty cycle（设置均衡时间）
        SpiWriteReg(cellIc, BAL_CTRL2, 0x02, 1, FRMWRT_SGL_W); //auto balance and BAL_GO（手动开始均
//  	u8 cell_ic = 1;
//	for(cell_ic = 1; cell_ic <= GetDaisyAFENum(); cell_ic++)
//	{
//	  
//	  	#if(0 == PORT_COMMTYPE_CHOICE)     //目前串口一次发多个指令可能有问题
//	 	Bq79616WriteReg(cell_ic, CB_CELL16_CTRL, 0x00, 8, FRMWRT_STK_W);   //cell 16-9 (8 byte max write)
//
//		Bq79616WriteReg(cell_ic, CB_CELL8_CTRL, 0x00, 8, FRMWRT_STK_W);    //cell 8-1
//		
//		Bq79616WriteReg(cell_ic, BAL_CTRL1, 0x00, 1, FRMWRT_STK_W); //30s duty cycle（设置均衡时间）
//		Bq79616WriteReg(cell_ic, BAL_CTRL2, 0x00, 1, FRMWRT_STK_W); //auto balance and BAL_GO（手动开始均衡）
//		#else
//		SpiWriteReg(cell_ic, CB_CELL16_CTRL, 0x00, 8, FRMWRT_SGL_W);   //cell 16-9 (8 byte max write)
//
//		SpiWriteReg(cell_ic, CB_CELL8_CTRL, 0x00, 8, FRMWRT_SGL_W);    //cell 8-1
//		
//		SpiWriteReg(cell_ic, BAL_CTRL1, 0x00, 1, FRMWRT_SGL_W); //30s duty cycle（设置均衡时间）
//		SpiWriteReg(cell_ic, BAL_CTRL2, 0x00, 1, FRMWRT_SGL_W); //auto balance and BAL_GO（手动开始均衡）
//		#endif
//	}  
}

/*=============================================================================================
** @Name      : void SetBq79616SampChangeDirection(e_BqBq79616Dire dir)
** @Input     : dir:  0正向  1反向
** @Output    : void
** @Function  : BQ79616菊花链方向切换接口
** @The notes : 用户调用
**===========================================================================================*/
void SetBq79616SampChangeDirection(e_BqBq79616Dire dir)
{
    u8 currentBoard = 0;
    
    if(dir >= eBq79616DireSum)//无效值
    {
        return;
    }
    
    if(eBq79616_DireCommH == dir)
    {
        ;//SpiWriteReg(0, CONTROL1, 0x00, 1, FRMWRT_SGL_W); //COMM_H，默认
    }
    else
    {
        SpiWriteReg(0, CONTROL1, 0x80, 1, FRMWRT_SGL_W); //COMM_L
    }
    SpiWriteReg(0, CONTROL1, 0x20, 1, FRMWRT_SGL_W);     //send wake tone to stack devices 
    
    //DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL (IF A DEVICE RESET OCCURED PRIOR TO THIS)
    SpiWriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
    SpiWriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);
    
    if(eBq79616_DireCommH == dir)
    {
        //ENABLE AUTO ADDRESSING MODE
        SpiWriteReg(0, CONTROL1, 0X01, 1, FRMWRT_ALL_W);
    }
    else
    {
        SpiWriteReg(0, CONTROL1, 0x80, 1, FRMWRT_ALL_W);    //brdcast write reverse 0x80 to address 0x309 (change stack devices direction DIR_SEL =1)
    }
    SpiWriteReg(0, COMM_CTRL, 0x02, 1, FRMWRT_ALL_W);   //brdcast Write 0x02 to address 0x308
    
    if(eBq79616_DireCommL == dir)
    {
        SpiWriteReg(0, CONTROL1, 0x81, 1, FRMWRT_ALL_W);    //brdcast Write 0x81 to address 0x309 (enable BQ7961X-Q1 auto addressing)
    }
    //brdcast Write consecutively to address 0x307 = 0,1,2,3 (address 1-3 assigned to BQ7961X-Q1, 0 assigned to BQ79600-Q1)
    //SET ADDRESSES FOR EVERY BOARD
    //for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
	for(currentBoard=0; currentBoard<(GetBQ79616BQ79616SumIcNumAPI() + BRIDGEDEVICE); currentBoard++)
    {
        if(eBq79616_DireCommH == dir)
        {
            SpiWriteReg(0, DIR0_ADDR, currentBoard, 1, FRMWRT_ALL_W);
        }
        else
        {
            SpiWriteReg(0, DIR1_ADDR, currentBoard, 1, FRMWRT_ALL_W);
        }
    }
    SpiWriteReg(0, COMM_CTRL, 0x02, 1, FRMWRT_ALL_W);//brdcast write 0x02 to address 0x308 (set BQ7961X-Q1 as stack device )
    
    //SET THE HIGHEST DEVICE IN THE STACK AS BOTH STACK AND TOP OF STACK
    //SpiWriteReg(TOTALBOARDS-1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W);    
	SpiWriteReg((GetBQ79616BQ79616SumIcNumAPI() + BRIDGEDEVICE) - 1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W); 
    
    //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
    SpiReadReg(0, OTP_ECC_DATAIN1, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN2, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN3, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN4, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN5, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN6, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN7, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    SpiReadReg(0, OTP_ECC_DATAIN8, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
    
    //OPTIONAL: read back all device addresses
    //for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
	for(currentBoard=0; currentBoard<(GetBQ79616BQ79616SumIcNumAPI() + BRIDGEDEVICE); currentBoard++)
    {
        if(eBq79616_DireCommH == dir)
        {
            SpiReadReg(currentBoard, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
        }
        else
        {
            SpiReadReg(currentBoard, DIR1_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
        }
    }
       
    //OPTIONAL: read register address 0x2001 and verify that the value is 0x14
    SpiReadReg(0, 0x2001, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
}

/*=================================================================================================
** @Name      : void SetBQ79616SampVAlarmFlagAPI(u8 ic_Num, u8 flag,u8 flag)
** @Input     : ic_Num:AFE序号  flag:  0正常  1异常
** @Output    : void
** @Function  : 设置IC电压采样异常标志
** @The notes : 
**===============================================================================================*/
void SetBQ79616SampVAlarmFlagAPI(u8 ic_Num, u8 flag)
{
  	if(ic_Num >= GetDaisyAFENum())
	{
		return;
	}
    if(flag > 1)
    {
        return;
    }
    BQ79616SampVAlarmFlag[ic_Num] = flag;
}

/*=================================================================================================
** @Name      : void SetBQ79616SampTAlarmFlagAPI(u8 flag)
** @Input     : flag:  0正常  1异常
** @Output    : void
** @Function  : 设置IC温度采样异常标志
** @The notes : 
**===============================================================================================*/
void SetBQ79616SampTAlarmFlagAPI(u8 ic_Num, u8 flag)
{
    if(flag > 1)
    {
        return;
    }
    if(ic_Num >= GetDaisyAFENum())
	{
		return;
	}
    BQ79616SampTAlarmFlag[ic_Num] = flag;
}

/*=================================================================================================
** @Name      : u8 GetBQ79616SampVAlarmFlagAPI(u8 ic_Num)
** @Input     : ic_Num：AFE序号
** @Output    : IC采样异常标志
** @Function  : 获取IC电压采样异常标志
** @The notes : 
**===============================================================================================*/
u8 GetBQ79616SampVAlarmFlagAPI(u8 ic_Num)
{
    if(ic_Num >= GetDaisyAFENum())
	{
		return 0;
	}
    return(BQ79616SampVAlarmFlag[ic_Num]);
}

/*=================================================================================================
** @Name      : u8 GetBF8915SampTAlarmFlagAPI(u8 ic_Num)
** @Input     : ic_Num：AFE序号
** @Output    : IC采样异常标志
** @Function  : 获取IC温度采样异常标志
** @The notes : 
**===============================================================================================*/
u8 GetBQ79616SampTAlarmFlagAPI(u8 ic_Num)
{
     if(ic_Num >= GetDaisyAFENum())
	{
		return 0;
	}
    return(BQ79616SampTAlarmFlag[ic_Num]);
}

/*=================================================================================================
** @Name      : u8 GetBQ79616BQ79616SumIcNumAPI(void)
** @Input     : void
** @Output    : 获取IC总数
** @Function  : 
** @The notes : 
**===============================================================================================*/
u8 GetBQ79616BQ79616SumIcNumAPI(void)
{
    return(BQ79616SumIcNum);
}

/*=============================================================================================
** @Static function definition
**===========================================================================================*/
#if(0)
/*=============================================================================================
** @Name      : static void ClrAfeSampVoltTemp(u8 ic_Num)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体电压和温度采样值为0
** @The notes : AFE单体电压温度采样失效时调用,设置采样一遍完成
**===========================================================================================*/
static void ClrAfeSampVoltTemp(u8 ic_Num)
{
    u8 i = 0;
    
    /*所有电池单体电压*/
	for(i = 0; i < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum]; i++)
    {
        /*输出单体电压值为0*/
		WriteSampCellVoltHook(ic_Num, i, 0);
    }
    
    /*所有电池单体温度*/
	for(i = 0; i < gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum]; i++)
    {
        /*输出单体温度值为-41*/
		WriteSampCellTempHook(ic_Num, i, -41);
    }

    /*所有均衡板温体温度*/
	for(i = 0; i < gGBmuHigLevPara_103[eBmuHigLevPara103_BalTNum]; i++)
    {
        /*输出单体温度值为-41*/
		WriteSampBalanTempHook(ic_Num, i, -41);
    }
    
    /*记录电压温度采样一遍完成*/
    WriteVSampFinFlag(1);
    WriteTSampFinFlag(1);
}
#endif

/*=============================================================================================
** @Name      : static void ClrAfeSampCellVolt(u8 ic_Num)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体电压采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellVolt(u8 ic_Num)
{
    u8 i = 0;

    /*所有电池单体电压*/
	for(i = 0; i < GetVoltSampNum(); i++)
    {
        /*输出单体电压值为0*/
		//WriteSampCellVoltHook(ic_Num, i, 0);
        InputBsuCellVolt(ic_Num, i, 0);
    }
}

/*=============================================================================================
** @Name      : static void ClrAfeSampCellTemp(u8 ic_Num)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体温度和均衡板温温度采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellTemp(u8 ic_Num)
{
    u8 i = 0;

    /*所有电池单体温度*/
	for(i = 0; i < GetTempCellNum(); i++)
    {
        /*输出单体温度值为-41*/
		//WriteSampCellTempHook(ic_Num, i, -41);
        InputBsuCellTemp(ic_Num, i, -41);
    }

    /*所有均衡板温体温度*/
	for(i = 0; i < DEFAULT_BTNUM; i++)
    {
        /*输出单体温度值为-41*/
		//WriteSampBalanTempHook(ic_Num, i, -41);
        InputBsuBalanTemp(ic_Num, i, -41);
    }
}

/*=============================================================================================
** @Name      : static u32 ChangeMcuTempVoltToR(u16 Vdata)
** @Input     : ADdata:温度采样电压值
** @Output    : 温度采样NTC阻值 1Ω
** @Function  : 将温度电压值转换为NTC阻值
** @The notes :
**===========================================================================================*/
static u32 ChangeMcuTempVoltToR(u16 Vdata)
{
    u32 tempR = 0;
    
    /*5.0(实测4.9v)*/
    if(Vdata < 4990)
    {
        /*10K电阻分压 V=4.9*(R/(R+100000))*/
        tempR = ((u32)Vdata * 10000) / (4990 - Vdata);//按公式算

        /*100K电阻分压 V=3.0*(R/(R+100000))
        tempR = ((u32)Vdata * 100000) / (3000 - Vdata);//按公式算*/
    }
    /*采样电压超量程(当做最大电压)*/
    else
    {
        tempR = 0xffffffff;
    }
    
    return(tempR);
}

/*=============================================================================================
** @Name      : static u8 SearchCellTempTabByR(u32 data)
** @Input     : data:要查表的数据阻值 1Ω
** @Output    : 应温度下标值
** @Function  : 通过NTC阻值查询单体温度表得到对应温度值
** @The notes :
**===========================================================================================*/
static u8 SearchCellTempTabByR(u32 data)
{
    u8 i = 0;

	/*断线阻值很大(-41℃)*/
    if(data >= sNTCTempTabR[0])
    {
    	return(0);
    }
	/*非断线(-40℃以上)*/
    else
    {
    	/*从-40℃开始检测*/
		for(i = 1; i < CEL_T_TAB_LEN; i++)
		{
			/*达到当前阻值(阻值表降序)*/
			if(data >= sNTCTempTabR[i])
			{
				/*判断与前一个值的中间值*/
				if(data <= ((sNTCTempTabR[i] + sNTCTempTabR[i - 1]) / 2))
				{
					/*没有超过中间值取当前值*/
					return(i);
				}
				/*超过中间值说明偏大取前一个更大阻值*/
				else
				{
					return(i - 1);
				}
			}
		}
    }

	/*阻值很小低于表中阻值*/
    return(CEL_T_TAB_LEN);
}

/*=============================================================================================
** @Name      : static s16 ChangeBalADtoTemp(u16 ADdata)
** @Input     : ADdata:均衡温度采样AD值
** @Output    : 均衡温度值 ℃
** @Function  : 将均衡温度AD值转换为实际温度值
** @The notes : AFE采到的AD值已经是电压值
**===========================================================================================*/
//static s8 ChangeBalADtoTemp(u16 ADdata)
//{
//    s8 temp = 0;
//
//    //3.0V(实测3.0v),16位采样 V=AD*3.0/65535
//    //temp = (u16)(((u32)ADdata * 3000) / 65535);//按公式算
//
//    temp = (((s16)ADdata - 500) + 5) / 10;/*电压对应温度关系：Y = (X - 500) / 10 */
//
//    /*防止越界*/
//    if(temp > 127)
//    {
//        temp = 127;
//    }
//
//    return(temp);
//}

///*=============================================================================================
//** @Name      : static u32 ChangeBalTempVoltToR(u8 channel, u16 Vdata)
//** @Input     : channel:均衡温度采样GPIO通道[0, 1] Vdata:均衡温度采样电压值 1mV
//** @Output    : 均衡温度值 ℃
//** @Function  : 将均衡温度AD值转换为实际温度值
//** @The notes : AFE采到的AD值已经是电压值
//**===========================================================================================*/
//static u32 ChangeBalTempVoltToR(u8 channel, u16 Vdata)
//{
//    u32 temp = 0;
//    //s32 volt = 0;
//
//    //47K电压对应阻值关系：Y=47*X/(5.1-X)
//	if(Vdata >= 5100)
//	{
//		temp = 0xffffffff;
//	}
//	else
//	{/*
//		//电压修正
//		//GPIO1
//		if(0 == channel)
//		{
//			//-40~0℃(-5mv误差)
//			if(Vdata > 2246)
//			{
//				volt = (1089 * (s32)Vdata - 134200) / 1000;
//			}
//			//1~25℃
//			else if(Vdata > 1465)
//			{
//				volt = (1067 * (s32)Vdata - 77100) / 1000;
//			}
//			//26℃以上
//			else
//			{
//				volt = (1012 * (s32)Vdata - 1500) / 1000;
//			}
//		}
//		//GPIO2
//		else
//		{
//			//-40~25℃(-5mv误差)
//			if(Vdata > 1495)
//			{
//				volt = (1009 * (s32)Vdata - 10200) / 1000;
//			}
//			//26℃以上
//			else
//			{
//				volt = (1005 * (s32)Vdata - 1000) / 1000;
//			}
//		}
//        
//		//电压不能超过基准3.0V
//		if(volt >= 3300)
//		{
//			Vdata = 3299;
//		}
//		else
//		{
//			Vdata = volt;
//		}
//        */
//		//电压转换成温感阻值(47K温感)
//		//temp = ((u32)Vdata * 47000) / (3300 - Vdata);
//		temp = ((u32)Vdata * 47000) / (5000 - Vdata);
//	}
//
//    return(temp);
//}
//
///*=============================================================================================
//** @Name      : static u8 SearchBalTempTabByR(u32 data)
//** @Input     : data:要查表的数据阻值 1Ω
//** @Output    : 应温度下标值
//** @Function  : 通过NTC阻值查询均衡板温温度表得到对应温度值
//** @The notes :
//**===========================================================================================*/
//static u8 SearchBalTempTabByR(u32 data)
//{
//    u8 i = 0;
//
//	/*断线阻值很大(-41℃)*/
//    if(data >= sNTCBalTTabR[0])
//    {
//    	return(0);
//    }
//	/*非断线(-40℃以上)*/
//    else
//    {
//    	/*从-40℃开始检测*/
//		for(i = 1; i < BAL_T_TAB_LEN; i++)
//		{
//			/*达到当前阻值(阻值表降序)*/
//			if(data >= sNTCBalTTabR[i])
//			{
//				/*判断与前一个值的中间值*/
//				if(data <= ((sNTCBalTTabR[i] + sNTCBalTTabR[i - 1]) / 2))
//				{
//					/*没有超过中间值取当前值*/
//					return(i);
//				}
//				/*超过中间值说明偏大取前一个更大阻值*/
//				else
//				{
//					return(i - 1);
//				}
//			}
//		}
//    }
//
//	/*阻值很小低于表中阻值*/
//    return(BAL_T_TAB_LEN);
//}

/*=============================================================================================
** @Name      : static u8 CorrectBalTempByKB(u8 channel, u8 temp)
** @Input     : channel:均衡温度采样GPIO通道[0, 1] temp:查表均衡温度[0, 166] 1℃
** @Output    : 修正后的均衡温度值
** @Function  : 通过KB值修正均衡板温温度值
** @The notes : KB值由实际测定
**===========================================================================================*/
static u8 CorrectBalTempByKB(u8 channel, u8 temp)
{
	s32 corrTemp = 0;
/*
	//GPIO1
	if(0 == channel)
	{
		//13℃及以上
		if(temp >= 54)
		{
			corrTemp = (9569 * (s32)temp - 1639) / 10000;
		}
		//-40~12℃
		else
		{
			corrTemp = temp;
		}
	}
	//GPIO2
	else
	{
		//10℃及以上
		if(temp >= 51)
		{
			corrTemp = (9529 * (s32)temp - 12747) / 10000;
		}
		//-40~9℃
		else
		{
			corrTemp = (10168 * (s32)temp - 17877) / 10000;
		}
	}
*/
    corrTemp = temp;
	//不能是负值
	if(corrTemp < 0)
	{
		corrTemp = 0;
	}
	//不能超过最大值
	else if(corrTemp > 167)
	{
		corrTemp = 167;
	}

	return((u8)corrTemp);
}

/*=============================================================================================
** @Name      : SetLv4051TempSampChan(e_BqTempSampStep Num)
** @Input     : void
** @Output    : void
** @Function  : 温度采集通道选择
** @The notes : 
**===========================================================================================*/
static void SetLv4051TempSampChan(e_BqTempSampStep Num)
{
    if(0 == Num)//temp1、temp7     0  0  0 
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x2D, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6低电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x2D, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6低电平
        #endif
    }
    else if(1 == Num)//temp5、temp8  1 0 0
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x2D, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6低电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x2D, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6低电平    
        #endif
    }
    else if(2 == Num)//temp10、temp9  0 1 0
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x2C, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6低电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x2C, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6低电平     
        #endif
    }
    else if(3 == Num)//temp14、temp11  1 1 0
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x2C, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6低电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x2C, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6低电平   
        #endif
    }
    else if(4 == Num)//temp2、temp12  0 0 1
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x25, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6高电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x25, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6高电平     
        #endif
    }
    else if(5 == Num)//temp3、temp13  1 0 1
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x25, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6高电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x25, 1, FRMWRT_STK_W);//GPIO5低电平、GPIO6高电平     
        #endif
    }
    else if(6 == Num)//temp4、temp15  0 1 1
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x24, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6高电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x28, 1, FRMWRT_STK_W);//GPIO4低电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x24, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6高电平       
        #endif
    }
    else if(7 == Num)//temp6、temp16  1 1 1
    {
        #if(0 == PORT_COMMTYPE_CHOICE)
        Bq79616WriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        Bq79616WriteReg(0, GPIO_CONF3, 0x24, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6高电平
        #else
        SpiWriteReg(0, GPIO_CONF2, 0x20, 1, FRMWRT_STK_W);//GPIO4高电平、(GPIO3高阻态)
        SpiWriteReg(0, GPIO_CONF3, 0x24, 1, FRMWRT_STK_W);//GPIO5高电平、GPIO6高电平       
        #endif
    }
}

