
#include "main.h"

#define BattECCount_NUM1    0
#define BattECCount_NUM2    1
#define BattECCount_NUM3    2
#define BattECCount_NUM4    3
#define BattECCount_NUM5    4
#define BattECCount_NUM6    5
#define BattECCount_NUM7    6
#define BattECCount_NUM8    7
#define BattECCount_NUM9    8
#define BattECCount_NUM10   9


//display
#define MAINSCREEN              1
#define MENUSCREEN             8
#define ALARMPOPSCREEN     5
#define PASSWORDSCREEN    34
#define CONFIRMSCREEN        35
#define UPDATESCREEN          36
#define SHELFCUESCREEN       37
#define NOACTIONSCREEN      38


//屏类型

//其它屏：
#define ZERO_SCREEN  0 //空白屏

//状态信息显示屏：
#define INFO_SCREEN  0x20
#define  INFO_Zero_SCREEN   (0x20+1)   /*1*/
#define  INFO_Sys_SCREEN  (0x20+2)
#define  INFO_Controller_SCREEN (0x20+3)
#define  INFO_Rect_SCREEN (0x20+4)
#define  INFO_ActiveAlarm_SCREEN (0x20+5)
#define  INFO_HistAlarm_SCREEN (0x20+6)
#define  UPDATE_POP_SCREEN (0x20+7)  // 升级提示
#define  INFO_MPPT_SCREEN  (0x20+8)
#define  INFO_STAT_SCREEN  (0x20+9)
#define  INFO_INSUL_SCREEN  (0x20+10)
#define  INFO_USER_SCREEN  (0x20+11)
#define  INFO_BMS_SCREEN   (0x20+12)

//菜单屏：
#define MENU_SCREEN  0x40
#define MENU_Main_SCREEN (0x40+1) /*8*/
#define MENU_Status_SCREEN (0x40+2)    
#define MENU_Setting_SCREEN (0x40+3)
#define MENU_Alarm_SCREEN (0x40+4)
#define MENU_Batt_SCREEN (0x40+5)
#define MENU_BattRecord_SCREEN (0x40+6)
#define MENU_USER_SCREEN (0x40+7)

#define CONFIRM_SCREEN (0x40+8)   // 提示需进行确认
#define SHELFCUE_POP_SCREEN (0x40+9) //  槽位提示
#define NOACTION_SCREEN (0x40 + 10)  // 提示没进行相应操作


//设置屏：
#define CFG_SCREEN  0x80
 #define  CFG_AlarmLevel_SCREEN   (0x80+1) 
 #define  CFG_AlarmSettings_SCREEN  (0x80+2)
 #define  CFG_DISettings_SCREEN  (0x80+3)
 #define  CFG_BattBasic_SCREEN  (0x80+4)
 #define  CFG_BattLVD_SCREEN  (0x80+5)
 #define  CFG_BattCharge_SCREEN (0x80+6)
 #define  CFG_BattTest_SCREEN (0x80+7)
 #define  CFG_BattTempComp_SCREEN (0x80+8)
 #define  CFG_AC_SCREEN (0x80+9)
 #define  CFG_DC_SCREEN  (0x80+10)
 #define  CFG_Rect_SCREEN (0x80+11)
 #define  CFG_EnviSensor_SCREEN (0x80+12)
 #define  CFG_DG_SCREEN (0x80+13)
 #define  CFG_EnviHeater_SCREEN (0x80+14)
 #define  CFG_SysPara_SCREEN (0x80+15)
 #define  CFG_CommPara_SCREEN  (0x80+16)
 #define  CFG_Energy_SCREEN (0x80+17)
 #define  CFG_FastSet_SCREEN (0x80+18)
 #define  CFG_CtlOut_SCREEN (0x80+19)
 #define CFG_CtlOut2_SCREEN  (0x80+20) // 手动输出2
 #define PASSWORD_SCREEN  (0x80+21) 
 #define  CFG_Mppt_SCREEN			(0x80+22) 
 #define  CFG_DG2_SCREEN (0x80+23)
 #define  CFG_TEST_SCREEN (0x80+24)
 #define  CFG_USER_SCREEN (0x80+25)

/*system name source*/
#define SYS_000     {"24V/100","24V/100"}
#define SYS_001     {"24V/300","24V/300"}
#define SYS_002     {"24V/500","24V/500"}
#define SYS_003     {"24V/1000","24V/1000"}
#define SYS_004     {"24V/SET","24V/SET"}
#define SYS_005     {"48V/100","48V/100"}
#define SYS_006     {"48V/300","48V/300"}
#define SYS_007     {"48V/500","48V/500"}
#define SYS_008     {"48V/1000","48V/1000"}
#define SYS_009     {"48V/SET","48V/SET"}

#define SYSTEM_ALL  { \
SYS_000, SYS_001, SYS_002, SYS_003, SYS_004, SYS_005, SYS_006, SYS_007, SYS_008, SYS_009,\
}

/*alarm name source*/
#define ALARM_000   {"",""}
#define ALARM_001   {"Alarm Block","告警阻塞"}
#define ALARM_002   {"SPD Alarm","防雷器告警"}
#define ALARM_003   {"DG1 Open Fail","油机1开启失败"}
#define ALARM_004   {"DG2 Open Fail","油机2开启失败"}
#define ALARM_005   {"L.F. Fail","支路 断"}
#define ALARM_006   {"Aux Load Fails","负载辅助支路断"}
#define ALARM_007   {"BattFuse  Fail","电池支路 断"}
#define ALARM_008   {"LVD1Ctrl Fail","负载下电回采错"}
#define ALARM_009   {"LVD2Ctrl Fail","电池保护回采错"}
#define ALARM_010   {"Batt Imbalance","电池不平衡"}  /*电池中点电压*/
#define ALARM_011   {"SelfDetect Fail","监控模块故障"}
#define ALARM_012   {"Manual Mode","手动管理"}
#define ALARM_013   {"Boost Charge","电池均充"}
#define ALARM_014   {"Batt Test","电池测试"}
#define ALARM_015   {"Batt Discharge","电池放电"}
#define ALARM_016   {"Curr Imbalance","系统电流不平衡"}
#define ALARM_017   {"DC Volt Fail","输出电压异常"}
#define ALARM_018   {"ShortTest Fail","电池短测试故障"}
#define ALARM_019   {"Batt Test Fail","电池测试故障"}
#define ALARM_020   {"LVD Protect","负载保护下电"}
#define ALARM_021   {"BLVD","电池保护"}
#define ALARM_022   {"Mains Failure","交流停电"}
#define ALARM_023   {"Rect OverLoad","模块过载"}
#define ALARM_024   {"Save Power","节能"}
#define ALARM_025   {"Rect Lost","模块丢失"}
#define ALARM_026   {"System Maintain","系统保养时间到"}
#define ALARM_027   {"MultiRect Alarm","多整流模块告警"}
#define ALARM_028   {"Rect  Comm Fail","模块  通讯中断"}
#define ALARM_029   {"Rect  AC Fail","模块  交流停电"}
#define ALARM_030   {"Rect  TempHigh","模块  过温"}
#define ALARM_031   {"Rect  Fail","模块  故障"}
#define ALARM_032   {"Rect  Protect","模块  保护"}
#define ALARM_033   {"Rect  FanFail","模块  风扇故障"}
#define ALARM_034   {"Rect  Derated","模块  限功率"}
#define ALARM_035   {"Rect  LoadShare","模块  不均流"}
#define ALARM_036   {"Rect  HVSD","模块  过压"}
#define ALARM_037   {"SavePowerFault","节能故障"}
#define ALARM_038   {"LVD fail","LVD故障"}
#define ALARM_039   {"Solar Comm Fail","太阳能通讯中断"}
#define ALARM_040   {"Solar Major","太阳能重要告警"}

#define ALARM_041   {"DG Start Fail","油机开启失败"}

#define ALARM_042   {"Bat1 Symm Fault","电池1不平衡"}
#define ALARM_043   {"Bat2 Symm Fault","电池2不平衡"}
#define ALARM_044   {"Bat3 Symm Fault","电池3不平衡"}
#define ALARM_045   {"Bat4 Symm Fault","电池4不平衡"}

#define ALARM_046   {"DCEM1 Comm Fail","DCEM1通讯中断"}
#define ALARM_047   {"DCEM2 Comm Fail","DCEM2通讯中断"}

#define ALARM_048	{"SCon Lost","光伏模块丢失"}
#define ALARM_049	{"SCon  Comm Fail","光模  通讯中断"}
#define ALARM_050	{"SCon  TempHigh","光模  过温"}
#define ALARM_051	{"SCon  Fail","光模  故障"}
#define ALARM_052   {"SCon  Protect","光模  保护"}
#define ALARM_053	{"SCon  FanFail","光模  风扇故障"}
#define ALARM_054   {"SCon  Derated","光模  限功率"}
#define ALARM_055	{"SCon  LoadShare","光模  不均流"}
#define ALARM_056   {"SCon  InPutHigh","光模  输入过压"}
#define ALARM_057   {"PV    Fail","光板  故障"}

#define ALARM_058	{"DCDC Lost","DCDC丢失"}
#define ALARM_059	{"DCDC  Comm Fail","DCDC  通讯中断"}
#define ALARM_060	{"DCDC  TempHigh","DCDC  过温"}
#define ALARM_061	{"DCDC  Fail","DCDC  故障"}
#define ALARM_062   {"DCDC  Protect","DCDC  保护"}
#define ALARM_063	{"DCDC  FanFail","DCDC  风扇故障"}
#define ALARM_064   {"DCDC  HVSD","DCDC  过压"}
#define ALARM_065   {"MultiDCDC Alarm","多DCDC模块告警"}
#define ALARM_066   {"LLVD1","负载下电1"}
#define ALARM_067   {"LLVD2","负载下电2"}
#define ALARM_068   {"LLVD3","负载下电3"}
#define ALARM_069   {"LLVD4","负载下电4"}

#define ALARM_ALL   { \
ALARM_000, ALARM_001, ALARM_002, ALARM_003, ALARM_004, ALARM_005, ALARM_006, ALARM_007, ALARM_008, ALARM_009,\
ALARM_010, ALARM_011, ALARM_012, ALARM_013, ALARM_014, ALARM_015, ALARM_016, ALARM_017, ALARM_018, ALARM_019,\
ALARM_020, ALARM_021, ALARM_022, ALARM_023, ALARM_024, ALARM_025, ALARM_026, ALARM_027, ALARM_028, ALARM_029,\
ALARM_030, ALARM_031, ALARM_032, ALARM_033, ALARM_034, ALARM_035, ALARM_036, ALARM_037, ALARM_038, ALARM_039,\
ALARM_040, ALARM_041, ALARM_042, ALARM_043, ALARM_044, ALARM_045, ALARM_046, ALARM_047, ALARM_048, ALARM_049,\
ALARM_050,ALARM_051, ALARM_052, ALARM_053, ALARM_054, ALARM_055, ALARM_056, ALARM_057,  ALARM_058, ALARM_059,\
ALARM_060,ALARM_061, ALARM_062, ALARM_063, ALARM_064, ALARM_065, ALARM_066, ALARM_067,  ALARM_068, ALARM_069,\
}

/*analog alarm name source*/
#define AALARM_00   {"DC Volt Low-","DC Volt Low","DC Volt High","DC Volt High+","","",\
                     "直流欠压","直流电压低","直流电压高","直流过压","",""}
#define AALARM_01   {"Batt Volt Low-","Batt Volt Low","Batt Volt High","Batt Volt High+","","",\
                     "电池欠压","电池电压低","电池电压高","电池过压","",""}
#define AALARM_02   {"Batt3 Discharge","Batt3 Discharge","Batt3 Over Chg","Batt3 Over Chg","","",\
                     "电池3放电","电池3放电","电池3充电过流","电池3充电过流","",""}
//#define AALARM_02   {"","","","","","",\
							        "","","","","",""}
#define AALARM_03   {"Batt1 Discharge","Batt1 Discharge","Batt1 Over Chg","Batt1 Over Chg","","",\
                     "电池1放电","电池1放电","电池1充电过流","电池1充电过流","",""}
#define AALARM_04   {"Batt2 Discharge","Batt2 Discharge","Batt2 Over Chg","Batt2 Over Chg","","",\
                     "电池2放电","电池2放电","电池2充电过流","电池2充电过流","",""}
#define AALARM_05   {"Temp1 Low-","Temp1 Low","Temp1 High","Temp1 High+","T1 No Probe","T1 Probe Error",\
                     "温度1过低","温度1低","温度1高","温度1过高","温度1无传感器","温度1传感器故障"}
#define AALARM_06   {"Temp2 Low-","Temp2 Low","Temp2 High","Temp2 High+","T2 No Probe","T2 Probe Error",\
                     "温度2过低","温度2低","温度2高","温度2过高","温度2无传感器","温度2传感器故障"}
#define AALARM_07   {"Temp3 Low-","Temp3 Low","Temp3 High","Temp3 High+","T3 No Probe","T3 Probe Error",\
                     "温度3过低","温度3低","温度3高","温度3过高","温度3无传感器","温度3传感器故障"}
//#define AALARM_08   {"AC1 Ph-A Fail","AC1-A Volt Low","AC1-A Volt High","AC1-A Volt High","","",\
                     "1路A相缺相","1路A相欠压","1路A相过压","1路A相过压","",""}
#define AALARM_08   {"AC Ph-A Fail","AC-A Volt Low","AC-A Volt High","AC-A Volt High","","",\
                     "交流A相缺相","交流A相欠压","交流A相过压","交流A相过压","",""}
//#define AALARM_09   {"AC1 Ph-B Fail","AC1-B Volt Low","AC1-B Volt High","AC1-B Volt High","","",\
                     "1路B相缺相","1路B相欠压","1路B相过压","1路B相过压","",""}
#define AALARM_09   {"AC Ph-B Fail","AC-B Volt Low","AC-B Volt High","AC-B Volt High","","",\
                     "交流B相缺相","交流B相欠压","交流B相过压","交流B相过压","",""}
//#define AALARM_10   {"AC1 Ph-C Fail","AC1-C Volt Low","AC1-C Volt High","AC1-C Volt High","","",\
                     "1路C相缺相","1路C相欠压","1路C相过压","1路C相过压","",""}
#define AALARM_10   {"AC Ph-C Fail","AC-C Volt Low","AC-C Volt High","AC-C Volt High","","",\
                     "交流C相缺相","交流C相欠压","交流C相过压","交流C相过压","",""}
#define AALARM_11   {"AC2 Ph-A Fail","AC2-A Volt Low","AC2-A Volt High","AC2-A Volt High","","",\
                     "2路A相缺相","2路A相欠压","2路A相过压","2路A相过压","",""}
#define AALARM_12   {"AC2 Ph-B Fail","AC2-B Volt Low","AC2-B Volt High","AC2-B Volt High","","",\
                     "2路B相缺相","2路B相欠压","2路B相过压","2路B相过压","",""}
#define AALARM_13   {"AC2 Ph-C Fail","AC2-C Volt Low","AC2-C Volt High","AC2-C Volt High","","",\
                     "2路C相缺相","2路C相欠压","2路C相过压","2路C相过压","",""}           
#define AALARM_14   {"SerialTemp Low-","SerialTemp Low","SerialTemp High","SerialTemp High+","No SerialProbe","SerialProbe Err",\
                     "串行温度过低","串行温度低","串行温度高","串行温度过高","无串行传感器","串行传感器故障"}
#define AALARM_15   {"SerialHum Low-","SerialHum Low","SerialHum High","SerialHum High+","No SerialProbe","SerialProbe Err",\
                     "串行湿度过低","串行湿度低","串行湿度高","串行湿度过高","无串行传感器","串行传感器故障"}

#define ANALOGALARM_ALL   { \
AALARM_00, AALARM_01, AALARM_02, AALARM_03, AALARM_04, AALARM_05, AALARM_06, AALARM_07,\
AALARM_08, AALARM_09, AALARM_10, AALARM_11, AALARM_12, AALARM_13, AALARM_14, AALARM_15,\
}


/* 1024个告警表每个告警对应的名称-----
  4种告警名称来源 : 
         0x0000~0x03FF - ALARM_ALL
         0x0400~0x07FF - ANALOGALARM_ALL
         0x0800~0x0BFF - ALIAS_ALL
         0x0C00~0x0FFF - SetData区                                                                           */                                                                                                                          
#define ALARM_TABLE_NAME  {\
1, 0, 0,0,46,47, 0x0400+0, 0x0400+1, 0x0400+2, 0x0400+3,/*0-9*/\
0x0400+4, 0x0400+5, 0x0400+6, 0x0400+8,0x0400+9,0x0400+10, 0x0400+11, 0x0400+12, 0x0400+13, 0,  /*10-19*/\
0, 0, 0, 8, 9, 0, 11, 12, 13, 14,   /*20-29*/\
15, 16, 17, 18, 19, 20, 21, 22, 23, 24,   /*30-39*/\
25, 26, 27, 28, 29, 30, 31, 32, 33, 34,   /*40-49*/\
35, 36, 0, 0, 0, 0, 0, 0, 0, 37,   /*50-59*/\
28, 28, 28, 28, 28,28,28,28,28,28,   /*60-69*/\
28, 28, 28, 28, 28,28,28,28,28,28,   /*70-79*/\
28, 28, 28, 28, 28,28,28,28,28,28,   /*80-89*/\
29, 29, 29, 29, 29, 29, 29, 29, 29, 29,/*90-99*/\
29, 29, 29, 29, 29, 29, 29, 29, 29, 29,/*100-109*/\
29, 29, 29, 29, 29, 29, 29, 29, 29, 29,/*110-119*/\
30, 30, 30, 30, 30, 30, 30, 30, 30, 30,/*120-129*/\
30, 30, 30, 30, 30, 30, 30, 30, 30, 30,/*130-139*/\
30, 30, 30, 30, 30, 30, 30, 30, 30, 30,/*140-149*/\
31, 31, 31, 31, 31, 31, 31, 31, 31, 31,/*150-169*/\
31, 31, 31, 31, 31, 31, 31, 31, 31, 31,/*160-169*/\
31, 31, 31, 31, 31, 31, 31, 31, 31, 31,/*170-179*/\
32, 32, 32, 32, 32, 32, 32, 32, 32, 32,/*180-189*/\
32, 32, 32, 32, 32, 32, 32, 32, 32, 32,/*190-199*/\
32, 32, 32, 32, 32, 32, 32, 32, 32, 32,    /*200-209*/\
33, 33, 33, 33, 33, 33, 33, 33, 33, 33,   /*210-219*/\
33, 33, 33, 33, 33, 33, 33, 33, 33, 33,    /*220-229*/\
33, 33, 33, 33, 33, 33, 33, 33, 33, 33,    /*230-239*/\
34, 34, 34, 34, 34, 34, 34, 34, 34, 34,   /*240-249*/\
34, 34, 34, 34, 34, 34, 34, 34, 34, 34,    /*250-259*/\
34, 34, 34, 34, 34, 34, 34, 34, 34, 34,    /*260-269*/\
35, 35, 35, 35, 35, 35, 35, 35, 35, 35,     /*270-279*/\
35, 35, 35, 35, 35, 35, 35, 35, 35, 35,     /*280-289*/\
35, 35, 35, 35, 35, 35, 35, 35, 35, 35,     /*290-299*/\
36, 36, 36, 36, 36, 36, 36, 36, 36, 36,    /*300-309*/\
36, 36, 36, 36, 36, 36, 36, 36, 36, 36,    /*310-319*/\
36, 36, 36, 36, 36, 36, 36, 36, 36, 36,    /*320-329*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    /*330-339*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    /*340-349*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*350-359*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*360-369*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*370-379*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*380-389*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*390-399*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*400-409*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*410-419*/\
0, 0, 0, 0, 0, 0, 0, 41, 0, 66,   /*420-429*/\
67, 68, 69, 0, 0, 0, 0, 0, 0, 0,   /*430-439*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*440-449*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*450-459*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*460-469*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*470-479*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*480-489*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*490-499*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*500-509*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*510-519*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*520-529*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*530-539*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*540-549*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*550-559*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*560-569*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*570-579*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*580-589*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*590-599*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*600-609*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*610-619*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*620-629*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*630-639*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*640-649*/\
0, 0, 0, 0x0C00+1, 0x0C00+2, 0x0C00+3,0x0C00+4,0x0C00+5,0x0C00+6,0x0C00+7,  /*650-659*/\
0x0C00+8,  2, 3, 4, 38, 5, 5, 5, 5, 5,   /*660-669*/\
5, 5, 5, 5, 6, 7, 7, 7, 7, 7,   /*670-679*/\
42, 43, 44, 45, 0, 0, 0, 0, 0,0,   /*680-689*/\
0, 0, 0, 0x0400+7, 0x0400+14,0x0400+15,  0, 0, 0, 0,  /*690-699*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*700-709*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*710-719*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*720-729*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*730-739*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*740-749*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*750-759*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*760-769*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 58,   /*770-779*/\
59, 59, 59, 59, 59, 59, 60, 60, 60, 60,   /*780-789*/\
60, 60, 61, 61, 61, 61, 61, 61, 62, 62,   /*790-799*/\
62, 62, 62, 62, 63, 63, 63, 63, 63, 63,   /*800-809*/\
64, 64, 64, 64, 64, 64, 0, 0, 0, 0,   /*810-819*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*820-829*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*830-839*/\
65, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*840-849*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*850-859*/\
48, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*860-869*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*870-879*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*880-889*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*890-899*/\
49, 49, 49, 49, 49, 49, 49, 49, 49, 49,   /*900-909*/\
50, 50, 50, 50, 50, 50, 50, 50, 50, 50,   /*910-919*/\
51, 51, 51, 51, 51, 51, 51, 51, 51, 51,   /*920-929*/\
52, 52, 52, 52, 52, 52, 52, 52, 52, 52,   /*930-939*/\
53, 53, 53, 53, 53, 53, 53, 53, 53, 53,   /*940-949*/\
54, 54, 54, 54, 54, 54, 54, 54, 54, 54,   /*950-959*/\
55, 55, 55, 55, 55, 55, 55, 55, 55, 55,   /*960-969*/\
56, 56, 56, 56, 56, 56, 56, 56, 56, 56,   /*970-9479*/\
57, 57, 57, 57, 57, 57, 57, 57, 57, 57,   /*980-989*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	 /*990-999*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*1000-1009*/\
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /*1010-1019*/\
0, 0, 0, 0,     /*1020-1023*/\
}

