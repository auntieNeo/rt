#include "drawableObject.h"

namespace rt { namespace scene {
  DrawableObject::DrawableObject(glm::dvec4 position, glm::dquat orientation) :
    SceneObject(position, orientation)
  {
  }

  DrawableObject::~DrawableObject() {
  }
} }
