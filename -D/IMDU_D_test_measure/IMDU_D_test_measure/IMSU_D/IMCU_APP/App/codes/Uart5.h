
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

#define IMEU_DO_CTRL_CMD	0x48

#define IMEU_DO_CTRL_CMD_TYPE	0x85

#define IMIU_CID1	0x60
#define IMEU_CID1	0x70




typedef struct tagIMIUData
{
	float fBusVolt;
	float fCurr;
}IMIUData_TypeDef;


enum UARTCMDID
{
      IMIU_ReadVoltCurr_COUNT_ID = 0,
      ALL_CMD_ID,
};

typedef struct tagUart5Comm
{
	UINT16 u16CMDCount[ALL_CMD_ID];
	UINT8  u8SendCmdFlag;
	UINT8  u8GetDataDelay;
	UINT8 u8GetDeviceAddress;

}Uart5Comm_TypeDef;
extern  IMIUData_TypeDef gIMIUData[ALL_GUN_NUM];
extern  IMEUData_TypeDef  gIMEUData[ALL_IMEU_NUM];
#endif