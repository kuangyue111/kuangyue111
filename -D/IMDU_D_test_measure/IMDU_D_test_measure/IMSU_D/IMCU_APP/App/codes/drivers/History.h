
/************************************************/
/*历史事件定义                                  */
/************************************************/
#define HISTORY_EVENT_RESET                   10
#define HISTORY_EVENT_POWER_ON                11
#define HISTORY_EVENT_SETDATA_RESET           12
#define HISTORY_EVENT_CTL_SETDATA_RESET       13
#define HISTORY_EVENT_PASSWORDINI             14
#define HISTORY_EVENT_CTL_EC                  15
#define HISTORY_EVENT_CTL_FC                  16
#define HISTORY_EVENT_CTL_LVD1_OFF            17
#define HISTORY_EVENT_CTL_LVD1_ON             18
#define HISTORY_EVENT_CTL_LVD2_OFF            19
#define HISTORY_EVENT_CTL_LVD2_ON             20
#define HISTORY_EVENT_CTL_RECT_CURR_LIMIT     21
#define HISTORY_EVENT_CTL_RECT_DC_OFF         23
#define HISTORY_EVENT_CTL_RECT_DC_ON          24
#define HISTORY_EVENT_CTL_TEST_START          25
#define HISTORY_EVENT_CTL_TEST_END            26

#define HISTORY_EVENT_SETDATA_SETCHAR		      27
#define HISTORY_EVENT_SETDATA_SETFLOAT	      28

#define HISTORY_EVENT_CTL_EC_RM	              31
#define HISTORY_EVENT_CTL_FC_RM	              32
#define HISTORY_EVENT_CTL_TEST_START_RM	      33
#define HISTORY_EVENT_CTL_TEST_END_RM	        34
#define HISTORY_EVENT_CTL_RECT_CURR_LIMIT_RM  35
#define HISTORY_EVENT_CTL_RECT_VOLTAGE_RM     36
#define HISTORY_EVENT_CTL_RECT_DC_ON_RM       37
#define HISTORY_EVENT_CTL_RECT_DC_OFF_RM      38

#define HISTORY_EVENT_SETDATA_SETCHAR_RM      39
#define HISTORY_EVENT_SETDATA_SETFLOAT_RM     40

#define HISTORY_EVENT_CTL_EC_CAP_OR_CURR      50
#define HISTORY_EVENT_CTL_EC_TIME             51
#define HISTORY_EVENT_CTL_FC_STABLECURRECTIME 52
#define HISTORY_EVENT_CTL_FC_ECPROTECTTIME    53
#define HISTORY_EVENT_CTL_FC_ACSTOP           54
#define HISTORY_EVENT_CTL_FC_BATTBREAK        55
#define HISTORY_EVENT_CTL_FC_NOBATT           56
#define HISTORY_EVENT_CTL_FC_CANCOMMBREAK     57
#define HISTORY_EVENT_CTL_FC_PROTECT          58
#define HISTORY_EVENT_CTL_PRE_LIMIT           59
#define HISTORY_EVENT_MANUAL_TO_AUTO          60
#define HISTORY_EVENT_CTL_LVD1_VOLT           65//负载下电(电压方式)
#define HISTORY_EVENT_CTL_LVD1_TIME           66//负载下电(时间方式)
#define HISTORY_EVENT_CTL_LVD2_VOLT           67//电池下电(电压方式)
#define HISTORY_EVENT_CTL_LVD2_TIME           68//电池下电(时间方式)
#define HISTORY_EVENT_LVD_RECONNECT           69
#define HISTORY_EVENT_TEST_END_VOLT           70
#define HISTORY_EVENT_TEST_END_TIME           71
#define HISTORY_EVENT_TEST_END_ALARM          72
#define HISTORY_EVENT_TEST_END_PROTECT        73
#define HISTORY_EVENT_TEST_END_CAP            74
#define HISTORY_EVENT_SHORT_TEST_START        75
#define HISTORY_EVENT_SHORT_TEST_END          76
#define HISTORY_EVENT_TIME_TEST_START         77
#define HISTORY_EVENT_CTL_FC_HIGH_TEMP        78
#define HISTORY_EVENT_CTL_FC_TIMEECTIME       79
#define HISTORY_EVENT_LVD1_EMERGENCYSHUTDOWN  80
#define HISTORY_EVENT_LVD2_EMERGENCYSHUTDOWN  81
#define HISTORY_EVENT_CTL_RECT_AC_ON_RM       82
#define HISTORY_EVENT_CTL_RECT_AC_OFF_RM      83
#define HISTORY_EVENT_CUT_BATT_HIGH_TEMP      84
#define HISTORY_EVENT_CTL_RECT_RESET_RM       85
#define HISTORY_EVENT_CTL_RECT_RESET          86
#define HISTORY_EVENT_CTL_RECT_AC_ON          87
#define HISTORY_EVENT_CTL_RECT_AC_OFF         88

#define HISTORY_EVENT_CLEAR_HISTORYALARM      89//手动清除历史告警
#define HISTORY_EVENT_CLEAR_BTALARM_MANUAL    90//手动清除电池测试告警
#define HISTORY_EVENT_CLEAR_STALARM_MANUAL    91//手动清除短测试告警
#define HISTORY_EVENT_CLEAR_RECTLOST_MANUAL   92//手动清除模块丢失告警
#define HISTORY_EVENT_CLEAR_SERVICE_MANUAL    93//手动清除系统维护告警???
#define HISTORY_EVENT_CLEAR_SPFAULT_MANUAL    94//手动消除节能异常告警 
#define HISTORY_EVENT_CLEAR_RECT_COMBREAK     96//手动消除模块通讯中断告警

