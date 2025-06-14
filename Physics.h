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

    float   GetTileHeight            (const Vec2& sensor, Entity* tile) const;
    float   GetTileWidth             (const Vec2& sensor, Entity* tile) const;
    Entity* GetTileForBottomSensor   (const Vec2& sensor, std::map<unsigned int, Entity*>& map);
    Entity* GetTileForTopSensor      (const Vec2& sensor, std::map<unsigned int, Entity*>& map);
    Entity* GetTileForRightSensor    (const Vec2& sensor, std::map<unsigned int, Entity*>& map);
    Entity* GetTileForLeftSensor     (const Vec2& sensor, std::map<unsigned int, Entity*>& map);
    float   GetTileAngleForPlayer    (Entity* player, Entity* tile);
    float   GetTileVerticalDistance  (const Vec2& sensor, Entity* tile) const;
    float   GetTileDistanceFromTop   (const Vec2& sensor, Entity* tile) const;
    float   GetTileDistanceFromLeft  (const Vec2& sensor, Entity* tile) const;
    float   GetTileDistanceFromRight (const Vec2& sensor, Entity* tile) const;
    float   GetLeftSensorOverlap     (const Vec2& sensor, Entity* tile) const;
    float   GetRightSensorOverlap    (const Vec2& sensor, Entity* tile) const;
    float   GetTopSensorOverlap      (const Vec2& sensor, Entity* tile) const;
    float   GetBottomSensorOverlap   (const Vec2& sensor, Entity* tile) const;
};

#endif