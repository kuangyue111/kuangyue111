#include "main.h"
#include "Uart6.h"
#include "Uart4and5.h"
#include "EtherNet.h"
#include "Measure.h"
#include "CAN_BMS.h"
#include "UDisk.h"

#define ABS(a)	(((a) < 0) ? (-(a)) : (a))
NOINIT UART6_RECV_INFO gUart6RecvBuff[RS232_UART6_RX_BUFF_NUM];
NOINIT UINT8 gUart6EVSendData[RS232_UART6_TX_MAX_NUM];
extern void GUN_A_TestRecordProcess(void);
extern void GUN_B_TestRecordProcess(void);

static UINT16 CRC_check(UINT8 *fp_CRC, UINT8 count)
{
	UINT8 i;
	UINT16 ModbusCRC;
	i = 0;
	ModbusCRC = 0xffff;
	while(count!=0)
	{
		ModbusCRC^=*fp_CRC; 
		for(i=0;i<8;i++) 
		{
			if((ModbusCRC&0x0001)==0)
			{
				ModbusCRC=ModbusCRC>>1;
			}
			else
			{ 
				ModbusCRC=ModbusCRC>>1; 
				ModbusCRC^=0xa001;
			}
		}
		count-=1;
		fp_CRC++;
	}
	return ModbusCRC;
}
static void SetUserSelectMode(UCHAR ucGunNo, UCHAR ucMode)
{
	gChargeCtrl.ucUserSelectMode[ucGunNo] = ucMode;
	if((ucGunNo == GUN_NO_A )&&(ucMode != NO_USER_MODE))
	{
		if(gChargeCtrl.ucParallelMode[GUN_NO_B] >0)
		{
			gChargeCtrl.ucParallelMode[GUN_NO_B] = 0;
		}
	}
	else if((ucGunNo == GUN_NO_B )&&(ucMode != NO_USER_MODE))
	{
		if(gChargeCtrl.ucParallelMode[GUN_NO_A] >0)
		{
			gChargeCtrl.ucParallelMode[GUN_NO_A] = 0;
		}
	}
	
}
static void MdbCmd0X05Proc(UINT8* pRcvBuf)
{
	WORD wRegValue = 0,wSIGValue =0;
	UCHAR StartCount = 0,ctemp,ucGunNo,i;
	ucGunNo = pRcvBuf[MDB_RCV_BYTE_0_ADDR]-MDB_ADDR_VALUE;
	wRegValue = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));
	wSIGValue = ((((WORD)pRcvBuf[MDB_SIG_VALUE_PST]) << 8) + ((WORD)pRcvBuf[MDB_SIG_VALUE_PST + 1] & 0xff));
	switch (wRegValue + 1)
	{
	case BUTTOM_START_1_CHARGE_1:
	case BUTTOM_START_2_CHARGE_2:
		gChargeCtrl.u8BMSPowerStep[ucGunNo]=(wRegValue + 1)-BUTTOM_START_1_CHARGE_1;
		gChargeCtrl.u8BMSPoweState[ucGunNo]	= mBMS_POWER_NO_CHECK;
		gChargeCtrl.u8ChargeState[ucGunNo] = START_CHARGE_STATE;    //充电状态
		gChargeCtrl.u16StartCount[ucGunNo] = 1;    //充电状态
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo] =BUTTOM_START_1_CHARGE+(wRegValue + 1)-BUTTOM_START_1_CHARGE_1;
		break;
	case BUTTOM_INSUL_START_4:
		gChargeCtrl.u8BMSPoweState[ucGunNo]	= mBMS_POWER_NO_CHECK;
		SetUserSelectMode(ucGunNo,USER_TEST_MODE);
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo] =BUTTOM_INSUL_START;
		
		for(i=0;i<22;i++)
		{
			gChargeCtrl.ucEVCode[ucGunNo][i] = 0;
		}
	
		
		break;
	case BUTTOM_BMS_START_5:
		gChargeCtrl.u8BMSPoweState[ucGunNo]	= mBMS_POWER_NO_CHECK;
		SetUserSelectMode(ucGunNo,USER_BMS_MODE);
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo] = BUTTOM_BMS_START;
		gChargeCtrl.u16PutResOnBusCount[ucGunNo] = 0;	 //绝缘开始计时
		break;
	case BUTTOM_AGING_MODE_3:
		gChargeCtrl.u8BMSPoweState[ucGunNo]	= mBMS_POWER_NO_CHECK;
		SetUserSelectMode(ucGunNo,USER_AGING_MODE);
		for(i=0;i<22;i++)
		{
			gChargeCtrl.ucEVCode[ucGunNo][i] = 0;
		}
		break;
	case BUTTOM_START_AGING_12:
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo]	= BUTTOM_START_AGING;
		break;

	case BUTTOM_STOP_CHARGE_6:
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo] = NO_BUTTOM_START;
		break;

	case BUTTOM_STOP:
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo]= NO_BUTTOM_START;	
		break;

	case BUTTOM_INSULCOUNT_8:
		gChargeCtrl.u16InsulState[ucGunNo] = 0;
		gChargeCtrl.u16PutResOnBusCount[ucGunNo] = 1;	 //绝缘开始计时
		gChargeCtrl.ucInsulationVoltCheckFlag[ucGunNo] = FALSE;
        break;

	case BUTTOM_TESTEND_11:
		SetUserSelectMode(ucGunNo,NO_USER_MODE);
		gChargeCtrl.ucUIStartChargeCMD[ucGunNo] = NO_BUTTOM_START;
		if(gChargeCtrl.ucTestRecordState[ucGunNo] == 1)
		{
			gChargeCtrl.ucTestRecordState[ucGunNo] = 2;
			if(ucGunNo == GUN_NO_A)
				GUN_A_TestRecordProcess();
			else
				GUN_B_TestRecordProcess();
		}
		
		
		break;
	case BUTTOM_SOURCE_13:
		SetUserSelectMode(ucGunNo,USER_SOURCE_MODE);
		break;
	default:
		break;;
	}

	dv_Uart6.fSendData(pRcvBuf, 8);
}



static void MdbCmd0X10Proc(UINT8* pRcvBuf)
{
    UCHAR i,ctemp,ucGunNo;
	struct tm  Settm;
	WORD wCRC = 0;
	WORD	wRegValue = 0;
	WORD	wSIGValue = 0;
	float   ftemp;
	ucGunNo = pRcvBuf[MDB_RCV_BYTE_0_ADDR]-MDB_ADDR_VALUE;
	if(ucGunNo>=ALL_GUN_NUM)
	{
		return;
	}
	wRegValue = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));
	wSIGValue = ((((WORD)pRcvBuf[MDB_SIG_VALUE_PST]) << 8) + ((WORD)pRcvBuf[MDB_SIG_VALUE_PST + 1] & 0xff));
	switch (wRegValue + 1)
	{
	case 6:
		gChargeCtrl.fBMSRequireCurr[ucGunNo] = wSIGValue * 0.01;  //需求电流
		break;
	case 7:
		gChargeCtrl.fUserSetVolt[ucGunNo]= wSIGValue * 0.1;  //需求电压
		break;
	case 8:
		gChargeCtrl.u16AgingTime[ucGunNo] = wSIGValue*60;          //测试时长
        gChargeCtrl.u16AgingCount[ucGunNo] = gChargeCtrl.u16AgingTime[ucGunNo];
		break;
	case 9:
		gChargeCtrl.fUserSetLoadCurr[ucGunNo] = wSIGValue * 0.01;  //需求电流
		break;
	case 10:
		gChargeCtrl.u16FaultStep[ucGunNo] = wSIGValue;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
		break;
    case 9300: 
        for(i=0;i<11;i++)
        {
            gChargeCtrl.ucEVCode[ucGunNo][i*2]   = pRcvBuf[MDB_SIG_VALUE_PST + 1+i*2];
            gChargeCtrl.ucEVCode[ucGunNo][i*2+1] = pRcvBuf[MDB_SIG_VALUE_PST+i*2];
        }
        break;
	case 9311: 
		gChargeCtrl.ucScanEVCodeNumber[ucGunNo] = wSIGValue;
		if((wSIGValue==1)&&(ucGunNo== GUN_NO_A))
		{
			gChargeCtrl.ucScanEVCodeNumber[GUN_NO_B] = 0;
		}
		if((wSIGValue==1)&&(ucGunNo== GUN_NO_B))
		{
			gChargeCtrl.ucScanEVCodeNumber[GUN_NO_A] = 0;
		}
		break;
	case 6600:
		Settm.tm_sec = ((((WORD)pRcvBuf[7]) << 8) + ((WORD)pRcvBuf[7 + 1] & 0xff));
		Settm.tm_min = ((((WORD)pRcvBuf[7 + 2]) << 8) + ((WORD)pRcvBuf[7 + 2+ 1] & 0xff));
		Settm.tm_hour = ((((WORD)pRcvBuf[7 + 4]) << 8) + ((WORD)pRcvBuf[7 + 4 + 1] & 0xff));
		Settm.tm_mday = ((((WORD)pRcvBuf[7 + 6]) << 8) + ((WORD)pRcvBuf[7 + 6 + 1] & 0xff));
		Settm.tm_mon = ((((WORD)pRcvBuf[7 + 8]) << 8) + ((WORD)pRcvBuf[7 + 8 + 1] & 0xff));
		Settm.tm_year = ((((WORD)pRcvBuf[7 + 10]) << 8) + ((WORD)pRcvBuf[7 + 10 + 1] & 0xff));
		gRTCData.fSetLocalTime(&Settm);
		break;
	case 6606:
		gFlashOptInfo.u8CleanRecordsFlag =(UCHAR) wSIGValue;
		break;
	case 6607:
		if((wSIGValue>0)&&(wSIGValue<=gChargeLogTable.u32TotalNum))
		{
			gs_UDiskData.u16RecordStartNum = wSIGValue;
		}
		else if(wSIGValue==0)
		{
			gs_UDiskData.u16RecordStartNum = 1;
		}
		else if(wSIGValue>gChargeLogTable.u32TotalNum)
		{
			gs_UDiskData.u16RecordStartNum = gChargeLogTable.u32TotalNum;
		}
		break;
	case 6608:
		if((wSIGValue>0)&&(wSIGValue<=gChargeLogTable.u32TotalNum))
		{
			gs_UDiskData.u16RecordEndNum = wSIGValue;
		}
		else if(wSIGValue==0)
		{
			gs_UDiskData.u16RecordEndNum = 1;
		}
		else if(wSIGValue>gChargeLogTable.u32TotalNum)
		{
			gs_UDiskData.u16RecordEndNum = gChargeLogTable.u32TotalNum;
		}
		if(gs_UDiskData.u16RecordStartNum > gs_UDiskData.u16RecordEndNum)
		{
			gs_UDiskData.u16RecordEndNum= gs_UDiskData.u16RecordStartNum;
		}
		break;
	case 6609:
		gs_UDiskData.ucCopyRecordFlag = wSIGValue;
		break;
	case 6882: 
		for(i=0;i<9;i++)
		{
			gChargeCtrl.ucVIN[ucGunNo][i*2] = pRcvBuf[MDB_SIG_VALUE_PST + 1+i*2];
			gChargeCtrl.ucVIN[ucGunNo][i*2+1] = pRcvBuf[MDB_SIG_VALUE_PST+i*2];
		}
		break;
	case 6892:
		//gEvChargeInfo.u16BROHead = wSIGValue;
		break;
	case 6893:
		//gEvChargeInfo.ucBRO00SendTestTime = (UCHAR)wSIGValue;
		break;
	case 19003:
		gChargeCtrl.fBMSRequireCurr[ucGunNo] = wSIGValue * 0.01;  //需求电流
		break;
	case 19002:
		gChargeCtrl.fUserSetVolt[ucGunNo] = wSIGValue * 0.1;  //需求电压
		break;
	case 19006:
	case 19400:
		gChargeCtrl.ucBMSProtocolType[ucGunNo] = wSIGValue ;  //需求电压
		break;
	case 19401:
		gChargeCtrl.ucTestGunNum[ucGunNo] = wSIGValue ;  //需求电压
		break;
	case 19007:
		gChargeCtrl.fUserSetLoadCurr[ucGunNo] = wSIGValue * 0.01;  //用户设置输出电流
		break;
	case 19008:
		gChargeCtrl.ucRectWorkMode[ucGunNo] = (UCHAR)wSIGValue;
		break;
	case 6809:
	case 19009:
		if(ucGunNo == GUN_NO_A)
		{
			ctemp = GUN_NO_B;
		}
		else if(ucGunNo == GUN_NO_B)
		{
			ctemp = GUN_NO_A;
		}
		if(gChargeCtrl.ucParallelMode[ctemp] >= TwoGun_Mode	)
		{
			gChargeCtrl.ucParallelMode[ucGunNo] = Independent_Mode;
		}
		else
		{
			gChargeCtrl.ucParallelMode[ucGunNo] = (UCHAR)wSIGValue;
		}
		
		break;
	case 6701:
		if(ucGunNo == GUN_NO_A)
		{
			gEvChargeInfo.fBattSet_A_Volt= wSIGValue * 0.1;
			gEvChargeInfo.u8Batt_1VoltSet_A_Flag = TRUE;
		}
		else
		{
			gEvChargeInfo.fBattSet_A_Volt= wSIGValue * 0.1;
			gEvChargeInfo.u8Batt_2VoltSet_A_Flag = TRUE;
		} 
		break;
	case 6702:
		if(ucGunNo == GUN_NO_A)
		{
			gEvChargeInfo.fBattSet_B_Volt= wSIGValue * 0.1;
			gEvChargeInfo.u8Batt_1VoltSet_B_Flag = TRUE;
		}
		else
		{
			gEvChargeInfo.fBattSet_B_Volt= wSIGValue * 0.1;
			gEvChargeInfo.u8Batt_2VoltSet_B_Flag = TRUE;
		} 
		break;
	case 6703:
		gIMIUData[ucGunNo].ucSetShuntCurr = TRUE;
		gIMIUData[ucGunNo].fSetShuntCurr = (float)wSIGValue;
		break;
	case 6705:
		if(gIMIUData[ucGunNo].ucAdjustVoltStep == 0)
		{
			gIMIUData[ucGunNo].ucAdjustVoltStep = 1;
			gIMIUData[ucGunNo].fAdjustVolt[0]= wSIGValue * 0.1;
		}
		break;
	case 6706:
		if(gIMIUData[ucGunNo].ucAdjustVoltStep == 2)
		{
			gIMIUData[ucGunNo].fAdjustVolt[1]= wSIGValue * 0.1;
			gIMIUData[ucGunNo].ucAdjustVoltStep = 3;
		}
		break;
	case 6708:
		if(gIMIUData[ucGunNo].ucAdjustCurrStep ==0)
		{
			gIMIUData[ucGunNo].ucAdjustCurrStep = 1;
			gIMIUData[ucGunNo].fAdjustCurr[0]= wSIGValue * 0.1;
		}
		break;
	case 6709:
		if(gIMIUData[ucGunNo].ucAdjustCurrStep == 2)
		{
			gIMIUData[ucGunNo].fAdjustCurr[1]= wSIGValue * 0.1;
			gIMIUData[ucGunNo].ucAdjustCurrStep = 3;
		}
		break;

	case 6880: 
		ftemp = wSIGValue * 0.1;
		SetFloatSetData(SETDATA_GUN1_fVoltDetal_NUM+ucGunNo,&ftemp);
		break;
	case 6881: 
		ftemp = wSIGValue * 0.1;
	    SetFloatSetData(SETDATA_GUN1_fCurrDetal_NUM+ucGunNo,&ftemp);
		break;
    case 6891: 
		ctemp = (UCHAR)wSIGValue;
		//SetCharSetData(SETDATA_ucProtocolType_NUM,&ctemp);
		break;
	case 7000: 
		ftemp=gBmsCommDataInfo[ucGunNo].fBCP_BattStartVolt = wSIGValue * 0.1;
		break;
    case 7001: 
        gBmsCommDataInfo[ucGunNo].fBCP_MaxVolt = wSIGValue * 0.1;
        break;
	case 7002: 
		gBmsCommDataInfo[ucGunNo].fBCP_MaxPermitCellVolt = wSIGValue * 0.01;
		break;
	case 7003: 
		gBmsCommDataInfo[ucGunNo].fBCP_MaxPermitTemp = wSIGValue;
		break;
	case 7004: 
		gBmsCommDataInfo[ucGunNo].fBCS_MaxCellVolt  = wSIGValue* 0.01;
		break;
	case 7005: 
		gBmsCommDataInfo[ucGunNo].fBSM_MaxCellTemp = wSIGValue;
		break;
    case 7006: 
		gBmsCommDataInfo[ucGunNo].fBCP_BattStartSOC = wSIGValue;
		break;
	case 7007: 
		gBmsCommDataInfo[ucGunNo].fBCP_MaxPermitCurr = wSIGValue * 0.1;
		break;
    case 7008: 
		ctemp = wSIGValue;
		SetCharSetData(SETDATA_GUN1_ucAuxPowerType_NUM+ucGunNo,&ctemp);
		break;
    case 7009: 
		ctemp = wSIGValue;
		SetCharSetData(SETDATA_ucLoadDataSource_NUM,&ctemp);
		break;
	case 7010: 
		ctemp = wSIGValue;
		SetCharSetData(SETDATA_ucDoCtrlBoard_NUM,&ctemp);
		break;
	case 20200:
		gChargeCtrl.ucRectWorkMode[ucGunNo] = (UCHAR)wSIGValue;
		break;
	case 20201:
		gChargeCtrl.ucParallelMode[ucGunNo] = (UCHAR)wSIGValue;
		break;
	case 20204:
		gChargeCtrl.fBMSRequireVolt[ucGunNo] = wSIGValue * 0.1;
		break;
	case 20205:
		gChargeCtrl.fBMSRequireCurr[ucGunNo] = wSIGValue * 0.1;
		break;
	case 20206:
		gChargeCtrl.ucSourceCtrlCmd[ucGunNo] = (UCHAR)wSIGValue;
		break;
	case 21000: 
		if(wSIGValue == 0)
		{
			if(gFlashOptInfo.u32ViewRecordNo>0)
				gFlashOptInfo.u32ViewRecordNo--;
			else
				gFlashOptInfo.u32ViewRecordNo =gChargeLogTable.u32TotalNum;
		}
		else if(wSIGValue == 1)
		{
			if(gFlashOptInfo.u32ViewRecordNo<gChargeLogTable.u32TotalNum)
				gFlashOptInfo.u32ViewRecordNo++;
			else
				gFlashOptInfo.u32ViewRecordNo =1;
		}
		break;
	case 22000:
		ctemp = wSIGValue;
		SetCharSetData(SETDATA_ucNetEnable_NUM,&ctemp);
		break;
	case 22001:
		ctemp = wSIGValue;
		SetCharSetData(SETDATA_ucDHCPEnable_NUM,&ctemp);
		break;
	case 22002:
	case 22003:
	case 22004:
	case 22005:
		ctemp = wSIGValue;
		i = wRegValue + 1-22002;
		SetCharSetData(SETDATA_ucSelfIP_1_NUM+i,&ctemp);
		break;
	case 22006:
	case 22007:
	case 22008:
	case 22009:
		ctemp = wSIGValue;
		i = wRegValue + 1-22006;
		SetCharSetData(SETDATA_ucSubMask_1_NUM+i,&ctemp);
		break;
	case 22010:
	case 22011:
	case 22012:
	case 22013:
		ctemp = wSIGValue;
		i = wRegValue + 1-22010;
		SetCharSetData(SETDATA_ucGateWay_1_NUM+i,&ctemp);
		break;
	case 22014:
	case 22015:
	case 22016:
	case 22017:
		ctemp = wSIGValue;
		i = wRegValue + 1-22010;
		SetCharSetData(SETDATA_ucServerIP_1_NUM+i,&ctemp);
		break;
	case 22018:
		ctemp = (UCHAR)(wSIGValue>>8);
		SetCharSetData(SETDATA_ucNetPort_H_NUM,&ctemp);
		ctemp = (UCHAR)(wSIGValue&0x00FF);
		SetCharSetData(SETDATA_ucNetPort_L_NUM,&ctemp);
		break;
	default:
		break;;
	}
	gUart6EVSendData[MDB_RCV_BYTE_0_ADDR] = MDB_ADDR_VALUE;
	gUart6EVSendData[MDB_RCV_BYTE_1_CMD] = 0x10;
	gUart6EVSendData[MDB_RCV_BYTE_2_REG_ADDR_H] = pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H];
	gUart6EVSendData[MDB_RCV_BYTE_3_REG_ADDR_L] = pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L];
	gUart6EVSendData[4] = 0;
	gUart6EVSendData[5] = pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L + 2];					//寄存器总数
	wCRC = CRC_check(gUart6EVSendData, 6);								//这种计算方法反过来了！

	gUart6EVSendData[7] = EV_MNEU24_HI(wCRC);
	gUart6EVSendData[6] = EV_MNEU24_LO(wCRC);

	dv_Uart6.fSendData(gUart6EVSendData, 8);
}








