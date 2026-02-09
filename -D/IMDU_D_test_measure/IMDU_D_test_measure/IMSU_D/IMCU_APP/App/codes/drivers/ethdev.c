  /*
 *****************************************************************************
 *                                                                           *
 *  Filename:   ethdev.c                                   *
 *  Date:       May 1, 2008    
 *  by caili
 *  Description:                                                             *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************/

#include "main.h"
#include "ethdev.h"

//#include "base_def.h"

//#define SETDATA_Mac_NUM                     3148

extern struct uip_eth_addr uip_ethaddr;
//extern unsigned char data_header_html[];

 /************************************************/
/*函数名称:network_device_read()		                */
/*函数功能:从缓冲区中读取数据							        */
/*入口参数:									                  */
/*出口参数:读出的数据长度									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
UCHAR flowctrl = 0;
static u16_t network_device_read(void)
{
    u16_t len;
    len =ReadBuff(uip_buf);
    return len;
}

  /************************************************/
/*函数名称:network_device_send()		                */
/*函数功能:从缓冲区中的数据发送出去						        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/

static UCHAR network_device_send(void)//return 1 成功
{
	return WriteBuff(uip_len, uip_buf);  
}


static void initnetport(void)
{
  low_level_init();
}

dv_ethdev_TypeDef dv_ethdev =
{
  0,
  initnetport,
  network_device_send,
  network_device_read, 
};
