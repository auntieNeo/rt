#ifndef RT_RENDER_POISSON_DISK_SAMPLE_DISTRIBUTION_H_
#define RT_RENDER_POISSON_DISK_SAMPLE_DISTRIBUTION_H_

#include "pixelSampleDistribution.h"

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
      std::vector<glm::vec2> m_samples;

    public:
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
      std::vector<glm::vec2> getSamples() const;

    private:
      void m_generateSamples(double r);
  };
} }

#endif
