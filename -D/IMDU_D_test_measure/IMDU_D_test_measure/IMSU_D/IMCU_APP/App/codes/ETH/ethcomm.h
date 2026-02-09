


#define CHK_CONN_STAT(u8Stat) (u8Stat&(1<<0))
#define SET_CONN_STAT(u8Stat) (u8Stat|=1<<0)
#define CLR_CONN_STAT(u8Stat) (u8Stat&=~(1<<0))


#define NET_S_NUM	12
#define NET_BUF_S_DATA_L	750	// 目前的协议没有太长的包，所以减小每个buffer，增加buffer深度 
struct NetSendBuf
{
	UINT8 u8DataFlag;			//如果为0 说明已经取走了，可以压数据  如果作为发送结构体，为0则说明可以用这个该BUF 如果未1说明该BUF被占用！
	UINT16 u16Lengh;
	UINT8 u8Buf[NET_BUF_S_DATA_L];
};
typedef struct NetSendBuf __NET_S_BUF;
//这两个宏定义不能进行更改，因为比较配置的时候比对需要用这个结构做缓存！！
#define NET_R_NUM	10
#define NET_BUF_R_DATA_L	1500	//MIN 2 
struct NetReceiveBuf
{
	UINT8 u8DataFlag;			//如果为0 说明已经取走了，可以压数据  如果作为发送结构体，为0则说明可以用这个该BUF 如果未1说明该BUF被占用！
	UINT16 u16Lengh;
	UINT8 u8Buf[NET_BUF_R_DATA_L];//
};
typedef struct NetReceiveBuf __NET_R_BUF;

struct NET_TCP_INFO
{
	UINT8   u8DHCPResult;
	UINT32	u32BreakCount;
	UINT8	iNetCreateSocketFlag;

	//bit 0标记 是否有数据需要发送		对应5有数据需要发送
	//bit 1标记 是否已经创建链接		对应7
	//BIT6 收到新数据！
	//bit 2标记 是否DHCP成功！
	UINT8 u8MachineStat;
	UINT16 LinkNetLocalPort[ALL_GUN_NUM];

	__NET_R_BUF u8ReadBuf[NET_R_NUM];	//这个结构体，被用于初始化读参数，需要保证大于8192字节
	__NET_S_BUF u8SendBuf[NET_S_NUM];	//这个结构体，被用于初始化读参数，需要保证大于8192字节
};
typedef struct NET_TCP_INFO __NET_TCP_INFO;