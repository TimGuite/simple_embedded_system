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

	printf("Writing check value\n");
	uint8_t check_bytes[1] = { 0x71 };
	i2c_write(i2c_dev, check_bytes, 1, 0x38);

	printf("Reading check value\n");
	uint8_t rcv_bytes[1] = { 0 };
	i2c_read(i2c_dev, rcv_bytes, 1, 0x38);

	printf("Received: %02X\n", rcv_bytes[0]);

	// const struct device *const dev = DEVICE_DT_GET_ONE(aosong_dht20);
	// int rc;

	// if (!device_is_ready(dev)) {
	// 	printf("Device %s is not ready\n", dev->name);
	// 	return 0;
	// }

	// printf("Device set up...\n");

	// while (true) {
	// 	struct sensor_value temp, hum;

	// 	printf("Fetching sample...\n");

	// 	rc = sensor_sample_fetch(dev);

	// 	printf("Sample fetched...\n");

	// 	if (rc == 0) {
	// 		rc = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP,
	// 					&temp);
	// 	}
	// 	if (rc == 0) {
	// 		rc = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY,
	// 					&hum);
	// 	}
	// 	if (rc != 0) {
	// 		printf("DHT20: failed: %d\n", rc);
	// 		break;
	// 	}

	// 	printf("DHT20: %.2f Cel ; %0.2f %%RH\n",
	// 	       sensor_value_to_double(&temp),
	// 	       sensor_value_to_double(&hum));

	// 	k_sleep(K_MSEC(2000));
	// }
	return 0;
}
