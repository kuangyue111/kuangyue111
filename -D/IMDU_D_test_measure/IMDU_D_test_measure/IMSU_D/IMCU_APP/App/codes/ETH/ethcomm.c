/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   Ethernet.c                                                   *
 *  Date:       May 1, 2007                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */

#include "main.h"
#include "ethdev.h"
#include "uip_arp.h"

//#include "interface.h"
#include "uip_arp.h"
#include "uip.h"
#include "timer.h"
#include "resolv.h"
#include "EtherNet.h"
#include "ethcomm.h"
//#include "MQTTClient.h"

uip_ipaddr_t gs_RemoteServerIp;
unsigned char g_ucRemoteServerIpIsOk = 0;
unsigned long  Trap_TrapNum=0;



struct timer periodic_timer, arp_timer;
UCHAR mac[10];
#define   RESET_CHIP_NUM1  35000
#define   RESET_CHIP_NUM2    1000 
//15000次没有接收到数据，就复位，以此做计数器
 long reset_netchip = 0;

//1000次有接收数据却没有发送数据，就复位，以此做计数器
long reset_netchip_send = 0; 

UCHAR ucARPOnce = 0;

UCHAR g_ucDHCPAddressPrepared = mDHCP_NEED_INIT;

NOINIT __NET_TCP_INFO gNET_TCP_Info;
uip_ipaddr_t eem_ipaddr[2];
void ReInitNetPort(void);

static void RefreshEthcommTime(void)
{
	struct tm  tmNow;
	static UCHAR  Sec = 0;

	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)//秒钟
	{
		Sec = (UCHAR)tmNow.tm_sec;
                //网络任务 计时处理 start
                if(gNET_TCP_Info.u32BreakCount<65535)
                {
                        gNET_TCP_Info.u32BreakCount++;
                }

		
	}
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
	//get the  ip
	for(i=0;i<4;i++)
	{
        GetCharSetData(SETDATA_ucSelfIP_1_NUM+i,ip1+i);
	}
       

	// set the ip
	uip_ipaddr(ipaddr, ip1[0],ip1[1],ip1[2],ip1[3]);
	uip_sethostaddr(ipaddr);
	//get the mask
	for(i=0;i<4;i++)
    {
		GetCharSetData(SETDATA_ucSubMask_1_NUM+i,ip1+i);
    }
       

	//set the mask
	uip_ipaddr(ipaddr, ip1[0],ip1[1],ip1[2],ip1[3]);
	uip_setnetmask(ipaddr);

	//get the gateway
	for(i=0;i<4;i++)
	{
		GetCharSetData(SETDATA_ucGateWay_1_NUM+i,ip1+i);
	}
        

	//set the gateway
	uip_ipaddr(ipaddr, ip1[0],ip1[1],ip1[2],ip1[3]);
	uip_setdraddr(ipaddr);

}


//读取ip，mask，gateway
static void ethInit(void)
{
	UCHAR ip[4];
	int p;
	uip_ipaddr_t ipaddr[2];
        __XXX_UNION_VALUE unVal;
	struct uip_udp_conn *udp_conn_ret;

	//following init the parameter of other part of the network
	

	for(p=0;p<4;p++)//get the  ip
	{
		GetCharSetData(SETDATA_ucSelfIP_1_NUM+p,&ip[p]);
	}
	uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
	uip_sethostaddr(ipaddr);//set the ip

	//NET_PRINTF(" Init Net IP=%d %d %d %d ",ip[0],ip[1],ip[2],ip[3]);

	for(p=0;p<4;p++)//get the  netmask
	{
		GetCharSetData(SETDATA_ucSubMask_1_NUM+p,&ip[p]);       
	}
	uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
	uip_setnetmask(ipaddr);//set the netmasik
	//NET_PRINTF(" Init Net Mask=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);
	for(p=0;p<4;p++)//get the  gateway
	{
		GetCharSetData(SETDATA_ucGateWay_1_NUM+p,&ip[p]);
	}
	uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
	uip_setdraddr(ipaddr);
	//NET_PRINTF(" Init Net Gateway=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);
	for(p=0;p<4;p++)//get the  eem_ip1
	{
		GetCharSetData(SETDATA_ucServerIP_1_NUM+p,&ip[p]); 
	}
	uip_ipaddr(eem_ipaddr[0], ip[0],ip[1],ip[2],ip[3]);
	//NET_PRINTF(" Init Net Powerstar1=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);

	for(p=0;p<4;p++)//get the  eem_ip1
	{
		ip[p] = 0;
	}
	uip_ipaddr(eem_ipaddr[1], ip[0],ip[1],ip[2],ip[3]);
	//NET_PRINTF(" Init Net Powerstar2=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);

	
	uip_ipaddr(ipaddr,255,255,255,255);
	udp_conn_ret = uip_udp_new(ipaddr,HTONS(0));
	if(udp_conn_ret != NULL)
	{
		uip_udp_bind(udp_conn_ret,HTONS(161));
	}
}

