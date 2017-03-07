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
 *    File  : lgtp_model_config_i2c.h
 *    Author(s)   : D3 BSP Touch Team < d3-bsp-touch@lge.com >
 *    Description :
 *
 ***************************************************************************/

#if !defined(_LGTP_MODEL_CONFIG_I2C_H_)
#define _LGTP_MODEL_CONFIG_I2C_H_

/****************************************************************************
* Nested Include Files
****************************************************************************/


/****************************************************************************
* Mainfest Constants / Defines
****************************************************************************/

#if defined (TOUCH_MODEL_M2) || defined (TOUCH_MODEL_PH1)
#define TOUCH_I2C_USE
#define TOUCH_I2C_ADDRESS_8BIT
#define TOUCH_I2C_BUS_NUM 5

#elif defined(TOUCH_MODEL_PH2)
#define TOUCH_I2C_USE
#define TOUCH_I2C_ADDRESS_8BIT
#define TOUCH_I2C_BUS_NUM 3 

#endif

/****************************************************************************
* Type Definitions
****************************************************************************/


/****************************************************************************
* Exported Variables
****************************************************************************/


/****************************************************************************
* Macros
****************************************************************************/


/****************************************************************************
* Global Function Prototypes
****************************************************************************/
struct of_device_id * TouchGetDeviceMatchTable(int index);

#endif /* _LGTP_MODEL_CONFIG_I2C_H_ */

/* End Of File */

