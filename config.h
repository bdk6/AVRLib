



#define F_CPU        16000000    // Set clock frequency in Hertz
// use date -u +%Y%m%d%H%M%S utc
#define BUILD_DATE 20230611
#define VERSION_MAJOR      0
#define VERSION_MINOR      1
#define VERSION_UPDATE     0

// ADC


// GPIO

// PWM

// Serial
#define SERIAL_RX_BUFFER_SIZE     64
#define SERIAL_TX_BUFFER_SIZE     64


// SoftSPI
//////////////////////////////////////////////////////////////////////////////

// Need to state the number of devices connected to SPI
//#define SOFTSPI_INTERFACES 4

// Define pins for SPI clock, MOSI, and MISO
// Using GPIO_PIN_xx names
// MOSI and MISO are optional.  If not used,
// Set them to GPIO_PIN_NONE

#define SOFTSPI_CLK         GPIO_PIN_B5
#define SOFTSPI_MOSI        GPIO_PIN_B3
#define SOFTSPI_MISO        GPIO_PIN_NONE

// Define the interfaces
// see softspi_t struct in softspi.c

// bps  ss_pin  delay_ticks  mode  bits
// bps:  Maximum bit rate of spi interface. 0 for fast as possible
// ss_pin:  GPIO pin for slave select, GPIO_PIN_NONE if not used
//   
// bits:  How many bits to transfer per cycle

// bps       ss_pin            mode                bits
// u32       gpio pin          mode_t              u8

#define SOFTSPI_INTERFACES   \
   0,        GPIO_PIN_C5, 0,  SPI_MODE_2_MSB_FIRST,   16,  \
   0,     GPIO_PIN_C4, 0, SPI_MODE_2_MSB_FIRST, 8


//{ 100000,   GPIO_PIN_C4,   SPI_MODE_2_MSB_FIRST,    8 }


//{  GPIO_PIN_C5, 16, SPI_MODE_2_MSB_FIRST, 0


// SPI

// Systick
#define SYSTICK_COUNT    4