//static void DHCPethInit(void);
//static void DHCPethInit(void)
//{
//	UCHAR ip[4];
//	int p;
//	uip_ipaddr_t ipaddr[2];
//
//	struct uip_udp_conn *udp_conn_ret;
//
//	//following init the parameter of other part of the network
//	memset(&gs_trap,0,sizeof(gs_trap));	
//
//	for(p=0;p<4;p++)//get the  ip
//	{
//		GetCharSetData(SETDATA_IP_NUM+p,&ip[p]);
//	}
//	uip_ipaddr(ipaddr, gEvChargeInfo.stDHCPNetInfo.u8IP[0],gEvChargeInfo.stDHCPNetInfo.u8IP[1],gEvChargeInfo.stDHCPNetInfo.u8IP[2],gEvChargeInfo.stDHCPNetInfo.u8IP[3]);
//	uip_sethostaddr(ipaddr);//set the ip
//
//	//NET_PRINTF(" Init Net IP=%d %d %d %d ",ip[0],ip[1],ip[2],ip[3]);
//
//	for(p=0;p<4;p++)//get the  netmask
//	{
//		GetCharSetData(SETDATA_Subnetmask_NUM+p,&ip[p]);
//	}
//	uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
//	uip_setnetmask(ipaddr);//set the netmasik
//	//NET_PRINTF(" Init Net Mask=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);
//	for(p=0;p<4;p++)//get the  gateway
//	{
//		GetCharSetData(SETDATA_Gateway_NUM+p,&ip[p]);
//	}
//	uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
//	uip_setdraddr(ipaddr);
//	//NET_PRINTF(" Init Net Gateway=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);
//	for(p=0;p<4;p++)//get the  eem_ip1
//	{
//		GetCharSetData(SETDATA_PowerStarIP_NUM+p,&ip[p]);
//	}
//	uip_ipaddr(eem_ipaddr[0], ip[0],ip[1],ip[2],ip[3]);
//	//NET_PRINTF(" Init Net Powerstar1=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);
//
//	for(p=0;p<4;p++)//get the  eem_ip1
//	{
//		GetCharSetData(SETDATA_PowerStarIP_NUM+6+p,&ip[p]);
//	}
//	uip_ipaddr(eem_ipaddr[1], ip[0],ip[1],ip[2],ip[3]);
//	//NET_PRINTF(" Init Net Powerstar2=%d %d %d %d \n",ip[0],ip[1],ip[2],ip[3]);
//
//	for(p=0;p<4;p++)//get the trap ip
//	{
//		GetCharSetData(SETDATA_NMSIP_NUM+p,&ip[p]);
//	}
//	gs_trap.uip_trap[0][0]=(ip[1])*256+ip[0];
//	gs_trap.uip_trap[0][1]=ip[3]*256+ip[2];
//
//	for(p=0;p<4;p++)//get the trap ip
//	{
//		GetCharSetData(SETDATA_NMSIP_NUM+6+p,&ip[p]);
//	}
//	gs_trap.uip_trap[1][0]=(ip[1])*256+ip[0];
//	gs_trap.uip_trap[1][1]=ip[3]*256+ip[2];
//
//	uip_ipaddr(ipaddr,255,255,255,255);
//	udp_conn_ret = uip_udp_new(ipaddr,HTONS(0));
//	if(udp_conn_ret != NULL)
//	{
//		uip_udp_bind(udp_conn_ret,HTONS(161));
//	}
//}
//

