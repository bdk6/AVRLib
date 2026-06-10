//////////////////////////////////////////////////////////////////////////////
/// @file button.h
/// @copy Copyright 2023 William R Cooke
/// @brief Interface for button reading.
//////////////////////////////////////////////////////////////////////////////

#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

#include "systick.h"
#include "gpio.h"


void BUTTON_init(void);
int BUTTON_waiting(void);
int BUTTON_get_button(void);


void BUTTON_assign(GPIO_Pin_t pin);

int BUTTON_is_pressed(int idx);

int BUTTON_press_count(int idx);


#endif
