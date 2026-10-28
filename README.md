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
  > GPIO_PIN_D7, etc.

  > GPIO pin names (GPIO_pin_t):
  * GPIO_PIN_B0         = 8,
  * GPIO_PIN_B1         = 9,
  * GPIO_PIN_B2         = 10,
  * GPIO_PIN_B3         = 11,
  * GPIO_PIN_B4         = 12,
  * GPIO_PIN_B5         = 13,
  * GPIO_PIN_B6         = 14,
  * GPIO_PIN_B7         = 15,
    
  * GPIO_PIN_C0         = 16,
  * GPIO_PIN_C1         = 17,
  * GPIO_PIN_C2         = 18,
  * GPIO_PIN_C3         = 19,
  * GPIO_PIN_C4         = 20,
  * GPIO_PIN_C5         = 21,
  * GPIO_PIN_C6         = 22,
    
  * GPIO_PIN_D0         = 24,
  * GPIO_PIN_D1         = 25,
  * GPIO_PIN_D2         = 26,
  * GPIO_PIN_D3         = 27,
  * GPIO_PIN_D4         = 28,
  * GPIO_PIN_D5         = 29,
  * GPIO_PIN_D6         = 30,
  * GPIO_PIN_D7         = 31,
    
  * GPIO_PIN_NONE       = 255


  > GPIO pin modes:
   * GPIO_PIN_MODE_INPUT    Input without a pullup.
   * GPIO_PIN_MODE_OUTPUT   Output.
   * GPIO_PIN_MODE_INPUT_PULLUP  Input with a pullup.

   > Functions for GPIO:
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
  > SoftSPI is a software implemented SPI interface using any available GPIO 
  > pins.  You can have any reasonable number of interfaces.  Each operates 
  > in any of the standard four SPI modes, with either LSB first or MSB first, 
  > either full speed or slow,  and can have a selectable number of bits per 
  > word 32.  They all use the same pins for CLK, MOSI, and MISO 
  > but have separate SS (slave select) pins.  If no input is needed, the 
  > MISO pin can be set to GPIO_PIN_NONE or if no output is needed the MOSI 
  > can be set to GPIO_PIN_NONE.  With a high-speed clock and without setting 
  > a lower rate, you can expect a transfer rate of around 1 Mbps.  If 
  > needed, a slower rate can be set and delay loops will execute between 
  > each bit for a slower transfer rate.  The rate is selectable per 
  > interface.

  > The CLK, MOSI, and MISO pins are set as:
  * #define SOFTSPI_CLK         GPIO_PIN_B5
  * #define SOFTSPI_MOSI        GPIO_PIN_B3
  * #define SOFTSPI_MISO        GPIO_PIN_NONE

  > softSPI modes:
  * SPI_MODE_0_MSB_FIRST              =  0, // idle low,  sample on leading
  * SPI_MODE_0_LSB_FIRST              =  1, // idle low,  sample on leading
  * SPI_MODE_1_MSB_FIRST              =  2, // idle low,  sample on trailing
  * SPI_MODE_1_LSB_FIRST              =  3, // idle low,  sample on trailing
  * SPI_MODE_2_MSB_FIRST              =  4, // idle high, sample on leading
  * SPI_MODE_2_LSB_FIRST              =  5, // idle high, sample on leading
  * SPI_MODE_3_MSB_FIRST              =  6, // idle high, sample on trailing
  * SPI_MODE_3_LSB_FIRST              =  7, // idle high, sample on trailing
  * SPI_MODE_0_MSB_FIRST_SLOW         =  8, // idle low,  sample on leading
  * SPI_MODE_0_LSB_FIRST_SLOW         =  9, // idle low,  sample on leading
  * SPI_MODE_1_MSB_FIRST_SLOW         = 10, // idle low,  sample on trailing
  * SPI_MODE_1_LSB_FIRST_SLOW         = 11, // idle low,  sample on trailing
  * SPI_MODE_2_MSB_FIRST_SLOW         = 12, // idle high, sample on leading
  * SPI_MODE_2_LSB_FIRST_SLOW         = 13, // idle high, sample on leading
  * SPI_MODE_3_MSB_FIRST_SLOW         = 14, // idle high, sample on trailing
  * SPI_MODE_3_LSB_FIRST_SLOW         = 15, // idle high, sample on trailing

  > The number of interfaces is set as:
  > // bps       ss_pin            mode                bits
  > // u32       gpio pin          mode_t              u8
  > #define SOFTSPI_INTERFACES   \
  >    {0,    GPIO_PIN_C5, 0,  SPI_MODE_2_MSB_FIRST,   16},  \
  >    {0,    GPIO_PIN_C4, 0,  SPI_MODE_2_MSB_FIRST,    8}


  > The functions available for the softSPI interfaces:

  * int SOFTSPI_init(GPIO_pin_t clk, GPIO_pin_t mosi, GPIO_pin_t miso);
  * int SOFTSPI_init2(void);
  * int SOFTSPI_set_interface(uint8_t idx, GPIO_pin_t ss, uint8_t bits, softspi_mode_t mode,  uint32_t bps);
  * uint32_t SOFTSPI_write(uint8_t idx, uint32_t data);


* Systick:
  >Systick offers a real-time clock, counting ticks and milliseconds.  It uses 
  > the main CPU clock divided by a selected prescaler.  The timer used is 
  > 8 bit timer 0, so it rolls over and interrupts every 256 cycles (after 
  > prescaling.)
  > Systick also provides for timed callbacks.  Each callback can run a 
  > single time or repetitively after a set number of ticks or milliseconds.
   * SYSTICK_COUNT   Set to the number of callback timers desired.

  

### device_config.h

