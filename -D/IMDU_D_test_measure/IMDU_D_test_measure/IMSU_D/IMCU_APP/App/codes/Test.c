#include "main.h"


NOINIT UINT8 Uart6_YDN23_SendBuff[UART6_YDN23_SEND_MAX_NUM];

NOINIT CanRxMsgTypeDef CanRxMsgBuff_TEST_A[CAN_A_B_TEST_BUF_MAX_NUM];
NOINIT CanRxMsgTypeDef CanRxMsgBuff_TEST_B[CAN_A_B_TEST_BUF_MAX_NUM];
uint32_t pRecvTEST_CAN2Idx = 0;
uint32_t pRecvTEST_CAN1Idx = 0;
UINT8 YDN23AnalayseUart6(UINT8 iAddr);
void Send41CMD(void);
void Send46CMD(void);
void CMD48XXXSetProc(void);
void CMD45XXXSetProc(void);
void CMDE1Reset(void);
void DO_Test_Deal(void);
void CAN_Test_Dela(void);
void InitCan3_250KPort(void);
void UART_Test_Deal(void);
static UINT8 uYDN23CheckReceive(void)	//Unit MS
{
	if (dv_Uart6.uRecvFinish == YDN23_END)
	{
		dv_Uart6.uRecvFinish = YDN23_NONE;
		return TRUE;
	}
	return FALSE;
}

void Send41CMD(void)
{
	//UINT8 u8Value= 0;
	UINT8 uTempVal = 0;
	UINT8 iSendLength = 0;
	UINT8 lenChk = 0;
	UINT16 iLenghId = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	UINT8 pFloatArarry[20];
	memset(Uart6_YDN23_SendBuff,0,UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL>>4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL&0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR>>4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR&0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1>>4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1&0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);

	iLenghId = 13;
	//1	板载温度
	floattoascii(gEvChargeInfo.fEV_InterTemp, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//2	外扩温度
	floattoascii(gEvChargeInfo.fEV_Ex2Temp, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//3	霍尔电压1
	floattoascii(gEvChargeInfo.fEV_BattVolt_1, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//4	霍尔电压2
	floattoascii(gEvChargeInfo.fEV_BattVolt_2, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//5	绝缘板分流器电流
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//6	绝缘板母排电压
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//7	绝缘板正对PE电压
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//8	绝缘板负对PE电压
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//9	绝缘板正对PE电阻
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//10	绝缘板负对PE电阻
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//11	绝缘板告警
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0&0x0f);

	//12	绝缘动作状态
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0&0x0f);

	//13	充电枪1位置
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_SpearPst[GUN_NO_A]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_SpearPst[GUN_NO_A]&0x0f);

	//14	充电枪2位置
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_SpearPst[GUN_NO_B]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_SpearPst[GUN_NO_B]&0x0f);

	/**************************************************************
		.0----.7
		15	板内DI1状态 
		16	板内DI2状态
		17	板内DI3状态
		18	板内DI4状态
		19	预留		RS232-2接口测试状态
		20	预留		RS232-3接口测试状态
		21	预留		CAN接口测试状态
		22	预留		485接口测试状态
	****************************************************************/
	//u8Value = gEvChargeInfo.u8EV_InterDI_COM&0x0f;
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_InterDI_COM >>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_InterDI_COM &0x0f);

	/******************************************************************
		 .0---.7
		 23	外接DI1状态
		 24	外接DI2状态
		 25	外接DI3状态
		 26	外接DI4状态
		 27	外接DI5状态
		 28	外接DI6状态
		 29	外接DI7状态
		 30	外接DI8状态
	*******************************************************************/
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_Ex1DI>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_Ex1DI&0x0f);

	/******************************************************************
		 .0---.7
		 31	外扩DI1状态
		 32	外扩DI2状态
		 33	外扩DI3状态
		 34	外扩DI4状态
		 35	预留	板内DI5
		 36	预留	板内DI6
		 37	预留	板内DI7
		 38	预留	板内DI8
	*******************************************************************/
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_Ex2DI>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_Ex2DI&0x0f);

	/******************************************************************
		 .0---.7
		39	板内信号DO1状态
		40	板内信号DO2状态
		41	板内信号DO3状态
		42	板内信号DO4状态
		43	板内信号DO5状态
		44	板内信号DO6状态
		45	板内 功率 DO1状态	DO7状态
		46	板内 功率 DO2状态	DO8状态
	*******************************************************************/
	//gEvChargeInfo.u8EV_InterDO &= ~(0x01<<6);
	//gEvChargeInfo.u8EV_InterDO &= ~(0x01<<7);
	//gEvChargeInfo.u8EV_InterDO |= ((gEvChargeInfo.u8EV_InterLVD & 0x01)<< 6);
	//gEvChargeInfo.u8EV_InterDO |= (((gEvChargeInfo.u8EV_InterLVD>>1) & 0x01)<< 7);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_InterDO>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(gEvChargeInfo.u8EV_InterDO&0x0f);

	/*******************************************************************
		 .0---.7
		47	板内功率DO3状态	   板内 DO9状态
		48	外接功率DO1状态	   板内 DO10状态
		49	外接功率DO2状态	   板内 DO11状态
		50	风机控制1
		51	风机控制2
		52	
		53	
		54	
	*********************************************************************/
	UINT8 u8DOValue1 = 0;
	u8DOValue1 |= gEvChargeInfo.u8EV_DO9_16 & 0x07;
	u8DOValue1 |= (gEvChargeInfo.u8Fan1Fan2Stat&0x03) << 3;
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(u8DOValue1 >> 4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(u8DOValue1 & 0x0f);

	/***************************************************************************
		.0---.7
		55	外扩DO1状态
		56	外扩DO2状态
		57	外扩DO3状态
		58	外扩DO4状态
		59	外扩DO5状态
		60	外扩DO6状态
		61	外扩DO7状态
		62	外扩DO8状态
	***************************************************************************/
	uTempVal = gEvChargeInfo.u16EV_Ex2DO & 0xff;
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(uTempVal >> 4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(uTempVal & 0x0f);

	/***************************************************************************
		 .0---.7
		63	外扩DO9状态
		64	外扩DO10状态
		65	外扩DO11状态
		66		板内DO12 
		67		板内DO13
		68		板内DO14
		69		板内DO15
		70		板内DO16
	***************************************************************************/
	uTempVal= 0;
	uTempVal = (gEvChargeInfo.u16EV_Ex2DO >> 8) & 0x07;
	uTempVal |= (gEvChargeInfo.u8EV_DO9_16 &0xf8); 
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(uTempVal >> 4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(uTempVal & 0x0f);

	lenChk = LenCheckSum((iLenghId -13));//Because init value 13
	Uart6_YDN23_SendBuff[9] = Hex2Ascii((lenChk & 0x0f));
	Uart6_YDN23_SendBuff[10] = Hex2Ascii(((iLenghId -13)>>8) & 0x0f);
	Uart6_YDN23_SendBuff[11] = Hex2Ascii(((iLenghId -13) & 0xf0)>>4);
	Uart6_YDN23_SendBuff[12] = Hex2Ascii((iLenghId -13) & 0x0f);

	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, iLenghId);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = YDN23_EOI_VAL;
	iSendLength = iLenghId;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff,iSendLength);
	OSTimeDly(50);
}

