
/*******************************************************************************
 *  Includes
 */
#include "includes.h"

/*******************************************************************************
 *  Constants
 */

/*******************************************************************************
 *  Typedef
 */

/*******************************************************************************
 *  Macros
 */

/*******************************************************************************
 *  Global Variables
 */
INT8U	 Times[2] = {0};
INT32U VoltSampleCnt[2] = {0};
INT32U VoltSampleBuf[2][VOLT_INSO_VFIT_TIMES] = {0};
/*******************************************************************************
 *  External Variables
 */

/*******************************************************************************
 *  External Functions
 */

/*******************************************************************************
 *  Local Variables
 */
co_time	VoltSampleTimer[2];

/*******************************************************************************
 *  Functions Prototypes Declaration
 */

/*******************************************************************************
 *  Local Functions Declaration
 */
void BSP_Clock_Init						(void);
void BSP_GPIO_Init						(void);
void BSP_ADC_Init						(void);
void BSP_Timer_Init						(void);
void BSP_Fwdg_Init						(void);


void BSP_Addr_Task						(void);
void BSP_GPIO_Task						(void);

/*******************************************************************************
 *  Public Functions Definition
 */
void BSP_Init(void)
{
	BSP_Clock_Init();
	
	BSP_GPIO_Init();

	BSP_Timer_Init();
	
	//BSP_ADC_Init();

	BSP_I2C_Init();

	//BSP_CAN_Init(CFG_CAN_Baudrate);

#if (CFG_DEBUG == 0)
	BSP_Fwdg_Init();
#endif
}

void BSP_Task(void)
{
	BSP_Addr_Task();
	
	BSP_GPIO_Task();
}

/*******************************************************************************
 *  Local Functions Definition
 */
void BSP_Clock_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_AF);
}

void BSP_GPIO_Init(void)
{
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);//I2C WP
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);//Uart ctrl

	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_12);//SW1
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_13);//SW2
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_14);//SW3
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_15);//SW4

	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);//PCB LED

	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_13);//VRE0
	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_14);//VRE1
}

#if 0
void BSP_ADC_Init(void)
{
/************************* clock enable **************************/
	/* enable GPIOA clock */
	rcu_periph_clock_enable(RCU_GPIOA);

	/* enable ADC clock */
	rcu_periph_clock_enable(RCU_ADC0);

	/* enable DMA0 clock */
	rcu_periph_clock_enable(RCU_DMA0);

	/* config ADC clock */
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
/************************* clock enable **************************/

/************************** ADC Config ***************************/
	/* reset ADC */
	adc_deinit(ADC0);

	/* ADC mode config */
	adc_mode_config(ADC_MODE_FREE);

	/* ADC contineous function enable */
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);

	/* ADC scan mode disable */
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

	/* ADC data alignment config */
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

	/* ADC channel length config */
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 4);

	/* ADC regular channel config */
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_1, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_2, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_3, ADC_SAMPLETIME_55POINT5);

	/* ADC trigger config */
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

	/* enable ADC interface */
	adc_enable(ADC0);
	delay_1ms(1);

	/* ADC calibration and reset calibration */
	adc_calibration_enable(ADC0);

	/* ADC DMA function enable */
	adc_dma_mode_enable(ADC0);

	/* ADC software trigger enable */
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
/************************** ADC Config ***************************/

/************************* ADC DMA INIT***************************/
	/* ADC_DMA_channel configuration */
	dma_parameter_struct dma_data_parameter;

	/* ADC DMA_channel configuration */
	dma_deinit(DMA0, DMA_CH0);

	/* initialize DMA single data mode */
	dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
	dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
	dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_data_parameter.number       = 4U;
	dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
	dma_init(DMA0, DMA_CH0, &dma_data_parameter);
	dma_circulation_enable(DMA0, DMA_CH0);

	/* enable DMA channel */
	dma_channel_enable(DMA0, DMA_CH0);
