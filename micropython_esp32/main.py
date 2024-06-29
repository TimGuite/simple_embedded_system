# Start Wokwi simulation and run:
# $ mpremote connect port:rfc2217://localhost:4000 cp driver_dht20.py :driver_dht20.py
# $ mpremote connect port:rfc2217://localhost:4000 run main.py

import utime
import sys
import machine

from machine import I2C
from driver_dht20 import DHT20

if __name__ == "__main__":
    print("Hello, Wokwi!")
    board = sys.implementation._machine.split(' with')[0]
    print(f"Running on {board} ({sys.platform}) at {machine.freq() / 1000000} MHz")

    sensor = DHT20(I2C(0))

    counter = 0
    while True:
        humidity, temperature = sensor.read()

        print(f"Temperature: {temperature:.2f} degC, Humidity: {humidity:.2f} %")

        counter += 1
        print(f"Uptime: {counter} seconds")
        utime.sleep_ms(1000)