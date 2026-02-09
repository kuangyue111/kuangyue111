
//PuTianMngDef gs_PuTianMng[2] = {0};

#define TRY_XXX_AGAIN_TIMES 2000

#define PT_EV_ADDR 0xE5
#define PT_CSD 0x1D
#define PT_BSD 0x1C
#define PT_CST 0x1a
#define PT_BSM 0x12
#define PT_BST 0x19
#define PT_BCL 0x10
#define PT_CCS 0x14
#define PT_BCS 0x11
#define PT_BCL 0x10
#define PT_CRM 0x01
#define PT_CR0 0x0A
#define PT_BR0 0x09
#define PT_CML 0x08
#define PT_CTL 0x07
#define PT_VBI 0x30
#define PT_BRM 0x02
#define PT_BCP 0x06
#define PT_BMV 0x15
#define PT_BEM 0x1E
#define PT_CEM 0x1F
#define PT_BFM 0x33
#define PT_BRM_VAL 0x02		//41字节
#define PT_VBI_VAL 0x03		//40字节
#define PT_BroadCast_VAL 0x31
#define	PF_TP_CM_RTS		0xEC
#define	PF_TP_CM_RTS_VALUE	16
#define	PF_TP_CM_CTS		17
#define	PF_TP_CM_EndofMsgAck	19
#define	PF_TP_CM_Abort		255

#define BMS_ADDR 0xf4
#define XXX_EC 0xEC
#define XXX_EB 0xEB


#define EV_XXX_BUF_NUM	800
NOINIT UINT8 EVBCLXXXbuf_2[EV_XXX_BUF_NUM];


void PT_StuffCSTData_B(UINT8 u8Case);

void InitPuTianParas_B(void)
{
	gs_PuTianMng[GUN_NO_B].u16SetMiltDataInterval = 75;
	gs_PuTianMng[GUN_NO_B].u16SetBCLTimeOutVal = 100;			//1S
};

void PuTianTimerDeal_B(void)
{
	if (gEvChargeInfo.u8Is_PUTIAN_V15[GUN_NO_B] != IS_PUTIAN_BMS)
	{
		return;
	}

	if(gs_PuTianMng[GUN_NO_B].u32CAN2_CRM00_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_CRM00_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_CRM00_TimeOUT = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BCP_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCP_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCP_TimeOUT = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_TimeOUT = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CML_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CML_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CML_Count = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CTS_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CTS_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CTS_Count = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_AA_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_AA_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_AA_TimeOUT = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_BCS_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_BCS_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_BCS_TimeOUT = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BCS_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCS_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BCS_TimeOUT = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BST_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BST_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BST_TimeOUT = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT >= 10)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT = 0;
	}


	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CSD_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CSD_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CSD_Count = 0;
	}

	if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CCS_Count >= 10)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CCS_Count -= 10;
	}
	else
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CCS_Count = 0;
	}
}


void CleanBCLXXXbuf_B(void)
{
	UINT16 u16Idx = 0;
	//NOINIT UINT8 EVBCLXXXbuf_2[EV_XXX_BUF_NUM];
	for (u16Idx = 0; u16Idx < EV_XXX_BUF_NUM/3; u16Idx++)
	{
		EVBCLXXXbuf_2[u16Idx] = 0;
	}
}

UINT8 PT_Type_Check_B(void)
{
	UINT16  i16TimeOUT = 0;
	int iRStartPst = -1;
	//__B_BMS_24V();
	__BMS_2_POWER_ON();
	for (i16TimeOUT = 0; i16TimeOUT < 500; i16TimeOUT++)
	{
		iRStartPst = Read_B_AllBuf(PT_BroadCast_VAL);					//收到普天广播协议！！
		if (iRStartPst >= 0)
		{
			return IS_PUTIAN_BMS;
		}
		OSTimeDly(3);
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
	}

	return ISNT_PUTIAN_BMS;
}

void PT_Send_TP_CM_Proc_B(TP_CM_RTS_INFO srVal, int TP_CM_Type)
{
	canmsg TxBufferCAN2;
	UINT8 byEVbuf[8];
	TxBufferCAN2.Id = ((UINT32)7<<26) + ((UINT32)XXX_EC << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	if (PF_TP_CM_CTS == TP_CM_Type)
	{
		byEVbuf[0] = TP_CM_Type;//17
		byEVbuf[1] = srVal.TotalPkgNum;					// 可发送数据包数
		byEVbuf[2] = 0x01;						// 下一个要发送的数据包编号
		byEVbuf[3] = 0xff;
		byEVbuf[4] = 0xff;
	}
	else if (PF_TP_CM_EndofMsgAck == TP_CM_Type )
	{
		byEVbuf[0] = TP_CM_Type;//19
		byEVbuf[1] = (srVal.TotalMsgByteNum) & 0xff;			//(srVal.TotalMsgByteNum >> 8) & 0xff;			//整个消息大小的字节数	bySendBuf[7] = (srVal.TotalMsgByteNum >> 8) & 0xff;//(srVal.TotalMsgByteNum) & 0xff;				//整个消息大小的字节数
		byEVbuf[2] = (srVal.TotalMsgByteNum >> 8) & 0xff;		//(srVal.TotalMsgByteNum) & 0xff;				//整个消息大小的字节数
		byEVbuf[3] = srVal.TotalPkgNum;					//全部数据包的数 	
		byEVbuf[4] = 0xff;
	}
	else //PF_TP_CM_Abort
	{
		byEVbuf[0] = PF_TP_CM_Abort;
		byEVbuf[1] = 0xff;
		byEVbuf[2] = 0xff;
		byEVbuf[3] = 0xff;
		byEVbuf[4] = 0xff;
	}
	byEVbuf[5] = srVal.PkgMsgNoLSB;						//打包参数组编号	LSB
	byEVbuf[6] = srVal.PkgMsgNoMID;
	byEVbuf[7] = srVal.PkgMsgNoMSB;
	TxBufferCAN2.Data[0] = ((UINT32)byEVbuf[0] << 24) + ((UINT32)byEVbuf[1] << 16) +  ((UINT32)byEVbuf[2] << 8) + byEVbuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)byEVbuf[4] << 24) + ((UINT32)byEVbuf[5] << 16) +  ((UINT32)byEVbuf[6] << 8) + byEVbuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 8);
}


static void  HandShake_PT_CRM00_B(void)
{
	gEVBms_2_Info.byEVCRM00AACRM = 0x00;
	UINT8 CRM00buf[8];
	canmsg TxBufferCAN2;
	TxBufferCAN2.Id = (((UINT32)6)<<26) + ((UINT32)PT_CRM << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	gEVBms_2_Info.byEVCRM00AACRM = 0x00;
	CRM00buf[0] = 0x00;										//编号
	CRM00buf[1] = 0x00;										//插头编号
	CRM00buf[2] = 0x00;										//充电机位置
	CRM00buf[3] = gEVBms_2_Info.byEVCRM00AACRM;				//充电机辨识结果  00 不能辨识， 01 BMS辨识
	CRM00buf[4] = 0xff;
	CRM00buf[5] = 0xff;
	CRM00buf[6] = 0xff;
	CRM00buf[7] = 0xff;
	TxBufferCAN2.Data[0] = ((UINT32)CRM00buf[0] << 24) + ((UINT32)CRM00buf[1] << 16) +  ((UINT32)CRM00buf[2] << 8) + CRM00buf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CRM00buf[4] << 24) + ((UINT32)CRM00buf[5] << 16) +  ((UINT32)CRM00buf[6] << 8) + CRM00buf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 8);
}

