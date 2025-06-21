#include <iostream>
#include <fstream>
#include <algorithm>
#include "Vec2.h"
#include "utils.h"
#include "Action.h"
#include "Entity.h"
#include "constants.h"
#include "Components.h"
#include "GameEngine.h"
#include "Scene_Play.h"

Scene_Play::Scene_Play (GameEngine* g, const std::string& path) {
  m_game = g;
  init(path);
}

void Scene_Play::init(const std::string& path) {
  registerAction(sf::Keyboard::Key::W, "UP");
  registerAction(sf::Keyboard::Key::A, "LEFT");
  registerAction(sf::Keyboard::Key::D, "RIGHT");
  registerAction(sf::Keyboard::Key::Space, "JUMP");

  std::string command;
  std::ifstream file(path);

  while (file >> command) {
    if (command == "Tile") {
      std::string name;
      unsigned int x, y;
      float a;
      int scaleX, scaleY;

      file >> name >> x >> y >> scaleX >> scaleY;

      auto entity = m_entities.addEntity("Tile");
      auto animation = m_game->assets().getAnimation(name);

      entity->addComponent<CAnimation>(*animation);
      entity->addComponent<CTransform>(gridToMidPixel(x, y, entity), Vec2(0,0), 0);
      entity->addComponent<CBoundingBox>(animation->getBoundingBox().x, animation->getBoundingBox().y);

      entity->getComponent<CTransform>().scale.x = scaleX;
      entity->getComponent<CTransform>().scale.y = scaleY;

      m_worldMap.insert_or_assign(genKey(x,y), entity);
      m_worldWidth = std::max((x * GRID_SIZE) + animation->getSize().x, m_worldWidth);

      std::ifstream info("assets/images/tiles/" + name + ".txt");

      while (info >> command) {
        if (command == "a") {
          info >> a;
          entity->getComponent<CBoundingBox>().angle = a;

        } else if (command == "v") {
          for (int i = 0; i < 32; i++) {
            info >> y;

            entity->getComponent<CBoundingBox>().height.push_back(y);
          }
        
        } else if (command == "h") {
          for (int i = 0; i < 32; i++) {
            info >> x;

            entity->getComponent<CBoundingBox>().width.push_back(x);
          }
        }
      }
    } else if (command == "Player") {
      file >> m_playerConfig.X 
        >> m_playerConfig.Y 
        >> m_playerConfig.ACC 
        >> m_playerConfig.DEC
        >> m_playerConfig.MAXSPEED
        >> m_playerConfig.GRAVITY;

        spawnPlayer();
    }
  }
}

