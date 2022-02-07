#include "pm.h"
#include <stdint.h>
//#include "sprites/sprites.h"
#include "sprites/scene1.h"

const uint8_t _rom tiles[] _at(0x015000) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void drawBL() {
  #define BL1 OAM[0]
  BL1.x = 8;
  BL1.y = 16;
  BL1.tile = 1;
  BL1.ctrl = OAM_ENABLE|OAM_FLIPV;

  #define BL2 OAM[1]
  BL2.x = 8;
  BL2.y = 32;
  BL2.tile = 0;
  BL2.ctrl = OAM_ENABLE|OAM_FLIPH;

  #define BL3 OAM[2]
  BL3 = BL2;
  BL3.y = 48;

  #define BL4 OAM[3]
  BL4.x = 8;
  BL4.y = 64;
  BL4.tile = 1;
  BL4.ctrl = OAM_ENABLE;
}

void drawBR() {
  #define BL5 OAM[4]
  BL5.x = 8 * 13;
  BL5.y = 16;
  BL5.tile = 1;
  BL5.ctrl = OAM_ENABLE|OAM_FLIPH|OAM_FLIPV;

  #define BL6 OAM[5]
  BL6.x = 8 * 13;
  BL6.y = 32;
  BL6.tile = 0;
  BL6.ctrl = OAM_ENABLE;

  #define BL7 OAM[6]
  BL7 = BL6;
  BL7.y = 48;

  #define BL8 OAM[7]
  BL8.x = 8 * 13;
  BL8.y = 64;
  BL8.tile = 1;
  BL8.ctrl = OAM_ENABLE|OAM_FLIPH;
}

void drawAngry() {
  #define A1 OAM[8]
  A1.x = 24;
  A1.y = 24;
  A1.tile = 0;
  A1.ctrl = OAM_ENABLE;

  #define A2 OAM[9]
  A2 = A1;
  A2.x = 40;
  A2.ctrl = A1.ctrl|OAM_FLIPH;

  #define A3 OAM[10]
  A3.x = A1.x;
  A3.y = 40;
  A3.tile = 3;
  A3.ctrl = OAM_ENABLE;

  #define A4 OAM[12]
  A4 = A3;
  A4.x = 40;
  A4.ctrl = A3.ctrl|OAM_FLIPH;
}

void drawCockedRevolver() {
  #define R1 OAM[13]
  R1.x = 56;
  R1.y = 36;
  R1.tile = 12;
  R1.ctrl = OAM_ENABLE;

  #define R2 OAM[14]
  R2 = R1;
  R2.x = R1.x + 16;
  R2.tile = 13;
}

void drawPC() {
  #define PC OAM[8]
  PC.x = 24;
  PC.y = 24;
  PC.tile = 4;
  PC.ctrl = OAM_ENABLE;
}

int main()
{
  uint8_t i;
  uint8_t tw = 16;
  uint8_t th = 12;

  PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
  PRC_RATE = RATE_36FPS;

  PRC_MAP = tiles;
  PRC_SPR = scene1;

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
  //drawAngry();
  //drawCockedRevolver();
  drawPC();

  for(;;) {
  }
}
