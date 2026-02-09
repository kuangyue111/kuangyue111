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

/*******************************************************************************
 *  Public Functions Definition
 */
void BSP_I2C_Init(void)
{
	rcu_periph_clock_enable(RCU_I2C1);
	
	/* connect PB10 to I2C1_SCL */
	/* connect PB11 to I2C1_SDA */
	gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);

	/* configure I2C clock */
	i2c_clock_config(I2C1, I2C1_SPEED, I2C_DTCY_2);//Set i2c clock and duty

	/* configure I2C address */
	i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS);

	/* enable I2C1 */
	i2c_enable(I2C1);

	/* enable acknowledge */
	i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

/*******************************************************************************
 *  Local Functions Definition
 */


