/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : CapEnerCorr.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : 总容量总电能自动标定模块
** @Instructions :
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "GroupInfo.h"
#include "GroupPara.h"
#include "ParaInterface.h"
#include "EnerInterface.h"
#include "SocInterface.h"
#include "CapEnerCorr.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
static u8 sCapEnerDropExpFlag = 0;  /*总容量总能量衰减异常标志*/

static t_CapECorr sCapECorrVHP;     /*总容量总能量检测记录高压点信息*/
static t_CapECorr sCapECorrVLP;     /*总容量总能量检测记录低压点信息*/

extern const u16 cTempCapPercent[]; /*不同温度下可用总容量百分比(0.1%)*/
extern const u16 cTempEnerPercent[];/*不同温度下可用总能量百分比(0.1%)*/
extern const u16 cCurrCapPercent[]; /*不同电流下可用总容量百分比(0.1%)*/
extern const u16 cCurrEnerPercent[];/*不同电流下可用总能量百分比(0.1%)*/

extern u16 gGBmuGenPara_102[];		/*主控通用参数值*/

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/
/*=============================================================================================
** @Name      : static void ChgCorrTotalCapEnerDeal(void)
** @Input     : void
** @Output    : void
** @Function  : 充电总容量总能量自动标定处理
** @The notes :
**===========================================================================================*/
static void ChgCorrTotalCapEnerDeal(void);

/*=============================================================================================
** @Name      : static void DhgCorrTotalCapEnerDeal(void)
** @Input     : void
** @Output    : void
** @Function  : 放电总容量总能量自动标定处理
** @The notes :
**===========================================================================================*/
static void DhgCorrTotalCapEnerDeal(void);

/*=============================================================================================
** @Name      : static void ClrAllCapEnerCorrInfo(void)
** @Input     : void
** @Output    : void
** @Function  : 清除所有总容量总能量检测记录信息
** @The notes : 清除EEPROM并复位当前检测值
**===========================================================================================*/
static void ClrAllCapEnerCorrInfo(void);

/*=============================================================================================
** @Name      : static u8 CheckHisCapEnerClrState(void)
** @Input     : void
** @Output    : 0:未清零 1已清零
** @Function  : 检测记录高低压点后历史电量是否清零
** @The notes : 被清零后原记录点无效
**===========================================================================================*/
static u8 CheckHisCapEnerClrState(void);

/*=============================================================================================
** @Name      : static u32 CalcTotalCapEnerByCTCorr(u8 sort, u32 data, s16 curr, s8 temp)
** @Input     : sort:类型(0:总容量 1:总能量) data:修正前检测到的总容量(1mAH)或总能量(1Wh) curr:标定电流(0.1A) temp:标定温度(1℃)
** @Output    : 电流温度修正后的总容量(1mAH)总能量(1Wh)
** @Function  : 计算总容量或总能量标定值经电流和温度修正后的真实值
** @The notes : 根据总容量标定终止时的电流和温度
**===========================================================================================*/
static u32 CalcTotalCapEnerByCTCorr(u8 sort, u32 data, s16 curr, s8 temp);

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : void TotalCapEnerCorrInit(void)
** @Input     : void
** @Output    : void
** @Function  : 总容量总能量自动标定初始化
** @The notes :
**===========================================================================================*/
void TotalCapEnerCorrInit(void)
{
    u32 eepInfo[17] = {0};

    //读取EEP记录信息
    if(TRUE == SocReadCapEnerCorrInfoHook(eepInfo, 17))
    {
		//低压点
		sCapECorrVLP.effect = eepInfo[1];
		sCapECorrVLP.timer = eepInfo[2];
		sCapECorrVLP.chgCap = eepInfo[3];
		sCapECorrVLP.dhgCap = eepInfo[4];
		sCapECorrVLP.chgEner = eepInfo[5];
		sCapECorrVLP.dhgEner = eepInfo[6];
		sCapECorrVLP.allCap = eepInfo[7];
		sCapECorrVLP.allEner = eepInfo[8];

		//高压点
		sCapECorrVHP.effect = eepInfo[9];
		sCapECorrVHP.timer = eepInfo[10];
		sCapECorrVHP.chgCap = eepInfo[11];
		sCapECorrVHP.dhgCap = eepInfo[12];
		sCapECorrVHP.chgEner = eepInfo[13];
		sCapECorrVHP.dhgEner = eepInfo[14];
		sCapECorrVHP.allCap = eepInfo[15];
		sCapECorrVHP.allEner = eepInfo[16];

    }
    else
    {
		//记录无效
		sCapECorrVHP.effect = 0;
		sCapECorrVLP.effect = 0;
    }
}

