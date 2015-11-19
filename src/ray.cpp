#include "ray.h"

namespace rt {
  Ray::Ray(glm::dvec4 origin, glm::dvec4 direction) {
    m_origin = origin;
    m_direction = direction;
  }

  Ray::~Ray() {
  }
}
