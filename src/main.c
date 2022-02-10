#include "pm.h"
#include <stdint.h>
//#include "sprites/sprites.h"
#include "sprites/scene1.h"

// define room bounds
#define BT 20
#define BB 60
#define BL 18
#define BR 94

// define combat variables
#define CDELAY 2
#define PSPEED 2
uint8_t tMoved = 0;
uint8_t sDelay = 0;

// define states
#define STITLE 0
#define SPLAY 1

// define entity attributes
#define ACTIVE 0x01
#define HOSTILE 0x02
#define FAST 0x04
#define GROUNDED 0x08
#define MOBILE 0x10
#define LOOT 0x20
#define SPECIAL 0x40
#define EXIT 0x80

// define entity misc
#define HIT1 0x01
#define HIT2 0x02
#define HIT3 0x04
#define HIT4 0x08
#define HEART 0x10
#define BOMB 0x20
#define KEY 0x40
#define COIN 0x80

// global variables
#define MAX_ENTITIES 6

struct entity {
  uint8_t attributes;
  uint8_t misc;
  uint8_t sprite1;
  uint8_t sprite2;
};

// 0x07f808 + for _rom blanks tiles out
const uint8_t _rom tiles[] _at(0x07f700) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void initPlayer() {
  // Player + their fists
  #define PC OAM[4]
  PC.x = 24 - 4;
  PC.y = 24 - 4;
  PC.tile = PC_ID;
  PC.ctrl = OAM_ENABLE;

  #define SHOT OAM[5]
  SHOT.x = 96;
  SHOT.y = 0;
  SHOT.tile = SHOT_ID;
  SHOT.ctrl = !OAM_ENABLE;

  #define SHOT2 OAM[3]
  SHOT2 = SHOT;

  // Enemy sprites
  #define E1 OAM[21]
  E1.x = 48;
  E1.y = 24;
  E1.tile = E1_ID;
  E1.ctrl = OAM_ENABLE;
//
//  #define E2 OAM[19]
//  E2.x = 64;
//  E2.y = 24;
//  E2.tile = E2_ID;
//  E2.ctrl = OAM_ENABLE;
//
//  #define E3 OAM[17]
//  E3.x = 72;
//  E3.y = 24;
//  E3.tile = PC_ID;
//  E3.ctrl = OAM_ENABLE;
}

void initItems() {
  #define HT OAM[0]
  HT.x = 96;
  HT.y = 0;
  HT.tile = HEART_ID;
  HT.ctrl = OAM_ENABLE;
}

void pcShoot(d, s, spd) {
  uint8_t yP = 0;
  uint8_t xP = 0;
  uint8_t bP = 4;
  uint8_t sFct = 3;

  if (d != 0 && s == 1 && sDelay == 0) {
    if (d == 1 && !(SHOT.y - bP <= BT)) {
      yP = -1;
      SHOT.ctrl = OAM_ENABLE;
      SHOT2.ctrl = !OAM_ENABLE;
    }
    else if (d == 2 && !(SHOT2.y + bP >= BB)) {
      yP = 1;
      SHOT.ctrl = !OAM_ENABLE;
      SHOT2.ctrl = OAM_ENABLE;
    }
    else if (d == 3 && !(SHOT.x - bP <= BL)) {
      xP = -1;
      SHOT.ctrl = OAM_ENABLE;
      SHOT2.ctrl = !OAM_ENABLE;
    }
    else if (d == 4 && !(SHOT.x + bP >= BR)) {
      xP = 1;
      SHOT.ctrl = OAM_ENABLE;
      SHOT2.ctrl = !OAM_ENABLE;
    }
    else {
      spd = PSPEED;
      tMoved = 0;
      sDelay = 1;
      SHOT.ctrl = !OAM_ENABLE;
      SHOT2.ctrl = !OAM_ENABLE;
    }

    sDelay = (tMoved % CDELAY == 0) ? 1 : 0;
    SHOT.x = (PC.x + (xP * sFct));
    SHOT.y = (PC.y + (yP * sFct));
    SHOT2.x = (PC.x + (xP * sFct));
    SHOT2.y = (PC.y + (yP * sFct));
  }
  else {
    tMoved = 0;
    sDelay = 0;
    SHOT.ctrl = !OAM_ENABLE;
    SHOT2.ctrl = !OAM_ENABLE;
  }
}

void handleInput(state) {
  uint8_t speed = PSPEED;
  uint8_t direction = 0;
  uint8_t shotFired = 0;

  if (state = SPLAY) {
    if ((~KEY_PAD & KEY_A) && sDelay == 0) {
      speed += 1;
      shotFired = 1;
    }
    if ((~KEY_PAD & KEY_DOWN) && PC.y < BB) {
      PC.y += speed;
      direction = 2;
      tMoved++;
    }
    else if ((~KEY_PAD & KEY_UP) && PC.y > BT) {
      PC.y -= speed;
      direction = 1;
      tMoved++;
    }
    else if ((~KEY_PAD & KEY_LEFT) && PC.x > BL) {
      PC.x -= speed;
      direction = 3;
      tMoved++;
    }
    else if ((~KEY_PAD & KEY_RIGHT) && PC.x < BR) {
      PC.x += speed;
      direction = 4;
      tMoved++;
    }

    pcShoot(direction, shotFired, speed);

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

  initPlayer();
  initItems();

  for(;;) {
    wait_vsync();
    handleInput(SPLAY);
  }
}
