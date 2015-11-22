#ifndef RT_RENDER_MATERIAL_PROPERTIES_H_
#define RT_RENDER_MATERIAL_PROPERTIES_H_

#include <memory>

#include <glm/glm.hpp>

namespace rt { namespace scene {
  class MaterialProperties {
    private:
      glm::dvec3 m_ambient, m_diffuse, m_specular;
      glm::dvec3 m_normalizedDiffuse, m_normalizedSpecular;
      double m_smoothness;
    public:
      MaterialProperties(const glm::dvec3 &ambient, const glm::dvec3 &diffuse,
          const glm::dvec3 &specular, double smoothness);
      ~MaterialProperties();

      const glm::dvec3 &ambient() const { return m_ambient; }
      const glm::dvec3 &diffuse() const { return m_diffuse; }
      const glm::dvec3 &specular() const { return m_specular; }
      double smoothness() const { return m_smoothness; }

      const glm::dvec3 &normalizedDiffuse() const { return m_normalizedDiffuse; }
      const glm::dvec3 &normalizedSpecular() const { return m_normalizedSpecular; }
  };

  typedef std::shared_ptr<MaterialProperties> MaterialPropertiesPtr;
} }

#endif
