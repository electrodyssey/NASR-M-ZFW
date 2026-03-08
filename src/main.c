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
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/__assert.h>

#include <ctype.h>

#ifdef CONFIG_ARCH_POSIX
#include <unistd.h>
#else
#include <zephyr/posix/unistd.h>
#endif

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

#define BOOTBTN_NODE DT_ALIAS(bootbtn)
#if DT_NODE_HAS_STATUS(BOOTBTN_NODE, okay)
static const struct gpio_dt_spec boot0btn = GPIO_DT_SPEC_GET(BOOTBTN_NODE, gpios);
#else
#error "Unsupported board: bootbtn devicetree alias is not defined"
#endif


#define SW_I2C_RST_NODE DT_ALIAS(swi2crst)
#if DT_NODE_HAS_STATUS(SW_I2C_RST_NODE, okay)
static const struct gpio_dt_spec sw_i2c_rst = GPIO_DT_SPEC_GET(SW_I2C_RST_NODE, gpios);
#else
#error "Unsupported board: swi2crst devicetree alias is not defined"
#endif


#define CLK_I2C_RST_NODE DT_ALIAS(clki2crst)
#if DT_NODE_HAS_STATUS(CLK_I2C_RST_NODE, okay)
static const struct gpio_dt_spec clk_i2c_rst = GPIO_DT_SPEC_GET(CLK_I2C_RST_NODE, gpios);
#else
#error "Unsupported board: clki2crst devicetree alias is not defined"
#endif


#define TEMP1_NODE DT_ALIAS(tmps1)
#if DT_NODE_HAS_STATUS(TEMP1_NODE, okay)
const struct device *const tdev1 = DEVICE_DT_GET(TEMP1_NODE);
#else
#error "Unsupported board: thermometer1 devicetree alias is not defined"
#endif


#define TEMP2_NODE DT_ALIAS(tmps2)
#if DT_NODE_HAS_STATUS(TEMP2_NODE, okay)
const struct device *const tdev2 = DEVICE_DT_GET(TEMP2_NODE);
#else
#error "Unsupported board: thermometer2 devicetree alias is not defined"
#endif


#define ZYNQ_PWCTL_EN_NODE DT_ALIAS(zynqpwctlen)
#if DT_NODE_HAS_STATUS(ZYNQ_PWCTL_EN_NODE, okay)
static const struct gpio_dt_spec zynqpwen = GPIO_DT_SPEC_GET(ZYNQ_PWCTL_EN_NODE, gpios);
#else
#error "Unsupported board: zynqpwctlen devicetree alias is not defined"
#endif

#define CCTL_LED_NODE DT_ALIAS(cctlled)
#if DT_NODE_HAS_STATUS(CCTL_LED_NODE, okay)
static const struct gpio_dt_spec clock_control_led = GPIO_DT_SPEC_GET(CCTL_LED_NODE, gpios);
//static const struct gpio_dt_spec clock_control_led = GPIO_DT_SPEC_GET(DT_NODELABEL(clkctlled), gpios);
#else
#error "Unsupported board: cctlled devicetree alias is not defined"
#endif

#define CCTL_PDN_NODE DT_ALIAS(cctlpdn)
#if DT_NODE_HAS_STATUS(CCTL_PDN_NODE, okay)
static const struct gpio_dt_spec cctlpdn = GPIO_DT_SPEC_GET(CCTL_PDN_NODE, gpios);
#else
#error "Unsupported board: cctlpdn devicetree alias is not defined"
#endif


#define CCTL_HWSW_NODE DT_ALIAS(clkctlhsw)
#if DT_NODE_HAS_STATUS(CCTL_HWSW_NODE, okay)
static const struct gpio_dt_spec cctlhsw = GPIO_DT_SPEC_GET(CCTL_HWSW_NODE, gpios);
#else
#error "Unsupported board: cctlhsw devicetree alias is not defined"
#endif

#define CCTL_REFSEL_NODE DT_ALIAS(cctlrsel)
#if DT_NODE_HAS_STATUS(CCTL_REFSEL_NODE, okay)
static const struct gpio_dt_spec cctlrsel = GPIO_DT_SPEC_GET(CCTL_REFSEL_NODE, gpios);
#else
#error "Unsupported board: cctlrsel devicetree alias is not defined"
#endif

