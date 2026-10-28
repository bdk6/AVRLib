//////////////////////////////////////////////////////////////////////////////
///
///  @file serial.c
///
///  @copyright copyright (c) 2023 William R Cooke
///
///  @brief Interface with hardware serial port(s).
///
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
  
#include "config.h"
#include "gpio.h"
#include "serial.h"
  
//////////////////////////////////////////////////////////////////////////////
/// @fn SERIAL_init
/// @brief Initialize hardware serial port.
/// @param[in]  idx Index of serial port number (0 if only one.)
/// @param[in]  bps Bits per second
/// @param[in]  parity Indicates type of parity: E,O,N,0,1
/// @param[in]  stop  Number of stop bits (1 or 2)
/// @param[in]  bits  Number of bits in word (5 to 9)
///
/// @return
/////////////////////////////////////////////////////////////////////////////
int SERIAL_init(const uint8_t idx, const uint32_t bps, const char parity, 
                const uint8_t stop, const uint8_t bits);


int SERIAL_write(const uint8_t idx, const int data);


int SERIAL_read(const uint8_t idx);


int SERIAL_received(const uint8_t idx);


int SERIAL_sending(const uint8_t idx);


int SERIAL_status(const uint8_t idx);
  
#ifdef __cplusplus
}
#endif  // __cplusplus

