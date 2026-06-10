//////////////////////////////////////////////////////////////////////////////
/// @file encoder.h
/// @copy Copyright 2023 William R Cooke
/// @brief Interface for reading a keypad.
//////////////////////////////////////////////////////////////////////////////

#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

#include "systick.h"
#include "gpio.h"

void ENCODER_init(void);
int32_t ENCODER_get_count(int idx);
void ENCODER_set_count(int idx, int32_t count);


#endif   // ENCODER_H