int PT_SendCMR00_Rcv_BRM_B(void)
{
	UINT16 byBRMdataIDX = 0;
	UINT16 u16PkgNum = 0;
	UINT16 byTriAgain = 0;
	UINT16 iTotalRcvBRMByteNum = 0;
	UINT8 u8Idx, byTP_CM_Flag_1 = FALSE;
	UINT16 i16TimeOUT = 0;
	int iRStartPst = -1;
	TP_CM_RTS_INFO	stTP_CM_RTSVal_1, stTP_CM_RTSVal_1Back;
	gs_PuTianMng[GUN_NO_B].u32CAN2_CRM00_TimeOUT = 5000;
	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count = 0;
	//RectOUTCtrl(NO_ADJUST_ON_OFF, ADJUST_RECT_VOLT_CURR);
	B_Spear_RectOnOffCtrl(RECT_OFF);
	/*********************************************************************************************************************
		普天协议， 
		BRM	000200
		VBI	003000
		广播	003100
		CRM 00 
		VBI 
	**********************************************************************************************************************/
	for (i16TimeOUT = 1;i16TimeOUT < 1666;i16TimeOUT++)//1666 *3ms = 5s
	{
		//BSP_CanCheck(0);
		
		//__B_BMS_24V();
		__BMS_2_POWER_ON();

		if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count <= 0)
		{
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count = 247;//250ms
			HandShake_PT_CRM00_B();
		}
		Read_B_AllBuf(PT_BroadCast_VAL);

		OSTimeDly(3);
		gSelfTest.fSetTaskID(TASK_ID_CAN2);

		if (gs_PuTianMng[GUN_NO_B].u32CAN2_CRM00_TimeOUT <= 0)
		{
			B_Stuff_CEM(HANDSHAKE_START_CRM00);
			gB_Result.u8LastReason = CEM_CRM00_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BRM);
			return EV_CEM_XXXX;
		}
		/***********************************************普天**********************************************************************/
		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(XXX_EC);
		if (iRStartPst >= 0)
		{
			stTP_CM_RTSVal_1.CTLByte = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			stTP_CM_RTSVal_1.TotalMsgByteNum = (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
			stTP_CM_RTSVal_1.TotalPkgNum = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
			stTP_CM_RTSVal_1.PkgMsgNo = (UINT32)gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5]
							+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6])<<8)&0x00ff00)
							+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7])<<16)&0xff000);
			stTP_CM_RTSVal_1.PkgMsgNoLSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
			stTP_CM_RTSVal_1.PkgMsgNoMID = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
			stTP_CM_RTSVal_1.PkgMsgNoMSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];

			if (PF_TP_CM_RTS_VALUE == stTP_CM_RTSVal_1.CTLByte 
						&& (PT_VBI == stTP_CM_RTSVal_1.PkgMsgNoMID || PT_BRM == stTP_CM_RTSVal_1.PkgMsgNoMID)
						&& (0 == stTP_CM_RTSVal_1.PkgMsgNoLSB)
						&& (0 == stTP_CM_RTSVal_1.PkgMsgNoMSB))
			{
				Clean_B_EB_Buf();
				stTP_CM_RTSVal_1Back = stTP_CM_RTSVal_1;
				byTP_CM_Flag_1 = TRUE;
				PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1, PF_TP_CM_CTS);
				OSTimeDly(2);
			}
		}

		if (TRUE == byTP_CM_Flag_1)
		{
			byTP_CM_Flag_1 = FALSE;
			byBRMdataIDX = 0;
			iTotalRcvBRMByteNum = 0;
			u16PkgNum = 0;
			gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT = 1250;
			gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = 2000;
			/***********************************************普天**********************************************************************/
			for (byTriAgain = 0; byTriAgain < TRY_XXX_AGAIN_TIMES; byTriAgain++)
			{
				//__B_BMS_24V();
				__BMS_2_POWER_ON();

				iRStartPst = -1;
				iRStartPst = Read_B_AllBuf(XXX_EB);
				if (iRStartPst >= 0)
				{
					byTriAgain = 0;
					byBRMdataIDX = 7 * (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0] - 1);			//pbyRcvBuf[5] sequenceNo
					byBRMdataIDX = MIN(byBRMdataIDX, EV_XXX_BUF_NUM - 7);
					gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = MAX((gs_PuTianMng[GUN_NO_B].u16SetMiltDataInterval*10),750);
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];
					UNUSED(EVBCLXXXbuf_2[0]);
					iTotalRcvBRMByteNum += 7;
					u16PkgNum++;
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT <= 0)					//下一包发送慢了！
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);
					OSTimeDly(2);
					break;
				}

				Read_B_AllBuf(PT_BroadCast_VAL);
				
				if ((gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT <= 0) && 0 == iTotalRcvBRMByteNum)	//T2 断开连接
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);
					OSTimeDly(2);
					break;
				}
				/***********************************************普天**********************************************************************/
				if (gs_PuTianMng[GUN_NO_B].u32CAN2_CRM00_TimeOUT <= 0)
				{
					B_Stuff_CEM(HANDSHAKE_START_CRM00);
					gB_Result.u8LastReason = CEM_CRM00_TIMEOUT;
					RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BRM);
					return EV_CEM_XXXX;
				}

				if (iTotalRcvBRMByteNum >= stTP_CM_RTSVal_1Back.TotalMsgByteNum || u16PkgNum >= stTP_CM_RTSVal_1Back.TotalPkgNum)
				{
					Read_B_AllBuf(0xff);
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_EndofMsgAck);
					OSTimeDly(2);
					byTP_CM_Flag_1 = FALSE;
					stTP_CM_RTSVal_1Back.PkgMsgNoMID = 0xff;
					gEVBms_2_Info.byEVCRM00AACRM = 0x01;						//可以辨识！！
					gEVBms_2_Info.u8BMSVersion = BMS_VERSION_2011;
					if (stTP_CM_RTSVal_1.PkgMsgNoMID == PT_VBI)
					{
						return CONFIG_RCV_BCP_OR_S_CRMAA; // HANDSHAKE_CRM_AA;
					}
					else if (stTP_CM_RTSVal_1.PkgMsgNoMID == PT_BRM)
					{
						gEVBms_2_Info.u8BRM_Version[0] = EVBCLXXXbuf_2[0];
						gEVBms_2_Info.u8BRM_Version[1] = EVBCLXXXbuf_2[1];
						gEVBms_2_Info.u8BRM_Version[2] = EVBCLXXXbuf_2[2];
						gEVBms_2_Info.u8BRM_BattType = EVBCLXXXbuf_2[3];
						gEVBms_2_Info.u8BRM_RatedCAP = (float)((UINT16)EVBCLXXXbuf_2[4] + ((UINT16)EVBCLXXXbuf_2[5]<<8))*0.1;
						gEVBms_2_Info.u8BRM_RatedVOLT = (float)((UINT16)EVBCLXXXbuf_2[6] + ((UINT16)EVBCLXXXbuf_2[7]<<8))*0.1;
						gEVBms_2_Info.u8BRM_FactoryName[0] = EVBCLXXXbuf_2[8];
						gEVBms_2_Info.u8BRM_FactoryName[1] = EVBCLXXXbuf_2[9];
						gEVBms_2_Info.u8BRM_FactoryName[2] = EVBCLXXXbuf_2[10];
						gEVBms_2_Info.u8BRM_FactoryName[3] = EVBCLXXXbuf_2[11];
						gEVBms_2_Info.u8BRM_BATTGNo[0] = EVBCLXXXbuf_2[12];
						gEVBms_2_Info.u8BRM_BATTGNo[1] = EVBCLXXXbuf_2[13];
						gEVBms_2_Info.u8BRM_BATTGNo[2] = EVBCLXXXbuf_2[14];
						gEVBms_2_Info.u8BRM_BATTGNo[3] = EVBCLXXXbuf_2[15];
						gEVBms_2_Info.u8BRM_YearMonDay[0] = EVBCLXXXbuf_2[16];
						gEVBms_2_Info.u8BRM_YearMonDay[1] = EVBCLXXXbuf_2[17];
						gEVBms_2_Info.u8BRM_YearMonDay[2] = EVBCLXXXbuf_2[18];
						gEVBms_2_Info.u32BRM_TotalTimes = EVBCLXXXbuf_2[19] + (EVBCLXXXbuf_2[20] << 8) + (EVBCLXXXbuf_2[21] << 16);
						gEVBms_2_Info.u8BRM_Reserve = EVBCLXXXbuf_2[23];
						for (u8Idx = 0; u8Idx < 17;u8Idx++)
						{
							gEVBms_2_Info.u8BRM_VIN[u8Idx] = EVBCLXXXbuf_2[u8Idx + 24];
						}

					}
					break;
				}

				gSelfTest.fSetTaskID(TASK_ID_CAN2);
				if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count <= 0)
				{
					gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRM00_Count = 247;//250ms
					HandShake_PT_CRM00_B();
				}
				OSTimeDly(2);
			}//end for RECEIVE 0XEB!!
		}//if (TRUE == byTP_CM_Flag_1)
	}
	B_Stuff_CEM(HANDSHAKE_START_CRM00);
	gB_Result.u8LastReason = CEM_CRM00_TIMEOUT;
	RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BRM);
	return EV_CEM_XXXX;
}

static void PT_SendCRM01_B(void)
{
	UINT8 CRMAAbuf[8];
	canmsg TxBufferCAN2;
	TxBufferCAN2.Id = (((UINT32)6)<<26) + ((UINT32)PT_CRM << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	gEVBms_2_Info.byEVCRM00AACRM = 0x01;
	CRMAAbuf[0] = 0x00;
	CRMAAbuf[1] = 0x00;
	CRMAAbuf[2] = 0x00;
	CRMAAbuf[3] = gEVBms_2_Info.byEVCRM00AACRM;
	CRMAAbuf[4] = 0xff;
	CRMAAbuf[5] = 0xff;
	CRMAAbuf[6] = 0xff;
	CRMAAbuf[7] = 0xff;
	TxBufferCAN2.Data[0] = ((UINT32)CRMAAbuf[0] << 24) + ((UINT32)CRMAAbuf[1] << 16) +  ((UINT32)CRMAAbuf[2] << 8) + CRMAAbuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CRMAAbuf[4] << 24) + ((UINT32)CRMAAbuf[5] << 16) +  ((UINT32)CRMAAbuf[6] << 8) + CRMAAbuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 8);
}


