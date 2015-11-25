#ifndef RT_RENDER_IMAGE_H_
#define RT_RENDER_IMAGE_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace rt { namespace render {
  class Image {
    private:
      std::vector<glm::dvec3> m_data;
      int m_width, m_height;

    public:
      Image(int width, int height);
      ~Image();

      int width() const { return m_width; }
      int height() const { return m_height; }

      glm::dvec2 dimensions() { return glm::dvec2(m_width, m_height); }

      glm::dvec3 *pixelData(int x, int y);
  };

  typedef std::shared_ptr<Image> ImagePtr;
} }

#endif
