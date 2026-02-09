  /*
 *****************************************************************************
 *                                                                           *
 *  Filename:   httpd-fsdata.c                                   *
 *  Date:       May 1, 2008    
 *  by caili
 *  Description:                                                             *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************/


#include "httpd-fsdata.h"
#include "main.h"
#include "string.h"
//#include "httpd.h"
#define NULL _NULL

//static const  char data_processes_shtml[] = {
	/* /processes.shtml */
//	0x2f, 0x70, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x65, 0x73, 0x2e, 0x73, 0x68, 0x74, 0x6d, 0x6c, 0,
//	0x73, 0xa, 0x25, 0x21, 0x3a, 0x20, 0x2f, 0x66, 0x6f, 0x6f,
//	0x74, 0x65, 0x72, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0};

#define   KBYTE    1024
#define DATA_404_HEADER_SIZE  10


const char data_stats_shtml[] = {
	// /stats.shtml 
"/stats.shtml \0<html></html> \0"
	};

NOINIT UCHAR data_404_html[8*1024+10];

const UCHAR  data_404_header[]={"/404.html\0"};

//Modified by zzc 090721 use the .html instead of .txt for WEB file upload
//const unsigned char data_1[]={"Active Alarm List\r\n"};
//const unsigned char data_2[]={"History Alarm List\r\n"};
//const unsigned char data_3[]={"Battery Log\r\nSeqNo\tStartTime\t\tStartMode\tStartVolt\tEndTime\t\t\tEndMode\t\tEndVolt\tBatt1DischargeCap\tBatt2DischargeCap\r\n"};
//const unsigned char data_4[]={"No Active Alarm \r\n"};
//const unsigned char data_5[]={"No History Alarm \r\n"};
//const unsigned char data_6[]={"No Battery Discharge Log \r\n"};
//const unsigned char data_7[]={"AlarmNo\t\tAlarmName\tBeginTime\tAlarmLevel\r\n"};
//const unsigned char data_8[]={"AlarmNo\t\tAlarmName\tBeginTime\tEndTime\r\n"};

/* 上传的文件由.txt文件转变成.html文件 zzc 09.07.15*/  
const unsigned char data_1[]={"<p>Active Alarm List</p>"};
const unsigned char data_2[]={"<p>History Alarm List</p>"};
const unsigned char data_3[]={"<p>Battery Log</p><p>SeqNo  StartTime       StartMode   StartVolt   EndTime         EndMode     EndVolt Batt1DischargeCap   Batt2DischargeCap</p>"};
const unsigned char data_4[]={"<p>No Active Alarm</p>"};
const unsigned char data_5[]={"<p>No History Alarm</p>"};
const unsigned char data_6[]={"<p>No Battery Discharge Log </p>"};
const unsigned char data_7[]={"<p>AlarmNo      AlarmName   BeginTime   AlarmLevel</p>"};
const unsigned char data_8[]={"<p>AlarmNo      AlarmName   BeginTime   EndTime</p>"};
const unsigned char data_9[]={"<p>Statistic List</p>"};
const unsigned char data_10[]={"<p>No Statistic</p>"};
const unsigned char data_11[]={"<p>RecordDate : Load -- SolarPwr -- MainsPwr -- DG Pwr -- MaxBattVolt -- MinBattVolt</p>"};
/************************************************/
/*函数名称:make_up_loadfile0()  			                */
/*函数功能:做上传配置文件							        */
/*入口参数:无									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
void make_up_loadfile_config_files(void)
{
  memset(data_404_html,0,8*KBYTE+10);
  memmove(data_404_html,data_404_header,10);
  memmove(data_404_html+DATA_404_HEADER_SIZE,&gSetDataInfo.SetData.NameData.ucSysType,8*KBYTE);
}





/************************************************/
/*函数名称:make_up_loadfile1()  			                */
/*函数功能:做上传告警文件							        */
/*入口参数:无									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/

//Modified by zzc 090721 use the .html instead of .txt for WEB file upload
#define MAX_ALARM_FILE_DATA_ITEM   80
void make_up_loadfile_active_alarms_file(void)
{
	int temp16;//num;
 	//char temp[100];
  	//int i; 
    int        j=DATA_404_HEADER_SIZE;
    //       memset(data_404_html,0,8*KBYTE+10);
    memset(data_404_html,0x20,8*KBYTE+10);/*解决上传文档不能用Ultraedit打开问题 wzp08.12.18*/
    memmove(data_404_html,data_404_header,10);
    temp16 =0; //gs_AlarmMng.fGetActiveAlarmNum();//get the active alarm num
    memmove(data_404_html+j,data_1,strlen((char *)data_1));

    j=j+strlen((char *)data_1);;
    if(temp16==0)
    {
        memmove(data_404_html+j,data_4,strlen((char *)data_4));
        j  += strlen((char *)data_4);
        return;
    }
    /*if(temp16>MAX_ALARM_FILE_DATA_ITEM)
    {
    num=MAX_ALARM_FILE_DATA_ITEM;
    // j += sprintf((char *) data_404_html + j, "Maximum Active Alarm Num can be shown : %d\r\n", num );
    j += sprintf((char *) data_404_html + j, "<p>Maximum Active Alarm Num can be shown : %d</p>", num );

    }
    else
    {
    num=temp16;
    //j += sprintf( (char *)data_404_html + j, "Active Alarm Num : %d\r\n", num );
    j += sprintf( (char *)data_404_html + j, "<p>Active Alarm Num : %d</p>", num );

    }
    memmove(data_404_html+j,data_7,strlen((char *)data_7));
    j  += strlen((char *)data_7);


    for(i=0;i<num;i++)
    {
    memset(temp,0,100);
    if(gs_AlarmMng.fGetAlarmNameFiles(i,&temp))
    {
    //j += sprintf( (char *)data_404_html + j, "%d\t  ", i+1);
    j += sprintf( (char *)data_404_html + j, "<p>%d      ", i+1);
    memmove(data_404_html+j,temp,strlen((char *)temp));
    j += strlen((char *)temp);
    }
    }*/
}

