#include "ray.h"

namespace rt {
  Ray::Ray(glm::dvec4 origin, glm::dvec4 direction) {
    m_origin = origin;
    m_direction = direction;
  }

  Ray::~Ray() {
  }

  Ray &Ray::operator*=(const glm::dmat4 &rhs) {
    m_origin = rhs * m_origin;
    m_direction = rhs * m_direction;
    return *this;
  }

  Ray operator*(const glm::dmat4 &lhs, const Ray &rhs) {
    Ray result = rhs;
    return result *= lhs;
  }
}
