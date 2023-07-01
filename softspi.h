//////////////////////////////////////////////////////////////////////////////
///
///  \file softspi.h
///
///  \copy copyright (c) 2023 William R Cooke
///
///  @brief Use GPIO to create SPI interface.
///
//////////////////////////////////////////////////////////////////////////////

#ifndef SOFTSPI_H
#define SOFTSPI_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "gpio.h"

#define SOFTSPI_VERSION_MAJOR     0
#define SOFTSPI_VERSION_MINOR     1
#define SOFTSPI_VERSION_BUILD     0
#define SOFTSPI_VERSION_DATE      (20230608L)


  // the modes have the clock idle either on high or low.
  // The data is shifted on the opposite edge of sampling.
  // 
  typedef enum SOFTSPI_MODE
    {        //                                   cpol        cpha
      SPI_MODE_0_MSB_FIRST              =  0, // idle low,  sample on leading
      SPI_MODE_0_LSB_FIRST              =  1, // idle low,  sample on leading
      SPI_MODE_1_MSB_FIRST              =  2, // idle low,  sample on trailing
      SPI_MODE_1_LSB_FIRST              =  3, // idle low,  sample on trailing
      SPI_MODE_2_MSB_FIRST              =  4, // idle high, sample on leading
      SPI_MODE_2_LSB_FIRST              =  5, // idle high, sample on leading
      SPI_MODE_3_MSB_FIRST              =  6, // idle high, sample on trailing
      SPI_MODE_3_LSB_FIRST              =  7, // idle high, sample on trailing
      SPI_MODE_0_MSB_FIRST_SLOW         =  8, // idle low,  sample on leading
      SPI_MODE_0_LSB_FIRST_SLOW         =  9, // idle low,  sample on leading
      SPI_MODE_1_MSB_FIRST_SLOW         = 10, // idle low,  sample on trailing
      SPI_MODE_1_LSB_FIRST_SLOW         = 11, // idle low,  sample on trailing
      SPI_MODE_2_MSB_FIRST_SLOW         = 12, // idle high, sample on leading
      SPI_MODE_2_LSB_FIRST_SLOW         = 13, // idle high, sample on leading
      SPI_MODE_3_MSB_FIRST_SLOW         = 14, // idle high, sample on trailing
      SPI_MODE_3_LSB_FIRST_SLOW         = 15, // idle high, sample on trailing

    } softspi_mode_t;
  
  
  
//////////////////////////////////////////////////////////////////////////////
/// @function SOFTSPI_init
/// @brief   Initializes SOFTSPI interfaces
/// @param[in] clk   GPIO pin for sclk
/// @param[in] mosi  GPIO pin for MOSI (-1 if not used)
/// @param[in] miso  GPIO pin for MISO (-1 if not used)

/// @return
/////////////////////////////////////////////////////////////////////////////
  int SOFTSPI_init(uint8_t clk, int8_t mosi, int8_t miso);

  ////////////////////////////////////////////////////////////////////////////
  /// @fn SOFTSPI_set_interface
  /// @brief Set up a softspi parameters
  /// @param[in] idx  SoftSPI index 0 to # buttons - 1
  /// @param[in] ss    GPIO pin for slave select, -1 if not used
  /// @param[in] bits  Number of bits per transfer
  /// @param[in] mode  SPI mode to set clock phase and polarity
  /// @param[in] bps   Max speed in bits per second: 0 for fast as possible.
  /// @return    TODO
  ////////////////////////////////////////////////////////////////////////////
  int SOFTSPI_set_interface(uint8_t idx, int8_t ss, uint8_t bits,
			    softspi_mode_t mode,  uint32_t bps);
    
  
  //////////////////////////////////////////////////////////////////////////////
  /// @fn SOFTSPI_write
  /// @brief Write (and read) word to/from interface
  /// @param[in] idx Interface index to use.
  /// @param[in] data Data (in low bits) to write
  /// @return Word read from interface
  /////////////////////////////////////////////////////////////////////////////
  uint32_t SOFTSPI_write(uint8_t idx, uint32_t data);
  
#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // #ifndef SOFTSPI_H
