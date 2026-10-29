////////////////////////////////////////////////////////
/// @file T6963_lcd.ino
/// @copy Copyright (C) 2021 Will Cooke
/// @brief Controls 240x64 T6963 display using T6963 (DG-24064-09-S2RB)
////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


// Pin definitions
#define T6963_WR        2         // write
#define T6963_RD        3         // Read
#define T6963_CE       A5         // Chip Enable
#define T6963_CD       A4         // Command / /Data
#define T6963_RES      A3         // Reset
#define T6963_D0        4         // Data 0
#define T6963_D1        5         // Data 1
#define T6963_D2        6         // Data 2
#define T6963_D3        7         // Data 3
#define T6963_D4        8         // Data 4
#define T6963_D5        9         // Data 5
#define T6963_D6       10         // Data 6
#define T6963_D7       11         // Data 7
#define T6963_FONT     12         // Font Select LOW: 8x8, HIGH: 6x8

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




// Display Modes Set
//  1  0  0  1 N3 N2 N1 N0  // N3 = graphics, N2 = text, N1 = cursor, N0 = cursor blink
#define T6963_DISPLAY_GRAPHICS          (1 << 3)   // Turn on/off graphics mode
#define T6963_DISPLAY_TEXT              (1 << 2)   // Turn on/off text mode
#define T6963_DISPLAY_CURSOR            (1 << 1)   // Turn on/off cursor
#define T6963_DISPLAY_BLINK             (1 << 0)   // Turn on/off cursor blink

#define T6963_DISPLAY_MODE                0x90     // Generic display mode cmd, add modes
#define T6963_DISPLAY_OFF                 0x90     // Turn off display
#define T6963_CURSOR_ON_BLINK_OFF         0x92     // Turn on cursor, blink off
#define T6963_CURSOR_ON_BLINK_ON          0x93     // Turn cursor on, blink on
#define T6963_TEXT_ON_GRAPHIC_OFF         0x94     // Turn text on, graphic off
#define T6963_TEXT_OFF_GRAPHIC_ON         0x98     // Turn text off, graphic on
#define T6963_TEXT_ON_GRAPHIC_ON          0x9c     // Turn text on, graphic on


// Cursor Pattern Select
//  1  0  1  0  0 N2 N1 N0   N2-N0 Number lines for cursor +1
#define T6963_CURSOR_SIZE                 0xa0     // Set number lines for cursor
#define T6963_CURSOR_SIZE_1               0xa0     // Set 1 line cursor (bottom line)
#define T6963_CURSOR_SIZE_2               0xa1     // Set 2 line cursor
#define T6963_CURSOR_SIZE_3               0xa2     // Set 3 line cursor
#define T6963_CURSOR_SIZE_4               0xa3     // Set 4 line cursor
#define T6963_CURSOR_SIZE_5               0xa4     // Set 5 line cursor
#define T6963_CURSOR_SIZE_6               0xa5     // Set 6 line cursor
#define T6963_CURSOR_SIZE_7               0xa6     // Set 7 line cursor
#define T6963_CURSOR_SIZE_8               0xa7     // Set 8 line cursor (block)

// Data Auto Read/Write
//  1  0  1  1  0  0 N1 N0      N0 = Data Auto Read N1 = Auto Reset (inc address pntr)
#define T6963_AUTO_WRITE_SET              0xb0     // Set auto write mode
#define T6963_AUTO_READ_SET               0xb1     // Set auto read mode
#define T6963_AUTO_RESET                  0xb2     // Reset ???

// Data Read/Write
//  1  1  0  0  0 N2 N1 N0   N2 = no inc/dec addr pntr, N1 = Addr pntr down, N0 = Read
#define T6963_DATA_WRITE_INC              0xc0     // write data and increment
#define T6963_DATA_WRITE_DEC              0xc2     // write data and decrement
#define T6963_DATA_WRITE                  0xc4     // write and stay in place
#define T6963_DATA_READ_INC               0xc1     // read data and increment
#define T6963_DATA_READ_DEC               0xc3     // Read data and decrement
#define T6963_DATA_READ                   0xc5     // Read and stay in place

// Screen peek
//   1  1  1  0  0  0  0  0
#define T6963_SCREEN_PEEK                 0xe0     // peek screen at current address

// Screen copy
//   1  1  1  0  1  0  0  0                Copies one line of displayed data
#define T6963_SCREEN_COPY                 0xe8    // Copies one line of data

