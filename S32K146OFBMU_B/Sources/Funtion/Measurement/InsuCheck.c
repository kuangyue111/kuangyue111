/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : InsuCheck.c
** @Founder      : ZSY342
** @Date         : 2019.10.30
** @Function     : 绝缘检测模块
** @Instructions :
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "LTC2949.h"
#include "PublicPara.h"
#include "GroupPara.h"
#include "GroupInfo.h"
#include "DEVGPIO.h"
#include "HigAFESamp.h"
#include "DiagInterface.h"
#include "TacInterface.h"
#include "TacUser.h"
#include "InsuCheck.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
extern u16 gGFstLeakInfo_67[];			/*直流电路信息数组*/
extern u16 gSysGenParaRO_100[];         /*系统通用设置*/
extern u16 gGBmuGenPara_102[];  		/*主控通用参数值*/
extern u16 gGBmuHigLevPara_103[];       /*主控高级参数值*/

t_IsoCalcMsg IsoCalcMsg;				/*绝缘检测计算信息结构体*/
t_AdhesionCalcMsg AdhesionCalcMsg;		/*接触器粘连检测计算信息结构体*/

static u8 sInsuVoltSampFlag = 0; 		/*绝缘电压采样标志 0:不进行采样  1:片选Vpgnd-V1电压 2:Vpgnd-V1电压采样    3:片选Vpgnd-V2电压 4:Vpgnd-V2电压采样 5:Vpgnd电压采样完成*/
static u8 sInsuErrStateFlag = 0;       	/*绝缘状态标志 0:正常 1：一级故障(严重) 2:二级故障 (轻微) 3:绝缘检测异常(失效)*/
static u8 sSwhAdhesDiagFlag = 0;  		/*接触器粘连检测标志 0:初始化 1:设置继电器    2:Link+/Link-采样 3:采样完成  4:检测完成*/
static u8 sSwhAdhesErrFlag = 0;       	/*接触器粘连异常标志 bit0:总正接触器  bit1：预充接触器  bit2:总负接触器*/

static u8 sInsuCheckFinFlag = 0; 		/*绝缘检测完成标志 0:未完成  1:完成*/
static u8 sSwhAdhesFinFlag = 0;  		/*接触器粘连检测完成标志 0:未完成  1:完成*/

float IsoRp = ISORp_Inval_Value;		/*正极对地绝缘阻抗(默认无效值3000K)*/
float IsoRn = ISORn_Inval_Value;		/*负极对地绝缘阻抗(默认无效值3000K)*/

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/
/*=============================================================================================
** @Name      : static u8 SysNeedInsuCheckState(void)
** @Input     : void
** @Output    : TRUE:需要 FALSE:不需要
** @Function  : 判断系统是否需要进行绝缘检测
** @The notes :
**===========================================================================================*/
static u8 SysNeedInsuCheckState(void);

/*=============================================================================================
** @Name      : static u8 SysNeedSwhAdhesCheckState(void)
** @Input     : void
** @Output    : TRUE:需要 FALSE:不需要
** @Function  : 判断系统是否需要进行接触器粘连检测
** @The notes : 只有外端无电压时才能检测
**===========================================================================================*/
static u8 SysNeedSwhAdhesCheckState(void);

/*=============================================================================================
** @Name      : static void InsuErrStateCheck(u16 isoRp, u16 isoRn)
** @Input     : isoRp:正极对地绝缘值  1KΩ isoRn:负极对地绝缘值 1KΩ
** @Output    : void
** @Function  : 判断正负极对地的绝缘值是否超出阈值
** @The notes :
**===========================================================================================*/
static void InsuErrStateCheck(u16 isoRp, u16 isoRn);

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : void InsuAdhesMeasurInit(void)
** @Input     : void
** @Output    : void
** @Function  : 绝缘采样和接触器粘连检测采样控制模块初始化
** @The notes : 初始化调用
**===========================================================================================*/
void InsuAdhesMeasurInit(void)
{
	/*断开绝缘检测PGND继电器*/
	DEVGPIOSetPortState(ePOut_ISO_PGND, ePSTATE_OFF);

	gGFstLeakInfo_67[eFstLeakInfo67_PasInsu] = (u16)ISORp_Inval_Value;	/*正极对地绝缘阻抗(默认无效值3000K)*/
	gGFstLeakInfo_67[eFstLeakInfo67_NegInsu] = (u16)ISORn_Inval_Value;	/*负极对地绝缘阻抗(默认无效值3000K)*/
}