void  DHCPset_ip(void );
void  DHCPset_ip(void )
{
	//int i;
	uip_ipaddr_t ipaddr[2];

	//UCHAR ip1[4];

	// set the ip
	uip_ipaddr(ipaddr, gEvChargeInfo.stDHCPNetInfo.u8IP[0],gEvChargeInfo.stDHCPNetInfo.u8IP[1],gEvChargeInfo.stDHCPNetInfo.u8IP[2],gEvChargeInfo.stDHCPNetInfo.u8IP[3]);
	uip_sethostaddr(ipaddr);

	//set the mask
	uip_ipaddr(ipaddr, gEvChargeInfo.stDHCPNetInfo.u8Mask[0],gEvChargeInfo.stDHCPNetInfo.u8Mask[1],gEvChargeInfo.stDHCPNetInfo.u8Mask[2],gEvChargeInfo.stDHCPNetInfo.u8Mask[3]);
	uip_setnetmask(ipaddr);

	//set the draddr
	uip_ipaddr(ipaddr, gEvChargeInfo.stDHCPNetInfo.u8Gateway[0],gEvChargeInfo.stDHCPNetInfo.u8Gateway[1],gEvChargeInfo.stDHCPNetInfo.u8Gateway[2],gEvChargeInfo.stDHCPNetInfo.u8Gateway[3]);
	uip_setdraddr(ipaddr);

}

