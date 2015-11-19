#include "sceneObject.h"

namespace rt { namespace scene {
  SceneObject::SceneObject(glm::dvec4 position, glm::dquat orientation) {
    m_position = position;
    m_orientation = orientation;
  }

  SceneObject::~SceneObject() {
  }
} }