/*=============================================================================================
** @Name      : void void InsuAdhesMeasurTask(void)
** @Input     : void
** @Output    : void
** @Function  : 绝缘采样和接触器粘连采样控制任务
** @The notes : 100ms周期函数
**===========================================================================================*/
void InsuAdhesMeasurTask(void)
{
	static u8 state = 0;
	static s8 diagNum = 0;

	/*预充电压计算中间变量*/
	s32 preVolt = 0;

	/*绝缘计算中间变量*/
	float IsoAuxValueA = 0;
	float IsoAuxValueB = 0;
	float IsoAuxValueC = 0;

	switch(state)
	{
		/*检测是否需要绝缘检测或接触器粘连检测*/
		case 0:
			/*系统需要进行绝缘检测*/
			if(TRUE == SysNeedInsuCheckState())
			{
				/*开始片选绝缘电压V1采样使能*/
				SetInsuVpgndSampFlagAPI(eIsoVpgnd_SeleV1);
				diagNum++;
				state = 2;
			}

			/*系统需要进行接触器粘连检测*/
			if(TRUE == SysNeedSwhAdhesCheckState())
			{
				/*开始片选接触器粘连检测电压采样使能*/
				SetSwhAdhesDiagFlagAPI(eAdhVlink_SeleV1);
				diagNum++;
				state = 1;
			}
			break;

		/*接触器粘连检测*/
		case 1:
			/*LTC2949复位*/
			if(eAdhVlink_Init == GetSwhAdhesDiagFlagAPI())
			{
				/*返回重新设置采样*/
				diagNum = 0;
				state = 2;
				return;
			}
			/*等待LTC2949电压Link+、Link-采样完成*/
			else if(eAdhVlink_SampOK != GetSwhAdhesDiagFlagAPI())
			{
				/*需要检测绝缘*/
				if(diagNum >= 2)
				{
					/*进入绝缘检测*/
					state = 2;
				}
				/*未启动绝缘检测*/
				else
				{
					/*判断是否需要进行绝缘检测*/
					if(TRUE == SysNeedInsuCheckState())
					{
						/*开始片选绝缘电压V1采样使能*/
						SetInsuVpgndSampFlagAPI(eIsoVpgnd_SeleV1);
						diagNum++;
						state = 2;
					}
				}
				return;
			}

//			/*主负接触器闭合预充电压取Link+电压*/
//			if((ePSTATE_ON == TacGetGroupSwhCtrlStateAPI(eGroupSwh_Neg))
//				|| ((ABS(GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link1), GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link2)) > 20)//有电压
//				&& (ABS(GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link1), GetGDcSideSampRealInfoAPI(eReferSampInfo200_BattHV)) < ABS((GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link1) - GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link2)), GetGDcSideSampRealInfoAPI(eReferSampInfo200_BattHV)))))
//			{
//				/*通过Link+采样值计算预充电压 0.1V*/
//				preVolt = GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link1);
//			}
//			/*主负接触器断开预充电压取Link+-Link-电压*/
//			else
//			{
//				/*通过Link+、Link-采样值计算预充电压 0.1V*/
//				preVolt = GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link1) - GetGDcSideSampRealInfoAPI(eReferSampInfo200_Link2);
//			}
//			/*记录预充电压0.01V转 0.1V*/
//			InputGroupHVSideData(HIG_AFE_PreV, preVolt * 10);
//
//        	/*设置预充采样完成标志*/
//			SampSetPreVSampFinStateAPI(1);

			/*只有接触器全断开才能检测接触器粘连*/
	  		if(TRUE == TacGetGroupRunSwhAllOffStateAPI())
			{
				/*采集负载电压时接触器有闭合*/
				if(AdhesionCalcMsg.SwhState > 0)
				{
					/*有接触器闭合负载电压按照粘连(预充)公式计算*/
					preVolt = Ltc2949ChangeADtoVolt(HIG_AFE_PreV, AdhesionCalcMsg.SampLoadV);	/*转换成高压 0.01V*/

					/*输入预充电压*/
					InputGroupHVSideData(HIG_AFE_PreV, preVolt);

					/*设置预充采样完成标志*/
					SampSetOutHVSampFinStateAPI(1);
				}
				/*采样前后接触器一直断开*/
				else
				{
					/*VLINK-＜Vref(3V),则主负粘连*/
					/*VLINK-=2.973,则主负粘连*/
					//if(AdhesionCalcMsg.SampLinkV2 < 3.0)
					if((AdhesionCalcMsg.SampLinkV2 >= (Adhesion_Link2_Vref - Adhesion_Link2_Voff))
						&& (AdhesionCalcMsg.SampLinkV2 < (Adhesion_Link2_Vref + Adhesion_Link2_Voff)))
					{
						/*使能接触器粘连检测*/
						if(1 == BitGet(gGBmuHigLevPara_103[eBmuHigLevPara103_AdhMonitType], 0))
						{
							/*总负接触器粘连*/
							BitSet(sSwhAdhesErrFlag, eGroupSwh_Neg);
						}

						/*有接触器闭合负载电压按照粘连(预充)公式计算*/
						preVolt = Ltc2949ChangeADtoVolt(HIG_AFE_PreV, AdhesionCalcMsg.SampLoadV);	/*转换成高压 0.01V*/

						/*输入预充电压*/
						InputGroupHVSideData(HIG_AFE_PreV, preVolt);

						/*设置预充采样完成标志*/
						SampSetOutHVSampFinStateAPI(1);

						/*VLINK+＞Vref(3V),差距为5mV左右，则主正可能粘连*/
						//if(AdhesionCalcMsg.SampLinkV1 > 3.0)
						if(AdhesionCalcMsg.SampLinkV1 > Adhesion_Link1_Vref)
						{
							/*使能接触器粘连检测*/
							if(1 == BitGet(gGBmuHigLevPara_103[eBmuHigLevPara103_AdhMonitType], 0))
							{
								/*总正接触器粘连*/
								//BitSet(sSwhAdhesErrFlag, eGroupSwh_Mai);
							}

							/*比较负载电压与总压大小关系才能确定是否真正粘连*/
							/*负载电压与总压很接近(压差≤10%Vsum)*/
							if(ABS(GetGDcSideSampRealInfoAPI(eReferSampInfo200_PChgHV), GetGDcSideSampRealInfoAPI(eReferSampInfo200_BattHV)) <= (GetGDcSideSampRealInfoAPI(eReferSampInfo200_BattHV) / 10))
							{
								/*使能接触器粘连检测*/
								if(1 == BitGet(gGBmuHigLevPara_103[eBmuHigLevPara103_AdhMonitType], 0))
								{
									/*总正接触器粘连*/
									BitSet(sSwhAdhesErrFlag, eGroupSwh_Mai);
								}
							}
							else
							{
								/*总正接触器正常*/
								BitClr(sSwhAdhesErrFlag, eGroupSwh_Mai);
							}
						}
						/*VLINK+=Vref(3V)*/
						else
						{
							/*总正接触器正常*/
							BitClr(sSwhAdhesErrFlag, eGroupSwh_Mai);

							/*确认无负载电压*/
							/*输入预充电压*/
							InputGroupHVSideData(HIG_AFE_PreV, 0);
						}
					}
					/*VLINK-=Vref(3V)*/
					/*VLINK-≠2.973V*/
					else
					{
						/*总负接触器正常*/
						BitClr(sSwhAdhesErrFlag, eGroupSwh_Neg);

						/*VLINK+＞Vref(3V),差距为5mV左右，则主正粘连*/
						//if(AdhesionCalcMsg.SampLinkV1 > 3.0)
						if(AdhesionCalcMsg.SampLinkV1 > Adhesion_Link1_Vref)
						{
							/*使能接触器粘连检测*/
							if(1 == BitGet(gGBmuHigLevPara_103[eBmuHigLevPara103_AdhMonitType], 0))
							{
								/*主正接触器粘连*/
								BitSet(sSwhAdhesErrFlag, eGroupSwh_Mai);
							}

							/*有接触器闭合负载电压按照粘连(预充)公式计算(待优化,需根据闭合Rl1/Rl2后的VLink-该电压是否真实负载电压)*/
							preVolt = Ltc2949ChangeADtoVolt(HIG_AFE_PreV, AdhesionCalcMsg.SampLoadV);	/*转换成高压 0.01V*/

							/*输入预充电压*/
							InputGroupHVSideData(HIG_AFE_PreV, preVolt);

							/*设置预充采样完成标志*/
							SampSetOutHVSampFinStateAPI(1);
						}
						/*VLINK+=Vref(3V)*/
						else
						{
							/*主正接触器正常*/
							BitClr(sSwhAdhesErrFlag, eGroupSwh_Mai);

							/*主正\主负均未粘连,负载电压按照非粘连(负载)公式计算*/
							preVolt = Ltc2949ChangeADtoVolt(HIG_AFE_LoadV, AdhesionCalcMsg.SampLoadV);	/*转换成高压 0.01V*/

							/*输入负载电压*/
							InputGroupHVSideData(HIG_AFE_LoadV, preVolt);

							/*设置预充采样完成标志*/
							SampSetOutHVSampFinStateAPI(1);
						}
					}
				}
			}
			/*有接触器闭合*/
			else
			{
				/*采集负载电压时接触器已经闭合*/
				if(AdhesionCalcMsg.SwhState > 0)
				{
					/*有接触器闭合负载电压按照粘连(预充)公式计算*/
					preVolt = Ltc2949ChangeADtoVolt(HIG_AFE_PreV, AdhesionCalcMsg.SampLoadV);	/*转换成高压 0.01V*/

					/*输入预充电压*/
					InputGroupHVSideData(HIG_AFE_PreV, preVolt);

					/*设置预充采样完成标志*/
					SampSetOutHVSampFinStateAPI(1);
				}
			}

			/*设置粘连检测完成标志*/
	  		sSwhAdhesFinFlag = 1;
			SetSwhAdhesDiagFlagAPI(eAdhVlink_DiagOK);

			diagNum--;
			/*需要检测绝缘*/
			if(diagNum > 0)
			{
				/*进入绝缘检测*/
				state = 2;
			}
			else
			{
				/*返回*/
				state = 0;
			}
			break;


		/*绝缘检测*/
		case 2:
			/*LTC2949复位*/
			if(eIsoVpgnd_Init == GetInsuVpgndSampFlagAPI())
			{
				/*返回重新设置采样*/
				diagNum = 0;
				state = 0;
				return;
			}
			/*等待LTC2949电压Vpgnd(两种情况)采样完成后计算绝缘值*/
			else if(eIsoVpgnd_SampOK != GetInsuVpgndSampFlagAPI())
			{
				/*需要检测接触器粘连*/
				if(diagNum >= 2)
				{
					/*进入触器粘连检测*/
					state = 1;
				}
				/*未启动接触器粘连检测*/
				else
				{
					/*判断是否需要进行接触器粘连检测*/
					if(TRUE == SysNeedSwhAdhesCheckState())
					{
						/*开始片选接触器粘连检测电压采样使能*/
						SetSwhAdhesDiagFlagAPI(eAdhVlink_SeleV1);
						diagNum++;
						state = 1;
					}
				}
				return;
			}

			/*总压大于48V才进行绝缘判断*/
			if(IsoCalcMsg.SampVbat >= ISO_MIN_BAT_V)
			{
				/*计算正极对地、负极对地绝缘阻抗*/
				IsoRp = (IsoCalcMsg.CalcCoeB - IsoCalcMsg.CalcCoeA) * ISO_R2 * 2 / (2 + IsoCalcMsg.CalcCoeA);

				//IsoRn = 2*(IsoCalcMsg.CalcCoeB - IsoCalcMsg.CalcCoeA) * (2*ISO_R2 + ISO_R1) * ISO_R2
				//		/ (2*IsoCalcMsg.CalcCoeA * (3*ISO_R2 + ISO_R1) - IsoCalcMsg.CalcCoeB * (2*ISO_R2 - 2*ISO_R2*IsoCalcMsg.CalcCoeA - ISO_R1*IsoCalcMsg.CalcCoeA));

				IsoAuxValueA = 2*(IsoCalcMsg.CalcCoeB - IsoCalcMsg.CalcCoeA) * (2*ISO_R2 + ISO_R1) * ISO_R2;

				//IsoAuxValueA = (u32)(IsoAuxValueA * 100);/*只保留三位小数点,防止越界*/
				//IsoAuxValueA = IsoAuxValueA / 100;

				IsoAuxValueB = 2*IsoCalcMsg.CalcCoeA * (3*ISO_R2 + ISO_R1);

				//IsoAuxValueB = (u32)(IsoAuxValueB * 100);/*只保留三位小数点,防止越界*/
				//IsoAuxValueB = IsoAuxValueB / 100;

				IsoAuxValueC = 2*ISO_R2 - 2*ISO_R2*IsoCalcMsg.CalcCoeA - ISO_R1*IsoCalcMsg.CalcCoeA;

				//IsoAuxValueC = (u32)(IsoAuxValueC * 100);/*只保留三位小数点,防止越界*/
				//IsoAuxValueC = IsoAuxValueC / 100;

				if((0 == IsoAuxValueB) && (0 == (IsoCalcMsg.CalcCoeB * IsoAuxValueC)))/*除数不能为0*/
				{
					IsoRn = 0;
				}
				else
				{
					IsoRn = IsoAuxValueA / (IsoAuxValueB - IsoCalcMsg.CalcCoeB * IsoAuxValueC);
				}

				//if((IsoRp < 0) || (IsoRp > ISORn_Inval_Value))
				if(IsoRp > ISORn_Inval_Value)
				{
					IsoRp = ISORp_Inval_Value;
				}

				//if((IsoRn < 0) || (IsoRn > ISORn_Inval_Value))
				if(IsoRn > ISORn_Inval_Value)
				{
					IsoRn = ISORn_Inval_Value;
				}

				/*正极阻抗计算为负数*/
				if(IsoRp < 0)
				{
					/*电压与总压一致为正边阻值为0*/
					if(ABS(IsoCalcMsg.SampV1, IsoCalcMsg.SampVbat) < 5.0)
					{
						IsoRp = 0;
					}
					/*电压与总压不一样为正边计算错处*/
					else
					{
						IsoRp = ISORn_Inval_Value;
					}
				}

				/*负极阻抗计算为负数*/
				if(IsoRn < 0)
				{
					/*无电压为负边阻值为0*/
					if(IsoCalcMsg.SampV1 < 1.0)
					{
						IsoRn = 0;
					}
					/*有电压为负边计算错处*/
					else
					{
						IsoRn = ISORn_Inval_Value;
					}
				}

				/*正负阻抗同时为0需要通过电压分辨具体是哪边为0*/
				if((0 == IsoRp) && (0 == IsoRn))
				{
					/*无电压为负边为0*/
					if(IsoCalcMsg.SampV1 < 1.0)
					{
						IsoRp = ISORp_Inval_Value;
					}
					/*有电压为正边为0*/
					else
					{
						IsoRn = ISORn_Inval_Value;
					}
				}
			}
			/*电池总压低不进行绝缘检测*/
			else
			{
				/*显示默认值*/
				IsoRp = ISORp_Inval_Value;
				IsoRn = ISORn_Inval_Value;
			}

			/*输出计算绝缘值显示  1KΩ*/
			gGFstLeakInfo_67[eFstLeakInfo67_PasInsu] = (u16)IsoRp;
			gGFstLeakInfo_67[eFstLeakInfo67_NegInsu] = (u16)IsoRn;

			/*判断绝缘值是否超出告警值阈值*/
			InsuErrStateCheck((u16)IsoRp, (u16)IsoRn);

			/*设置绝缘检测完成标志*/
	  		sInsuCheckFinFlag = 1;
			SetInsuVpgndSampFlagAPI(eIsoVpgnd_DiagOK);

			diagNum--;
			/*需要检测接触器粘连*/
			if(diagNum > 0)
			{
				/*进入接触器粘连检测*/
				state = 1;
			}
			else
			{
				/*返回*/
				state = 0;
			}
			break;

		default:
			state = 0;
			diagNum = 0;
			break;
	}
}