void Send46CMD(void)
{
	UINT8 iSendLength = 0;
	UINT8 lenChk = 0;
	UINT16 iLenghId = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	UINT8 pFloatArarry[20];
	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL>>4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL&0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR>>4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR&0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1>>4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1&0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);

	iLenghId = 13;
	//1	霍尔1校准系数a
	floattoascii(gEvChargeInfo.fBattVolt_1CoefA, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//2	霍尔1校准系数b
	floattoascii(gEvChargeInfo.fBattVolt_1CoefB, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//3	霍尔2校准系数a
	floattoascii(gEvChargeInfo.fBattVolt_2CoefA, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//4	霍尔2校准系数b
	floattoascii(gEvChargeInfo.fBattVolt_2CoefB, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//5	绝缘板分流器系数
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//6	绝缘板分流器电流校准系数a
	floattoascii(1, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//7	绝缘板分流器电流校准系数b
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//8	绝缘板母排电压采样校准系数a
	floattoascii(1, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//9	绝缘板母排电压采样校准系数b
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//10	绝缘告警绝缘电阻值
	floattoascii(0, pFloatArarry);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[0]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(pFloatArarry[3]&0x0f);

	//11	绝缘检测控制状态
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0&0x0f);

	//12	电池反接是否告警
	__XXX_UNION_VALUE unVal;
	ExGetSigInfo(E_TYPE_SYS, ID_u8PolarityAlmMask_2, &unVal, NULL, BY_SRAM);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(unVal.chtemp[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(unVal.chtemp[0]&0x0f);
	
	UINT8 mac[8];
	for(UINT8 i=0;i<6;i++)
	{
		//GetCharSetData(SETDATA_Mac_NUM+i,&mac[i]);
		ExGetSigInfo(E_TYPE_SYS, ID_u8Mac_1 + i, &unVal, NULL, BY_SRAM);
		mac[i] = unVal.chtemp[0];
	}
	

	//MAC地址 高四字节
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(0);

	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[5]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[5]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[4]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[4]&0x0f);

	//MAC地址 低四字节
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[3]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[3]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[2]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[2]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[1]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[1]&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[0]>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(mac[0]&0x0f);

	struct tm  tmNow;
	gRTCData.fDataInitRTC();
	gRTCData.fGetLocalTime(&tmNow);
	//tmNow.tm_sec = BCDToHex(dv_RTC.fReadRTC(SECONDSREG) & 0x7f);		/*秒*/
	//tmNow.tm_min = BCDToHex(dv_RTC.fReadRTC(MINUTESREG) & 0x7f);		/*分*/
	//tmNow.tm_hour = BCDToHex(dv_RTC.fReadRTC(HOURSREG) & 0x3f);			/*时*/
	//tmNow.tm_mday = BCDToHex(dv_RTC.fReadRTC(DAYSREG) & 0x3f);			/*日*/
	//tmNow.tm_wday = RTCWeekToHex(dv_RTC.fReadRTC(WEEKDAYSREG) & 0x7f);	/*星期*/
	//tmNow.tm_mon = BCDToHex(dv_RTC.fReadRTC(MONTHSREG) & 0x1f) - 1;		/*月*/
	//tmNow.tm_year = BCDToHex(dv_RTC.fReadRTC(YEARSREG)) + 2000 - 1900;	/*年*/

	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_year-2010)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_year-2010)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_mon)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_mon)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_mday)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_mday)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_hour)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_hour)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_min)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_min)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_sec)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((tmNow.tm_sec)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((0xff)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((0xff)&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((0xff)>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii((0xff)&0x0f);


	lenChk = LenCheckSum((iLenghId -13));//Because init value 13
	Uart6_YDN23_SendBuff[9] = Hex2Ascii((lenChk & 0x0f));
	Uart6_YDN23_SendBuff[10] = Hex2Ascii(((iLenghId -13)>>8) & 0x0f);
	Uart6_YDN23_SendBuff[11] = Hex2Ascii(((iLenghId -13) & 0xf0)>>4);
	Uart6_YDN23_SendBuff[12] = Hex2Ascii((iLenghId -13) & 0x0f);

	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, iLenghId);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[iLenghId++] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[iLenghId++] = YDN23_EOI_VAL;
	iSendLength = iLenghId;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff,iSendLength);
	OSTimeDly(20);
}


void CMD48XXXSetProc(void)
{
	UINT8 u8Buf[6];
	struct tm  tmNow;
	UINT16* pBuf = Uart6_YDN23_RecvBuff;
	UINT8 u8CmdType = 0;
	float fVal = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	//UCHAR	ucDHCPClientEnable = 0;
	float fK1 = 1;
	float fK1_2 = 1;
	__XXX_UNION_VALUE unTempVal;
	u8CmdType = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
	switch (u8CmdType)
	{
	case 0x80://80H		霍尔1校准系数a		霍尔1校准系数b
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		fK1 = fVal;
		gEvChargeInfo.fBattVolt_1CoefA = fK1 * gEvChargeInfo.fBattVolt_1CoefA;
		unTempVal.ftemp = gEvChargeInfo.fBattVolt_1CoefA;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_1CoefA, unTempVal, NULL, BY_E2PROM);

		pBuf = Uart6_YDN23_RecvBuff + 15 + 8;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		if (fK1 < 0.8 || fK1 >= 1.3)
		{
			break;
		}
		if ((fVal < -100) || (fVal > 100))
		{
			break;
		}
		//gEvChargeInfo.fBattVolt_1CoefB = fK1 * gEvChargeInfo.fBattVolt_1CoefB;
		gEvChargeInfo.fBattVolt_1CoefB = fK1*gEvChargeInfo.fBattVolt_1CoefB + fVal;
		unTempVal.ftemp = gEvChargeInfo.fBattVolt_1CoefB;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_1CoefB, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x81://2	81H		霍尔2校准系数a		霍尔2校准系数b
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		fK1_2 = fVal;
		gEvChargeInfo.fBattVolt_2CoefA = fK1_2 * gEvChargeInfo.fBattVolt_2CoefA;
		unTempVal.ftemp = gEvChargeInfo.fBattVolt_2CoefA;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_2CoefA, unTempVal, NULL, BY_E2PROM);

		pBuf = Uart6_YDN23_RecvBuff + 15 + 8;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		if (fK1_2 < 0.8 || fK1_2 >= 1.3)
		{
			break;
		}
		if ((fVal < -100) || (fVal > 100))
		{
			break;
		}
		//gEvChargeInfo.fBattVolt_2CoefB = fK1_2 * gEvChargeInfo.fBattVolt_2CoefB;
		gEvChargeInfo.fBattVolt_2CoefB = gEvChargeInfo.fBattVolt_2CoefB + fVal;
		unTempVal.ftemp = gEvChargeInfo.fBattVolt_2CoefB;
		ExSetSigInfo(E_TYPE_SYS, ID_fBattVolt_2CoefB, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x82://3	82H	绝缘板分流器系数	0
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		//gEvChargeInfo.fSetJYMShunt_Coef[0] = fVal;
		break;
	case 0x83:	//4	83H	绝缘板电流采样校准系数a	绝缘板电流采样校准系数b
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		//gEvChargeInfo.fSetJYMCurr_CoefA[0]  = fVal;

		pBuf = Uart6_YDN23_RecvBuff + 15 + 8;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		//gEvChargeInfo.fSetJYMCurr_CoefB[0]  = fVal;
		break;
	case 0x84://5	84H	绝缘板母排电压采样校准系数a	绝缘板母排电压采样校准系数b
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		//gEvChargeInfo.fSetJYMVolt_CoefA[0]  = fVal;

		pBuf = Uart6_YDN23_RecvBuff + 15 + 8;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		//gEvChargeInfo.fSetJYMVolt_CoefA[0]  = fVal;
		break;
	case 0x85://6	85H	绝缘告警绝缘电阻值	0
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		//gEvChargeInfo.fSetJYMAlm_Resist[0]  = fVal;
		//gEvChargeInfo.u8FloatParamUpDataCount++;
		break;
	case 0x86://7	86H	绝缘检测控制(3：手动 2：后台 1：自动，0：禁止)	0
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		break;
	case 0x87://8	87H	电池反接是否告警( 1：使能，0禁止)
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_SYS, ID_u8PolarityAlmMask_2, unTempVal, NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_SYS, ID_u8PolarityAlmMask_1, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x88:		//充电电压
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		break;
	case 0x89:		//充电电流
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		break;
	case 0x8A://充电时间
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		break;
	case 0x8b:												//基准电流
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		break;
	case 0x8c:													//WARKING IN 时间 
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		gEvChargeInfo.fRectLoadStartTime = fVal;
		unTempVal.ftemp = fVal;
		ExSetSigInfo(E_TYPE_SYS, ID_fRectLoadStartTime, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x8D:												//系统约束电流
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		u8Buf[1] = TwoAscii2Hex(*(pBuf+2), *(pBuf+3));
		u8Buf[2] = TwoAscii2Hex(*(pBuf+4), *(pBuf+5));
		u8Buf[3] = TwoAscii2Hex(*(pBuf+6), *(pBuf+7));
		FourByte2float(u8Buf, &fVal);
		unTempVal.ftemp = fVal;
		ExSetSigInfo(E_TYPE_SYS, ID_fSetLimitMaxOutCurr_1, unTempVal, NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_SYS, ID_fSetLimitMaxOutCurr_2, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x8E://电表类型	0 直流表	1交流表
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_WH2, ID_u8WH_TYPE, unTempVal, NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_WH1, ID_u8WH_TYPE, unTempVal, NULL, BY_E2PROM);
		//gEvChargeInfo.u8WH_TYPE  = u8Buf[0];
		//gEvChargeInfo.u8CharParamUpDataCount++;
		break;
	case 0x8F://电表告警屏蔽 1禁用不告警	0不屏蔽
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_WH2, ID_u8WH_ALM_Mask, unTempVal, NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_WH1, ID_u8WH_ALM_Mask, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x90://刷卡机告警屏蔽
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		//unTempVal.chtemp[0] = u8Buf[0];
		//ExSetSigInfo(E_TYPE_BRUSH, ID_u8WH_ALM_Mask, unTempVal, NULL, BY_E2PROM);
		gEvChargeInfo.u8BRUSH_ALM_Mask  = u8Buf[0];				//没存参数
		//gEvChargeInfo.u8CharParamUpDataCount++;
		break;
	case 0x91://风机控制极性	1 极性取反
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_SYS, ID_u8PWMPolarityCover, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x92://门禁告警	1 禁止	0使能
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_SYS, ID_u8DoorAlmMask_1, unTempVal, NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_SYS, ID_u8DoorAlmMask_2, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x93://SPD 防雷告警	1 禁止	0使能
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_SYS, ID_u8SPDAlmMask, unTempVal, NULL, BY_E2PROM);
		break;
	case 0x94:
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		unTempVal.chtemp[0] = u8Buf[0];
		ExSetSigInfo(E_TYPE_SYS, ID_NET_DHCP, unTempVal, NULL, BY_E2PROM);
		break;
	case 0xa0:
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));//年
		tmNow.tm_year = u8Buf[0] + 2010;
		pBuf+=2;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));//月
		tmNow.tm_mon = u8Buf[0];
		pBuf+=2;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));//日
		tmNow.tm_mday = u8Buf[0];
		pBuf+=2;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));//时				
		tmNow.tm_hour = u8Buf[0];
		pBuf+=2;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));//分
		tmNow.tm_min = u8Buf[0];
		pBuf+=2;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));//秒
		tmNow.tm_sec = u8Buf[0];
		pBuf+=2;
		gRTCData.fSetLocalTime(&tmNow);
		break;
	default:
		iCID2 = 0x04;
		break;
	}
	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);

	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1 >> 4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1 & 0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);
	Uart6_YDN23_SendBuff[9] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[10] = 0;
	Uart6_YDN23_SendBuff[11] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[12] = 0;
	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 10);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
	OSTimeDly(20);
}



