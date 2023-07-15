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

        
static const int8_t transitions[4][4] =
{
        {0, 1, -1, 0},
        {-1, 0, 0, 1},
        {1, 0, 0, -1},
        {0, -1, 1, 0}
};

        static int32_t process_change(uint8_t idx, uint8_t reading)
        {
                int32_t rtn = 0;
                uint8_t prev = last_read[idx];
                uint32_t time = 0;  // TODO read clock
                reading &= 0x03;  // make sure we only use 2 bits
                rtn = transitions[prev][reading];
                if(rtn != 0)
                {
                        last_read[idx] = reading;
                        //last_change[idx] = time;
                        uint32_t delta = time - last_read[idx];
                        last_change[idx] = time;
                        if(delta <= ENCODER_TICKS_MED_HIGH)
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
                        
                
                        
                        
void ENCODER_init(void)
{
        // stuff
        for (int i = 0; i < NUMBER_ENCODER_PINS; i++)
        {
                GPIO_pin_mode(pins[i], GPIO_PIN_MODE_INPUT_PULLUP);
        }
        int callbackTimer = SYSTICK_set_timer_ms(1, 0, encoder_callback);
        
}
        
int32_t ENCODER_get_count(int idx);
void ENCODER_set_count(int idx, int32_t count);

        static void encoder_callback(void)
        {
                // do callback
        }
        

#ifdef __cplusplus
}
#endif