/*=============================================================================================
** @Name      : void TotalCapEnerCorrTask(void)
** @Input     : void
** @Output    : void
** @Function  : 电池组总容量总能量自动标定任务
** @The notes : 1s
**===========================================================================================*/
void TotalCapEnerCorrTask(void)
{
    u16 state = 0;
    static u8 sFlag = 0;

    if((eMODE_EMERG == GetGWorkModeAPI())       //非正常模式不检测
    	|| (eMODE_DEBUG == GetGWorkModeAPI())   //非正常模式不检测
		|| (1 == CheckHisCapEnerClrState()))    //发生历史电量被清零
    {
        if(0 == sFlag)
        {
        	ClrAllCapEnerCorrInfo();            //清除记录的容量检测信息
            sFlag = 1;
        }
    }
    else if(1 == EnerGetGroupBattReadyHook())   //电池数据就绪
    {
        sFlag = 0;

        state = GetGChgDhgStateAPI();           //充放电状态

        if(eCURR_DHG != state)                  //正在充电(或空闲)
        {
        	ChgCorrTotalCapEnerDeal();
        }

        if(eCURR_CHG != state)                 //正在放电(或空闲)
        {
        	DhgCorrTotalCapEnerDeal();
        }
    }
}

/*=============================================================================================
** @Name      : u8 GetCapEnerDropExpAPI(void)
** @Input     : void
** @Output    : 容量衰减异常标志(Bit0:总容量检测衰减 bit1:总能量衰减)[0正常 1异常]
** @Function  : 获取总容量总能量衰减异常标志
** @The notes :
**===========================================================================================*/
u8 GetCapEnerDropExpAPI(void)
{
    return(sCapEnerDropExpFlag);
}