#define HISTORY_EVENT_SET_DATEANDTIME    		  95
#define HISTORY_EVENT_SET_ALARMLEVELNAME 		  97
#define HISTORY_EVENT_SET_RECTLOCATION	      98
#define HISTORY_EVENT_SETDATA_BARCODE		      99

#define HISTORY_EVENT_CTL_RECT_VOLTAGE        100

//101-114 用于powersplit和主从模式
/*			101:powersplit转均充				*/
/*			102:powersplit转浮充				*/
/*			103:powersplit转测试				*/
/*			104:powersplit停止测试				*/
/*			105:powersplit负载下电				*/
/*			106:powersplit负载上电				*/
/*			107:powersplit电池保护				*/
/*			108:powersplit电池上电				*/
/*			109:主机控制负载下电				*/
/*			110:主机控制负载上电				*/
/*			111:主机控制电池保护				*/
/*			112:主机控制电池上电				*/
/*			113:设置从机后均充转浮充			*/
/*			114:设置从机后停止测试				*/
#define HISTORY_EVENT_POWERSPLIT_FC           102
#define HISTORY_EVENT_POWERSPLIT_STOPTEST     104

#define HISTORY_EVENT_TEST_END_CURR_SMALL     115//电流小于2A电池测试结束
#define HISTORY_EVENT_CTL_FC_ECSOLAROVERVOLT  116//太阳能模式下，手动均充转浮充


#define HISTORY_EVENT_CTL_SMDU_LVD1           120//控制SMDULVD1下电
#define HISTORY_EVENT_SMDU_LVD_RECONNECT      121
#define HISTORY_EVENT_CTL_SMDU_LVD2           122//控制SMDULVD2下电
#define HISTORY_EVENT_CTL_SMDU_LVD_TIME       123//控制SMDULVD1,2下电(时间方式)

#define HISTORY_EVENT_CLEAR_HISALARM_RM       130//遥控清除历史告警
#define HISTORY_EVENT_CLEAR_BTALARM_RM        131//遥控清除电池测试告警
#define HISTORY_EVENT_CLEAR_STALARM_RM        132//遥控清除短测试告警
#define HISTORY_EVENT_CLEAR_RECTLOST_RM       133//遥控清除模块丢失告警
#define HISTORY_EVENT_CLEAR_SERVICE_RM        134//遥控清除系统维护告警
#define HISTORY_EVENT_CLEAR_SPFAULT_RM        135//遥控清除节能异常告警
#define HISTORY_EVENT_CLEAR_STATICTS_RM       136//遥控清除电池充放电统计信息

#define HISTORY_EVENT_Open_DG1_RM       	   137//开启油机1
#define HISTORY_EVENT_Close_DG1_RM       	   138//关闭油机1
#define HISTORY_EVENT_Open_DG2_RM       	   139//开启油机2
#define HISTORY_EVENT_Close_DG2_RM       	   140//关闭油机2
#define HISTORY_EVENT_CLEAR_BattRecord_RM        141//关闭油机2

#define HISTORY_EVENT_CLEAR_MPPTLOST_RM       142//遥控清除MPPT模块丢失告警
#define HISTORY_EVENT_CLEAR_MPPTLOST_MANUAL   143//手动清除MPPT模块丢失告警
#define HISTORY_EVENT_CLEAR_STATISTIC_RM	  144//遥控清除统计数据
#define HISTORY_EVENT_CLEAR_STATISTIC_MANUAL  145//手动清除统计数据
#define HISTORY_EVENT_CLEAR_BATTRECORD_RM     146//遥控清除电池记录信息
#define HISTORY_EVENT_CLEAR_BATTRECORD_MANUAL 147//手动清除电池记录信息
#define HISTORY_EVENT_SHORT_TEST_END_ALARM    172
#define HISTORY_EVENT_SHORT_TEST_END_PROTECT  173
         
#define HISTORY_EVENT_SETTING_FROM_NETWORK   180
#define HISTORY_EVENT_DOWNLOAD_FROM_NETWORK  181
#define HISTORY_EVENT_CTL_FC_MPPTCOMMBREAK    184  //mppt通讯中断
#define HISTORY_EVENT_CTL_FC_ECHIGHTEMP       185  //均充温度过高
#define HISTORY_EVENT_CTL_FC_ACSTOP_NOSOLAR   186  //交流停电且无光照
#define HISTORY_EVENT_CTL_FC_NOSOLAR          187  //无光照

#define HISTORY_EVENT_CTL_MPPT_CURR_LIMIT     190
#define HISTORY_EVENT_CTL_MPPT_DC_OFF         191
#define HISTORY_EVENT_CTL_MPPT_DC_ON          192
#define HISTORY_EVENT_CTL_MPPT_VOLTAGE        193
#define HISTORY_EVENT_CTL_MPPT_RESET_RM       194

#define HISTORY_EVENT_CLEAR_DCDCLOST_MANUAL   200
#define HISTORY_EVENT_CLEAR_DCDCLOST_RM       201