static UINT8 u8IsLowLevelBreak(void)
{
	if(gNET_TCP_Info.u32BreakCount >= 360)
	{
		gNET_TCP_Info.u8DHCPResult = FALSE;
		g_ucDHCPAddressPrepared = mDHCP_NEED_INIT;
		gs_EthAppCommMng.ui16DHCPReconfigTimeOUT  = 0;
		gNET_TCP_Info.iNetCreateSocketFlag = TRUE;
		gNET_TCP_Info.u32BreakCount =0;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void resolv_found(char *name, u16_t *ipaddr)
{
	if(ipaddr == NULL)
	{

	}
	else
	{
		uip_ipaddr_copy(&gs_RemoteServerIp, ipaddr); 
		g_ucRemoteServerIpIsOk = 1;
	}
}
/************************************************/
/*函数名称:TaskofWebComm()		                */
/*函数功能:网络任务							    */
/*入口参数:									    */
/*出口参数:无									*/
/*资源:           					            */
/*子函数:	         							*/
/*更改日期:2007.5.31      						*/
/************************************************/
#define UIPBUF ((struct uip_eth_hdr *)&uip_buf[0])
static void net_poll(void)
{
	int i;
	__XXX_UNION_VALUE unVal;
	UINT8 ucDHCPClientEnable = DISABLE;
	UINT8 ucServerIsDomainName = 0;
	static unsigned char s_uNeedReconnectFlag = 0;
	//get the input
	uip_len = dv_ethdev.fNetWorkDeviceRead();

	if (gNET_TCP_Info.iNetCreateSocketFlag == TRUE)
	{
		gNET_TCP_Info.iNetCreateSocketFlag = FALSE;
		GetCharSetData(SETDATA_ucDHCPEnable_NUM, &ucDHCPClientEnable);
		ucDHCPClientEnable = unVal.chtemp[0];
		if (ucDHCPClientEnable == DISABLE)
		{
			ReInitNetPort();
			uip_len =0;
			set_ip();
			uip_init();
			if(ucServerIsDomainName)
			{
				ask_dns_server_get_ip();
				s_uNeedReconnectFlag = 1;
			}
			else
			{
				tcp_client_reconnect();
				s_uNeedReconnectFlag = 0;
			}
		}
		else
		{
			if (TRUE == gNET_TCP_Info.u8DHCPResult)
			{
				uip_len =0;
				DHCPset_ip();
				uip_init();
				if(ucServerIsDomainName)
				{
					ask_dns_server_get_ip();
					s_uNeedReconnectFlag = 1;
				}
				else
				{
					tcp_client_reconnect();
					s_uNeedReconnectFlag = 0;
				}
			}
		}
		return;
	}
	if(s_uNeedReconnectFlag)
	{
		if(g_ucRemoteServerIpIsOk)
		{		
			tcp_client_reconnect_domainname();
			s_uNeedReconnectFlag = 0;
		}
	}

	if(uip_len == 0)
	{
		if (u8IsLowLevelBreak())
		{
			g_ucRemoteServerIpIsOk = 0;
			resolv_init();
			return;
		}
	}
	else
	{
		gNET_TCP_Info.u32BreakCount = 0;
	}
    
	if(uip_len > 0)
	{
		reset_netchip = 0;
		//if it is arp packet
		if(UIPBUF->type == htons(UIP_ETHTYPE_ARP))
		{
			gSelfTest.fSetTaskID(Tasko_ID_ethernet);
			uip_arp_arpin();
			if (uip_len > 0)
			{
				reset_netchip_send = 0;               
				dv_ethdev.fNetWorkDeviceSend();
				gSelfTest.fSetTaskID(Tasko_ID_ethernet);
			}
		}
		//if it is ip packet
		else if(UIPBUF->type == htons(UIP_ETHTYPE_IP))
		{
			uip_arp_ipin();
			uip_input();
			if (uip_len > 0)
			{              
				gSelfTest.fSetTaskID(Tasko_ID_ethernet);
				uip_arp_out();              
				reset_netchip_send = 0;				                
				dv_ethdev.fNetWorkDeviceSend();
				gSelfTest.fSetTaskID(Tasko_ID_ethernet);
			}
		}
	}
	else if(timer_expired(&periodic_timer))
	{
		gSelfTest.fSetTaskID(Tasko_ID_ethernet);
		timer_reset(&periodic_timer);
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);
			if(uip_len > 0)
			{
				uip_arp_out();			//加以太网头结构，在主动连接时可能要构造ARP请求
				dv_ethdev.fNetWorkDeviceSend();
			}
		}
#if UIP_UDP	//UIP_UDP 
		//轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//处理UDP通信事件
			//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
			if(uip_len > 0)
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				dv_ethdev.fNetWorkDeviceSend();//发送数据到以太网
			}
		}
#endif 
		gSelfTest.fSetTaskID(Tasko_ID_ethernet);
		/* Call the ARP timer function every 10 seconds. */
		if(timer_expired(&arp_timer)) {
			timer_reset(&arp_timer);
			uip_arp_timer();
			ucARPOnce = 0;
		}
	}
}




void ReInitNetPort(void)
{
	UCHAR mac[6];
	UCHAR i;
	__XXX_UNION_VALUE unVal;
	//init  the network
	for(i=0;i<6;i++)
	{
		GetCharSetData(SETDATA_ucMACAddress_1_NUM+i,&mac[i]);
	}
	dv_ethdev.fInitNetPort();
	uip_init();
	uip_arp_init();
	ethInit();
	uip_router_init();
	timer_set(&periodic_timer, ((CLOCK_SECOND * OS_TICKS_PER_SEC) / 2));
	timer_set(&arp_timer, ((CLOCK_SECOND * OS_TICKS_PER_SEC) * 10));
}

