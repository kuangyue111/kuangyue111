/**
 * \addtogroup apps
 * @{
 */

/**
 * \defgroup httpd Web server
 * @{
 * The uIP web server is a very simplistic implementation of an HTTP
 * server. It can serve web pages and files from a read-only ROM
 * filesystem, and provides a very small scripting language.

 */

/**
 * \file
 *         Web server
 * \author
 *         Adam Dunkels <adam@sics.se>
 */


/*
 * Copyright (c) 2004, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: httpd.c,v 1.2 2006/06/11 21:46:38 adam Exp $
 */

 /*
 *****************************************************************************
 *                                                                           *
 *  Filename:   httpd.c                                             *
 *  Date:       May 1, 2008    
 *  by caili
 *  Description:                                                             *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************/

#include "timer.h"
#include "uip.h"
#include "httpd.h"
#include "httpd-fs.h"
//#include "httpd-cgi.h"
#include "http-strings.h"
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "Http_download.h"
#define STATE_WAITING 0
#define STATE_OUTPUT  1
//#define STATE_WAITING 0
//#define STATE_OUTPUT  1
#define  STATE_POST    7 
#define ISO_nl      0x0a
#define ISO_space   0x20
#define ISO_bang    0x21
#define ISO_percent 0x25
#define ISO_period  0x2e
#define ISO_slash   0x2f
#define ISO_colon   0x3a
#define ISO_A1  0x3f
#define ISO_A2 0x3d
#define ISO_A3 0x2e
#define ISO_A4 0x26

#define MULBOUNDARYSIZE 50
#define MULLENTHSIZE 20
#define BANK1SIZE               0x00000400

#define FLAGFSTADD   0xFFFF
#define FLAGSECADD   0xFFFE

//Modified by hulw 090628 Set sitename on WEB
#define GO_TO_BOOTLAODER				30

#define SET_TEMPERATURE_COMPENSATION	1
#define SET_BATTERY_TEST				2
#define SET_CURRENT_LIMITATION			3
#define SET_BOOST_CHARGE				4
#define SET_UNDER_VOLTAGE_LEVEL1		5
#define SET_UNDER_VOLTAGE_LEVEL2		6
#define SET_LVD1_LEVEL					7
#define SET_LVD2_LEVEL					8
#define SET_SYSTEM_VOLTAGE				9
#define SET_BOOST_VOLTAGE				10

#define SET_TRAP_DESTINATION1			11
#define SET_TRAP_DESTINATION2			12
#define SET_TCPIP_ADDRESS				13
#define SET_SUBNET_MASK					14
#define SET_DEFAULT_GATEWAY				15
#define SET_SITE_NAME					16
#define SET_REMOTE1_IP					20
#define SET_REMOTE2_IP					21
/*#define LOCALFLAGFST      0x5A
#define LOCALFLAGSEC      0xA5
#define REMOTEFLAGFST     0xA5
#define REMOTEFLAGSEC     0x5A*/

//BSP_I2C_EEPROM_WriteByte(FLAGFSTADD, REMOTEFLAGFST);
//BSP_I2C_EEPROM_WriteByte(FLAGSECADD, REMOTEFLAGSEC);
struct timer download_timer;

struct HTTP_MUL_PART http_mul_part;

static char multipart_boundary[MULBOUNDARYSIZE + 100];
static char multipart_length[MULLENTHSIZE + 100];
static char new_multipart_boundary[MULLENTHSIZE + 100];
//INT8U BinBuffer[9*1024];
extern uip_ipaddr_t uip_trap[2 + 5];
extern NOINIT UCHAR data_404_html[8*1024+10];

/*---------------------------------------------------------------------------*/
typedef  struct rec_data
{
	int N;
	float f;
	char strSetStr[33];//Modified by hulw 090628 Set sitename on WEB
}rec_data;
typedef struct 
{
	UCHAR ipaddr[4];

}ipad;
typedef struct  
{
    ipad ipadress[3];
}ipwhole;

#define NOT_UP_LOAD_FILE 0
#define CONFIG_FILES    1
#define ACTIVE_ALARMS_FILE 2
#define HISTRORY_ALARMS_FILE 3
#define BATTERY_DISCHARGE_LOG 4
#define STATISTIC_LOG 5
/************************************************/
/*函数名称:strtoint    			          */
/*函数功能:把string转化为int  					        */
/*入口参数:  char *in,int begin,int end                                 */
/*出口参数:int              		                  */
/*资源:       					          */
/*子函数:	         								              */
/*更改日期:2008.4.1      						          */
/************************************************/
#define CMD_STRING_SIZE     128

// Exported macro & Common routines
#define IS_AF(c)	((c >= 'A') && (c <= 'F'))
#define IS_af(c)	((c >= 'a') && (c <= 'f'))
#define IS_09(c)	((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)	IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)	IS_09(c)
#define CONVERTDEC(c)	(c - '0')

#define CONVERTHEX_alpha(c)	(IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

/*******************************************************************************
* Function Name  : strbuffer
* Description    : This function is the standard string search. The Keil library
*                  does not provide it.  It looks for one string in another string
*                  and returns a pointer to it if found, otherwise returns NULL.
* Input          : psock - psock *psock
*                  string - char* needle
* Output         : None
* Return         : position of string - char*
*******************************************************************************/

static INT32U Str2Int(INT8U *inputstr, INT32S *intnum)
{
    INT32U i = 0, res = 0;
    INT32U val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                res = 1; /* return 1; */
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                /* return 0; Invalid input */
                res = 0;
                break;
            }
        }

        if (i >= 11) res = 0; /* over 8 digit hex --invalid */
    }
    else /* max 10-digit decimal input */
    {
        for (i = 0;i < 11;i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {

                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
                val = val * 10 + CONVERTDEC(inputstr[i]);
            else
            {
                /* return 0; Invalid input */
                res = 0;
                break;
            }
        }
        if (i >= 11) res = 0; /* Over 10 digit decimal --invalid */
    }

    return res;
}

/*******************************************************************************
* Function Name  : Set_data_buf_len
* Description    : 这个函数找到设置参数命令的实际长度
* Input          : char *in
* Output         : 数据的实际长度
* Return         : 
*******************************************************************************/
static int Set_data_buf_len(char *in)
{
	int i;
  	int len=strlen(in);
         
       for(i=0;i<len;i++)
	{
	  	if(*(in+i)==0x20)
		{
			len=i;
	  		break;
		}
	}
	return len;
	
}

/************************************************/
/*函数名称:strtoint  			                */
/*函数功能:把字符串从begin到end的字符转换为int	        */
/*入口参数:									                  */
/*出口参数:转换字符									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static int strtoint(char *in,int begin,int end)//chang string ti int
{
	int ip1;	
	char tr[5];
	int i=0,j=0;
	for(j=0,i=begin;i<end;i++,j++)
	{
		tr[j]=in[i];
	}
        tr[j]='\0';
	ip1=atoi(tr);
	return ip1;
}

/************************************************/
/*函数名称:strtofloat    			          */
/*函数功能:把string转化为float  					        */
/*入口参数:  char *in,int begin,int end                                 */
/*出口参数:float              		                  */
/*资源:       					          */
/*子函数:	         								              */
/*更改日期:2008.4.1      						          */
/************************************************/
static float strtofloat(char *in,int begin,int end)//change 
{
	float ip1;	
	char tr[6];//Modified by hulw 090628 提高健壮性
	int i=0,j=0;
	for(j=0,i=begin;i<end && j < 5;i++,j++)//Modified by hulw 090628 提高健壮性
	{
		tr[j]=in[i];
	}
        tr[j]='\0';
   	 ip1=atof(tr);
	return ip1;
}

