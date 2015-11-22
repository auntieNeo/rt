#include <math.h>

#include "materialProperties.h"

namespace rt { namespace scene {
  MaterialProperties::MaterialProperties(
      const glm::dvec3 &ambient, const glm::dvec3 &diffuse,
      const glm::dvec3 &specular, double smoothness)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
    m_smoothness(smoothness)
  {
    // Normalized diffuse and specular factors that more faithfully approximate
    // the underlying BRDF. See Real-Time Rendering, Third Edition, Akenine,
    // pages 253-255.
    m_normalizedDiffuse = diffuse / M_PI;
    m_normalizedSpecular = ((smoothness + 8.0) / (8.0 * M_PI)) * specular;
  }
  MaterialProperties::~MaterialProperties() {
  }
} }
