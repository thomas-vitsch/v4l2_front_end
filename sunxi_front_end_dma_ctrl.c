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
#include "sunxi_front_end_dma_ctrl.h"

static uint64_t div64(uint64_t a, uint32_t b)
{
	uint64_t mult, bb, result;

	bb = b;
	mult = 1;
	while (bb < a) {
		bb <<= 1;
		mult <<= 1;
	}

	result = 0;
	while (mult > 0) {
		while (a >= bb) {
			a -= bb;
			result += mult;
		}
		bb >>= 1;
		mult >>= 1;
	}
	return result;
}

int setup_fe_idma_channels(struct sunxi_fe_device *sunxi_fe_dev)
{

	switch (sunxi_fe_dev->input_fmt)  {
	case DRM_FORMAT_YUV420:

	if (setup_fe_idma_channel(sunxi_fe_dev, IN_CHAN_Y) ) {
		printk("Cannot set input channels with current input "
		    "format\n");
		return -1;
	}
	if (setup_fe_idma_channel(sunxi_fe_dev, IN_CHAN_UV) ) {
		printk("Cannot set input channels with current input "
		    "format\n");
		return -1;
	}
		break;
	default:
		printk("Cannot set input channels with current input "
		    "format\n");
		return -1;
	}

	return 0;
}

int setup_fe_odma_channels(struct sunxi_fe_device *sunxi_fe_dev)
{

	switch (sunxi_fe_dev->input_fmt)  {
	case DRM_FORMAT_YUV420:
		if (setup_fe_odma_channel(sunxi_fe_dev, OUT_CHAN_Y) < 0) {
			printk("Cannot set output channels with current "
			    "output format\n");
			return -1;
		}
		if (setup_fe_odma_channel(sunxi_fe_dev, OUT_CHAN_UV) < 0) {
			printk("Cannot set output channels with current "
			    "output format\n");
			return -1;
		}
		break;
	default:
		printk("Cannot set output channels with current output "
		    "format\n");
		return -1;
	}
	return 0;
}

int setup_fe_odma_channel(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel)
{
	struct regmap *r;
	int result;

	if (set_fe_odma_outsize(sunxi_fe_dev, channel, sunxi_fe_dev->out_width,
	     sunxi_fe_dev->out_height) < 0) {
		printk("Could not configure outsize of channel %d\n", channel);
		return -1;
	}
	if (set_fe_odma_scaler(sunxi_fe_dev, channel, sunxi_fe_dev->in_width,
	    sunxi_fe_dev->in_height, sunxi_fe_dev->out_width,
	    sunxi_fe_dev->out_height) < 0) {
		printk("Could not configure scaler of channel %d\n", channel);
		return -1;
	}
	return 0;
}

int set_fe_odma_outsize(struct sunxi_fe_device *sunxi_fe_dev, uint32_t channel,
	uint32_t width, uint32_t height)
{
	int result;
	uint32_t channel_reg;

	switch (sunxi_fe_dev->input_fmt) {
	case DRM_FORMAT_YUV420:
		if (channel == IN_CHAN_UV)
			width++; //Needs to be incremented by 1.
		break;
	default:
		/* Nothing, continue */
		break;
	}

	channel_reg = DEFE_CH0_OUTSIZE_REG + (channel *
	    IN_CHAN_OUTSIZE_OFFSET);

	result = regmap_write(sunxi_fe_dev->regs, channel_reg,
	    DEFE_CHX_OUT_WIDTH(width) | DEFE_CHX_OUT_HEIGHT(height));
	if (result == -EIO) {
		printk("Could not set ch0 out size\n");
		return -1;
	}
	return 0;
}

uint32_t calc_fe_scaler_uv_fact(uint32_t x_in, uint32_t x_out)
{

	return calc_fe_scaler_y_fact(x_in/2, x_out);
}

uint32_t calc_fe_scaler_y_fact(uint32_t x_in, uint32_t x_out)
{

	return (uint32_t)div64(TO_SCALER_FLOAT((uint32_t)x_in), x_out);
}