/************************************************/
/*函数名称:strtoip    			          */
/*函数功能:把string转化为ip					        */
/*入口参数:  char *in,ipwhole *ip                               */
/*出口参数:int              		                  */
/*资源:       					          */
/*子函数:	         								              */
/*更改日期:2008.4.1      						          */
/************************************************/

static int  strtoip(char *in,ipwhole *ip)
{
	int len=Set_data_buf_len(in);
	int N=0;
	int iBegin,iEnd;//
	iBegin=0;
	iEnd=0;
	int flag=0;
        int flag1=1;
	int p=0;//the p of the ip
	int q=0;//the q of the ip
	while(iEnd<=len)
	{
                 if(in[iEnd]==ISO_A1)
		{
			iBegin=iEnd+1;


		}
                if((in[iEnd]==ISO_A2)&&flag1)
                {
                      if(iEnd>iBegin)
			{
			N=strtoint(in,iBegin,iEnd);
			}
                }
		if((in[iEnd]==ISO_A2)||flag)
		{
			if(in[iEnd]==ISO_A2)//find the equal
			{
				flag=1;//find the equal
				iBegin=iEnd+1;

			}
			if(in[iEnd]==ISO_A3)//find the point
			{
				ip->ipadress[q].ipaddr[p]=strtoint(in,iBegin,iEnd);
				iBegin=iEnd+1;
				p++;

			}
			if(in[iEnd]==ISO_A4||iEnd==len)//find the &
			{
				ip->ipadress[q].ipaddr[p]=strtoint(in,iBegin,iEnd);
				p=0;
				flag=0;//a new ip
				q++;

			}
		}
		iEnd++;
	}
        return N;
}

/************************************************/
/*函数名称:strtodata    			          */
/*函数功能:把string转化为data					        */
/*入口参数:  char *in,rec_data *t                               */
/*出口参数:void              		                  */
/*资源:       					          */
/*子函数:	         								              */
/*更改日期:2008.4.1      						          */
/************************************************/

static void strtodata(char *in,rec_data *t)//chang the string inte data
{
	int iBegin = 0,iEnd = 0;//Modified by hulw 090628 提高健壮性
	int len=Set_data_buf_len(in);

	for(int i=0;i< len;i++)
	{
		if(in[i]==ISO_A1)
		{
			iBegin=i+1;


		}
		if(in[i]==ISO_A2)
		{
			iEnd=i;
			if(iEnd>iBegin)
			{
				t->N=strtoint(in,iBegin,iEnd);
			}
			iBegin=i+1;
				break;
		}
	}
	iEnd=len;
    	t->f=strtofloat(in,iBegin,iEnd);
	memcpy(t->strSetStr, in + iBegin, MIN(iEnd - iBegin, sizeof(t->strSetStr) - 1));
	t->strSetStr[sizeof(t->strSetStr) - 1] = '\0';
	return ;

}


