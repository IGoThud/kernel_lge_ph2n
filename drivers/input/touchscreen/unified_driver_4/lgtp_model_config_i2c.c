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
 *    File  : lgtp_model_config_i2c.c
 *    Author(s)   : D3 BSP Touch Team < d3-bsp-touch@lge.com >
 *    Description :
 *
 ***************************************************************************/
#define LGTP_MODULE "[CONFIG]"

/****************************************************************************
* Include Files
****************************************************************************/
#include <linux/input/unified_driver_4/lgtp_common.h>
#include <linux/input/unified_driver_4/lgtp_model_config_i2c.h>



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
static struct of_device_id Mit300_MatchTable[] = {
    { .compatible = "unified_driver4,mit300", },
	{ },
};


/****************************************************************************
* Extern Function Prototypes
****************************************************************************/


/****************************************************************************
* Local Function Prototypes
****************************************************************************/


/****************************************************************************
* Local Functions
****************************************************************************/


/****************************************************************************
* Global Functions
****************************************************************************/
struct of_device_id * TouchGetDeviceMatchTable(int index)
{
	struct of_device_id * match_table = NULL;

    if (index == FIRST_MODULE)
		match_table = Mit300_MatchTable;

	return match_table;

}


/* End Of File */

