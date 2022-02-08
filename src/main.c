#include "pm.h"
#include <stdint.h>
//#include "sprites/sprites.h"
#include "sprites/scene1.h"

const uint8_t _rom tiles[] _at(0x015000) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void drawEntities() {
  #define PC OAM[23]
  PC.x = 24 - 4;
  PC.y = 24 - 4;
  PC.tile = PC_ID;
  PC.ctrl = OAM_ENABLE;

  #define E1 OAM[21]
  E1.x = 48;
  E1.y = 24;
  E1.tile = E1_ID;
  E1.ctrl = OAM_ENABLE;

  #define E2 OAM[19]
  E2.x = 64;
  E2.y = 24;
  E2.tile = E2_ID;
  E2.ctrl = OAM_ENABLE;

  #define SHOT OAM[17]
  E3.x = 160;
  E3.y = 24;
  E3.tile = SHOT_ID;
  E3.ctrl = OAM_ENABLE;
}

void drawItems() {
  #define HT OAM[0]
  HT.x = 160;
  HT.y = 0;
  HT.tile = HEART_ID;
  HT.ctrl = OAM_ENABLE;
}

void handleInput() {
  uint8_t speed = 2;

  if ((~KEY_PAD & KEY_DOWN) && PC.y < 60) {
    PC.y += speed;
  }
  else if ((~KEY_PAD & KEY_UP) && PC.y > 20) {
    PC.y -= speed;
  }
  else if ((~KEY_PAD & KEY_LEFT) && PC.x > 20) {
    PC.x -= speed;
  }
  else if ((~KEY_PAD & KEY_RIGHT) && PC.x < 92) {
    PC.x += speed;
  }
  else if ((~KEY_PAD & KEY_RIGHT & KEY_UP) && PC.y > 20 && PC.x < 92) {
    speed /= 2;
    PC.x += speed;
    PC.y -= speed;
  }
  else if ((~KEY_PAD & KEY_LEFT & KEY_UP) && PC.y > 20 && PC.x > 20) {
    speed /= 2;
    PC.x -= speed;
    PC.y -= speed;
  }
  else if ((~KEY_PAD & KEY_RIGHT & KEY_DOWN) && PC.y < 60 && PC.x < 92) {
    speed /= 2;
    PC.x += speed;
    PC.y += speed;
  }
  else if ((~KEY_PAD & KEY_RIGHT & KEY_UP) && PC.y > 20 && PC.x < 92) {
    speed /= 2;
    PC.x += speed;
    PC.y -= speed;
  }
}

int main()
{
  uint8_t i;
  uint8_t tw = 16;
  uint8_t th = 12;

  TMR1_OSC = 0x13;
  TMR1_SCALE = 0x08 | 0x02 | 0x80 | 0x20;
  TMR1_CTRL = 0x86;

  PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
  PRC_RATE = RATE_12FPS;

  PRC_MAP = tiles;
  PRC_SPR = scene1;

  // create blank tilemap
  for (i = 0; i < tw * th; i++) {
    if (i < 12 || i > 108 || i % tw == 0 || i == 27 || i == 43 || i == 59 || i == 75 || i == 91 || i == 107) {
      TILEMAP[i] = 0xff;
    }
    else if (i == 26 || i == 42 || i == 58 || i == 74 || i == 90 || i == 106) {
      TILEMAP[i] = 0x00;
    }
    else {
      TILEMAP[i] = 0x00;
    }
  }

  drawEntities();
  drawItems();

  for(;;) {
    wait_vsync();
    handleInput();
  }
}
