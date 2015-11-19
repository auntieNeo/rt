#ifndef RT_RAY_H_
#define RT_RAY_H_

#include <glm/glm.hpp>

namespace rt {
  class Ray {
    private:
      glm::dvec4 m_origin, m_direction;

    public:
      Ray(glm::dvec4 origin, glm::dvec4 direction);
      ~Ray();

      glm::dvec4 origin() const { return m_origin; }
      glm::dvec4 direction() const { return m_direction; }
  };
}

#endif
