
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
INT8U EEPROM_ReadBuf[EEPROM_BUFFER_MAX] = {0};
INT8U EEPROM_WriteBuf[EEPROM_BUFFER_MAX] = {0};

/*******************************************************************************
 *  External Variables
 */

/*******************************************************************************
 *  External Functions
 */

/*******************************************************************************
 *  Local Variables
 */

/*******************************************************************************
 *  Functions Prototypes Declaration
 */

/*******************************************************************************
 *  Local Functions Declaration
 */
INT8U EEPROM_write_page														(INT8U *WriteBuf, INT16U Address, INT16U Number);
void	i2c_bus_reset																(void);
void	EEPROM_wait_standby													(void);



/*******************************************************************************
 *  Public Functions Definition
 */
void EEPROM_write(INT8U *WriteBuf, INT16U WriteAddress, INT16U Length)
{
	INT8U NumberOfPage = 0u, NumberOfSingle = 0u, count = 0u;
	INT16U Address = 0u;

	Address					= WriteAddress % EEPROM_PAGE_SIZE;
	count						= EEPROM_PAGE_SIZE - Address;
	NumberOfPage		= Length / EEPROM_PAGE_SIZE;
	NumberOfSingle	= Length % EEPROM_PAGE_SIZE;

	/* if write_address is EEPROM_PAGE_SIZE aligned  */
	if ((Length + Address) <= EEPROM_PAGE_SIZE)//First page to write
	{
		EEPROM_write_page(WriteBuf, WriteAddress, Length);
		EEPROM_wait_standby();
	}
	else
	{
		EEPROM_write_page(WriteBuf, Address, count);
		EEPROM_wait_standby();
		WriteAddress	+= count;
		WriteBuf			+= count;
		Length				-= count;

		if (Length >= EEPROM_PAGE_SIZE)
		{
			NumberOfPage		= Length / EEPROM_PAGE_SIZE;
			NumberOfSingle	= Length % EEPROM_PAGE_SIZE;

			while (NumberOfPage--)
			{
				EEPROM_write_page(WriteBuf, WriteAddress, EEPROM_PAGE_SIZE);
				EEPROM_wait_standby();
				WriteAddress	+= EEPROM_PAGE_SIZE;
				WriteBuf			+= EEPROM_PAGE_SIZE;
			}

			if (NumberOfSingle != 0u)
			{
				EEPROM_write_page(WriteBuf, WriteAddress, NumberOfSingle);
				EEPROM_wait_standby();
			}
		}
		else
		{
			EEPROM_write_page(WriteBuf, Address, count);
			EEPROM_wait_standby();
		}
	}

#if 0	
	if (Address == 0u)
	{
		while (NumberOfPage--)
		{
			EEPROM_write_page(WriteBuf, WriteAddress, EEPROM_PAGE_SIZE);
			EEPROM_wait_standby();
			WriteAddress	+= EEPROM_PAGE_SIZE;
			WriteBuf			+= EEPROM_PAGE_SIZE;
		}

		if (NumberOfSingle != 0u)
		{
			EEPROM_write_page(WriteBuf, WriteAddress, NumberOfSingle);
			EEPROM_wait_standby();
		}
	}
	else
	{
		/* if write_address is not EEPROM_PAGE_SIZE aligned */
		if ((Address + Length) > EEPROM_PAGE_SIZE)
		{
			EEPROM_write_page(WriteBuf, Address, count);
			EEPROM_wait_standby();
			WriteAddress	+= count;
			WriteBuf			+= count;

			Length -= (count-1);
			NumberOfPage	 = Length / EEPROM_PAGE_SIZE;
			NumberOfSingle = Length % EEPROM_PAGE_SIZE;

			if (NumberOfPage > 0)
			{

			}
		}

		

		if (count != 0u)
		{
			EEPROM_write_page(WriteBuf, WriteAddress, Length);
			EEPROM_wait_standby();
			WriteAddress	+= count;
			WriteBuf			+= count;
		}

		if (NumberOfPage > 1)
		{
			//write page
			while (NumberOfPage--)
			{
				EEPROM_write_page(WriteBuf, WriteAddress, EEPROM_PAGE_SIZE);
				EEPROM_wait_standby();
				WriteAddress	+= EEPROM_PAGE_SIZE;
				WriteBuf			+= EEPROM_PAGE_SIZE;
			}

			//write single
			if (NumberOfSingle != 0u)
			{
				EEPROM_write_page(WriteBuf, WriteAddress, NumberOfSingle);
				EEPROM_wait_standby();
			}
		}
	}
#endif
	BSP_I2C_WP_SET();
	BSP_I2C_WP_SET();
	BSP_I2C_WP_SET();
}

