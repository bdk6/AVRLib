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

//////////////////////////////////////////////////////////////////////////////
/// @struct button
/// @brief Holds details for a single button: active state and stable count.
//////////////////////////////////////////////////////////////////////////////
typedef struct button
{
  uint8_t    active;   // 0 low, 1 high
  uint8_t    count;
} button_t;

//////////////////////////////////////////////////////////////////////////////
/// @array buttons
/// @brief Array to hold details for all defined buttons.
//////////////////////////////////////////////////////////////////////////////
static button_t buttons[NUMBER_BUTTONS];

//////////////////////////////////////////////////////////////////////////////
/// @array buffer
/// @brief Array FIFO for all button presses.
/////////////////////////////////////////////////////////////////////////////
static uint8_t buffer[BUTTON_BUFFER_SIZE];

/////////////////////////////////////////////////////////////////////////////
/// @var head
/// Point to head of FIFO
/////////////////////////////////////////////////////////////////////////
static uint8_t head = 0;

//////////////////////////////////////////////////////////////////////////////
/// var tail 
static uint8_t tail = 0;

static void insert(uint8_t b)
{
  if ((tail + 1) % BUTTON_BUFFER_SIZE != head)
  {
     tail = (tail + 1) % BUTTON_BUFFER_SIZE;
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
          GPIO_pin_mode(button_pins[i], GPIO_PIN_MODE_INPUT_PULLUP);
    buttons[i].active = 0;
    buttons[i].count =  0;
  }
  //                   ms rpt  callback
  SYSTICK_set_timer_ms(5, 0, callback);
}                
                
        

static void callback(void)
{
  for(int i = 0; i < NUMBER_BUTTONS; i++)
  {
    if(GPIO_read_pin(button_pins[i]) == buttons[i].active)
    {
      // inc count
      if(buttons[i].count < 255)
      {
        buttons[i].count++;
        // If stable long enough, count it as a press.
        if(buttons[i].count == BUTTON_STABLE_COUNT)
        {
          insert(i);
        }
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
