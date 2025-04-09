#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vec2.h"
#include "Entity.h"

class Physics {
  public:
    Vec2 GetOverlap (Entity* a, Entity* b) const; 
    Vec2 GetPreviousOverlap (Entity* a, Entity* b) const; 
};

#endif