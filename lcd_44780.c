//////////////////////////////////////////////////////////////////////////////
///  @file lcd_44780.c
///  copyright(c) 2023,2014 William R Cooke
///  @brief Interfaces AVR to character LCDs using HD44780 or compatible
//////////////////////////////////////////////////////////////////////////////

#include "device_config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "gpio.h"
#include "lcd_44780.h"


#define CMD 0   // Select CMD reg
#define DAT 1   // Select DATA reg
#define RD  1   // Select READ mode
#define WR  0   // Select WRITE mode

#define DELAY_1     10

static uint8_t columns = 8;
static uint8_t rows = 1;

//#ifndef RS
//#warn Need to define these
static uint8_t RS;         // Register select pin
static uint8_t EN;         // Enable pin
static uint8_t D7;         // D7 pin
static uint8_t D6;         // D6 pin
static uint8_t D5;         // D5 pin
static uint8_t D4;         // D4 pin
//#endif






static void RS_set(int rs)
{
  GPIO_write_pin(RS, rs);
}

static void E_set(int e) // pb5
{
  GPIO_write_pin(EN, e);
}


static void wait(void)
{
}

static void write_nibble(uint8_t nib)
{
  GPIO_write_pin(D7, nib & 0x08);
  GPIO_write_pin(D6, nib & 0x04);
  GPIO_write_pin(D5, nib & 0x02);
  GPIO_write_pin(D4, nib & 0x01);
  
  //nib &= 0x0f;  // Use only low four bits
  //uint8_t tmp = PORTB;
  //tmp &= 0xf0;  // Keep port's high bits as-is
  //tmp |= nib;   // Merge the nibble bits
  //PORTB = tmp;
  _delay_us(DELAY_1);//5);
  //E_set(1);
  GPIO_write_pin(EN, 1);
  _delay_us( DELAY_1); //5);
  //E_set(0);
  GPIO_write_pin(EN, 0);
  _delay_us( DELAY_1 ); //5);
  
}


//////////////////////////////////////////////////////////////////////////////
///  \b LCD_44780_init
///  \brief Initializes LCD display
///  \param[in] num_cols Number of columns on display
///  \param[in] num_rows Number of rows on display
///  @param[in] rs  Pin for register select
///  @param[in] en  Pin for enable
///  @param[in] d7  Pin for data 7
///  @param[in] d6  Pin for data 6
///  @param[in] d5  Pin for data 5
///  @param[in] d4  Pin for data 4
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_init(uint8_t num_cols, uint8_t num_rows ,
		    uint8_t rs, uint8_t en,
		    uint8_t d7, uint8_t d6, uint8_t d5, uint8_t d4)
{
   columns = num_cols;
   rows = num_rows;
   RS = rs;
   EN = en;
   D7 = d7;
   D6 = d6;
   D5 = d5;
   D4 = d4;
   
   GPIO_pin_mode(RS, GPIO_PIN_MODE_OUTPUT);
   GPIO_pin_mode(EN, GPIO_PIN_MODE_OUTPUT);
   GPIO_pin_mode(D7, GPIO_PIN_MODE_OUTPUT);
   GPIO_pin_mode(D6, GPIO_PIN_MODE_OUTPUT);
   GPIO_pin_mode(D5, GPIO_PIN_MODE_OUTPUT);
   GPIO_pin_mode(D4, GPIO_PIN_MODE_OUTPUT);
   

   //DDRB |= (1 << 4);  // RS as output
   //DDRC |= (1 << 0);  // RW as output
   //DDRB |= (1 << 5);  // E as output
   //DDRB |= 0x0f;      // D4 to D7 as output

   //RS_set(0);  // Command mode
   GPIO_write_pin(RS, 0);

   // not using RW
   //RW_set(0);  // Write mode
   
   //E_set(0);
   GPIO_write_pin(EN, 0);
   
   _delay_ms( 50 );  // wait for it to finish initialization
   // Set 4 bit mode
   write_nibble(3);
   _delay_us( DELAY_1 );   // was 15
   write_nibble(3);
   _delay_us( DELAY_1 );
   write_nibble(2);
   _delay_us( DELAY_1 );
   
   // Now set it how we want it
   LCD_44780_function_set(LCD_44780_TWO_LINES);
   LCD_44780_entry_mode(LCD_44780_INCREMENT);
   LCD_44780_display_enable(LCD_44780_ON); // turn on the display
   LCD_44780_clear();
   LCD_44780_home();
}

//////////////////////////////////////////////////////////////////////////////
///  \b LCD_44780_write_command
///  \brief Send a command to lcd
///  \param[in]  cmd  The command byte to send
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_write_command( uint8_t cmd)
{
  RS_set(CMD);
  write_nibble( (cmd >> 4) & 0x0f );
  write_nibble( cmd & 0x0f );
  _delay_us( DELAY_1 );
}

