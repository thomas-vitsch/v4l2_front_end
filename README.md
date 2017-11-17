# v4l2_front_end
(WIP) This V4L2 device will not be developed any furter as this hardware block needs to be implemented in a drm/kms module.

Notes:
Some setup was done in u-boot, the clock for the front-end is setup here, so there is one missing config.
No IRQ
Important: see sunxi_front-end:601.
    The registers seem to be a double buffered set? The following code is currently in place
    -------------------------
    		/* TODO: First step: Change this into a loop waits for the self-clearing 
		* bit to clear before setting it again.
		* Better fix: Only write the update register once instead of twice and 
		* find out what register is incorrect in the double-buffered register set. 
		*/
		if (regmap_write(sunxi_fe_dev->regs, DEFE_FRM_CTRL_REG,
		    DEFE_REG_RDY_EN(ENABLE) | DEFE_COEF_RDY_EN(ENABLE) |
		    DEFE_FRM_START_BIT(ENABLE))) {
			printk("Could not start frontend.\n");
			return -1;   	
		}
		msleep(100);
		if (regmap_write(sunxi_fe_dev->regs, DEFE_FRM_CTRL_REG,
		    DEFE_REG_RDY_EN(ENABLE) | DEFE_COEF_RDY_EN(ENABLE) |
		    DEFE_FRM_START_BIT(ENABLE))) {
			printk("Could not start frontend.\n");
			return -1;
		}
    -------------------------
    
Hopes this helps anyone.
