/*
 * Copyright (c) 2018 naehrwert
 * Copyright (C) 2018 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils/btn.h"
#include "soc/i2c.h"
#include "soc/gpio.h"
#include "soc/t210.h"
#include "utils/util.h"
#include "power/max77620.h"

u32 btn_read()
{
	u32 res = 0;
	if (!gpio_read(GPIO_PORT_X, GPIO_PIN_7))
		res |= BTN_VOL_DOWN;
	if (!gpio_read(GPIO_PORT_X, GPIO_PIN_6))
		res |= BTN_VOL_UP;
	if (i2c_recv_byte(4, MAX77620_I2C_ADDR, 0x15) & 0x4)
		res |= BTN_POWER;
	return res;
}

u32 btn_wait()
{
	u32 res = 0, btn = btn_read();
	bool pwr = false;

	//Power button down, raise a filter.
	if (btn & BTN_POWER)
	{
		pwr = true;
		btn &= ~BTN_POWER;
	}

	do
	{
		res = btn_read();
		//Power button up, remove filter.
		if (!(res & BTN_POWER) && pwr)
			pwr = false;
		else if (pwr) //Power button still down.
			res &= ~BTN_POWER;
	} while (btn == res);

	return res;
}

u32 btn_wait_timeout(u32 time_ms, u32 mask)
{
	u32 timeout = get_tmr_ms() + time_ms;
	u32 res = btn_read() & mask;

	do
	{
		if (!(res & mask))
			res = btn_read() & mask;
	} while (get_tmr_ms() < timeout);

	return res;
}