void EEPROM_load(INT8U *ReadBuf,	INT16U ReadAddress, INT16U Length)
{
	dma_parameter_struct dma_init_struct;

	INT8U	 state				= eState_I2C_Start;
	INT8U	 read_cycle		= 0u;
	INT8U	 TimeoutFlag	= CO_FALSE;
	INT16U timeout			= 0u;

	while (!TimeoutFlag)
	{
		switch (state)
		{
			case eState_I2C_Start:
				{
					if (read_cycle == 0u)//Need write read address first
					{
						/* disable I2C1 */
						i2c_disable(I2C1);
						
						/* enable I2C1 */
						i2c_enable(I2C1);

						/* enable acknowledge */
						i2c_ack_config(I2C1, I2C_ACK_ENABLE);

						/* i2c master sends start signal only when the bus is idle */
						while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY) && (timeout < I2C_TIMEOUT))
						{
							timeout++;
						}

						if (timeout < I2C_TIMEOUT)
						{
							/* send the start signal */
							i2c_start_on_bus(I2C1);
							timeout = 0;
							state		= eState_I2C_SendAddress;
						}
						else
						{
							i2c_bus_reset();
							timeout = 0;
							state		= eState_I2C_Start;
						}
					}
					else//start receive data
					{
						i2c_start_on_bus(I2C1);//restart signal
						timeout = 0;
						state = eState_I2C_SendAddress;
					}
				}
				break;

			case eState_I2C_SendAddress:
				{
					/* i2c master sends START signal successfully */
					while ((!i2c_flag_get(I2C1, I2C_FLAG_SBSEND)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						if (read_cycle == 0u)
						{
							i2c_master_addressing(I2C1, EEPROM_DEVICE_ADDR, I2C_TRANSMITTER);//bit0 --> 0
						}
						else
						{
							i2c_master_addressing(I2C1, EEPROM_DEVICE_ADDR, I2C_RECEIVER);//bit0 --> 1
						}

						timeout = 0u;
						state		= eState_I2C_ClearAddrFlag;
					}
					else
					{
						timeout			= 0u;
						read_cycle	= 0u;
						state				= eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_ClearAddrFlag:
				{
					/* address flag set means i2c slave sends ACK */      
					while ((!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
						timeout = 0u;
						state		= eState_I2C_Transmit;
					}
					else
					{
						timeout			= 0u;
						read_cycle	= 0u;
						state				= eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_Transmit:
				{
					if (read_cycle == 0u)
					{
						/* wait until the transmit data buffer is empty */
						while ((!i2c_flag_get(I2C1, I2C_FLAG_TBE)) && (timeout < I2C_TIMEOUT))
						{
							timeout++;
						}

						if (timeout < I2C_TIMEOUT)
						{
							/* send the EEPROM's internal address to write to : High 8 bits address */
							i2c_data_transmit(I2C1, (INT8U)((ReadAddress >> 8) & 0xFF));
							timeout = 0;
						}
						else
						{
							timeout			= 0u;
							read_cycle	= 0u;
							state				= eState_I2C_Start;
						}

						/* wait until BTC bit is set */
						while ((!i2c_flag_get(I2C1, I2C_FLAG_BTC)) && (timeout < I2C_TIMEOUT))
						{
							timeout++;
						}

						if (timeout < I2C_TIMEOUT)
						{
							/* send the EEPROM's internal address to write to : Low 8 bits address */
							i2c_data_transmit(I2C1, (INT8U)(ReadAddress & 0xFF));
							timeout = 0;
						}
						else
						{
							timeout			= 0u;
							read_cycle	= 0u;
							state				= eState_I2C_Start;
						}

						/* wait until BTC bit is set */
						while ((!i2c_flag_get(I2C1, I2C_FLAG_BTC)) && (timeout < I2C_TIMEOUT))
						{
							timeout++;
						}

						if (timeout < I2C_TIMEOUT)
						{
							read_cycle++;
							timeout = 0;
							state = eState_I2C_Start;
						}
						else
						{
							timeout			= 0u;
							read_cycle	= 0u;
							state				= eState_I2C_Start;
						}
					}
					else
					{
						/* one byte master reception procedure (polling) */
						if (Length < 2)
						{
							/* disable acknowledge */
							i2c_ack_config(I2C1, I2C_ACK_DISABLE);
							
							/* clear ADDSEND register by reading I2C_STAT0 then I2C_STAT1 register (I2C_STAT0 has already been read) */
							i2c_flag_get(I2C1, I2C_FLAG_ADDSEND);
							
							/* send a stop condition to I2C bus*/
							i2c_stop_on_bus(I2C1);

							/* wait for the byte to be received */
							while (!i2c_flag_get(I2C1, I2C_FLAG_RBNE));
							
							/* read the byte received from the EEPROM */
							*ReadBuf = i2c_data_receive(I2C1);
							
							/* decrement the read bytes counter */
							Length--;
							timeout = 0;
							state = eState_I2C_Stop;
						}
						else
						{
							/* more than one byte master reception procedure (DMA) */
							dma_deinit(DMA0, DMA_CH4);
							dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
							dma_init_struct.memory_addr = (INT32U)ReadBuf;
							dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
							dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
							dma_init_struct.number = Length;
							dma_init_struct.periph_addr = (INT32U)&I2C_DATA(I2C1);
							dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
							dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
							dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
							dma_init(DMA0, DMA_CH4, &dma_init_struct);

							i2c_dma_last_transfer_config(I2C1, I2C_DMALST_ON);
							
							/* enable I2C1 DMA */
							i2c_dma_enable(I2C1, I2C_DMA_ON);
							
							/* enable DMA0 channel4 */
							dma_channel_enable(DMA0, DMA_CH4);
							
							/* wait until BTC bit is set */
							while(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF))
							{
								state = eState_I2C_Stop;
							}
						}
					}
				}
				break;

			case eState_I2C_Stop:
				{
					/* send a stop condition to I2C bus */
					i2c_stop_on_bus(I2C1);
					
					/* i2c master sends STOP signal successfully */
					while ((I2C_CTL0(I2C1)&0x0200) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						timeout = 0;
						state = eState_I2C_StateMax;
						TimeoutFlag = !CO_FALSE;
					}
					else
					{
						timeout			= 0u;
						read_cycle	= 0u;
						state				= eState_I2C_Start;
					}
				}
				break;

			default:
				{
					timeout			= 0u;
					read_cycle	= 0u;
					state				= eState_I2C_Start;
					TimeoutFlag = CO_FALSE;
				}
				break;
		}
	}
}

/*******************************************************************************
 *  Local Functions Definition
 */
void i2c_bus_reset(void)
{
	i2c_deinit(I2C1);
	
	/* configure SDA/SCL for GPIO */
	GPIO_BC(GPIOB) |= GPIO_PIN_10|GPIO_PIN_11;
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_BOP(GPIOB) |= GPIO_PIN_10;
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_BOP(GPIOB) |= GPIO_PIN_11;
	
	/* connect PB10 to I2C1_SCL */
	/* connect PB11 to I2C1_SDA */
	gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
	
	/* configure the I2C1 interfaces */
	BSP_I2C_Init();
}

INT8U EEPROM_write_page(INT8U *WriteBuf, INT16U Address, INT16U Number)
{
	dma_parameter_struct dma_init_struct;

	INT8U state				= eState_I2C_Start;
	INT8U TimeoutFlag = CO_FALSE;
	INT16U timeout		= 0u;

	while (!TimeoutFlag)
	{
		switch (state)
		{
			case eState_I2C_Start:
				{
					while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						i2c_start_on_bus(I2C1);
						BSP_I2C_WP_RESET();
						BSP_I2C_WP_RESET();
						BSP_I2C_WP_RESET();
						
						timeout = 0;
						state		= eState_I2C_SendAddress;
					}
					else
					{
						i2c_bus_reset();
						timeout = 0;
						state = eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_SendAddress:
				{
					while ((!i2c_flag_get(I2C1, I2C_FLAG_SBSEND)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						i2c_master_addressing(I2C1, EEPROM_DEVICE_ADDR, I2C_TRANSMITTER);
						timeout = 0;
						state = eState_I2C_ClearAddrFlag;
					}
					else
					{
						timeout = 0;
						state = eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_ClearAddrFlag:
				{
					while ((!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
						timeout = 0;
						state = eState_I2C_Transmit;
					}
					else
					{
						timeout = 0;
						state = eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_Transmit:
				{
					while((!i2c_flag_get(I2C1, I2C_FLAG_TBE)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						/* send the EEPROM's internal address to write to : High 8 bits address */
						i2c_data_transmit(I2C1, (INT8U)((Address >> 8) & 0xFF));
						timeout = 0;
					}
					else
					{
						timeout = 0;
						state = eState_I2C_Start;
					}

					while ((!i2c_flag_get(I2C1, I2C_FLAG_BTC)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						/* send the EEPROM's internal address to write to : Low 8 bits address */
						i2c_data_transmit(I2C1, (INT8U)(Address & 0xFF));
						timeout = 0;
					}
					else
					{
						timeout = 0;
						state = eState_I2C_Start;
					}

					while ((!i2c_flag_get(I2C1, I2C_FLAG_BTC)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						dma_deinit(DMA0, DMA_CH3);
						dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
						dma_init_struct.memory_addr = (INT32U)WriteBuf;
						dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
						dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
						dma_init_struct.number = Number;
						dma_init_struct.periph_addr = (INT32U)&I2C_DATA(I2C1);
						dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
						dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
						dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
						dma_init(DMA0, DMA_CH3, &dma_init_struct);

						/* enable I2C1 DMA */
						i2c_dma_enable(I2C1, I2C_DMA_ON);

						/* enable DMA0 channel3 */
						dma_channel_enable(DMA0, DMA_CH3);

						/* wait until BTC bit is set */
						while (!dma_flag_get(DMA0, DMA_CH3, DMA_FLAG_FTF));

						/* wait until BTC bit is set */
						while ((!i2c_flag_get(I2C1, I2C_FLAG_BTC)) && (timeout < I2C_TIMEOUT))
						{
							timeout++;
						}

						if (timeout < I2C_TIMEOUT)
						{
							timeout = 0;
							state 	= eState_I2C_Stop;
						}
						else
						{
							timeout = 0;
							state = eState_I2C_Start;
						}
					}
					else
					{
						timeout = 0;
						state = eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_Stop:
				{
					/* send a stop condition to I2C bus */
					i2c_stop_on_bus(I2C1);

					/* i2c master sends STOP signal successfully */
					while ((I2C_CTL0(I2C1)&0x0200) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						timeout = 0;
						state = eState_I2C_StateMax;
						TimeoutFlag = !CO_FALSE;
					}
				}
				break;

			default:
				{
					state = eState_I2C_Start;
					TimeoutFlag = CO_FALSE;
					timeout = 0;
				}
				break;
		}
	}

	return state;
}

void EEPROM_wait_standby(void)
{
	INT8U state							= eState_I2C_Start;
	INT8U TimeoutFlag				= CO_FALSE;
	INT16U timeout					= 0u;
	__IO INT32U val					= 0u;

	while (!TimeoutFlag)
	{
		switch (state)
		{
			case eState_I2C_Start:
				{
					/* i2c master sends start signal only when the bus is idle */
					while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						i2c_start_on_bus(I2C1);
						timeout = 0;
						state		= eState_I2C_SendAddress;
					}
					else
					{
						i2c_bus_reset();
						timeout = 0;
						state		= eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_SendAddress:
				{
					/* i2c master sends START signal successfully */
					while ((!i2c_flag_get(I2C1, I2C_FLAG_SBSEND)) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						i2c_master_addressing(I2C1, EEPROM_DEVICE_ADDR, I2C_TRANSMITTER);
						timeout = 0;
						state		= eState_I2C_ClearAddrFlag;
					}
					else
					{
						timeout = 0;
						state		= eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_ClearAddrFlag:
				{
					while ((!((i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)) || (i2c_flag_get(I2C1, I2C_FLAG_AERR))))&&(timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						if (i2c_flag_get(I2C1, I2C_FLAG_ADDSEND))
						{
							i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
							timeout = 0;
							
							/* send a stop condition to I2C bus */
							i2c_stop_on_bus(I2C1);
							TimeoutFlag = !CO_FALSE;
						}
						else
						{
							/* clear the bit of AE */
							i2c_flag_clear(I2C1, I2C_FLAG_AERR);
							timeout = 0;
							state		= eState_I2C_Stop;
						}
					}
					else
					{
						timeout = 0;
						state		= eState_I2C_Start;
					}
				}
				break;

			case eState_I2C_Stop:
				{
					/* send a stop condition to I2C bus */
					i2c_stop_on_bus(I2C1);

					/* i2c master sends STOP signal successfully */
					while ((I2C_CTL0(I2C1) & 0x0200) && (timeout < I2C_TIMEOUT))
					{
						timeout++;
					}

					if (timeout < I2C_TIMEOUT)
					{
						timeout = 0;
						state		= eState_I2C_Start;
					}
					else
					{
						timeout = 0;
						state		= eState_I2C_Start;
					}
				}
				break;

			default:
				{
					timeout = 0;
					state		= eState_I2C_Start;
				}
				break;
		}
	}
}

void EEPROM_set_default_configuration(void)
{
	INT16U Length = 0u;
	co_dword dword;

	dword.f = 1.0f;//EEPROM_ADDR_4x_sample_ratio_K0
	EEPROM_WriteBuf[Length++] = dword.b[3];
	EEPROM_WriteBuf[Length++] = dword.b[2];
	EEPROM_WriteBuf[Length++] = dword.b[1];
	EEPROM_WriteBuf[Length++] = dword.b[0];

	dword.f = 1.0f;//EEPROM_ADDR_4x_sample_ratio_K1
	EEPROM_WriteBuf[Length++] = dword.b[3];
	EEPROM_WriteBuf[Length++] = dword.b[2];
	EEPROM_WriteBuf[Length++] = dword.b[1];
	EEPROM_WriteBuf[Length++] = dword.b[0];

	dword.f = 0.0f;//EEPROM_ADDR_4x_sample_ratio_B0
	EEPROM_WriteBuf[Length++] = dword.b[3];
	EEPROM_WriteBuf[Length++] = dword.b[2];
	EEPROM_WriteBuf[Length++] = dword.b[1];
	EEPROM_WriteBuf[Length++] = dword.b[0];

	dword.f = 0.0f;//EEPROM_ADDR_4x_sample_ratio_B1
	EEPROM_WriteBuf[Length++] = dword.b[3];
	EEPROM_WriteBuf[Length++] = dword.b[2];
	EEPROM_WriteBuf[Length++] = dword.b[1];
	EEPROM_WriteBuf[Length++] = dword.b[0];

	dword.v = 0u;//EEPROM_ADDR_can_transmit_mode  slave
	EEPROM_WriteBuf[Length++] = dword.b[0];

	dword.v = 50u;//EEPROM_ADDR_4x_can_send_period 50ms
	EEPROM_WriteBuf[Length++] = dword.b[3];
	EEPROM_WriteBuf[Length++] = dword.b[2];
	EEPROM_WriteBuf[Length++] = dword.b[1];
	EEPROM_WriteBuf[Length++] = dword.b[0];

	EEPROM_write(EEPROM_WriteBuf, EEPROM_ADDR_4x_sample_ratio_K0, Length);

	dword.w[0] = CFG_EEPROM_VERSION;
	EEPROM_WriteBuf[0] = dword.b[1];
	EEPROM_WriteBuf[1] = dword.b[0];
	EEPROM_write(EEPROM_WriteBuf, EEPROM_cVersion, 2);
}

void EEPROM_configure_load(void)
{
	INT16U Length = 0u;
	co_dword dword;

	EEPROM_load(EEPROM_ReadBuf, EEPROM_ADDR_4x_sample_ratio_K0, EEPROM_BUFFER_MAX);

	dword.b[3] = EEPROM_ReadBuf[Length++];
	dword.b[2] = EEPROM_ReadBuf[Length++];
	dword.b[1] = EEPROM_ReadBuf[Length++];
	dword.b[0] = EEPROM_ReadBuf[Length++];
	hal_data->CalibrationInfo[0].Ratio_K = dword.f;
	
	dword.b[3] = EEPROM_ReadBuf[Length++];
	dword.b[2] = EEPROM_ReadBuf[Length++];
	dword.b[1] = EEPROM_ReadBuf[Length++];
	dword.b[0] = EEPROM_ReadBuf[Length++];
	hal_data->CalibrationInfo[1].Ratio_K = dword.f;

	dword.b[3] = EEPROM_ReadBuf[Length++];
	dword.b[2] = EEPROM_ReadBuf[Length++];
	dword.b[1] = EEPROM_ReadBuf[Length++];
	dword.b[0] = EEPROM_ReadBuf[Length++];
	hal_data->CalibrationInfo[0].Ratio_B = dword.f;

	dword.b[3] = EEPROM_ReadBuf[Length++];
	dword.b[2] = EEPROM_ReadBuf[Length++];
	dword.b[1] = EEPROM_ReadBuf[Length++];
	dword.b[0] = EEPROM_ReadBuf[Length++];
	hal_data->CalibrationInfo[1].Ratio_B = dword.f;

	hal_data->CanInfo.transmit_mode = EEPROM_ReadBuf[Length++];

	dword.b[3] = EEPROM_ReadBuf[Length++];
	dword.b[2] = EEPROM_ReadBuf[Length++];
	dword.b[1] = EEPROM_ReadBuf[Length++];
	dword.b[0] = EEPROM_ReadBuf[Length++];
	hal_data->CanInfo.period = dword.v;
}

