/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : CellDNB110xSamp.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : 大唐AFE DNB1101A电池采样模块
** @Instructions : 
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "BSPConfig.h"
#include "BSPSPI.h"
#include "DEVGPIO.h"
#include "BSPCOS.h"
#include "EepData.h"
#include "DiagCheck.h"
#include "linx_def.h"
#include "linx_spi.h"
#include "linx_drv.h"
#include "linx_enum.h"
#include "linx_config.h"
#include "BalanCtrl.h"
#include "SystemInfo.h"
#include "CellDNB110xSamp.h"
//#include "SEGGER_RTT.h"
//#if(1 == AFE_TYPE_CHOICE)
/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
extern float LinxCalBufferCH0[2][LINX_CHIP_NUM];
extern uint16_t  IcStateBufferCH0[];
extern double ZrArrCH0[];
extern double ZiArrCH0[];

extern u16 gCellBCtrl_86[][INDEX_86_LEN];   //均衡控制命令

//static u16 LinxDNB110xBalanVolt[250] = {0}; //最多250ID

u8 LinxZmSampNeedFlag = 1;           //阻抗采样需求标志,上电检测一次
static u8 LinxZmSampErrFlag = 0;            //阻抗采样异常标志标志(RSC未更新)

static u8 LinxICSampAlarmFlag = 0;          //IC采样异常标志
static u8 LinxdioModeNum = 1;               //记录枚举成功的ID总数，至少有一个(桥接芯片)

L_DATABUF_BITS* pZrDataBuf = NULL;
L_DATABUF_BITS* pZiDataBuf = NULL;
L_DATABUF_BITS* pVzmDataBuf = NULL;
u16* pVzmAckBuf = NULL;

//用于断线检测判断
u16 IcStateBuf[LINX_CHIP_NUM] = {0};
u8 IcStateErrFlag[3] = {0};

//
static u8 LinxSampVAlarmFlag[MAX_DASIY_NUM] = {0};          //电压异常标志
static u8 LinxSampTAlarmFlag[MAX_DASIY_NUM] = {0};          //温度异常标志
/*=============================================================================================
** @Static function declaration
**===========================================================================================*/
/*=================================================================================================
** @Name      : static void ClrAfeSampCellMsgNoEnum(void)
** @Input     : void
** @Output    : void
** @Function  : 清除没有枚举成功的电池数据
** @The notes : 
**===============================================================================================*/
static void ClrAfeSampCellMsgNoEnum(void);

/*=================================================================================================
** @Name      : static void Linx_ClrSampleCellMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 输出电压采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_ClrSampleCellMsg(void);

/*=============================================================================================
** @Name      : static void ClrAfeSampCellVolt(u8 id)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体电压采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellVolt(u8 id);

/*=============================================================================================
** @Name      : static void ClrAfeSampCellTemp(u8 id)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体温度和均衡板温温度采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellTemp(u8 id);

/*=============================================================================================
** @Name      : static void ClrAfeSampCellZm(u8 id)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有阻抗采样值为65535
** @The notes : AFE采集失效时调用
**===========================================================================================*/
static void ClrAfeSampCellZm(u8 id);

/*=================================================================================================
** @Name      : static void Linx_SampleCellVoltMsg_Output(void)
** @Input     : void
** @Output    : void
** @Function  : 板级信息周期上传任务
** @The notes : 50ms
**===============================================================================================*/
static void Linx_SampleCellVoltMsg_Output(void);

/*=================================================================================================
** @Name      : static void Linx_SampleCellVoltMsg_Output(void)
** @Input     : void
** @Output    : void
** @Function  : 输出温度采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_SampleCellTempMsg_Output(void);

/*=================================================================================================
** @Name      : static void Linx_SampleCellZmMsg_Output(void)
** @Input     : void
** @Output    : void
** @Function  : 输出阻抗采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_SampleCellZmMsg_Output(void);

///*=================================================================================================
//** @Name      : static void Linx_FindNeedBanlanID(void)
//** @Input     : void
//** @Output    : void
//** @Function  : 查找需要开启均衡的电池电压
//** @The notes : 
//**===============================================================================================*/
//static void Linx_FindNeedBanlanID(void);

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : void CellAfeDNBSampInit(void)
** @Input     : void
** @Output    : void
** @Function  : 初始化模拟前端配置寄存器所需的变量
** @The notes :
**===========================================================================================*/
void CellAfeDNBSampInit(void)
{   
    //make_crc_table();/*初始化crc校验*/
    //DEVGPIOSetPortState(ePOut_DNB1101_CS1, ePSTATE_ON);//SPI_en拉高，SPI模式，硬件直接拉高，软件控制无效
    //DEVGPIOSetPortState(ePOut_DNB1101_CS2, ePSTATE_ON);//SPI_en拉高，SPI模式，硬件直接拉高，软件控制无效
}

//static u8 eTime = 0;
int RscUpdate[300] = {0};//测试RSC

static u16 sSampErrTimer = 0;

