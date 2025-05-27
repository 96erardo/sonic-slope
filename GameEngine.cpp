#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>
#include "Action.h"
#include "constants.h"
#include "GameEngine.h"
#include "Scene_Play.h"
#include "ProfilerTimer.h"

GameEngine::GameEngine (const std::string& level) {
  init(level);
};

void GameEngine::init(const std::string& level) {
  PROFILE_FUNCTION();
  m_assets = Assets();

  m_window = sf::RenderWindow(
    sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), 
    "Sonic",
    sf::Style::Default,
    sf::State::Windowed
  );

  m_window.setFramerateLimit(60);

  loadAssets();

  changeScene("PLAY", new Scene_Play(this, level));
}

void GameEngine::update() {
  sUserInput();

  currentScene()->update();
}

void GameEngine::run() {
  while (m_window.isOpen() && isRunning()) {
    update();
  }
}

void GameEngine::quit() {
  m_running = false;
}

void GameEngine::changeScene (const std::string& sceneName, Scene* scene, bool endCurrentScene) {
  if (m_scenes.find(m_currentScene) != m_scenes.end()) {
    delete m_scenes.at(m_currentScene);
  }

  m_scenes.erase(m_currentScene);

  m_currentScene = sceneName;
  m_scenes.insert_or_assign(sceneName, scene);
}

Scene* GameEngine::currentScene () {
  return m_scenes[m_currentScene];
}

bool GameEngine::isRunning () {
  return m_running == true;
}

sf::RenderWindow& GameEngine::window () {
  return m_window;
}

Assets& GameEngine::assets () {
  return m_assets;
}

void GameEngine::loadAssets () {
  std::string command;
  std::string name;
  std::string path;
  std::string texture;
  int frames = 0;
  int duration = 0;
  float cx, cy = 0;
  
  std::ifstream file("assets/config.txt");

  while (file >> command) {
    if (command == "Font") {
      file >> name >> path;

      m_assets.addFont(
        name,
        path
      );
    
    } else if (command == "Texture") {
      file >> name >> path;

      m_assets.addTexture(
        name,
        path
      );
    
    } else if (command == "Animation") {
      file >> name >> texture >> frames >> duration >> cx >> cy;

      m_assets.addAnimation(
        name,
        *(m_assets.getTexture(texture)),
        frames,
        duration,
        cx,
        cy
      );
    }
  }
}

void GameEngine::sUserInput () {
  while (const std::optional event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
        m_window.close();
    }
    
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (currentScene()->getActionMap().find(keyPressed->code) == currentScene()->getActionMap().end()) {
        continue;
      }

      currentScene()->doAction(Action(currentScene()->getActionMap().at(keyPressed->code), "START"));
    
    }
    
    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
      if (currentScene()->getActionMap().find(keyReleased->code) == currentScene()->getActionMap().end()) {
        continue;
      }

      currentScene()->doAction(Action(currentScene()->getActionMap().at(keyReleased->code), "END"));
    }
  }
}