int PT_Config_RcvBCP_B(void)
{
	UINT16 iBCPTimeOUT= 0;
	int iRStartPst = -1;
	UINT16 byBRMdataIDX = 0;
	UINT16 byTriAgain = 0;
	UINT16 iTotalRcvBRMByteNum = 0;
	UINT8 u8Idx, byTP_CM_Flag_1 = FALSE;
	TP_CM_RTS_INFO	stTP_CM_RTSVal_1,stTP_CM_RTSVal_1Back;

#define TRY_BCP_AGAIN_TIMES	2000
	gEVBms_2_Info.byEVCRM00AACRM = 0x01;
	//gEvChargeInfo.uRunPhase = 1;

	gEVBms_2_Info.fBCP_CurrBattVolt = 0;

	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count = 250;
	gs_PuTianMng[GUN_NO_B].u32CAN2_BCP_TimeOUT = 5000;
	/*********************************************************************************************************************
		普天协议， 
		BRM	000200
		VBI	003000
		广播	003100
		CRM 01
		BCP
	**********************************************************************************************************************/
	for (iBCPTimeOUT = 1; iBCPTimeOUT < 3000; iBCPTimeOUT++)		//1666*3ms = 5S
	{
		//BSP_CanCheck(1);
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
		if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count <= 0)
		{
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count = 250;
			PT_SendCRM01_B();
		}
		OSTimeDly(2);
		Read_B_AllBuf(PT_BroadCast_VAL);
		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BCP_TimeOUT <= 0)
		{
			B_Stuff_CEM(CONFIG_RCV_BCP_OR_S_CRMAA);
			gB_Result.u8LastReason = CEM_RCV_BCP_OR_S_CRMAA_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCP);
			return EV_CEM_XXXX;
		}

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(XXX_EC);
		if (iRStartPst >= 0)
		{
			stTP_CM_RTSVal_1.CTLByte = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			stTP_CM_RTSVal_1.TotalMsgByteNum = (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
			stTP_CM_RTSVal_1.TotalPkgNum = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
			stTP_CM_RTSVal_1.PkgMsgNo = (UINT32)gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5]
						+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6])<<8)&0x00ff00)
						+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7])<<16)&0xff000);
			stTP_CM_RTSVal_1.PkgMsgNoLSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
			stTP_CM_RTSVal_1.PkgMsgNoMID = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
			stTP_CM_RTSVal_1.PkgMsgNoMSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];

			if ((PF_TP_CM_RTS_VALUE == stTP_CM_RTSVal_1.CTLByte) 
						&& (PT_BCP == stTP_CM_RTSVal_1.PkgMsgNoMID || PT_BRM == stTP_CM_RTSVal_1.PkgMsgNoMID)
						&& (stTP_CM_RTSVal_1.TotalMsgByteNum <= EV_XXX_BUF_NUM - 7)
						&& (0x00 == stTP_CM_RTSVal_1.PkgMsgNoLSB)
						&& (0x00 == stTP_CM_RTSVal_1.PkgMsgNoMSB))
			{
				stTP_CM_RTSVal_1Back = stTP_CM_RTSVal_1;
				byTP_CM_Flag_1 = TRUE;
				Read_B_AllBuf(0xff);
				CleanBCLXXXbuf_B();
				OSTimeDly(2);
				PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1, PF_TP_CM_CTS);
				OSTimeDly(2);
			}
		}
		/****************************************************普天*****************************************************************/
		if (TRUE == byTP_CM_Flag_1)
		{
			byTP_CM_Flag_1 = FALSE;
			byBRMdataIDX = 0;
			iTotalRcvBRMByteNum = 0;
			gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT = 1250;
			gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = 2000;
			for (byTriAgain = 0; byTriAgain < TRY_BCP_AGAIN_TIMES; byTriAgain++)					//间隔4MS
			{
				iRStartPst = -1;
				iRStartPst = Read_B_AllBuf(XXX_EB);
				if (iRStartPst >= 0)
				{
					byTriAgain = 0;
					byBRMdataIDX = 7 * (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0] - 1);			//pbyRcvBuf[5] sequenceNo
					byBRMdataIDX = MIN(byBRMdataIDX, EV_XXX_BUF_NUM - 10);

					gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = MAX((gs_PuTianMng[GUN_NO_B].u16SetMiltDataInterval*10),750);

					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];
					UNUSED(EVBCLXXXbuf_2[0]);
					iTotalRcvBRMByteNum += 7;
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT <= 0 && 0 == iTotalRcvBRMByteNum)
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);				//TIME OUT
					OSTimeDly(2);
					break;
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT <= 0)
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);				//TIME OUT
					OSTimeDly(2);
					break;
				}
				/****************************************************普天*****************************************************************/
				if (iTotalRcvBRMByteNum >= stTP_CM_RTSVal_1Back.TotalMsgByteNum)
				{
					byTP_CM_Flag_1 = FALSE;
					Clean_B_EB_Buf();
					OSTimeDly(2);
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_EndofMsgAck);
					OSTimeDly(2);
					if (stTP_CM_RTSVal_1Back.PkgMsgNoMID == PT_BCP)
					{
						/*********************？？？？？？******************/
						gEVBms_2_Info.fBCP_CellMaxVolt =  (((UINT16)EVBCLXXXbuf_2[1] << 8) + EVBCLXXXbuf_2[0]) * 0.01;
						gEVBms_2_Info.fBCP_MaxAllowCurr = ABS((((EVBCLXXXbuf_2[3] << 8) + EVBCLXXXbuf_2[2]) * 0.1) - 800.00);
						gEVBms_2_Info.fBCP_RatedPower = EVBCLXXXbuf_2[4];													//最大允许充电容量 0XDC==220AH
						gEVBms_2_Info.fBCP_MaxChargeVolt = (((UINT16)EVBCLXXXbuf_2[6] << 8) + EVBCLXXXbuf_2[5]) * 0.1;		//最高允许充电总电压 A0 0F == 400V
						gEVBms_2_Info.fBCP_MaxAllowTemp = EVBCLXXXbuf_2[7] - 50;											//2820最高允许温度
						//2821 整车蓄电池估计剩余容量  [9] [8]
						gEVBms_2_Info.fBCP_CurrBattVolt = (((UINT16)EVBCLXXXbuf_2[11] << 8) + EVBCLXXXbuf_2[10]) * 0.1;		//2822  整车蓄电池总电压 
						gEVBms_2_Info.fBCS_MeasureVolt = gEVBms_2_Info.fBCP_CurrBattVolt;
						//if (TRUE == gEVBms_2_Info.u8JustSTART)
						//{
						//	gEVBms_2_Info.fBCP_CurrSOC = EVBCLXXXbuf_2[12];													//整车SOH	电池健康状态！
						//	gEVBms_2_Info.u8JustSTART = FALSE;
						//}
						return CONFIG_SEND_CML_RCV_BRO;
					}
					else if (stTP_CM_RTSVal_1Back.PkgMsgNoMID == PT_BRM)
					{
						gEVBms_2_Info.u8BRM_Version[0] = EVBCLXXXbuf_2[0];
						gEVBms_2_Info.u8BRM_Version[1] = EVBCLXXXbuf_2[1];
						gEVBms_2_Info.u8BRM_Version[2] = EVBCLXXXbuf_2[2];
						gEVBms_2_Info.u8BRM_BattType = EVBCLXXXbuf_2[3];
						gEVBms_2_Info.u8BRM_RatedCAP = (float)((UINT16)EVBCLXXXbuf_2[4] + ((UINT16)EVBCLXXXbuf_2[5]<<8))*0.1;
						gEVBms_2_Info.u8BRM_RatedVOLT = (float)((UINT16)EVBCLXXXbuf_2[6] + ((UINT16)EVBCLXXXbuf_2[7]<<8))*0.1;
						gEVBms_2_Info.u8BRM_FactoryName[0] = EVBCLXXXbuf_2[8];
						gEVBms_2_Info.u8BRM_FactoryName[1] = EVBCLXXXbuf_2[9];
						gEVBms_2_Info.u8BRM_FactoryName[2] = EVBCLXXXbuf_2[10];
						gEVBms_2_Info.u8BRM_FactoryName[3] = EVBCLXXXbuf_2[11];
						gEVBms_2_Info.u8BRM_BATTGNo[0] = EVBCLXXXbuf_2[12];
						gEVBms_2_Info.u8BRM_BATTGNo[1] = EVBCLXXXbuf_2[13];
						gEVBms_2_Info.u8BRM_BATTGNo[2] = EVBCLXXXbuf_2[14];
						gEVBms_2_Info.u8BRM_BATTGNo[3] = EVBCLXXXbuf_2[15];
						gEVBms_2_Info.u8BRM_YearMonDay[0] = EVBCLXXXbuf_2[16];
						gEVBms_2_Info.u8BRM_YearMonDay[1] = EVBCLXXXbuf_2[17];
						gEVBms_2_Info.u8BRM_YearMonDay[2] = EVBCLXXXbuf_2[18];
						gEVBms_2_Info.u32BRM_TotalTimes = EVBCLXXXbuf_2[19] + (EVBCLXXXbuf_2[20] << 8) + (EVBCLXXXbuf_2[21] << 16);
						gEVBms_2_Info.u8BRM_Reserve = EVBCLXXXbuf_2[23];
						for (u8Idx = 0; u8Idx < 17;u8Idx++)
						{
							gEVBms_2_Info.u8BRM_VIN[u8Idx] = EVBCLXXXbuf_2[u8Idx + 24];
						}

					}
					break;
				}

				if (gs_PuTianMng[GUN_NO_B].u32CAN2_BCP_TimeOUT <= 0)
				{
					B_Stuff_CEM(CONFIG_RCV_BCP_OR_S_CRMAA);
					gB_Result.u8LastReason = CEM_RCV_BCP_OR_S_CRMAA_TIMEOUT;
					RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCP);
					return EV_CEM_XXXX;
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count <= 0)
				{
					gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRMAA_Count = 250;
					PT_SendCRM01_B();
				}
				OSTimeDly(2);
				gSelfTest.fSetTaskID(TASK_ID_CAN2);
			}//end for
		}//if (TRUE == byTP_CM_Flag_1)
	}//BCP time OUT!!
	B_Stuff_CEM(CONFIG_RCV_BCP_OR_S_CRMAA);
	gB_Result.u8LastReason = CEM_RCV_BCP_OR_S_CRMAA_TIMEOUT;
	RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCP);
	return EV_CEM_XXXX;
}

void PT_SendCML_B(void)		//DLC 6
{
	UINT8 CMLbuf[8];
	float MaxOutVolt = 750;
	float MinOutVolt = 100;
	float MaxOutCurr = 240;
	UINT16 iTempValue = 0;
	iTempValue = (UINT16)((((-1) * MaxOutCurr) + 800) * 10);				//MAX CURR
	canmsg TxBufferCAN2;
	TxBufferCAN2.Id = (((UINT32)6)<<26) + ((UINT32)PT_CML<< 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	CMLbuf[0] = ((UINT16)(MaxOutVolt * 10)&0xff);						//先传低字节
	CMLbuf[1] = ((UINT16)(MaxOutVolt * 10) >> 8);
	CMLbuf[2] = ((UINT16)(MinOutVolt * 10)&0xff);						//先传低字节
	CMLbuf[3] = ((UINT16)(MinOutVolt * 10) >> 8);
	CMLbuf[4] = iTempValue & 0xff;
	CMLbuf[5] = 0xff&(iTempValue >>8);
	CMLbuf[6] = 0;										//不具备放电能力
	CMLbuf[7] = 0;										//不具备放电能力
	TxBufferCAN2.Data[0] = ((UINT32)CMLbuf[0] << 24) + ((UINT32)CMLbuf[1] << 16) +  ((UINT32)CMLbuf[2] << 8) + CMLbuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CMLbuf[4] << 24) + ((UINT32)CMLbuf[5] << 16) +  ((UINT32)CMLbuf[6] << 8) + CMLbuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 8);
}

void PT_SendCTS_B(void)		//DLC  7   时间
{
	struct tm  tmNow;
	gRTCData.fGetLocalTime(&tmNow);
	UINT16 u16Year = tmNow.tm_year;
	UINT8 CTSbuf[8];
	canmsg TxBufferCAN2;
	TxBufferCAN2.Id = (((UINT32)6)<<26) + ((UINT32)PT_CTL << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	CTSbuf[0] = HexToBCD(tmNow.tm_sec);
	CTSbuf[1] = HexToBCD(tmNow.tm_min);
	CTSbuf[2] = HexToBCD(tmNow.tm_hour);
	CTSbuf[3] = HexToBCD(tmNow.tm_mday);
	CTSbuf[4] = HexToBCD(tmNow.tm_mon);
	CTSbuf[5] = HexToBCD((u16Year&0xff));
	CTSbuf[6] = HexToBCD((u16Year >> 8)&0xff);
	CTSbuf[7] = 0xff;
	TxBufferCAN2.Data[0] = ((UINT32)CTSbuf[0] << 24) + ((UINT32)CTSbuf[1] << 16) +  ((UINT32)CTSbuf[2] << 8) + CTSbuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CTSbuf[4] << 24) + ((UINT32)CTSbuf[5] << 16) +  ((UINT32)CTSbuf[6] << 8);
	BSP_Can_2_Tx(&TxBufferCAN2, 7);
}