/*alias source*/
#define ALIAS_000   {"",""}
#define ALIAS_001   {"N","否"}
#define ALIAS_002   {"No","否"}
#define ALIAS_003   {"Yes","是"}
#define ALIAS_004   {"Y","有"}
#define ALIAS_005   {"N","无"}
#define ALIAS_006   {"Yes","有"}
#define ALIAS_007   {"No","无"}
#define ALIAS_008   {"Nor","正常"}
#define ALIAS_009   {"Alm","告警"}
#define ALIAS_010   {"Normal","正常"}
#define ALIAS_011   {"Alarm","告警"}
#define ALIAS_012   {"OB","一般告警"}
#define ALIAS_013   {"MA","严重告警"}
#define ALIAS_014   {"CA","紧急告警"}
#define ALIAS_015   {"On","开"}
#define ALIAS_016   {"Off","关"}
#define ALIAS_017   {"FC","浮充"}
#define ALIAS_018   {"EC","均充"}
#define ALIAS_019   {"TEST","测试"}
#define ALIAS_020   {"TempComp","温补"}
#define ALIAS_021   {"Charge","充电"}
#define ALIAS_022   {"Stop","停止"}
#define ALIAS_023   {"Full","充满"}
#define ALIAS_024   {"Cycle EC","定时均充"}
#define ALIAS_025   {"CC Test","恒流测试"}
#define ALIAS_026   {"Short Test","短测试"}
#define ALIAS_027   {"Charge","充电"}
#define ALIAS_028   {"Stop","停止"}//TXY
#define ALIAS_029   {"Reconnect","上电"}
#define ALIAS_030   {"Discon","下电"}
#define ALIAS_031   {"RS485","RS485"}
#define ALIAS_032   {"MODEM","MODEM"}
#define ALIAS_033   {"2400","2400"}
#define ALIAS_034   {"4800","4800"}
#define ALIAS_035   {"9600","9600"}
#define ALIAS_036   {"19200","19200"}
#define ALIAS_037   {"Chinese","中文"}
#define ALIAS_038   {"English","英文"}
#define ALIAS_039   {"Res","复位"}
#define ALIAS_040   {"Def","默认"}
#define ALIAS_041   {"Other","其它"}
#define ALIAS_042   {"LA","铅酸"}
#define ALIAS_043   {"Nimh","镍氢"}
#define ALIAS_044   {"Detect","检测"}
#define ALIAS_045   {"Stop","待机"}
#define ALIAS_046   {"",""}
#define ALIAS_047   {"Bat.T2","电池温度2"}
#define ALIAS_048   {"Open","断开"}
#define ALIAS_049   {"Close","闭合"}
#define ALIAS_050   {"YDN23","YDN23"}
#define ALIAS_051   {"1","1"}
#define ALIAS_052   {"2","2"}
#define ALIAS_053   {"3","3"}
#define ALIAS_054   {"4","4"}
#define ALIAS_055   {"5","5"}
#define ALIAS_056   {"6","6"}
#define ALIAS_057   {"7","7"}
#define ALIAS_058   {"8","8"}
#define ALIAS_059   {"Y","是"}
#define ALIAS_060   {"3min","3min"}
#define ALIAS_061   {"10min","10min"}
#define ALIAS_062   {"1h","1h"}
#define ALIAS_063   {"4h","4h"}
#define ALIAS_064   {"Open","常开"}
#define ALIAS_065   {"Volt","电压"}
#define ALIAS_066   {"Time","时间"}
#define ALIAS_067   {"1-PH","单相"}
#define ALIAS_068   {"3-PH","三相"}
#define ALIAS_069   {"Enable","允许"}
#define ALIAS_070   {"Disable","禁止"}
#define ALIAS_071   {"Temp1","温度1"}
#define ALIAS_072   {"Temp2","温度2"}
#define ALIAS_073   {"DI1","DI1"}
#define ALIAS_074   {"LFP","铁锂"}
#define ALIAS_075   {"Lock","锁止"}
#define ALIAS_076   {"Unlock","解锁"}
#define ALIAS_077   {"DC OFF","关直流"}
#define ALIAS_078   {"AC ON","开交流"}
#define ALIAS_079   {"AC OFF","关交流"}
#define ALIAS_080   {"Reset","复位"}
#define ALIAS_081   {"Load Fuse Alarm","负载支路断"}
#define ALIAS_082   {"Batt Fuse Alarm","电池支路断"}
#define ALIAS_083   {"Batt Over Chg","充电过流"}
#define ALIAS_084   {"Temp Low","温度低"}
#define ALIAS_085   {"Temp High1","温度高"}
#define ALIAS_086   {"Temp High2","温度过高"}
#define ALIAS_087   {"AC PH Fail","交流缺相"}
#define ALIAS_088   {"AC Low","交流欠压"}
#define ALIAS_089   {"AC High","交流过压"}
#define ALIAS_090   {"History","历史告警"}
#define ALIAS_091   {"Rect Lost","模块丢失"}
#define ALIAS_092   {"TestFail","测试"}
#define ALIAS_093   {"ShortTest","短测试"}
#define ALIAS_094   {"Maintain","系统保养"}
#define ALIAS_095   {"ESaveFail","节能异常"}
#define ALIAS_096   {"Batt Record","电池记录"}
#define ALIAS_097   {"Null","空"} 
#define ALIAS_098   {"1200","1200"}
#define ALIAS_099   {"Grid","市电"}
#define ALIAS_100   {"DG","油机"}
#define ALIAS_101   {"Grid+DG","市电+油机"}

#define ALIAS_102   {"Battery","电池"}
#define ALIAS_103   {"XXX","XXX"}
#define ALIAS_104   {"XXX","XXX"}
#define ALIAS_105   {"SPD Alarm","防雷器告警"}
#define ALIAS_106   {"Temp3","温度3"}

#define ALIAS_107   {"Op1","模式1"}
#define ALIAS_108   {"Op2","模式2"}
#define ALIAS_109   {"Op3","模式3"}
#define ALIAS_110   {"Op4","模式4"}

#define ALIAS_111   {"Clr1","清零1"}
#define ALIAS_112   {"Clr2","清零2"}
#define ALIAS_113   {"Clr3","清零3"}
#define ALIAS_114   {"Clr4","清零4"}
#define ALIAS_115   {"Dis1","禁止1"}
#define ALIAS_116   {"Dis2","禁止2"}
#define ALIAS_117   {"Dis3","禁止3"}
#define ALIAS_118   {"Dis4","禁止4"}
#define ALIAS_119   {"EnAll","使能"}

#define ALIAS_120   {"Batt","电池"}
#define ALIAS_121   {"Load","负载"}

#define ALIAS_122   {"Bat Symm Fault","电池不平衡"}
#define ALIAS_123	{"AC+Rect","开关电源"}
#define ALIAS_124   {"Switch.S.C","集成互补"}
#define ALIAS_125	{"AC+Solar","光电混合"}
#define ALIAS_126   {"Solar","太阳能"}
#define ALIAS_127	{"SConLost","光模丢失"}
#define ALIAS_128	{"Stat.","统计信息"}
#define ALIAS_129	{"CommFail","通讯中断"}
#define ALIAS_130	{"Save","节能"}
#define ALIAS_131	{"Stably","稳定"}
#define ALIAS_132   {"DI","DI"}

#define ALIAS_133   {"DI2","DI2"}
#define ALIAS_134   {"DI3","DI3"}
#define ALIAS_135   {"DI4","DI4"}
#define ALIAS_136   {"DI5","DI5"}
#define ALIAS_137   {"DI6","DI6"}
#define ALIAS_138   {"DI7","DI7"}
#define ALIAS_139   {"DI8","DI8"}


#define ALIAS_140	{"Com","串口"}
#define ALIAS_141   {"Net","网口"}
#define ALIAS_142   {"CTCC","电信"}
#define ALIAS_143	{"Other","其它"}
#define ALIAS_144   {"USER","用户"}
#define ALIAS_145   {"",""}
#define ALIAS_146   {"",""}
#define ALIAS_147   {"",""}
#define ALIAS_148   {"",""}
#define ALIAS_149   {"",""}


#define ALIAS_ALL   { \
	ALIAS_000, ALIAS_001, ALIAS_002, ALIAS_003, ALIAS_004, ALIAS_005, ALIAS_006, ALIAS_007, ALIAS_008, ALIAS_009, \
	ALIAS_010, ALIAS_011, ALIAS_012, ALIAS_013, ALIAS_014, ALIAS_015, ALIAS_016, ALIAS_017, ALIAS_018, ALIAS_019, \
	ALIAS_020, ALIAS_021, ALIAS_022, ALIAS_023, ALIAS_024, ALIAS_025, ALIAS_026, ALIAS_027, ALIAS_028, ALIAS_029, \
	ALIAS_030, ALIAS_031, ALIAS_032, ALIAS_033, ALIAS_034, ALIAS_035, ALIAS_036, ALIAS_037, ALIAS_038, ALIAS_039, \
	ALIAS_040, ALIAS_041, ALIAS_042, ALIAS_043, ALIAS_044, ALIAS_045, ALIAS_046, ALIAS_047, ALIAS_048, ALIAS_049, \
	ALIAS_050, ALIAS_051, ALIAS_052, ALIAS_053, ALIAS_054, ALIAS_055, ALIAS_056, ALIAS_057, ALIAS_058, ALIAS_059, \
	ALIAS_060, ALIAS_061, ALIAS_062, ALIAS_063, ALIAS_064, ALIAS_065, ALIAS_066, ALIAS_067, ALIAS_068, ALIAS_069, \
	ALIAS_070, ALIAS_071, ALIAS_072, ALIAS_073, ALIAS_074, ALIAS_075, ALIAS_076, ALIAS_077, ALIAS_078, ALIAS_079, \
	ALIAS_080, ALIAS_081, ALIAS_082, ALIAS_083, ALIAS_084, ALIAS_085, ALIAS_086, ALIAS_087, ALIAS_088, ALIAS_089, \
	ALIAS_090, ALIAS_091, ALIAS_092, ALIAS_093, ALIAS_094, ALIAS_095, ALIAS_096, ALIAS_097, ALIAS_098, ALIAS_099, \
	ALIAS_100, ALIAS_101, ALIAS_102, ALIAS_103, ALIAS_104, ALIAS_105, ALIAS_106, ALIAS_107, ALIAS_108, ALIAS_109, \
	ALIAS_110, ALIAS_111, ALIAS_112, ALIAS_113, ALIAS_114, ALIAS_115, ALIAS_116, ALIAS_117, ALIAS_118, ALIAS_119, \
	ALIAS_120, ALIAS_121, ALIAS_122, ALIAS_123, ALIAS_124, ALIAS_125, ALIAS_126, ALIAS_127,  ALIAS_128,ALIAS_129,\
	ALIAS_130, ALIAS_131, ALIAS_132, ALIAS_133, ALIAS_134, ALIAS_135, ALIAS_136, ALIAS_137,  ALIAS_138,ALIAS_139,\
	ALIAS_140, ALIAS_141, ALIAS_142, ALIAS_143, ALIAS_144, ALIAS_145, ALIAS_146, ALIAS_147,  ALIAS_148,ALIAS_149,\
}
/*行对象 别名 line title */
#define LTitle_000   {"",""}
#define LTitle_001   {"NeedVolt:","需求电压:"}
#define LTitle_002   {"NeedCurr:","需求电流:"}
#define LTitle_003   {"ChrgTime:","充电时间:"}
#define LTitle_004   {"ChrgEng:","充电电量:"}
#define LTitle_005   {"BMS Info","BMS信息"}
#define LTitle_006   {"BattType:","电池类型:"}
#define LTitle_007   {"Phase A:","A相电压:"}
#define LTitle_008   {"Phase B:","B相电压:"}
#define LTitle_009   {"Phase C:","C相电压:"}
#define LTitle_010   {"Phase A:","A相电流:"}
#define LTitle_011   {"Phase B:","B相电流:"}
#define LTitle_012   {"Phase C:","C相电流:"}
#define LTitle_013   {"Model:","监控型号:"}
#define LTitle_014   {"RataCap:","额定容量:"}
#define LTitle_015   {"RateVolt:","额定电压"}
#define LTitle_016   {"MaxVolt:","最高电压:"}      
#define LTitle_017   {"MaxTemp:","最高温度:"}
#define LTitle_018   {"Settings","参数设置"}
#define LTitle_019   {"Maintenance","控制输出"}
#define LTitle_020   {"Energy Saving","节能参数"}
#define LTitle_021   {"Fast Settings","快速控制"}
#define LTitle_022   {"Rectifiers","模块信息"}
#define LTitle_023   {"Active Alarm","当前告警"}
#define LTitle_024   {"History Alarm","历史告警"}
#define LTitle_025   {"GuaYeKeJi","光烨科技"}
#define LTitle_026   {"ID:","ID:"}
#define LTitle_027   {"Out Volt:","输出电压:"}
#define LTitle_028   {"Out Curr:","输出电流:"}
#define LTitle_029   {"Curr Limit:","限流点:"}
#define LTitle_030   {"In  Volt:","输入电压:"}
#define LTitle_031   {"AC State:","交流状态:"}
#define LTitle_032   {"DC State:","直流状态:"}
#define LTitle_033   {"AC Derated:","交流限功率:"}
#define LTitle_034   {"Temp Derated:","温度限功率:"}
#define LTitle_035   {"BattVolt:","电池电压:"}
#define LTitle_036   {"ChgVolt:","充电电压:"}
#define LTitle_037   {"ChgCurr:","充电电流:"}
#define LTitle_038   {"MaxCellVolt:","最高单体电压:"}
#define LTitle_039	 {"MinCellVolt:","最低单体电压:"}
#define LTitle_040   {"MaxCellTemp:","最高单体温度:"}	
#define LTitle_041   {"MinCellTemp:","最低单体温度:"}
#define LTitle_042	 {"Insul:","绝缘检测:"}
#define LTitle_043   {"System Info","系统信息"}
#define LTitle_044   {"Balance:","卡内金额:"}
#define LTitle_045   {"ChgGun:","充电枪:"}	        /*DCEM 日电量*/
#define LTitle_046   {"HallVolt1:","霍尔电压1:"}	/*DCEM 日电量*/
#define LTitle_047   {"Password:","输入密码:"}
#define LTitle_048   {"ESC Quit","ESC放弃"}
#define LTitle_049   {"ENT Continue","ENT进入"}
#define LTitle_050   {"Save Enable:","节能允许:"}
#define LTitle_051   {"HallVolt2:","霍尔电压2:"}
#define LTitle_052   {"No operation!","不能进行操作!"}
#define LTitle_053   {"M-Volt:","电表电压:"}
#define LTitle_054   {"M-Curr:","电表电流:"}
#define LTitle_055   {"Temp:","柜内温度:"}
#define LTitle_056   {"RectTrim:","模块调压:"}
#define LTitle_057   {"RectLimit:","模块限流:"}
#define LTitle_058   {"Module:","模块:"}
#define LTitle_059   {"Alarm Settings","告警参数"}
#define LTitle_060   {"Bat. Settings","电池参数"}
#define LTitle_061   {"AC Settings","交流参数"}
#define LTitle_062   {"DC Settings","直流参数"}
#define LTitle_063   {"Rect Settings","模块参数"}
#define LTitle_064   {"Fan1 Rate:","风机1转速:"}
#define LTitle_065   {"Sys Settings","系统参数"}
#define LTitle_066   {"Comm Settings","通讯参数"}
#define LTitle_067   {"Alarm Level","告警级别"}
#define LTitle_068   {"Alarm Control","告警控制"}
#define LTitle_069   {"DI Settings","开关量设置"}
#define LTitle_070   {"Alarm Type:","告警类型:"}
#define LTitle_071   {"Level:","级别:"}
#define LTitle_072   {"Out Relay:","关联继电器:"}
#define LTitle_073   {"Voice Sign:","告警音长:"}
#define LTitle_074   {"Clear:","清空:"}
#define LTitle_075   {"Block Alarm:","阻塞当前告警:"}
#define LTitle_076   {"DI No:","开关量序号:"}
#define LTitle_077   {"Active:","告警方式:"}
#define LTitle_078   {"Basic Settings","基本参数"}
#define LTitle_079   {"LVD Settings","下电保护"}
#define LTitle_080   {"Charge","充电管理"}
#define LTitle_081   {"Fan2 Rate:","风机2转速:"}
#define LTitle_082   {"Temp. Comp","温补参数"}
#define LTitle_083   {"ChgCtrl:","充电控制:"}
#define LTitle_084   {"Bat. Fuse:","电池熔丝组:"}
#define LTitle_085   {"Capacity:","电池容量:"}
#define LTitle_086   {"Bat. Shunt1:","电池分流器1:"}
#define LTitle_087   {"Bat. Shunt2:","电池分流器2:"}
#define LTitle_088   {"Shunt Coeff:","分流器系数:"}
#define LTitle_089   {"-Curr:","-电流:"}
#define LTitle_090   {"-Volt:","-电压:"}
#define LTitle_091   {"LVD1 Enable:","负载下电允许:"}
#define LTitle_092   {"LVD2 Enable:","电池保护允许:"}
#define LTitle_093   {"SOC:","剩余容量:"}
#define LTitle_094   {"LVD1 Volt:","负载下电:"}
#define LTitle_095   {"LVD2 Volt:","电池保护:"}
#define LTitle_096   {"LVD1Time:","负载下电:"}
#define LTitle_097   {"LVD2Time:","电池保护:"}
#define LTitle_098   {"Float:","浮充电压:"}
#define LTitle_099   {"Boost:","均充电压:"}
#define LTitle_100   {"Limit:","限流点:"}
#define LTitle_101   {"Over:","过流点:"}
#define LTitle_102   {"P-G:","正对地:"}
#define LTitle_103   {"N-G:","负对地:"}
#define LTitle_104   {"Volt:","电压:"}
#define LTitle_105   {"Curr:","电流:"}
#define LTitle_106   {"Contactor:","输出接触器:"}
#define LTitle_107   {"-Enable:","-允许:"}
#define LTitle_108   {"Insul Info","绝缘板信息"}
#define LTitle_109   {"Test Settings","测试参数"}
#define LTitle_110   {"ChgMode:","充电模式:"}
#define LTitle_111   {"NeedVolt:","允许电压:"}
#define LTitle_112   {"NeedCurr:","允许电流"}
#define LTitle_113   {"RectCtrl:","模块开关:"}
#define LTitle_114   {"Login:","登录:"}
#define LTitle_115   {"FanFull:","风机全速:"}
#define LTitle_116   {"Cyc Test En:","定时测试允许:"}
#define LTitle_117   {"Short Test:","快速测试:"}
#define LTitle_118   {"Unauthorized","密码错误"}
#define LTitle_119   {"-Alarm:","-告警点:"}
#define LTitle_120   {"OverVolt2:","过压告警:"}
#define LTitle_121   {"OverVolt1:","高压告警:"}
#define LTitle_122   {"Stable Test:","恒流测试:"}
#define LTitle_123   {"Confirm","确认"}
#define LTitle_124   {"Center Temp:","温补中心点:"}
#define LTitle_125   {"Coeff:","系数:"}
#define LTitle_126   {"Over Volt:","过压告警:"}
#define LTitle_127   {"Low Volt:","欠压告警:"}
#define LTitle_128   {"Under Volt:","缺相告警:"}
#define LTitle_129   {"AC In:","交流输入:"}
#define LTitle_130   {"AC PH:","交流相数:"}
#define LTitle_131   {"LowVolt1:","低压告警:"}
#define LTitle_132   {"LowVolt2:","欠压告警:"}
#define LTitle_133   {"3rd Shunt:","分流器3:"}
#define LTitle_134   {"Load Fuse:","负载熔丝:"}
#define LTitle_135   {"Position En:","槽位允许:"}
#define LTitle_136   {"HVSD:","过压保护:"}
#define LTitle_137   {"Default V:","默认电压:"}
#define LTitle_138   {"Walk-in On:","缓启动允许:"}
#define LTitle_139   {"Walk-in T:","缓启动时间:"}
#define LTitle_140   {"Interval T:","顺序启机:"}
#define LTitle_141   {"AC OverV On:","交流过压开机:"}
#define LTitle_142   {"ACCurrLim:","输入限流:"}
#define LTitle_143   {"Mains In:","市电状态:"}
#define LTitle_144   {"-Volt En:","-电压使能:"}
//#define LTitle_145   {"DG CAP:","油机容量:"}
#define LTitle_145   {"KWH:","KWH:"}		/*DCEM 日电量*/
#define LTitle_146   {"Temp1:","温度1:"}
#define LTitle_147   {"Temp2:","温度2:"}
#define LTitle_148   {"DG Status","油机信息"}
#define LTitle_149   {"DG Run:","油机运行状态:"}
#define LTitle_150   {"TempComp:","温补:"}
#define LTitle_151   {"Batt Record","电池记录"}
#define LTitle_152   {"Current Mon","本月记录"}
#define LTitle_153   {"Three Mon","三月记录"}
#define LTitle_154   {"Batt T H2:","电池过温:"}
#define LTitle_155   {"Batt T H1:","电池高温:"}
#define LTitle_156   {"Batt T L1:","电池低温:"}
#define LTitle_157   {"Amb High:","环境高温:"}
#define LTitle_158   {"Amb Low:","环境低温:"}
#define LTitle_159   {"Rect Curr","模块电流"}
#define LTitle_160   {"Batt Charge:","电池充电:"}
#define LTitle_161   {"Batt Discharge:","电池放电:"}
#define LTitle_162   {"Batt Disc Num:","电池放电次数："}
#define LTitle_163   {"Batt Disc Time:","电池放电时间:"}
#define LTitle_164   {"Avg Disc Depth:","平均放电深度:"}
#define LTitle_165   {"Total Rect Cur:","模块总电流:"}
#define LTitle_166   {"-BattCur En:","-电流使能:"}
//#define LTitle_167   {"Fan Group1:","风机组1:"}
#define LTitle_167   {"DCEM1Ctrl:","DCEM1控制:"}
//#define LTitle_168   {"Fan Group2:","风机组2:"}
#define LTitle_168   {"DCEM2Ctrl:","DCEM2控制:"}
#define LTitle_169   {"Stop DG:","关油机条件:"}
#define LTitle_170   {"-Delay:","-延时:"}
#define LTitle_171   {"DG Run Time:","油机运行时长:"}
#define LTitle_172   {"-Max:","-最长:"}
#define LTitle_173   {"Lang:","语言:"}
#define LTitle_174   {"Tzone:","时区:"}
#define LTitle_175   {"Date:","日期:"}
#define LTitle_176   {"Time:","时间:"}
#define LTitle_177   {"System Mode:","系统模式:"}
#define LTitle_178   {"System Type:","系统类型:"}
#define LTitle_179   {"Reset PWD:","密码重置:"}
#define LTitle_180   {"Reset Para:","系统重置:"}
#define LTitle_181   {"Op1 PWD:","用户密码:"}
#define LTitle_182   {"Op2 PWD:","工程密码:"}
#define LTitle_183   {"Adm PWD:","管理密码:"}
#define LTitle_184   {"Notice:","确认:"}
#define LTitle_185   {"Address:","本机地址:"}
#define LTitle_186   {"Comm Mode:","通讯方式:"}
#define LTitle_187   {"Protocol:","协议:"}
#define LTitle_188   {"BaudRate:","波特率:"}
#define LTitle_189   {"IP/Subnet/Gate:","IP/掩码/网关:"}
#define LTitle_190   {"CallbackTime:","回叫次数:"}
#define LTitle_191   {"Phone Number:","电话:"}
#define LTitle_192   {"CycPeriod:","开关周期:"}
#define LTitle_193   {"Rect Work:","模块工作点:"}
#define LTitle_194   {"Bat.T:","电池仓:"}
#define LTitle_195   {"Device.T:","设备仓:"}
#define LTitle_196   {"Bat2.T:","电池柜:"}
#define LTitle_197   {"Amb Temp:","环境温度:"}
#define LTitle_198   {"BattTemp:","电池温度:"}
#define LTitle_199   {"Amb2 Temp:","环境温度2:"}
#define LTitle_200   {"Batt2Temp:","电池温度2:"}
#define LTitle_201   {"SoftVer:","软件版本:"}
#define LTitle_202   {"RunTime:","运行时间:"}
#define LTitle_203   {"Alarm","告警"}
#define LTitle_204   {"HistAlarm","历史告警"}
#define LTitle_205   {"Nothing!","当前无告警!"}
#define LTitle_206   {"Nothing!","历史无告警!"}
#define LTitle_207   {"MAIN MENU","主菜单"}
#define LTitle_208   {"Old Ver:","原版本:"}
#define LTitle_209   {"New Ver:","新版本:"}
#define LTitle_210   {"Any Key Reset..","任意键重置..."}
//#define LTitle_211   {"Heater Num:","加热器个数:"}
#define LTitle_211   {"CT Coeff:","CT系数:"}
#define LTitle_212   {"Up/Down to Ctrl","手动下请下翻"}
#define LTitle_213   {"Rect DC:","模块直流:"}
#define LTitle_214   {"Rect AC:","模块交流:"}
#define LTitle_215   {"RectRS:","R-复位:"}

#define LTitle_216   {"Temp3:","温度3:"}
#define LTitle_217   {"Symmetry:","电池平衡:"}

#define LTitle_218   {"AC Input:","交流输入:"}
#define LTitle_219   {"DownLoad:","软件下载:"}

#define LTitle_220   {"Symm Alarm:","不平衡点:"}

#define LTitle_221   {"Volt:","电压:"}
#define LTitle_222   {"Curr:","限流点:"}
#define LTitle_223   {"Coeff:","系数:"}
#define LTitle_224   {"Time:","时间:"}

#define LTitle_225   {"PowerSplit","PowerSplit"}
#define LTitle_226   {"PowerSplit:","PowerSplit:"}

#define LTitle_227   {"Batt Symmetry","电池电压"}

