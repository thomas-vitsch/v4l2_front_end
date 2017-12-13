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

#ifndef SUNXI_FRONT_END_REGISTERS_H_
#define SUNXI_FRONT_END_REGISTERS_H_

#define FE_BASE				0x01e80000

/* The registers and their offsets are based on the info from:
 * A20 User Manual : "A20_0user_manual_v1.3_2014_10_10.pdf"
 * (Revision 1.3)
 * Chapter: Chapter / page: "5.3. Front End"
 * Page 477 DEFE Register List
 */

/* The following registers can be offset by n * DMA_CHANNEL_OFFSET where
 * n is the input/output dma channel available.
 * regs: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 */
#define IDMA0				0 /* Input dma channel 0 */
#define IDMA1				1 /* Input dma channel 1 */
#define IDMA2				2 /* Input dma channel 2 */
#define ODMA0				0 /* Output dma channel 0 */
#define ODMA1				1 /* Output dma channel 1 */
#define ODMA2				2 /* Output dma channel 2 */

/* All bits that can enable/disable things in the Front End are
 * enabled by setting the bit to 1 and are disabled by setting the bit
 * to 0 */
#define ENABLE 				1
#define DISABLE 			0

// MASK_BITS x = value, mask = bits_to_mask, pos = pos in register
#define MASK_BITS(x, mask, pos)		(((x) & (mask)) << (pos))
#define MASK_BIT(x, pos)		MASK_BITS(x, 0x1, pos)

/*** Front End registers ****/
/* DEFE Module Enable Register */
#define DEFE_EN_REG			0x0
#define DEFE_EN_BIT(x)			MASK_BIT(x, 0)
#define DEFE_EN_MASK			BIT(0)

/* DEFE Frame Process Control Register */
#define DEFE_FRM_CTRL_REG		0x4
#define DEFE_FRM_START_BIT(x)		MASK_BIT(x, 16)
#define DEFE_COEF_RDY_EN(x)		MASK_BIT(x, 1)
#define DEFE_REG_RDY_EN(x)		MASK_BIT(x, 0)
#define DEFE_REG_RDY_MASK		BIT(0)
#define DEFE_FRM_START_START_MASK	BIT(1)

/* DEFE CSC By-Pass Register */
#define DEFE_BYPASS_REG			0x8
#define DEFE_CSC_BYPASS_EN(x)		MASK_BIT(x, 1)

/* DEFE Input Channel 0 Buffer Address Register */
#define DEFE_BUF_ADDR0_REG		0x20

/* DEFE Input Channel 1 Buffer Address Register */
#define DEFE_BUF_ADDR1_REG		0x24

/* DEFE Channel 0 Tile-Based Offset Register */
#define DEFE_TB_OFF0_REG		0x30
#define DEFE_TB_OFF1_REG		0x34
#define DEFE_TB_X_OFFSET1(x)		MASK_BITS(x, 0x1f, 16)
#define DEFE_TB_Y_OFFSET0(x)		MASK_BITS(x, 0x1f, 8)
#define DEFE_TB_X_OFFSET0(x)		MASK_BITS(x, 0x1f, 0)
#define DEFE_TB_OFFSETS(x1, y0, x0)	(DEFE_TB_X_OFFSET1(x1) | \
    DEFE_TB_X_OFFSET0(x0) | DEFE_TB_Y_OFFSET0(y0))

/* DEFE Channel 1 Tile-Based Offset Register */
#define DEFE_TB_OFF1_REG		0x34

/* DEFE Channel 0 Line Stride Register */
#define DEFE_LINESTRD0_REG		0x40
#define DEFE_TILED_LINESTRIDE(width, tile_length)	((tile_length * width) \
    - (tile_length * tile_length) + tile_length)

/* DEFE Channel 1 Line Stride Register */
#define DEFE_LINESTRD1_REG		0x44

/* DEFE Input Format Register */
#define DEFE_INPUT_FMT_REG		0x4C
#define DEFE_INPUT_SCAN_MOD(x)		MASK_BIT(x, 12)
#define DEFE_INP_SCAN_INTERLACE 	0
#define DEFE_INP_SCAN_PROGRESSIVE	1
#define DEFE_INPUT_DATA_MOD(x) 		MASK_BITS(x, 0x7, 8)
#define DEFE_MOD_TILE_BASED_UV_COMBINED 0x6
#define DEFE_MOD_NON_TILE_BASED_PLANAR 	0x0
#define DEFE_INPUT_DATA_FMT(x)		MASK_BITS(x, 0x7, 4)
#define DEFE_INP_FMT_YUV444		0x0
#define DEFE_INP_FMT_YUV422		0x1
#define DEFE_INP_FMT_YUV420		0x2
#define DEFE_INP_FMT_YUV411		0x3
#define DEFE_INP_FMT_CSI_RGB_DATA	0x4
#define DEFE_INP_FMT_RGB888		0x5
#define DEFE_INPUT_PS(x) 		MASK_BITS(x, 0x3, 0)
#define DEFE_INP_PS_V1U1V0U0		0x0
#define DEFE_INP_PS_U1V1U0V0		0x1
#define DEFE_INP_PS_ARGB		0x1

/* DEFE Output Format Register */
#define DEFE_OUTPUT_FMT_REG		0x5C
#define DEFE_OUTPUT_DATA_FMT(x)		MASK_BITS(x, 0x3, 0)
#define DEFE_OUT_FMT_INTERL_ARGB8888	0x02/*A = padded 0xff*/