/*=============================================================================================
** @Static function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : static void ChgCorrTotalCapEnerDeal(void)
** @Input     : void
** @Output    : void
** @Function  : 充电总容量总能量自动标定处理
** @The notes :
**===========================================================================================*/
static void ChgCorrTotalCapEnerDeal(void)
{
    u32 allCap = 0;
    u32 chgCap = 0;
    u32 dhgCap = 0;
    u32 calcCap = 0;
    u32 allEner = 0;
    u32 chgEner = 0;
    u32 dhgEner = 0;
    u32 calcEner = 0;
    s16 finCurr = 0;
    s8 finTemp = 0;
    static u32 sTime = 0;

    if(eCURR_CHG == GetGChgDhgStateAPI())                                       //正在充电
    {
        sCapECorrVHP.timer++;                                                   //计算充电时长
    }

    if((1 == EnerGetGroupVHLimStateHook())                                      //充电到高压截止点
    	|| (1 == GetSocSoeGSyncChgDhgFinState()))                               //并机运行其他组满充导致充电截止
    {
        //记录高压点信息
        sCapECorrVHP.effect = 1;                                                //高压记录点有效
        sCapECorrVHP.chgCap = GetGHisChgCapAPI();                               //高压时充电电量
        sCapECorrVHP.dhgCap = GetGHisDhgCapAPI();                               //高压时放电电量
        sCapECorrVHP.chgEner = GetGHisChgEnerAPI();                             //高压时充电电能
        sCapECorrVHP.dhgEner = GetGHisDhgEnerAPI();                             //高压时放电电能
        SocWriteCapEnerCorrInfoHook(1, (u32*)&sCapECorrVHP, 6);                 //保存高压点信息

        if(1 == sCapECorrVLP.effect)                                            //已记录低压点
        {
            if(IsInside(CAP_CHECK_MIN_T, sCapECorrVHP.timer, CAP_CHECK_MAX_T))  //充电时长有效
            {
                allCap = GetGroupTotalCapAPI();                                 //获得总容量(mAH)
                chgCap = sCapECorrVHP.chgCap - sCapECorrVLP.chgCap;             //低压到高压过程的充电电量
                chgCap = chgCap * CAP_CHG_DHG_PER / 1000;                       //根据充放电容量比计算对应的有效充电容量
                dhgCap = sCapECorrVHP.dhgCap - sCapECorrVLP.dhgCap;             //低压到高压过程的放电电量(中间可能有放电过程)


                if(chgCap > dhgCap)                                             //计算容量正确(充电量大)
                {
                    calcCap = (chgCap - dhgCap) * 1000;                         //计算的实际充电容量(mAH)

                    calcCap = CalcTotalCapEnerByCTCorr(0, calcCap, finCurr, finTemp);//计算电流温度修正后的充电容量(mAH)

                    //计算充电容量与放电容量误差20%
                    if(1 == IsInside((sCapECorrVLP.allCap * 80), (calcCap * 100), (sCapECorrVLP.allCap * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcCap = (calcCap + sCapECorrVLP.allCap) / 2;         //计算需修正的总容量
                    }
                    //计算充电容量与原容量误差20%
                    else if(1 == IsInside((allCap * 80), (calcCap * 100), (allCap * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);

                        if(allCap != GetGroupStandCapAPI())                     //非第1次修正
                        {
                            calcCap = (calcCap + allCap) / 2;                   //计算需修正的总容量(取1/2)
                        }
                        else                                                    //第1次修正
                        {
                            calcCap = (calcCap * 20 + allCap * 12) / 32;        //计算需修正的总容量(取2/3)
                        }
                    }
                    //本次计算充电容量与上次计算充电电容量误差20%
                    else if(1 == IsInside((sCapECorrVHP.allCap * 80), (calcCap * 100), (sCapECorrVHP.allCap * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcCap = (calcCap + sCapECorrVHP.allCap) / 2;         //计算需修正的总容量
                    }
                    //计算第1次修正充电容量与原容量误差70%
                    else if((1 == IsInside((allCap * 70), (calcCap * 100), (allCap * 130)))
                        && (allCap == GetGroupStandCapAPI()))                   //未修正过总容量
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcCap = (calcCap * 20 + allCap * 12) / 32;            //计算需修正的总容量(取2/3)
                    }
                    //本次计算总容量错误
                    else
                    {
                        calcCap = 0;                                            //总容量清零

                        BitSet(sCapEnerDropExpFlag, 0);                         //计算容量衰减异常
                    }

                    if(0 == BitGet(sCapEnerDropExpFlag, 0))                     //测定总容量正常
                    {
                        if((ABS(calcCap, allCap) > (allCap / 100))              //修正量超过1%
                            && (allCap != GetGroupStandCapAPI()))               //非第一次修正
                        {
                            if(calcCap > allCap)
                            {
                                calcCap = allCap + (allCap / 100);              //只取修正1%
                            }
                            else
                            {
                                calcCap = allCap - (allCap / 100);              //只取修正1%
                            }
                        }

                        CorrGroupTotalCapAPI(calcCap);                          //修正总容量
                    }
                }

                allEner = GetGroupTotalEnerAPI();                               //获得总能量(1Wh)
                chgEner = sCapECorrVHP.chgEner - sCapECorrVLP.chgEner;          //低压到高压过程的充电电能
                chgEner = chgEner * ENER_CHG_DHG_PER / 1000;                    //根据充放电能量比计算对应的有效充电能量
                dhgEner = sCapECorrVHP.dhgEner - sCapECorrVLP.dhgEner;          //低压到高压过程的放电电能(中间可能有放电过程)

                if(chgEner > dhgEner)                                           //计算能量正确(充电量大)
                {
                    calcEner = (chgEner - dhgEner) * 100;                       //计算的实际充电能量(Wh)

                    calcEner = CalcTotalCapEnerByCTCorr(1, calcEner, finCurr, finTemp);//计算电流温度修正后的充电能量(Wh)

                    //计算充电能量与放电能量误差20%
                    if(1 == IsInside((sCapECorrVLP.allEner * 80), (calcCap * 100), (sCapECorrVLP.allEner * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 1);
                        calcEner = (calcEner + sCapECorrVLP.allEner) / 2;       //计算需修正的总能量
                    }
                    //计算充电能量与原能量误差20%
                    else if(1 == IsInside((allEner * 80), (calcEner * 100), (allEner * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 1);

                        if(allEner != GetGroupStandEnerAPI())                   //非第1次修正
                        {
                            calcEner = (calcEner + allEner) / 2;                //计算需修正的总能量(取1/2)
                        }
                        else                                                    //第1次修正
                        {
                            calcEner = (calcEner * 20 + allEner * 12) / 32;     //计算需修正的总能量(取2/3)
                        }
                    }
                    //本次计算充电能量与上次计算充电电能量误差20%
                    else if(1 == IsInside((sCapECorrVHP.allEner * 80), (calcEner * 100), (sCapECorrVHP.allEner * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 1);
                        calcEner = (calcEner + sCapECorrVHP.allEner) / 2;       //计算需修正的总能量
                    }
                    //计算第1次修正充电能量与原能量误差70%
                    else if((1 == IsInside((allEner * 70), (calcEner * 100), (allEner * 130)))
                        && (allEner == GetGroupStandEnerAPI()))                 //未修正过总能量
                    {
                        BitClr(sCapEnerDropExpFlag, 1);
                        calcEner = (calcEner * 20 + allEner * 12) / 32;         //计算需修正的总能量(取2/3)
                    }
                    //本次计算总能量错误
                    else
                    {
                        calcEner = 0;                                           //总能量清零

                        BitSet(sCapEnerDropExpFlag, 1);                         //计算能量衰减异常
                    }

                    if(0 == BitGet(sCapEnerDropExpFlag, 1))                     //测定总能量正常
                    {
                        if((ABS(calcEner, allEner) > (allEner / 100))           //修正量超过1%
                            && (allEner != GetGroupStandEnerAPI()))             //非第一次修正
                        {
                            if(calcEner > allEner)
                            {
                                calcEner = allEner + (allEner / 100);           //只取修正1%
                            }
                            else
                            {
                                calcEner = allEner - (allEner / 100);           //只取修正1%
                            }
                        }

                        //CorrGroupTotalEnerAPI(calcEner);                      //修正总能量
                        CorrGroupStandEnerAPI(calcCap);                         //修正标称能量(充电能量大当做标称容量)
                    }
                }
            }

            //记录高压点计算的总容量(无法计算或计算错误时为0)
            sCapECorrVHP.allCap = calcCap;                                     //更新计算到的充电容量)
            sCapECorrVHP.allEner = calcEner;                                   //更新计算到的充电能量
            SocWriteCorrTotalCapEnerHook(1, (u32*)&sCapECorrVHP.allCap, 2);    //保存高压点充电容量充电能量

            //到高压点后则下一步等待低压，因此需清除低压点,防止再次高压时误修正
            sCapECorrVLP.effect = 0;
            sCapECorrVLP.timer = 0;
            SocWriteCapEnerCorrInfoHook(0, (u32*)&sCapECorrVLP, 2);            //保存低压点信息
        }
    }
    //非充电截止
    else
    {
        finCurr = GetGSampOutCurrAPI();
        finTemp = GetGCellAvgTempAPI();

        if(sCapECorrVHP.timer >= (sTime + CAP_CHECK_RECORD_T))
        {
        	SocWriteCapEnerCorrTimeHook(1, sCapECorrVHP.timer);                 //每1min记录一次充电时间
        }
    }
}

/*=============================================================================================
** @Name      : static void DhgCorrTotalCapEnerDeal(void)
** @Input     : void
** @Output    : void
** @Function  : 放电总容量总能量自动标定处理
** @The notes :
**===========================================================================================*/
static void DhgCorrTotalCapEnerDeal(void)
{
    u32 allCap = 0;
    u32 chgCap = 0;
    u32 dhgCap = 0;
    u32 calcCap = 0;
    u32 allEner = 0;
    u32 chgEner = 0;
    u32 dhgEner = 0;
    u32 calcEner = 0;
    s16 finCurr = 0;
    s8 finTemp = 0;
    static u32 sTime = 0;

    if(eCURR_DHG == GetGChgDhgStateAPI())                                       //正在放电
    {
        sCapECorrVLP.timer++;                                                   //计算放电时长
    }

    if((1 == EnerGetGroupVLLimStateHook())                                      //放电到低压截止点
    	|| (2 == GetSocSoeGSyncChgDhgFinState()))                               //并机运行其他组满放导致放电截止
    {
        //记录低压点
        sCapECorrVLP.effect = 1;                                                //低压记录点有效
        sCapECorrVLP.chgCap = GetGHisChgCapAPI();                               //低压时充电电量
        sCapECorrVLP.dhgCap = GetGHisDhgCapAPI();                               //低压时放电电量
        sCapECorrVLP.chgEner = GetGHisChgEnerAPI();                             //高压时充电电能
        sCapECorrVLP.dhgEner = GetGHisDhgEnerAPI();                             //高压时放电电能
        SocWriteCapEnerCorrInfoHook(0, (u32*)&sCapECorrVLP, 6);                 //保存低压点信息

        if(1 == sCapECorrVHP.effect)                                            //已记录高压点
        {
            if(IsInside(CAP_CHECK_MIN_T, sCapECorrVLP.timer, CAP_CHECK_MAX_T))  //放电时长有效
            {
                allCap = GetGroupTotalCapAPI();                                 //获得总容量(mAH)
                dhgCap = sCapECorrVLP.dhgCap - sCapECorrVHP.dhgCap;             //高压到低压过程的放电电量
                chgCap = sCapECorrVLP.chgCap - sCapECorrVHP.chgCap;             //高压到低压过程的充电电量(中间可能有充电过程)
                chgCap = chgCap * CAP_CHG_DHG_PER / 1000;                       //根据充放电容量比计算对应的有效充电容量

                if(dhgCap > chgCap)                                             //计算容量正确(放电量大)
                {
                    calcCap = (dhgCap - chgCap) * 1000;                         //计算的实际放电容量(mAH)

                    calcCap = CalcTotalCapEnerByCTCorr(0, calcCap, finCurr, finTemp);//计算电流温度修正后的放电容量(mAH)

                    //计算放电容量与充电容量误差20%
                    if(1 == IsInside((sCapECorrVHP.allCap * 80), (calcCap * 100), (sCapECorrVHP.allCap * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcCap = (calcCap + sCapECorrVHP.allCap) / 2;         //计算需修正的总容量
                    }
                    //计算放电容量与原容量误差20%
                    else if(1 == IsInside((allCap * 80), (calcCap * 100), (allCap * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);

                        if(allCap != GetGroupStandCapAPI())                     //非第1次修正
                        {
                            calcCap = (calcCap + allCap) / 2;                   //计算需修正的总容量(取1/2)
                        }
                        else                                                    //第1次修正
                        {
                            calcCap = (calcCap * 20 + allCap * 12) / 32;        //计算需修正的总容量(取2/3)
                        }
                    }
                    //本次计算放电容量与上次计算放电电容量误差20%
                    else if(1 == IsInside((sCapECorrVLP.allCap * 80), (calcCap * 100), (sCapECorrVLP.allCap * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcCap = (calcCap + sCapECorrVLP.allCap) / 2;          //计算需修正的总容量
                    }
                    //计算第1次修正放电容量与原容量误差50%
                    else if((1 == IsInside((allCap * 70), (calcCap * 100), (allCap * 130)))
                        && (allCap == GetGroupStandCapAPI()))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcCap = (calcCap * 20 + allCap * 12) / 32;            //计算需修正的总容量(取2/3)
                    }
                    //本次计算总容量错误
                    else
                    {
                        calcCap = 0;
                        BitSet(sCapEnerDropExpFlag, 0);                         //记录容量衰减异常
                    }

                    if(0 == BitGet(sCapEnerDropExpFlag, 0))
                    {
                        if((ABS(calcCap, allCap) > (allCap / 100))              //修正量超过1%
                            && (allCap != GetGroupStandCapAPI()))               //非第一次修正
                        {
                            if(calcCap > allCap)
                            {
                                calcCap = allCap + (allCap / 100);              //只取修正1%
                            }
                            else
                            {
                                calcCap = allCap - (allCap / 100);              //只取修正1%
                            }
                        }

                        CorrGroupTotalCapAPI(calcCap);                          //修正总容量
                    }
                }

                allEner = GetGroupTotalEnerAPI();                               //获得总能量(Wh)
                dhgEner = sCapECorrVLP.dhgEner - sCapECorrVHP.dhgEner;          //高压到低压过程的放电电能
                chgEner = sCapECorrVLP.chgEner - sCapECorrVHP.chgEner;          //高压到低压过程的充电电能(中间可能有充电过程)
                chgEner = chgEner * ENER_CHG_DHG_PER / 1000;                    //根据充放电能量比计算对应的有效充电能量

                if(dhgEner > chgEner)                                           //计算容量正确(放电量大)
                {
                    calcEner = (dhgEner - chgEner) * 100;                       //计算的实际放电能量(Wh)

                    calcEner = CalcTotalCapEnerByCTCorr(1, calcEner, finCurr, finTemp);//计算电流温度修正后的放电能量(Wh)

                    //计算放电能量与原能量误差20%
                    if(1 == IsInside((allEner * 80), (calcEner * 100), (allEner * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);

                        if(allEner != GetGroupStandEnerAPI())                   //非第1次修正
                        {
                            calcEner = (calcEner + allEner) / 2;                //计算需修正的总能量(取1/2)
                        }
                        else                                                    //第1次修正
                        {
                            calcEner = (calcEner * 20 + allEner * 12) / 32;     //计算需修正的总能量(取2/3)
                        }
                    }
                    //本次计算放电能量与上次计算放电电能量误差20%
                    else if(1 == IsInside((sCapECorrVLP.allEner * 80), (calcEner * 100), (sCapECorrVLP.allEner * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcEner = (calcEner + sCapECorrVLP.allEner) / 2;       //计算需修正的总能量
                    }
                    //计算放电能量与充电能量误差20%
                    else if(1 == IsInside((sCapECorrVHP.allEner * 80), (calcEner * 100), (sCapECorrVHP.allEner * 120)))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        //calcEner = (calcEner + sCapECorrVHP.allEner) / 2;     //计算需修正的总能量
                        calcEner = (calcEner * 20 + sCapECorrVHP.allEner * 12) / 32;//计算需修正的总能量(取2/3)
                    }
                    //计算第1次修正放电能量与原能量误差30%
                    else if((1 == IsInside((allEner * 70), (calcEner * 100), (allEner * 130)))
                        && (allEner == GetGroupStandEnerAPI()))
                    {
                        BitClr(sCapEnerDropExpFlag, 0);
                        calcEner = (calcEner * 20 + allEner * 12) / 32;         //计算需修正的总能量(取2/3)
                    }
                    //本次计算总能量错误
                    else
                    {
                        calcEner = 0;
                        BitSet(sCapEnerDropExpFlag, 0);                         //记录能量衰减异常
                    }

                    if(0 == BitGet(sCapEnerDropExpFlag, 0))
                    {
                        if((ABS(calcEner, allEner) > (allEner / 100))           //修正量超过1%
                            && (allEner != GetGroupStandEnerAPI()))             //非第一次修正
                        {
                            if(calcEner > allEner)
                            {
                                calcEner = allEner + (allEner / 100);           //只取修正1%
                            }
                            else
                            {
                                calcEner = allEner - (allEner / 100);           //只取修正1%
                            }
                        }

                        CorrGroupTotalEnerAPI(calcEner);                        //修正总容量
                    }
                }
            }

            //记录低压点计算的总容量(无法计算或计算错误时为0)
            sCapECorrVLP.allCap = calcCap;                                      //更新计算到的放电容量
            sCapECorrVLP.allEner = calcEner;                                    //更新计算到的放电能量
            SocWriteCorrTotalCapEnerHook(0, (u32*)&sCapECorrVLP.allCap, 2);     //保存高压点放电容量放电能量

            //到低压点后则下一步等待高压，因此需清除高压点,防止再次低压时误修正
            sCapECorrVHP.effect = 0;
            sCapECorrVHP.timer = 0;
            SocWriteCapEnerCorrInfoHook(1, (u32*)&sCapECorrVHP, 2);             //清除高压点信息
        }
    }
    else
    {
        finCurr = GetGSampOutCurrAPI();
        finTemp = GetGCellAvgTempAPI();

        if(sCapECorrVLP.timer >= (sTime + CAP_CHECK_RECORD_T))
        {
        	SocWriteCapEnerCorrTimeHook(0, sCapECorrVLP.timer);                 //每1min记录一次放电时间
        }
    }
}

/*=============================================================================================
** @Name      : static void ClrAllCapEnerCorrInfo(void)
** @Input     : void
** @Output    : void
** @Function  : 清除所有总容量总能量检测记录信息
** @The notes : 清除EEPROM并复位当前检测值
**===========================================================================================*/
static void ClrAllCapEnerCorrInfo(void)
{
    //异常标志
	sCapEnerDropExpFlag = 0;

    //低压点
    sCapECorrVLP.effect = 0;
    sCapECorrVLP.timer = 0;
    sCapECorrVLP.chgCap = 0;
    sCapECorrVLP.dhgCap = 0;
    sCapECorrVLP.allCap = 0;
    sCapECorrVLP.chgEner = 0;
    sCapECorrVLP.dhgEner = 0;
    sCapECorrVLP.allEner = 0;

    //高压点
    sCapECorrVHP.effect = 0;
    sCapECorrVHP.timer = 0;
    sCapECorrVHP.chgCap = 0;
    sCapECorrVHP.dhgCap = 0;
    sCapECorrVHP.allCap = 0;
    sCapECorrVHP.chgEner = 0;
    sCapECorrVHP.dhgEner = 0;
    sCapECorrVHP.allEner = 0;

    //初始化EEPROM数据为0
    SocWriteCapEnerCorrInfoHook(0, (u32*)&sCapECorrVLP, 8);
    SocWriteCapEnerCorrInfoHook(1, (u32*)&sCapECorrVHP, 8);
}

/*=============================================================================================
** @Name      : static u8 CheckHisCapEnerClrState(void)
** @Input     : void
** @Output    : 0:未清零 1已清零
** @Function  : 检测记录高低压点后历史电量是否清零
** @The notes : 被清零后原记录点无效
**===========================================================================================*/
static u8 CheckHisCapEnerClrState(void)
{
    u32 hisChgCap = 0;
    u32 hisDhgCap = 0;
    u32 hisChgEner = 0;
    u32 hisDhgEner = 0;

    //允许记录值误差10AH/10kWh
    hisChgCap = (GetGHisChgCapAPI() + 10) * 1000;
    hisDhgCap = (GetGHisDhgCapAPI() + 10) * 1000;
    hisChgEner = (GetGHisChgEnerAPI() + 100) * 100;
    hisDhgEner = (GetGHisDhgEnerAPI() + 100) * 100;

    //已记录高压点
    if(1 == sCapECorrVHP.effect)
    {
        //记录值比累计值小(说明累计值被清零)
        if((sCapECorrVHP.chgCap > hisChgCap)
            || (sCapECorrVHP.dhgCap > hisDhgCap)
            || (sCapECorrVHP.chgEner > hisChgEner)
            || (sCapECorrVHP.dhgEner > hisDhgEner))
        {
        	//历史值已经过清零
            return(1);
        }
    }

    if(1 == sCapECorrVLP.effect)
    {
    	//记录值比累计值小(说明累计值被清零)
		if((sCapECorrVLP.chgCap > hisChgCap)
			|| (sCapECorrVLP.dhgCap > hisDhgCap)
			|| (sCapECorrVLP.chgEner > hisChgEner)
			|| (sCapECorrVLP.dhgEner > hisDhgEner))
        {
        	//历史值已经过清零
            return(1);
        }
    }

    return(0);
}

/*=============================================================================================
** @Name      : static u32 CalcTotalCapEnerByCTCorr(u8 sort, u32 data, s16 curr, s8 temp)
** @Input     : sort:类型(0:总容量 1:总能量) data:修正前检测到的总容量(1mAH)或总能量(1Wh) curr:标定电流(0.1A) temp:标定温度(1℃)
** @Output    : 电流温度修正后的总容量(1mAH)总能量(1Wh)
** @Function  : 计算总容量或总能量标定值经电流和温度修正后的真实值
** @The notes : 根据总容量标定终止时的电流和温度
**===========================================================================================*/
static u32 CalcTotalCapEnerByCTCorr(u8 sort, u32 data, s16 curr, s8 temp)
{
	u8 index = 0;
	u16 currPer = 0;
    u32 corrPer = 0;
    u32 aftData = 0;
    const u16 *perTab = NULL;

    /*修正总容量*/
    if(0 == sort)
    {
    	perTab = cCurrCapPercent;
    }
    /*修正总能量*/
    else if(1 == sort)
    {
    	perTab = cCurrEnerPercent;
    }
    else
    {
    	return(data);
    }

    //根据标称容量计算当前电流倍率(0.1C)
    currPer = (u32)ABS(0, curr) * 1000 / GetGroupStandCapAPI();

	/*将电流倍率转化为百分率表索引([0.2C,2C]对应0~9)*/
	if(currPer <= (CAPENER_PER_FST_C / 2))
	{
		/*电流很小不修正*/
		corrPer = 1000;
	}
	else if(currPer <= CAPENER_PER_FST_C)
	{
		/*电流在起始点*/
		index = 0;
		corrPer = perTab[index];
	}
	else if(currPer >= ((s8)CAPENER_PER_FST_C + (CAPENER_PER_SUB_C * (CAPENER_C_PER_LEN - 1))))
	{
		index = CAPENER_C_PER_LEN - 1;
		corrPer = perTab[index];
	}
	else
	{
		/*取下端*/
		index = (currPer - CAPENER_PER_FST_T) / CAPENER_PER_SUB_C;
		corrPer = perTab[index] + (((s16)perTab[index + 1] - (s16)perTab[index]) * ((currPer - CAPENER_PER_FST_C) % CAPENER_PER_SUB_C) / CAPENER_PER_SUB_C);
	}

	/*求真实值*/
	aftData = data * 1000 / corrPer;

	/*将温度转化为百分率表索引([-30,60]对应0~9)*/
	if(temp <= (s8)CAPENER_PER_FST_T)
	{
		index = 0;
		corrPer = perTab[index];
	}
	else if(temp >= ((s8)CAPENER_PER_FST_T + (CAPENER_PER_SUB_T * (CAPENER_T_PER_LEN - 1))))
	{
		index = CAPENER_T_PER_LEN - 1;
		corrPer = perTab[index];
	}
	else
	{
		/*取下端*/
		index = (temp + (0 - (s8)CAPENER_PER_FST_T)) / CAPENER_PER_SUB_T;
		corrPer = perTab[index] + (((s16)perTab[index + 1] - (s16)perTab[index]) * ((temp + (0 - (s8)CAPENER_PER_FST_T)) % CAPENER_PER_SUB_T) / CAPENER_PER_SUB_T);
	}

	/*求真实值*/
	aftData = aftData * 1000 / corrPer;

    return(aftData);
}