#define LTitle_228   {"Symm V1:","电压1:"}
#define LTitle_229   {"Symm V2:","电压2:"}
#define LTitle_230   {"Symm V3:","电压3:"}
#define LTitle_231   {"Symm V4:","电压4:"}
#define LTitle_232   {"Symm V5:","电压5:"}
#define LTitle_233   {"Symm V6:","电压6:"}
#define LTitle_234   {"Symm V7:","电压7:"}

#define LTitle_235   {"DG Chg Limit:","油机充电限流:"}

#define LTitle_236   {"DCEM1:","DCEM1:"}
#define LTitle_237   {"-Sh1 Curr:","-分流器1:"}
#define LTitle_238   {"-Sh1 Volt:","-分流器1:"}
#define LTitle_239   {"-Sh2 Curr:","-分流器2:"}
#define LTitle_240   {"-Sh2 Volt:","-分流器2:"}
#define LTitle_241   {"-Sh3 Curr:","-分流器3:"}
#define LTitle_242   {"-Sh3 Volt:","-分流器3:"}
#define LTitle_243   {"-Sh4 Curr:","-分流器4:"}
#define LTitle_244   {"-Sh4 Volt:","-分流器4:"}
#define LTitle_245   {"DCEM2:","DCEM2:"}

#define LTitle_246   {"DCEM Settings","DCEM参数"}
#define LTitle_247   {"DCEM Status","DCEM信息"}
#define LTitle_248   {"-Volt:","-电压:"}
#define LTitle_249   {"CH1 I:","CH1 I:"}
#define LTitle_250   {"CH2 I:","CH2 I:"}
#define LTitle_251   {"CH3 I:","CH3 I:"}
#define LTitle_252   {"CH4 I:","CH4 I:"}
#define LTitle_253   {"KWH:","KWH:"}
#define LTitle_254   {"KW:","KW:"}
#define LTitle_255   {"Symm V8:","电压8:"}

#define LTitle_256	{"Rect FC:","整流模块浮充:"}
#define LTitle_257	{"Rect BC:","整流模块均充:"}
#define LTitle_258  {"SConTrim:","光模调压:"}
#define LTitle_259  {"SConLimit:","光模限流:"}
#define LTitle_260	{"SCon DC:","光模直流:"}
#define LTitle_261  {"SConRS:","SC复位:"}
#define LTitle_262  {"LoadShare En:","均流允许:"}

#define LTitle_263	{"Statistic","统计信息"}
#define LTitle_264	{"Load:","负载累计耗电:"}
#define LTitle_265	{"Solar Pwr:","太阳能累计发电:"}
#define LTitle_266	{"Mains Pwr:","市电累计发电:"}
#define LTitle_267	{"DG Pwr:","油机累计发电:"}
#define LTitle_268  {"Wind Pwr:","风机累计发电:"}
#define LTitle_269	{"In Curr:","输入电流:"}
#define LTitle_270  {"Out Pwr:","输出功率:"}
#define LTitle_271  {"Sys Work Mode:","监控工作模式:"}
#define LTitle_272   {"Solar Convt","光伏模块"}
#define LTitle_273  {"CtrlMode:","控制模式:"}
#define LTitle_274	{"MpptDelta:","MPPT压差:"}

#define LTitle_275   {"DG Ctrl Set","油机控制参数"}
#define LTitle_276	 {"Day:","天:"}			/*DCEM 日电量*/
#define LTitle_277   {"-Time:","-定时:"}
#define LTitle_278   {"-Time En:","-定时使能: "}
#define LTitle_279   {"DG In:","油机状态:"}

#define LTitle_280   {"DG RunTime Sum:","累计运行时间:"}
#define LTitle_281   {"Number of runs:","运行次数:"}

#define LTitle_282   {"Temp.:","温度:"}
#define LTitle_283   {"DC Converter","DCDC模块"}

/*debug for India*/
#define LTitle_284    {"CstNum:","CstNum:"}
#define LTitle_285    {"SiteNum1:","SiteNum1:"}
#define LTitle_286    {"SiteNum2:","SiteNum2:"}
#define LTitle_287    {"SiteNum3:","SiteNum3:"}
#define LTitle_288    {"CommFailClose:","CommFailClose:"}


#define LTitle_289    {"User Settings","用户参数"}
#define LTitle_290    {"User Name:","用户名称:"}
#define LTitle_291    {"Curr Shunt:","电流分流器:"}
#define LTitle_292    {"User Info","用户信息"}
#define LTitle_293    {"LVD Time:","下电时间:"}
#define LTitle_294    {"-Power:","-功率:"}
#define LTitle_295    {"-Energy:","-功耗:"}
#define LTitle_296    {"User1 Setting","用户1参数"}
#define LTitle_297    {"User2 Setting","用户2参数"}
#define LTitle_298    {"User3 Setting","用户3参数"}
#define LTitle_299    {"User4 Setting","用户4参数"}
#define LTitle_300    {"-EngRate:","-功耗比:"}


#define LTitle_ALL   { \
LTitle_000, LTitle_001, LTitle_002, LTitle_003, LTitle_004, LTitle_005, LTitle_006, LTitle_007,LTitle_008, LTitle_009,\
LTitle_010, LTitle_011, LTitle_012, LTitle_013, LTitle_014, LTitle_015, LTitle_016, LTitle_017,LTitle_018, LTitle_019,\
LTitle_020, LTitle_021, LTitle_022, LTitle_023, LTitle_024, LTitle_025, LTitle_026, LTitle_027,LTitle_028, LTitle_029,\
LTitle_030, LTitle_031, LTitle_032, LTitle_033, LTitle_034, LTitle_035, LTitle_036, LTitle_037,LTitle_038, LTitle_039,\
LTitle_040, LTitle_041, LTitle_042, LTitle_043, LTitle_044, LTitle_045, LTitle_046, LTitle_047,LTitle_048, LTitle_049,\
LTitle_050, LTitle_051, LTitle_052, LTitle_053, LTitle_054, LTitle_055, LTitle_056, LTitle_057,LTitle_058, LTitle_059,\
LTitle_060, LTitle_061, LTitle_062, LTitle_063, LTitle_064, LTitle_065, LTitle_066, LTitle_067,LTitle_068, LTitle_069,\
LTitle_070, LTitle_071, LTitle_072, LTitle_073, LTitle_074, LTitle_075, LTitle_076, LTitle_077,LTitle_078, LTitle_079,\
LTitle_080, LTitle_081, LTitle_082, LTitle_083, LTitle_084, LTitle_085, LTitle_086, LTitle_087,LTitle_088, LTitle_089,\
LTitle_090, LTitle_091, LTitle_092, LTitle_093, LTitle_094, LTitle_095, LTitle_096, LTitle_097,LTitle_098, LTitle_099,\
LTitle_100, LTitle_101, LTitle_102, LTitle_103, LTitle_104, LTitle_105, LTitle_106, LTitle_107,LTitle_108, LTitle_109,\
LTitle_110, LTitle_111, LTitle_112, LTitle_113, LTitle_114, LTitle_115, LTitle_116, LTitle_117,LTitle_118, LTitle_119,\
LTitle_120, LTitle_121, LTitle_122, LTitle_123, LTitle_124, LTitle_125, LTitle_126, LTitle_127,LTitle_128, LTitle_129,\
LTitle_130, LTitle_131, LTitle_132, LTitle_133, LTitle_134, LTitle_135, LTitle_136, LTitle_137,LTitle_138, LTitle_139,\
LTitle_140, LTitle_141, LTitle_142, LTitle_143, LTitle_144, LTitle_145, LTitle_146, LTitle_147,LTitle_148, LTitle_149,\
LTitle_150, LTitle_151, LTitle_152, LTitle_153, LTitle_154, LTitle_155, LTitle_156, LTitle_157,LTitle_158, LTitle_159,\
LTitle_160, LTitle_161, LTitle_162, LTitle_163, LTitle_164, LTitle_165, LTitle_166, LTitle_167,LTitle_168, LTitle_169,\
LTitle_170, LTitle_171, LTitle_172, LTitle_173, LTitle_174, LTitle_175, LTitle_176, LTitle_177,LTitle_178, LTitle_179,\
LTitle_180, LTitle_181, LTitle_182, LTitle_183, LTitle_184, LTitle_185, LTitle_186, LTitle_187,LTitle_188, LTitle_189,\
LTitle_190, LTitle_191, LTitle_192, LTitle_193, LTitle_194, LTitle_195, LTitle_196, LTitle_197,LTitle_198, LTitle_199,\
LTitle_200, LTitle_201, LTitle_202, LTitle_203, LTitle_204, LTitle_205, LTitle_206, LTitle_207,LTitle_208, LTitle_209,\
LTitle_210, LTitle_211, LTitle_212, LTitle_213, LTitle_214, LTitle_215, LTitle_216, LTitle_217,LTitle_218, LTitle_219,\
LTitle_220, LTitle_221, LTitle_222, LTitle_223, LTitle_224, LTitle_225, LTitle_226, LTitle_227,LTitle_228, LTitle_229,\
LTitle_230, LTitle_231, LTitle_232, LTitle_233, LTitle_234, LTitle_235, LTitle_236, LTitle_237,LTitle_238, LTitle_239,\
LTitle_240, LTitle_241, LTitle_242, LTitle_243, LTitle_244, LTitle_245, LTitle_246, LTitle_247,LTitle_248, LTitle_249,\
LTitle_250, LTitle_251, LTitle_252, LTitle_253, LTitle_254, LTitle_255, LTitle_256, LTitle_257, LTitle_258, LTitle_259,\
LTitle_260, LTitle_261, LTitle_262, LTitle_263, LTitle_264, LTitle_265, LTitle_266, LTitle_267, LTitle_268, LTitle_269,\
LTitle_270, LTitle_271, LTitle_272, LTitle_273, LTitle_274, LTitle_275, LTitle_276, LTitle_277, LTitle_278, LTitle_279,\
LTitle_280, LTitle_281, LTitle_282, LTitle_283, LTitle_284, LTitle_285, LTitle_286, LTitle_287, LTitle_288, LTitle_289,\
LTitle_290, LTitle_291, LTitle_292, LTitle_293, LTitle_294, LTitle_295, LTitle_296, LTitle_297, LTitle_298, LTitle_299,\
LTitle_300,\
}



/***********************************************************
菜单屏所使用的行 --  目前共40个
typedef struct tagMenu_Line
{
  UCHAR  LineNum;
  UCHAR  TitleNum; //Title[2][16];//15+1
  UCHAR  TitleDisplayStart;
  UCHAR  TitleDisplayMode;//标题显示模式
  
  UCHAR  PasswordScreen;
  UCHAR  PasswordLevel;
  UCHAR  ConfirmScreen;
  UCHAR  RelationorFloat;//用于浮点数设置的关联和浮点数显示浮动与否:0-无关联无浮动，1－关联上限，2－关联下限，3－关联上下限
  UCHAR  cFloatDisplayValue;//用于显示浮动  
  UCHAR  RelationUporFloatHigh;//用于浮点数设置的上限关联配置浮点数序号，浮动关联的配置数据序号高位
  UCHAR  RelationDnorFloatLow;//用于浮点数设置的下限关联配置浮点数序号，浮动关联的配置数据序号低位
  float fFloatDisplayValue;//用于显示浮动  
}Menu_Line_TypeDef;//数据长度15   
*************************************************************/
/*主菜单：运行信息*/
#define MENU_LINE_000 { \
  0,/*LineNum*/\
  17,/*Title   LTitle_017*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*主菜单：参数设置*/
