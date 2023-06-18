//////////////////////////////////////////////////////////////////////////////
///
///  \file systick.c
///
///  copyright (c) 2023, 2014 William R Cooke
///
///  Setup and handle systick timer and interrupt on  AVR
///
//////////////////////////////////////////////////////////////////////////////
#include <avr/interrupt.h>
#include <stdint.h>
#include <stddef.h>   // for NULL
//#include "LPC11xx.h"
//#include "system_LPC11xx.h"
#include <avr/io.h>
#include "config.h"
#include "systick.h"



static volatile uint64_t ticks = 0;

static float irq_freq;
static float ms_per_tick = 0.0;
//static volatile uint32_t milliseconds = 0;
//static volatile uint32_t milliseconds_high = 0;

static volatile systick_timer_t *timers;
static uint8_t num_timers;

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_init
///
///  \brief Initialize the systick timer and turn on interrupt
///
///  \param[in] source    Clock source for timer.
///  \param[in] tim       Array of systick_timer_t structs for user timers
///  \param[in] number    Number of user timers in array
///
//////////////////////////////////////////////////////////////////////////////

void SYSTICK_init(Prescale_t source, systick_timer_t *tim, uint8_t number) 
{
  // Set timer 0 clock source.
  uint8_t tmp = TCCR0 & ~0x07;
  tmp |= source & 0x07;
  TCCR0 = tmp;

  // Find ms per irq
  float freq = (float)F_CPU;
  freq /= 256;   // 8 bit counter rolls over at 256
  switch(source)
    {
    case CLK_DIV_1:
      ; // nothing to do
      break;

    case CLK_DIV_8:
      freq /= 8;
      break;

    case CLK_DIV_64:
      freq /= 64;
      break;

    case CLK_DIV_256:
      freq /= 256;
      break;

    case CLK_DIV_1024:
      freq /= 1024;
      break;

      // everything else we don't know
    default:
      ;
      break;
    }

  ms_per_tick = 1000.0 / freq;
  irq_freq = freq;

  // Set up interrupts
  // TIMSK
  // [ OCIE2 | TOIE2 | TICIE1 | OCIE1A | OCIE1B | TOIE1 | ... | TOIE0 ]
  TIMSK |= 0x01;  // enable T0 overflow interrupt
  

  // Set up user timers
  num_timers = number;
  timers = tim;

  for(int index = 0; index < num_timers; ++index)
    {
      timers[index].timeout_ticks = 0;
      timers[index].ticks_left = 0;
      timers[index].callback = NULL;
    }
  

}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ticks
///
///  \brief gets low 32 bits of tick counter
///
///  \return The number of timer ticks since startup mod 2^32
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_ticks(void)
{
  // disable interrupts
  uint8_t tmp = TIMSK;
  TIMSK &= ~(0x01);
  uint32_t t = (uint32_t) (ticks & 0xffffffff);
  // enable interrupts
  TIMSK = tmp;
  return t;
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ticks_long
///
///  \brief gets value of tick counter
///
///  \return The number of timer ticks since startup
///
//////////////////////////////////////////////////////////////////////////////
uint64_t SYSTICK_get_ticks_long(void)
{
  uint8_t tmp = TIMSK;
  TIMSK &= ~(0x01);
  uint64_t t = ticks;
  TIMSK = tmp;
  return t;
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_milliseconds
///
///  \brief gets value of milliseconds
///
///  \return number of milliseconds since startup
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_milliseconds(void)
{
  // disable interrupts
  uint8_t tmp = TIMSK;
  TIMSK &= ~(0x01);
  uint64_t t = ticks;
  TIMSK = tmp;
  
  return (uint32_t)(t * ms_per_tick);
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_milliseconds_long
///
///  \brief gets value of milliseconds as a uint64_t
///
///  \return number of milliseconds since startup
///
//////////////////////////////////////////////////////////////////////////////
uint64_t SYSTICK_get_milliseconds_long(void)
{
   // stop interrupts
   // TODO BDK disable irq NVIC_DisableIRQ(-1);
   uint8_t tmp = TIMSK;
   TIMSK &= ~(0x01);  // disable interrupt
   uint64_t t = ticks;
   TIMSK = tmp;  // restore previous state
   // TODO BDK enable irq NVIC_EnableIRQ(-1);
   TIMSK = tmp;   // restore interrupt to previous state
   return (uint64_t) ( t * ms_per_tick);
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_set_timer_ms
///
///  \brief sets a timer with number of milliseconds
///  
///  \param[in] index   timer number 0 to n-1
///  \param[in] ms      number of milliseconds to run
///  \param[in] repeat  TODO whts this mean?
///  \param[in] cb      Pointer to callback function (can be NULL.)
///
//////////////////////////////////////////////////////////////////////////////
void SYSTICK_set_timer_ms(uint32_t index, int32_t ms,
			  uint8_t repeat, callback_t cb)
{
   if (index < num_timers)
   {
     uint8_t tmp = TIMSK;
     TIMSK &= ~(0x01);  // disable interrupt
     timers[index].timeout_ticks = (uint32_t)(ms / ms_per_tick);
     timers[index].ticks_left = timers[index].timeout_ticks;
     timers[index].repeat = repeat;
     timers[index].callback = cb;
     TIMSK = tmp;
   }
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_set_timer_ticks
///
///  \brief sets a timer with number of ticks
///  
///  \param[in] index   timer number 0 to n-1
///  \param[in] ms      number of ticks to run
///  \param[in] repeat  TODO whts this mean?
///  \param[in] cb      Pointer to callback function (can be NULL.)
///
//////////////////////////////////////////////////////////////////////////////
void SYSTICK_set_timer_ticks(uint32_t index, int32_t ticks,
			     uint8_t repeat, callback_t cb)
{
   if (index < num_timers)
   {
     uint8_t tmp = TIMSK;
     TIMSK &= ~(0x01);  // disable interrupt
     timers[index].timeout_ticks = ticks;
     timers[index].ticks_left = timers[index].timeout_ticks;
     timers[index].repeat = repeat;
     timers[index].callback = cb;
     TIMSK = tmp;
   }
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ms_remaining
///  
///  \brief returns remaining ms of timer
/// 
///  \param index  timer number 0 to n-1
///  \return   Number of ticks remaining on timer
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_ms_remaining(uint32_t index)
{
  uint32_t ms = 0;
   if (index < num_timers)
   {
     uint8_t tmp = TIMSK;
     TIMSK &= ~(0x01);
     uint32_t ticks = timers[index].ticks_left;
     TIMSK = tmp;
     ms = (uint32_t)(ticks / ms_per_tick);
   }
   return ms;
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ticks_remaining
///  
///  \brief returns remaining ticks of timer
/// 
///  \param index  timer number 0 to n-1
///  \return   Number of ticks remaining on timer
///
//////////////////////////////////////////////////////////////////////////////
uint32_t SYSTICK_get_ticks_remaining(uint32_t index)
{
  uint32_t t = 0;
   if (index < num_timers)
   {
     uint8_t tmp = TIMSK;
     TIMSK &= ~(0x01);
     t = timers[index].ticks_left;
     TIMSK = tmp;
   }
   return t;
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b systick_set_callback
///
///  \brief sets callback of timer
///
///  \param[in]  index   Timer number to set 0 to n-1
///  \param[in]  cb      Pointer to callback function (can be NULL.)
///
//////////////////////////////////////////////////////////////////////////////
void SYSTICK_set_callback(uint32_t index, callback_t cb)
{
   if (index < num_timers)
   {
     uint8_t tmp = TIMSK;
     TIMSK &= ~(0x01);
     timers[index].callback = cb;
     TIMSK = tmp;
   }
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_irq_frequency
///
///  \brief Gets frequency of timer interrupts.
///
///  @return Frequency of timer interrupts
///
//////////////////////////////////////////////////////////////////////////////
float SYSTICK_get_irq_frequency(void)
{
  return irq_freq;
}

//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_get_ms_per_tick
///
///  \brief Gets number of milliseconds for each clock tick.
///
///  @return Milliseconds per tick
///
//////////////////////////////////////////////////////////////////////////////
float SYSTICK_get_ms_per_tick(void)
{
  return ms_per_tick;
}


//////////////////////////////////////////////////////////////////////////////
///
///  \b SYSTICK_handler
///
///  \brief Systick Timer interrupt handler
///  @remark Runs every IRQ.  Keep callbacks SHORT!  They run in
///  interrupt context.
///
/////////////////////////////////////////////////////////////////////////////
// LPC void SYSTICK_handler(void)
ISR(TIMER0_OVF_vect)
{
   ++ticks;

   for(int index = 0; index < num_timers; ++index)
     {
       // For one-shot it decrements to 0 and stays there.
       if(timers[index].ticks_left != 0)
	 {
	   timers[index].ticks_left--;
	   if(timers[index].ticks_left == 0)
	     {
	       if(timers[index].repeat != 0)
		 {
		   timers[index].ticks_left = timers[index].timeout_ticks;
		 }
	       if(timers[index].callback != NULL)
		 {
		   timers[index].callback();
		 }
	     }
	 }
     }

}