/*=============================================================================================
** @Name      : void CellInfoDNBSampleTask(void *p)
** @Input     : *p:形参,任务函数参数需要
** @Output    : void
** @Function  : 单体电压、温度采样任务
** @The notes : 该任务目前只支持单线采样(暂不实现双线)
**===========================================================================================*/
void CellInfoDNBSampleTask(void *p)
{
    static u8 sSampStep = 0xaa;
    static u8 enumtTime = 0;//枚举计数
    static u8 dioModeTime = 0;//记录枚举ID数目和实际物理ID数目不相等计数
    static u8 isRscUpdateCnt = 0;
    static u8 isRscUpdateErr = 0;/*RSC未更新重复等待计数*/
    static u8 setFreqIndex = 1; /*设置频率索引*/
    u8 i = 0;
    u8 ch = 0;/*参数需要*/
    u8 id = 0;
    u16 bsuNum = 0;
    //u8 ic_Num = 0;
    //u8 index = 0;
    static u8 ic_Num = 0;
    static u8 Linx_index = 0;
    u16 indexId = 0;
//    u8 vzmPos = 0;
//    u8 zrPos = 0;
//    u8 ziPos = 0;
//	u8 tmpSamplePerFreq = 0;
    u8 isRscUpdateFlag = 0;
	float vzmVal = 0;
	int pos = 0;
//	int waitTime = 0;
//	double real = 0.0;
//  double imag = 0.0;
    L_CHL_t* pCh = NULL;   
    L_ST_tu res;
    L_STATUS_te ret = L_STATUS_OK;
    
    p = p;

    bsuNum = GetDaisyAFENum()*GetVoltSampNum();	/*组内从控并联串数 */
    //bsuVNum = gGBmuHigLevPara_103[eBmuHigLevPara103_BsuVNum];	/*一个从控电压采样节数*/
    //bsuTNum = gGBmuHigLevPara_103[eBmuHigLevPara103_BsuTNum];	/*一个从控温度采样节数*/
    //balTNum = gGBmuHigLevPara_103[eBmuHigLevPara103_BalTNum];	/*一个从控板温采样节数*/
    
    if(0 != GetLinxICSampAlarmFlagAPI())//枚举异常需要重新上电
    {
        sSampErrTimer++;
        if(sSampErrTimer > 500)
        {
            sSampErrTimer = 0;
            WriteVSampFinFlag(1);
            WriteTSampFinFlag(1);
            WriteZSampFinFlag(1);
        }
        sSampStep = 0xee;//无效值
        BSPCosStart(COS_ID_CELL_SAMP, 2);
    }
       
    switch(sSampStep)
    {   
        //枚举   
        case 0xaa:
            WriteVSampFinFlag(0);
            WriteTSampFinFlag(0);
            WriteZSampFinFlag(0);
            if(enumtTime < ENUM_NUM)
            {
                //time =1;
                (void)Test_linx_enum_start(ch);   
            }
            sSampStep = EvtEnum;
            BSPCosStart(COS_ID_CELL_SAMP, 500);
            break;
            
        //枚举   
        case EvtEnum:
            if(enumtTime < ENUM_NUM)
            {
                //enumtTime = 0xff;
                linx_drv_enum(ch, false);   
            }
            //enumtTime++;//周期枚举
            
            if(enumtTime > ENUM_NUM)//防止越界
            {
                enumtTime = 200;
            }
            pCh = linx_get_chlPtr(ch);
            
            if(0 == dioModeTime)//只记录一次
            {
                dioModeTime = 1;
                LinxdioModeNum = pCh->dioModeNum;//保存第一次枚举的ID总数量
            }
            
            if(0 == pCh->dioModeNum)//枚举失败或者硬件没有连接
            {
                dioModeTime = 0;
                enumtTime = 0;
                sSampStep = 0xaa;   
                SetLinxICSampAlarmFlagAPI(1);    //设置采样异常标志
                Linx_ClrSampleCellMsg();        /*清除所有数据*/
                //WriteVSampFinFlag(1);
                //WriteTSampFinFlag(1);
                //WriteZSampFinFlag(1);
            }
            else
            {
                //枚举完之后判断pCh->dioModeNum是否与电池柜实际总数量相等否则重新枚举，达到枚举次数之后判断采样异常或枚举失败，需要检查硬件
                if(pCh->dioModeNum != bsuNum)
                //if(pCh->dioModeNum != LINX_BOARD_MAXNUM)//测试
                {
                    //if(GetDaisyAFENum() > pCh->dioModeNum)
                    if(bsuNum > pCh->dioModeNum)
                    {
                        //for(id = (pCh->dioModeNum+1); id <= GetDaisyAFENum(); id++)
                        for(id = (pCh->dioModeNum+1); id <= bsuNum; id++)
                        {
                            ClrAfeSampCellVolt(id);
                            ClrAfeSampCellTemp(id);
                            ClrAfeSampCellZm(id);
                        }
                        //清除没有枚举成功的电池箱数据
                        ClrAfeSampCellMsgNoEnum();
                    } 
                    //SetLinxICSampAlarmFlagAPI(1);    //采样异常标志置位
                }
                sSampStep = 0xff;       //关闭均衡                   
            }
            BSPCosStart(COS_ID_CELL_SAMP, 20);
            break;            
            
        case 0xff:
            //CtrlBattBalanSetState(0);//无条件关闭所有均衡
            linx_drv_stop_balance(ch, 0xff);//关闭均衡
            sSampStep = EvtVMm;
            BSPCosStart(COS_ID_CELL_SAMP, 10);
            break;  
           
        //获取主电压
        case EvtVMm:
            id = 0xff;//参数输入，实际没有使用   
            pCh = linx_get_chlPtr(ch);
            linx_drv_get_volt_main(ch, id); 
            Linx_SampleCellVoltMsg_Output();
            WriteVSampFinFlag(1);
            sSampStep = EvtDTm;  
            BSPCosStart(COS_ID_CELL_SAMP, 5);
            break;
            
        //获取主温度值
        case EvtDTm:
            //ch=pData[0];
            id = 0xff;//参数输入，实际没有使用
            pCh = linx_get_chlPtr(ch);
            linx_drv_get_dieTemp_main(ch, id);
            Linx_SampleCellTempMsg_Output();
            WriteTSampFinFlag(1);
            sSampStep = EvtBalCfg;//测试，只枚举一次
            BSPCosStart(COS_ID_CELL_SAMP, 5);
            break;                                        
        
        //配置均衡参数
        case EvtBalCfg:
            for(i=0;i<linx.chlNum;i++)
            {
                //linx_drv_config_balance(i, LINX_EXT_SOURCE, LINX_BAL_VOLT_BASE, BALANCE_PWM_DUTY, LinxDNB110xBalanVolt[id], BALANCE_TIMER_OUT);
                linx_drv_config_balance(i, LINX_EXT_SOURCE, LINX_BAL_VOLT_BASE, BALANCE_PWM_DUTY, BALANCE_VOLT_ON, BALANCE_TIMER_OUT);//
            }
            sSampStep = EvtBalSt;
            BSPCosStart(COS_ID_CELL_SAMP, 1);
            break;
            
        //开始均衡
        case EvtBalSt:
            id = 0xFF;
            pCh = linx_get_chlPtr(ch);
            UpdateBattBalanCtrlMsg();
            
//            CtrlBattBalanSetState(1);//按需求开启均衡
          
            //for(ic_Num = 0; ic_Num < GetDaisyAFENum(); ic_Num++)  //一次性控制异常    
            {
                //for(index = 0; index < MAX_VOLT_NUM; index++)//保证均衡开启正常
                {
                    //indexId = index + ic_Num*MAX_VOLT_NUM + 1;//计算IC的ID号，id=0为桥接芯片，采样芯片从1开始
                    //indexId = MAX_VOLT_NUM - (index + ic_Num*MAX_VOLT_NUM);//计算IC的ID号，id=0为桥接芯片，采样芯片从1开始,序号相反
                    if(pCh->dioModeNum > MAX_VOLT_NUM)
                    {
                        indexId = MAX_VOLT_NUM*GetDaisyAFENum() - (Linx_index + ic_Num*MAX_VOLT_NUM);//计算IC的ID号，id=0为桥接芯片，采样芯片从1开始,序号相反
                    }
                    else
                    {
                        indexId = pCh->dioModeNum - (Linx_index + ic_Num*MAX_VOLT_NUM);//计算IC的ID号，id=0为桥接芯片，采样芯片从1开始,序号相反
                    }
                    if(1 == BitGet(gCellBCtrl_86[ic_Num][e86BCtrl_PasDhg], Linx_index))
                    {
                        ret = linx_drv_start_balance(ch, indexId);//开启均衡

                        if(ret != L_STATUS_OK)//控制异常再控制一次
                        {
                            ret = linx_drv_start_balance(ch, indexId);//开启均衡
                        }
                    }
//                    else
//                    {
//                        linx_drv_stop_balance(ch, indexId);//关闭均衡
//                    }
                }
            }                   
                  
            if(1 == LinxZmSampNeedFlag)//有电芯阻抗测试需求
            {
                linx_drv_stop_balance(ch, 0xff);//有阻抗测量关闭均衡
                SetLinxZmSampAlarmFlagAPI(0);  /*清除阻抗采样异常*/
                sSampStep = EvtZM;
                BSPCosStart(COS_ID_CELL_SAMP, 20);
            }
            else
            {
                Linx_index++;     //逐一单个控制
                if(pCh->dioModeNum > MAX_VOLT_NUM)
                {
                    if(Linx_index >= MAX_VOLT_NUM)//单个PACK轮训完成
                    {
                        Linx_index = 0;                
                        ic_Num++;//下一个PACK
                        
                        if(ic_Num >= GetDaisyAFENum())//所有PACK轮训完成
                        {
                            ic_Num = 0;
                            sSampStep = 0xff;
                            BSPCosStart(COS_ID_CELL_SAMP, 200);//均衡时间
                        }
                        else
                        {
                            sSampStep = EvtBalSt;
                            BSPCosStart(COS_ID_CELL_SAMP, 1); 
                        }
                    }
                    else
                    {
                        sSampStep = EvtBalSt;
                        BSPCosStart(COS_ID_CELL_SAMP, 1); 
                    }
                }
                //只有一个PACk或不满一个PACK
                else
                {
                    if(Linx_index >= pCh->dioModeNum)//PACK轮训完成
                    {
                        Linx_index = 0;             
                        ic_Num = 0;
                        sSampStep = 0xff;
                        BSPCosStart(COS_ID_CELL_SAMP, 200);
                    }
                    else
                    {
                        sSampStep = EvtBalSt;
                        BSPCosStart(COS_ID_CELL_SAMP, 1); 
                    }                
                }
            }           
            break;   
#if 1                  
        case EvtZM: 
                            //正在进行电芯阻抗测量标志
            pCh = linx_get_chlPtr(ch);
            pCh->sendDummyCmd = true; 
            
            pVzmDataBuf = linx_get_dataBufPtr(pCh, OutVZM);
            pZrDataBuf = linx_get_dataBufPtr(pCh, OutZR);
            pZiDataBuf = linx_get_dataBufPtr(pCh, OutZI);
            pVzmAckBuf = linx_get_ackBufPtr(pCh, OutVZM);

            linx_drv_get_srFlag(ch, 0xFF);
            linx_drv_rst_sr(ch, 0xFF);
            linx_drv_set_mode(ch, 0xFF, NormalMode);
            
            sSampStep = 0xcc;
            BSPCosStart(COS_ID_CELL_SAMP, 10); 
            break;
            
        case 0xcc:
            pCh = linx_get_chlPtr(ch);
            linx_drv_set_freq(ch, setFreqIndex, LINX_ZM_FREQ);//设置频率1000Hz，频率的宏定义后期改为参数
//            for(id = 1;id <= pCh->dioModeNum; id++)
//            {
//                linx_drv_set_freq(ch, id, LINX_ZM_FREQ);//设置频率1000Hz
//            }
            
//            for(id = 1; id <= pCh->dioModeNum; id++)
//            {
//                //((L_ACK_tu*)&pCh->ackBuf[vzmPos][id])->val = 0;
//                ((L_ACK_tu*)&pVzmAckBuf[id])->val = 0;
//            } 
            if(setFreqIndex == pCh->dioModeNum)
            {
                sSampStep = 0xdd;
                BSPCosStart(COS_ID_CELL_SAMP, 10);  
            }
            else
            {
                setFreqIndex++;//单个设置保证成功率
                sSampStep = 0xcc;
                BSPCosStart(COS_ID_CELL_SAMP, 2);  
            }
                    
            break;
                            
//        case 0xcc:          
//            id = 0xFF;
//            pCh = linx_get_chlPtr(ch);
//            linx_drv_start_zm(ch, 0xFF, false, LINX_GAIN_1, LINX_MAX_TIMEOUT);
//            //linx_drv_start_zm(ch, id, true, LINX_GAIN_1, LINX_MAX_TIMEOUT);	
//            //for(id = 1;id <= pCh->dioModeNum; id++)//单个开启程序回卡顿(原因待查)
//            {
//                
//                //linx_drv_set_freq(ch, LINX_BOARD_ALLID, LINX_ZM_FREQ);
//            }
//            sSampStep = 0xdd;
//            BSPCosStart(COS_ID_CELL_SAMP, 500);  
//            break;
            
        case 0xdd:  
            id = 0xFF;
            pCh = linx_get_chlPtr(ch);
            //for(id = 1;id <= pCh->dioModeNum; id++)
            {
                res.val = linx_drv_start_zm(ch, id, true, LINX_GAIN_1, LINX_MAX_TIMEOUT);	
            }
            //linx_drv_set_freq(ch, LINX_BOARD_ALLID, LINX_ZM_FREQ);
//            for(id = 1;id <= pCh->dioModeNum; id++)
//            {
//                //linx_drv_set_freq(ch, id, LINX_ZM_FREQ);
//            }  
            
            if(1 == res.fmt.isErrAck)
            {
                sSampStep = EvtZM;
                BSPCosStart(COS_ID_CELL_SAMP, 1);   
            }
            else
            {      
                sSampStep = EvtZmCtl;
                BSPCosStart(COS_ID_CELL_SAMP, 100);                  
            } 
            break;
            
        case EvtZmCtl:
            pCh = linx_get_chlPtr(ch);

            id = LINX_BOARD_ALLID;
            linx_drv_get_vzm(ch, LINX_BOARD_ALLID);
//            for(id = 1; id <= pCh->dioModeNum; id++)
//            {          
//                //linx_drv_get_vzm(ch, id);
//            }
            
            for(id = 1; id <= pCh->dioModeNum; id++)
			{
                if(true == ((L_ACK_tu*)&pVzmAckBuf[id])->fmt.isRscUpdate)//测试查看已更新的RSC
                {
                    RscUpdate[id] = true;
					//SEGGER_RTT_printf(0,"RscUpdate[%d] = 1\r\n",id);
                }
                else
                {
                    RscUpdate[id] = false;     
					//SEGGER_RTT_printf(0,"RscUpdate[%d] = 0\r\n",id);
                }
				//if(!((L_ACK_tu*)&pCh->ackBuf[vzmPos][id])->fmt.isRscUpdate)
				if(!((L_ACK_tu*)&pVzmAckBuf[id])->fmt.isRscUpdate)
				{
                    isRscUpdateCnt++;//RSC已更新的ID计数
                    isRscUpdateFlag = 1;//测试，正常使用时要放开
                    break;
				}
                else
                {
                    isRscUpdateFlag = 0;
                }
            }
            
            if(1 == isRscUpdateFlag)//有ID的RSC未更新
            //if(0 == test_isRscUpdate)//有ID的RSC未更新
            {
                sSampStep = EvtZmCtl;
                if(isRscUpdateErr > 5)/*重复次数超过5次，防止测量异常时，程序没办法恢复正常采样*/
                {
                    isRscUpdateCnt = 0;
                     sSampStep = EvtVMm;  
                     linx_drv_stop_zm(ch, 0xFF);    /*停止所有电芯阻抗测量*/
                     //LinxZmSampNeedFlag = 0;        /*结束阻抗测量流程*/
                     SetLinxZmSampAlarmFlagAPI(1);  /*设置阻抗采样异常*/
                }
                isRscUpdateFlag = 0;
                isRscUpdateErr++;/*重复等待计数*/
                BSPCosStart(COS_ID_CELL_SAMP, 500);     //等待时间不能太短
            }
            else
            {
//                isRscUpdateCnt = 0;
//                isRscUpdateErr = 0;/*重复次数清零*/
//                
//                if(eTime > 2)
                {
                    //eTime = 0;
                    sSampStep = 0x23;//计算阻抗
                }
//                else
//                {
//                    sSampStep = EvtZmCtl;
//                }
                BSPCosStart(COS_ID_CELL_SAMP, 10);       
            }
            //eTime++;
            break;

        case 0x23:
            pCh = linx_get_chlPtr(ch);
            
            for(i = 0; i < pCh->maxChipNum; i++)
            {
                pCh->zrArr[i] = 0;
                pCh->ziArr[i] = 0;
            }            
            
            for(id = 1;id <= pCh->dioModeNum; id++)
            {
                ((L_ACK_tu*)&pVzmAckBuf[id])->fmt.isRscUpdate = false;
            }
            
            //for(id = 1;id <= pCh->dioModeNum; id++)
            {          
                linx_drv_get_zr(ch, LINX_BOARD_ALLID);
                linx_drv_get_zi(ch, LINX_BOARD_ALLID);
            }
        
            for(id = 1;id <= pCh->dioModeNum; id++)
            {
//                if((((L_ACK_tu*)&pVzmAckBuf[id])->fmt.isRscUpdate)&&
//                    (((L_ACK_tu*)&pVzmAckBuf[id])->fmt.cRSC != 0)&&
//                    (((L_ACK_tu*)&pVzmAckBuf[id])->fmt.isRsc0Got))
                
//                if((((L_ACK_tu*)&pCh->ackBuf[vzmPos][id])->fmt.isRscUpdate)&&
//                    (((L_ACK_tu*)&pCh->ackBuf[vzmPos][id])->fmt.cRSC!=0)&&
//                    (((L_ACK_tu*)&pCh->ackBuf[vzmPos][id])->fmt.isRsc0Got))                
                {
                    pCh->zmCnter[id]++;
                    //LINX_LOG(OP_DEBUG,"isRscUpdate=%d\r\n",((L_ACK_tu*)&pVzmAckBuf[i])->fmt.isRscUpdate);
//                    ((L_ACK_tu*)&pCh->ackBuf[vzmPos][id])->fmt.isRscUpdate = false;
//                    ((L_ICST_tu*)&pCh->icStBuf[id])->fmt.isZmUpdate = true;
//                    ((L_ACK_tu*)&pVzmAckBuf[id])->fmt.isRscUpdate = false;
                    //((L_ICST_tu*)&pCh->icStBuf[id])->fmt.isZmUpdate = true;
                    pos=pCh->freqPosArr[id];
                    #if LINX_CONF_SAVE_PAYLOAD_ONLY
                    //LINX_LOG(OP_DEBUG,"pos=%d,vzm%08x,zr%08x,zi%08x\r\n",pos,pVzmDataBuf[i],pZrDataBuf[i],pZiDataBuf[i]);
                    vzmVal = LINX_CAL_VOLT(pVzmDataBuf[id]);
                    //pCh->zrArr[id] = linx_drv_cal_zm(pZrDataBuf[id], pCh->Rext, vzmVal, pos);
                    pCh->zrArr[id] = linx_drv_cal_zm(pZrDataBuf[id], pCh->Rext, vzmVal, pos);
                    pCh->ziArr[id] = linx_drv_cal_zm(pZiDataBuf[id], pCh->Rext, vzmVal, pos);
                    #else
                    //LINX_LOG(OP_DEBUG,"pos=%d,vzm%08x,zr%08x,zi%08x\r\n",pos,pVzmDataBuf[i],pZrDataBuf[i],pZiDataBuf[i]);
//                    vzmVal = LINX_CAL_VOLT(((L_CONF_tu*)&pCh->dataBuf[vzmPos][id])->fmt.data);
//                    pCh->zrArr[id] = linx_drv_cal_zm(((L_CONF_tu*)&pCh->dataBuf[zrPos][id])->fmt.data, pCh->Rext, vzmVal, pos);
//                    pCh->ziArr[id] = linx_drv_cal_zm(((L_CONF_tu*)&pCh->dataBuf[ziPos][id])->fmt.data, pCh->Rext, vzmVal, pos);
                    vzmVal = LINX_CAL_VOLT(((L_CONF_tu*)&pVzmDataBuf[id])->fmt.data);
                    pCh->zrArr[id] = linx_drv_cal_zm(((L_CONF_tu*)&pZrDataBuf[id])->fmt.data, pCh->Rext, vzmVal, pos);
                    pCh->ziArr[id] = linx_drv_cal_zm(((L_CONF_tu*)&pZiDataBuf[id])->fmt.data, pCh->Rext, vzmVal, pos);
                    #endif
                    //LINX_LOG(OP_DEBUG,"RSC=%d\r\n",(((L_ACK_tu*)&pVzmAckBuf[i])->fmt.cRSC));
                    //LINX_LOG(OP_DATA,"<<<ZM%d\t%d\t%d\t%7.2f\t%.2f\t%.2f\t>>>\r\n",ch,i,pCh->zmCnter[i],linx.defParaSet->freqMap[pos],pCh->zrArr[i],pCh->ziArr[i]);
                }
            }  
            //linx_drv_set_freq(ch, 0xFF, LINX_ZM_FREQ);
            Linx_SampleCellZmMsg_Output();  //输出阻抗采样值
            WriteZSampFinFlag(1);           //电芯阻抗测量完成一遍标志
            linx_drv_start_zm(ch, 0xFF, false, LINX_GAIN_1, LINX_MAX_TIMEOUT);
            linx_drv_stop_zm(ch, 0xFF);     /*停止所有电芯阻抗测量*/
            LinxZmSampNeedFlag = 0;        /*结束阻抗测量流程*/
            pCh->sendDummyCmd = false;
            sSampStep = EvtVMm;      
            BSPCosStart(COS_ID_CELL_SAMP, 100);   
            break;           
#endif            
                
        default:
            sSampStep = 0;
            BSPCosStart(COS_ID_CELL_SAMP, 0);
            break;           
    }    
}

