#ifndef RT_RENDER_SIMPLE_SAMPLE_DISTRIBUTION_H_
#define RT_RENDER_SIMPLE_SAMPLE_DISTRIBUTION_H_

#include "sample.h"

namespace rt { namespace render {
  class SimpleSampleDistribution {
    public:
      class Iterator;

      SimpleSampleDistribution();
      ~SimpleSampleDistribution();

      Iterator begin() { return Iterator(0); }
      Iterator end() { return Iterator(1); }

      size_t numSamples() const { return 1; }

      class Iterator {
        private:
          int m_index;
        public:
          Iterator(int index);
          ~Iterator();

          Iterator &operator++() {  // prefix
            m_index += 1;
          }
//          Iterator &operator++(int);  // postfix

          bool operator==(const Iterator &other) {
            return this->m_index == other.m_index;
          }
          bool operator!=(const Iterator &other) {
            return !(*this == other);
          }

          Sample operator*() {
            // Always sample in the center of the pixel
            return Sample(0.5, 0.5);
          }
      };
  };
} }

#endif
