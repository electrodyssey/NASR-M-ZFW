# NASR-M-ZFW

NASR-M base controller Zephyr firmware.
The board repository is located here: [NASR-M](https://github.com/electrodyssey/NASR-M), more info on my home page: [NASR-M SYZYGY carrier board](https://electrodyssey.net/nasr-m-syzygy-carrier-board.html)


Work in progress..


Things that are already functional:

+ Zephyr shell with RS-232 access, custom commands, and I2C discovery 

+ Control of the primary ATX PSU

+ Control of the sencondary on-board PSU

+ TCA6416A GPIO driver that controls LMK03328 bootstrap pins

+ LMK03328 PLL ciock driver

+ Onboard thermometer (TMP102) readings

+ Power sensor (INA219) readings 



Things to do:

+ Onboard LAN8742A ethernet

+ Administration web page

+ IPMI extensions
