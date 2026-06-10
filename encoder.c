//////////////////////////////////////////////////////////////////////////////
/// @file encoder.c
/// @copy Copyright 2023 William R Cooke
/// @brief Implementation for reading a keypad.
//////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "device_config.h"
#include "systick.h"
#include "gpio.h"
        #include "lcd_44780.h"
#include "encoder.h"


static const uint8_t pins[] = { ENCODER_PINS };
//static const uint8_t NUMBER_ENCODER_PINS = (sizeof(pins) / sizeof(pins[0]));
//static const uint8_t NUMBER_ENCODERS = (sizeof(pins) / sizeof(pins[0]) / 2 );

#define NUMBER_ENCODER_PINS (sizeof(pins) / sizeof(pins[0]))
#define NUMBER_ENCODERS (NUMBER_ENCODER_PINS / 2)
        
// Keep track of the current count and last time position changed.
static int32_t counts[NUMBER_ENCODERS];
static uint32_t last_read[NUMBER_ENCODERS];
static uint32_t last_change[NUMBER_ENCODERS];

static void encoder_callback(void);

//////////////////////////////////////////////////////////////////////////////
/// @var transitions
/// @brief Transition table for valid rotation increment / decrement.
/////////////////////////////////////////////////////////////////////////////
static const int8_t transitions[4][4] =
{
        { 0, 0, 0,  0 },  // {0, 1, -1, 0},
        { 0, 0, 0, -1 },  //{-1, 0, 0, 1},
        { 0, 0, 0,  1 },  //{1, 0, 0, -1},
        { 0, 0, 0,  0 }   //{0, -1, 1, 0}
};

//////////////////////////////////////////////////////////////////////////////
/// @fn read_encoder
/// @brief Reads raw encoder pins.
/// @param[in] idx Index of encoder to read.
/// @return Value read from encoder (0-to3), 255 if invalid;
//////////////////////////////////////////////////////////////////////////////
 static uint8_t read_encoder(uint8_t idx)
 {
         // TODO
         #warning Must finish read_encoder
         uint8_t rtn = 255;   // signals invalid

         if(idx < NUMBER_ENCODERS)
         {
                 uint8_t start = idx * 2;  // two pins per encoder
                 uint8_t A = GPIO_read_pin(pins[start]);
                 uint8_t B = GPIO_read_pin(pins[start + 1]);
                 rtn = (A << 1) | B;
         }
         
   return rtn;
}
        
//////////////////////////////////////////////////////////////////////////////
/// @fn process_change
/// @brief Find increment / decrement value of encoder.
/// @param[in] idx  Number of the encoder to adjust.
/// @param[in] reading Value read from pins.
/// @return Adjustment value for this encoder.
//////////////////////////////////////////////////////////////////////////////
static int32_t process_change(uint8_t idx, uint8_t reading)
{
  int32_t rtn = 0;
  uint8_t prev = last_read[idx];
  uint32_t time = SYSTICK_get_milliseconds();  // TODO read clock
  reading &= 0x03;  // make sure we only use 2 bits
  rtn = transitions[prev][reading];
  last_read[idx] = reading;
  //last_change[idx] = time;
  if(rtn != 0)
  {
          // last_read[idx] = reading;
          //last_change[idx] = time;
    uint32_t delta = time - last_change[idx];
    //last_change[idx] = time;
    last_change[idx] = time;
    if(delta <= ENCODER_TICKS_HIGH_VERY_HIGH)
    {
            rtn *= ENCODER_INC_VERY_HIGH;
    }
    else if(delta <= ENCODER_TICKS_MED_HIGH)
    {
      rtn *= ENCODER_INC_HIGH;
    }
    else if(delta <= ENCODER_TICKS_LOW_MED)
    {
      rtn *= ENCODER_INC_MED;
    }
    else
    {
      rtn *= ENCODER_INC_LOW;
    }
  }
  return rtn;
}
                        
                
        int tmr = 42;                  
//////////////////////////////////////////////////////////////////////////////
/// @fn ENCODER_init
/// @brief Uses device_config.h to configure hardware and variables.
//////////////////////////////////////////////////////////////////////////////
void ENCODER_init(void)
{
  for (int i = 0; i < NUMBER_ENCODER_PINS; i++)
  {
    GPIO_pin_mode(pins[i], GPIO_PIN_MODE_INPUT_PULLUP);
  }
   tmr = SYSTICK_set_timer_ms(1, 0, encoder_callback);
   counts[0] = tmr;
}

//////////////////////////////////////////////////////////////////////////////
/// @fn ENCODER_get_count
/// @brief Gets count value for a specific encoder.
/// @param[in] idx Index of the encoder to read.
/// @return Most recent count value.
//////////////////////////////////////////////////////////////////////////////
int32_t ENCODER_get_count(int idx)
{
  int32_t rtn = 0;
  if(idx < NUMBER_ENCODERS)
  {
          rtn = counts[idx];
  }
  return rtn;
}

//////////////////////////////////////////////////////////////////////////////
/// @fn ENCODER_set_count
/// @brief Pre-sets an encoder count.
/// @param[in] idx Index of the encoder to set.
/// @param[in] count Value to set encoder count to.
//////////////////////////////////////////////////////////////////////////////
void ENCODER_set_count(int idx, int32_t count)
{
  if(idx < NUMBER_ENCODERS)
  {
    counts[idx] = count;
  }
}

//////////////////////////////////////////////////////////////////////////////
/// @fn encoder_callback
/// @brief Processes any encoder pin changes on SYSTICK timer call.
//////////////////////////////////////////////////////////////////////////////
static void encoder_callback(void)
{
        
  // do callback
        for(uint8_t i = 0; i < NUMBER_ENCODERS; i++)
        {
                uint8_t enc = read_encoder(i);
                counts[i] += process_change(i, enc);
        }
        //    counts[0] +=7;
                
}

#warning Need to finish encoder.c       


#ifdef __cplusplus
}
#endif
