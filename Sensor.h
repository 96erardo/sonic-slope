#ifndef SENSOR_H
#define SENSOR_H
#include "Vec2.h"

class Sensor {
  Vec2 m_floor;
  Vec2 m_right;
  Vec2 m_left;
  Vec2 m_ceiling;

  public:
    enum Direction {
      top,
      bottom,
      left,
      right
    };

    enum Mode {
      floor,
      wRight,
      wLeft,
      ceiling
    };
    
    Direction direction;
    Mode      mode;

    Sensor () {};
    Sensor (Vec2 f, Vec2 r, Vec2 l, Vec2 c, Direction d);

    const Vec2&       pos     () const;
    void              setMode (int angle);
    Sensor::Direction getCurrentDirection () const;
};

#endif