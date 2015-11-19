#include "pixel.h"

namespace rt { namespace render {
  Pixel::Pixel(ImagePtr image, int x, int y)
    : m_image(image), m_data(image->pixelData(x, y)), m_x(x), m_y(y)
  {
  }

  Pixel::~Pixel() {
  }
} }
