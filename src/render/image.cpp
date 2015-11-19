#include "image.h"

namespace rt { namespace render {
  Image::Image(int width, int height) {
    m_width = width;
    m_height = height;
  }

  Image::~Image() {
  }

  glm::dvec3 *Image::pixelData(int x, int y) {
    return m_data.data() + (x + y * m_width);
  }
} }