#define MDB_MENU13_DETAILED_REG			220
#define MDB_MENU25_MAINTENANCE_REG		100
#define MDB_MENU37_MAINTENANCE_2_REG	150
#define MDB_MENU27_RESULT_REG			200
#define MDB_MENU24_CHARGE_REG			300
#define	MDB_MENU11_EVINFO_REG			1
#define	MDB_MENU18_BMS_SET_REG			270
#define MDB_MENU19_TIME_SET_REG			350
#define MDB_MENU20_ENERGY_SET_REG		390
#define MDB_MENU_MONEY_SET_REG			430
#define MAIN_MENU_10_PLC_CTL_500		500
#define MAIN_MENU_36_BMS_INFO_550		550
#define MDBU_MENU35_ALM_INFO			850

#define MDBU_MENU75_03_AGING_SET_REG		6
#define MDBU_MENU79_03_AGING_REG			6801
#define MDBU_MENU79_04_AGING_REG			6800
#define MDBU_MENU93_04_AGING_REG			9300
#define MDBU_MENU101_03_REG					10100
#define MDBU_MENU103_03_REG					10300
#define MDBU_MENU101_04_REG					10105
#define MDBU_MENU105_03_REG					10500
#define MDBU_MENU127_04_REG					5
static UCHAR CMDAnalyse( UINT16 ReadPF)
{
#define host_XXX_CHM 0x26
#define host_XXX_BHM 0x27
#define host_XXX_CRM 0x01
#define host_XXX_BRM 0x02
#define host_XXX_EC  0xEC
#define host_XXX_BCP 0x06
#define host_XXX_CTS 0x07
#define host_XXX_CML 0x08
#define host_XXX_BR0 0x09
#define host_XXX_CR0 0x0A
#define host_XXX_CCS 0x12
#define host_XXX_BCL 0x10
#define host_XXX_BCS 0x11
#define host_XXX_CST 0x1a
#define host_XXX_BST 0x19
#define host_XXX_BSD 0x1C
#define host_XXX_CSD 0x1D
#define host_XXX_CEM 0x1F
#define host_XXX_BSM 0x13
#define host_XXX_BEM 0x1E
#define host_XXX_EB 0xEB
	if((ReadPF ==host_XXX_CHM)||(ReadPF ==host_XXX_BHM))
	{
		return 1;
	}
	else if((ReadPF ==host_XXX_CRM)||(ReadPF ==host_XXX_BRM))
	{
		return 2;
	}
	else if((ReadPF ==host_XXX_CTS)||(ReadPF ==host_XXX_BCP))
	{
		return 3;
	}
	else if((ReadPF ==host_XXX_CML)||(ReadPF ==host_XXX_BR0))
	{
		return 4;
	}
	else if((ReadPF ==host_XXX_CR0)||(ReadPF ==host_XXX_BCL))
	{
		return 5;
	}
	else if((ReadPF ==host_XXX_CCS)||(ReadPF ==host_XXX_BCS))
	{
		return 6;
	}
	else if((ReadPF ==host_XXX_CST)||(ReadPF ==host_XXX_BST))
	{
		return 7;
	}
	else if((ReadPF ==host_XXX_CSD)||(ReadPF ==host_XXX_BSD))
	{
		return 8;
	}
	else if((ReadPF ==host_XXX_CEM)||(ReadPF ==host_XXX_BSM))
	{
		return 9;
	}
	else if(ReadPF ==host_XXX_EC)
	{
		return 10;
	}
	else if(ReadPF ==host_XXX_EB)
	{
		return 11;
	}
	else if(ReadPF ==host_XXX_BEM)
	{
		return 12;
	}
	else if((ReadPF == 0x100)||(ReadPF ==0x108))
	{
		return 13;
	}
	else if((ReadPF == 0x101)||(ReadPF ==0x109))
	{
		return 14;
	}
	else if((ReadPF == 0x102)||(ReadPF ==0x118))
	{
		return 15;
	}
	else if(ReadPF == 0x110)
	{
		return 16;
	}
}
static void RPT_UpMENU65_04_REG(UCHAR ucGunNo)
{
#define Menu65_04_IDX_ADDR				0
#define Menu65_04_IDX_CMD				1
#define Menu65_04_IDX_BYTES_NUM			2

#define Menu65_04_IDX_0H_RTN			3
#define Menu65_04_IDX_0L_RTN			4

#define Menu65_04_IDX_CRC_HI			5
#define Menu65_04_IDX_CRC_LO			6
#define Menu65_04_IDX_MAX_SIG_NUM		7

	UINT8 i;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu65_04_IDX_MAX_SIG_NUM);

	pSendData[Menu65_04_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu65_04_IDX_CMD]		= 0x04;
	pSendData[Menu65_04_IDX_BYTES_NUM]	= (Menu65_04_IDX_CRC_HI - Menu65_04_IDX_BYTES_NUM-1);


	pSendData[Menu65_04_IDX_0H_RTN] = 0;
	pSendData[Menu65_04_IDX_0L_RTN] = gChargeCtrl.ucStepRTN[ucGunNo];  


	wCRC = CRC_check(pSendData, Menu65_04_IDX_CRC_HI);
	pSendData[Menu65_04_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu65_04_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, Menu65_04_IDX_MAX_SIG_NUM);
}
static void RPT_UpMENU93_04_REG(UCHAR ucGunNo)
{
#define Menu93_04_IDX_ADDR				0
#define Menu93_04_IDX_CMD				1
#define Menu93_04_IDX_BYTES_NUM			2

#define Menu93_04_IDX_0H_CODE			3
#define Menu93_04_IDX_0L_CODE			4

#define Menu93_04_IDX_0H_SCAN			25
#define Menu93_04_IDX_0L_SCAN			26

#define Menu93_04_IDX_CRC_HI			27
#define Menu93_04_IDX_CRC_LO			28
#define Menu93_04_IDX_MAX_SIG_NUM		29

	UINT8 i;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData , 0, Menu93_04_IDX_MAX_SIG_NUM);

	pSendData[Menu93_04_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu93_04_IDX_CMD]			= 0x04;
	pSendData[Menu93_04_IDX_BYTES_NUM]	= (Menu93_04_IDX_CRC_HI - Menu93_04_IDX_BYTES_NUM-1);

	for(i=0;i<11;i++)
	{
		pSendData[Menu93_04_IDX_0L_CODE+i*2] = gChargeCtrl.ucEVCode[ucGunNo][i*2];
		pSendData[Menu93_04_IDX_0H_CODE+i*2] = gChargeCtrl.ucEVCode[ucGunNo][i*2+1];  
	}

	pSendData[Menu93_04_IDX_0H_SCAN] = 0;
	pSendData[Menu93_04_IDX_0L_SCAN] = gChargeCtrl.ucScanEVCodeNumber[ucGunNo];
	

	wCRC = CRC_check(pSendData, Menu93_04_IDX_CRC_HI);
	pSendData[Menu93_04_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu93_04_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, Menu93_04_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU190_04_REG(UCHAR ucGunNo)
{
#define Menu190_04_IDX_ADDR				0
#define Menu190_04_IDX_CMD				1
#define Menu190_04_IDX_BYTES_NUM		2

#define Menu190_04_IDX_0H_Phase			3
#define Menu190_04_IDX_0L_Phase			4
#define Menu190_04_IDX_0H_Stop			5
#define Menu190_04_IDX_0L_Stop			6
#define Menu190_04_IDX_0H_EVCMD			7
#define Menu190_04_IDX_0L_EVCMD			8
#define Menu190_04_IDX_0H_BMSCMD		9
#define Menu190_04_IDX_0L_BMSCMD		10

#define Menu190_04_IDX_CRC_HI			11
#define Menu190_04_IDX_CRC_LO			12
#define Menu190_04_IDX_MAX_SIG_NUM		13

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu190_04_IDX_MAX_SIG_NUM);

	pSendData[Menu190_04_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu190_04_IDX_CMD]		= 0x04;
	pSendData[Menu190_04_IDX_BYTES_NUM]	= (Menu190_04_IDX_CRC_HI - Menu190_04_IDX_BYTES_NUM-1);


	pSendData[Menu190_04_IDX_0H_Phase] = EV_MNEU24_HI(0);
	pSendData[Menu190_04_IDX_0L_Phase] = EV_MNEU24_LO(gChargeCtrl.ucBMSPhase[ucGunNo]);
	pSendData[Menu190_04_IDX_0H_Stop] = EV_MNEU24_HI(0);
	pSendData[Menu190_04_IDX_0L_Stop] = EV_MNEU24_LO(gChargeCtrl.ucStopReason[ucGunNo]);

	pSendData[Menu190_04_IDX_0H_EVCMD] = EV_MNEU24_HI(0);
	pSendData[Menu190_04_IDX_0L_EVCMD] = EV_MNEU24_LO(CMDAnalyse(gChargeCtrl.u16EVCMDGet[ucGunNo]));

	pSendData[Menu190_04_IDX_0H_BMSCMD] = EV_MNEU24_HI(0);
	pSendData[Menu190_04_IDX_0L_BMSCMD] = EV_MNEU24_LO(CMDAnalyse(gChargeCtrl.u16BMSCMDSend[ucGunNo]));

	wCRC = CRC_check(pSendData, Menu190_04_IDX_CRC_HI);
	pSendData[Menu190_04_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu190_04_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu190_04_IDX_MAX_SIG_NUM);
}