/*=============================================================================================
** @Name      : void LinxDNB110xPasEqual(u16* commande, u8 total_ic)
** @Input     : commande:均衡命令   total_ic:从件个数
** @Output    : void
** @Function  : DNB110x的被动均衡执行函数
** @The notes : 一个IC一路均衡
**===========================================================================================*/
void LinxDNB110xPasEqual(u16* commande, u8 total_ic)
{   
    u8 ch = 0;/*参数需要*/
    u16 id = 0;
    u8 index = 0;
    u8 ic_Num = 0;/*IC序号*/
    L_CHL_t* pCh = NULL; 
    L_STATUS_te ret = L_STATUS_OK;
        
    pCh = linx_get_chlPtr(ch);
    
    if(pCh == NULL)
    {
        return;
    }
    
    for(ic_Num = 0; ic_Num < total_ic; ic_Num++)      //一次控制，等待时间过长，会导致控制失败
    {
        for(index = 0; index < MAX_VOLT_NUM; index++)
        {
            id = index + ic_Num*MAX_VOLT_NUM + 1;//计算IC的ID号，id=0为桥接芯片，采样芯片从1开始
            //id = MAX_VOLT_NUM - (index + ic_Num*MAX_VOLT_NUM);//计算IC的ID号，id=0为桥接芯片，采样芯片从1开始
            
            //if(1 == BitGet(commande[ic_Num], index))
            if(1 == BitGet(gCellBCtrl_86[ic_Num][e86BCtrl_PasDhg], index))
            {
                ret = linx_drv_start_balance(ch, id);//开启均衡
                if(ret != L_STATUS_OK)
                {
                    ret = linx_drv_start_balance(ch, id);//开启均衡
                }
            }
            else
            {
                linx_drv_stop_balance(ch, id);//关闭均衡
            }
        }
    }
}