void CMD45XXXSetProc(void)
{
	NEWDoChannel_TypeDef u8DOIdx = NEWDO1;
	UINT8 u8Buf[4];
	UINT16* pBuf = Uart6_YDN23_RecvBuff;
	UINT8 u8CmdType = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	u8CmdType = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
	switch (u8CmdType)
	{
	case 0x80://80H		后台控制绝缘检测命令	0x02待机，0x01检测，0x00脱离
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		//gEvChargeInfo.u8CTRLInsulationCMD[0] = u8Buf[0];
		//MutexLock(gEvChargeInfo.hMutexWriteEVInfo);
		//gEvChargeInfo.u8CTRLInsulationCMDFlag[0] = TRUE;
		//MutexUnlock(gEvChargeInfo.hMutexWriteEVInfo);
		break;

	case 0x81:/***********************************************************************
			81H	板内DO状态控制动作
			（低4位表示DO号，高4位表示动作）	DO号：0x00~0x0f
			动作：0x10：动作  0x00：正常
		*************************************************************************/
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		gEvChargeInfo.u8CTRLInterDOCMD = u8Buf[0];
		gEvChargeInfo.u8CTRLInterDOCMDFlag = TRUE;
		break;
	case 0x96:
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		gEvChargeInfo.u8CTRLInterDOCMD = u8Buf[0];   //9~15对应DO10~DO16
		gEvChargeInfo.u8CTRLInterDOCMDFlag_2 = TRUE;
		break;

	case 0x82:/*********************************************************************
			82H	外接DO状态控制
			（低4位表示DO号，高4位表示动作）	DO号：0x00~0x0f
			动作：0x10：动作  0x00：正常
		**************************************************************************/
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		gEvChargeInfo.u8CTRLEx1DOCMD = u8Buf[0];
		gEvChargeInfo.u8CTRLEx1DOCMDFlag = TRUE;
		break;
	case 0x83:/****************************************************************************
			  DO号：0x00~0x0f
			  0x10：动作  0x00：正常
			  0~8对应DO1~DO9
			  9代表风机1//全速，停机
			  10代表风机2//全速，停机
			  11~15预留
			83H	外扩DO状态控制
			（低4位表示DO号，高4位表示动作）	DO号：0x00~0x0f
			动作：0x10：动作  0x00：正常		
		*******************************************************************************/
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		gEvChargeInfo.u8CTRLEx2DOCMD = u8Buf[0];
		gEvChargeInfo.u8CTRLEx2DOCMDFlag = TRUE;
		break;
	case 0x84:
		//gEvChargeInfo.uChargeTypeNO = TIME_IMMEDIATELY_TYPE;
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		if (1 == u8Buf[0])
		{
			//SetChargeSTART_CMD(START_FROM_CONSOLE);
		}
		else
		{
			//SetChargeSTOP_Cmd(CONSOLE_STOP_CMD); //后台 串口方式 停止充电
		}
		break;
	case 0x95:
		pBuf = Uart6_YDN23_RecvBuff + 15;
		u8Buf[0] = TwoAscii2Hex(*pBuf, *(pBuf+1));
		if (1 == u8Buf[0])
		{
			gEvChargeInfo.u8TEST_MODE_FLAG = TRUE;
			gEvChargeInfo.u8EV_InterDO |= 0x01;
			gEvChargeInfo.u8EV_InterDO |= (0x01<<1);
			gEvChargeInfo.u8EV_InterDO |= (0x01<<2);
			gEvChargeInfo.u8EV_InterDO |= (0x01<<3);
			gEvChargeInfo.u8EV_InterDO |= (0x01<<4);
			gEvChargeInfo.u8EV_InterDO |= (0x01<<5);
			gEvChargeInfo.u8EV_InterLVD |= (0x01<<0);
			gEvChargeInfo.u8EV_InterLVD |= (0x01<<1);
			gEvChargeInfo.u8EV_DO9_16 |= (0x01<<0);
			gEvChargeInfo.u8EV_DO9_16 |= (0x01<<1);
			gEvChargeInfo.u8EV_DO9_16 |= (0x01<<1);
			dv_Capture_PWM.fOutputFanDriver(mFAN1,100); //风机调速
			gEvChargeInfo.u8Fan1Fan2Stat |= (0x01);
			for (u8DOIdx = NEWDO1; u8DOIdx <= NEWDO16; u8DOIdx++)
			{
				BSP_DO_Ctrl(u8DOIdx, DO_ON);
			}
		}
		else
		{
			gEvChargeInfo.u8TEST_MODE_FLAG = FALSE;
			for (u8DOIdx = NEWDO1; u8DOIdx <= NEWDO16; u8DOIdx++)
			{
				BSP_DO_Ctrl(u8DOIdx, DO_OFF);
			}
		}
		break;
	default:
		iCID2 = 0x04;
		break;
	}

	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1 >> 4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1 & 0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);
	Uart6_YDN23_SendBuff[9] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[10] = 0;
	Uart6_YDN23_SendBuff[11] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[12] = 0;
	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 10);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
	OSTimeDly(20);
}

