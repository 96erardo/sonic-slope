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

bool Physics::areColliding (Entity* a, Entity* b) const {
  return (
    (a->getComponent<CTransform>().pos.x - a->getComponent<CBoundingBox>().halfSize.x) < (b->getComponent<CTransform>().pos.x + b->getComponent<CBoundingBox>().halfSize.x) &&
    (b->getComponent<CTransform>().pos.x - b->getComponent<CBoundingBox>().halfSize.x) < (a->getComponent<CTransform>().pos.x + a->getComponent<CBoundingBox>().halfSize.x) &&
    (a->getComponent<CTransform>().pos.y - a->getComponent<CBoundingBox>().halfSize.y) < (b->getComponent<CTransform>().pos.y + b->getComponent<CBoundingBox>().halfSize.y) &&
    (b->getComponent<CTransform>().pos.y - b->getComponent<CBoundingBox>().halfSize.y) < (a->getComponent<CTransform>().pos.y + a->getComponent<CBoundingBox>().halfSize.y)
  );
}

float Physics::GetLeftSensorOverlap (const Vec2& sensor, Entity* tile) const {
  int index = 0;

  if (tile->getComponent<CTransform>().scale.y == 1) {
    index = floorf(sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y));
  
  } else {
    index = ceilf((tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y) - sensor.y);
  }

  if (index < 0 || index > (tile->getComponent<CBoundingBox>().width.size() - 1)) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.x == 1) {
    if (tile->getComponent<CBoundingBox>().width[index] > 0) {
      float overlap = (tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x;
    
      if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().size.x) {
        return overlap;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    float overlap = tile->getComponent<CBoundingBox>().width[index] - (sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x));

    if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().width[index]) {
      return overlap;
    } else {
      return 0;
    }
  }
}

float Physics::GetRightSensorOverlap (const Vec2& sensor, Entity* tile) const {
  int index = 0;

  if (tile->getComponent<CTransform>().scale.y == 1) {
    index = floorf(sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y));

  } else {
    index = ceilf((tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y) - sensor.y);
  }

  if (index < 0 || index > (tile->getComponent<CBoundingBox>().width.size() - 1)) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.x == 1) {
    float overlap = tile->getComponent<CBoundingBox>().width[index] - ((tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x);

    if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().width[index]) {
      return overlap;
    } else {
      return 0;
    }
  } else {
    if (tile->getComponent<CBoundingBox>().width[index] > 0) {
      float overlap = sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x);
      
      if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().size.x) {
        return overlap;
      
      } else {
        return 0;
      }    
    } else {
      return 0;
    }
  }
}

float Physics::GetTopSensorOverlap (const Vec2& sensor, Entity* tile) const {
  int index = 0;

  if (tile->getComponent<CTransform>().scale.x == 1) {
    index = floorf(sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x));
  } else {
    index = ceilf((tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x);
  }

  if (index < 0 || index > (tile->getComponent<CBoundingBox>().height.size() - 1)) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.y == 1) {
    if (tile->getComponent<CBoundingBox>().height[index] > 0) {
      float overlap = (tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y) - sensor.y;
      
      if (overlap > 0 && overlap < tile->getComponent<CBoundingBox>().size.y) {
        return overlap;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    float overlap = tile->getComponent<CBoundingBox>().height[index] - (sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y));

    if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().height[index]) {
      return overlap;

    } else {
      return 0;
    }
  }
}

float Physics::GetBottomSensorOverlap (const Vec2& sensor, Entity* tile) const {
  int index = 0;
  
  if (tile->getComponent<CTransform>().scale.x == 1) {
    index = floorf(sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x));
  } else {
    index = ceilf((tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x);
  }

  if (index < 0 || index > (tile->getComponent<CBoundingBox>().height.size() - 1)) {
    return 0;
  }

  float overlap = tile->getComponent<CBoundingBox>().height[index] - ((tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y) - sensor.y);

  if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().height[index]) {
    return overlap;

  } else {
    return 0;
  }
}