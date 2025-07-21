#include "Sensor.h"
#include "Vec2.h"

Sensor::Sensor (Vec2 f, Vec2 r, Vec2 l, Vec2 c, Sensor::Direction d): m_floor(f), m_right(r), m_left(l), m_ceiling(c), direction(d) {}

const Vec2& Sensor::pos () const {
  switch (mode) {
    case Sensor::Mode::floor:
      return m_floor;
    case Sensor::Mode::wLeft:
      return m_left;
    case Sensor::Mode::wRight:
      return m_right;
    case Sensor::Mode::ceiling:
      return m_ceiling;
    default:
      return m_floor;
  }
}

void Sensor::setMode (int angle) {
  if (direction == Sensor::Direction::bottom) {
    if (angle >= 315 || angle <= 45) {
      mode = Sensor::Mode::floor;
    
    } else if (angle <= 314 && angle >= 226) {
      mode = Sensor::Mode::wRight;

    } else if (angle <= 225 && angle >= 135) {
      mode = Sensor::Mode::ceiling;

    } else if (angle <= 134 && angle >= 46) {
      mode = Sensor::Mode::wLeft;
    }
  } else if (direction == Sensor::Direction::left || direction == Sensor::Direction::right || direction == Sensor::Direction::top) {
    if (angle >= 316 || angle <= 44) {
      mode = Sensor::Mode::floor;
    
    } else if (angle <= 315 && angle >= 225) {
      mode = Sensor::Mode::wRight;

    } else if (angle <= 224 && angle >= 136) {
      mode = Sensor::Mode::ceiling;

    } else if (angle <= 125  && angle >= 45) {
      mode = Sensor::Mode::wLeft;
    }
  }
}