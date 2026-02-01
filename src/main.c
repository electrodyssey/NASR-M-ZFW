/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include <zephyr/shell/shell.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include <zephyr/drivers/uart.h>
#include <ctype.h>

#ifdef CONFIG_ARCH_POSIX
#include <unistd.h>
#else
#include <zephyr/posix/unistd.h>
#endif

//#include <gpio.h>

/* LED configuration */
#define LED0_NODE DT_ALIAS(led0)
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#define ATXON_NODE DT_ALIAS(atxon)
#if DT_NODE_HAS_STATUS(ATXON_NODE, okay)
static const struct gpio_dt_spec atxpwon = GPIO_DT_SPEC_GET(ATXON_NODE, gpios);
#else
#error "Unsupported board: atxon devicetree alias is not defined"
#endif

#define INVALIDB_NODE DT_ALIAS(uartinvalid)
#if DT_NODE_HAS_STATUS(INVALIDB_NODE, okay)
static const struct gpio_dt_spec uart_invalid_b = GPIO_DT_SPEC_GET(INVALIDB_NODE, gpios);
#else
#error "Unsupported board: uartinvalid devicetree alias is not defined"
#endif

#define ATXPBTN_NODE DT_ALIAS(atxpbtn)
#if DT_NODE_HAS_STATUS(ATXPBTN_NODE, okay)
static const struct gpio_dt_spec atxpbtn = GPIO_DT_SPEC_GET(ATXPBTN_NODE, gpios);
#else
#error "Unsupported board: atxpbtn devicetree alias is not defined"
#endif

#define ATXOK_NODE DT_ALIAS(atxok)
#if DT_NODE_HAS_STATUS(ATXOK_NODE, okay)
static const struct gpio_dt_spec atxpwok = GPIO_DT_SPEC_GET(ATXOK_NODE, gpios);
#else
#error "Unsupported board: atxok devicetree alias is not defined"
#endif



char cmd_version[512] = "ver. test 0.1";

static int cmd_demo_ping(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "pong");

	return 0;
}

static int cmd_demo_params(const struct shell *sh, size_t argc,
                           char **argv)
{
        int cnt;

        shell_print(sh, "argc = %d", argc);
        for (cnt = 0; cnt < argc; cnt++) {
                shell_print(sh, "  argv[%d] = %s", cnt, argv[cnt]);
        }
        return 0;
}

/* Creating subcommands (level 1 command) array for command "demo". */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo,
        SHELL_CMD(params, NULL, "Print params command.",
                                               cmd_demo_params),
        SHELL_CMD(ping,   NULL, "Ping command.", cmd_demo_ping),
        SHELL_SUBCMD_SET_END
);
/* Creating root (level 0) command "demo" without a handler */
SHELL_CMD_REGISTER(demo, &sub_demo, "Demo commands", NULL);

/* Creating root (level 0) command "version" */
SHELL_CMD_REGISTER(version, NULL, "Show kernel version", cmd_version);


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


	gpio_pin_configure_dt(&atxpwon, GPIO_OUTPUT_ACTIVE);

	gpio_pin_configure_dt(&atxpwok, GPIO_INPUT);

	gpio_pin_configure_dt(&atxpbtn, GPIO_INPUT);
	
	gpio_pin_configure_dt(&uart_invalid_b, GPIO_INPUT);	
	

	

	gpio_pin_set_dt(&atxpwon, 1);
	//	gpio_pin_set_dt(&led, 1);

	/* Blink loop */


	while (1) {

	  //gpio_dt_spec stt;
	  int stt;

	  stt = gpio_pin_get_dt(&atxpwok);
	  if (stt)
	    {
		  gpio_pin_set_dt(&led, stt);

	      /*
	      ret = gpio_pin_toggle_dt(&led);
		
	      if (ret < 0) {
		printk("ERROR: Failed to toggle LED (error %d)\n", ret);
		return -1;
	      }
		
	      led_state = !led_state;
	      printk("LED: %s\n", led_state ? "ON " : "OFF");
	      */
		
	      //k_sleep(K_MSEC(1000));
	    }

	}

		       
		       
			


	return 0;
}