/************************************************/
/*函数名称:make_up_loadfile2()  			                */
/*函数功能:做上传历史告警文件							        */
/*入口参数:无									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
//Modified by zzc 090721 use the .html instead of .txt for WEB file upload

void make_up_loadfile_history_alarms_file(void)
{

    int temp16;//num;
    //char temp[100];
    int j=DATA_404_HEADER_SIZE;//i,
    //	 memset(data_404_html,0,8*KBYTE+10);
    memset(data_404_html,0x20,8*KBYTE+10);/*解决上传文档不能用Ultraedit打开问题 wzp08.12.18*/
    memmove(data_404_html,data_404_header,10);
    temp16 = 0;//gs_AlarmMng.fGetHistoryAlarmNum();//get the history alarm num

    memmove(data_404_html+j,data_2,strlen((char *)data_2));
    j  += strlen((char *)data_2);

    if(temp16==0)
    {
        memmove(data_404_html+j,data_5,strlen((char *)data_5));
        j  += strlen((char *)data_5);
        return;
    }
    /*if(temp16>MAX_ALARM_FILE_DATA_ITEM)
    {
    num=MAX_ALARM_FILE_DATA_ITEM;
    //j += sprintf( (char *)data_404_html + j, "Maximum History Alarm Num can be shown : %d\r\n", num );
    j += sprintf( (char *)data_404_html + j, "<p>Maximum History Alarm Num can be shown : %d</p>", num );

    }
    else
    {
    num=temp16;
     //j += sprintf( (char *)data_404_html + j, "History Alarm Num : %d\r\n", num );
    j += sprintf( (char *)data_404_html + j, "<p>History Alarm Num : %d</p>", num );

    }
         memmove(data_404_html+j,data_8,strlen((char *)data_8));
    j  += strlen((char *)data_8);

    for(i=0;i<num;i++)
    {
    memset(temp,0,100);
    if(gs_HistoryAlarm.fGetHistoryAlarmFiles(i,&temp))
    {
       // j += sprintf( (char *)data_404_html + j, "%d\t  ", i+1 );//write seqno 
        j += sprintf( (char *)data_404_html + j, "<p>%d      ", i+1 );//write seqno 
        memmove(data_404_html+j,temp,strlen((char *)temp));//write  data
        j  += strlen((char *)temp);
    }
    }*/
   
}
/************************************************/
/*函数名称:make_up_loadfile2()  			                */
/*函数功能:做上传统计纪录							        */
/*入口参数:无									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
void make_up_loadfile_statistic_file(void)
{
    int temp16;//num;
    //char temp[88];
    int j=DATA_404_HEADER_SIZE;// i,
    //	 memset(data_404_html,0,8*KBYTE+10);
    memset(data_404_html,0x20,8*KBYTE+10);/*解决上传文档不能用Ultraedit打开问题 wzp08.12.18*/
    memmove(data_404_html,data_404_header,10);
    temp16 = 0;//gs_StatisticMng.DayStatisticNum;//get the num

    memmove(data_404_html+j,data_9,strlen((char *)data_9));
    j  += strlen((char *)data_9);

    if(temp16==0)
    {
        memmove(data_404_html+j,data_10,strlen((char *)data_10));
        j  += strlen((char *)data_10);
        return;
    }
   /* if(temp16>mDAYRECORDNUM)
    {
        num=mDAYRECORDNUM;
    }
    else
    {
        num=temp16;		
    }


    j += sprintf( (char *)data_404_html + j, "<p>Daily-Statistic Num : %d</p>", num );

    memmove(data_404_html+j,data_11,strlen((char *)data_11));
    j  += strlen((char *)data_11);

    for(i=0;i<num;i++)
    {
        memset(temp,0,88);
        if(gs_StatisticMng.fGetDayStatisticFiles(i,&temp))
        {
            memmove(data_404_html+j,temp,strlen((char *)temp));//write  data
            j  += strlen((char *)temp);
        }
    }*/
}
/************************************************/
/*函数名称:make_up_loadfile3()  			                */
/*函数功能:做上传电池记住文件							        */
/*入口参数:无									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
//Modified by zzc 090721 use the .html instead of .txt for WEB file upload
void make_up_loadfile_battery_discharge_log(void)
{
    int batt_log_num=0;
    //    	char temp[100];
    char temp[120]; /*长度不够导致数组溢出 wzp08.12.18*/
    int i,j=DATA_404_HEADER_SIZE;
    memset(data_404_html,0x20,8*KBYTE+10);/*解决上传文档不能用Ultraedit打开问题 wzp08.12.18*/
    memmove(data_404_html,data_404_header,10);
    memmove(data_404_html+j,data_3,strlen((char *)data_3));

    j  += strlen((char *)data_3);
    for(i=0;i<10;i++)
    {
        //	memset(temp,0,100);
        memset(temp,0,120);
        /*if(gs_BattMng.fGetBattTestLog(i,&temp))
        {
            batt_log_num++;
            memmove(data_404_html+j,temp,strlen((char *)temp));
            j += strlen((char *)temp);
        }*/
    }
    if(batt_log_num==0)
    {
        memset(data_404_html+DATA_404_HEADER_SIZE,0x20,8*KBYTE);
        memmove(data_404_html+DATA_404_HEADER_SIZE,data_6,strlen((char *)data_6));
    }
  
}

//it must be const 

//static const unsigned char data_files_shtml[] = {
	/* /files.shtml */
//	"/files.shtml\0"};
 char val[]={"#7fff00"};

const char data_temp1_html[]={
"/footer.html\0<html>\n\
<script>\n\
function initbody () \n\
{\n\
if(top.jm.foot!=null)\n\
{\n\
top.jm.timer = 0;\n\
top.jm.initdata();\n\
}\n\
}\n\
function SD1(n1,v1)\n\
{	var x;\n\
x=document.getElementById(\"W\");\n\
document.getElementById(\"W1\").name=n1;\n\
document.getElementById(\"W1\").value=v1;\n\
x.submit();\n\
}\n\
\0"
}; 

//Modified by hulw 090628 Set sitename on WEB
//char data_footer_html[1110];
char data_footer_html[1110 + 33];
 
const char data_temp2_html[]={
"</script><body onload=\"initbody()\"><form id=\"W\">\
<input TYPE=\"TEXT\" id=\"W1\"></form></body></html>\0"
}; 



const char batt_mode[8][15]={
	{"Float"},
	{"Temp Comp"},
	{"NeedBC"},
	{"Boost"},
	{"CycBoost"},
	{"Test"},
	{"ConCurrTest"},
	{"ShortTest"}
};

