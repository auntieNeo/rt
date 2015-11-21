#include "materialProperties.h"

namespace rt { namespace scene {
  MaterialProperties::MaterialProperties(glm::dvec3 ambient, glm::dvec3 diffuse, glm::dvec3 specular)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
  {
  }
  MaterialProperties::~MaterialProperties() {
  }
} }
