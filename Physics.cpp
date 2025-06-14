#include "Physics.h"
#include "Components.h"
#include "constants.h"
#include "utils.h"

bool Physics::areColliding (Entity* a, Entity* b) const {
  return (
    (a->getComponent<CTransform>().pos.x - a->getComponent<CBoundingBox>().halfSize.x) < (b->getComponent<CTransform>().pos.x + b->getComponent<CBoundingBox>().halfSize.x) &&
    (b->getComponent<CTransform>().pos.x - b->getComponent<CBoundingBox>().halfSize.x) < (a->getComponent<CTransform>().pos.x + a->getComponent<CBoundingBox>().halfSize.x) &&
    (a->getComponent<CTransform>().pos.y - a->getComponent<CBoundingBox>().halfSize.y) < (b->getComponent<CTransform>().pos.y + b->getComponent<CBoundingBox>().halfSize.y) &&
    (b->getComponent<CTransform>().pos.y - b->getComponent<CBoundingBox>().halfSize.y) < (a->getComponent<CTransform>().pos.y + a->getComponent<CBoundingBox>().halfSize.y)
  );
}

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

float Physics::GetTileHeight (const Vec2& sensor, Entity* tile) const {
  float index = 0;
  
  if (tile->getComponent<CTransform>().scale.x == 1) {
    index = sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x);
  } else {
    index = (tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x;
  }

  if (index == 0 || index == GRID_SIZE) {
    return -1;
  }

  return tile->getComponent<CBoundingBox>().height[floorf(index)];
}

float Physics::GetTileWidth (const Vec2& sensor, Entity* tile) const {
  float index = 0;

  if (tile->getComponent<CTransform>().scale.y == 1) {
    index = sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y);
  } else {
    index = (tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y) - sensor.y;
  }

  if (index == 0 || index == GRID_SIZE) {
    return -1;
  }

  return tile->getComponent<CBoundingBox>().width[floorf(index)];
}

Entity* Physics::GetTileForBottomSensor (const Vec2& sensor, std::map<unsigned int, Entity*>& map) {
  Vec2 gridPos     = pixelToGridPosition(sensor);
  Vec2 extension   = Vec2(0, -1);
  Vec2 regresion   = Vec2(0,  1);
  unsigned int key = genKey(gridPos.x, gridPos.y);

  if (map.find(key) != map.end()) {
    auto tile = map.at(key);
    int height = GetTileHeight(sensor, tile);

    if (height == GRID_SIZE) {
      Vec2 regPos = gridPos + regresion;
      unsigned int regKey = genKey(regPos.x, regPos.y);

      if (map.find(regKey) != map.end()) {
        return map.at(regKey);
      } else {
        return tile;
      }
    } else if (height == 0) {
      Vec2 extPos = gridPos + extension;
      unsigned int extKey = genKey(extPos.x, extPos.y);

      if (map.find(extKey) != map.end()) {
        return map.at(extKey);
      } else {
        return tile;
      }
    } else {
      return tile;
    }
  } else {
    Vec2 extPos = gridPos + extension;
    unsigned int extKey = genKey(extPos.x, extPos.y);

    if (map.find(extKey) != map.end()) {
      return map.at(extKey);
    } else {
      return nullptr;
    }
  }
}

Entity* Physics::GetTileForTopSensor(const Vec2& sensor, std::map<unsigned int, Entity*>& map) {
  Vec2 gridPos     = pixelToGridPosition(sensor);
  Vec2 extension   = Vec2(0,  1);
  Vec2 regresion   = Vec2(0, -1);
  unsigned int key = genKey(gridPos.x, gridPos.y);

  if (map.find(key) != map.end()) {
    auto tile = map.at(key);
    int height = GetTileHeight(sensor, tile);

    if (height == GRID_SIZE) {
      Vec2 regPos = gridPos + regresion;
      unsigned int regKey = genKey(regPos.x, regPos.y);

      if (map.find(regKey) != map.end()) {
        return map.at(regKey);
      } else {
        return tile;
      }
    } else if (height == 0) {
      Vec2 extPos = gridPos + extension;
      unsigned int extKey = genKey(extPos.x, extPos.y);

      if (map.find(extKey) != map.end()) {
        return map.at(extKey);
      } else {
        return tile;
      }
    } else {
      return tile;
    }
  } else {
    Vec2 extPos = gridPos + extension;
    unsigned int extKey = genKey(extPos.x, extPos.y);

    if (map.find(extKey) != map.end()) {
      return map.at(extKey);
    } else {
      return nullptr;
    }
  }
}