/************************************************/
/*函数名称:make_data_file()		                */
/*函数功能:制作数据文件 							        */
/*入口参数:									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
void make_data_file(int user)
{
 // __XXX_UNION_VALUE unVal;
	//UINT16  u_temp;
	//UCHAR  uc_temp;
	//int i;
	//int j=0,j1=0,p=0;
	//float f32ftemp;
	//char s_manual[]="Manual";
	//char s_auto[]="Auto";
	////char s2[]="china";
	//UCHAR  site_name[33];
	//UCHAR ip[4];
	//UCHAR hw[4];

	//char  buffer[25];

	//memset(data_footer_html,0,820);
	//memmove(data_footer_html,data_temp1_html,sizeof(data_temp1_html));
	//j=sizeof(data_temp1_html);
 //       j=j-2;//it is for moliza  
	//f32ftemp=0;
 //      //gMeasureInfo.fGetMeasureAnalogData(DCV_NUM, &f32ftemp);
	//j+=sprintf(data_footer_html+j,"var strSystemVolt=%3.1f,",f32ftemp);

	//gMeasureInfo.fGetMeasureAnalogData(LOADCURR_NUM, &f32ftemp);
	//j+=sprintf(data_footer_html+j,"strSystemLoad=%3.1f,",f32ftemp);

	////u_temp=gs_AlarmMng.fGetMajorAlarmNum();
	//u_temp=0;//gs_AlarmMng.fGetMajorAlarmNum()+gs_AlarmMng.fGetCriticalAlarmNum();
	//j+=sprintf(data_footer_html+j,"strMANumber=%d,",u_temp);

	//u_temp=0;//gs_AlarmMng.fGetObservationAlarmNum();
	//j+=sprintf(data_footer_html+j,"strOANumber=%d,",u_temp);
	//memset(buffer,0,25);
	//j1=0;
	// GetCharSetData(SETDATA_CtlMode_NUM, &uc_temp);
	//if(uc_temp==1)
	//{
 // 		j1=sprintf(buffer,"%s",s_manual);
	////j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//}
	//if(uc_temp==2)
	//{
 //		 j1=sprintf(buffer,"%s",s_auto);
	//}
 //    	 
 //      uc_temp = 0;//gs_BattMng.BattMngProc.fGetECFlag();

 //      j1+=sprintf(buffer+j1,"%s",batt_mode[uc_temp]);
	//  
	//j+=sprintf(data_footer_html+j,"strBattMode=\"%s\",",buffer);	
	//	
	//for(i=0;i<32;i++)
	//{
 //		GetCharSetData(SETDATA_SiteName_NUM+i, &uc_temp);
 //		site_name[i]=uc_temp;
	//}
	//site_name[32] = '\0';
	//j+=sprintf(data_footer_html+j,"strSiteName=\"%s\",",(char *)site_name);

	//for(i=0;i<3;i++)
	//{
 //		GetCharSetData(SETDATA_OFF_LINE_PSWD+25+i, &uc_temp);
 //		hw[i]=uc_temp;
	//}
	//hw[3]='\0';

	//j+=sprintf(data_footer_html+j,"strHWVersion=\"%s\",",hw);

 //	GetFloatSetData(SETDATA_SoftwareVersion_NUM , &f32ftemp);
	//memset(hw,0,sizeof(hw));
	//sprintf((char *)hw,"%3.2f",f32ftemp);


	//j+=sprintf(data_footer_html+j,"strSWVersion=\"%s\"\n",hw);

	//GetCharSetData(SETDATA_TempCompEnable_NUM, &uc_temp);
	//j+=sprintf(data_footer_html+j,"var varBatteryTempComp=%d,",uc_temp);

	//GetCharSetData(SETDATA_TimeTestEnable_NUM, &uc_temp);
	//j+=sprintf(data_footer_html+j,"varBatteryTest=%d,",uc_temp);

 //   //    GetFloatSetData(SETDATA_DCLowVolt_NUM + uctemp * 2, &f32rectcaplimit);
	//GetCharSetData(SETDATA_CurrLimitMode_NUM, &uc_temp);

	//j+=sprintf(data_footer_html+j,"varCurrentLimitation=%d,",uc_temp);

	//GetCharSetData(SETDATA_TimeECEnable_NUM, &uc_temp);

	//j+=sprintf(data_footer_html+j,"varBoostCharge=%d,",uc_temp);

	//GetCharSetData(SETDATA_BattType_NUM, &uc_temp);
	////		GetFloatSetData(2*uc_temp+SETDATA_DCLowVolt_NUM,&f_temp);
	//GetFloatSetData(2*uc_temp+SETDATA_DCLowVolt_NUM , &f32ftemp);
	//j+=sprintf(data_footer_html+j,"varUnderVoltageLevel1=%3.2f,",f32ftemp);
 //   	GetFloatSetData(2*uc_temp+SETDATA_DCLowVolt_NUM+1 , &f32ftemp);
	//j+=sprintf(data_footer_html+j,"varUnderVoltageLevel2=%3.2f,",f32ftemp);

	//GetFloatSetData(uc_temp+SETDATA_LoadCutVolt_NUM , &f32ftemp);
	//j+=sprintf(data_footer_html+j,"varLVDLevel1=%3.2f,",f32ftemp);

	//GetFloatSetData(uc_temp+SETDATA_BattCutVolt_NUM , &f32ftemp);
	//j+=sprintf(data_footer_html+j,"varLVDLevel2=%3.2f,",f32ftemp);

	//
	////j+=sprintf(data_footer_html+j,"varSystemVolt1=%d;\n",baterrtT);

	//GetFloatSetData(uc_temp+SETDATA_FCVolt_NUM , &f32ftemp);
	//j+=sprintf(data_footer_html+j,"varSystemVolt1=%3.2f,",f32ftemp);

	//GetFloatSetData(uc_temp+SETDATA_ECVolt_NUM , &f32ftemp);
	//j+=sprintf(data_footer_html+j,"varBoostVolt1=%3.2f;\n",f32ftemp);

	//memset(buffer,0,20);
	//j1=0;
	//for(p=0;p<4;p++)//get the trap ip
	//{
	//	//GetCharSetData(SETDATA_NMSIP_NUM+p,&ip[p]);
	//	ExGetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + p, &unVal, NULL, BY_SRAM);
	//	ip[p]= unVal.chtemp[0]; 
	//}
	//j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
	//j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"var varTrapDestination1=\"%s\",",buffer);


	//memset(buffer,0,20);
	//j1=0;
 //      for(p=0;p<4;p++)//get the trap ip
 //   	{
 //     		//GetCharSetData(SETDATA_NMSIP_NUM+p+6,&ip[p]);
 //         ip[p] = 0;
 //   	}
 //  	j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
 //      j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"varTrapDestination2=\"%s\"\n",buffer);

	//memset(buffer,0,20);
	//j1=0;
	//for(p=0;p<4;p++)//get the trap ip
	//{
	//	//GetCharSetData(SETDATA_IP_NUM+p,&ip[p]);
	//	 ExGetSigInfo(E_TYPE_SYS, ID_u8IP_1 + i, &unVal, NULL, BY_SRAM);
	//	ip[p]= unVal.chtemp[0]; 
	//
	//}
 //  	j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
 //      j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"var varIPAddress=\"%s\",",buffer);

	//memset(buffer,0,20);
	//j1=0;
 //     	for(p=0;p<4;p++)//get the trap ip
 //   	{
 //     		//GetCharSetData(SETDATA_Gateway_NUM+p,&ip[p]);
 //                                         ExGetSigInfo(E_TYPE_SYS, ID_u8Gateway_1 + p, &unVal, NULL, BY_SRAM);
 //                ip[p]= unVal.chtemp[0]; 
 //   	}
 //  	j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
 //      j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"varDefaultGateway=\"%s\",",buffer);
	//memset(buffer,0,20);
	//j1=0;
 //     	for(p=0;p<4;p++)//get the trap ip
 //   {
 //     //GetCharSetData(SETDATA_Subnetmask_NUM+p,&ip[p]);
 //                                               ExGetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_1 + p, &unVal, NULL, BY_SRAM);
 //                ip[p]= unVal.chtemp[0]; 
 //   }
 //  	j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
 //      j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"varSubnetMask=\"%s\",",buffer);

	//memset(buffer,0,20);
	//j1=0;
 //     	for(p=0;p<4;p++)//get the trap ip
 //   	{
 //     		//GetCharSetData(SETDATA_PowerStarIP_NUM+p,&ip[p]);
 //                 ExGetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + p, &unVal, NULL, BY_SRAM);
 //                ip[p]= unVal.chtemp[0]; 
 //   	}
 //  	j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
 //      j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"varEEM1=\"%s\",",buffer);
	//memset(buffer,0,20);
	//j1=0;
 //     	for(p=0;p<4;p++)//get the trap ip
 //   {
 //       //GetCharSetData(SETDATA_PowerStarIP_NUM+6+p,&ip[p]);
 //                       ExGetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + p, &unVal, NULL, BY_SRAM);
 //                ip[p]= unVal.chtemp[0]; 
 //   }
 //  	j1=sprintf(buffer,"%d.",ip[0]);
	//j1+=sprintf(buffer+j1,"%d.",ip[1]);
	//j1+=sprintf(buffer+j1,"%d.",ip[2]);
 //      j1+=sprintf(buffer+j1,"%d",ip[3]);
	//j+=sprintf(data_footer_html+j,"varEEM2=\"%s\",",buffer);

	////Modified by hulw 090628 Set sitename on WEB
	//j+=sprintf(data_footer_html+j,"varSiteName=\"%s\",",(char *)site_name);

	//if(user==OPERATOR_USER)
	//{
	//j+=sprintf(data_footer_html+j,"varUser=%d\n",1);
	//}
	//else
	//{
	//j+=sprintf(data_footer_html+j,"varUser=%d\n",0);
	//
	//}	
	
	//memmove(data_footer_html+j,data_temp2_html,sizeof(data_temp2_html));

	//   memmove(data_footer_html,data_footer_html1,sizeof(data_footer_html1));
}

/*
	if(timer==1)\n\
	{	alert(\"The http link has ended\");	 }\n\
	timer=1;\n\
*/
//Modified by hulw 090628 Save code space
static const char data_header_html[/*16184*/] = {//16169] = {//16745] = {
	
"/header.html\0<html>\n\
<STYLE>\n\
body\n\
{margin:5px;\n\
padding:5px;\n\
font-family:Arial, Helvetica, sans-serif;\n\
font-size:small;\n\
background:#f1f1f1;}\n\
table\n\
{width:100%;\n\
border:1px solid #cccccc;\n\
background:#f1f1f1;\n\
border-collapse:collapse;}\n\
table th,table td\n\
{margin:0;\n\
padding:8px 10px;\n\
text-align:left;\n\
border-bottom:1px solid #b5b5b5;\n\
border-left:1px solid #b5b5b5;\n\
border-right:1px solid #b5b5b5;}\n\
.inputext \n\
{width: 100%;}\n\
.tilte\n\
{font-size:90%;\n\
font-weight:bold;\n\
color:#666666;}\n\
.message\n\
{\n\
font-size:90%;\n\
font-weight:bold;\n\
color:#333333;\n\
}\n\
th\n\
{\n\
background-color: #99ccff;\n\
color: #000066;\n\
width: 105%;\n\
}\n\
select\n\
{\n\
width: 100%;\n\
}\n\
.foot1\n\
{\n\
CLEAR: both;\n\
PADDING-RIGHT: 5px;\n\
BORDER-TOP: #e9e9e9 1px solid;\n\
MARGIN-TOP: 3px;\n\
PADDING-LEFT: 5px;\n\
FONT-SIZE: 90%;\n\
font-weight:bold;\n\
MARGIN-BOTTOM: 10px;\n\
PADDING-BOTTOM: 5px;\n\
PADDING-TOP: 5px;\n\
TEXT-ALIGN: center\n\
}\n\
.alt\n\
{\n\
background-color:#E6E6E6;\n\
}\n\
</STYLE>\n\
<SCRIPT>\n\
var Tf,Lf=0;\n\
var timer;\n\
var sh,fh,hh;\n\
function check_link()\n\
{\n\
if(timer==3)\n\
{alert(\"The http link has ended\");}\n\
if(timer<3)timer++;}\n\
function timeUp()\n\
{Lf = 0;\n\
window.clearTimeout(Tf);\n\
}\n\
function check_click()\n\
{\n\
if(Lf==0)\n\
{\n\
Lf = 1;\n\
Tf = window.setTimeout(\"timeUp()\",3000);\n\
return true;\n\
}\n\
else\n\
{\n\
alert(\"Click too frequently,please wait\");\n\
return false;\n\
}\n\
}\n\
function t(id1,fg)\n\
{var v1,n1;\n\
v1=(document.getElementById(id1).value);\n\
n1=(document.getElementById(id1).name);\n\
if(fg!=0)\n\
{if(isNaN(v1)){alert(\"The data form is not correct\");\n\
return;	}\n\
if((v1<20)||(v1>60))\n\
{alert(\"The setting data is out of range\");\n\
return;}}\n\
if(check_click()==true)\n\
{top.md.SD(n1,v1,0,0,0,0,0);}}\n\
function isNumeric(strval)\n\
{var c; for(var i=0; i<strval.length;i++)\n\
{c=strval.charAt(i);\n\
if(c<\"0\"||c>\"9\"){return false;}}\n\
return true;}\n\
function atoi( str )\n\
{if(!isNumeric(str) )\n\
return 0;\n\
var s1=\"\",i,c;\n\
for(i=0;i<str.length;i++){c=\"\"+str.charAt(i);\n\
if(c!=\"0\")break;}\n\
if(i==str.length)\n\
return 0;\n\
for(;i<str.length;i++)\n\
s1=s1+str.charAt(i);\n\
return parseInt(s1);}\n\
function convertIP(what)\n\
{\n\
if(!validateIP(what))\n\
return 0;\n\
var myArray=what.split(/\\./);\n\
var ip=0, i;\n\
for(i=0; i<4; i++){\n\
ip <<= 8;\n\
ip += atoi(myArray[i]);\n\
}\n\
return ip;\n\
}\n\
function validateIP( what ) {\n\
if(what.search(/^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$/) == -1)\n\
return false;\n\
var myArray = what.split(/\\./);\n\
var i;\n\
for( i = 0; i < 4; i ++ ) \n\
{\n\
if( !isNumeric(myArray[i]) )\n\
return false;\n\
var t=atoi(myArray[i]);\n\
if( (t<0) || (t>255) )\n\
return false;\n\
}\n\
return true;\n\
}\n\
function validateMask( mask ) {\n\
var is1 = false;\n\
var b1 = 1;\n\
if( mask == 0 )\n\
return true;\n\
while( b1 != 0 ) {\n\
if( (mask & b1) != 0 )\n\
is1 = true;\n\
else if( is1 )\n\
return false;\n\
b1 <<= 1;\n\
}\n\
return true;\n\
}\n\
function checkNetIp() {\n\
var strVal;\n\
var ipBrd=convertIP(\"255.255.255.255\");\n\
var strIp=document.getElementById(\"B13\").value;\n\
var name1=document.getElementById(\"B13\").name;\n\
var ip=convertIP(strIp);\n\
if( !validateIP(strIp) || (ip==0) || (ip==ipBrd)) {\n\
window.alert( \"IP input error\");\n\
return false;\n\
}\n\
var strSubnet =document.getElementById(\"B14\").value;\n\
if(strSubnet==\"\")\n\
strSubnet = document.getElementById(\"C14\").value;\n\
var name2=document.getElementById(\"B14\").name;\n\
var mask=convertIP(strSubnet);\n\
if( !validateIP(strSubnet) || (mask==ipBrd) || !validateMask(mask) ) {\n\
window.alert( \"IP mask is error\");\n\
return false;\n\
}\n\
if( (ip&mask) == ip ) {\n\
window.alert( \"IP & Mask is Error\" );\n\
return false;\n\
}\n\
var strGate = document.getElementById(\"B15\").value;\n\
if(strGate==\"\")\n\
strGate = document.getElementById(\"C15\").value;\n\
var name3=document.getElementById(\"B15\").name;\n\
var gate=convertIP(strGate);\n\
if( !validateIP(strGate) || (gate==ipBrd) ) {\n\
window.alert( \"Gateway IP was error\");\n\
return false;\n\
}\n\
if( ((gate != 0) && ((ip & mask) != (gate & mask))) )	{\n\
window.alert( \"IP,gateway mask mismatch. input again.\" );\n\
return false;\n\
}\n\
if(confirm(\"The IP address will change,Please connect again\") == true) {\n\
if(check_click()==true)\n\
{\n\
top.md.SD(name1,strIp,name2,strSubnet,name3,strGate,1);\n\
}\n\
}\n\
}\n\
function check_ip(id1)\n\
{var name1;\n\
name1=document.getElementById(id1).name;\n\
var strIp=document.getElementById(id1).value;\n\
var ipBrd=convertIP(\"255.255.255.255\");\n\
var ip=convertIP(strIp);\n\
if( !validateIP(strIp) || (ip==0) || (ip==ipBrd)) {\n\
window.alert( \"IP input error\");\n\
}\n\
else {\n\
if(check_click()==true)\n\
{\n\
top.md.SD(name1,strIp,0,0,0,0,0);\n\
}\n\
}\n\
}\n\
function upLoadTarFile()\n\
{\n\
var x = document.getElementById(\"uploadfile\");\n\
if(check_click()==true)\n\
{\n\
if(x.selectedIndex==0)\n\
window.open(\"/ConfigsFile.dat\");\n\
if(x.selectedIndex==1)\n\
window.open(\"/ActiveAlarms.html\");\n\
if(x.selectedIndex==2)\n\
window.open(\"/HistoryAlarms.html\");\n\
if(x.selectedIndex==3)\n\
window.open(\"/BatteryDischargelog.html\");\n\
if(x.selectedIndex==4)\n\
window.open(\"/Statisticlog.html\");\n\
}\n\
}\n\
function getElement( id ) \n\
{\n\
var x;\n\
if (document.getElementById )\n\
x = document.getElementById(id);\n\
else if (document.all)\n\
x = document.all[id];\n\
return x;\n\
}\n\
function link_bootload()\n\
{\n\
var k;\n\
top.da.location.replace('index.html');\
k=window.open(\"/\");\n\
if(k!=null)\n\
{ top.window.opener=null;\n\
top.window.close();\n\
window.clearTimeout(sh);\n\
} else\n\
{\n\
k.close();\n\
alert(\"Go to Bootloader Failed\");\n\
}\n\
}\n\
function GotoBoot()\n\
{\n\
if(check_click()==true)\n\
{if(confirm(\"Go to the Firmware Download Mode? Please wait for about 30 seconds\") == true){\n\
clearInterval(fh);clearInterval(hh);\n\
var v1,n1;\n\
v1= 30;\n\
n1= 30;\n\
top.md.SD(n1,v1,0,0,0,0,0);\n\
sh=setTimeout(\"link_bootload();\", 30000);}}\n\
}\n\
function checkValidFile(varFileName)\n\
{\n\
var varSolutionFile = /\\\\ConfigsFile\\.dat$/;\n\
if(varSolutionFile.test(varFileName))\n\
{\n\
return true;\n\
}\n\
return false;\n\
}\n\
function sendUploadCommand()\n\
{\n\
if(check_click()==true)\n\
{\n\
var s = getElement(\"_fileselect\");\n\
if( s.value ==\"\" )\n\
{\n\
alert(\"File name can't be empty!\");\n\
return;\n\
}\n\
else\n\
{\n\
if(checkValidFile(s.value) == true)\n\
{\n\
if(confirm(\"Confirm file\" + \"?\") == true) \n\
{\n\
document._uploadfile.upfile.value=document.all[\"_fileselect\"].value;\n\
document._uploadfile.submit();\n\
}\n\
}\n\
else\n\
alert(\"The download file is not correct\");\n\
}\n\
}\n\
}\n\
</SCRIPT>\n\
<body onload=\"initbody();\" onbeforeunload=\"top.da.location.replace('footer.html');\">\n\
<table>\n\
<tr>\n\
<td class=\"message\"><div id=\"A1\"></div></td>\n\
<td class=\"message\"><div id=\"A2\"></div></td>\n\
<td class=\"message\"><div id=\"A3\"></div></td>\n\
<td class=\"message\"><div id=\"A4\"></div></td>\n\
</tr>\n\
<tr>\n\
<td class=\"message\"><div id=\"A5\"></div></td>\n\
<td class=\"message\"><div id=\"A6\"></div></td>\n\
<td class=\"message\"><div id=\"A7\"></div></td>\n\
<td class=\"message\"><div id=\"A8\"></div></td>\n\
</tr>\n\
</table>\n\
<hr>\n\
<table>\n\
<tr>\n\
<th colspan=\"9\">Settings</th>\n\
</tr>\n\
<tr>\n\
<td colspan=\"3\"><span class=\"tilte\">Temperature Compensation</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Battery Test</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Current Limitation</span></td>\n\
</tr>\n\
<tr>\n\
<td width=\"13%\"><select id=\"C1\" size=\"1\"  disabled  >\n\
<option value=1>Disabled</option>\n\
<option value=0>Enabled</option>\n\
</select></td>\n\
<td width=\"13%\"><select  size=\"1\" id=\"B1\" name=\"1\">\n\
<option value=1>Enabled</option>\n\
<option value=0>Disabled</option>\n\
</select>        </td>\n\
<td width=\"4%\">\n\
<input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B1\",0) value=\"Set\"> </td>\n\
<td width=\"13%\"><select id=\"C2\" size=\"1\" disabled  >\n\
<option value=1>Disabled</option>\n\
<option value=0>Enabled</option>\n\
</select></td>\n\
<td width=\"13%\">\n\
<select size=\"1\" id=\"B2\" name=\"2\">\n\
<option value=1>Enabled</option>\n\
<option value=0>Disabled</option>\n\
</select></td>\n\
<td width=\"4%\">\n\
<input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B2\",0) value=\"Set\"></td>\n\
<td width=\"13%\"><select id=\"C3\" size=\"1\" disabled  >\n\
<option value=0>Enabled</option>\n\
<option value=1>Disabled</option>\n\
</select></td>\n\
<td width=\"13%\"><select size=\"1\" id=\"B3\" name=\"3\">\n\
<option value=0>Enabled</option>\n\
<option value=1>Disabled</option>\n\
</select></td>\n\
<td width=\"4%\"><input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B3\",0) value=\"Set\"></td>\n\
</tr>\n\
<tr>\n\
<td colspan=\"3\"><span class=\"tilte\">Boost Charge</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">LVD1 Level</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">LVD2 Level</span></td>\n\
</tr>\n\
<tr>\n\
<td><select id=\"C4\"  size=\"1\" disabled >\n\
<option value=1>Disabled</option>\n\
<option value=0>Enabled</option>\n\
</select></td>\n\
<td>\n\
<select size=\"1\" id=\"B4\" name=\"4\">\n\
<option value=1>Enabled</option>\n\
<option value=0>Disabled</option>\n\
</select></td>\n\
<td>\n\
<input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B4\",0) value=\"Set\">\n\
</td>\n\
<td><input type=\"TEXT\" class=\"inputext\" disabled id=\"C7\" size=\"10\" maxlength=\"5\" ></td>\n\
<td>\n\
<input name=\"7\" type=\"TEXT\" class=\"inputext\" id=\"B7\" size=\"10\" maxlength=\"5\">\n\
</td>\n\
<td>\n\
<input type=\"button\" value=\"Set\" onClick=t(\"B7\",1)>\n\
</td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"C8\" size=\"10\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"B8\" name=\"8\" size=\"10\"></td>\n\
<td><input type=\"button\" value=\"Set\" onClick=t(\"B8\",1)></td>\n\
</tr>\n\
<tr>\n\
<td colspan=\"3\"><span class=\"tilte\">Under Voltage Level1</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Under Voltage Level2</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">System Voltage</span></td>\n\
</tr>\n\
<tr>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"C5\" size=\"10\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"B5\" name=\"5\" size=\"10\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B5\",2) value=\"Set\"></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"C6\" size=\"10\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"B6\" name=\"6\" size=\"10\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B6\",2) value=\"Set\"></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"C9\" size=\"10\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"B9\" name=\"9\" size=\"10\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B9\",1) value=\"Set\"></td>\n\
</tr>\n\
<tr>\n\
<td colspan=\"3\"><span class=\"tilte\">Boost Voltage</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Trap Destination1</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Trap Destination2</span></td>\n\
</tr>\n\
<tr>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"C10\" size=\"10\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"5\" id=\"B10\" name=\"10\" size=\"10\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B10\",1) value=\"Set\"></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C11\" size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"B11\" name=\"11\" size=\"15\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=check_ip(\"B11\") value=\"Set\"></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C12\" size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\"  id=\"B12\" name=\"12\" size=\"15\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=check_ip(\"B12\") value=\"Set\"></td>\n\
</tr>\n\
<tr>\n\
<td colspan=\"3\"><span class=\"tilte\">Remote1 IP</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Remote2 IP</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Site Name</span></td>\n\
</tr>\n\
<tr>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C20\" size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"B20\" name=\"20\" size=\"15\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=check_ip(\"B20\") value=\"Set\"></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C21\" size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\"  id=\"B21\" name=\"21\" size=\"15\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=check_ip(\"B21\") value=\"Set\"></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"32\" id=\"C16\" size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"32\"  id=\"B16\" name=\"16\" size=\"15\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=t(\"B16\",0) value=\"Set\"></td>\n\
</tr>\n\
<tr>\n\
<td colspan=\"3\"><span class=\"tilte\">TCP/IP Address</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Subnet Mask</span></td>\n\
<td colspan=\"3\"><span class=\"tilte\">Default Gateway</span></td>\n\
</tr>\n\
<tr>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C13\"  size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"B13\" name=\"13\" size=\"15\"></td>\n\
<td>&nbsp;</td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C14\"  size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"B14\" name=\"14\" size=\"15\"></td>\n\
<td>&nbsp;</td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"C15\"  size=\"15\" disabled ></td>\n\
<td><input type=\"TEXT\" class=\"inputext\" maxlength=\"15\" id=\"B15\" name=\"15\" size=\"15\"></td>\n\
<td><input name=\"button\" type=\"button\" id=\"button\" onClick=\"checkNetIp()\" value=\"Set\"></td>\n\
</tr>\n\
</table>\n\
<hr>\n\
<table>\n\
<tr>\n\
<th colspan=\"4\">Upload/Download Files</th>\n\
</tr>\n\
<tr>\n\
<td colspan=\"2\"><span class=\"tilte\">Upload File</span></td>\n\
<td colspan=\"2\"><span class=\"tilte\">Select Configuration File</span></td>\n\
</tr>\n\
<tr>\n\
<td width=\"26%\"><select id=\"uploadfile\"  size=\"1\" name=\"_uploadfiletype\">\n\
<option selected>Configuration File</option>\n\
<option>Active Alarms List</option>\n\
<option>History Alarms List</option>\n\
<option>Battery Discharge Log</option>\n\
<option>Daily Statistic Log</option>\n\
</select></td>\n\
<td width=\"17%\"><input name=\"button\" type=\"button\" id=\"button\" onClick=\"upLoadTarFile()\" value=\"Upload\"  ></td>\n\
<td width=\"42%\">\n\
<form name=\"_uploadfile\" method=\"post\" encType=\"multipart/form-data\"><input id=\"_fileselect\" type=\"file\" size=\"40\" name=\"upfile\"></form></td>\n\
<td width=\"15%\"><input name=\"button\" type=\"button\" id=\"H1\" onClick=\"sendUploadCommand();\" value=\"Download\" ></td>\n\
</tr>\n\
</table>\n\
<table>\n\
<td><input  type=\"hidden\" MaxLength=\"15\" id=\"Z15\" NAME=\"20\" SIZE=\"15\">  <input name=\"button\" type=\"button\" id=\"H2\" onClick=\"GotoBoot();\" value=\"Go to Firmware Download Mode\" ></td>\n\
</table>\n\
<hr>\n\
<TABLE>\n\
<TBODY>\n\
<TR>\n\
<TD class=\"foot1\">Copyright&copy; 2016-2020 infyPower All Rights Reserved.</TD>\n\
</TR></TBODY></TABLE>\n\
<hr>\n\
</body>\n\
<script>\n\
function initdata()\n\
{\n\
var node = document.getElementById(\"A1\");\n\
node.innerHTML = \"System Voltage:  \"+top.da.strSystemVolt+\"V\";\n\
node = document.getElementById(\"A2\");\n\
node.innerHTML = \"System Load:  \"+top.da.strSystemLoad+\"A\";\n\
node = document.getElementById(\"A3\");\n\
node.innerHTML = \"MA/CA Number:  \"+top.da.strMANumber;\n\
node = document.getElementById(\"A4\");\n\
node.innerHTML = \"OA Number:  \"+top.da.strOANumber;\n\
node = document.getElementById(\"A5\");\n\
node.innerHTML = \"Battery Mode:  \"+top.da.strBattMode;\n\
node = document.getElementById(\"A6\");\n\
node.innerHTML = \"Site Name:  \"+top.da.strSiteName;\n\
node = document.getElementById(\"A7\");\n\
node.innerHTML = \"HW Version:  \"+top.da.strHWVersion;\n\
node = document.getElementById(\"A8\");\n\
node.innerHTML = \"SW Version:  \"+top.da.strSWVersion;\n\
node=document.getElementById(\"C1\");\n\
node.selectedIndex=top.da.varBatteryTempComp;\n\
node=document.getElementById(\"C2\");\n\
node.selectedIndex=top.da.varBatteryTest;\n\
node=document.getElementById(\"C3\");\n\
node.selectedIndex=top.da.varCurrentLimitation;\n\
node=document.getElementById(\"C4\");\n\
node.selectedIndex=top.da.varBoostCharge;\n\
node=document.getElementById(\"C5\");\n\
node.value=top.da.varUnderVoltageLevel1;\n\
node=document.getElementById(\"C6\");\n\
node.value=top.da.varUnderVoltageLevel2;\n\
node=document.getElementById(\"C7\");\n\
node.value=top.da.varLVDLevel1;\n\
node=document.getElementById(\"C8\");\n\
node.value=top.da.varLVDLevel2;\n\
node=document.getElementById(\"C9\");\n\
node.value=top.da.varSystemVolt1;\n\
node=document.getElementById(\"C10\");\n\
node.value=top.da.varBoostVolt1;\n\
node=document.getElementById(\"C11\");\n\
node.value=top.da.varTrapDestination1;\n\
node=document.getElementById(\"C12\");\n\
node.value=top.da.varTrapDestination2;\n\
node=document.getElementById(\"C13\");\n\
node.value=top.da.varIPAddress;\n\
node=document.getElementById(\"C15\");\n\
node.value=top.da.varDefaultGateway;\n\
node=document.getElementById(\"C14\");\n\
node.value=top.da.varSubnetMask;\n\
node=document.getElementById(\"D1\");\n\
node=document.getElementById(\"C20\");\n\
node.value=top.da.varEEM1;\n\
node=document.getElementById(\"C21\");\n\
node.value=top.da.varEEM2;\n\
node=document.getElementById(\"C16\");\n\
node.value=top.da.varSiteName;\n\
if(top.da.varUser==1)\n\
{\n\
node=document.getElementById(\"H1\");\n\
node.disabled = true;\n\
node=document.getElementById(\"H2\");\n\
node.disabled = true;\n\
}\n\
else\n\
{\n\
node=document.getElementById(\"H1\");\n\
node.disabled = false;\n\
node=document.getElementById(\"H2\");\n\
node.disabled = false;\n\
}\n\
}\n\
function initbody () {\n\
fh=setInterval(\"top.da.location.replace('footer.html');\", 5000);\n\
hh=setInterval(\"check_link();\", 30000);\n\
timer=0;\n\
initdata();\n\
}\n\
var foot=0;\n\
</script></html>\0"
};