int set_fe_odma_scaler(struct sunxi_fe_device *sunxi_fe_dev, uint32_t channel,
    uint32_t in_width, uint32_t in_height, uint32_t out_width,
    uint32_t out_height)
{
	uint32_t horz_fact, vert_fact;
	uint32_t channel_reg;
	int result;

	switch (sunxi_fe_dev->input_fmt) {
	case DRM_FORMAT_YUV420:
		if (channel == IN_CHAN_Y) {
			horz_fact = calc_fe_scaler_y_fact(in_width, out_width);
			vert_fact = calc_fe_scaler_y_fact(in_height,
			    out_height);
		} else if (channel == IN_CHAN_UV) {
			horz_fact = calc_fe_scaler_uv_fact(in_width,
			    out_width);
			vert_fact = calc_fe_scaler_uv_fact(in_height,
			    out_height);
	} else {
		printk("Error: invalid channel selected for current input "
		    "format\n");
		return -1;
	}
		break;
	default:
		printk("Error: Unknown format for scaler channels.\n");
		return -1;
	}

	channel_reg = DEFE_CH0_HORZFACT_REG + (channel *
	    IN_CHAN_HORZ_VERT_FACT_OFFSET);

	result = regmap_write(sunxi_fe_dev->regs, channel_reg, horz_fact);
	if (result == -EIO) {
		printk("Could not set ch0 horz scale factor.\n");
		return result;
	}

	channel_reg = DEFE_CH0_VERTFACT_REG + (channel *
	    IN_CHAN_HORZ_VERT_FACT_OFFSET);

	result = regmap_write(sunxi_fe_dev->regs, channel_reg, vert_fact);
	if (result == -EIO) {
		printk("Could not set ch0 vert scale factor.\n");
		return result;
	}

	return 0;
}

int setup_fe_idma_channel(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel)
{
	struct regmap *r;
	int result;

	r = sunxi_fe_dev->regs;

	result = set_fe_idma_tile_offsets(sunxi_fe_dev, channel, TILE_LEN);
	if (result < 0)
		return result;

	result = set_fe_idma_linestride(sunxi_fe_dev, channel,
	    sunxi_fe_dev->in_width, TILE_LEN);
	if (result < 0)
		return result;

	result = set_fe_idma_insize(sunxi_fe_dev, channel,
	    sunxi_fe_dev->in_width, sunxi_fe_dev->in_height);
	if (result < 0)
		return result;

	return 0;
}

int set_fe_idma_tile_offsets(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel, uint32_t tile_len)
{
	uint32_t channel_reg;

	channel_reg = DEFE_TB_OFF0_REG + (channel * IN_CHAN_ADDR_OFFSET);
	return regmap_write(sunxi_fe_dev->regs, channel_reg,
	    DEFE_TB_OFFSETS(tile_len, 0, tile_len));
}

int set_fe_idma_linestride(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel, uint32_t width, uint32_t tile_len)
{
	uint32_t channel_reg;

	channel_reg = DEFE_LINESTRD0_REG + (channel * IN_CHAN_ADDR_OFFSET);

	//TODO: DRM_FORMAT_YUV420 is not tiled, go find tiled define!!!!!
	switch (sunxi_fe_dev->input_fmt) {
	case DRM_FORMAT_YUV420:
		return regmap_write(sunxi_fe_dev->regs, channel_reg,
		    DEFE_TILED_LINESTRIDE(width, tile_len));
		break;
	default:
		printk("Unknown linestride calc for input format %d\n",
		    sunxi_fe_dev->input_fmt);
		return -1;
	}
}

int set_fe_idma_insize(struct sunxi_fe_device *sunxi_fe_dev, uint32_t channel,
    uint32_t width, uint32_t height)
{
	uint32_t channel_reg;

	channel_reg = DEFE_CH0_INSIZE_REG + (channel * IN_CHAN_INSIZE_OFFSET);

	//TODO: DRM_FORMAT_YUV420 is not tiled, go find tiled define!!!!!
	if ((sunxi_fe_dev->input_fmt == DRM_FORMAT_YUV420) &&
	    channel == IN_CHAN_UV)
		return regmap_write(sunxi_fe_dev->regs, channel_reg,
		    DEFE_CHX_IN_WIDTH_UV(width) |
		    DEFE_CHX_IN_HEIGHT_UV(height));
	else
		return regmap_write(sunxi_fe_dev->regs, channel_reg,
		    DEFE_CHX_IN_WIDTH_Y(width) | DEFE_CHX_IN_HEIGHT_Y(height));
}

// TODO make set channel addr function
// /* Input consists of 2 input channels. Y and UV */
// /* TODO derive phys addr from a gem buffer handle. */
// if (set_fe_idma_channel_dma_addr(IN_CHAN_Y, 0x01000000, 
//	 sunxi_fe_dev) < 0)
//	 return -1;
// if (set_fe_idma_channel_dma_addr(IN_CHAN_UV, 0x01400000, 
//	 sunxi_fe_dev) < 0)
//	 return -1;

// /* Input consists of 2 input channels. Y and UV */
// /* TODO derive phys addr from a gem buffer handle. */
// if (set_fe_idma_channel_dma_addr(IN_CHAN_Y, 0x01000000, 
//	 sunxi_fe_dev) < 0)
//	 return -1;
// if (set_fe_idma_channel_dma_addr(IN_CHAN_UV, 0x01400000, 
//	 sunxi_fe_dev) < 0)
//	 return -1;

