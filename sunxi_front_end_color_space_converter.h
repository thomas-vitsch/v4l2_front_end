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

#ifndef SUNXI_FRONT_END_COLOR_SPACE_CONVERTER_H_
#define SUNXI_FRONT_END_COLOR_SPACE_CONVERTER_H_

#include "sunxi_front_end.h"

#define NR_CSC_COLORS			3
#define NR_CSC_COLOR_COEF		4
#define CSC_COLOR_CONST_COEF_POS	3
#define COEF_OFFSET			4

struct sunxi_fe_device;

int setup_csc(struct sunxi_fe_device *sunxi_fe_dev);

#endif /* SUNXI_FRONT_END_COLOR_SPACE_CONVERTER_H_ */