/************************* ADC DMA INIT***************************/
}
#endif
void BSP_TimerConfig(INT8U Timer)
{
	if (Timer == e_Timer0)//->APB2-->120Mhz Max
	{
		/* ----------------------------------------------------------------------------
		TIMER0 Configuration:
    ---------------------------------------------------------------------------- */
        timer_parameter_struct timer_initpara;
		//timer_ic_parameter_struct timer_icinitpara;

		rcu_periph_clock_enable(RCU_TIMER0);

		gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_12);//f_VoltA sample
		//gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);//PA12 PARTIAL Remap

		timer_deinit(TIMER0);
		/* initialize TIMER init parameter struct */
		timer_struct_para_init(&timer_initpara);

		/* TIMER0 configuration */
		timer_initpara.prescaler         = 0;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = 0xFFFF;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
		timer_init(TIMER0, &timer_initpara);

		/* slave mode selection : TIMER0 */
		/* TIMER0 input trigger : external trigger connected to CI0 */
		timer_input_trigger_source_select(TIMER0, TIMER_SMCFG_TRGSEL_ETIFP);
		timer_slave_mode_select(TIMER0, TIMER_SLAVE_MODE_EXTERNAL0);

		/* enable the TIMER interrupt */
		timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_TRG);
		timer_interrupt_enable(TIMER0, TIMER_INT_FLAG_TRG);
		nvic_irq_enable(TIMER0_TRG_CMT_IRQn, 1, 0);
		timer_enable(TIMER0);
	}
	if (Timer == e_Timer1)//APB1prescale = 2 Timer1 clock = APB1 clock *2 = 120MHz
	{
		/* ----------------------------------------------------------------------------
		TIMER1 Configuration:
    ---------------------------------------------------------------------------- */
        timer_parameter_struct timer_initpara;
		//timer_ic_parameter_struct timer_icinitpara;

		rcu_periph_clock_enable(RCU_TIMER1);

		gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0);//f_VoltB sample
		//gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);//PA12 PARTIAL Remap

		timer_deinit(TIMER1);
		/* initialize TIMER init parameter struct */
		timer_struct_para_init(&timer_initpara);

		/* TIMER0 configuration */
		timer_initpara.prescaler         = 0;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = 0xFFFF;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
		timer_init(TIMER1, &timer_initpara);

		/* slave mode selection : TIMER0 */
		/* TIMER0 input trigger : external trigger connected to CI0 */
		timer_input_trigger_source_select(TIMER1, TIMER_SMCFG_TRGSEL_ETIFP);
		timer_slave_mode_select(TIMER1, TIMER_SLAVE_MODE_EXTERNAL0);

		/* enable the TIMER interrupt */
		timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_TRG);
		timer_interrupt_enable(TIMER1, TIMER_INT_FLAG_TRG);
		nvic_irq_enable(TIMER1_IRQn, 1, 0);
		timer_enable(TIMER1);
	}
	else if (Timer == e_Timer2)//APB1prescale = 2 Timer2 clock = APB1 clock *2 = 120MHz
	{
		timer_oc_parameter_struct timer_ocintpara;
		timer_parameter_struct timer_initpara;

		gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);//PWM_EV GPIO Init
		gpio_pin_remap_config(GPIO_TIMER2_FULL_REMAP, ENABLE);//PC9 FULL Remap
		/* -----------------------------------------------------------------------
		TIMER2 configuration: generate 3 PWM signals with 3 different duty cycles:
		----------------------------------------------------------------------- */
		rcu_periph_clock_enable(RCU_TIMER2);
		timer_deinit(TIMER2);

		/* TIMER2 configuration */
		timer_initpara.prescaler         = 119;/* 120MHz/(119+1)= 1us */
		timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
		timer_initpara.counterdirection  = TIMER_COUNTER_UP;
		timer_initpara.period            = 24;/* 1us * (24+1) = 25us = 40KHz*/ //24
		timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
		timer_initpara.repetitioncounter = 0;
		timer_init(TIMER2, &timer_initpara);
		
		/* CH3 configuration in OC TOGGLE mode */
		timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
		timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
		timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
		timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
		timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
		timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
		timer_channel_output_config(TIMER2, TIMER_CH_3, &timer_ocintpara);

		timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, 12);//50% Duty
		timer_channel_output_mode_config(TIMER2, TIMER_CH_3,TIMER_OC_MODE_PWM0);//
		timer_channel_output_shadow_config(TIMER2, TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

		timer_primary_output_config(TIMER2, ENABLE);

		/* auto-reload preload enable */
		timer_auto_reload_shadow_enable(TIMER2);
		timer_enable(TIMER2);
	}
	else if (Timer == e_Timer3)//APB1prescale = 2 Timer3 clock = APB1 clock *2 = 120MHz
	{
		timer_parameter_struct timer_initpara;

		rcu_periph_clock_enable(RCU_TIMER3);
		
		timer_deinit(TIMER3);
		timer_struct_para_init(&timer_initpara);

		timer_initpara.prescaler         = 119;/* 120M / (119+1) 1MHz*/
		timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
		timer_initpara.counterdirection  = TIMER_COUNTER_UP;
		timer_initpara.period            = 999;/* 1MHz / (9+1) = 1KHz*/
		timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;

		timer_init(TIMER3, &timer_initpara);

		timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
		timer_interrupt_enable(TIMER3, TIMER_INT_UP);
		nvic_irq_enable(TIMER3_IRQn, 0, 0);
		timer_enable(TIMER3);
	}
}