/*=================================================================================================
** @Name      : void SetLinxICSampAlarmFlagAPI(u8 flag)
** @Input     : flag:  0正常  1异常
** @Output    : void
** @Function  : 设置IC采样异常标志
** @The notes : 
**===============================================================================================*/
void SetLinxICSampAlarmFlagAPI(u8 flag)
{
    if(flag > 1)
    {
        return;
    }
    
    LinxICSampAlarmFlag = flag;
}

/*=================================================================================================
** @Name      : u8 GetLinxICSampAlarmFlagAPI(void)
** @Input     : void
** @Output    : IC采样异常标志
** @Function  : 获取IC采样异常标志
** @The notes : 
**===============================================================================================*/
u8 GetLinxICSampAlarmFlagAPI(void)
{
    return(LinxICSampAlarmFlag);
}
/*=================================================================================================
** @Name      : void SetLinxSampVAlarmFlagAPI(u8 ic_Num, u8 flag)
** @Input     : ic_Num:AFE序号  flag:  0正常  1异常
** @Output    : void
** @Function  : 设置电压采样异常标志
** @The notes : 
**===============================================================================================*/
void SetLinxSampVAlarmFlagAPI(u8 ic_Num, u8 flag)
{
    if(ic_Num >= GetDaisyAFENum())
	{
		return;
	}
    if(flag > 1)
    {
        return;
    }
    LinxSampVAlarmFlag[ic_Num] = flag;
}
/*=================================================================================================
** @Name      : u8 GetLinxSampVAlarmFlagAPI(u8 ic_Num)
** @Input     : ic_Num：AFE序号
** @Output    : 电压采样异常标志
** @Function  : 获取电压异常标志
** @The notes : 
**===============================================================================================*/
u8 GetLinxSampVAlarmFlagAPI(u8 ic_Num)
{
    if(ic_Num >= GetDaisyAFENum())
	{
		return 0;
	}
    return(LinxSampVAlarmFlag[ic_Num]);
}
/*=================================================================================================
** @Name      : void SetLinxSampTAlarmFlagAPI(u8 ic_Num, u8 flag)
** @Input     : ic_Num:AFE序号  flag:  0正常  1异常
** @Output    : void
** @Function  : 设置温度采样异常标志
** @The notes : 
**===============================================================================================*/
void SetLinxSampTAlarmFlagAPI(u8 ic_Num, u8 flag)
{
    if(ic_Num >= GetDaisyAFENum())
	{
		return;
	}
    if(flag > 1)
    {
        return;
    }
    LinxSampTAlarmFlag[ic_Num] = flag;
}
/*=================================================================================================
** @Name      : u8 GetLinxSampTAlarmFlagAPI(u8 ic_Num)
** @Input     : ic_Num：AFE序号
** @Output    : 温度采样异常标志
** @Function  : 获取温度异常标志
** @The notes : 
**===============================================================================================*/
u8 GetLinxSampTAlarmFlagAPI(u8 ic_Num)
{
    if(ic_Num >= GetDaisyAFENum())
	{
		return 0;
	}
    return(LinxSampTAlarmFlag[ic_Num]);
}
/*=============================================================================================
** @Name      : void SetLinxZmSampAlarmFlagAPI(u8 flag)
** @Input     : flag: 0正常   1异常
** @Output    : void
** @Function  : 设置阻抗采样异常标志
** @The notes : 
**===========================================================================================*/
void SetLinxZmSampAlarmFlagAPI(u8 flag)
{
   if(flag > 1)
   {
       return;
   }
    
    LinxZmSampErrFlag = flag;   
}