// Bit SET/RESET
//   1  1  1  1 N3 N2 N1 N0   N3 = SET, N2-N0 = bit
#define T6963_SET_RESET                   0xf0     // Set/reset - add codes
#define T6963_SET                         0xf8     // Set any bit (add bit #)
#define T6963_SET_0                       0xf8     // Set bit 0 
#define T6963_SET_1                       0xf9     // Set bit 1
#define T6963_SET_2                       0xfa     // Set bit 2
#define T6963_SET_3                       0xfb     // Set bit 3
#define T6963_SET_4                       0xfc     // Set bit 4
#define T6963_SET_5                       0xfd     // Set bit 5
#define T6963_SET_6                       0xfe     // Set bit 6
#define T6963_SET_7                       0xff     // Set bit 7
#define T6963_RESET                       0xf0     // Reset any bit (add bit #)
#define T6963_RESET_0                     0xf0     // Reset bit 0
#define T6963_RESET_1                     0xf1     // Reset bit 1
#define T6963_RESET_2                     0xf2     // Reset bit 2
#define T6963_RESET_3                     0xf3     // Reset bit 3
#define T6963_RESET_4                     0xf4     // Reset bit 4
#define T6963_RESET_5                     0xf5     // Reset bit 5
#define T6963_RESET_6                     0xf6     // Reset bit 6
#define T6963_RESET_7                     0xf7     // Reset bit 7

//


uint16_t T6963_setVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr = 0);
uint16_t T6963_setGraphicVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr = 0);


// TODO: We can simplify a lot of code by storing the pin #'s in an array
static int dataPins[8] = {4,5,6,7,8,9,10,11};

//  Store display text width in characters
static uint8_t displayWidth = 32;  // default startup with 8x8 font

// Store display text height in characters
static uint8_t displayHeight = 8;  // default startup

// Store graphics display width in characters
static uint8_t graphicDisplayWidth = 32;  // default startup with 8x8 font

// Store graphics display height in lines
static uint8_t graphicDisplayHeight = 64;

//  Store virtual text display width
static uint8_t displayVirtualWidth = 32;  // default startup with 8x8 font

// Store virtual text display height
static uint8_t displayVirtualHeight = 8;  // default startup

// Store virtual graphics display width
static uint8_t graphicDisplayVirtualWidth = 32;  // default startup with 8x8 font

// Store virtual graphics display height in lines
static uint8_t graphicDisplayVirtualHeight = 64;

// Store next text print column (0 to displayVirtualWidth-1)
static uint8_t printColumn = 0;

// Store next text print row (0 to displayVirtualHeight-1)
static uint8_t printRow = 0;

//  Keep track of current hardware cursor location
static uint8_t cursorX = 0;   //  0 to width-1
static uint8_t cursorY = 0;   //  0 height-1

// Start of text area
static uint16_t textHomeAddress = 0;

// Current address
static uint16_t ramPointer = 0;

// Where to start displaying
static uint16_t textDisplayRow = 0;
static uint16_t textDisplayCol = 0;
static uint16_t textDisplayAddress = 0;

// Start of graphic area
static uint16_t graphicHomeAddress = 0;

//  Track display mode (graphics, text, cursor, blink)
static uint8_t displayMode = 0;

// Track graphic mode (ROM/RAM CG, OR, XOR, AND)
static uint8_t graphicMode = 0;

// Track the character generator offset address
static uint8_t CG_offset = 0;

// Are we using 6 or 8 bit wide font?
static uint8_t fontSize = 8;

