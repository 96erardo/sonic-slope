#include "Assets.h"

Assets::Assets() {};

void Assets::addFont (const std::string& name, const std::string& path) {
  m_fonts.insert({
    name, 
    new sf::Font(path)
  });
}

void Assets::addTexture (const std::string& name, const std::string& path) {
  m_textures.insert({
    name, 
    new sf::Texture(path)
  });
}

void Assets::addAnimation (
  const std::string& name, 
  const sf::Texture& texture, 
  int frameCount, 
  int duration, 
  float cx, 
  float cy
) {
  m_animations.insert({
    name, 
    new Animation(name, texture, frameCount, duration, cx, cy)
  });
}

sf::Font* Assets::getFont (const std::string& name) const {
  return m_fonts.at(name);
}

sf::Texture* Assets::getTexture (const std::string& name) const {
  return m_textures.at(name);
}

Animation* Assets::getAnimation (const std::string& name) const {
  return m_animations.at(name);
}