/*=============================================================================================
** @Name      : u8 GetLinxZmSampAlarmFlagAPI(void)
** @Input     : void
** @Output    : 0正常   1异常
** @Function  : 获取阻抗采样异常标志
** @The notes : 
**===========================================================================================*/
u8 GetLinxZmSampAlarmFlagAPI(void)
{
   return(LinxZmSampErrFlag);
}

/*=============================================================================================
** @Static function definition
**===========================================================================================*/
/*=================================================================================================
** @Name      : static void ClrAfeSampCellMsgNoEnum(void)
** @Input     : void
** @Output    : void
** @Function  : 清除没有枚举成功的电池数据
** @The notes : 上电枚举异常之后调用
**===============================================================================================*/
static void ClrAfeSampCellMsgNoEnum(void)
{
    u8 i = 0;
    u8 ch = 0;
    u8 ic_Num = 0;
    L_CHL_t* pCh = NULL; 
       
    pCh = linx_get_chlPtr(ch);
    
    if(pCh == NULL)
    {
        return;
    }
    
    if(0 == pCh->dioModeNum)
    {
        return;
    }
    
    //bsuNum = GetDaisyAFENum()*GetVoltSampNum();	/*组内从控并联串数 */
    
    if(0 != (pCh->dioModeNum / MAX_VOLT_NUM))//不是电池箱的整数倍
    {
        ic_Num = pCh->dioModeNum / MAX_VOLT_NUM;//计算是第几个电池箱
    }
    else
    {
        ic_Num = (pCh->dioModeNum / MAX_VOLT_NUM) + 1;//计算是第几个电池箱
    }
    for(/*ic_Num = 0 */;ic_Num < GetDaisyAFENum(); ic_Num++)
    {
        for(i = 0; i < MAX_VOLT_NUM; i++)
        {                 
            InputBsuCellVolt(ic_Num, i, 0);
            InputBsuCellTemp(ic_Num, i, -41);
            //WriteSampCellVoltHook(ic_Num, i, 0);       /*输出电压值*/  
            //WriteSampCellTempHook(ic_Num, i, -41);     /*输出温度值*/    
            WriteSampCellZmHook(ic_Num, i, 65535);    /*输出阻抗实部值*/                
        }                
    }    
}

