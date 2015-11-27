#include "drawableObject.h"

namespace rt { namespace scene {
  DrawableObject::DrawableObject(MaterialPropertiesPtr materialProperties,
      const glm::dvec4 &position, const glm::dquat &orientation)
    : SceneObject(position, orientation), m_materialProperties(materialProperties)
  {
  }

  DrawableObject::~DrawableObject() {
  }
} }