#define HISTORY_EVENT_COM_DOWNLOAD_MODE       252
#define HISTORY_EVENT_NET_DOWNLOAD_MODE       253
#define HISTORY_EVENT_MENU_RESET              254
#define HISTORY_EVENT_SETDATA_DEFAULT         255


#define ALARMTYPE_ALARM                0x0000   
#define ALARMTYPE_ALIAS                 0x0400   
#define ALARMTYPE_ALIAS1                0x0800
#define ALARMTYPE_SWITCHNAME            0x0c00  
/*设置告警级别: 告警类型的显示名称存储相对位置
       相对于ALIAS_ALL 、ALARM_ALL、ANALOGALARM_ALL、*/
// ALARM_ALL[]
 #define   ALARMBLOCK_NAME             		  1
#define   SPD_NAME                            2
#define   DG1OpenFail_NAME                    3
#define   DG2OpenFail_NAME                    4

#define   LVD1BACK_NAME			   8
#define   LVD2BACK_NAME  		   9
#define     SELFTEST_NAME                11 
#define     MANUAL_NAME                  12
#define    BATTEC_NAME                    13
#define     BATTTEST_NAME               14
#define     BATTDISCHARGE_NAME    15
#define     CURRUNBALANCE_NAME   16
#define     VOLTUNBALANCE_NAME   17
#define     SHORTTESTFAULT_NAME  18
#define     TESTFAULT_NAME             19
#define     LVD1_NAME_NAME            20
#define     LVD2_NAME_NAME            21
#define     ACSTOP_NAME                  22
#define     RECTOVERLOAD_NAME      23
#define     RECTLOST_NAME              25
#define     SERVICE_NAME                 26
#define     MULTIRECTALARM_NAME  27
#define     ANYRECTCOMMBREAK_NAME 28
#define     ANYRECTACSTOP_NAME     29
#define     ANYRECTOVERTEMP_NAME 30
#define     ANYRECTFAULT_NAME        31
#define     ANYRECTPROTECT_NAME    32
#define     ANYRECTFANFAULT_NAME   33
#define     ANYRECTPOWERLIMIT_NAME 34
#define     ANYRECTCURRUNBALANCE_NAME  35
#define     ANYRECTHVSD_NAME        36    
#define    SAVEPOWER_NAME            24
#define    SAVEPOWERFAULT_NAME  37
#define		DCEM1COMMFAIL_NAME		46
#define		DCEM2COMMFAIL_NAME		47
//#define    SOLARCOMMBREAK_NAME  39
//#define    SolarMajorAlarm_NAME   	40	
//#define    SolarObservAlarm_NAME	41
#define   MPPTLOST_NAME					48
#define   MPPTCOMMBREAK_NAME			49
#define   MPPTOVERTEMP_NAME				50
#define   MPPTFAULT_NAME				51
#define   MPPTPROTECT_NAME				52
#define   MPPTFANFAULT_NAME				53
#define   MPPTPOWERLIMIT_NAME			54
#define   MPPTCURRUNBALANCE_NAME		55
#define   MPPTINPUTVOLTHIGH_NAME		56
#define   MPPTPVFAULT_NAME				57
#define   DGSTARTFAIL_NAME              41

#define   DCDCLOST_NAME					58
#define   DCDCCOMMBREAK_NAME			59
#define   DCDCOVERTEMP_NAME				60
#define   DCDCFAULT_NAME				61
#define   DCDCROTECT_NAME				62
#define   DCDCFANFAULT_NAME				63
#define   DCDCHVSD_NAME			64
#define   MULTI_DCDC_NAME			65
#define     LLVD1_NAME_NAME            66
#define     LLVD2_NAME_NAME            67
#define     LLVD3_NAME_NAME            68
#define     LLVD4_NAME_NAME            69

// ANALOGALARM_ALL[]
#define DCVOLT_UNDER_NAME              0
#define DCVOLT_LOW_NAME                  1
#define DCVOLT_HIGH_NAME                 2
#define DCVOLT_OVER_NAME                 3
#define BDCVOLT_UNDER_NAME            6
#define BDCVOLT_LOW_NAME                7
#define BDCVOLT_OVER_NAME               8
// ALIAS_ALL[]
#define LFUSE_NAME                      	 81
#define BFUSE_NAME                        82
#define BCURR_OVER_NAME              83  
#define TEMP_LOW_NAME                 84
#define TEMP_OVER_NAME                85
#define TEMP_HIGH_NAME                86
#define ACVOLT_UNDER_NAME          87
#define ACVOLT_LOW_NAME              88
#define ACVOLT_OVER_NAME             89
//#define POWERMAJOR_NAME                 97
//#define POWERMINOR_NAME                 98
#define SPD_ALARM_NAME                105
#define BATTSYMM_NAME				122

#define ALARMTYPE_Level                  0x0000   
#define ALARMTYPE_Level2                0x0400
#define ALARMTYPE_Relay                  0x0000   
#define ALARMTYPE_Relay2                0x0400
#define ALARMTYPE_Level2_Relay2                0x0400
/*设置告警级别: 告警类型的级别、 关联继电器 
    的存储相对位置 (0:低低限 1:低限 2:高限 3: 高高限)--
    (SetData中) AlarmLevel[] AlarmRelay[] AlarmLevel2[] AlarmRelay2[])     */                          
#define DCVOLT_UNDER_TYPE               0
#define DCVOLT_LOW_TYPE                 1
#define DCVOLT_HIGH_TYPE                2
#define DCVOLT_OVER_TYPE                3