int PT_Config_SendCML_RcvBRO_B(void)
{
	int iRStartPst = -1;
	UINT32 i32BROTimeOUT = 60000;
	gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_TimeOUT = 10000;
	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CML_Count = 0;
	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CTS_Count = 0;
	gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_AA_TimeOUT  = 60000;				//60S
	/*********************************************************************************************************************
			普天协议， 
			BRM	000200
			VBI	003000
			广播	003100
			CML CTL  
			BRO
	**********************************************************************************************************************/
	for (i32BROTimeOUT = 0;i32BROTimeOUT < 4000; i32BROTimeOUT++)				//2ms*1000 * 60 = 5min 等待
	{
		//BSP_CanCheck(1);

		if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CML_Count <= 0)
		{
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CML_Count = 250;
			PT_SendCML_B();
			OSTimeDly(2);
		}

		if(gs_PuTianMng[GUN_NO_B].i16CAN2Send_CTS_Count <= 0)
		{
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CTS_Count = 500;
			PT_SendCTS_B();
			OSTimeDly(2);
		}

		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BRO_TimeOUT <= 0)
		{
			B_Stuff_CEM(CONFIG_SEND_CML_RCV_BRO);
			gB_Result.u8LastReason = CEM_SEND_CML_RCV_BRO_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BRO_00);
			return EV_CEM_XXXX;
		}

		if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
		{
			B_Spear_RectOnOffCtrl(RECT_OFF);
			//CtrlRectONOFF(RECT_OFF_CMD,TRUE);
			//gEvChargeInfo.uRunPhase = 0;
			//gEvChargeInfo.uRunEvIndicate = 0;					//未充电
			return EV_TIME_OUT;
		}
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
		iRStartPst = -1;
		/***************************************************普天******************************************************************/
		iRStartPst = Read_B_AllBuf(PT_BR0);
		if (iRStartPst >= 0)
		{
			if (0x04 == gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0])		//BMS准备好！
			{
				return CONFIG_SEND_CRO;
			}
			gEVBms_2_Info.u8BRO_RedayOK = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
		}
		OSTimeDly(2);
	}//END Time Out
	
	B_Stuff_CEM(CONFIG_SEND_CML_RCV_BRO);
	gB_Result.u8LastReason = CEM_SEND_CML_RCV_BRO_TIMEOUT;
	RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BRO_00);
	return EV_CEM_XXXX;
}

int PT_SendCRO04_B(void)
{
	canmsg TxBufferCAN2;
	UINT8 CRObuf[8];

	gEVBms_2_Info.byEVCRO = 0x04;
	TxBufferCAN2.Id = (((UINT32)5)<<26) + ((UINT32)PT_CR0<< 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	CRObuf[0] = gEVBms_2_Info.byEVCRO;
	TxBufferCAN2.Data[0] = ((UINT32)CRObuf[0] << 24) + ((UINT32)CRObuf[1] << 16) +  ((UINT32)CRObuf[2] << 8) + CRObuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CRObuf[4] << 24) + ((UINT32)CRObuf[5] << 16) +  ((UINT32)CRObuf[6] << 8) + CRObuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 1);

	return CHARGING_RCV_BCS_OR_S_CRO;
}

void PT_SendCRO01_B(void)
{
	/*********************************************************************************************************************
		普天协议， 
		CRO 04 
	**********************************************************************************************************************/
	canmsg TxBufferCAN2;
	UINT8 CRObuf[8];
	gEVBms_2_Info.byEVCRO = 0x04;
	TxBufferCAN2.Id = (((UINT32)5)<<26) + ((UINT32)PT_CR0<< 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	CRObuf[0] = gEVBms_2_Info.byEVCRO;
	TxBufferCAN2.Data[0] = ((UINT32)CRObuf[0] << 24) + ((UINT32)CRObuf[1] << 16) +  ((UINT32)CRObuf[2] << 8) + CRObuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CRObuf[4] << 24) + ((UINT32)CRObuf[5] << 16) +  ((UINT32)CRObuf[6] << 8) + CRObuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 1);
}

#define BATT_VOLT_D_VALUE	5
int PT_Charge_RcvBCS_BCL_B(void)
{
	int iHaveBCSFlag = FALSE;
	int iHaveBCLFlag = FALSE;
	UINT16 i16BCSBCLTimeOUT = 5000;
	UINT16 byBRMdataIDX = 0;
	UINT16 byTriAgain = 0;
	UINT16 i16PkgNum = 0;
	UINT16 iTotalRcvBRMByteNum = 0;
	UINT8 byTP_CM_Flag_1 = FALSE;
	int iRStartPst = -1;
	TP_CM_RTS_INFO	stTP_CM_RTSVal_1,stTP_CM_RTSVal_1Back;
	//断开泄放电阻，马上快要输出了
	//__RESIST_OFF();__RESIST_OFF();__RESIST_OFF();
#define TRY_BCS_BCL_AGAIN_TIMES 2000
	/*********************************************************************************************************************
		普天协议， 
		BCL	BCS 
		广播	003100
		CRO 01
	**********************************************************************************************************************/
	gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_BCS_TimeOUT = 5000;
	gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT = 5000;
	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count = 0;
	gEVBms_2_Info.fBCS_MaxCellVolt = 0;
	for (i16BCSBCLTimeOUT = 0; i16BCSBCLTimeOUT < 20000; i16BCSBCLTimeOUT++)	
	{
		//BSP_CanCheck(1);
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
		if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count <= 0)
		{
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count = 250;
			PT_SendCRO01_B();
		}

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BCL);
		if (iRStartPst >= 0)
		{
			gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT = 5000;
			/******************???????????????????????*******************************/
			gEVBms_2_Info.fBMSRequireVolt = (gEvChargeInfo.fEV_BattVolt_2 - BATT_VOLT_D_VALUE);
			//gEVBms_2_Info.fBMSRequireVolt = ((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0]) * 0.1;
			gEVBms_2_Info.fBMSRequireCurr =ABS((((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]) * 0.1) - 800.00);

			//CtrlRectVoltCurr(gEvChargeInfo.fBMSRequireVolt,gEvChargeInfo.fBMSRequireCurr,FALSE);								//尽快预调电压！
			B_AdjstVoltCurr(gEVBms_2_Info.fBMSRequireVolt, gEVBms_2_Info.fBMSRequireCurr);

			iHaveBCLFlag = TRUE;
			i16BCSBCLTimeOUT = 0;
			return CHARGING_RCV_BCL_SEND_CCS;
		}
		/*****************************************************普天****************************************************************/
		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(XXX_EC);
		if (iRStartPst >= 0)
		{
			stTP_CM_RTSVal_1.CTLByte = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			stTP_CM_RTSVal_1.TotalMsgByteNum = (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
			stTP_CM_RTSVal_1.TotalPkgNum = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
			stTP_CM_RTSVal_1.PkgMsgNo = (UINT32)gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5]
							+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6])<<8)&0x00ff00)
							+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7])<<16)&0xff000);
			stTP_CM_RTSVal_1.PkgMsgNoLSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
			stTP_CM_RTSVal_1.PkgMsgNoMID = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
			stTP_CM_RTSVal_1.PkgMsgNoMSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];

			if ((PF_TP_CM_RTS_VALUE == stTP_CM_RTSVal_1.CTLByte) 
					&& (PT_BCS == stTP_CM_RTSVal_1.PkgMsgNoMID)
					&& (stTP_CM_RTSVal_1.TotalMsgByteNum < (EV_XXX_BUF_NUM - 10))
					&& (0 == stTP_CM_RTSVal_1.PkgMsgNoLSB)
					&& (0 == stTP_CM_RTSVal_1.PkgMsgNoMSB))
			{
				stTP_CM_RTSVal_1Back = stTP_CM_RTSVal_1;
				byTP_CM_Flag_1 = TRUE;
				CleanBCLXXXbuf_B();
				OSTimeDly(2);
				PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1, PF_TP_CM_CTS);
				OSTimeDly(2);
			}
		}

		/*****************************************************普天****************************************************************/
		if (byTP_CM_Flag_1)
		{
			byTP_CM_Flag_1 = FALSE;
			byBRMdataIDX = 0;
			iTotalRcvBRMByteNum = 0;
			gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT = 1250;
			gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = 2000;
			i16PkgNum = 0;
			for (byTriAgain = 0; byTriAgain < TRY_BCS_BCL_AGAIN_TIMES; byTriAgain++)
			{
				OSTimeDly(2);
				iRStartPst = -1;
				iRStartPst = Read_B_AllBuf(XXX_EB);
				if (iRStartPst >= 0)
				{
					byBRMdataIDX = 7 * (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0] - 1);			//pbyRcvBuf[5] sequenceNo
					byBRMdataIDX = MIN(byBRMdataIDX, EV_XXX_BUF_NUM - 10);
					gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = MAX((gs_PuTianMng[GUN_NO_B].u16SetMiltDataInterval*10),750);
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];
					UNUSED(EVBCLXXXbuf_2[0]);
					iTotalRcvBRMByteNum += 7;
					i16PkgNum++;
				}

				if (iTotalRcvBRMByteNum >= stTP_CM_RTSVal_1Back.TotalMsgByteNum || i16PkgNum >= stTP_CM_RTSVal_1Back.TotalPkgNum)
				{
					Clean_B_EB_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_EndofMsgAck);
					OSTimeDly(2);
					break;
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count <= 0)
				{
					gs_PuTianMng[GUN_NO_B].i16CAN2Send_CRO_Count = 250;
					PT_SendCRO01_B();
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT <= 0)
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);			//TIME OUT
					OSTimeDly(2);
					break;
				}

				if ((gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT <= 0) && 0 == iTotalRcvBRMByteNum)	//T1 断开连接
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);
					OSTimeDly(2);
					break;
				}

				if ((gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_BCS_TimeOUT <= 0))
				{
					B_Stuff_CEM(CHARGING_RCV_BCS_OR_S_CRO);
					gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
					RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCS_FIRST);
					return EV_CEM_XXXX;
				}

				gSelfTest.fSetTaskID(TASK_ID_CAN2);
			}//end for
		}

		if ((gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_BCS_TimeOUT <= 0))
		{
			B_Stuff_CEM(CHARGING_RCV_BCS_OR_S_CRO);
			gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCS_FIRST);
			return EV_CEM_XXXX;
		}
		if ((gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT <= 0))
		{
			B_Stuff_CEM(CHARGING_RCV_BCL_SEND_CCS);
			gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCL_FIRST);
			return EV_CEM_XXXX;
		}

		if (i16BCSBCLTimeOUT%10==0)
		{
			OSTimeDly(2);
		}
	}//END FOR

	if (FALSE == iHaveBCSFlag)
	{
		gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
		RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCS_FIRST);
		B_Stuff_CEM(CHARGING_RCV_BCS_OR_S_CRO);
	}

	if (FALSE == iHaveBCLFlag)
	{
		gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
		RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCL_FIRST);
		B_Stuff_CEM(CHARGING_RCV_BCL_SEND_CCS);
	}

	return EV_CEM_XXXX;
}

