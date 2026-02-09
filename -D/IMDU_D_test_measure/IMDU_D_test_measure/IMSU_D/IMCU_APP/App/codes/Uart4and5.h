

#define SendDataCMD				0
#define GetDataCMD				1

#define GUN_CONNECT		0x01
#define GUN_DISCONNECT	0x00

#define YDN23_IMIU_ADDR	180
#define YDN23_IMEU_ADDR	184

#define YDN23_SOI_VAL	0x7e
#define YDN23_EOI_VAL	0x0d
#define YDN23_VER_VAL	0x21

#define YDN23_Data_PST	13

#define YDN23_LEN_EXCEPT_DATA	18


#define YDN23_RTN_OK  0x00

#define YDN23_ProtocolVersion1 0x20
#define YDN23_ProtocolVersion2 0x21

#define NO_ANY_CMD				0
#define IMIU_ReadVoltCurr_CMD	0x41
#define IMIU_PutRes_CMD			0x45
#define IMIU_GetCoeff_CMD		0x46
#define IMIU_SetCoeff_CMD		0x48
#define IMIU_TestMode_CMD		0x03

#define IMIU_PutResN_ID			0x91
#define IMIU_PutResP_ID			0x92
#define IMIU_SetShuntCurr_ID	0x82
#define IMIU_SetCurrCoeff_ID	0x83
#define IMIU_SetVoltCoeff_ID	0x84

#define IMEU1_DO_CTRL_CMD	0x48
#define IMEU2_DO_CTRL_CMD	0x45

#define IMEU1_DO_CTRL_CMD_TYPE	0x85
#define IMEU2_DO_CTRL_CMD_TYPE	0x72

#define IMIU_CID1	0x60
#define IMEU_CID1	0x70
#define IMIU_TestMode_CID1	0xE1

#define Data_HI(w)  ((UINT8)((UINT16)(w) >> 8))
#define Data_LO(w)  ((UINT8)((UINT16)(w) &0xff))

 enum OUTSIDE_DO_CTRL
{
	OUTSIDE_GUN1_GREEN_CTRL = 0,
	OUTSIDE_GUN1_RED_CTRL = 1,
	OUTSIDE_GUN2_GREEN_CTRL = 2,
	OUTSIDE_GUN2_RED_CTRL = 3,
	OUTSIDE_AC_GREEN_CTRL = 4,
	OUTSIDE_AC_RED_CTRL = 5,
	OUTSIDE_GUN1_CCS1_2_LOCK	=6,
	OUTSIDE_GUN2_CCS1_AC_LOCK	=7,
	OUTSIDE_DC_AC_SWICH	=8,
	OUTSIDE_RESERVE = 9,
	OUTSIDE_AC_INPUT = 10,
	ALL_DO_CTRL_CMD_ID,
};

typedef struct tagIMEUData
{
	UCHAR ucDoCtrlState[ALL_DO_CTRL_CMD_ID];
	UCHAR ucDoCtrlFlag[ALL_DO_CTRL_CMD_ID];
	UCHAR ucDOMactchDeviceNum[ALL_DEVICE_ID][ALL_DO_CTRL_CMD_ID];
}IMEUData_TypeDef;
 enum UARTCMDID
{
	IMIU_ReadVoltCurr_COUNT_ID = 0,
	ALL_UART4_5_CMD_ID,
};
typedef struct tagUart4_5Comm
{
	UINT16 u16CMDCount[ALL_UART4_5_CMD_ID];
	UINT8  u8SendCmdFlag;
	UINT8  u8SendDataDelay;
	UINT8 u8GetDeviceAddress;

}Uart4_5_Comm_TypeDef;


typedef struct tagIMIUData
{
	float fBusVolt;
	float fCurr;

	UCHAR ucSetShuntCurr;
	UCHAR ucGetShuntCurr;
	float fSetShuntCurr;
	float fRealShuntCurr;

	UCHAR ucAdjustVoltStep;
	float fAdjustVolt[2];
	float fReadVolt[2];
	float fVoltCoeff[2]; //需要设置的系数
	float fRealVoltCoeff[2];//当前IMIU系数

	UCHAR ucAdjustCurrStep;
	float fAdjustCurr[2];
	float fReadCurr[2];
	float fCurrCoeff[2];//需要设置的系数
	float fRealCurrCoeff[2];//当前IMIU系数

	UCHAR ucSetTestMode;   //开机进入测试模式
	UCHAR ucPutResistance;    //0不投入，1投入   0x10 下发不投入   0x11 投入已下发

}IMIUData_TypeDef;



extern  IMIUData_TypeDef gIMIUData[ALL_GUN_NUM];
extern  IMEUData_TypeDef  gIMEUData[ALL_IMEU_NUM];
