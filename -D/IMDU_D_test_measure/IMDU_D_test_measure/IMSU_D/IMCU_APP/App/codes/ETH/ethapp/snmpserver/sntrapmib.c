/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   sntrapmib.c                                                   *
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
#include "mibtrap.h"
#include "asn1.h"

#include "mib.h"
 
 
//Modified by hulw 090308 Incorrect sequence of trap
///* All table and leaf variables that are found in the MIB */
//const struct mib_trap_info mib_trap[] = {
//  
//		//{  { {1,2,1,1,3,0},6},ASN1_TIMETICKS, sn_trap,TRAP_7},
//		//{{ {1, 6, 3,1,1,4,1,0}, 8}, ASN1_OBJID,sn_trap,TRAP_8},
//		  {  { {1,2,1,1,3,0},6},ASN1_TIMETICKS, sn_trap,TRAP_7},
//		{{ {1, 6, 3,1,1,4,1,0}, 8}, ASN1_OBJID,sn_trap,TRAP_8},
//		{{ {1, 4, 1,6302,2,1,4,1,1}, 9}, ASN1_INT,sn_trap,TRAP_1},
//
//		{{ {1, 4, 1,6302,2,1,4,1,2}, 9}, ASN1_INT ,sn_trap,TRAP_2},
//		{{ {1, 4, 1,6302,2,1,4,1,3}, 9}, ASN1_INT  ,sn_trap,TRAP_3},
//		{{ {1, 4, 1,6302,2,1,4,1,4}, 9}, ASN1_INT  ,sn_trap,TRAP_4},
//		{{ {1, 4, 1,6302,2,1,4,1,5}, 9}, ASN1_OCTSTR,sn_trap,TRAP_5},
//		{{ {1, 4, 1,6302,2,1,4,1,6}, 9}, ASN1_OCTSTR,sn_trap,TRAP_6},
//
//
//};
/* All table and leaf variables that are found in the MIB */
const struct mib_trap_info mib_trap[] = {
  
		{  { {1,2,1,1,3,0},6},ASN1_TIMETICKS, sn_trap,TRAP_7},
		{{ {1, 6, 3,1,1,4,1,0}, 8}, ASN1_OBJID,sn_trap,TRAP_8},

		{{ {1, 4, 1,6302,2,1,4,1,1}, 9}, ASN1_COUNTER,sn_trap,TRAP_1},
		{{ {1, 4, 1,6302,2,1,4,1,2}, 9}, ASN1_OCTSTR,sn_trap,TRAP_5},
		{{ {1, 4, 1,6302,2,1,4,1,3}, 9}, ASN1_INT  ,sn_trap,TRAP_4},
		{{ {1, 4, 1,6302,2,1,4,1,4}, 9}, ASN1_INT  ,sn_trap,TRAP_3},
		{{ {1, 4, 1,6302,2,1,4,1,5}, 9}, ASN1_OCTSTR,sn_trap,TRAP_6},
		{{ {1, 4, 1,6302,2,1,4,1,6}, 9}, ASN1_INT ,sn_trap,TRAP_2},

};

int mib_trap_entries = sizeof(mib_trap) / sizeof(struct mib_trap_info);


