///////////////////////////////////////////////////////////////////////
///  VFD Itron CU20025 (BG Micro) demo
///  wrc Nov 19, 2020 (arduino)
/// wrc 20261101
///////////////////////////////////////////////////////////////////////

#include "gpio.h"
#include "vfd_cu20025.h"


void VFD_Command(uint8_t cmd)
{
  GPIO_write(RS_PIN, RS_CMD);
  uint8_t p = DATAPORT;
  p &= 0xf0;    // clear low bits
  p |= (cmd >> 4);  // set high bits of cmd
  DATAPORT = p;
  GPIO_write(E_PIN, HIGH);
  GPIO_write(E_PIN, LOW);
  // now low bits
  p = DATAPORT;
  p &= 0xf0;
  p |= (cmd & 0x0f);
  DATAPORT = p;
  GPIO_write(E_PIN, HIGH);
  GPIO_write(E_PIN, LOW);
  //delay(1);
  
}

void VFD_Data(uint8_t dat)
{
  digitalWrite(RS_PIN, RS_DATA);
  uint8_t p = DATAPORT;
  p &= 0xf0;    // clear low bits
  p |= (dat >> 4);  // set high bits of cmd
  DATAPORT = p;
  digitalWrite(E_PIN, HIGH);
  digitalWrite(E_PIN, LOW);
  // now low bits
  p = DATAPORT;
  p &= 0xf0;
  p |= (dat & 0x0f);
  DATAPORT = p;
  digitalWrite(E_PIN, HIGH);
  digitalWrite(E_PIN, LOW);
  delay(1);
}

void VFD_Write(uint8_t dat, int cd = RS_DATA)
{
  digitalWrite(RS_PIN, cd);
  uint8_t p = DATAPORT;
  p &= 0xf0;    // clear low bits
  p |= (dat >> 4);  // set high bits of cmd
  DATAPORT = p;
  digitalWrite(E_PIN, HIGH);
  digitalWrite(E_PIN, LOW);
  // now low bits
  p = DATAPORT;
  p &= 0xf0;
  p |= (dat & 0x0f);
  DATAPORT = p;
  digitalWrite(E_PIN, HIGH);
  digitalWrite(E_PIN, LOW);
  //delay(1);
}

void VFD_WriteCommand(uint8_t cmd)
{
  VFD_Write(cmd, RS_CMD);
}

void VFD_WriteData(uint8_t dat)
{
  VFD_Write(dat, RS_DATA);
}

void VFD_SetFourBitMode()
{
  uint8_t d = DATAPORT;
  d &= 0xf0;
  d |= 0x02;
  DATAPORT = d;
  GPIO_write(E_PIN, HIGH);
  //delay(1);
  GPIO_write(E_PIN, LOW);
}

void setup() 
{
  GPIO_pin_mode(RW_PIN, GPIO_PIN_MODE_OUTPUT);
  GPIO_write(RW_PIN, LOW);
  GPIO_pin_mode(RS_PIN, OUTPUT);
  GPIO_write(RS_PIN, LOW);
  GPIO_pin_mode(E_PIN, GPIO_PIN_MODE_OUTPUT);
  GPIO_write(E_PIN, LOW);
  uint8_t ddr = DDRB;     // get current settings
  ddr |= 0x0f;        // set data pins as outputs
  DDRB = ddr;

  // Init display
  // Set 4 bit mode
  //VFD_Command(0x20);
//  uint8_t d = DATAPORT;
//  d &= 0xf0;
//  d |= 0x02;
//  DATAPORT = d;
//  digitalWrite(E_PIN, HIGH);
//  delay(1);
//  digitalWrite(E_PIN, LOW);
  VFD_SetFourBitMode();
  
  // Set brightness to 100%
  VFD_WriteData(0x02);

// clear display
  VFD_WriteCommand(0x01);
  delay(5);

 // VFD_Data('H');
 // VFD_Data('i');
  for(uint8_t c = 128; c < 148; c++)
  {
    VFD_WriteData(c);
  }
  
  
  

}

void loop() 
{
  // put your main code here, to run repeatedly:

}