/*
 * Copyright (c) 2018 naehrwert
 * Copyright (c) 2018 CTCaer
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

#include <string.h>

#include "soc/hw_init.h"
#include "gfx/di.h"
#include "mem/mc.h"
#include "mem/sdram.h"
#include "power/max77620.h"
#include "power/max7762x.h"
#include "sec/se.h"
#include "sec/se_t210.h"
#include "soc/clock.h"
#include "soc/fuse.h"
#include "soc/gpio.h"
#include "soc/i2c.h"
#include "soc/pinmux.h"
#include "soc/pmc.h"
#include "soc/t210.h"
#include "soc/uart.h"
#include "storage/sdmmc.h"
#include "utils/util.h"
#include "utils/fs_utils.h"

void _config_oscillators()
{
	CLOCK(CLK_RST_CONTROLLER_SPARE_REG0) = (CLOCK(CLK_RST_CONTROLLER_SPARE_REG0) & 0xFFFFFFF3) | 4;
	SYSCTR0(SYSCTR0_CNTFID0) = 19200000;
	TMR(TIMERUS_USEC_CFG) = 0x45F; // For 19.2MHz clk_m.
	CLOCK(CLK_RST_CONTROLLER_OSC_CTRL) = 0x50000071;
	PMC(APBDEV_PMC_OSC_EDPD_OVER) = (PMC(APBDEV_PMC_OSC_EDPD_OVER) & 0xFFFFFF81) | 0xE;
	PMC(APBDEV_PMC_OSC_EDPD_OVER) = (PMC(APBDEV_PMC_OSC_EDPD_OVER) & 0xFFBFFFFF) | 0x400000;
	PMC(APBDEV_PMC_CNTRL2) = (PMC(APBDEV_PMC_CNTRL2) & 0xFFFFEFFF) | 0x1000;
	PMC(APBDEV_PMC_SCRATCH188) = (PMC(APBDEV_PMC_SCRATCH188) & 0xFCFFFFFF) | 0x2000000;
	CLOCK(CLK_RST_CONTROLLER_CLK_SYSTEM_RATE) = 0x10;
	CLOCK(CLK_RST_CONTROLLER_PLLMB_BASE) &= 0xBFFFFFFF;
	PMC(APBDEV_PMC_TSC_MULT) = (PMC(APBDEV_PMC_TSC_MULT) & 0xFFFF0000) | 0x249F; //0x249F = 19200000 * (16 / 32.768 kHz)
	CLOCK(CLK_RST_CONTROLLER_SCLK_BURST_POLICY) = 0x20004444;
	CLOCK(CLK_RST_CONTROLLER_SUPER_SCLK_DIVIDER) = 0x80000000;
	CLOCK(CLK_RST_CONTROLLER_CLK_SYSTEM_RATE) = 2;
}

void _config_gpios()
{
	
	/* Clear PINMUX_AUX I2C3 registers; required for reliable touch init */
	PINMUX_AUX(PINMUX_AUX_X_I2C_SCL(I2C_4)) = 0; // actually listed in the TRM as for I2C3
	PINMUX_AUX(PINMUX_AUX_X_I2C_SDA(I2C_4)) = 0; // actually listed in the TRM as for I2C3
	
	PINMUX_AUX(PINMUX_AUX_UART2_TX) = 0;
	PINMUX_AUX(PINMUX_AUX_UART3_TX) = 0;

	PINMUX_AUX(PINMUX_AUX_GPIO_PE6) = PINMUX_INPUT_ENABLE;
	PINMUX_AUX(PINMUX_AUX_GPIO_PH6) = PINMUX_INPUT_ENABLE;

	gpio_config(GPIO_PORT_E, GPIO_PIN_6, GPIO_MODE_GPIO);
	gpio_config(GPIO_PORT_H, GPIO_PIN_6, GPIO_MODE_GPIO);
	gpio_output_enable(GPIO_PORT_G, GPIO_PIN_0, GPIO_OUTPUT_DISABLE);
	gpio_output_enable(GPIO_PORT_D, GPIO_PIN_1, GPIO_OUTPUT_DISABLE);
	gpio_output_enable(GPIO_PORT_E, GPIO_PIN_6, GPIO_OUTPUT_DISABLE);
	gpio_output_enable(GPIO_PORT_H, GPIO_PIN_6, GPIO_OUTPUT_DISABLE);

	pinmux_config_i2c(I2C_1);
	pinmux_config_i2c(I2C_3);
	pinmux_config_i2c(I2C_5);
	pinmux_config_uart(UART_A);

	// Configure volume up/down as inputs.
	gpio_config(GPIO_PORT_X, GPIO_PIN_6, GPIO_MODE_GPIO);
	gpio_config(GPIO_PORT_X, GPIO_PIN_7, GPIO_MODE_GPIO);
	gpio_output_enable(GPIO_PORT_X, GPIO_PIN_6, GPIO_OUTPUT_DISABLE);
	gpio_output_enable(GPIO_PORT_X, GPIO_PIN_7, GPIO_OUTPUT_DISABLE);

	PINMUX_AUX(PINMUX_AUX_DAP4_SCLK) = PINMUX_PULL_UP | 3;
	gpio_config(GPIO_PORT_J, GPIO_PIN_7, GPIO_MODE_GPIO);
	gpio_output_enable(GPIO_PORT_J, GPIO_PIN_7, GPIO_OUTPUT_ENABLE);
	gpio_write(GPIO_PORT_J, GPIO_PIN_7, GPIO_HIGH);
}

