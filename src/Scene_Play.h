#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Scene.h"
#include "Entity.h"
#include "Command.h"
#include "Physics.h"

struct PlayerConfig {
  float X, Y, ACC, AIR_ACC, FRIC, DEC, MAXSPEED, SLOPE_FACTOR, JUMP, GRAVITY;
};

class Scene_Play: public Scene {
  sf::Color    m_background;
  Entity*      m_player;
  PlayerConfig m_playerConfig;
  Physics      m_physics;
  float        m_worldWidth = 0;
  float        m_offset     = 0;

  Command*     jump;
  Command*     moveLeft;
  Command*     moveRight;

  void init (const std::string& path) override;
  void spawnPlayer ();
  
  void sGravity ();
  void sVelocity ();
  void sMovementX ();
  void sCollisionX ();
  void sMovementY ();
  void sCollisionY ();
  void sState ();
  void sAnimation ();
  void sRender () override;
  
  Vec2 gridToMidPixel (float gridX, float gridY, Entity* entity);

  public:
    Scene_Play(GameEngine* g, const std::string& path);
    ~Scene_Play();
    
    void update () override;
    void doAction (const Action& action) override;
};

#endif