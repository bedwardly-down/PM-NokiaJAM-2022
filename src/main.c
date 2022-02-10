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

// define OAM Indexes
#define PLINDEX 4

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
  oam_sprite_t *oam;
  uint8_t attributes;
  uint8_t hits;
  uint8_t loot;
  uint8_t speed;
};

// 0x07f808 + for _rom blanks tiles out
const uint8_t _rom tiles[] _at(0x07f700) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void initPlayer(struct entity *p) {
  // PC pointer
  p->attributes = ACTIVE|GROUNDED|MOBILE;
  p->speed = PSPEED;
  p->oam = OAM;
  p->oam[PLINDEX].x = 20;
  p->oam[PLINDEX].y = 20;
  p->oam[PLINDEX].tile = PC_ID;
  p->oam[PLINDEX].ctrl = OAM_ENABLE;

  // Fist1
  p->oam[PLINDEX + 1].x = p->oam[PLINDEX].x;
  p->oam[PLINDEX + 1].y = p->oam[PLINDEX].y;
  p->oam[PLINDEX + 1].tile = SHOT_ID;
  p->oam[PLINDEX + 1].ctrl = !OAM_ENABLE;

  // Fist2
  p->oam[PLINDEX - 1].x = p->oam[PLINDEX].x;
  p->oam[PLINDEX - 1].y = p->oam[PLINDEX].y;
  p->oam[PLINDEX - 1].tile = SHOT_ID;
  p->oam[PLINDEX - 1].ctrl = !OAM_ENABLE;

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

void pcShoot(struct entity *p, uint8_t d, uint8_t s) {
  uint8_t yP = 0;
  uint8_t xP = 0;
  uint8_t bP = 3;
  uint8_t sFct = 3;

  if (d != 0 && s == 1 && sDelay == 0) {
    if (d == 1 && !(p->oam[PLINDEX + 1].y - bP <= BT)) {
      yP = -1;
      p->oam[PLINDEX + 1].ctrl = OAM_ENABLE;
      p->oam[PLINDEX - 1].ctrl = !OAM_ENABLE;
    }
    else if (d == 2 && !(p->oam[PLINDEX - 1].y + bP >= BB)) {
      yP = 1;
      p->oam[PLINDEX + 1].ctrl = !OAM_ENABLE;
      p->oam[PLINDEX - 1].ctrl = OAM_ENABLE;
    }
    else if (d == 3 && !(p->oam[PLINDEX + 1].x - bP <= BL)) {
      xP = -1;
      p->oam[PLINDEX + 1].ctrl = OAM_ENABLE;
      p->oam[PLINDEX - 1].ctrl = !OAM_ENABLE;
    }
    else if (d == 4 && !(p->oam[PLINDEX + 1].x + bP >= BR)) {
      xP = 1;
      p->oam[PLINDEX + 1].ctrl = OAM_ENABLE;
      p->oam[PLINDEX - 1].ctrl = !OAM_ENABLE;
    }
    else {
      p->speed = PSPEED;
      tMoved = 0;
      sDelay = 1;
      p->oam[PLINDEX - 1].ctrl = !OAM_ENABLE;
      p->oam[PLINDEX + 1].ctrl = !OAM_ENABLE;
    }

    sDelay = (tMoved % CDELAY == 0) ? 1 : 0;
    p->oam[PLINDEX - 1].x = (p->oam[PLINDEX].x + (xP * sFct));
    p->oam[PLINDEX - 1].y = (p->oam[PLINDEX].y + (yP * sFct));
    p->oam[PLINDEX + 1].x = (p->oam[PLINDEX].x + (xP * sFct));
    p->oam[PLINDEX + 1].y = (p->oam[PLINDEX].y + (yP * sFct));
  }
  else {
    p->speed = PSPEED;
    tMoved = 0;
    sDelay = 0;
    p->oam[PLINDEX - 1].ctrl = !OAM_ENABLE;
    p->oam[PLINDEX + 1].ctrl = !OAM_ENABLE;
  }
}

void handleInput(uint8_t state, struct entity *p) {
  uint8_t direction = 0;
  uint8_t shotFired = 0;

  if (state = SPLAY) {
    if ((~KEY_PAD & KEY_A) && sDelay == 0) {
      //p->speed = ((p->oam[PLINDEX].y > BB && direction == 2) || (p->oam[PLINDEX].y < BT && direction == 1) || (p->oam[PLINDEX].x > BR && direction == 4) || (p->oam[PLINDEX].x < BL && direction == 3)) ? 0 : PSPEED + 1;
      shotFired = 1;
    }
    if ((~KEY_PAD & KEY_DOWN) && p->oam[PLINDEX].y < BB) {
      p->oam[PLINDEX].y += p->speed;
      direction = 2;
      tMoved++;
    }
    else if ((~KEY_PAD & KEY_UP) && p->oam[PLINDEX].y > BT) {
      p->oam[PLINDEX].y -= p->speed;
      direction = 1;
      tMoved++;
    }
    else if ((~KEY_PAD & KEY_LEFT) && p->oam[PLINDEX].x > BL) {
      p->oam[PLINDEX].x -= p->speed;
      direction = 3;
      tMoved++;
    }
    else if ((~KEY_PAD & KEY_RIGHT) && p->oam[PLINDEX].x < BR) {
      p->oam[PLINDEX].x += p->speed;
      direction = 4;
      tMoved++;
    }

    pcShoot(p, direction, shotFired);

  }
}

int main()
{
  uint8_t i;
  uint8_t tw = 16;
  uint8_t th = 12;

  // Player Entity Ptr
  struct entity player;
  struct entity *pPtr;

  pPtr = &player;

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

  initPlayer(pPtr);
  //initItems();

  for(;;) {
    wait_vsync();
    handleInput(SPLAY, pPtr);
  }
}
