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

#ifndef SUNXI_FRONT_END_H_
#define SUNXI_FRONT_END_H_

#include <linux/regmap.h>
#include "sunxi_front_end_dma_ctrl.h"
#include "sunxi_front_end_color_space_converter.h"
#include <uapi/misc/sunxi_front_end.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>

#define DRV_NAME "sunxi-front-end"

#define PRINT_DE_FE(fmt, args...)	((sunxi_de_fe_debug_lvl) == 0 ? 0 : \
    (printk( fmt, ## args)))

extern uint32_t sunxi_de_fe_debug_lvl;

struct sunxi_de_fe_fmt {
	u32					fourcc;
	int					depth;
	u32					types;
	unsigned int 	num_planes;
};

struct sunxi_de_fe_ctx {
	struct v4l2_fh				fh;
	struct sunxi_fe_device			*dev;

	/* Todo: thomas remove obsolete structs, such as destination fmt */
	struct sunxi_de_fe_fmt 			*vpu_src_fmt;
	struct sunxi_de_fe_fmt 			*vpu_dst_fmt;
	struct v4l2_pix_format_mplane 		src_fmt;
	struct v4l2_pix_format_mplane 		dst_fmt;

	struct v4l2_ctrl_handler 		hdl;

	struct vb2_buffer 			*dst_bufs[VIDEO_MAX_FRAME];

	struct v4l2_ctrl 			*mpeg2_frame_hdr_ctrl;
	struct v4l2_ctrl 			*mpeg4_frame_hdr_ctrl;
};

struct sunxi_fe_device {
	const char				*phys_name;
	struct device				*dev;
	struct regmap				*regs;
	struct clk				*ahb_clk;
	struct clk				*ram_clk;
	struct clk				*mod_clk;
	struct reset_control			*reset;

	struct v4l2_device			v4l2_dev;
	struct v4l2_m2m_dev			*m2m_dev;
	struct video_device			vfd;

	// /* Mutex for device file */
	// struct mutex				dev_mutex;
	// /* Spinlock for interrupt */
	// spinlock_t				irqlock;

	struct dma_control			dma_ctrl;

	struct sfe_input_buffers		in_bufs;
	uint32_t				in_width, in_height;
	uint32_t				out_width, out_height;
	uint32_t				input_fmt, output_fmt;

	dma_addr_t				dma_in_addr[MAX_INPUT_BUFFERS];
};

int fe_start_conversion(void);
void setup_fe_for_yuv_in_argb_out(uint32_t va_y_addr, uint32_t va_uv_addr,
    uint32_t input_frame_width, uint32_t input_frame_height,
    uint32_t output_frame_width, uint32_t output_frame_height);

#endif /* SUNXI_FRONT_END_H_ */
