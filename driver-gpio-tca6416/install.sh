#!/bin/bash

echo cp ti,tca6416a.yaml ../../zephyr/dts/bindings/gpio/
echo cp gpio_tca6416a.c ../../zephyr/drivers/gpio/
echo cp Kconfig.tca6416a ../../zephyr/drivers/gpio/

echo "add following to zephyrproject/zephyr/drivers/gpio/CMakeLists.txt right after the tca6424 string:"
echo "zephyr_library_sources_ifdef(CONFIG_GPIO_TCA6416A   gpio_tca6416a.c)"