/*=================================================================================================
** @Name      : static void Linx_ClrSampleCellMsg(void)
** @Input     : void
** @Output    : void
** @Function  : 清除电压采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_ClrSampleCellMsg(void)
{
    u8 i = 0;
    u16 sumNumV = 0;
    u16 sumNumT = 0;
    u16 sumNumZ = 0;
    u8 ic_Num = 0;
    //u8 cell_Num = 0;
    
    sumNumV = GetDaisyAFENum()* MAX_VOLT_NUM / MAX_VOLT_NUM;
     
    if(0 == sumNumV)
    {
        ic_Num = 0; 
        for(i = 0;i < MAX_VOLT_NUM; i++)
        {     
            //cell_Num = i - 1;
            //WriteSampCellVoltHook(ic_Num, i, 0);       /*输出电压值*/
          InputBsuCellVolt(ic_Num, i, 0);
        }
    }
    else
    {        
        for(ic_Num = 0; ic_Num < sumNumV; ic_Num++)
        {
            for(i = 0; i < MAX_VOLT_NUM; i++)
            {  
                //cell_Num = (i - 1) - (MAX_VOLT_NUM*ic_Num);
                //WriteSampCellVoltHook(ic_Num, i, 0);       /*输出电压值*/
              InputBsuCellVolt(ic_Num, i, 0);
            }
        }
    } 
    
    sumNumT = GetDaisyAFENum() * MAX_TEMP_NUM / MAX_TEMP_NUM;
     
    if(0 == sumNumT)
    {
        ic_Num = 0; 
        for(i = 0; i < MAX_TEMP_NUM; i++)
        {     
            //cell_Num = i - 1;
            //WriteSampCellTempHook(ic_Num, i, -41);       /*输出温度值*/
           InputBsuCellTemp(ic_Num, i, -41);
        }
    }
    else
    {        
        for(ic_Num = 0; ic_Num < sumNumT; ic_Num++)
        {
            for(i = 0; i < MAX_TEMP_NUM; i++)
            {  
                //cell_Num = (i - 1) - (MAX_TEMP_NUM*ic_Num);
                //WriteSampCellTempHook(ic_Num, i, -41);       /*输出温度值*/
               InputBsuCellTemp(ic_Num, i, -41);
            }
        }
    }     
    
    sumNumZ = GetDaisyAFENum() * DEFAULT_ZNUM / DEFAULT_ZNUM;
     
    if(0 == sumNumZ)
    {
        ic_Num = 0; 
        for(i = 0;i < GetTotalTempNum(); i++)  //大唐温度和内阻通道数一致
        {     
            //cell_Num = i - 1;
            WriteSampCellZmHook(ic_Num, i, 65535);       /*输出阻抗实部值*/
        }
    }
    else
    {        
        for(ic_Num = 0; ic_Num < sumNumZ; ic_Num++)
        {
            for(i = 0;i < GetTotalTempNum(); i++)
            {  
                //cell_Num = (i - 1) - (DEFAULT_ZNUM*ic_Num);
                WriteSampCellZmHook(ic_Num, i, 65535);    /*输出阻抗实部值*/
            }
        }
    }
}

/*=============================================================================================
** @Name      : static void ClrAfeSampCellVolt(u8 id)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体电压采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellVolt(u8 id)
{
//    u8 i = 0;
//
//    /*所有电池单体电压*/
//	for(i = 0; i < MAX_VOLT_NUM; i++)
//    {
//        /*输出单体电压值为0*/
//		WriteSampCellVoltHook(ic_Num, i, 0);
//    }
//    WriteSampCellVoltHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][i]);       /*输出电压值*/
    
    LinxCalBufferCH0[0][id] = 0;
}

/*=============================================================================================
** @Name      : static void ClrAfeSampCellTemp(u8 id)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有单体温度和均衡板温温度采样值为0
** @The notes : AFE单体电压温度采样失效时调用
**===========================================================================================*/
static void ClrAfeSampCellTemp(u8 id)
{
//    u8 i = 0;
//
//    /*所有电池单体温度*/
//	for(i = 0; i < MAX_TEMP_NUM; i++)
//    {
//        /*输出单体温度值为-41*/
//		WriteSampCellTempHook(ic_Num, i, -41);
//    }
    
    LinxCalBufferCH0[1][id] = -41;
}

/*=============================================================================================
** @Name      : static void ClrAfeSampCellZm(u8 id)
** @Input     : void
** @Output    : void
** @Function  : 清除AFE采集的所有阻抗采样值为65535
** @The notes : AFE采集失效时调用
**===========================================================================================*/
static void ClrAfeSampCellZm(u8 id)
{  
    ZrArrCH0[id] = 65535;
}

