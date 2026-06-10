//////////////////////////////////////////////////////////////////////////////
/// @file keypad.c
/// @copy Copyright 2023 William R Cooke
/// @brief Read keypad on GPIO pins.
//////////////////////////////////////////////////////////////////////////////


#include "device_config.h"
#include "keypad.h"


static GPIO_Pin_t rows[KEYPAD_NUMBER_ROWS] = {KEYPAD_ROW_PINS};
static GPIO_Pin_t cols[KEYPAD_NUMBER_COLS] = {KEYPAD_COL_PINS};

// Use compile-time calculations for sizes of arrays
//static const uint8_t numberRows = 4; // TODO sizeof(rows) / sizeof(rows[0]);
//static const uint8_t numberCols = 4; // TODO sizeof(cols) / sizeof(cols[0]);

typedef struct key
{
        uint8_t pressed;
} key_t;

key_t keys[KEYPAD_NUMBER_ROWS][KEYPAD_NUMBER_COLS];

#if (KEYPAD_BUFFER_SIZE > 255)
#error KEYPAD BUFFER MUST BE less than 255 BYTES
#endif

static uint8_t buffer[KEYPAD_BUFFER_SIZE];        
static uint8_t head = 0;
static uint8_t tail = 0;

static void scan_callback(void);

//////////////////////////////////////////////////////////////////////////////
/// @fn insert
/// @brief Inserts a keypress code (release later) into FIFO.
/// @param[in] k Key code to insert
//////////////////////////////////////////////////////////////////////////////
static void insert(uint8_t k)
{
  if( (tail + 1) % KEYPAD_BUFFER_SIZE != head)
  {
    buffer[tail] = k;
    tail = (tail + 1) % KEYPAD_BUFFER_SIZE;
  }
}

//////////////////////////////////////////////////////////////////////////////
/// @fn KEYPAD_init
/// @brief Use config file (device_config.h) to configure operation.
//////////////////////////////////////////////////////////////////////////////
void KEYPAD_init(void)
{
  // Initialize row pins and set them high
  // Could also make them inputs and make output to scan that row.
  for(int r = 0; r < KEYPAD_NUMBER_ROWS; r++)
  {
    GPIO_pin_mode(rows[r], GPIO_PIN_MODE_OUTPUT);
    GPIO_write_pin(rows[r], 1);
  }

  // Initialize column pins as inputs with pullups
  for(int c = 0; c < KEYPAD_NUMBER_COLS; c++)
  {
    GPIO_pin_mode(cols[c], GPIO_PIN_MODE_INPUT_PULLUP);
  }

  // Initialize key structs
  for(int r = 0; r < KEYPAD_NUMBER_ROWS; r++)
  {
    for(int c = 0; c < KEYPAD_NUMBER_COLS; c++)
    {
      keys[r][c].pressed = 0;
    }
  }
  for(int i = 0; i < KEYPAD_BUFFER_SIZE; i++) buffer[i] = 9;
  SYSTICK_set_timer_ms(5, 0, scan_callback);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn KEYPAD_waiting
/// @brief Return number of codes waiting in FIFO.
/// @return Number of codes waiting in FIFO.
/////////////////////////////////////////////////////////////////////////////
uint8_t KEYPAD_waiting(void)
{
  int tmp = (int)tail - head;
  if(tmp < 0)
  {
    tmp += KEYPAD_BUFFER_SIZE;
  }
  return  tmp;
}

//////////////////////////////////////////////////////////////////////////////
/// @fn KEYPAD_get_key
/// @brief Retrieves key code from FIFO.
/// @return Next key code or -1 if empty.
/////////////////////////////////////////////////////////////////////////////
int KEYPAD_get_key(void)
{
  int rtn = -1;
  cli();
  if(tail != head)
  {
    rtn = buffer[head];
    head = (head + 1) % KEYPAD_BUFFER_SIZE;
  }
  sei();
  return  rtn;
}


// TODO functions that might come in handy later

//////////////////////////////////////////////////////////////////////////////
/// @fn KEYPAD_is_pressed
/// @brief Checks if a key is currently down.
/// @param[in] row Row of key to check
/// @param[in] col Column of key to check
/// @return 1 if pressed, 0 if not
//////////////////////////////////////////////////////////////////////////////
uint8_t KEYPAD_is_pressed(uint8_t row, uint8_t col);

//////////////////////////////////////////////////////////////////////////////
/// @fn KEYPAD_press_count
/// @brief Gets number of events key has been pressed (time.)
/// @param[in] row Row of key to check
/// @param[in] col Column of key to check
int KEYPAD_press_count(uint8_t row, uint8_t col);


//////////////////////////////////////////////////////////////////////////////
/// @fn scan_callback
/// @brief Scans keypad when called by SYSTICK timer
//////////////////////////////////////////////////////////////////////////////
static void scan_callback(void)
{
  // scan
  for(uint8_t r = 0; r < KEYPAD_NUMBER_ROWS; r++) GPIO_write_pin(rows[r],1);
  for(uint8_t r = 0; r < KEYPAD_NUMBER_ROWS; r++)
  {
    GPIO_write_pin(rows[r], 0);
    for(uint8_t c = 0; c < KEYPAD_NUMBER_COLS; c++)
    {
      uint8_t val = GPIO_read_pin(cols[c]);
      if(val == 0)
      {
        if(keys[r][c].pressed < 255)
        {
          keys[r][c].pressed++;
        }
                                        
        if(keys[r][c].pressed == KEYPAD_STABLE_COUNT)
        {
          // put key in buffer
          uint8_t k = r * KEYPAD_NUMBER_COLS + c;
          insert(k);
        }
      }
      else
      {
        // TODO: put in "release" codes
        // if pressed > KEYPAD_STABLE_COUNT
        // and goes to 0, then insert release code
        if(keys[r][c].pressed >= KEYPAD_STABLE_COUNT)
        {
          uint8_t k = r * KEYPAD_NUMBER_COLS + c;
          // set release bit flag
          k |= 0x80;  // high bit
          // insert(k);
        }
        keys[r][c].pressed = 0;
      }
    }
    GPIO_write_pin(rows[r], 1);
  }
}
        

