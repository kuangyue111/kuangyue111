#include "main.h"
#include "Uart4and5.h"
#include "CAN_BMS.h"
NOINIT UINT8 gUart4RecvBuff[RS485_UART4_RX_MAX_NUM];
NOINIT UINT8 gUart4EVSendData[RS485_UART4_TX_MAX_NUM];
Uart4_5_Comm_TypeDef gUART4Comm;


static void RefreshUart4Time(void)
{
	struct tm  tmNow;
	static UCHAR  Sec = 0;
	UCHAR i;
	gRTCData.fGetLocalTime(&tmNow);
	if((UCHAR)tmNow.tm_sec != Sec)/*1秒钟*/
	{
		Sec = (UCHAR)tmNow.tm_sec;	
		if(gUART4Comm.u8SendDataDelay >0)
			gUART4Comm.u8SendDataDelay--;  
	}
}
static UINT8 Uart4_LenCheckSum(UINT16 wLen)
{
	UINT8 byLenCheckSum;

	//若长度为0，不需要计算
	if (wLen == 0)
	{
		return 0;
	}

	byLenCheckSum = 0;
	byLenCheckSum += wLen & 0x000F;         //取最后4个BIT
	byLenCheckSum += (wLen >> 4) & 0x000F;  //取第4~7个BIT
	byLenCheckSum += (wLen >> 8) & 0x000F;  //取第8~11个BIT
	byLenCheckSum %= 16;                    //模16
	byLenCheckSum = (~byLenCheckSum) + 1;   //取反加1
	byLenCheckSum &= 0x0F;                  //只取4个BIT

	return byLenCheckSum;
}
static UINT16 UART4_CalcSendCheckSum(UINT8* pSendBuf, UINT16 dataL)
{
	UINT16 i = 0;
	UINT16 IICRC;
	IICRC = 0;
	for (i = 0; i < dataL; i++)
	{
		IICRC += pSendBuf[YDN23_VER_PST + i];
	}
	IICRC = IICRC%65536;

	return ((~IICRC)+1);
}
static void UART4_PackFrameHead(UINT8 ucDevAddress,UINT8 ucCMD1,UINT8 ucCMD2,UINT16 u16DataLength)
{
	UINT8 lenChk;
	UCHAR* pBuff = gUart4EVSendData;
	pBuff[0]= YDN23_SOI_VAL;
	pBuff[1] = Hex2Ascii(YDN23_VER_VAL>>4);
	pBuff[2] = Hex2Ascii(YDN23_VER_VAL&0x0f);
	pBuff[3] = Hex2Ascii(ucDevAddress>>4);
	pBuff[4] = Hex2Ascii(ucDevAddress&0x0f);
	pBuff[5] = Hex2Ascii(ucCMD1>>4);
	pBuff[6] = Hex2Ascii(ucCMD1&0x0f);
	pBuff[7] = Hex2Ascii(ucCMD2>>4);
	pBuff[8] = Hex2Ascii(ucCMD2&0x0f);

	lenChk = Uart4_LenCheckSum(u16DataLength);						//Because init value 13
	pBuff[9] = Hex2Ascii((lenChk & 0x0f));
	pBuff[10] = Hex2Ascii((u16DataLength >> 8) & 0x0f);
	pBuff[11] = Hex2Ascii(((u16DataLength) & 0xf0)>>4);
	pBuff[12] = Hex2Ascii((u16DataLength) & 0x0f);
}
static void  UART4_PackFrameEnd(UINT16 u16Length)
{
	UINT16 u16CHECKSUM = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UCHAR* pBuff = gUart4EVSendData;

	u16CHECKSUM = UART4_CalcSendCheckSum(pBuff, u16Length-1);
	CheckSumH = 0xff&(u16CHECKSUM>>8);
	CheckSumL = 0xff&(u16CHECKSUM);
	pBuff[u16Length++] = Hex2Ascii(CheckSumH>>4);
	pBuff[u16Length++] = Hex2Ascii(CheckSumH&0x0f);
	pBuff[u16Length++] = Hex2Ascii(CheckSumL>>4);
	pBuff[u16Length++] = Hex2Ascii(CheckSumL&0x0f);
	pBuff[u16Length++] = YDN23_EOI_VAL;
};
static void UART4_SendFrame(UCHAR ucCMD,UINT16 u16length)
{
	dv_Uart4.fSendData(gUart4EVSendData,u16length);
	gUART4Comm.u8SendDataDelay = 5;
	gUART4Comm.u8SendCmdFlag = ucCMD;
}

