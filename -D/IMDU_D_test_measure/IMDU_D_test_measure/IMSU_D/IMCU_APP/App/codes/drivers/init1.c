#include "main.h"
#include "EtherNet.h"
#include "ethcomm.h"
extern NOINIT __NET_TCP_INFO gNET_TCP_Info;

void InitHIHardware(void)
{
	//I2C
	dv_E2PROM.fReadE2PROM(0);
	Delay(5000);
	dv_E2PROM.fReadE2PROM(1);
	Delay(5000);
	dv_E2PROM.fReadE2PROM(2);
	Delay(5000);

	//rtc
	dv_RTC.fDvInitRTC();

	WTDOG_FEED();

	//if(dv_LCD.fLCDOnLine()!=mTRUE)
	//{
	//	dv_LCD.fSetnoLCD(TRUE);
	//}
	//dv_LCD.fInitLCD();
	//dv_LCD.fOpenGreenLED();
	//dv_LCD.fCloseRedLED();
	//dv_LCD.fCloseYellowLED();

	WTDOG_FEED();
	WTDOG_FEED();
	dv_Output.fCloseBeep();
	//dv_LCD.fControlLCD(IOC_SET_BL, LCD_ON | LCD_HALF_CONTRAST);/*开背光  0x60*/
	dv_CAN1.fCAN1BusInit();
	WTDOG_FEED();
}
void  InitOBJ(void)
{
	init_gs_Measure();
	init_gs_E2PROM();
	InitgCANComm();
	init_gs_SetData();
	//init_gs_Screen();
}

static void UpdateSetData(void)
{
	UINT16  i;
	UCHAR * pTest, E2PROMPageRead[20], E2PROMPageWrite[20];
	
	pTest = &gSetDataInfo.SetData.NameData.ucSysType;	/*设置数据开始地址*/
	for(i = 0; i < mSETDATADIV16; i++)
	{
		dv_E2PROM.fReadE2PROMPage(E2PROMPageRead, E2PROMCharData + i * 16, 16);
		if(CompareSetData(pTest, E2PROMPageRead, 16) == FALSE)
		{
			WriteE2PROMBuffer(pTest, E2PROMPageWrite, 16);
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData + i * 16, 16);
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData1 + i * 16, 16);
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData2 + i * 16, 16);
		}
		pTest += 16;
		WTDOG_FEED();
	}
}

static void initSetData1(void)
{
	SetData_TypeDef * pSetData;
	pSetData = &gSetDataInfo.SetData;
	pSetData->NameData.ucSysType = 0;	
	pSetData->NameData.ucSoftWareVersion[0] = mVERHIGH;
	pSetData->NameData.ucSoftWareVersion[1] = mVERLOW;
	pSetData->NameData.ucSetDataVersion[0] = mSETDATAVERHIGH ;
	pSetData->NameData.ucSetDataVersion[1] = mSETDATAVERLOW ;
	pSetData->NameData.ucLoadDataSource = FROM_RECT;
	pSetData->NameData.ucAuxPowerType[GUN_NO_A] = mBMS_POWER_BOTH;
	pSetData->NameData.ucAuxPowerType[GUN_NO_B] = mBMS_POWER_BOTH;
	pSetData->NameData.ucDoCtrlBoard = IMEU2_BOARD;
	pSetData->NameData.ucNetEnable = FALSE;						/*序号:9,网络使能*/	
	pSetData->NameData.ucDHCPEnable= FALSE;						/*序号:10,网络使能*/	
	pSetData->NameData.ucSelfIP[0] = 192;						/*序号:11-14,本地IP*/	
	pSetData->NameData.ucSelfIP[1] = 168;						/*序号:11-14,本地IP*/	
	pSetData->NameData.ucSelfIP[2] = 1;						/*序号:11-14,本地IP*/	
	pSetData->NameData.ucSelfIP[3] = 7;						/*序号:11-14,本地IP*/	
	pSetData->NameData.ucSubMask[0] = 255;						/*序号:15-18,子网掩码*/	
	pSetData->NameData.ucSubMask[1] = 255;						/*序号:15-18,子网掩码*/	
	pSetData->NameData.ucSubMask[2] = 255;						/*序号:15-18,子网掩码*/	
	pSetData->NameData.ucSubMask[3] = 0;						/*序号:15-18,子网掩码*/	
	pSetData->NameData.ucGateWay[0] = 192;						/*序号:11-14,网关*/	
	pSetData->NameData.ucGateWay[1] = 168;						/*序号:11-14,网关*/	
	pSetData->NameData.ucGateWay[2] = 1;						/*序号:11-14,网关*/	
	pSetData->NameData.ucGateWay[3] = 1;						/*序号:11-14,网关*/	

	pSetData->NameData.ucServerIP[0]= 0;					/*序号:23-26,服务器IP*/	
	pSetData->NameData.ucServerIP[1]= 0;						/*序号:23-26,服务器IP*/	
	pSetData->NameData.ucServerIP[2]= 0;						/*序号:23-26,服务器IP*/	
	pSetData->NameData.ucServerIP[3]= 0;					/*序号:23-26,服务器IP*/	
	pSetData->NameData.ucNetPort[0] = 0;						/*序号:27-28,网络端口*/	
	pSetData->NameData.ucNetPort[1] = 0;						/*序号:27-28,网络端口*/	
}


