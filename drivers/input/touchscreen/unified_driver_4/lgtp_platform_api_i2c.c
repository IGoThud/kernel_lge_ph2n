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
 *    File  	: lgtp_platform_i2c.c
 *    Author(s)   : D3 BSP Touch Team < d3-bsp-touch@lge.com >
 *    Description :
 *
 ***************************************************************************/
#define LGTP_MODULE "[I2C]"

/****************************************************************************
* Include Files
****************************************************************************/
#include <linux/input/unified_driver_4/lgtp_common.h>
#include <linux/input/unified_driver_4/lgtp_model_config_i2c.h>
#include <linux/input/unified_driver_4/lgtp_platform_api_misc.h>


/****************************************************************************
* Manifest Constants / Defines
****************************************************************************/
#define LGE_TOUCH_NAME "touch_i2c"

/****************************************************************************
 * Macros
 ****************************************************************************/


/****************************************************************************
* Type Definitions
****************************************************************************/

/****************************************************************************
* Variables
****************************************************************************/
static struct i2c_client *pClient = NULL;

/****************************************************************************
* Extern Function Prototypes
****************************************************************************/


/****************************************************************************
* Local Function Prototypes
****************************************************************************/


/****************************************************************************
* Local Functions
****************************************************************************/
static int i2c_read(u8 *reg, int regLen, u8 *buf, int dataLen)
{
	int ret = 0;
    int retry = 0;

    struct i2c_msg msgs[2] = {
        { .addr = pClient->addr, .flags = 0, .len = regLen, .buf = reg, },
        { .addr = pClient->addr, .flags = I2C_M_RD, .len = dataLen, .buf = buf, },
    };

    do {
        ret = i2c_transfer(pClient->adapter, &msgs[0], 1);
	ret += i2c_transfer(pClient->adapter, &msgs[1], 1);

        if (ret < 0) {
            TOUCH_ERR("i2c retry [%d]\n", retry+1);
            msleep(20);
        } else {
            return TOUCH_SUCCESS;
        }
    } while(++retry < 3);

	return TOUCH_FAIL;
}

static int i2c_write(u8 *reg, int regLen, u8 *buf, int dataLen)
{
	int ret = 0;
    int retry = 0;
	struct i2c_msg msg = {
		.addr = pClient->addr, .flags = pClient->flags, .len = (regLen+dataLen), .buf = NULL,
	};

	u8 *pTmpBuf = NULL;

	 pTmpBuf = (u8 *)kcalloc(1, regLen+dataLen, GFP_KERNEL);
	 if(pTmpBuf != NULL) {
		 memset(pTmpBuf, 0x00, regLen+dataLen);
	 } else {
	     TOUCH_ERR("i2c_write kcalloc fail\n");
		 return TOUCH_FAIL;
	 }

	memcpy(pTmpBuf, reg, regLen);
	memcpy((pTmpBuf+regLen), buf, dataLen);

	msg.buf = pTmpBuf;

    do{
	    ret = i2c_transfer(pClient->adapter, &msg, 1);

	if (ret < 0) {
            TOUCH_ERR("i2c retry [%d]\n", retry+1);
            msleep(20);
	    }else{
            goto out;
	    }
    }while(retry++ < 3);

	kfree(pTmpBuf);
	return TOUCH_FAIL;
out:
    kfree(pTmpBuf);
    return TOUCH_SUCCESS;
}


/****************************************************************************
* Global Functions
****************************************************************************/
struct i2c_client *Touch_Get_I2C_Handle(void)
{
	return pClient;
}

int Mit300_I2C_Read ( struct i2c_client *client, u8 *addr, u8 addrLen, u8 *rxbuf, int len )
{
	int ret = 0;

	ret = i2c_read( addr, addrLen, rxbuf, len);
	if( ret == TOUCH_FAIL ) {
		if (printk_ratelimit()) {
			TOUCH_ERR("failed to read i2c ( reg = %d )\n", (u16)((addr[0]<<7)|addr[1]));
		}
		return TOUCH_FAIL;
	}

	return TOUCH_SUCCESS;
}

