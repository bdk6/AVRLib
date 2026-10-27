# AVRLib
Library for AVR specific devices, generic devices, and miscellaneous -- Model for generic libs for other processors.


The complete library is in two parts.  There is a general purpose 
library for controlling AVR specific devices in a well defined way and a 
library for generic peripheral devices.  

The controlled AVR devices are:
ADC     -- Analog to Digital Converter.
GPIO    -- General Purpose Input / Output pins.
PWM     -- Pulse Width Modulation using AVR timer(s).
Serial  -- Serial input and output using the AVR UART.
SPI     -- Serial Peripheral Interface using AVR hardware SPI interface.
SoftSPI -- Serial Peripheral Interface using GPIO pins.
systick -- A system timer for periodic timer ticks.

The generic device library depends on the AVR library.
The controlled generic devices are:
button     -- Individual pushbutton switches.
DDS_9833   -- The Analog Devices AD9833 Direct Digital Synthesis IC.
encoder    -- Generic quadrature rotary encoder.
keypad     -- Matrix keypad of pushbutton switches.
lcd_44780 -- Standard character LCDs using HD44780 or equivalent.

The two libraries should be configured and built for your specific project 
using the two config files:
config.h          for the AVR library
device_config.h   for the generic library

config.h
Configure the following items:
F_CPU         The CPU operating frequency in Hertz (8 MHz is 8000000).
BUILD_DATE    Date of this build in YYYYMMDD format
VERSION_MAJOR  |
VERSION_MINOR   > Version number of the library -- do not change
VERSION_UPDATE |

ADC:
  

GPIO:
  
PWM:
  
Serial:
  SERIAL_RX_BUFFER_SIZE    The number of bytes to use for the receive buffer.
  SERIAL_TX_BUFFER_SIZE    The number of bytes for the transmit buffer.

SoftSPI:
  


