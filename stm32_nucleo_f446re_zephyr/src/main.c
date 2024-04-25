/*
 * Copyright (c) 2018 Peter Bigot Consulting, LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <stdio.h>

int main(void)
{
	printf("Main function...\n");

	const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));

	if (i2c_dev == NULL || !device_is_ready(i2c_dev))
	{
		printf("Could not get I2C device\n");
		return 0;
	}

	printf("Trigger Measurement\n");
	uint8_t trigger_bytes[3] = { 0xAC, 0x33, 0x00 };
	i2c_write(i2c_dev, trigger_bytes, 3, 0x38);

	printf("Reading values\n");
	uint8_t rcv_bytes[7] = { 0 };
	i2c_read(i2c_dev, rcv_bytes, 7, 0x38);

	printf("Received: %02X\n", rcv_bytes[0]);
	if ((rcv_bytes[0] & 0x18) == 0x18)
	{
		printf("Status good\n");
	}

	return 0;
}