/*=============================================================================================
** @Name      : void InputSwhAdhesSampData(e_AdhesSampChan chan, float data)
** @Input     : chan:通道    data：采样值
** @Output    : void
** @Function  : 输入接触器粘连检测采样值
** @The notes : Link+/Link-采样值
**===========================================================================================*/
void InputSwhAdhesSampData(e_AdhesSampChan chan, float data)
{
	if(chan > eAdhSamp_Num)
	{
		return;
	}

	switch(chan)
	{
		case eAdhSamp_LoadV:
			AdhesionCalcMsg.SampLoadV = (u32)(data * Isu_Calc_Dec_Places);/*只保留三位小数点*/
			AdhesionCalcMsg.SampLoadV = AdhesionCalcMsg.SampLoadV / Isu_Calc_Dec_Places;
			AdhesionCalcMsg.SwhState = GetGOutPortStateAPI() & 0x07;
			break;

		case eAdhSamp_Link1:
			AdhesionCalcMsg.SampLinkV1 = (u32)(data * Isu_Calc_Dec_Places);/*只保留三位小数点*/
			AdhesionCalcMsg.SampLinkV1 = AdhesionCalcMsg.SampLinkV1 / Isu_Calc_Dec_Places;
			break;

		case eAdhSamp_Link2:
			AdhesionCalcMsg.SampLinkV2 = (u32)(data * Isu_Calc_Dec_Places);/*只保留三位小数点*/
			AdhesionCalcMsg.SampLinkV2 = AdhesionCalcMsg.SampLinkV2 / Isu_Calc_Dec_Places;
			break;

		default:
			break;
	}
}