void PT_SendCCS_B(void);
void PT_SendCCS_B(void)
{
	float fOutVolt, fOutCurr;
	UINT8 CCSbuf[8];
	canmsg TxBufferCAN2;

	fOutVolt = gs_CANComm.fGetRectAnalog(GRP_B, 0, ID_RECT_OUT_VOLT);
	fOutCurr = gs_CANComm.GrpData[GRP_B].fOutCurr;
	if (gEvChargeInfo.RectOwner[GUN_NO_A] == GRP_B)
	{
		fOutCurr += gs_CANComm.GrpData[GRP_A].fOutCurr;
	}

	TxBufferCAN2.Id = (((UINT32)6)<<26) + ((UINT32)PT_CCS<< 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	CCSbuf[0] = 0xff & (UINT16)(fOutVolt * 10);
	CCSbuf[1] = (UINT16)(fOutVolt * 10) >> 8;
	CCSbuf[2] = 0xff &((UINT16)((( (-1) * fOutCurr) + 400) * 10));
	CCSbuf[3] = 0xff &((unsigned int)((( (-1) * fOutCurr) + 400) * 10) >> 8);
	CCSbuf[4] = 0xff &( (UINT16)gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B]);
	CCSbuf[5] = ((UINT16)gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B]) >> 8;

	TxBufferCAN2.Data[0] = ((UINT32)CCSbuf[0] << 24) + ((UINT32)CCSbuf[1] << 16) +  ((UINT32)CCSbuf[2] << 8) + CCSbuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)CCSbuf[4] << 24) + ((UINT32)CCSbuf[5] << 16) +  ((UINT32)CCSbuf[6] << 8) + CCSbuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 6);
}

static void PT_SEND_CCS_Deal_B(void);
static void PT_SEND_CCS_Deal_B(void)
{
	if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CCS_Count <= 0)
	{
		gs_PuTianMng[GUN_NO_B].i16CAN2Send_CCS_Count = 10;
		PT_SendCCS_B();
	}
}