void CMDE1Reset(void)
{
	UINT8 test1,test2;
	UINT8 boot_flag[2];
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	
	test1 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
	test2 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);
	if((test1 == 0xa5) && (test2 == 0x55))
	{
		boot_flag[0] = LOCALFLAGFST;
		boot_flag[1] = LOCALFLAGSEC;
		dv_E2PROM.fWriteE2PROMPage(boot_flag, E2PROMFLAGADD1, 2);

		if(gSelfTest.StopFeedDogReason == 0)
		{
			gSelfTest.StopFeedDogReason = 6;
			//gSelfTest.u32DogBit_Bak = ((UINT32)E2PROMPageRead[0]<<8) + ((UINT32)gSelfTest.fLastSetDataValue & 0xff) + ((UINT32)gSelfTest.LastSetDataID<<16);
			gs_E2PROM.fSetHistoryEventUpdate();
		}
		Trap();
	}
	else
	{
		iCID2 = 0x04;
	}
	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1 >> 4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1 & 0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);
	Uart6_YDN23_SendBuff[9] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[10] = 0;
	Uart6_YDN23_SendBuff[11] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[12] = 0;
	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 10);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
	OSTimeDly(20);
}
void ReadUart6MAC(void);
void ReadUart6MAC(void)
{
		//UINT8 test1,test2, test3, u8temp, u8Buf[4];
		__XXX_UNION_VALUE unTempVal;

		UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	
		UINT8 CheckSumH = 0;
		UINT8 CheckSumL = 0;
		UINT16 iCHECKSUM = 0;

		for(UINT8 i = 0; i < 6; i++)
		{
			ExGetSigInfo(E_TYPE_SYS, ID_u8Mac_1 + i, &unTempVal,NULL, BY_E2PROM);
			u8Mac[i] = unTempVal.chtemp[0];
		}
	
		/*test1 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
		test2 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);
		test3 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[17], Uart6_YDN23_RecvBuff[18]);
		u16temp = (((UINT16)test2) << 8) + ((UINT16)test3 & 0xff);
		if(test1 == 0x00)
		{
			GetCharSetData(u16temp , u8Buf); 
		}
		else if(test1 == 0x01)
		{
			GetFloatSetData(u16temp ,&ftemp); 
	
			floattoascii(ftemp, u8Buf);
		}*/
	
		Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
		Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
		Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
		Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
		Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
		Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
		Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
		Uart6_YDN23_SendBuff[7] = Hex2Ascii(0);
		Uart6_YDN23_SendBuff[8] = Hex2Ascii(0);
		Uart6_YDN23_SendBuff[9] = 0x34;					//LENGTH
		Uart6_YDN23_SendBuff[10] = 0x30;
		Uart6_YDN23_SendBuff[11] = 0x30;					//LENGTH
		Uart6_YDN23_SendBuff[12] = 0x3C;
		Uart6_YDN23_SendBuff[13] = Hex2Ascii(u8Mac[0]>>4);
		Uart6_YDN23_SendBuff[14] = Hex2Ascii(u8Mac[0]&0x0f);
		Uart6_YDN23_SendBuff[15] = Hex2Ascii(u8Mac[1]>>4);
		Uart6_YDN23_SendBuff[16] = Hex2Ascii(u8Mac[1]&0x0f);
		Uart6_YDN23_SendBuff[17] = Hex2Ascii(u8Mac[2]>>4);
		Uart6_YDN23_SendBuff[18] = Hex2Ascii(u8Mac[2]&0x0f);
		Uart6_YDN23_SendBuff[19] = Hex2Ascii(u8Mac[3]>>4);
		Uart6_YDN23_SendBuff[20] = Hex2Ascii(u8Mac[3]&0x0f);
		Uart6_YDN23_SendBuff[21] = Hex2Ascii(u8Mac[4]>>4);
		Uart6_YDN23_SendBuff[22] = Hex2Ascii(u8Mac[4]&0x0f);
		Uart6_YDN23_SendBuff[23] = Hex2Ascii(u8Mac[5]>>4);
		Uart6_YDN23_SendBuff[24] = Hex2Ascii(u8Mac[5]&0x0f);

	
		iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 24);
		CheckSumH = 0xff&(iCHECKSUM>>8);
		CheckSumL = 0xff&(iCHECKSUM);
		Uart6_YDN23_SendBuff[25] = Hex2Ascii(CheckSumH>>4);
		Uart6_YDN23_SendBuff[26] = Hex2Ascii(CheckSumH&0x0f);
		Uart6_YDN23_SendBuff[27] = Hex2Ascii(CheckSumL>>4);
		Uart6_YDN23_SendBuff[28] = Hex2Ascii(CheckSumL&0x0f);
		Uart6_YDN23_SendBuff[29] = YDN23_EOI_VAL;
		dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 30);
		OSTimeDly(35);
}
void SetUart6MAC(void);
void SetUart6MAC(void)
{
	//UINT8 u8Buf[6];
	UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	//UINT16* pBuf = Uart6_YDN23_RecvBuff;
	//UINT8 u8CmdType = 0;
	//float fVal = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	__XXX_UNION_VALUE unTempVal;
	u8Mac[5] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
	u8Mac[4] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);	
	u8Mac[3] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[17], Uart6_YDN23_RecvBuff[18]);	
	u8Mac[2] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[19], Uart6_YDN23_RecvBuff[20]);	
	u8Mac[1] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[21], Uart6_YDN23_RecvBuff[22]);	
	u8Mac[0] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[23], Uart6_YDN23_RecvBuff[24]);	
	for(UINT8 i = 0; i < 6; i++)
	{
		unTempVal.chtemp[0] = u8Mac[i];
		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_1 + i, unTempVal,NULL, BY_E2PROM);
		//SetCharSetData(SETDATA_Mac_NUM + i, &u8Mac[i]);
	}

	dv_E2PROM.fWriteE2PROMPage(u8Mac, E2PROMMACADD, 6);//恢复默认配置的时候，可以将MAC地址写回去。

	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);
	Uart6_YDN23_SendBuff[9] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[10] = 0;
	Uart6_YDN23_SendBuff[11] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[12] = 0;
	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 10);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
	OSTimeDly(20);
}