static const char data_index_html[] = {
	/* /index.html */
	
"/index.html\0<html><head><title>InfyPower</title><meta http-equiv=\"Pragma\" content=\"no-cache\"><meta http-equiv=\"Cache-Control\" content=\"no-cache\"></head>\n\
<frameset rows=\"100%,0%,0%\">\n\
<frame src=\"header.html\" name=\"jm\" scrolling=\"yes\">\n\
<frame src=\"footer.html\" name=\"da\" scrolling=\"yes\">\n\
<frame src=\"files.html\" name=\"md\" scrolling=\"yes\">\n\
</frameset></html>\0"
};
//static const  char data_style_css[] = {
//	0x2f, 0x73, 0x74, 0x79, 0x6c, 0x65, 0x2e, 0x63, 0x73, 0x73, 0,
//	0x68, 0};
//  char data_tcp_shtml[1100];
//char data_tcp_shtml[1050];
char data_tcp_shtml[880] = {  
  "/back.html\0<html>\n\
<STYLE>\n\
body\n\
{\n\
font-size:small;\n\
background:#f1f1f1;\n\
}\n\
.message\n\
{\n\
font-size:105%;\n\
font-weight:bold;\n\
color:#000066;\n\
}\n\
</STYLE>\n\
<body onLoad=\"initbody();\">\n\
<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" height=\"100%\">\n\
<tr><td align=\"center\" valign=\"middle\">\n\
<table align=\"center\" valign=\"middle\">\n\
<tr><td><div class=\"message\" id=\"A1\"></div></td></tr>\n\
<tr>\n\
<td><input type=\"button\" size=\"50\" value=\"Back to the Homepage\" onClick=\"goback();\"></td></tr>\n\
</table></td></tr></table></body>\n\
<script>\n\
function initbody () \n\
{var node = document.getElementById(\"A1\");\n\
if(dd==1)\n\
node.innerHTML = \"Download Successful\";\n\
else\n\
node.innerHTML = \"Download Failed\";\n\
}\n\
function goback()\n\
{var k;\n\
k=window.open(\"/\",\"_blank\");\n\
if(k!=null)\n\
{ top.window.opener=null;\n\
top.window.close();\n\
} else\n\
{k.close();\n\
alert(\"Go to Web failed\");\n\
}\n\
}\n\
\0"

};

 void make_header_file(int download_flag)
 {
 
	char temp[30];
	memset(temp,0,30);

	sprintf(temp,"var dd=%d;</script></html>\0",download_flag);
	//memmove(da.tem,temp,30);
	//memmove(data_tcp_shtml+983,temp,30);
        
            //    memmove(data_tcp_shtml,data_tcp_shtml1,983);

	memmove(data_tcp_shtml+850,temp,30);

 }


	char data_fade_png[860]={
  "/files.html\0<html>\n\
<script>\n\
function SD(n1,v1,n2,v2,n3,v3,fg)\n\
{var x;\n\
if(fg)\n\
{x=document.getElementById(\"Y\");\n\
document.getElementById(\"Y1\").name=n1;\n\
document.getElementById(\"Y1\").value=v1;\n\
document.getElementById(\"Y2\").name=n2;\n\
document.getElementById(\"Y2\").value=v2;\n\
document.getElementById(\"Y3\").name=n3;\n\
document.getElementById(\"Y3\").value=v3;\n\
}\n\
else\n\
{x=document.getElementById(\"Z\");\n\
document.getElementById(\"Z1\").name=n1;\n\
document.getElementById(\"Z1\").value=v1;\n\
}\n\
x.submit();\n\
}\n\
</script>\n\
<body onload=\"T()\">\n\
<form id=\"Z\">\n\
<input TYPE=\"TEXT\" id=\"Z1\">\n\
</form>\n\
<form id=\"Y\">\n\
<input TYPE=\"TEXT\" id=\"Y1\">\n\
<input TYPE=\"TEXT\" id=\"Y2\">\n\
<input TYPE=\"TEXT\" id=\"Y3\">\n\
</form>\n\
</body>\n\
<script>\n\
function T() {\n\
if(S==1)\n\
alert(\"Setting Successful\");\n\
if(S==2)\n\
alert(\"Setting Failed\");\n\
if(S==3)\n\
alert(\"You are restricted to set\");\n\
S=0;\n\
}\0"
};

 //char data_fade_png[1000];

