//////////////////////////////////////////////////////////////////////////////
/// @file button.c
/// @copy Copyright 2023 William R Cooke
/// @brief Implements button reading module.
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

#include "device_config.h"
#include "systick.h"
#include "button.h"

GPIO_Pin_t button_pins[] = {BUTTON_PINS};
#define NUMBER_BUTTONS (sizeof(button_pins) / sizeof(GPIO_Pin_t))

        static void callback(void);
        
typedef struct button
{
  uint8_t    active;   // 0 low, 1 high
  uint8_t    count;
} button_t;

static button_t buttons[NUMBER_BUTTONS];
        static uint8_t buffer[BUTTON_BUFFER_SIZE];
        static uint8_t head = 0;
        static uint8_t tail = 0;

        static void insert(uint8_t b)
        {
                if ((tail + 1) % BUTTON_BUFFER_SIZE != head)
                {
                        tail = (tail + 1) % KEYPAD_BUFFER_SIZE;
                        buffer[tail] = b;
                }
        }

        int BUTTON_waiting(void)
        {
                int tmp = (int)tail - head;
                if(tmp < 0)
                {
                        tmp += BUTTON_BUFFER_SIZE;
                }
                return tmp;
        }

        int BUTTON_get_button(void)
        {
                int rtn = -1;
                if(tail != head)
                {
                        rtn = buffer[head];
                        head = (head + 1) % BUTTON_BUFFER_SIZE;
                }
                return rtn;
        }
        
                
        
void BUTTON_init(void)
{
  for(int i = 0; i < NUMBER_BUTTONS; i++)
  {
    buttons[i].active = 0;
    buttons[i].count =  0;
  }
  //                  idx ms rpt  callback
  SYSTICK_set_timer_ms(0, 5, 0, callback);
}                
                
        

        

static void callback(void)
{
  for(int i = 0; i < NUMBER_BUTTONS; i++)
  {
    if(GPIO_read_pin(button_pins[i]) == buttons[i].active)
    {
      if(buttons[i].count == BUTTON_STABLE_COUNT)
      {
         // TODO output code
      }
                                
      // inc count
      if(buttons[i].count < 255)
      {
        buttons[i].count++;
      }
                             
    }
    else
    {
            // not active, reset
            buttons[i].count = 0;
    }
  }
}
        
#ifdef __cplusplus
}  // end extern c
#endif