//////////////////////////////////////////////////////////////////////////////
///  \b LCD_44780_write_data
///  \param[in] data The data byte to write to LCD
/////////////////////////////////////////////////////////////////////////////
void LCD_44780_write_data(uint8_t data)
{
  RS_set(DAT);
  write_nibble( (data >> 4) & 0x0f );
  write_nibble( data & 0x0f );
  _delay_us(  DELAY_1 );
}

//////////////////////////////////////////////////////////////////////////////
/// \b LCD_44780_clear
/// \brief Clears display and sets address to 0
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_clear(void)
{
  LCD_44780_write_command(0x01);
  _delay_ms(5);
}

//////////////////////////////////////////////////////////////////////////////
/// \b LCD_44780_home
/// \brief Homes cursor
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_home(void)
{
  LCD_44780_write_command(0x02);
  _delay_ms(5);
}

//////////////////////////////////////////////////////////////////////////////
/// \b LCD_44780_entry_mode
/// \brief Sets cursor move directon and display shift on entry
/// @param[in] p Entry Mode enums or'ed together.
/// @param
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_entry_mode(LCD_44780_param_t p)
{
  uint8_t cmd = 0x04;
  cmd |= p;
  LCD_44780_write_command(cmd);
  _delay_us( DELAY_1 );  // was 40
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_display_enable
/// @brief Enables or disables display, cursor, and blink
/// @param[in] p Display Enable enums or'ed together
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_display_enable(int p)
{
  uint8_t cmd = 0x08;
  cmd |= p;
  LCD_44780_write_command(cmd);
  _delay_us( DELAY_1 ); // 40);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_cursor_display_shift
/// @brief Sets cursor or display shift mode
/// @param[in] p Cursor Display Shift enums or'ed together.
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_cursor_display_shift(int p)
{
  uint8_t cmd = 0x10;
  cmd |= p;
  LCD_44780_write_command(cmd);
  _delay_us( DELAY_1); //40);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_function_set
/// @brief SEts display length, lines, and font
/// @param[in] p Function Set enums or'ed together.
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_function_set(int p)
{
  uint8_t cmd = 0x20;
  cmd |= p;
  LCD_44780_write_command(cmd);
  _delay_us( DELAY_1); // 40);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_set_CGRAM_address
/// @brief Sets CGRAM address pointer.
/// @param[in] adr  Address to set.
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_set_CGRAM_address(int adr)
{
  uint8_t cmd = 0x40 + (uint8_t)(adr & 0x3f);
  LCD_44780_write_command(cmd);
  _delay_us( DELAY_1); // 40);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_set_DDRAM_address
/// @brief Sets DDRAM address pointer.
/// @param[in] adr Address to set.
//////////////////////////////////////////////////////////////////////////////
void LCD_44780_set_DDRAM_address(int adr)
{
  uint8_t cmd = 0x80 + (uint8_t)(adr & 0x7f);
  LCD_44780_write_command(cmd);
  _delay_us( DELAY_1); // 40);
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_goto
/// @brief Move cursor to column and row given
/// @param[in] col  Column to go to, 0 to 39
/// @param[in] row  Row to go to , 0 to 4.
/// @return Returns 1 if succesful, 0 if not
//////////////////////////////////////////////////////////////////////////////
uint8_t LCD_44780_goto(int col, int row)
{
  uint8_t rtn = 0;
  if(col < columns && row < rows)
    {
      rtn = 1;
      LCD_44780_set_DDRAM_address(row * 40 + col);
    }
  
  return rtn;
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_read_status
/// @brief Reads busy flag and address.
/// @return Returns byte with busy flag and address.
//////////////////////////////////////////////////////////////////////////////
uint8_t LCD_44780_read_status(void)
{
  uint8_t rtn = 0;
  // TODO read byte here
  return rtn;
}

//////////////////////////////////////////////////////////////////////////////
/// @fn LCD_44780_read_data
/// @brief Reads data from display.
/// @return The data.
//////////////////////////////////////////////////////////////////////////////
uint8_t LCD_44780_read_data(void)
{
  uint8_t rtn = 0;
  // TODO add stuff here
  return rtn;
}


/////////////////////////////////////////////////////////////////////////////
///  \b LCD_44780_write_string
///  \param[in] str String to write, null terminated
/// @return Number of characters written.
//////////////////////////////////////////////////////////////////////////////
int LCD_44780_write_string(char* str)
{
  int cnt = 0;
  char ch;
  while( ch = *str )
    {
      cnt++;
      LCD_44780_write_data(ch);
      str++;
    }
  return cnt;
}

  
