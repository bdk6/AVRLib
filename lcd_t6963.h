/////////////////////////////////////////////////////////////////////////////
/// @file lcd_t6963.h
/// @copyright 2026 William R Cooke
/// @brief Header for T6963 graphic lcd control.
/////////////////////////////////////////////////////////////////////////////

#ifndef _LCD_T6963_H_
#define _LCD_T6963_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"


// Pin definitions
// #define T6963_WR        2         // write
// #define T6963_RD        3         // Read
// #define T6963_CE       A5         // Chip Enable
// #define T6963_CD       A4         // Command / /Data
// #define T6963_RES      A3         // Reset
// #define T6963_D0        4         // Data 0
// #define T6963_D1        5         // Data 1
// #define T6963_D2        6         // Data 2
// #define T6963_D3        7         // Data 3
// #define T6963_D4        8         // Data 4
// #define T6963_D5        9         // Data 5
// #define T6963_D6       10         // Data 6
// #define T6963_D7       11         // Data 7
// #define T6963_FONT     12         // Font Select LOW: 8x8, HIGH: 6x8

// T6963 Command Definitions

// ****   NOTE!   ****
// When using MSB=0 commands, a status read must be performed
// Pointer Set Commands
//   0 0 1 0 0 N2 N1 N0
#define T6963_SET_CURSOR_POINTER          0x21     // [X Address] [Y Address]
#define T6963_SET_OFFSET_REGISTER         0x22     // [Data] [0]
#define T6963_SET_ADDRESS_POINTER         0x24     // [Low Address] [High Address]

// Control Word Set Commands
//  0  1  0  0  0  0 N1 N0
#define T6963_SET_TEXT_HOME_ADDRESS       0x40     // [Low Address] [High Address]
#define T6963_SET_TEXT_AREA               0x41     // [Columns] [0]
#define T6963_SET_GRAPHIC_HOME_ADDRESS    0x42     // [Low Address] [High Address]
#define T6963_SET_GRAPHIC_AREA            0x43     // [Columns] [0]

// Mode Set Commands
//  1  0  0  0 CG N2 N1 N0    // CG = char gen, N2-N0 = logic mode
#define T6963_SET_MODE                    0x80     // Generic SET MODE command
#define T6963_MODE_CG_ROM                 (1 << 3) // Use RAM character generator
#define T6963_MODE_OR                     0x00     // Set OR mode
#define T6963_MODE_XOR                    0x01     // Set XOR mode
#define T6963_MODE_AND                    0x03     // Set AND mode
#define T6963_MODE_ATTRIBUTE              0x04     // Set Text / Attribute mode

#define T6963_SET_OR_MODE                 0x80     // Set graphics OR mode
#define T6963_SET_EXOR_MODE               0x81     // Set graphics XOR mode
#define T6963_SET_AND_MODE                0x83     // Set graphics AND mode
#define T6963_SET_TEXT_ATTRIBUTE_MODE     0x84     // Set text attribute mode
#define T6963_SET_INTERNAL_CG_ROM_MODE    0x80     // Set Internal Char Gen mode
#define T6963_SET_EXTERNAL_CG_RAM_MODE    0x84     // Set External Char Gen mode

// Add or OR below modes with SET_MODE command
#define T6963_MODE_RAM_CG                (1 << 3)  // Use RAM character generator
#define T6963_MODE_OR                     0x00     // OR Mode
#define T6963_MODE_EXOR                   0x01     // XOR mode
#define T6963_MODE_AND                    0x03     // AND mode
#define T6963_MODE_TEXT_ATTRIBUTE         0x04     // TEXT ATTRIBUTE MODE



uint16_t T6963_setVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr);
uint16_t T6963_setGraphicVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr);




////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_init
///  @brief  Initialize display for use
////////////////////////////////////////////////////////////////////////////////
void T6963_init();


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_getStatus
///  @brief  Retrieves status byte from T6963 controller
///  @return  T6963 status byte
////////////////////////////////////////////////////////////////////////////////
//static uint8_t T6963_getStatus();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_getDataByte
///  @brief  Gets data byte from T6963 controller 
///  @return  Data byte read from controller
////////////////////////////////////////////////////////////////////////////////
// static uint8_t T6963_getDataByte();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_writeDataByte
///  @brief  Send a byte of data to controller
///  @param[in]  dat The data byte to send
////////////////////////////////////////////////////////////////////////////////
void T6963_writeDataByte(uint8_t dat);

