#ifndef SCENE_H
#define SCENE_H
#include <iostream>
#include <map>
#include "Action.h"
#include "EntityManager.h"

class GameEngine;

typedef std::map<sf::Keyboard::Key, std::string> ActionMap;

class Scene {
  protected:
    GameEngine*   m_game;
    EntityManager m_entities;
    ActionMap     m_actionMap;
    int           m_currentFrame;
    bool          m_paused;
    bool          m_hasEnded;

    virtual void init (const std::string& path) = 0;
    virtual void sRender () = 0;

  public: 
    virtual ~Scene() = default;

    virtual void update () = 0;

    const ActionMap& getActionMap () const;
    virtual void doAction (const Action& action) = 0;
    void registerAction (sf::Keyboard::Key code, const std::string& name);
};

#endif