//#define BDCVOLT_UNDER_TYPE              4
//#define BDCVOLT_LOW_TYPE                5
//#define BDCVOLT_HIGH_TYPE               6
//#define BDCVOLT_OVER_TYPE               7

#define BCURR_UNDER_TYPE                4
#define BCURR_LOW_TYPE                  5
#define BCURR_HIGH_TYPE                 6
#define BCURR_OVER_TYPE                 7

#define TEMP_UNDER_TYPE                 16
#define TEMP_LOW_TYPE                   17
#define TEMP_HIGH_TYPE                  18
#define TEMP_OVER_TYPE                  19

#define AC_UNDER_TYPE                   28
#define AC_LOW_TYPE                     29
#define AC_HIGH_TYPE                    30
#define AC_OVER_TYPE                    31

/*end*/


/************************************************/
/*历史告警定义                                  */
/************************************************/
//告警表序号							                  /*开关量18,模拟量16,下电2,模块48*9*/
#define ALARMTABLE_ALARMBLOCK           0		/*0:告警阻塞*/
//#define ALARMTABLE_SPD                  1   /*1:SPD告警*/
//#define ALARMTABLE_DIGIT1               2		/*2:DIN1*/
//#define ALARMTABLE_DIGIT2               3		/*3:DIN2*/
//#define ALARMTABLE_DIGIT3               4		/*4:DIN3*/
//#define ALARMTABLE_DIGIT4               5		/*5:DIN4*/
#define ALARMTABLE_DCEM1_COMM_BREAK     4
#define ALARMTABLE_DCEM2_COMM_BREAK     5

#define ALARMTABLE_ANALOG_START         6	  /*6-18:7+6路模拟量(VFC)告警*/
#define ALARMTABLE_DCVOLTAGE            6   /*6:母排电压*/
#define ALARMTABLE_BATTVOLTAGE					7	  /*7:电池电压*/
//#define ALARMTABLE_LOADCURRENT					8	  /*8:电流1(负载电流)*/
#define ALARMTABLE_BATTCURRENT3					8	  /*8:电流1(电池3电流)*/
#define ALARMTABLE_BATTCURRENT1					9	  /*9:电流2(电池1电流)*/
#define ALARMTABLE_BATTCURRENT2					10 	/*10:电流3(电池2电流)*/
#define ALARMTABLE_TEMP1    						11  /*11:温度1*/
#define ALARMTABLE_TEMP2    						12  /*12:温度2*/
#define ALARMTABLE_AC1VA                13    /*20:交流电压*/
#define ALARMTABLE_AC1VB                14    /*21:交流电压*/
#define ALARMTABLE_AC1VC                15    /*22:交流电压*/
#define ALARMTABLE_AC1IA                16    /*23:交流电流*/
#define ALARMTABLE_AC1IB                17    /*24:交流电流*/
#define ALARMTABLE_AC1IC                18    /*25:交流电流*/

#define ALARMTABLE_ANALOG_END						15	


//#define ALARMTABLE_LOADFUSE1            19	/*19:熔丝1*/
//#define ALARMTABLE_LOADFUSE2            20	/*20:熔丝2*/
//#define ALARMTABLE_BATTFUSE1						21	/*21:电池熔丝1*/
//#define ALARMTABLE_BATTFUSE2						22	/*22:电池熔丝2*/
#define ALARMTABLE_LVD1BACK						  23	/*23:LVD1回采*/
#define ALARMTABLE_LVD2BACK 						24	/*24:LVD2回采*/

#define ALARMTABLE_SELFWORK             25	/*25:监控单元工作*/
#define ALARMTABLE_SELFTEST             26	/*26:硬件自检*/
#define ALARMTABLE_MANUAL								27	/*27:手动*/
#define ALARMTABLE_BATTEC								28	/*28:均充*/
#define ALARMTABLE_BATTTEST							29	/*29:测试*/
#define ALARMTABLE_BATTDISCHARGE				30	/*30:放电*//*保留*/
#define ALARMTABLE_CURRUNBALANCE				31	/*31:电流不平衡*/
#define ALARMTABLE_VOLTUNBALANCE				32  /*32:DC电压差别大*/
#define ALARMTABLE_SHORTTESTFAULT				33  /*33:电池短测试故障告警*/
#define ALARMTABLE_TESTFAULT						34	/*34:电池测试故障告警*/
#define ALARMTABLE_LVD1									35	/*35:负载下电*/
#define ALARMTABLE_LVD2									36	/*36:电池保护*/
#define ALARMTABLE_ACSTOP								37	/*37:交流停电*/
#define ALARMTABLE_RECTOVERLOAD					38	/*38:模块输出大告警*/
#define ALARMTABLE_SAVEPOWER           		39	/*39:节能告警*/
#define ALARMTABLE_RECTLOST							40	/*40:模块丢失告警*/

#define ALARMTABLE_SERVICE							41	/*41:模块工作时间超时告警（换风扇）*/

#define ALARMTABLE_MULTIRECTALARM				42	/*42:多模块告警*/

