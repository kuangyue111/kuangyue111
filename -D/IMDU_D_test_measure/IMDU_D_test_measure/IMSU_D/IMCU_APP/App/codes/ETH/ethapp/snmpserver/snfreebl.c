/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   snfreebl.c                                                   *
 *  Date:       April 1, 2008                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */
#include "kernel.h"
#include "snmp.h"
#include "main.h"
//caili
//#define          MEM_SIZE1    156
//#define          MEM_NUM1        20

//extern int  temp_i_snbentry;

//extern int temp_i_malloc_char;
//extern int temp_i_malloc_u_char;
//extern OS_MEM  *p_MEM;
//extern  INT8U	  MEM_ARRAY[MEM_NUM1][MEM_SIZE1];
void
snfreebl(struct snbentry **bl)
{
	
	
	
	if (*bl == (void *)NULL)
		return;
	
	
       gs_trap.temp_i_snbentry=0;
gs_trap.temp_i_malloc_char=0;
gs_trap.temp_i_malloc_u_char=0;
 memset(&gs_trap.temp_snbentry[0],0,sizeof(gs_trap.temp_snbentry[0])*30);
        *bl = (struct snbentry *)NULL;
        
}
//#endif	/* SNMP */
