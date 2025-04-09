#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap (Entity* a, Entity* b) const {
  float dx = abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x);
  float dy = abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y);
  float xo = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x;
  float yo = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y;

  return Vec2(xo - dx, yo - dy);
}

Vec2 Physics::GetPreviousOverlap (Entity* a, Entity* b) const {
  float dx = abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x);
  float dy = abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y);
  float xo = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x;
  float yo = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y;
  
  return Vec2(xo - dx, yo - dy);
}
