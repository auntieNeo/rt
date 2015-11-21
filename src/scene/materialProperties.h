#ifndef RT_RENDER_MATERIAL_PROPERTIES_H_
#define RT_RENDER_MATERIAL_PROPERTIES_H_

#include <memory>

#include <glm/glm.hpp>

namespace rt { namespace scene {
  class MaterialProperties {
    private:
      glm::dvec3 m_ambient, m_diffuse, m_specular;
    public:
      MaterialProperties(glm::dvec3 ambient, glm::dvec3 diffuse, glm::dvec3 specular);
      ~MaterialProperties();

      glm::dvec3 ambient() const { return m_ambient; }
      glm::dvec3 diffuse() const { return m_diffuse; }
      glm::dvec3 specular() const { return m_specular; }
  };

  typedef std::shared_ptr<MaterialProperties> MaterialPropertiesPtr;
} }

#endif
