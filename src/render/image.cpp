#include <cassert>

#include "image.h"

namespace rt { namespace render {
  Image::Image(int width, int height)
    : m_data(width * height, glm::dvec3(0.0, 0.0, 0.0)),
    m_width(width), m_height(height)
  {
  }

  Image::~Image() {
  }

  glm::dvec3 *Image::pixelData(int x, int y) {
    size_t index = x + y * m_width;
    assert(index < m_data.size());
    return m_data.data() + (x + y * m_width);
  }
} }
