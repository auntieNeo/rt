#ifndef RT_RENDER_SUBIMAGE_H_
#define RT_RENDER_SUBIMAGE_H_

#include "image.h"
#include "pixel.h"

namespace rt { namespace render {
  class Subimage {
    private:
      ImagePtr m_image;
      int m_x, m_y, m_width, m_height;
    public:
      Subimage(ImagePtr image, int x, int y, int width, int height);
      ~Subimage();

      int x() const { return m_x; }
      int y() const { return m_y; }
      int width() const { return m_width; }
      int height() const { return m_height; }

      Pixel pixel(int x, int y);
  };
} }

#endif