#define MENU_LINE_001 \
{\
  1,/*LineNum*/\
  18,/*Title    LTitle_018*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*主菜单：BMS信息*/
#define MENU_LINE_002 \
{\
  2,/*LineNum*/\
  5,/*Title    LTitle_005*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*主菜单：绝缘板信息*/
#define MENU_LINE_003 \
{\
  3,/*LineNum*/\
  108,/*Title   LTitle_108*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*主菜单：快速设置*/
#define MENU_LINE_004 \
{\
	4,/*LineNum*/\
  21,/*Title    LTitle_021*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*运行信息：模块信息*/
#define MENU_LINE_005 \
{\
  5,/*LineNum*/\
  22,/*Title   LTitle_022*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*运行信息：当前告警*/
#define MENU_LINE_006 \
{\
	6,/*LineNum*/\
  23,/*Title   LTitle_023*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*运行信息：历史告警*/
#define MENU_LINE_007 \
{\
	7,/*LineNum*/\
  24,/*Title   LTitle_024*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*主菜单：系统信息*/
#define MENU_LINE_008 \
{\
  8,/*LineNum*/\
  43,/*Title    LTitle_043*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

/*参数设置：告警参数*/
#define MENU_LINE_009 \
{\
	9,/*LineNum*/\
  59,/*Title   LTitle_059*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置：电池参数*/
#define MENU_LINE_010 \
{\
	10,/*LineNum*/\
  60,/*Title   LTitle_060*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置：交流参数*/
#define MENU_LINE_011 \
{\
	11,/*LineNum*/\
  61,/*Title   LTitle_061*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置：直流参数*/
#define MENU_LINE_012 \
{\
	12,/*LineNum*/\
  62,/*Title  LTitle_062*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置：模块参数*/
#define MENU_LINE_013 \
{\
	13,/*LineNum*/\
  63,/*Title   LTitle_063*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*测试参数*/
#define MENU_LINE_014 \
{\
	14,/*LineNum*/\
  109,/*Title   LTitle_109*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_PASS_WORD_LOW,/*RelationorFloat*/\
  PASSWORD_LEVEL3,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置：系统参数*/
#define MENU_LINE_015 \
{\
	15,/*LineNum*/\
  65,/*Title  LTitle_065*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置：通讯参数*/
#define MENU_LINE_016 \
{\
	16,/*LineNum*/\
  66,/*Title  LTitle_066*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*告警参数：告警级别*/
#define MENU_LINE_017 \
{\
	17,/*LineNum*/\
  67,/*Title  LTitle_067*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*告警参数：告警设置*/
#define MENU_LINE_018 \
{\
	18,/*LineNum*/\
  68,/*Title  LTitle_068*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*告警参数：开关量设置*/
#define MENU_LINE_019 \
{\
	19,/*LineNum*/\
  69,/*Title LTitle_069*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*电池参数：基本参数*/
#define MENU_LINE_020 \
{\
	20,/*LineNum*/\
  78,/*Title LTitle_078*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*电池参数：下电保护 */
#define MENU_LINE_021 \
{\
	21,/*LineNum*/\
  79,/*Title  LTitle_079*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*电池参数：充电管理 */
#define MENU_LINE_022 \
{\
	22,/*LineNum*/\
  80,/*Title   LTitle_080*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*电池参数：电池测试 */
#define MENU_LINE_023 \
{\
	23,/*LineNum*/\
  81,/*Title   LTitle_081*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
 /*电池参数：温补参数*/
#define MENU_LINE_024 \
{\
	24,/*LineNum*/\
  82,/*Title  LTitle_082 */\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*环境温控参数：传感器设置*/
#define MENU_LINE_025 \
{\
	25,/*LineNum*/\
  0,/*Title  LTitle_0*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*环境温控参数：风机管理*/
#define MENU_LINE_026 \
{\
	26,/*LineNum*/\
  0, /*Title  LTitle_*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*环境温控参数：加热器管理*/
#define MENU_LINE_027 \
{\
	27,/*LineNum*/\
  145, /*Title   LTitle_145*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*主菜单-标题*/
#define MENU_LINE_028 \
{\
	28,/*LineNum*/\
  207, /*LTitle_207*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*运行信息菜单-标题*/
#define MENU_LINE_029 \
{\
	29,/*LineNum*/\
  17, /*LTitle_017*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*参数设置菜单-标题*/
#define MENU_LINE_030 \
{\
	30,/*LineNum*/\
  18, /*LTitle_018*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*告警参数菜单-标题*/
#define MENU_LINE_031 \
{\
	31,/*LineNum*/\
  59, /*LTitle_059*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*电池参数菜单-标题*/
#define MENU_LINE_032 \
{\
	32,/*LineNum*/\
  60, /*LTitle_060*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*环境温控参数菜单-标题*/
#define MENU_LINE_033 \
{\
	33,/*LineNum*/\
  0, /*LTitle_*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*确认提示屏:  Ent 继续   (标题)*/
#define MENU_LINE_034 \
{\
	34,/*LineNum*/\
  49,/*LTitle_049*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*确认提示屏:  取消退出   (标题)*/
#define MENU_LINE_035 \
{\
	35,/*LineNum*/\
  48,/*LTitle_048*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*确认提示屏:  确认(标题)*/
#define MENU_LINE_036 \
{\
	36,/*LineNum*/\
  123,/*LTitle_123*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/*无动作提示屏:  在输出控制时提示无相应动作(标题)*/
#define MENU_LINE_037 \
{\
	37,/*LineNum*/\
  52,/*LTitle_052*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

/* 槽位设置提示屏:  标题 */
#define MENU_LINE_038 \
{\
	38,/*LineNum*/\
  112, /*LTitle_112*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 油机参数 */
#define MENU_LINE_039 \
{\
	39,/*LineNum*/\
  38, /*LTitle_038*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 油机信息 */
#define MENU_LINE_040 \
{\
	40,/*LineNum*/\
  148, /*LTitle_148*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 电池记录 */
#define MENU_LINE_041 \
{\
	41,/*LineNum*/\
  151, /*LTitle_151*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 本月记录 */
#define MENU_LINE_042 \
{\
	42,/*LineNum*/\
  152, /*LTitle_148*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 三月记录 */
#define MENU_LINE_043 \
{\
	43,/*LineNum*/\
  153, /*LTitle_148*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 模块总电流*/
#define MENU_LINE_044 \
{\
	44,/*LineNum*/\
  159, /*LTitle_148*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}
/* 电池记录 */
#define MENU_LINE_045 \
{\
	45,/*LineNum*/\
  151, /*LTitle_151*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

/*电池参数：基本参数*/
#define MENU_LINE_046 \
{\
	46,/*LineNum*/\
  225,/*Title LTitle_225*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

/* 电池中点电压 */
#define MENU_LINE_047 \
{\
	47,/*LineNum*/\
  227, /*LTitle_227*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_DISABLE_BATT_MID_VOLT,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_BattSymmetryOption_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_BattSymmetryOption_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

#define MENU_LINE_048 \
{\
	48,/*LineNum*/\
  246,/*Title  LTitle_246*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

#define MENU_LINE_049 \
{\
	49,/*LineNum*/\
  247,/*Title  LTitle_247*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

#define MENU_LINE_050 \
{\
	50,/*LineNum*/\
  40,/*Title  LTitle_040*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
}

#define MENU_LINE_051 \
{\
	51,/*LineNum*/\
	272,/*Title  LTitle_272 */\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreenPASSWORDSCREEN*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
    FLOAT_EQ_NOMPPT,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
	SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}
/* 参数设置：MPPT模块参数*/
#define MENU_LINE_052 \
{\
	52,/*LineNum*/\
	272,/*Title   LTitle_272*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	FLOAT_EQ_NOMPPT,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}
/*运行信息：统计信息*/
#define MENU_LINE_053 \
{\
	53,/*LineNum*/\
	263,/*Title   LTitle_227*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}
/* 油机控制参数 */
#define MENU_LINE_054 \
{\
	54,/*LineNum*/\
        275, /*LTitle_275*/\
        2,/*TitleDisplayStart*/\
        DIRECT_NORMAL,/*TitleDisplayMode*/\
              0,/*PasswordScreen*/\
        0,/*PasswordLevel*/\
        0,/*ConfirmScreen*/\
        FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
        SYSMODE_SOLAR,/*cFloatDisplayValue*/\
        SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
        SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
        0,/*fFloatDisplayValue*/\
}

/* DCDC模块参数*/
#define MENU_LINE_055 \
{\
	55,/*LineNum*/\
	283,/*Title   LTitle_283*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}

/* 用户参数*/
#define MENU_LINE_056 \
{\
	56,/*LineNum*/\
	289,/*Title LTitle_289*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}
/* 用户参数(标题)*/
#define MENU_LINE_057 \
{\
	57,/*LineNum*/\
	289,/*Title LTitle_289*/\
	0,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}

/* 用户1*/
#define MENU_LINE_058 \
{\
	58,/*LineNum*/\
	296,/*Title LTitle_296*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}

/* 用户2*/
#define MENU_LINE_059 \
{\
	59,/*LineNum*/\
	297,/*Title LTitle_297*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}
/* 用户3*/
#define MENU_LINE_060 \
{\
	60,/*LineNum*/\
	298,/*Title LTitle_298*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}
/* 用户4*/
#define MENU_LINE_061 \
{\
	61,/*LineNum*/\
	299,/*Title LTitle_299*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}

/* 用户信息*/
#define MENU_LINE_062 \
{\
	63,/*LineNum*/\
	292,/*Title LTitle_299*/\
	2,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
}



#define MENU_LINE_ALL   { \
MENU_LINE_000, MENU_LINE_001, MENU_LINE_002, MENU_LINE_003, MENU_LINE_004, MENU_LINE_005, MENU_LINE_006, MENU_LINE_007, MENU_LINE_008, MENU_LINE_009,\
MENU_LINE_010, MENU_LINE_011, MENU_LINE_012, MENU_LINE_013, MENU_LINE_014, MENU_LINE_015, MENU_LINE_016, MENU_LINE_017, MENU_LINE_018, MENU_LINE_019,\
MENU_LINE_020, MENU_LINE_021, MENU_LINE_022, MENU_LINE_023, MENU_LINE_024, MENU_LINE_025, MENU_LINE_026, MENU_LINE_027,MENU_LINE_028, MENU_LINE_029,\
MENU_LINE_030, MENU_LINE_031, MENU_LINE_032, MENU_LINE_033, MENU_LINE_034, MENU_LINE_035, MENU_LINE_036, MENU_LINE_037,MENU_LINE_038, MENU_LINE_039,\
MENU_LINE_040, MENU_LINE_041, MENU_LINE_042, MENU_LINE_043, MENU_LINE_044, MENU_LINE_045, MENU_LINE_046, MENU_LINE_047,MENU_LINE_048, MENU_LINE_049,\
MENU_LINE_050, MENU_LINE_051, MENU_LINE_052, MENU_LINE_053, MENU_LINE_054, MENU_LINE_055, MENU_LINE_056, MENU_LINE_057,MENU_LINE_058, MENU_LINE_059,\
MENU_LINE_060, MENU_LINE_061, MENU_LINE_062,\
}


/*****************************************************************
查询状态屏所使用的行 --  目前共80个
typedef struct tagInfo_Line
{
  UCHAR LineNum;
  UCHAR  TitleNum; //Title[2][16];//15+1
  UCHAR  TitleDisplayStart;
  UCHAR  TitleDisplayMode;//标题显示模式
  
  UCHAR  PasswordScreen;
  UCHAR  PasswordLevel;
  UCHAR  ConfirmScreen;
  UCHAR  RelationorFloat;//用于浮点数设置的关联和浮点数显示浮动与否:0-无关联无浮动，1－关联上限，2－关联下限，3－关联上下限
  UCHAR  cFloatDisplayValue;//用于显示浮动
  UCHAR  RelationUporFloatHigh;//用于浮点数设置的上限关联配置浮点数序号，浮动关联的配置数据序号高位
  UCHAR  RelationDnorFloatLow;//用于浮点数设置的下限关联配置浮点数序号，浮动关联的配置数据序号低位
  float fFloatDisplayValue;//用于显示浮动
  
  UCHAR  SignalMode;//信号类型：0：无，1：采集开关量，2：采集模拟量，3：模块开关量，4：模块模拟量，
  UCHAR  SignalUnit[4];//信号单位 4+1
  UINT16 SignalIndex;
  UCHAR  SignalDisplayMode;//信号显示模式：0：不显示，1：直接显示，2：直接反白显示，3：间接显示，4：间接反白显示
  UCHAR  SignalDisplayAlias[10];
  UCHAR  SignalDisplayStart;
  UCHAR  SignalDisplayLen;
  UCHAR  SignalDisplayDecimal;   
}Info_Line_TypeDef;//数据长度35
********************************************************************/
/*系统运行屏：时间*/
#define INFO_LINE_000 \
{\
  0,/*LineNum*/\
  0,/*Title   LTitle_000*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  {0},/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：充电状态*/
#define INFO_LINE_001 \
{\
  1,/*LineNum*/\
  0,/*Title   LTitle_000*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CHARGE_STATUS,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  22,21,23,22,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  1,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*系统运行屏：系统电压*/
#define INFO_LINE_002 \
{\
  2,/*LineNum*/\
  0,/*Title*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MEASURE_ANALOG,/*SignalMode*/\
  "V",/*SignalUnit*/\
  DCV_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0},/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*系统运行屏：负载电流*/
#define INFO_LINE_003 \
{\
	3,/*LineNum*/\
  0,/*Title*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MEASURE_ANALOG,/*SignalMode*/\
  "A",/*SignalUnit*/\
  LOADCURR_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0},/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
 /*系统运行屏：剩余容量*/
#define INFO_LINE_004 \
{\
  4,/*LineNum*/\
  93,/*Title_093*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MEASURE_ANALOG,/*SignalMode*/\
  "%",/*SignalUnit*/\
  SIG_ID_BATT_SOC,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：系统状态*/
#define INFO_LINE_005 \
{\
  5,/*LineNum*/\
  0,/*Title   LTitle_000*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  ALARM_SYSTEM,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*ALARMTABLE_SYSTEM SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  8, 9, 0, 0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：需求电压*/
#define INFO_LINE_006 \
{\
	6,/*LineNum*/\
	1,/*LTitle_001  Title*/\
	0,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
	SIG_ID_REQUIRE_VOLT,/*SignalMode*/\
	"V",/*SignalUnit*/\
	0,/*SignalIndex*/\
	DIRECT_NORMAL,/*SignalDisplayMode*/\
	0, 0, 0, 0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
	9,/*SignalDisplayStart*/\
	5,/*SignalDisplayLen*/\
	1,/*SignalDisplayDecimal*/\
}
/*系统运行屏：需求电流*/
#define INFO_LINE_007 \
{\
	7,/*LineNum*/\
	2,/*LTitle_002  Title*/\
	0,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
	SIG_EV_REQUIRE_CURR,/*SignalMode*/\
	"A",/*SignalUnit*/\
	0,/*SignalIndex*/\
	DIRECT_NORMAL,/*SignalDisplayMode*/\
	 0, 0, 0, 0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
	9,/*SignalDisplayStart*/\
	5,/*SignalDisplayLen*/\
	1,/*SignalDisplayDecimal*/\
}
/*系统运行屏：累计充电时间*/
#define INFO_LINE_008 \
{\
	8,/*LineNum*/\
	3,/*LTitle_003  Title*/\
	0,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
	SIG_EV_CHARGE_TIME,/*SignalMode*/\
	"Min",/*SignalUnit*/\
	0,/*SignalIndex*/\
	DIRECT_NORMAL,/*SignalDisplayMode*/\
	0, 0, 0, 0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
	9,/*SignalDisplayStart*/\
	3,/*SignalDisplayLen*/\
	0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：充电电量*/
#define INFO_LINE_009 \
{\
	9,/*LineNum*/\
	4,/*LTitle_004  Title*/\
	0,/*TitleDisplayStart*/\
	DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
	0,/*PasswordLevel*/\
	0,/*ConfirmScreen*/\
	0,/*RelationorFloat*/\
	0,/*cFloatDisplayValue*/\
	0,/*RelationUporFloatHigh*/\
	0,/*RelationDnorFloatLow*/\
	0,/*fFloatDisplayValue*/\
	SIG_EV_CHARGE_ENERGY,/*SignalMode*/\
	"Kwh",/*SignalUnit*/\
	0,/*SignalIndex*/\
	DIRECT_NORMAL,/*SignalDisplayMode*/\
	0, 0, 0, 0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
	9,/*SignalDisplayStart*/\
	3,/*SignalDisplayLen*/\
	0,/*SignalDisplayDecimal*/\
}
/*BMS信息：电池类型*/
#define INFO_LINE_010 \
{\
  10,/*LineNum*/\
  6,/*LTitle_006*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "",/*SignalUnit*/\
  BMS_BATT_TYPE ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  41, 42, 43, 74,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：额定容量*/
#define INFO_LINE_011 \
{\
  11,/*LineNum*/\
  14,/*LTitle_014*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "Ah",/*SignalUnit*/\
  BMS_BATT_RATE_CAP,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0, 0, 0, 0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：额定电压*/
#define INFO_LINE_012 \
{\
  12,/*LineNum*/\
  15,/*LTitle_015*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMS_BATT_RATE_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*BMS信息：最高电压*/
#define INFO_LINE_013 \
{\
  13,/*LineNum*/\
  16,/*LTitle_016*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMS_BATT_MAX_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*系统信息：交流1AB/A电压*/
#define INFO_LINE_014 \
{\
  14,/*LineNum*/\
  7,/*LTitle_007*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  METER_PHASE_A_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息：交流1BC/B电压*/
#define INFO_LINE_015 \
{\
  15,/*LineNum*/\
  8,/*LTitle_008*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  METER_PHASE_B_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息：交流1CA/C*/
#define INFO_LINE_016 \
{\
	16,/*LineNum*/\
  9,/*LTitle_009*/\
   0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  METER_PHASE_C_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息:最高温度 */
#define INFO_LINE_017 \
{\
  17,/*LineNum*/\
  17,/*LTitle_017*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "C",/*SignalUnit*/\
  BMS_BATT_MAX_TEMP ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*BMS信息：电池电压*/
#define INFO_LINE_018 \
{\
  18,/*LineNum*/\
  35,/*LTitle_035*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMS_BATT_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*BMS信息:充电电压 */
#define INFO_LINE_019 \
{\
  19,/*LineNum*/\
  36,/*LTitle_036*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "C",/*SignalUnit*/\
  BMS_CHARGE_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*系统运行屏：A相电流 (标题)*/
#define INFO_LINE_020 \
{\
	20,/*LineNum*/\
  10,/*LTitle_010*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "A",/*SignalUnit*/\
  METER_PHASE_A_CURR,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：B相电流 */
#define INFO_LINE_021 \
{\
	21,/*LineNum*/\
  11,/*LTitle_011*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "A",/*SignalUnit*/\
  METER_PHASE_B_CURR,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：C相电流*/
#define INFO_LINE_022 \
{\
	22,/*LineNum*/\
  12,/*LTitle_012*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "A",/*SignalUnit*/\
  METER_PHASE_C_CURR,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：充电电流*/
#define INFO_LINE_023 \
{\
  23,/*LineNum*/\
  37,/*LTitle_037*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "A",/*SignalUnit*/\
  BMS_CHARGE_CURR,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*BMS信息：最高单体电压（标题） */
#define INFO_LINE_024 \
{\
  24,/*LineNum*/\
  38,/*LTitle_038*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：最高单体电压 */
#define INFO_LINE_025 \
{\
  25,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMS_MAX_CELL_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  2,/*SignalDisplayDecimal*/\
}
/*BMS信息：最低单体电压（标题） */
#define INFO_LINE_026 \
{\
  26,/*LineNum*/\
  39,/*LTitle_039*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：最低单体电压 */
#define INFO_LINE_027 \
{\
  27,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMS_MIN_CELL_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  2,/*SignalDisplayDecimal*/\
}
/*BMS信息：最高单体温度（标题） */
#define INFO_LINE_028 \
{\
  28,/*LineNum*/\
  40,/*LTitle_038*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：最高单体温度 */
#define INFO_LINE_029 \
{\
  29,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "C",/*SignalUnit*/\
  BMS_MAX_BATT_TEMP,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*系统信息：卡内金额*/
#define INFO_LINE_030 \
{\
  30,/*LineNum*/\
  44,/*LTitle_044*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SYSTEM_INFO,/*SignalMode*/\
  "Y",/*SignalUnit*/\
  SYS_BALANCE_IN_CARD,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：模块 n? */
#define INFO_LINE_031 \
{\
  31,/*LineNum*/\
  58,/*LTitle_058*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_NUM,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：模块ID */
#define INFO_LINE_032 \
{\
	32,/*LineNum*/\
  26,/*LTitle_026*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_BARCODE,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：输出电压 */
#define INFO_LINE_033 \
{\
	33,/*LineNum*/\
  27,/*LTitle_027*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_VOLTAGE,/*SignalMode*/\
  "V",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*整流模块信息：输出电流 */
#define INFO_LINE_034 \
{\
	34,/*LineNum*/\
  28,/*LTitle_028*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_CURRENT,/*SignalMode*/\
  "A",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*整流模块信息：限流点 */
#define INFO_LINE_035 \
{\
	35,/*LineNum*/\
  29,/*LTitle_029*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_CURR_LIMIT,/*SignalMode*/\
  "%",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：输入电压 */
#define INFO_LINE_036 \
{\
	36,/*LineNum*/\
  30,/*LTitle_030*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_AC_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：交流状态 */
#define INFO_LINE_037 \
{\
	37,/*LineNum*/\
  31,/*LTitle_031*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_AC_ONOFF,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15, 16, 0,  0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：直流状态 */
#define INFO_LINE_038 \
{\
	38,/*LineNum*/\
  32,/*LTitle_032*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_DC_ONOFF,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15, 16, 0,  0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*整流模块信息：交流限功率 */
#define INFO_LINE_039 \
{\
	39,/*LineNum*/\
  33,/*LTitle_033*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_AC_DERATE,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1, 59, 0,  0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
} 
/*整流模块信息：温度限功率 */
#define INFO_LINE_040 \
{\
	40,/*LineNum*/\
  34,/*LTitle_034*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_TEMP_DERATE,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1, 59, 0,  0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*当前告警：x/y */
#define INFO_LINE_041 \
{\
	41,/*LineNum*/\
  203,/*LTitle_203*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED10,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*当前告警：告警名称 */
#define INFO_LINE_042 \
{\
	42,/*LineNum*/\
  0,/*LTitle_203*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED11,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  140,141,142,144,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*当前告警：告警级别*/
#define INFO_LINE_043 \
{\
	43,/*LineNum*/\
  0,/*LTitle_203*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED12,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7, 12, 13, 14, 0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*当前告警：开始时间*/
#define INFO_LINE_044 \
{\
	44,/*LineNum*/\
  0,/*LTitle_203*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED13,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*历史告警：x/y */
#define INFO_LINE_045 \
{\
	45,/*LineNum*/\
  0,/*LTitle_204*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED14,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*历史告警：名称 */
#define INFO_LINE_046 \
{\
	46,/*LineNum*/\
  0,/*LTitle_204*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED15,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  140,141,142,144,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*历史告警：开始时间*/
#define INFO_LINE_047 \
{\
	47,/*LineNum*/\
  0,/*LTitle_204*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MACRO_UNUSED16,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*历史告警：结束时间*/
#define INFO_LINE_048 \
{\
	48,/*LineNum*/\
  0,/*LTitle_204*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SIG_EV_CHARGE_ENERGY,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*当前无告警--  或无历史告警--或无模块信息 */
#define INFO_LINE_049 \
{\
	49,/*LineNum*/\
  0,/*LTitle_205*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  90,91,92,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  2,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*无历史告警*/
#define INFO_LINE_050 \
{\
	50,/*LineNum*/\
  206,/*LTitle_206*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：最低单体温度（标题） */
#define INFO_LINE_051 \
{\
  51,/*LineNum*/\
  41,/*LTitle_038*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*BMS信息：最低单体温度 */
#define INFO_LINE_052 \
{\
  52,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BMS_INFO,/*SignalMode*/\
  "C",/*SignalUnit*/\
  BMS_MIN_BATT_TEMP,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*系统信息：充电枪*/
#define INFO_LINE_053 \
{\
  53,/*LineNum*/\
  45,/*LTitle_045*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SYSTEM_INFO,/*SignalMode*/\
  "",/*SignalUnit*/\
  SYS_CHARGE_GUN_STATE,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  76,75,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息:霍尔电压1*/
#define INFO_LINE_054 \
{\
  54,/*LineNum*/\
  46,/*LTitle_046*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SYSTEM_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  SYS_HALL_VOLT_1,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息:霍尔电压2*/
#define INFO_LINE_055 \
{\
  55,/*LineNum*/\
  51,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SYSTEM_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  SYS_HALL_VOLT_2,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*系统信息：柜内温度*/
#define INFO_LINE_056 \
{\
  56,/*LineNum*/\
  55,/*LTitle_055*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SYSTEM_INFO,/*SignalMode*/\
  "C",/*SignalUnit*/\
  SYS_CABINET_TEMP,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息:风机1转速*/
#define INFO_LINE_057 \
{\
  57,/*LineNum*/\
  64,/*LTitle_064*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
   SYSTEM_INFO,/*SignalMode*/\
  "%",/*SignalUnit*/\
  SYS_FAN1_RATE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息:风机2转速*/
#define INFO_LINE_058 \
{\
  58,/*LineNum*/\
  81,/*LTitle_081*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
   SYSTEM_INFO,/*SignalMode*/\
  "%",/*SignalUnit*/\
  SYS_FAN2_RATE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息：电表电压*/
#define INFO_LINE_059 \
{\
  59,/*LineNum*/\
  53,/*LTitle_053*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  METER_PHASE_A_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息：电表电流*/
#define INFO_LINE_060 \
{\
  60,/*LineNum*/\
  54,/*LTitle_054*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_METER_TYPE,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  METER_INFO,/*SignalMode*/\
  "A",/*SignalUnit*/\
  METER_PHASE_A_CURR,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统信息:输出继电器状态*/
#define INFO_LINE_061 \
{\
  61,/*LineNum*/\
  106,/*LTitle_106*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SYSTEM_INFO,/*SignalMode*/\
  "",/*SignalUnit*/\
  SYS_OUTPUT_SWITCH_STATE,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  48,49,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*绝缘板信息:电压*/
#define INFO_LINE_062 \
{\
  62,/*LineNum*/\
  104,/*LTitle_104*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  INSUL_INFO,/*SignalMode*/\
  "V",/*SignalUnit*/\
  INSUL_VOLT,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*绝缘板信息:电流*/
#define INFO_LINE_063 \
{\
  63,/*LineNum*/\
  105,/*LTitle_105*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  INSUL_INFO,/*SignalMode*/\
  "A",/*SignalUnit*/\
  INSUL_CURR,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*绝缘板信息:正对地电阻*/
#define INFO_LINE_064 \
{\
  64,/*LineNum*/\
  102,/*LTitle_102*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  INSUL_INFO,/*SignalMode*/\
  "K",/*SignalUnit*/\
  INSUL_P_G_RES ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*绝缘板信息:负对地电阻*/
#define INFO_LINE_065 \
{\
  65,/*LineNum*/\
  103,/*LTitle_103*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  INSUL_INFO,/*SignalMode*/\
  "K",/*SignalUnit*/\
  INSUL_N_G_RES ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*电池记录－模块电流*/
#define INFO_LINE_066 \
{\
	66,/*LineNum*/\
  165,/*LTitle_165*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
 /*电池记录－模块电流值*/
#define INFO_LINE_067 \
{\
	67,/*LineNum*/\
  0,/*LTitle_041*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RECT_ANALOG,/*SignalMode*/\
  "A",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*电池记录－三月充电功率值*/
#define INFO_LINE_068 \
{\
	68,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BAT_CHG_CNT,/*SignalMode*/\
  "KWHr",/*SignalUnit*/\
  BattECCount_NUM6,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*电池记录－三月放电功率*/
#define INFO_LINE_069 \
{\
	69,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BAT_CHG_CNT,/*SignalMode*/\
  "KWHr",/*SignalUnit*/\
  BattECCount_NUM7,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*电池记录－三月放电次数*/
#define INFO_LINE_070 \
{\
	70,/*LineNum*/\
  0,/*LTitle_044*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BAT_CHG_CNT,/*SignalMode*/\
  "",/*SignalUnit*/\
  BattECCount_NUM8,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*电池记录－三月放电时间*/
#define INFO_LINE_071 \
{\
	71,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BAT_CHG_CNT,/*SignalMode*/\
  "min",/*SignalUnit*/\
  BattECCount_NUM9,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*电池记录－三月平均放电深度*/
#define INFO_LINE_072 \
{\
	72,/*LineNum*/\
  0,/*LTitle_046*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
 	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BAT_CHG_CNT,/*SignalMode*/\
  "%",/*SignalUnit*/\
  BattECCount_NUM10,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*产品信息屏：监控ID */
#define INFO_LINE_073 \
{\
	73,/*LineNum*/\
  13,/*LTitle_026*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_ModelName,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_ModelName_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*产品信息屏：监控 软件版本*/
#define INFO_LINE_074 \
{\
	74,/*LineNum*/\
  201,/*LTitle_201*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_SoftwareVersion_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  2,/*SignalDisplayDecimal*/\
}
 /*产品信息屏：监控 运行时间*/
#define INFO_LINE_075 \
{\
	75,/*LineNum*/\
  202,/*LTitle_202*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
  "h",/*SignalUnit*/\
  SETDATA_OperationTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/* 系统时间 */
#define INFO_LINE_076 \
{\
	76,/*LineNum*/\
  0,/*Title   LTitle_000*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  RTC_TIME,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0},/*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  10,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/* 预留 */
/*产品信息屏：监控ID -第二行*/
#define INFO_LINE_077 \
{\
	77,/*LineNum*/\
  0,/*LTitle_026*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_BarCode1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  16,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*系统运行屏：电池3 电流*/
#define INFO_LINE_078 \
{\
	12,/*LineNum*/\
  15,/*LTitle_015*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mBATTCURR,/*cFloatDisplayValue*/\
  SETDATA_CurrCfg_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CurrCfg_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MEASURE_ANALOG,/*SignalMode*/\
  "A",/*SignalUnit*/\
  BATT3CURR_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*系统运行屏：电池3 剩余容量*/
#define INFO_LINE_079 \
{\
	13,/*LineNum*/\
  5,/*LTitle_005*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mBATTCURR,/*cFloatDisplayValue*/\
  SETDATA_CurrCfg_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CurrCfg_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
  "%",/*SignalUnit*/\
  (SETDATA_BattCap_NUM),/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*系统运行屏：交流2CA/C*/
#define INFO_LINE_080 \
{\
	80,/*LineNum*/\
  9,/*LTitle_009*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
}
/* 交流1路 -标题*/
#define INFO_LINE_081 \
{\
	81,/*LineNum*/\
  216,/*LTitle_216*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/* 交流2路-标题*/
#define INFO_LINE_082 \
{\
	82,/*LineNum*/\
  217,/*LTitle_217*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*FLOAT_NEQ_SETDATA_CHAR,*//*RelationorFloat*/\
  mAUTO,/*cFloatDisplayValue*/\
  SETDATA_ACinCtl_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_ACinCtl_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/* 交流1路单相电压*/
#define INFO_LINE_083 \
{\
	83,/*LineNum*/\
  218,/*LTitle_218*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  m1PHASE,/*cFloatDisplayValue*/\
  SETDATA_ACinType_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_ACinType_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MEASURE_ANALOG,/*SignalMode*/\
  "V",/*SignalUnit*/\
  AC1VA_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/* 交流1路单相电流*/
#define INFO_LINE_084 \
{\
	84,/*LineNum*/\
  218,/*LTitle_218*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR_OR_NOAC,/*RelationorFloat*/\
  m1PHASE,/*cFloatDisplayValue*/\
  SETDATA_ACinType_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_ACinType_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MEASURE_ANALOG,/*SignalMode*/\
  "A",/*SignalUnit*/\
  AC1IA_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/* 升级提示屏:  原软件版本*/
#define INFO_LINE_085 \
{\
	85,/*LineNum*/\
  208,/*LTitle_208*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SoftwareVersion_NUM ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
}
/* 升级提示屏:  新软件版本*/
#define INFO_LINE_086 \
{\
	86,/*LineNum*/\
  209,/*LTitle_209*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  OLD_SW_VER,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
}
/*升级提示屏: 按任意键继续*/
#define INFO_LINE_087 \
{\
	87,/*LineNum*/\
  210,/*LTitle_210*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}


/*MPPT模块信息：模块 n? */
#define INFO_LINE_088 \
{\
	0,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/* 预留 */
#define INFO_LINE_089 \
{\
	0,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_090 \
{\
	90,/*LineNum*/\
  228,/*LTitle_228*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV2_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_091 \
{\
	91,/*LineNum*/\
  229,/*LTitle_229*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV3_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_092 \
{\
	92,/*LineNum*/\
  230,/*LTitle_230*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV4_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_093 \
{\
	93,/*LineNum*/\
  231,/*LTitle_231*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV5_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_094 \
{\
	94,/*LineNum*/\
  232,/*LTitle_232*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV6_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_095 \
{\
	95,/*LineNum*/\
  233,/*LTitle_233*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV7_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_096 \
{\
	96,/*LineNum*/\
  234,/*LTitle_234*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV8_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_097 \
{\
	97,/*LineNum*/\
  45,/*LTitle_045*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCEM_NUM_D,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_098 \
{\
	98,/*LineNum*/\
  248,/*LTitle_248*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "V",/*SignalUnit*/\
  DCEM_Voltage_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_099 \
{\
	99,/*LineNum*/\
  249,/*LTitle_249*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEM_Current1_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_100 \
{\
	100,/*LineNum*/\
  250,/*LTitle_250*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEM_Current2_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_101 \
{\
	101,/*LineNum*/\
  251,/*LTitle_251*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEM_Current3_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_102 \
{\
	102,/*LineNum*/\
  252,/*LTitle_252*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEM_Current4_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_103 \
{\
	103,/*LineNum*/\
  253,/*LTitle_253*/\
  1,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWH1_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_104 \
{\
	104,/*LineNum*/\
  253,/*LTitle_253*/\
  1,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWH2_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_105 \
{\
	105,/*LineNum*/\
  253,/*LTitle_253*/\
  1,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWH3_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_106 \
{\
	106,/*LineNum*/\
  253,/*LTitle_253*/\
  1,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWH4_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

///////////////////
#define INFO_LINE_107 \
{\
	107,/*LineNum*/\
  254,/*LTitle_254*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWatt1_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_108 \
{\
	108,/*LineNum*/\
  254,/*LTitle_248*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWatt2_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_109 \
{\
	109,/*LineNum*/\
  254,/*LTitle_249*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWatt3_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_110 \
{\
	110,/*LineNum*/\
  254,/*LTitle_250*/\
  2,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWatt4_VALUE,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_111 \
{\
	111,/*LineNum*/\
  251,/*LTitle_251*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_DCEM2_COMM,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEM_Current3_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_112 \
{\
	112,/*LineNum*/\
  252,/*LTitle_252*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_DCEM2_COMM,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEM_Current4_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_113 \
{\
	113,/*LineNum*/\
  253,/*LTitle_253*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_DCEM2_COMM,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "Kwh",/*SignalUnit*/\
  DCEM_KWH1_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_114 \
{\
	114,/*LineNum*/\
  254,/*LTitle_254*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_DCEM2_COMM,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "Kwh",/*SignalUnit*/\
  DCEM_KWH2_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_115 \
{\
	115,/*LineNum*/\
  255,/*LTitle_255*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  BATT_SYMMETRY_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  BMV9_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_116 \
{\
	116,/*LineNum*/\
  16,/*LTitle_016*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_DCEM2_COMM,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "Kwh",/*SignalUnit*/\
  DCEM_KWH4_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_117 \
{\
	117,/*LineNum*/\
  40,/*LTitle_016*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_118 \
{\
	118,/*LineNum*/\
  45,/*LTitle_016*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "Kwh",/*SignalUnit*/\
  DCEM_KWH4_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

#define INFO_LINE_119 \
{\
	119,/*LineNum*/\
  46,/*LTitle_016*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  DCEM_KWH4_VALUE + 20,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*MPPT模块信息：模块 n? */
#define INFO_LINE_120 \
{\
	88,/*LineNum*/\
  58,/*LTitle_058*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_NUM,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：模块ID */
#define INFO_LINE_121 \
{\
	89,/*LineNum*/\
  26,/*LTitle_026*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_BARCODE,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：输出电压 */
#define INFO_LINE_122 \
{\
	90,/*LineNum*/\
  27,/*LTitle_027*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_VOLTAGE,/*SignalMode*/\
  "V",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：输出电流 */
#define INFO_LINE_123 \
{\
	91,/*LineNum*/\
  28,/*LTitle_028*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_CURRENT,/*SignalMode*/\
  "A",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：限流点 */
#define INFO_LINE_124 \
{\
	92,/*LineNum*/\
  29,/*LTitle_029*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_CURR_LIMIT,/*SignalMode*/\
  "%",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：输入电压 */
#define INFO_LINE_125 \
{\
	93,/*LineNum*/\
  30,/*LTitle_030*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_INPUT_VOLT,/*SignalMode*/\
  "V",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：输入电流 */
#define INFO_LINE_126 \
{\
	94,/*LineNum*/\
  269,/*LTitle_269*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_INPUT_CURR,/*SignalMode*/\
  "A",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：输出功率 */
#define INFO_LINE_127 \
{\
	95,/*LineNum*/\
  270,/*LTitle_270*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_OUTPUT_PWR,/*SignalMode*/\
  "KW",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  2,/*SignalDisplayDecimal*/\
}
/*MPPT模块信息：直流输出状态 */
#define INFO_LINE_128 \
{\
	96,/*LineNum*/\
  32,/*LTitle_032*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  MPPT_DC_ONOFF,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15, 16, 0,  0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 负载累计耗电量(标题) */
#define INFO_LINE_129 \
{\
	129,/*LineNum*/\
  264,/*LTitle_228*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 负载累计耗电量 */
#define INFO_LINE_130 \
{\
	100,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  STATISTIC_SUM,/*SignalMode*/\
  "KWH",/*SignalUnit*/\
  UNUSED_XXX_ID,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 太阳能累计发电量(标题) */
#define INFO_LINE_131 \
{\
	131,/*LineNum*/\
  265,/*LTitle_265*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 太阳能累计发电量 */
#define INFO_LINE_132 \
{\
	102,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  STATISTIC_SUM,/*SignalMode*/\
  "KWH",/*SignalUnit*/\
  UNUSED_XXX_ID,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 市电累计发电量(标题) */
#define INFO_LINE_133 \
{\
	133,/*LineNum*/\
  266,/*LTitle_230*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 市电累计发电量 */
#define INFO_LINE_134 \
{\
	134,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  STATISTIC_SUM,/*SignalMode*/\
  "KWH",/*SignalUnit*/\
  UNUSED_XXX_ID,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 油机累计发电量(标题) */
#define INFO_LINE_135 \
{\
	135,/*LineNum*/\
  267,/*LTitle_231*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 油机累计发电量 */
#define INFO_LINE_136 \
{\
	136,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  STATISTIC_SUM,/*SignalMode*/\
  "KWH",/*SignalUnit*/\
  UNUSED_XXX_ID,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 风力机累计发电量(标题) */
#define INFO_LINE_137 \
{\
	137,/*LineNum*/\
  268,/*LTitle_232*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 统计信息 - 风力机累计发电量 */
#define INFO_LINE_138 \
{\
	138,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  STATISTIC_SUM,/*SignalMode*/\
  "KWH",/*SignalUnit*/\
  UNUSED_XXX_ID,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

/* 油机信息 - 本次油机运行时间(标题) */
#define INFO_LINE_139 \
{\
	139,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*油机信息 - 本次油机运行时间*/
#define INFO_LINE_140 \
{\
	140,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DG_NOWRUNTIME,/*SignalMode*/\
  "H",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/* 油机信息 - 累计油机运行时间(标题) */
#define INFO_LINE_141 \
{\
	141,/*LineNum*/\
  280,/*LTitle_280*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*油机信息 - 累计油机运行时间*/
#define INFO_LINE_142 \
{\
	142,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DG_TOTALRUNTIME,/*SignalMode*/\
  "H",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*油机信息 - 累计油机运行次数(标题)*/
#define INFO_LINE_143 \
{\
	143,/*LineNum*/\
  281,/*LTitle_281*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*油机信息 - 累计油机运行次数*/
#define INFO_LINE_144 \
{\
	144,/*LineNum*/\
  0,/*LTitle_247*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DG_STARTTIMER,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}



/*DCDC模块信息：模块 n? */
#define INFO_LINE_145 \
{\
	145,/*LineNum*/\
  58,/*LTitle_058*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_NUM,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*DCDC模块信息：模块ID */
#define INFO_LINE_146 \
{\
	146,/*LineNum*/\
  26,/*LTitle_026*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_BARCODE,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*DCDC模块信息：输出电压 */
#define INFO_LINE_147 \
{\
	147,/*LineNum*/\
  27,/*LTitle_027*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_VOLTAGE,/*SignalMode*/\
  "V",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

/*DCDC模块信息：输出电流 */
#define INFO_LINE_148 \
{\
	148,/*LineNum*/\
  28,/*LTitle_028*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_CURR,/*SignalMode*/\
  "A",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*DCDC模块信息：温度 */
#define INFO_LINE_149 \
{\
	149,/*LineNum*/\
  282,/*LTitle_282*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_TEMP,/*SignalMode*/\
  "C",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*DCDC模块信息：直流输出状态 */
#define INFO_LINE_150 \
{\
	150,/*LineNum*/\
  32,/*LTitle_032*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_DC_ONOFF,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15, 16, 0,  0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*首屏：DCDC平均输出电压 */
#define INFO_LINE_151 \
{\
	151,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_OUTVOLTAVE,/*SignalMode*/\
  "V",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}
/*首屏：DCDC负载率 */
#define INFO_LINE_152 \
{\
	152,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_PWRRATE,/*SignalMode*/\
  "%",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
/*首屏：DCDC输出电流 */
#define INFO_LINE_153 \
{\
	153,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
	0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DCDC_OUTCURRENT,/*SignalMode*/\
  "A",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0, /*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*用户信息-用户名称及路号 */
#define INFO_LINE_154 \
{\
	154,/*LineNum*/\
  0,/*LTitle_000*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  USER_NAME,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_UserName_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  {140,141,142,144,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*用户信息-电流 */
#define INFO_LINE_155 \
{\
  155,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  USER_CURR,/*SignalMode*/\
  "A",/*SignalUnit*/\
  DCEMCURR1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

/*用户信息-功率 */
#define INFO_LINE_156 \
{\
  156,/*LineNum*/\
  294,/*LTitle_294*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  USER_POWER,/*SignalMode*/\
  "KW",/*SignalUnit*/\
  DCEMCURR1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

/*用户信息-功耗 */
#define INFO_LINE_157 \
{\
  157,/*LineNum*/\
  295,/*LTitle_295*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}

/*用户信息-功耗 */
#define INFO_LINE_158 \
{\
  158,/*LineNum*/\
  0,/*LTitle_295*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  USER_ENERGY,/*SignalMode*/\
  "KWH",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
}

/*用户信息-功耗比 */
#define INFO_LINE_159 \
{\
  159,/*LineNum*/\
  300,/*LTitle_300*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  USER_ENERGYRATE,/*SignalMode*/\
  "%",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0,0,0,0,0,0,0,0,0,0}, /*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
}
#define INFO_LINE_ALL   { \
INFO_LINE_000, INFO_LINE_001, INFO_LINE_002, INFO_LINE_003, INFO_LINE_004, INFO_LINE_005, INFO_LINE_006, INFO_LINE_007, INFO_LINE_008, INFO_LINE_009,\
INFO_LINE_010, INFO_LINE_011, INFO_LINE_012, INFO_LINE_013, INFO_LINE_014, INFO_LINE_015, INFO_LINE_016, INFO_LINE_017, INFO_LINE_018, INFO_LINE_019,\
INFO_LINE_020, INFO_LINE_021, INFO_LINE_022, INFO_LINE_023, INFO_LINE_024, INFO_LINE_025, INFO_LINE_026, INFO_LINE_027, INFO_LINE_028, INFO_LINE_029,\
INFO_LINE_030, INFO_LINE_031, INFO_LINE_032, INFO_LINE_033, INFO_LINE_034, INFO_LINE_035, INFO_LINE_036, INFO_LINE_037, INFO_LINE_038, INFO_LINE_039,\
INFO_LINE_040, INFO_LINE_041, INFO_LINE_042, INFO_LINE_043, INFO_LINE_044, INFO_LINE_045, INFO_LINE_046, INFO_LINE_047, INFO_LINE_048, INFO_LINE_049,\
INFO_LINE_050, INFO_LINE_051, INFO_LINE_052, INFO_LINE_053, INFO_LINE_054, INFO_LINE_055, INFO_LINE_056, INFO_LINE_057, INFO_LINE_058, INFO_LINE_059,\
INFO_LINE_060, INFO_LINE_061, INFO_LINE_062, INFO_LINE_063, INFO_LINE_064, INFO_LINE_065, INFO_LINE_066, INFO_LINE_067, INFO_LINE_068, INFO_LINE_069,\
INFO_LINE_070, INFO_LINE_071, INFO_LINE_072, INFO_LINE_073, INFO_LINE_074, INFO_LINE_075, INFO_LINE_076, INFO_LINE_077, INFO_LINE_078, INFO_LINE_079,\
INFO_LINE_080, INFO_LINE_081, INFO_LINE_082, INFO_LINE_083, INFO_LINE_084, INFO_LINE_085, INFO_LINE_086, INFO_LINE_087, INFO_LINE_088, INFO_LINE_089,\
INFO_LINE_090, INFO_LINE_091, INFO_LINE_092, INFO_LINE_093, INFO_LINE_094, INFO_LINE_095, INFO_LINE_096, INFO_LINE_097, INFO_LINE_098, INFO_LINE_099,\
INFO_LINE_100, INFO_LINE_101, INFO_LINE_102, INFO_LINE_103, INFO_LINE_104, INFO_LINE_105, INFO_LINE_106, INFO_LINE_107, INFO_LINE_108, INFO_LINE_109,\
INFO_LINE_110, INFO_LINE_111, INFO_LINE_112, INFO_LINE_113, INFO_LINE_114, INFO_LINE_115, INFO_LINE_116, INFO_LINE_117, INFO_LINE_118, INFO_LINE_119,\
INFO_LINE_120, INFO_LINE_121, INFO_LINE_122, INFO_LINE_123, INFO_LINE_124, INFO_LINE_125, INFO_LINE_126, INFO_LINE_127, INFO_LINE_128, INFO_LINE_129,\
INFO_LINE_130, INFO_LINE_131, INFO_LINE_132, INFO_LINE_133, INFO_LINE_134, INFO_LINE_135, INFO_LINE_136, INFO_LINE_137, INFO_LINE_138, INFO_LINE_139,\
INFO_LINE_140, INFO_LINE_141, INFO_LINE_142, INFO_LINE_143, INFO_LINE_144, INFO_LINE_145, INFO_LINE_146, INFO_LINE_147, INFO_LINE_148, INFO_LINE_149,\
INFO_LINE_150, INFO_LINE_151, INFO_LINE_152, INFO_LINE_153, INFO_LINE_154, INFO_LINE_155, INFO_LINE_156, INFO_LINE_157, INFO_LINE_158, INFO_LINE_159,\
}


/*****************************************************************
参数设置屏所使用的行 --  目前共200个
设置参数屏：有标题，有对应信号，并且可以设置
typedef struct tagSet_Line
{
  UCHAR LineNum;
  UCHAR  TitleNum; //Title[2][16];//15+1
  UCHAR  TitleDisplayStart;
  UCHAR  TitleDisplayMode;//标题显示模式
  
   UCHAR  PasswordScreen;
  UCHAR  PasswordLevel;
  UCHAR  ConfirmScreen;
  UCHAR  RelationorFloat;//用于浮点数设置的关联和浮点数显示浮动与否:0-无关联无浮动，1－关联上限，2－关联下限，3－关联上下限
  UCHAR  cFloatDisplayValue;//用于显示浮动
  UCHAR  RelationUporFloatHigh;//用于浮点数设置的上限关联配置浮点数序号，浮动关联的配置数据序号高位
  UCHAR  RelationDnorFloatLow;//用于浮点数设置的下限关联配置浮点数序号，浮动关联的配置数据序号低位
  float fFloatDisplayValue;//用于显示浮动
  
  UCHAR  SignalMode;//信号类型：0：无，1：采集开关量，2：采集模拟量，3：模块开关量，4：模块模拟量，
  UCHAR  SignalUnit[4];//信号单位 4+1
  UINT16 SignalIndex;
  UCHAR  SignalDisplayMode;//信号显示模式：0：不显示，1：直接显示，2：直接反白显示，3：间接显示，4：间接反白显示
  UCHAR  SignalDisplayAlias[10];
  UCHAR  SignalDisplayStart;
  UCHAR  SignalDisplayLen;
  UCHAR  SignalDisplayDecimal;
 
  UCHAR  cStep;
  UCHAR  cFastStep;
  UCHAR  CharUpLimit;
  UCHAR  CharDnLimit;
  float fStep;
  float fFastStep;
  float fUpLimit;
  float fDnLimit;  
}Set_Line_TypeDef;//数据长度55
*****************************************************************/
/*空行 */
#define CFG_LINE_000 \
{\
	0,/*LineNum*/\
  0,/*Title*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  {0},/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*告警级别屏: 告警类型(标题)*/
#define CFG_LINE_001 \
{\
	1,/*LineNum*/\
  70,/*LTitle_070*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  ALARM_TYPE_NUM,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  {0},/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*告警级别屏: 告警类型--(名称)*/
#define CFG_LINE_002 \
{\
	2,/*LineNum*/\
  0,/*LTitle_070*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  ALARMTYPE_NAME,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  140,141,142,144,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}	
/*告警级别屏: 告警级别*/
#define CFG_LINE_003 \
{\
	3,/*LineNum*/\
  71,/*LTitle_071*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  ALARMTYPE_LEVEL,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_AlarmLevel_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,12,13,14, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  3,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}	
/*告警级别屏: 关联继电器*/
#define CFG_LINE_004 \
{\
	4,/*LineNum*/\
  72,/*LTitle_072*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  ALARM_RELAY,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_AlarmRelay_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,51,52,53,54,55,56,57,58, 0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  8,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*告警设置屏: 告警设置(标题)*/
#define CFG_LINE_005 \
{\
	5,/*LineNum*/\
  68,/*LTitle_068*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
 /*告警设置屏: 告警音长*/
#define CFG_LINE_006 \
{\
	6,/*LineNum*/\
  73,/*LTitle_073*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_AlarmTime_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  16,60,61,62,63,64, 0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  5,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*告警设置屏: 清空-- 历史告警、测试数据、统计信息 */
#define CFG_LINE_007 \
{\
	7,/*LineNum*/\
  74,/*LTitle_074*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_CLEAR,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  90, 91,92,93,94,95,96,129,128,127,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  9,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  8,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*告警设置屏:  阻塞当前告警*/
#define CFG_LINE_008 \
{\
	8,/*LineNum*/\
  75,/*LTitle_075*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_AlarmBlock_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1, 59,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*开关量设置屏:(标题)*/
#define CFG_LINE_009 \
{\
	9,/*LineNum*/\
  69,/*LTitle_069*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
 /*开关量设置屏:  开关量序号*/
#define CFG_LINE_010 \
{\
	10,/*LineNum*/\
  76,/*LTitle_076*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  DI_Number,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  51,52,53,54,55,56,57,58, 0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  1,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  7,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*开关量设置屏:  开关量名称*/
#define CFG_LINE_011 \
{\
	11,/*LineNum*/\
  0,/*LTitle_076*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_DIN_NAME,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SwitchName1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  16,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  NAME_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*开关量设置屏:  告警方式*/
#define CFG_LINE_012 \
{\
	12,/*LineNum*/\
  77,/*LTitle_077*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SwitchCfg1_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  48,49, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--基本参数 设置屏:  基本参数(标题)*/
#define CFG_LINE_013 \
{\
	13,/*LineNum*/\
  78,/*LTitle_078*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*快速控制：充电控制*/
#define CFG_LINE_014 \
{\
	14,/*LineNum*/\
  83,/*LTitle_083*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTRL_SIGNAL,/*SignalMode*/\
  "",/*SignalUnit*/\
  CHARGE_CTRL,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  21,22,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--基本参数 设置屏:  电池镕丝组*/
#define CFG_LINE_015 \
{\
	15,/*LineNum*/\
  84,/*LTitle_084*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattNum_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  4,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--基本参数 设置屏:  电池标称容量*/
#define CFG_LINE_016 \
{\
	16,/*LineNum*/\
  85,/*LTitle_085*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_BattNum_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"AH",/*SignalUnit*/\
  SETDATA_BattStdCap_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  5000,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*电池参数--基本参数 设置屏:  电池分流器1 - 有/无*/
#define CFG_LINE_017 \
{\
	17,/*LineNum*/\
  86,/*LTitle_086*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CurrCfg_NUM1,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  5,4, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--基本参数 设置屏:  电池分流器2*/
#define CFG_LINE_018 \
{\
	18,/*LineNum*/\
  87,/*LTitle_087*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CurrCfg_NUM2,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  5,4, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--基本参数 设置屏:  分流器系数(标题)*/
#define CFG_LINE_019 \
{\
	19,/*LineNum*/\
  88,/*LTitle_088*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--基本参数 设置屏:  分流器系数-电流*/
#define CFG_LINE_020 \
{\
	20,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_BCurrCoeffI_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2000,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*电池参数--基本参数 设置屏:  分流器系数-电压*/
#define CFG_LINE_021 \
{\
	21,/*LineNum*/\
  90,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_BCurrCoeffV_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  500,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
 /*电池参数--下电保护 设置屏:(标题) */
#define CFG_LINE_022 \
{\
	22,/*LineNum*/\
  79,/*LTitle_079*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--下电保护 设置屏:  负载下电允许*/
#define CFG_LINE_023 \
{\
	23,/*LineNum*/\
  91,/*LTitle_091*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_LoadCutEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--下电保护 设置屏:  电池保护允许*/
#define CFG_LINE_024 \
{\
	24,/*LineNum*/\
  92,/*LTitle_092*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattCutEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--下电保护 设置屏:  下电方式 */
#define CFG_LINE_025 \
{\
	25,/*LineNum*/\
  93,/*LTitle_093*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CutMode_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  65,66, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--下电保护 设置屏:  负载下电 */
#define CFG_LINE_026 \
{\
	26,/*LineNum*/\
  94,/*LTitle_094*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DCUnderVolt_NUM,/*RelationUporFloatHigh*/\
  SETDATA_BattCutVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_LoadCutVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
} 
/*电池参数--下电保护 设置屏:  电池保护*/
#define CFG_LINE_027 \
{\
	27,/*LineNum*/\
  95,/*LTitle_095*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_LoadCutVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_BattCutVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
} 
/*快速设置：绝缘检测*/
#define CFG_LINE_028 \
{\
  28,/*LineNum*/\
  42,/*LTitle_042*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTRL_SIGNAL ,/*SignalMode*/\
  "",/*SignalUnit*/\
  INSUL_CTRL,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  22,44,45,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--下电保护 设置屏:  电池保护(时间)*/
#define CFG_LINE_029 \
{\
	29,/*LineNum*/\
  97,/*LTitle_097*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_LoadCutTime_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_BattCutTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  999,/*fUpLimit*/\
  3,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:(标题)*/
#define CFG_LINE_030 \
{\
	30,/*LineNum*/\
  80,/*LTitle_080*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  浮充电压*/
#define CFG_LINE_031 \
{\
	31,/*LineNum*/\
  98,/*LTitle_098*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_ECVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_FCVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  42,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  均充电压*/
#define CFG_LINE_032 \
{\
	32,/*LineNum*/\
  99,/*LTitle_099*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_FCVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_ECVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  42,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  限流点*/
#define CFG_LINE_033 \
{\
	33,/*LineNum*/\
  100,/*LTitle_100*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_OverCurr_NUM,/*RelationUporFloatHigh*/\
  SETDATA_ECChgCurr_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_LimitCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
 3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.01,/*fFastStep*/\
  0.25,/*fUpLimit*/\
  0.1,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  过流点*/
#define CFG_LINE_034 \
{\
	34,/*LineNum*/\
  101,/*LTitle_101*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_LimitCurr_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_OverCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.01,/*fFastStep*/\
  1,/*fUpLimit*/\
  0.3,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  自动均充 (标题)*/
#define CFG_LINE_035 \
{\
	35,/*LineNum*/\
  102,/*LTitle_102*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  自动均充 -使能*/
#define CFG_LINE_036 \
{\
	36,/*LineNum*/\
  107,/*LTitle_107*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_AutoECEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  2,3, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  自动均充 -转均充电流*/
#define CFG_LINE_037 \
{\
	37,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_LimitCurr_NUM,/*RelationUporFloatHigh*/\
  SETDATA_StableECCurr_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_ECChgCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.01,/*fFastStep*/\
  0.08,/*fUpLimit*/\
  0.04,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  自动均充 -转均充容量*/
#define CFG_LINE_038 \
{\
	38,/*LineNum*/\
  105,/*LTitle_105*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"%",/*SignalUnit*/\
  SETDATA_ECChgCapRate_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  10,/*fFastStep*/\
  99,/*fUpLimit*/\
  10,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  稳流均充 (标题)*/
#define CFG_LINE_039 \
{\
	39,/*LineNum*/\
  106,/*LTitle_106*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  稳流均充 -稳流均充起始电流*/
#define CFG_LINE_040 \
{\
	40,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_ECChgCurr_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_StableECCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.01,/*fFastStep*/\
  0.02,/*fUpLimit*/\
  0.002,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  稳流均充 -稳流均充持续时间*/
#define CFG_LINE_041 \
{\
	41,/*LineNum*/\
  104,/*LTitle_104*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_StableECTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1440,/*fUpLimit*/\
  5,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  周期均充 (标题)*/
#define CFG_LINE_042 \
{\
	42,/*LineNum*/\
  103,/*LTitle_103*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
 /*电池参数--充电管理 设置屏:  周期定时均充 -使能*/
#define CFG_LINE_043 \
{\
	43,/*LineNum*/\
  107,/*LTitle_107*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_TimeECEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  2,3, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  周期均充 -周期均充周期*/
#define CFG_LINE_044 \
{\
	44,/*LineNum*/\
  109,/*LTitle_109*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"h",/*SignalUnit*/\
  SETDATA_ECPeriod_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  8760,/*fUpLimit*/\
  48,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  周期均充 -周期均充持续时间*/
#define CFG_LINE_045 \
{\
	45,/*LineNum*/\
  104,/*LTitle_104*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_ECTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2880,/*fUpLimit*/\
  30,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  均充保护时间 (标题)*/
#define CFG_LINE_046 \
{\
	46,/*LineNum*/\
  110,/*LTitle_110*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  均充保护时间 -- 时间*/
#define CFG_LINE_047 \
{\
	47,/*LineNum*/\
  0,/*LTitle_104*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_ECProtTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2880,/*fUpLimit*/\
  60,/*fDnLimit*/\
} 
/*电池参数--充电管理 设置屏:  电池安装日期 (标题)*/
#define CFG_LINE_048 \
{\
	48,/*LineNum*/\
  111,/*LTitle_111*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
 /*电池参数--充电管理 设置屏:  电池安装日期 -- 时间*/
#define CFG_LINE_049 \
{\
	49,/*LineNum*/\
  0,/*LTitle_104*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_BATT_DATE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattInstallDate_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  9,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*测试参数 充电模式 */
#define CFG_LINE_050 \
{\
  50,/*LineNum*/\
  110,/*LTitle_110*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTRL_SIGNAL,/*SignalMode*/\
  "",/*SignalUnit*/\
  CHARGE_MODE_SET,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  14,/*SignalDisplayStart*/\
  1,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  7,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*测试参数 模块开关 */
#define CFG_LINE_051 \
{\
  51,/*LineNum*/\
  113,/*LTitle_110*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTRL_SIGNAL,/*SignalMode*/\
  "",/*SignalUnit*/\
  RECT_ON_OFF_CTRL,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  16,15,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*测试参数 风机全速 */
#define CFG_LINE_052 \
{\
  52,/*LineNum*/\
  115,/*LTitle_115*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTRL_SIGNAL,/*SignalMode*/\
  "",/*SignalUnit*/\
  FAN_FULL_CTRL,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  16,15,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 

/*测试参数 输出接触器 */
#define CFG_LINE_053 \
{\
  53,/*LineNum*/\
  106,/*LTitle_106*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTRL_SIGNAL,/*SignalMode*/\
  "",/*SignalUnit*/\
  CONTACTOR_CTRL,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  49,48,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*测试参数－最大允许电压*/
#define CFG_LINE_054 \
{\
  54,/*LineNum*/\
  111,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SET_SIGNAL,/*SignalMode*/\
  "V",/*SignalUnit*/\
  MAX_NEED_VOLT ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  900,/*fUpLimit*/\
  100,/*fDnLimit*/\
} 
/*测试参数－最大允许电流*/
#define CFG_LINE_055 \
{\
  55,/*LineNum*/\
  112,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SET_SIGNAL,/*SignalMode*/\
  "V",/*SignalUnit*/\
  MAX_NEED_CURR ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  900,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏:  终止测试 - 容量*/
#define CFG_LINE_056 \
{\
	56,/*LineNum*/\
  105,/*LTitle_105*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_BattTestCap_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.1,/*fFastStep*/\
  1.0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 定时测试允许*/
#define CFG_LINE_057 \
{\
	57,/*LineNum*/\
  116,/*LTitle_116*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_TimeTestEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 定时测试时间  (标题) */
#define CFG_LINE_058 \
{\
	58,/*LineNum*/\
  115,/*LTitle_115*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池测试 设置屏: 定时测试时间-时间1*/
#define CFG_LINE_059 \
{\
	59,/*LineNum*/\
  0,/*LTitle_116*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_TEST_DATE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattTestDate_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  11,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池测试 设置屏: 定时测试时间-时间2*/
#define CFG_LINE_060 \
{\
	60,/*LineNum*/\
  0,/*LTitle_116*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_TEST_DATE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattTestDate2_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  11,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池测试 设置屏: 定时测试时间-时间3*/
#define CFG_LINE_061 \
{\
	61,/*LineNum*/\
  0,/*LTitle_116*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_TEST_DATE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattTestDate3_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  11,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池测试 设置屏: 定时测试时间-时间4*/
#define CFG_LINE_062 \
{\
	62,/*LineNum*/\
  0,/*LTitle_116*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_TEST_DATE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattTestDate4_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  3,/*SignalDisplayStart*/\
  11,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池测试 设置屏: 快速测试 (标题) */
#define CFG_LINE_063 \
{\
	63,/*LineNum*/\
  117,/*LTitle_117*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池测试 设置屏: 快速测试 -使能*/
#define CFG_LINE_064 \
{\
	64,/*LineNum*/\
  107,/*LTitle_107*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_ShortTestEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  2,3, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 快速测试 -告警电流*/
#define CFG_LINE_065 \
{\
	65,/*LineNum*/\
  119,/*LTitle_119*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_BattShortTestAlarm_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 快速测试 -周期*/
#define CFG_LINE_066 \
{\
	66,/*LineNum*/\
  109,/*LTitle_109*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"h",/*SignalUnit*/\
  SETDATA_BattShortTestPeriod_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  8760,/*fUpLimit*/\
  24,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 快速测试 -持续时间*/
#define CFG_LINE_067 \
{\
	67,/*LineNum*/\
  104,/*LTitle_104*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_BattShortTestTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  60,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 恒流测试  (标题) */
#define CFG_LINE_068 \
{\
	68,/*LineNum*/\
  122,/*LTitle_122*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
 /*电池参数--电池测试 设置屏: 恒流测试 -使能*/
#define CFG_LINE_069 \
{\
	69,/*LineNum*/\
  107,/*LTitle_107*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_StableTestEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  2,3, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*电池参数--电池测试 设置屏: 恒流测试 -告警电流*/
#define CFG_LINE_070 \
{\
	70,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_BattTestCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  9999,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*电池参数--电池温补 设置屏: 温补  (标题) */
#define CFG_LINE_071 \
{\
	71,/*LineNum*/\
  82,/*LTitle_082*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--电池温补 设置屏: 温补-中心点*/
#define CFG_LINE_072 \
{\
	72,/*LineNum*/\
  124,/*LTitle_124*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_TempCompCenter_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  40,/*fUpLimit*/\
  10,/*fDnLimit*/\
} 
/*电池参数--电池温补 设置屏: 温补-系数*/
#define CFG_LINE_073 \
{\
	73,/*LineNum*/\
  125,/*LTitle_125*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV/C",/*SignalUnit*/\
  SETDATA_TempCompCoeff_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  500,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*交流参数  设置屏: (标题) */
#define CFG_LINE_074 \
{\
	74,/*LineNum*/\
  61,/*LTitle_061*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*交流参数  设置屏: 过压告警*/
#define CFG_LINE_075 \
{\
	75,/*LineNum*/\
  126,/*LTitle_126*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_ACLowVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_ACOverVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  500,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
 /*交流参数  设置屏: 欠压告警*/
#define CFG_LINE_076 \
{\
	76,/*LineNum*/\
  127,/*LTitle_127*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_ACOverVolt_NUM,/*RelationUporFloatHigh*/\
  SETDATA_ACLow2Volt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_ACLowVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  500,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*交流参数  设置屏: 缺相告警*/
#define CFG_LINE_077 \
{\
	77,/*LineNum*/\
  128,/*LTitle_128*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_ACLowVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_ACLow2Volt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  500,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*交流参数  设置屏: 交流采样输入*/
#define CFG_LINE_078 \
{\
	78,/*LineNum*/\
  129,/*LTitle_129*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
    FLOAT_MTEQ_SETDATA_CHAR,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  SETDATA_DGFunctionEnable_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_DGFunctionEnable_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_ACinCtl_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,27,28,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*2,*//*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
 /*交流参数  设置屏: 交流输入-三、单项*/
#define CFG_LINE_079 \
{\
	79,/*LineNum*/\
  130,/*LTitle_130*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_ACinType_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7, 67,68, 0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  1,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*直流参数  设置屏: (标题)*/
#define CFG_LINE_080 \
{\
	80,/*LineNum*/\
  62,/*LTitle_062*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*直流参数  设置屏: 过压告警*/
#define CFG_LINE_081 \
{\
	81,/*LineNum*/\
  120,/*LTitle_120*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_DCOverVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  (SETDATA_DCOverVolt_NUM+1),/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
} 
/*直流参数  设置屏: 高压告警*/
#define CFG_LINE_082 \
{\
	82,/*LineNum*/\
  121,/*LTitle_121*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  (SETDATA_DCOverVolt_NUM+1),/*RelationUporFloatHigh*/\
  SETDATA_DCLowVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DCOverVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
} 
/*直流参数  设置屏: 低压告警*/
#define CFG_LINE_083 \
{\
	83,/*LineNum*/\
  131,/*LTitle_131*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DCOverVolt_NUM,/*RelationUporFloatHigh*/\
  SETDATA_DCUnderVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DCLowVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
} 
/*直流参数  设置屏: 欠压告警*/
#define CFG_LINE_084 \
{\
	84,/*LineNum*/\
  132,/*LTitle_132*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DCLowVolt_NUM,/*RelationUporFloatHigh*/\
  SETDATA_LoadCutVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DCUnderVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
} 
/*模块参数  设置屏: (标题) */
#define CFG_LINE_085 \
{\
	85,/*LineNum*/\
  63,/*LTitle_063*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*模块参数  设置屏: 槽位使能*/
#define CFG_LINE_086 \
{\
	86,/*LineNum*/\
  135,/*LTitle_135*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_RectACPhaseEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*模块参数  设置屏: 过压保护*/
#define CFG_LINE_087 \
{\
	87,/*LineNum*/\
  136,/*LTitle_136*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_RectVoltageUp_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  59,/*fUpLimit*/\
  56,/*fDnLimit*/\
} 
/*模块参数  设置屏:  默认保护*/
#define CFG_LINE_088 \
{\
	88,/*LineNum*/\
  137,/*LTitle_137*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_RectDefaultVoltage_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  48,/*fDnLimit*/\
} 
/*模块参数  设置屏: 缓启动使能*/
#define CFG_LINE_089 \
{\
	89,/*LineNum*/\
  138,/*LTitle_138*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SetWalkIn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
 /*模块参数  设置屏: 缓启动时间*/
#define CFG_LINE_090 \
{\
	90,/*LineNum*/\
  139,/*LTitle_139*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"S",/*SignalUnit*/\
  SETDATA_RectLoadStartTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  128,/*fUpLimit*/\
  8,/*fDnLimit*/\
}
/*模块参数  设置屏: 顺序启机间隔时间*/
#define CFG_LINE_091 \
{\
	91,/*LineNum*/\
  140,/*LTitle_140*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"S",/*SignalUnit*/\
  SETDATA_RectOnInterval_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  10,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*模块参数  设置屏: 交流过压开机使能*/
#define CFG_LINE_092 \
{\
	92,/*LineNum*/\
  141,/*LTitle_141*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_FORCERECTACON_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*模块参数  设置屏: 输入限流*/
#define CFG_LINE_093 \
{\
	93,/*LineNum*/\
  142,/*LTitle_142*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_RectACCurrLimit_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  50,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*环境参数 - 传感器  设置屏: (标题) */
#define CFG_LINE_094 \
{\
	94,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*环境参数 - 传感器  设置屏: 温度1 */
#define CFG_LINE_095 \
{\
	95,/*LineNum*/\
  146,/*LTitle_146*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_Temp1Cfg_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,45,44,41,42,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*环境参数 - 传感器  设置屏: 温度2 */
#define CFG_LINE_096 \
{\
	96,/*LineNum*/\
  147,/*LTitle_147*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_Temp2Cfg_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,45,44,41,42, 0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 油机容量（标题） */
#define CFG_LINE_097 \
{\
	97,/*LineNum*/\
  41,/*LTitle_041*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 油机参数 - 油机容量 */
#define CFG_LINE_098 \
{\
	0,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"KVA",/*SignalUnit*/\
  SETDATA_DGPowerCap_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  100,/*fUpLimit*/\
  5,/*fDnLimit*/\
}
/* 油机参数 - 启动油机条件(标题) */
#define CFG_LINE_099 \
{\
	218,/*LineNum*/\
  42,/*LTitle_042*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/*环境参数 - 传感器  设置屏: 温补传感器 */
#define CFG_LINE_100 \
{\
	100,/*LineNum*/\
  150,/*LTitle_150*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_TCSelect_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  71,72,73, 0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 油机控制 - 启动电池电压 */
#define CFG_LINE_101 \
{\
	101,/*LineNum*/\
  90,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_BattCutVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DGStartVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
}
/* 油机参数 - 油机控制 - 停止电压 */
#define CFG_LINE_102 \
{\
	102,/*LineNum*/\
  90,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_FCVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DGStopVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  60,/*fUpLimit*/\
  40,/*fDnLimit*/\
}

/* 油机参数 - 根据电池剩余容量启机 - 使能 */
#define CFG_LINE_103 \
{\
	103,/*LineNum*/\
  44,/*LTitle_044*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGStartByBattCapEn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/*环境参数 - 传感器  设置屏: 环境高温*/
#define CFG_LINE_104 \
{\
	104,/*LineNum*/\
  157,/*LTitle_157*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_TempLow_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_TempOver_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  -40,/*fDnLimit*/\
} 
/*环境参数 - 传感器  设置屏: 环境低温*/
#define CFG_LINE_105 \
{\
	105,/*LineNum*/\
  158,/*LTitle_158*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_TempOver_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_TempLow_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  -40,/*fDnLimit*/\
} 
/*环境参数 - 传感器  设置屏: 电池过温-比高温更高*/
#define CFG_LINE_106 \
{\
	106,/*LineNum*/\
  154,/*LTitle_154*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_BattTempOver_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_BattTempOver2_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
} 
/*环境参数 - 传感器  设置屏: 电池高温*/
#define CFG_LINE_107 \
{\
	107,/*LineNum*/\
  155,/*LTitle_155*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UPDN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_BattTempOver2_NUM,/*RelationUporFloatHigh*/\
  SETDATA_BattTempLow_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_BattTempOver_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
} 
/*环境参数 - 传感器  设置屏: 电池低温*/
#define CFG_LINE_108 \
{\
	108,/*LineNum*/\
  156,/*LTitle_156*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_BattTempOver_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_BattTempLow_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  10,/*fUpLimit*/\
  -40,/*fDnLimit*/\
} 

/* 油机参数 - 根据电池剩余容量启机 - 容量 */
#define CFG_LINE_109 \
{\
	109,/*LineNum*/\
  43,/* LTitle_043*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DGStopSOC_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_DGStopSOC_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"%",/*SignalUnit*/\
  SETDATA_DGOpenBattCap_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  100,/*CharUpLimit*/\
  10,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 关油机条件(标题) */
#define CFG_LINE_110 \
{\
	110,/*LineNum*/\
  169,/*LTitle_169*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,53,54,55,56,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  6,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 根据电池电压停止油机 - 使能 */
#define CFG_LINE_111 \
{\
	111,/*LineNum*/\
  144,/*LTitle_144*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGStopByVoltEn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 70,69, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 油机参数 - 根据电池充电电流停止油机 - 使能 */
#define CFG_LINE_112 \
{\
	112,/*LineNum*/\
  166,/* LTitle_166*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGStopByBattCurrEn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 70,69, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 油机参数 - 根据电池充电电流停止油机 - 充电电流 */
#define CFG_LINE_113 \
{\
	113,/*LineNum*/\
  89, /*89 LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_DGStopBattCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.01,/*fFastStep*/\
  0.06,/*fUpLimit*/\
  0.00,/*fDnLimit*/\
}
/* 油机参数 - 满足停油机条件后的确认时间（分钟） */
#define CFG_LINE_114 \
{\
	114,/*LineNum*/\
  170,/*LTitle_170*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_DGStopDelay_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 油机运行保护(标题) */
#define CFG_LINE_115 \
{\
	246,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 油机参数 - 油机运行保护-最长时间(分钟) */
#define CFG_LINE_116 \
{\
	116,/*LineNum*/\
  172,/*LTitle_172*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DGRunProtTimeMin_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_DGRunProtTimeMin_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_DGRunProtTimeMax_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2880,/*fUpLimit*/\
  30,/*fDnLimit*/\
}
/* 油机参数 - 油机运行保护-最短时间(分钟) */
#define CFG_LINE_117 \
{\
	117,/*LineNum*/\
  14,/*LTitle_014*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DGRunProtTimeMax_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_DGRunProtTimeMax_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_DGRunProtTimeMin_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  720,/*fUpLimit*/\
  5,/*fDnLimit*/\
}

/* 油机参数 - 市电运行状态检测用DI */
#define CFG_LINE_118 \
{\
	118,/*LineNum*/\
  143,/*LTitle_143*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  MAINS_DG_ACPOWER,/*cFloatDisplayValue*/\
  SETDATA_DGWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_DGWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGMainsDI_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 7,73,133,134,135,136,137,138,139,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  8,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 油机控制用Relay */
#define CFG_LINE_119 \
{\
	119,/*LineNum*/\
  64,/*LTitle_064*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGCtrlRelay_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 7,51,52,53,54,55,56,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  6,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/*环境参数 - 风机管理  设置屏:  风机组1-热交换型：半速温度*/
#define CFG_LINE_120 \
{\
	120,/*LineNum*/\
  167,/*LTitle_167*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*//*DCEM 日电量*/\
  5,/*PasswordLevel*//*DCEM 日电量*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  8,111,112,113,114,115,116,117,118,119,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  9,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

 /*环境参数 - 风机管理  设置屏:  风机组1-热交换型：全速温度*/
#define CFG_LINE_121 \
{\
	121,/*LineNum*/\
  170,/*LTitle_170*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mHOT,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_HotFan1FullT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2  (标题)*/
#define CFG_LINE_122 \
{\
	122,/*LineNum*/\
  168,/*LTitle_168*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_DCEM2_COMM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  1,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  8,111,112,113,114,115,116,117,118,119,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  9,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2-热交换型：起转温度*/
#define CFG_LINE_123 \
{\
	123,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mHOT,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_HotFan2StartT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2-热交换型：全速温度*/
#define CFG_LINE_124 \
{\
	124,/*LineNum*/\
  170,/*LTitle_170*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mHOT,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_HotFan2FullT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2-热交换型：停转温度*/
#define CFG_LINE_125 \
{\
	125,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mHOT,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_HotFan2StopT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组1-直通风型：起转温度*/
#define CFG_LINE_126 \
{\
	126,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mAIR,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_AirFan1StartT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组1-直通风型：全速温度*/
#define CFG_LINE_127 \
{\
	127,/*LineNum*/\
  170,/*LTitle_170*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mAIR,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_AirFan1FullT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组1-直通风型：停转温度*/
#define CFG_LINE_128 \
{\
	128,/*LineNum*/\
  172,/*LTitle_172*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mAIR,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_AirFan1StopT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2-直通风型：起转温度*/
#define CFG_LINE_129 \
{\
	129,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mAIR,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_AirFan2StartT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2-直通风型：全速温度*/
#define CFG_LINE_130 \
{\
	130,/*LineNum*/\
  170,/*LTitle_170*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mAIR,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_AirFan2FullT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
/*环境参数 - 风机管理  设置屏:  风机组2-直通风型：停转温度*/
#define CFG_LINE_131 \
{\
	131,/*LineNum*/\
  172,/*LTitle_172*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mAIR,/*cFloatDisplayValue*/\
  SETDATA_FanMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_FanMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_AirFan2StopT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  50,/*fUpLimit*/\
  20,/*fDnLimit*/\
}
 /*系统参数   设置屏:(标题)*/
#define CFG_LINE_132 \
{\
	132,/*LineNum*/\
  65,/*LTitle_065*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  语言*/
#define CFG_LINE_133 \
{\
	133,/*LineNum*/\
  173,/*LTitle_173*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_Language_NUM ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  38,37, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  7,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  时区*/
#define CFG_LINE_134 \
{\
	134,/*LineNum*/\
  174,/*LTitle_174*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"GMT",/*SignalUnit*/\
  SETDATA_LocalTimeZone_NUM ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  9,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  15,/*fStep*/\
	15,/*fFastStep*/\
  13*60,/*fUpLimit*/\
-12*60,/*fDnLimit*/\
}
/*系统参数   设置屏:  日期*/
#define CFG_LINE_135 \
{\
	135,/*LineNum*/\
  175,/*LTitle_175*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CFG_RTC_DATE,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  5,/*SignalDisplayStart*/\
  10,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  30,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  时间*/
#define CFG_LINE_136 \
{\
	136,/*LineNum*/\
  176,/*LTitle_176*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CFG_RTC_TIME,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  60,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  系统类型  (标题)*/
#define CFG_LINE_137 \
{\
	137,/*LineNum*/\
  178,/*LTitle_178*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  系统类型*/
#define CFG_LINE_138 \
{\
	138,/*LineNum*/\
  0,/*LTitle_176*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  4,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR_SYS,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SysType_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  9,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  密码重置*/
#define CFG_LINE_139 \
{\
	139,/*LineNum*/\
  179,/*LTitle_179*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  5,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  INIT_PWD,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
 /*系统参数   设置屏:  系统重置*/
#define CFG_LINE_140 \
{\
	140,/*LineNum*/\
  180,/*LTitle_180*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  4,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SW_CONFIG,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  工程1密码*/
#define CFG_LINE_141 \
{\
	141,/*LineNum*/\
  181,/*LTitle_181*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  3,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_PASSWORD,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PassWord1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PASSWORD_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  工程2密码*/
#define CFG_LINE_142 \
{\
	142,/*LineNum*/\
  182,/*LTitle_182*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  4,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_PASSWORD,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PassWord2_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PASSWORD_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*系统参数   设置屏:  管理密码*/
#define CFG_LINE_143 \
{\
	143,/*LineNum*/\
  183,/*LTitle_183*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  5,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_PASSWORD,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PassWord3_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PASSWORD_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:(标题)*/
#define CFG_LINE_144 \
{\
	144,/*LineNum*/\
  66,/*LTitle_066*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  本机地址*/
#define CFG_LINE_145 \
{\
	145,/*LineNum*/\
  185,/*LTitle_185*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SelfAddress_NUM ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  254,/*CharUpLimit*/\
  1,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  通讯方式*/
#define CFG_LINE_146 \
{\
	146,/*LineNum*/\
  186,/*LTitle_186*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CommMode_NUM ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  31,32, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  通讯协议*/
#define CFG_LINE_147 \
{\
	147,/*LineNum*/\
  187,/*LTitle_187*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_ProtocolMode_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  5, 0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  波特率*/
#define CFG_LINE_148 \
{\
	148,/*LineNum*/\
  188,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BaudRate_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  35,34,33,98, 36,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  4,/*cStep*/\
  4,/*cFastStep*/\
  4,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  IP/掩码/网关*/
#define CFG_LINE_149 \
{\
	149,/*LineNum*/\
  189,/*LTitle_189*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  60,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  ip*/
#define CFG_LINE_150 \
{\
	150,/*LineNum*/\
  0,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_IP,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_IP_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,35,36, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏: 掩码 */
#define CFG_LINE_151 \
{\
	151,/*LineNum*/\
  0,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_IP,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_Subnetmask_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,35,36, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏: 网关 */
#define CFG_LINE_152 \
{\
	152,/*LineNum*/\
  0,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_IP,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_Gateway_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,35,36, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏:  回叫次数*/
#define CFG_LINE_153 \
{\
	153,/*LineNum*/\
  190,/*LTitle_190*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  mMODEM,/*cFloatDisplayValue*/\
  SETDATA_CommMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CommMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CallNum_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  4,/*CharUpLimit*/\
  1,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/*通信参数   设置屏: 电话号码1*/
#define CFG_LINE_154 \
{\
	154,/*LineNum*/\
  0,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  mMODEM,/*cFloatDisplayValue*/\
  SETDATA_CommMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CommMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_PHONE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PhoneNum1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  97,0,0,0, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PHONE_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏: 电话号码2*/
#define CFG_LINE_155 \
{\
	155,/*LineNum*/\
  0,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  mMODEM,/*cFloatDisplayValue*/\
  SETDATA_CommMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CommMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_PHONE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PhoneNum2_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  97,0,0,0, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PHONE_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*通信参数   设置屏: 电话号码3*/
#define CFG_LINE_156 \
{\
	156,/*LineNum*/\
  0,/*LTitle_188*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  mMODEM,/*cFloatDisplayValue*/\
  SETDATA_CommMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CommMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_PHONE,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PhoneNum3_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  97,0,0,0, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  12,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PHONE_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*节能参数   设置屏:(标题)*/
#define CFG_LINE_157 \
{\
	157,/*LineNum*/\
  20,/*LTitle_020*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*节能参数   设置屏:  使能/禁止*/
#define CFG_LINE_158 \
{\
	158,/*LineNum*/\
  50,/*LTitle_050*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SavePower_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
 2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*节能参数   设置屏:  开关周期*/
#define CFG_LINE_159 \
{\
	159,/*LineNum*/\
  192,/*LTitle_192*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"h",/*SignalUnit*/\
  SETDATA_RectOnOffTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  8760,/*fUpLimit*/\
  1,/*fDnLimit*/\
}
/*节能参数   设置屏:  模块工作点*/
#define CFG_LINE_160 \
{\
	160,/*LineNum*/\
  193,/*LTitle_193*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"%",/*SignalUnit*/\
  SETDATA_RectBestWork_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
   1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  30,/*fDnLimit*/\
}
/*节能参数   设置屏:  最少工作模块数*/
#define CFG_LINE_161 \
{\
	161,/*LineNum*/\
  108,/*LTitle_108*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_WORKRECTMIN_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  30,/*CharUpLimit*/\
  1,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*快速设置   设置屏:(标题)*/
#define CFG_LINE_162 \
{\
  162,/*LineNum*/\
  21,/*LTitle_021*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*充电控制  */
#define CFG_LINE_163 \
{\
	163,/*LineNum*/\
  16,/*LTitle_016*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SiteNumber_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*预留*/
#define CFG_LINE_164 \
{\
	164,/*LineNum*/\
  285,/*LTitle_285*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CstNumber0_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*预留  */
#define CFG_LINE_165 \
{\
	165,/*LineNum*/\
  286,/*LTitle_286*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CstNumber1_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:(标题)*/
#define CFG_LINE_166 \
{\
	166,/*LineNum*/\
  19,/*LTitle_019*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  启动 均浮充*/
#define CFG_LINE_167 \
{\
	167,/*LineNum*/\
  53,/*LTitle_053*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_BATT_MNG,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  17,18,19, 0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  电池 上下电*/
#define CFG_LINE_168 \
{\
	168,/*LineNum*/\
  54,/*LTitle_054*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_LVD2,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  29,30,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  9,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  负载 上下电*/
#define CFG_LINE_169 \
{\
	169,/*LineNum*/\
  55,/*LTitle_055*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_LVD1,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  29,30,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  9,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  模块调压*/
#define CFG_LINE_170 \
{\
	170,/*LineNum*/\
  56,/*LTitle_056*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
    NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_VOLTAGE,/*SignalMode*/\
"V",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  42,/*fDnLimit*/\
}
/*控制输出   设置屏:  模块限流*/
#define CFG_LINE_171 \
{\
	171,/*LineNum*/\
  57,/*LTitle_057*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_SOLAR,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_CURRENT_LIMIT,/*SignalMode*/\
"%",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  121,/*fUpLimit*/\
  10,/*fDnLimit*/\
}
/*控制输出   设置屏:  模块 交直流开关/复位*/
#define CFG_LINE_172 \
{\
	172,/*LineNum*/\
  213,/*LTitle_213*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_RECT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_RECT_DC_ONOFF,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15,16,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*模块参数  设置屏: 槽位--设置*/
#define CFG_LINE_173 \
{\
	173,/*LineNum*/\
  51,/*LTitle_051*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_RECT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_RECT_LOCATION,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_RectLocation_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0, 0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  30,/*CharUpLimit*/\
  1,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*登录屏   设置屏:(标题)*/
#define CFG_LINE_174 \
{\
	174,/*LineNum*/\
  114,/*LTitle_114*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*登录屏 ：  输入密码*/
#define CFG_LINE_175 \
{\
	175,/*LineNum*/\
  47,/*LTitle_047*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  ENTER_PASSWORD,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  PASSWORD_LEN,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  PASSWORD_END,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*密码错误   设置屏:(标题)*/
#define CFG_LINE_176 \
{\
	176,/*LineNum*/\
  118,/*LTitle_118*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/*系统参数  启动下载*/
#define CFG_LINE_177 \
{\
  177,/*LineNum*/\
  219,/*LTitle_219*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  5,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  Bootloader,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,140,141,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 太阳能使能 */
#define CFG_LINE_178 \
{\
	178,/*LineNum*/\
  14,/*LTitle_14*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  4,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_SolarEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
 2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 油机参数 - 油机运行状态检测用DI */
#define CFG_LINE_179 \
{\
	179,/*LineNum*/\
  279,/*LTitle_279*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGRunDI_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 7,73,133,134,135,136,137,138,139,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  8,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 预留 */
#define CFG_LINE_180 \
{\
	180,/*LineNum*/\
  287,/*LTitle_287*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CstNumber2_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  255,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 预留 */
#define CFG_LINE_181 \
{\
	181,/*LineNum*/\
  288,/*LTitle_288*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CloseEn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 5,4,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  14,/*SignalDisplayStart*/\
  1,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 预留 */
#define CFG_LINE_182 \
{\
	182,/*LineNum*/\
  0,/*LTitle_288*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}


/*环境参数 - 加热器管理  设置屏:-(标题)*/
#define CFG_LINE_183 \
{\
	183,/*LineNum*/\
  145,/*LTitle_145*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*环境参数 - 加热器管理  设置屏:  加热器个数*/
#define CFG_LINE_184 \
{\
	184,/*LineNum*/\
  211,/*LTitle_211*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_HeaterNum_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
 7,51,52,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*环境参数 - 加热器管理  设置屏: 加热器1 -(标题) */
#define CFG_LINE_185 \
{\
	185,/*LineNum*/\
  152,/*LTitle_152*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 


/*环境参数 - 加热器管理  设置屏: 加热器1 -启动温度*/
#define CFG_LINE_186 \
{\
	186,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_Heater1StartT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  40,/*fUpLimit*/\
-40,/*fDnLimit*/\
}
/*环境参数 - 加热器管理  设置屏: 加热器1 -加热范围*/
#define CFG_LINE_187 \
{\
	187,/*LineNum*/\
  177,/*LTitle_177*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_Heater1UpT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  20,/*fUpLimit*/\
  5,/*fDnLimit*/\
}
/*环境参数 - 加热器管理  设置屏: 加热器2 -(标题)*/
#define CFG_LINE_188 \
{\
	188,/*LineNum*/\
  153,/*LTitle_153*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*环境参数 - 加热器管理  设置屏: 加热器2 -启动温度*/
#define CFG_LINE_189 \
{\
	189,/*LineNum*/\
  171,/*LTitle_171*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_Heater2StartT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  40,/*fUpLimit*/\
-40,/*fDnLimit*/\
}
/*环境参数 - 加热器管理  设置屏: 加热器2 -加热范围*/
#define CFG_LINE_190 \
{\
	190,/*LineNum*/\
  177,/*LTitle_177*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_Heater2UpT_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  20,/*fUpLimit*/\
  5,/*fDnLimit*/\
}

/*通信参数   设置屏:  电话号码  (标题)*/
#define CFG_LINE_191 \
{\
	191,/*LineNum*/\
  191,/*LTitle_191*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  mMODEM,/*cFloatDisplayValue*/\
  SETDATA_CommMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CommMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}


/*直流参数    设置屏:  负载分流器*/
#define CFG_LINE_192 \
{\
	192,/*LineNum*/\
  133,/*LTitle_133*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CurrCfg_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,120,121,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*直流参数    设置屏:  负载分流器系数-电流*/
#define CFG_LINE_193 \
{\
	193,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_CurrCoeffI_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2000,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 
/*直流参数    设置屏:  负载分流器系数-电压*/
#define CFG_LINE_194 \
{\
	194,/*LineNum*/\
  90,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQS_SYSTYPE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_CurrCoeffV_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  500,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 

/* 控制输出屏:  提示手动下下翻 */
#define CFG_LINE_195 \
{\
	195,/*LineNum*/\
  212,/*LTitle_212*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/*控制输出   设置屏:  模块 交直流开关/复位*/
#define CFG_LINE_196 \
{\
	196,/*LineNum*/\
  214,/*LTitle_214*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_RECT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_RECT_AC_ONOFF,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15,16,0,79,80,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  模块 交直流开关/复位*/
#define CFG_LINE_197 \
{\
	197,/*LineNum*/\
  215,/*LTitle_215*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_RECT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_RECT_RESET,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  10,39,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 系统模式 （标题） */
#define CFG_LINE_198 \
{\
	198,/*LineNum*/\
  177,/*LTitle_177*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 系统模式 （标题） */
#define CFG_LINE_199 \
{\
	199,/*LineNum*/\
  0,/*LTitle_199*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  4,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  FLOAT_NOT_DISPLAY_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR_Systemmode,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_SystemMode_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  104,103,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_200 \
{\
	200,/*LineNum*/\
  216,/*LTitle_216*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_Temp3Cfg_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,45,44,41,42, 0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  2,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_201 \
{\
	201,/*LineNum*/\
  217,/*LTitle_217*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattSymmetryOption_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  7,107,108,109,110,132,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  5,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_202 \
{\
	202,/*LineNum*/\
  220,/*LTitle_220*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_BattSymmetryAlarm_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  0.1,/*fFastStep*/\
  1,/*fUpLimit*/\
  0.2,/*fDnLimit*/\
}

#define CFG_LINE_203 \
{\
	203,/*LineNum*/\
  221,/*LTitle_221*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_PowerSplitVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  0.1,/*fFastStep*/\
  2,/*fUpLimit*/\
  0.1,/*fDnLimit*/\
}

#define CFG_LINE_204 \
{\
	204,/*LineNum*/\
  222,/*LTitle_222*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"%",/*SignalUnit*/\
  SETDATA_PowerSplitCurrLimit_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  90,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_205 \
{\
	205,/*LineNum*/\
  223,/*LTitle_223*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PowerSplitCoeff_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  100,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_206 \
{\
	206,/*LineNum*/\
  224,/*LTitle_224*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"S",/*SignalUnit*/\
  SETDATA_PowerSplitTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_207 \
{\
	207,/*LineNum*/\
  226,/*LTitle_226*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_PowerMode_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  2,3,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*预留 (油机限流功能-标题)*/
#define CFG_LINE_208 \
{\
	208,/*LineNum*/\
  235,/*LTitle_235*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
  "",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*预留 (油机限流功能-使能)*/
#define CFG_LINE_209 \
{\
	209,/*LineNum*/\
  107,/*LTitle_107*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_BattChargeLimitEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  2,3,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  12,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*预留 (油机限流功能-限流点)*/
#define CFG_LINE_210 \
{\
	210,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"C",/*SignalUnit*/\
  SETDATA_BattChargeLimit_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  3,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.001,/*fStep*/\
  0.001,/*fFastStep*/\
  0.1,/*fUpLimit*/\
  0.03,/*fDnLimit*/\
}

#define CFG_LINE_211 \
{\
	211,/*LineNum*/\
  236,/*LTitle_236*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_212 \
{\
	212,/*LineNum*/\
  237,/*LTitle_237*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM1ShuntCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_213 \
{\
	213,/*LineNum*/\
  238,/*LTitle_238*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM1ShuntVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_214 \
{\
	214,/*LineNum*/\
  239,/*LTitle_239*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM1ShuntCurr_NUM+1,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_215 \
{\
	215,/*LineNum*/\
  240,/*LTitle_240*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM1ShuntVolt_NUM+1,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_216 \
{\
	216,/*LineNum*/\
  241,/*LTitle_241*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM1ShuntCurr_NUM+2,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_217 \
{\
	217,/*LineNum*/\
  242,/*LTitle_242*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM1ShuntVolt_NUM+2,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_218 \
{\
	218,/*LineNum*/\
  243,/*LTitle_243*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM1ShuntCurr_NUM+3,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_219 \
{\
	219,/*LineNum*/\
  244,/*LTitle_244*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM1ShuntVolt_NUM+3,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

//////////
#define CFG_LINE_220 \
{\
	220,/*LineNum*/\
  245,/*LTitle_245*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_221 \
{\
	221,/*LineNum*/\
  237,/*LTitle_237*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM2ShuntCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_222 \
{\
	222,/*LineNum*/\
  238,/*LTitle_238*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM2ShuntVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_223 \
{\
	223,/*LineNum*/\
  239,/*LTitle_239*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM2ShuntCurr_NUM+1,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_224 \
{\
	224,/*LineNum*/\
  240,/*LTitle_240*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM2ShuntVolt_NUM+1,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_225 \
{\
	225,/*LineNum*/\
  241,/*LTitle_241*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM2ShuntCurr_NUM+2,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_226 \
{\
	226,/*LineNum*/\
  242,/*LTitle_242*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM2ShuntVolt_NUM+2,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_227 \
{\
	227,/*LineNum*/\
  243,/*LTitle_243*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"A",/*SignalUnit*/\
  SETDATA_DCEM2ShuntCurr_NUM+3,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_228 \
{\
	228,/*LineNum*/\
  244,/*LTitle_244*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"mV",/*SignalUnit*/\
  SETDATA_DCEM2ShuntVolt_NUM+3,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

#define CFG_LINE_229 \
{\
	229,/*LineNum*/\
  211,/*LTitle_211*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_CTCoeff_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  10000,/*fUpLimit*/\
  10,/*fDnLimit*/\
}
/*DCEM 日电量*/
#define CFG_LINE_230 \
{\
	230,/*LineNum*/\
  45,/*LTitle_045*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  1,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_231 \
{\
	231,/*LineNum*/\
  46,/*LTitle_046*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  1,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  3,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_232 \
{\
	232,/*LineNum*/\
  276,/*LTitle_276*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  1,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  49,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

#define CFG_LINE_233 \
{\
	233,/*LineNum*/\
  145,/*LTitle_145*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  6,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
///////////
#define CFG_LINE_234 \
{\
	234,/*LineNum*/\
  271,/*LTitle_271*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*参数设置-系统参数  设置屏： 监控工作模式*/
#define CFG_LINE_235 \
{\
	235,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  5,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR_SYSMODE,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_SysWorkMode_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode  */\
  123,124,125,126,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  4,/*SignalDisplayStart*/\
  10,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  3,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}


/* 电池参数--充电管理   设置屏- MPPT混插模式下 整流模块浮充电压 (标题)*/
#define CFG_LINE_236 \
{\
  236,/*LineNum*/\
  256,/*LTitle_256*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_MIXMODE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_MixModeRectFcVolt_NUM,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  10,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--充电管理 设置屏:  MPPT混插模式下 整流模块浮充电压*/
#define CFG_LINE_237 \
{\
	237,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP|FLOAT_NEQ_MIXMODE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_FCVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_MixModeRectFcVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  45,/*fDnLimit*/\
} 
/* 电池参数--充电管理  设置屏- MPPT混插模式下 整流模块均充电压 (标题)*/
#define CFG_LINE_238 \
{\
	238,/*LineNum*/\
  257,/*LTitle_221*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_NEQ_MIXMODE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_MixModeRectEcVolt_NUM,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  10,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*电池参数--充电管理 设置屏:  MPPT混插模式下 整流模块均充电压*/
#define CFG_LINE_239 \
{\
	239,/*LineNum*/\
  0,/*LTitle_*/\
  0,/*TitleDisplayStart*/\
  0,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_UP|FLOAT_NEQ_MIXMODE,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  SETDATA_ECVolt_NUM,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_MixModeRectEcVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  45,/*fDnLimit*/\
} 

/*MPPT模块参数  设置屏: 槽位使能*/
#define CFG_LINE_240 \
{\
	240,/*LineNum*/\
  135,/*LTitle_135*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_MpptPhaseEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*MPPT模块参数  设置屏: 过压保护*/
#define CFG_LINE_241 \
{\
	241,/*LineNum*/\
  136,/*LTitle_136*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_MpptVoltageUp_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  59,/*fUpLimit*/\
  56,/*fDnLimit*/\
} 
/*MPPT模块参数  设置屏:  默认输出电压*/
#define CFG_LINE_242 \
{\
	242,/*LineNum*/\
  137,/*LTitle_137*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT_V,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_MpptDefaultVoltage_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  58,/*fUpLimit*/\
  48,/*fDnLimit*/\
} 
/*MPPT模块参数  设置屏: 槽位--设置*/
#define CFG_LINE_243 \
{\
	243,/*LineNum*/\
  51,/*LTitle_051*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_MPPT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_MPPT_LOCATION,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_MpptLocation_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0, 0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  10,/*CharUpLimit*/\
  1,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}

/* 设置屏- MPPT混插模式下 均流允许设置*/
#define CFG_LINE_244 \
{\
	244,/*LineNum*/\
  262,/*LTitle_262*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  5,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_MpptLoadShareEnable_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  MPPT模块调压*/
#define CFG_LINE_245 \
{\
	245,/*LineNum*/\
  258,/*LTitle_258*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_NOMPPT,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_MPPT_VOLTAGE,/*SignalMode*/\
"V",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
	1,/*fFastStep*/\
  58,/*fUpLimit*/\
  42,/*fDnLimit*/\
}
/*控制输出   设置屏:  MPPT模块限流*/
#define CFG_LINE_246 \
{\
	246,/*LineNum*/\
  259,/*LTitle_259*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  FLOAT_EQ_NOMPPT,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_MPPT_CURRENT_LIMIT,/*SignalMode*/\
"%",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  121,/*fUpLimit*/\
  10,/*fDnLimit*/\
}
/*控制输出   设置屏:  MPPT模块 直流开关*/
#define CFG_LINE_247 \
{\
	247,/*LineNum*/\
  260,/*LTitle_260*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_MPPT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_MPPT_DC_ONOFF,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  15,16,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*控制输出   设置屏:  MPPT模块 复位*/
#define CFG_LINE_248 \
{\
	248,/*LineNum*/\
  261,/*LTitle_261*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  NOACTIONSCREEN,/*ConfirmScreen*/\
  FLOAT_EQ_MPPT_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  CTL_MPPT_RESET,/*SignalMode*/\
"",/*SignalUnit*/\
  0 ,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  10,39,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  7,/*SignalDisplayStart*/\
  8,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
	0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 光电混合模式下系统管理侧重点：  稳定/节能 */
#define CFG_LINE_249 \
{\
	249,/*LineNum*/\
  273,/*LTitle_273  控制模式*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  5,/*PasswordLevel*/\
  CONFIRMSCREEN,/*ConfirmScreen*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  SYSMODE_MIX,/*cFloatDisplayValue*/\
  SETDATA_SysWorkMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_SysWorkMode_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_EnergyCtrlMode_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  130,131,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  9,/*SignalDisplayStart*/\
  6,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/*光伏模块参数  设置屏:  进入MPPT模式的压差条件 V*/
#define CFG_LINE_250 \
{\
	250,/*LineNum*/\
  274,/*LTitle_274*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_StartMpptDeltaVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  3,/*fUpLimit*/\
  0.2,/*fDnLimit*/\
} 

/* 油机参数 - 根据时间停止油机 - 使能 */
#define CFG_LINE_251 \
{\
	251,/*LineNum*/\
  278,/* LTitle_278*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGStopByTimeEn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59, 70,69, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 油机参数 - 根据时间停止油机 - 时间 */
#define CFG_LINE_252 \
{\
	252,/*LineNum*/\
  277, /* LTitle_277*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"min",/*SignalUnit*/\
  SETDATA_DGRunDurationMinute_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  8,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  1440,/*fUpLimit*/\
  10,/*fDnLimit*/\
}

/* 油机参数 - 根据SOC停止油机 - 使能 */
#define CFG_LINE_253 \
{\
  253,/*LineNum*/\
  44,/*LTitle_044*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"",/*SignalUnit*/\
  SETDATA_DGStopBySOCEn_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  1,59,0,0, 0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}
/* 油机参数 - 根据SOC停止油机 - SOC */

#define CFG_LINE_254 \
{\
  254,/*LineNum*/\
  43,/* LTitle_043*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  RELATION_DN,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  SETDATA_DGOpenBattCap_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_DGOpenBattCap_NUM%256,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CHAR,/*SignalMode*/\
"%",/*SignalUnit*/\
  SETDATA_DGStopSOC_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  10,/*cFastStep*/\
  100,/*CharUpLimit*/\
  10,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
}


/*DCDC模块参数  设置屏: 限流点*/
#define CFG_LINE_255 \
{\
	255,/*LineNum*/\
  29,/*LTitle_029*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"%",/*SignalUnit*/\
  SETDATA_DCDCCurrentLimit_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  11,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  100,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 
/*DCDC模块参数  设置屏:  输出电压*/
#define CFG_LINE_256 \
{\
	256,/*LineNum*/\
  27,/*LTitle_027*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DCDCOutputVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  28,/*fUpLimit*/\
  24,/*fDnLimit*/\
} 
/*DCDC模块参数  设置屏:  默认输出电压*/
#define CFG_LINE_257 \
{\
	257,/*LineNum*/\
  137,/*LTitle_137*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DCDCDefaultOutputVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  28,/*fUpLimit*/\
  24,/*fDnLimit*/\
} 

/*DCDC模块参数  设置屏: 过压点*/
#define CFG_LINE_258 \
{\
	258,/*LineNum*/\
  136,/*LTitle_136*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_FLOAT,/*SignalMode*/\
"V",/*SignalUnit*/\
  SETDATA_DCDCHVSD_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
 0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  1,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0.1,/*fStep*/\
  1,/*fFastStep*/\
  30,/*fUpLimit*/\
  27,/*fDnLimit*/\
} 
/*用户参数  设置屏: 客户名称*/
#define CFG_LINE_259 \
{\
  259,/*LineNum*/\
  290,/*LTitle_290*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_USER_CHAR,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_UserName_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  140,141,142,143,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  5,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  3,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 

/*用户参数  设置屏: 下电时间*/
#define CFG_LINE_260 \
{\
  260,/*LineNum*/\
  293,/*LTitle_293*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_USER_FLOAT,/*SignalMode*/\
  "m",/*SignalUnit*/\
  SETDATA_Load1CutTime_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  2880,/*fUpLimit*/\
  3,/*fDnLimit*/\
}  

/*用户参数  设置屏: 分流器有无*/
#define CFG_LINE_261 \
{\
  261,/*LineNum*/\
  291,/*LTitle_291*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_USER_CHAR,/*SignalMode*/\
  "",/*SignalUnit*/\
  SETDATA_UserCurrCfg_NUM,/*SignalIndex*/\
  INDIRECT_NORMAL,/*SignalDisplayMode*/\
  5,4, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  13,/*SignalDisplayStart*/\
  2,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  1,/*cStep*/\
  1,/*cFastStep*/\
  1,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 

/*用户参数  设置屏: 分流器电流*/
#define CFG_LINE_262 \
{\
  262,/*LineNum*/\
  89,/*LTitle_089*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_USER_FLOAT,/*SignalMode*/\
  "A",/*SignalUnit*/\
  SETDATA_DCEM3ShuntCurr_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0, 0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  4,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  10,/*fFastStep*/\
  2000,/*fUpLimit*/\
  1,/*fDnLimit*/\
} 

/*用户参数  设置屏: 分流器电压*/
#define CFG_LINE_263 \
{\
  263,/*LineNum*/\
  90,/*LTitle_090*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_USER_FLOAT,/*SignalMode*/\
  "mV",/*SignalUnit*/\
  SETDATA_DCEM3ShuntVolt_NUM,/*SignalIndex*/\
  DIRECT_NORMAL,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  10,/*SignalDisplayStart*/\
  3,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  1,/*fStep*/\
  1,/*fFastStep*/\
  500,/*fUpLimit*/\
  1,/*fDnLimit*/\
}  

/*电池参数--基本参数 设置屏:  分流器系数(标题)*/
#define CFG_LINE_264 \
{\
  264,/*LineNum*/\
  88,/*LTitle_088*/\
  0,/*TitleDisplayStart*/\
  DIRECT_NORMAL,/*TitleDisplayMode*/\
  0,/*PasswordScreen*/\
  0,/*PasswordLevel*/\
  0,/*ConfirmScreen*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
  0,/*fFloatDisplayValue*/\
  0,/*SignalMode*/\
"",/*SignalUnit*/\
  0,/*SignalIndex*/\
  0,/*SignalDisplayMode*/\
  0,0,0,0,0,0,0,0,0,0,/*SignalDisplayAlias*/\
  0,/*SignalDisplayStart*/\
  0,/*SignalDisplayLen*/\
  0,/*SignalDisplayDecimal*/\
  0,/*cStep*/\
  0,/*cFastStep*/\
  0,/*CharUpLimit*/\
  0,/*CharDnLimit*/\
  0,/*fStep*/\
  0,/*fFastStep*/\
  0,/*fUpLimit*/\
  0,/*fDnLimit*/\
} 


#define CFG_LINE_ALL   { \
CFG_LINE_000, CFG_LINE_001, CFG_LINE_002, CFG_LINE_003, CFG_LINE_004, CFG_LINE_005, CFG_LINE_006, CFG_LINE_007,CFG_LINE_008, CFG_LINE_009,\
CFG_LINE_010, CFG_LINE_011, CFG_LINE_012, CFG_LINE_013, CFG_LINE_014, CFG_LINE_015, CFG_LINE_016, CFG_LINE_017,CFG_LINE_018, CFG_LINE_019,\
CFG_LINE_020, CFG_LINE_021, CFG_LINE_022, CFG_LINE_023, CFG_LINE_024, CFG_LINE_025, CFG_LINE_026, CFG_LINE_027,CFG_LINE_028, CFG_LINE_029,\
CFG_LINE_030, CFG_LINE_031, CFG_LINE_032, CFG_LINE_033, CFG_LINE_034, CFG_LINE_035, CFG_LINE_036, CFG_LINE_037,CFG_LINE_038, CFG_LINE_039,\
CFG_LINE_040, CFG_LINE_041, CFG_LINE_042, CFG_LINE_043, CFG_LINE_044, CFG_LINE_045, CFG_LINE_046, CFG_LINE_047,CFG_LINE_048, CFG_LINE_049,\
CFG_LINE_050, CFG_LINE_051, CFG_LINE_052, CFG_LINE_053, CFG_LINE_054, CFG_LINE_055, CFG_LINE_056, CFG_LINE_057,CFG_LINE_058, CFG_LINE_059,\
CFG_LINE_060, CFG_LINE_061, CFG_LINE_062, CFG_LINE_063, CFG_LINE_064, CFG_LINE_065, CFG_LINE_066, CFG_LINE_067,CFG_LINE_068, CFG_LINE_069,\
CFG_LINE_070, CFG_LINE_071, CFG_LINE_072, CFG_LINE_073, CFG_LINE_074, CFG_LINE_075, CFG_LINE_076, CFG_LINE_077,CFG_LINE_078, CFG_LINE_079,\
CFG_LINE_080, CFG_LINE_081, CFG_LINE_082, CFG_LINE_083, CFG_LINE_084, CFG_LINE_085, CFG_LINE_086, CFG_LINE_087,CFG_LINE_088, CFG_LINE_089,\
CFG_LINE_090, CFG_LINE_091, CFG_LINE_092, CFG_LINE_093, CFG_LINE_094, CFG_LINE_095, CFG_LINE_096, CFG_LINE_097,CFG_LINE_098, CFG_LINE_099,\
CFG_LINE_100, CFG_LINE_101, CFG_LINE_102, CFG_LINE_103, CFG_LINE_104, CFG_LINE_105, CFG_LINE_106, CFG_LINE_107,CFG_LINE_108, CFG_LINE_109,\
CFG_LINE_110, CFG_LINE_111, CFG_LINE_112, CFG_LINE_113, CFG_LINE_114, CFG_LINE_115, CFG_LINE_116, CFG_LINE_117,CFG_LINE_118, CFG_LINE_119,\
CFG_LINE_120, CFG_LINE_121, CFG_LINE_122, CFG_LINE_123, CFG_LINE_124, CFG_LINE_125, CFG_LINE_126, CFG_LINE_127,CFG_LINE_128, CFG_LINE_129,\
CFG_LINE_130, CFG_LINE_131, CFG_LINE_132, CFG_LINE_133, CFG_LINE_134, CFG_LINE_135, CFG_LINE_136, CFG_LINE_137,CFG_LINE_138, CFG_LINE_139,\
CFG_LINE_140, CFG_LINE_141, CFG_LINE_142, CFG_LINE_143, CFG_LINE_144, CFG_LINE_145, CFG_LINE_146, CFG_LINE_147,CFG_LINE_148, CFG_LINE_149,\
CFG_LINE_150, CFG_LINE_151, CFG_LINE_152, CFG_LINE_153, CFG_LINE_154, CFG_LINE_155, CFG_LINE_156, CFG_LINE_157,CFG_LINE_158, CFG_LINE_159,\
CFG_LINE_160, CFG_LINE_161, CFG_LINE_162, CFG_LINE_163, CFG_LINE_164, CFG_LINE_165, CFG_LINE_166, CFG_LINE_167,CFG_LINE_168, CFG_LINE_169,\
CFG_LINE_170, CFG_LINE_171, CFG_LINE_172, CFG_LINE_173, CFG_LINE_174, CFG_LINE_175, CFG_LINE_176, CFG_LINE_177,CFG_LINE_178, CFG_LINE_179,\
CFG_LINE_180, CFG_LINE_181, CFG_LINE_182, CFG_LINE_183, CFG_LINE_184, CFG_LINE_185, CFG_LINE_186, CFG_LINE_187,CFG_LINE_188, CFG_LINE_189,\
CFG_LINE_190, CFG_LINE_191, CFG_LINE_192, CFG_LINE_193, CFG_LINE_194, CFG_LINE_195, CFG_LINE_196, CFG_LINE_197,CFG_LINE_198, CFG_LINE_199,\
CFG_LINE_200, CFG_LINE_201, CFG_LINE_202, CFG_LINE_203, CFG_LINE_204, CFG_LINE_205, CFG_LINE_206, CFG_LINE_207,CFG_LINE_208, CFG_LINE_209,\
CFG_LINE_210, CFG_LINE_211, CFG_LINE_212, CFG_LINE_213, CFG_LINE_214, CFG_LINE_215, CFG_LINE_216, CFG_LINE_217,CFG_LINE_218, CFG_LINE_219,\
CFG_LINE_220, CFG_LINE_221, CFG_LINE_222, CFG_LINE_223, CFG_LINE_224, CFG_LINE_225, CFG_LINE_226, CFG_LINE_227,CFG_LINE_228, CFG_LINE_229,\
CFG_LINE_230, CFG_LINE_231, CFG_LINE_232, CFG_LINE_233, CFG_LINE_234, CFG_LINE_235, CFG_LINE_236, CFG_LINE_237, CFG_LINE_238, CFG_LINE_239,\
CFG_LINE_240, CFG_LINE_241, CFG_LINE_242, CFG_LINE_243, CFG_LINE_244, CFG_LINE_245, CFG_LINE_246, CFG_LINE_247, CFG_LINE_248, CFG_LINE_249,\
CFG_LINE_250, CFG_LINE_251, CFG_LINE_252, CFG_LINE_253, CFG_LINE_254, CFG_LINE_255, CFG_LINE_256, CFG_LINE_257, CFG_LINE_258, CFG_LINE_259,\
CFG_LINE_260, CFG_LINE_261, CFG_LINE_262, CFG_LINE_263, CFG_LINE_264,\
}







/*
typedef struct tagScreenCfg
{
  UCHAR  ScreenNum;
  UCHAR  Mode;
  UCHAR  LineNum;
  UCHAR  PasswordScreen;
  UCHAR  LineIndex[MAX_SCREENLINE_NUM];
  UCHAR  EntIndex[MAX_SCREENLINE_NUM];
  UCHAR  PasswordLevel;
  UCHAR  LockHeader;
  //UCHAR  Up;
  //UCHAR  Down;
  UCHAR  Esc;
  UCHAR  Ent;
  UCHAR  RelationorFloat;//用于浮点数设置的关联和浮点数显示浮动与否:0-无关联无浮动，1－关联上限，2－关联下限，3－关联上下限
  UCHAR  cFloatDisplayValue;//用于显示浮动
  float  fFloatDisplayValue;//用于显示浮动
  UCHAR  RelationUporFloatHigh;//用于浮点数设置的上限关联配置浮点数序号，浮动关联的配置浮点数序号和字节型序号高位
  UCHAR  RelationDnorFloatLow;//用于浮点数设置的下限关联配置浮点数序号，浮动关联的配置字节型序号低位
  UCHAR  temp1;
  UCHAR  temp2;
}ScreenCfg_TypeDef;//数据长度68
*/
/* 目前共有37个屏对象 */

/* 空白屏 */
#define SCREEN_000  \
{\
  0,/*ScreenNum*/\
  ZERO_SCREEN,/*Mode*/\
  0,/*LineNum*/\
  0,/*PasswordScreen*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  MAINSCREEN,/*Up*/\
  MAINSCREEN,/*Down*/\
  MAINSCREEN,/*Esc*/\
  MAINSCREEN,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/* 状态信息类--屏*/

/*系统运行屏-等待屏*/
#define SCREEN_001  \
{\
  1,/*ScreenNum*/\
  INFO_Zero_SCREEN ,/*Mode*/\
  8,/*LineNum*/\
  0,/*PasswordScreen*/\
  76,2,3,1,5,4,6,7,8,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  31,/*Up*/\
  31,/*Down*/\
  3,/*Esc*/\
  8,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*系统信息屏*/
#define SCREEN_002  \
{\
  2,/*ScreenNum*/\
  INFO_Sys_SCREEN,/*Mode*/\
  16,/*LineNum*/\
  0,/*PasswordScreen*/\
  30,53,54,55,56,57,58,61,59,60,14,15,16,20,21,22,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*监控信息屏*/
#define SCREEN_003  \
{\
  3,/*ScreenNum*/\
  INFO_Controller_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  73,77,74,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  1,/*Up*/\
  1,/*Down*/\
  1,/*Esc*/\
  1,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*模块信息屏*/
#define SCREEN_004  \
{\
  4,/*ScreenNum*/\
  INFO_Rect_SCREEN,/*Mode*/\
  8,/*LineNum*/\
  0,/*PasswordScreen*/\
  31,32,33,34,35,36,37,38,39,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*当前告警屏*/
#define SCREEN_005  \
{\
  5,/*ScreenNum*/\
  INFO_ActiveAlarm_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  41,42,43,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  FLOAT_EQ_ACTIVEALARM_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*历史告警屏*/
#define SCREEN_006  \
{\
  6,/*ScreenNum*/\
  INFO_HistAlarm_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  45,46,47,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  FLOAT_EQ_HISTORYALARM_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*BMS信息屏*/
#define SCREEN_007  \
{\
  7,/*ScreenNum*/\
  INFO_BMS_SCREEN,/*Mode*/\
  16,/*LineNum*/\
  0,/*PasswordScreen*/\
  10,11,12,13,17,18,19,23,24,25,26,27,28,29,51,52,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0 ,/* RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 


/*菜单类--屏*/

/*主菜单屏*/
#define SCREEN_008  \
{\
  8,/*ScreenNum*/\
  MENU_Main_SCREEN,/*Mode*/\
  6,/*LineNum*/\
  0,/*PasswordScreen*/\
  28,2,5,8,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,7,4,2,51,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  1,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 
/*运行信息-子菜单屏*/
#define SCREEN_009  \
{\
  9,/*ScreenNum*/\
  MENU_Status_SCREEN,/*Mode*/\
  14,/*LineNum*/\
  0,/*PasswordScreen*/\
  29,6,7,5,51,40,41,8,47,49,50,53,55,62,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0, 5,6,4,46,27,13,7,42,44,45,48,51,54,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 
/*参数设置-子菜单屏*/
#define SCREEN_010  \
{\
  10,/*ScreenNum*/\
  MENU_Setting_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  30,15,16,14,0,0,9,10,11,12,13,52,39,54,48,55,56,0,0,0,0,0,0,14,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,28,29,50,0,0,11,12,22,23,24,47,26,49,43,50,53,0,0,0,0,0,0,13,/*EntIndex*/\
  3,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 
 /*告警参数-子菜单屏*/
#define SCREEN_011  \
{\
  11,/*ScreenNum*/\
  MENU_Alarm_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  31,17,18,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,14,15,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 
/*电池参数-子菜单屏*/
#define SCREEN_012  \
{\
  12,/*ScreenNum*/\
  MENU_Batt_SCREEN,/*Mode*/\
  7,/*LineNum*/\
  0,/*PasswordScreen*/\
  32,20,21,22,23,24,46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,17,18,19,20,21,41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 
 /*电池记录-子菜单屏*/
#define SCREEN_013  \
{\
  13,/*ScreenNum*/\
  MENU_BattRecord_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  45,42,43,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,25,39,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 



/*参数设置类--屏*/

/*告警级别 - 设置屏*/
#define SCREEN_014  \
{\
  14,/*ScreenNum*/\
  CFG_AlarmLevel_SCREEN ,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  11,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 
/*告警设置 - 设置屏*/
#define SCREEN_015  \
{\
  15,/*ScreenNum*/\
  CFG_AlarmSettings_SCREEN,/*Mode*/\
  3,/*LineNum*/\
  0,/*PasswordScreen*/\
  5,6,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  11,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*数字量输入设置 - 设置屏*/
#define SCREEN_016  \
{\
  16,/*ScreenNum*/\
  CFG_DISettings_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  11,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*电池参数-基本参数设置 - 设置屏*/
#define SCREEN_017  \
{\
  17,/*ScreenNum*/\
  CFG_BattBasic_SCREEN,/*Mode*/\
  10,/*LineNum*/\
  0,/*PasswordScreen*/\
  14,15,16,17,18,19,20,21,201,202,0,0,0,0,0,0,0,0,0,0,0,0,0,13,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  12,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*电池参数-下电保护设置 - 设置屏*/
#define SCREEN_018  \
{\
  18,/*ScreenNum*/\
  CFG_BattLVD_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  23,24,26,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  12,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*电池参数-充电管理设置 - 设置屏*/
#define SCREEN_019  \
{\
  19,/*ScreenNum*/\
  CFG_BattCharge_SCREEN,/*Mode*/\
  24,/*17,*//*LineNum*/\
  0,/*PasswordScreen*/\
  31,236,237,32,238,239,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,208,209,210,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  12,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*电池参数-电池测试 - 设置屏*/
#define SCREEN_020  \
{\
  20,/*ScreenNum*/\
  CFG_BattTest_SCREEN,/*Mode*/\
  18,/*LineNum*/\
  0,/*PasswordScreen*/\
  53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,0,0,0,0,0,52,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  12,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*电池参数-温补设置 - 设置屏*/
#define SCREEN_021  \
{\
  21,/*ScreenNum*/\
  CFG_BattTempComp_SCREEN,/*Mode*/\
  8,/*LineNum*/\
  0,/*PasswordScreen*/\
  72,73,95,96,200,106,107,108,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,71,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  12,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*交流参数  - 设置屏*/
#define SCREEN_022  \
{\
  22,/*ScreenNum*/\
  CFG_AC_SCREEN,/*Mode*/\
  6,/*LineNum*/\
  0,/*PasswordScreen*/\
  75,76,77,78,79,229,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,74,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*直流参数  - 设置屏*/
#define SCREEN_023  \
{\
  23,/*ScreenNum*/\
  CFG_DC_SCREEN,/*Mode*/\
  10,/*LineNum*/\
  0,/*PasswordScreen*/\
  81,82,83,84,104,105,192,19,193,194,0,0,0,0,0,0,0,0,0,0,0,0,0,80,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*模块参数  - 设置屏*/
#define SCREEN_024  \
{\
  24,/*ScreenNum*/\
  CFG_Rect_SCREEN,/*Mode*/\
  9,/*LineNum*/\
  0,/*PasswordScreen*/\
  86,173,87,88,89,90,91,92,93,0,0,0,0,0,0,0,0,0,0,0,0,0,0,85,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*本月电池记录*/
#define SCREEN_025  \
{\
  25,/*ScreenNum*/\
  0,/*Mode*/\
  10,/*LineNum*/\
  0,/*PasswordScreen*/\
  56,57,58,59,60,61,62,63,64,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  13,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*油机参数 - 油机参数  - 设置屏*/
#define SCREEN_026  \
{\
  26,/*ScreenNum*/\
  CFG_DG_SCREEN,/*Mode*/\
  7,/*LineNum*/\
  0,/*PasswordScreen*/\
  50,51,97,98,118,179,119,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_ACinCtl_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_ACinCtl_NUM%256,/*RelationDnorFloatLow*/\
}
/*油机信息 - 油机模式 */
#define SCREEN_027  \
{\
  27,/*ScreenNum*/\
  0,/*Mode*/\
  8,/*LineNum*/\
  0,/*PasswordScreen*/\
  54,55,139,140,141,142,143,144,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*系统参数  - 设置屏*/
#define SCREEN_028  \
{\
  28,/*ScreenNum*/\
  CFG_SysPara_SCREEN,/*Mode*/\
  7,/*LineNum*/\
  0,/*PasswordScreen*/\
  133,134,135,136,139,141,177,142,143,137,138,234,235,249,140,198,199,178,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*通讯参数  - 设置屏*/
#define SCREEN_029  \
{\
  29,/*ScreenNum*/\
  CFG_CommPara_SCREEN,/*Mode*/\
  12,/*LineNum*/\
  0,/*PasswordScreen*/\
  145,146,148,149,150,151,152,153,191,154,155,156,0,0,0,0,0,0,0,0,0,0,0,144,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*节能参数  - 设置屏*/
#define SCREEN_030  \
{\
  30,/*ScreenNum*/\
  CFG_Energy_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  158,159,160,161,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,157,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  4,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*快速设置  - 设置屏*/
#define SCREEN_031  \
{\
  31,/*ScreenNum*/\
  CFG_FastSet_SCREEN,/*Mode*/\
  3,/*LineNum*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  162,14,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,162,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  1,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/*控制输出  - 设置屏*/
#define SCREEN_032  \
{\
  32,/*ScreenNum*/\
  CFG_CtlOut_SCREEN,/*Mode*/\
  2,/*LineNum*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  14,195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  3,/*PasswordLevel*/\
  0,/*LockHeader*/\
  33,/*Up*/\
  33,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/* 控制输出2 -- 设置屏 */
#define SCREEN_033  \
{\
  33,/*ScreenNum*/\
  CFG_CtlOut2_SCREEN,/*Mode*/\
  13,/*LineNum*/\
  PASSWORDSCREEN,/*PasswordScreen*/\
  166,167,168,169,170,171,172,196,197,245,246,247,248,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  3,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  FLOAT_NEQ_SETDATA_CHAR,/*RelationorFloat*/\
  mMANUAL,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_CtlMode_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_CtlMode_NUM%256,/*RelationDnorFloatLow*/\
}


/********其它屏***********/
/* 密码输入屏  */
#define SCREEN_034  \
{\
  34,/*ScreenNum*/\
  PASSWORD_SCREEN,/*Mode*/\
  2,/*LineNum*/\
  0,/*PasswordScreen*/\
  174,175,176,177,178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  0,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*确认提示屏*/
#define SCREEN_035  \
{\
  35,/*ScreenNum*/\
  CONFIRM_SCREEN,/*Mode*/\
  3,/*LineNum*/\
  0,/*PasswordScreen*/\
  36,34,35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  MAINSCREEN,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}


/*升级提示屏*/
#define SCREEN_036  \
{\
  36,/*ScreenNum*/\
  UPDATE_POP_SCREEN,/*Mode*/\
  3,/*LineNum*/\
  0,/*PasswordScreen*/\
  85,86,87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  MAINSCREEN,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/* 槽位设置 提示屏*/
#define SCREEN_037  \
{\
  37,/*ScreenNum*/\
  SHELFCUE_POP_SCREEN,/*Mode*/\
  1,/*LineNum*/\
  0,/*PasswordScreen*/\
  38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  MAINSCREEN,/*Esc*/\
  MAINSCREEN,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/* 不能操作提示屏-( 输出控制时提示)*/
#define SCREEN_038  \
{\
  38,/*ScreenNum*/\
  CONFIRM_SCREEN,/*NOACTION_SCREEN  Mode*/\
  1,/*LineNum*/\
  0,/*PasswordScreen*/\
  37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  MAINSCREEN,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/* 三月电池记录 */
#define SCREEN_039  \
{\
  39,/*ScreenNum*/\
  0,/*Mode*/\
  10,/*LineNum*/\
  0,/*PasswordScreen*/\
 56,68,58,69,60,70,62,71,64,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  13,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*模块电流 */
#define SCREEN_040  \
{\
  40,/*ScreenNum*/\
  0,/*Mode*/\
  2,/*LineNum*/\
  0,/*PasswordScreen*/\
 66,67,58,69,60,70,62,71,64,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  13,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

#define SCREEN_041  \
{\
  41,/*ScreenNum*/\
  CFG_SCREEN,/*Mode*/\
  5,/*LineNum*/\
  0,/*PasswordScreen*/\
  207,203,204,205,206,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  12,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

#define SCREEN_042  \
{\
  42,/*ScreenNum*/\
  0,/*Mode*/\
  8,/*LineNum*/\
  0,/*PasswordScreen*/\
  90,91,92,93,94,95,96,115,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

#define SCREEN_043  \
{\
  43,/*ScreenNum*/\
  CFG_DC_SCREEN,/*Mode*/\
  20,/*LineNum*/\
  0,/*PasswordScreen*/\
  211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,120,122,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

#define SCREEN_044  \
{\
  44,/*ScreenNum*/\
  0,/*Mode*/\
  14,/*LineNum*/\
  0,/*PasswordScreen*/\
  97,98,99,107,103,100,108,104,101,109,105,102,110,106,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  FLOAT_EQ_DCEM_NUM,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*DCEM 日电量*/
#define SCREEN_045  \
{\
  45,/*ScreenNum*/\
  CFG_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  230,231,232,233,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*运行信息-MPPT模块信息  信息屏*/
#define SCREEN_046 \
{\
  46,/*ScreenNum*/\
  INFO_MPPT_SCREEN,/*Mode*/\
  9,/*LineNum*/\
  0,/*PasswordScreen*/\
  120,121,122,123,124,125,126,127,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  FLOAT_EQ_MPPT_NUM, /*,RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*参数设置-MPPT模块参数设置  设置屏*/
#define SCREEN_047 \
{\
  47,/*ScreenNum*/\
  CFG_Mppt_SCREEN,/*Mode*/\
  3,/*LineNum*/\
  0,/*PasswordScreen*/\
  241,242,250,244,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,243,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

#if 1
/* 统计信息屏  */
#define SCREEN_048  \
{\
  48,/*ScreenNum*/\
  INFO_STAT_SCREEN,/*Mode*/\
  8,/*LineNum*/\
  0,/*PasswordScreen*/\
  129,130,131,132,133,134,135,136,137,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
#endif

/*油机参数 - 油机控制参数  - 设置屏*/
#define SCREEN_049  \
{\
  49,/*ScreenNum*/\
  CFG_DG2_SCREEN,/*Mode*/\
  17,/*LineNum*/\
  0,/*PasswordScreen*/\
  99,101,103,109,110,111,102,112,113,251,252,253,254,114,115,116,117,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  1,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  SETDATA_ACinCtl_NUM/256,/*RelationUporFloatHigh*/\
  SETDATA_ACinCtl_NUM%256,/*RelationDnorFloatLow*/\
}

/*参数设置- 测试参数  - 设置屏*/
#define SCREEN_050  \
{\
  50,/*ScreenNum*/\
  CFG_TEST_SCREEN,/*Mode*/\
  6,/*LineNum*/\
  0,/*PasswordScreen*/\
  50,54,55,51,52,53,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}
/* 绝缘板信息*/
#define SCREEN_051 \
{\
  51,/*ScreenNum*/\
  INFO_INSUL_SCREEN,/*Mode*/\
  4,/*LineNum*/\
  0,/*PasswordScreen*/\
  62,63,64,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  8,/*Esc*/\
  0,/*Ent*/\
  0, /*,RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}


/*参数设置- 用户参数  - 设置屏*/
#define SCREEN_052  \
{\
  52,/*ScreenNum*/\
  CFG_USER_SCREEN,/*Mode*/\
  6,/*LineNum*/\
  0,/*PasswordScreen*/\
  259,260,261,264,262,263,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  53,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
}

/*用户参数-子菜单屏*/
#define SCREEN_053  \
{\
  53,/*ScreenNum*/\
  MENU_USER_SCREEN,/*Mode*/\
  5,/*LineNum*/\
  0,/*PasswordScreen*/\
  57,58,59,60,61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,52,52,52,52,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  1,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  10,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 

/*用户信息-子菜单屏*/
#define SCREEN_054  \
{\
  54,/*ScreenNum*/\
  INFO_USER_SCREEN,/*Mode*/\
  6,/*LineNum*/\
  0,/*PasswordScreen*/\
  154,155,156,157,158,159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*LineIndex[MAX_SCREENLINE_NUM]*/\
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/*EntIndex*/\
  0,/*PasswordLevel*/\
  0,/*LockHeader*/\
  0,/*Up*/\
  0,/*Down*/\
  9,/*Esc*/\
  0,/*Ent*/\
  0,/*RelationorFloat*/\
  0,/*cFloatDisplayValue*/\
  0,/*fFloatDisplayValue*/\
  0,/*RelationUporFloatHigh*/\
  0,/*RelationDnorFloatLow*/\
} 

#define  SCREEN_ALL { \
SCREEN_000, SCREEN_001, SCREEN_002, SCREEN_003, SCREEN_004, SCREEN_005, SCREEN_006, SCREEN_007,SCREEN_008, SCREEN_009,\
SCREEN_010, SCREEN_011, SCREEN_012, SCREEN_013, SCREEN_014, SCREEN_015, SCREEN_016, SCREEN_017,SCREEN_018, SCREEN_019,\
SCREEN_020, SCREEN_021, SCREEN_022, SCREEN_023, SCREEN_024, SCREEN_025, SCREEN_026, SCREEN_027,SCREEN_028, SCREEN_029,\
SCREEN_030, SCREEN_031, SCREEN_032, SCREEN_033, SCREEN_034, SCREEN_035, SCREEN_036, SCREEN_037,SCREEN_038, SCREEN_039,\
SCREEN_040, SCREEN_041, SCREEN_042, SCREEN_043, SCREEN_044, SCREEN_045, SCREEN_046, SCREEN_047,SCREEN_048, SCREEN_049,\
SCREEN_050, SCREEN_051, SCREEN_052, SCREEN_053, SCREEN_054,\
}