#define CCTL0_NODE DT_ALIAS(clkio0)
#if DT_NODE_HAS_STATUS(CCTL0_NODE, okay)
static const struct gpio_dt_spec cctl0 = GPIO_DT_SPEC_GET(CCTL0_NODE, gpios);
#else
#error "Unsupported board: clkio0 devicetree alias is not defined"
#endif

#define CCTL1_NODE DT_ALIAS(clkio1)
#if DT_NODE_HAS_STATUS(CCTL1_NODE, okay)
static const struct gpio_dt_spec cctl1 = GPIO_DT_SPEC_GET(CCTL1_NODE, gpios);
#else
#error "Unsupported board: clkio1 devicetree alias is not defined"
#endif

#define CCTL2_NODE DT_ALIAS(clkio2)
#if DT_NODE_HAS_STATUS(CCTL2_NODE, okay)
static const struct gpio_dt_spec cctl2 = GPIO_DT_SPEC_GET(CCTL2_NODE, gpios);
#else
#error "Unsupported board: clkio2 devicetree alias is not defined"
#endif

#define CCTL3_NODE DT_ALIAS(clkio3)
#if DT_NODE_HAS_STATUS(CCTL3_NODE, okay)
static const struct gpio_dt_spec cctl3 = GPIO_DT_SPEC_GET(CCTL3_NODE, gpios);
#else
#error "Unsupported board: clkio3 devicetree alias is not defined"
#endif

#define CCTL4_NODE DT_ALIAS(clkio4)
#if DT_NODE_HAS_STATUS(CCTL4_NODE, okay)
static const struct gpio_dt_spec cctl4 = GPIO_DT_SPEC_GET(CCTL4_NODE, gpios);
#else
#error "Unsupported board: clkio4 devicetree alias is not defined"
#endif

#define CCTL5_NODE DT_ALIAS(clkio5)
#if DT_NODE_HAS_STATUS(CCTL5_NODE, okay)
static const struct gpio_dt_spec cctl5 = GPIO_DT_SPEC_GET(CCTL5_NODE, gpios);
#else
#error "Unsupported board: clkio5 devicetree alias is not defined"
#endif


char version[512] = "ver. test 0.1\0";

int32_t ambient_temp1 = 0;
int32_t ambient_temp2 = 0;

int32_t boot_stt = 0;

static void cmd_temp(const struct device *dev, uint8_t temp_no)
{

  
        int ret;
        struct sensor_value temp_value;
        struct sensor_value attr;

        attr.val1 = 150;
        attr.val2 = 0;
        ret = sensor_attr_set(dev, SENSOR_CHAN_AMBIENT_TEMP,
                              SENSOR_ATTR_FULL_SCALE, &attr);
        if (ret) {
                printk("sensor_attr_set failed ret %d\n", ret);
                return;
        }

        attr.val1 = 8;
        attr.val2 = 0;
        ret = sensor_attr_set(dev, SENSOR_CHAN_AMBIENT_TEMP,
                              SENSOR_ATTR_SAMPLING_FREQUENCY, &attr);
        if (ret) {
                printk("sensor_attr_set failed ret %d\n", ret);
                return;
        }

	//        while (1) {
                ret = sensor_sample_fetch(dev);
                if (ret) {
                        printk("sensor_sample_fetch failed ret %d\n", ret);
                        return;
                }

                ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_value);
                if (ret) {
                        printk("sensor_channel_get failed ret %d\n", ret);
                        return;
                }

                printk("thermometer %u; temp is %d (%d micro)\n", temp_no, temp_value.val1, temp_value.val2);

		if (1 == temp_no)
		  ambient_temp1 = temp_value.val1;
		else
		  ambient_temp2 = temp_value.val1;

		//k_sleep(K_MSEC(1000));
		//}
}


