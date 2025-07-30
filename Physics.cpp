#include <cmath>
#include "Physics.h"
#include "Components.h"
#include "constants.h"
#include "Sensor.h"
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

  if (index == GRID_SIZE) {
    index -= 1;
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

  if (index == GRID_SIZE) {
    index -= 1;
  }

  return tile->getComponent<CBoundingBox>().width[floorf(index)];
}

Entity* Physics::GetTileForSensor (
  const Vec2& sensor, 
  std::map<int, Entity*>& map,
  Sensor::Direction d
) {
  Vec2 gridPos     = pixelToGridPosition(sensor);
  Vec2 extension   = getExtension(d);
  Vec2 regresion   = getRegression(d);
  unsigned int key = genKey(gridPos.x, gridPos.y);

  if (map.find(key) != map.end()) {
    auto tile = map.at(key);
    int value = 0;

    if (d == Sensor::Direction::top || d == Sensor::Direction::bottom) {
      value = GetTileHeight(sensor, tile);
    } else {
      value = GetTileWidth(sensor, tile);
    }

    if (value == GRID_SIZE) {
      Vec2 regPos = gridPos + regresion;
      unsigned int regKey = genKey(regPos.x, regPos.y);

      if (map.find(regKey) != map.end()) {
        return map.at(regKey);
      } else {
        return tile;
      }
    } else if (value == 0) {
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

float Physics::GetTileAngleForPlayer (Entity* player, Entity* tile) {
  if (tile->getComponent<CBoundingBox>().angle == -1) {
    return GetSnappedAngle(player);
  }

  if (tile->getComponent<CTransform>().scale.x == 1 && tile->getComponent<CTransform>().scale.y == 1) {
    return 360 - tile->getComponent<CBoundingBox>().angle;
  
  } else if (tile->getComponent<CTransform>().scale.x == 1 && tile->getComponent<CTransform>().scale.y == -1) {
    return 270 - tile->getComponent<CBoundingBox>().angle;
  
  } else if (tile->getComponent<CTransform>().scale.x == -1 && tile->getComponent<CTransform>().scale.y == -1) {
    return 180 - tile->getComponent<CBoundingBox>().angle;
  
  } else if (tile->getComponent<CTransform>().scale.x == -1 && tile->getComponent<CTransform>().scale.y == 1) {
    return tile->getComponent<CBoundingBox>().angle;
  }

  return 0;
}

float Physics::GetSnappedAngle (Entity* player) {
  return std::fmod(std::round(player->getComponent<CTransform>().angle / 90.0f), 4) * 90;
}

float Physics::GetDistanceToTile (const Vec2& sensor, Entity* tile, Sensor::Direction d) const {
  switch (d) {
    case Sensor::Direction::bottom:
      return GetTileDistanceFromBottom(sensor, tile);
    case Sensor::Direction::left:
      return GetTileDistanceFromLeft(sensor, tile);
    case Sensor::Direction::right:
      return GetTileDistanceFromRight(sensor, tile);
    case Sensor::Direction::top:
      return GetTileDistanceFromTop(sensor, tile);  
  }
}

float Physics::GetTileDistanceFromBottom (const Vec2& sensor, Entity* tile) const {
  int height = GetTileHeight(sensor, tile);

  if (height == -1) {
    return 0;
  }

  if (tile->getComponent<CTransform>().scale.y == 1) {
    return (
      (tile->getComponent<CTransform>().pos.y + tile->getComponent<CBoundingBox>().halfSize.y - height) - sensor.y
    );
  } else {
    return (
      sensor.y - (tile->getComponent<CTransform>().pos.y - tile->getComponent<CBoundingBox>().halfSize.y + height)
    );
  }
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
