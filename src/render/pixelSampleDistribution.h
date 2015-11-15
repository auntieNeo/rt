#ifndef RT_RENDER_PIXEL_SAMPLE_DISTRIBUTION_H_
#define RT_RENDER_PIXEL_SAMPLE_DISTRIBUTION_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace rt { namespace render {
  /**
   * This class describes the sample distribution over a single pixel.
   *
   * The PixelSampleDistribution class generates a number of sample locations
   * over the area of a pixel, from 0.0 to 1.0 in both dimensions. The samples
   * might also be somewhat past the pixel boundries.
   *
   * The PixelSampleDistribution class is intended to be instantiated by a
   * SampleScheme class; the SampleScheme class is a collection of pixel sample
   * distributions that cover the entire image.
   */
  class PixelSampleDistribution {
    public:
      /**
       * Virtual destructor for PixelSampleDistribution objects.
       */
      virtual ~PixelSampleDistribution();

      /**
       * Returns a list of sample locations for this sample distribution.
       *
       * Implementing classes must generate a list of sample locations and
       * return them with this method. The each sample location should
       * generally be within the 0.0 to 1.0 boundries of a pixel, but they may
       * go slightly beyond those boundries.
       */
      virtual std::vector<glm::vec2> getSamples() const = 0;
  };

  typedef std::shared_ptr<PixelSampleDistribution> PixelSampleDistributionPtr;
} }

#endif