/* DEFE Channel 0 Input Size Register */
#define DEFE_CH0_INSIZE_REG		0x100
#define DEFE_CHX_IN_WIDTH_Y(x)		MASK_BITS(x, 0x1fff, 0)
#define DEFE_CHX_IN_HEIGHT_Y(x)		MASK_BITS(x, 0x1fff, 16)
#define DEFE_CHX_IN_WIDTH_UV(x)		DEFE_CHX_IN_WIDTH_Y(((x) / 2))
#define DEFE_CHX_IN_HEIGHT_UV(x)	DEFE_CHX_IN_HEIGHT_Y(((x) / 2))

/* DEFE Channel 0 Output Size Register */
#define DEFE_CH0_OUTSIZE_REG		0x104
#define DEFE_CHX_OUT_WIDTH(x)		DEFE_CHX_IN_WIDTH_Y(x)
#define DEFE_CHX_OUT_WIDTH_TILED(x)	(DEFE_CHX_IN_WIDTH_Y(x) + 1)
#define DEFE_CHX_OUT_HEIGHT(x)		DEFE_CHX_IN_HEIGHT_Y(x)

/* DEFE Channel 0 Horizontal Factor Register */
#define DEFE_CH0_HORZFACT_REG		0x108
#define DEFE_CHX_HORZFACT_INT(x)	MASK_BITS(x, 0xff, 16)
#define DEFE_CHX_HORZFACT_FRACT(x)	MASK_BITS(x, 0xffff, 0)

/* DEFE Channel 0 Vertical factor Register */
#define DEFE_CH0_VERTFACT_REG		0x10C
#define DEFE_CHX_VERTFACT_INT		DEFE_CHX_HORZFACT_INT
#define DEFE_CHX_VERTFACT_FRACT		DEFE_CHX_HORZFACT_FRACT

#define DEFE_CH1_INSIZE_REG		0x200
#define DEFE_CH1_OUTSIZE_REG		0x204

/*
 * The allwinner documents are unclear about the following.
 * For combining a UV input with a Y input we have to use the
 * horizontal and vertical scaler.
 * These registers accept a int part and a fract part.
 * Inserting 1.0 results in no scaling as output.
 * Inserting 0.5 results in 2x the input size as output.
 * Inserting 2.0 results in 0.5 the input size as output.
 * The linestride for the input channels need to be as their actual
 * sizes in bytes.
 *
 * The UV buffer contains 1 UV pair (2 bytes) per 2 horizontal Y
 * pixels. The UV buffer still needs to be enlarged by a factor 2
 * as the linestride size is equal to the amount of UV pairs per line
 * instead of the amount of bytes in a horizontal line.
 */
#define DEFE_CH1_HORZFACT_REG		0x208
#define DEFE_CH1_VERTFACT_REG		0x20C

/*
 * These are the names of the coefficient registers as defined in the Allwinner
 * documentation. Note: The order of these registers isn't logically numbered.
 */
#define DEFE_CSC_COEF00_REG		0x70
#define DEFE_CSC_COEF01_REG		0x74
#define DEFE_CSC_COEF02_REG		0x78
#define DEFE_CSC_COEF03_REG		0x7C
#define DEFE_CSC_COEF10_REG		0x80
#define DEFE_CSC_COEF11_REG		0x84
#define DEFE_CSC_COEF12_REG		0x88
#define DEFE_CSC_COEF13_REG		0x8C
#define DEFE_CSC_COEF20_REG		0x90
#define DEFE_CSC_COEF21_REG		0x94
#define DEFE_CSC_COEF22_REG		0x98
#define DEFE_CSC_COEF23_REG		0x9C

/*
 * Map Allwinner register names to logically named defines. Each color has 4
 * coefficients: 3 magnitudes and 1 constant offset.
 */
#define DEFE_CSC_COEF_Y_IN_R_REG	DEFE_CSC_COEF10_REG
#define DEFE_CSC_COEF_U_IN_R_REG	DEFE_CSC_COEF12_REG
#define DEFE_CSC_COEF_V_IN_R_REG	DEFE_CSC_COEF11_REG
#define DEFE_CSC_COEF_CONST_R_REG	DEFE_CSC_COEF13_REG
#define DEFE_CSC_COEF_Y_IN_G_REG	DEFE_CSC_COEF00_REG
#define DEFE_CSC_COEF_U_IN_G_REG	DEFE_CSC_COEF01_REG
#define DEFE_CSC_COEF_V_IN_G_REG	DEFE_CSC_COEF02_REG
#define DEFE_CSC_COEF_CONST_G_REG	DEFE_CSC_COEF03_REG
#define DEFE_CSC_COEF_Y_IN_B_REG	DEFE_CSC_COEF20_REG
#define DEFE_CSC_COEF_U_IN_B_REG	DEFE_CSC_COEF22_REG
#define DEFE_CSC_COEF_V_IN_B_REG	DEFE_CSC_COEF21_REG
#define DEFE_CSC_COEF_CONST_B_REG	DEFE_CSC_COEF23_REG

/*
 * The CSC_COEF values have their sign bits at bit 12.
 * Therefore we need to set this bit if we got a negative value.
 * Same for the CSC_CONST_COEF except that this sign bit is at bit 13.
 */
#define DEFE_CSC_COEF_MAG(x)		((x) < 0 ? ((x) + 0x2000) : (x))
#define DEFE_CSC_COEF_CONST(x)		((x) < 0 ? ((x) + 0x4000) : (x))

#endif /* SUNXI_FRONT_END_REGISTERS_H */