#define ALARMTABLE_ANYRECTCOMMBREAK     43  /*43:任何模块通讯中断*/
#define ALARMTABLE_ANYRECTACSTOP				44	/*44:任何模块交流停电*/
#define ALARMTABLE_ANYRECTOVERTEMP			45	/*45:任何模块过温*/
#define ALARMTABLE_ANYRECTFAULT					46	/*46:任何模块故障*/
#define ALARMTABLE_ANYRECTPROTECT				47	/*47:任何模块保护*/
#define ALARMTABLE_ANYRECTFANFAULT			48	/*48:任何模块风扇故障*/
#define ALARMTABLE_ANYRECTPOWERLIMIT		49	/*49:任何模块限功率*/
#define ALARMTABLE_ANYRECTCURRUNBALANCE	50	/*50:任何模块不均流*/
#define ALARMTABLE_ANYRECTHVSD      		51	/*51:任何模块HVSD**/
                                            /*52-58:保留*/
#define ALARMTABLE_SAVEPOWERFAULT       59

#define ALARMTABLE_RECT_START           60
#define ALARMTABLE_RECTCOMMBREAK				60	/*60-89:30个模块通讯中断*/
#define ALARMTABLE_RECTACSTOP						90	/*90-109:30个模块交流停电*/
#define ALARMTABLE_RECTOVERTEMP					120	/*120-149:30个模块过温*/
#define ALARMTABLE_RECTFAULT						150	/*150-179:30个模块故障*/
#define ALARMTABLE_RECTPROTECT					180	/*180-209:30个模块保护*/
#define ALARMTABLE_RECTFANFAULT					210	/*210-239:30个模块风扇故障*/
#define ALARMTABLE_RECTPOWERLIMIT				240	/*240-269:30个模块限功率*/
#define ALARMTABLE_RECTCURRUNBALANCE		270	/*270-299:30个模块不均流*/
#define ALARMTABLE_RECTHVSD          		300	/*300-329:30个模块HVSD*/
                                            /*330-359:保留*/
#define ALARMTABLE_RECT_END             329
#define ALARMTABLE_SMDU1				360	/*360-390:SMDU1 31个告警量*/
                                            /*360-375:SM1 LF*/
#define ALARMTABLE_SMDU1LVD1BAK         376 /*376:sm1 LVD1 back*/
#define ALARMTABLE_SMDU1LVD2BAK         377 /*377:sm1 LVD2 back*/
#define ALARMTABLE_SMDU1SELF            378 /*378:sm1 自检故障*/
#define ALARMTABLE_SMDU1LVD1            379 /*379:sm1 LVD1*/
#define ALARMTABLE_SMDU1LVD2            380 /*380:sm1 LVD2*/
#define ALARMTABLE_SMDU1BF1             382 /*382-385:sm1 BF*/
#define ALARMTABLE_SMDU1COMMBREAK		391	/*391:SMDU1通讯中断告警*/
#define ALARMTABLE_SMDU2				392	/*392-422:SMDU2 31个告警量*/
                                            /*392-407:SM2 LF*/
#define ALARMTABLE_SMDU2LVD1BAK         408 /*408:sm2 LVD1 back*/
#define ALARMTABLE_SMDU2LVD2BAK         409 /*409:sm2 LVD2 back*/
#define ALARMTABLE_SMDU2SELF            410 /*410:sm2 自检故障*/
#define ALARMTABLE_SMDU2LVD1            411 /*411:sm2 LVD1*/
#define ALARMTABLE_SMDU2LVD2            412 /*412:sm2 LVD2*/
#define ALARMTABLE_SMDU2BF1             414 /*414-417:sm2 BF*/
#define ALARMTABLE_SMDU2COMMBREAK		423	/*423:SMDU2通讯中断告警*/

#define ALARMTABLE_SolarCOMMBREAK		424	/*424:Solar通讯中断告警*/
#define ALARMTABLE_SolarMajorAlarm   	425	/*424:Solar有重要告警*/
#define  ALARMTABLE_SolarObservAlarm	426	/*424:Solar有一般告警*/

#define  ALARMTABLE_DG1OpenFailAlarm	427	/*427:油机1开启失败*/
#define  ALARMTABLE_DG2OpenFailAlarm	428	/*428:油机2开启失败*/

#define ALARMTABLE_LLVD1		429	/*429:负载1下电*/
#define ALARMTABLE_LLVD2		430	/*430:负载2下电*/
#define ALARMTABLE_LLVD3		431	/*431:负载3下电*/
#define ALARMTABLE_LLVD4		432	/*432:负载4下电*/
                                            /*429-479:INVERTER保留*/

#define ALARMTABLE_OB                   480 /*50*/
#define ALARMTABLE_OB_SW_START          (ALARMTABLE_OB + 10)
#define ALARMTABLE_OB_FUSE1             ALARMTABLE_OB_SW_START
#define ALARMTABLE_OB_FUSE2             (ALARMTABLE_OB_FUSE1 + 1)

//#define ALARMTABLE_BATTFUSE3            (ALARMTABLE_OB + 26)
//#define ALARMTABLE_BATTFUSE4            (ALARMTABLE_BATTFUSE3 + 1)
//#define ALARMTABLE_BATTFUSE5            (ALARMTABLE_BATTFUSE4 + 1)
//#define ALARMTABLE_BATTFUSE6            (ALARMTABLE_BATTFUSE5 + 1)
#define ALARMTABLE_OB_LVD1              (ALARMTABLE_OB + 30)
#define ALARMTABLE_OB_LVD2              (ALARMTABLE_OB_LVD1 + 1)
#define ALARMTABLE_OB_LVD1BACK          (ALARMTABLE_OB + 32)
#define ALARMTABLE_OB_LVD2BACK          (ALARMTABLE_OB_LVD1BACK + 1)