#define TRAP1_NUM 11
#define TRAP2_NUM 12
/************************************************/
/*函数名称:set_trap_ip  			                */
/*函数功能:	        */
/*入口参数:									                  */
/*出口参数:转换字符									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static int set_trap_ip(ipwhole *ip,int num)//设置IP
{
  __XXX_UNION_VALUE unVal;
	int  seq = 0;
	UCHAR ip1[4];
	int p = 0;
	float  ftemp = 0;
        ftemp = ftemp;
	for(p=0;p<4;p++)//get the  ip
	{
		//GetCharSetData(SETDATA_IP_NUM+p,&ip1[p]);
                ExGetSigInfo(E_TYPE_SYS, ID_u8IP_1 + p, &unVal, NULL, BY_SRAM);
                 ip1[p]= unVal.chtemp[0]; 
	}

	//can/t be the same ip
	if((ip->ipadress[0].ipaddr[0]==ip1[0])
		&&(ip->ipadress[0].ipaddr[1]==ip1[1])
		&&(ip->ipadress[0].ipaddr[2]==ip1[2])
		&&(ip->ipadress[0].ipaddr[3]==ip1[3]))

		return  SET_DATA_FAIL;

	//can't be the 127.0.0.1
	if((ip->ipadress[0].ipaddr[0]==0x7f)
		&&(ip->ipadress[0].ipaddr[1]==0)
		&&(ip->ipadress[0].ipaddr[2]==0)
		&&(ip->ipadress[0].ipaddr[3]==1))
		return  SET_DATA_FAIL;

	if (num==TRAP1_NUM)
	{
		//第一个trap_ip的设置

		for(int i=0;i<4;i++)
		{
			//SetCharSetData(SETDATA_NMSIP_NUM+i,&(ip->ipadress[0].ipaddr[i]));
		}
		seq = 0;
	}

	if(num==TRAP2_NUM)
	{
		//第二个trap ip的设置

		for(int i=0;i<4;i++)
		{
			//SetCharSetData(SETDATA_NMSIP_NUM+i+6,&(ip->ipadress[0].ipaddr[i]));
		}
		seq = 1;
	}

	gs_trap.uip_trap[seq][0]=(ip->ipadress[0].ipaddr[0])+(ip->ipadress[0].ipaddr[1]*256);
	gs_trap.uip_trap[seq][1]=(ip->ipadress[0].ipaddr[2])+(ip->ipadress[0].ipaddr[3]*256);

	uip_ipaddr(&ftemp,ip->ipadress[0].ipaddr[0],ip->ipadress[0].ipaddr[1],ip->ipadress[0].ipaddr[2],ip->ipadress[0].ipaddr[3]);

	//RecordEvent(HISTORY_EVENT_DOWNLOAD_FROM_NETWORK, SETDATA_NMSIP_NUM+6*seq, 0,ftemp);
	return  SET_DATA_SUCCESS;
}


/************************************************/
/*函数名称:set_eem_ip  			                */
/*函数功能:	        */
/*入口参数:									                  */
/*出口参数:转换字符									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
#define EEM_1  20
#define EEM_2  21

extern uip_ipaddr_t eem_ipaddr[2];

static int set_EEM_ip(ipwhole *ip,int num)//设置IP
{

	__XXX_UNION_VALUE unVal;
	UCHAR ip1[4];
	int p = 0;
	int i=0;
//can't be the 127.0.0.1
	if((ip->ipadress[0].ipaddr[0]==0x7f)
			&&(ip->ipadress[0].ipaddr[1]==0)
			&&(ip->ipadress[0].ipaddr[2]==0)
			&&(ip->ipadress[0].ipaddr[3]==1))
			return  SET_DATA_FAIL;
				  
	if (num==EEM_1)
	{
		//第一个trap_ip的设置
			
		for(i=0;i<4;i++)
		{
			//SetCharSetData(SETDATA_PowerStarIP_NUM+i,&(ip->ipadress[0].ipaddr[i]));
			unVal.chtemp[0] = (ip->ipadress[0].ipaddr[i]);
			ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + i, unVal, NULL, BY_E2PROM);
		}

		for(p=0;p<4;p++)//get the  eem_ip1
		{
			//GetCharSetData(SETDATA_PowerStarIP_NUM+p,&ip1[p]);
			//unVal.chtemp[0] = (ip1[p]);
			ExGetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + p, &unVal, NULL, BY_E2PROM);
			ip1[p] = unVal.chtemp[0];
		}
		uip_ipaddr(eem_ipaddr[0], ip1[0],ip1[1],ip1[2],ip1[3]);
	}
	
	if(num==EEM_2)
	{
		//第二个trap ip的设置
		for(int i=0;i<4;i++)
		{
			//SetCharSetData(SETDATA_PowerStarIP_NUM+i+6,&(ip->ipadress[0].ipaddr[i]));
			unVal.chtemp[0] = (ip->ipadress[0].ipaddr[i]);
			ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + i, unVal, NULL, BY_E2PROM);
		}
		for(p=0;p<4;p++)//get the  eem_ip1
		{
			//GetCharSetData(SETDATA_PowerStarIP_NUM+6+p,&ip1[p]);
			ExGetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + p, &unVal, NULL, BY_E2PROM);
			ip1[p] = unVal.chtemp[0];
		}
		uip_ipaddr(eem_ipaddr[1], ip1[0],ip1[1],ip1[2],ip1[3]);
	}
	
	return  SET_DATA_SUCCESS;
}

/************************************************/
/*函数名称:set_ip  			                */
/*函数功能:	从flash中读出配置，进行设置        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
void  set_ip(void )
{
	int i;
	__XXX_UNION_VALUE unVal;
	uip_ipaddr_t ipaddr[2]; 
	UCHAR ip1[4];

	for(i=0;i<4;i++)//get the  ip
	{
		//GetCharSetData(SETDATA_IP_NUM+i,&ip1[i]);
		ExGetSigInfo(E_TYPE_SYS, ID_u8IP_1 + i, &unVal, NULL, BY_E2PROM);
		ip1[i] = unVal.chtemp[0];
	}
	// set the ip
	uip_ipaddr(ipaddr, ip1[0],ip1[1],ip1[2],ip1[3]);
	uip_sethostaddr(ipaddr);


	for(i=0;i<4;i++)//get the mask
    {
      //GetCharSetData(SETDATA_Subnetmask_NUM+i,&ip1[i]);
		ExGetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_1 + i, &unVal, NULL, BY_E2PROM);
		ip1[i] = unVal.chtemp[0];
    }
	//set the mask
	uip_ipaddr(ipaddr, ip1[0],ip1[1],ip1[2],ip1[3]);
	uip_setnetmask(ipaddr);


	for(i=0;i<4;i++)//get the gateway
	{
		//GetCharSetData(SETDATA_Gateway_NUM+i,&ip1[i]);
		ExGetSigInfo(E_TYPE_SYS, ID_u8Gateway_1 + i, &unVal, NULL, BY_E2PROM);
		ip1[i] = unVal.chtemp[0];
	}
	//set the draddr
	uip_ipaddr(ipaddr, ip1[0],ip1[1],ip1[2],ip1[3]);
	uip_setdraddr(ipaddr);

}

/************************************************/
/*函数名称:set_whole_ip  			                */
/*函数功能:	把ip存下去在设下去        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static int set_whole_ip(ipwhole *ip)//设置IP
{
	int i;
	float ftemp;
	ftemp = 0;
	ftemp = ftemp;
	__XXX_UNION_VALUE unVal;

	for(i=0;i<3;i++)
		{
		if((ip->ipadress[i].ipaddr[0]==127)
			&&(ip->ipadress[i].ipaddr[1]==0)
			&&(ip->ipadress[i].ipaddr[2]==0)
			&&(ip->ipadress[i].ipaddr[3]==1))
			return  SET_DATA_FAIL;
		}
	//set the ip
	for(i=0;i<4;i++)
	{
		//SetCharSetData(SETDATA_IP_NUM+i,&(ip->ipadress[0].ipaddr[i]));
		unVal.chtemp[0] = (ip->ipadress[0].ipaddr[i]);
		ExSetSigInfo(E_TYPE_SYS, ID_u8PowerStarIP_1 + i, unVal, NULL, BY_E2PROM);
	}
	//set the mask
	 for(i=0;i<4;i++)
	{
		//SetCharSetData(SETDATA_Subnetmask_NUM+i,&(ip->ipadress[1].ipaddr[i]));
		unVal.chtemp[0] = (ip->ipadress[1].ipaddr[i]);
		ExSetSigInfo(E_TYPE_SYS, ID_u8Subnetmask_1 + i, unVal, NULL, BY_E2PROM);
	}
	//set the gateway
	for(i=0;i<4;i++)
	{
		// SetCharSetData(SETDATA_Gateway_NUM+i,&(ip->ipadress[2].ipaddr[i]));
		unVal.chtemp[0] = (ip->ipadress[2].ipaddr[i]);
		ExSetSigInfo(E_TYPE_SYS, ID_u8Gateway_1 + i, unVal, NULL, BY_E2PROM);
		
	}
	//set the ip,mask,gateway into the web  
	set_ip();

	uip_ipaddr(&ftemp,ip->ipadress[2].ipaddr[0],ip->ipadress[2].ipaddr[1],ip->ipadress[2].ipaddr[2],ip->ipadress[2].ipaddr[3]);
	
	 //RecordEvent(HISTORY_EVENT_DOWNLOAD_FROM_NETWORK, SETDATA_Gateway_NUM, 0,ftemp);   
 	return  SET_DATA_SUCCESS;
  
}

typedef int(*OUTINT_INTERFACE_PROC)(rec_data  *t);
typedef struct Set_Data_Interface
{
	int   seqno;
	OUTINT_INTERFACE_PROC fproc;
	
}I2CSet_Data_Interface;

/************************************************/
/*函数名称:check_set_data_SETDATA_DCLowVolt_NUM  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
/*undervol1   */
static int check_set_data_SETDATA_DCLowVolt_NUM(rec_data *t)
{
		UCHAR  u_temp;
		//float  f_temp;
	GetCharSetData(SETDATA_SysType_NUM, &u_temp);
	if(u_temp<5)
	{
		if((t->f<20)||(t->f>30))
			return SETTING_FAIL;
	}
	else //if(u_temp>=5)
	{
		if((t->f<40)||(t->f>60))
			return SETTING_FAIL;
	}
		
		//GetCharSetData(SETDATA_BattType_NUM, &u_temp);
		//GetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM+1,&f_temp);
		//if(t->f<f_temp)
		//	return SETTING_FAIL;
		//	GetFloatSetData(2*u_temp+SETDATA_FCVolt_NUM,&f_temp);
		//GetFloatSetData(2*u_temp+SETDATA_DCOverVolt_NUM,&f_temp);
		//if(t->f>f_temp)
		//	return SETTING_FAIL;
		//else
		//SetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM,&t->f);
		//RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK, 2*u_temp+SETDATA_DCLowVolt_NUM, 0,t->f);
		return SETTING_SUCCESS;
			
}


/************************************************/
/*函数名称:check_set_data_SETDATA_DCLowVolt_NUM_11  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
/*undervol2   */
static int check_set_data_SETDATA_DCLowVolt_NUM_11(rec_data *t)
{
	UCHAR  u_temp;
	//float  f_temp;
	GetCharSetData(SETDATA_SysType_NUM, &u_temp);
	if(u_temp<5)
	{
		if((t->f<20)||(t->f>30))
			return SETTING_FAIL;
	}
	else //if(u_temp>=5)
	{
		if((t->f<40)||(t->f>60))
			return SETTING_FAIL;
	}
	// GetCharSetData(SETDATA_BattType_NUM, &u_temp);
	//GetFloatSetData(SETDATA_BattCutVolt_NUM+u_temp,&f_temp);
	//if(t->f<f_temp)
	//	return SETTING_FAIL;
	//GetFloatSetData(SETDATA_LoadCutVolt_NUM+u_temp,&f_temp);
	//if(t->f<f_temp)
	//	return SETTING_FAIL;

			
	//GetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM,&f_temp);
	//if(t->f>f_temp)
	//	return SETTING_FAIL;
	//else
		//SetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM+1,&t->f);
	//RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK, 2*u_temp+SETDATA_DCLowVolt_NUM+1, 0,t->f);
	return SETTING_SUCCESS;
}

