#include <math.h>

#include "materialProperties.h"

namespace rt { namespace scene {
  MaterialProperties::MaterialProperties(
      const glm::dvec3 &ambient, const glm::dvec3 &diffuse,
      const glm::dvec3 &specular, double smoothness,
      const glm::dvec3 &mirror, const glm::dvec3 &refraction,
      const glm::dvec3 &emission, double refractiveIndex)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
    m_smoothness(smoothness), m_mirror(mirror), m_refraction(refraction),
    m_emission(emission), m_refractiveIndex(refractiveIndex)
  {
    // Normalized diffuse and specular factors that more faithfully approximate
    // the underlying BRDF. See Real-Time Rendering, Third Edition, Akenine,
    // pages 253-255.
    m_normalizedDiffuse = diffuse / M_PI;
    m_normalizedSpecular = ((smoothness + 8.0) / (8.0 * M_PI)) * specular;

    m_isDiffuse = (diffuse != glm::dvec3(0.0, 0.0, 0.0)) ||
                  (specular != glm::dvec3(0.0, 0.0, 0.0));
    m_isMirror = (mirror != glm::dvec3(0.0, 0.0, 0.0));
    m_isRefraction = (refraction != glm::dvec3(0.0, 0.0, 0.0));
    m_isEmission = (emission != glm::dvec3(0.0, 0.0, 0.0));
  }

  MaterialProperties::~MaterialProperties() {
  }
} }
