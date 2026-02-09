/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   mibtrap.c                                                   *
 *  Date:       April 1, 2008                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */
#ifndef _MIB_TRAP_H_
#define _MIB_TRAPH_
#define TRAP_1 1
#define TRAP_2 2
#define TRAP_3 3
#define TRAP_4 4
#define TRAP_5 5
#define TRAP_6 6
#define TRAP_7 7
#define TRAP_8 8
/* mib information structure: 1 per mib entry;  Initialized in snmib.c	*/
struct mib_trap_info {

	struct 	oid mi_objid;	/* object identifier			*/
	int	mi_vartype;

	//Modified by hulw 090319 处理TRAP时时间串中可能有'/0'，则只被发送'/0'前字串，造成信息不全
	//int	(*mi_trap)(int ,long* , char*);
	int	(*mi_trap)(int ,long* , char*, int*);
	int trap_iNode;
				/* function to implement get/set/getnext*/


};

extern 	const struct mib_trap_info mib_trap[];	/* array with one entry per MIB variable*/
extern	int mib_trap_entries;	/* number of entries in mib array	*/





#endif /* _MIB_H_ */
