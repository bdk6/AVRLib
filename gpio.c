//////////////////////////////////////////////////////////////////////////////
/// @file gpio.c
/// @copyright 2023 William R Cooke
/// @brief Implements generic interface for GPIO pins
//////////////////////////////////////////////////////////////////////////////


#include "gpio.h"
#include <stdint.h>

// Need to revisit this
// void GPIO_init( int modeb, int pinb, int modec, int pinc, int moded, int pind)
// {
//   DDRB = modeb;
//   PORTB = pinb;
//   DDRC = modec;
//   PORTC = pinc;
//   DDRD = moded;
//   PORTD = pind;
// }

/////////////////////////////////////////////////////////////////////////////
/// @fn GPIO_pin_mode
/// @brief Sets a single pin to the desired input/output mode.
/// @param[in] pin  The pin to set.
/// @param[in] mode What mode to set it to (in, out, in with pullup).
/////////////////////////////////////////////////////////////////////////////
void GPIO_pin_mode(const GPIO_pin_t pin, const GPIO_mode_t mode)
{
  uint8_t bit  = (uint8_t)(pin & 0x07);  // low three bits are bit number
  uint8_t port = (uint8_t)(pin >> 3);    // higher bits are port
  uint8_t mask = (uint8_t)(1 << bit);    // bitmask

  if(mode == GPIO_PIN_MODE_OUTPUT)
  {
    switch(port)
	  {
	  case GPIO_PORT_B:
	    DDRB |= mask;
	    break;
	  
	  case GPIO_PORT_C:
	    DDRC |= mask;
	    break;
	  
	  case GPIO_PORT_D:
	    DDRD |= mask;
	    break;

	  default:
	    break;
	  }
  }
  else if(mode == GPIO_PIN_MODE_INPUT)
  {
    uint8_t dirmask = ~mask;   // invert the bits to set read mode
    switch(port)
	  {
	  case GPIO_PORT_B:
	    DDRB &= dirmask;
	    break;

	  case GPIO_PORT_C:
	    DDRC &= dirmask;
	    break;

	  case GPIO_PORT_D:
	    DDRD &= dirmask;
	    break;

	  default:
	    break;
	  }
  }
  else if(mode == GPIO_PIN_MODE_INPUT_PULLUP)
  {
    uint8_t dirmask = ~mask;   // invert bits to set read mode
    switch(port)
	  {
	  case GPIO_PORT_B:
	    DDRB &= dirmask;
	    PORTB |= mask;
	    break;

	  case GPIO_PORT_C:
	    DDRC &= dirmask;
	    PORTC |= mask;
	    break;

	  case GPIO_PORT_D:
	    DDRD &= dirmask;
	    PORTD |= mask;
	    break;

	  default:
	    break;
	  }
  }
}

      
    
// Any non-zero value writes a 1
/////////////////////////////////////////////////////////////////////////////
/// @fn GPIO_write_pin
/// @brief Sets an output pin to the desired state.
/// @param[in] pin Which pin to set.
/// @param[in] val Desired state (any non-zero value is HIGH).
/////////////////////////////////////////////////////////////////////////////
void GPIO_write_pin(const GPIO_pin_t pin, const int val)
{
  uint8_t bit = (uint8_t)(pin & 0x07);
  uint8_t port = (uint8_t)(pin >> 3);
  uint8_t mask = (uint8_t)(1 << bit);
  if(val == 0)
  {
    mask = ~mask;
    switch(port)
	  {
	  case GPIO_PORT_B:
	    PORTB &= mask;
	    break;

	  case GPIO_PORT_C:
	    PORTC &= mask;
	    break;

	  case GPIO_PORT_D:
	    PORTD &= mask;
	    break;

	  default:
	    break;
	  }
  }
  else
  {
    switch(port)
	  {
	  case GPIO_PORT_B:
	    PORTB |= mask;
	    break;

	  case GPIO_PORT_C:
	    PORTC |= mask;
	    break;

	  case GPIO_PORT_D:
	    PORTD |= mask;
	    break;

	  default:
	    break;
	  }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @fn GPIO_toggle_pin
/// @brief Toggles the state of an output pin.
/// @param[in] pin Which pin to toggle.
/////////////////////////////////////////////////////////////////////////////
void GPIO_toggle_pin(const GPIO_pin_t pin)
{
  uint8_t bit = (uint8_t)(pin & 0x07);
  uint8_t port = (uint8_t)(pin >> 3);
  uint8_t mask = (uint8_t)(1 << bit);
  mask = ~mask;
  switch(port)
  {
  case GPIO_PORT_B:
    PORTB &= mask;
    break;

  case GPIO_PORT_C:
    PORTC &= mask;
    break;

  case GPIO_PORT_D:
    PORTD &= mask;
    break;

  default:
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @fn GPIO_read_pin
/// @brief Reads the state of an input pin.
/// @param[in] pin Which pin to read.
/// @return 0 if pin is LOW, 1 if pin is HIGH.
/////////////////////////////////////////////////////////////////////////////
int GPIO_read_pin(const GPIO_pin_t pin)
{
  int rtn = 0;
  uint8_t bit = (uint8_t)(pin & 0x07);
  uint8_t port = (uint8_t)(pin >> 3);

  switch(port)
  {
  case GPIO_PORT_B:
    rtn = PINB;
    break;

  case GPIO_PORT_C:
    rtn = PINC;
    break;

  case GPIO_PORT_D:
    rtn = PIND;
    break;

  default:
    break;
  }
  rtn = (rtn >> bit) & 0x01;

  return rtn;
}

/////////////////////////////////////////////////////////////////////////////
/// @fn GPIO_read_output_pin
/// @brief Reads the output state of a pin.
/// @param[in] pin Which pin to read.
/// @return 0 if output is set LOW, 1 if set HIGH.
////////////////////////////////////////////////////////////////////////////
int GPIO_read_output_pin(const GPIO_pin_t pin)
{
  int rtn = 0;
  uint8_t bit = (uint8_t)(pin & 0x07);
  uint8_t port = (uint8_t)(pin >> 3);

  switch(port)
  {
  case GPIO_PORT_B:
    rtn = PORTB;
    break;

  case GPIO_PORT_C:
    rtn = PORTC;
    break;

  case GPIO_PORT_D:
    rtn = PORTD;
    break;

  default:
    break;
  }
  rtn = (rtn >> bit) & 0x01;

  return rtn;
}



  

  
