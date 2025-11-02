#include "Vec2.h"
#include "Animation.h"

Animation::Animation (const sf::Texture& t): m_sprite(t) {};

Animation::Animation (const std::string& name, const sf::Texture& t, size_t frameCount, size_t duration, float cx, float cy): m_sprite(t) {
  m_name = name;
  m_frameCount = frameCount;
  m_duration = duration;
  m_boundingBox = Vec2(cx, cy);
  
  auto [width, height] = t.getSize();
  float tileWidth = width / frameCount;
  int x = m_currentFrame * tileWidth;
  
  m_size = Vec2(tileWidth, height);
  m_sprite.setTextureRect(sf::IntRect({ x, 0 }, sf::Vector2i(tileWidth, height)));
  m_sprite.setOrigin(sf::Vector2f((tileWidth / 2), (height / 2)));
}

void Animation::update () {
  if (m_frameCount == 1) {
    return;
  }

  m_frames++;
  
  if (m_frames % m_duration == 0) {
    m_currentFrame++;

    m_currentFrame = m_currentFrame % m_frameCount;
    auto [width, height] = m_sprite.getTexture().getSize();
    int x = m_currentFrame * (width / m_frameCount);

    m_sprite.setTextureRect(sf::IntRect({ x, 0 }, sf::Vector2<int>(width / m_frameCount, height)));
  }
}

const std::string& Animation::getName () const {
  return m_name;
}

const Vec2& Animation::getSize() const {
  return m_size;
}

const Vec2& Animation::getBoundingBox() const {
  return m_boundingBox;
}

sf::Sprite& Animation::getSprite() {
  return m_sprite;
}

void Animation::setEndable () {
  m_endable = true;
}

bool Animation::hasEnded () const {
  return m_endable && (m_currentFrame + 1 == m_frameCount);
}