#include "main.h"
#include "Uart6_new.h"


void MdbusDeal(void)
{
	unsigned long u32SigAddr;
	UINT16 u16Value;
	UINT16 byFrameBytesNum = 0;
	UINT16 u16Idx = 0;
	UINT8 ArryIdx = 0;
	static UINT32 u32CheckUart6Times = 0;

	gSelfTest.fSetTaskID(Task_ID_Console);

	for (ArryIdx = 0; ArryIdx < MDBUS_RCV_ARRY_NUM; ArryIdx++)
	{
		if (TRUE == gMdbusRcvInfo[ArryIdx].uReceivFlag)
		{
			if((gMdbusRcvInfo[ArryIdx].u16ReceivLenth == 12)
				&& (gMdbusRcvInfo[ArryIdx].uBuf[1] == 0x77))
			{
				u32SigAddr = ((unsigned long)gMdbusRcvInfo[ArryIdx].uBuf[2]<<24) + ((unsigned long)gMdbusRcvInfo[ArryIdx].uBuf[3]<<16) + ((unsigned long)gMdbusRcvInfo[ArryIdx].uBuf[4]<<8) + (gMdbusRcvInfo[ArryIdx].uBuf[5]);
				u16Value = ((unsigned short)gMdbusRcvInfo[ArryIdx].uBuf[6]<<8) + (gMdbusRcvInfo[ArryIdx].uBuf[7]);

				if(u32SigAddr == TOPWAY_16BIT_ADDR_CURR_PAGE_NO)
				{
					
				}
			}


			gMdbusRcvInfo[ArryIdx].uReceivFlag = FALSE;
		}
	}
}