void ReadUart6HWVer(void);
void ReadUart6HWVer(void)
{
		//UINT8 test1,test2, test3, u8temp, u8Buf[4];
    	//__XXX_UNION_VALUE unTempVal;

		UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	
		UINT8 CheckSumH = 0;
		UINT8 CheckSumL = 0;
		UINT16 iCHECKSUM = 0;

		dv_E2PROM.fReadE2PROMPage(u8Mac, E2PROM_HW_VER_HIGH, 2);

		gEvChargeInfo.u8HardwareVer[0] = u8Mac[0];
		gEvChargeInfo.u8HardwareVer[1] = u8Mac[1];
	
		Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
		Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
		Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
		Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
		Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
		Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
		Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
		Uart6_YDN23_SendBuff[7] = Hex2Ascii(0);
		Uart6_YDN23_SendBuff[8] = Hex2Ascii(0);
		Uart6_YDN23_SendBuff[9] = 0x3C;					//LENGTH
		Uart6_YDN23_SendBuff[10] = 0x30;
		Uart6_YDN23_SendBuff[11] = 0x30;					//LENGTH
		Uart6_YDN23_SendBuff[12] = 0x34;
		Uart6_YDN23_SendBuff[13] = Hex2Ascii(u8Mac[0]>>4);
		Uart6_YDN23_SendBuff[14] = Hex2Ascii(u8Mac[0]&0x0f);
		Uart6_YDN23_SendBuff[15] = Hex2Ascii(u8Mac[1]>>4);
		Uart6_YDN23_SendBuff[16] = Hex2Ascii(u8Mac[1]&0x0f);
			
		iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 16);
		CheckSumH = 0xff&(iCHECKSUM>>8);
		CheckSumL = 0xff&(iCHECKSUM);
		Uart6_YDN23_SendBuff[17] = Hex2Ascii(CheckSumH>>4);
		Uart6_YDN23_SendBuff[18] = Hex2Ascii(CheckSumH&0x0f);
		Uart6_YDN23_SendBuff[19] = Hex2Ascii(CheckSumL>>4);
		Uart6_YDN23_SendBuff[20] = Hex2Ascii(CheckSumL&0x0f);
		Uart6_YDN23_SendBuff[21] = YDN23_EOI_VAL;
		dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 22);
		OSTimeDly(35);
}