/************************************************/
/*函数名称:check_set_data_SETDATA_LoadCutVolt_NUM  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
/*LVD1 level*/
static int check_set_data_SETDATA_LoadCutVolt_NUM(rec_data *t)
{
			UCHAR  u_temp;
		//float  f_temp;
		GetCharSetData(SETDATA_SysType_NUM, &u_temp);
		if(u_temp<5)
		{
            if((t->f<20)||(t->f>30))
                return SETTING_FAIL;
		}
		else //if(u_temp>=5)
		{
            if((t->f<40)||(t->f>60))
                return SETTING_FAIL;
		}
		//GetCharSetData(SETDATA_BattType_NUM, &u_temp);
		//GetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM+1,&f_temp);

		//if(t->f>f_temp)
		//	return SETTING_FAIL;
		//else
		//SetFloatSetData(SETDATA_LoadCutVolt_NUM+u_temp,&t->f);
        //RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK,SETDATA_LoadCutVolt_NUM+u_temp, 0,t->f);			
	    return SETTING_SUCCESS;
}


/************************************************/
/*函数名称:check_set_data_SETDATA_BattCutVolt_NUM  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
/*LVD2 level*/
static int check_set_data_SETDATA_BattCutVolt_NUM(rec_data *t)
{
				UCHAR  u_temp;
		//float  f_temp;
		GetCharSetData(SETDATA_SysType_NUM, &u_temp);
		if(u_temp<5)
		{
            if((t->f<20)||(t->f>30))
                return SETTING_FAIL;
		}
		else //if(u_temp>=5)
		{
            if((t->f<40)||(t->f>60))
                return SETTING_FAIL;
		}
		//GetCharSetData(SETDATA_BattType_NUM, &u_temp);
		//GetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM+1,&f_temp);

			//if(t->f>f_temp)
			//return SETTING_FAIL;
				//else
				//SetFloatSetData(SETDATA_BattCutVolt_NUM+u_temp,&t->f);
				//RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK,SETDATA_BattCutVolt_NUM+u_temp, 0,t->f);
			 	return SETTING_SUCCESS;
		
}

/************************************************/
/*函数名称:check_set_data_SETDATA_FCVolt_NUM  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
#define UNDER_VOLTAGE     65
#define OVER_OVER_VOLTAGE  88
/*system volt*/
static int check_set_data_SETDATA_FCVolt_NUM(rec_data *t)
{		
			UCHAR  u_temp;
		//float  f_temp;
		GetCharSetData(SETDATA_SysType_NUM, &u_temp);
	if(u_temp<5)
	{
		if((t->f<21)||(t->f>29))
			return SETTING_FAIL;
	}
	else //if(u_temp>=5)
	{
		if((t->f<42)||(t->f>58))
			return SETTING_FAIL;
	}
        
        //2011-05-06 太阳能模式下:浮充电压设置下限必须比下电恢复电压点高0.5V，以防无法上电
		//GetCharSetData(SETDATA_SysWorkMode_NUM, &u_temp);
		if(u_temp != SYSMODE_AC)
		{
			//GetFloatSetData(SETDATA_RLVDVolt_NUM, &f_temp);
			//f_temp += RECT_FCVOLT_HIGHTHAN_LVDRSTVOLT;
			//if(t->f < f_temp)
				return SETTING_FAIL;
		}
		
			//GetCharSetData(SETDATA_BattType_NUM, &u_temp);
		/*	GetFloatSetData(2*u_temp+SETDATA_DCLowVolt_NUM,&f_temp);
			if(t->f<f_temp)
				return SETTING_FAIL;*/
			
			//GetFloatSetData(SETDATA_ECVolt_NUM+u_temp,&f_temp);
			//if(t->f>f_temp)
			//	return SETTING_FAIL;
			//else
				//SetFloatSetData(SETDATA_FCVolt_NUM+u_temp,&t->f);
			//RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK, SETDATA_FCVolt_NUM+u_temp, 0,t->f);
			 	return SETTING_SUCCESS;
		
}

/************************************************/
/*函数名称:check_set_data_SETDATA_ECVolt_NUM  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
/*boost volt*/
static int check_set_data_SETDATA_ECVolt_NUM(rec_data *t)
{		
					UCHAR  u_temp;
		//float  f_temp;
		GetCharSetData(SETDATA_SysType_NUM, &u_temp);
		if(u_temp<5)
		{
            if((t->f<21)||(t->f>29))
                return SETTING_FAIL;
		}
	    else //if(u_temp>=5)
		{
            if((t->f<42)||(t->f>58))
                return SETTING_FAIL;
		}
		//GetCharSetData(SETDATA_BattType_NUM, &u_temp);
		//GetFloatSetData(SETDATA_FCVolt_NUM+u_temp,&f_temp);
		//if(t->f<f_temp)
			//return SETTING_FAIL;
		/*	GetFloatSetData(OVER_OVER_VOLTAGE+2*u_temp,&f_temp);
			if(t->f>f_temp)
				return SETTING_FAIL;*/
		//else
			//SetFloatSetData(SETDATA_ECVolt_NUM+u_temp,&t->f);
		//RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK, SETDATA_ECVolt_NUM+u_temp, 0,t->f);
		return SETTING_SUCCESS;
		
}


/************************************************/
/*函数名称:I2CSet_Data_Interface  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
I2CSet_Data_Interface Set_Tab[11]={
{0},
{SETDATA_TempCompEnable_NUM},
{SETDATA_TimeTestEnable_NUM},
{SETDATA_CurrLimitMode_NUM},
{SETDATA_TimeECEnable_NUM},
{SETDATA_DCLowVolt_NUM,check_set_data_SETDATA_DCLowVolt_NUM},
{SETDATA_DCLowVolt_NUM+1,check_set_data_SETDATA_DCLowVolt_NUM_11},
{SETDATA_LoadCutVolt_NUM,check_set_data_SETDATA_LoadCutVolt_NUM},
{SETDATA_BattCutVolt_NUM,check_set_data_SETDATA_BattCutVolt_NUM},
{SETDATA_FCVolt_NUM,check_set_data_SETDATA_FCVolt_NUM},
{SETDATA_ECVolt_NUM,check_set_data_SETDATA_ECVolt_NUM}
};


/************************************************/
/*函数名称:int  set_data(rec_data *t) 			                */
/*函数功能:设置数据下去        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
//Modified by hulw 090628 Set sitename on WEB
//#define GO_TO_BOOTLAODER   20
static int  set_data(rec_data *t,struct httpd_state *s)
{
	/*it is bootloader command*/
	if(t->N==GO_TO_BOOTLAODER)
		{

		if(s->passflag == ADMIN_USER)
		{

			
		  UCHAR boot_flag[2];
                  boot_flag[0] = REMOTEFLAGFST;
                  boot_flag[1] = REMOTEFLAGSEC;
                  dv_E2PROM.fWriteE2PROMPage(boot_flag, E2PROMFLAGADD1, 2);
		Trap();
		return SET_DATA_SUCCESS;
			}
		else
			return  SET_DATA_FAIL;
		}
	else
		{
	if(t->N<0||t->N>10)
		return	SET_DATA_FAIL;
  	if(t->N<=4)
  	{
  		UCHAR temp;
 		 temp=(UCHAR)t->f;
  		SetCharSetData(Set_Tab[t->N].seqno,&temp);
		//RecordEvent(HISTORY_EVENT_SETTING_FROM_NETWORK, Set_Tab[t->N].seqno, temp,0);
     		return SET_DATA_SUCCESS;
  	}
  	else
  	{
  		if(Set_Tab[t->N].fproc(t))
  		{
  			//SetFloatSetData(Set_Tab[t->N].seqno,&t->f);
			return SET_DATA_SUCCESS;
  		}
		else
			return SET_DATA_FAIL;
  	}
		}
  
}


