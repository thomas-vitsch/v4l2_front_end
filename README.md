# v4l2_front_end
(WIP) This V4L2 device will not be developed any furter as this hardware block needs to be implemented in a drm/kms module.

Notes:
Some setup was done in u-boot, the clock for the front-end is setup here, so there is one missing config.

We have a u-boot which had some code we could enable that also setup the
front-end, this part had not been implemented in the kernel part yet.

So if you are also using u-boot, take a look at the following file. If you don't
then here is the info you need.

Code: u-boot/drivers/video/sunxi_display.c
=======================================
#define CONFIG_MACH_SUN4I //Enables using the front end.

static const u32 sun4i_vert_coef[32] = {
        0x00004000, 0x000140ff, 0x00033ffe, 0x00043ffd,
        0x00063efc, 0xff083dfc, 0x000a3bfb, 0xff0d39fb,
        0xff0f37fb, 0xff1136fa, 0xfe1433fb, 0xfe1631fb,
        0xfd192ffb, 0xfd1c2cfb, 0xfd1f29fb, 0xfc2127fc,
        0xfc2424fc, 0xfc2721fc, 0xfb291ffd, 0xfb2c1cfd,
        0xfb2f19fd, 0xfb3116fe, 0xfb3314fe, 0xfa3611ff,
        0xfb370fff, 0xfb390dff, 0xfb3b0a00, 0xfc3d08ff,
        0xfc3e0600, 0xfd3f0400, 0xfe3f0300, 0xff400100,
};

static const u32 sun4i_horz_coef[64] = {
        0x40000000, 0x00000000, 0x40fe0000, 0x0000ff03,
        0x3ffd0000, 0x0000ff05, 0x3ffc0000, 0x0000ff06,
        0x3efb0000, 0x0000ff08, 0x3dfb0000, 0x0000ff09,
        0x3bfa0000, 0x0000fe0d, 0x39fa0000, 0x0000fe0f,
        0x38fa0000, 0x0000fe10, 0x36fa0000, 0x0000fe12,
        0x33fa0000, 0x0000fd16, 0x31fa0000, 0x0000fd18,
        0x2ffa0000, 0x0000fd1a, 0x2cfa0000, 0x0000fc1e,
        0x29fa0000, 0x0000fc21, 0x27fb0000, 0x0000fb23,
        0x24fb0000, 0x0000fb26, 0x21fb0000, 0x0000fb29,
        0x1ffc0000, 0x0000fa2b, 0x1cfc0000, 0x0000fa2e,
        0x19fd0000, 0x0000fa30, 0x16fd0000, 0x0000fa33,
        0x14fd0000, 0x0000fa35, 0x11fe0000, 0x0000fa37,
        0x0ffe0000, 0x0000fa39, 0x0dfe0000, 0x0000fa3b,
        0x0afe0000, 0x0000fa3e, 0x08ff0000, 0x0000fb3e,
        0x06ff0000, 0x0000fb40, 0x05ff0000, 0x0000fc40,
        0x03ff0000, 0x0000fd41, 0x01ff0000, 0x0000fe42,
};

static void sunxi_frontend_init(void)
{
        struct sunxi_ccm_reg * const ccm =
                (struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
        struct sunxi_de_fe_reg * const de_fe =
                (struct sunxi_de_fe_reg *)SUNXI_DE_FE0_BASE;
        int i;

        /* Clocks on */
        setbits_le32(&ccm->ahb_gate1, 1 << AHB_GATE_OFFSET_DE_FE0);
        setbits_le32(&ccm->dram_clk_gate, 1 << CCM_DRAM_GATE_OFFSET_DE_FE0);
        clock_set_de_mod_clock(&ccm->fe0_clk_cfg, 300000000);

        debug("DAAN: fe0_clk_cfg=%08x\n", readl(&ccm->fe0_clk_cfg));

        setbits_le32(&de_fe->enable, SUNXI_DE_FE_ENABLE_EN);

        for (i = 0; i < 32; i++) {
                writel(sun4i_horz_coef[2 * i], &de_fe->ch0_horzcoef0[i]);
                writel(sun4i_horz_coef[2 * i + 1], &de_fe->ch0_horzcoef1[i]);
                writel(sun4i_vert_coef[i], &de_fe->ch0_vertcoef[i]);
                writel(sun4i_horz_coef[2 * i], &de_fe->ch1_horzcoef0[i]);
                writel(sun4i_horz_coef[2 * i + 1], &de_fe->ch1_horzcoef1[i]);
                writel(sun4i_vert_coef[i], &de_fe->ch1_vertcoef[i]);
        }

        setbits_le32(&de_fe->frame_ctrl, SUNXI_DE_FE_FRAME_CTRL_COEF_RDY);
}

No IRQ
Important: see sunxi_front-end:601.

The manually added IOCTL are stale. These were added as a starting point for
using the Allwinner A20 Display Engine front end.
So ignore the msleep(100);

Hopes this helps anyone.
