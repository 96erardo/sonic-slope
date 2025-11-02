#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <SFML/Graphics.hpp>
#include <map>
#include "Scene.h"
#include "Assets.h"

class GameEngine {
  sf::RenderWindow               m_window;
  std::map<std::string, Scene*>  m_scenes;
  std::string                    m_currentScene;
  Assets                         m_assets;
  bool                           m_running = true;

  void init (const std::string& level);
  void update ();  
  void loadAssets();
  void sUserInput(); 

  public:
    GameEngine (const std::string& level);

    void run();
    void quit();
    bool isRunning();

    sf::RenderWindow&      window ();
    Scene*                 currentScene ();
    Assets&                assets ();

    void changeScene (const std::string& sceneName, Scene* scene, bool endCurrentScene = false);
};

#endif