static void Clean_PT_Buf_B(void);
static void Clean_PT_Buf_B(void)
{
	UINT16 iIdx = 0;
	for (iIdx = 0;iIdx < CAN_A_B_BUF_MAX_NUM; iIdx++)
	{
		/**************************************************************************************************************************************************************************************************************************************************
		表1 协议数据单元（PDU）
			P	R	DP	PF	PS	SA	DATA
		位->	3	1	1	8	8	8	0-64
		数据格式要求：
		1、P为优先权：从最高0设置到最低7。
		2、R为保留位：备今后开发使用，本标准设为0。
		3、DP为数据页：用来选择参数组描述的辅助页，本标准设为0。
		4、PF为PDU格式：用来确定PDU的格式，以及数据域对应的参数组编号。
		5、PS为特定PDU格式：PS值取决于PDU 格式。在本标准中采用PDU1格式，PS值为目标地址。
		6、SA为源地址：发送此报文的源地址。
		7、DATA为数据域：若给定参数组数据长度≤8字节，按照本标准第9章规定的报文长度进行传输。若给定参数组数据长度为9~1785字节时，数据传输需多个CAN数据帧，通过传输协议功能的连接管理能力来建立和关闭多包参数组的通信, 详见本标准6.5的规定。
		***************************************************************************************************************************************************************************************************************************************************/
		if ((PT_BroadCast_VAL == (0x3ff&(CanRxMsgBuff_B[iIdx].ExtId >> 16)))
			|| (0x15 == (0x3ff&(CanRxMsgBuff_B[iIdx].ExtId >> 16)))
			|| (0x17 == (0x3ff&(CanRxMsgBuff_B[iIdx].ExtId >> 16)))
			|| (0x16 == (0x3ff&(CanRxMsgBuff_B[iIdx].ExtId >> 16))))
		{
			gCan2RxMsgBuf[iIdx].iFlag = FALSE;
			gCan2RxMsgBuf[iIdx].RxMsg.ExtId = 0;
			CanRxMsgBuff_B[iIdx].ExtId = 0;
		}
	}


	/*UINT16 iIdx = 0;
	for (iIdx = 0;iIdx < CAN_A_B_BUF_MAX_NUM; iIdx++)
	{
		gCan2RxMsgBuf[iIdx].RxMsg = CanRxMsgBuff_B[iIdx];
		gCan2RxMsgBuf[iIdx].iFlag = FALSE;
		gCan2RxMsgBuf[iIdx].RxMsg.ExtId = 0;
		CanRxMsgBuff_B[iIdx].ExtId = 0;
	}*/
}
int PT_Charge_RcvBCL_SendCCS_B(void)
{
#define TRY_BCL_CCS_AGAIN_TIMES 1300
	
	UINT8 u8AlmRst, u8BSMRst, u8PreRslt;
	UINT8 u8BSDSoc = 0;
	UINT32 iBCLTimeOUT = 5000;
	//UINT32 iRect_ON_Delay = 0;
	UINT16 byBRMdataIDX = 0;
	UINT16 byTriAgain = 0;
	UINT16 iTotalRcvBRMByteNum = 0;
	UINT16 iTotalPkgNum = 0;
	UINT8 byTP_CM_Flag_1 = FALSE;
	UINT8 PolarityAlmMask = TRUE;
	UINT8 MccbCheck = TRUE;
	INT16 iRStartPst = -1;
	UINT8 u8IsMinOut = FALSE;
	//UINT8 u8AlmResult = 0;
	__XXX_UNION_VALUE unVal;
	TP_CM_RTS_INFO	stTP_CM_RTSVal_1,stTP_CM_RTSVal_1Back;

	gEVBms_2_Info.byEV_BSM_byte6 = 0;
	gEVBms_2_Info.byEV_BSM_byte7 =0x02;									//允许充电
	//gEvChargeInfo.u16ChargeTime = 0;
	//gEvChargeInfo.fOutEnergyByCalc = 0;
	//ChargePolarityALMCheckByBMS();
	//gEvChargeInfo.uRunPhase = 2;										//充电阶段
	//gEvChargeInfo.u8WorkStat = 2;
	gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT = 5000;							//规定说100MS超时   我们先暂时用1S
	gs_PuTianMng[GUN_NO_B].u32CAN2_BCS_TimeOUT = 5000;	
	//__FAN_ON();
	gEVBms_2_Info.fBCS_MaxCellVolt = 0;

	__B_YELLOW_LED_ON();
	__BMS_2_POWER_ON();
	//__INIT_TP_CM_RTS(stTP_CM_RTSVal,stTP_CM_RTSValBack);

	ExGetSigInfo(E_TYPE_SYS, ID_u8MCCBOnCheckEnable_1, &unVal, NULL, BY_SRAM);
	MccbCheck = unVal.chtemp[0];
	ExGetSigInfo(E_TYPE_SYS, ID_u8PolarityAlmMask_1, &unVal, NULL, BY_SRAM);
	PolarityAlmMask = unVal.chtemp[0];

	PT_StuffCSTData_B(CST_NORMAL);
	
	//Refresh_Charge_Time_Count(TRUE);

	B_Charge_Out_Deal(IS_FIRST_RUNNING ,PolarityAlmMask, MccbCheck,u8IsMinOut);

	for (iBCLTimeOUT = 0; iBCLTimeOUT < 90000; iBCLTimeOUT++)			//1S超时！
	{		
		//BSP_CanCheck(1);
		gSelfTest.fSetTaskID(TASK_ID_CAN2);

		PT_SEND_CCS_Deal_B();

		//Charge_SetCostFlag();

		//if (u8IsBattHiALM())										//电池电压是否异常！
		/*if (u8Is_A_BattHiALM())
		{
			B_SetChargeSTOP_Cmd(CHARGE_BUS_OVER_VOLT);
			return EV_CST_BST;
		}*/


		PT_SEND_CCS_Deal_B();


		if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
		{
			PT_StuffCSTData_B(CST_MAN_MADE);			

			return EV_CST_BST;
		}

		u8AlmRst = u8_B_AlmDela();
		if (u8AlmRst == EV_CST_BST)
		{
			return EV_CST_BST;
		}
		else if (u8AlmRst == EV_TIME_OUT)
		{
			return EV_TIME_OUT;
		}

		if (EV_CST_BST == B_FinishConditionDeal())
		{
			PT_StuffCSTData_B(CST_CODITION_REACH);			
			B_SetChargeSTOP_Cmd(BMS_SOC_100);
			return EV_CST_BST;
		}	

		u8PreRslt = B_Charge_Out_Deal(ISNT_FIRST_RUNNING ,PolarityAlmMask, MccbCheck,u8IsMinOut);
		if (u8PreRslt == PRE_DEAL_TIME_OUT)
		{
			PT_StuffCSTData_B(CST_OTHER);
			//__A_STOP_SEND_CCS();
			B_SetChargeSTOP_Cmd(CRO_BUS_VOLT_ERR);
			return EV_CST_BST;
		}
		else if (PRE_DEAL_POLORI_ALM == u8PreRslt)
		{
			PT_StuffCSTData_B(CST_OTHER);
			//__A_STOP_SEND_CCS();
			B_SetChargeSTOP_Cmd(BATT_VOLT_IS_NEGATIVE);
			return EV_CST_BST;
		}
		else if (PRE_DEAL_OK == u8PreRslt)
		{
			//Ctrl Out 
			B_MCCBCtrl(MCCB_CONNECT);
		}//else PRE_DEAL_RUNNING
		else if(PRE_DEAL_RUNNING == u8PreRslt)
		{
			OSTimeDly(2);
			if(gEVBms_2_Info.fBCP_CurrBattVolt < gEVBms_2_Info.fBCS_MeasureVolt)
			{
				gEVBms_2_Info.fBCP_CurrBattVolt = gEVBms_2_Info.fBCS_MeasureVolt;
			}
		}

		u8BSMRst = u8_B_BSM_Deal();
		if (EV_CST_BST == u8BSMRst)
		{
			PT_StuffCSTData_B(CST_OTHER);			

			B_SetChargeSTOP_Cmd(CHARGE_BSM_ERR);
			return EV_CST_BST;
		}
		else if (EV_BSM_PAUSE == u8BSMRst)
		{
			u8IsMinOut = TRUE;
			gEvChargeInfo.u16GunNoCurrEndDelay[1] = 0;
		}
		else if (EV_BSM_NONE == u8BSMRst)
		{
			u8IsMinOut = FALSE;
		}//No RCV
		
		
		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BCL);
		if (iRStartPst >= 0)
		{
			iBCLTimeOUT = 0;
			gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT = MAX(5000, gs_PuTianMng[GUN_NO_B].u16SetBCLTimeOutVal*10);
			gEVBms_2_Info.fBMSRequireVolt = ((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0]) * 0.1;
			gEVBms_2_Info.fBMSRequireCurr =ABS((((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]) * 0.1) - 800.00);
			//B_AdjstVoltCurr(gEVBms_2_Info.fBMSRequireVolt, gEVBms_2_Info.fBMSRequireCurr);
			////保证一输出	就能够达到我们所需要的数值！
			//PT_SEND_CCS_Deal_B();
			//iRect_ON_Delay++;
			//if (iRect_ON_Delay >= 10)
			//{
			//	iRect_ON_Delay = 0;
			//	_DC_MCCB_ON();
			//	RectOUTCtrl(CTRL_RECT_OPEN, ADJUST_RECT_VOLT_CURR);

			//	__YELLOW_LED_ON();
			//	gEvChargeInfo.uRunEvIndicate = 1;						//充电中
			//}
		}
		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT <= 0)
		{
			B_Stuff_CEM(CHARGING_RCV_BCL_SEND_CCS);
			gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCL);
			return EV_CEM_XXXX;
		}

		PT_SEND_CCS_Deal_B();

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BCS);
		if (iRStartPst >= 0)
		{
			gs_PuTianMng[GUN_NO_B].u32CAN2_BCS_TimeOUT = 5000;
			/*************************************************************************
				2 字节  3075    充电电压测量值（V） 
				2 字节  3076    充电电流测量值（A） 
				1 字节  3077 	电池组最低温度
				1 字节 	3078 	电池组最高温度
				1 字节 	3079 	当前荷电状态 SOC
				1 字节 	3080 	估算剩余时间
			 ****************************************************************/
			gEVBms_2_Info.fBCS_MeasureVolt = (((UINT16)gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0]) * 0.1;    //SPN 3075
			gEVBms_2_Info.fBCS_MeasureCurr = ABS(((((UINT16)gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]) * 0.1) - 800);   //SPN 3076
			gEVBms_2_Info.fBSM_MinBattTemp = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4] -50;
			gEVBms_2_Info.fBSM_MaxBattTemp = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5] -50;
			gEVBms_2_Info.fBCS_SOC = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
			if (TRUE == gEVBms_2_Info.u8JustSTART)
			{
				gEVBms_2_Info.fBCP_CurrSOC = gEVBms_2_Info.fBCS_SOC;													//整车SOH	电池健康状态！
				gEVBms_2_Info.u8JustSTART = FALSE;
			}
			gEVBms_2_Info.u8BSD_StopSOC = (UINT8)gEVBms_2_Info.fBCS_SOC;
			gEVBms_2_Info.u16BCS_LeaveTime = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];
		}
		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BCS_TimeOUT <= 0)
		{
			B_Stuff_CEM(CHARGING_RCV_BCS_OR_S_CRO);
			gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCS);
			return EV_CEM_XXXX;
		}

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BSD);
		if (iRStartPst >= 0)
		{
			u8BSDSoc = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			if (u8BSDSoc >= gEVBms_2_Info.fBCS_SOC)
			{
				gEVBms_2_Info.fBCS_SOC = u8BSDSoc;
			}
			gEVBms_2_Info.u8BSD_StopSOC = (UINT8)gEVBms_2_Info.fBCS_SOC;
			gEVBms_2_Info.fBSD_MinCellVolt = (((UINT16)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2])<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1])*0.01;
			gEVBms_2_Info.fBSD_MaxCellVolt =  (((UINT16)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4])<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3])*0.01;
			gEVBms_2_Info.fBSD_MinTemp = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5] - 50;
			gEVBms_2_Info.fBSD_MaxTemp =  gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6] - 50;
		}

		//ReadAllBuf(PT_BroadCast_VAL);
		//ReadAllBuf(PT_BroadCast_VAL);
		//ReadAllBuf(PT_BroadCast_VAL);
		//ReadAllBuf(0x15);//BMV
		//ReadAllBuf(0x17);//BSOC
		//ReadAllBuf(0x16);//BMT
		PT_SEND_CCS_Deal_B();
		Clean_PT_Buf_B();

		Read_B_AllBuf(PT_BEM);

		PT_SEND_CCS_Deal_B();

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BSM);
		if (iRStartPst >= 0)
		{
			gEVBms_2_Info.fBCS_MaxCellVolt =  0.01*(((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1]<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0])&0xfff);
			gEVBms_2_Info.byEV_BSM_byte6 = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];				
			//178	16:44:12:539	0	接收	1812E5F4	数据帧	扩展帧	8	4F 01 36 5E 00 00 02 00 
			//实际普天协议BSM与第7个字节 和 27930-2015 是不一样的，但是没做处理，抓普天报文看没问题。
			gEVBms_2_Info.byEV_BSM_byte7 =gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];				
		}


		//if (gEvChargeInfo.u8_Charge_CMD[GUN_NO_B] == STOP_CHARGE_VAL)
		//{
		//	CtrlRectONOFF(RECT_OFF_CMD,TRUE);
		//	PT_StuffCSTData_B(CST_MAN_MADE);
		//	gEvChargeInfo.uRunEvIndicate = 0;		//未充电
		//	return EV_CST_BST;
		//}
		
		PT_SEND_CCS_Deal_B();
		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BST);
		if (iRStartPst >= 0)
		{
			gEVBms_2_Info.u8BST_StopReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			gEVBms_2_Info.u16BST_FaultReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] + (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2] << 8);
			gEVBms_2_Info.u8BST_ErrorReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
			gEVBms_2_Info.fBMSRequireVolt = 1;
			//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);
			//if ((gEvChargeInfo.byEV_BSM_byte6 != 0))
			//{
			//	gEvChargeInfo.uRunEvIndicate = 0;								//停止
			//	gEvChargeInfo.u8BMSAbortStopALM = TRUE;
			//	SetChargeSTOP_Cmd(CHARGE_BSM_ERR);//BSM报文异常
			//}
			//else
			//{
			//	gEvChargeInfo.uRunEvIndicate = 2;								//充满
			//	SetChargeSTOP_Cmd(CHARGE_RECEIVE_BST);							//收到BST
			//}
			//RecordStopReason(10);
			Clean_B_EC_Buf();
			PT_SEND_CCS_Deal_B();
			OSTimeDly(2);
			PT_SEND_CCS_Deal_B();
			PT_StuffCSTData_B(CST_RCV_BST_CMD);

			//PT_StuffCSTData_B(CST_RCV_BST_CMD);	

			//__A_STOP_SEND_CCS();
			B_SetChargeSTOP_Cmd(Get_BST_Stop_Reason_B());


			return EV_BSD_CSD;
		}
		
		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(XXX_EC);
		if (iRStartPst >= 0)
		{
			stTP_CM_RTSVal_1.CTLByte = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			stTP_CM_RTSVal_1.TotalMsgByteNum = (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
			stTP_CM_RTSVal_1.TotalPkgNum = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
			stTP_CM_RTSVal_1.PkgMsgNo = (UINT32)gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5]
							+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6])<<8)&0x00ff00)
							+ (((UINT32)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7])<<16)&0xff000);
			stTP_CM_RTSVal_1.PkgMsgNoLSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
			stTP_CM_RTSVal_1.PkgMsgNoMID = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
			stTP_CM_RTSVal_1.PkgMsgNoMSB = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];

			if (PF_TP_CM_RTS_VALUE == stTP_CM_RTSVal_1.CTLByte 
					&& (stTP_CM_RTSVal_1.TotalMsgByteNum <= (EV_XXX_BUF_NUM - 10))
					&& (stTP_CM_RTSVal_1.PkgMsgNoLSB == 0)
					&& (stTP_CM_RTSVal_1.PkgMsgNoMSB == 0))
			{
				stTP_CM_RTSVal_1Back = stTP_CM_RTSVal_1;
				byTP_CM_Flag_1 = TRUE;
				CleanBCLXXXbuf_B();
				Clean_B_EB_Buf();
				PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1, PF_TP_CM_CTS);
				PT_SEND_CCS_Deal_B();
				OSTimeDly(2);
			}
		}

		if (byTP_CM_Flag_1)
		{
			byTP_CM_Flag_1 = FALSE;
			byBRMdataIDX = 0;
			iTotalRcvBRMByteNum = 0;
			iTotalPkgNum = 0;
			gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT = 1250;
			gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = 2000;
			for (byTriAgain = 0; byTriAgain < TRY_BCL_CCS_AGAIN_TIMES; byTriAgain++)
			{
				gSelfTest.fSetTaskID(TASK_ID_CAN2);

				PT_SEND_CCS_Deal_B();


				u8AlmRst = u8_B_AlmDela();
				if (u8AlmRst == EV_CST_BST)
				{
					return EV_CST_BST;
				}
				else if (u8AlmRst == EV_TIME_OUT)
				{
					return EV_TIME_OUT;
				}

				if (EV_CST_BST == B_FinishConditionDeal())
				{
					PT_StuffCSTData_B(CST_CODITION_REACH);			
					B_SetChargeSTOP_Cmd(BMS_SOC_100);
					return EV_CST_BST;
				}	


				//u8AlmResult = u8ChargeALMProcByBMS();
				//if (EV_CST_BST == u8AlmResult)						//门禁  电子锁  CC1
				//{
				//	return EV_CST_BST;
				//}

				//if (EV_TIME_OUT == u8AlmResult)						//急刹车
				//{
				//	return HANDSHAKE_START_CRM00;
				//}//FALSE  no process!!!
				//
				//if (u8IsBattHiALM())
				//{
				//	RecordStopReason(26);
				//	return EV_CST_BST;
				//}

				u8BSMRst = u8_B_BSM_Deal();
				if (EV_CST_BST == u8BSMRst)
				{
					PT_StuffCSTData_B(CST_OTHER);			

					B_SetChargeSTOP_Cmd(CHARGE_BSM_ERR);
					return EV_CST_BST;
				}
				else if (EV_BSM_PAUSE == u8BSMRst)
				{
					u8IsMinOut = TRUE;
					gEvChargeInfo.u16GunNoCurrEndDelay[1] = 0;
				}
				else if (EV_BSM_NONE == u8BSMRst)
				{
					u8IsMinOut = FALSE;
				}//No RCV

				iRStartPst = -1;
				iRStartPst = Read_B_AllBuf(PT_BCL);
				if (iRStartPst >= 0)
				{
					iBCLTimeOUT = 0;
					gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT = MAX(9000, gs_PuTianMng[GUN_NO_B].u16SetBCLTimeOutVal*10);
					gEVBms_2_Info.fBMSRequireVolt = ((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0]) * 0.1;
					gEVBms_2_Info.fBMSRequireCurr =ABS((((gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3] << 8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2]) * 0.1) - 800.00);
					/*if (byTriAgain%10 == 0)
					{
						RectOUTCtrl(NO_ADJUST_ON_OFF, ADJUST_RECT_VOLT_CURR);
					}*/
				}
				if (gs_PuTianMng[GUN_NO_B].u32CAN2_BCL_TimeOUT <= 0)
				{
					B_Stuff_CEM(CHARGING_RCV_BCL_SEND_CCS);
					gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
					RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCL);
					return EV_CEM_XXXX;
				}

				if (gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT <= 0)					//下一包发送慢了！
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);
					PT_SEND_CCS_Deal_B();
					OSTimeDly(2);
					break;
				}

				iRStartPst = -1;
				iRStartPst = Read_B_AllBuf(XXX_EB);
				if (iRStartPst >= 0)
				{
					byBRMdataIDX = 7 * (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0] - 1);			//pbyRcvBuf[5] sequenceNo
					byBRMdataIDX = MIN(byBRMdataIDX, EV_XXX_BUF_NUM - 7);
					gs_PuTianMng[GUN_NO_B].i16CAN2_Interval_T1_TimeOUT = MAX((gs_PuTianMng[GUN_NO_B].u16SetMiltDataInterval*10),750);;
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6];
					EVBCLXXXbuf_2[byBRMdataIDX++] = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[7];
					iTotalRcvBRMByteNum += 7;
					iTotalPkgNum++;
				}

				if (iTotalRcvBRMByteNum >= stTP_CM_RTSVal_1Back.TotalMsgByteNum || iTotalPkgNum>= stTP_CM_RTSVal_1Back.TotalPkgNum)
				{
					Clean_B_EB_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_EndofMsgAck);
					OSTimeDly(2);
					byTP_CM_Flag_1 = FALSE;
					if (PT_BMV == stTP_CM_RTSVal_1Back.PkgMsgNoMID && iTotalRcvBRMByteNum > 8)
					{
						stTP_CM_RTSVal_1.PkgMsgNoMID = 0xff;
						stTP_CM_RTSVal_1Back.PkgMsgNoMID = 0xff;
					}
					iTotalRcvBRMByteNum = 0;
					iTotalPkgNum = 0;
					break;
				}

				if ((gs_PuTianMng[GUN_NO_B].i16CAN2_MutilData_T2_TimeOUT <= 0) && 0 == iTotalRcvBRMByteNum)				//T2 断开连接
				{
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);
					OSTimeDly(2);
					break;
				}

				PT_SEND_CCS_Deal_B();

				iRStartPst = -1;
				iRStartPst = Read_B_AllBuf(PT_BST);
				if (iRStartPst >= 0)
				{
					gEVBms_2_Info.u8BST_StopReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
					gEVBms_2_Info.u16BST_FaultReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] + (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2] << 8);
					gEVBms_2_Info.u8BST_ErrorReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
					gEVBms_2_Info.fBMSRequireVolt = 1;
					//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);
					if ((gEVBms_2_Info.byEV_BSM_byte6 != 0))
					{
						//gEvChargeInfo.uRunEvIndicate = 0;					//停止
						//gEvChargeInfo.u8BMSAbortStopALM = TRUE;
						B_SetChargeSTOP_Cmd(CHARGE_BSM_ERR);					//BSM报文异常
					}
					else
					{
						//gEvChargeInfo.uRunEvIndicate = 2;	//充满
						B_SetChargeSTOP_Cmd(CHARGE_RECEIVE_BST);//收到BST
					}
					
					Clean_B_EC_Buf();
					PT_Send_TP_CM_Proc_B(stTP_CM_RTSVal_1Back, PF_TP_CM_Abort);
					PT_SEND_CCS_Deal_B();
					OSTimeDly(2);
					PT_StuffCSTData_B(CST_RCV_BST_CMD);
					return EV_BSD_CSD;
				}

				if(byTriAgain%2==0)
				{
					PT_SEND_CCS_Deal_B();
					OSTimeDly(2);
					PT_SEND_CCS_Deal_B();
				}
				gSelfTest.fSetTaskID(TASK_ID_CAN2);
			}//END FOR
		}//END IF 多包数据传输！！

		if (iBCLTimeOUT%10==0)
		{
			PT_SEND_CCS_Deal_B();
			OSTimeDly(2);
			PT_SEND_CCS_Deal_B();
		}
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
	}
	//gEvChargeInfo.uRunEvIndicate = 0;	//停止
	B_Stuff_CEM(CHARGING_RCV_BCL_SEND_CCS);
	gB_Result.u8LastReason = CEM_RCV_BCL_SEND_CCS_TIMEOUT;
	RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BCS);
	return EV_CEM_XXXX;
}
	