/*=============================================================================================
** @Name      : void UserReqSwhAdhesDiagAPI(void)
** @Input     : void
** @Output    : void
** @Function  : 设置用户请求接触器粘连检测
** @The notes :
**===========================================================================================*/
void UserReqSwhAdhesDiagAPI(void)
{
	/*清除粘连检测完成标志即重新开始检测*/
	sSwhAdhesFinFlag = 0;
}

/*=============================================================================================
** @Name      : void SetSwhAdhesDiagFlagAPI(e_AdhVlinkDiagStep flag)
** @Input     : flag:接触器粘连检测步骤标志 0:初始化 1:设置继电器    2:Link+/Link-采样 3:采样完成  4:检测完成
** @Output    : void
** @Function  : 设置接触器粘连检测步骤标志
** @The notes :
**===========================================================================================*/
void SetSwhAdhesDiagFlagAPI(e_AdhVlinkDiagStep flag)
{
	if(flag >= eAdhVlink_Fin)
	{
		return;
	}

//	/*进行过采样就已经完成继电器控制*/
//	if((eAdhVlink_DiagOK == sSwhAdhesDiagFlag)
//		&& (eAdhVlink_SeleV1 == flag))
//	{
//		/*直接进行V1采样*/
//		flag = eAdhVlink_SampV1;
//	}

	sSwhAdhesDiagFlag = flag;
}

/*=============================================================================================
** @Name      : u8 GetSwhAdhesDiagFlagAPI(void)
** @Input     : void
** @Output    : 接触器粘连检测步骤标志:0:初始化 1:设置继电器    2:Link+/Link-采样 3:采样完成  4:检测完成
** @Function  : 获取接触器粘连检测步骤标志
** @The notes :
**===========================================================================================*/
u8 GetSwhAdhesDiagFlagAPI(void)
{
    return(sSwhAdhesDiagFlag);
}

/*=============================================================================================
** @Name      : u8 GetSwhAdhesErrFlagAPI(void)
** @Input     : void
** @Output    : 按bit位表示接触器粘连异常状态:bit0:主正接触器 bit1:预充接触器 bit2:负极接触器 [0:正常 1:粘连]
** @Function  : 获取接触器粘连异常状态标志
** @The notes :
**===========================================================================================*/
u8 GetSwhAdhesErrFlagAPI(void)
{
    return(sSwhAdhesErrFlag);
}