#define ALARMTABLE_OBCOMMBREAK          530

#define ALARMTABLE_IB                   531 /*40*/
#define ALARMTABLE_IB_SW_START          ALARMTABLE_IB
#define ALARMTABLE_IB_DIN1              ALARMTABLE_IB
#define ALARMTABLE_IB_DIN2              (ALARMTABLE_IB_DIN1 + 1)
#define ALARMTABLE_IB_DIN3              (ALARMTABLE_IB_DIN2 + 1)
#define ALARMTABLE_IB_DIN4              (ALARMTABLE_IB_DIN3 + 1)
#define ALARMTABLE_IB_DIN5              (ALARMTABLE_IB_DIN4 + 1)
#define ALARMTABLE_IB_DIN6              (ALARMTABLE_IB_DIN5 + 1)
#define ALARMTABLE_IB_DIN7              (ALARMTABLE_IB_DIN6 + 1)
#define ALARMTABLE_IB_DIN8              (ALARMTABLE_IB_DIN7 + 1)
#define ALARMTABLE_IB_DIN9              (ALARMTABLE_IB_DIN8 + 1)

#define ALARMTABLE_IB_TEMP1             569
#define ALARMTABLE_IB_TEMP2             570

#define ALARMTABLE_IBCOMMBREAK          571


#define ALARMTABLE_EIB                        572 /*80*/
#define ALARMTABLE_EIBCOMMBREAK         652


/*  新500s 新扩展告警表653-697:45个  */
#define ALARMTABLE_OSP_SW_START    653
#define ALARMTABLE_DIGIT1                  ALARMTABLE_OSP_SW_START		/*653 DIN1*/
#define ALARMTABLE_DIGIT2                  (ALARMTABLE_OSP_SW_START+1)	/*DIN2*/
#define ALARMTABLE_DIGIT3                  (ALARMTABLE_OSP_SW_START+2)	/*DIN3*/
#define ALARMTABLE_DIGIT4                  (ALARMTABLE_OSP_SW_START+3)	/*DIN4*/
#define ALARMTABLE_DIGIT5                  (ALARMTABLE_OSP_SW_START+4)	/*DIN5*/
#define ALARMTABLE_DIGIT6                  (ALARMTABLE_OSP_SW_START+5)	/*DIN6*/
#define ALARMTABLE_DIGIT7                  (ALARMTABLE_OSP_SW_START+6)	/*DIN7*/
#define ALARMTABLE_DIGIT8                  (ALARMTABLE_OSP_SW_START+7)	/*DIN8*/

#define ALARMTABLE_SPD                  	 (ALARMTABLE_OSP_SW_START+8)     /*661 SPD告警*/
#define ALARMTABLE_IR                   	 (ALARMTABLE_OSP_SW_START+9)	 /* 662 红外告警*/
#define ALARMTABLE_WATER                  (ALARMTABLE_OSP_SW_START+ 10)	 /* 663 水浸告警*/
#define ALARMTABLE_LVDFUSE               (ALARMTABLE_OSP_SW_START+ 11)   /* 664 lvd fuse-高电平故障*/
#define ALARMTABLE_LOADFUSE1           (ALARMTABLE_OSP_SW_START+12)	 /*665 熔丝1*/
#define ALARMTABLE_LOADFUSE2           (ALARMTABLE_OSP_SW_START+13)	 /*熔丝2*/
#define ALARMTABLE_LOADFUSE3           (ALARMTABLE_OSP_SW_START+14)	 /*熔丝3*/
#define ALARMTABLE_LOADFUSE4           (ALARMTABLE_OSP_SW_START+15)	 /*熔丝4*/
#define ALARMTABLE_LOADFUSE5           (ALARMTABLE_OSP_SW_START+16)	 /*熔丝5*/
#define ALARMTABLE_LOADFUSE6           (ALARMTABLE_OSP_SW_START+17)	 /*熔丝6*/
#define ALARMTABLE_LOADFUSE7           (ALARMTABLE_OSP_SW_START+18)	 /*熔丝7*/
#define ALARMTABLE_LOADFUSE8           (ALARMTABLE_OSP_SW_START+19)	 /*熔丝8*/
#define ALARMTABLE_LOADFUSE9           (ALARMTABLE_OSP_SW_START+20)	 /*熔丝9*/
#define ALARMTABLE_LOADFUSE10         (ALARMTABLE_OSP_SW_START+21)	 /*熔丝10*/

#define ALARMTABLE_BATTFUSE1		  (ALARMTABLE_OSP_SW_START+22)	/*675 电池熔丝1*/
#define ALARMTABLE_BATTFUSE2		  (ALARMTABLE_OSP_SW_START+23)	/*电池熔丝2*/
#define ALARMTABLE_BATTFUSE3		  (ALARMTABLE_OSP_SW_START+24)	/*电池熔丝3*/
#define ALARMTABLE_BATTFUSE4		  (ALARMTABLE_OSP_SW_START+25)	/*电池熔丝4*/
#define ALARMTABLE_BATTFUSE5		  (ALARMTABLE_OSP_SW_START+26)	/*电池熔丝5*/

