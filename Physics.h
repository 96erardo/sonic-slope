#ifndef PHYSICS_H
#define PHYSICS_H
#include <map>
#include "Vec2.h"
#include "Entity.h"

class Physics {
  public:
    Vec2 GetOverlap (Entity* a, Entity* b) const; 
    Vec2 GetPreviousOverlap (Entity* a, Entity* b) const;
    
    bool areColliding (Entity* a, Entity* b) const;

    float   GetTileHeight           (const Vec2& sensor, Entity* tile) const;
    float   GetTileAngleForPlayer   (Entity* player, Entity* tile);
    Entity* GetTileForBottomSensor  (const Vec2& sensor, std::map<unsigned int, Entity*>& map);
    float   GetTileVerticalDistance (const Vec2& sensor, Entity* tile) const;
    float   GetLeftSensorOverlap    (const Vec2& sensor, Entity* tile) const;
    float   GetRightSensorOverlap   (const Vec2& sensor, Entity* tile) const;
    float   GetTopSensorOverlap     (const Vec2& sensor, Entity* tile) const;
    float   GetBottomSensorOverlap  (const Vec2& sensor, Entity* tile) const;
};

#endif