/************************************************/
/*函数名称:int process(char *in)  			                */
/*函数功能:	检查需要设置的数据        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/

#define setting_data_cmd  22
#define setting_ip_cmd  35


static int process(struct httpd_state *s)
{
	  rec_data  data;
	  ipwhole  ip1;
         int num;
         int len=Set_data_buf_len(s->inputbuf);
         
       /*it is setting command*/
    //     if(BSP_Ver_Status()==FALSE)
   //      {
   //        return SET_NOT_RIGHT;
   //      }
   //      else
         {
		  	 //Modified by hulw 090628 Set sitename on WEB
			 memset(&data,0,sizeof(data));
			 strtodata(s->inputbuf,&data);

			 memset(&ip1,0,sizeof(ip1));

			 if (data.N == GO_TO_BOOTLAODER 
				 || (data.N >= SET_TEMPERATURE_COMPENSATION 
				 && data.N <= SET_BOOST_VOLTAGE))
			 {
				 return set_data(&data,s);
			 }
			 else if (data.N == SET_TRAP_DESTINATION1
				 || data.N == SET_TRAP_DESTINATION2)
			 {
				 num=strtoip(s->inputbuf,&ip1);
				 return set_trap_ip(&ip1,num);
			 }
			 else if (data.N == SET_REMOTE1_IP
				 || data.N == SET_REMOTE2_IP)
			 {
				 num=strtoip(s->inputbuf,&ip1);
				 return set_EEM_ip(&ip1,num);
			 }
			 else if (data.N == SET_TCPIP_ADDRESS)
			 {
				 strtoip(s->inputbuf,&ip1);
				 return set_whole_ip(&ip1);
			 }
			 else if (data.N == SET_SITE_NAME)
			 {
				 data.strSetStr[32] = '\0';

				 for(int i = 0; i < 33; i++)
				 {
					 //SetCharSetData(SETDATA_SiteName_NUM+i, data.strSetStr + i);
				 }

				 return  SET_DATA_SUCCESS;
			 }
			 else
			 {
				 return SET_DATA_FAIL;

			 }
		}
}

/*---------------------------------------------------------------------------*/

