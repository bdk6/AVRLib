//////////////////////////////////////////////////////////////////////////////
/// @file keypad.h
/// @copy Copyright 2023 William R Cooke
/// @brief Interface for reading a keypad.
//////////////////////////////////////////////////////////////////////////////

#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

#include "systick.h"
#include "gpio.h"


void KEYPAD_init(void);
uint8_t KEYPAD_waiting(void);
int KEYPAD_get_key(void);



uint8_t KEYPAD_is_pressed(uint8_t row, uint8_t col);

int KEYPAD_press_count(uint8_t row, uint8_t col);


#endif