static void	initSetData2(void)
{
	UINT16 i;
	UCHAR* pTest;
	float ftemp;
	UCHAR	E2PROMPageRead[E2PROM_BUFFER_SIZE];				/*E2PROM连续读缓冲区*/
	SetData_TypeDef * pSetData;
	pSetData = &gSetDataInfo.SetData;

	pSetData->NameData.fBusVoltDelat[GUN_NO_A] = 1;
	pSetData->NameData.fBusVoltDelat[GUN_NO_B] = 1;
	pSetData->NameData.fBusCurrDelat[GUN_NO_A] = 1;
	pSetData->NameData.fBusCurrDelat[GUN_NO_B] = 1;
	
	if((dv_E2PROM.fReadE2PROM(E2PROMAdjFlag1) != 0x55) ||
		(dv_E2PROM.fReadE2PROM(E2PROMAdjFlag2) != 0xaa))				/*是否已校准*/
	{
		pSetData->NameData.fCC2_Coeff_A[GUN_NO_A] = 1;
		pSetData->NameData.fCC2_Coeff_A[GUN_NO_B] = 1;
		pSetData->NameData.fVolt_Coeff_A[GUN_NO_A] = 1;
		pSetData->NameData.fVolt_Coeff_A[GUN_NO_B] = 1;
		pSetData->NameData.fCC2_Coeff_B[GUN_NO_A] = 0;
		pSetData->NameData.fCC2_Coeff_B[GUN_NO_B] = 0;
		pSetData->NameData.fVolt_Coeff_B[GUN_NO_A] = 0;
		pSetData->NameData.fVolt_Coeff_B[GUN_NO_B] = 0;
		
	}
	else											/*已校准,读出E2PROM校准系数*/
	{
		pTest = (unsigned char *)(&pSetData->NameData.fCC2_Coeff_A[GUN_NO_A]);
		dv_E2PROM.fReadE2PROMPage(E2PROMPageRead, E2PROMFloatData + SETDATA_GUN1_fCC2_Coeff_A_NUM * 4, 80);
		for(i = 0; i < 80; i++)
		{
			*pTest++ = E2PROMPageRead[i];	/*写入RAM设置数据区*/
		}
		ftemp =1;
		for(i = SETDATA_GUN1_fCC2_Coeff_A_NUM ; i <= SETDATA_GUN2_fVolt_Coeff_A_NUM ; i++)
		{
			if((gSetDataInfo.SetData.ArrayData.CfgFloatData[i] < 0.5) ||
				(gSetDataInfo.SetData.ArrayData.CfgFloatData[i] > 1.5))
			{
				SetFloatSetData(i, &ftemp);
			}
		}
	}
}
static void	initSetData2Default(void);
static void	initSetData2Default(void)
{
	//UCHAR  i, 
        //UCHAR SysType;
	//UCHAR* pTest;
	//SetData_TypeDef * pSetData;
	//pSetData = &gSetDataInfo.SetData;

	///*??????????????????*/
	//pSetData->NameData.ACOverVolt[0] = 285;					/*??????????280*/
	//pSetData->NameData.ACOverVolt[1] = 285;					/*??????????280*/
	//pSetData->NameData.ACLowVolt[0] = 175;					/*?????????180*/
	//pSetData->NameData.ACLowVolt[1] = 80;					/*?????????80*/
	//pSetData->NameData.AC3POverVolt[0] = 493;
	//pSetData->NameData.AC3POverVolt[1] = 493;
	//pSetData->NameData.AC3PLowVolt[0] = 303;
	//pSetData->NameData.AC3PLowVolt[1] = 138;
	//pSetData->NameData.RectACCurrLimit = 30;
	//pSetData->NameData.RectCapLimit = 0.75;

	//pSetData->NameData.RectVoltageUp = 770;					/*???????????*/
	//pSetData->NameData.RectDefaultVoltage = 500;				/*??????????????*/

	//pSetData->NameData.RectTempUp = 85;				/*???????????*/
	//pSetData->NameData.RectPFCVoltage = 420;			/*PFC???????*/
	//pSetData->NameData.RectOverVoltResetTime = 300;			/*?????护???????(??)*/
	//pSetData->NameData.RectLoadStartTime = 4;			/*????????????(??)*/

	//pSetData->NameData.OperationTime = 0;			/*?????????????(????*/
	//pSetData->NameData.RectTime = 0;			/*??????????(????*/

	//pSetData->NameData.RectTimeAlarm = 8760;						/*???????????????(????1??*/
}