void SetUart6Level3Password(void);
void SetUart6Level3Password(void)
{
	//UINT8 u8Buf[6];
	UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	//UINT16* pBuf = Uart6_YDN23_RecvBuff;
	//UINT8 u8CmdType = 0;
	//float fVal = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	__XXX_UNION_VALUE unTempVal;
	u8Mac[0] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
	u8Mac[1] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);	
	u8Mac[2] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[17], Uart6_YDN23_RecvBuff[18]);	
	u8Mac[3] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[19], Uart6_YDN23_RecvBuff[20]);	
	u8Mac[4] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[21], Uart6_YDN23_RecvBuff[22]);	
	u8Mac[5] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[23], Uart6_YDN23_RecvBuff[24]);	
	for(UINT8 i = 0; i < 6; i++)
	{
		unTempVal.chtemp[0] = u8Mac[i];
		ExSetSigInfo(E_TYPE_SYS, ID_u8OffLinePwsd1 + i, unTempVal,NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_SYS, ID_UserPassword_L2_1 + i, unTempVal,NULL, BY_E2PROM);
		ExSetSigInfo(E_TYPE_SYS, ID_UserPassword_L3_1 + i, unTempVal,NULL, BY_E2PROM);
	}
	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);
	Uart6_YDN23_SendBuff[9] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[10] = 0;
	Uart6_YDN23_SendBuff[11] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[12] = 0;
	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 10);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
	OSTimeDly(20);
}
void ReadUart6Level3Password(void);
void ReadUart6Level3Password(void)
{
		//UINT8 test1,test2, test3, u8temp, u8Buf[4];
		__XXX_UNION_VALUE unTempVal;

		UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	
		UINT8 CheckSumH = 0;
		UINT8 CheckSumL = 0;
		UINT16 iCHECKSUM = 0;

		for(UINT8 i = 0; i < 6; i++)
		{
			ExGetSigInfo(E_TYPE_SYS, ID_UserPassword_L3_1 + i, &unTempVal,NULL, BY_E2PROM);
			u8Mac[i] = unTempVal.chtemp[0];
		}
	
		Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
		Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
		Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
		Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
		Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
		Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
		Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
		Uart6_YDN23_SendBuff[7] = Hex2Ascii(0);
		Uart6_YDN23_SendBuff[8] = Hex2Ascii(0);
		Uart6_YDN23_SendBuff[9] = 0x34;					//LENGTH
		Uart6_YDN23_SendBuff[10] = 0x30;
		Uart6_YDN23_SendBuff[11] = 0x30;					//LENGTH
		Uart6_YDN23_SendBuff[12] = 0x3C;
		Uart6_YDN23_SendBuff[13] = Hex2Ascii(u8Mac[0]>>4);
		Uart6_YDN23_SendBuff[14] = Hex2Ascii(u8Mac[0]&0x0f);
		Uart6_YDN23_SendBuff[15] = Hex2Ascii(u8Mac[1]>>4);
		Uart6_YDN23_SendBuff[16] = Hex2Ascii(u8Mac[1]&0x0f);
		Uart6_YDN23_SendBuff[17] = Hex2Ascii(u8Mac[2]>>4);
		Uart6_YDN23_SendBuff[18] = Hex2Ascii(u8Mac[2]&0x0f);
		Uart6_YDN23_SendBuff[19] = Hex2Ascii(u8Mac[3]>>4);
		Uart6_YDN23_SendBuff[20] = Hex2Ascii(u8Mac[3]&0x0f);
		Uart6_YDN23_SendBuff[21] = Hex2Ascii(u8Mac[4]>>4);
		Uart6_YDN23_SendBuff[22] = Hex2Ascii(u8Mac[4]&0x0f);
		Uart6_YDN23_SendBuff[23] = Hex2Ascii(u8Mac[5]>>4);
		Uart6_YDN23_SendBuff[24] = Hex2Ascii(u8Mac[5]&0x0f);

	
		iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 24);
		CheckSumH = 0xff&(iCHECKSUM>>8);
		CheckSumL = 0xff&(iCHECKSUM);
		Uart6_YDN23_SendBuff[25] = Hex2Ascii(CheckSumH>>4);
		Uart6_YDN23_SendBuff[26] = Hex2Ascii(CheckSumH&0x0f);
		Uart6_YDN23_SendBuff[27] = Hex2Ascii(CheckSumL>>4);
		Uart6_YDN23_SendBuff[28] = Hex2Ascii(CheckSumL&0x0f);
		Uart6_YDN23_SendBuff[29] = YDN23_EOI_VAL;
		dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 30);
		OSTimeDly(35);
}
void SetUart6HWVer(void);
void SetUart6HWVer(void)
{
	//UINT8 u8Buf[6];
	UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	//UINT16* pBuf = Uart6_YDN23_RecvBuff;
	//UINT8 u8CmdType = 0;
	//float fVal = 0;
	UINT8 iCID2 = 0;
	UINT8 CheckSumH = 0;
	UINT8 CheckSumL = 0;
	UINT16 iCHECKSUM = 0;
	//__XXX_UNION_VALUE unTempVal;
	u8Mac[0] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
	u8Mac[1] = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);	
	
	dv_E2PROM.fWriteE2PROMPage(u8Mac, E2PROM_HW_VER_HIGH, 2);//恢复默认配置的时候，可以将MAC地址写回去。

	gEvChargeInfo.u8HardwareVer[0] = u8Mac[0];
	gEvChargeInfo.u8HardwareVer[1] = u8Mac[1];

	memset(Uart6_YDN23_SendBuff, 0, UART6_YDN23_SEND_MAX_NUM);
	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
	Uart6_YDN23_SendBuff[7] = Hex2Ascii(iCID2>>4);
	Uart6_YDN23_SendBuff[8] = Hex2Ascii(iCID2&0x0f);
	Uart6_YDN23_SendBuff[9] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[10] = 0;
	Uart6_YDN23_SendBuff[11] = 0;					//LENGTH
	Uart6_YDN23_SendBuff[12] = 0;
	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 10);
	CheckSumH = 0xff&(iCHECKSUM>>8);
	CheckSumL = 0xff&(iCHECKSUM);
	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
	OSTimeDly(20);
}
//void WriteSetData(void)
//{
//	UINT8 test1,test2, test3, u8temp, u8Buf[4];
//	UINT16 u16temp;
//	float ftemp;
//	UINT8 CheckSumH = 0;
//	UINT8 CheckSumL = 0;
//	UINT16 iCHECKSUM = 0;
//
//	__XXX_UNION_VALUE unTempVal;
//
//	test1 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
//	test2 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);
//	test3 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[17], Uart6_YDN23_RecvBuff[18]);
//	u16temp = (((UINT16)test2) << 8) + ((UINT16)test3 & 0xff);
//
//	u8temp = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[19], Uart6_YDN23_RecvBuff[20]);
//	if(test1 == 0x00)
//	{
//		//SetCharSetData(u16temp, &u8temp);
//		unTempVal.chtemp[0] = u8temp;
//		ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_1 + i, unTempVal,NULL, BY_E2PROM);
//	}
//	else if(test1 == 0x01)
//	{
//		u8Buf[0] = TwoAscii2Hex(Uart6_YDN23_RecvBuff[19], Uart6_YDN23_RecvBuff[20]);
//		u8Buf[1] = TwoAscii2Hex(Uart6_YDN23_RecvBuff[21], Uart6_YDN23_RecvBuff[22]);
//		u8Buf[2] = TwoAscii2Hex(Uart6_YDN23_RecvBuff[23], Uart6_YDN23_RecvBuff[24]);
//		u8Buf[3] = TwoAscii2Hex(Uart6_YDN23_RecvBuff[25], Uart6_YDN23_RecvBuff[26]);
//		FourByte2float(u8Buf, &ftemp);
//		SetFloatSetData(u16temp, &ftemp); 
//	}
//	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
//	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
//	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
//	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
//	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
//	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
//	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
//	Uart6_YDN23_SendBuff[7] = Hex2Ascii(0);
//	Uart6_YDN23_SendBuff[8] = Hex2Ascii(0);
//	Uart6_YDN23_SendBuff[9] = 0x30;					//LENGTH
//	Uart6_YDN23_SendBuff[10] = 0x30;
//	Uart6_YDN23_SendBuff[11] = 0x30;					//LENGTH
//	Uart6_YDN23_SendBuff[12] = 0x30;
//	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 12);
//	CheckSumH = 0xff&(iCHECKSUM>>8);
//	CheckSumL = 0xff&(iCHECKSUM);
//	Uart6_YDN23_SendBuff[13] = Hex2Ascii(CheckSumH>>4);
//	Uart6_YDN23_SendBuff[14] = Hex2Ascii(CheckSumH&0x0f);
//	Uart6_YDN23_SendBuff[15] = Hex2Ascii(CheckSumL>>4);
//	Uart6_YDN23_SendBuff[16] = Hex2Ascii(CheckSumL&0x0f);
//	Uart6_YDN23_SendBuff[17] = YDN23_EOI_VAL;
//	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 18);
//	OSTimeDly(25);
//}
//
//void ReadSetData(void)
//{
//	UINT8 test1,test2, test3, u8temp, u8Buf[4];
//	UINT16 u16temp;
//	float ftemp;
//	__XXX_UNION_VALUE unTempVal;
//
//	UINT8 CheckSumH = 0;
//	UINT8 CheckSumL = 0;
//	UINT16 iCHECKSUM = 0;
//
//	test1 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[13], Uart6_YDN23_RecvBuff[14]);
//	test2 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[15], Uart6_YDN23_RecvBuff[16]);
//	test3 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[17], Uart6_YDN23_RecvBuff[18]);
//	u16temp = (((UINT16)test2) << 8) + ((UINT16)test3 & 0xff);
//	if(test1 == 0x00)
//	{
//		GetCharSetData(u16temp , u8Buf); 
//	}
//	else if(test1 == 0x01)
//	{
//		GetFloatSetData(u16temp ,&ftemp); 
//
//		floattoascii(ftemp, u8Buf);
//	}
//
//	Uart6_YDN23_SendBuff[0]= YDN23_SOI_VAL;
//	Uart6_YDN23_SendBuff[1] = Hex2Ascii(YDN23_VER_VAL >> 4);
//	Uart6_YDN23_SendBuff[2] = Hex2Ascii(YDN23_VER_VAL & 0x0f);
//	Uart6_YDN23_SendBuff[3] = Hex2Ascii(YDN23_IMCU_ADDR >> 4);
//	Uart6_YDN23_SendBuff[4] = Hex2Ascii(YDN23_IMCU_ADDR & 0x0f);
//	Uart6_YDN23_SendBuff[5] = Hex2Ascii(YDN23_IMCU_CID1_COPY >> 4);
//	Uart6_YDN23_SendBuff[6] = Hex2Ascii(YDN23_IMCU_CID1_COPY & 0x0f);
//	Uart6_YDN23_SendBuff[7] = Hex2Ascii(0);
//	Uart6_YDN23_SendBuff[8] = Hex2Ascii(0);
//	Uart6_YDN23_SendBuff[9] = 0x38;					//LENGTH
//	Uart6_YDN23_SendBuff[10] = 0x30;
//	Uart6_YDN23_SendBuff[11] = 0x30;					//LENGTH
//	Uart6_YDN23_SendBuff[12] = 0x38;
//	Uart6_YDN23_SendBuff[13] = Hex2Ascii(u8Buf[0]>>4);
//	Uart6_YDN23_SendBuff[14] = Hex2Ascii(u8Buf[0]&0x0f);
//	Uart6_YDN23_SendBuff[15] = Hex2Ascii(u8Buf[1]>>4);
//	Uart6_YDN23_SendBuff[16] = Hex2Ascii(u8Buf[1]&0x0f);
//	Uart6_YDN23_SendBuff[17] = Hex2Ascii(u8Buf[2]>>4);
//	Uart6_YDN23_SendBuff[18] = Hex2Ascii(u8Buf[2]&0x0f);
//	Uart6_YDN23_SendBuff[19] = Hex2Ascii(u8Buf[3]>>4);
//	Uart6_YDN23_SendBuff[20] = Hex2Ascii(u8Buf[3]&0x0f);
//
//	iCHECKSUM = CalcSendCheckSum(Uart6_YDN23_SendBuff, 20);
//	CheckSumH = 0xff&(iCHECKSUM>>8);
//	CheckSumL = 0xff&(iCHECKSUM);
//	Uart6_YDN23_SendBuff[21] = Hex2Ascii(CheckSumH>>4);
//	Uart6_YDN23_SendBuff[22] = Hex2Ascii(CheckSumH&0x0f);
//	Uart6_YDN23_SendBuff[23] = Hex2Ascii(CheckSumL>>4);
//	Uart6_YDN23_SendBuff[24] = Hex2Ascii(CheckSumL&0x0f);
//	Uart6_YDN23_SendBuff[25] = YDN23_EOI_VAL;
//	dv_Uart6.fSendDataTXY(Uart6_YDN23_SendBuff, 26);
//	OSTimeDly(35);
//}


