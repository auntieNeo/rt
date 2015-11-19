#ifndef RT_RENDER_PIXEL_H_
#define RT_RENDER_PIXEL_H_

#include <memory>

#include "image.h"

namespace rt { namespace render {
  class Pixel {
    private:
      ImagePtr m_image;
      glm::dvec3 *m_data;
      int m_x, m_y;
    public:
      Pixel(ImagePtr image, int x, int y);
      ~Pixel();

      int x() const { return m_x; }
      int y() const { return m_y; }
      glm::dvec3 *data() { return m_data; }
  };

  typedef std::shared_ptr<Pixel> PixelPtr;
} }

#endif
