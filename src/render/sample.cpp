#include "sample.h"

namespace rt { namespace render {
  Sample::Sample(double x, double y)
    : m_position(x, y)
  {
  }

  Sample::Sample(const glm::dvec2 &position)
    : m_position(position)
  {
  }

  Sample::~Sample() {
  }
} }
