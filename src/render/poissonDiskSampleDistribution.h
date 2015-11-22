#ifndef RT_RENDER_POISSON_DISK_SAMPLE_DISTRIBUTION_H_
#define RT_RENDER_POISSON_DISK_SAMPLE_DISTRIBUTION_H_

#include "pixelSampleDistribution.h"
#include "sample.h"

namespace rt { namespace render {
  /**
   * Describes a randomized sample distribution with uniform distance between samples
   *
   * This class implements a poisson disk sample distribution within a pixel,
   * as described in "Poster Quality Screenshots" by Steve Rabin on page 383 of
   * "Game Programming Gems 4".
   */
  class PoissonDiskSampleDistribution : public PixelSampleDistribution {
    private:
      std::vector<Sample> m_samples;
    public:
      class Iterator;

      /**
       * Constructs a poisson disk sample distribution without specifying the
       * random seed.
       *
       * \a d is the minimum distance between samples.
       */
      PoissonDiskSampleDistribution(double r = 0.1f);

      /**
       * Destructor for poisson disk sample distribution objects.
       */
      ~PoissonDiskSampleDistribution();

      /**
       * This class implements the getSamples() method to return the actual
       * sample locations for the poisson-disk sample distribution.
       */
      std::vector<Sample> getSamples() const;

      Iterator begin() const {
        return Iterator(0, &m_samples);
      }

      Iterator end() const {
        return Iterator(m_samples.size(), &m_samples);
      }

      size_t numSamples() const { return m_samples.size(); }

      class Iterator {
        friend PoissonDiskSampleDistribution;
        private:
          const std::vector<Sample> *m_samples;
          int m_index;
          Iterator(int index, const std::vector<Sample> *samples);
        public:
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

          const Sample &operator*() {
            return m_samples->at(m_index);
          }
      };

    private:
      void m_generateSamples(double r);
  };
} }

#endif
