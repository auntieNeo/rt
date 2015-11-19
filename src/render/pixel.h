#ifndef RT_RENDER_PIXEL_H_
#define RT_RENDER_PIXEL_H_

#include <memory>

#include <glm/glm.hpp>

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

      glm::dvec2 position() const { return glm::dvec2(double(m_x), double(m_y)); }
      int x() const { return m_x; }
      int y() const { return m_y; }
      glm::dvec3 &value() { return *m_data; }
      double &r() { return (*m_data)[0]; }
      double &g() { return (*m_data)[1]; }
      double &b() { return (*m_data)[2]; }
      glm::dvec3 value() const { return *m_data; }
      double r() const { return (*m_data)[0]; }
      double g() const { return (*m_data)[1]; }
      double b() const { return (*m_data)[2]; }
  };

  typedef std::shared_ptr<Pixel> PixelPtr;
} }

#endif
