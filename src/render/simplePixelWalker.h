#ifndef RT_RENDER_SIMPLE_PIXEL_WALKER_H_
#define RT_RENDER_SIMPLE_PIXEL_WALKER_H_

#include <cstdio>

#include "pixel.h"
#include "subimage.h"

namespace rt { namespace render {
  template <class SampleWalker>
  class SimplePixelWalker {
    private:
      Subimage m_subimage;
    public:
      class Iterator;

      SimplePixelWalker(Subimage subimage);
      ~SimplePixelWalker();

      Iterator begin() { return Iterator(m_subimage, 0, 0); }
      Iterator end() {
        return Iterator(m_subimage, 0, m_subimage.height());
      }

      class Iterator {
        private:
          Subimage m_subimage;
          int m_x, m_y;
        public:
          Iterator(Subimage subimage, int x, int y);
          ~Iterator();

          Iterator &operator++();  // prefix
//          Iterator &operator++(int);  // postfix

          bool operator==(const Iterator &other) {
            return (this->m_x == other.m_x) &&
                   (this->m_y == other.m_y);
          }
          bool operator!=(const Iterator &other) {
            return !(*this == other);
          }

          Pixel operator*() {
            return m_subimage.pixel(m_x, m_y);
          }
      };
  };

  template <class SampleWalker>
  SimplePixelWalker<SampleWalker>::SimplePixelWalker(Subimage subimage)
    : m_subimage(subimage)
  {
  }

  template <class SampleWalker>
  SimplePixelWalker<SampleWalker>::~SimplePixelWalker() {
  }

  template <class SampleWalker>
  SimplePixelWalker<SampleWalker>::Iterator::Iterator(Subimage subimage, int x, int y)
    : m_subimage(subimage), m_x(x), m_y(y)
  {
  }

  template <class SampleWalker>
  SimplePixelWalker<SampleWalker>::Iterator::~Iterator() {
  }

  template <class SampleWalker>
  typename SimplePixelWalker<SampleWalker>::Iterator &
  SimplePixelWalker<SampleWalker>::Iterator::operator++() {
    m_x = (m_x + 1) % m_subimage.width();
    m_y = (m_x == 0) ? m_y + 1 : m_y;
  }
} }

#endif
