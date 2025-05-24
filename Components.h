#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Vec2.h"

class Component {
  public:
    bool has = false;
};

class CTransform: public Component {
  public:
    Vec2  pos     = { 0,0 };
    Vec2  prevPos = { 0,0 };
    Vec2  vel     = { 0,0 };
    Vec2  scale   = { 1,1 };
    int   angle   = 0;

    CTransform () {};
    CTransform (const Vec2& p, const Vec2& v, int a):
      pos(p),
      prevPos(p.x, p.y),
      vel(v),
      angle(a)
    {}
};

class CGravity: public Component {
  public: 
    float value = 0;

    CGravity () {};
    CGravity (float v): value(v) {};
};

class CLifespan: public Component {
  public:
    size_t value = 0;

    CLifespan () {};
    CLifespan (size_t v): value(v) {};
};

class CBoundingBox: public Component {
  public:
    Vec2                size     = { 0,0 };
    Vec2                halfSize = { 0,0 };
    float               angle = 0;
    std::vector<int>    height;
    std::vector<int>    width;

    CBoundingBox () {};
    CBoundingBox (float x, float y):
      size(x, y),
      halfSize(x / 2, y / 2) {};
};

class CCollisionSensor: public Component {

  public:
    std::vector<Vec2>  top;
    std::vector<Vec2>  bottom;
    Vec2  left;
    Vec2  right;

    CCollisionSensor () {};
};

class CInput: public Component {
  public:
    bool left = false;
    bool right = false;
    bool jump = false;
    bool canJump = false;

    CInput () {};
};

class CAnimation: public Component {
  public:
    Animation animation;

    CAnimation (): animation(Animation(sf::Texture())) {};
    CAnimation (Animation& a): animation(a) {};
};

class CState: public Component {
  public:
    std::string state = "none";

    CState () {}
    CState (const std::string& s): state(s) {};
};

#endif