void _config_pmc_scratch()
{
	PMC(APBDEV_PMC_SCRATCH20) &= 0xFFF3FFFF;
	PMC(APBDEV_PMC_SCRATCH190) &= 0xFFFFFFFE;
	PMC(APBDEV_PMC_SECURE_SCRATCH21) |= 0x10;
}

void _mbist_workaround()
{
	CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_SOR1) = (CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_SOR1) | 0x8000) & 0xFFFFBFFF;
	CLOCK(CLK_RST_CONTROLLER_PLLD_BASE) |= 0x40800000u;
	CLOCK(CLK_RST_CONTROLLER_RST_DEV_Y_CLR) = 0x40;
	CLOCK(CLK_RST_CONTROLLER_RST_DEV_X_CLR) = 0x40000;
	CLOCK(CLK_RST_CONTROLLER_RST_DEV_L_CLR) = 0x18000000;
	usleep(2);

	I2S(I2S1_CTRL) |= I2S_CTRL_MASTER_EN;
	I2S(I2S1_CG)   &= ~I2S_CG_SLCG_ENABLE;
	I2S(I2S2_CTRL) |= I2S_CTRL_MASTER_EN;
	I2S(I2S2_CG)   &= ~I2S_CG_SLCG_ENABLE;
	I2S(I2S3_CTRL) |= I2S_CTRL_MASTER_EN;
	I2S(I2S3_CG)   &= ~I2S_CG_SLCG_ENABLE;
	I2S(I2S4_CTRL) |= I2S_CTRL_MASTER_EN;
	I2S(I2S4_CG)   &= ~I2S_CG_SLCG_ENABLE;
	I2S(I2S5_CTRL) |= I2S_CTRL_MASTER_EN;
	I2S(I2S5_CG)   &= ~I2S_CG_SLCG_ENABLE;
	DISPLAY_A(_DIREG(DC_COM_DSC_TOP_CTL)) |= 4;
	VIC(0x8C) = 0xFFFFFFFF;
	usleep(2);

	CLOCK(CLK_RST_CONTROLLER_RST_DEV_Y_SET) = 0x40;
	CLOCK(CLK_RST_CONTROLLER_RST_DEV_L_SET) = 0x18000000;
	CLOCK(CLK_RST_CONTROLLER_RST_DEV_X_SET) = 0x40000;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_H) = 0xC0;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_L) = 0x80000130;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_U) = 0x1F00200;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_V) = 0x80400808;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_W) = 0x402000FC;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_X) = 0x23000780;
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_Y) = 0x300;
	CLOCK(CLK_RST_CONTROLLER_LVL2_CLK_GATE_OVRA) = 0;
	CLOCK(CLK_RST_CONTROLLER_LVL2_CLK_GATE_OVRB) = 0;
	CLOCK(CLK_RST_CONTROLLER_LVL2_CLK_GATE_OVRC) = 0;
	CLOCK(CLK_RST_CONTROLLER_LVL2_CLK_GATE_OVRD) = 0;
	CLOCK(CLK_RST_CONTROLLER_LVL2_CLK_GATE_OVRE) = 0;
	CLOCK(CLK_RST_CONTROLLER_PLLD_BASE) &= 0x1F7FFFFF;
	CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_SOR1) &= 0xFFFF3FFF;
	CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_VI) = (CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_VI) & 0x1FFFFFFF) | 0x80000000;
	CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_HOST1X) = (CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_HOST1X) & 0x1FFFFFFF) | 0x80000000;
	CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_NVENC) = (CLOCK(CLK_RST_CONTROLLER_CLK_SOURCE_NVENC) & 0x1FFFFFFF) | 0x80000000;
}