UINT8 PT_BST_CST_PRC_B(void)		
{
	int iRStartPst = -1;
	UINT16 iIdx = 0;
	canmsg TxBufferCAN2;
	float fCurr;

	TxBufferCAN2.Id = (((UINT32)5)<<26) + ((UINT32)PT_CST << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	TxBufferCAN2.Data[0] = ((UINT32)gCSTbuf_2[0] << 24) + ((UINT32)gCSTbuf_2[1] << 16) +  ((UINT32)gCSTbuf_2[2] << 8) + gCSTbuf_2[3];
	//gEvChargeInfo.fBMSRequireVolt = 100;
	//gEvChargeInfo.fBMSRequireCurr = 1;
	//gEvChargeInfo.uRunPhase = 3;							//结束阶段
	//gEvChargeInfo.fBMSRequireVolt = 153;
	//gEvChargeInfo.u8WorkStat = 3;							//充电结束
	/*RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);
	RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);*/
	gs_PuTianMng[GUN_NO_B].u32CAN2_BST_TimeOUT = 5000;
	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count = 0;
	//gs_CANComm.fSetRectCtlVoltCurr(CTL_CURRENT_LIMIT, 1);				//最小输出方式！
	//gs_CANComm.fSetRectCtlVoltCurr(CTL_VOLTAGE, 0);
	//__FAN_OFF();
	//if (u8RectIsAllClose())								//所有模块关闭状态
	//{
	//	__ONLY_DC_MCCB_OFF();
	//}
	for(iIdx= 1; iIdx < 12500; iIdx++)						//2MS * 2500 = 5S
	{
		fCurr = fGet_B_RectOutCurr();
		if (fCurr <= 5 || gEvChargeInfo.fEV_BattVolt_2 < 150)
		{
			B_MCCBCtrl(MCCB_DISCONNECT);
		}

		if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count <= 0)
		{
			BSP_Can_2_Tx(&TxBufferCAN2, 4);
			/*gEvChargeInfo.fBMSRequireVolt = 1;
			RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);*/
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count = 10;
		}

		//if (u8RectIsAllClose() && TRUE == gEvChargeInfo.u8BUSVoltAbortALM)								
		//{
		//	__ONLY_DC_MCCB_OFF();
		//	//STEP 10
		//	__RESIST_ON();
		//}
		if ((iIdx % 5) == 0)
		{
			OSTimeDly(2);
		}

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BST);
		if (iRStartPst >= 0)
		{
			gEVBms_2_Info.u8BST_StopReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			gEVBms_2_Info.u16BST_FaultReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1] + (gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2] << 8);
			gEVBms_2_Info.u8BST_ErrorReason = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3];
			//if (u8RectIsAllClose())							//所有模块关闭状态
			//{
			//	__ONLY_DC_MCCB_OFF();
			//}
			return EV_BSD_CSD;
		}
		gSelfTest.fSetTaskID(TASK_ID_CAN2);

		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BST_TimeOUT <= 0)
		{
			B_Stuff_CEM(EV_CST_BST);
			RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BST);
			return EV_TIME_OUT;
		}

		//if (u8RectIsAllClose())					//所有模块关闭状态
		//{
		//	__ONLY_DC_MCCB_OFF();
		//}
	}
	B_Stuff_CEM(EV_CST_BST);
	RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BST);
	return EV_TIME_OUT;
}	

