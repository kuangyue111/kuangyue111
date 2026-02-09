#ifndef UDISK_H
#define UDISK_H


struct _U_DISK_DATA_ST
{
	UCHAR ucCopyRecordFlag;
	UINT16 u16RecordStartNum;
	UINT16 u16RecordEndNum;
	UCHAR  ucRecordWriteStep;
	UINT16 u16RecordWriteIndex;	
	UINT16 u16RecordWriteTestIndex;
	UINT16 u16RecordWriteAgingIndex;
	UCHAR ucFileName[30];
};
typedef struct _U_DISK_DATA_ST U_DISK_DATA_ST;
extern U_DISK_DATA_ST gs_UDiskData;




#endif

