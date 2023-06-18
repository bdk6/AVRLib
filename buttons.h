//////////////////////////////////////////////////////////////////////////////
///
///  \file buttons.h
///
///  \copy copyright (c) 2023 William R Cooke
///
///  @brief Debounce buttons on GPIO pins.
///
//////////////////////////////////////////////////////////////////////////////

#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"
#include "gpio.h"

#define BUTTONS_VERSION_MAJOR     0
#define BUTTONS_VERSION_MINOR     1
#define BUTTONS_VERSION_BUILD     0
#define BUTTONS_VERSION_DATE      (20230608L)

typedef struct button
{
  uint16_t count;    // Count of current state.
  uint8_t  state;    // Current state: pressed or not pressed
  uint8_t  previous; // Previous time???  TODO
  uint8_t  active;   // Is pressed high or low?
  uint8_t  pin;      // Number of GPIO pin attached to.
  uint8_t  minimum;  // Min number counts for "pressed"
} button_t;
  
static button_t btns[NUMBER_OF_BUTTONS];
  static uint8_t pressed[99];   //FIFO of pressed buttons (press and release?) TODO
    static uint8_t fifo[BUTTONS_FIFO_SIZE];
  
//////////////////////////////////////////////////////////////////////////////
/// @function BUTTONS_init
/// @brief 
/// @param[in] 
/// @return
/////////////////////////////////////////////////////////////////////////////
int BUTTONS_init(button_t [], uint8_t how_many);

  ////////////////////////////////////////////////////////////////////////////
  /// @fn BUTTONS_set_button
  /// @brief Set up a button parameters
  /// @param[in] idx  Button index 0 to # buttons - 1
  /// @param[in] pin  GPIO pin number of button.
  /// @param[in] act  0 for active low, non-zero for active high
  /// @param[in] min  Minimum stable counts for pressed.
  /// @return 0 if not succesful, negative  if succesful (error code)
  ////////////////////////////////////////////////////////////////////////////
  int BUTTONS_set_button(uint8_t idx, uint8_t pin, uint8_t act, uint8_t min);
  
  
////////////////////////////////////////////////////////////////////////////
/// @function BUTTONS_get_button
/// @brief Reads status of a particular button.
/// @param[in] index  Button number to check
/// @return 0 if not pressed, -1 if error, number milliseconds pressed
///////////////////////////////////////////////////////////////////////////
int BUTTONS_get_button(uint16_t index);
  
//////////////////////////////////////////////////////////////////////////////
/// @fn BUTTONS_scan
/// @brief callback to scan all buttons.  Use systick or other periodic timer.
//////////////////////////////////////////////////////////////////////////////
void BUTTONS_scan(void)
{
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    // read gpio
    // compare to previous
    // if same, inc count
    //   if count == min set flag
    // else set count 0, set prev
    // if count
  }
}
  
#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // #ifndef BUTTONS_H
