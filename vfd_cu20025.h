///////////////////////////////////////////////////////////////////////
///  VFD Itron CU20025 (BG Micro) demo
///  wrc Nov 19, 2020 (arduino)
/// wrc 20261101
///////////////////////////////////////////////////////////////////////

#ifndef _VFD_CU20025_H_
#define _VFD_CU20025_H_

#include <stdint.h>

#include "device_config.h"



void VFD_Command(uint8_t cmd);


void VFD_Data(uint8_t dat);


void VFD_Write(uint8_t dat, int cd);


void VFD_WriteCommand(uint8_t cmd);


void VFD_WriteData(uint8_t dat);


void VFD_SetFourBitMode();


void setup();


#endif

