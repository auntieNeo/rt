#include <cassert>

#include "subimage.h"

namespace rt { namespace render {

  Subimage::Subimage(ImagePtr image, int x, int y, int width, int height)
    : m_image(image), m_x(x), m_y(y), m_width(width), m_height(height)
  {
  }

  Subimage::~Subimage() {
  }

  Pixel Subimage::pixel(int x, int y) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < m_width);
    assert(y < m_height);
    assert(m_x + x < m_image->width());
    assert(m_y + y < m_image->height());
    return Pixel(m_image, m_x + x, m_y + y);
  }
} }