/*=============================================================================================
** @Name      : void InputInsuSampDataAPI(e_InsuChan chan, float data)
** @Input     : chan:通道    data：采样电压值 1V
** @Output    : void
** @Function  : 记录绝缘检测需要的采样电压值数据
** @The notes : LTC2949高压采样调用
**===========================================================================================*/
void InputInsuSampDataAPI(e_InsuChan chan, float data)
{
	switch(chan)
	{
		case eInsu_Bat:
			if(data <= 0.001)
			{
				data = 0.001;
			}

			IsoCalcMsg.SampVbat = (u32)(data * Isu_Calc_Dec_Places);/*只保留三位小数点*/
			IsoCalcMsg.SampVbat = IsoCalcMsg.SampVbat / Isu_Calc_Dec_Places;
			break;

		case eInsu_V1:
			if(data <= 0.001)
			{
				data = 0.001;
			}

			IsoCalcMsg.SampV1 = (u32)(data * Isu_Calc_Dec_Places);/*只保留三位小数点*/
			IsoCalcMsg.SampV1 = IsoCalcMsg.SampV1 / Isu_Calc_Dec_Places;

			IsoCalcMsg.CalcCoeA = (IsoCalcMsg.SampVbat - IsoCalcMsg.SampV1) / IsoCalcMsg.SampV1;/*计算系数A*/
			IsoCalcMsg.CalcCoeA = (u32)(IsoCalcMsg.CalcCoeA * 10000);/*只保留四位小数点*/
			IsoCalcMsg.CalcCoeA = (float)IsoCalcMsg.CalcCoeA / 10000;
			break;

		case eInsu_V2:
			if(data <= 0.001)
			{
				data = 0.001;
			}

			IsoCalcMsg.SampV2 = (u32)(data * Isu_Calc_Dec_Places);/*只保留三位小数点*/
			IsoCalcMsg.SampV2 = IsoCalcMsg.SampV2 / Isu_Calc_Dec_Places;

			IsoCalcMsg.CalcCoeB = (IsoCalcMsg.SampVbat - IsoCalcMsg.SampV2) / IsoCalcMsg.SampV2;/*计算系数B*/
			IsoCalcMsg.CalcCoeB = (u32)(IsoCalcMsg.CalcCoeB * 10000);/*只保留四位小数点*/
			IsoCalcMsg.CalcCoeB = IsoCalcMsg.CalcCoeB / 10000;
			break;

		default:
			break;
	}
}

/*=============================================================================================
** @Name      : void UserReqInsuCheckAPI(void)
** @Input     : void
** @Output    : void
** @Function  : 设置用户请求绝缘检测
** @The notes :
**===========================================================================================*/
void UserReqInsuCheckAPI(void)
{
	/*清除绝缘检测完成标志即重新开始检测*/
	sInsuCheckFinFlag = 0;
}

/*=============================================================================================
** @Name      : void SetInsuVpgndSampFlagAPI(e_IsoVpgndSampStep flag)
** @Input     : flag:绝缘Vpgnd电压采样步骤标志 0:不进行采样  1:片选Vpgnd-V1电压 2:Vpgnd-V1电压采样    3:片选Vpgnd-V2电压 4:Vpgnd-V2电压采样 5:Vpgnd电压采样完成
** @Output    : void
** @Function  : 设置绝缘Vpgnd电压采样步骤标志(状态)
** @The notes :
**===========================================================================================*/
void SetInsuVpgndSampFlagAPI(e_IsoVpgndSampStep flag)
{
	if(flag >= eIsoVpgnd_Fin)
	{
		return;
	}

	/*进行过采样继电器且PGND继电器已经复位到V1状态*/
	if((eIsoVpgnd_DiagOK == sInsuVoltSampFlag)
		&& (eIsoVpgnd_SeleV1 == flag)
		&& (ePSTATE_ON == DEVGPIOGetOutPortState(ePOut_ISO_PGND)))
	{
		/*直接进行V1采样*/
		flag = eIsoVpgnd_SampV1;
	}

	sInsuVoltSampFlag = flag;
}

/*=============================================================================================
** @Name      : u8 GetInsuVpgndSampFlagAPI(void)
** @Input     : void
** @Output    : 绝缘Vpgnd电压采样步骤标志 0:不进行采样  1:片选Vpgnd-V1电压 2:Vpgnd-V1电压采样    3:片选Vpgnd-V2电压 4:Vpgnd-V2电压采样 5:Vpgnd电压采样完成 6:绝缘检测完成
** @Function  : 获取绝缘Vpgnd电压采样步骤标志
** @The notes :
**===========================================================================================*/
u8 GetInsuVpgndSampFlagAPI(void)
{
    return(sInsuVoltSampFlag);
}

/*=============================================================================================
** @Name      : u8 GetInsuErrStateFlagAPI(void)
** @Input     : void
** @Output    : 绝缘状态标志:0:正常 1：一级故障(严重) 2:二级故障 (轻微) 3:绝缘检测异常(失效)
** @Function  : 获取绝缘状态标志
** @The notes :
**===========================================================================================*/
u8 GetInsuErrStateFlagAPI(void)
{
    return(sInsuErrStateFlag);
}

/*=============================================================================================
** @Name      : void ClrInsuAdhesErrFlagAPI(void)
** @Input     : void
** @Output    : void
** @Function  : 清除接触器粘连和绝缘异常状态标志
** @The notes :
**===========================================================================================*/
void ClrInsuAdhesErrFlagAPI(void)
{
    sSwhAdhesErrFlag = 0;
    sInsuErrStateFlag = 0;
}

/*=============================================================================================
** @Name      : u8 GetInsuAdhesFinStateAPI(void)
** @Input     : void
** @Output    : 绝缘和接触器粘连检测第一遍检测完成状态  TRUE:完成 FALSE:未完成
** @Function  : 获取绝缘和接触器粘连检测第一遍检测完成状态
** @The notes :
**===========================================================================================*/
u8 GetInsuAdhesFinStateAPI(void)
{
	/*自身采高压时有效*/
	#if(1 == BMU_DC_VH_TYPE)
//	static u8 sHisFlag = 0;

//	/*已确认检测完成*/
//	if(1 == sHisFlag)
//	{
//		return(TRUE);
//	}
//	/*粘连检测完成*/
//	else if(eAdhVlink_DiagOK == sSwhAdhesDiagFlag)
//	{
//		/*有绝缘检测功能*/
//		if(gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType] > 0)
//		{
//			/*绝缘检测完成*/
//			if(eIsoVpgnd_DiagOK == sInsuVoltSampFlag)
//			{
//				/*确认检测完成*/
//				sHisFlag = 1;
//				return(TRUE);
//			}
//			/*绝缘检测未完成*/
//			else
//			{
//				return(FALSE);
//			}
//		}
//		/*无绝缘检测功能*/
//		else
//		{
//			/*确认检测完成*/
//			sHisFlag = 1;
//			return(TRUE);
//		}
//	}
//	/*粘连检测未完成*/
//	else
//	{
//		return(FALSE);
//	}

	/*绝缘检测未完成*/
	if(0 == sInsuCheckFinFlag)
	{
		return(FALSE);
	}

	/*粘连检测未完成*/
	if(0 == sSwhAdhesFinFlag)
	{
		return(FALSE);
	}
	#endif

	return(TRUE);
}

