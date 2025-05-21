#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Scene.h"
#include "Entity.h"
#include "Physics.h"

struct PlayerConfig {
  float X, Y, ACC, DEC, MAXSPEED, GRAVITY;
};

class Scene_Play: public Scene {
  sf::Color                        m_background;
  Entity*                          m_player;
  PlayerConfig                     m_playerConfig;
  Physics                          m_physics;
  std::map<unsigned int, Entity*>  m_worldMap;
  float                            m_worldWidth = 0;
  float                            m_offset     = 0;

  void init (const std::string& path) override;
  void spawnPlayer ();
  void spawnBullet ();
  void spawnExplosion (Entity* entity);
  void spawnHitCoin (Entity* entity);
  
  void sGravity ();
  void sVelocity ();
  void sMovementX ();
  void sCollisionX ();
  void sMovementY ();
  void sCollisionY ();
  void sLifespan ();
  void sState ();
  void sAnimation ();
  void sRender () override;
  
  Vec2 gridToMidPixel (float gridX, float gridY, Entity* entity);

  public:
    Scene_Play(GameEngine* g, const std::string& path);
    
    void update () override;
    void doAction (const Action& action) override;
};

#endif