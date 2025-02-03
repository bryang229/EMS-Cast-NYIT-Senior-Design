# Arduino Nano Pin Mapping

| Pin  | Digital? | Interrupt | PWM | Special Function          | Project Mapping |
|------|----------|-----------|-----|---------------------------|-----------------|
| D0 | Yes    | No      | No | RX (Serial) | Unused |
| D1 | Yes    | No      | No | TX (Serial) | Unused |
| D2 | Yes    | Yes      | No | External Interrupt 0 | ROT_A |
| D3 | Yes    | Yes      | Yes | PWM / External Interrupt 1 | ROT_B |
| D4 | Yes    | No      | No |  | ROT_SW |
| D5 | Yes    | No      | Yes | PWM | PW_SW |
| D6 | Yes    | No      | Yes | PWM | SW_LED |
| D7 | Yes    | No      | No |  | EN_CH1_POS |
| D8 | Yes    | No      | No |  | EN_CH1_NEG |
| D9 | Yes    | No      | Yes | PWM | EN_CH2_POS |
| D10 | Yes    | No      | Yes | PWM / SPI (SS) | EN_CH2_NEG |
| D11 | Yes    | No      | Yes | PWM / SPI (MOSI) | Unused |
| D12 | Yes    | No      | No | SPI (MISO) | Unused |
| D13 | Yes    | No      | No | SPI (SCK) / Built-in LED | Unused |
| A0 | Yes    | No      | No | Analog Input | Unused |
| A1 | Yes    | No      | No | Analog Input | Unused |
| A2 | Yes    | No      | No | Analog Input | Unused |
| A3 | Yes    | No      | No | Analog Input | Unused |
| A4 | Yes    | No      | No | Analog Input / I2C (SDA) | LCD / POT / RTC (DATA) |
| A5 | Yes    | No      | No | Analog Input / I2C (SCL) | LCD / POT / RTC (CLK) |
| A6 | No    | No      | No | Analog Input Only | Unused |
| A7 | No    | No      | No | Analog Input Only | Unused |


## Dependencies: (Arduino Libraries Needed)
- LiquidCrystal_I2C
- RTCLib
- RotaryEncoder