/*=============================================================================================
** @Static function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : static u8 SysNeedSwhAdhesCheckState(void)
** @Input     : void
** @Output    : TRUE:需要 FALSE:不需要
** @Function  : 判断系统是否需要进行接触器粘连检测
** @The notes : 包括需要采预充电压时
**===========================================================================================*/
static u8 SysNeedSwhAdhesCheckState(void)
{
	/*自身采高压时有效*/
	#if(1 == BMU_DC_VH_TYPE)
	static u32 sTime = 0;
	static u16 sSumVolt = 0;
	static u16 sPreVolt = 0;
	static u32 sChkTime = 0;
	static u32 sRunTime = 0;
	static u8 sHisState = 0;

	/*上电首次前5s需要检测*/
	if((0 == sTime) && (BSPGetOSMillSecTimeAPI() < 5000))
	{
		sTime = GetSysSecondTimeAPI();
		return(TRUE);
	}

	/*调试模式连续检测*/
	//非多组电池并机(可能其它组高压已闭合影响检测)
	if((eMODE_DEBUG == GetGWorkModeAPI())
    	&& (gSysGenParaRO_100[eSGenPara100_AllGNum] <= 1))
	{
		sTime = 0;
		return(TRUE);
	}

	/*使能接触器粘连检测*/
	if(1 == BitGet(gGBmuHigLevPara_103[eBmuHigLevPara103_AdhMonitType], 0))
	{
		/*高压采样异常不进行接触器粘连检测*/
		if(SampGetHigVSampExpFlagAPI() > 0)
		{
			sTime = 0;
			sSwhAdhesFinFlag = 1;
			return(FALSE);
		}

		/*未进行粘连检测(用户请求)*/
		if(0 == sSwhAdhesFinFlag)
		{
			/*首次检测*/
			if(0 == sChkTime)
			{
				/*记录开始检测时间*/
				sChkTime = GetSysSecondTimeAPI();

				/*确保运行状态也连续检测*/
				//sRunTime = GetSysSecondTimeAPI();
			}
			/*连续检测60s*/
			else if(ABS(GetSysSecondTimeAPI(), sChkTime) >= ADH_EXE_DIAGTIME)
			{
				/*记录本次检测时间(等待下次检测周期)*/
				sChkTime = 0;
				sTime = GetSysSecondTimeAPI();
			}
			return(TRUE);
		}

		/*正在初始化需持续采集外端预充电压*/
		if(eWORK_INIT == GetGWorkStateAPI())
		{
			sTime = 0;
			sRunTime = GetSysSecondTimeAPI();
			return(TRUE);
		}
		/*正在启动/正在停机*/
		else if((eWORK_START == GetGWorkStateAPI())
			|| (eWORK_STOP == GetGWorkStateAPI()))
		{
			sRunTime = GetSysSecondTimeAPI();

			/*单组非并机接触器未正负极全闭合需持续采集外端预充电压*/
			if((gSysGenParaRO_100[eSGenPara100_AllGNum] <= 1)
				&& (FALSE == TacGetGroupRunSwhAllOnStateAPI()))
			{
				sTime = 0;
				return(TRUE);
			}
			/*多组并机不能检接触器粘连(可能其它组高压已闭合影响检测)*/
			else if(gSysGenParaRO_100[eSGenPara100_AllGNum] > 1)
			{
				sTime = 0;
				return(FALSE);
			}
		}
		//运行状态
		else if(eWORK_RUN == GetGWorkStateAPI())
		{
			/*已运行60s不检测(充放电变化重新当作初始运行)*/
			if(ABS(GetSysSecondTimeAPI(), sRunTime) >= ADH_EXE_DIAGTIME)
			{
				/*充放电变化*/
				if(sHisState != GetGChgDhgStateAPI())
				{
					/*需要重新检测*/
					sRunTime = GetSysSecondTimeAPI();
				}

				sHisState = GetGChgDhgStateAPI();
				return(FALSE);
			}
			/*运行初始60s需要采集Link采样值*/
			else
			{
				sTime = 0;
				sHisState = GetGChgDhgStateAPI();
				return(TRUE);
			}
		}
		//接触器断开
		else
		{
			sRunTime = GetSysSecondTimeAPI();
		}

		/*有接触器闭合不需要进行粘连检测
		if(FALSE == TacGetGroupRunSwhAllOffStateAPI())
		{
			sTime = 0;
			return(FALSE);
		}*/

		/*总压变化50V或掉到0V检测1次*/
		if((ABS(GetGSampSumVoltAPI(), sSumVolt) >= 500)
			|| ((GetGSampSumVoltAPI() <= 10) && (sSumVolt > 10)))
		{
			sTime = GetSysSecondTimeAPI();
			sSumVolt = GetGSampSumVoltAPI();
			return(TRUE);
		}

		/*预充电压变化50V或从无变有或从有变无检测1次*/
		if((ABS(GetGSampPreVoltAPI(), sPreVolt) >= 500)
			|| ((GetGSampPreVoltAPI() > 100) && (sPreVolt <= 100))
			|| ((GetGSampPreVoltAPI() <= 100) && (sPreVolt > 100)))
		{
			sTime = GetSysSecondTimeAPI();
			sPreVolt = GetGSampPreVoltAPI();
			return(TRUE);
		}

		/*上电首次或达到正常检测周期*/
		if((0 == sTime) || (ABS(GetSysSecondTimeAPI(), sTime) >= gGBmuHigLevPara_103[eBmuHigLevPara103_AdhMonitCycl]))
		{
			/*首次检测*/
			if(0 == sChkTime)
			{
				/*记录开始检测时间*/
				sChkTime = GetSysSecondTimeAPI();
			}
			/*连续检测60s*/
			else if(ABS(GetSysSecondTimeAPI(), sChkTime) >= ADH_EXE_DIAGTIME)
			{
				/*记录本次检测时间(等待下次检测周期)*/
				sChkTime = 0;
				sTime = GetSysSecondTimeAPI();
			}
			return(TRUE);
		}

		/*报故障时达到故障检测周期*/
		if((sSwhAdhesErrFlag > 0) && (ABS(GetSysSecondTimeAPI(), sTime) >= ADH_ERR_DIAGTIME))
		{
			/*首次检测*/
			if(0 == sChkTime)
			{
				/*记录开始检测时间*/
				sChkTime = GetSysSecondTimeAPI();
			}
			/*连续检测60s*/
			else if(ABS(GetSysSecondTimeAPI(), sChkTime) >= ADH_EXE_DIAGTIME)
			{
				/*记录本次检测时间(等待下次检测周期)*/
				sChkTime = 0;
				sTime = GetSysSecondTimeAPI();
			}
			return(TRUE);
		}

		/*正在判定接触器异常时要持续检测*/
		if((eErr_A == DiagGetErrFilStateAPI(eERR_ID_K0MAI_EXP))
			&& (eErr_A == DiagGetErrFilStateAPI(eERR_ID_K1PRE_EXP))
			&& (eErr_A == DiagGetErrFilStateAPI(eERR_ID_K2NEG_EXP))
			&& (sSwhAdhesErrFlag > 0))
		{
			sTime = GetSysSecondTimeAPI();
			return(TRUE);
		}

		/*已开始检测需连续检测60s*/
		if(sChkTime > 0)
		{
			if(ABS(GetSysSecondTimeAPI(), sChkTime) >= ADH_EXE_DIAGTIME)
			{
				/*记录本次检测时间(等待下次检测周期)*/
				sChkTime = 0;
				sTime = GetSysSecondTimeAPI();
			}
			else
			{
				return(TRUE);
			}
		}
	}
	/*无粘连检测*/
	else
	{
		sSwhAdhesFinFlag = 1;
	}
	/*无自身高压采样*/
	#else
	sSwhAdhesFinFlag = 1;
	#endif

	return(FALSE);
}

