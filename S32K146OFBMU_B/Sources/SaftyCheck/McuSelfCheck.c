/*==========================================Copyright(c)=======================================
**                                  Shenzhen Infypower Co., Ltd.
**                                  Power conversion Department
**                                       BMS Product Line
**
**---------------------------------------File Information--------------------------------------
** @File Name    : McuSelfCheck.c
** @Founder      : ZSY342
** @Date         : 2019.10.31
** @Function     : MCU检测模块
** @Instructions :
**===========================================================================================*/

/*=============================================================================================
** @Header file definition
**===========================================================================================*/
#include "BSPTypeDef.h"
#include "Safety_CMU.h"
#include "Safety_ADC.h"
#include "Safety_SRAM.h"
#include "Safety_CRC.h"
#include "Safety_WDOG.h"
#include "Safety_Flash.h"
#include "Safety_NVIC.h"
#include "Safety_SCST.h"
#include "Safety_RegCheck.h"
#include "adc_driver.h"
#include "BSPADC.h"
#include "BSPEEPROM.h"
#include "TacUser.h"
#include "ParaInterface.h"
#include "TacInterface.h"
#include "McuSelfCheck.h"

/*=============================================================================================
** @Global variable definition
**===========================================================================================*/
extern uint16_t regPortPcrCrcBaseValue;		//base crc value is calculated after PORT initialization

static u8 gMcuSelfCheckFaultFlag = 0;


u8 gGroupSwh_MaiState = 0;	/*主接触器状态*/
u8 gGroupSwh_NegState = 0;  /*负接触器状态*/

/*=============================================================================================
** @Static function declaration
**===========================================================================================*/
/*=============================================================================================
** @Name      : static u8 McuRegTest(void)
** @Input     : void
** @Output    : MCU特殊寄存器测试通过
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuRegTest(void);

/*=============================================================================================
** @Name      : static u8 McuClockTest(void)
** @Input     : void
** @Output    : MCU时钟测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuClockTest(void);

/*=============================================================================================
** @Name      : static u8 McuNvicTest(void)
** @Input     : void
** @Output    : MCU NVIC测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuNvicTest(void);

///*=============================================================================================
//** @Name      : static u8 McuWogTest(void)
//** @Input     : void
//** @Output    : MCU看门狗测试
//** @Function  :
//** @The notes : 初始化调用
//**===========================================================================================*/
//static u8 McuWogTest(void);

/*=============================================================================================
** @Name      : static u8 McuAdcTest(void)
** @Input     : void
** @Output    : MCU ADC测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuAdcTest(void);

/*=============================================================================================
** @Name      : static u8 McuFlashTest(void)
** @Input     : void
** @Output    : MCU FLASH测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuFlashTest(void);

/*=============================================================================================
** @Name      : static u8 McuRamTest(void)
** @Input     : void
** @Output    : MCU Ram测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuRamTest(void);

/*=============================================================================================
** @Name      : static u8 McuGpioTest(void)
** @Input     : void
** @Output    : MCU GPIO测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuGpioTest(void);

///*=============================================================================================
//** @Name      : static u8 McuCommTest(void)
//** @Input     : void
//** @Output    : MCU 通讯测试(设计功能安全的通讯CAN、SPI)
//** @Function  :
//** @The notes : 初始化调用
//**===========================================================================================*/
//static u8 McuCommTest(void);

/*=============================================================================================
** @Name      : static u8 McuCrcTest(void)
** @Input     : void
** @Output    : MCU CRC测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuCrcTest(void);

/*=============================================================================================
** @Interface function definition
**===========================================================================================*/

/*=============================================================================================
** @Name      : void McuSelfCheckInit(void)
** @Input     : void
** @Output    : void
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
void McuSelfCheckInit(void)
{
	u8 result = 0;

	LMEM_CacheDisable();
	CRC_DRV_Init(INST_CRC1, &crc1_InitConfig0);    //SRAM CRC init
	CMU_Init();					/*CLOCK init*/
	ERM_DRV_Init(INST_ERM1, ERM_CHANNEL_COUNT0, erm1_InitConfig0);	//SRAM ERM init
	RegCheck_Init();			/*GPIO、通讯IO检测初始化*/

	ADC_PorSelfTest();			/*ADC self-test*/
	CRC_SelfTest();				/*CRC self-test*/
	SCST_SelfTest();			/*SCST self-test*/
	//WDOG_PorTest();			/*DOG self-test*/
	SRAM_EccEimTest();			/*SRAM self-test*/
	FTFC_EccSelfTest();			/*flash self-test*/

	result = McuRegTest();		/*MCU 寄存器检测*/
	result |= McuClockTest();	/*MCU 时钟检测*/
	//result |= McuWogTest();	/*MCU 看门狗检测*/
	result |= McuAdcTest();		/*MCU ADC检测*/
	result |= McuNvicTest();	/*MCU NVIC检测*/
	result |= McuGpioTest();	/*MCU GPIO检测*/
	result |= McuFlashTest();	/*MCU Flash检测*/
	result |= McuRamTest();		/*MCU RAM检测*/
	//result |= McuCommTest();	/*MCU COMM检测，原理同GPIO*/
	result |= McuCrcTest();		/*MCU CRC检测*/

	if(result > 0)
	{
		;
	}
	LMEM_CacheEnable();
}

