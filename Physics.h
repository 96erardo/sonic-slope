#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vec2.h"
#include "Entity.h"

class Physics {
  public:
    Vec2 GetOverlap (Entity* a, Entity* b) const; 
    Vec2 GetPreviousOverlap (Entity* a, Entity* b) const;
    
    bool areColliding (Entity* a, Entity* b) const;

    float GetLeftSensorOverlap      (const Vec2& sensor, Entity* tile) const;
    float GetRightSensorOverlap     (const Vec2& sensor, Entity* tile) const;
    float GetTopSensorOverlap       (const Vec2& sensor, Entity* tile) const;
    float GetBottomSensorOverlap    (const Vec2& sensor, Entity* tile) const;
};

#endif