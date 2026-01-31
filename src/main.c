/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* LED configuration */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

int main(void)
{
	int ret;
	bool led_state = true;

	printk("\n\n=== NASR-M LED Test ===\n");
	printk("Testing LED on GPIO Port %c, Pin %d\n", 
	       'A' + (led.port->name[4] - 'A'), led.pin);

	/* Check if LED GPIO is ready */
	if (!gpio_is_ready_dt(&led)) {
		printk("ERROR: LED GPIO device %s is not ready\n", led.port->name);
		return -1;
	}
	printk("LED GPIO device is ready\n");

	/* Configure LED pin as output */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("ERROR: Failed to configure LED GPIO (error %d)\n", ret);
		return -1;
	}
	printk("LED GPIO configured successfully\n");

	/* Turn LED ON initially */
	ret = gpio_pin_set_dt(&led, 1);
	if (ret < 0) {
		printk("ERROR: Failed to set LED (error %d)\n", ret);
		return -1;
	}
	printk("LED turned ON\n");
	printk("\nStarting blink loop...\n\n");

	/* Blink loop */


	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			printk("ERROR: Failed to toggle LED (error %d)\n", ret);
			return -1;
		}
		
		led_state = !led_state;
		printk("LED: %s\n", led_state ? "ON " : "OFF");
		
		k_sleep(K_MSEC(500));  
	}


	return 0;
}
