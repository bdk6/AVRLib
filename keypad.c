//////////////////////////////////////////////////////////////////////////////
/// @file keypad.c
/// @copy Copyright 2023 William R Cooke
/// @brief Read keypad on GPIO pins.
//////////////////////////////////////////////////////////////////////////////


#include "device_config.h"
#include "keypad.h"


static GPIO_pin_t rows[] = {KEYPAD_ROW_PINS};
static GPIO_pin_t cols[] = {KEYPAD_COL_PINS};

// Use compile-time calculations for sizes of arrays
static const uint8_t numberRows = sizeof(rows) / sizeof(rows[0]);
static const uint8_t numberCols = sizeof(cols) / sizeof(cols[0]);

typedef struct key
{
        uint8_t pressed;
} key_t;

key_t keys[numberRows][numberCols];

#if (KEYPAD_BUFFER_SIZE > 255)
#error KEYPAD BUFFER MUST BE less than 255 BYTES
#endif

static uint8_t buffer[KEYPAD_BUFFER_SIZE];        
static uint8_t head = 0;
static uint8_t tail = 0;

static void insert(uint8_t k)
{
        if( (tail + 1) % KEYPAD_BUFFER_SIZE != head)
        {
                tail = (tail + 1) % KEYPAD_BUFFER_SIZE;
                buffer[tail] = k;
        }
}

int KEYPAD_waiting(void)
{
        int tmp = (int)tail - head;
        if(tmp < 0)
        {
                tmp += KEYPAD_BUFFER_SIZE;
        }
        return  tmp;
}

int KEYPAD_get_key(void)
{
        int rtn = -1;
        if(tail != head)
        {
                rtn = buffer[head];
                head = (head + 1) % KEYPAD_BUFFER_SIZE;
        }
        return rtn;
}

        
void KEYPAD_init(void)
{
        numberRows = sizeof(rows) / sizeof(rows[0]);
        numberCols = sizeof(cols) / sizeof(cols[0]);

        // Initialize row pins and set them high
        // Could also make them inputs and make output to scan that row.
        for(int r = 0; r < numberRows; r++)
        {
                GPIO_pin_mode(rows[r], GPIO_PIN_MODE_OUTPUT);
                GPIO_write_pin(rows[r], 1);
        }

        // Initialize column pins as inputs with pullups
        for(int c = 0; c < numberCols; c++)
        {
                GPIO_pin_mode(cols[c], GPIO_PIN_MODE_INPUT_PULLUP);
        }

        // Initialize key structs
        for(int r = 0; r < numRows; r++)
        {
                for(int c = 0; c < numCols; c++)
                {
                        keys[r][c].pressed = 0;
                }
        }
        
        
}

        static void scan_callback(void)
        {
                // scan
                for(uint8_t r = 0; r < numRows; r++)
                {
                        GPIO_write_pin(rows[r], 0)
                        for(uint8_t c = 0; c < numCols; c++)
                        {
                                // something
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
                                                uint8_t k = r * numCols + c;
                                                insert(k);
                                        }
                                }
                                else
                                {
                                        // TODO: put in "release" codes
                                        // if pressed > KEYPAD_STABLE_COUNT
                                        // and goes to 0, then insert release code
                                        keys[r][c].pressed = 0;
                                }
                                
                        }
                }
        }
        

