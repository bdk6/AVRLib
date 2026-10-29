# AVRLib
Library for AVR specific devices, generic devices, and miscellaneous -- Model for generic libs for other processors.


The complete library is in two parts.  There is a general purpose 
library for controlling AVR specific devices in a well defined way and a 
library for generic peripheral devices.  

The controlled AVR devices are:
* ADC     -- Analog to Digital Converter.
* GPIO    -- General Purpose Input / Output pins.
* PWM     -- Pulse Width Modulation using AVR timer(s).
* Serial  -- Serial input and output using the AVR UART.
* SPI     -- Serial Peripheral Interface using AVR hardware SPI interface.
* SoftSPI -- Serial Peripheral Interface using GPIO pins.
* systick -- A system timer for periodic timer ticks.

The generic device library depends on the AVR library.
The controlled generic devices are:
* button     -- Individual pushbutton switches.
* DDS_9833   -- The Analog Devices AD9833 Direct Digital Synthesis IC.
* encoder    -- Generic quadrature rotary encoder.
* keypad     -- Matrix keypad of pushbutton switches.
* lcd_44780 -- Standard character LCDs using HD44780 or equivalent.

The two libraries should be configured and built for your specific project 
using the two config files:
* config.h          for the AVR library
* device_config.h   for the generic library

## Using the Libraries
The short story on how to use the libraries is this:
1. Configure the needed devices in _config.h_ and _device_config.h_
2. Run **make** with **make** for the AVRlib library or **make -f Makefile_device** for the device library.
3. Add needed includes and function calls into your code, build, and link with the two libraries as needed.


### config.h
Configure the following items:
* **General Configuration**
  * F_CPU         CPU frequency in Hertz (8 MHz is 8000000). Needed for all timing.
  * BUILD_DATE    Date of this build in YYYYMMDD format
  * VERSION_MAJOR  |
  * VERSION_MINOR   > Version number of the library -- do not change
  * VERSION_UPDATE |

* **ADC**  (Not yet implemented)
  
* **GPIO**
  >GPIO doesn't require any configuration parameters.  Each pin is 
  > numbered consecitively by port.  Port A, bit 0 is pin 0.  Port 
  > C, bit 3 is pin 19.  There are enums defined as GPIO_PIN_B0, 
  > GPIO_PIN_D7, etc.

  > GPIO pin names (GPIO_pin_t):
   * GPIO_PIN_B0         = 8
   * GPIO_PIN_B1         = 9
   * GPIO_PIN_B2         = 10
   * GPIO_PIN_B3         = 11
   * GPIO_PIN_B4         = 12
   * GPIO_PIN_B5         = 13
   * GPIO_PIN_B6         = 14
   * GPIO_PIN_B7         = 15
    
   * GPIO_PIN_C0         = 16
   * GPIO_PIN_C1         = 17
   * GPIO_PIN_C2         = 18
   * GPIO_PIN_C3         = 19
   * GPIO_PIN_C4         = 20
   * GPIO_PIN_C5         = 21
   * GPIO_PIN_C6         = 22
    
   * GPIO_PIN_D0         = 24
   * GPIO_PIN_D1         = 25
   * GPIO_PIN_D2         = 26
   * GPIO_PIN_D3         = 27
   * GPIO_PIN_D4         = 28
   * GPIO_PIN_D5         = 29
   * GPIO_PIN_D6         = 30
   * GPIO_PIN_D7         = 31
    
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
  
* **PWM**  (Not yet implemented)
  
* **Serial**
  * SERIAL_RX_BUFFER_SIZE    The number of bytes to use for the receive buffer.
  * SERIAL_TX_BUFFER_SIZE    The number of bytes for the transmit buffer.

* **SPI**       (Not yet implemented)

* **SoftSPI**   
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


