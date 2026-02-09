#include "uip.h"
#include  "snmp.h"

extern u16_t uip_slen;

/************************************************/
/*函数名称:void snmp()	               	 */
/*函数功能:process snmp input packet ,then make output packet					        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
extern void *uip_appdata;
extern void *uip_sappdata; 
extern u16_t uip_len, uip_slen;

void snmp(void)
{	
	struct req_desc rqd;
	//struct 	snbentry *bl;

	rqd.bindlf = (struct snbentry *)NULL;

	if(snparse(&rqd ,uip_appdata,uip_len)==SYSERR)
	{
		     	snfreebl(&rqd.bindlf);
					return;
		} 
				
			
			if (sna2b(&rqd) == SYSERR) {
				snfreebl(&rqd.bindlf);
				return;	
			}
						
			
			if (snrslv(&rqd) == SYSERR) {	
				
				snfreebl(&rqd.bindlf);
				return;				
			}
                        uip_slen = mksnmp(&rqd, uip_sappdata, PDU_RESP);
        
			if (uip_slen == SYSERR)
			{
                          snfreebl(&rqd.bindlf);
				return;
							
			
			}
			snfreebl(&rqd.bindlf);
			return;
	
	
}

/************************************************/
/*函数名称:int  snmp_trap()               	 */
/*函数功能:process snmp input packet ,then make output packet					        */
/*入口参数:									                  */
/*出口参数:									                  */
/*资源:           					          */
/*子函数:	         								              */
/*更改日期:2007.5.31      						          */
/************************************************/
int  snmp_trap(void)
{

	struct req_desc caili;
	if (build_trap(&caili)!=1)
	{
		return make_trap_failure;
	}	
	uip_slen = mksnmp(&caili, uip_sappdata, PDU_TRAP);

	if (uip_slen == SYSERR)
        {
           snfreebl(&caili.bindlf);
		return make_trap_failure;
        }
	else
        {
           snfreebl(&caili.bindlf);
		return make_trap_success;
        }
}
