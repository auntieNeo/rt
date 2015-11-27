#include "sceneObject.h"

namespace rt { namespace scene {
  SceneObject::SceneObject(const glm::dvec4 &position,
      const glm::dquat &orientation)
  {
    m_position = position;
    m_orientation = orientation;
  }

  SceneObject::~SceneObject() {
  }
} }