//x.submit();\n\
/************************************************/
/*函数名称:make_setdata_file(int setting_flag) 			                */
/*函数功能:做设置文件							        */
/*入口参数:setting_flag									                  */
/*出口参数:无									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
 void make_setdata_file(int setting_flag)
 {
	char temp[30];
	memset(temp,0,30);
	sprintf(temp,"var S=%d;</script></html>\0",setting_flag);
      //  memmove(data_fade_png,data_fade_temp,857);
	memmove(data_fade_png+830,temp,30);
	
 }
/*
static const unsigned char data_fade_png1[] = {
	// /fade.png
	0x2f, 0x66, 0x61, 0x64, 0x65, 0x2e, 0x70, 0x6e, 0x67, 0,
	0x4e, 0x44, 0xae, 0x42, 0x60, 0x82, 0};
*/


//const struct httpd_fsdata_file file_processes_shtml[] = {{NULL, data_processes_shtml, data_processes_shtml + 17, sizeof(data_processes_shtml) - 17}};

//const struct httpd_fsdata_file file_404_html[] = {{file_processes_shtml, (char *)data_404_html, (char *)data_404_html + 10, sizeof(data_404_html) - 10}};
const struct httpd_fsdata_file file_404_html[] = {{NULL, (char *)data_404_html, (char *)data_404_html + 10, sizeof(data_404_html) - 10}};
//const struct httpd_fsdata_file file_files_shtml[] = {{file_404_html, (char *)data_files_shtml, (char *)data_files_shtml + 12, sizeof(data_files_shtml) - 12}};

