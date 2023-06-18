//////////////////////////////////////////////////////////////////////////////
/// @file gpio.c
/// @copyright 2023 William R Cooke
/// @brief Implements generic interface for GPIO pins
//////////////////////////////////////////////////////////////////////////////


#include "gpio.h"
#include <stdint.h>


void GPIO_init( int modeb, int pinb, int modec, int pinc, int moded, int pind)
{
  DDRB = modeb;
  PORTB = pinb;
  DDRC = modec;
  PORTC = pinc;
  DDRD = moded;
  PORTD = pind;
}

void GPIO_pin_mode(int pin, int mode)
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
void GPIO_write_pin(int pin, int val)
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

void GPIO_toggle_pin(int pin)
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

int GPIO_read_pin(int pin)
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

int GPIO_read_output_pin(int pin)
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

  

  