* **Systick**
  >Systick offers a real-time clock, counting ticks and milliseconds.  It uses 
  > the main CPU clock divided by a selected prescaler.  The timer used is 
  > 8 bit timer 0, so it rolls over and interrupts every 256 cycles (after 
  > prescaling.)
  > Systick also provides for timed callbacks.  Each callback can run a 
  > single time or repetitively after a set number of ticks or milliseconds.
    * SYSTICK_COUNT   Set to the number of callback timers desired.
    * typedef void ( *callback_t) (void);

  > Prescale_t
    * CLK_NONE     = 0,  // Turn off clock
    * CLK_DIV_1    = 1,  // F_CPU / 1
    * CLK_DIV_8    = 2,  // F_CPU / 8
    * CLK_DIV_64   = 3,  // F_CPU / 64
    * CLK_DIV_256  = 4,  // F_CPU / 256
    * CLK_DIV_1024 = 5,  // F_CPU / 1024
    * CLK_EXT_T0   = 6,  // Clock from T0 input pin
    * CLK_EXT_T1   = 7   // Clock from T1 input pin

  * #define SYSTICK_VERSION_MAJOR     0
  * #define SYSTICK_VERSION_MINOR     1
  * #define SYSTICK_VERSION_BUILD     1
  * #define SYSTICK_VERSION_DATE      (20261027L)


  > Functions for SYSTICK:
    * void SYSTICK_init(const Prescale_t source);
    * uint32_t SYSTICK_get_ticks(void)
      > Return number of timer ticks since startup, mod 2^32.
    * uint64_t SYSTICK_get_ticks_long(void);
      > Return (long) number of ticks since startup.
    * uint32_t SYSTICK_get_milliseconds(void);
      > Return number of milliseconds since startup, mod 2^32.
    * uint64_t SYSTICK_get_milliseconds_long(void);
      > Return (long) number of milliseconds since startup.
    * int SYSTICK_set_timer_ms(const int32_t ms, const uint8_t repeat, const callback_t cb);
      > Initialize one of the callback timers with time in ms, repeat value, callback fn. Return the timer index.
    * int SYSTICK_set_timer_ticks(const int32_t ticks, const uint8_t repeat, const callback_t cb);
      > Initalize one of the callback timers with the time in ticks, repeat value, callback fn.  Return the timer index.
    * void SYSTICK_modify_timer_ms(const uint16_t index, const int32_t ms, const uint8_t repeat, callback_t cb);
      > Modify an initialized timer by index with ms, repeat value, callback fn.
    * void SYSTICK_modify_timer_ticks(const uint16_t index, const int32_t ticks, const uint8_t repeat, const callback_t cb);
      > Modify an initialized timer by index with ticks, repeat value, callback fn.
    * uint32_t SYSTICK_get_ms_remaining(const uint16_t index);
      > Get the number of millisconds remaining before the timer given by index fires (again.)
    * uint32_t SYSTICK_get_ticks_remaining(const uint16_t index);
      > Get the number of ticks remaining before the timer given by index fires (again.)
    * void SYSTICK_set_callback(const uint16_t index, const callback_t callback);
      > Set only the callback of previously allocated timer given by index.
    * float SYSTICK_get_irq_frequency(void);
      > Get the frequency of the timer ticks.
    * float SYSTICK_get_ms_per_tick(void);
      > Get the number of ms for each tick.
    * // void SYSTICK_handler(void) __attribute__((used,interrupt));


### device_config.h


* **button**
  * BUTTON_PINS should list GPIO pin names for all buttons, comma separated.
    > #define BUTTON_PINS  GPIO_PIN_B4, GPIO_PIN_D2, GPIO_PIN_D7
  * BUTTON_BUFFER_SIZE defines size of a FIFO circular buffer of presses.
    > #define BUTTON_BUFFER_SIZE        4
  * BUTTON_STABLE_COUNT Is how many reads in a row must button be stable.
    > #define BUTTON_STABLE_COUNT      10

* **dds_9833**
  > The DDS 9833 uses softSPI, so configure the pins in the 
  > softSPI config section.  Set the chip clock rate here.
  * #define DDS_MASTER_CLOCK 25000000
  
  * DDS 9833 functions
    * void DDS_9833_init(void)
      > Initialize the interface
    * void DDS_9833_write_frequency(uint32_t hz)
      > Sets frequency in Hertz of current channel.
    * void DDS_9833_write_phase(uint16_t deg)
      > Sets phase of currently selected channel in degrees.
    * void select_channel(uint8_t chan)
      > Select channel 0 or 1.


* **encoder**
  > Quadrature rotary encoder(s).  Can have multiple encoders, two pins each.
  > The code will track a count based on rotation of the encoder. The count 
  > will increment for clockwise rotation and decrement for counter-clockwise 
  > rotation.  The amount it increments per amount of turn will vary based 
  > on speed of rotation.  There are four rates of change: LOW, MED, FAST, 
  > and VERY_FAST.  The number of ticks between changes of the encoder 
  > determines which rate to use.  The boundary number of ticks for each 
  > level are configured here.
    * ENCODER_PINS
      > A list of pins used for the encoders.  As many encoders as you have 
      > pins available for can be accommodated.  Each takes two pins.  The 
      > list is in order, encoder 1 pin A, encoder 1 pin B, encoder 2 pin A, 
      > encoder 2 pin B, ... etc.  Separate all pin names with commas.  
    * ENCODER_INC_LOW
      > Number to change count when encoder is rotated slowly.
    * ENCODER_INC_MED
      > Number to change count when encoder is rotated medium fast.
    * ENCODER_INC_HIGH
      > Number to change count when encoder is rotated fast.
    * ENCODER_INC_VERY_HIGH
      > Number to change count when encoder is rotated very fast.
    * ENCODER_TICKS_LOW_MED
      > Number of ticks boundary from low to med speed rotation.
    * ENCODER_TICKS_MED_HIGH
      >Number of ticks boundary from med to high speed.
    * ENCODER_TICKS_HIGH_VERY_HIGH
      >Number of ticks boundary from high to very high.



