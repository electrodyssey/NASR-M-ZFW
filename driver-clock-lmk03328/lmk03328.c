/*
 * Copyright (c) 2024 NASR
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Driver for Texas Instruments LMK03328 I2C clock generator.
 *
 * Reads register values from device tree and writes them to the device
 * during initialization. Each reg-values entry is a 16-bit encoded word:
 *   bits[15:8] = register address
 *   bits[7:0]  = register value
 */

#define DT_DRV_COMPAT ti_lmk03328

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lmk03328, CONFIG_LMK03328_LOG_LEVEL);

/* R12 (address 0x0C): bit 7 = 0 triggers software reset */
#define LMK03328_REG_RESET     0x0C
#define LMK03328_BIT_RESET     BIT(7)

struct lmk03328_config {
	struct i2c_dt_spec i2c;
	const uint32_t *regs;
	size_t num_regs;
	bool software_reset;
};

static int lmk03328_program(const struct device *dev)
{
	const struct lmk03328_config *config = dev->config;
	int ret;

	if (!i2c_is_ready_dt(&config->i2c)) {
		LOG_ERR("I2C bus not ready");
		return -ENODEV;
	}

	LOG_DBG("Writing %zu registers to LMK03328 at I2C addr 0x%02x",
		config->num_regs, config->i2c.addr);

	for (size_t i = 0; i < config->num_regs; i++) {
		uint8_t reg_addr = (config->regs[i] >> 8) & 0xFF;
		uint8_t reg_val  = config->regs[i] & 0xFF;

		ret = i2c_reg_write_byte_dt(&config->i2c, reg_addr, reg_val);
		if (ret < 0) {
			LOG_ERR("Failed to write reg 0x%02x=0x%02x: %d",
				reg_addr, reg_val, ret);
			return ret;
		}
	}

	if (config->software_reset) {
		ret = i2c_reg_update_byte_dt(&config->i2c, LMK03328_REG_RESET,
					     LMK03328_BIT_RESET, 0);
		if (ret < 0) {
			LOG_ERR("Software reset failed: %d", ret);
			return ret;
		}
		LOG_DBG("Software reset issued");
	}

	return 0;
}

int lmk03328_reinit(const struct device *dev)
{
	int ret;

	if (!device_is_ready(dev)) {
		return -ENODEV;
	}

	LOG_INF("Re-initializing LMK03328");

	ret = lmk03328_program(dev);
	if (ret < 0) {
		LOG_ERR("Re-initialization failed: %d", ret);
		return ret;
	}

	LOG_INF("LMK03328 re-initialized successfully");

	return 0;
}

static int lmk03328_init(const struct device *dev)
{
	/*
	int ret = lmk03328_program(dev);

	if (ret == 0) {
		const struct lmk03328_config *config = dev->config;

		LOG_INF("LMK03328 initialized (%zu registers written)", config->num_regs);
	}

	return ret;
	*/

	//don't do anything

	return 0;
}

#define LMK03328_DEFINE(inst)							\
	static const uint32_t lmk03328_regs_##inst[] =				\
		DT_INST_PROP(inst, reg_values);					\
										\
	static const struct lmk03328_config lmk03328_config_##inst = {		\
		.i2c            = I2C_DT_SPEC_INST_GET(inst),			\
		.regs           = lmk03328_regs_##inst,				\
		.num_regs       = DT_INST_PROP_LEN(inst, reg_values),		\
		.software_reset = DT_INST_PROP(inst, software_reset),		\
	};									\
										\
	DEVICE_DT_INST_DEFINE(inst, lmk03328_init, NULL,			\
			      NULL, &lmk03328_config_##inst,			\
			      POST_KERNEL,					\
			      DT_INST_PROP_OR(inst, init_priority,		\
					      CONFIG_LMK03328_INIT_PRIORITY),	\
			      NULL);

DT_INST_FOREACH_STATUS_OKAY(LMK03328_DEFINE)
