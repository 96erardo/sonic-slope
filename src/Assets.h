#ifndef ASSETS_H
#define ASSETS_H
#include <SFML/Graphics.hpp>
#include <map>
#include "Animation.h"

class Assets {
  std::map<std::string, Animation*> m_animations;
  std::map<std::string, sf::Texture*> m_textures;
  std::map<std::string, sf::Font*> m_fonts;

  public:
    Assets ();

    ~Assets () {
      for (auto it = m_fonts.begin(); it != m_fonts.end(); ++it) {
        delete it->second;
      }

      for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
        delete it->second;
      }

      for (auto it = m_animations.begin(); it != m_animations.end(); ++it) {
        delete it->second;
      }
    };

    void addFont (const std::string& name, const std::string& path);
    void addTexture (const std::string& name, const std::string& path);
    void addAnimation (const std::string& name, const sf::Texture& texture, int frameCount, int duration, float cx, float cy);

    sf::Font* getFont (const std::string& name) const;
    sf::Texture* getTexture (const std::string& name) const;
    Animation* getAnimation (const std::string& name) const;
};

#endif