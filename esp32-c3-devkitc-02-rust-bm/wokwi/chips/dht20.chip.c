// Wokwi Custom Chip - For information and examples see:
// https://link.wokwi.com/custom-chips-alpha

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
} chip_state_t;

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
    return true; /* Ack */
}

uint8_t on_i2c_read(void *user_data)
{
    printf("I2C Read\n");
    return 0;
}

bool on_i2c_write(void *user_data, uint8_t data)
{
    chip_state_t * chip_state = user_data;

    printf("I2C Write: %d\n", data);
    switch (chip_state->write_byte_count)
    {
        case 0:
        {
            if (data == 0xAC)
            {
                chip_state->write_byte_count = 1;
            }
        }
        case 1:
        {
            if (data == 0x33)
            {
                chip_state->write_byte_count = 2;
            } else {
                chip_state->write_byte_count = 0;
            }
        }
        case 2:
        {
            if (data == 0x00)
            {
                chip_state->measurement_triggered = 0;
                chip_state->temperature = attr_read_float(chip_state->temperature_attr);
                chip_state->humidity = attr_read_float(chip_state->humidity_attr);
                printf("Triggered measurement!\n");
                printf(
                    "Temperature: %.1f, Humidity: %.1f\n",
                    chip_state->temperature,
                    chip_state->humidity
                );
            }
            // Reset
            chip_state->write_byte_count = 0;
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
