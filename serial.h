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
#endif  // #ifndef SERIAL_H
