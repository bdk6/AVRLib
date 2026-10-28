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

### config.h
Configure the following items:
* General Configuration
   * F_CPU         The CPU operating frequency in Hertz (8 MHz is 8000000).
   * BUILD_DATE    Date of this build in YYYYMMDD format
  * VERSION_MAJOR  |
  * VERSION_MINOR   > Version number of the library -- do not change
  * VERSION_UPDATE |

* ADC:  (Not yet implemented)
  
* GPIO:
  >GPIO doesn't require any configuration parameters.  Each pin is 
  > numbered consecitively by port.  Port A, bit 0 is pin 0.  Port 
  > C, bit 3 is pin 19.  There are enums defined as GPIO_PIN_B0, 
  > GPIO_PIN_D7, etc.  Each pin can have its mode set to one of the 
  > following:
   * GPIO_PIN_MODE_INPUT    Input without a pullup.
   * GPIO_PIN_MODE_OUTPUT   Output.
   * GPIO_PIN_MODE_INPUT_PULLUP  Input with a pullup.

   * void GPIO_pin_mode(const GPIO_pin_t pin, const GPIO_mode_t mode);
   * void GPIO_write_pin(const GPIO_pin_t pin, const int val);
   * void GPIO_toggle_pin(const GPIO_pin_t pin);
   * int GPIO_read_pin(const GPIO_pin_t pin);
   * int GPIO_read_output_pin(const GPIO_pin_t pin);
  
* PWM:  (Not yet implemented)
  
* Serial:
  * SERIAL_RX_BUFFER_SIZE    The number of bytes to use for the receive buffer.
  * SERIAL_TX_BUFFER_SIZE    The number of bytes for the transmit buffer.

* SPI:       (Not yet implemented)

* SoftSPI:   

* Systick:
  >Systick offers a real-time clock, counting ticks and milliseconds.  It uses 
  > the main CPU clock divided by a selected prescaler.  The timer used is 
  > 8 bit timer 0, so it rolls over and interrupts every 256 cycles (after 
  > prescaling.)
  > Systick also provides for timed callbacks.  Each callback can run a 
  > single time or repetitively after a set number of ticks or milliseconds.
   * SYSTICK_COUNT   Set to the number of callback timers desired.

  

### device_config.h