static void MdbCmd0X04Proc(UINT8* pRcvBuf)
{
	WORD wRegValue = 0;
	UCHAR ucGunNo;
	ucGunNo = pRcvBuf[MDB_RCV_BYTE_0_ADDR]-MDB_ADDR_VALUE;
	wRegValue = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));
	switch((wRegValue + 1))
	{
	case 6503:	
		RPT_UpMENU65_04_REG(ucGunNo);	
		break;
	case  MDBU_MENU93_04_AGING_REG:
		RPT_UpMENU93_04_REG(ucGunNo);	
		break;
	case  19004:
		RPT_UpMENU190_04_REG(ucGunNo);	
		break;
	default:
		break;
	}

}
static void RPT_UpMENU15_03_REG(UCHAR ucGunNo)
{
	INT16  i16TempVal = 0;
	float fMenu24Val  = 0;
#define Menu15_03_IDX_ADDR				0
#define Menu15_03_IDX_CMD				1
#define Menu15_03_IDX_BYTES_NUM		    2

#define Menu15_03_IDX_0H_ACTUAL_TEMP	3
#define Menu15_03_IDX_0L_ACTUAL_TEMP	4

#define Menu15_03_IDX_CRC_HI			5
#define Menu15_03_IDX_CRC_LO			6
#define Menu15_03_IDX_MAX_SIG_NUM		7

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu15_03_IDX_MAX_SIG_NUM);

	pSendData[Menu15_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu15_03_IDX_CMD]		= 0x03;
	pSendData[Menu15_03_IDX_BYTES_NUM]	= (Menu15_03_IDX_CRC_HI - Menu15_03_IDX_BYTES_NUM-1);

	if (gMeasureInfo.fSystemTemp  > 0)
	{
		pSendData[Menu15_03_IDX_0H_ACTUAL_TEMP] = EV_MNEU24_HI(gMeasureInfo.fSystemTemp  * 10);
		pSendData[Menu15_03_IDX_0L_ACTUAL_TEMP] = EV_MNEU24_LO(gMeasureInfo.fSystemTemp  * 10);
	}
	else
	{
		fMenu24Val = gMeasureInfo.fSystemTemp  * 10;
		i16TempVal = (INT16)(ABS(fMenu24Val));
		i16TempVal = (~(i16TempVal-1))&0xffff;
		pSendData[Menu15_03_IDX_0H_ACTUAL_TEMP] = EV_MNEU24_HI(i16TempVal);
		pSendData[Menu15_03_IDX_0L_ACTUAL_TEMP] = EV_MNEU24_LO(i16TempVal);
	}

	wCRC = CRC_check(pSendData, Menu15_03_IDX_CRC_HI);
	pSendData[Menu15_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu15_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu15_03_IDX_MAX_SIG_NUM);


}
static void RPT_UpMENU127_03_REG(UCHAR ucGunNo)
{
#define Menu127_03_IDX_ADDR				0
#define Menu127_03_IDX_CMD				1
#define Menu127_03_IDX_BYTES_NUM		2

#define Menu127_03_IDX_0H_FAULT			3
#define Menu127_03_IDX_0L_FAULT			4

#define Menu127_03_IDX_CRC_HI			5
#define Menu127_03_IDX_CRC_LO			6
#define Menu127_03_IDX_MAX_SIG_NUM		7

	UINT8 u8RectNum = 0;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu127_03_IDX_MAX_SIG_NUM);

	pSendData[Menu127_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu127_03_IDX_CMD]		= 0x03;
	pSendData[Menu127_03_IDX_BYTES_NUM]	= (Menu127_03_IDX_CRC_HI - Menu127_03_IDX_BYTES_NUM-1);
	
	pSendData[Menu127_03_IDX_0H_FAULT]	= EV_MNEU24_HI(gChargeCtrl.u16FaultStep[ucGunNo]);
	pSendData[Menu127_03_IDX_0L_FAULT]  = EV_MNEU24_LO(gChargeCtrl.u16FaultStep[ucGunNo]);

	wCRC = CRC_check(pSendData, Menu127_03_IDX_CRC_HI);
	pSendData[Menu127_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu127_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu127_03_IDX_MAX_SIG_NUM);

}

static void RPT_UpMENU75_03_AGING_SET_REG(UCHAR ucGunNo)
{
#define Menu75_03_IDX_ADDR				0
#define Menu75_03_IDX_CMD				1
#define Menu75_03_IDX_BYTES_NUM			2
#define Menu75_03_IDX_0H_REQUIRE_CURR	3
#define Menu75_03_IDX_0L_REQUIRE_CURR	4
#define Menu75_03_IDX_0H_REQUIRE_VOLT	5
#define Menu75_03_IDX_0L_REQUIRE_VOLT	6
#define Menu75_03_IDX_0H_TIME			7
#define Menu75_03_IDX_0L_TIME			8
#define Menu75_03_IDX_0H_LoadCurr		9
#define Menu75_03_IDX_0L_LoadCurr		10
#define Menu75_03_IDX_CRC_HI			11
#define Menu75_03_IDX_CRC_LO			12
#define Menu75_03_IDX_MAX_SIG_NUM		13

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu75_03_IDX_MAX_SIG_NUM);

	pSendData[Menu75_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu75_03_IDX_CMD]		= 0x03;
	pSendData[Menu75_03_IDX_BYTES_NUM]	= (Menu75_03_IDX_CRC_HI - Menu75_03_IDX_BYTES_NUM);

	pSendData[Menu75_03_IDX_0H_REQUIRE_CURR] = EV_MNEU24_HI(gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100);
	pSendData[Menu75_03_IDX_0L_REQUIRE_CURR] = EV_MNEU24_LO(gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100);
	pSendData[Menu75_03_IDX_0H_REQUIRE_VOLT] = EV_MNEU24_HI(gChargeCtrl.fUserSetVolt[ucGunNo] * 10);
	pSendData[Menu75_03_IDX_0L_REQUIRE_VOLT] = EV_MNEU24_LO(gChargeCtrl.fUserSetVolt[ucGunNo] * 10);
	pSendData[Menu75_03_IDX_0H_TIME]		 = EV_MNEU24_HI(gChargeCtrl.u16AgingTime[ucGunNo]/60);
	pSendData[Menu75_03_IDX_0L_TIME]		 = EV_MNEU24_LO(gChargeCtrl.u16AgingTime[ucGunNo]/60);
	pSendData[Menu75_03_IDX_0H_LoadCurr]		 = EV_MNEU24_HI(gChargeCtrl.fUserSetLoadCurr[ucGunNo]*100);
	pSendData[Menu75_03_IDX_0L_LoadCurr]		 = EV_MNEU24_LO(gChargeCtrl.fUserSetLoadCurr[ucGunNo]*100);

	wCRC = CRC_check(pSendData, Menu75_03_IDX_CRC_HI);
	pSendData[Menu75_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu75_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, Menu75_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU72_03_Version(UCHAR ucGunNo)
{
#define Menu72_03_IDX_ADDR				0
#define Menu72_03_IDX_CMD				1
#define Menu72_03_IDX_BYTES_NUM			2
#define Menu72_03_IDX_0H_Version		3
#define Menu72_03_IDX_0L_Version		4

#define Menu72_03_IDX_CRC_HI			5
#define Menu72_03_IDX_CRC_LO			6
#define Menu72_03_IDX_MAX_SIG_NUM		7

	WORD wCRC = 0;
	UINT16 u16temp;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu72_03_IDX_MAX_SIG_NUM);

	pSendData[Menu72_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu72_03_IDX_CMD]		= 0x03;
	pSendData[Menu72_03_IDX_BYTES_NUM]	= (Menu72_03_IDX_CRC_HI - Menu72_03_IDX_BYTES_NUM);

	u16temp = mVERHIGH*100+mVERLOW;	
  
	pSendData[Menu72_03_IDX_0H_Version] = EV_MNEU24_HI(u16temp);
	pSendData[Menu72_03_IDX_0L_Version] = EV_MNEU24_LO(u16temp);

	wCRC = CRC_check(pSendData, Menu72_03_IDX_CRC_HI);
	pSendData[Menu72_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu72_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, Menu72_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU65_03_REG(UCHAR ucGunNo)
{
#define Menu65_03_IDX_ADDR				0
#define Menu65_03_IDX_CMD				1
#define Menu65_03_IDX_BYTES_NUM		    2

#define Menu65_03_IDX_0H_ACTUAL_VOLT	3
#define Menu65_03_IDX_0L_ACTUAL_VOLT	4
#define Menu65_03_IDX_0H_ACTUAL_CURR	5
#define Menu65_03_IDX_0L_ACTUAL_CURR	6

#define Menu65_03_IDX_CRC_HI			7
#define Menu65_03_IDX_CRC_LO			8
#define Menu65_03_IDX_MAX_SIG_NUM		9

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	float fVolt,fCurr;
	memset(pSendData, 0, Menu65_03_IDX_MAX_SIG_NUM);

	pSendData[Menu65_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu65_03_IDX_CMD]		= 0x03;
	pSendData[Menu65_03_IDX_BYTES_NUM]	= (Menu65_03_IDX_CRC_HI - Menu65_03_IDX_BYTES_NUM-1);
	fVolt = GetBusData(ucGunNo,BUS_VOLT);
	pSendData[Menu65_03_IDX_0H_ACTUAL_VOLT] = EV_MNEU24_HI(fVolt  * 10);
	pSendData[Menu65_03_IDX_0L_ACTUAL_VOLT] = EV_MNEU24_LO(fVolt  * 10);
	fCurr = GetBusData(ucGunNo,BUS_CURR);
	pSendData[Menu65_03_IDX_0H_ACTUAL_CURR] = EV_MNEU24_HI(fCurr * 100);
	pSendData[Menu65_03_IDX_0L_ACTUAL_CURR] = EV_MNEU24_LO(fCurr * 100);

	wCRC = CRC_check(pSendData, Menu65_03_IDX_CRC_HI);
	pSendData[Menu65_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu65_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, Menu65_03_IDX_MAX_SIG_NUM);

}

static void SystemTime_Updata(void)
{
#define SystemTime_IDX_ADDR				0
#define SystemTime_IDX_CMD				1
#define SystemTime_IDX_BYTES_NUM		2


#define SystemTime_IDX_0H_YEAR			3	
#define SystemTime_IDX_0L_YEAR			4
#define SystemTime_IDX_1H_MONTH			5	
#define SystemTime_IDX_1L_MONTH			6
#define SystemTime_IDX_2H_DAY			7	
#define SystemTime_IDX_2L_DAY			8
#define SystemTime_IDX_3H_HOUR			9	
#define SystemTime_IDX_3L_HOUR			10
#define SystemTime_IDX_4H_MIN			11	
#define SystemTime_IDX_4L_MIN			12
#define SystemTime_IDX_5H_SECOND		13	
#define SystemTime_IDX_5L_SECOND		14

#define Clear_Record_IDX_6H				15	
#define Clear_Record_IDX_6L				16	
#define Record_START_IDX_7H				17	
#define Record_START_IDX_7L				18	
#define Record_END_IDX_8H				19	
#define Record_END_IDX_8L				20	
#define Copy_Record_IDX_9H				21	
#define Copy_Record_IDX_9L				22	


	//addr sig here!!
#define SystemTime_IDX_CRC_HI			23
#define SystemTime_IDX_CRC_LO			24
#define SystemTime_IDX_MAX_SIG_NUM		25

	WORD wCRC = 0;
	struct tm  tmNow;
	UCHAR* pSendData = gUart6EVSendData;
	gRTCData.fGetLocalTime(&tmNow);

	memset(pSendData, 0, SystemTime_IDX_MAX_SIG_NUM);

	pSendData[SystemTime_IDX_ADDR] = MDB_ADDR_VALUE;
	pSendData[SystemTime_IDX_CMD] = 0x03;
	pSendData[SystemTime_IDX_BYTES_NUM] = (SystemTime_IDX_CRC_HI - SystemTime_IDX_BYTES_NUM - 1);

	pSendData[SystemTime_IDX_0H_YEAR]   = EV_MNEU24_HI(tmNow.tm_year);
	pSendData[SystemTime_IDX_0L_YEAR]   = EV_MNEU24_LO(tmNow.tm_year);
	pSendData[SystemTime_IDX_1H_MONTH]  = EV_MNEU24_HI(tmNow.tm_mon);
	pSendData[SystemTime_IDX_1L_MONTH]  = EV_MNEU24_LO(tmNow.tm_mon);
	pSendData[SystemTime_IDX_2H_DAY]	= EV_MNEU24_HI(tmNow.tm_mday);
	pSendData[SystemTime_IDX_2L_DAY]	= EV_MNEU24_LO(tmNow.tm_mday);
	pSendData[SystemTime_IDX_3H_HOUR]   = EV_MNEU24_HI(tmNow.tm_hour);
	pSendData[SystemTime_IDX_3L_HOUR]   = EV_MNEU24_LO(tmNow.tm_hour);
	pSendData[SystemTime_IDX_4H_MIN]    = EV_MNEU24_HI(tmNow.tm_min);
	pSendData[SystemTime_IDX_4L_MIN]    = EV_MNEU24_LO(tmNow.tm_min);
	pSendData[SystemTime_IDX_5H_SECOND] = EV_MNEU24_HI(tmNow.tm_sec);
	pSendData[SystemTime_IDX_5L_SECOND] = EV_MNEU24_LO(tmNow.tm_sec);

	pSendData[Clear_Record_IDX_6H] = EV_MNEU24_HI(gFlashOptInfo.u8CleanRecordsFlag);
	pSendData[Clear_Record_IDX_6L] = EV_MNEU24_LO(gFlashOptInfo.u8CleanRecordsFlag);
	pSendData[Record_START_IDX_7H] = EV_MNEU24_HI(gs_UDiskData.u16RecordStartNum);
	pSendData[Record_START_IDX_7L] = EV_MNEU24_LO(gs_UDiskData.u16RecordStartNum);
	pSendData[Record_END_IDX_8H	] = EV_MNEU24_HI(gs_UDiskData.u16RecordEndNum);
	pSendData[Record_END_IDX_8L] = EV_MNEU24_LO(gs_UDiskData.u16RecordEndNum);
	pSendData[Copy_Record_IDX_9H] = EV_MNEU24_HI(gs_UDiskData.ucCopyRecordFlag);
	pSendData[Copy_Record_IDX_9L] = EV_MNEU24_LO(gs_UDiskData.ucCopyRecordFlag);

	wCRC = CRC_check(pSendData, SystemTime_IDX_CRC_HI);
	pSendData[SystemTime_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[SystemTime_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, SystemTime_IDX_MAX_SIG_NUM);
}


static void RPT_UpMENU67_03_REG(UCHAR ucGunNo)
{

#define Menu67_03_IDX_ADDR				0
#define Menu67_03_IDX_CMD				1
#define Menu67_03_IDX_BYTES_NUM		    2

#define Menu67_03_IDX_0H_Volt1			3
#define Menu67_03_IDX_0L_Volt1			4
#define Menu67_03_IDX_1H_Volt2			5
#define Menu67_03_IDX_1L_Volt2			6
#define Menu67_03_IDX_2H_Volt3			7
#define Menu67_03_IDX_2L_Volt3			8
#define Menu67_03_IDX_3H_ShuntCurr		9
#define Menu67_03_IDX_3L_ShuntCurr		10
#define Menu67_03_IDX_4H_BusVolt		11
#define Menu67_03_IDX_4L_BusVolt		12
#define Menu67_03_IDX_5H_BusVolt1		13
#define Menu67_03_IDX_5L_BusVolt1		14
#define Menu67_03_IDX_6H_BusVolt2		15
#define Menu67_03_IDX_6L_BusVolt2		16
#define Menu67_03_IDX_7H_LoadCurr		17
#define Menu67_03_IDX_7L_LoadCurr		18
#define Menu67_03_IDX_8H_LoadCurr1		19
#define Menu67_03_IDX_8L_LoadCurr1		20
#define Menu67_03_IDX_9H_LoadCurr2		21
#define Menu67_03_IDX_9L_LoadCurr2		22

#define Menu67_03_IDX_CRC_HI			23
#define Menu67_03_IDX_CRC_LO			24
#define Menu67_03_IDX_MAX_SIG_NUM		25

	WORD wCRC = 0;
	float ftemp = 0;
	UCHAR uctemp,i;
	UCHAR* pSendData = gUart6EVSendData;

	memset(pSendData, 0, Menu67_03_IDX_MAX_SIG_NUM);

	pSendData[Menu67_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu67_03_IDX_CMD]			= 0x03;
	pSendData[Menu67_03_IDX_BYTES_NUM]	= (Menu67_03_IDX_CRC_HI - Menu67_03_IDX_BYTES_NUM-1);

	if(gMeasureInfo.fBattVolt[ucGunNo]<0)
	{
		ftemp = 0;
	}
	else
	{
		ftemp = gMeasureInfo.fBattVolt[ucGunNo];
	}
	pSendData[Menu67_03_IDX_0H_Volt1] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_0L_Volt1] = EV_MNEU24_LO(ftemp*10);
	ftemp = gEvChargeInfo.fBattSet_A_Volt;
	pSendData[Menu67_03_IDX_1H_Volt2] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_1L_Volt2] = EV_MNEU24_LO(ftemp*10);
	ftemp = gEvChargeInfo.fBattSet_B_Volt;
	pSendData[Menu67_03_IDX_2H_Volt3] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_2L_Volt3] = EV_MNEU24_LO(ftemp*10);

	ftemp = gIMIUData[ucGunNo].fRealShuntCurr;
	pSendData[Menu67_03_IDX_3H_ShuntCurr] = EV_MNEU24_HI(ftemp);
	pSendData[Menu67_03_IDX_3L_ShuntCurr] = EV_MNEU24_LO(ftemp);
	ftemp = gIMIUData[ucGunNo].fBusVolt;
	pSendData[Menu67_03_IDX_4H_BusVolt] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_4L_BusVolt] = EV_MNEU24_LO(ftemp*10);
	ftemp = gIMIUData[ucGunNo].fAdjustVolt[0];
	pSendData[Menu67_03_IDX_5H_BusVolt1] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_5L_BusVolt1] = EV_MNEU24_LO(ftemp*10);
	ftemp = gIMIUData[ucGunNo].fAdjustVolt[1];
	pSendData[Menu67_03_IDX_6H_BusVolt2] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_6L_BusVolt2] = EV_MNEU24_LO(ftemp*10);
	ftemp = gIMIUData[ucGunNo].fCurr;
	pSendData[Menu67_03_IDX_7H_LoadCurr] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_7L_LoadCurr] = EV_MNEU24_LO(ftemp*10);
	ftemp = gIMIUData[ucGunNo].fAdjustCurr[0];
	pSendData[Menu67_03_IDX_8H_LoadCurr1] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_8L_LoadCurr1] = EV_MNEU24_LO(ftemp*10);
	ftemp = gIMIUData[ucGunNo].fAdjustCurr[1];
	pSendData[Menu67_03_IDX_9H_LoadCurr2] = EV_MNEU24_HI(ftemp*10);
	pSendData[Menu67_03_IDX_9L_LoadCurr2] = EV_MNEU24_LO(ftemp*10);

	wCRC = CRC_check(pSendData, Menu67_03_IDX_CRC_HI);
	pSendData[Menu67_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu67_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu67_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU79_03_AGING_REG(UCHAR ucGunNo)
{
#define Menu79_03_IDX_ADDR				0
#define Menu79_03_IDX_CMD				1
#define Menu79_03_IDX_BYTES_NUM		    2
#define Menu79_03_IDX_0H_AGING_STATE	3
#define Menu79_03_IDX_0L_AGING_STATE	4
#define Menu79_03_IDX_0H_REQUIRE_VOLT	5
#define Menu79_03_IDX_0L_REQUIRE_VOLT	6
#define Menu79_03_IDX_0H_REQUIRE_CURR	7
#define Menu79_03_IDX_0L_REQUIRE_CURR	8
#define Menu79_03_IDX_0H_TIME			9
#define Menu79_03_IDX_0L_TIME			10

#define Menu79_03_IDX_0H_ACTUAL_VOLT	11
#define Menu79_03_IDX_0L_ACTUAL_VOLT	12
#define Menu79_03_IDX_0H_ACTUAL_CURR	13
#define Menu79_03_IDX_0L_ACTUAL_CURR	14
#define Menu79_03_IDX_0H_REMAINDER_MIN	15
#define Menu79_03_IDX_0L_REMAINDER_MIN	16
#define Menu79_03_IDX_0H_REMAINDER_SEC	17
#define Menu79_03_IDX_0L_REMAINDER_SEC	18
#define Menu79_03_IDX_0H_CHARGE_TYPE	19
#define Menu79_03_IDX_0L_CHARGE_TYPE	20
#define Menu79_03_IDX_0H_Parrallel      21
#define Menu79_03_IDX_0L_Parrallel      22
	
#define Menu79_03_IDX_CRC_HI			23
#define Menu79_03_IDX_CRC_LO			24
#define Menu79_03_IDX_MAX_SIG_NUM		25

	WORD wCRC = 0;
	float ftemp;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData , 0, Menu79_03_IDX_MAX_SIG_NUM);

	pSendData[Menu79_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu79_03_IDX_CMD]		= 0x03;
	pSendData[Menu79_03_IDX_BYTES_NUM]	= (Menu79_03_IDX_CRC_HI - Menu79_03_IDX_BYTES_NUM-1);

	pSendData[Menu79_03_IDX_0H_AGING_STATE]= EV_MNEU24_HI(gChargeCtrl.ucAgingState[ucGunNo]);
	pSendData[Menu79_03_IDX_0L_AGING_STATE]= EV_MNEU24_LO(gChargeCtrl.ucAgingState[ucGunNo]);
	pSendData[Menu79_03_IDX_0H_REQUIRE_VOLT] = EV_MNEU24_HI(gChargeCtrl.fUserSetVolt[ucGunNo] * 10);
	pSendData[Menu79_03_IDX_0L_REQUIRE_VOLT] = EV_MNEU24_LO(gChargeCtrl.fUserSetVolt[ucGunNo] * 10);
	pSendData[Menu79_03_IDX_0H_REQUIRE_CURR] = EV_MNEU24_HI(gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100);
	pSendData[Menu79_03_IDX_0L_REQUIRE_CURR] = EV_MNEU24_LO(gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100);
	pSendData[Menu79_03_IDX_0H_TIME]		 = EV_MNEU24_HI(gChargeCtrl.u16AgingTime[ucGunNo]/60);
	pSendData[Menu79_03_IDX_0L_TIME]		 = EV_MNEU24_LO(gChargeCtrl.u16AgingTime[ucGunNo]/60);

	pSendData[Menu79_03_IDX_0H_ACTUAL_VOLT] = EV_MNEU24_HI(gChargeCtrl.fEVOutVolt[ucGunNo] * 10);
	pSendData[Menu79_03_IDX_0L_ACTUAL_VOLT] = EV_MNEU24_LO(gChargeCtrl.fEVOutVolt[ucGunNo] * 10);
	pSendData[Menu79_03_IDX_0H_ACTUAL_CURR] = EV_MNEU24_HI(gChargeCtrl.fEVOutCurr[ucGunNo] * 100);
	pSendData[Menu79_03_IDX_0L_ACTUAL_CURR] = EV_MNEU24_LO(gChargeCtrl.fEVOutCurr[ucGunNo] * 100);
	pSendData[Menu79_03_IDX_0H_REMAINDER_MIN]= EV_MNEU24_HI(gChargeCtrl.u16AgingCount[ucGunNo]/60);
	pSendData[Menu79_03_IDX_0L_REMAINDER_MIN]= EV_MNEU24_LO(gChargeCtrl.u16AgingCount[ucGunNo]/60);
	pSendData[Menu79_03_IDX_0H_REMAINDER_SEC]= EV_MNEU24_HI(gChargeCtrl.u16AgingCount[ucGunNo]%60);
	pSendData[Menu79_03_IDX_0L_REMAINDER_SEC]= EV_MNEU24_LO(gChargeCtrl.u16AgingCount[ucGunNo]%60);

	pSendData[Menu79_03_IDX_0H_CHARGE_TYPE]= EV_MNEU24_HI(gChargeCtrl.ucBMSProtocolType[ucGunNo]);
	pSendData[Menu79_03_IDX_0L_CHARGE_TYPE]= EV_MNEU24_LO(gChargeCtrl.ucBMSProtocolType[ucGunNo]);
	pSendData[Menu79_03_IDX_0H_Parrallel] = 0;
	pSendData[Menu79_03_IDX_0L_Parrallel] = gChargeCtrl.ucParallelMode[ucGunNo];




	wCRC = CRC_check(pSendData, Menu79_03_IDX_CRC_HI);
	pSendData[Menu79_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu79_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu79_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU81_03_REG(UCHAR ucGunNo)
{
#define Menu81_03_IDX_ADDR				0
#define Menu81_03_IDX_CMD				1
#define Menu81_03_IDX_BYTES_NUM		    2
#define Menu81_03_IDX_0H_AGING_STATE	3
#define Menu81_03_IDX_0L_AGING_STATE	4

#define Menu81_03_IDX_CRC_HI			5
#define Menu81_03_IDX_CRC_LO			6
#define Menu81_03_IDX_MAX_SIG_NUM		7

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData , 0, Menu81_03_IDX_MAX_SIG_NUM);

	pSendData[Menu81_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu81_03_IDX_CMD]		= 0x03;
	pSendData[Menu81_03_IDX_BYTES_NUM]	= (Menu81_03_IDX_CRC_HI - Menu81_03_IDX_BYTES_NUM-1);

	pSendData[Menu81_03_IDX_0H_AGING_STATE]= EV_MNEU24_HI(gChargeCtrl.ucAgingState[ucGunNo]);
	pSendData[Menu81_03_IDX_0L_AGING_STATE]= EV_MNEU24_LO(gChargeCtrl.ucAgingState[ucGunNo]);
	
	wCRC = CRC_check(pSendData, Menu81_03_IDX_CRC_HI);
	pSendData[Menu81_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu81_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu81_03_IDX_MAX_SIG_NUM);


}
static void RPT_UpMENU97_03_REG(UCHAR ucGunNo)
{
#define Menu97_03_IDX_ADDR				0
#define Menu97_03_IDX_CMD				1
#define Menu97_03_IDX_BYTES_NUM			2

#define Menu97_03_IDX_0H_BMS_TYPE		3
#define Menu97_03_IDX_0L_BMS_TYPE		4

#define Menu97_03_IDX_CRC_HI			5
#define Menu97_03_IDX_CRC_LO			6
#define Menu97_03_IDX_MAX_SIG_NUM		7

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	UCHAR uctemp;
	memset(pSendData, 0, Menu97_03_IDX_MAX_SIG_NUM);

	pSendData[Menu97_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu97_03_IDX_CMD]		= 0x03;
	pSendData[Menu97_03_IDX_BYTES_NUM]	= (Menu97_03_IDX_CRC_HI - Menu97_03_IDX_BYTES_NUM-1);

	GetCharSetData(SETDATA_GUN1_ucAuxPowerType_NUM+ucGunNo,&uctemp);
	pSendData[Menu97_03_IDX_0H_BMS_TYPE ]  = 0;
	pSendData[Menu97_03_IDX_0L_BMS_TYPE ]  = uctemp;

	wCRC = CRC_check(pSendData, Menu97_03_IDX_CRC_HI);
	pSendData[Menu97_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu97_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu97_03_IDX_MAX_SIG_NUM);
}
static void RPT_UpMENU99_03_REG(UCHAR ucGunNo)
{
#define Menu99_03_IDX_ADDR				0
#define Menu99_03_IDX_CMD				1
#define Menu99_03_IDX_BYTES_NUM			2

#define Menu99_03_IDX_0H_CHARGE_STATE	3
#define Menu99_03_IDX_0L_CHARGE_STATE	4

#define Menu99_03_IDX_CRC_HI			5
#define Menu99_03_IDX_CRC_LO			6
#define Menu99_03_IDX_MAX_SIG_NUM		7

	UINT8 i;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu99_03_IDX_MAX_SIG_NUM);

	pSendData[Menu99_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu99_03_IDX_CMD]		= 0x03;
	pSendData[Menu99_03_IDX_BYTES_NUM]	= (Menu99_03_IDX_CRC_HI - Menu99_03_IDX_BYTES_NUM-1);

	if(gChargeCtrl.u8ChargeState[ucGunNo]!=2)
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 6;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
	}
	else
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 0;
		gChargeCtrl.ucStepRTN[ucGunNo] = 1;
	}
	if(gChargeCtrl.u8ChargeState[ucGunNo] == 3)
	{
		//RED_LED_ON();
	}

	pSendData[Menu99_03_IDX_0H_CHARGE_STATE] = 0;
	pSendData[Menu99_03_IDX_0L_CHARGE_STATE] = gChargeCtrl.u8ChargeState[ucGunNo];  


	wCRC = CRC_check(pSendData, Menu99_03_IDX_CRC_HI);
	pSendData[Menu99_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu99_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu99_03_IDX_MAX_SIG_NUM);
}