* **keypad**
  > Define the pins for rows and columns with a comma separated list
  > of pin names for each.
  > BUFFER_SIZE is the number of bytes for the FIFO buffer of key presses
  > STABLE_COUNT is how many scans the press must be stable to be valid

  * #define KEYPAD_NUMBER_ROWS        4
  * #define KEYPAD_NUMBER_COLS        4
  * #define KEYPAD_ROW_PINS  GPIO_PIN_D4, GPIO_PIN_D5, GPIO_PIN_D6, GPIO_PIN_D7
  * #define KEYPAD_COL_PINS  GPIO_PIN_C0, GPIO_PIN_C1, GPIO_PIN_C2, GPIO_PIN_C3
  * #define KEYPAD_BUFFER_SIZE        8
  * #define KEYPAD_STABLE_COUNT       5


* **LCD_44780**
  >  Interfaces character LCD displays based on the Hitachi 
  >  HD44780 chip or equivalent (most character LCDs.)

  * Define the pins for LCD using GPIO pin names
  * LCD is operated in 4 bit mode, write only.
  * RS Register Select pin.
  * EN Enable pin.
  * D7 - D4 Data bus pins.
  * COLUMNS How many columns the LCD shows.
  * ROWS How many rows the LCD shows.

  * #define LCD_44780_RS            GPIO_PIN_B1
  * #define LCD_44780_EN            GPIO_PIN_B0
  * #define LCD_44780_D7            GPIO_PIN_D7
  * #define LCD_44780_D6            GPIO_PIN_D6
  * #define LCD_44780_D5            GPIO_PIN_D5
  * #define LCD_44780_D4            GPIO_PIN_D4
  * #define LCD_44780_COLUMNS       16
  * #define LCD_44780_ROWS          2

  * LCD_44780_param_t enums for below function calls.
    * LCD_44780_DECREMENT         = 0,                 // Entry Mode
    * LCD_44780_INCREMENT         = (1<<1),            // Entry Mode
    * // What about 'S' bit?                           // Entry Mode
    * LCD_44780_ON                = (1<<2),            // Display Enable
    * LCD_44780_CURSOR            = (1<<1),            // Display Enable
    * LCD_44780_BLINK             = (1<<0),            // Display Enable
    * LCD_44780_SHIFT_RIGHT       = ((1<<3)|(1<<2)),   // Cursor Display Shift
    * LCD_44780_SHIFT_LEFT        = (1<<3),            // Cursor Display Shift
    * LCD_44780_8_BIT             = (1<<4),            // Function Set
    * LCD_44780_TWO_LINES         = (1<<3),            // Function Set
    * LCD_44780_5X10              = (1<<2),            // Function Set
    * LCD_44780_param_t;


  * LCD_44780 functions
    * void LCD_44780_init(uint8_t num_cols, uint8_t num_rows, uint8_t rs, uint8_t en, uint8_t d7, uint8_t d6, uint8_t d5,  uint8_t d4);
      > Initialize the display with the given parameters. Four bit mode, write only.
    * void LCD_44780_init2(void);
      > Initialize the display from the config file parameters. Four bit mode, write only.
    * void LCD_44780_write_command( uint8_t cmd);
      > Write a command byte to the display.
    * void LCD_44780_write_data(uint8_t data);
      > Write a data byte to the display.
    * void LCD_44780_clear(void);
      > Clear the display.
    * void LCD_44780_home(void);
      > Home the cursor.
    * uint8_t LCD_44780_goto(int col, int row);
      > Move display to given column and row.
    * void LCD_44780_entry_mode(LCD_44780_param_t p);
      > Sets cursor move direction and display shift on entry.  Enums are ORd together.
    * void LCD_44780_display_enable(int p);
      > Enables or disables display, cursor, and blink. Enums ORd together.
    * void LCD_44780_cursor_display_shift(int p);
      > Sets cursor or display shift mode. Cursor Display Shift enums ORd together.
    * void LCD_44780_function_set(int p);
      > Sets display length, lines, and font.  Function Set enums ORd together.
    * void LCD_44780_set_CGRAM_address(int adr);
      > Sets CGRAM address pointer to adr.
    * void LCD_44780_set_DDRAM_address(int adr);
      > Sets DDRAM address to adr.
    * uint8_t LCD_44780_read_status(void);
      > Reads busy flag and address (NOT VALID FOR CURRENT LIBRARY!)
      > returns byte with current busy flag and address.
    * uint8_t LCD_44780_read_data(void);
      > Reads data from current adr. (NOT VALID FOR CURRENT LIBRARY!)
    * int LCD_44780_write_string(char* str);
      > Writes NULL terminated string to display. Returns number of chars written.


