//////////////////////////////////////////////////////////////////////////////
///
///  \file systick.h
///
///  \copy copyright (c) 2023, 2014 William R Cooke
///
///  @brief Setup and handle systick timer and interrupt on AVR
///
//////////////////////////////////////////////////////////////////////////////

#ifndef SYSTICK_H
#define SYSTICK_H

#ifdef __cplusplus
extern "C"
{
#endif

  #include <avr/interrupt.h>
  #include <stdint.h>
  


  // TODO
  // STuff to go in some other config file

  typedef void (*callback_t)(void);

  typedef enum Prescale
    {
      CLK_NONE     = 0,
      CLK_DIV_1    = 1,
      CLK_DIV_8    = 2,
      CLK_DIV_64   = 3,
      CLK_DIV_256  = 4,
      CLK_DIV_1024 = 5,
      CLK_EXT_T0   = 6,
      CLK_EXT_T1   = 7
    } Prescale_t;
  
      

  // END TODO




  
#include <stdint.h>
// BDK #include "LPC11xx.h"

#define SYSTICK_VERSION_MAJOR     0
#define SYSTICK_VERSION_MINOR     1
#define SYSTICK_VERSION_BUILD     1
#define SYSTICK_VERSION_DATE      (20230608L)



//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_init
///
///  \brief Initialize the systick timer and turn on interrupt
///
///  \param[in]  source    Clock source for timer.
///
//////////////////////////////////////////////////////////////////////////////

void SYSTICK_Init(Prescale_t source);


//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ticks
///
///  \brief gets low 32 bits of tick counter
///
///  \return The number of timer ticks since startup mod 2^32
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_ticks(void);

  //////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ticks_long
///
///  \brief gets value of tick counter
///
///  \return The number of timer ticks since startup
///
//////////////////////////////////////////////////////////////////////////////
uint64_t SYSTICK_get_ticks_long(void);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_milliseconds
///
///  \brief gets value of milliseconds
///
///  \return number of milliseconds since startup
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_milliseconds(void);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_milliseconds_long
///
///  \brief gets value of milliseconds as a uint64_t
///
///  \return number of milliseconds since startup
///
//////////////////////////////////////////////////////////////////////////////
  uint64_t SYSTICK_get_milliseconds_long(void);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_set_timer_ms
///
///  \brief sets a timer with number of milliseconds
///
///  \param[in]  ms     number of milliseconds to run
///  \param[in]  repeat TODO whats this mean?
///  \param[in]  cb     Pointer to callback function (can be NULL.)
///  \return      Index of timer set, -1 if none available.
//////////////////////////////////////////////////////////////////////////////
int SYSTICK_set_timer_ms(int32_t ms, uint8_t repeat, callback_t cb);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_set_timer_ticks
///
///  \brief sets a timer with number of milliseconds
///  
///  \param[in]  ms     number of milliseconds to run
///  \param[in]  repeat TODO whats this mean?
///  \param[in]  cb     Pointer to callback function (can be NULL.)
///  \return     Index of timer set, -1 if none available.
//////////////////////////////////////////////////////////////////////////////
int SYSTICK_set_timer_ticks(int32_t ticks, uint8_t repeat, callback_t cb);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_modify_timer_ms
///
///  \brief modifies a timer with number of milliseconds
///  
///  \param[in]  idx    index of timer to modify
///  \param[in]  ms     number of milliseconds to run
///  \param[in]  repeat TODO whats this mean?
///  \param[in]  cb     Pointer to callback function (can be NULL.)
///
//////////////////////////////////////////////////////////////////////////////
void SYSTICK_modify_timer_ms(uint16_t index, int32_t ms,
			  uint8_t repeat, callback_t cb);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_modify_timer_ticks
///
///  \brief modified a timer with number of milliseconds
///  
///  \param[in]  index  timer number 0 to n-1
///  \param[in]  ms     number of milliseconds to run
///  \param[in]  repeat TODO whats this mean?
///  \param[in]  cb     Pointer to callback function (can be NULL.)
///
//////////////////////////////////////////////////////////////////////////////
void SYSTICK_modify_timer_ticks(uint16_t index, int32_t ticks,
			     uint8_t repeat, callback_t cb);
  
//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ms_remaining
///  
///  \brief returns remaining milliseconds of a  timer
/// 
///  \param index  timer number 0 to n-1
///  \return   Number of milliseconds remaining on timer
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_ms_remaining(uint16_t index);

  //////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ticks_remaining
///  
///  \brief returns remaining ticks of a  timer
/// 
///  \param index  timer number 0 to n-1
///  \return   Number of ticks remaining on timer
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_ticks_remaining(uint16_t index);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_set_callback
///
///  \brief sets callback of timer
///
///  \param[in]  index   timer number to set 0 to n-1
///  \param[in   callback  pointer to callback function
///
//////////////////////////////////////////////////////////////////////////////
void SYSTICK_set_callback(uint16_t index, callback_t callback);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_irq_frequency
///
///  \brief Gets frequency of timer interrupts.
///
///  @return Frequency of timer interrupts
///
//////////////////////////////////////////////////////////////////////////////
float SYSTICK_get_irq_frequency(void);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ms_per_tick
///
///  \brief Gets number of milliseconds for each clock tick.
///
///  @return Milliseconds per tick
///
//////////////////////////////////////////////////////////////////////////////
float SYSTICK_get_ms_per_tick(void);

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_handler
///
///  \brief Systick Timer interrupt handler
///
/////////////////////////////////////////////////////////////////////////////
// void SYSTICK_handler(void) __attribute__((used,interrupt));

#ifdef __cplusplus
}
#endif

#endif