int Mit300_I2C_Write ( struct i2c_client *client, u8 *writeBuf, u32 write_len )
{
	int ret = 0;

	ret = i2c_write( writeBuf, 2, writeBuf+2, write_len-2);
	if( ret == TOUCH_FAIL ) {
		if (printk_ratelimit()) {
			TOUCH_ERR("failed to write i2c ( reg = %d )\n", (u16)((writeBuf[0]<<7)|writeBuf[1]));
		}
		return TOUCH_FAIL;
	}

	return TOUCH_SUCCESS;
}

int Touch_I2C_Read ( u16 addr, u8 *rxbuf, int len )
{
	int ret = 0;

    #if defined(TOUCH_I2C_ADDRESS_8BIT)
       u8 regValue = (u8)addr;
       ret = i2c_read(&regValue, 1, rxbuf, len);
    #elif defined(TOUCH_I2C_ADDRESS_16BIT)
       u8 regValue[2] = { ( (addr>>8) & 0xFF ), ( addr & 0xFF ) };
       ret = i2c_read(regValue, 2, rxbuf, len);
    #endif

	if( ret == TOUCH_FAIL ) {
		if (printk_ratelimit()) {
			TOUCH_ERR("failed to read i2c ( reg = %d )\n", addr);
		}
		return TOUCH_FAIL;
	}

	return TOUCH_SUCCESS;
}

int Touch_I2C_Write ( u16 addr, u8 *txbuf, int len )
{
	int ret = 0;

    #if defined(TOUCH_I2C_ADDRESS_8BIT)
        u8 regValue = (u8)addr;
        ret = i2c_write(&regValue, 1, txbuf, len);
    #elif defined(TOUCH_I2C_ADDRESS_16BIT)
        u8 regValue[2] = { ( (addr>>8) & 0xFF ), ( addr & 0xFF ) };
        ret = i2c_write(regValue, 2, txbuf, len);
    #endif
	if( ret == TOUCH_FAIL ) {
		if (printk_ratelimit()) {
			TOUCH_ERR("failed to write i2c ( reg = %d )\n", addr);
		}
		return TOUCH_FAIL;
	}

	return TOUCH_SUCCESS;
}

static int touch_i2c_pm_suspend(struct device *dev)
{
	TOUCH_FUNC();

	return TOUCH_SUCCESS;
}

static int touch_i2c_pm_resume(struct device *dev)
{
	TOUCH_FUNC();

	return TOUCH_SUCCESS;
}

static int touch_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	TOUCH_FUNC();

	pClient = client;

	return TOUCH_SUCCESS;
}

static int touch_i2c_remove(struct i2c_client *client)
{
	pClient = NULL;

	TOUCH_FUNC();

	return TOUCH_SUCCESS;
}

static struct i2c_device_id touch_i2c[] = {
	{LGE_TOUCH_NAME, 0},
};

static struct dev_pm_ops touch_i2c_pm_ops = {
	.suspend = touch_i2c_pm_suspend,
	.resume = touch_i2c_pm_resume,
};

static struct i2c_driver lge_touch_driver = {
	.probe = touch_i2c_probe,
	.remove = touch_i2c_remove,
	.id_table = touch_i2c,
	.driver = {
		.name = LGE_TOUCH_NAME,
		.owner = THIS_MODULE,
		.pm = &touch_i2c_pm_ops,
	},
};

static int __init touch_i2c_init(void)
{

#if defined(TOUCH_I2C_USE)
    int idx = 0;

	TOUCH_FUNC();

	idx = TouchGetModuleIndex();

	lge_touch_driver.driver.of_match_table = TouchGetDeviceMatchTable(idx);

	if( i2c_add_driver ( &lge_touch_driver ) ) {
		TOUCH_ERR("failed at i2c_add_driver()\n" );
		return -ENODEV;
	}
#else
    TOUCH_LOG("TOUCH_I2C_USE is not defined in this model\n" );
#endif
	return TOUCH_SUCCESS;

}

static void __exit touch_i2c_exit(void)
{
	TOUCH_FUNC();

	i2c_del_driver(&lge_touch_driver);
}

module_init(touch_i2c_init);
module_exit(touch_i2c_exit);

MODULE_AUTHOR("D3 BSP Touch Team");
MODULE_DESCRIPTION("LGE Touch Unified Driver");
MODULE_LICENSE("GPL");

/* End Of File */