static void RPT_UpMENU101_03_REG(UCHAR ucGunNo)
{
	INT16  i16TempVal = 0;
	float fMenu24Val  = 0;
#define Menu101_03_IDX_ADDR				0
#define Menu101_03_IDX_CMD				1
#define Menu101_03_IDX_BYTES_NUM		2

#define Menu101_03_IDX_0H_REQUIRE_VOLT	3
#define Menu101_03_IDX_0L_REQUIRE_VOLT	4
#define Menu101_03_IDX_0H_ACTUAL_VOLT	5
#define Menu101_03_IDX_0L_ACTUAL_VOLT	6
#define Menu101_03_IDX_0H_DELTA_VOLT	7
#define Menu101_03_IDX_0L_DELTA_VOLT	8

#define Menu101_03_IDX_0H_EV_VOLT		9
#define Menu101_03_IDX_0L_EV_VOLT		10
#define Menu101_03_IDX_0H_DELTA2_VOLT	11
#define Menu101_03_IDX_0L_DELTA2_VOLT	12
#define Menu101_03_IDX_0H_DELTA1		13
#define Menu101_03_IDX_0L_DELTA1		14

#define Menu101_03_IDX_0H_DELTA2		15
#define Menu101_03_IDX_0L_DELTA2		16

#define Menu101_03_IDX_0H_RTN			17
#define Menu101_03_IDX_0L_RTN			18


#define Menu101_03_IDX_CRC_HI			19
#define Menu101_03_IDX_CRC_LO			20
#define Menu101_03_IDX_MAX_SIG_NUM		21

	WORD wCRC = 0;
	float ftemp;
	UCHAR RTN[3];
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu101_03_IDX_MAX_SIG_NUM);

	pSendData[Menu101_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu101_03_IDX_CMD]		= 0x03;
	pSendData[Menu101_03_IDX_BYTES_NUM]	= (Menu101_03_IDX_CRC_HI - Menu101_03_IDX_BYTES_NUM-1);

	pSendData[Menu101_03_IDX_0H_REQUIRE_VOLT] = EV_MNEU24_HI(gChargeCtrl.fUserSetVolt[ucGunNo] * 10);
	pSendData[Menu101_03_IDX_0L_REQUIRE_VOLT] = EV_MNEU24_LO(gChargeCtrl.fUserSetVolt[ucGunNo] * 10);
	pSendData[Menu101_03_IDX_0H_ACTUAL_VOLT]  = EV_MNEU24_HI(gChargeCtrl.fActualVoltBack[ucGunNo] * 10);
	pSendData[Menu101_03_IDX_0L_ACTUAL_VOLT]  = EV_MNEU24_LO(gChargeCtrl.fActualVoltBack[ucGunNo] * 10);
	if((gChargeCtrl.fUserSetVolt[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo])> 0)
	{
		pSendData[Menu101_03_IDX_0H_DELTA_VOLT]	 = EV_MNEU24_HI((gChargeCtrl.fUserSetVolt[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo]) * 10);
		pSendData[Menu101_03_IDX_0L_DELTA_VOLT]	 = EV_MNEU24_LO((gChargeCtrl.fUserSetVolt[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo]) * 10);
	}
	else
	{
		fMenu24Val = (gChargeCtrl.fUserSetVolt[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo]) * 10;
		i16TempVal = (INT16)(ABS(fMenu24Val));
		i16TempVal = (~(i16TempVal-1))&0xffff;
		pSendData[Menu101_03_IDX_0H_DELTA_VOLT]	 = EV_MNEU24_HI(i16TempVal);
		pSendData[Menu101_03_IDX_0L_DELTA_VOLT]	 = EV_MNEU24_LO(i16TempVal);
	}


	pSendData[Menu101_03_IDX_0H_EV_VOLT] = EV_MNEU24_HI(gChargeCtrl.fEVOutVoltBack[ucGunNo] * 10);
	pSendData[Menu101_03_IDX_0L_EV_VOLT] = EV_MNEU24_LO(gChargeCtrl.fEVOutVoltBack[ucGunNo] * 10);

	if((gChargeCtrl.fEVOutVoltBack[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo])> 0)
	{
		pSendData[Menu101_03_IDX_0H_DELTA2_VOLT]	 = EV_MNEU24_HI((gChargeCtrl.fEVOutVoltBack[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo]) * 10);
		pSendData[Menu101_03_IDX_0L_DELTA2_VOLT]	 = EV_MNEU24_LO((gChargeCtrl.fEVOutVoltBack[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo]) * 10);
	}
	else
	{
		fMenu24Val = (gChargeCtrl.fEVOutVoltBack[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo]) * 10;
		i16TempVal = (INT16)(ABS(fMenu24Val));
		i16TempVal = (~(i16TempVal-1))&0xffff;
		pSendData[Menu101_03_IDX_0H_DELTA2_VOLT]	= EV_MNEU24_HI(i16TempVal);
		pSendData[Menu101_03_IDX_0L_DELTA2_VOLT]	= EV_MNEU24_LO(i16TempVal);
	}
	
	GetFloatSetData(SETDATA_GUN1_fVoltDetal_NUM+ucGunNo,&ftemp);
	if(ABS(gChargeCtrl.fUserSetVolt[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo])>ftemp)
	{
		pSendData[Menu101_03_IDX_0H_DELTA1] = EV_MNEU24_HI(0);
		pSendData[Menu101_03_IDX_0L_DELTA1] = EV_MNEU24_LO(2);
		RTN[0] =2;
	}
	else
	{
		pSendData[Menu101_03_IDX_0H_DELTA1] = EV_MNEU24_HI(0);
		pSendData[Menu101_03_IDX_0L_DELTA1] = EV_MNEU24_LO(1);
		RTN[0] =1;
	}

	if(ABS(gChargeCtrl.fEVOutVoltBack[ucGunNo]-gChargeCtrl.fActualVoltBack[ucGunNo])>ftemp)
	{
		pSendData[Menu101_03_IDX_0H_DELTA2] = EV_MNEU24_HI(0);
		pSendData[Menu101_03_IDX_0L_DELTA2] = EV_MNEU24_LO(2);
		RTN[1] =2;
	}
	else
	{
		pSendData[Menu101_03_IDX_0H_DELTA2] = EV_MNEU24_HI(0);
		pSendData[Menu101_03_IDX_0L_DELTA2] = EV_MNEU24_LO(1);
		RTN[1] =1;
	}
	if((RTN[0]!=1)||(RTN[1]!=1))
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 7;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
		RTN[2] = 2;
	}
	else
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 0;
		gChargeCtrl.ucStepRTN[ucGunNo] = 1;
		RTN[2] = 1;
	}
	pSendData[Menu101_03_IDX_0H_RTN] = EV_MNEU24_HI(0);
	pSendData[Menu101_03_IDX_0L_RTN] = EV_MNEU24_LO(RTN[2]);


	wCRC = CRC_check(pSendData, Menu101_03_IDX_CRC_HI);
	pSendData[Menu101_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu101_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu101_03_IDX_MAX_SIG_NUM);
}

