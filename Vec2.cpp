#include "Vec2.h"
#include <iostream>

Vec2::Vec2() {};

Vec2::Vec2 (float _x, float _y) : x(_x), y(_y) {};

bool Vec2::operator== (const Vec2& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!= (const Vec2& rhs) const {
  return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator+ (const Vec2& rhs) const {
  return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator- (const Vec2& rhs) const {
  return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator* (float rhs) const {
  return Vec2(x * rhs, y * rhs);
}

Vec2 Vec2::operator/ (float rhs) const {
  return Vec2(x * (1/rhs), y * (1/rhs));
}

void Vec2::operator+= (const Vec2& rhs) {
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator-= (const Vec2& rhs) {
  x -= rhs.x;
  y -= rhs.y;  
}

void Vec2::operator*= (float rhs) {
  x *= rhs;
  y *= rhs;
}

void Vec2::operator/= (float rhs) {
  x = x * (1/rhs);
  y = y * (1/rhs);
}

float Vec2::length () const {
  return std::sqrtf((x * x) + (y * y));
}

float Vec2::dist (const Vec2& rhs) const {
  Vec2 vec = rhs - *this;

  return vec.length();
}
