#include <cmath>
#include "utils.h"
#include "constants.h"

unsigned int genKey (unsigned int x, unsigned int y) {
  return (x << 16) | y;
}

Vec2 pixelToGridPosition (const Vec2& pos) {
  return Vec2(
    floorf(pos.x / GRID_SIZE),
    floorf(abs(WINDOW_HEIGHT - pos.y) / GRID_SIZE)
  );
}