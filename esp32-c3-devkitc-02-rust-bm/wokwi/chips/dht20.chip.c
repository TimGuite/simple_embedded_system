// Wokwi Custom Chip - For information and examples see:
// https://link.wokwi.com/custom-chips-alpha

// DHT20 Datasheet: https://cdn.sparkfun.com/assets/8/a/1/5/0/DHT20.pdf

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int ADDRESS = 0x38;

typedef struct
{
    uint8_t write_byte_count;
    uint8_t read_byte_count;
    bool measurement_triggered;
    float temperature;
    float humidity;
    uint32_t temperature_attr;
    uint32_t humidity_attr;
    uint8_t read_data[7];
} chip_state_t;

// Helper function borrowed from Zephyr
uint8_t crc8(
    const uint8_t *src,
    size_t len,
    uint8_t polynomial,
    uint8_t initial_value,
    bool reversed
)
{
	uint8_t crc = initial_value;
	size_t i, j;

	for (i = 0; i < len; i++) {
		crc ^= src[i];

		for (j = 0; j < 8; j++) {
			if (reversed) {
				if (crc & 0x01) {
					crc = (crc >> 1) ^ polynomial;
				} else {
					crc >>= 1;
				}
			} else {
				if (crc & 0x80) {
					crc = (crc << 1) ^ polynomial;
				} else {
					crc <<= 1;
				}
			}
		}
	}

	return crc;
}

// Helper function to populate the bytes for a measurement read
void calculate_data_bytes(chip_state_t * chip_state)
{
    chip_state->read_data[0] = 0x1C;

    uint32_t humidity_signal = (uint32_t)((chip_state->humidity / 100.0) * 1048576.0);
    uint32_t temperature_signal = (uint32_t)(((chip_state->temperature + 50) / 200.0) * 1048576.0);

    chip_state->read_data[1] = (uint8_t)((humidity_signal >> 12) & 0xFF);
    chip_state->read_data[2] = (uint8_t)((humidity_signal >> 4 & 0xFF));
    chip_state->read_data[3] = (uint8_t)((humidity_signal & 0x0F) + ((temperature_signal >> 16) & 0xFF));
    chip_state->read_data[4] = (uint8_t)((temperature_signal >> 8) & 0xFF);
    chip_state->read_data[5] = (uint8_t)(temperature_signal & 0xFF);

    const uint8_t crc_value = crc8(chip_state->read_data, 6, 0x31, 0xFF, false);
    chip_state->read_data[6] = crc_value;
}

static bool on_i2c_connect(void *user_data, uint32_t address, bool connect);
static uint8_t on_i2c_read(void *user_data);
static bool on_i2c_write(void *user_data, uint8_t data);
static void on_i2c_disconnect(void *user_data);

void chip_init()
{
    chip_state_t *chip = malloc(sizeof(chip_state_t));

    const i2c_config_t i2c_config = {
        .user_data = chip,
        .address = ADDRESS,
        .scl = pin_init("SCL", INPUT),
        .sda = pin_init("SDA", INPUT),
        .connect = on_i2c_connect,
        .read = on_i2c_read,
        .write = on_i2c_write,
        .disconnect = on_i2c_disconnect, // Optional
    };
    i2c_init(&i2c_config);

    chip->temperature_attr = attr_init_float("temperature", 25);
    chip->humidity_attr = attr_init_float("humidity", 50);

    // The following message will appear in the browser's DevTools console:
    printf("Hello from custom chip!\n");
}

bool on_i2c_connect(void *user_data, uint32_t address, bool connect)
{
    printf("I2C Connect\n");

    chip_state_t *chip_state = user_data;

    // Reset
    chip_state->write_byte_count = 0;
    chip_state->read_byte_count = 0;
    return true; /* Ack */
}

uint8_t on_i2c_read(void *user_data)
{
    // printf("I2C Read\n");

    chip_state_t *chip_state = user_data;

    // If there is no measurement triggered then return 0xFF indicating the measurement
    // is not ready - 7.4.3 of datasheet
    if (false == chip_state->measurement_triggered)
    {
        return 0xFF;
    }

    for (int i = 0; i < 7; i++)
    {
        printf("%02X ", chip_state->read_data[i]);
    }
    printf("\n");

    uint8_t output_byte = 0x00;

    if (chip_state->read_byte_count < 7)
    {
        printf("%02X\n", chip_state->read_data[chip_state->read_byte_count]);
        output_byte = chip_state->read_data[chip_state->read_byte_count];
    }

    // Trigger a new measurement
    if (chip_state->read_byte_count >= 6)
    {
        chip_state->measurement_triggered = false;
    }

    chip_state->read_byte_count++;

    return output_byte;
}

bool on_i2c_write(void *user_data, uint8_t data)
{
    chip_state_t *chip_state = user_data;

    printf("I2C Write: %d\n", data);
    switch (chip_state->write_byte_count)
    {
    case 0:
    {
        if (data == 0xAC)
        {
            chip_state->write_byte_count = 1;
        }
        break;
    }
    case 1:
    {
        if (data == 0x33)
        {
            chip_state->write_byte_count = 2;
        }
        else
        {
            chip_state->write_byte_count = 0;
        }
        break;
    }
    case 2:
    {
        if (data == 0x00)
        {
            chip_state->measurement_triggered = true;
            chip_state->temperature = attr_read_float(chip_state->temperature_attr);
            chip_state->humidity = attr_read_float(chip_state->humidity_attr);
            calculate_data_bytes(chip_state);
            printf("Triggered measurement!\n");
            printf(
                "Temperature: %.1f, Humidity: %.1f\n",
                chip_state->temperature,
                chip_state->humidity);
        }
        break;
    }
    default:
        chip_state->write_byte_count = 0;
    }
    return true; // Ack
}

void on_i2c_disconnect(void *user_data)
{
    printf("I2C Disconnect\n");
}
