#ifndef UPGRADE_H
#define UPGRADE_H

#include "main.h"
#include "EtherNet.h"
/* 说明
   共16Mbits(2M Bytes) 分32 Blocks 
   每个Block分16个扇区，每个扇区16Pages,每个Page 256bytes
   每Block: 有256页,有64Kbyes
   每sector: 有4Kbytes
    
   Block0: 0x00 0000 - 0x00 FFFF
   Block1: 0x01 0000 - 0x01 FFFF
   .
   .
   .
   Block31: 0x1F 0000 - 0x1F FFFF

   擦除最小单位为sector, 费时200ms. 擦除block要2秒，擦除chip要20秒。
	写数据时，使用02命令，page program
	支持一次写最少1个字节，最多256个字节。
*/

//最大支持512K bytes
#define EX_FLASH_INFO						(256*2)    //存放文件信息?
#define EX_UPGRADE_START_BLOCK_NO			23		
#define EX_UPGRADE_BLOCK_MAX_NUM			8
#define EX_RECV_DATA_MAX_NUM				((EX_UPGRADE_BLOCK_MAX_NUM*(PAGE_NUM_PER_BLOCK*BYTES_NUM_PER_PAGE))-EX_FLASH_INFO)
#define EX_RECV_CACHE_MAX_NUM				(PAGE_NUM_PER_BLOCK*BYTES_NUM_PER_PAGE)
#define EX_RECV_ONE_CACHE_NUM				512

#define UPGRADE_OPT_IDL					0
#define UPGRADE_OPT_EREASE				1
#define UPGRADE_OPT_NAME				2
#define UPGRADE_OPT_SIZE				3
#define UPGRADE_OPT_TRANSMIT			4
#define UPGRADE_OPT_TRANSMIT_FINISH		5


#define EX_RECV_CACHE_MAX_NUM				(PAGE_NUM_PER_BLOCK*BYTES_NUM_PER_PAGE)
struct UpGradeInfo
{
	UINT8 u8UpGradeOpt;
	UINT32 u32FileSize;
	UINT32 u32TatolBlock;
	UINT32 u32BlockNo;
	UINT32 u32RcvBytesNum;
	UINT8 UpGradeData[EX_RECV_CACHE_MAX_NUM+1];
	INT32 i32UpDataTimeOut;
};
typedef struct UpGradeInfo   __UPGRADE_INFO;

extern NOINIT __UPGRADE_INFO  gUpGradeData;

#endif