static UCHAR UART4_CheckNewFrame(void)
{
	return dv_Uart4.ucFrameFinishFlag;
}
static UCHAR UART4_CheckFrame(void)
{
	UCHAR lenH = 0,lenL= 0,temp = 0;
	UINT16 CheckSum = 0,i,datalen,GetSum;
	UCHAR *pbuffer;
	pbuffer = gUart4RecvBuff;
	if(pbuffer[0] != YDN23_START)
	{
		return FALSE;
	}
	temp = TwoAscii2Hex(pbuffer[1],pbuffer[2]);
	if((temp != YDN23_ProtocolVersion1)&& (temp != YDN23_ProtocolVersion2))
	{
		return FALSE;
	}
	/*temp = TwoAscii2Hex(pbuffer[3],pbuffer[4]);
	if(temp != gUART4Comm.u8GetDeviceAddress) 
	{
		return FALSE;
	}
	temp = TwoAscii2Hex(pbuffer[5],pbuffer[6]);
	if((temp != IMIU_CID1)&&(temp !=IMEU_CID1)) 
	{
		return FALSE;
	}
	temp = TwoAscii2Hex(pbuffer[7],pbuffer[8]);
	if(temp != YDN23_RTN_OK) 
	{
		return FALSE;
	}*/

	lenH = TwoAscii2Hex(pbuffer[9],pbuffer[10]);
	lenL = TwoAscii2Hex(pbuffer[11],pbuffer[12]);

	temp = lenH & 0x0f;						/*高字节低4bit*/
	temp += lenL >> 4;						/*低字节高4bit*/
	temp += lenL & 0x0f;					/*低字节低4bit*/
	temp = (~temp + 1) & 0x0f;					/*取反加1再取低4bit*/
	if((lenH >> 4) != temp)
	{
		return FALSE;
	}	
	datalen = 	(lenH & 0x0f)*256+lenL;

	for(i=0;i<(12+datalen);i++)
	{
		CheckSum += pbuffer[i+1];
	}
	CheckSum =~CheckSum+1;               
	GetSum = TwoAscii2Hex(pbuffer[13+datalen],pbuffer[13+datalen+1])*256;   
	GetSum += TwoAscii2Hex(pbuffer[13+datalen+2],pbuffer[13+datalen+3]); 
	if(CheckSum !=  GetSum) 
	{
		return FALSE;
	}

	if(pbuffer[13+datalen+4] !=YDN23_END)
	{
		return FALSE;
	}  
	return  TRUE;
}

static void TestPackDataSend(void)
{
	UCHAR i, uctemp;
	UINT16 u16Length;
	UCHAR* pBuff = gUart4EVSendData;
	u16Length = YDN23_Data_PST;
	pBuff[u16Length++] = Hex2Ascii(0);
	UART4_PackFrameHead(YDN23_IMEU_ADDR,IMEU_CID1,IMEU1_DO_CTRL_CMD,u16Length-YDN23_Data_PST);
	UART4_PackFrameEnd(u16Length);
	UART4_SendFrame(IMEU1_DO_CTRL_CMD,u16Length+5);
}
static void UART4_ReadCommDataProcess(void)
{
	UCHAR ucRTN,uctemp;
	ucRTN = UART4_CheckNewFrame();

	if(ucRTN== TRUE)
	{
		uctemp = UART4_CheckFrame();
		if(uctemp== TRUE)
		{
			gUART4Comm.u8SendCmdFlag = SendDataCMD;
			gUART4Comm.u8SendDataDelay = 30;
		}
	}
}
void TASK_Uart4Proc(void* pdata)
{
	dv_Uart4.fInitUart(m9600, UARTX_PARITY_NONE);				//InitUart4
	dv_Uart4.fEnableRx();							//使能收中断
	dv_Uart4.ReceiveFlag = YDN23_NONE;
	gUART4Comm.u8SendCmdFlag = SendDataCMD;
	gUART4Comm.u8SendDataDelay = 0;
	dv_Uart4.fRS485CTL(mReceive);
	OSTimeDly(50);
	for(;;)
	{
		gSelfTest.fSetTaskID(TASK_ID_UART4);
		OSTimeDly(20);
		RefreshUart4Time();
		
		if((gUART4Comm.u8SendCmdFlag == GetDataCMD)&&(gUART4Comm.u8SendDataDelay !=0))//等待接收数据则停止发送其他命令
		{
			UART4_ReadCommDataProcess();
		}
		else if((gUART4Comm.u8SendDataDelay ==0)&&(gUART4Comm.u8SendCmdFlag != SendDataCMD))
		{
			gUART4Comm.u8SendCmdFlag = SendDataCMD;
		}
		if((gUART4Comm.u8SendDataDelay ==0)&&(gUART4Comm.u8SendCmdFlag == SendDataCMD))
		{
			TestPackDataSend();
			gUART4Comm.u8SendCmdFlag= GetDataCMD;
			gUART4Comm.u8SendDataDelay =5;
		}
	}
}