void Scene_Play::spawnPlayer () {
  auto entity = m_entities.addEntity("Player");
  auto animation = m_game->assets().getAnimation("Stand");
  
  entity->addComponent<CInput>();
  entity->addComponent<CAnimation>(*animation);
  entity->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, entity), Vec2(0,0), 0);
  entity->addComponent<CGravity>(m_playerConfig.GRAVITY);
  entity->addComponent<CBoundingBox>(animation->getBoundingBox().x, animation->getBoundingBox().y);
  entity->addComponent<CCollisionSensor>();
  entity->addComponent<CState>("Stand");

  // bottom right
  entity->getComponent<CCollisionSensor>().bottom.push_back(
    Vec2(entity->getComponent<CBoundingBox>().halfSize.x, entity->getComponent<CBoundingBox>().halfSize.y)
  );
  
  // // bottom left
  entity->getComponent<CCollisionSensor>().bottom.push_back(
    Vec2(-entity->getComponent<CBoundingBox>().halfSize.x, entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // top right
  entity->getComponent<CCollisionSensor>().top.push_back(
    Vec2(entity->getComponent<CBoundingBox>().halfSize.x, -entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // top left
  entity->getComponent<CCollisionSensor>().top.push_back(
    Vec2(-entity->getComponent<CBoundingBox>().halfSize.x, -entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // mid left
  entity->getComponent<CCollisionSensor>().left = Vec2(-entity->getComponent<CBoundingBox>().halfSize.x - 1, 0);

  // mid right
  entity->getComponent<CCollisionSensor>().right = Vec2(entity->getComponent<CBoundingBox>().halfSize.x + 1, 0);

  m_player = entity;
}

Vec2 Scene_Play::gridToMidPixel (float gridX, float gridY, Entity* entity) {
  Vec2 size = entity->getComponent<CAnimation>().animation.getSize();

  float x = (gridX * GRID_SIZE) + (size.x / 2);
  float y = abs((WINDOW_HEIGHT - (gridY * GRID_SIZE) - (size.y / 2)));

  return Vec2(x, y);
}

void Scene_Play::doAction (const Action& action) {
  if (action.type() == "START") {
    if (action.name() == "LEFT") {
      m_player->getComponent<CInput>().left = true;
    }
    
    if (action.name() == "RIGHT") {
      m_player->getComponent<CInput>().right = true;
    }

    if (action.name() == "JUMP") {
      m_player->getComponent<CInput>().jump = true;
    }
  } else if (action.type() == "END") {
    if (action.name() == "LEFT") {
      m_player->getComponent<CInput>().left = false;
    }
    
    if (action.name() == "RIGHT") {
      m_player->getComponent<CInput>().right = false;
    }

    if (action.name() == "JUMP") {
      m_player->getComponent<CInput>().jump = false;
      m_player->getComponent<CInput>().canJump = false;
    }
  }
}

void Scene_Play::sGravity () {
  for (auto entity : m_entities.getEntities()) {
    if (entity->hasComponent<CGravity>()) {
        entity->getComponent<CTransform>().vel.y += entity->getComponent<CGravity>().value;
    }
  }
}

void Scene_Play::sVelocity () {
  m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos;

  if (m_player->getComponent<CInput>().right) {
    m_player->getComponent<CState>().state = "Running";

    if (m_player->getComponent<CTransform>().vel.x < 0) {
      m_player->getComponent<CTransform>().vel.x = std::min(
        m_playerConfig.MAXSPEED,
        m_player->getComponent<CTransform>().vel.x + m_playerConfig.DEC
      );

      m_player->getComponent<CState>().state = "Stopping";

    } else {
      m_player->getComponent<CTransform>().vel.x = std::min(
        m_playerConfig.MAXSPEED,
        m_player->getComponent<CTransform>().vel.x + m_playerConfig.ACC
      );

      m_player->getComponent<CTransform>().scale = Vec2(1, 1);

      if (abs(m_player->getComponent<CTransform>().vel.x) > 3.5) {
        m_player->getComponent<CState>().state = "RunningFast";
      }
    }

  } else if (m_player->getComponent<CInput>().left) {
    m_player->getComponent<CState>().state = "Running";
    
    if (m_player->getComponent<CTransform>().vel.x > 0) {
      m_player->getComponent<CTransform>().vel.x = std::max(
        -m_playerConfig.MAXSPEED,
        m_player->getComponent<CTransform>().vel.x - m_playerConfig.DEC
      );

      m_player->getComponent<CState>().state = "Stopping";

    } else {
      m_player->getComponent<CTransform>().vel.x = std::max(
        -m_playerConfig.MAXSPEED,
        m_player->getComponent<CTransform>().vel.x - m_playerConfig.ACC
      );

      m_player->getComponent<CTransform>().scale = Vec2(-1, 1);

      if (abs(m_player->getComponent<CTransform>().vel.x) > 3.5) {
        m_player->getComponent<CState>().state = "RunningFast";
      }
    }

  } else {
    if (m_player->getComponent<CTransform>().vel.x != 0) {
      if (m_player->getComponent<CTransform>().vel.x > 0) {
        m_player->getComponent<CTransform>().vel.x = std::max(
          0.0f,
          m_player->getComponent<CTransform>().vel.x - m_playerConfig.ACC
        );
      } else {
        m_player->getComponent<CTransform>().vel.x = std::min(
          0.0f,
          m_player->getComponent<CTransform>().vel.x + m_playerConfig.ACC
        );
      }

      if (m_player->getComponent<CState>().state != "Stopping") {
        m_player->getComponent<CState>().state = "Running";

        if (abs(m_player->getComponent<CTransform>().vel.x) > 3.5) {
          m_player->getComponent<CState>().state = "RunningFast";
        }
      }
    } else {
      m_player->getComponent<CState>().state = "Stand";
    }
  }

  if (
    m_player->getComponent<CInput>().jump &&
    m_player->getComponent<CInput>().canJump
  ) {
    m_player->getComponent<CInput>().canJump = false;
    m_player->getComponent<CTransform>().vel.y += -10;
  }
  
  if (m_player->getComponent<CInput>().canJump == false) {
    m_player->getComponent<CState>().state = "Jumping";
  }
}

void Scene_Play::sMovementX () {
  m_player->getComponent<CTransform>().pos.x += m_player->getComponent<CTransform>().vel.x;

  if (m_player->getComponent<CTransform>().pos.x - m_player->getComponent<CBoundingBox>().halfSize.x < 0) {
    m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CBoundingBox>().halfSize.x;
    m_player->getComponent<CTransform>().vel.x = 0;
    m_player->getComponent<CState>().state = "Pushing";
  }

  if (m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CBoundingBox>().halfSize.x > m_worldWidth) {
    m_player->getComponent<CTransform>().pos.x = m_worldWidth - m_player->getComponent<CBoundingBox>().halfSize.x;
    m_player->getComponent<CTransform>().vel.x = 0;
    m_player->getComponent<CState>().state = "Pushing";
  }
}

void Scene_Play::sCollisionX () {
  if (m_player->getComponent<CTransform>().vel.x > 0) {
    Vec2 sensor    = m_player->getComponent<CTransform>().pos + m_player->getComponent<CCollisionSensor>().right;
    Entity* tile   = m_physics.GetTileForSensor(sensor, m_worldMap, Direction::right);

    if (tile != nullptr) {
      float distance = m_physics.GetTileDistanceFromRight(sensor, tile); 

      if (distance < 0) {
        m_player->getComponent<CTransform>().pos.x += distance;
        m_player->getComponent<CTransform>().vel.x = 0;
        m_player->getComponent<CState>().state = "Pushing";
      }
    }
  } else if (m_player->getComponent<CTransform>().vel.x < 0) {
    Vec2 sensor    = m_player->getComponent<CTransform>().pos + m_player->getComponent<CCollisionSensor>().left;
    Entity* tile   = m_physics.GetTileForSensor(sensor, m_worldMap, Direction::left);

    if (tile != nullptr) {
      float distance = m_physics.GetTileDistanceFromLeft(sensor, tile); 

      if (distance < 0) {
        m_player->getComponent<CTransform>().pos.x -= distance;
        m_player->getComponent<CTransform>().vel.x = 0;
        m_player->getComponent<CState>().state = "Pushing";
      }
    }
  }

  if (m_player->getComponent<CTransform>().vel.x != 0) {
    float distance = ((GRID_SIZE * 2) + 1);
    Entity* tile = nullptr;
  
    for (auto& b : m_player->getComponent<CCollisionSensor>().bottom) {
      Vec2 sensor    = m_player->getComponent<CTransform>().pos + b;
      Entity* entity = m_physics.GetTileForSensor(sensor, m_worldMap, Direction::bottom);

      if (entity != nullptr) {
        float d = m_physics.GetTileDistanceFromBottom(sensor, entity);

        if (d < distance) {
          distance = d;
          tile = entity;
        }
      }
    }
  
    if (tile != nullptr) {
      if (distance < 0) {
        m_player->getComponent<CTransform>().pos.y += distance;
        m_player->getComponent<CTransform>().vel.y  = 0;
        m_player->getComponent<CInput>().canJump    = true;
        m_player->getComponent<CTransform>().angle  = m_physics.GetTileAngleForPlayer(m_player, tile);
      }
    }
  }
}

void Scene_Play::sMovementY () {
  m_player->getComponent<CTransform>().pos.y += m_player->getComponent<CTransform>().vel.y;
}

void Scene_Play::sCollisionY () {
  if (m_player->getComponent<CTransform>().vel.y < 0) {
    float distance = ((GRID_SIZE * 2) + 1);
    Entity* tile = nullptr;

    for (auto& t : m_player->getComponent<CCollisionSensor>().top) {
      Vec2 sensor    = m_player->getComponent<CTransform>().pos + t;
      Entity* entity = m_physics.GetTileForSensor(sensor, m_worldMap, Direction::top);

      if (entity != nullptr) {
        float d = m_physics.GetTileDistanceFromTop(sensor, entity);

        if (d < distance) {
          distance = d;
          tile = entity;
        }
      }

      if (tile != nullptr) {
        if (distance < 0) {
          m_player->getComponent<CTransform>().pos.y -= distance;
          m_player->getComponent<CTransform>().vel.y  = 0;
          m_player->getComponent<CInput>().canJump    = false;
        }
      }
    }
  } else if (m_player->getComponent<CTransform>().vel.y > 0) {
    float distance = ((GRID_SIZE * 2) + 1);
    Entity* tile = nullptr;
  
    for (auto& b : m_player->getComponent<CCollisionSensor>().bottom) {
      Vec2 sensor    = m_player->getComponent<CTransform>().pos + b;
      Entity* entity = m_physics.GetTileForSensor(sensor, m_worldMap, Direction::bottom);

      if (entity != nullptr) {
        float d = m_physics.GetTileDistanceFromBottom(sensor, entity);

        if (d < distance) {
          distance = d;
          tile = entity;
        }
      }
    }
  
    if (tile != nullptr) {
      if (distance < 0) {
        m_player->getComponent<CTransform>().pos.y += distance;
        m_player->getComponent<CTransform>().vel.y  = 0;
        m_player->getComponent<CInput>().canJump    = true;
        m_player->getComponent<CTransform>().angle  = m_physics.GetTileAngleForPlayer(m_player, tile);
      }
    }
  }
}

void Scene_Play::sState () {
  for (auto entity : m_entities.getEntities()) {
    if (entity->hasComponent<CState>() && entity->hasComponent<CAnimation>()) {
      if (
        entity->getComponent<CAnimation>().animation.getName() != entity->getComponent<CState>().state
      ) {
        auto animation = m_game->assets().getAnimation(
          entity->getComponent<CState>().state
        );
  
        m_player->removeComponent<CAnimation>();
        m_player->addComponent<CAnimation>(*animation);
      }
    }
  }
}

void Scene_Play::sAnimation () {
  for (auto entity : m_entities.getEntities()) {
    entity->getComponent<CAnimation>().animation.getSprite().setPosition({
      entity->getComponent<CTransform>().pos.x,
      entity->getComponent<CTransform>().pos.y,
    });

    entity->getComponent<CAnimation>().animation.update();

    if (entity->hasComponent<CTransform>()) {
      entity->getComponent<CAnimation>().animation.getSprite().setScale({
        entity->getComponent<CTransform>().scale.x,
        entity->getComponent<CTransform>().scale.y  
      });

      entity->getComponent<CAnimation>().animation.getSprite().setRotation(
        sf::degrees(entity->getComponent<CTransform>().angle)
      );
    }

    if (entity->getComponent<CAnimation>().animation.hasEnded()) {
      entity->destroy();
    }
  }
}

void Scene_Play::sRender () {
  m_game->window().clear(sf::Color::White);
  
  float x = std::min(
    m_worldWidth - WINDOW_WIDTH / 2,
    std::max(
      (float) WINDOW_WIDTH / 2,
      m_player->getComponent<CTransform>().pos.x
    )
  );

  float y = std::min((float) WINDOW_HEIGHT / 2, m_player->getComponent<CTransform>().pos.y);

  sf::View view = m_game->window().getDefaultView();  
  view.setCenter({x,y});
  m_game->window().setView(view);

  for (auto entity : m_entities.getEntities()) {
    m_game->window().draw(entity->getComponent<CAnimation>().animation.getSprite());
  }

  sf::RectangleShape tr({3,3});
  
  tr.setFillColor(sf::Color::Red);
  tr.setPosition({ 
    m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CCollisionSensor>().top[0].x - 3, 
    m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CCollisionSensor>().top[0].y
  });

  sf::RectangleShape tl({3,3});
  
  tl.setFillColor(sf::Color::Red);  
  tl.setPosition({ 
    m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CCollisionSensor>().top[1].x, 
    m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CCollisionSensor>().top[1].y
  });
  
  sf::RectangleShape br({3,3});
  
  br.setFillColor(sf::Color::Blue);
  br.setPosition({ 
    m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CCollisionSensor>().bottom[0].x - 3,
    m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CCollisionSensor>().bottom[0].y - 3
  });

  sf::RectangleShape bl({3,3});
  
  bl.setFillColor(sf::Color::Blue);
  bl.setPosition({ 
    m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CCollisionSensor>().bottom[1].x,
    m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CCollisionSensor>().bottom[1].y - 3
  });
  
  sf::RectangleShape left ({3,3});
  
  left.setFillColor(sf::Color::Green);
  left.setPosition({ 
    m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CCollisionSensor>().left.x,
    m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CCollisionSensor>().left.y
  });
  
  
  sf::RectangleShape right ({3,3});
  
  right.setFillColor(sf::Color::Green);
  right.setPosition({ 
    m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CCollisionSensor>().right.x - 3,
    m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CCollisionSensor>().right.y
  });  
  
  m_game->window().draw(br);
  m_game->window().draw(bl);
  m_game->window().draw(tr);
  m_game->window().draw(tl);
  m_game->window().draw(left);
  m_game->window().draw(right);

  m_game->window().display();
}

void Scene_Play::update () {
  m_entities.update();

  sGravity();
  sVelocity();
  sMovementX();
  sCollisionX();
  sMovementY();
  sCollisionY();
  sState();
  sAnimation();
  sRender();
}