////////////////////////////////////////////////////////////////////////////////
///  @fn writeCommandByte
///  @brief  Sends a command byte to controller. Send parameters prior to cmd.
///  @param[in] cmd The command byte to send
////////////////////////////////////////////////////////////////////////////////
void T6963_writeCommandByte(uint8_t cmd);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setCursor
///  @brief  Set cursor location
///  @param[in] x The horizontal coordinate (0 to 127)
///  @param[in] y The vertical coordinate (0 to 63)
///  @return  Zero on success, -1 if out of bounds
////////////////////////////////////////////////////////////////////////////////
int T6963_setCursor(int x, int y);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setOffsetPointer
///  @brief  Sets high five bits of char gen RAM location (2K granularity)
///  @param[in]  offs  The offset page (0 to 31)
///  @return  Zero on success, -1 if out of bounds
////////////////////////////////////////////////////////////////////////////////
int T6963_setOffsetPointer(uint8_t offs);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAddress
///  @brief  Sets controller RAM address for reads and writes
///  @param[in] addr  The RAM address to set (0 to 65535) (8191 on DG24064)
////////////////////////////////////////////////////////////////////////////////
void T6963_setAddress(uint16_t addr);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setTextHomeAddress
///  @brief  Sets RAM address used as top left text displayed text character
///  @param[in] addr The RAM address to use for top left character (0 - 65535)
////////////////////////////////////////////////////////////////////////////////
void T6963_setTextHomeAddress(uint16_t addr);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setGraphicHomeAddress
///  @brief Sets RAM address of top left graphics byte or text attribute byte
///  @param[in] addr The RAM address for graphics or attributes (0 - 65535)
////////////////////////////////////////////////////////////////////////////////
void T6963_setGraphicHomeAddress(uint16_t addr);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setTextArea
///  @brief  Define number of columns for text area of RAM, independent of h/w
///  @param[in] cols  Number of text columns
////////////////////////////////////////////////////////////////////////////////
void T6963_setTextArea(uint8_t cols);
////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setGraphicArea
///  @brief Define number of (text) columns for graphic area, independent of h/w
///  @param[in] cols Number of text columns
////////////////////////////////////////////////////////////////////////////////
void T6963_setGraphicArea(uint8_t cols);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setOrMode
///  @brief Set mode to OR text and graphic data and choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setOrMode(uint8_t CG);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setXorMode
///  @brief Set mode to XOR text and graphic data, choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setXorMode(uint8_t CG);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAndMode
///  @brief Set mode to AND text and graphic data, choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setAndMode(uint8_t CG);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setTextAttributeMode
///  @brief Set text attribute mode and choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setTextAttributeMode(uint8_t CG);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setDisplayMode
///  @brief  Choose to display or not, text, graphics, cursor, blink
///  @param[in] txt Set non-zero to display text.
///  @param[in] grph Set non-zero to display graphics.
///  @param[in] curs Set non-zero to display the cursor.
///  @param[in] blnk Set non-zero to make the cursor blink.
////////////////////////////////////////////////////////////////////////////////
void T6963_setDisplayMode(uint8_t txt, uint8_t grph, uint8_t curs, uint8_t blnk);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setCursorSize
///  @brief Select size of cursor from 1 (bottom only) to 8 (full block)
///  @param[in] siz Number of lines to use for cursor.
////////////////////////////////////////////////////////////////////////////////
void T6963_setCursorSize(uint8_t siz);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAutoWrite
///  @brief  Begins the autowrite mode.  Stays in that mode until autoreset.
////////////////////////////////////////////////////////////////////////////////
void T6963_setAutoWrite();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAutoRead
///  @brief Begins the autoread mode.  Stays in that mode until autoreset.
////////////////////////////////////////////////////////////////////////////////
void T6963_setAutoRead();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAutoReset
///  @brief  Ends autoread or autowrite mode.
////////////////////////////////////////////////////////////////////////////////
void T6963_setAutoReset();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataWriteIncrement
///  @brief  Write data at current address, increment address
///  @param[in] dat The data to write to RAM
////////////////////////////////////////////////////////////////////////////////
void T6963_dataWriteIncrement(uint8_t dat);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataWriteDecrement
///  @brief Write data at current address, decrement address
///  @param[in] dat The data to write to RAM
////////////////////////////////////////////////////////////////////////////////
void T6963_dataWriteDecrement(uint8_t dat);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataWrite
///  @brief  Write data at current address, leave address as is.
///  @param[in] dat The data to write to RAM
///  @return
////////////////////////////////////////////////////////////////////////////////
void T6963_dataWrite(uint8_t dat);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataReadIncrement
///  @brief  Read data from current address, increment address
///  @return  The byte read from RAM
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_dataReadIncrement();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataReadDecrement
///  @brief Read Data from current address, decrement address
///  @return The byte read from RAM
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_dataReadDecrement();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataRead
///  @brief Read byte from current address, leave address as is.
///  @return The byte read from RAM
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_dataRead();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_screenPeek
///  @brief  Read display byte from screen, using address pointer to graphics.
///  @return Byte read from screen.
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_screenPeek();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_screenCopy
///  @brief Copy one row of screen to graphics area
////////////////////////////////////////////////////////////////////////////////
void T6963_screenCopy();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setBit
///  @brief  Set a single bit at location in address pointer.
///  @param[in] b Bit number to set in byte (0 to 7)
////////////////////////////////////////////////////////////////////////////////
void T6963_setBit(uint8_t b);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_resetBit
///  @brief  Clear a single bit at location in addrss pointer
///  @param[in] b Bit number to clear in byte (0 to 7)
///  @return
////////////////////////////////////////////////////////////////////////////////
void T6963_resetBit(uint8_t b);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setVirtualSize
///  @brief  Sets size of virtual text screen
///  @param[in] address Address in display memory to place text buffer.
///  @param[in] width Width in columns of virtual area (32 to 80).
///  @param[in] height Height in rows of virtual area (8 to 2048).
///  @param[in] clr  Non zero to clear the new virtual screen.
///  @return  Next available address after this area
////////////////////////////////////////////////////////////////////////////////
uint16_t T6963_setVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_printChar
///  @brief  Print a single ASCII character to LCD
///  @param[in] c Character to print
////////////////////////////////////////////////////////////////////////////////
void T6963_printChar(char c);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_printString
///  @brief  Print a null terminated string to LCD
///  @param[in] str Pointer to the string to print
////////////////////////////////////////////////////////////////////////////////
void T6963_printString(char* str);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_textClear
///  @brief  Clear all text from screen
////////////////////////////////////////////////////////////////////////////////
void T6963_textClear();


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_graphicClear
///  @brief  Clear all graphics from screen
////////////////////////////////////////////////////////////////////////////////
void T6963_graphicClear();


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_clear
///  @brief  Clear all text and graphic data from screen
////////////////////////////////////////////////////////////////////////////////
void T6963_clear();

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_gotoXY
///  @brief  Move cursor to column (0 to width-1), row (0 to height-1) 
////////////////////////////////////////////////////////////////////////////////
void T6963_gotoXY(uint8_t x, uint8_t y);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setGraphicVirtualSize
///  @brief  Sets size of virtual graphic screen
///  @param[in] address Address in display memory to place graphic buffer.
///  @param[in] width Width in columns of virtual area (32 to 80).
///  @param[in] height Height in graphic lines of virtual area (64 to 256).
///  @param[in] clr  Non zero to clear the new virtual screen.
///  @return Next available address after this area.
////////////////////////////////////////////////////////////////////////////////
uint16_t T6963_setGraphicVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr);

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_plot
///  @brief Plot a point on display
///  @param[in] x  X coordinate 
///  @param[in] y  Y coordinate
///  @param[in] on  0 to clear, non zero to set
////////////////////////////////////////////////////////////////////////////////

void T6963_plot( int x, int y, int on);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_line
///  @brief Plot a line on display
///  @param[in] x1  Starting X coordinate
///  @param[in] y1  Startint Y coordinate
///  @param[in] x2  Ending X coordinate
///  @param[in] y2  Ending Y coordinate
///  @param[in] on  0 to clear, non zero to set
////////////////////////////////////////////////////////////////////////////////

void T6963_line( int x1, int y1, int x2, int y2, int on);


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_circle
///  @brief Plot a circle on display
///  @param[in] xc  X coordinate of center
///  @param[in] yc  Y coordinate of center
///  @param[in] R  Radius of circle
///  @param[in] on  0 to clear, non zero to set
////////////////////////////////////////////////////////////////////////////////

void T6963_circle(int xc, int yc, int R, int on);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
