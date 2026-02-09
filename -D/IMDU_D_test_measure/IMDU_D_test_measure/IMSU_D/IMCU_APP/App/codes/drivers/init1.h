
#define LCD_ON              0x40
#define LCD_OFF             (~LCDON)
#define LCD_HALF_CONTRAST   39

#define E2PROM_BUFFER_SIZE  200

#define OFFSET_TIMEZONE   12
#define CHINESE_TIMEZONE  8

//int prio 0-15,max:15,0:no int,
#define UART6_INT_PRIO  7
#define TIM0_INT_PRIO   3   //tast mng
//#define RTC_INT_PRIO    5
#define XTI_INT_PRIO    6

/************************************************/
/*模拟量采集的通道地址（板上的硬件选通地址）	  */
/************************************************/
//见bsp_adc.h



#define	mSETDATADIV16		(sizeof(gSetDataInfo.SetData) / 16)	/*8192/16=512*/
#define	mPHONELEN		sizeof(gSetDataInfo.SetData.NameData.PhoneNum1)
#define	mPASSLEN		sizeof(gSetDataInfo.SetData.NameData.PassWord1)
#define	mSERIALLEN		sizeof(gSetDataInfo.SetData.NameData.SysSerialNum)
#define	mDATELEN		7
#define	mBATTTYPE		gSetDataInfo.SetData.NameData.BattType

// Watchdog definition
#define WTDOG_FEED()    dv_Dog.fFeedDog()

#define CMD_SYS_SET				0x40 //initialize device and display
#define CMD_SLEEP_IN				0x53
#define CMD_DISP_OFF				0x58
#define CMD_DISP_ON				0x59
#define CMD_SCROLL_BLOCK			0x44 //set display start address and display region/set the scroll block start address and contained lines
#define CMD_SET_CURSOR_TYPE			0x5d //set cusor type
#define CMD_SET_CGRAMADDR			0x5c //set cg ram address
#define CMD_SET_CURSOR_AUTO_RIGHT		0x4c //set cursor auto increment direction
#define CMD_SET_CURSOR_AUTO_LEFT		0x4d
#define CMD_SET_CURSOR_AUTO_UP			0x4e
#define CMD_SET_CURSOR_AUTO_DOWN		0x4f
#define CMD_SET_HORIZON_SCROLL_POS		0x5a  //set horizontal scroll position
#define CMD_SET_OVLAY				0x5b  //set display overlay format
#define CMD_SET_CURSOR_ADDR			0x46  //set cursor address
#define CMD_GET_CURSOR_ADDR			0x47  //get cursor address
#define CMD_WRITE_TO_VRAM			0x42  //write to display mem
#define CMD_READ_FROM_VRAM			0x43  //read from display mem

#define IOC_TEXT_MODE				0x6b01
#define IOC_GRAPHIC_MODE			0x6b02
#define IOC_SET_CURSOR_DIR			0x6b03
#define IOC_SET_CURSOR_SHAPE			0x6b04
#define IOC_SET_BL				0x6b05
#define IOC_LCD_INIT				0x6b06
#define IOC_CLEAR_SCREEN			0x6b07
#define IOC_DISP_OFF				0x6b08
#define IOC_DISP_ON				0x6b09
#define IOC_CURSOR_ON				0x6b0a
#define IOC_CURSOR_OFF				0x6b0b
#define IOC_BEEP_ON				0x6b80
#define IOC_BEEP_OFF				0x6b90

#define IOC_XOR_MODE				0x6b0c
#define IOC_OR_MODE				0x6b0d
#define IOC_AND_MODE				0x6b0e
#define IOC_NXOR_MODE				0x6b0f
#define IOC_FILL_SCREEN				0x6b10
#define IOC_COPY_MODE				0x6b11
#define IOC_SET_LCD_CURSOR			0x6b12


//大小LCD定义
#define LCD_LINE_8

#ifdef LCD_LINE_8
#define FULL_SCREEN_LINE    8
#define FULL_SCREEN_LINE_WITH_HEADER    7
#define MAX_SCROLLLINE_NUM  32 * 3
#else
#define FULL_SCREEN_LINE    4
#define FULL_SCREEN_LINE_WITH_HEADER    3
#define MAX_SCROLLLINE_NUM  32
#endif


#define	SCREEN_WIDTH				128  //the screen is 128*64 pixels
#define	MAX_X					      127

#ifdef LCD_LINE_8
#define	SCREEN_HIGHT				128
#define	MAX_Y					      127
#else
#define	SCREEN_HIGHT				64
#define	MAX_Y					      63
#endif

#define BYTES_SCREEN_HIGHT  (SCREEN_HIGHT / 8)

#define FONT_HEIGHT				16  //lines
#define FONT_WIDTH				8	//pixels
#define LINE_INTERVAL				16	//Interval between lines, pixels
#define BYTES_PER_LINE				16	//SCREEN_WIDTH / 8
#define FONTS_PER_LINE				16	//SCREEN_WIDTH / FONT_WIDTH


//functions
void  InitOBJ(void);
void InitHIHardware(void);
void  start(void);
void  InitRunArgs(void);

void	WriteE2PROMBuffer(UCHAR * pTest, UCHAR * pData, UCHAR len);

UCHAR	CompareSetData(UCHAR *pRAM1, UCHAR *pRAM2, UINT16 Length);
void	CopySetData(UCHAR *pRAM1, UCHAR *pRAM2, UINT16 Length);