static int cmd_pwr_atx(const struct shell *sh, size_t argc, char **argv)
{
  if (2 != argc)
    {
      shell_print(sh, "command accepts a single parameter, either 'on' or 'off'");

      return 0;
    }

  
  if (0 == strncmp(argv[1], "on", 2))
    {
	shell_print(sh, "Turning ATX ON");
	gpio_pin_set_dt(&atxpwon, 1);
	
    } else if (0 == strncmp(argv[1], "off", 3))
    {
	shell_print(sh, "Turning ATX OFF");
	gpio_pin_set_dt(&atxpwon, 0);
    }
     else
    {
      shell_print(sh, "valid parameters are: 'on', 'off'");
    }

  return 0;
}

static int cmd_pwr_soc(const struct shell *sh, size_t argc, char **argv)
{
  if (2 != argc)
    {
      shell_print(sh, "command accepts a single parameter, either 'on' or 'off'");

      return 0;
    }

  
  if (0 == strncmp(argv[1], "on", 2))
    {
	shell_print(sh, "Turning SOC power ON");
	gpio_pin_set_dt(&zynqpwen, 1);
	
    } else if (0 == strncmp(argv[1], "off", 3))
    {
	shell_print(sh, "Turning SOC power OFF");
	gpio_pin_set_dt(&zynqpwen, 0);
    }
     else
    {
      shell_print(sh, "valid parameters are: 'on', 'off'");
    }

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

static int cmd_version (const struct shell *sh, size_t argc,
                           char **argv)
{
        shell_print(sh, "NASR-M MCU firmware version: %s\r\n", version);

        return 0;
}


/* Creating subcommands (level 1 command) array for command "power". */

SHELL_STATIC_SUBCMD_SET_CREATE(sub_power,
        SHELL_CMD(params, NULL, "Print params command.",
                                               cmd_demo_params),
        SHELL_CMD(atx,   NULL, "ATX power control.", cmd_pwr_atx),
        SHELL_CMD(soc,   NULL, "SOC power control.", cmd_pwr_soc),
        SHELL_SUBCMD_SET_END
);

/* Creating root (level 0) command "demo" without a handler */
SHELL_CMD_REGISTER(power, &sub_power, "Power control commands", NULL);

/* Creating root (level 0) command "version" */
SHELL_CMD_REGISTER(version, NULL, "Show NASR-M base controller firmware version", cmd_version);

static int cmd_reboot(const struct shell* shell, size_t argc, char** argv)
{
    printf("Cold booting the base controller...\r\n");
    NVIC_SystemReset();
    return 0;
}

SHELL_CMD_REGISTER(reboot, NULL, "Base controller reboot", cmd_reboot);

void init_clock(bool state)
{

  gpio_pin_set_dt(&cctl0, 0);
  gpio_pin_set_dt(&cctl1, 0);
  gpio_pin_set_dt(&cctl2, 0);
  gpio_pin_set_dt(&cctl3, 0);
  gpio_pin_set_dt(&cctl4, 0);
  gpio_pin_set_dt(&cctl5, 0);

  if (false == state)
    {
       	gpio_pin_set_dt(&cctlpdn, 0); //clock is off
	gpio_pin_set_dt(&cctlhsw, 0); //hwsw soft mode
	gpio_pin_set_dt(&cctlrsel, 0); //refsel =1

  	gpio_pin_set_dt(&cctlpdn, 0); //

    }
  else
    {
  
        printf("starting up PLL\r\n");
  	gpio_pin_set_dt(&cctlpdn, 0); //clk is off
	gpio_pin_set_dt(&cctlhsw, 1); //hwsw soft mode
	gpio_pin_set_dt(&cctlrsel, 1); //refsel =1

  	gpio_pin_set_dt(&cctlpdn, 1); //clk is on

        gpio_pin_set_dt(&clock_control_led, 1);
    }
}

/* returns 1 for on; 0 for off, -1 if error*/
int char_to_bool (char* c)
{
  int res = -1;
  
  if (0 == strncmp(c, "on", 2))
    {
      res = 1;
    } else if (0 == strncmp(c, "off", 3))
    {
      res = 0;
    }

  return res;
}

static int cmd_clock(const struct shell *sh, size_t argc, char **argv)
{

  if (3 != argc && 11 != argc)
    {
      goto help_screen;
    }

  if (3 == argc)
    {
      int stt = char_to_bool(argv[2]);

      if (-1 == stt)
	goto help_screen;

      shell_print(sh, "new stt = %d", stt);
      
      if (0 == strncmp(argv[1], "pwr", 3))
	{
 	  gpio_pin_set_dt(&cctlpdn, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "led", 3))
	{
	  gpio_pin_set_dt(&clock_control_led, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "hw", 2))
	{
	  gpio_pin_set_dt(&cctlhsw, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "ref", 3))
	{
	  gpio_pin_set_dt(&cctlrsel, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "io0", 3))
	{
	  gpio_pin_set_dt(&cctl0, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "io1", 3))
	{
	  gpio_pin_set_dt(&cctl1, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "io2", 3))
	{
	  gpio_pin_set_dt(&cctl2, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "io3", 3))
	{
	  gpio_pin_set_dt(&cctl3, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "io4", 3))
	{
	  gpio_pin_set_dt(&cctl4, stt);
	  return 0;
	}
      else if (0 == strncmp(argv[1], "io5", 3))
	{
	  gpio_pin_set_dt(&cctl5, stt);
	  return 0;
	}
      else goto help_screen;
    }
  else if (11 == argc)
    {
      int pwr = char_to_bool(argv[1]);
      int hw =  char_to_bool(argv[2]);
      int ref = char_to_bool(argv[3]);
      int led = char_to_bool(argv[4]);

      int io0 = char_to_bool(argv[5]);
      int io1 = char_to_bool(argv[6]);
      int io2 = char_to_bool(argv[7]);
      int io3 = char_to_bool(argv[8]);
      int io4 = char_to_bool(argv[9]);
      int io5 = char_to_bool(argv[10]);


      gpio_pin_set_dt(&cctl0, io0);
      gpio_pin_set_dt(&cctl1, io1);
      gpio_pin_set_dt(&cctl2, io2);
      gpio_pin_set_dt(&cctl3, io3);
      gpio_pin_set_dt(&cctl4, io4);
      gpio_pin_set_dt(&cctl5, io5);

      
      gpio_pin_set_dt(&cctlhsw, hw);
      gpio_pin_set_dt(&cctlrsel, ref); 
      gpio_pin_set_dt(&clock_control_led, led);
      
      gpio_pin_set_dt(&cctlpdn, pwr);
    }


  return 0;

 help_screen:

      shell_print(sh, "clk pwr on|off");

      shell_print(sh, "clk led on|off");
      shell_print(sh, "clk hw  on|off");

      shell_print(sh, "clk io0 on|off");
      shell_print(sh, "clk io1 on|off");
      shell_print(sh, "clk io2 on|off");
      shell_print(sh, "clk io3 on|off");
      shell_print(sh, "clk io4 on|off");
      shell_print(sh, "clk io5 on|off");

      shell_print(sh, "clk pwr hw ref led io0 io1 io2 io3 io4 io5");

  return 0;
}


/* Creating root (level 0) command "clock" */
SHELL_CMD_REGISTER(clk, NULL, "Clock control commands", cmd_clock);


static int cmd_info(const struct shell* shell, size_t argc, char** argv)
{

  cmd_temp(tdev1, 1);
  printf("ambient temperature1: %d\r\n", ambient_temp1);

  //cmd_temp(tdev2, 2);
  //printf("ambient temperature2: %d\r\n", ambient_temp2);

  //  boot_stt = gpio_pin_get_dt(&boot0btn);
  //  printf("boot btn: %d\r\n", boot_stt);
  //  gpio_pin_set_dt(&clock_control_led, 1);
  //  gpio_pin_set_dt(&cctlpdn, 0);

  init_clock(true);
	
  return 0;
}

SHELL_CMD_REGISTER(info, NULL, "system info", cmd_info);






int main(void)
{
	int ret;
	bool led_state = true;

	printk("\n\n=== NASR-M Base Controller start ===\n");
	//printk("Testing LED on GPIO Port %c, Pin %d\n", 
	//	       'A' + (led.port->name[4] - 'A'), led.pin);

	/* Check if LED GPIO is ready */
	//if (!gpio_is_ready_dt(&led)) {
	//	printk("ERROR: LED GPIO device %s is not ready\n", led.port->name);
	//	return -1;
	//}
	//printk("LED GPIO device is ready\n");

	/* Configure LED pin as output */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("ERROR: Failed to configure LED GPIO (error %d)\n", ret);
		return -1;
	}
	//printk("LED GPIO configured successfully\n");

	/* Turn LED ON initially */
	ret = gpio_pin_set_dt(&led, 1);
	if (ret < 0) {
		printk("ERROR: Failed to set LED (error %d)\n", ret);
		return -1;
	}
	//	printk("LED turned ON\n");
	//	printk("\nStarting blink loop...\n\n");


	gpio_pin_configure_dt(&atxpwon, GPIO_OUTPUT_ACTIVE);

	gpio_pin_configure_dt(&sw_i2c_rst, GPIO_OUTPUT_ACTIVE);

	//	gpio_pin_configure_dt(&clk_i2c_rst, GPIO_OUTPUT_ACTIVE);

	gpio_pin_configure_dt(&clock_control_led, GPIO_OUTPUT_INACTIVE);

	gpio_pin_configure_dt(&cctlpdn, GPIO_OUTPUT_INACTIVE);

	gpio_pin_configure_dt(&cctlrsel, GPIO_OUTPUT_INACTIVE);

	gpio_pin_configure_dt(&cctl0, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&cctl1, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&cctl2, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&cctl3, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&cctl4, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&cctl5, GPIO_OUTPUT_INACTIVE);


	gpio_pin_configure_dt(&zynqpwen, GPIO_OUTPUT_ACTIVE);
	

	gpio_pin_configure_dt(&atxpwok, GPIO_INPUT);

	gpio_pin_configure_dt(&atxpbtn, GPIO_INPUT);

	gpio_pin_configure_dt(&boot0btn, GPIO_INPUT);
	
	gpio_pin_configure_dt(&uart_invalid_b, GPIO_INPUT);
	

	gpio_pin_set_dt(&atxpwon, 1);




	gpio_pin_set_dt(&sw_i2c_rst, 1);

	
	//gpio_pin_set_dt(&clk_i2c_rst, 1);


	//	gpio_pin_set_dt(&clk_i2c_rst, 1);

	//gpio_pin_set_dt(&clock_control_led, 1);
	//gpio_pin_set_dt(&cctlpdn, 1);

	gpio_pin_set_dt(&zynqpwen, 0);


	
	gpio_pin_configure_dt(&cctlpdn, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&cctlhsw, GPIO_OUTPUT_INACTIVE); //hwsw =0
	gpio_pin_configure_dt(&cctlrsel, GPIO_OUTPUT_ACTIVE); //refsel =1


	init_clock(false);

	//	const struct device *const tdev = DEVICE_DT_GET_ANY(ti_tmp112);
	//	const struct device *const tdev = DEVICE_DT_GET(tmp112@48);
	
        __ASSERT(tdev1 != NULL, "Failed to get device binding thermometer1");
        __ASSERT(device_is_ready(tdev1), "Device %s is not ready", tdev1->name);

	printk("device is %p, name is %s\n", tdev1, tdev1->name);
	//	cmd_temp(tdev1);



        __ASSERT(tdev2 != NULL, "Failed to get device binding thermometer2");
        __ASSERT(device_is_ready(tdev2), "Device %s is not ready", tdev2->name);

	printk("device is %p, name is %s\n", tdev2, tdev2->name);
	//cmd_temp(tdev2);

	
	/* Blink loop */


	const struct device *dev;
	uint32_t dtr = 0;


	dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	if (!device_is_ready(dev)) {
	  gpio_pin_set_dt(&led, 0);
	  return 0;
	}
	
	while (!dtr) {

	  //gpio_dt_spec stt;
	  //int stt;

	  
	  
	  //gpio_pin_set_dt(&led, stt);

	 
	  uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
	  k_sleep(K_MSEC(10));


	    
	}


	return 0;
}
