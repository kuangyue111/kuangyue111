/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   snmib.c                                                   *
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
#include "mib.h"
#include "asn1.h"

extern struct oid SysObjectID;

#define SAVE_FLASH_OCCUPIED

#ifndef SAVE_FLASH_OCCUPIED

//Modified by hulw 090429 to support SNMP WALK
/* All table and leaf variables that are found in the MIB */
struct mib_info mib[] = {
	{ "ident", "global.", { {1,4,1,6302,2,1,1},7}, T_AGGREGATE,
		FALSE, NLEAF, 0, 0, 0,IDENT},
	{ "identManufacturer", "ident.", { {1, 4, 1,6302,2,1,1,1,0}, 9}, ASN1_OCTSTR,
	FALSE, LEAF, snleaf, 0, 0,IDENTMANUFACTURER},
	{ "identModel", "ident.", { {1, 4, 1,6302,2,1,1,2,0}, 9}, ASN1_OCTSTR,
	FALSE, LEAF, snleaf, 0, 0,IDENTMODEL},
	{ "identControllerFirmwareVersion", "ident.", { {1, 4, 1,6302,2,1,1,3,0}, 9}, ASN1_OCTSTR,
	FALSE, LEAF, snleaf, 0, 0,IDENTCONTROLLERFIRMWAREVERSION},
	{ "identName", "ident.", { {1, 4, 1,6302,2,1,1,4,0}, 9}, ASN1_OCTSTR,
	TRUE, LEAF, snleaf, 0, 0,IDENTNAME},
	{ "systemstatue", "system.", { {1, 4, 1,6302,2,1,2,1,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSSTATUE},
	{ "systemvoltage", "system.", { {1, 4, 1,6302,2,1,2,2,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSVOLTAGE},
	{ "systemcurrent", "system.", { {1, 4, 1,6302,2,1,2,3,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSCURRENT},
	{ "systemusedcapacity", "system.", { {1, 4, 1,6302,2,1,2,4,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSUSEDCAPACITY},
	{ "psbatteryvoltage", "psBattery.", { {1,4,1,6302,2,1,2,5,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSBATTERYVOLTAGE},
	{ "psBatteryCurrent", "psBattery.", { {1,4,1,6302,2,1,2,5,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSBATTERYCURRENT},
	{ "psInputLineAVoltage", "psInput.", { {1,4,1,6302,2,1,2,6,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSINPUTLINEAVOLTAGE},
	{ "psInputLineBVoltage", "psInput.", { {1,4,1,6302,2,1,2,6,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSINPUTLINEBVOLTAGE},
	{ "psInputLineCVoltage", "psInput.", { {1,4,1,6302,2,1,2,6,3,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSINPUTLINECVOLTAGE},
	{ "psTemperature1", "psTemperature.", { {1,4,1,6302,2,1,2,7,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSTEMPERATURE1},
	{ "psTemperature2", "psTemperature.", { {1,4,1,6302,2,1,2,7,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSTEMPERATURE2},
	{ "psStatusCommunication", "psStatus.", { {1,4,1,6302,2,1,2,8,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSTATUSCOMMUNICATION},
	{ "psStatusBatteryMode", "psStatus.", { {1,4,1,6302,2,1,2,9,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSTATUSBATTERYMODE},

	{ "psSMACNumber", "psSMNumber.", { {1,4,1,6302,2,1,2,10,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSMACNUMBER},
	{ "psSMBATNumber", "psSMNumber.", { {1,4,1,6302,2,1,2,10,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSMBATNUMBER},
	
        { "psSMIONumber", "psSMNumber.", { {1,4,1,6302,2,1,2,10,3,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSMIONUMBER},

	{ "alarmLastTrapNo", "powerMIB.", { {1,4,1,6302,2,1,3,0}, 8}, ASN1_COUNTER,
	FALSE, LEAF, snleaf, 0, 0,ALARMLASTTRAPNO},
        
 #ifdef SUPPORT_TRAP_TABLE  
 
	{ "alarmTrapNo", "alarmTrapEntry.", { {1,4,1,6302,2,1,4,1,1,1}, 10}, ASN1_COUNTER,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMTRAPNO},

	{ "alarmTime", "alarmTrapEntry.", { {1,4,1,6302,2,1,4,1,2,1}, 10}, ASN1_OCTSTR,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMTIME},

	{ "alarmStatusChange", "alarmTrapEntry.", { {1,4,1,6302,2,1,4,1,3,1}, 10}, ASN1_INT,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMSTATUSCHANGE},

	{ "alarmSeverity", "alarmTrapEntry.", { {1,4,1,6302,2,1,4,1,4,1}, 10}, ASN1_INT,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMSEVERITY},

	{ "alarmDescription", "alarmTrapEntry.", { {1,4,1,6302,2,1,4,1,5,1}, 10}, ASN1_OCTSTR,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMDESCRIPTION},

	{ "alarmType", "alarmTrapEntry.", { {1,4,1,6302,2,1,4,1,6,1}, 10}, ASN1_INT,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMTYPE},
#endif//SUPPORT_TRAP_TABLE

	{"powerMIB", "global.", { {1,4,1,6302,2,1},6}, T_AGGREGATE,
	FALSE, NLEAF, 0, 0, 0,POWERMIB},

};

int mib_entries = sizeof(mib) / sizeof(struct mib_info);\

void sninit(void)
{
	//Modified by hulw 090429 to support SNMP WALK
	for(int i = 0; i< mib_entries - 2; i++)
	{
		mib[i].mi_next = &(mib[i+1]);
	}

	mib[mib_entries - 1].mi_next = &mib[IDENTMANUFACTURER];

}

#else

/* All table and leaf variables that are found in the MIB */
struct mib_info mib[] = {
	{ "", "", { {1,4,1,6302,2,1,1},7}, T_AGGREGATE,
		FALSE, NLEAF, 0, 0, 0,IDENT},
	{ "", "", { {1, 4, 1,6302,2,1,1,1,0}, 9}, ASN1_OCTSTR,
	FALSE, LEAF, snleaf, 0, 0,IDENTMANUFACTURER},
	{ "", "", { {1, 4, 1,6302,2,1,1,2,0}, 9}, ASN1_OCTSTR,
	FALSE, LEAF, snleaf, 0, 0,IDENTMODEL},
	{ "", "", { {1, 4, 1,6302,2,1,1,3,0}, 9}, ASN1_OCTSTR,
	FALSE, LEAF, snleaf, 0, 0,IDENTCONTROLLERFIRMWAREVERSION},
	{ "", "", { {1, 4, 1,6302,2,1,1,4,0}, 9}, ASN1_OCTSTR,
	TRUE, LEAF, snleaf, 0, 0,IDENTNAME},
	{ "", "", { {1, 4, 1,6302,2,1,2,1,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSSTATUE},
	{ "", "", { {1, 4, 1,6302,2,1,2,2,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSVOLTAGE},
	{ "", "", { {1, 4, 1,6302,2,1,2,3,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSCURRENT},
	{ "", "", { {1, 4, 1,6302,2,1,2,4,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,SYSUSEDCAPACITY},
	{ "", "", { {1,4,1,6302,2,1,2,5,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSBATTERYVOLTAGE},
	{ "", "", { {1,4,1,6302,2,1,2,5,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSBATTERYCURRENT},
	{ "", "", { {1,4,1,6302,2,1,2,6,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSINPUTLINEAVOLTAGE},
	{ "", "", { {1,4,1,6302,2,1,2,6,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSINPUTLINEBVOLTAGE},
	{ "", "", { {1,4,1,6302,2,1,2,6,3,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSINPUTLINECVOLTAGE},
	{ "", "", { {1,4,1,6302,2,1,2,7,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSTEMPERATURE1},
	{ "", "", { {1,4,1,6302,2,1,2,7,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSTEMPERATURE2},
	{ "", "", { {1,4,1,6302,2,1,2,8,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSTATUSCOMMUNICATION},
	{ "", "", { {1,4,1,6302,2,1,2,9,0}, 9}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSTATUSBATTERYMODE},

	{ "", "", { {1,4,1,6302,2,1,2,10,1,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSMACNUMBER},
	{ "", "", { {1,4,1,6302,2,1,2,10,2,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSMBATNUMBER},
	
        { "", "", { {1,4,1,6302,2,1,2,10,3,0}, 10}, ASN1_INT,
	FALSE, LEAF, snleaf, 0, 0,PSSMIONUMBER},

	{ "", "", { {1,4,1,6302,2,1,3,0}, 8}, ASN1_COUNTER,
	FALSE, LEAF, snleaf, 0, 0,ALARMLASTTRAPNO},
        
 #ifdef SUPPORT_TRAP_TABLE  
 
	{ "", "", { {1,4,1,6302,2,1,4,1,1,1}, 10}, ASN1_COUNTER,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMTRAPNO},

	{ "", "", { {1,4,1,6302,2,1,4,1,2,1}, 10}, ASN1_OCTSTR,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMTIME},

	{ "", "", { {1,4,1,6302,2,1,4,1,3,1}, 10}, ASN1_INT,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMSTATUSCHANGE},

	{ "", "", { {1,4,1,6302,2,1,4,1,4,1}, 10}, ASN1_INT,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMSEVERITY},

	{ "", "", { {1,4,1,6302,2,1,4,1,5,1}, 10}, ASN1_OCTSTR,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMDESCRIPTION},

	{ "", "", { {1,4,1,6302,2,1,4,1,6,1}, 10}, ASN1_INT,
	FALSE, LEAF, snalarmleaf, 0, 0,ALARMTYPE},
#endif//SUPPORT_TRAP_TABLE

	{"", "", { {1,4,1,6302,2,1},6}, T_AGGREGATE,
	FALSE, NLEAF, 0, 0, 0,POWERMIB},

};

int mib_entries = sizeof(mib) / sizeof(struct mib_info);\

void sninit(void)
{
	//Modified by hulw 090429 to support SNMP WALK
	for(int i = 0; i< mib_entries - 2; i++)
	{
		mib[i].mi_next = &(mib[i+1]);
	}
	mib[mib_entries - 1].mi_next = &mib[IDENTMANUFACTURER];
}

#endif //SAVE_FLASH_OCCUPIED
