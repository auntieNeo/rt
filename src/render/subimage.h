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
      class Iterator;

      Subimage(ImagePtr image, int x, int y, int width, int height);
      ~Subimage();

      int x() const { return m_x; }
      int y() const { return m_y; }
      int width() const { return m_width; }
      int height() const { return m_height; }

      Pixel pixel(int x, int y);
      const Pixel pixel(int x, int y) const;

      Iterator begin() const {
        return Iterator(0, 0, this);
      }
      Iterator end() const {
        return Iterator(0, m_height, this);
      }

      class Iterator {
        friend Subimage;
        private:
          const Subimage *m_subimage;
          int m_x, m_y;

          Iterator(int x, int y, const Subimage *subimage)
            : m_x(x), m_y(y), m_subimage(subimage)
          {
          }
        public:
          ~Iterator() {}

          Iterator &operator++() {
            m_x = (m_x + 1) % m_subimage->width();
            m_y = (m_x == 0) ? m_y + 1 : m_y;
          }

          bool operator==(const Iterator &other) const {
            return (this->m_x == other.m_x) &&
                   (this->m_y == other.m_y);
          }
          bool operator!=(const Iterator &other) const {
            return !(*this == other);
          }

          const Pixel operator*() {
            return m_subimage->pixel(m_x, m_y);
          }
      };
  };
} }

#endif
