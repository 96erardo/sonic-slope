#include <cmath>
#include "utils.h"
#include "Sensor.h"
#include "constants.h"

int genKey (int x, int y) {
  return ((x + BITWISE_OFFSET) << 16) | (y + BITWISE_OFFSET);
}

Vec2 pixelToGridPosition (const Vec2& pos) {
  return Vec2(
    floorf(pos.x / GRID_SIZE),
    floorf(abs(WINDOW_HEIGHT - pos.y) / GRID_SIZE)
  );
}

Vec2 getExtension (Sensor::Direction d) {
  switch (d) {
    case Sensor::Direction::top:
      return Vec2(0,1);
    case Sensor::Direction::bottom:
      return Vec2(0,-1);
    case Sensor::Direction::left:
      return Vec2(-1, 0);
    case Sensor::Direction::right:
      return Vec2(1, 0);
  }
}

Vec2 getRegression (Sensor::Direction d) {
  switch (d) {
    case Sensor::Direction::top:
      return Vec2(0,-1);
    case Sensor::Direction::bottom:
      return Vec2(0,1);
    case Sensor::Direction::left:
      return Vec2(1, 0);
    case Sensor::Direction::right:
      return Vec2(-1, 0);
  }
}