void BSP_Timer_Init(void)
{
	BSP_TimerConfig(e_Timer0);
	BSP_TimerConfig(e_Timer1);
	BSP_TimerConfig(e_Timer2);
	BSP_TimerConfig(e_Timer3);
}

void BSP_Fwdg_Init(void)
{
	fwdgt_config(3750, FWDGT_PSC_DIV32);//40KHz/32->0.8ms once  0.8*3750 ->3 Seconds
	fwdgt_enable();
}

void BSP_Addr_Task(void)
{
	hal_data->Addr &= ~0xF0;//2^4 0000 xxxx ->The upper 4 bits of the address need to be cleared 
	if (BSP_SW1_VALUE() == 1u)
	{
		hal_data->Addr |= 0x01;
	}
	else
	{
		hal_data->Addr &= ~0x01;
	}

	if (BSP_SW2_VALUE() == 1u)
	{
		hal_data->Addr |= 0x02;
	}
	else
	{
		hal_data->Addr &= ~0x02;
	}

	if (BSP_SW3_VALUE() == 1u)
	{
		hal_data->Addr |= 0x04;
	}
	else
	{
		hal_data->Addr &= ~0x04;
	}

	if (BSP_SW4_VALUE() == 1u)
	{
		hal_data->Addr |= 0x08;
	}
	else
	{
		hal_data->Addr &= ~0x08;
	}
}

INT32U Freq_AverageCalculation(INT32U *DataBuf, INT8U Num)
{
	INT8U i = 0u;
	INT32U Sum = 0u;
	INT32U nowDataBuf = 0u;
    INT32U maxDataBuf = 0u;
    INT32U minDataBuf = 0xffffffff;

	for (i = 0; i < Num; i++)
	{
		//Sum += *DataBuf++;
        nowDataBuf = DataBuf[i];
        Sum += nowDataBuf;

        if(nowDataBuf > maxDataBuf)
        {
            maxDataBuf = nowDataBuf;
        }

        if(nowDataBuf < minDataBuf)
        {
            minDataBuf = nowDataBuf;
        }
	}

	//return (INT32U)(Sum / Num);
	return ((Sum - maxDataBuf - minDataBuf) / (Num - 2));
}

void BSP_GPIO_Task(void)
{
	if (BSP_VRE0_VALUE() == 0u)
	{
		hal_data->Hardware_Version &= ~0x01;
	}
	else
	{
		hal_data->Hardware_Version |= 0x01;
	}

	if (BSP_VRE1_VALUE() == 0u)
	{
		hal_data->Hardware_Version &= ~0x02;
	}
	else
	{
		hal_data->Hardware_Version |= 0x02;
	}
	
	if (coTimeout(&VoltSampleTimer[0], 40u))
	{
		VoltSampleBuf[0][Times[0]] = VoltSampleCnt[0] * 25;//10ms once -> 10 * 100 = 1 Second
		VoltSampleCnt[0] = 0u;

		if (++Times[0] >= VOLT_INSO_VFIT_TIMES)
		{
			Times[0] = 0u;
		}
	}

	if (coTimeout(&VoltSampleTimer[1], 40u))
	{
		VoltSampleBuf[1][Times[1]] = VoltSampleCnt[1] * 25;//10ms once -> 10 * 100 = 1 Second
		VoltSampleCnt[1] = 0u;

		if (++Times[1] >= VOLT_INSO_VFIT_TIMES)
		{
			Times[1] = 0u;
		}
	}

	hal_data->VoltFreq[0] = Freq_AverageCalculation(VoltSampleBuf[0], VOLT_INSO_VFIT_TIMES);
	hal_data->VoltSample_Value[0] = Freq_RatioSample(hal_data->VoltFreq[0]);
	hal_data->VoltSample_Value_jiaozhun[0] = hal_data->VoltSample_Value[0] * hal_data->CalibrationInfo[0].Ratio_K + hal_data->CalibrationInfo[0].Ratio_B;

	hal_data->VoltFreq[1] = Freq_AverageCalculation(VoltSampleBuf[1], VOLT_INSO_VFIT_TIMES);
	hal_data->VoltSample_Value[1] = Freq_RatioSample(hal_data->VoltFreq[1]);
	hal_data->VoltSample_Value_jiaozhun[1] = hal_data->VoltSample_Value[1] * hal_data->CalibrationInfo[1].Ratio_K + hal_data->CalibrationInfo[1].Ratio_B;
}

