#include "Scene.h"

void Scene::registerAction (sf::Keyboard::Key code, const std::string& name) {
  m_actionMap.insert_or_assign(code, name);
}

const ActionMap& Scene::getActionMap () const {
  return m_actionMap;
}