Entity* Physics::GetTileForRightSensor(const Vec2& sensor, std::map<unsigned int, Entity*>& map) {
  Vec2 gridPos     = pixelToGridPosition(sensor);
  Vec2 extension   = Vec2( 1, 0);
  Vec2 regresion   = Vec2(-1, 0);
  unsigned int key = genKey(gridPos.x, gridPos.y);

  if (map.find(key) != map.end()) {
    auto tile = map.at(key);
    int width = GetTileWidth(sensor, tile);

    if (width == GRID_SIZE) {
      Vec2 regPos = gridPos + regresion;
      unsigned int regKey = genKey(regPos.x, regPos.y);

      if (map.find(regKey) != map.end()) {
        return map.at(regKey);
      } else {
        return tile;
      }
    } else if (width == 0) {
      Vec2 extPos = gridPos + extension;
      unsigned int extKey = genKey(extPos.x, extPos.y);

      if (map.find(extKey) != map.end()) {
        return map.at(extKey);
      } else {
        return tile;
      }
    } else {
      return tile;
    }
  } else {
    Vec2 extPos = gridPos + extension;
    unsigned int extKey = genKey(extPos.x, extPos.y);

    if (map.find(extKey) != map.end()) {
      return map.at(extKey);
    } else {
      return nullptr;
    }
  }
}

Entity* Physics::GetTileForLeftSensor(const Vec2& sensor, std::map<unsigned int, Entity*>& map) {
  Vec2 gridPos     = pixelToGridPosition(sensor);
  Vec2 extension   = Vec2(-1,0);
  Vec2 regresion   = Vec2( 1,0);
  unsigned int key = genKey(gridPos.x, gridPos.y);

  if (map.find(key) != map.end()) {
    auto tile = map.at(key);
    int width = GetTileWidth(sensor, tile);

    if (width == GRID_SIZE) {
      Vec2 regPos = gridPos + regresion;
      unsigned int regKey = genKey(regPos.x, regPos.y);

      if (map.find(regKey) != map.end()) {
        return map.at(regKey);
      } else {
        return tile;
      }
    } else if (width == 0) {
      Vec2 extPos = gridPos + extension;
      unsigned int extKey = genKey(extPos.x, extPos.y);

      if (map.find(extKey) != map.end()) {
        return map.at(extKey);
      } else {
        return nullptr;
      }
    } else {
      return tile;
    }
  } else {
    Vec2 extPos = gridPos + extension;
    unsigned int extKey = genKey(extPos.x, extPos.y);

    if (map.find(extKey) != map.end()) {
      return map.at(extKey);
    } else {
      return nullptr;
    }
  }
}

float Physics::GetTileAngleForPlayer (Entity* player, Entity* tile) {
  if (player->getComponent<CTransform>().scale.x == 1) {
    if (tile->getComponent<CTransform>().scale.x == 1) {
      return -tile->getComponent<CBoundingBox>().angle;          
    } else {
      return tile->getComponent<CBoundingBox>().angle;
    }
  } else if (player->getComponent<CTransform>().scale.x == -1) {
    if (tile->getComponent<CTransform>().scale.x == 1) {
      return -tile->getComponent<CBoundingBox>().angle;          
    } else {
      return tile->getComponent<CBoundingBox>().angle;
    }
  }

  return 0;
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
    
      if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().size.x && overlap < tile->getComponent<CBoundingBox>().width[index]) {
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

float Physics::GetTileVerticalDistance (const Vec2& sensor, Entity* tile) const {
  int height = GetTileHeight(sensor, tile);

  if (height == -1) {
    return 0;
  }

  return (
    (tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y - height) - sensor.y
  );
}

float Physics::GetTileDistanceFromTop  (const Vec2& sensor, Entity* tile) const {
  int height = GetTileHeight(sensor, tile);

  if (height == - 1) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.y == 1) {
    if (height > 0) {
      return sensor.y - (tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y);
    } else {
      return sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y);
    }
  } else {
    return sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y + height);
  }
};

float Physics::GetTileDistanceFromRight  (const Vec2& sensor, Entity* tile) const {
  int width = GetTileWidth(sensor, tile);

  if (width == -1) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.x == 1) {
    return (tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x - width) - sensor.x;
  } else {
    if (width > 0) {
      if (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x + width > sensor.x) {
        return (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x;
      } else {
        return 0;
      }
    } else {
      return (tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x;
    }
  }
};

float Physics::GetTileDistanceFromLeft  (const Vec2& sensor, Entity* tile) const {
  int width = GetTileWidth(sensor, tile);

  if (width == -1) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.x == 1) {
    if (width > 0) {
      if ((tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x - width) < sensor.x) {
        return sensor.x - (tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x);
      } else {
        return 0;
      }
    } else {
      return sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x);
    }
  } else {
    return sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x + width);
  }
};

float Physics::GetBottomSensorOverlap (const Vec2& sensor, Entity* tile) const {
  int index = 0;
  
  if (tile->getComponent<CTransform>().scale.x == 1) {
    index = sensor.x - (tile->getComponent<CTransform>().pos.x - tile->getComponent<CBoundingBox>().halfSize.x);
  } else {
    index = ceilf((tile->getComponent<CTransform>().pos.x + tile->getComponent<CBoundingBox>().halfSize.x) - sensor.x);
  }

  if (index <= 0 || index > (tile->getComponent<CBoundingBox>().height.size() - 1)) {
    return 0;
  }

  float overlap = tile->getComponent<CBoundingBox>().height[index] - ((tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y) - sensor.y);

  if (overlap > 0 && overlap <= tile->getComponent<CBoundingBox>().height[index]) {
    return overlap;

  } else {
    return 0;
  }
}
