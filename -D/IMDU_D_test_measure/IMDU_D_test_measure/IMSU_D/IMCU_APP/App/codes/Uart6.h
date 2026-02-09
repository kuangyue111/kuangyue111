
#define  RS232_UART6_RX_BUFF_NUM	4
#define  RS232_UART6_RX_MAX_NUM		60
#define  RS232_UART6_TX_MAX_NUM		300

struct _Uart6RecvInfo
{
	UINT8 uReceivFlag;			//1ok	0无数据
	UINT8 uBuf[RS232_UART6_RX_MAX_NUM];
};
typedef struct _Uart6RecvInfo  UART6_RECV_INFO;


#define START_CHARGE_STATE 	0
#define CHARGING_STATE 		1
#define CHARGE_OK_STATE 	2
#define CHARGE_FAIL_STATE 	3

#define NO_BUTTOM_START			0
#define BUTTOM_START_1_CHARGE_1	1	//第一次开始充电
#define BUTTOM_START_2_CHARGE_2	2	//第二次开始充电
#define BUTTOM_AGING_MODE_3		3	//开始老化
#define BUTTOM_INSUL_START_4	4	//绝缘检测
#define BUTTOM_BMS_START_5	    5	//BMS模拟启动

#define BUTTOM_STOP_CHARGE_6	6	//退出界面 停止充电
#define BUTTOM_STOP				7	//退出充电
#define BUTTOM_INSULCOUNT_8     8
#define BUTTOM_EMERGENCY		9
#define BUTTOM_TESTEND_11		11
#define BUTTOM_START_AGING_12	12	//开始老化
#define BUTTOM_SOURCE_13		13


#define BUTTOM_AUTO_TEST		12  //自动测试开始



#define BUTTOM_AUTO_NEXT_STEP	15  //自动测试下一步

#define EV_MNEU24_HI(w)  ((UINT8)((UINT16)(w) >> 8))
#define EV_MNEU24_LO(w)  ((UINT8)((UINT16)(w) &0xff))
extern NOINIT UART6_RECV_INFO gUart6RecvBuff[RS232_UART6_RX_BUFF_NUM];