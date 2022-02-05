#include "pm.h"
#include <stdint.h>
#include "sprites.h"

const uint8_t _rom tiles[] _at(0x015000) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t _rom borderL[] _at(0x15100) = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
};

const uint8_t _rom borderR[] _at(0x15200) = {
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

void drawBL() {
  #define BL1 OAM[0]
  BL1.x = 8;
  BL1.y = 16;
  BL1.tile = 5;
  BL1.ctrl = OAM_ENABLE|OAM_FLIPV;

  #define BL2 OAM[1]
  BL2.x = 8;
  BL2.y = 32;
  BL2.tile = 2;
  BL2.ctrl = OAM_ENABLE|OAM_FLIPH;

  #define BL3 OAM[2]
  BL3 = BL2;
  BL3.y = 48;

  #define BL4 OAM[3]
  BL4.x = 8;
  BL4.y = 64;
  BL4.tile = 5;
  BL4.ctrl = OAM_ENABLE;
}

void drawBR() {
  #define BL5 OAM[4]
  BL5.x = 8 * 13;
  BL5.y = 16;
  BL5.tile = 5;
  BL5.ctrl = OAM_ENABLE|OAM_FLIPH|OAM_FLIPV;

  #define BL6 OAM[5]
  BL6.x = 8 * 13;
  BL6.y = 32;
  BL6.tile = 2;
  BL6.ctrl = OAM_ENABLE;

  #define BL7 OAM[6]
  BL7 = BL6;
  BL7.y = 48;

  #define BL8 OAM[7]
  BL8.x = 8 * 13;
  BL8.y = 64;
  BL8.tile = 5;
  BL8.ctrl = OAM_ENABLE|OAM_FLIPH;
}

int main()
{
  uint8_t i;
  uint8_t tw = 16;
  uint8_t th = 12;

  PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
  PRC_RATE = RATE_36FPS;

  PRC_MAP = tiles;
  PRC_SPR = sprites1;

  // create blank tilemap
  for (i = 0; i < tw * th; i++) {
    if (i < 12 || i > 108) {
      TILEMAP[i] = 0xff;
    }
    else {
      TILEMAP[i] = 0x00;
    }
  }

  drawBL();
  drawBR();

  for(;;) {
    //drawAngry();
  }
}
