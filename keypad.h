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


void KEYPAD_assign(GPIO_pin_t pin);

int KEYPAD_is_pressed(int idx);

int KEYPAD_press_count(int idx);


#endif
