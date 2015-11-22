#ifndef RT_RENDER_GRID_SUBIMAGE_WALKER_H_
#define RT_RENDER_GRID_SUBIMAGE_WALKER_H_

#include "image.h"
#include "subimage.h"

namespace rt { namespace render {
  template <class PixelWalker,
            class SampleWalker>
  class GridSubimageWalker {
    private:
      ImagePtr m_image;
    public:
      class Iterator;

      // FIXME: Make these configurable using template parameters
      static const int rows = 5, columns = 5;

      GridSubimageWalker(ImagePtr image) { m_image = image; }
      ~GridSubimageWalker() {}

      Iterator begin() {
        return Iterator(m_image, 0, 0);
      }

      Iterator end() {
        return Iterator(m_image, rows, 0);
      }

      class Iterator {
        private:
          ImagePtr m_image;
          int m_row, m_column;
        public:
          Iterator(ImagePtr image, int row, int column)
            : m_image(image), m_row(row), m_column(column) {}
          ~Iterator() {}

          Iterator &operator++() {  // prefix
            m_column = (m_column + 1) % columns;
            m_row = m_column == 0 ? m_row + 1 : m_row;
          }

          bool operator==(const Iterator &other) {
            return (this->m_row == other.m_row) &&
                   (this->m_column == other.m_column);
          }
          bool operator!=(const Iterator &other) { return !(*this == other); }

          Subimage operator*() {
            int rowSize = m_image->height() / rows;
            int columnSize = m_image->width() / columns;
            int y = rowSize * m_row;
            int x = columnSize * m_column;

            // Add the extra pixels to the last row and column
            int height = (m_row == rows - 1) ?
                         rowSize + m_image->height() % rows :
                         rowSize;
            int width = (m_column == columns - 1) ?
                        columnSize + m_image->width() % columns :
                        columnSize;

            return Subimage(m_image, x, y, width, height);
          }
      };
  };
} }

#endif
