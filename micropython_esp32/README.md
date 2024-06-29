# Setup

1. Install requirements: `pip install -r requirements.txt`
2. Follow the instructions here: https://micropython.org/download/ESP32_GENERIC_C3/
- `esptool --chip esp32c3 --port {MY_PORT} erase_flash`
- `esptool --chip esp32c3 --port {MY_PORT} --baud 460800 write_flash -z 0x0 ESP32_GENERIC_C3-20240602-v1.23.0.bin`

# Program the board

```bash
$ mpremote connect port:COM5 run main.py
$ mpremote connect port:COM5 run main.py
```

# Testing

```bash
$ pytest
=== test session starts ===
platform win32 -- Python 3.11.9, pytest-8.2.2, pluggy-1.5.0
rootdir: C:\Users\Tim\Github\simple_embedded_system\micropython_esp32
collected 2 items

test_dht20_driver.py .. [100%] 

=== 2 passed in 0.07s ===
```

# Simulation

Launch Wokwi simulator in VSCode then program the board from a new terminal:

```bash
$ mpremote connect port:rfc2217://localhost:4000 cp driver_dht20.py :driver_dht20.py
$ mpremote connect port:rfc2217://localhost:4000 run main.py
```