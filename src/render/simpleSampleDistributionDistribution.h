#ifndef RT_RENDER_SIMPLE_SAMPLE_DISTRIBUTION_DISTRIBUTION_H_
#define RT_RENDER_SIMPLE_SAMPLE_DISTRIBUTION_DISTRIBUTION_H_

#include "pixel.h"

namespace rt { namespace render {
  template <class SampleDistribution>
  class SimpleSampleDistributionDistribution {
    private:
      SampleDistribution m_sampleDistribution;
    public:
      SimpleSampleDistributionDistribution(int width, int height);
      ~SimpleSampleDistributionDistribution();

      SampleDistribution atPixel(int x, int y);
  };

  template <class SampleDistribution>
  SimpleSampleDistributionDistribution<SampleDistribution>::SimpleSampleDistributionDistribution(
      int width, int height)
  {
  }

  template <class SampleDistribution>
  SimpleSampleDistributionDistribution<SampleDistribution>::~SimpleSampleDistributionDistribution()
  {
  }

  template <class SampleDistribution>
  SampleDistribution SimpleSampleDistributionDistribution<SampleDistribution>::atPixel(int x, int y) {
    // Return the same sample distribution for each pixel
    return m_sampleDistribution;
  }
} }

#endif
