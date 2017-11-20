/*
 * Copyright (C) 2017 Vitsch Electronics
 *
 * Thomas van Kleef <linux-dev@vitsch.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <uapi/drm/drm_fourcc.h>
#include "sunxi_front_end.h"
#include "sunxi_front_end_registers.h"
#include "sunxi_front_end_color_space_converter.h"

/*
 * See https://en.wikipedia.org/wiki/YCbCr#ITU-R_BT.601_conversion
 *
 * These are the coefficients for BT.601 YUV->RGB conversion in studio swing,
 * scaled to the resolution of the CSC's coefficient registers.
 * We configure this as default here. The driver should eventually expose an
 * interface to let the user configure the color space conversion coefficients
 * on the fly so that a movie that's encoded in a different color space (e.g.
 * BT.709 or full swing) can be played back correctly.
 */
static const int bt_601_coef[NR_CSC_COLORS][NR_CSC_COLOR_COEF] = {
	// Y mag	U mag	V mag	const
	// *2^10	*2^10	*2^10	*2^4
	// R
	{ +1192,	0,	+1634,	-3567 },
	// G
	{ +1192,	-401,	-832,	+2169 },
	// B
	{ +1192,	+2066,	+0,	-4429 },
};

/* Map coefficient indexes to CSC register numbers. */
static const uint32_t coef_to_reg[NR_CSC_COLORS][NR_CSC_COLOR_COEF] = {
	{
		DEFE_CSC_COEF_Y_IN_R_REG,
		DEFE_CSC_COEF_U_IN_R_REG,
		DEFE_CSC_COEF_V_IN_R_REG,
		DEFE_CSC_COEF_CONST_R_REG,
	},
	{
		DEFE_CSC_COEF_Y_IN_G_REG,
		DEFE_CSC_COEF_U_IN_G_REG,
		DEFE_CSC_COEF_V_IN_G_REG,
		DEFE_CSC_COEF_CONST_G_REG,
	},
	{
		DEFE_CSC_COEF_Y_IN_B_REG,
		DEFE_CSC_COEF_U_IN_B_REG,
		DEFE_CSC_COEF_V_IN_B_REG,
		DEFE_CSC_COEF_CONST_B_REG,
	},
};

int setup_csc(struct sunxi_fe_device *sunxi_fe_dev)
{
	int ret;
	uint8_t i, j;
	uint16_t val;

	// switch (sunxi_fe_dev->input_fmt) {
	// case DRM_FORMAT_YUV420:
	for (i = 0; i < NR_CSC_COLORS; i++) {
		for (j = 0; j < NR_CSC_COLOR_COEF; j++) {
			if (j < CSC_COLOR_CONST_COEF_POS)
				val = DEFE_CSC_COEF_MAG(bt_601_coef[i][j]);
			else
				val = DEFE_CSC_COEF_CONST(bt_601_coef[i][j]);

			PRINT_DE_FE("set coef @ offset 0x%x to 0x%x \n",
			    coef_to_reg[i][j], val);
			ret = regmap_write(sunxi_fe_dev->regs,
			    coef_to_reg[i][j], val);
			if (ret == -EIO) {
				printk("Could not set csc matrix[%d][%d].\n",
				    i, j);
				return -1;
			}
		}
	};

	ret = regmap_write(sunxi_fe_dev->regs, DEFE_INPUT_FMT_REG,
	    DEFE_INPUT_DATA_MOD(DEFE_MOD_TILE_BASED_UV_COMBINED) |
	    DEFE_INPUT_DATA_FMT(DEFE_INP_FMT_YUV420) |
	    DEFE_INPUT_PS(DEFE_INP_PS_U1V1U0V0) );
	if (ret == -EIO) {
		printk("Could not set input format.\n");
		return -1;
	}
	// default:
	// 	printk("Unsupported input format for csc.\n");
	// 	return -1;
	// }

	// switch (sunxi_fe_dev->output_fmt) {
	// case DRM_FORMAT_XRGB8888:
	ret = regmap_write(sunxi_fe_dev->regs, DEFE_OUTPUT_FMT_REG,
	    DEFE_OUTPUT_DATA_FMT(DEFE_OUT_FMT_INTERL_ARGB8888));
	if (ret == -EIO) {
		printk("Could not set output format.\n");
		return -1;
	}
	// break;
	// default:
	// 	printk("Unsupported output format for csc.\n");
	// 	return -1;
	// }

	ret = regmap_write(sunxi_fe_dev->regs, DEFE_BYPASS_REG,
	    DEFE_CSC_BYPASS_EN(DISABLE));
	if (ret == -EIO) {
		printk("Could not enable csc.\n");
		return -1;
	}

	return 0;
}

