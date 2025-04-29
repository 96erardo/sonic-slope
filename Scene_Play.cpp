#include <iostream>
#include <fstream>
#include <algorithm>
#include "Vec2.h"
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
      float x, y;
      int scaleX, scaleY;

      file >> name >> x >> y >> scaleX >> scaleY;

      auto entity = m_entities.addEntity("Tile");
      auto animation = m_game->assets().getAnimation(name);

      entity->addComponent<CAnimation>(*animation);
      entity->addComponent<CTransform>(gridToMidPixel(x, y, entity), Vec2(0,0), 0);
      entity->addComponent<CBoundingBox>(animation->getBoundingBox().x, animation->getBoundingBox().y);

      entity->getComponent<CTransform>().scale.x = scaleX;
      entity->getComponent<CTransform>().scale.y = scaleY;

      m_worldWidth = std::max((x * GRID_SIZE) + animation->getSize().x, m_worldWidth);

      std::ifstream info("assets/images/tiles/" + name + ".txt");

      while (info >> command) {
        if (command == "v") {
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
  entity->getComponent<CCollisionSensor>().down.push_back(
    Vec2(entity->getComponent<CBoundingBox>().halfSize.x, entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // bottom left
  entity->getComponent<CCollisionSensor>().down.push_back(
    Vec2(-entity->getComponent<CBoundingBox>().halfSize.x, entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // top right
  entity->getComponent<CCollisionSensor>().up.push_back(
    Vec2(entity->getComponent<CBoundingBox>().halfSize.x, -entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // top left
  entity->getComponent<CCollisionSensor>().up.push_back(
    Vec2(-entity->getComponent<CBoundingBox>().halfSize.x, -entity->getComponent<CBoundingBox>().halfSize.y)
  );

  // mid left
  entity->getComponent<CCollisionSensor>().left = Vec2(-entity->getComponent<CBoundingBox>().halfSize.x, 0);

  // mid right
  entity->getComponent<CCollisionSensor>().right = Vec2(entity->getComponent<CBoundingBox>().halfSize.x, 0);

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
      m_player->getComponent<CTransform>().vel.x = std::min(
        m_playerConfig.MAXSPEED,
        m_player->getComponent<CTransform>().vel.x - m_playerConfig.DEC
      );

      m_player->getComponent<CState>().state = "Stopping";

    } else {
      m_player->getComponent<CTransform>().vel.x = std::min(
        m_playerConfig.MAXSPEED,
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
  for (auto entity : m_entities.getEntities("Tile")) {
    if (m_physics.areColliding(m_player, entity)) {
      if (m_player->getComponent<CTransform>().vel.x > 0) {
        // mid right sensor
        Vec2 mr = m_player->getComponent<CTransform>().pos + m_player->getComponent<CCollisionSensor>().right;
        float overlap = m_physics.GetRightSensorOverlap(mr, entity);

        if (overlap > 0) {
          m_player->getComponent<CTransform>().pos.x -= overlap;
          m_player->getComponent<CTransform>().vel.x = 0;
          m_player->getComponent<CState>().state = "Pushing";
        }
      } else if (m_player->getComponent<CTransform>().vel.x < 0) {
        // mid left sensor
        Vec2 ml = m_player->getComponent<CTransform>().pos + m_player->getComponent<CCollisionSensor>().left;
        float overlap = m_physics.GetLeftSensorOverlap(ml, entity);

        if (overlap > 0) {
          m_player->getComponent<CTransform>().pos.x += overlap;
          m_player->getComponent<CTransform>().vel.x = 0;
          m_player->getComponent<CState>().state = "Pushing";
        }
      }

      if (m_player->getComponent<CTransform>().vel.y > 0) {
        for (auto& bottom : m_player->getComponent<CCollisionSensor>().down) {
          Vec2 sensor   = m_player->getComponent<CTransform>().pos + bottom;
          float overlap = m_physics.GetBottomSensorOverlap(sensor, entity);

          if (overlap > 0) {
            m_player->getComponent<CTransform>().pos.y -= overlap;
          }
        }
      }
    }
  }
}

void Scene_Play::sMovementY () {
  m_player->getComponent<CTransform>().pos.y += m_player->getComponent<CTransform>().vel.y;
}

void Scene_Play::sCollisionY () {
  for (auto entity : m_entities.getEntities("Tile")) {
    Vec2 boxOverlap = m_physics.GetOverlap(m_player, entity);

    if (boxOverlap.x > 0 && boxOverlap.y > 0) {
      if (m_player->getComponent<CTransform>().vel.y > 0) {
        for (auto& bottom : m_player->getComponent<CCollisionSensor>().down) {
          Vec2 sensor   = m_player->getComponent<CTransform>().pos + bottom;
          float overlap = m_physics.GetBottomSensorOverlap(sensor, entity);

          if (overlap > 0) {
            m_player->getComponent<CTransform>().pos.y -= overlap;
            m_player->getComponent<CTransform>().vel.y = 0;
            m_player->getComponent<CInput>().canJump = true;
          }
        }
      } else if (m_player->getComponent<CTransform>().vel.y < 0) {
        for (auto& top : m_player->getComponent<CCollisionSensor>().up) {
          Vec2 sensor   = m_player->getComponent<CTransform>().pos + top;
          float overlap = m_physics.GetTopSensorOverlap(sensor, entity);

          if (overlap > 0) {
            m_player->getComponent<CTransform>().pos.y += overlap;
            m_player->getComponent<CTransform>().vel.y = 0;
            m_player->getComponent<CInput>().canJump = false;
          }
        }
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