static void RPT_UpMENU103_03_REG(UCHAR ucGunNo)
{
	INT16  i16TempVal = 0;
	float fMenu24Val  = 0;
#define Menu103_03_IDX_ADDR				0
#define Menu103_03_IDX_CMD				1
#define Menu103_03_IDX_BYTES_NUM		2

#define Menu103_03_IDX_0H_REQUIRE_CURR	3
#define Menu103_03_IDX_0L_REQUIRE_CURR	4
#define Menu103_03_IDX_0H_ACTUAL_CURR	5
#define Menu103_03_IDX_0L_ACTUAL_CURR	6
#define Menu103_03_IDX_0H_DELTA_CURR	7
#define Menu103_03_IDX_0L_DELTA_CURR	8

#define Menu103_03_IDX_0H_EV_CURR		9
#define Menu103_03_IDX_0L_EV_CURR		10
#define Menu103_03_IDX_0H_DELTA2_CURR	11
#define Menu103_03_IDX_0L_DELTA2_CURR	12
#define Menu103_03_IDX_0H_DELTA1		13
#define Menu103_03_IDX_0L_DELTA1		14

#define Menu103_03_IDX_0H_DELTA2		15
#define Menu103_03_IDX_0L_DELTA2		16

#define Menu103_03_IDX_0H_RTN			17
#define Menu103_03_IDX_0L_RTN			18


#define Menu103_03_IDX_CRC_HI			19
#define Menu103_03_IDX_CRC_LO			20
#define Menu103_03_IDX_MAX_SIG_NUM		21

	WORD wCRC = 0;
	float ftemp;
	UCHAR RTN[3];
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu103_03_IDX_MAX_SIG_NUM);

	pSendData[Menu103_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu103_03_IDX_CMD]		= 0x03;
	pSendData[Menu103_03_IDX_BYTES_NUM]	= (Menu103_03_IDX_CRC_HI - Menu103_03_IDX_BYTES_NUM-1);

	pSendData[Menu103_03_IDX_0H_REQUIRE_CURR] = EV_MNEU24_HI(gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100);
	pSendData[Menu103_03_IDX_0L_REQUIRE_CURR] = EV_MNEU24_LO(gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100);
	pSendData[Menu103_03_IDX_0H_ACTUAL_CURR]  = EV_MNEU24_HI(gChargeCtrl.fActualCurrBack[ucGunNo] * 100);
	pSendData[Menu103_03_IDX_0L_ACTUAL_CURR]  = EV_MNEU24_LO(gChargeCtrl.fActualCurrBack[ucGunNo] * 100);
	if((gChargeCtrl.fBMSRequireCurr[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo])> 0)
	{
		pSendData[Menu103_03_IDX_0H_DELTA_CURR]	 = EV_MNEU24_HI((gChargeCtrl.fBMSRequireCurr[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo]) * 100);
		pSendData[Menu103_03_IDX_0L_DELTA_CURR]	 = EV_MNEU24_LO((gChargeCtrl.fBMSRequireCurr[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo]) * 100);
	}
	else
	{
		fMenu24Val = (gChargeCtrl.fBMSRequireCurr[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo]) * 100;
		i16TempVal = (INT16)(ABS(fMenu24Val));
		i16TempVal = (~(i16TempVal-1))&0xffff;
		pSendData[Menu103_03_IDX_0H_DELTA_CURR]	 = EV_MNEU24_HI(i16TempVal);
		pSendData[Menu103_03_IDX_0L_DELTA_CURR]	 = EV_MNEU24_LO(i16TempVal);
	}


	pSendData[Menu103_03_IDX_0H_EV_CURR] = EV_MNEU24_HI(gChargeCtrl.fEVOutCurrBack[ucGunNo] * 100);
	pSendData[Menu103_03_IDX_0L_EV_CURR] = EV_MNEU24_LO(gChargeCtrl.fEVOutCurrBack[ucGunNo] * 100);

	if((gChargeCtrl.fEVOutCurrBack[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo])> 0)
	{
		pSendData[Menu103_03_IDX_0H_DELTA2_CURR]	 = EV_MNEU24_HI((gChargeCtrl.fEVOutCurrBack[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo]) * 100);
		pSendData[Menu103_03_IDX_0L_DELTA2_CURR]	 = EV_MNEU24_LO((gChargeCtrl.fEVOutCurrBack[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo]) * 100);
	}
	else
	{
		fMenu24Val = (gChargeCtrl.fEVOutCurrBack[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo]) * 100;
		i16TempVal = (INT16)(ABS(fMenu24Val));
		i16TempVal = (~(i16TempVal-1))&0xffff;
		pSendData[Menu103_03_IDX_0H_DELTA2_CURR]	= EV_MNEU24_HI(i16TempVal);
		pSendData[Menu103_03_IDX_0L_DELTA2_CURR]	= EV_MNEU24_LO(i16TempVal);
	}

	GetFloatSetData(SETDATA_GUN1_fCurrDetal_NUM+ucGunNo,&ftemp);
	if(ABS(gChargeCtrl.fBMSRequireCurr[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo])>ftemp)
	{
		pSendData[Menu103_03_IDX_0H_DELTA1] = EV_MNEU24_HI(0);
		pSendData[Menu103_03_IDX_0L_DELTA1] = EV_MNEU24_LO(2);
		RTN[0] =2;
	}
	else
	{
		pSendData[Menu103_03_IDX_0H_DELTA1] = EV_MNEU24_HI(0);
		pSendData[Menu103_03_IDX_0L_DELTA1] = EV_MNEU24_LO(1);
		RTN[0] =1;
	}

	if(ABS(gChargeCtrl.fEVOutCurrBack[ucGunNo]-gChargeCtrl.fActualCurrBack[ucGunNo])>ftemp)
	{
		pSendData[Menu103_03_IDX_0H_DELTA2] = EV_MNEU24_HI(0);
		pSendData[Menu103_03_IDX_0L_DELTA2] = EV_MNEU24_LO(2);
		RTN[1] =2;
	}
	else
	{
		pSendData[Menu103_03_IDX_0H_DELTA2] = EV_MNEU24_HI(0);
		pSendData[Menu103_03_IDX_0L_DELTA2] = EV_MNEU24_LO(1);
		RTN[1] =1;
	}
	if((RTN[0]!=1)||(RTN[1]!=1))
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 7;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
		RTN[2] = 2;
	}
	else
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 0;
		gChargeCtrl.ucStepRTN[ucGunNo] = 1;
		RTN[2] = 1;
	}
	pSendData[Menu103_03_IDX_0H_RTN] = EV_MNEU24_HI(0);
	pSendData[Menu103_03_IDX_0L_RTN] = EV_MNEU24_LO(RTN[2]);


	wCRC = CRC_check(pSendData, Menu103_03_IDX_CRC_HI);
	pSendData[Menu103_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu103_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu103_03_IDX_MAX_SIG_NUM);
}
static void RPT_UpMENU105_03_REG(UCHAR ucGunNo)
{
#define Menu105_03_IDX_ADDR				0
#define Menu105_03_IDX_CMD				1
#define Menu105_03_IDX_BYTES_NUM		2

#define Menu105_03_IDX_0H_BMSPOWER		3
#define Menu105_03_IDX_0L_BMSPOWER		4

#define Menu105_03_IDX_CRC_HI			5
#define Menu105_03_IDX_CRC_LO			6
#define Menu105_03_IDX_MAX_SIG_NUM		7

	UINT8 u8RectNum = 0;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu105_03_IDX_MAX_SIG_NUM);

	pSendData[Menu105_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu105_03_IDX_CMD]		= 0x03;
	pSendData[Menu105_03_IDX_BYTES_NUM]	= (Menu105_03_IDX_CRC_HI - Menu105_03_IDX_BYTES_NUM-1);
	if(gChargeCtrl.u8BMSPoweState[ucGunNo]==mBMS_POWER_12V_Fail)
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 9;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
	}
	else if(gChargeCtrl.u8BMSPoweState[ucGunNo]==mBMS_POWER_24V_Fail)
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 21;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
	}
	else if(gChargeCtrl.u8BMSPoweState[ucGunNo]==mBMS_POWER_NO_CHECK)
	{
		if(gChargeCtrl.u8BMSPowerStep[ucGunNo] == 1)
			gChargeCtrl.u16FaultStep[ucGunNo] = 9;
		else if(gChargeCtrl.u8BMSPowerStep[ucGunNo] == 2)
			gChargeCtrl.u16FaultStep[ucGunNo] = 21;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
	}
	else
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 0;
		gChargeCtrl.ucStepRTN[ucGunNo] = 1;
	}
	pSendData[Menu105_03_IDX_0H_BMSPOWER] = EV_MNEU24_HI(gChargeCtrl.u8BMSPoweState[ucGunNo]);
	pSendData[Menu105_03_IDX_0L_BMSPOWER] = EV_MNEU24_LO(gChargeCtrl.u8BMSPoweState[ucGunNo]);

	wCRC = CRC_check(pSendData, Menu105_03_IDX_CRC_HI);
	pSendData[Menu105_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu105_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu105_03_IDX_MAX_SIG_NUM);
}




static void RPT_UpMENU109_03_REG(UCHAR ucGunNo)
{
#define Menu109_03_IDX_ADDR				0
#define Menu109_03_IDX_CMD				1
#define Menu109_03_IDX_BYTES_NUM		2

#define Menu109_03_IDX_0H_INSUL_STATE	3
#define Menu109_03_IDX_0L_INSUL_STATE	4

#define Menu109_03_IDX_CRC_HI			5
#define Menu109_03_IDX_CRC_LO			6
#define Menu109_03_IDX_MAX_SIG_NUM		7

	UINT8 i;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu109_03_IDX_MAX_SIG_NUM);

	pSendData[Menu109_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;;
	pSendData[Menu109_03_IDX_CMD]			= 0x03;
	pSendData[Menu109_03_IDX_BYTES_NUM]	= (Menu109_03_IDX_CRC_HI - Menu109_03_IDX_BYTES_NUM-1);

	if(gChargeCtrl.u16InsulState[ucGunNo]!=1)
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 4;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
	}
	else
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 0;
		gChargeCtrl.ucStepRTN[ucGunNo] = 1;
	}
	if(gChargeCtrl.u16InsulState[ucGunNo] == 2)
	{
		SetIMEUDOState(IMEU_1_ID,OUTSIDE_GUN1_RED_CTRL+ucGunNo*2,TRUE);
	}

	pSendData[Menu109_03_IDX_0H_INSUL_STATE] = 0;
	pSendData[Menu109_03_IDX_0L_INSUL_STATE] = gChargeCtrl.u16InsulState[ucGunNo];  


	wCRC = CRC_check(pSendData, Menu109_03_IDX_CRC_HI);
	pSendData[Menu109_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu109_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu109_03_IDX_MAX_SIG_NUM);
}
static void RPT_UpMENU113_03_REG(UCHAR ucGunNo)
{
#define Menu113_03_IDX_ADDR				0
#define Menu113_03_IDX_CMD				1
#define Menu113_03_IDX_BYTES_NUM		2
#define Menu113_03_IDX_0H_AGING_STATE	3
#define Menu113_03_IDX_0L_AGING_STATE	4

#define Menu113_03_IDX_CRC_HI			5
#define Menu113_03_IDX_CRC_LO			6
#define Menu113_03_IDX_MAX_SIG_NUM		7

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, Menu113_03_IDX_MAX_SIG_NUM);

	pSendData[Menu113_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu113_03_IDX_CMD]		= 0x03;
	pSendData[Menu113_03_IDX_BYTES_NUM]	= (Menu113_03_IDX_CRC_HI - Menu113_03_IDX_BYTES_NUM-1);

	if((gChargeCtrl.u8ChargeState[ucGunNo]>0)&&(gChargeCtrl.ucVoltCurrOKCount[ucGunNo]>20))
	{
		pSendData[Menu113_03_IDX_0H_AGING_STATE] = EV_MNEU24_HI(0);
		pSendData[Menu113_03_IDX_0L_AGING_STATE] = EV_MNEU24_LO(1);
	}
	else
	{
		pSendData[Menu113_03_IDX_0H_AGING_STATE] = EV_MNEU24_HI(0);
		pSendData[Menu113_03_IDX_0L_AGING_STATE] = EV_MNEU24_LO(0);
	}

	wCRC = CRC_check(pSendData, Menu113_03_IDX_CRC_HI);
	pSendData[Menu113_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu113_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu113_03_IDX_MAX_SIG_NUM);
}
static void RPT_UpMENU115_03_REG(UCHAR ucGunNo)
{
#define Menu115_03_IDX_ADDR				0
#define Menu115_03_IDX_CMD				1
#define Menu115_03_IDX_BYTES_NUM		2

#define Menu115_03_IDX_0H_EMERGENCY		3
#define Menu115_03_IDX_0L_EMERGENCY		4

#define Menu115_03_IDX_CRC_HI			5
#define Menu115_03_IDX_CRC_LO			6
#define Menu115_03_IDX_MAX_SIG_NUM		7

	UINT8 i;
	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	float fVolt;
	memset(pSendData, 0, Menu115_03_IDX_MAX_SIG_NUM);
	pSendData[Menu115_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu115_03_IDX_CMD]		= 0x03;
	pSendData[Menu115_03_IDX_BYTES_NUM]	= (Menu115_03_IDX_CRC_HI - Menu115_03_IDX_BYTES_NUM-1);
	fVolt = GetBusData(ucGunNo,BUS_VOLT);
	if(fVolt  >150)
	{
		gChargeCtrl.ucEmergencyState = 1;
	}
	else
	{
		gChargeCtrl.ucEmergencyState = 0;
	}

	if(gChargeCtrl.ucEmergencyState!=0)
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 22;
		gChargeCtrl.ucStepRTN[ucGunNo] = 0;
	}
	else
	{
		gChargeCtrl.u16FaultStep[ucGunNo] = 0;
		gChargeCtrl.ucStepRTN[ucGunNo] = 1;
	}

	pSendData[Menu115_03_IDX_0H_EMERGENCY] = 0;
	pSendData[Menu115_03_IDX_0L_EMERGENCY] = gChargeCtrl.ucEmergencyState;  


	wCRC = CRC_check(pSendData, Menu115_03_IDX_CRC_HI);
	pSendData[Menu115_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu115_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, Menu115_03_IDX_MAX_SIG_NUM);
}

