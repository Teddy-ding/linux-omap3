/*
 * Code for TL3730 EVM.
 *
 * Copyright (C) 2016 GuangZhou Tronlong, Inc. - http://www.tronlong.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/leds.h>

#include <linux/err.h>

#include "mux.h"

/* Convert GPIO signal to GPIO pin number */
#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))


#define TI3730_USER_LED1		GPIO_TO_PIN(6, 1)
#define TI3730_USER_LED2		GPIO_TO_PIN(6, 2)
#define TI3730_USER_LED3		GPIO_TO_PIN(6, 13)


/* assign the TI3730 evm board LED-GPIOs*/

static struct gpio_led ti3730_evm_leds[] = {
	{
		.active_low = 0,
		.gpio = TI3730_USER_LED1,
		.name = "user_led1",
		.default_trigger = "default-on",
	},
	{
		.active_low = 0,
		.gpio = TI3730_USER_LED2,
		.name = "user_led2",
		.default_trigger = "default-on",
	},
	{
		.active_low = 0,
		.gpio = TI3730_USER_LED3,
		.name = "user_led3",
		.default_trigger = "default-on",
	},
};

static struct gpio_led_platform_data ti3730_evm_leds_pdata = {
	.leds = ti3730_evm_leds,
	.num_leds = ARRAY_SIZE(ti3730_evm_leds),
};

static void user_leds_release(struct device *dev){
}

static struct platform_device ti3730_evm_leds_device = {
	.name		= "leds-gpio",
	.id		= 2,
	.dev = {
		.platform_data = &ti3730_evm_leds_pdata,
		.release = &user_leds_release,
	}
};

static int __init ti3730_evm_user_leds_init(void)
{
	int ret;

	ret = platform_device_register(&ti3730_evm_leds_device);
	if (ret) {
		pr_warning("Could not register som GPIO expander LEDS");
	}

	return ret;
}

static void __exit ti3730_evm_user_leds_exit(void)
{
    platform_device_unregister(&ti3730_evm_leds_device);
}

module_init(ti3730_evm_user_leds_init);
module_exit(ti3730_evm_user_leds_exit);

MODULE_AUTHOR("lufeicjw<lufeicjw@163.com>");
MODULE_DESCRIPTION("TL3730 user leds module init driver");
MODULE_LICENSE("GPL v2");