//const struct httpd_fsdata_file file_footer_html[] = {{file_files_shtml, data_footer_html, data_footer_html + 13, sizeof(data_footer_html) - 13}};
const struct httpd_fsdata_file file_footer_html[] = {{file_404_html, data_footer_html, data_footer_html + 13, sizeof(data_footer_html) - 13}};

const struct httpd_fsdata_file file_header_html[] = {{file_footer_html, data_header_html, data_header_html + 13, sizeof(data_header_html) - 13}};
const struct httpd_fsdata_file file_index_html[] = {{file_header_html, data_index_html, data_index_html + 12, sizeof(data_index_html) - 12}};

//const struct httpd_fsdata_file file_style_css[] = {{file_index_html, data_style_css, data_style_css + 11, sizeof(data_style_css) - 11}};

//const struct httpd_fsdata_file file_tcp_shtml[] = {{file_style_css, data_tcp_shtml, data_tcp_shtml + 11, sizeof(data_tcp_shtml) - 11}};
const struct httpd_fsdata_file file_tcp_shtml[] = {{file_index_html, data_tcp_shtml, data_tcp_shtml + 11, sizeof(data_tcp_shtml) - 11}};

const struct httpd_fsdata_file file_fade_png[] = {{file_tcp_shtml, data_fade_png, data_fade_png + 10, sizeof(data_fade_png) - 10}};

const struct httpd_fsdata_file file_stats_shtml[] = {{file_fade_png, data_stats_shtml, data_stats_shtml + 13, sizeof(data_stats_shtml) - 13}};

#define HTTPD_FS_ROOT file_stats_shtml


#define HTTPD_FS_NUMFILES 7
