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

      glm::dvec4 point(double t) const { return this->origin() + t * this->direction(); }

      Ray &operator*=(const glm::dmat4 &rhs);
  };

  Ray operator*(const glm::dmat4 &lhs, const Ray &rhs);
}

#endif
