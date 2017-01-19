/*
 * LCD panel driver for innolux AT070TN83
 *
 * Copyright (C) 2008 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fb.h>
#include <linux/err.h>
#include <linux/slab.h>

#include <plat/display.h>

struct innolux_data {
	struct backlight_device *bl;
};

static struct omap_video_timings innolux_ls_timings = {
	.x_res = 800,
	.y_res = 480,

	.pixel_clock	= 33300,

	.hsw		= 48,
	.hfp		= 1,
	.hbp		= 80,

	.vsw		= 3,
	.vfp		= 13,
	.vbp		= 29,
};

static int innolux_ls_panel_probe(struct omap_dss_device *dssdev)
{
	dssdev->panel.config = OMAP_DSS_LCD_TFT | OMAP_DSS_LCD_IVS |
		OMAP_DSS_LCD_IHS;
	dssdev->panel.timings = innolux_ls_timings;

	return 0;
}

static void innolux_ls_panel_remove(struct omap_dss_device *dssdev)
{

}

static int innolux_ls_power_on(struct omap_dss_device *dssdev)
{
	int r = 0;

	if (dssdev->state == OMAP_DSS_DISPLAY_ACTIVE)
		return 0;

	r = omapdss_dpi_display_enable(dssdev);
	if (r)
		goto err0;

	/* wait couple of vsyncs until enabling the LCD */
	msleep(50);

	if (dssdev->platform_enable) {
		r = dssdev->platform_enable(dssdev);
		if (r)
			goto err1;
	}

	return 0;
err1:
	omapdss_dpi_display_disable(dssdev);
err0:
	return r;
}

static void innolux_ls_power_off(struct omap_dss_device *dssdev)
{
	if (dssdev->state != OMAP_DSS_DISPLAY_ACTIVE)
		return;

	if (dssdev->platform_disable)
		dssdev->platform_disable(dssdev);

	/* wait at least 5 vsyncs after disabling the LCD */

	msleep(100);

	omapdss_dpi_display_disable(dssdev);
}

static int innolux_ls_panel_enable(struct omap_dss_device *dssdev)
{
	int r;
	r = innolux_ls_power_on(dssdev);
	dssdev->state = OMAP_DSS_DISPLAY_ACTIVE;
	return r;
}

static void innolux_ls_panel_disable(struct omap_dss_device *dssdev)
{
	innolux_ls_power_off(dssdev);
	dssdev->state = OMAP_DSS_DISPLAY_DISABLED;
}

static int innolux_ls_panel_suspend(struct omap_dss_device *dssdev)
{
	innolux_ls_power_off(dssdev);
	dssdev->state = OMAP_DSS_DISPLAY_SUSPENDED;
	return 0;
}

static int innolux_ls_panel_resume(struct omap_dss_device *dssdev)
{
	int r;
	r = innolux_ls_power_on(dssdev);
	dssdev->state = OMAP_DSS_DISPLAY_ACTIVE;
	return r;
}

static struct omap_dss_driver innolux_ls_driver = {
	.probe		= innolux_ls_panel_probe,
	.remove		= innolux_ls_panel_remove,

	.enable		= innolux_ls_panel_enable,
	.disable	= innolux_ls_panel_disable,
	.suspend	= innolux_ls_panel_suspend,
	.resume		= innolux_ls_panel_resume,

	.driver         = {
		.name   = "at070tn83",
		.owner  = THIS_MODULE,
	},
};

static int __init innolux_ls_panel_drv_init(void)
{
	return omap_dss_register_driver(&innolux_ls_driver);
}

static void __exit innolux_ls_panel_drv_exit(void)
{
	omap_dss_unregister_driver(&innolux_ls_driver);
}

module_init(innolux_ls_panel_drv_init);
module_exit(innolux_ls_panel_drv_exit);
MODULE_LICENSE("GPL");
