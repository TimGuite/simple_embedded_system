# Start Wokwi simulation and run:
# $ mpremote connect port:rfc2217://localhost:4000 run main.py

import utime
import sys
import machine

from machine import I2C

print("Hello, Wokwi!")
board = sys.implementation._machine.split(' with')[0]
print(f"Running on {board} ({sys.platform}) at {machine.freq() / 1000000} MHz")

i2c = I2C(0)

counter = 0
while True:
    i2c.writeto(0x38, bytes([0xAC, 0x33, 0x00]))
    read_buffer = i2c.readfrom(0x38, 7)

    humidity = ((read_buffer[1] << 12) + (read_buffer[2] << 4) + (read_buffer[3] >> 4)) * 9.536743e-5
    temperature = ((((read_buffer[3] & 0x0F) << 16) + (read_buffer[4] << 8) + read_buffer[5]) * 1.9073486e-4) - 50

    print(f"Temperature: {temperature:.2f} degC, Humidity: {humidity:.2f} %")

    counter += 1
    print(f"Uptime: {counter} seconds")
    utime.sleep_ms(1000)