void _config_se_brom()
{
	// Bootrom part we skipped.
	u32 sbk[4] = { 
		FUSE(FUSE_PRIVATE_KEY0),
		FUSE(FUSE_PRIVATE_KEY1),
		FUSE(FUSE_PRIVATE_KEY2),
		FUSE(FUSE_PRIVATE_KEY3)
	};
	// Set SBK to slot 14.
	se_aes_key_set(14, sbk, 0x10);

	// Lock SBK from being read.
	SE(SE_KEY_TABLE_ACCESS_REG_OFFSET + 14 * 4) = 0x7E;

	// This memset needs to happen here, else TZRAM will behave weirdly later on.
	memset((void *)TZRAM_BASE, 0, 0x10000);
	PMC(APBDEV_PMC_CRYPTO_OP) = 0;
	SE(SE_INT_STATUS_REG_OFFSET) = 0x1F;

	// Lock SSK (although it's not set and unused anyways).
	SE(SE_KEY_TABLE_ACCESS_REG_OFFSET + 15 * 4) = 0x7E;

	// Clear the boot reason to avoid problems later
	PMC(APBDEV_PMC_SCRATCH200) = 0x0;
	PMC(APBDEV_PMC_RST_STATUS) = 0x0;
	APB_MISC(APB_MISC_PP_STRAPPING_OPT_A) = (APB_MISC(APB_MISC_PP_STRAPPING_OPT_A) & 0xF0) | (7 << 10);
}

void config_hw()
{
	// Bootrom stuff we skipped by going through rcm.
	_config_se_brom();
	//FUSE(FUSE_PRIVATEKEYDISABLE) = 0x11;
	SYSREG(AHB_AHB_SPARE_REG) &= 0xFFFFFF9F;
	PMC(APBDEV_PMC_SCRATCH49) = ((PMC(APBDEV_PMC_SCRATCH49) >> 1) << 1) & 0xFFFFFFFD;

	_mbist_workaround();
	clock_enable_se();

	// Enable fuse clock.
	clock_enable_fuse(true);
	// Disable fuse programming.
	fuse_disable_program();

	mc_enable();

	_config_oscillators();
	APB_MISC(APB_MISC_PP_PINMUX_GLOBAL) = 0;
	_config_gpios();

	clock_enable_cl_dvfs();

	clock_enable_i2c(I2C_1);
	clock_enable_i2c(I2C_3);
	clock_enable_i2c(I2C_5);

	clock_enable_unk2();

	i2c_init(I2C_1);
	i2c_init(I2C_3);
	i2c_init(I2C_5);

	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_CNFGBBC, 0x40);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_ONOFFCNFG1, 0x78); // PWR delay for forced shutdown off.

	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_CFG0, 0x38);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_CFG1, 0x3A);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_CFG2, 0x38);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_LDO4, 0xF);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_LDO8, 0xC7);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_SD0, 0x4F);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_SD1, 0x29);
	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_SD3, 0x1B);

    // Enables LDO6 for touchscreen AVDD supply
	i2c_send_byte(I2C_5, 0x3C, MAX77620_REG_LDO6_CFG, 0xEA);
	i2c_send_byte(I2C_5, 0x3C, MAX77620_REG_LDO6_CFG2, 0xDA);

	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_FPS_GPIO3, 0x22); // 3.x+

	i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_SD0, 42); //42 = (1125000uV - 600000) / 12500 -> 1.125V

	_config_pmc_scratch(); // Missing from 4.x+

	CLOCK(CLK_RST_CONTROLLER_SCLK_BURST_POLICY) = (CLOCK(CLK_RST_CONTROLLER_SCLK_BURST_POLICY) & 0xFFFF8888) | 0x3333;

	sdram_init();
}

void reconfig_hw_workaround(bool extra_reconfig, u32 magic)
{
	// Re-enable clocks to Audio Processing Engine as a workaround to hanging.
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_V) |= (1 << 10); // Enable AHUB clock.
	CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_Y) |= (1 <<  6);  // Enable APE clock.

	if (extra_reconfig)
	{
		msleep(10);
		PMC(APBDEV_PMC_PWR_DET_VAL) |= PMC_PWR_DET_SDMMC1_IO_EN;

		clock_disable_cl_dvfs();

		// Disable Joy-con GPIOs.
		gpio_config(GPIO_PORT_G, GPIO_PIN_0, GPIO_MODE_SPIO);
		gpio_config(GPIO_PORT_D, GPIO_PIN_1, GPIO_MODE_SPIO);
		gpio_config(GPIO_PORT_E, GPIO_PIN_6, GPIO_MODE_SPIO);
		gpio_config(GPIO_PORT_H, GPIO_PIN_6, GPIO_MODE_SPIO);
		
		// Fix GPU after warmboot for Linux.
		i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_GPIO5, 2);
		i2c_send_byte(I2C_5, MAX77620_I2C_ADDR, MAX77620_REG_GPIO6, 2);

		// Touch
		gpio_config(GPIO_PORT_J, GPIO_PIN_7, GPIO_MODE_SPIO);
	}

	// Power off display.
	display_end();

	// Enable clock to USBD and init SDMMC1 to avoid hangs with bad hw inits.
	if (magic == 0xBAADF00D)
	{
		CLOCK(CLK_RST_CONTROLLER_CLK_OUT_ENB_L) |= (1 << 22);
		sdmmc_init(&g_sd_sdmmc, SDMMC_1, SDMMC_POWER_3_3, SDMMC_BUS_WIDTH_1, 5, 0);
		clock_disable_cl_dvfs();

		msleep(200);
	}
}
