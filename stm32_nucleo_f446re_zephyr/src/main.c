/*
 * Copyright (c) 2018 Peter Bigot Consulting, LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

int main(void)
{
	const struct device *const dev = DEVICE_DT_GET_ONE(aosong_dht20);
	int rc;

	if (!device_is_ready(dev)) {
		printf("Device %s is not ready\n", dev->name);
		return 0;
	}

	while (true) {
		struct sensor_value temp, hum;

		rc = sensor_sample_fetch(dev);
		if (rc == 0) {
			rc = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP,
						&temp);
		}
		if (rc == 0) {
			rc = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY,
						&hum);
		}
		if (rc != 0) {
			printf("DHT20: failed: %d\n", rc);
			break;
		}

		printf("DHT20: %.2f Cel ; %0.2f %%RH\n",
		       sensor_value_to_double(&temp),
		       sensor_value_to_double(&hum));

		k_sleep(K_MSEC(2000));
	}
	return 0;
}
