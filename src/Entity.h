#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "Components.h"

typedef std::tuple<
  CTransform,
  CGroundSpeed,
  CBoundingBox,
  CCollisionSensor,
  CGravity,
  CLifespan,
  CInput,
  CAnimation,
  CState
> ComponentTuple;

class Entity {
  size_t         m_id     = 0;
  std::string    m_tag    = "default";
  bool           m_active = true;
  ComponentTuple m_components;
  
  public:
    Entity (size_t id, const std::string& tag);

    size_t id () const;
    std::string tag () const;
    bool isActive () const;
    void destroy ();

    template <typename T, typename... TArgs>
    T& addComponent (TArgs&&... mArgs) {
      auto & component = getComponent<T>();
      component = T(std::forward<TArgs>(mArgs)...);
      component.has = true;

      return component;
    }

    template <typename T>
    T& getComponent () {
      return std::get<T>(m_components);
    }

    template <typename T>
    void removeComponent () {
      auto & component = getComponent<T>();
      component = T();
      component.has = false;
    }

    template <typename T>
    bool hasComponent () const {
      return std::get<T>(m_components).has;
    }
};

#endif