void	WriteE2PROMBuffer(UCHAR * pTest, UCHAR * pData, UCHAR len)
{
	UCHAR  i;

	for(i = 0;i < len;i++)
		*pData++ = *pTest++;		/*写缓冲区*/
}


static void	ReadE2PROMBuffer(UCHAR * pTest, UCHAR * pData, UCHAR len)
{
	UCHAR  i;
	for(i = 0;i < len;i++)
		*pTest++ = *pData++;		/*读缓冲区*/
}

static void  initRAM(void)
{
}


unsigned char	CompareSetData(unsigned char *pRAM1, unsigned char *pRAM2, UINT16 Length)
{
	UINT16 ii;
	for(ii = 0;ii < Length;ii++)
	{
		if(*pRAM1++ != *pRAM2++)
			return	FALSE;							/*两组数据不相同*/
	}
	return	TRUE;									/*两组数据相同*/
}


void	CopySetData(unsigned char *pRAM1, unsigned char *pRAM2, UINT16 Length)
{
	UINT16 ii;
  for(ii = 0;ii < Length;ii++)					/*循环拷贝*/
		*pRAM1++ = *pRAM2++;
}


static void ReadData(void)
{
	UINT16 ii;
	UCHAR  test1, test2, test3;
	UCHAR * pTest;
	UCHAR * t1;
	UCHAR * t2;

	//UCHAR EEPROM_RxBuffer1[8192+20];
	//UCHAR EEPROM_RxBuffer2[8192+20];

	t1 = &gNET_TCP_Info.u8ReadBuf[0].u8DataFlag;		//9k
	t2 = &gNET_TCP_Info.u8ReadBuf[6].u8DataFlag;		//9k
	
	UCHAR	E2PROMPageRead[E2PROM_BUFFER_SIZE];				/*E2PROM连续读缓冲区*/
	UCHAR	E2PROMPageWrite[E2PROM_BUFFER_SIZE];			/*E2PROM连续写缓冲区*/

	//read data from E2PROM
	pTest = &gSetDataInfo.SetData.NameData.ucSysType;			/*设置数据开始地址*/
	for(ii = 0;ii < mSETDATADIV16;ii++)
	{
		dv_E2PROM.fReadE2PROMPage(E2PROMPageRead, E2PROMCharData + ii * 16, 16);/*读E2PROM16字节*/
		ReadE2PROMBuffer(pTest, E2PROMPageRead, 16);
		pTest += 16;
		WTDOG_FEED();								/*喂狗*/
	}

	/*备份1*/
	pTest = t1;			/*设置数据开始地址*/
	for(ii = 0;ii < mSETDATADIV16;ii++)
	{
		dv_E2PROM.fReadE2PROMPage(E2PROMPageRead, E2PROMCharData1 + ii * 16, 16);/*读E2PROM16字节*/
		ReadE2PROMBuffer(pTest, E2PROMPageRead, 16);
		pTest += 16;
		WTDOG_FEED();								/*喂狗*/
	}

	/*备份2*/
	pTest = t2;			/*设置数据开始地址*/
	for(ii = 0;ii < mSETDATADIV16;ii++)
	{	
		dv_E2PROM.fReadE2PROMPage(E2PROMPageRead, E2PROMCharData2 + ii * 16, 16);/*读E2PROM16字节*/
		ReadE2PROMBuffer(pTest, E2PROMPageRead, 16);
		pTest += 16;
		WTDOG_FEED();								/*喂狗*/
	}

	/*比较设置数据*/
	/*和备份1比较*/
	test1 = CompareSetData(&gSetDataInfo.SetData.NameData.ucSysType, t1, sizeof(gSetDataInfo.SetData));
	WTDOG_FEED();									/*喂狗*/

	/*和备份2比较*/
	test2 = CompareSetData(&gSetDataInfo.SetData.NameData.ucSysType, t2, sizeof(gSetDataInfo.SetData));
	WTDOG_FEED();								/*喂狗*/

	/*备份1和备	份2比较*/
	test3 = CompareSetData(t1,	t2, sizeof(gSetDataInfo.SetData));
	WTDOG_FEED();									/*喂狗*/

	/*和备份1比较*/
	if((test1 == 0) && (test2 == 0) && (test3 == 0))/*1!=2!=3*/
	{
		//Trap2();								/*三组设置数据均不相同,重置设置数据*/
		/*三组设置数据均不相同,取第一组设置数据*/
		/*save data to E2PROM:配置数据备份2,3*/
		pTest = &gSetDataInfo.SetData.NameData.ucSysType;	/*设置数据开始地址*/
		for(ii = 0;ii < mSETDATADIV16;ii++)
		{
			WriteE2PROMBuffer(pTest, E2PROMPageWrite, 16);
			pTest += 16;
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData1 + ii * 16, 16);	/*写E2PROM16字节*/
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData2 + ii * 16, 16);	/*写E2PROM16字节*/
			WTDOG_FEED();							/*喂狗*/
		}
	}
	else if((test1 == 1) && (test2 == 0) && (test3 == 0))	/*1==2!=3*/
	{
		/*save data to E2PROM:设置数据3*/
		pTest = &gSetDataInfo.SetData.NameData.ucSysType;		/*设置数据开始地址*/
		for(ii = 0;ii < mSETDATADIV16;ii++)
		{
			WriteE2PROMBuffer(pTest, E2PROMPageWrite, 16);
			pTest += 16;
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData2 + ii * 16, 16);	/*写E2PROM16字节*/
			WTDOG_FEED();							/*喂狗*/
		}
	}
	else if((test1 == 0) && (test2 == 1) && (test3 == 0))	/*1==3!=2*/
	{
		/*save data to E2PROM:设置数据2*/
		pTest = &gSetDataInfo.SetData.NameData.ucSysType;		/*设置数据开始地址*/
		for(ii = 0;ii < mSETDATADIV16;ii++)
		{
			WriteE2PROMBuffer(pTest, E2PROMPageWrite, 16);
			pTest += 16;
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData1 + ii * 16, 16);	/*写E2PROM16字节*/
			WTDOG_FEED();							/*喂狗*/
		}
	}
	else if((test1 == 0) && (test2 == 0) && (test3 == 1))	/*2==3!=1*/
	{
		/*更正内存设置数据*/
		CopySetData(&gSetDataInfo.SetData.NameData.ucSysType, t1, sizeof(gSetDataInfo.SetData));
		WTDOG_FEED();								/*喂狗*/
		/*save data to E2PROM:设置数据1*/
		pTest = &gSetDataInfo.SetData.NameData.ucSysType;		/*设置数据开始地址*/
		for(ii = 0;ii < mSETDATADIV16;ii++)
		{
			WriteE2PROMBuffer(pTest, E2PROMPageWrite, 16);
			pTest += 16;
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData + ii * 16, 16);/*写E2PROM16字节*/
			WTDOG_FEED();							/*喂狗*/
		}
	}

	WTDOG_FEED();									/*喂狗*/
}