#define ALARMTABLE_BMV1			 (ALARMTABLE_OSP_SW_START+27)	/*680 电池中点电压1*/
#define ALARMTABLE_BMV2 			 (ALARMTABLE_OSP_SW_START+28)	/*电池中点电压2*/
#define ALARMTABLE_BMV3			 (ALARMTABLE_OSP_SW_START+29)	/*电池中点电压3*/
#define ALARMTABLE_BMV4 			 (ALARMTABLE_OSP_SW_START+30)	/*电池中点电压4*/

//#define ALARMTABLE_FAN1                     (ALARMTABLE_OSP_SW_START+31)	/*684 风机1故障*/
//#define ALARMTABLE_FAN2                     (ALARMTABLE_OSP_SW_START+32)	/* 风机2故障*/
//#define ALARMTABLE_FAN3                     (ALARMTABLE_OSP_SW_START+33)	/* 风机3故障*/
//#define ALARMTABLE_FAN4                     (ALARMTABLE_OSP_SW_START+34)	/* 风机4故障*/
//#define ALARMTABLE_FANLOST               (ALARMTABLE_OSP_SW_START+35)	/* 风机丢失*/
//#define ALARMTABLE_HEATER1                     (ALARMTABLE_OSP_SW_START+36)	/* 689加热器1故障*/
//#define ALARMTABLE_HEATER2                     (ALARMTABLE_OSP_SW_START+37)	/* 加热器2故障*/
#define ALARMTABLE_SYMMETRY_V1      684
#define ALARMTABLE_SYMMETRY_V2      685
#define ALARMTABLE_SYMMETRY_V3      686
#define ALARMTABLE_SYMMETRY_V4      687
#define ALARMTABLE_SYMMETRY_V5      688
#define ALARMTABLE_SYMMETRY_V6      689
#define ALARMTABLE_SYMMETRY_V7      690

#define ALARMTABLE_OSP_ANALOG_START   (ALARMTABLE_OSP_SW_START+40)  /*693*/     
#define ALARMTABLE_TEMP3    		 (ALARMTABLE_OSP_ANALOG_START+0) /*温度3*/
#define ALARMTABLE_SERIALTEMP		 (ALARMTABLE_OSP_ANALOG_START+1) /*串行接口温度*/
#define ALARMTABLE_SERIALHUM 		 (ALARMTABLE_OSP_ANALOG_START+2) /*串行接口湿度*/



#define ALARMTABLE_BM1                  698 /*80*/
#define ALARMTABLE_BM1COMMBREAK         778

//#define ALARMTABLE_BM2                  779 /*80*/
//#define ALARMTABLE_BM2COMMBREAK         859

#define ALARMTABLE_DCDCLOST		  779		/*779:模块丢失告警*/
#define ALARMTABLE_DCDC_START             780
#define ALARMTABLE_DCDCCOMMBREAK	  780		/*780-785:6个模块通讯中断*/
#define ALARMTABLE_DCDCOVERTEMP		  786		/*786-791:6个模块过温*/
#define ALARMTABLE_DCDCFAULT		  792		/*792-797:6个模块故障*/
#define ALARMTABLE_DCDCPROTECT		  798		/*798-803:6个模块保护*/
#define ALARMTABLE_DCDCFANFAULT		  804		/*804-809:6个模块风扇故障*/
#define ALARMTABLE_DCDCHVSD   	          810 	        /*810-815:6个模块过压*/
#define ALARMTABLE_DCDC_END	          815

#define ALARMTABLE_MULTIDCDCALARM	  840	        /*840:多DCDC模块告警*/
//#define ALARMTABLE_BM3                  860 /*80*/
//#define ALARMTABLE_BM3COMMBREAK         940

//#define ALARMTABLE_BM4                  941 /*80*/
//#define ALARMTABLE_BM4COMMBREAK         1021

#define ALARMTABLE_MPPTLOST				860		/*860:模块丢失告警*/
#define ALARMTABLE_MPPT_SERVICE			861		/*861:模块工作时间超时告警（换风扇）*/
#define ALARMTABLE_MULTIMPPTALARM		862		/*862:多模块告警*/

#define ALARMTABLE_ANYMPPTCOMMBREAK     863		/*863:任何模块通讯中断*/
#define ALARMTABLE_ANYMPPTOVERTEMP		864		/*864:任何模块过温*/
#define ALARMTABLE_ANYMPPTFAULT			865		/*865:任何模块故障*/
#define ALARMTABLE_ANYRMPPTPROTECT		866		/*866:任何模块保护*/
#define ALARMTABLE_ANYMPPTFANFAULT		867		/*867:任何模块风扇故障*/
#define ALARMTABLE_ANYMPPTPOWERLIMIT	868		/*868:任何模块限功率*/
#define ALARMTABLE_ANYMPPTCURRUNBALANCE	869		/*869:任何模块不均流*/
#define ALARMTABLE_ANYMPPTINPUTVOLTHIGH	870		/*870:任何模块输入过压**/
#define ALARMTABLE_ANYPVFAULT     		871		/*871:任何方阵故障**/
                                           /*872-899:保留*/
