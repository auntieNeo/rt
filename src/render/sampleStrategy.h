#ifndef RT_RENDER_SAMPLE_STRATEGY_H_
#define RT_RENDER_SAMPLE_STRATEGY_H_

#include "image.h"
#include "pixel.h"
#include "subimage.h"

namespace rt { namespace render {
  /**
   * This abstract class defines the sampling pattern over the entire image.
   *
   * Classes that implement the SamplingStrategy class will assign a
   * PixelSampleDistribution object to each of the pixels in an image. Most
   * implementations will use instancing of the PixelSampleDistribution classes
   * for pixels that share the same sample distribution.
   */
  template <template<class, class> class SubimageWalker,
            template<class> class PixelWalker,
            template<class> class SampleWalker,
            template<class> class SampleDistributionDistribution,
            class SampleDistribution,
            class HaltingStrategy>
  class SampleStrategy {
    private:
      SampleDistributionDistribution<SampleDistribution> m_samples;
    public:
      SampleStrategy(int width, int height);
      ~SampleStrategy();

      bool checkHaltCondition(
          ImagePtr image, size_t passCount, size_t sampleCount) const
      {
        return HaltingStrategy().checkHaltCondition(image, passCount, sampleCount);
      }

      auto subimageWalker(ImagePtr image) {
        return SubimageWalker<PixelWalker<SampleWalker<SampleDistribution>>, SampleWalker<SampleDistribution>>(image);
      }

      auto pixelWalker(Subimage subimage) {
        return PixelWalker<SampleWalker<SampleDistribution>>(subimage);
      }

      auto sampleWalker(Pixel pixel) {
        return SampleWalker<SampleDistribution>(pixel, m_samples.atPixel(pixel.x(), pixel.y()));
      }
  };

  template <template<class, class> class SubimageWalker,
            template<class> class PixelWalker,
            template<class> class SampleWalker,
            template<class> class SampleDistributionDistribution,
            class SampleDistribution,
            class HaltingStrategy>
  SampleStrategy<SubimageWalker, PixelWalker, SampleWalker,
    SampleDistributionDistribution, SampleDistribution,
    HaltingStrategy>::SampleStrategy(int width, int height)
      : m_samples(width, height)
  {
  }

  template <template<class, class> class SubimageWalker,
            template<class> class PixelWalker,
            template<class> class SampleWalker,
            template<class> class SampleDistributionDistribution,
            class SampleDistribution,
            class HaltingStrategy>
  SampleStrategy<SubimageWalker, PixelWalker, SampleWalker,
    SampleDistributionDistribution, SampleDistribution,
    HaltingStrategy>::~SampleStrategy()
  {
  }
} }

#endif