void PT_SendCSTData_B(void)
{
	canmsg TxBufferCAN2;
	TxBufferCAN2.Id = (((UINT32)4)<<26) + ((UINT32)PT_CST << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	TxBufferCAN2.Data[0] = ((UINT32)gCSTbuf_2[0] << 24) + ((UINT32)gCSTbuf_2[1] << 16) +  ((UINT32)gCSTbuf_2[2] << 8) + gCSTbuf_2[3];
	BSP_Can_2_Tx(&TxBufferCAN2, 4);
}

void PT_SendCSDData_B(void)
{
	UINT16 u16TempValue;
	canmsg TxBufferCAN2;
	UINT8 gCSDbuf[8];

	//充电机编号
	gCSDbuf[4] = 0x01;
	gCSDbuf[5] = 0xff;
	gCSDbuf[6] = 0xff;
	gCSDbuf[7] = 0xff;
	//充电时间 
	gCSDbuf[0] = ((UINT16)gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B]) & 0xff;
	gCSDbuf[1] = (((UINT16)gEvChargeInfo.fBMS_ChargeTime[GUN_NO_B])&0xff00)>>8;
	//计量 
	u16TempValue = (UINT16)(gEvChargeInfo.fTotalOutEnergy[GUN_NO_B] * 10);
	gCSDbuf[2] = u16TempValue& 0xff;
	gCSDbuf[3] = (u16TempValue&0xff00)>>8;

	TxBufferCAN2.Id = (((UINT32)6)<<26) + ((UINT32)PT_CSD << 16) + ((UINT32)BMS_ADDR<<8) + PT_EV_ADDR;
	TxBufferCAN2.Data[0] = ((UINT32)gCSDbuf[0] << 24) + ((UINT32)gCSDbuf[1] << 16) +  ((UINT32)gCSDbuf[2] << 8) + gCSDbuf[3];
	TxBufferCAN2.Data[1] = ((UINT32)gCSDbuf[4] << 24) + ((UINT32)gCSDbuf[5] << 16) +  ((UINT32)gCSDbuf[6] << 8) + gCSDbuf[7];
	BSP_Can_2_Tx(&TxBufferCAN2, 8);
}

UINT8 PT_BSD_CSD_PRC_B(void)	//CSD 结束条件：1）重新开始握手发送CRM帧 2）关闭辅助电源   CSD：开始条件 收到BSD!
{
	UINT8 u8BSDSoc = 0;
	int iRStartPst;
	UINT32 iIdx = 0;
	float fCurr;

	//gs_CANComm.fSetRectCtlVoltCurr(CTL_CURRENT_LIMIT, 1);		//最小输出方式！
	//gs_CANComm.fSetRectCtlVoltCurr(CTL_VOLTAGE, 100);
	//gEvChargeInfo.fBMSRequireVolt = 159;
	//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);
	//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);
	//gEvChargeInfo.u8WorkStat = 3;					//充电结束
	UINT8 u8ReceiveBSDFlag = FALSE;

	//if (u8RectIsAllClose())						//所有模块关闭状态
	//{
	//	__ONLY_DC_MCCB_OFF();
	//}
	//__ONLY_DC_MCCB_OFF();
	//__FAN_OFF();
	//gEvChargeInfo.uRunPhase = 3;					//结束阶段

	gEVBms_2_Info.byEV_BSM_byte6 = 0;
	gEVBms_2_Info.byEV_BSM_byte7 = 0x30;
	gEVBms_2_Info.fBMSRequireVolt = 100;
	gEVBms_2_Info.fBMSRequireCurr = 1;

	//if (BMS_VERSION_2011 == gEVBms_2_Info.u8BMSVersion)
	{
		gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT = 5000;
	}
	//else
	//{
	//	gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT = 10000;
	//}

	PT_SendCSTData_B();
	gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count = 0;
	UINT32 u32CSDKeep = 0;
	for (iIdx = 0; iIdx < 22500; iIdx++)				//5s 超时
	{
		fCurr = fGet_B_RectOutCurr();
		if (fCurr <= 5 || gEvChargeInfo.fEV_BattVolt_2 < 150)
		{
			B_MCCBCtrl(MCCB_DISCONNECT);
			if (u32CSDKeep > 100)		//250MS发一次	4毫秒加1.
			{
				//__A_STOP_SEND_CST();
				//__A_STOP_SEND_CSD();
				////B_MCCBCtrl(MCCB_DISCONNECT);
				//__A_STOP_ALL_CMD();
				Clean_B_ALL_Buf();

				__BMS_2_POWER_OFF();
				return EV_TIME_OUT;
			}
		}

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BSD);
		if (iRStartPst >= 0)
		{
			u8BSDSoc = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[0];
			if (u8BSDSoc > gEVBms_2_Info.fBCS_SOC)
			{
				gEVBms_2_Info.fBCS_SOC = u8BSDSoc;
			}
			gEVBms_2_Info.u8BSD_StopSOC = (UINT8)gEVBms_2_Info.fBCS_SOC;
			gEVBms_2_Info.fBSD_MinCellVolt = (((UINT16)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[2])<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[1])*0.01;
			gEVBms_2_Info.fBSD_MaxCellVolt =  (((UINT16)(gCan2RxMsgBuf[iRStartPst].RxMsg.Data[4])<<8) + gCan2RxMsgBuf[iRStartPst].RxMsg.Data[3])*0.01;
			gEVBms_2_Info.fBSD_MinTemp = gCan2RxMsgBuf[iRStartPst].RxMsg.Data[5] - 50;
			gEVBms_2_Info.fBSD_MaxTemp =  gCan2RxMsgBuf[iRStartPst].RxMsg.Data[6] - 50;
			/*RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);*/
			u8ReceiveBSDFlag = TRUE;
			break;
		}

		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT <= 0)
		{
			B_Stuff_CEM(EV_BSD_CSD);
			//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);

			//if (u8RectIsAllClose())					//所有模块关闭状态
			//{
			//	__ONLY_DC_MCCB_OFF();
			//}
			B_MCCBCtrl(MCCB_DISCONNECT);
			__BMS_2_POWER_OFF();
			return EV_TIME_OUT;
		}

		if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count <= 0)
		{
			PT_SendCSTData_B();
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CST_Count = 10;
		}

		if ((iIdx%5) == 0)
		{
			OSTimeDly(2);
		}
		//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);
		//if (u8RectIsAllClose())						//所有模块关闭状态
		//{
		//	__ONLY_DC_MCCB_OFF();
		//}
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
	}//END FOR

	gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT = 5000;
	for (iIdx = 0; iIdx < 22500; iIdx++)					//1）重新开始握手发送CRM帧	2）关闭辅助电源
	{
		gSelfTest.fSetTaskID(TASK_ID_CAN2);
		if (gs_PuTianMng[GUN_NO_B].i16CAN2Send_CSD_Count <= 0)
		{
			gs_PuTianMng[GUN_NO_B].i16CAN2Send_CSD_Count = 250;
			//再收一次BSD
			PT_SendCSDData_B();
		}

		if (gs_PuTianMng[GUN_NO_B].u32CAN2_BSD_TimeOUT <= 0)
		{
			if ( FALSE == u8ReceiveBSDFlag)
			{
				B_Stuff_CEM(EV_BSD_CSD);
				//RectOUTCtrl(CTRL_RECT_CLOSE, ADJUST_RECT_VOLT_CURR);

				//if (u8RectIsAllClose())					//所有模块关闭状态
				//{
				//	__ONLY_DC_MCCB_OFF();
				//}
				B_MCCBCtrl(MCCB_DISCONNECT);
				__BMS_2_POWER_OFF();
				RecordChargeRecord(GUN_NO_B, EVENT_TIMEOUT_BSD);
				return EV_TIME_OUT;
			}
			else
			{
				break;
			}
		}

		if (iIdx >= 2500 && TRUE == u8ReceiveBSDFlag)					//2500*2/50 意思有BSD 后发个10包左右的BSD就退出来
		{
			break;
		}

		iRStartPst = -1;
		iRStartPst = Read_B_AllBuf(PT_BSD);
		if (iRStartPst >= 0)
		{
			OSTimeDly(2);
			break;
		}
		OSTimeDly(2);
		//if (u8RectIsAllClose())					//所有模块关闭状态
		//{
		//	__ONLY_DC_MCCB_OFF();
		//}
	}

	if (iIdx <= 2500)									//前边BREK了，多发几包数据
	{
		PT_SendCSDData_B();
		OSTimeDly(2);
		PT_SendCSDData_B();
		OSTimeDly(2);
		PT_SendCSDData_B();
		OSTimeDly(2);
		PT_SendCSDData_B();
		OSTimeDly(2);
		PT_SendCSDData_B();
		OSTimeDly(2);
		PT_SendCSDData_B();
		OSTimeDly(2);
		PT_SendCSDData_B();
		OSTimeDly(2);
	}

	B_MCCBCtrl(MCCB_DISCONNECT);
	__BMS_2_POWER_OFF();
	return EV_TIME_OUT;
}
void PT_StuffCSTData_B(UINT8 u8Case)
{
	switch (u8Case)
	{
	case CST_NORMAL: //00 00 00 00
		gCSTbuf_2[0]= 0x00;
		gCSTbuf_2[1]= 0x00;
		gCSTbuf_2[2]= 0x00;
		break;
	case CST_CONNECT_FAILS:
		gCSTbuf_2[0]= 0x00;
		gCSTbuf_2[1]= (0x01<<2);//充电机连接器故障
		gCSTbuf_2[2]= 0x00;
		break;
	case CST_CODITION_REACH://达到人工预设条件终止
		gCSTbuf_2[0]= 0x01;										
		gCSTbuf_2[1]= 0x00;
		gCSTbuf_2[2]= 0x00;
		break;
	case CST_EV_OVER_TEMP:
		gCSTbuf_2[0]= 0x00;
		gCSTbuf_2[1]= 0x01;//有故障
		gCSTbuf_2[2]= 0x00;
		break;

	case CST_RCV_BST_CMD://BMS主动终止
		gCSTbuf_2[0]= 0x00;							
		gCSTbuf_2[1]= 0x00;
		gCSTbuf_2[2]= 0x00;
		break;

		//0000 0000
	case CST_MAN_MADE:
		gCSTbuf_2[0]= 0x00;						
		gCSTbuf_2[1]= 0x00;
		gCSTbuf_2[2]= 0x01<<4;	//人工终止
		break;

		//0000 0000
	case CST_OTHER:
	case CST_EPO_ALM:
	case CST_OVER_VOLT:
		gCSTbuf_2[0]= 0x00;							
		gCSTbuf_2[1]= 0x01<<5;//故障终止
		gCSTbuf_2[2]= 0;
		break;
	default:
		gCSTbuf_2[0]= 0x00;
		gCSTbuf_2[1]= 0x00;
		gCSTbuf_2[2]= 0x00;
		break;
	}
}