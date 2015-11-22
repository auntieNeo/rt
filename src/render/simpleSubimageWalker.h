#ifndef RT_RENDER_SIMPLE_SUBIMAGE_WALKER_H_
#define RT_RENDER_SIMPLE_SUBIMAGE_WALKER_H_

#include "image.h"
#include "subimage.h"

namespace rt { namespace render {
  template <class PixelWalker,
            class SampleWalker>
  class SimpleSubimageWalker {
    private:
      ImagePtr m_image;
    public:
      class Iterator;

      SimpleSubimageWalker(ImagePtr image);
      ~SimpleSubimageWalker();

      Iterator begin();
      Iterator end();

      class Iterator {
        private:
          ImagePtr m_image;
          int m_index;
        public:
          Iterator(ImagePtr m_image, int index);
          ~Iterator();

          Iterator &operator++();  // prefix

          bool operator==(const Iterator &other);
          bool operator!=(const Iterator &other) { return !(*this == other); }

          Subimage operator*();
      };
  };

  template <class PixelWalker,
            class SampleWalker>
  SimpleSubimageWalker<PixelWalker, SampleWalker>::SimpleSubimageWalker(ImagePtr image)
    : m_image(image)
  {
    // We break the image up into one huge subimage... nothing to see here
  }

  template <class PixelWalker,
            class SampleWalker>
  SimpleSubimageWalker<PixelWalker, SampleWalker>::~SimpleSubimageWalker() {
  }

  template <class PixelWalker,
            class SampleWalker>
  typename SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator
  SimpleSubimageWalker<PixelWalker, SampleWalker>::begin() {
    // Zero is the first index in our single element "list" of subimages
    return Iterator(m_image, 0);
  }

  template <class PixelWalker,
            class SampleWalker>
  typename SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator
  SimpleSubimageWalker<PixelWalker, SampleWalker>::end() {
    // Return iterator at index 1, exactly one index past the end of our single
    // element "list" of subimages
    return Iterator(m_image, 1);
  }

  template <class PixelWalker,
            class SampleWalker>
  SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator::Iterator(ImagePtr image, int index)
    : m_image(image), m_index(index)
  {
  }

  template <class PixelWalker,
            class SampleWalker>
  SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator::~Iterator() {
  }

  template <class PixelWalker,
            class SampleWalker>
  typename SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator &
  SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator::operator++() {  // prefix
    m_index += 1;
    return *this;
  }

//  Iterator &operator++(int);  // postfix

  template <class PixelWalker,
            class SampleWalker>
  bool SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator::operator==(const Iterator &other) {
    return this->m_index == other.m_index;
  }

  template <class PixelWalker,
            class SampleWalker>
  Subimage SimpleSubimageWalker<PixelWalker, SampleWalker>::Iterator::operator*() {
    // Return the entire image as a subimage
    return Subimage(m_image, 0, 0, m_image->width(), m_image->height());
  }
} }

#endif