u8 data[10] = {0, 1,2,3,4,5,6,7,8,9};
u8 watchpData[10] = {0};
/*=============================================================================================
** @Name      : void TestMcuFaultTask(void)
** @Input     : void
** @Output    : MCU自检检测任务
** @Function  :
** @The notes : 上电只检测一次
**===========================================================================================*/
void TestMcuFaultTask(void)
{
	static u8 step = 0;
	u8 errorNum = 0;
	u8 i = 0;

	McuSelfCheckInit();

	if(0 == step)
	{
		step = 0;
		for(i = 0; i < 10; i++)
		{
			if((i == MCU_SELF_CHECK_Wdog) || (i == MCU_SELF_CHECK_Comm))
			{
				;/*内部狗由外部硬件狗代替，通讯检测原理同GPIO*/
			}
			else
			{
				BSPINTEEPReadByte(MCU_SELF_CHECK_RESULT_ADDR + i, &watchpData[i], 1);
			}

			if(1 == watchpData[i])
			{
				errorNum++;
			}
		}
		if(errorNum > 0)
		{
			gMcuSelfCheckFaultFlag = 1;
		}
	}

	gGroupSwh_MaiState = TacGetGroupSwhBackStateAPI(eGroupSwh_Mai);	/*主接触器状态*/
	gGroupSwh_NegState = TacGetGroupSwhBackStateAPI(eGroupSwh_Neg);		/*负接触器状态*/
}

/*=============================================================================================
** @Name      : u8 GetMcuSelfCheckFaultFlagAPI(void)
** @Input     : void
** @Output    : MCU自检检测结果
** @Function  : 获取MCU自检检测结果
** @The notes :
**===========================================================================================*/
u8 GetMcuSelfCheckFaultFlagAPI(void)
{
	return(gMcuSelfCheckFaultFlag);
}

/*=============================================================================================
** @Static function definition
**===========================================================================================*/
/*=============================================================================================
** @Name      : static u8 McuRegTest(void)
** @Input     : void
** @Output    : MCU特殊寄存器测试通过
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuRegTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetScstSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Reg, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Reg, &data, 1);
	}

	return(TRUE);
}

/*=============================================================================================
** @Name      : static u8 McuClockTest(void)
** @Input     : void
** @Output    : MCU时钟测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuClockTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetCmuSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Clock, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Clock, &data, 1);
	}

	return(TRUE);
}

/*=============================================================================================
** @Name      : static u8 McuNvicTest(void)
** @Input     : void
** @Output    : MCU NVIC测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuNvicTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetNvicSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Nvic, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		SetNvicSelfTestStatus(0);
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Nvic, &data, 1);
	}

	return(TRUE);
}

///*=============================================================================================
//** @Name      : static u8 McuWogTest(void)
//** @Input     : void
//** @Output    : MCU看门狗测试
//** @Function  :
//** @The notes : 初始化调用
//**===========================================================================================*/
//static u8 McuWogTest(void)
//{
//	u8 result = 0;
//	u8 data = 0;
//
//	result = GetWdogSelfTestStatus();
//
//	if(result > 0)/*0:正常  1:异常*/
//	{
//		data = 1;
//		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Wdog, &data, 1);
//		return(FALSE);
//	}
//	else
//	{
//		data = 0;
//		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Wdog, &data, 1);
//	}
//
//	return(TRUE);
//}

/*=============================================================================================
** @Name      : static u8 McuAdcTest(void)
** @Input     : void
** @Output    : MCU ADC测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuAdcTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetAdcSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Adc, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Adc, &data, 1);
	}

	return(TRUE);
}

/*=============================================================================================
** @Name      : static u8 McuFlashTest(void)
** @Input     : void
** @Output    : MCU FLASH测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuFlashTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetFlashSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Flash, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Flash, &data, 1);
	}

	return(TRUE);
}

/*=============================================================================================
** @Name      : static u8 McuRamTest(void)
** @Input     : void
** @Output    : MCU Ram测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuRamTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetSRamSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Ram, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Ram, &data, 1);
	}

	return(TRUE);
}

/*=============================================================================================
** @Name      : static u8 McuGpioTest(void)
** @Input     : void
** @Output    : MCU GPIO测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuGpioTest(void)
{
	u8 result = 0;
	u8 data = 0;

	RegCheck_Compare(regPortPcrCrcBaseValue);

	result = GetPortSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Gpio, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Gpio, &data, 1);
	}

	return(TRUE);
}

///*=============================================================================================
//** @Name      : static u8 McuCommTest(void)
//** @Input     : void
//** @Output    : MCU 通讯测试(设计功能安全的通讯CAN、SPI)
//** @Function  :
//** @The notes : 初始化调用
//**===========================================================================================*/
//static u8 McuCommTest(void)
//{
//
//
//	return(TRUE);
//}

/*=============================================================================================
** @Name      : static u8 McuCrcTest(void)
** @Input     : void
** @Output    : MCU CRC测试
** @Function  :
** @The notes : 初始化调用
**===========================================================================================*/
static u8 McuCrcTest(void)
{
	u8 result = 0;
	u8 data = 0;

	result = GetCrcSelfTestStatus();

	if(result > 0)/*0:正常  1:异常*/
	{
		data = 1;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Crc, &data, 1);
		return(FALSE);
	}
	else
	{
		data = 0;
		BSPINTEEPWriteByte(MCU_SELF_CHECK_RESULT_ADDR + MCU_SELF_CHECK_Crc, &data, 1);
	}

	return(TRUE);
}