UINT8 YDN23AnalayseUart6(UINT8 iAddr)
{
	UINT8 CID1 = 0;
	UINT8 CID2 = 0;
	UINT8 ADDR = 0;
	UINT16 IICRC = 0;
	UINT16 RevCRC = 0;
	UINT16 pCheckSumPst = 0;
	ADDR = TwoAscii2Hex(Uart6_YDN23_RecvBuff[YDN23_ADDR_PST], Uart6_YDN23_RecvBuff[YDN23_ADDR_PST+1]);
	CID1 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[YDN23_CID1_PST], Uart6_YDN23_RecvBuff[YDN23_CID1_PST + 1]);
	CID2 = (UINT8)TwoAscii2Hex(Uart6_YDN23_RecvBuff[YDN23_CID2_PST], Uart6_YDN23_RecvBuff[YDN23_CID2_PST + 1]);
	if (iAddr != ADDR)
	{
		return FALSE;
	}

	if (CID1 == YDN23_IMCU_CID1 || CID1 == YDN23_IMCU_CID1_COPY)
	{
		//return FALSE;
	}
	else
	{
		return FALSE;
	}

	if (dv_Uart6.Head < 13)
	{
		return FALSE;
	}

	IICRC = CalcRcvCheckSum(UART6_SEQUECE_NO);

	pCheckSumPst = dv_Uart6.Head - 5;
	//pCheckSumPst = MAX(0, pCheckSumPst);
	pCheckSumPst = MIN(UART6_YDN23_RECV_MAX_NUM, pCheckSumPst);
	RevCRC = (UINT16)(TwoAscii2Hex(Uart6_YDN23_RecvBuff[pCheckSumPst], Uart6_YDN23_RecvBuff[pCheckSumPst + 1]) << 8)
				+ TwoAscii2Hex(Uart6_YDN23_RecvBuff[pCheckSumPst + 2], Uart6_YDN23_RecvBuff[pCheckSumPst + 2 + 1]);
	if (IICRC != RevCRC)
	{
		return FALSE;
	}

	DO_Test_Deal();
	CAN_Test_Dela();
	UART_Test_Deal();
	ALM_LED_BLINK();
	switch (CID2)
	{
	case 0x41:
		Send41CMD();
		break;
	case 0x46:
		Send46CMD();
		break;
	case 0x48:
		CMD48XXXSetProc();
		break;
	case 0x45:
		CMD45XXXSetProc();
		break;
	case 0xE1:
		CMDE1Reset();
		break;
	case 0xd0:
		SetUart6MAC();
		break;
	case 0xd1:
		ReadUart6MAC();
		break;

	case 0xde:
		SetUart6HWVer();
		break;
	case 0xdf:
		ReadUart6HWVer();
		break;

	case 0xce:
		SetUart6Level3Password();
		break;
	case 0xcf:
		ReadUart6Level3Password();
		break;
	

	default:
		
		break;
	}
	return TRUE;
}

void DO_Test_Deal(void)
{
	UINT8 u8DOAction = 0;
	UINT8 u8DOno = 0;
	if (FALSE == gEvChargeInfo.u8TEST_MODE_FLAG)
	{
		return;
	}
	/***********************************************************************
			81H	板内DO状态控制动作
			（低4位表示DO号，高4位表示动作）	DO号：0x00~0x0f
			动作：0x10：动作  0x00：正常
	*************************************************************************/
	if (TRUE == gEvChargeInfo.u8CTRLInterDOCMDFlag)
	{
		gEvChargeInfo.u8CTRLInterDOCMDFlag = FALSE;
		u8DOno = gEvChargeInfo.u8CTRLInterDOCMD&0x0f;				//0~8对应DO1~DO9
		u8DOAction = gEvChargeInfo.u8CTRLInterDOCMD&0xf0;
		switch (u8DOno)
		{
		case 0:	   //DO1
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO1, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= 0x01;
			}
			else
			{
				BSP_DO_Ctrl(NEWDO1, DO_OFF);
				gEvChargeInfo.u8EV_InterDO &= ~0x01;
			}
			break;
		case 1:	   //DO2
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO2, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= (0x01<<1);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO2, DO_OFF);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<1);
			}
			break;
		case 2:	  //DO3
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO3, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= (0x01<<2);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO3, DO_OFF);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<2);
			}
			break;
		case 3:		//DO4
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO4, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= (0x01<<3);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO4, DO_OFF);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<3);
			}
			break;
		case 4:		//DO5
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO5, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= (0x01<<4);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO5, DO_OFF);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<4);
			}
			break;
		case 5:		//DO6			并机继电器
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO6, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= (0x01<<5);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO6, DO_OFF);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<5);
			}
			break;
		case 6://	   //DO7
			if (0x10== u8DOAction)
			{
				gEvChargeInfo.u8EV_InterDO |= (0x01<<6);
				//gEvChargeInfo.u8EV_InterLVD |= (0x01<<0);
				BSP_DO_Ctrl(NEWDO7, DO_ON);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO7, DO_OFF);
				//gEvChargeInfo.u8EV_InterLVD &= ~(0x01<<0);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<6);
			}
			break;
		case 7://		 //DO8
			if (0x10 == u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO8, DO_ON);
				gEvChargeInfo.u8EV_InterDO |= (0x01<<7);
				//gEvChargeInfo.u8EV_InterLVD |= (0x01<<1);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO8, DO_OFF);
				//gEvChargeInfo.u8EV_InterLVD &= ~(0x01<<1);
				gEvChargeInfo.u8EV_InterDO &= ~(0x01<<7);
			}
			break;
		case 8://		//DO9
			if (0x10 == u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO9, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<0);
			}
			else
			{
				
				BSP_DO_Ctrl(NEWDO9, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<0);
			}
			break;
		case 9:		//PWM1 见风扇控制线程
				if (0x10 == u8DOAction)
				{
					dv_Capture_PWM.fOutputFanDriver(mFAN1,100); //风机调速
					gEvChargeInfo.u8Fan1Fan2Stat |= (0x01);
					
				}
				else
				{
					dv_Capture_PWM.fOutputFanDriver(mFAN1,0); //风机调速
					gEvChargeInfo.u8Fan1Fan2Stat &= ~(0x01<<0);
				}
			break;
		case 10:	//PWM2 见风扇控制线程
				if (0x10 == u8DOAction)
				{
					//辅助源供电用！！
					gEvChargeInfo.u8Fan1Fan2Stat |= (0x01<<1);
				}
				else
				{
					gEvChargeInfo.u8Fan1Fan2Stat &= ~(0x01<<1);
				}
			break;
		default:
			break;
		}
	}

	
	if (TRUE == gEvChargeInfo.u8CTRLInterDOCMDFlag_2)
	{
		gEvChargeInfo.u8CTRLInterDOCMDFlag_2 = FALSE;
		u8DOno = gEvChargeInfo.u8CTRLInterDOCMD&0x0f;	//9~15对应DO10~DO16
		u8DOAction = gEvChargeInfo.u8CTRLInterDOCMD&0xf0;
		switch (u8DOno)
		{
		case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:
			break;
		case 9:	 //DO10
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO10, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<1);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO10, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<1);
			}
			break;
		case 10://DO11
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO11, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<2);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO11, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<2);
			}
			break;
		case 11://DO12
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO12, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<3);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO12, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<3);
			}
			break;
		case 12://DO13
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO13, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<4);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO13, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<4);
			}
			break;
		case 13://DO14
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO14, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<5);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO14, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<5);
			}
			break;
		case 14://DO15
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO15, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<6);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO15, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<6);
			}
			break;
		case 15://DO16
			if (0x10== u8DOAction)
			{
				BSP_DO_Ctrl(NEWDO16, DO_ON);
				gEvChargeInfo.u8EV_DO9_16 |= (0x01<<7);
			}
			else
			{
				BSP_DO_Ctrl(NEWDO16, DO_OFF);
				gEvChargeInfo.u8EV_DO9_16 &= ~(0x01<<7);
			}
			break;
		default:
			break;
		}
	}

	//82H	外接DO状态控制
	//（低4位表示DO号，高4位表示动作）	DO号：0x00~0x0f
	//动作：0x10：动作  0x00：正常
	//IMSU - D 已经没有 这个 数值 了。
	if (gEvChargeInfo.u8CTRLEx1DOCMDFlag == TRUE)//OUT14		0UT15
	{
		gEvChargeInfo.u8CTRLEx1DOCMDFlag = FALSE;
		UINT8 u8exDoNo = gEvChargeInfo.u8CTRLEx1DOCMD & 0x0f;
		UINT8 u8DoAction = gEvChargeInfo.u8CTRLEx1DOCMD & 0xf0;
		if (0 == u8exDoNo)
		{
			if (0x10 == u8DoAction)
			{
			}
			else
			{
			}
		}

		if (1 == u8exDoNo)
		{
			if (0x10 == u8DoAction)
			{
			}
			else
			{
			}
		}
	}
}

