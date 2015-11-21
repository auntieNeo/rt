#include <math.h>

#include "math.h"

namespace rt { namespace math {
  glm::dvec4 nextaftervec(const glm::dvec4 &from, const glm::dvec4 &to) {
    return glm::dvec4(
        nextafter(from[0], to[0]),
        nextafter(from[1], to[1]),
        nextafter(from[2], to[2]),
        from[3]);
  }
} }
