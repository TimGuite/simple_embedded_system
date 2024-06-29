# Start Wokwi simulation and run:
# $ mpremote connect port:rfc2217://localhost:4000 run main.py

import utime
import sys
import machine

print("Hello, Wokwi!")
board = sys.implementation._machine.split(' with')[0]
print(f"Running on {board} ({sys.platform}) at {machine.freq() / 1000000} MHz")

counter = 0
while True:
    counter += 1
    print(f"Uptime: {counter} seconds")
    utime.sleep_ms(1000)