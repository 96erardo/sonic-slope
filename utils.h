#ifndef UTILS_H
#define UTILS_H
#include "Vec2.h"
#include "constants.h"
  
int genKey (int x, int y);

Vec2 pixelToGridPosition (const Vec2& pos);

Vec2 getExtension (Direction::Direction dir);

Vec2 getRegression (Direction::Direction dir);

#endif