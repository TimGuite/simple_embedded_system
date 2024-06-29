from pytest import approx
from unittest.mock import Mock

from driver_dht20 import DHT20

def test_dht20_driver_calls_i2c_write_then_i2c_read_with_correct_address_and_values():
    mock_i2c = Mock()
    mock_i2c.readfrom.return_value = [0] * 7

    d = DHT20(mock_i2c)
    d.read()
    
    mock_i2c.writeto.assert_called_once_with(0x38, bytes([0xAC, 0x33, 0x00]))
    mock_i2c.readfrom.assert_called_once_with(0x38, 7)

def test_dht20_driver_converts_correctly():
    mock_i2c = Mock()
    mock_i2c.readfrom.return_value = b'\x1ck\r\xf5\xf1.r'

    d = DHT20(mock_i2c)
    (humidity, temperature) = d.read()

    assert humidity == approx(41.81814)
    assert temperature == approx(24.27635)