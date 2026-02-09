
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
can_receive_message_struct receive_message;
INT8U Can_transmit_mode = 0u;

/*******************************************************************************
 *  External Variables
 */

/*******************************************************************************
 *  External Functions
 */

/*******************************************************************************
 *  Local Variables
 */
INT8U u8RxFrameHeader;
INT8U u8RxFrameBufferIndex;
INT8U u8RxFrameBufferFreeLength;

t_CANMSG sRxFrame[CAN_RX_BUFFER_LENGTH];
co_time CanSendTimer;

/*******************************************************************************
 *  Functions Prototypes Declaration
 */

/*******************************************************************************
 *  Local Functions Declaration
 */
void	CanMessage_Analysis								(t_CANMSG *CanFrame);
INT8U Can_ReadMessage										(INT8U *Addr);
/*******************************************************************************
 *  Public Functions Definition
 */
void BSP_CAN_Init(INT16U BaudRate)
{
	INT32U FrameID = 0u;
	can_parameter_struct  can_parameter;
	can_filter_parameter_struct  can_filter;
		
    /* enable CAN clock */
    rcu_periph_clock_enable(RCU_CAN0);
        
    /* configure CAN0 GPIO */
    gpio_init(GPIOB,GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_8);//RX
    gpio_init(GPIOB,GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);//TX
    gpio_pin_remap_config(GPIO_CAN_PARTIAL_REMAP, ENABLE);
    
    /* initialize CAN register */
    can_deinit(CAN0);
    
    /* initialize CAN parameters */
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = DISABLE;
    can_parameter.auto_wake_up = DISABLE;
    can_parameter.no_auto_retrans = DISABLE;
    can_parameter.rec_fifo_overwrite = DISABLE;
    can_parameter.trans_fifo_order = DISABLE;
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_5TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_4TQ;
    
    if(1000 == BaudRate)
    {
        can_parameter.prescaler = 6;
    }
    else if(500 == BaudRate)
    {
        can_parameter.prescaler = 12; 
    }
    else if(250 == BaudRate)
    {
        can_parameter.prescaler = 24;
    }
    else if(125 == BaudRate)
    {
        can_parameter.prescaler = 48;
    }
    else
    {
        can_parameter.prescaler = 24;        
    }   
    
    /* initialize CAN */
    can_init(CAN0, &can_parameter);
    
    /* initialize filter */ 
    can_filter.filter_number=0;
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x0000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x0000;
    can_filter.filter_mask_low = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO1;
    can_filter.filter_enable = ENABLE;
    
#if 0    
	/* initialize filter */
	can_filter.filter_number = 0;

	FrameID = (CFG_U2_DEVICENUM << 22) | ((hal_data->Addr + CFG_DEFAULT_CAN_ADDR) << 8);
	/* initialize filter */
	can_filter.filter_mode = CAN_FILTERMODE_MASK;
	can_filter.filter_bits = CAN_FILTERBITS_32BIT;
	can_filter.filter_list_high		= ((FrameID << 3) & 0xFFFF0000) >> 16;
	can_filter.filter_list_low		= (FrameID  << 3) & 0xFFFF;
	can_filter.filter_mask_high		= ((CFG_CAN_MASK_BITMAP << 3) & 0xFFFF0000) >> 16;
	can_filter.filter_mask_low		= (CFG_CAN_MASK_BITMAP	<< 3) & 0xFFFF;
	can_filter.filter_fifo_number = CAN_FIFO1;
	can_filter.filter_enable			= ENABLE;
#endif
    can_filter_init(&can_filter);
    
    /* enable can receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INT_RFNE1);
    
    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_RX1_IRQn,0,4);
}


void Can_Task(void)
{
	INT8U Addr, Index = 0u;
	co_dword dword;
	can_trasnmit_message_struct transmit_message;
	
	if (Can_ReadMessage(&Addr) == !FALSE)
	{
		CanMessage_Analysis(&sRxFrame[Addr]);
	}
  
	if ((hal_data->CanInfo.transmit_mode == 1u) && (coTimeout(&CanSendTimer, hal_data->CanInfo.period)))
	//if ((hal_data->CanInfo.transmit_mode == 1u) && (coTimeout(&CanSendTimer, mSec(3u))))
	{
		can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
        transmit_message.tx_efid = (CFG_U2_DEVICENUM << 22) | (0x3B << 16u) | (hal_data->HostCanAddress << 8) | (hal_data->Addr + CFG_DEFAULT_CAN_ADDR);
        transmit_message.tx_ft = CAN_FT_DATA;
        transmit_message.tx_ff = CAN_FF_EXTENDED;

		dword.f = hal_data->VoltSample_Value_jiaozhun[0];
		transmit_message.tx_data[Index++] = dword.b[3];
		transmit_message.tx_data[Index++] = dword.b[2];
		transmit_message.tx_data[Index++] = dword.b[1];
		transmit_message.tx_data[Index++] = dword.b[0];
		dword.f = hal_data->VoltSample_Value_jiaozhun[1];
		transmit_message.tx_data[Index++] = dword.b[3];
		transmit_message.tx_data[Index++] = dword.b[2];
		transmit_message.tx_data[Index++] = dword.b[1];
		transmit_message.tx_data[Index++] = dword.b[0];
        transmit_message.tx_dlen = Index;
		
		can_message_transmit(CAN0, &transmit_message);
	}
}

void Can_SendCalibrationResult(INT8U ch, INT8U Result)
{
    INT8U Index = 0u;
    can_trasnmit_message_struct transmit_message;

    can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
    transmit_message.tx_efid = (CFG_U2_DEVICENUM << 22) | (0x3A << 16u) | (hal_data->HostCanAddress << 8) | (hal_data->Addr + CFG_DEFAULT_CAN_ADDR);
    transmit_message.tx_ft = CAN_FT_DATA;
    transmit_message.tx_ff = CAN_FF_EXTENDED;

    transmit_message.tx_data[Index++] = ch;
    transmit_message.tx_data[Index++] = Result;
    transmit_message.tx_dlen = Index;

    can_message_transmit(CAN0, &transmit_message);
}
/*******************************************************************************
 *  Local Functions Definition
 */
void CanMessage_Analysis(t_CANMSG *CanFrame)
{
	INT8U SrcAddr, DstAddr = 0u;
	INT8U cmd, DeviceNum	 = 0u;
	can_trasnmit_message_struct transmit_message;
	co_dword dword;

	SrcAddr 	= (INT8U)(CanFrame->ID 				 & 0xFF);
	DstAddr 	= (INT8U)((CanFrame->ID >>  8) & 0xFF);
	cmd 			= (INT8U)((CanFrame->ID >> 16) & 0x3F);
	DeviceNum = (INT8U)((CanFrame->ID >> 22) & 0x0F);

	if ((DstAddr == CFG_DEFAULT_CAN_ADDR + hal_data->Addr) && (DeviceNum == CFG_U2_DEVICENUM))
	//if (DstAddr == CFG_DEFAULT_CAN_ADDR + hal_data->Addr)
	{
		hal_data->HostCanAddress = SrcAddr;
		
		switch (cmd)
		{
			case 0x33://set
				{			
					if (CanFrame->Data[0] == 0x01)//U2
					{
						if (CanFrame->Data[1] == 0x01)//U2 Comm mode
						{
							dword.b[3] = CanFrame->Data[4];
							dword.b[2] = CanFrame->Data[5];
							dword.b[1] = CanFrame->Data[6];
							dword.b[0] = CanFrame->Data[7];

							if ((dword.v == 1u) || (dword.v == 0u))//1 Report 0:slave
							{
								EEPROM_load(EEPROM_ReadBuf, EEPROM_ADDR_can_transmit_mode, 1u);
								hal_data->CanInfo.transmit_mode = (INT8U)dword.v;//Set CFG in RAM
								
								if (EEPROM_ReadBuf[0] != (INT8U)dword.v)
								{
									EEPROM_WriteBuf[0] = (INT8U)dword.v;
									EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_can_transmit_mode, 1u);

									EEPROM_load(EEPROM_ReadBuf, EEPROM_ADDR_can_transmit_mode, 1u);
									dword.b[3] = EEPROM_ReadBuf[0];
									dword.b[2] = EEPROM_ReadBuf[1];
									dword.b[1] = EEPROM_ReadBuf[2];
									dword.b[0] = EEPROM_ReadBuf[3];
								}
							}
						}
						else if (CanFrame->Data[1] == 0x02)//U2 send period
						{
							dword.b[3] = CanFrame->Data[4];
							dword.b[2] = CanFrame->Data[5];
							dword.b[1] = CanFrame->Data[6];
							dword.b[0] = CanFrame->Data[7];

							if (fabs(dword.v, hal_data->CanInfo.period))
							{
								hal_data->CanInfo.period = dword.v;
								EEPROM_WriteBuf[0] = dword.b[3];
								EEPROM_WriteBuf[1] = dword.b[2];
								EEPROM_WriteBuf[2] = dword.b[1];
								EEPROM_WriteBuf[3] = dword.b[0];
								EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_4x_can_send_period, 4u);

								EEPROM_load(EEPROM_ReadBuf, EEPROM_ADDR_4x_can_send_period, 4u);
								dword.b[3] = EEPROM_ReadBuf[0];
								dword.b[2] = EEPROM_ReadBuf[1];
								dword.b[1] = EEPROM_ReadBuf[2];
								dword.b[0] = EEPROM_ReadBuf[3];
							}
						}
					}

					can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
                    transmit_message.tx_efid = CanFrame->ID &= ~0xFFFF;
					transmit_message.tx_efid |= (((INT32U)SrcAddr << 16) + DstAddr);
                    transmit_message.tx_ft = CAN_FT_DATA;
                    transmit_message.tx_ff = CAN_FF_EXTENDED;

					transmit_message.tx_data[0] = 0x01;
					transmit_message.tx_data[1] = CanFrame->Data[0];
					transmit_message.tx_data[2] = 0x00;
					transmit_message.tx_data[3] = 0x00;
					transmit_message.tx_data[4] = dword.b[3];
					transmit_message.tx_data[5] = dword.b[2];
					transmit_message.tx_data[6] = dword.b[1];
					transmit_message.tx_data[7] = dword.b[0];
					transmit_message.tx_dlen = 8u;
					can_message_transmit(CAN0, &transmit_message);
				}
				break;

			case 0x35:
				{
					can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
                    transmit_message.tx_efid = CanFrame->ID &= ~0xFFFF;
					transmit_message.tx_efid |= (((INT32U)SrcAddr << 16) + DstAddr);
                    transmit_message.tx_ft = CAN_FT_DATA;
                    transmit_message.tx_ff = CAN_FF_EXTENDED;

					transmit_message.tx_data[0] = 0u;//U2 for IMSU_X
					transmit_message.tx_data[1] = hal_data->Hardware_Version;
					transmit_message.tx_data[2] = cVersion / 100;
					transmit_message.tx_data[3] = cVersion % 100;
					transmit_message.tx_data[4] = 0u;
					transmit_message.tx_data[5] = 0u;
					transmit_message.tx_data[6] = 0u;
					transmit_message.tx_data[7] = 0u;
					transmit_message.tx_dlen = 8u;
					can_message_transmit(CAN0, &transmit_message);
				}
				break;

			case 0x3A:
				{
					if (CanFrame->Data[0] == 0x01)//Gun A
					{
						if (CanFrame->Data[1] == 0x01)//Voltage 1
						{
							if (hal_data->CalibrationInfo[0].Working == CO_FALSE)
							{
								coTimeout(&Calibration_SumTimer[0], 0u);
								coTimeout(&Calibration_Timeout[0], 0u);
								hal_data->CalibrationInfo[0].Working = !CO_FALSE;//Voltage 1 receive first
							}
							
							dword.b[3] = CanFrame->Data[4];
							dword.b[2] = CanFrame->Data[5];
							dword.b[1] = CanFrame->Data[6];
							dword.b[0] = CanFrame->Data[7];			
							hal_data->CalibrationInfo[0].Volt_Comm1 = dword.f;
						}
						else if (CanFrame->Data[1] == 0x02)//Voltage 2
						{
							if (hal_data->CalibrationInfo[0].Working == !CO_FALSE)//Voltage 2 receive second
							{
								dword.b[3] = CanFrame->Data[4];
								dword.b[2] = CanFrame->Data[5];
								dword.b[1] = CanFrame->Data[6];
								dword.b[0] = CanFrame->Data[7];
								hal_data->CalibrationInfo[0].Volt_Comm2 = dword.f;
								
								if (hal_data->CalibrationInfo[0].step == 0u)
								{
									coTimeout(&Calibration_Timeout[0], 0u);
									hal_data->CalibrationInfo[0].step = 1u;
								}
							}
						}
					}
					else if (CanFrame->Data[0] == 0x02)//Gun B
					{
						if (CanFrame->Data[1] == 0x01)//Voltage 1
						{
							if (hal_data->CalibrationInfo[1].Working == CO_FALSE)
							{
								coTimeout(&Calibration_SumTimer[1], 0u);
								coTimeout(&Calibration_Timeout[1], 0u);
								hal_data->CalibrationInfo[1].Working = !CO_FALSE;//Voltage 1 receive first
							}
							
							dword.b[3] = CanFrame->Data[4];
							dword.b[2] = CanFrame->Data[5];
							dword.b[1] = CanFrame->Data[6];
							dword.b[0] = CanFrame->Data[7];
							hal_data->CalibrationInfo[1].Volt_Comm1 = dword.f;
						}
						else if (CanFrame->Data[1] == 0x02)//Voltage 2
						{
							if (hal_data->CalibrationInfo[1].Working == !CO_FALSE)//Voltage 2 receive second
							{
								dword.b[3] = CanFrame->Data[4];
								dword.b[2] = CanFrame->Data[5];
								dword.b[1] = CanFrame->Data[6];
								dword.b[0] = CanFrame->Data[7];
								hal_data->CalibrationInfo[1].Volt_Comm2 = dword.f;

								if (hal_data->CalibrationInfo[1].step == 0u)
								{
									coTimeout(&Calibration_Timeout[1], 0u);
									hal_data->CalibrationInfo[1].step = 1u;
								}
							}
						}
					}
				}
				break;

			case 0x3B:
				{
					can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
                    transmit_message.tx_efid = CanFrame->ID &= ~0xFFFF;
					transmit_message.tx_efid |= (((INT32U)SrcAddr << 8) + DstAddr);
                    transmit_message.tx_ft = CAN_FT_DATA;
                    transmit_message.tx_ff = CAN_FF_EXTENDED;

					dword.f = hal_data->VoltSample_Value_jiaozhun[0];
					transmit_message.tx_data[0] = dword.b[3];
					transmit_message.tx_data[1] = dword.b[2];
					transmit_message.tx_data[2] = dword.b[1];
					transmit_message.tx_data[3] = dword.b[0];
					dword.f = hal_data->VoltSample_Value_jiaozhun[1];
					transmit_message.tx_data[4] = dword.b[3];
					transmit_message.tx_data[5] = dword.b[2];
					transmit_message.tx_data[6] = dword.b[1];
					transmit_message.tx_data[7] = dword.b[0];
					transmit_message.tx_dlen = 8u;
					can_message_transmit(CAN0, &transmit_message);
				}
				break;

			case 0x3C:
				{
				}
				break;

			default:
				break;
		}
	}
	else
	{
		return;
	}
}

INT8U Can_ReadMessage(INT8U *Addr)
{
	// disable interrupt
	//DisableInterrupts;
	if ((u8RxFrameHeader != u8RxFrameBufferIndex) || (u8RxFrameBufferFreeLength == 0))
	{
		*Addr = u8RxFrameHeader++;

		if (u8RxFrameHeader >= CAN_RX_BUFFER_LENGTH)
		{
			u8RxFrameHeader = 0;
		}
		u8RxFrameBufferFreeLength++;
	}
	else
	{
		// enable interrupt
		//EnableInterrupts;
		return CO_FALSE;
	}

	// enable interrupt
	//EnableInterrupts;
	return !CO_FALSE;
}

