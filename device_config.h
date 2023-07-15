//////////////////////////////////////////////////////////////////////////////
///  @file device_config.h
///  @copy Copyright 2023 William R Cooke
///  @brief Configures devices controlled by device library.
//////////////////////////////////////////////////////////////////////////////

#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

// buttons
// PINS should list GPIO pin names for all buttons, comma separated
#define BUTTON_PINS  GPIO_PIN_D0, GPIO_PIN_D2, GPIO_PIN_D7
#define BUTTON_BUFFER_SIZE        4
#define BUTTON_STABLE_COUNT      10
// #define BUTTON_ACTIVE 1,0,0  // same # as pins
// #define BUTTON_STABLE_COUNTS 10, 5, 4  // same # as pins

// encoder
// Can have multiple encoders, two pins each
//                        A            B            A            B
#define ENCODER_PINS  GPIO_PIN_D3, GPIO_PIN_D4, GPIO_PIN_D5, GPIO_PIN_D6
// How many counts to change for slow, medium, fast revolution
#define ENCODER_INC_LOW  5, 100
#define ENCODER_INC_MED  50, 10000
#define ENCODER_INC_HIGH 500, 100000
// Ticks to change from slow to medium resolution
#define ENCODER_TICKS_LOW_MED   10
// Ticks to change from medium to fast resolution
#define ENCODER_TICKS_MED_HIGH   5


// keypad
// Define the pins for rows and columns with a comma separated list
// of pin names for each.
// BUFFER_SIZE is the number of bytes for the FIFO buffer of key  presses
// STABLE_COUNT is how many scans the press must be stable to be valid
#define KEYPAD_ROW_PINS  GPIO_PIN_B3, GPIO_PIN_B4, GPIO_PIN_B5, GPIO_PIN_B6
#define KEYPAD_COL_PINS  GPIO_PIN_C0, GPIO_PIN_C3, GPIO_PIN_C4
#define KEYPAD_BUFFER_SIZE        4
#define KEYPAD_STABLE_COUNT       5


// LCD_44780
// Define the pins for LCD using GPIO pin names

//#define RS   GPIO_PIN_B1
//#define EN   GPIO_PIN_B0
//#define D7   GPIO_PIN_D7
//#define D6   GPIO_PIN_D6
//#define D5   GPIO_PIN_D5
//#define D4   GPIO_PIN_D4

#endif 
