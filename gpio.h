//////////////////////////////////////////////////////////////////////////////
/// @file gpio.h
/// @copyright 2023 William R Cooke
/// @brief Defines generic interface for GPIO pins
//////////////////////////////////////////////////////////////////////////////
#ifndef GPIO_H
#define GPIO_H
#include <avr/io.h>

//////////////////////////////////////////////////////////////////////////////
/// @enum GPIO_pin_t
/// @brief Defines names for gpio pin numbers
/// @remark Initially defined for ATMega 8 pins.  Revisit for others.
/////////////////////////////////////////////////////////////////////////////

typedef enum GPIO_pin
  {
    GPIO_PIN_B0         = 8,
    GPIO_PIN_B1         = 9,
    GPIO_PIN_B2         = 10,
    GPIO_PIN_B3         = 11,
    GPIO_PIN_B4         = 12,
    GPIO_PIN_B5         = 13,
    GPIO_PIN_B6         = 14,
    GPIO_PIN_B7         = 15,
    
    GPIO_PIN_C0         = 16,
    GPIO_PIN_C1         = 17,
    GPIO_PIN_C2         = 18,
    GPIO_PIN_C3         = 19,
    GPIO_PIN_C4         = 20,
    GPIO_PIN_C5         = 21,
    GPIO_PIN_C6         = 22,
    
    GPIO_PIN_D0         = 24,
    GPIO_PIN_D1         = 25,
    GPIO_PIN_D2         = 26,
    GPIO_PIN_D3         = 27,
    GPIO_PIN_D4         = 28,
    GPIO_PIN_D5         = 29,
    GPIO_PIN_D6         = 30,
    GPIO_PIN_D7         = 31,
    
    GPIO_PIN_NONE       = 255
  } GPIO_pin_t;

//////////////////////////////////////////////////////////////////////////////
/// @enum GPIO_port_t
/// @brief Defines names for gpio ports
/// @remark Currently defined for ATMega8, revisit for others
/////////////////////////////////////////////////////////////////////////////
typedef enum GPIO_port
  {
    GPIO_PORT_B         = 1,
    GPIO_PORT_C         = 2,
    GPIO_PORT_D         = 3
  } GPIO_port_t;


//////////////////////////////////////////////////////////////////////////////
/// @enum GPIO_mode_t
/// @brief Defines names for gpio pin modes
//////////////////////////////////////////////////////////////////////////////
typedef enum GPIO_mode
  {
    GPIO_PIN_MODE_INPUT        = 0,
    GPIO_PIN_MODE_OUTPUT       = 1,
    GPIO_PIN_MODE_INPUT_PULLUP = 2
  } GPIO_mode_t;

    
// void GPIO_init( int modeb, int pinb, int modec, int pinc, int moded, int pind);
void GPIO_pin_mode(const GPIO_pin_t pin, const GPIO_mode_t mode);
void GPIO_write_pin(const GPIO_pin_t pin, const int val);
void GPIO_toggle_pin(const GPIO_pin_t pin);
int GPIO_read_pin(const GPIO_pin_t pin);
int GPIO_read_output_pin(const GPIO_pin_t pin);

#endif
