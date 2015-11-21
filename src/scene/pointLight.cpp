#include "pointLight.h"

namespace rt { namespace scene {
  PointLight::PointLight(glm::dvec3 intensity,
      glm::dvec4 position, glm::dquat orientation)
    : SceneObject(position, orientation), m_intensity(intensity)
  {
  }

  PointLight::~PointLight() {
  }
} }
