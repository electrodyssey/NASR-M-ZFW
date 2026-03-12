/*
 * Copyright (c) 2024 NASR
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Public API for the TI LMK03328 clock generator driver.
 */

#ifndef LMK03328_H_
#define LMK03328_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Re-initialize the LMK03328 by replaying all device-tree register
 *        values to the chip over I2C.
 *
 * Useful after a power cycle or hardware reset of the LMK03328 to restore
 * the programmed clock configuration without rebooting the MCU.
 *
 * @param dev  Pointer to the LMK03328 device obtained via DEVICE_DT_GET().
 *
 * @retval 0         Success.
 * @retval -ENODEV   Device not ready or I2C bus unavailable.
 * @retval <0        I2C transfer error code.
 */
int lmk03328_reinit(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif /* LMK03328_H_ */
