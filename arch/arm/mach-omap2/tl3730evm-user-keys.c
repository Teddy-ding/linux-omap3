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
#include <linux/gpio_keys.h>
#include <linux/input.h>

#include <linux/err.h>

#include "mux.h"

/* Convert GPIO signal to GPIO pin number */
#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))

#define TI3730_USER_KEY0               GPIO_TO_PIN(0, 149)
#define TI3730_USER_KEY1               GPIO_TO_PIN(0, 150)

#define TI3730_KEYS_DEBOUNCE_MS        10
/*
 * At 200ms polling interval it is possible to miss an
 * event by tapping very lightly on the push button but most
 * pushes do result in an event; longer intervals require the
 * user to hold the button whereas shorter intervals require
 * more CPU time for polling.
 */
#define TI3730_GPIO_KEYS_POLL_MS       200

static struct gpio_keys_button gpio_buttons[] = {
       {
               .type                   = EV_KEY,
               .active_low             = 1,
               .wakeup                 = 0,
               .debounce_interval      = TI3730_KEYS_DEBOUNCE_MS,
               .code                   = KEY_PROG1,
               .desc                   = "user_key0",
               .gpio                   = TI3730_USER_KEY0,
       },
       {
               .type                   = EV_KEY,
               .active_low             = 1,
               .wakeup                 = 0,
               .debounce_interval      = TI3730_KEYS_DEBOUNCE_MS,
               .code                   = KEY_PROG2,
               .desc                   = "user_key1",
               .gpio                   = TI3730_USER_KEY1,
       },
};

static struct gpio_keys_platform_data gpio_key_info = {
	.buttons	= gpio_buttons,
	.nbuttons	= ARRAY_SIZE(gpio_buttons),
};

static void user_keys_release(struct device *dev){
}

static struct platform_device keys_gpio = {
	.name	= "gpio-keys",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_key_info,
		.release = &user_keys_release,
	},
};

static int __init ti3730_evm_user_keys_init(void)
{
	int ret;

	ret = platform_device_register(&keys_gpio);
	if (ret) {
		pr_warning("Could not register som GPIO expander KEYS");
	}

	return ret;
}

static void __exit ti3730_evm_user_keys_exit(void)
{
    platform_device_unregister(&keys_gpio);
}

module_init(ti3730_evm_user_keys_init);
module_exit(ti3730_evm_user_keys_exit);

MODULE_AUTHOR("lufeicjw<lufeicjw@163.com>");
MODULE_DESCRIPTION("TL3730 user keys module init driver");
MODULE_LICENSE("GPL v2");