void netIP_Deal(void);
void netIP_Deal(void)									//DHCP static!
{
	__XXX_UNION_VALUE unVal;
	UCHAR ucDHCPClientEnable = mENABLE;
	//UCHAR mac[10];
	UCHAR i = 0;

	gSelfTest.fSetTaskID(Tasko_ID_ethernet);
	GetCharSetData(SETDATA_ucDHCPEnable_NUM, &ucDHCPClientEnable);
	if (ucDHCPClientEnable == mDISABLE)				//不使能则更新设置IP
	{
		set_ip();
		g_ucDHCPAddressPrepared = mDHCP_NEED_INIT;
	}
	else
	{
		if (g_ucDHCPAddressPrepared == mDHCP_NEED_INIT)
		{
			ReInitNetPort();

			//Re-init to avoid UDP connections fully occupied.
			uip_init();
			uip_arp_init();
			ethInit();
			uip_router_init();
			for(i=0;i<6;i++)
			{
				GetCharSetData(SETDATA_ucMACAddress_1_NUM+i,&mac[5-i]);
			}
			extern void dhcpc_init(const void *mac_addr, int mac_len);
			dhcpc_init(mac, 6);
			g_ucDHCPAddressPrepared = mDHCP_REQUESTING;
		}
		else if(g_ucDHCPAddressPrepared == mDHCP_REQUESTING)
		{
			gSelfTest.fSetTaskID(Tasko_ID_ethernet);
			extern void dhcpc_appcall(void);
			dhcpc_appcall();
			g_ucDHCPAddressPrepared = mDHCP_CONFIGURED;		//让void uip_router_udp_appcall(void)继续DHCP！
		}
	}
}

void netInitMac(void);

//给全局变量uip_ethaddr  赋值
void netInitMac(void)
{
  __XXX_UNION_VALUE unVal;
	UCHAR i = 0;
	mac[5] = 0x00;
	mac[4] = 0x09;
	mac[3] = 0xf5;
	mac[2] = 0x0c;
	mac[1] = 0xE2;
	mac[0] = 0x02;
	for(i=0;i<6;i++)
	{
	//	SetCharSetData(SETDATA_ucMACAddress_1_NUM+i,&mac[i]);
		uip_ethaddr.addr[5-i] = mac[i];
	}
}

void NET_Clean_RBuf(void)
{
	UCHAR i = 0;
	for (i = 0; i < NET_R_NUM; i++)
	{
		gNET_TCP_Info.u8ReadBuf[i].u8DataFlag = 0;
		gNET_TCP_Info.u8ReadBuf[i].u16Lengh = 0;
	}
}
void NET_Clean_SBuf(void)
{
	UINT8 u32Idx = 0;
	UINT32 u32Temp = 0;
	MutexLock(gEvChargeInfo.hMetexNet_W);
	for(u32Idx = 0; u32Idx < NET_S_NUM; u32Idx++)
	{
		gNET_TCP_Info.u8SendBuf[u32Idx].u8DataFlag = FALSE;
		for (u32Temp = 0; u32Temp < NET_BUF_S_DATA_L/2; u32Temp++)
		{
			gNET_TCP_Info.u8SendBuf[u32Idx].u8Buf[u32Temp] = 0;//BUF 清 0
		}
	}
	MutexUnlock(gEvChargeInfo.hMetexNet_W);
	return;
}

//TRUE Send OK,   FALSE Send Fails
void NET_Send(UINT8* pBuf, UINT16 u16Len)
{
	UINT8  i,u8BufIdx = 0;
	static UCHAR lastSendNum =0;
	if(u16Len >NET_BUF_S_DATA_L)
	{
		u16Len = NET_BUF_S_DATA_L;
	}
	for(i = 0; i < NET_S_NUM; i++)
	{	
		if(lastSendNum  >= (NET_S_NUM-1))
		{
			u8BufIdx = 0;
		}
		else
		{
			u8BufIdx = lastSendNum+1;
		}

		if (FALSE == gNET_TCP_Info.u8SendBuf[u8BufIdx].u8DataFlag)
		{
			MutexLock(gEvChargeInfo.hMetexNet_W);
			memcpy(gNET_TCP_Info.u8SendBuf[u8BufIdx].u8Buf, pBuf, u16Len);
			gNET_TCP_Info.u8SendBuf[u8BufIdx].u8DataFlag = TRUE;
			gNET_TCP_Info.u8SendBuf[u8BufIdx].u16Lengh = u16Len;
			MutexUnlock(gEvChargeInfo.hMetexNet_W);
			lastSendNum = u8BufIdx;
			break;
		}
	}
	if(i== NET_S_NUM)
	{
		if(lastSendNum >=(NET_S_NUM -1))
		{
			lastSendNum = 0;
		}
		else
		{
			lastSendNum ++;
		}
		MutexLock(gEvChargeInfo.hMetexNet_W);
		memcpy(gNET_TCP_Info.u8SendBuf[lastSendNum].u8Buf, pBuf, u16Len);
		gNET_TCP_Info.u8SendBuf[lastSendNum].u8DataFlag = TRUE;
		gNET_TCP_Info.u8SendBuf[lastSendNum].u16Lengh = u16Len;
		MutexUnlock(gEvChargeInfo.hMetexNet_W);
	}
}


