#include <iostream>
#include "Entity.h"

Entity::Entity (size_t id, const std::string& tag): m_id(id), m_tag(tag) {
  m_components = std::make_tuple(CTransform(), CBoundingBox(), CCollisionSensor(), CGravity(), CLifespan(), CInput(), CAnimation(), CState());
};
  
size_t Entity::id () const {
  return m_id;
}

std::string Entity::tag () const {
  return m_tag;
}

bool Entity::isActive () const {
  return m_active;
}

void Entity::destroy () {
  m_active = false;
}