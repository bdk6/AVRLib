//////////////////////////////////////////////////////////////////////////////
///  @file dds_9833.h
///  copyright(c) 2023,2014 William R Cooke
///  @brief Interfaces to Analog Devices AD9833 Direct Digital Synthesizer
//////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
}
#endif
#warning REmove extra brace from extern "C" declaration in dds_9833.c line 9


#include <stdint.h>
#include "device_config.h"

#define F_CPU 16000000
#include <util/delay.h>

#include "device_config.h"
#include "softspi.h"
#include "gpio.h"
#include "dds_9833.h"

#define MASTER_CLOCK     25000000L     // Master reference clock of AD9833

//////////////////////////////////////////////////////////////////////////////
/// @fn DDS_9833_init
/// @brief Initializes DDS interface from device_config.h
//////////////////////////////////////////////////////////////////////////////
void DDS_9833_init(void)
{
  SOFTSPI_init(GPIO_PIN_B5, GPIO_PIN_B3, -1);
  // ss on pb2
  SOFTSPI_set_interface(0, GPIO_PIN_C5, 16, SPI_MODE_2_MSB_FIRST, 0);
  DDRB |= 0x08;  // b3 output
        
  DDS_write_word(0x2100);  // Set B28 and RESET bits in control reg.
  DDS_write_frequency(0L);  // Set output to 0 hz at startup
  DDS_write_phase(0);
  DDS_write_word(0x2000);  // B2b, out of reset
}


//////////////////////////////////////////////////////////////////////////////
/// @fn write_word
/// @brief STATIC writes generic 16 bit word to  DDS
/// @param[in] wd  Data to write
//////////////////////////////////////////////////////////////////////////////
static void write_word(uint16_t wd)
{
  SOFTSPI_write(0,wd);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn DDS_9833_write_requency
/// @brief Sets frequency in Hertz of current channel.
/// @param[in] hz Frequency to set.
//////////////////////////////////////////////////////////////////////////////
void DDS_9833_write_frequency(uint32_t hz)
{
  uint32_t n = (uint32_t)((uint64_t) hz * (1L << 28) / MASTER_CLOCK);
  write_word( (uint16_t) (n & 0x3fff) | 0x4000);
  write_word( (uint16_t) ((n >> 14) & 0x3fff) | 0x4000);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn DDS_9833_write_phase
/// @brief Sets phase in degrees of currently selected channel.
/// @param[in] deg Degrees of phase to set.
//////////////////////////////////////////////////////////////////////////////
void DDS_9833_write_phase(uint16_t deg)
{
  deg %= 360;
  uint16_t ph = (uint16_t) (4096L * deg / 360);
  // write it to phase 0 reg
  ph |= 0xc000;  // Set D15, D14 to select phase 0
  write_word(ph);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn DDS_9833_select_channel
/// @brief Selects which channel further writes will affect.
/// @param[in] chan Channel to select
//////////////////////////////////////////////////////////////////////////////
void select_channel(uint8_t chan)
{
}


#ifdef __cplusplus
}
#endif


