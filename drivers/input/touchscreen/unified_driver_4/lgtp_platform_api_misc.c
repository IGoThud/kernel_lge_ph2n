/***************************************************************************
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *    File  : lgtp_platform_api.c
 *    Author(s)   : D3 BSP Touch Team < d3-bsp-touch@lge.com >
 *    Description :
 *
 ***************************************************************************/
#define LGTP_MODULE "[MISC]"

/****************************************************************************
* Include Files
****************************************************************************/
#include <linux/input/unified_driver_4/lgtp_common.h>

#include <linux/input/unified_driver_4/lgtp_model_config_misc.h>
#include <linux/input/unified_driver_4/lgtp_model_config_i2c.h>

#include <linux/input/unified_driver_4/lgtp_platform_api_misc.h>
#include <linux/input/unified_driver_4/lgtp_platform_api_i2c.h>


/****************************************************************************
* Manifest Constants / Defines
****************************************************************************/

/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
* Type Definitions
****************************************************************************/

/****************************************************************************
* Variables
****************************************************************************/
static int nIrq_num;
atomic_t	touch_irq_mask;

/****************************************************************************
* Extern Function Prototypes
****************************************************************************/
extern int touch_module;


/****************************************************************************
* Local Function Prototypes
****************************************************************************/


/****************************************************************************
* Local Functions
****************************************************************************/
#if 0 /* TBD */
#define istate core_internal_state__do_not_mess_with_it
#define IRQS_PENDING 0x00000200
static void TouchClearPendingIrq(void)
{
	unsigned long flags;
	struct irq_desc *desc = irq_to_desc(nIrq_num);

	if (desc) {
		raw_spin_lock_irqsave(&desc->lock, flags);
		desc->istate &= ~IRQS_PENDING;
		raw_spin_unlock_irqrestore(&desc->lock, flags);
	}
}
#endif

static void TouchMaskIrq(void)
{
	struct irq_desc *desc = irq_to_desc(nIrq_num);

	if (desc->irq_data.chip->irq_mask)
		desc->irq_data.chip->irq_mask(&desc->irq_data);
}

static void TouchUnMaskIrq(void)
{
	struct irq_desc *desc = irq_to_desc(nIrq_num);

	if (desc->irq_data.chip->irq_unmask)
		desc->irq_data.chip->irq_unmask(&desc->irq_data);
}


/****************************************************************************
* Global Functions
****************************************************************************/
int TouchGetModuleIndex(void)
{
	int index = FIRST_MODULE;
 
    TOUCH_LOG("Touch Module Get. Module index = %d\n",index);
	return index;
}

int TouchGetBootMode(void)
{
	int mode = BOOT_NORMAL;

	enum lge_boot_mode_type lge_boot_mode;

	lge_boot_mode = lge_get_boot_mode();

	if (lge_boot_mode == LGE_BOOT_MODE_CHARGERLOGO)
		mode = BOOT_OFF_CHARGING;
	else if (lge_boot_mode >=  LGE_BOOT_MODE_QEM_56K)
		mode = BOOT_MINIOS;
	else
		mode = BOOT_NORMAL;

	return mode;
}

int TouchInitializeGpio(void)
{
	int ret = 0;

	TOUCH_FUNC();

	ret = gpio_request(TOUCH_GPIO_RESET, "touch_reset");
	if (ret < 0)
		TOUCH_ERR("failed at gpio_request(reset pin)\n");

	gpio_direction_output(TOUCH_GPIO_RESET, 1);

	ret = gpio_request(TOUCH_GPIO_INTERRUPT, "touch_int");
	if (ret < 0)
		TOUCH_ERR("failed at gpio_request(interrupt pin)\n");

	gpio_direction_input(TOUCH_GPIO_INTERRUPT);

	return TOUCH_SUCCESS;
}

void TouchSetGpioReset(int isHigh)
{
	if (isHigh) {
		gpio_set_value(TOUCH_GPIO_RESET, 1);

		TOUCH_LOG("Reset Pin was set to HIGH\n");
	} else {
		gpio_set_value(TOUCH_GPIO_RESET, 0);

		TOUCH_LOG("Reset Pin was set to LOW\n");
	}
}

int TouchReadGpioInterrupt(void)
{
	int gpioState = 0;

	gpioState = gpio_get_value(TOUCH_GPIO_INTERRUPT);

	return gpioState;
}

int TouchRegisterIrq(TouchDriverData *pDriverData, irq_handler_t irqHandler, irq_handler_t threaded_irqHandler)
{
	int ret = 0;

	TOUCH_FUNC();

	ret = request_threaded_irq(pDriverData->client->irq, irqHandler, threaded_irqHandler, TOUCH_IRQ_FLAGS, pDriverData->client->name, pDriverData);
	if (ret < 04) {
		TOUCH_ERR("failed at request_irq() ( error = %d )\n", ret );
		return TOUCH_FAIL;
	}
	enable_irq_wake(pDriverData->client->irq);

	nIrq_num = pDriverData->client->irq;

	return TOUCH_SUCCESS;
}

void TouchEnableIrq(void)
{
	TouchUnMaskIrq();

	/* TouchClearPendingIrq(); TBD */
	if (atomic_read(&touch_irq_mask) != 0) {
		atomic_set(&touch_irq_mask, 0);
		enable_irq(nIrq_num);
	}

	TOUCH_LOG("Interrupt Enabled\n");
}

void TouchDisableIrq(void)
{
	if (atomic_read(&touch_irq_mask) == 0) {
		atomic_set(&touch_irq_mask, 1);
		disable_irq_nosync(nIrq_num);
	}

	TouchMaskIrq();

	TOUCH_LOG("Interrupt Disabled\n");
}


int TouchReadReg(u16 addr, u8 *rxbuf, int len)
{
    int ret = 0;
    #if defined(TOUCH_I2C_USE)
       ret = Touch_I2C_Read( addr, rxbuf, len );
    #elif defined(TOUCH_SPI_USE)
       ret = Touch_SPI_Read( addr, rxbuf, len );
    #endif
    return ret;
}

int TouchWriteReg(u16 addr, u8 *txbuf, int len)
{
    int ret = 0;
    #if defined(TOUCH_I2C_USE)
         ret = Touch_I2C_Write( addr, txbuf, len );
    #elif defined(TOUCH_SPI_USE)
         ret = Touch_SPI_Write( addr, txbuf, len );
    #endif
    return ret;
}

int TouchReadByteReg(u16 addr, u8 *rxbuf)
{
    int ret = 0;

    #if defined(TOUCH_I2C_USE)
       ret = Touch_I2C_Read( addr, rxbuf, 1 );
    #elif defined(TOUCH_SPI_USE)
       ret = Touch_SPI_Read( addr, rxbuf, 1 );
    #endif

    return ret;
}

int TouchWriteByteReg(u16 addr, u8 txbuf)
{
    int ret = 0;
	u8 data = txbuf;

    #if defined(TOUCH_I2C_USE)
         ret = Touch_I2C_Write( addr, &data, 1 );
    #elif defined(TOUCH_SPI_USE)
         ret = Touch_SPI_Write( addr, &data, 1);
    #endif

    return ret;
}

/* End Of File */