//一次只能全读取出来！ 
UINT32 NET_Read(void *input_buf)
{	
	UINT32 u32Len = 0;
	INT8 uIdx = 0;
	MutexLock(gEvChargeInfo.hMetexNet_R);
	for (uIdx = NET_R_NUM - 1; uIdx >= 0 ; uIdx--)
	{
		if (gNET_TCP_Info.u8ReadBuf[uIdx].u8DataFlag & 0x01)
		{
			memcpy(input_buf, gNET_TCP_Info.u8ReadBuf[uIdx].u8Buf, NET_BUF_R_DATA_L-1);
			u32Len = (UINT16)gNET_TCP_Info.u8ReadBuf[uIdx].u16Lengh;
			memset(gNET_TCP_Info.u8ReadBuf[uIdx].u8Buf, 0, NET_BUF_R_DATA_L/2);
			gNET_TCP_Info.u8ReadBuf[uIdx].u8DataFlag = 0;
			MutexUnlock(gEvChargeInfo.hMetexNet_R);
			return u32Len;
		}
	}
	MutexUnlock(gEvChargeInfo.hMetexNet_R);
	return 0;
}


/************************************************/
/*软件名称:TaskofEthernet        				        */
/*软件功能:网络通讯          					          */
/*软件版本:1.0									                */
/*版本升级:										                  */
/*适用系统:           							            */
/*入口参数:										                  */
/*出口参数:										                  */
/*资源:   	               						          */
/*子函数:              							            */
/*设计日期:										                  */
/*设计人员: 									                  */
/*更改日期:			      						              */
/*更改人员: 									                  */
/************************************************/
void TASK_Ethernet (void* pdata)
{

	OSTimeDly(100);
	//UINT8 u8ItvTime = 0;

	netInitMac();
	CLR_CONN_STAT(gNET_TCP_Info.u8MachineStat);			//无链接！
	NET_Clean_RBuf();
        gNET_TCP_Info.iNetCreateSocketFlag = FALSE;
	// init the network
	netInitMac();
	dv_ethdev.fInitNetPort();
	uip_init();	
	uip_arp_init();
	resolv_init();

	ethInit();
	uip_router_init();
	timer_set(&periodic_timer, ((CLOCK_SECOND * OS_TICKS_PER_SEC) / 2));
	timer_set(&arp_timer, ((CLOCK_SECOND * OS_TICKS_PER_SEC) * 10));
	OSTimeDly(50);

	for(;;)
	{
		gSelfTest.fSetTaskID(Tasko_ID_ethernet);
                RefreshEthcommTime();
		if (gEvChargeInfo.u8NET_Mask == TRUE)
		{
			OSTimeDly(300);
			gNET_TCP_Info.iNetCreateSocketFlag = TRUE;
			CLR_CONN_STAT(gNET_TCP_Info.u8MachineStat);
			
			continue;
		}

		netIP_Deal();

		net_poll();
		
		//u8ItvTime++;
		//if (u8ItvTime > 254)
		//{
		//	u8ItvTime = 0;
		//}
		//if (u8ItvTime%10==0)
		{
			OSTimeDly(20);			// 100mS
		}

		gSelfTest.fSetTaskID(Tasko_ID_ethernet);
	}
}
