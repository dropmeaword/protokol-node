//static int boardIdBaseAddress = 0;
//
////byte MARK[4];
//byte GUID[8];

#include <Arduino.h>
#include <EEPROM.h>
#include "guid.h"

int boardIdBaseAddress = 0;
byte GUID[8];

unsigned long longRandom()
{
 analogReference(INTERNAL);
 unsigned long rv = 0;
 for (byte i=0; i< 32; i++) rv |= (analogRead(8) & 1L) << i;
 return rv;
}


void guid_store() {
  for(int i = 4; i < 7; i++) {
    EEPROM.write(boardIdBaseAddress + i, GUID[i]); 
  }
}

void guid_clear() {
  for(int i = 0; i < 8; i++) {
    GUID[i] = 0xFF;
    EEPROM.write(boardIdBaseAddress + i, GUID[i]); 
  }
}

void guid_set(unsigned long muid) {
  GUID[4] = ((muid >> 24) & 0xFF);
  GUID[5] = ((muid >> 16) & 0xFF);
  GUID[6] = ((muid >> 8) & 0xFF);
  GUID[7] = (muid & 0xFF);
  guid_store();
}

unsigned long guid_init()
{
  int i;
  
  // read ID marker (4 bytes)
  for(i = 0; i < 4; i++) {
    GUID[i] = EEPROM.read(boardIdBaseAddress + i);
  }

  // read GUID (4 bytes)
  for(i = 4; i < 8; i++) {
    GUID[i] = EEPROM.read(boardIdBaseAddress + i);
  }

  if( (GUID[0] != 'G') || (GUID[1] != 'U') || (GUID[2] != 'I') || (GUID[3] != 'D') )
  {
    GUID[0] = 'G';
    GUID[1] = 'U';
    GUID[2] = 'I';
    GUID[3] = 'D';

    for(int i = 0; i < 4; i++) {
      EEPROM.write(boardIdBaseAddress + i, GUID[i]);
    }

    // generate GUID
    randomSeed( analogRead(8) );
    GUID[4] = random(256);
    GUID[5] = random(256);
    GUID[6] = random(256);
    GUID[7] = random(256);

    //unsigned long rnd = longRandom();
    //guid_set( rnd );
    guid_store();
  }

  long tmp0, tmp1, tmp2, tmp3;
  tmp0 = GUID[0];
  tmp1 = GUID[1];
  tmp2 = GUID[2];
  tmp3 = GUID[3];
  
  return ((tmp0 << 24) | (tmp1 << 16) | (tmp2 << 8) | (tmp3)); 
}

unsigned long guid_randomize() {
  guid_clear();
  return guid_init();
}