/*=============================================================================================
** @Name      : static u8 SysNeedInsuCheckState(void)
** @Input     : void
** @Output    : TRUE:需要 FALSE:不需要
** @Function  : 判断系统是否需要进行绝缘检测
** @The notes :
**===========================================================================================*/
static u8 SysNeedInsuCheckState(void)
{
	static u8 sState = 0;
	static u16 sSumVolt = 0;
	static u16 sNegVolt = 0;
	static u32 sTime = 0;
	static u32 sChkTime = 0;

	/*绝缘检测功能禁止*/
	if(0 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])		/*不检测不上报绝缘*/
	{
		sTime = 0;
		sInsuErrStateFlag = 0;
		sInsuCheckFinFlag = 1;
		sState = GetGWorkStateAPI();
		return(FALSE);
	}

	/*非并机系统上电前5s需要检测*/
	if((0 == sTime) && (BSPGetOSMillSecTimeAPI() < 5000)
		&& (gSysGenParaRO_100[eSGenPara100_AllGNum] <= 1))
	{
		sState = GetGWorkStateAPI();
		sTime = GetSysSecondTimeAPI();
		return(TRUE);
	}

	/*调试模式连续检测*/
	//非多组电池并机(可能其它组高压已闭合影响检测)
	if((eMODE_DEBUG == GetGWorkModeAPI())
    	&& (gSysGenParaRO_100[eSGenPara100_AllGNum] <= 1))
	{
		sTime = 0;
		sState = GetGWorkStateAPI();
		return(TRUE);
	}

	/*绝缘检测功能检测条件*/
	if((SampGetHigVSampExpFlagAPI() > 0)							/*高压采样异常不进行绝缘检测*/
		/*|| (GetGSampSumVoltAPI() < (ISO_MIN_BAT_V * 10)*/)		/*48V以下不进行绝缘判断(需要继续采样监测PGND电压)*/
	{
		sTime = 0;
		sInsuErrStateFlag = 0;
		sInsuCheckFinFlag = 1;
		sState = GetGWorkStateAPI();
		return(FALSE);
	}

	/*未进行绝缘检测(用户请求)*/
	if(0 == sInsuCheckFinFlag)
	{
		/*首次检测*/
		if(0 == sChkTime)
		{
			/*记录开始检测时间*/
			sChkTime = GetSysSecondTimeAPI();
		}
		/*连续检测10s(已采集到的真实值则停止)*/
		else if((ABS(GetSysSecondTimeAPI(), sChkTime) >= 10)
			|| ((ABS(GetSysSecondTimeAPI(), sChkTime) >= 5)
			&& (ABS(GetGSampSumVoltAPI(), (GetGPgndNegVoltAPI() * 2)) <= 10)))
		{
			/*记录本次检测时间(等待下次检测周期)*/
			sChkTime = 0;
			sState = GetGWorkStateAPI();
			sTime = GetSysSecondTimeAPI();
		}
		return(TRUE);
	}

	/*正在判定绝缘时要持续检测(需要上报故障时)*/
	if((sInsuErrStateFlag > 0)
		&& (eErr_A == DiagGetErrFilStateAPI(eERR_ID_LEAK2))
		&& (eErr_A == DiagGetErrFilStateAPI(eERR_ID_LEAK1))
		&& (eErr_Norm != DiagGetFilErrSortAPI(eERR_ID_LEAK1)))
	{
		/*需要上报故障时(全程报故障或接触器全断开时报故障)*/
		if((5 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])
			|| (6 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])
			|| (7 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])
			|| (8 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])
			|| (((2 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])
			|| (4 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType]))
			&& (1 == TacGetGroupRunSwhAllOffStateAPI())))
		{
			sState = GetGWorkStateAPI();
			sTime = GetSysSecondTimeAPI();
			return(TRUE);
		}
	}

	/*总压或负母线电压电压变化10V或掉到0V检测1次*/
	if((ABS(GetGSampSumVoltAPI(), sSumVolt) >= 100)
		|| ((GetGSampSumVoltAPI() <= 10) && (sSumVolt > 10))
		|| (ABS(GetGPgndNegVoltAPI(), sNegVolt) >= 100)
		|| ((GetGPgndNegVoltAPI() <= 10) && (sNegVolt > 10))/*负母线电压为0可能负极漏电*/
		|| ((ABS(GetGPgndNegVoltAPI(), GetGSampSumVoltAPI()) <= 100)/*负母线电压与总压一致可能正极漏电*/
		&& (ABS(sNegVolt, GetGSampSumVoltAPI()) > 100)))
	{
		sSumVolt = GetGSampSumVoltAPI();
		sNegVolt = GetGPgndNegVoltAPI();
		sTime = GetSysSecondTimeAPI();
		sState = GetGWorkStateAPI();
		return(TRUE);
	}

	/*无异常时根据参数配置需要检测*/
	if((0 == sInsuErrStateFlag) && (0 == sChkTime))
	{
		/*只在接触器断开时检测*/
		if((1 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType]) 		//非运行检测不上报
			|| (2 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])) //非运行检测非运行上报
		{
			if((0 == TacGetGroupRunSwhAllOffStateAPI())                     //有运行接触器闭合
				|| (eWORK_RUN == GetGWorkStateAPI())                        //系统运行
				|| (eWORK_START == GetGWorkStateAPI())                      //系统启动
				|| (eWORK_STOP == GetGWorkStateAPI()))                      //系统停机
			{
				sTime = 0;
				sState = GetGWorkStateAPI();
				return(FALSE);
			}
		}
		/*只在运行时检测*/
		else if((6 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType]) //运行检测不上报
			|| (7 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType])) //运行检测上报
		{
			if(eWORK_RUN != GetGWorkStateAPI())                             //系统非运行
			{
				sTime = 0;
				sState = GetGWorkStateAPI();
				return(FALSE);
			}
		}
		/*只在需要时检测(非周期检测)*/
		else if(8 == gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitType]) //请求时检测上报
		{
			sTime = 0;
			sState = GetGWorkStateAPI();
			return(FALSE);
		}
	}

	/*系统状态变化需要检测*/
	if((sState != GetGWorkStateAPI())
		&& ((eWORK_IDLE == GetGWorkStateAPI())
		|| (eWORK_RUN == GetGWorkStateAPI())
		|| (eWORK_ERR == GetGWorkStateAPI())))
	{
		sState = GetGWorkStateAPI();
		sTime = GetSysSecondTimeAPI();
		return(TRUE);
	}

	/*达到检测周期(正在启动不开始检测)*/
	if((0 == sTime) || ((ABS(GetSysSecondTimeAPI(), sTime) >= gGBmuHigLevPara_103[eBmuHigLevPara103_LeakMonitCycl])
		&& (eWORK_START != GetGWorkStateAPI())))
	{
		/*首次检测*/
		if(0 == sChkTime)
		{
			/*记录开始检测时间*/
			sChkTime = GetSysSecondTimeAPI();
		}
		/*连续检测10s(已采集到的真实值则停止)*/
		else if((ABS(GetSysSecondTimeAPI(), sChkTime) >= 10)
			|| ((ABS(GetSysSecondTimeAPI(), sChkTime) >= 5)
			&& (ABS(GetGSampSumVoltAPI(), (GetGPgndNegVoltAPI() * 2)) <= 10)))
		{
			/*记录本次检测时间(等待下次检测周期)*/
			sChkTime = 0;
			sState = GetGWorkStateAPI();
			sTime = GetSysSecondTimeAPI();
		}
		return(TRUE);
	}

	/*报故障时达到故障检测周期*/
	if((sInsuErrStateFlag > 0) && (ABS(GetSysSecondTimeAPI(), sTime) >= ISO_ERR_DIAGTIME))
	{
		/*首次检测*/
		if(0 == sChkTime)
		{
			/*记录开始检测时间*/
			sChkTime = GetSysSecondTimeAPI();
		}
		/*连续检测10s(已采集到的真实值则停止)*/
		else if((ABS(GetSysSecondTimeAPI(), sChkTime) >= 10)
			|| ((ABS(GetSysSecondTimeAPI(), sChkTime) >= 5)
			&& (ABS(GetGSampSumVoltAPI(), (GetGPgndNegVoltAPI() * 2)) <= 10)))
		{
			/*记录本次检测时间(等待下次检测周期)*/
			sChkTime = 0;
			sState = GetGWorkStateAPI();
			sTime = GetSysSecondTimeAPI();
		}
		return(TRUE);
	}

	/*已开始检测需连续检测10s(已采集到的真实值则停止)*/
	if(sChkTime > 0)
	{
		if((ABS(GetSysSecondTimeAPI(), sChkTime) >= 10)
			|| ((ABS(GetSysSecondTimeAPI(), sChkTime) >= 5)
			&& (ABS(GetGSampSumVoltAPI(), (GetGPgndNegVoltAPI() * 2)) <= 10)))
		{
			/*记录本次检测时间(等待下次检测周期)*/
			sChkTime = 0;
			sTime = GetSysSecondTimeAPI();
		}
		else
		{

			sState = GetGWorkStateAPI();
			return(TRUE);
		}
	}

	sState = GetGWorkStateAPI();

	return(FALSE);
}

