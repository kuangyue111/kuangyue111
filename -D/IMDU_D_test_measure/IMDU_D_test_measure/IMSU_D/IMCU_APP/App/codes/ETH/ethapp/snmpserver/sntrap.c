/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   sntrap.c                                                   *
 *  Date:       April 1, 2008                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */


#include "kernel.h"

#include "main.h"
#include "snmp.h"
#include "mibtrap.h"
#include "asn1.h"
#include "mib.h"
//#include <time.h>
//#include "interface.h"
//#include "main.h"
//#define OIDSIZE sizeof(struct oid)
/*------------------------------------------------------------------------
 * snleaf - perform the requested operation on the leaf SNMP variable
 *------------------------------------------------------------------------
 */
//extern char  temp_malloc_char[10][50];
//extern int temp_i_malloc_char;

//extern struct snbentry  temp_snbentry[30];//int  temp_i_snbentry=0;
//extern int  temp_i_snbentry;
struct oid trap_oid={ {1, 4, 1,6302,2,1,0,1}, 8};

//caili
//extern OS_MEM  *p_MEM;
#define TM_YEAR_START	1900
//extern AlarmToSNMPTrap_TypeDef  trap;
 
//extern AlarmToSNMPTrap_TypeDef  trap;

//Modified by hulw 090319 处理TRAP时时间串中可能有'/0'，则只被发送'/0'前字串，造成信息不全
//int sn_trap(int nNodeID, long* lpData, char* string)
int sn_trap(int nNodeID, long* lpData, char* string, int* pStrLen)
{
  
	//long lp=8;
//	long	clktime;
//	char s[12] = "I am caili";
		switch(nNodeID)
	{
	case TRAP_1:
		*lpData =  gs_trap.trap.TrapNum;
		break;
	case TRAP_2:
		*lpData = gs_trap.trap.AlarmNum;
  
		break;
	case TRAP_3:
		*lpData =gs_trap. trap.TrapType;
		
		break;
	case TRAP_4:
			*lpData =gs_trap. trap.TrapStatus;
		break;
	case TRAP_5:
			memcpy(string,gs_trap.trap.TrapTime,sizeof(gs_trap.trap.TrapTime));

			//Modified by hulw 090319
			*pStrLen = sizeof(gs_trap.trap.TrapTime);

		break;
	case TRAP_6:
		
		memcpy(string,gs_trap.trap.TrapName,sizeof(gs_trap.trap.TrapName));

		//Modified by hulw 090319
		*pStrLen = strlen((char const*)(gs_trap.trap.TrapName));
			break;
	default :
		break;
	/*		
	case TRAP_7:
//		 clktime =time(NULL);
//		 	*lpData = clktime;
		break;
		*/

	}

	return ERR_SNP_OK;
}

int build_trap(struct req_desc  *rqdp)
{

	long lpData;
	char recieve[MAX_SEND];
	char		*strp;
	int len;
	
	int i;
	struct oid	*oip;
	const struct mib_trap_info  *pi_trap;
	struct snbentry  *bl,*lastbl = 0;

	//Modified by hulw 090319
	int nStrLen = 0;

	rqdp->reqidlen = 0;
	rqdp->err_stat = 0;
	rqdp->err_idx = 0;
	
	rqdp->bindlf = rqdp->bindle = (struct snbentry *)NULL;
		pi_trap = mib_trap;
	for(i=0;i<mib_trap_entries;i++)

	{
                //caili
		//bl = (struct snbentry *)OSMemGet(p_MEM,&err);
               // bl = (struct snbentry *)malloc(sizeof(struct snbentry));

memset(&gs_trap.temp_snbentry[gs_trap.temp_i_snbentry],0,sizeof(gs_trap.temp_snbentry[gs_trap.temp_i_snbentry]));



				bl = (struct snbentry *) (&gs_trap.temp_snbentry[gs_trap.temp_i_snbentry]);
		gs_trap.temp_i_snbentry++;
		bl->sb_next =NULL;
		bl->sb_prev = NULL;
		if(rqdp->bindlf)
		{
			lastbl->sb_next = bl;
			bl->sb_prev = lastbl;
			lastbl =bl;
		}
		else
		lastbl = rqdp->bindlf = bl;
	
		memcpy(bl->sb_oid.id,pi_trap->mi_objid.id,pi_trap->mi_objid.len*2);
		bl->sb_oid.len = pi_trap->mi_objid.len;

        //get data
		//Modified by hulw 090319
		//sn_trap(pi_trap->trap_iNode, &lpData, recieve);
		sn_trap(pi_trap->trap_iNode, &lpData, recieve, &nStrLen);
		SVTYPE(bl) = pi_trap->mi_vartype;

			switch(pi_trap->mi_vartype) {
							case ASN1_INT:
							case ASN1_TIMETICKS:
							case ASN1_GAUGE:
									case ASN1_COUNTER:

									SVINT(bl) = (int)lpData;
									break;
									case ASN1_OCTSTR:
									strp = recieve;
									if (strp == NULL) {
									SVSTRLEN(bl) = 0;
									SVSTR(bl) = NULL;
									break;
									}

									//Modified by hulw 090319
								//	len = SVSTRLEN(bl) = strlen(recieve);
									len = SVSTRLEN(bl) = nStrLen;
                                                                        //caili
                                                                        	SVSTR(bl) = (char *)(& gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0]);
									gs_trap.temp_i_malloc_char++;
								//	SVSTR(bl) = (char *)OSMemGet(p_MEM,&err);
                                                                        //SVSTR(bl) = (char *) malloc(len);
									memcpy(SVSTR(bl), recieve, len);
									break;
									case ASN1_OBJID:
									oip = &trap_oid;
									SVOIDLEN(bl) = oip->len;
									memcpy(SVOID(bl), oip->id, oip->len * 2);
									break;
									}
			                    pi_trap++;
	}
	rqdp->bindle =lastbl;

	return 1;
}