/*=================================================================================================
** @Name      : static void Linx_SampleCellVoltMsg_Output(void)
** @Input     : void
** @Output    : void
** @Function  : 输出电压采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_SampleCellVoltMsg_Output(void)
{
    u8 id = 0;
    u8 ch = 0;
    u8 sumNum = 0;
    L_CHL_t* pCh = NULL; 
    u8 ic_Num = 0;
    u8 cell_Num = 0;
    u8 dioModeNum = 0;
    static u8 LinxDioOffFlag = 0;//菊花链有断线异常
    static u16 IcStateErr[LINX_CHIP_NUM] = {0};   //IC状态异常计数
        
    pCh = linx_get_chlPtr(ch);
    
    if(pCh == NULL)
    {
        return;
    }
   
    if(1 == IcStateErrFlag[0])
    {
        //判断断线，并清除原始采样值
        for(id = 1; id <= pCh->dioModeNum; id++)
        {
            //if(L_DIO_ERR_ZERO == IcStateBuf[id])
            if(L_DIO_ERR_ZERO == IcStateBuf[id-1])//原因待查，debug过程中少一个
            {
                if(IcStateErr[id] >= 1) //异常达到次数
                {
                    IcStateErr[id] = 0;
                    ClrAfeSampCellVolt(id);
                    //IcStateErrFlag[0] = 1;
                    //break;
                }
                IcStateErr[id]++;//错误计数        
            }
        }
        IcStateErrFlag[0] = 0;
    }
    
    if(LinxdioModeNum != pCh->dioModeNum)
    {
        dioModeNum = LinxdioModeNum;
        //dioModeNum = LINX_BOARD_MAXNUM;//最多250个ID
        LinxDioOffFlag = 1;
    }
    else
    {
        LinxDioOffFlag = 0;
        dioModeNum = pCh->dioModeNum;
    }

    sumNum = dioModeNum / MAX_VOLT_NUM;
     
    if(0 == sumNum)
    {
        ic_Num = 0; 
        
        /*采样点压值判断断线*/
        if(1 == LinxDioOffFlag)
        {
            for(id = 1; id <= pCh->dioModeNum; id++)
            {
                cell_Num = id - 1;
                DiagCheckCellVOffLFlagAPI(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
                WriteSampCellVoltHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/      
            }
            
//            if(1 == (LinxdioModeNum - pCh->dioModeNum))//最后一个电芯断线
//            {
//                id = LinxdioModeNum;
//                cell_Num = id - 1;
//                DiagCheckCellVOffLFlagAPI(ic_Num, cell_Num, (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
//                WriteSampCellVoltHook(ic_Num, cell_Num, (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/                 
//            }
//            else if(1 == pCh->dioModeNum)//正向第二个ID开始断线
//            {
//                for(id = 2; id <= LinxdioModeNum; id++)//清除异常的ID信息
//                {
//                    cell_Num = id - 1;
//                    DiagCheckCellVOffLFlagAPI(ic_Num, (LinxdioModeNum - cell_Num), (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
//                    WriteSampCellVoltHook(ic_Num, (LinxdioModeNum - cell_Num), (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/                     
//                }
//            }
//            else
            {
                for(id = (LinxdioModeNum - pCh->dioModeNum + 1); id <= LinxdioModeNum; id++)//清除异常的ID信息
                {
                    cell_Num = id - 1;
                    DiagCheckCellVOffLFlagAPI(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
                    //WriteSampCellVoltHook(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/                  
                	InputBsuCellVolt(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), 0);
				}
            }
			SetLinxSampVAlarmFlagAPI(ic_Num,1);
        }
        else
        {
		  	SetLinxSampVAlarmFlagAPI(ic_Num,0);
            for(id = 1;id <= dioModeNum; id++)
            {
                cell_Num = id - 1;
                DiagCheckCellVOffLFlagAPI(ic_Num, (LinxdioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
                WriteSampCellVoltHook(ic_Num, (LinxdioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/ 
            }
        }               
    }
    else
    {        
        //for(ic_Num = 0; ic_Num < (sumNum+1); ic_Num++)
        for(ic_Num = 0; ic_Num < sumNum; ic_Num++)
        {
            /*采样点压值判断断线*/
            if(1 == LinxDioOffFlag)
            {                     
                for(id = (1 + MAX_VOLT_NUM* ic_Num); id <= pCh->dioModeNum; id++)
                {
                    cell_Num = (id - 1) - (MAX_VOLT_NUM*ic_Num);
                    DiagCheckCellVOffLFlagAPI(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
                    WriteSampCellVoltHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/      
                }  
                    
                for(id = (LinxdioModeNum - pCh->dioModeNum + 1); id <= LinxdioModeNum; id++)//清除异常的ID信息
                {
                    cell_Num = (id - 1) - (MAX_VOLT_NUM*ic_Num);
                    DiagCheckCellVOffLFlagAPI(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[0][id]);   /*电压采样线断线检测*/
                    //WriteSampCellVoltHook(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[0][id]);       /*输出电压值*/                  
                	InputBsuCellVolt(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), 0);
				} 
				SetLinxSampVAlarmFlagAPI(ic_Num,1);
            }
            else
            {
			  	SetLinxSampVAlarmFlagAPI(ic_Num,0);
                for(id = 0; id < MAX_VOLT_NUM; id++)
                {  
                    cell_Num = LinxdioModeNum - (id  + (MAX_VOLT_NUM*ic_Num));
                    
                    if(LinxdioModeNum > (id  + (MAX_VOLT_NUM*ic_Num)))
                    {
                        /*采样点压值判断断线*/
                        DiagCheckCellVOffLFlagAPI(ic_Num, id, (u16)LinxCalBufferCH0[0][cell_Num]);   /*电压采样线断线检测*/
                        
                        WriteSampCellVoltHook(ic_Num, id, (u16)LinxCalBufferCH0[0][cell_Num]);       /*输出电压值*/
                    }
                    else
                    {
                        /*采样点压值判断断线*/
                        DiagCheckCellVOffLFlagAPI(ic_Num, id, 0);   /*电压采样线断线检测*/
                        
                        //WriteSampCellVoltHook(ic_Num, id, 0);       /*输出电压值*/     
						InputBsuCellVolt(ic_Num, id, 0);
                    }
                }                
            }
        }
    }  
}

/*=================================================================================================
** @Name      : static void Linx_SampleCellVoltMsg_Output(void)
** @Input     : void
** @Output    : void
** @Function  : 输出温度采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_SampleCellTempMsg_Output(void)
{
    u8 id = 0;
    u8 ch = 0;
    u8 sumNum = 0;
    L_CHL_t* pCh = NULL; 
    u8 ic_Num = 0;
    u8 cell_Num = 0;
    u8 dioModeNum = 0;
    static u8 LinxDioOffFlag = 0;//菊花链有断线异常
    static u16 IcStateErr[LINX_CHIP_NUM] = {0};   //IC状态异常计数
        
    pCh = linx_get_chlPtr(ch);
    
    if(pCh == NULL)
    {
        return;
    }
    
    if(1 == IcStateErrFlag[1])
    {
        //判断断线，并清除原始采样值
        for(id = 1; id <= pCh->dioModeNum; id++)
        {
            //if(L_DIO_ERR_ZERO == IcStateBuf[id])
            if(L_DIO_ERR_ZERO == IcStateBuf[id-1])//原因待查，debug过程中少一个
            {
                if(IcStateErr[id] >= 1) //异常达到次数
                {
                    IcStateErr[id] = 0;
                    //ClrAfeSampCellVolt(id);
                    ClrAfeSampCellTemp(id);
                    //IcStateErrFlag[1] = 1;
                    //break;
                }
                IcStateErr[id]++;//错误计数        
            }
        }
        IcStateErrFlag[1] = 0;
    }
    
    if(LinxdioModeNum != pCh->dioModeNum)
    {
        //dioModeNum = LinxdioModeNum;
        dioModeNum = LINX_BOARD_MAXNUM;//最多250个ID
        LinxDioOffFlag = 1;
    }
    else
    {
        LinxDioOffFlag = 0;
        dioModeNum = pCh->dioModeNum;
    }    
    
    sumNum = dioModeNum / MAX_TEMP_NUM;
     
    if(0 == sumNum)
    {
        ic_Num = 0; 
//        for(id = 1;id <= dioModeNum; id++)
//        {     
//            cell_Num = id - 1;
//           
////            DiagCheckCellTOffLFlagAPI(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);   /*单体温度采样线断线检测*/
////            WriteSampCellTempHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);       /*输出温度值*/
//           
//            DiagCheckCellTOffLFlagAPI(ic_Num, cell_Num, (u16)LinxCalBufferCH0[1][id]);   /*单体温度采样线断线检测*/
//            WriteSampCellTempHook(ic_Num, cell_Num, (u16)LinxCalBufferCH0[1][id]);       /*输出温度值*/
//        }
        
        /*采样点压值判断断线*/
        if(1 == LinxDioOffFlag)
        {
            for(id = 1; id <= pCh->dioModeNum; id++)
            {
                cell_Num = id - 1;
                DiagCheckCellTOffLFlagAPI(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);   /*单体温度采样线断线检测*/
                WriteSampCellTempHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);       /*输出温度值*/    
            }
            
            for(id = (LinxdioModeNum - pCh->dioModeNum + 1); id <= LinxdioModeNum; id++)//清除异常的ID信息
            {
                cell_Num = id - 1;
                DiagCheckCellTOffLFlagAPI(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[1][id]);   /*单体温度采样线断线检测*/
                //WriteSampCellTempHook(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[1][id]);       /*输出温度值*/         
            	InputBsuCellTemp(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), -41);
			}
			SetLinxSampTAlarmFlagAPI(ic_Num,1);
        }
        else
        {
		  	SetLinxSampTAlarmFlagAPI(ic_Num,0);
            for(id = 1;id <= dioModeNum; id++)
            {
                cell_Num = id - 1;
                DiagCheckCellTOffLFlagAPI(ic_Num, (LinxdioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);   /*单体温度采样线断线检测*/
                WriteSampCellTempHook(ic_Num, (LinxdioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);       /*输出温度值*/
            }
        }             
    }
    else
    {        
        //for(ic_Num = 0; ic_Num < (sumNum+1); ic_Num++)
        for(ic_Num = 0; ic_Num < sumNum; ic_Num++)
        {
            if(1 == LinxDioOffFlag)
            {                     
                for(id = (1 + MAX_TEMP_NUM* ic_Num); id <= pCh->dioModeNum; id++)
                {
                    cell_Num = (id - 1) - (MAX_TEMP_NUM*ic_Num);
                    DiagCheckCellTOffLFlagAPI(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);   /*电压采样线断线检测*/
                    WriteSampCellTempHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)LinxCalBufferCH0[1][id]);       /*输出电压值*/      
                }  
                    
                for(id = (LinxdioModeNum - pCh->dioModeNum + 1); id <= LinxdioModeNum; id++)//清除异常的ID信息
                {
                    cell_Num = (id - 1) - (MAX_TEMP_NUM*ic_Num);
                    DiagCheckCellTOffLFlagAPI(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[1][id]);   /*电压采样线断线检测*/
                    //WriteSampCellTempHook(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), (u16)LinxCalBufferCH0[1][id]);       /*输出电压值*/                  
                	InputBsuCellTemp(ic_Num, (LinxdioModeNum - 1 - cell_Num + (LinxdioModeNum - pCh->dioModeNum)), -41);
				}
				SetLinxSampTAlarmFlagAPI(ic_Num,1);
            }
            else
            {
			  	SetLinxSampTAlarmFlagAPI(ic_Num,0);
                for(id = 0; id < MAX_TEMP_NUM; id++)
                {  
                    cell_Num = LinxdioModeNum - (id  + (MAX_TEMP_NUM*ic_Num));
                    
                    if(LinxdioModeNum > (id  + (MAX_TEMP_NUM*ic_Num)))
                    {
                        /*采样点压值判断断线*/
                        DiagCheckCellTOffLFlagAPI(ic_Num, id, (u16)LinxCalBufferCH0[1][cell_Num]);   /*电压采样线断线检测*/
                        
                        WriteSampCellTempHook(ic_Num, id, (u16)LinxCalBufferCH0[1][cell_Num]);       /*输出电压值*/
                    }
                    else
                    {
                        /*采样点压值判断断线*/
                        DiagCheckCellTOffLFlagAPI(ic_Num, id, -41);   /*温度采样线断线检测*/
                        
                        //WriteSampCellTempHook(ic_Num, id, -41);       /*输出温度值*/
						InputBsuCellTemp(ic_Num, id, -41);
						
                    }
                }                 
            }            
        }
    }  
}

