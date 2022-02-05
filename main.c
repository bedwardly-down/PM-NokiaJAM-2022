#include "pm.h"
#include <stdint.h>
#include "sprites.h"

const uint8_t _rom tiles[] _at(0x015000) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void drawAngry() {
  #define ANGRY_TOP_LEFT OAM[0]
  ANGRY_TOP_LEFT.x = 16 + 6;
  ANGRY_TOP_LEFT.y = 16 + 8;
  ANGRY_TOP_LEFT.tile = 0;
  ANGRY_TOP_LEFT.ctrl = OAM_ENABLE;

  #define ANGRY_TOP_RIGHT OAM[1]

  #define ANGRY_BOTTOM_LEFT OAM[2]

  #define ANGRY_BOTTOM_RIGHT OAM[3]
}

int main()
{
  uint8_t i;
  uint8_t tw = 16;
  uint8_t th = 12;

  PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
  PRC_RATE = RATE_36FPS;

  PRC_MAP = tiles;
  PRC_SPR = Angry1_sprites1;

  // create blank tilemap
  for (i = 0; i < tw * th; i++) {
    TILEMAP[i] = 0;
  }

  for(;;) {
    drawAngry();
  }
}
