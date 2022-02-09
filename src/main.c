#include "pm.h"
#include <stdint.h>
//#include "sprites/sprites.h"
#include "sprites/scene1.h"

const uint8_t _rom tiles[] _at(0x015000) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void drawEntities() {
  #define PC OAM[22]
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

  #define SHOT OAM[23]
  SHOT.x = 96;
  SHOT.y = 0;
  SHOT.tile = SHOT_ID;
  SHOT.ctrl = !OAM_ENABLE;
}

void drawItems() {
  #define HT OAM[0]
  HT.x = 96;
  HT.y = 0;
  HT.tile = HEART_ID;
  HT.ctrl = OAM_ENABLE;
}

void pcShoot(d, bL, bR, bT, bB, s) {
  uint8_t yP = 0;
  uint8_t xP = 0;
  uint8_t bP = 4;
  //uint8_t i;
  //uint8_t sTime = 3;

  if (d != 0 && s >= 1) {
    SHOT.ctrl = OAM_ENABLE;
    PRC_RATE = RATE_6FPS;
    if (d == 1 && !(SHOT.y - bP <= bT)) {
      yP = -1;
    }
    else if (d == 2 && !(SHOT.y + bP >= bB)) {
      yP = 1;
    }
    else if (d == 3 && !(SHOT.x - bP <= bL)) {
      xP = -1;
    }
    else if (d == 4 && !(SHOT.x + bP >= bR)) {
      xP = 1;
    }

    // time the shots so you can't spam them / hold too long
    s = 0;
  }
  else {
    SHOT.ctrl = !OAM_ENABLE;
    PRC_RATE = RATE_12FPS;
  }
  SHOT.x = (PC.x + (xP * 3));
  SHOT.y = (PC.y + (yP * 3));
}

void handleInput(p) {
  uint8_t speed = 2;
  uint8_t direction = 0;
  uint8_t shotFired = 0;
  uint8_t shotDelay = 0;
  uint8_t shotSpaces = 2;

  // bounds
  uint8_t bT = 20;
  uint8_t bB = 60;
  uint8_t bL = 18;
  uint8_t bR = 94;

  if ((~KEY_PAD & KEY_A) && shotDelay == 0) {
    shotFired = 1;
    if (p != PC.x || p != PC.y) {
      shotSpaces--;
      if (shotSpaces == 0) {
        shotDelay = 1;
        p = 0;
      }
    }
  }
  if ((~KEY_PAD & KEY_DOWN) && PC.y < bB) {
    p = PC.y;
    PC.y += speed;
    direction = 2;
  }
  else if ((~KEY_PAD & KEY_UP) && PC.y > bT) {
    p = PC.y;
    PC.y -= speed;
    direction = 1;
  }
  else if ((~KEY_PAD & KEY_LEFT) && PC.x > bL) {
    p = PC.x;
    PC.x -= speed;
    direction = 3;
  }
  else if ((~KEY_PAD & KEY_RIGHT) && PC.x < bR) {
    p = PC.x;
    PC.x += speed;
    direction = 4;
  }

  pcShoot(direction, bL, bR, bT, bB, shotFired);

  /*else if ((~KEY_PAD & KEY_RIGHT & KEY_UP) && PC.y > bT && PC.x < bR) {
    speed /= 2;
    PC.x += speed;
    PC.y -= speed;
  }
  else if ((~KEY_PAD & KEY_LEFT & KEY_UP) && PC.y > bT && PC.x > bL) {
    speed /= 2;
    PC.x -= speed;
    PC.y -= speed;
  }
  else if ((~KEY_PAD & KEY_RIGHT & KEY_DOWN) && PC.y < bB && PC.x < bR) {
    speed /= 2;
    PC.x += speed;
    PC.y += speed;
  }
  else if ((~KEY_PAD & KEY_RIGHT & KEY_DOWN) && PC.y < bB && PC.x < bR) {
    speed /= 2;
    PC.x += speed;
    PC.y -= speed;
  }*/
}

int main()
{
  uint8_t i;
  uint8_t tw = 16;
  uint8_t th = 12;

  // Track player movement updated outside of loop
  uint8_t prevPos = 0;

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
    handleInput(prevPos);
  }
}