/*=================================================================================================
** @Name      : static void Linx_SampleCellZmMsg_Output(void)
** @Input     : void
** @Output    : void
** @Function  : 输出阻抗采集信息
** @The notes : 
**===============================================================================================*/
static void Linx_SampleCellZmMsg_Output(void)
{
    L_CHL_t* pCh = NULL; 
    u8 id = 0;
    u8 ch = 0;
    u8 ic_Num = 0;
    u8 cell_Num = 0;
    u8 sumNum = 0;
    u8 dioModeNum = 0;
    static u16 IcStateErr[LINX_CHIP_NUM] = {0};   //IC状态异常计数
        
    pCh = linx_get_chlPtr(ch);
    
    if(pCh == NULL)
    {
        return;
    } 
    
    if(1 == IcStateErrFlag[2])
    {
        //判断断线，并清除原始采样值
        for(id = 1; id <= pCh->dioModeNum; id++)
        {
            //if(L_DIO_ERR_ZERO == IcStateBuf[id])
            if(L_DIO_ERR_ZERO == IcStateBuf[id-1])//原因待查，debug过程中少一个
            {
                if(IcStateErr[id] >= 1) //异常达到次数
                {
                    IcStateErr[id] = 0;
                    //ClrAfeSampCellVolt(id);
                    ClrAfeSampCellZm(id);
                    //IcStateErrFlag[2] = 1;
                    //break;
                }
                IcStateErr[id]++;//错误计数        
            }
        }
        IcStateErrFlag[2] = 0;
    }
    
    if(LinxdioModeNum != pCh->dioModeNum)
    {
        //dioModeNum = LinxdioModeNum;
        dioModeNum = LINX_BOARD_MAXNUM;//最多250个ID
    }
    else
    {
        dioModeNum = pCh->dioModeNum;
    }    
    
    sumNum = dioModeNum / MAX_ZM_NUM;
    //SEGGER_RTT_printf(0,"dioModeNum = %d\r\n",dioModeNum); 
    if(0 == sumNum)
    {
        ic_Num = 0; 
		 for(id = 1;id <= pCh->dioModeNum; id++)
        {     
            //cell_Num = pCh->dioModeNum - id; //3 2 1 0
           	cell_Num = id - 1; //3 2 1 0
            //WriteSampCellZmHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), pCh->zrArr[id]);       /*输出阻抗实部值*/
            WriteSampCellZmHook(ic_Num, (pCh->dioModeNum - 1 - cell_Num), (u16)(pCh->zrArr[id]/10));       /*输出阻抗实部值*/
			//SEGGER_RTT_printf(0,"ic_Num = %d cell_Num = %d zeizu = %d\r\n",ic_Num,cell_Num, (u16)(pCh->zrArr[cell_Num]/10)); 
        } 
    }
    else
    {        
        for(ic_Num = 0; ic_Num < sumNum; ic_Num++)
        {
            for(id = 0; id < MAX_ZM_NUM; id++)
            {  
                cell_Num = LinxdioModeNum - (id  + (MAX_ZM_NUM*ic_Num));
                
                if((LinxdioModeNum > (id  + (MAX_ZM_NUM*ic_Num))) && (0 == GetLinxZmSampAlarmFlagAPI()))/*阻抗采样正常*/
                {
                    WriteSampCellZmHook(ic_Num, id, (u16)(pCh->zrArr[cell_Num]/10));    /*输出阻抗实部值*/
                }
                else
                {
                    WriteSampCellZmHook(ic_Num, id, 65535);           /*输出阻抗实部值*/   
                }
            }            
        }        
    }     
}

///*=================================================================================================
//** @Name      : static void Linx_FindNeedBanlanID(void)
//** @Input     : void
//** @Output    : void
//** @Function  : 查找需要开启均衡的电池电压
//** @The notes : 
//**===============================================================================================*/
//static void Linx_FindNeedBanlanID(void)
//{
//    u8 ch = 0;
//    u8 index = 0;
//    u8 sumNum = 0;
//    u8 ic_Num = 0;
//    L_CHL_t* pCh = NULL;     
//        
//    pCh = linx_get_chlPtr(ch);
//  
//    if(pCh == NULL)
//    {
//        return;
//    }   
//    
//    if(0 == ((pCh->dioModeNum) % MAX_VOLT_NUM))
//    {
//        sumNum = pCh->dioModeNum / MAX_VOLT_NUM;
//    }
//    else
//    {
//        sumNum = pCh->dioModeNum / MAX_VOLT_NUM + 1;
//    }
//    
//    for(ic_Num = 0; ic_Num < sumNum; ic_Num++)      
//    {
//        for(index = 0; index < MAX_VOLT_NUM; index++)
//        {
//            if(1 == BitGet(gCellBCtrl_86[ic_Num][e86BCtrl_PasDhg], index))
//            {
//                LinxDNB110xBalanVolt[index + ic_Num*MAX_VOLT_NUM] = BALANCE_VOLT_ON;
//            }
//            else
//            {
//                LinxDNB110xBalanVolt[index + ic_Num*MAX_VOLT_NUM] = BALANCE_VOLT_OFF;
//            }
//        }
//    }
//}

//#endif


/*=================================================================================================
** @Name      : u8 gGetLinxAfeSumNum(void)
** @Input     : void
** @Output    : 菊花链枚举读取的ID总数
** @Function  : 获取菊花链枚举读取的ID总数
** @The notes : 
**===============================================================================================*/
u8 gGetLinxAfeSumNum(void)
{
    u8 ch = 0;
    L_CHL_t* pCh = NULL;    
    pCh = linx_get_chlPtr(ch);
    
    if(pCh == NULL)
    {
        return 0;
    }
    
    return(pCh->dioModeNum);
}

////测试代码
//void SetDNBSampStepAPI(u8 step)
//{
//    sSampStep = step;
//}
//
//u8 GetDNBSampStepAPI(void)
//{
//    return(sSampStep);
//}
    