#define ALARMTABLE_MPPT_START           900
#define ALARMTABLE_MPPTCOMMBREAK		900		/*900-909:10个模块通讯中断*/
#define ALARMTABLE_MPPTOVERTEMP			910		/*910-919:10个模块过温*/
#define ALARMTABLE_MPPTFAULT			920		/*920-929:10个模块故障*/
#define ALARMTABLE_MPPTPROTECT			930		/*930-939:10个模块保护*/
#define ALARMTABLE_MPPTFANFAULT			940		/*940-949:10个模块风扇故障*/
#define ALARMTABLE_MPPTPOWERLIMIT		950		/*950-959:10个模块限功率*/
#define ALARMTABLE_MPPTCURRUNBALANCE	960 	/*960-969:10个模块均流故障*/
#define ALARMTABLE_MPPTINPUTVOLTHIGH   	970 	/*970-979:10个模块输入过压*/
#define ALARMTABLE_PVFAULT	        	980 	/*980-989:10个方阵故障*/
#define ALARMTABLE_MPPT_END	            989


#define ALARMTABLE_SYSTEM		1022
#define ALARMTABLE_MAJOR		1022/*1022:有紧急重要告警*/
#define ALARMTABLE_OBSERVATION          1023/*1023:有一般告警*/






#define ANALOG_ALARM_START              ALARMTABLE_ANALOG_START
#define ANALOG_ALARM_END                ALARMTABLE_ANALOG_END
//#define OB_ANALOG_ALARM_START           ALARMTABLE_OB
//#define OB_ANALOG_ALARM_END             (ALARMTABLE_OB + mOBAANUM - 1)
//#define EIB_ANALOG_ALARM_START          ALARMTABLE_EIB
//#define EIB_ANALOG_ALARM_END            (ALARMTABLE_EIB + mEIBAANUM - 1)

#define DIN_ALARM_START                 ALARMTABLE_DIGIT1
#define DIN_ALARM_END                   ALARMTABLE_DIGIT8
//#define OB_DIN_ALARM_START
//#define OB_DIN_ALARM_END
//#define IB_DIN_ALARM_START              ALARMTABLE_IB
//#define IB_DIN_ALARM_END                (IB_DIN_ALARM_START + 8 - 1)
//#define EIB_DIN_ALARM_START             ALARMTABLE_EIB
//#define EIB_DIN_ALARM_END               (EIB_DIN_ALARM_START + 8 - 1)

#define INTERNAL_ALARM_SELFWORK         ALARMTABLE_SELFWORK
#define INTERNAL_ALARM_RECTSTART        ALARMTABLE_ANYRECTCOMMBREAK
#define INTERNAL_ALARM_RECTEND          ALARMTABLE_ANYRECTHVSD
#define INTERNAL_ALARM_SAVEPOWER        ALARMTABLE_SAVEPOWER

#define RECT_ALARM_START                ALARMTABLE_RECTCOMMBREAK
#define RECT_ALARM_END                  (ALARMTABLE_RECTHVSD + 29) //329

#define SMDU1_VOLTAGE_ALARM             (ALARMTABLE_SMDU1 + 21) //381
#define SMDU1_BATTFUSE_ALARM            (SMDU1_VOLTAGE_ALARM + 1)//382
#define SMDU1_CURR_ALARM_START          (ALARMTABLE_SMDU1 + 26) //386
#define SMDU1_CURR_ALARM_END            (SMDU1_CURR_ALARM_START + 3) //389

#define SMDU2_VOLTAGE_ALARM             (ALARMTABLE_SMDU2 + 21) //413
#define SMDU2_BATTFUSE_ALARM            (SMDU2_VOLTAGE_ALARM + 1)//414
#define SMDU2_CURR_ALARM_START          (ALARMTABLE_SMDU2 + 26) //418
#define SMDU2_CURR_ALARM_END            (SMDU2_CURR_ALARM_START + 3) //421

#define INTERNAL_ALARM_SYSTEM           ALARMTABLE_SYSTEM

#define RECT_ALARM_NUM  ((ALARMTABLE_RECT_END-ALARMTABLE_RECT_START+1)/mRECTNUM)
//#define RECT_ALARM_NUM  ((ALARMTABLE_RECT_END-ALARMTABLE_RECT_START+1)/48)
#define MPPT_ALARM_NUM  ((ALARMTABLE_MPPT_END-ALARMTABLE_MPPT_START+1)/mMPPTNUM)

#define PLC_ALARM_4_START       mALARMNUM                               //1024
#define PLC_ALARM_6_START       (mALARMNUM + (mANALOGALARMNUM - 2) * 4) //1056
#define PLC_ALARM_4_END         (PLC_ALARM_6_START - 1)                 //1055
#define PLC_ALARM_6_END         (PLC_ALARM_6_START + 12 - 1)            //1067
#define PLC_ALARM_4_SMDU_START  (PLC_ALARM_6_END + 1)                   //1068
#define PLC_ALARM_4_SMDU_END    (PLC_ALARM_4_SMDU_START + 40 - 1)       //1107
#define PLC_ALARM_4_OB_START    (PLC_ALARM_4_SMDU_END + 1)//1108
#define PLC_ALARM_4_OB_END      (PLC_ALARM_4_OB_START + 10 * 4 - 1)
#define PLC_ALARM_4_EIB_START   (PLC_ALARM_4_OB_END + 1)//1148
#define PLC_ALARM_4_EIB_END     (PLC_ALARM_4_EIB_START + 14 * 4 - 1)//1204-1

#define DCDC_ALARM_NUM  ((ALARMTABLE_DCDC_END-ALARMTABLE_DCDC_START+1)/mDCDCNUM)




