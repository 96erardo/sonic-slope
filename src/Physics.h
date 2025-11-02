#ifndef PHYSICS_H
#define PHYSICS_H
#include <map>
#include "Vec2.h"
#include "Entity.h"
#include "Sensor.h"
#include "constants.h"

class Physics {
  private:
    float GetSnappedAngle (Entity* player);

  public:
    Vec2 GetOverlap (Entity* a, Entity* b) const; 
    Vec2 GetPreviousOverlap (Entity* a, Entity* b) const;
    
    bool areColliding (Entity* a, Entity* b) const;

    float   GetTileHeight             (const Vec2& sensor, Entity* tile) const;
    float   GetTileWidth              (const Vec2& sensor, Entity* tile) const;
    Entity* GetTileForSensor          (const Vec2& sensor, std::map<int, Entity*>& map, Sensor::Direction d);
    float   GetTileAngleForPlayer     (Entity* player, Entity* tile);
    float   GetDistanceToTile         (const Vec2& sensor, Entity* tile, Sensor::Direction d) const;
    float   GetTileDistanceFromBottom (const Vec2& sensor, Entity* tile) const;
    float   GetTileDistanceFromTop    (const Vec2& sensor, Entity* tile) const;
    float   GetTileDistanceFromLeft   (const Vec2& sensor, Entity* tile) const;
    float   GetTileDistanceFromRight  (const Vec2& sensor, Entity* tile) const;
};

#endif