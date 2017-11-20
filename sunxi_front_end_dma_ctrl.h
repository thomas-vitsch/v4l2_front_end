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

#ifndef SUNXI_FRONT_END_DMA_CTRL_H_
#define SUNXI_FRONT_END_DMA_CTRL_H_

#include "sunxi_front_end.h"

/*
 * Info below is taken from: "A13_user_manual_v1.2_2013_01_08.pdf"
 * Page: 290
 *
 * DEFE supports planar or interleaved video component data input via 3 input
 * channels: channel0, channel1, and channel2.
 * In planar mode, if the U, V data are not combined, channe0, 1, 2 refer to
 * the Y, U, V data channel respectively. If the U, V data are combined,
 * channel0 refers to the Y channel and channel 1 refers to the U, V combined
 * channel. Channel2 will be inactive in this case.
 * In interleaved mode, channel0 refers to UYVY (or VYUY, YUYV, or YVYU
 * depending on the configuration), channel1 and channel2 will be inactive.
 * Note: In interleaved YUV mode, only YUV422 and YUV444 formats are valid.
 */
#define IN_CHAN_Y				0
#define IN_CHAN_UV				1
#define IN_CHAN_ADDR_OFFSET			0x4
#define IN_CHAN_INSIZE_OFFSET			0x100
#define IN_CHAN_OUTSIZE_OFFSET			0x100
#define IN_CHAN_HORZ_VERT_FACT_OFFSET		0x100
#define TILE_LEN				32

#define OUT_CHAN_Y				0
#define OUT_CHAN_UV				1

/*
 * The scaler value contains an interger part (bit 24..16) and a fraction part
 * (bit 15..0). This value can be used to up-scale an interger to fit in the
 * scaler register. 1<<16 == 1.00000.
 */
#define TO_SCALER_FLOAT(x)			(x << 16)

/*
 * tile_offsets
 * x_bottom_right_end_tile: The x offset of the bottom-right point in the
 *  end tile.
 * y_top_left_first_tile: The y offset in the top-left point in the first
 *  tile.
 * x_top_left_first_tile: The x offset in the top-left point in the first
 *  tile.
 */
struct tile_offsets {
	uint8_t				x_bottom_right_end_tile;
	uint8_t				y_top_left_first_tile;
	uint8_t				x_top_left_first_tile;
};

/*
 * idma Input dma channel
 * id: channel id.
 * dma_addr: Src address to be used by dma_channel.
 * line_width: Width of frame in bytes or tile stride in bytes.
 *  till the next !vertical tile in a frame.
 * frame_height: Frame heigth in pixels.
 * frame_width: Frame width in pixels.
 */
struct idma {
	const uint32_t			id;
	uint32_t			dma_addr;
	uint32_t			line_width;
	uint16_t			frame_height, frame_width;
};

/*
 * odma output dma channel
 * id: channel id.
 * dma_addr: dest address to be used by dma_channel.
 * line_width: Width of frame in bits.
 */
// struct odma {
//     const uint32_t			id;
//     uint32_t				dma_addr;
//     uint32_t				line_width;
// };

/*
 * The front end contains 3 input dma channels and 3 output channels.
 * According to the set output format specific output channels and input
 * channels are used.
 *
 * Table below is taken from: "A20_0user_manual_v1.3_2014_10_10.pdf"
 * Page: 335
 * ============================================================================
 * Output data mode and output data ports mapping:
 * Output data mode			| Output data channel selection
 *					| Ch. 0		| Ch. 1		| Ch. 2
 * A8R8G8B8 or interleaved AYUV8888	| ARGB or AYUV	| Ignore	| Ignore
 * A4R4G4B4				| ARGB		| Ignore	| Ignore
 * A1R5G5B5				| ARGB		| Ignore	| Ignore
 * R5G6B5				| RGB		| Ignore	| Ignore
 * interleaved YUV422			| YUV		| Ignore	| Ignore
 * planar YUV422 (UV combined)		| Y		| UV		| Ignore
 * planar YUV422			| Y		| U		| V
 * 8bpp MONO				| MONO		| Ignore	| Ignore
 * 4bpp MONO				| MONO		| Ignore	| Ignore
 * 2bpp MONO				| MONO		| Ignore	| Ignore
 * 1bpp MONO				| MONO		| Ignore	| Ignore
 * planar YUV420 (UV combined)		| Y		| UV		| Ignore
 * planar YUV420			| Y		| U		| V
 * planar YUV411 (UV combined)		| Y		| UV		| Ignore
 * planar YUV411			| Y		| U		|
 * ----------------------------------------------------------------------------
 */
struct dma_control {
	//Input channels
	struct idma			idma0;
	struct idma			idma1;
	struct idma			idma2;
	struct idma			idma3;

	// //Output channels
	// struct odma 			odma0;
	// struct odma 			odma1;
	// struct odma 			odma2;

	uint16_t			output_frame_height;
	uint16_t			output_frame_width;
	uint8_t				output_fmt;
};

struct sunxi_fe_device;

int setup_fe_idma_channels(struct sunxi_fe_device *sunxi_fe_dev);
int setup_fe_idma_channel(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel);
int set_fe_idma_tile_offsets(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel, uint32_t tile_len);
int set_fe_idma_linestride(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel, uint32_t width_reg, uint32_t tile_len);
int set_fe_idma_insize(struct sunxi_fe_device *sunxi_fe_dev, uint32_t channel,
    uint32_t width, uint32_t height);
int setup_fe_odma_channels(struct sunxi_fe_device *sunxi_fe_dev);
int setup_fe_odma_channel(struct sunxi_fe_device *sunxi_fe_dev,
    uint32_t channel);
int set_fe_odma_outsize(struct sunxi_fe_device *sunxi_fe_dev, uint32_t channel,
    uint32_t width, uint32_t height);
int set_fe_odma_scaler(struct sunxi_fe_device *sunxi_fe_dev, uint32_t channel,
    uint32_t in_width, uint32_t in_height, uint32_t out_width,
    uint32_t out_height);

uint32_t calc_fe_scaler_uv_fact(uint32_t x_in, uint32_t x_out);
uint32_t calc_fe_scaler_y_fact(uint32_t x_in, uint32_t x_out);

#endif /* SUNXI_FRONT_END_DMA_CTRL_H_ */
