class DHT20:
    def __init__(self, i2c_bus):
        self.i2c_bus = i2c_bus
    
    def read(self):
        self.i2c_bus.writeto(0x38, bytes([0xAC, 0x33, 0x00]))
        read_buffer = self.i2c_bus.readfrom(0x38, 7)

        humidity = ((read_buffer[1] << 12) + (read_buffer[2] << 4) + (read_buffer[3] >> 4)) * 9.536743e-5
        temperature = ((((read_buffer[3] & 0x0F) << 16) + (read_buffer[4] << 8) + read_buffer[5]) * 1.9073486e-4) - 50  

        return (humidity, temperature)