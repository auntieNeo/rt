#ifndef RT_MATH_MATH_H_
#define RT_MATH_MATH_H_

#include <glm/glm.hpp>

namespace rt { namespace math {
  glm::dvec4 nextaftervec(const glm::dvec4 &from, const glm::dvec4 &to);
  const double EPSILON = 0.0000000001;
} }

#endif