static const unsigned char pr2six[256] =
{
        	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63, 52, 53, 54,
		55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64, 64, 0, 1, 2, 3,
		4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
		21,22, 23, 24, 25, 64,  64, 64, 64, 64, 64, 26, 27, 28, 29, 30, 31, 
		32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
		50, 51, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

/*---------------------------------------------------------------------------*/
 char compare[]={"Authorization: Basic "};
 char bufplain[100];

password pass[2]={
	{"admin"},
	{"operator"}
};

/************************************************/
/*函数名称:char * ap_uudecode(const char *bufcoded) 			                */
/*函数功能:	密码检查        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static char * ap_uudecode(const char *bufcoded)
{
    int nbytesdecoded;
     const unsigned char *bufin;
    //register char *bufplain;
     unsigned char *bufout;
     int nprbytes;
    /* Strip leading whitespace. */
	
    while (*bufcoded == ' ' || *bufcoded == '\t')
		bufcoded++;
	
	/* Figure out how many characters are in the input buffer.
	 * Allocate this many from the per-transaction pool for the result.
	 */
    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
//	bufplain =(char *)malloc(100);
    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;
    while (nprbytes > 0) {
		*(bufout++) =
			(unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		*(bufout++) =
			(unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		*(bufout++) =
			(unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		bufin += 4;
		nprbytes -= 4;
    }
	
    if (nprbytes & 03) {
		if (pr2six[bufin[-2]] > 63)
			nbytesdecoded -= 2;
		else
			nbytesdecoded -= 1;
    }
    bufplain[nbytesdecoded] = '\0';

    return bufplain;
}


/************************************************/
/*函数名称:int Get_UserPass( char* buf, char* username, char* passwd ) 			                */
/*函数功能:	密码检查        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/

static int Get_UserPass( char* buf, char* username, char* passwd )
{
	int i,len,tag;
	int passlen;
	
	tag = 0;
	len = strlen( buf );
        if(len > 18)
          return -1;
	//printf("the len is %d\n",len);
#define MAX_CODELEN
	for( i =0; i< len ;i++ ) {
		if( tag == 0 && *(buf+i) != ':' )
			continue;
		if( *(buf+i) == ':' )
			tag = i;
		if( tag != 0 ) {
			memcpy( username,buf,tag );
			username[tag] = 0;
			passlen = len - tag -1;
			memcpy( passwd,buf+tag+1,passlen );
			passwd[passlen] = 0;
			return 0;
		}
	}
	return -1;
}



/************************************************/
/*函数名称:int CheckSecurity_post( char*username,char* passwd)		                */
/*函数功能:	密码检查        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/

static int CheckSecurity( char*username,char* passwd)
{
    for(int i=0;i<2;i++)
	{
          
			if((!strcmp((const char *)pass[i].admin,(const char *)username))
                           &&(!strcmp((const char *)pass[i].password1,(const char *)passwd)))
		
                        {
                          if(i==0)
                            return   ADMIN_USER;
                          if(i==1)
                            return   OPERATOR_USER;
                           
                        }

	}
    return -1;
}

/************************************************/
/*函数名称:char *my_strstr(char *str,char *sub_str)		                */
/*函数功能:	密码检查        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static char *my_strstr(char *str,char *sub_str)
{
       int i=0,j=0;
        while(str[i]!='\0'&&sub_str[j]!='\0') {
                if(str[i]==sub_str[j]) {
                        i++;
                        j++;
                }
                else {
                        i=i-j+1;
                        j=0;
                }
        }
        if(sub_str[j]=='\0')
                return (char *)(str+i-j);
        return NULL;
}
/*---------------------------------------------------------------------------*/


static unsigned short
generate_part_of_file(void *state)
{
  struct httpd_state *s = (struct httpd_state *)state;

  if(s->file.len > uip_mss()) {
    s->len = uip_mss();
  } else {
    s->len = s->file.len;
  }
  memcpy(uip_appdata, s->file.data, s->len);
  
  return s->len;
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_file(struct httpd_state *s))
{
  PSOCK_BEGIN(&s->sout);
  PT_YIELD_FLAG=PT_YIELD_FLAG;
  do {
    PSOCK_GENERATOR_SEND(&s->sout, generate_part_of_file, s);
    s->file.len -= s->len;
    s->file.data += s->len;
  } while(s->file.len > 0);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_headers(struct httpd_state *s, const char *statushdr))
{
  char *ptr;

  PSOCK_BEGIN(&s->sout);
  PT_YIELD_FLAG = PT_YIELD_FLAG;
  PSOCK_SEND_STR(&s->sout, statushdr);
    if(s->uploadflag==0)  // 
  {
  ptr = strrchr(s->filename, ISO_period);
  if(ptr == NULL) {
    PSOCK_SEND_STR(&s->sout, http_content_type_binary);
  } else if(strncmp(http_html, ptr, 5) == 0 ||
	    strncmp(http_shtml, ptr, 6) == 0) {
    PSOCK_SEND_STR(&s->sout, http_content_type_html);
  } else if(strncmp(http_css, ptr, 4) == 0) {
    PSOCK_SEND_STR(&s->sout, http_content_type_css);
  } else if(strncmp(http_png, ptr, 4) == 0) {
    PSOCK_SEND_STR(&s->sout, http_content_type_png);
  } else if(strncmp(http_gif, ptr, 4) == 0) {
    PSOCK_SEND_STR(&s->sout, http_content_type_gif);
  } else if(strncmp(http_jpg, ptr, 4) == 0) {
    PSOCK_SEND_STR(&s->sout, http_content_type_jpg);
  } else {
    PSOCK_SEND_STR(&s->sout, http_content_type_plain);
  }
	}
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_headers1(struct httpd_state *s, const char *statushdr))
{
 

  PSOCK_BEGIN(&s->sout);
  PT_YIELD_FLAG = PT_YIELD_FLAG;
  PSOCK_SEND_STR(&s->sout, statushdr);


  PSOCK_END(&s->sout);
}

const char filedataname_footer[]={"/footer.html"};


/************************************************/
/*函数名称:(handle_output(struct httpd_state *s))		                */
/*函数功能:	处理输出        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static
PT_THREAD(handle_output(struct httpd_state *s))
{
  char *ptr;
  
  PT_BEGIN(&s->outputpt);
  PT_YIELD_FLAG = PT_YIELD_FLAG;
  
 
  if(!httpd_fs_open(s->filename, &s->file)) {
    httpd_fs_open(http_404_html, &s->file);
    strcpy(s->filename, http_404_html);
    PT_WAIT_THREAD(&s->outputpt,
		   send_headers(s,
		   http_header_404));
    PT_WAIT_THREAD(&s->outputpt,
		   send_file(s));
  } else 
  {
  			
  
	      if(s->passflag==NOT_PASS_AUTHORIZTON)
    		{
    		//没有通过密码验证
    			PT_WAIT_THREAD(&s->outputpt,
		   		send_headers1(s,
		   		http_header_password));
    			s->passflag=NOT_PASS_AUTHORIZTON;
    		}
		else
		{
		//通过密码验证
  			if(s->uploadflag!=NOT_UP_LOAD_FILE)//have file to upload
    			{
    			//是上传文件
				    PT_WAIT_THREAD(&s->outputpt,
				   send_headers(s,
					   http_header_upload));
			 }
		    else
  			  {
  			  //不是上传文件
 		   		PT_WAIT_THREAD(&s->outputpt,
			 	  send_headers(s,
			   	http_header_200));
			 }
  		  		ptr = strchr(s->filename, ISO_period);
  		 	 if(ptr != NULL && strncmp(ptr, http_shtml, 6) == 0) {
    	 // PT_INIT(&s->scriptpt);
     	//	 PT_WAIT_THREAD(&s->outputpt, handle_script(s));
   				 } else {
   				 	if (strncmp(s->filename, filedataname_footer, sizeof(filedataname_footer)) == 0)
   				 		{
   				 make_data_file(s->passflag);
				
   				 		}
     			 PT_WAIT_THREAD(&s->outputpt,
		     send_file(s));

    			}
  		}
  	}
  memset(s->inputbuf,0,HTTP_SATAE_INPUTBUF_SIZE);
  //memset(s->inputbuf,0,sizeof(s->inputbuf));
		
  PSOCK_CLOSE(&s->sout);
  PT_END(&s->outputpt);
}

/************************************************/
/*函数名称:HexToAscii1(unsigned char Input)		                */
/*函数功能:	HexToAscii1        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/

static unsigned char	HexToAscii1(unsigned char Input)
{

	 if(Input < 10)
        return Input + 0x30;  
        else if(Input < 36)
         return Input + 55;    //0x61='A-Z'
        else if(Input < 62)
        return Input + 61;      //c[j] += 61;//0x61='a-z'
        else //if(uc[j] >= 62)
        {
          Input= 0x00;
          return Input;
        }
	
}

/************************************************/
/*函数名称:HexToAscii1(unsigned char Input)		                */
/*函数功能:	HexToAscii1        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static void boundary_pick_up(const char *boundary,char * new_boundary)
{
	int flag=0;
	int j=0;
	int k=strlen(boundary);
	for(int i=0;i<k;i++)
	{
		if(boundary[i]!='-')
		{
			flag=1;
		}
		if((boundary[i]=='\r')&&(flag))
		{
			flag=0;
			return;
		}

		if(flag)
		{
			new_boundary[j] = boundary[i];
			j++;
		}
	}
}
/*----------------------------------------------------------------*/
/************************************************/
/*函数名称:HexToAscii1(unsigned char Input)		                */
/*函数功能:	HexToAscii1        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static void Get_password(void)
{
              UCHAR  u_temp; 
		int i=0; 
		for(i=0;i<8;i++)
		{
		 GetCharSetData(SETDATA_PassWord3_NUM+i,&u_temp);
		 pass[0].password1[i]=HexToAscii1(u_temp);
		}
		pass[0].password1[7]='\0';
		for(i=6;i>=0;i--)
		{
			if(pass[0].password1[i]!=0x30)
				{
				break;				
				}
			else
				{
				pass[0].password1[i]='\0';
				}
		}

				for(i=0;i<8;i++)
		{
		 GetCharSetData(SETDATA_PassWord1_NUM+i,&u_temp);
		 pass[1].password1[i]=HexToAscii1(u_temp);
		}
		pass[1].password1[7]='\0';
		for(i=6;i>=0;i--)
		{
			if(pass[1].password1[i]!=0x30)
				{
				break;				
				}
			else
				{
				pass[1].password1[i]='\0';
				}
		}


}

/************************************************/
/*函数名称:HexToAscii1(unsigned char Input)		                */
/*函数功能:	HexToAscii1        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
const  char headername[]="/back.html";
static void send_header_file(struct httpd_state *s,int downloadresult)
{
   		memset(s->filename,0,sizeof(s->filename));
		s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
                if(s->passflag == ADMIN_USER )
                {
             
			make_header_file(downloadresult);
         		//RecordEvent(HISTORY_EVENT_DOWNLOAD_FROM_NETWORK, 0, downloadresult,0);
                }
                else
                {
               	 make_header_file(UNATHURIZED_DOWNLOAD);
				 //RecordEvent(HISTORY_EVENT_DOWNLOAD_FROM_NETWORK, 0, UNATHURIZED_DOWNLOAD,0);
            
                }
              
         	strncpy(s->filename, headername, sizeof(headername));
              s->state = STATE_OUTPUT;

}

const char filename[]="/404.html";
const char filedataname[]="/files.html";


const char txt[]=".dat";
//Modified by hulw 090628 change alarm file name from .dat to .txt
const char txt1[]=".txt";

const char htm[]=".html";
//Modified by zzc 090721 use the .html instead of .txt for WEB file upload
const char file1[] = "ActiveAlarms.html";
const char file2[] = "HistoryAlarms.html";
const char file3[] = "BatteryDischargelog.html";
const char file4[] = "Statisticlog.html";

const  char setdatafile[]="files.html?";

/*this is upload file structure*/
typedef void(*OUTVOID_INTERFACE_PROC)(void);
typedef struct Up_Load_Interface
{
	 char  file[20];   //upload file name
	 UCHAR   seqno;   //upload file seq
	OUTVOID_INTERFACE_PROC fproc;  //upload function
	
}I2CUp_Load_Interface;
const I2CUp_Load_Interface uplaodfiledata[5]={
	{"ConfigsFile",CONFIG_FILES,make_up_loadfile_config_files},
	{"ActiveAlarms",ACTIVE_ALARMS_FILE,make_up_loadfile_active_alarms_file},
	{"HistoryAlarms",HISTRORY_ALARMS_FILE,make_up_loadfile_history_alarms_file},
	{"BatteryDischargelog",BATTERY_DISCHARGE_LOG,make_up_loadfile_battery_discharge_log},
	{"Statisticlog",STATISTIC_LOG,make_up_loadfile_statistic_file},
};

/************************************************/
/*函数名称:handle_input(struct httpd_state *s)	                */
/*函数功能:	处理input        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static
PT_THREAD(handle_input(struct httpd_state *s))
{
  	int i;
    register struct uip_conn *uip_connr = uip_conn;
  	char *p,*buf;
  	char username[10],passwd[10];     
	

	PSOCK_BEGIN(&s->sin);
  	PT_YIELD_FLAG = PT_YIELD_FLAG;
  	PSOCK_READTO(&s->sin, ISO_space);
	s->uploadflag=NOT_UP_LOAD_FILE;  //设置upload标志位
  
  	if(strncmp(s->inputbuf, http_get, 4) != 0) 
	{//if it is not get
			if (strncmp(s->inputbuf, http_post, 5) == 0)//if it is post
    			{
    				//发现是post包
						timer_set(&download_timer, CLOCK_SECOND * 30/60);
             					memset(&http_mul_part,0,sizeof(http_mul_part));
   	   					http_mul_part.post_state = STATE_POST;
						http_mul_part.rec_count_num = 0;
                        
      // get the sequence number that we expect to receive next packet
						 for (i=0; i<4; i++)
      						{
        						http_mul_part.rcv_nxt[i] =uip_connr->rcv_nxt[i];
      						}
						for (i=0; i < MULBOUNDARYSIZE; i++)
      						{
        						multipart_boundary[i] = 0;
        						if (i < MULLENTHSIZE)
        						{
          							multipart_length[i] = 0;
        						}
      			}
			u16_t temp_count1, temp_count2;
 			memset(multipart_boundary,0,MULBOUNDARYSIZE);
				//memset(BinBuffer,0,9*1024);
			memset(data_404_html,0,8*1024+10);
				
      			if (strbuffer(&s->sin, http_content_multipart_boundary) == BUF_FOUND_STR)
      			{
	        		PSOCK_READTO(&s->sin, '=');
       	 		temp_count1 = s->sin.readlen;
       	 		PSOCK_READTO(&s->sin, ISO_nl); // find 0x0a
        			temp_count2 = s->sin.readlen;
        			strncpy(multipart_boundary, s->inputbuf, abs(temp_count2-temp_count1));
      			}	
	  		memset(new_multipart_boundary,0,MULLENTHSIZE);
	  		boundary_pick_up(multipart_boundary,new_multipart_boundary);

			if (strbuffer(&s->sin, http_content_length) == BUF_FOUND_STR)
      			{
        			PSOCK_READTO(&s->sin, ' ');
        			temp_count1 = s->sin.readlen;
        			PSOCK_READTO(&s->sin, ISO_nl); // find 0x0a
        			temp_count2 = s->sin.readlen;
        			strncpy(multipart_length, s->inputbuf, abs(temp_count1-temp_count2-2));
        			Str2Int((unsigned char *)multipart_length, &http_mul_part.multipart_bytes);
      			}
                        
                        uip_buf[uip_len+54]='\0';
                        p=my_strstr((char*)(uip_buf+54),(char*)compare);
                        p=p+sizeof(compare)-1;
        	       	buf = ap_uudecode((const char *)p);
				if(Get_UserPass( buf, username, passwd )<0)
                                 {
                                   s->passflag=NOT_PASS_AUTHORIZTON;
	       			goto  send_pass;		
                                 }
                                 username[9]='\0';
                                 passwd[9]='\0';
        			
        			Get_password();
		             	s->passflag=CheckSecurity(username,passwd);
         			if(s->passflag<0)
         			{
             				s->passflag=NOT_PASS_AUTHORIZTON;
	       			goto  send_pass;		
			 	}
                        
                        
      				return PT_EXITED; // exit to handle the download data
    		}
	//PSOCK_CLOSE_EXIT(&s->sin);
 	}

  	else
  	{//it is get
		  	uip_buf[uip_len+54]='\0';
  			 s->passflag=NOT_PASS_AUTHORIZTON;
  			p=my_strstr((char*)(uip_buf+54),(char*)compare);
	  		if(p==NULL)
 	 		{
  	  			s->passflag=NOT_PASS_AUTHORIZTON;
           			strncpy(s->filename, http_index_html, sizeof(s->filename));//find the file
	       		goto  send_pass;	
  		  	}
 	 		else
  			{
				p=p+sizeof(compare)-1;
        	       	buf = ap_uudecode((const char *)p);
				 if(Get_UserPass( buf, username, passwd )<0)
                                 {
                                   s->passflag=NOT_PASS_AUTHORIZTON;
	       			goto  send_pass;		
                                 }
                                 username[9]='\0';
                                 passwd[9]='\0';
        			Get_password();
		             	s->passflag=CheckSecurity(username,passwd);
         			if(s->passflag<0)
         			{
             				s->passflag=NOT_PASS_AUTHORIZTON;
	       			goto  send_pass;		
			 	}
         			//s->passflag=PASS_AUTHORIZTON;
  			}
  
	  			PSOCK_READTO(&s->sin, ISO_space);
	 		 if(s->inputbuf[0] != ISO_slash)
			{
					 PSOCK_CLOSE_EXIT(&s->sin);
			 }



	 		//if(strstr(s->inputbuf,txt)!=NULL)//判断是否为上传文件
			 //Modified by zzc 090721 use the .html instead of .txt for WEB file upload
			 	if((strstr(s->inputbuf,txt) != NULL) || (strstr(s->inputbuf,txt1) != NULL)
               || (strstr(s->inputbuf, file1) != NULL)
            || (strstr(s->inputbuf, file2) != NULL) 
			|| (strstr(s->inputbuf, file3) != NULL)
			|| (strstr(s->inputbuf, file4) != NULL)
			)
  		{//
			for(i=0;i<5;i++)
					{
					if(strstr(s->inputbuf,uplaodfiledata[i].file)!=NULL)//根据文件名设置标志位
         					{
         					s->uploadflag=uplaodfiledata[i].seqno;
						uplaodfiledata[i].fproc();
						break;
		 				}
					}
		
              		s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
              		strncpy(s->filename, filename, sizeof(filename));
                      
       		}    
   			else//it is not file upload
  			{

       			if(s->inputbuf[1] == ISO_space) 
            			 {
        	   	 			strncpy(s->filename, http_index_html, sizeof(s->filename));
							make_header_file(NONE_DOWNLOAD);
							make_setdata_file(NONE_SETTING);
  				}
        			else 
        			{
                     		if((strstr(s->inputbuf,setdatafile)!=NULL)&&(strlen(s->inputbuf)>12))//it is setting 
		     			{
		   				//s->setdataflag=process(s->inputbuf);
		   				int setting_flag=process(s);
						make_setdata_file(setting_flag);
						memset(s->inputbuf,0,HTTP_SATAE_INPUTBUF_SIZE);
 		              		//memset(s->inputbuf,0,sizeof(s->inputbuf));
                             		memset(s->filename,0,sizeof(s->filename));
				 		s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
				
         					strncpy(s->filename, filedataname, sizeof(filedataname));
         				}		     	
			 		else
					{  
					//make_setdata_file(NONE_SETTING);
	 					//make_data_file(s->passflag);
								     
    						s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
   						 strncpy(s->filename, &s->inputbuf[0], sizeof(s->filename));
					}
  				}
 			}
  		}
	//memset(s->inputbuf,0,sizeof(s->inputbuf));
	//memset(s->inputbuf,0,80);
send_pass: 
  memset(s->inputbuf,0,HTTP_SATAE_INPUTBUF_SIZE);
  s->state = STATE_OUTPUT;
  
  while(1) {
    PSOCK_READTO(&s->sin, ISO_nl);

    if(strncmp(s->inputbuf, http_referer, 8) == 0) {
      s->inputbuf[PSOCK_DATALEN(&s->sin) - 2] = 0;
      //     httpd_log(&s->inputbuf[9]);
    }
  }
  
  PSOCK_END(&s->sin);
}

/************************************************/
/*函数名称:void prcess_post_packet(struct httpd_state *s)	                */
/*函数功能:	处理post包        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
#define   DOWNLOAD_SIZE     8192	
static void prcess_post_packet(struct httpd_state *s)
{
register struct uip_conn *uip_connr = uip_conn;
  unsigned char *pDownload;
	int i=0;
	  u16_t temp_count1, temp_count2;
	 for (i=0; i<4; i++)
   	 {
     		 http_mul_part.multipart_state = 1;
      		if (http_mul_part.rcv_nxt[i] != uip_buf[i + 38])
     		 {
     		   http_mul_part.multipart_state = 0; // no multipart packet, drop it
      		  break;
     		 }
    	}
    // multipart packet, process it
      if (http_mul_part.multipart_state == 1)
     {
     //pDownload = (unsigned char *)uip_appdata;
 	 if(timer_state(&download_timer))
 		{
 			if(timer_expired(&download_timer))
			{
				            	memset(&http_mul_part,0,sizeof(http_mul_part));
						timer_disable(&download_timer);
                   				send_header_file(s,2);
     				}
    	
    			}
     		 http_mul_part.rec_count_num++; // count the receiving packets
      // update the sequence number that we expect to receive next packet
      		for (i=0; i<4; i++)
     		 {
      		  http_mul_part.rcv_nxt[i] =uip_connr->rcv_nxt[i];
   		   }
      // process the first receiving multipart data in here
    		  if (http_mul_part.rec_count_num <= 1)
       	 {
                  				 s->sin.readlen = uip_len;
       		 		s->sin.readptr = (u8_t *)uip_appdata;
					//the second packet 
					if (strbuffer(&s->sin, "ConfigsFile.dat") != BUF_FOUND_STR)
					{
						memset(&http_mul_part,0,sizeof(http_mul_part));
						timer_disable(&download_timer);
						memset(uip_buf,0,UIP_CONF_BUFFER_SIZE);
					  	uip_abort();
       				    return;					  
					}
					 if (strbuffer(&s->sin, http_content_type_binary) == BUF_FOUND_STR)
      			 		 {
						  psock_readto(&s->sin, 0x0d); // '\r'
       		   			  psock_readto(&s->sin, 0x0a); // '\n'
						  psock_readto(&s->sin, 0x0d); // '\r'
        					  psock_readto(&s->sin, 0x0a); // '\n'


			         		   pDownload = s->sin.readptr;
				        	     http_mul_part.rec_head = uip_len - s->sin.readlen;
					 		//if the second packet find boundary,wrong downlaod  
					   	 if (strbuffer(&s->sin, new_multipart_boundary) == BUF_FOUND_STR)
					    	{
					    //wrong download 
					    	      	psock_readto(&s->sin, '\r'); // 0x0d
       		    			 	psock_readto(&s->sin, '\n'); // 0x0a
            		     				memset(&http_mul_part,0,sizeof(http_mul_part));
							timer_disable(&download_timer);
							memset(uip_buf,0,UIP_CONF_BUFFER_SIZE);
							send_header_file(s,2);
					    		
					    	}
					  	else
					  	{
							 
					  		 for(i=0; i<(uip_len-http_mul_part.rec_head); i++)
        			  			{
        		 	  				 data_404_html[i] = *pDownload;
       			   		  		pDownload++;
           					 		http_mul_part.rec_bytes++;
         			 			}
						}
        				}
					

					//not the second packet 
      			  		else
       				 {
          // do	wnload is fail.
        				 	 http_mul_part.post_state = STATE_WAITING;
        				 	 http_mul_part.multipart_state = 0;
       		  		 	http_mul_part.rec_bytes = 0;
      			    			http_mul_part.rec_count_num = 0;
       				       return;
       				 }
				}
 		         
     			 else
     			 {
   
        		pDownload = (unsigned char *)uip_appdata;
                        s->sin.readptr = (u8_t *)uip_appdata;

   				 s->sin.readlen = uip_len;
				if((uip_len <1460)
				&&((strbuffer(&s->sin, new_multipart_boundary) == BUF_FOUND_STR)))
        			{//last packet
         									
		  			if((http_mul_part.multipart_bytes<8200)||(http_mul_part.multipart_bytes>9000))
		  			{
			       	psock_readto(&s->sin, '\r'); // 0x0d
           			 	psock_readto(&s->sin, '\n'); // 0x0a
            				temp_count1 = s->sin.readlen;
             				memset(&http_mul_part,0,sizeof(http_mul_part));
					timer_disable(&download_timer);
				//gs_SetData.fDownLoadSetData(1,data_404_html);
					memset(uip_buf,0,UIP_CONF_BUFFER_SIZE);
					send_header_file(s,2);
		  		}
				  else
		  	  	{
          			
			       psock_readto(&s->sin, '\r'); // 0x0d
           			 psock_readto(&s->sin, '\n'); // 0x0a
            			temp_count1 = s->sin.readlen;
            // multipart boundary length : temp_count2
            			temp_count2 = strlen(multipart_boundary)+6;
            // true binary length in the last packet : temp_count1
         			  http_mul_part.rec_tail = temp_count1 + temp_count2 + 4;

      			  	    for (i=0; i<(uip_len - temp_count2); i++)
            				{
          			    data_404_html[http_mul_part.rec_bytes] = *pDownload;
          			    pDownload++;
           			   http_mul_part.rec_bytes++;
           			 	}
 	
                                if(http_mul_part.rec_bytes==DOWNLOAD_SIZE)
                                {
            			memset(&http_mul_part,0,sizeof(http_mul_part));
		
				timer_disable(&download_timer);
                                
                                
                                
                                if(s->passflag==ADMIN_USER)
				{
                                  gSetDataInfo.fDownLoadSetData(1,data_404_html);
                                }
				memset(uip_buf,0,UIP_CONF_BUFFER_SIZE);
				send_header_file(s,1);
                                }
                                else
                                {
                                  memset(&http_mul_part,0,sizeof(http_mul_part));
		
				  timer_disable(&download_timer);
                                  memset(uip_buf,0,UIP_CONF_BUFFER_SIZE);
				  send_header_file(s,2);
                                }
		  		}
					  	
      			}

			 if(http_mul_part.multipart_state == 1)
        			{
          				for(i=0; i<uip_len; i++)
          				{
          				if(http_mul_part.rec_bytes>8*1024)
          					{
          					  http_mul_part.multipart_state = 0; //youzhiyi 20091113
          					break;
          					}
					else
						{
							
           			 			data_404_html[http_mul_part.rec_bytes] = *pDownload;
         			 			 pDownload++;
           			 			http_mul_part.rec_bytes++;
						}
          				}
        			}
    		}
	  }

}

/************************************************/
/*函数名称:handle_connection	                */
/*函数功能:	处理input和output        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
static void
handle_connection(struct httpd_state *s)
{
	if (http_mul_part.post_state != STATE_POST)
  	{       
    		handle_input(s);
	}
  	else
 	{
 //处理post包
 		prcess_post_packet(s);
	}
	  if(s->state == STATE_OUTPUT)
  	{
    		handle_output(s);
  	}
}













/*---------------------------------------------------------------------------*/
/************************************************/
/*函数名称:httpd_appcall(void)		                */
/*函数功能:http包的入口					       */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/
void
httpd_appcall(void)
{

	struct httpd_state *s = (struct httpd_state *)&(uip_conn->appstate);

	if(uip_closed() || uip_aborted() || uip_timedout()) {
	} 	
	else if(uip_connected()) {
		PSOCK_INIT(&s->sin, s->inputbuf, sizeof(s->inputbuf) - 1);
		PSOCK_INIT(&s->sout, s->inputbuf, sizeof(s->inputbuf) - 1);
		PT_INIT(&s->outputpt);
		s->state = STATE_WAITING;
		/*    timer_set(&s->timer, CLOCK_SECOND * 100);*/
		s->timer = 0;
		handle_connection(s);
	}	
	else if(s != NULL) {
		if(uip_poll()) {
			++s->timer;
			if(s->timer >= 20) {
				uip_abort();
			}
		} else {
			s->timer = 0;
		}
		handle_connection(s);
	}
	else {
		uip_abort();
	}

}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Initialize the web server
 *
 *             This function initializes the web server and should be
 *             called at system boot-up.
 */
 /************************************************/
/*函数名称:void httpd_init(void)		                */
/*函数功能:	web出世化     */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.25      						          */
/************************************************/


void
httpd_init(void)
{
  	uip_listen(HTONS(WEB_SERVER_PORT));
  	memset(multipart_boundary, 0, MULBOUNDARYSIZE);
  	memset(multipart_length, 0, MULLENTHSIZE);
}
/*---------------------------------------------------------------------------*/
/** @} */
