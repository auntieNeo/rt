#ifndef RT_RENDER_RENDERER_H_
#define RT_RENDER_RENDERER_H_

#include "image.h"

namespace rt {
  namespace scene {
    class Scene;
  }

  namespace render {
    template <class SampleStrategy, class DebugStrategy>
    class Renderer {
      public:
        Renderer();
        virtual ~Renderer();

        void renderScene(const scene::Scene &scene, int width = 640, int height = 480); 
    };

    template <class SampleStrategy, class DebugStrategy>
    Renderer<SampleStrategy, DebugStrategy>::Renderer() {
    }

    template <class SampleStrategy, class DebugStrategy>
    Renderer<SampleStrategy, DebugStrategy>::~Renderer() {
    }

    template <class SampleStrategy, class DebugStrategy>
    void Renderer<SampleStrategy, DebugStrategy>::renderScene(
        const scene::Scene &scene, int width, int height)
    {
      DebugStrategy debugStrategy = DebugStrategy::getDebugStrategy();
      ImagePtr image(new Image(width, height));
      debugStrategy.startImage(image);
      SampleStrategy sampleStrategy(width, height);
      size_t passCount = 0;
      size_t sampleCount = 0;
      // Loop until the sample strategy determines that we should stop
      while (!sampleStrategy.checkHaltCondition(image, passCount, sampleCount)) {
        // TODO: Give the sample distribution an opportunity to re-calculate
        // the sample distribution and the sample distribution here.
        debugStrategy.startPass(image);
        // Loop through each subimage, assigning each to a thread
        for (auto subimage : sampleStrategy.subimageWalker(image)) {
          debugStrategy.startSubimage(subimage);
          // Loop through each pixel in this subimage
          for (auto pixel : sampleStrategy.pixelWalker(subimage)) {
            debugStrategy.startPixel(pixel);
            // Loop through each sample in this pixel
            for (auto sample : sampleStrategy.sampleWalker(pixel)) {
              debugStrategy.startSample(sample);
              sampleCount += 1;
              // Find the position of this sample in normalized coordinates
              glm::dvec2 normalizedSamplePos =
                ((pixel.position() + glm::dvec2(0.5, 0.5) + sample.position()) / image->dimensions()) * 2.0 - 1.0;
              // Find the ray direction vector in world space

              // TODO: Cast a ray through this sample

              // TODO: Add the sample's contribution to the pixel
              pixel.value() = glm::vec3(0.0, 1.0, 0.0);
              debugStrategy.endSample(sample);
            }
            debugStrategy.endPixel(pixel);
          }
          debugStrategy.endSubimage(subimage);
        }
        debugStrategy.endPass(image);
        passCount += 1;
      }
      debugStrategy.endImage(image);
    }
  }
}

#endif
