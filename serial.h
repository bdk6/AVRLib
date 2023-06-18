//////////////////////////////////////////////////////////////////////////////
///
///  \file serial.h
///
///  \copy copyright (c) 2023 William R Cooke
///
///  @brief Interface with hardware serial port(s).
///
//////////////////////////////////////////////////////////////////////////////

#ifndef SERIAL_H
#define SERIALS_H

#ifdef __cplusplus
extern "C"
{
#endif

  #include <stdint.h>
  #include <avr/interrupt.h>
  #include <avr/io.h>
  
#include "config.h"
#include "gpio.h"

#define SERIAL_VERSION_MAJOR     0
#define SERIAL_VERSION_MINOR     1
#define SERIAL_VERSION_BUILD     0
#define SERIAL_VERSION_DATE      (20230608L)


  
  //static button_t btns[NUMBER_OF_BUTTONS];
  // static uint8_t pressed[99];   //FIFO of pressed buttons (press and release?) TODO
  //static uint8_t fifo[BUTTONS_FIFO_SIZE];
  
//////////////////////////////////////////////////////////////////////////////
/// @function SERIAL_init
/// @brief Initialize hardware serial port.
/// @param[in]  idx Index of serial port number (0 if only one.)
/// @param[in]  bps Bits per second
/// @param[in]  parity Indicates type of parity: E,O,N,0,1
/// @param[in]  stop  Number of stop bits (1 or 2)
/// @param[in]  bits  Number of bits in word (5 to 9)
///
/// @return
/////////////////////////////////////////////////////////////////////////////
int SERIAL_init(uint8_t idx, uint32_t bps, char parity, uint8_t stop, uint8_t bits);

  int SERIAL_write(uint8_t idx, int data);

  int SERIAL_read(uint8_t idx);

  int SERIAL_received(uint8_t idx);

  int SERIAL_sending(uint8_t idx);

  int SERIAL_status(uint8_t idx);

  
#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // #ifndef SERIAL_H
