/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   mib.h                                                   *
 *  Date:       April 1, 2008                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */
#ifndef _MIB_H_
#define _MIB_H_

#ifndef SUPPORT_TRAP_TABLE
#define SUPPORT_TRAP_TABLE
#endif
#define IDENT    0
#define IDENTMANUFACTURER  1
#define IDENTMODEL  2
#define IDENTCONTROLLERFIRMWAREVERSION  3
#define IDENTNAME  4
#define SYSSTATUE  5
#define SYSVOLTAGE  6
#define SYSCURRENT  7
#define SYSUSEDCAPACITY  8
#define PSBATTERYVOLTAGE  9
#define PSBATTERYCURRENT  10
#define PSINPUTLINEAVOLTAGE  11
#define PSINPUTLINEBVOLTAGE  12
#define PSINPUTLINECVOLTAGE  13
#define PSTEMPERATURE1  14
#define PSTEMPERATURE2  15
#define PSSTATUSCOMMUNICATION  16
#define PSSTATUSBATTERYMODE  17
#define PSSMACNUMBER  18
#define PSSMBATNUMBER  19
#define PSSMIONUMBER  20
//Modified by hulw 090429 to support Trap alarm table

#define ALARMLASTTRAPNO  21

#define ALARMTRAPNO  22
#define ALARMTIME  23
#define ALARMSTATUSCHANGE  24
#define ALARMSEVERITY  25
#define ALARMDESCRIPTION  26
#define ALARMTYPE  27

#define POWERMIB  28
#define MAX_SEND 35
#define ERR_SNP_OK  1

/* mib information structure: 1 per mib entry;  Initialized in snmib.c	*/
struct mib_info {
//	int     iNode;
	char	*mi_name;	/* name of mib variable in English	*/
	char	*mi_prefix;	/* prefix in English (e.g., "tcp.")	*/
	struct 	oid mi_objid;	/* object identifier			*/
	int	mi_vartype;	/* type: integer, aggregate, octet str	*/
	Bool	mi_writable;	/* is this variable writable?		*/
	Bool	mi_varleaf;	/* is this a leaf with a single value	*/
	int	(*mi_func)(struct snbentry *, struct mib_info *, int);
				/* function to implement get/set/getnext*/
	char	*mi_param;	/* parameter used with function		*/

	struct 	mib_info *mi_next; /* pointer to next var. in lexi-order*/
	int i_Node;
};

extern 	struct mib_info mib[];	/* array with one entry per MIB variable*/
extern	int mib_entries;	/* number of entries in mib array	*/



#define	LEAF		1	/* This MIB variable is a leaf.		*/
#define	NLEAF		0	/* This MIB variable is not a leaf.	*/

#define	T_TCPTABLE		0x0	/* var is the TCP conn. table	*/
#define	T_RTTABLE		0x1	/* var is the routing table	*/
#define	T_AETABLE		0x2	/* var is the address entry tbl	*/
#define	T_ATTABLE		0x3	/* var is the addr translat tbl	*/
#define	T_IFTABLE		0x4	/* var is the interface table	*/
#define	T_UDPTABLE		0x5	/* var is the UDP table		*/
#define	T_NMTABLE		0x6	/* var is the IP NetToMedia table */
/*#define T_EGPTABLE		0x7*/	/* var is the egp table		*/
#define	T_AGGREGATE		0x8	/* var is an aggregate		*/

/* this type specifies in mib.c that the object is a table.  The value is
   different than any of the ASN.1 types that SNMP uses. */
#define T_TABLE			01

#endif /* _MIB_H_ */