////////////////////////////////////////////////////////////////////////////////
///  @fn ports_init
///  @brief  Initializes all gpio ports for use
///  @return  True if initialized, false otherwise
////////////////////////////////////////////////////////////////////////////////
static bool ports_init()
{
  bool rtn = false;
  
  pinMode(T6963_CE, OUTPUT);
  digitalWrite(T6963_CE, HIGH);
  pinMode(T6963_WR, OUTPUT);
  digitalWrite(T6963_WR, HIGH);
  pinMode(T6963_RD, OUTPUT);
  digitalWrite(T6963_RD, HIGH);
  pinMode(T6963_CD, OUTPUT);

  pinMode(T6963_RES, OUTPUT);
  digitalWrite(T6963_RES, LOW);   // Reset while we're here
  pinMode(T6963_FONT, OUTPUT);
  digitalWrite(T6963_FONT, HIGH);  // 6x8 font
  fontSize = 6;
  
  //digitalWrite(T6963_FONT, LOW);   // 8x8 font
  // fontSize = 8;
  setDataDirection(OUTPUT);
  
  delay(5);
  digitalWrite(T6963_RES, HIGH);
  
  // set /ce to high
  // set data pins to input
  // set rd / wr to high
  // set c/d to command
  // set /res to high

  rtn = true;
  return rtn;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn setDataDirection
///  @brief  Sets Data Bus pins direction
///  @param[in]  INPUT, OUTPUT to choose direction
////////////////////////////////////////////////////////////////////////////////

static void setDataDirection(int dir)
{
//  pinMode(T6963_D0, dir);
//  pinMode(T6963_D1, dir);
//  pinMode(T6963_D2, dir);
//  pinMode(T6963_D3, dir);
//  pinMode(T6963_D4, dir);
//  pinMode(T6963_D5, dir);
//  pinMode(T6963_D6, dir);
//  pinMode(T6963_D7, dir);

  for(int i = 0; i < 8; i++)
  {
    pinMode(dataPins[i], dir);
  }
}


////////////////////////////////////////////////////////////////////////////////
///  @fn setDataBits
///  @brief Sets output data onto data bus pins
///  @param[in] d: Byte to set on bus pins
////////////////////////////////////////////////////////////////////////////////

static void setDataBits(uint8_t d)
{
//  digitalWrite(T6963_D0, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D1, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D2, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D3, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D4, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D5, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D6, d & 0x01);
//  d >>= 1;
//  digitalWrite(T6963_D7, d & 0x01);

  for(int i = 0; i < 8; i++)
  {
    digitalWrite(dataPins[i], d & 0x01);
    d >>= 1;
  }
}

////////////////////////////////////////////////////////////////////////////////
///  @fn getDataBits
///  @brief  Reads data bus pins
///  @return  Byte value read from pins
////////////////////////////////////////////////////////////////////////////////

static uint8_t getDataBits()
{
  uint8_t rtn = 0;
  
//  if(digitalRead(T6963_D7) == HIGH)
//  {
//    rtn |= (1 << 7);
//  }
//  if(digitalRead(T6963_D6) == HIGH)
//  {
//    rtn |= (1 << 6);
//  }
//  if(digitalRead(T6963_D5) == HIGH)
//  {
//    rtn |= (1 << 5);
//  }
//  if(digitalRead(T6963_D4) == HIGH)
//  {
//    rtn |= (1 << 4);
//  }
//  if(digitalRead(T6963_D3) == HIGH)
//  {
//    rtn |= (1 << 3);
//  }
//  if(digitalRead(T6963_D2) == HIGH)
//  {
//    rtn |= (1 << 2);
//  }
//  if(digitalRead(T6963_D1) == HIGH)
//  {
//    rtn |= (1 << 1);
//  }
//  if(digitalRead(T6963_D0) == HIGH)
//  {
//    rtn |= (1 << 0);
//  }

  for(int i = 0; i < 8; i++)
  {
    if(digitalRead(dataPins[i]) == HIGH)
    {
      rtn |= (1 << i);
    }
  }

  return rtn;
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_init
///  @brief  Initialize display for use
////////////////////////////////////////////////////////////////////////////////
static bool T6963_init()
{
  displayVirtualWidth = 40;
  displayWidth = 40;
  fontSize = 6;
  displayVirtualHeight = 8;
  displayHeight = 8;
  printColumn = 0;
  printRow = 0;
  cursorX = 0;
  cursorY = 0;
  textHomeAddress = 0;
  ramPointer = 0;
  textDisplayRow = 0;
  textDisplayCol = 0;
  graphicHomeAddress = 6144;  //(my display has 8K ram)
  displayMode = 0;  // no graphics, no text, no cursor, no blink
  graphicMode = 0;  // ROM cg, OR mode
  T6963_setVirtualSize(0, 32, 8, 1);
  T6963_setGraphicVirtualSize(256, 32, 64, 1);
  
  
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_getStatus
///  @brief  Retrieves status byte from T6963 controller
///  @return  T6963 status byte
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_getStatus()
{
  uint8_t rtn = 0;
  setDataDirection(INPUT);
  digitalWrite(T6963_CD, HIGH);
  digitalWrite(T6963_RD, LOW);
  digitalWrite(T6963_CE, LOW);
 // delay(1);
  rtn = getDataBits();
  digitalWrite(T6963_CE, HIGH);
  digitalWrite(T6963_RD, HIGH);
  setDataDirection(OUTPUT);
  return rtn;
  
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_getDataByte
///  @brief  Gets data byte from T6963 controller 
///  @return  Data byte read from controller
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_getDataByte()
{
  uint8_t rtn = 0;
  setDataDirection(INPUT);
  digitalWrite(T6963_CD, LOW);
  digitalWrite(T6963_RD, LOW);
  digitalWrite(T6963_CE, LOW);
 // delay(1);
  rtn = getDataBits();
  digitalWrite(T6963_CE, HIGH);
  digitalWrite(T6963_RD, HIGH);
  setDataDirection(OUTPUT);
  return rtn;
  
}

////////////////////////////////////////////////////////////////////////////////
///  @fn wait
///  @brief   Waits for STATUS0 and STATUS1 to both indicate ready
////////////////////////////////////////////////////////////////////////////////
static void wait()
{
  while(T6963_getStatus() & 0x03 != 3);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn waitAuto
///  @brief   Waits for STATUS2 or STATUS3 to either indicate ready
////////////////////////////////////////////////////////////////////////////////
static void waitAuto()
{
  while(T6963_getStatus() & 0x0c == 0);  // wait while neither bit set
}

////////////////////////////////////////////////////////////////////////////////
///  @fn waitAutoRead
///  @brief  Waits until auto read capable bit is set
////////////////////////////////////////////////////////////////////////////////
static void waitAutoRead()
{
  while(T6963_getStatus() & 0x04 != 4);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn waitAutoWrite
///  @brief Waits until auto write capable bit is set
////////////////////////////////////////////////////////////////////////////////
static void waitAutoWrite()
{
  while(T6963_getStatus() & 0x08 != 8);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_writeDataByte
///  @brief  Send a byte of data to controller
///  @param[in]  dat The data byte to send
////////////////////////////////////////////////////////////////////////////////
void T6963_writeDataByte(uint8_t dat)
{
  wait();
  setDataDirection(OUTPUT);
  setDataBits(dat);
  digitalWrite(T6963_CD, LOW);
  digitalWrite(T6963_WR, LOW);
  digitalWrite(T6963_CE, LOW);
 // delay(1);
  digitalWrite(T6963_CE, HIGH);  // min pulse width 80 nS
  digitalWrite(T6963_WR, HIGH);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn writeCommandByte
///  @brief  Sends a command byte to controller. Send parameters prior to cmd.
///  @param[in] cmd The command byte to send
////////////////////////////////////////////////////////////////////////////////
void T6963_writeCommandByte(uint8_t cmd)
{
  wait();
  setDataDirection(OUTPUT);
  setDataBits(cmd);
  digitalWrite(T6963_CD, HIGH);
  digitalWrite(T6963_WR, LOW);
  digitalWrite(T6963_CE, LOW);
 // delay(1);
  digitalWrite(T6963_CE, HIGH);  // min pulse width 80 nS
  digitalWrite(T6963_WR, HIGH);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setCursor
///  @brief  Set cursor location
///  @param[in] x The horizontal coordinate (0 to 127)
///  @param[in] y The vertical coordinate (0 to 63)
///  @return  Zero on success, -1 if out of bounds
////////////////////////////////////////////////////////////////////////////////
int T6963_setCursor(int x, int y)
{
  int rtn = 0;
  if(x < 0 || x > 127 || y < 0 || y > 63)
  {
    rtn = -1;
  }
  else
  {
    T6963_writeDataByte(x);
    T6963_writeDataByte(y);
    T6963_writeCommandByte(T6963_SET_CURSOR_POINTER);
    cursorX = x;
    cursorY = y;
  }
  return rtn;
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setOffsetPointer
///  @brief  Sets high five bits of char gen RAM location (2K granularity)
///  @param[in]  offs  The offset page (0 to 31)
///  @return  Zero on success, -1 if out of bounds
////////////////////////////////////////////////////////////////////////////////
int T6963_setOffsetPointer(uint8_t offs)
{
  int rtn = 0;
  if(offs > 31)
  {
    rtn = -1;
  }
  else
  {
    T6963_writeDataByte(offs);
    T6963_writeDataByte(0);
    T6963_writeCommandByte(T6963_SET_OFFSET_REGISTER);
    CG_offset = offs;
  }
  return rtn;
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAddress
///  @brief  Sets controller RAM address for reads and writes
///  @param[in] addr  The RAM address to set (0 to 65535) (8191 on DG24064)
////////////////////////////////////////////////////////////////////////////////
void T6963_setAddress(uint16_t addr)
{
  T6963_writeDataByte(addr & 0xff); // low byte
  T6963_writeDataByte( (addr >> 8) & 0xff);  // high byte
  T6963_writeCommandByte(T6963_SET_ADDRESS_POINTER);
  ramPointer = addr;
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setTextHomeAddress
///  @brief  Sets RAM address used as top left text displayed text character
///  @param[in] addr The RAM address to use for top left character (0 - 65535)
////////////////////////////////////////////////////////////////////////////////
void T6963_setTextHomeAddress(uint16_t addr)
{
  T6963_writeDataByte(addr & 0xff);
  T6963_writeDataByte( (addr >> 8) & 0xff);
  T6963_writeCommandByte(T6963_SET_TEXT_HOME_ADDRESS);
  textDisplayAddress = addr;
  textHomeAddress = addr;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setGraphicHomeAddress
///  @brief Sets RAM address of top left graphics byte or text attribute byte
///  @param[in] addr The RAM address for graphics or attributes (0 - 65535)
////////////////////////////////////////////////////////////////////////////////
void T6963_setGraphicHomeAddress(uint16_t addr)
{
  T6963_writeDataByte(addr & 0xff);
  T6963_writeDataByte( (addr >> 8) & 0xff);
  T6963_writeCommandByte(T6963_SET_GRAPHIC_HOME_ADDRESS);
  graphicHomeAddress = addr;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setTextArea
///  @brief  Define number of columns for text area of RAM, independent of h/w
///  @param[in] cols  Number of text columns
////////////////////////////////////////////////////////////////////////////////
void T6963_setTextArea(uint8_t cols)
{
  T6963_writeDataByte(cols);
  T6963_writeDataByte(0);
  T6963_writeCommandByte(T6963_SET_TEXT_AREA);
  displayVirtualWidth = cols;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setGraphicArea
///  @brief Define number of (text) columns for graphic area, independent of h/w
///  @param[in] cols Number of text columns
////////////////////////////////////////////////////////////////////////////////
void T6963_setGraphicArea(uint8_t cols)
{
  T6963_writeDataByte(cols);
  T6963_writeDataByte(0);
  T6963_writeCommandByte(T6963_SET_GRAPHIC_AREA);
  graphicDisplayVirtualWidth = cols;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setOrMode
///  @brief Set mode to OR text and graphic data and choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setOrMode(uint8_t CG = 0)
{
  if(CG != 0)
  {
    CG = T6963_MODE_RAM_CG;
  }
  T6963_writeCommandByte(T6963_SET_OR_MODE | CG);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setXorMode
///  @brief Set mode to XOR text and graphic data, choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setXorMode(uint8_t CG = 0)
{
  if(CG != 0)
  {
    CG = T6963_MODE_RAM_CG;
  }
  T6963_writeCommandByte(T6963_SET_EXOR_MODE | CG);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAndMode
///  @brief Set mode to AND text and graphic data, choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setAndMode(uint8_t CG = 0)
{
  if(CG != 0)
  {
    CG = T6963_MODE_RAM_CG;
  }
  T6963_writeCommandByte(T6963_SET_AND_MODE | CG);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setTextAttributeMode
///  @brief Set text attribute mode and choose character generator
///  @param[in] CG choose 0 (default) for ROM, non-zero for RAM char gen
////////////////////////////////////////////////////////////////////////////////
void T6963_setTextAttributeMode(uint8_t CG = 0)
{
  if(CG != 0)
  {
    CG = T6963_MODE_RAM_CG;
  }
  T6963_writeCommandByte(T6963_SET_TEXT_ATTRIBUTE_MODE | CG);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setDisplayMode
///  @brief  Choose to display or not, text, graphics, cursor, blink
///  @param[in] txt Set non-zero to display text.
///  @param[in] grph Set non-zero to display graphics.
///  @param[in] curs Set non-zero to display the cursor.
///  @param[in] blnk Set non-zero to make the cursor blink.
////////////////////////////////////////////////////////////////////////////////
void T6963_setDisplayMode(uint8_t txt = 0, uint8_t grph = 0, uint8_t curs = 0, uint8_t blnk = 0)
{
  uint8_t cmd = T6963_DISPLAY_MODE;
  if(txt != 0)
  {
    cmd |= T6963_DISPLAY_TEXT;
  }
  if(grph != 0)
  {
    cmd |= T6963_DISPLAY_GRAPHICS;
  }
  if(curs != 0)
  {
    cmd |= T6963_DISPLAY_CURSOR;
  }
  if(blnk != 0)
  {
    cmd |= T6963_DISPLAY_BLINK;
  }
  T6963_writeCommandByte(cmd);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setCursorSize
///  @brief Select size of cursor from 1 (bottom only) to 8 (full block)
///  @param[in] siz Number of lines to use for cursor.
////////////////////////////////////////////////////////////////////////////////
void T6963_setCursorSize(uint8_t siz)
{
  if(siz > 8)
  {
    siz = 8;
  }
  if(siz < 1)
  {
    siz = 1;
  }
  siz--;  // Convert 1-8 to 0-7
  T6963_writeCommandByte(T6963_CURSOR_SIZE  | siz );
}

#define T6963_AUTO_WRITE_SET              0xb0     // Set auto write mode
#define T6963_AUTO_READ_SET               0xb1     // Set auto read mode
#define T6963_AUTO_RESET                  0xb2     // Reset ???

// After one of these commands, only command is  RESET
// Set Address Pointer first

// From manual
//  Auto Read                  Auto Write
//  set addr pntr              set addr pntr
//  status check  0,1          status check  0,1
//  send auto read             send auto write
//  status check 2             status check 3
//  data read                  data write

//  repeat                     repeat
//  status check 2             status check 2
//  data read                  data write

//  status check 2             status check 2
//  auto reset                 auto reset

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAutoWrite
///  @brief  Begins the autowrite mode.  Stays in that mode until autoreset.
////////////////////////////////////////////////////////////////////////////////
void T6963_setAutoWrite()
{
  T6963_writeCommandByte(T6963_AUTO_WRITE_SET);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAutoRead
///  @brief Begins the autoread mode.  Stays in that mode until autoreset.
////////////////////////////////////////////////////////////////////////////////
void T6963_setAutoRead()
{
  T6963_writeCommandByte(T6963_AUTO_READ_SET);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setAutoReset
///  @brief  Ends autoread or autowrite mode.
////////////////////////////////////////////////////////////////////////////////
void T6963_setAutoReset()
{
  T6963_writeCommandByte(T6963_AUTO_RESET);
}

#define T6963_DATA_WRITE_INC              0xc0     // write data and increment
#define T6963_DATA_WRITE_DEC              0xc2     // write data and decrement
#define T6963_DATA_WRITE                  0xc4     // write and stay in place
#define T6963_DATA_READ_INC               0xc1     // read data and increment
#define T6963_DATA_READ_DEC               0xc3     // Read data and decrement
#define T6963_DATA_READ                   0xc5     // Read and stay in place

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataWriteIncrement
///  @brief  Write data at current address, increment address
///  @param[in] dat The data to write to RAM
////////////////////////////////////////////////////////////////////////////////
void T6963_dataWriteIncrement(uint8_t dat)
{
  wait();
  T6963_writeDataByte(dat);
  wait();
  T6963_writeCommandByte(T6963_DATA_WRITE_INC);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataWriteDecrement
///  @brief Write data at current address, decrement address
///  @param[in] dat The data to write to RAM
////////////////////////////////////////////////////////////////////////////////
void T6963_dataWriteDecrement(uint8_t dat)
{
  wait();
  T6963_writeDataByte(dat);
  wait();
  T6963_writeCommandByte(T6963_DATA_WRITE_DEC);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataWrite
///  @brief  Write data at current address, leave address as is.
///  @param[in] dat The data to write to RAM
///  @return
////////////////////////////////////////////////////////////////////////////////
void T6963_dataWrite(uint8_t dat)
{
  wait();
  T6963_writeDataByte(dat);
  wait();
  T6963_writeCommandByte(T6963_DATA_WRITE_INC);
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataReadIncrement
///  @brief  Read data from current address, increment address
///  @return  The byte read from RAM
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_dataReadIncrement()
{
  uint8_t rtn = 0;
  wait();
  T6963_writeCommandByte(T6963_DATA_READ_INC);
  wait();
 // TODO rtn =T6963_readDataByte();
  return rtn;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataReadDecrement
///  @brief Read Data from current address, decrement address
///  @return The byte read from RAM
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_dataReadDecrement()
{
  uint8_t rtn = 0;
  wait();
  T6963_writeCommandByte(T6963_DATA_READ_DEC);
  wait();
 // TODO rtn =T6963_readDataByte();
  return rtn;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_dataRead
///  @brief Read byte from current address, leave address as is.
///  @return The byte read from RAM
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_dataRead()
{
  uint8_t rtn = 0;
  wait();
  T6963_writeCommandByte(T6963_DATA_READ);
  wait();
 // TODO rtn =T6963_readDataByte();
  return rtn;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_screenPeek
///  @brief  Read display byte from screen, using address pointer to graphics.
///  @return Byte read from screen.
////////////////////////////////////////////////////////////////////////////////
uint8_t T6963_screenPeek()
{
  uint8_t rtn = 0;
  wait();
  T6963_writeCommandByte(T6963_SCREEN_PEEK);
  wait();
 // TODO rtn =T6963_readDataByte();
  return rtn;
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_screenCopy
///  @brief Copy one row of screen to graphics area
////////////////////////////////////////////////////////////////////////////////
void T6963_screenCopy()
{
  wait();
  T6963_writeCommandByte(T6963_SCREEN_COPY);
  wait();
 // TODO rtn =T6963_readDataByte();
}


#define T6963_SET_RESET                   0xf0     // Set/reset - add codes
#define T6963_SET                         0xf8     // Set any bit (add bit #)

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setBit
///  @brief  Set a single bit at location in address pointer.
///  @param[in] b Bit number to set in byte (0 to 7)
////////////////////////////////////////////////////////////////////////////////
void T6963_setBit(uint8_t b)
{
  if(b < 8)
  {
    wait();
    T6963_writeCommandByte(T6963_SET | b);
  }
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_resetBit
///  @brief  Clear a single bit at location in addrss pointer
///  @param[in] b Bit number to clear in byte (0 to 7)
///  @return
////////////////////////////////////////////////////////////////////////////////
void T6963_resetBit(uint8_t b)
{
  if(b < 8)
  {
    wait();
    T6963_writeCommandByte(T6963_RESET | b);
  }
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setVirtualSize
///  @brief  Sets size of virtual text screen
///  @param[in] address Address in display memory to place text buffer.
///  @param[in] width Width in columns of virtual area (32 to 80).
///  @param[in] height Height in rows of virtual area (8 to 2048).
///  @param[in] clr  Non zero to clear the new virtual screen.
///  @return  Next available address after this area
////////////////////////////////////////////////////////////////////////////////
uint16_t T6963_setVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr = 0)
{
  uint16_t rtn = addr;
  if(width >= 32 && width <= 80 && height >= 8 && height <= 2048)
  {
    textHomeAddress = addr;
    T6963_setTextHomeAddress(addr);
    displayVirtualWidth = width;
    displayVirtualHeight = height;
    rtn = width* height + addr;
    
    if(clr != 0)
    {
      T6963_textClear();
    }
  }
  return rtn;
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_printChar
///  @brief  Print a single ASCII character to LCD
///  @param[in] c Character to print
////////////////////////////////////////////////////////////////////////////////
void T6963_printChar(char c)
{
  static uint8_t  escaped = 0;
  // TODO: make this more sophisticated
  switch(c)
  {
    case 8:  // backspace
      if(printColumn != 0)
      {
        printColumn--;
        T6963_setAddress(textHomeAddress + printRow * displayVirtualWidth + printColumn);
      }
      break;
    case 9:
      // tab
      break;
    case 10:  // line feed
      printRow++;
      T6963_setAddress(textHomeAddress + printRow * displayVirtualWidth + printColumn);
      break;
    case 12:
      // ff
      break;
    case 13:  // carriage return
      printColumn = 0;
      T6963_setAddress(textHomeAddress + printRow * displayVirtualWidth + printColumn);
      break;
    case 27:
      escaped = 1;
      break;
    default:
      c -= 32;
      T6963_dataWriteIncrement(c);
      printColumn++;
      if(printColumn == displayVirtualWidth)
      {
        printColumn = 0;
        printRow++;
        if(printRow == displayVirtualHeight)
        {
          printRow = 0;
        }
      }
  }

}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_printString
///  @brief  Print a null terminated string to LCD
///  @param[in] str Pointer to the string to print
////////////////////////////////////////////////////////////////////////////////
void T6963_printString(char* str)
{
  if(str != NULL)
  {
    char ch;
    while( (ch = *str) != 0)
    {
      T6963_printChar(ch);
      str++;
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_textClear
///  @brief  Clear all text from screen
////////////////////////////////////////////////////////////////////////////////
static void T6963_textClear()
{
  // set address
  T6963_setAddress(textHomeAddress);
  T6963_setAutoWrite();
  int bytes = displayVirtualWidth * displayVirtualHeight;
  for(int cnt = 0; cnt < bytes; cnt++)
  {
    T6963_writeDataByte(0); // 0 is the space character
  }
  T6963_setAutoReset();
  // set address to base
  T6963_setAddress(textHomeAddress);
  cursorX = 0;
  cursorY = 0;
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_graphicClear
///  @brief  Clear all graphics from screen
////////////////////////////////////////////////////////////////////////////////
static void T6963_graphicClear()
{
  T6963_setAddress(graphicHomeAddress);
  T6963_setAutoWrite();
  int bytes = graphicDisplayVirtualWidth * graphicDisplayVirtualHeight; 
  for(int cnt = 0; cnt < bytes; cnt++)
  {
    T6963_writeDataByte(0); // clear all bits
  }
  T6963_setAutoReset();
  T6963_setAddress(textHomeAddress);
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_clear
///  @brief  Clear all text and graphic data from screen
////////////////////////////////////////////////////////////////////////////////
static void T6963_clear()
{
  T6963_textClear();
  T6963_graphicClear();
}

////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_gotoXY
///  @brief  Move cursor to column (0 to width-1), row (0 to height-1) 
////////////////////////////////////////////////////////////////////////////////
static void T6963_gotoXY(uint8_t x, uint8_t y)
{
  if(x < displayVirtualWidth && y < displayVirtualHeight)
  {
    // TODO modify for virtual screen
    wait();
    T6963_writeDataByte(x);
    cursorX = x;
    wait();
    T6963_writeDataByte(y);
    cursorY = y;
    wait();
    T6963_writeCommandByte(T6963_SET_CURSOR_POINTER);
    T6963_setAddress(textHomeAddress + y * displayVirtualWidth + x);
  }
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_setGraphicVirtualSize
///  @brief  Sets size of virtual graphic screen
///  @param[in] address Address in display memory to place graphic buffer.
///  @param[in] width Width in columns of virtual area (32 to 80).
///  @param[in] height Height in graphic lines of virtual area (64 to 256).
///  @param[in] clr  Non zero to clear the new virtual screen.
///  @return Next available address after this area.
////////////////////////////////////////////////////////////////////////////////
uint16_t T6963_setGraphicVirtualSize(uint16_t addr, uint16_t width, uint16_t height, int clr = 0)
{
  uint16_t rtn = addr;
  if(width >= 32 && width <= 80 && height >= 64 && height <= 256)
  {
    graphicHomeAddress = addr;
    T6963_setGraphicHomeAddress(addr);
    graphicDisplayVirtualWidth = width;
    graphicDisplayVirtualHeight = height;
    rtn = width * height + addr;
    if(clr != 0)
    {
      T6963_graphicClear();
    }
  }
  return rtn;
}
////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_plot
///  @brief Plot a point on display
///  @param[in] x  X coordinate 
///  @param[in] y  Y coordinate
///  @param[in] on  0 to clear, non zero to set
////////////////////////////////////////////////////////////////////////////////

void T6963_plot( int x, int y, int on = 1)
{
  // The graphics bits are displayed with the number of bytes same as number of 
  // characters displayed.  If the font is 6x8 then there are 40 characters and 
  // 40 bytes width.  For 8x8 font, there are 32 characters and 32 bytes.
  // The number of bits used from each byte is the same as the font width.  So 
  // the 6x8 font uses the least significant 6 bits (0 to 5) of the byte and the 
  // 8x8 font uses all 8.  The most significant bit is to the left.
  int addr = graphicHomeAddress;
  int oldRamPointer = ramPointer;
  int bit = (fontSize-1) - (x % fontSize);  // bit #s are reversed
  addr += y * graphicDisplayVirtualWidth + x / fontSize;
  if(addr != ramPointer)
  {
    T6963_setAddress(addr);
  }
  if(on)
  {
    T6963_setBit(bit);
  }
  else
  {
    T6963_resetBit(bit);
  }
  T6963_setAddress(oldRamPointer);
  
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_line
///  @brief Plot a line on display
///  @param[in] x1  Starting X coordinate
///  @param[in] y1  Startint Y coordinate
///  @param[in] x2  Ending X coordinate
///  @param[in] y2  Ending Y coordinate
///  @param[in] on  0 to clear, non zero to set
////////////////////////////////////////////////////////////////////////////////

void T6963_line( int x1, int y1, int x2, int y2, int on)
{
  int temp;
  // Get delta x to the right (positive)
  if(x2 < x1)
  {
    temp = x2;
    x2 = x1;
    x1 = temp;
    temp = y2;
    y2 = y1;
    y1 = temp;
  }

  int deltaX = x2-x1;
  int deltaY = y2-y1;
  if(abs(deltaY) > abs(deltaX))  // deltaX must be positive here
  {
    // increment y
    if(deltaY >= 0)
    {
      unsigned int inc = 256 * deltaX / deltaY;
      unsigned int x = (x1 * 256); // + 128;
      for(int y = y1; y <= y2; y++)
      {
        T6963_plot(x / 256, y, on);
        x += inc;
      }
    }
    else // deltaY negative
    {
      unsigned int inc = 256 * deltaX / deltaY;
    }
    
  }
  else
  {
    if(deltaY >= 0)
    {
      int inc = 256 * deltaY / deltaX ;
      int y = (y1 * 256); //  + 128;
      for(int x = x1; x <= x2; x++)
      {
        T6963_plot(x,y / 256, on);
        y += inc;
      }
    }
    else  // deltaY negative
    {
      int inc = 256 * deltaY / deltaX;
      int y = (y1 * 256 );// - 127;
      for(int x = x1; x <= x2; x++)
      {
        T6963_plot(x, y / 256, on);
        y += inc;
      }
    }
    
  }
  
}


////////////////////////////////////////////////////////////////////////////////
///  @fn T6963_circle
///  @brief Plot a circle on display
///  @param[in] xc  X coordinate of center
///  @param[in] yc  Y coordinate of center
///  @param[in] R  Radius of circle
///  @param[in] on  0 to clear, non zero to set
////////////////////////////////////////////////////////////////////////////////

void T6963_circle(int xc, int yc, int R, int on = 1)
{
  int limit = 94 * R / 128;   // Go .707 of radius
  int R2 = R*R;
  
  for (int x = 0; x < limit; x++)
  {
    int y2 = R2 - x*x;
    int y = sqrt(y2);
    T6963_plot(xc+x, yc+y, on);
    T6963_plot(xc-x, yc+y, on);
    T6963_plot(xc+x, yc-y, on);
    T6963_plot(xc-x, yc-y, on);
    T6963_plot(xc+y, yc+x, on);
    T6963_plot(xc+y, yc-x, on);
    T6963_plot(xc-y, yc+x, on);
    T6963_plot(xc-y, yc-x, on);
  }
}

////////////////////////////////////////////////////////////////////////////////
///  @fn setup
///  @brief
///  @param[]
///  @return
////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  pinMode(13, OUTPUT);

  delay(100);
  ports_init();
  T6963_setAddress(0);
  T6963_setCursor(0,0);
  T6963_setTextHomeAddress(0);
  T6963_setGraphicHomeAddress(2000);
  T6963_setDisplayMode(1,1,1,1);  // text, graphics, cursor, blink
//  T6963_setDisplayMode(1,0,1,0);
  T6963_setAddress(0);
  T6963_setTextArea(40);
  T6963_setGraphicArea(40);
  T6963_setOrMode(0);
  T6963_textClear();
  T6963_graphicClear();



  delay(500);
  
//  
//  delay(100);
//  for(int i = 0; i < 800; i++)
//  {
//    T6963_dataWriteIncrement(0);
//  }
//  T6963_setAddress(0);
//  T6963_setAutoWrite();
//  for(int i = 2000; i < 4000; i++)
//  {
//    T6963_writeDataByte(0x55);
//  }
//  T6963_setAutoReset();
  

  T6963_setAddress(0);

  T6963_textClear();

 // draw some points
 T6963_clear();


  // Try some lines

  for(int y = 0; y < 64; y++)
  {
    T6963_line(220, 32, 239, y, 1);
  }

  T6963_setXorMode();

  T6963_clear();
  delay(1000);
  T6963_setTextArea(64);
  T6963_setAddress(0);


  T6963_printString("1: Hello World.  I am here now.\r\n");
  T6963_printString("2: Would you like to meet me?  \r\n");
  T6963_printString("3: This is the second line of the strings.  It will be on some line that may or may \r\n");
  T6963_printString("5: be on the second (now third) line.  But we will just have to wait and see.");
  T6963_printString("Might as well keep going.  Let's see what happens at the bottom.  ");
  T6963_printString("It 'should' start over at the top.  But I want to check that.  ");
  T6963_printString("They call that testing.  ");
  T6963_printChar('X');
  T6963_printChar('Y');
  T6963_printChar('Z');

  T6963_setCursor(3,3);
  

}

////////////////////////////////////////////////////////////////////////////////
///  @fn loop
///  @brief
////////////////////////////////////////////////////////////////////////////////

void loop() 
{
  // put your main code here, to run repeatedly:

}
