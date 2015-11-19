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

      glm::dvec3 &at(int x, int y) { return m_data.at(x + y * m_data.size()); }

      int width() const { return m_width; }
      int height() const { return m_height; }

      glm::dvec3 *pixelData(int x, int y);
  };

  typedef std::shared_ptr<Image> ImagePtr;
} }

#endif