void start(void)
{
    /*有无LCD*/
    //if(dv_LCD.fGetnoLCD())
    //{
    //    return;
    //}
    //dv_Output.fCloseBeep();
    //gScreenType.fClearScreen();
    WTDOG_FEED();
    //dv_LCD.fControlLCD(IOC_SET_LCD_CURSOR, 0x44);
    //dv_LCD.fWriteLCD("Start...", DIRECT_NORMAL, 0);
    //dv_LCD.fControlLCD(IOC_SET_LCD_CURSOR, 0x64);
    //dv_LCD.fWriteLCD("??...", DIRECT_NORMAL, 0);
    WTDOG_FEED();
    return;
}


/*参数初始化*/
void  InitRunArgs(void)
{
	UINT8 u8Mac[8] = {0,0,0,0,0,0,0,0};
	//UINT8 u8Mac[8] = {0xAC,0x65,0xDD,0x30,0x01,0x8C,0,0};

	__XXX_UNION_VALUE unTempVal;
	UINT8 i;
	//UCHAR	ctemp;
	float	ftemp, fversion;
	UCHAR	SetFlag1, SetFlag2;
	UCHAR*	pTest;
	UINT16	ii, u16temp;
	UCHAR	E2PROMPageRead[E2PROM_BUFFER_SIZE];			/*E2PROM连续读缓冲区*/
	UCHAR	E2PROMPageWrite[E2PROM_BUFFER_SIZE];			/*E2PROM连续写缓冲区*/
	UCHAR   ucdata[3];
	ucdata[0] = 0;
	ucdata[0] = ucdata[0];

	//硬件版本
	dv_E2PROM.fReadE2PROMPage(gEvChargeInfo.u8HardwareVer, E2PROM_HW_VER_HIGH, 2);
	
	SetFlag1 = dv_E2PROM.fReadE2PROM(E2PROMSetFlag1);							/*读E2PROM设置标志*/
	SetFlag2 = dv_E2PROM.fReadE2PROM(E2PROMSetFlag2);
	if((SetFlag1 != 0xAA) || (SetFlag2 != 0x55))	
	{
		dv_E2PROM.fReadE2PROMPage(u8Mac, E2PROMMACADD, 6);
		for(i = 0; i < 6; i++)
		{
			unTempVal.chtemp[0] = u8Mac[i];
			ExSetSigInfo(E_TYPE_SYS, ID_u8Mac_1 + i, unTempVal,NULL, BY_E2PROM);
		}

		initSetData1();															/*初始化设置数据*/
		WTDOG_FEED();
		initSetData2();															/*初始化设置数据*/
		WTDOG_FEED();

		//save data to E2PROM
		pTest = &gSetDataInfo.SetData.NameData.ucSysType;					/*设置数据开始地址*/
		for(ii = 0;ii < mSETDATADIV16;ii++)								/*8192字节:设置数据*/
		{
			WriteE2PROMBuffer(pTest, E2PROMPageWrite, 16);
			pTest += 16;
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData + ii * 16, 16);		/*写E2PROM16字节*/
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData1 + ii * 16, 16);		/*写E2PROM16字节*/
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMCharData2 + ii * 16, 16);		/*写E2PROM16字节*/
			dv_E2PROM.fWriteE2PROMPage(E2PROMPageWrite, E2PROMDEFAULTCharData + ii * 16, 16);	/*写E2PROM16字节*/
			WTDOG_FEED();
		}

		dv_E2PROM.fWriteE2PROM(E2PROMNumber, 0);
		dv_E2PROM.fWriteE2PROM(E2PROMDEFAULTFlag1, 0x55);
		dv_E2PROM.fWriteE2PROM(E2PROMDEFAULTFlag2, 0xAA);
		dv_E2PROM.fWriteE2PROM(E2PROMSetFlag1, 0xAA);
		dv_E2PROM.fWriteE2PROM(E2PROMSetFlag2, 0x55);
	}
	else										/*已设置*/
	{
		SetFlag1 = dv_E2PROM.fReadE2PROM(E2PROMDEFAULTFlag1);
		SetFlag2 = dv_E2PROM.fReadE2PROM(E2PROMDEFAULTFlag2);
		if((SetFlag1 != 0x55) || (SetFlag2 != 0xAA))				//恢复默认值
		{
			//read data from E2PROM
			pTest = &gSetDataInfo.SetData.NameData.ucSysType;			/*设置数据开始地址*/
			for(ii = 0;ii < mSETDATADIV16;ii++)
			{
				dv_E2PROM.fReadE2PROMPage(E2PROMPageRead, E2PROMDEFAULTCharData + ii * 16, 16);/*读E2PROM16字节*/  	
				ReadE2PROMBuffer(pTest, E2PROMPageRead, 16);
				dv_E2PROM.fWriteE2PROMPage(E2PROMPageRead, E2PROMCharData + ii * 16, 16);
				dv_E2PROM.fWriteE2PROMPage(E2PROMPageRead, E2PROMCharData1 + ii * 16, 16);
				dv_E2PROM.fWriteE2PROMPage(E2PROMPageRead, E2PROMCharData2 + ii * 16, 16);
				pTest += 16;
				WTDOG_FEED();								
			}
			dv_E2PROM.fWriteE2PROM(E2PROMDEFAULTFlag1, 0x55);
			dv_E2PROM.fWriteE2PROM(E2PROMDEFAULTFlag2, 0xAA);
		}

		/*dv_E2PROM.fWriteE2PROMPage(u8Mac, E2PROMCharData + ADDR_u8Mac_1, 6);
		dv_E2PROM.fWriteE2PROMPage(u8Mac, E2PROMCharData1 + ADDR_u8Mac_1, 6);
		dv_E2PROM.fWriteE2PROMPage(u8Mac, E2PROMCharData2 + ADDR_u8Mac_1, 6);*/
	
		//read data from E2PROM
		ReadData();							/*从E2PROM中读取数据*/
		WTDOG_FEED();
		
		GetCharSetData(SETDATA_ucSetDataVersion_H_NUM, &ucdata[0]);
		GetCharSetData(SETDATA_ucSetDataVersion_L_NUM, &ucdata[1]);
		ftemp=((float) (ucdata[0]*100+ucdata[1]))/100;
		fversion = (float)mSETDATAVERHIGH + (float)mSETDATAVERLOW / 100;

		if(ftemp < (1.06 - 0.002))
		{
			//UpdateSetData();
		}
		if(ucdata[0]!=mSETDATAVERHIGH) 
		{
			ucdata[0]=mSETDATAVERHIGH;
			SetCharSetData(SETDATA_ucSetDataVersion_H_NUM , &ucdata[0]);
		}
		if(ucdata[1]!=mSETDATAVERLOW) 
		{
			ucdata[1]=mSETDATAVERLOW	;
			SetCharSetData(SETDATA_ucSetDataVersion_L_NUM , &ucdata[1]);
		}

		GetCharSetData(SETDATA_ucSoftWareVersion_H_NUM , &ucdata[0]);
		GetCharSetData(SETDATA_ucSoftWareVersion_L_NUM , &ucdata[1]);
		
		if(ucdata[0]!=mVERHIGH) 
		{
			ucdata[0]=mVERHIGH;
			SetCharSetData(SETDATA_ucSoftWareVersion_H_NUM , &ucdata[0]);
		}
		if(ucdata[1]!=mVERLOW) 
		{
			ucdata[1]=mVERLOW	;
			SetCharSetData(SETDATA_ucSoftWareVersion_L_NUM , &ucdata[1]);
		}
	}

	WTDOG_FEED();

	initRAM();

	WTDOG_FEED();

	gRTCData.fDataInitRTC();

	WTDOG_FEED();
}