/*=============================================================================================
** @Name      : static void InsuErrStateCheck(u16 isoRp, u16 isoRn)
** @Input     : isoRp:正极对地绝缘值  1KΩ isoRn:负极对地绝缘值 1KΩ
** @Output    : void
** @Function  : 判断正负极对地的绝缘值是否超出阈值
** @The notes :
**===========================================================================================*/
static void InsuErrStateCheck(u16 isoRp, u16 isoRn)
{
	/*总压低于48V无绝缘告警*/
	if((IsoCalcMsg.SampVbat < ISO_MIN_BAT_V)
	    || (GetGSampSumVoltAPI() < (ISO_MIN_BAT_V * 10)))
	{
		/*消除告警、保护状态*/
		sInsuErrStateFlag = 0;
		return;
	}

	/*绝缘值为失效值*/
	if((0xffff == isoRp) || (0xffff == isoRn))
	{
		/*绝缘检测2级告警*/
		sInsuErrStateFlag = 3;
	}
	/*绝缘值在一级(严重)范围*/
	else if((((u32)isoRp * 1000) < ((u32)gGBmuGenPara_102[eBmuGenPara102_Leak1Lim] * IsoCalcMsg.SampVbat))
		|| (((u32)isoRn * 1000) < ((u32)gGBmuGenPara_102[eBmuGenPara102_Leak1Lim] * IsoCalcMsg.SampVbat)))
	{
		/*设置绝缘检测1级保护*/
		sInsuErrStateFlag = 1;
	}
	/*绝缘值在二级(轻微)范围*/
	else if((((u32)isoRp * 1000) < ((u32)gGBmuGenPara_102[eBmuGenPara102_Leak2Lim] * IsoCalcMsg.SampVbat))
		|| (((u32)isoRn * 1000) < ((u32)gGBmuGenPara_102[eBmuGenPara102_Leak2Lim] * IsoCalcMsg.SampVbat)))
	{
		/*绝缘检测2级告警*/
		sInsuErrStateFlag = 2;
	}
	else
	{
		/*消除告警、保护状态*/
		sInsuErrStateFlag = 0;
	}
}
