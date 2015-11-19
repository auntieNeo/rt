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
      ImagePtr image(new Image(width, height));
      SampleStrategy sampleStrategy(width, height);
      size_t passCount = 0;
      size_t sampleCount = 0;
      // Loop until the sample strategy determines that we should stop
      while (!sampleStrategy.checkHaltCondition(image, passCount, sampleCount)) {
        // Loop through each subimage, assigning each to a thread
        for (auto subimage : sampleStrategy.subimageWalker(image)) {
          // Loop through each pixel in this subimage
          for (auto pixel : sampleStrategy.pixelWalker(subimage)) {
            // Loop through each sample in this pixel
            for (auto sample : sampleStrategy.sampleWalker(pixel)) {
              sampleCount += 1;
              // TODO: Cast a ray for this sample
              fprintf(stderr, "pixel<%d, %d>, sample<%g, %g>\n",
                  pixel.x(), pixel.y(),
                  sample.x(), sample.y());
            }
          }
        }
        passCount += 1;
      }
    }
  }
}

#endif