static void RPT_UpMENU68_03_REG(UCHAR ucGunNo)
{

#define Menu68_03_IDX_ADDR				0
#define Menu68_03_IDX_CMD				1
#define Menu68_03_IDX_BYTES_NUM		    2

#define Menu68_03_IDX_0H_Volt_Detal		3
#define Menu68_03_IDX_0L_Volt_Detal		4

#define Menu68_03_IDX_1H_Curr_Detal		5
#define Menu68_03_IDX_1L_Curr_Detal		6

#define Menu68_03_IDX_2H_VIN_1			7
#define Menu68_03_IDX_2L_VIN_2			8
#define Menu68_03_IDX_3H_VIN_3			9
#define Menu68_03_IDX_3L_VIN_4			10
#define Menu68_03_IDX_4H_VIN_5			11
#define Menu68_03_IDX_4L_VIN_6			12
#define Menu68_03_IDX_5H_VIN_7			13
#define Menu68_03_IDX_5L_VIN_8			14
#define Menu68_03_IDX_6H_VIN_9			15
#define Menu68_03_IDX_6L_VIN_10			16
#define Menu68_03_IDX_7H_VIN_11			17
#define Menu68_03_IDX_7L_VIN_12			18
#define Menu68_03_IDX_8H_VIN_13			19
#define Menu68_03_IDX_8L_VIN_14			20
#define Menu68_03_IDX_9H_VIN_15			21
#define Menu68_03_IDX_9L_VIN_16			22
#define Menu68_03_IDX_10H_VIN_17		23
#define Menu68_03_IDX_10L_VIN_18		24

#define Menu68_03_IDX_11H_PROTOCOL		25
#define Menu68_03_IDX_11L_PROTOCOL		26

#define Menu68_03_IDX_12H_BRO			27
#define Menu68_03_IDX_12L_BRO			28

#define Menu68_03_IDX_13H_BROTEST		29
#define Menu68_03_IDX_13L_BROTEST		30

#define Menu68_03_IDX_CRC_HI			31
#define Menu68_03_IDX_CRC_LO			32
#define Menu68_03_IDX_MAX_SIG_NUM		33

	WORD wCRC = 0;
	float ftemp = 200;
	UCHAR uctemp,i;
	UCHAR* pSendData = gUart6EVSendData;

	memset(pSendData, 0, Menu68_03_IDX_MAX_SIG_NUM);

	pSendData[Menu68_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu68_03_IDX_CMD]			= 0x03;
	pSendData[Menu68_03_IDX_BYTES_NUM]	= (Menu68_03_IDX_CRC_HI - Menu68_03_IDX_BYTES_NUM-1);

	GetFloatSetData(SETDATA_GUN1_fVoltDetal_NUM+ucGunNo,&ftemp);
	pSendData[Menu68_03_IDX_0H_Volt_Detal] = EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu68_03_IDX_0L_Volt_Detal] = EV_MNEU24_LO(ftemp * 10);

	GetFloatSetData(SETDATA_GUN1_fCurrDetal_NUM+ucGunNo,&ftemp);
	pSendData[Menu68_03_IDX_1H_Curr_Detal] = EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu68_03_IDX_1L_Curr_Detal] = EV_MNEU24_LO(ftemp * 10);

	for(i= 0;i<18;i++)
	{
		pSendData[Menu68_03_IDX_2L_VIN_2+i*2] = gChargeCtrl.ucVIN[ucGunNo][i*2];
		pSendData[Menu68_03_IDX_2H_VIN_1+i*2] = gChargeCtrl.ucVIN[ucGunNo][i*2+1];
	}
	//GetCharSetData(SETDATA_ucProtocolType_NUM,&uctemp);
	uctemp = 0;
	pSendData[Menu68_03_IDX_11H_PROTOCOL] = 0;
	pSendData[Menu68_03_IDX_11L_PROTOCOL] = uctemp;

	pSendData[Menu68_03_IDX_12H_BRO] = EV_MNEU24_HI(0);
	pSendData[Menu68_03_IDX_12L_BRO] = EV_MNEU24_LO(0);

	pSendData[Menu68_03_IDX_13H_BROTEST] = EV_MNEU24_HI(0);
	pSendData[Menu68_03_IDX_13L_BROTEST] = EV_MNEU24_LO(0);

	wCRC = CRC_check(pSendData, Menu68_03_IDX_CRC_HI);
	pSendData[Menu68_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu68_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu68_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU70_03_REG(UCHAR ucGunNo)
{

#define Menu70_03_IDX_ADDR				0
#define Menu70_03_IDX_CMD				1
#define Menu70_03_IDX_BYTES_NUM		    2

#define Menu70_03_IDX_0H_Start_Volt		3
#define Menu70_03_IDX_0L_Start_Volt		4

#define Menu70_03_IDX_1H_Max_Volt		5
#define Menu70_03_IDX_1L_Max_Volt		6

#define Menu70_03_IDX_2H_Prelimit_Max_Volt		7
#define Menu70_03_IDX_2L_Prelimit_Max_Volt		8

#define Menu70_03_IDX_3H_Prelimit_Max_Temp		9
#define Menu70_03_IDX_3L_Prelimit_Max_Temp		10

#define Menu70_03_IDX_4H_Cell_Max_Volt		11
#define Menu70_03_IDX_4L_Cell_Max_Volt		12

#define Menu70_03_IDX_5H_Cell_Max_Temp		13
#define Menu70_03_IDX_5L_Cell_Max_Temp		14

#define Menu70_03_IDX_6H_SOC		        15
#define Menu70_03_IDX_6L_SOC		        16
#define Menu70_03_IDX_7H_WAIT		        17
#define Menu70_03_IDX_7L_WAIT		        18
#define Menu70_03_IDX_8H_BMSPowerType		19
#define Menu70_03_IDX_8L_BMSPowerType		20
#define Menu70_03_IDX_9H_SOURCE				21
#define Menu70_03_IDX_9L_SOURCE				22
#define Menu70_03_IDX_10H_BOARD				23
#define Menu70_03_IDX_10L_BOARD				24

#define Menu70_03_IDX_CRC_HI			25
#define Menu70_03_IDX_CRC_LO			26
#define Menu70_03_IDX_MAX_SIG_NUM		27

	WORD wCRC = 0;
	float ftemp = 200;
	UCHAR uctemp;
	UCHAR* pSendData = gUart6EVSendData;

	memset(pSendData, 0, Menu70_03_IDX_MAX_SIG_NUM);

	pSendData[Menu70_03_IDX_ADDR]		= MDB_ADDR_VALUE+ucGunNo;
	pSendData[Menu70_03_IDX_CMD]		= 0x03;
	pSendData[Menu70_03_IDX_BYTES_NUM]	= (Menu70_03_IDX_CRC_HI - Menu70_03_IDX_BYTES_NUM-1);


    ftemp = gBmsCommDataInfo[ucGunNo].fBCP_BattStartVolt;
	pSendData[Menu70_03_IDX_0H_Start_Volt] = EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu70_03_IDX_0L_Start_Volt] = EV_MNEU24_LO(ftemp * 10);

	ftemp = gBmsCommDataInfo[ucGunNo].fBCP_MaxVolt;
	pSendData[Menu70_03_IDX_1H_Max_Volt] = EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu70_03_IDX_1L_Max_Volt] = EV_MNEU24_LO(ftemp * 10);

	ftemp = gBmsCommDataInfo[ucGunNo].fBCP_MaxPermitCellVolt ;
	pSendData[Menu70_03_IDX_2H_Prelimit_Max_Volt] = EV_MNEU24_HI(ftemp * 100);
	pSendData[Menu70_03_IDX_2L_Prelimit_Max_Volt] = EV_MNEU24_LO(ftemp * 100);


	ftemp = gBmsCommDataInfo[ucGunNo].fBCP_MaxPermitTemp;
	pSendData[Menu70_03_IDX_3H_Prelimit_Max_Temp] = EV_MNEU24_HI(ftemp);
	pSendData[Menu70_03_IDX_3L_Prelimit_Max_Temp] = EV_MNEU24_LO(ftemp);

	ftemp = gBmsCommDataInfo[ucGunNo].fBCS_MaxCellVolt;
	pSendData[Menu70_03_IDX_4H_Cell_Max_Volt] = EV_MNEU24_HI(ftemp * 100);
	pSendData[Menu70_03_IDX_4L_Cell_Max_Volt] = EV_MNEU24_LO(ftemp * 100);

	ftemp = gBmsCommDataInfo[ucGunNo].fBSM_MaxCellTemp;
	pSendData[Menu70_03_IDX_5H_Cell_Max_Temp] = EV_MNEU24_HI(ftemp);
	pSendData[Menu70_03_IDX_5L_Cell_Max_Temp] = EV_MNEU24_LO(ftemp);

	ftemp = gBmsCommDataInfo[ucGunNo].fBCP_BattStartSOC ;
	pSendData[Menu70_03_IDX_6H_SOC] = EV_MNEU24_HI(ftemp);
	pSendData[Menu70_03_IDX_6L_SOC] = EV_MNEU24_LO(ftemp);

	ftemp = gBmsCommDataInfo[ucGunNo].fBCP_MaxPermitCurr;
	pSendData[Menu70_03_IDX_7H_WAIT] =  EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu70_03_IDX_7L_WAIT] =  EV_MNEU24_LO(ftemp * 10);

	GetCharSetData(SETDATA_GUN1_ucAuxPowerType_NUM+ucGunNo,&uctemp);
	pSendData[Menu70_03_IDX_8H_BMSPowerType] =  0;
	pSendData[Menu70_03_IDX_8L_BMSPowerType] =  uctemp;
	GetCharSetData(SETDATA_ucLoadDataSource_NUM, &uctemp);
	pSendData[Menu70_03_IDX_9H_SOURCE] =  EV_MNEU24_HI(uctemp);
	pSendData[Menu70_03_IDX_9L_SOURCE] =  EV_MNEU24_LO(uctemp);
	GetCharSetData(SETDATA_ucDoCtrlBoard_NUM, &uctemp);
	pSendData[Menu70_03_IDX_10H_BOARD] =  EV_MNEU24_HI(uctemp);
	pSendData[Menu70_03_IDX_10L_BOARD] =  EV_MNEU24_LO(uctemp);

	wCRC = CRC_check(pSendData, Menu70_03_IDX_CRC_HI);
	pSendData[Menu70_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu70_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu70_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU190_03_REG(UCHAR ucGunNo)
{
#define Menu190_03_IDX_ADDR				0
#define Menu190_03_IDX_CMD				1
#define Menu190_03_IDX_BYTES_NUM		2

#define Menu190_03_IDX_0H_ACTUAL_VOLT	3
#define Menu190_03_IDX_0L_ACTUAL_VOLT	4
#define Menu190_03_IDX_0H_ACTUAL_CURR	5
#define Menu190_03_IDX_0L_ACTUAL_CURR	6
#define Menu190_03_IDX_0H_REQUIRE_VOLT	7
#define Menu190_03_IDX_0L_REQUIRE_VOLT	8
#define Menu190_03_IDX_0H_REQUIRE_CURR	9
#define Menu190_03_IDX_0L_REQUIRE_CURR	10
#define Menu190_03_IDX_0H_BMS_POWER	    11
#define Menu190_03_IDX_0L_BMS_POWER		12
#define Menu190_03_IDX_0H_CC2_VOLT	    13
#define Menu190_03_IDX_0L_CC2_VOLT		14
#define Menu190_03_IDX_0H_EVTYPE	    15
#define Menu190_03_IDX_0L_EVTYPE		16
#define Menu190_03_IDX_0H_LoadCurr	    17
#define Menu190_03_IDX_0L_LoadCurr		18
#define Menu190_03_IDX_0H_RectMode		19
#define Menu190_03_IDX_0L_RectMode		20
#define Menu190_03_IDX_0H_Parrallel	    21
#define Menu190_03_IDX_0L_Parrallel		22


#define Menu190_03_IDX_CRC_HI			23
#define Menu190_03_IDX_CRC_LO			24
#define Menu190_03_IDX_MAX_SIG_NUM		25

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	float ftemp;
	memset(gUart6EVSendData, 0, Menu190_03_IDX_MAX_SIG_NUM);

	pSendData[Menu190_03_IDX_ADDR]		= MDB_ADDR_VALUE +ucGunNo;
	pSendData[Menu190_03_IDX_CMD]		= 0x03;
	pSendData[Menu190_03_IDX_BYTES_NUM]	= (Menu190_03_IDX_CRC_HI - Menu190_03_IDX_BYTES_NUM-1);
	ftemp = gChargeCtrl.fEVOutVolt[ucGunNo] * 10;
	pSendData[Menu190_03_IDX_0H_ACTUAL_VOLT] = EV_MNEU24_HI(ftemp);
	pSendData[Menu190_03_IDX_0L_ACTUAL_VOLT] = EV_MNEU24_LO(ftemp);
	ftemp = gChargeCtrl.fEVOutCurr[ucGunNo] * 100;
	pSendData[Menu190_03_IDX_0H_ACTUAL_CURR] = EV_MNEU24_HI(ftemp);
	pSendData[Menu190_03_IDX_0L_ACTUAL_CURR] = EV_MNEU24_LO(ftemp);
	ftemp = gChargeCtrl.fUserSetVolt[ucGunNo] * 10;
	pSendData[Menu190_03_IDX_0H_REQUIRE_VOLT] = EV_MNEU24_HI(ftemp);
	pSendData[Menu190_03_IDX_0L_REQUIRE_VOLT] = EV_MNEU24_LO(ftemp);
	ftemp = gChargeCtrl.fBMSRequireCurr[ucGunNo] * 100;
	pSendData[Menu190_03_IDX_0H_REQUIRE_CURR] = EV_MNEU24_HI(ftemp);
	pSendData[Menu190_03_IDX_0L_REQUIRE_CURR] = EV_MNEU24_LO(ftemp);
	if(gMeasureInfo.fBattVolt[ucGunNo]<0)
		ftemp = 0;
	else
		ftemp = gMeasureInfo.fBattVolt[ucGunNo];
	pSendData[Menu190_03_IDX_0H_BMS_POWER] = EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu190_03_IDX_0L_BMS_POWER] = EV_MNEU24_LO(ftemp* 10);
	if(gMeasureInfo.fCC1Volt[ucGunNo]<0)
		ftemp = 0;
	else
		ftemp = gMeasureInfo.fCC1Volt[ucGunNo];
	pSendData[Menu190_03_IDX_0H_CC2_VOLT] = EV_MNEU24_HI(ftemp * 10);
	pSendData[Menu190_03_IDX_0L_CC2_VOLT] = EV_MNEU24_LO(ftemp * 10);
    pSendData[Menu190_03_IDX_0H_EVTYPE] = EV_MNEU24_HI(gChargeCtrl.ucBMSProtocolType[ucGunNo]);
    pSendData[Menu190_03_IDX_0L_EVTYPE] = EV_MNEU24_LO(gChargeCtrl.ucBMSProtocolType[ucGunNo]);
	ftemp = gChargeCtrl.fUserSetLoadCurr[ucGunNo] * 100;
	pSendData[Menu190_03_IDX_0H_LoadCurr] = EV_MNEU24_HI(ftemp);
	pSendData[Menu190_03_IDX_0L_LoadCurr] = EV_MNEU24_LO(ftemp);
	pSendData[Menu190_03_IDX_0H_RectMode] = 0;
	pSendData[Menu190_03_IDX_0L_RectMode] = gChargeCtrl.ucRectWorkMode[ucGunNo];
	pSendData[Menu190_03_IDX_0H_Parrallel] = 0;
	pSendData[Menu190_03_IDX_0L_Parrallel] = gChargeCtrl.ucParallelMode[ucGunNo];

	wCRC = CRC_check(pSendData, Menu190_03_IDX_CRC_HI);
	pSendData[Menu190_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu190_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu190_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU194_03_REG(UCHAR ucGunNo)
{
#define Menu194_03_IDX_ADDR				0
#define Menu194_03_IDX_CMD				1
#define Menu194_03_IDX_BYTES_NUM		2

#define Menu194_03_IDX_0H_EVTYPE	    3
#define Menu194_03_IDX_0L_EVTYPE		4
#define Menu194_03_IDX_1H_GUNNUM	    5
#define Menu194_03_IDX_1L_GUNNUM		6
	

#define Menu194_03_IDX_CRC_HI			7
#define Menu194_03_IDX_CRC_LO			8
#define Menu194_03_IDX_MAX_SIG_NUM		9

	WORD wCRC = 0;
	UCHAR* pSendData = gUart6EVSendData;
	float ftemp;
	memset(gUart6EVSendData, 0, Menu194_03_IDX_MAX_SIG_NUM);

	pSendData[Menu194_03_IDX_ADDR]		= MDB_ADDR_VALUE +ucGunNo;
	pSendData[Menu194_03_IDX_CMD]		= 0x03;
	pSendData[Menu194_03_IDX_BYTES_NUM]	= (Menu194_03_IDX_CRC_HI - Menu194_03_IDX_BYTES_NUM-1);
	
	pSendData[Menu194_03_IDX_0H_EVTYPE] = EV_MNEU24_HI(gChargeCtrl.ucBMSProtocolType[ucGunNo]);
	pSendData[Menu194_03_IDX_0L_EVTYPE] = EV_MNEU24_LO(gChargeCtrl.ucBMSProtocolType[ucGunNo]);
	pSendData[Menu194_03_IDX_1H_GUNNUM] = EV_MNEU24_HI(gChargeCtrl.ucTestGunNum[ucGunNo]);
	pSendData[Menu194_03_IDX_1L_GUNNUM] = EV_MNEU24_LO(gChargeCtrl.ucTestGunNum[ucGunNo]);
	


	wCRC = CRC_check(pSendData, Menu194_03_IDX_CRC_HI);
	pSendData[Menu194_03_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[Menu194_03_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);


	dv_Uart6.fSendData(pSendData, Menu194_03_IDX_MAX_SIG_NUM);

}
static void RPT_UpMENU202_03_REG(UCHAR ucGunNo)
{
#define SourceCtrl_IDX_ADDR					0
#define SourceCtrl_IDX_CMD					1
#define SourceCtrl_IDX_BYTES_NUM			2

#define SourceCtrl_Rect_OutputMode_NUM_0H	3
#define SourceCtrl_Rect_OutputMode_NUM_0L	4
#define SourceCtrl_Paralla_Mode_NUM_1H		5
#define SourceCtrl_Paralla_Mode_NUM_1L		6
#define SourceCtrl_Output_Volt_NUM_2H		7
#define SourceCtrl_Output_Volt_NUM_2L		8
#define SourceCtrl_Output_Curr_NUM_3H		9
#define SourceCtrl_Output_Curr_NUM_3L		10
#define SourceCtrl_Require_Volt_NUM_4H		11
#define SourceCtrl_Require_Volt_NUM_4L		12
#define SourceCtrl_Require_Curr_NUM_5H		13
#define SourceCtrl_Require_Curr_NUM_5L		14
#define SourceCtrl_CTRL_CMD_NUM_6H			15
#define SourceCtrl_CTRL_CMD_NUM_6L			16

#define SourceCtrl_IDX_CRC_HI				17
#define SourceCtrl_IDX_CRC_LO				18
#define IDXUp_SourceCtrl_MAX_SIG_NUM		19

	WORD	wCRC = 0,i;
	UINT32  u32temp;
	UINT16	u16temp,RTN;
	UCHAR	uctemp;
	float	ftemp;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, IDXUp_SourceCtrl_MAX_SIG_NUM);

	pSendData[SourceCtrl_IDX_ADDR] = MDB_ADDR_VALUE+ucGunNo;
	pSendData[SourceCtrl_IDX_CMD]  = 0x03;
	pSendData[SourceCtrl_IDX_BYTES_NUM] = (SourceCtrl_IDX_CRC_HI - SourceCtrl_IDX_BYTES_NUM- 1);

	pSendData[SourceCtrl_Rect_OutputMode_NUM_0H] = 0;
	pSendData[SourceCtrl_Rect_OutputMode_NUM_0L] = gChargeCtrl.ucRectWorkMode[ucGunNo];
	pSendData[SourceCtrl_Paralla_Mode_NUM_1H] = 0;
	pSendData[SourceCtrl_Paralla_Mode_NUM_1H] = gChargeCtrl.ucParallelMode[ucGunNo];
	ftemp = GetBusData(ucGunNo,BUS_VOLT);
	ftemp = ftemp*10;
	pSendData[SourceCtrl_Output_Volt_NUM_2H] = EV_MNEU24_HI(ftemp);
	pSendData[SourceCtrl_Output_Volt_NUM_2L] = EV_MNEU24_LO(ftemp);
	ftemp = GetBusData(ucGunNo,BUS_CURR);
	ftemp = ftemp*10;
	pSendData[SourceCtrl_Output_Curr_NUM_3H] = EV_MNEU24_HI(ftemp);
	pSendData[SourceCtrl_Output_Curr_NUM_3L] = EV_MNEU24_LO(ftemp);
	ftemp = gChargeCtrl.fBMSRequireVolt[ucGunNo]*10;
	pSendData[SourceCtrl_Require_Volt_NUM_4H] = EV_MNEU24_HI(ftemp);
	pSendData[SourceCtrl_Require_Volt_NUM_4L] = EV_MNEU24_LO(ftemp);
	ftemp = gChargeCtrl.fBMSRequireCurr[ucGunNo]*10;
	pSendData[SourceCtrl_Require_Curr_NUM_5H] = EV_MNEU24_HI(ftemp);
	pSendData[SourceCtrl_Require_Curr_NUM_5L] = EV_MNEU24_LO(ftemp);

	pSendData[SourceCtrl_CTRL_CMD_NUM_6H] = 0;
	pSendData[SourceCtrl_CTRL_CMD_NUM_6L] = gChargeCtrl.ucSourceCtrlCmd[ucGunNo];

	wCRC = CRC_check(pSendData, SourceCtrl_IDX_CRC_HI);		//这种计算方法反过来了！
	pSendData[SourceCtrl_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[SourceCtrl_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, IDXUp_SourceCtrl_MAX_SIG_NUM);

}
static void RPT_UpMENU210_03_REG(void)
{
#define RecordInfo_IDX_ADDR					0
#define RecordInfo_IDX_CMD					1
#define RecordInfo_IDX_BYTES_NUM			2

#define RecordInfo_INDEX_NUM_0H				3
#define RecordInfo_INDEX_NUM_0L				4
#define RecordInfo_TOTAL_NUM_1H				5
#define RecordInfo_TOTAL_NUM_1L				6
#define RecordInfo_SERIAL_NUM_2H			7
#define RecordInfo_SERIAL_NUM_2L			8
#define RecordInfo_START_TIME_YEAR_3H		29
#define RecordInfo_START_TIME_YEAR_3L		30
#define RecordInfo_START_TIME_MON_4H		31
#define RecordInfo_START_TIME_MON_4L		32
#define RecordInfo_START_TIME_DAY_5H		33
#define RecordInfo_START_TIME_DAY_5L		34
#define RecordInfo_START_TIME_HOUR_6H		35
#define RecordInfo_START_TIME_HOUR_6L		36
#define RecordInfo_START_TIME_MIN_7H		37
#define RecordInfo_START_TIME_MIN_7L		38
#define RecordInfo_START_TIME_SEC_8H		39
#define RecordInfo_START_TIME_SEC_8L		40

#define RecordInfo_END_TIME_YEAR_9H			41
#define RecordInfo_END_TIME_YEAR_9L			42
#define RecordInfo_END_TIME_MON_10H			43
#define RecordInfo_END_TIME_MON_10L			44
#define RecordInfo_END_TIME_DAY_11H			45
#define RecordInfo_END_TIME_DAY_11L			46
#define RecordInfo_END_TIME_HOUR_12H		47
#define RecordInfo_END_TIME_HOUR_12L		48
#define RecordInfo_END_TIME_MIN_13H			49
#define RecordInfo_END_TIME_MIN_13L			50
#define RecordInfo_END_TIME_SEC_14H			51
#define RecordInfo_END_TIME_SEC_14L			52

#define RecordInfo_REQ_VOLT_15H				53
#define RecordInfo_REQ_VOLT_15L				54
#define RecordInfo_REQ_CURR_16H				55
#define RecordInfo_REQ_CURR_16L				56
#define RecordInfo_REQ_TIME_17H				57
#define RecordInfo_REQ_TIME_17L				58

#define RecordInfo_REAL_VOLT_18H			59
#define RecordInfo_REAL_VOLT_18L			60
#define RecordInfo_REAL_CURR_19H			61
#define RecordInfo_REAL_CURR_19L			62
#define RecordInfo_REAL_TIME_20H			63
#define RecordInfo_REAL_TIME_20L			64

#define RecordInfo_RESULT_21H				65
#define RecordInfo_RESULT_21L				66

#define RecordInfo_GUN_TYPE_22H				67
#define RecordInfo_GUN_TYPE_22L				68
#define RecordInfo_GUN_LINK_VOLT_23H		69
#define RecordInfo_GUN_LINK_VOLT_23L		70
#define RecordInfo_BMS_POWER_24H			71
#define RecordInfo_BMS_POWER_24L			72
#define RecordInfo_GUN_NUM_25H				73
#define RecordInfo_GUN_NUM_25L				74

#define RecordInfo_IDX_CRC_HI				75
#define RecordInfo_IDX_CRC_LO				76
#define IDXUp_RecordInfo_MAX_SIG_NUM		77

	WORD	wCRC = 0,i;
	UINT32  u32temp;
	UINT16	u16temp,RTN;
	UCHAR	uctemp;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, IDXUp_RecordInfo_MAX_SIG_NUM);

	pSendData[RecordInfo_IDX_ADDR] = MDB_ADDR_VALUE;
	pSendData[RecordInfo_IDX_CMD]  = 0x03;
	pSendData[RecordInfo_IDX_BYTES_NUM] = (RecordInfo_IDX_CRC_HI - RecordInfo_IDX_BYTES_NUM- 1);


	pSendData[RecordInfo_INDEX_NUM_0H]	= EV_MNEU24_HI(gFlashOptInfo.u32ViewRecordNo);
	pSendData[RecordInfo_INDEX_NUM_0L]	= EV_MNEU24_LO(gFlashOptInfo.u32ViewRecordNo);
	pSendData[RecordInfo_TOTAL_NUM_1H]	= EV_MNEU24_HI(gChargeLogTable.u32TotalNum);
	pSendData[RecordInfo_TOTAL_NUM_1L]	= EV_MNEU24_LO(gChargeLogTable.u32TotalNum);
	if((gFlashOptInfo.u32ViewRecordNo > gChargeLogTable.u32TotalNum)&&(gChargeLogTable.u32TotalNum == 0))
	{
		for (i = RecordInfo_SERIAL_NUM_2H; i < RecordInfo_IDX_CRC_HI; i++)
		{
				pSendData[i] = 0;
		}
		gFlashOptInfo.u32ViewRecordNo = 0;
	}
	else 
	{
		if(gFlashOptInfo.u32ViewRecordNo > gChargeLogTable.u32TotalNum)
		{
			gFlashOptInfo.u32ViewRecordNo = 1;
		}
		else if(gFlashOptInfo.u32ViewRecordNo <=0)
		{
			gFlashOptInfo.u32ViewRecordNo = gChargeLogTable.u32TotalNum;
		}
		gFlashOptInfo.u32ViewPageIdx = FLASH_GetPageIdx(gFlashOptInfo.u32ViewRecordNo);
		FLASH_ReadRecords(gFlashOptInfo.u32ViewPageIdx*BYTES_NUM_PER_PAGE, &stViewChargeRecordFlash.u8GunType,sizeof(stViewChargeRecordFlash)/sizeof(UINT8));

		for(i=0;i<11;i++)
		{
			pSendData[RecordInfo_SERIAL_NUM_2L+i*2] = stViewChargeRecordFlash.u8EVSerialNum[i*2];
			pSendData[RecordInfo_SERIAL_NUM_2H+i*2] = stViewChargeRecordFlash.u8EVSerialNum[i*2+1];  
		}
		u16temp = stViewChargeRecordFlash.u8StartTime[0]*100+stViewChargeRecordFlash.u8StartTime[1];
		pSendData[RecordInfo_START_TIME_YEAR_3H] =  EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_START_TIME_YEAR_3L] =  EV_MNEU24_LO(u16temp);

		pSendData[RecordInfo_START_TIME_MON_4H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8StartTime[2]);
		pSendData[RecordInfo_START_TIME_MON_4L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8StartTime[2]);
		pSendData[RecordInfo_START_TIME_DAY_5H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8StartTime[3]);
		pSendData[RecordInfo_START_TIME_DAY_5L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8StartTime[3]);
		pSendData[RecordInfo_START_TIME_HOUR_6H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8StartTime[4]);
		pSendData[RecordInfo_START_TIME_HOUR_6L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8StartTime[4]);
		pSendData[RecordInfo_START_TIME_MIN_7H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8StartTime[5]);
		pSendData[RecordInfo_START_TIME_MIN_7L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8StartTime[5]);
		pSendData[RecordInfo_START_TIME_SEC_8H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8StartTime[6]);
		pSendData[RecordInfo_START_TIME_SEC_8L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8StartTime[6]);

		u16temp = stViewChargeRecordFlash.u8EndTime[0]*100+stViewChargeRecordFlash.u8EndTime[1];
		pSendData[RecordInfo_END_TIME_YEAR_9H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_END_TIME_YEAR_9L] = EV_MNEU24_LO(u16temp);

		pSendData[RecordInfo_END_TIME_MON_10H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8EndTime[2]);
		pSendData[RecordInfo_END_TIME_MON_10L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8EndTime[2]);
		pSendData[RecordInfo_END_TIME_DAY_11H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8EndTime[3]);
		pSendData[RecordInfo_END_TIME_DAY_11L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8EndTime[3]);
		pSendData[RecordInfo_END_TIME_HOUR_12H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8EndTime[4]);
		pSendData[RecordInfo_END_TIME_HOUR_12L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8EndTime[4]);
		pSendData[RecordInfo_END_TIME_MIN_13H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8EndTime[5]);
		pSendData[RecordInfo_END_TIME_MIN_13L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8EndTime[5]);
		pSendData[RecordInfo_END_TIME_SEC_14H] = EV_MNEU24_HI(stViewChargeRecordFlash.u8EndTime[6]);
		pSendData[RecordInfo_END_TIME_SEC_14L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8EndTime[6]);

		u16temp = (UINT16)(stViewChargeRecordFlash.fUserSetVolt*10);
		pSendData[RecordInfo_REQ_VOLT_15H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_REQ_VOLT_15L] = EV_MNEU24_LO(u16temp);
		u16temp = (UINT16)(stViewChargeRecordFlash.fUserSetCurr*10);
		pSendData[RecordInfo_REQ_CURR_16H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_REQ_CURR_16L] = EV_MNEU24_LO(u16temp);
		u16temp = (UINT16)(stViewChargeRecordFlash.u16UserSetTime/60);
		pSendData[RecordInfo_REQ_TIME_17H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_REQ_TIME_17L] = EV_MNEU24_LO(u16temp);
		
		u16temp = (UINT16)(stViewChargeRecordFlash.fMeasuerMaxVolt*10);
		pSendData[RecordInfo_REAL_VOLT_18H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_REAL_VOLT_18L] = EV_MNEU24_LO(u16temp);
		u16temp = (UINT16)(stViewChargeRecordFlash.fMeasuerMaxCurr*10);
		pSendData[RecordInfo_REAL_CURR_19H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_REAL_CURR_19L] = EV_MNEU24_LO(u16temp);
		u16temp = (UINT16)(stViewChargeRecordFlash.u16TestTime/60);
		pSendData[RecordInfo_REAL_TIME_20H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_REAL_TIME_20L] = EV_MNEU24_LO(u16temp);
		u16temp = (UINT16)(stViewChargeRecordFlash.u8TestResult);
		pSendData[RecordInfo_RESULT_21H	] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_RESULT_21L	] = EV_MNEU24_LO(u16temp);
	
		pSendData[RecordInfo_GUN_TYPE_22H] = EV_MNEU24_HI(0);
		pSendData[RecordInfo_GUN_TYPE_22L] = EV_MNEU24_LO(stViewChargeRecordFlash.u8GunType);
		u16temp = (UINT16)(stViewChargeRecordFlash.fCC2Volt*10);
		pSendData[RecordInfo_GUN_LINK_VOLT_23H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_GUN_LINK_VOLT_23L] = EV_MNEU24_LO(u16temp);
		u16temp = (UINT16)(stViewChargeRecordFlash.fBMSVolt*10);
		pSendData[RecordInfo_BMS_POWER_24H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_BMS_POWER_24L] = EV_MNEU24_LO(u16temp);
		u16temp = stViewChargeRecordFlash.u8GunNum;
		pSendData[RecordInfo_GUN_NUM_25H] = EV_MNEU24_HI(u16temp);
		pSendData[RecordInfo_GUN_NUM_25L] = EV_MNEU24_LO(u16temp);

	}
		
	

	wCRC = CRC_check(pSendData, RecordInfo_IDX_CRC_HI);		//这种计算方法反过来了！
	pSendData[RecordInfo_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[RecordInfo_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, IDXUp_RecordInfo_MAX_SIG_NUM);

}
static void RPT_UpMENU220_03_REG(void)
{
#define NetParaInfo_IDX_ADDR			0
#define NetParaInfo_IDX_CMD				1
#define NetParaInfo_IDX_BYTES_NUM		2

#define NetParaInfo_NetEanble_0H		3
#define NetParaInfo_NetEanble_0L		4
#define NetParaInfo_DHCP_1H				5
#define NetParaInfo_DHCP_1L				6
#define NetParaInfo_IP1_2H				7
#define NetParaInfo_IP1_2L				8
#define NetParaInfo_IP2_3H				9
#define NetParaInfo_IP2_3L				10
#define NetParaInfo_IP3_4H				11
#define NetParaInfo_IP3_4L				12
#define NetParaInfo_IP4_5H				13
#define NetParaInfo_IP4_5L				14
#define NetParaInfo_NetMask1_6H			15	
#define NetParaInfo_NetMask1_6L			16	
#define NetParaInfo_NetMask2_7H			17	
#define NetParaInfo_NetMask2_7L			18	
#define NetParaInfo_NetMask3_8H			19	
#define NetParaInfo_NetMask3_8L			20	
#define NetParaInfo_NetMask4_9H			21	
#define NetParaInfo_NetMask4_9L			22	
#define NetParaInfo_GateWay1_10H		23	
#define NetParaInfo_GateWay1_10L		24
#define NetParaInfo_GateWay2_11H		25
#define NetParaInfo_GateWay2_11L		26
#define NetParaInfo_GateWay3_12H		27
#define NetParaInfo_GateWay3_12L		28
#define NetParaInfo_GateWay4_13H		29
#define NetParaInfo_GateWay4_13L		30
#define NetParaInfo_Server1_14H			31
#define NetParaInfo_Server1_14L			32
#define NetParaInfo_Server2_15H			33
#define NetParaInfo_Server2_15L			34
#define NetParaInfo_Server3_16H			35
#define NetParaInfo_Server3_16L			36
#define NetParaInfo_Server4_17H			37
#define NetParaInfo_Server4_17L			38
#define NetParaInfo_ServerPort_18H		39
#define NetParaInfo_ServerPort_18L		40

#define NetParaInfo_IDX_CRC_HI			41
#define NetParaInfo_IDX_CRC_LO			42
#define IDXUp_NetParaInfo_MAX_SIG_NUM	43

	WORD	wCRC = 0,i;
	UINT32  u32temp;
	UINT16	u16temp,RTN;
	UCHAR	uctemp;
	UCHAR* pSendData = gUart6EVSendData;
	memset(pSendData, 0, IDXUp_NetParaInfo_MAX_SIG_NUM);

	pSendData[NetParaInfo_IDX_ADDR] = MDB_ADDR_VALUE;
	pSendData[NetParaInfo_IDX_CMD]  = 0x03;
	pSendData[NetParaInfo_IDX_BYTES_NUM] = (NetParaInfo_IDX_CRC_HI - NetParaInfo_IDX_BYTES_NUM- 1);

	GetCharSetData(SETDATA_ucNetEnable_NUM, &uctemp);
	pSendData[NetParaInfo_NetEanble_0H] = 0;
	pSendData[NetParaInfo_NetEanble_0L] = uctemp;

	GetCharSetData(SETDATA_ucDHCPEnable_NUM, &uctemp);
	pSendData[NetParaInfo_DHCP_1H] = 0;
	pSendData[NetParaInfo_DHCP_1L] = uctemp;

	for(i= 0;i<4;i++)
	{
		GetCharSetData(SETDATA_ucSelfIP_1_NUM+i, &uctemp);
		pSendData[NetParaInfo_IP1_2H+i*2] = 0; 
		pSendData[NetParaInfo_IP1_2L+i*2] = uctemp;
		GetCharSetData(SETDATA_ucSubMask_1_NUM+i, &uctemp);
		pSendData[NetParaInfo_NetMask1_6H+i*2] = 0;  
		pSendData[NetParaInfo_NetMask1_6L+i*2] = uctemp;
		GetCharSetData(SETDATA_ucGateWay_1_NUM+i, &uctemp);
		pSendData[NetParaInfo_GateWay1_10H+i*2] = 0;
		pSendData[NetParaInfo_GateWay1_10L+i*2] = uctemp;
		GetCharSetData(SETDATA_ucServerIP_1_NUM+i, &uctemp);
		pSendData[NetParaInfo_Server1_14H+i*2] = 0;
		pSendData[NetParaInfo_Server1_14L+i*2] = uctemp;
	}
	GetCharSetData(SETDATA_ucNetPort_H_NUM, &uctemp);	
	pSendData[NetParaInfo_ServerPort_18H] = uctemp;
	GetCharSetData(SETDATA_ucNetPort_L_NUM, &uctemp);	
	pSendData[NetParaInfo_ServerPort_18L] = uctemp;
		
	wCRC = CRC_check(pSendData, NetParaInfo_IDX_CRC_HI);		//这种计算方法反过来了！
	pSendData[NetParaInfo_IDX_CRC_HI] = EV_MNEU24_LO(wCRC);
	pSendData[NetParaInfo_IDX_CRC_LO] = EV_MNEU24_HI(wCRC);

	dv_Uart6.fSendData(pSendData, IDXUp_NetParaInfo_MAX_SIG_NUM);

}
static void MdbCmd0X03Proc(UINT8* pRcvBuf)
{
	WORD wRegValue = 0;
	UCHAR ucGunNo;
	ucGunNo = pRcvBuf[MDB_RCV_BYTE_0_ADDR]-MDB_ADDR_VALUE;
	if(ucGunNo>=ALL_GUN_NUM)
	{
		return;
	}
	wRegValue = (((WORD )pRcvBuf[MDB_RCV_BYTE_2_REG_ADDR_H] << 8) + ((WORD)pRcvBuf[MDB_RCV_BYTE_3_REG_ADDR_L] & 0xff));
	switch((wRegValue + 1))
	{
	case  5:
		RPT_UpMENU127_03_REG(ucGunNo);	
		break;
	case  6:
		RPT_UpMENU75_03_AGING_SET_REG(ucGunNo);	
		break;
	case  10:
		RPT_UpMENU72_03_Version(ucGunNo);	
		break;
	case  1500:
		RPT_UpMENU15_03_REG(ucGunNo);	
		break;
	case  6500:
		RPT_UpMENU65_03_REG(ucGunNo);	
		break;
	case  6600:
		SystemTime_Updata();
		break;
	case  6700:
		RPT_UpMENU67_03_REG(ucGunNo);	
		break;
	case  6800:
		RPT_UpMENU79_03_AGING_REG(ucGunNo);	
		break;
	case  6880:
		RPT_UpMENU68_03_REG(ucGunNo);	
		break;
	case  7000:
		RPT_UpMENU70_03_REG(ucGunNo);	
		break;
	case  8100:
		RPT_UpMENU81_03_REG(ucGunNo);	
		break;
	case 9700:
		RPT_UpMENU97_03_REG(ucGunNo);
		break;
	case 9900:	
		RPT_UpMENU99_03_REG(ucGunNo);	
		break;
	case  MDBU_MENU101_03_REG:
		RPT_UpMENU101_03_REG(ucGunNo);	
		break;
    case MDBU_MENU103_03_REG:
        RPT_UpMENU103_03_REG(ucGunNo);	
		break;
	case  MDBU_MENU105_03_REG:	
		RPT_UpMENU105_03_REG(ucGunNo);	
		break;
	case 10900:	
		RPT_UpMENU109_03_REG(ucGunNo);	
		break;
	case  11300:
		RPT_UpMENU113_03_REG(ucGunNo);	
		break;
   case  11500:
        RPT_UpMENU115_03_REG(ucGunNo);	
        break;
	case  19000:
		RPT_UpMENU190_03_REG(ucGunNo);	
		break;
	case  19400:
		RPT_UpMENU194_03_REG(ucGunNo);	
		break;
	case 20200:
		RPT_UpMENU202_03_REG(ucGunNo);	
		break;
	case 21001:
		RPT_UpMENU210_03_REG();	
		break;
	case 22000:
		RPT_UpMENU220_03_REG();	
		break;
	default:
		break;
	}
}
void TASK_Uart6Proc (void* pdata)
{
	UINT8 byCRCLo = 0;
	UINT8 byCRCHi = 0;
	UINT8 u8BufIdx = 0;
	UINT8  byCRCHiCal = 0x00;
	UINT8  byCRCLoCal = 0x00;
	UINT8 byFrameBytesNum = 0;
	WORD  wCRC = 0;
	UINT16 ResetCount = 0;
	dv_Uart6.fSetUart6BaudRate();			//19200 E 8 1
	dv_Uart6.fEnableUart6Rx();
	OSTimeDly(800);
	
	for(;;)
	{
		gSelfTest.fSetTaskID(Task_ID_Console);
		OSTimeDly(10);
		for (u8BufIdx = 0; u8BufIdx < RS232_UART6_RX_BUFF_NUM; u8BufIdx++)
		{
			if (TRUE == gUart6RecvBuff[u8BufIdx].uReceivFlag)
			{
				byFrameBytesNum = 0;
				if (UI_MODBUS_CMD_03_ID == gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD]
					||UI_MODBUS_CMD_05_ID == gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD]
					||UI_MODBUS_CMD_01_ID == gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD]
					||UI_MODBUS_CMD_02_ID == gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD]
					||UI_MODBUS_CMD_04_ID == gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD])
				{
					byFrameBytesNum = 8;
				}
				else if (UI_MODBUS_CMD_10_ID == gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD])
				{
					//6 寄存器总 字节数;	2 CRC;	+1 字节数自己加进来，见协议文本
					byFrameBytesNum = (MDB_RCV_BYTE6_10H_BYTE_NUM + 1) + 2 + gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE6_10H_BYTE_NUM];
				}
				else
				{
					memset(gUart6RecvBuff[u8BufIdx].uBuf,0,RS232_UART6_RX_MAX_NUM-1);
					continue;
				}

				if (byFrameBytesNum < RS232_UART6_RX_MAX_NUM)
				{
					//Received CRC
					byCRCLo = gUart6RecvBuff[u8BufIdx].uBuf[byFrameBytesNum - 1];
					byCRCHi = gUart6RecvBuff[u8BufIdx].uBuf[byFrameBytesNum - 2];
					wCRC = CRC_check(gUart6RecvBuff[u8BufIdx].uBuf, (byFrameBytesNum - 2));		//这种计算方法反过来了
					byCRCHiCal = 0;
					byCRCLoCal = 0;
					byCRCLoCal = 0xff&((wCRC&0xff00) >> 8);
					byCRCHiCal = wCRC & 0xff;
					//CRC is Error
					if ((byCRCHiCal != byCRCHi)
						||(byCRCLoCal != byCRCLo))
					{
						memset(gUart6RecvBuff[u8BufIdx].uBuf,0,RS232_UART6_RX_MAX_NUM-1);
					}
					else
					{
						ResetCount = 0;
						switch(gUart6RecvBuff[u8BufIdx].uBuf[MDB_RCV_BYTE_1_CMD])
						{
						case UI_MODBUS_CMD_03_ID:
							MdbCmd0X03Proc(gUart6RecvBuff[u8BufIdx].uBuf);
							break;

						case UI_MODBUS_CMD_04_ID:
							MdbCmd0X04Proc(gUart6RecvBuff[u8BufIdx].uBuf);
							break;

						case UI_MODBUS_CMD_05_ID:
							MdbCmd0X05Proc(gUart6RecvBuff[u8BufIdx].uBuf);
							break;
						case UI_MODBUS_CMD_10_ID:
							MdbCmd0X10Proc(gUart6RecvBuff[u8BufIdx].uBuf);
							break;

						default:
							break;
						}
					}
				}
				else
				{
					memset(gUart6RecvBuff[u8BufIdx].uBuf,0,RS232_UART6_RX_MAX_NUM-1);
				}
				gUart6RecvBuff[u8BufIdx].uReceivFlag = FALSE;
			}
		}//END FOR PROCESS 
		ResetCount++;
		if (ResetCount > 2000)		//X S无数据 复位Uart6
		{
			ResetCount = 0;
			dv_Uart6.fSetUart6BaudRate();			//InitUart6
			dv_Uart6.fEnableUart6Rx();									//使能收中断
			OSTimeDly(5);
		}
	}
}