void InitCan3_250KPort(void)
{
	BSP_InitSPI3();
	OSTimeDly(500);
	UINT32 u32Mask_1 = 0x0380FF00;
	//UINT32 u32Mask_2 = 0x0380E000;
	UINT32 u32RcvID_1 = 0x0280F000;
	UINT32 u32RcvID_2 = 0x03001010;
	MCP2515_init(CAN_250KBPS);
	MCP2515_Set_RecID(0, u32Mask_1);
	MCP2515_Set_RecID(1, u32Mask_1);
	MCP2515_Set_Recmask(0, u32RcvID_1);
	MCP2515_Set_Recmask(1, u32RcvID_2);
	OSTimeDly(4);
}

void CAN_Test_Dela(void)
{
	canmsg Test_Msg;
	int i = 0;
	UINT8 u8Rst = 0;
	static int sialmDly = 0;
	if (FALSE == gEvChargeInfo.u8TEST_MODE_FLAG)
	{
		BSP_vR_LED_OFF();
		return;
	}

	InitCan3_250KPort();
	OSTimeDly(5);

	BSP_Can_1_Init();
	BSP_Can1_RxEnable();
	BSP_CanCheck(0);
	OSTimeDly(5);

	BSP_Can_2_Init();
	BSP_Can2_RxEnable();
	BSP_CanCheck(1);
	OSTimeDly(5);

	//Send 
	Test_Msg.Id = 0x177756F4;
	Test_Msg.Data[0] = 0x12345678;
	Test_Msg.Data[1] = 0x87654321;
	gs_CANComm.fTxCAN3Buff(&Test_Msg, 8);
	OSTimeDly(5);

	//Read
	for (i = 0; i < CAN_A_B_TEST_BUF_MAX_NUM; i++)
	{
		if (Test_Msg.Id == CanRxMsgBuff_TEST_B[i].ExtId)
		{
			u8Rst++;
			CanRxMsgBuff_TEST_B[i].ExtId = 0;
			break;
		}
	}

	//Read
	for (i = 0; i < CAN_A_B_TEST_BUF_MAX_NUM; i++)
	{
		if (Test_Msg.Id == CanRxMsgBuff_TEST_A[i].ExtId)
		{
			u8Rst++;
			CanRxMsgBuff_TEST_A[i].ExtId = 0;
			break;
		}
	}

	if (u8Rst >= 2)
	{
		sialmDly = 0;
		gEvChargeInfo.u8EV_InterDI_COM &= ~(0x01<<6);//CAN 测试成功
	}
	else
	{
		sialmDly++;
		if (sialmDly > 100)
		{
			sialmDly = 0;
			gEvChargeInfo.u8EV_InterDI_COM |= (0x01<<6);//CAN 测试失败
		}
	}
}


void UART_Test_Deal(void)
{
	UINT8 Rs485Rst = FALSE;
	UINT8 Rs232Rst = FALSE;
	int i = 0;
	UINT8 TestData[2];
	if (FALSE == gEvChargeInfo.u8TEST_MODE_FLAG)
	{
		return;
	}
	dv_Uart5.fInitUart5(m9600, UARTX_PARITY_EVEN);				//InitUart5
	dv_Uart5.fEnableRx();										//使能收中断
	dv_Uart4.fInitUart(m9600, UARTX_PARITY_EVEN);				//InitUart4
	dv_Uart4.fEnableRx();										//使能收中断
	OSTimeDly(2);
	TestData[0] = 0x5a;
	TestData[1] = 0xa5;
	dv_Uart5.fSendDataTXY(TestData, 2);
	OSTimeDly(2);
	for (i = 0; i < dv_Uart4.pData; i++)
	{
		if (gUart4RecvBuff[i] = TestData[0] || gUart4RecvBuff[i] == TestData[1])
		{
			Rs485Rst = TRUE;
			break;
		}
	}

	if (Rs485Rst == TRUE)
	{
		gEvChargeInfo.u8EV_InterDI_COM &= ~(0x01 << 7);		//OK
	}
	else
	{
		gEvChargeInfo.u8EV_InterDI_COM |= (0x01 << 7);			//超时！
	}


	dv_Uart1.fSendDataTXY(TestData, 2);
	for (i = 0; i < dv_Uart1.pData; i++)
	{
		if (TestData[0] == gUART1RecvData[i] || TestData[1] == gUART1RecvData[i])
		{
			Rs232Rst = TRUE;
			break;
		}
	}

	if (Rs232Rst)
	{
		gEvChargeInfo.u8EV_InterDI_COM &= ~(0x01 << 4);
	}
	else
	{
		gEvChargeInfo.u8EV_InterDI_COM |= (0x01 << 4);
	}
}

//UART6		J20
void Test_API(void)
{
	static UINT8 sIsFirst = TRUE;
	if (sIsFirst == TRUE)
	{
		sIsFirst = FALSE;
		dv_Uart6.Head = 0;
		dv_Uart6.uRecvFinish = YDN23_NONE;

		gEvChargeInfo.u8TEST_MODE_FLAG = FALSE;
		gEvChargeInfo.u8CTRLEx2DOCMD = 0;
		gEvChargeInfo.u8CTRLEx2DOCMDFlag = FALSE;
		gEvChargeInfo.u8CTRLInterDOCMDFlag_2 = FALSE;
		gEvChargeInfo.u8CTRLEx1DOCMDFlag = FALSE;
		gEvChargeInfo.u8CTRLEx1DOCMD = 0;
		gEvChargeInfo.u8CTRLInterDOCMD = 0;
		gEvChargeInfo.u8CTRLInterDOCMDFlag = FALSE;

		gEvChargeInfo.u8EV_InterDI_COM |= (0x01<<6);//CAN 测试失败
		gEvChargeInfo.u8EV_InterDI_COM |= (0x01 << 7);//485 超时！
		gEvChargeInfo.u8EV_InterDI_COM |= (0x01 << 4);//232	 失败

	}

	if (uYDN23CheckReceive())
	{
		gEvChargeInfo.u8RcvYdn23Flag = TRUE;
		OSTimeDly(80);
		YDN23AnalayseUart6(YDN23_IMCU_ADDR);
		OSTimeDly(80);
	}
}