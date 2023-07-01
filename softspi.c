//////////////////////////////////////////////////////////////////////////////
///
///  \file softspi.c
///
///  \copy copyright (c) 2023 William R Cooke
///
///  @brief Use GPIO to create SPI interface.
///
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <avr/io.h>
  //#include <util/delay.h>
  
#include "avrlib_config.h"
  //  #include <util/delay.h>
  #include <util/delay_basic.h>
#include "gpio.h"
#include "softspi.h"
  

  static uint8_t sclk_pin;
  static int8_t mosi_pin;
  static int8_t miso_pin;
  
  typedef struct softspi
{
  uint32_t        bits_per_second;  // Max speed in bps, 0 fast as possible.
  int8_t          ss_pin;    // Slave Select Pin, -1 if not used
  uint8_t         delay_ticks;  // Optional delay between bits: calc from bps.
  softspi_mode_t  mode;      // Clock phase and polarity
  uint8_t         bits;      // How many bits to transfer per cycle
} softspi_t;

  static softspi_t spis[ SOFTSPI_INTERFACES];

//////////////////////////////////////////////////////////////////////////////
/// @function SOFTSPI_init
/// @brief   Initializes SOFTSPI interfaces
/// @param[in] clk   GPIO pin for sclk
/// @param[in] mosi  GPIO pin for MOSI (-1 if not used)
/// @param[in] miso  GPIO pin for MISO (-1 if not used)
/// @return  nonzero on error
/////////////////////////////////////////////////////////////////////////////
  int SOFTSPI_init(uint8_t clk, int8_t mosi, int8_t miso)
  {
    int rtn = 0;
    sclk_pin = clk;
    GPIO_pin_mode(clk, GPIO_PIN_MODE_OUTPUT);
    mosi_pin = mosi;
    //   if(mosi >= 0)
      {
	GPIO_pin_mode(mosi, GPIO_PIN_MODE_OUTPUT);
      }
    miso_pin = miso;
    if(miso >= 0)
      {
	GPIO_pin_mode(miso, GPIO_PIN_MODE_INPUT_PULLUP);
      }

    return rtn;
  }
  
    

  ////////////////////////////////////////////////////////////////////////////
  /// @fn SOFTSPI_set_interface
  /// @brief Set up a softspi parameters
  /// @param[in] idx   SoftSPI index 0 to # buttons - 1
  /// @param[in] ss    GPIO pin for slave select, -1 if not used
  /// @param[in] bits  Number of bits per transfer
  /// @param[in] mode  SPI mode to set clock phase and polarity
  /// @param[in] bps   Max speed in bits per second. 0 for fast as possible.
  /// @return    Zero on success or error code.
  ////////////////////////////////////////////////////////////////////////////
  int SOFTSPI_set_interface(uint8_t idx, int8_t ss, uint8_t bits,
			    softspi_mode_t mode, uint32_t bps)
  {
    int rtn = 0;
    if(idx < SOFTSPI_INTERFACES)
      {
	spis[idx].ss_pin = ss;
	GPIO_write_pin(ss, 1);   // initialize deselected
	GPIO_pin_mode(ss, GPIO_PIN_MODE_OUTPUT);
	spis[idx].bits_per_second = bps;
	spis[idx].mode = mode;
	spis[idx].bits = bits;
        spis[idx].delay_ticks = 0;   // For now.  May change if bps is requested.
      }
    else
      {
	rtn = -1;
      }

    // The a slow speed is requested, it is handled with _delay_us().
    // The max speed without any intentional slowing is somewhere
    // around 1 Mbs;  each bit takes approximately 1000 nS to send.  If
    // a specific max bit rate is requested, we calculate the time
    // per bit and use that for a delay loop.  Subtracting
    // the always-present 1000 nS overhead, we use the result as the
    // delay time.
    //
    // CAUTION!
    // This is not very accurate,but we try to err on the side
    // of caution.

    if(bps != 0 )
      {
        uint32_t ns = 1000000000L / bps;  // nS between bits
        ns -= 1000;  // Account for overhead time.
        uint32_t ns_per_tick = 1000000000L / F_CPU * 3; // per docs, 3 clk / tick.
        uint32_t ticks = ns / ns_per_tick;
        if(ticks > 255)
          {
            ticks = 0;  // Corresponds to 256 ticks, the max
          }
        spis[idx].delay_ticks = (uint8_t) ticks;
      }
        
    
    return rtn;
  }
  
    
  
  //////////////////////////////////////////////////////////////////////////////
  /// @fn SOFTSPI_write
  /// @brief Write (and read) word to/from interface
  /// @param[in] idx Interface index to use.
  /// @param[in] data Data (in low bits) to write
  /// @return Word read from interface
  /////////////////////////////////////////////////////////////////////////////
  uint32_t SOFTSPI_write(const uint8_t idx, const uint32_t data)
  {
    uint32_t rtn = 0;
    int8_t ss = spis[idx].ss_pin;
    uint8_t dly = spis[idx].delay_ticks;
    softspi_mode_t mode = spis[idx].mode;
    uint8_t bits = spis[idx].bits;
    uint32_t word = data;

    switch(mode)
    {
    #ifdef SOFTSPI_ENABLE_MODE_0_MSB_FIRST
    case SPI_MODE_0_MSB_FIRST:      // idle low, sample leading
        GPIO_write_pin(sclk_pin, 0);  // idle low
        GPIO_write_pin(ss, 0);
        word <<= (32 - bits);
        for(int b = 0; b < bits; b++)
        {
                GPIO_write_pin(mosi_pin, (int)((word >> 16)  & 0x8000));
          GPIO_write_pin(sclk_pin, 1);
          GPIO_write_pin(sclk_pin, 0);
          word <<= 1;
        }
        GPIO_write_pin(ss, 1);
        break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_0_LSB_FIRST  
    case SPI_MODE_0_LSB_FIRST:      // idle low, sample leading
        GPIO_write_pin(sclk_pin, 0);  // idle low
        GPIO_write_pin(ss, 0);
        for(int b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)(word & 0x01));
	    GPIO_write_pin(sclk_pin, 1);
	    GPIO_write_pin(sclk_pin, 0);
	    word >>= 1;
	  }
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_1_MSB_FIRST
    case SPI_MODE_1_MSB_FIRST:      // idle low, sample trailing
          GPIO_write_pin(sclk_pin, 0);  // idle low
          GPIO_write_pin(ss, 0);
          word <<= (32 - bits);
          GPIO_write_pin(ss, 0);
          for(int b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)((word >> 16)  & 0x8000));
            GPIO_write_pin(sclk_pin, 1);
            GPIO_write_pin(sclk_pin, 0);
            word <<= 1;
          }
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_1_LSB_FIRST  
    case SPI_MODE_1_LSB_FIRST:      // idle low, sample trailing
          GPIO_write_pin(sclk_pin, 0);  // idle low
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 1);
            GPIO_write_pin(sclk_pin, 0);
            word >>= 1;
          }
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_2_MSB_FIRST
    case SPI_MODE_2_MSB_FIRST:      // idle high, sample leading
          GPIO_write_pin(sclk_pin, 1);  // idle high
          word <<= (32 - bits);
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)((word >> 16) & 0x8000));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word <<= 1;
          }
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_2_LSB_FIRST
    case SPI_MODE_2_LSB_FIRST:      // idle high, sample leading
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word >>= 1;
          }
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_3_MSB_FIRST
    case SPI_MODE_3_MSB_FIRST:      // idle high, sample trailing
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          word <<= (sizeof(uint32_t) - bits);
          for(uint8_t b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)((word >> 16) & 0x8000));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word <<= 1;
          }
          GPIO_write_pin(ss, 1);
          break;
    #endif

    #ifdef SOFTSPI_ENABLE_MODE_3_LSB_FIRST
    case SPI_MODE_3_LSB_FIRST:      // idle high, sample trailing
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
                  GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word >>= 1;
          }
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_0_MSB_FIRST_SLOW
    case SPI_MODE_0_MSB_FIRST_SLOW:  // idle low, sample leading
          GPIO_write_pin(sclk_pin, 0);   // idle low
          GPIO_write_pin(ss, 0);
          word <<= (sizeof(uint32_t) - bits);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)((word >> 16) & 0x8000));
            GPIO_write_pin(sclk_pin, 1);
            GPIO_write_pin(sclk_pin, 0);
            word <<= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_0_LSB_FIRST_SLOW
    case SPI_MODE_0_LSB_FIRST_SLOW: // idle low, sample leading
          GPIO_write_pin(sclk_pin, 0);  // idle low
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 1);
            GPIO_write_pin(sclk_pin, 0);
            word >>= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_1_MSB_FIRST_SLOW
    case SPI_MODE_1_MSB_FIRST_SLOW:  // idle low, sample trailing
          GPIO_write_pin(sclk_pin, 0);   // idle low
          GPIO_write_pin(ss, 0);
          word <<= (sizeof(uint32_t) - bits);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)((word >> 16) & 0x8000));
            GPIO_write_pin(sclk_pin, 1);
            GPIO_write_pin(sclk_pin, 0);
            word <<= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_1_LSB_FIRST_SLOW
    case SPI_MODE_1_LSB_FIRST_SLOW:  // idle low, sample trailing
          GPIO_write_pin(sclk_pin, 0);  // idle low
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 1);
            GPIO_write_pin(sclk_pin, 0);
            word >>= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_2_MSB_FIRST_SLOW
    case SPI_MODE_2_MSB_FIRST_SLOW: // idle high, sample leading
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          word <<= (sizeof(uint32_t) - bits);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)((word >> 16) & 0x8000));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word <<= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_2_LSB_FIRST_SLOW
    case SPI_MODE_2_LSB_FIRST_SLOW: // idle high, sample leading
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word >>= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_3_MSB_FIRST_SLOW
    case SPI_MODE_3_MSB_FIRST_SLOW: // idle high, sample trailing
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          word <<= (sizeof(uint32_t) - bits);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)((word >> 16) & 0x8000));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word <<= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif
            
    #ifdef SOFTSPI_ENABLE_MODE_3_LSB_FIRST_SLOW
    case SPI_MODE_3_LSB_FIRST_SLOW: // idle high, sample trailing
          GPIO_write_pin(sclk_pin, 1);  // idle high
          GPIO_write_pin(ss, 0);
          for(uint8_t b = 0; b < bits; b++)
          {
            _delay_loop_1(dly);
            GPIO_write_pin(mosi_pin, (int)(word & 0x01));
            GPIO_write_pin(sclk_pin, 0);
            GPIO_write_pin(sclk_pin, 1);
            word >>= 1;
          }
          _delay_loop_1(dly);
          GPIO_write_pin(ss, 1);
          break;
    #endif            

	default:
	  break;
	}
	
    return rtn;
  }
  
  
#ifdef __cplusplus
}
#endif  // __cplusplus

