# T6963_lcd
Drivers for Toshiba T6963 LCD display controller.

bool T6963_init()
uint8_t T6963_getStatus()
  get the status byte from display

uint8_t T6963_getDataByte()
  return a data byte from display

void T6963_writeDataByte(uint8_t dat)
  write a byte of data to display

void T6963_writeCommandByte(uint8_t cmd)
  write a command byte to display

void T6963_setCursor(int x, int y)
  Set dispaly cursor to column and row given

int T6963_setOffsetPointer(uint8_t offs)
  A 5 bit # giving the 2K page to use for RAM character generator

void T6963_setAddress(uint16_t addr)
  This sets the address in RAM to write to/read from

void T6963_setTextHomeAddress(uint16_t addr)
  Sets RAM address used as top left displayed text character

void T6963_setGraphicHomeAddress(uint16_t addr)
  Sets RAM address used as top left graphics byte or text attribute

void T6963_setTextArea(uint8_t cols)
  Width of virtual display, can be wider than physical

void T6963_setGraphicArea(uint8_t cols)
  Width of graphic virtual display, can be wider than physical

void T6963_setOrMode(uint8_t CG = 0)
  Sets graphics to OR mode: OR graphic dot with text, CG ROM(0) or RAM(1)

void T6963_setXorMode(uint8_t CG = 0)
  Sets graphics to XOR mode: XOR graphics and text, CG ROM(0) or RAM(1)

void T6963_setAndMode(uint8_t CG = 0)
  Sets graphics to AND mode: AND graphics and text, CG ROM(0) or RAM(1)

void T6963_setTextAttributeMode(uint8_t CG = 0)
  Sets graphics mode to Text attributes

void T6963_setDisplayMode(uint8_t txt=0 uint8_t grph=0 uint8_t curs=0 uint8_t b=0)
  Selects text, graphics, cursor, blink modes.

void T6963_setCursorSize(uint8_t siz)
  Sets number of lines for cursor, 1 to 8.  Lines start at bottom.

void T6963_setAutoWrite()
  Begins the autowrite mode.  All data after gets automatically written, addr++

void T6963_setAutoRead()
  Begins autoread mode.  All reads return next byte after.

void T6963_setAutoReset()
  End the autowrite or autoread mode.

void T6963_dataWriteIncrement(uint8_t dat)
  Write dat to address and increment address.

void T6963_dataWriteDecrement(uint8_t dat)
  Write dat to address and decrement address.

void T6963_dataWrite(uint8_t dat)
  Write dat to address.  Don't change address.

uint8_t T6963_dataReadIncrement()
  Return byte from address and increment address.

uint8_t T6963_dataReadDecrement()
  Return byte from address and decrement address.

uint8_t T6963_dataRead()
  Return byte from address.  Don't change address.

uint8_t T6963_screenPeek()
  Return actual displayed byte from screen at address.

void T6963_screenCopy()
  Copy one row of screen to graphics area.

void T6963_setBit(uint8_t b)
  Set bit b of byte at address.

void T6963_resetBit(uint8_t b)
  Reset bit b of byte at address.

void T6963_printChar(char c)
  Print c to screen using normal ascii codes (eg cr, lf, ff, etc)

void T6963_printString(char* str)
  Print a null terminated string using the printChar function above.

void T6963_clear()
  Clear all text and data from the screen.

void T6963_textClear()
  Clear all text from the screen.

void T6963_graphicsClear()
  Clear all graphics from the screen.

void T6963_gotoXY(uint8_t x, uint8_t y)
  Move address (and cursor) to column x, row y (0 based)

