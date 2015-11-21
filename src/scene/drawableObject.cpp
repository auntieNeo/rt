#include "drawableObject.h"

namespace rt { namespace scene {
  DrawableObject::DrawableObject(MaterialPropertiesPtr materialProperties,
      glm::dvec4 position, glm::dquat orientation)
    : SceneObject(position, orientation), m_materialProperties(materialProperties)
  {
  }

  DrawableObject::~DrawableObject() {
  }
} }
