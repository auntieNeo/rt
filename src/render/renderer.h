#ifndef RT_RENDER_RENDERER_H_
#define RT_RENDER_RENDERER_H_

#define GLM_SWIZZLE
#include <glm/glm.hpp>

#include <omp.h>

#include "../ray.h"
#include "../scene/drawableObject.h"
#include "image.h"

namespace rt {
  namespace scene {
    class Camera;
    class Scene;
  }

  namespace render {
    template <class SampleStrategy, class DebugStrategy>
    class Renderer {
      public:
        Renderer();
        virtual ~Renderer();

        void renderScene(const scene::Scene &scene, const scene::Camera &camera, int width = 640, int height = 480); 
    };

    template <class SampleStrategy, class DebugStrategy>
    Renderer<SampleStrategy, DebugStrategy>::Renderer() {
    }

    template <class SampleStrategy, class DebugStrategy>
    Renderer<SampleStrategy, DebugStrategy>::~Renderer() {
    }

    template <class SampleStrategy, class DebugStrategy>
    void Renderer<SampleStrategy, DebugStrategy>::renderScene(
        const scene::Scene &scene, const scene::Camera &camera, int width, int height)
    {
      DebugStrategy debugStrategy = DebugStrategy::getDebugStrategy();
      ImagePtr image(new Image(width, height));
      debugStrategy.startImage(image);
      SampleStrategy sampleStrategy(width, height);
      size_t passCount = 0;
      size_t sampleCount = 0;


      glm::mat4 invProj = glm::inverse(camera.projection(double(width) / double(height)));

      // Loop until the sample strategy determines that we should stop
      while (!sampleStrategy.checkHaltCondition(image, passCount, sampleCount)) {
        // TODO: Give the sample distribution an opportunity to re-calculate
        // the sample distribution and the sample distribution here.
        debugStrategy.startPass(image);
        // Loop through each subimage, assigning each to a thread
        for (auto subimage : sampleStrategy.subimageWalker(image)) {
          debugStrategy.startSubimage(subimage);
          // TODO: Each subimage needs a new random number generator whose seed
          // is based on the coordinates of the subimage. This is to preserve
          // determinism across threads.
          // Loop through each pixel in this subimage
          for (auto pixel : sampleStrategy.pixelWalker(subimage)) {
            debugStrategy.startPixel(pixel);
            // Loop through each sample in this pixel
            for (auto sample : sampleStrategy.sampleWalker(pixel)) {
              debugStrategy.startSample(sample);
              // Find the position of this sample in normalized coordinates
              glm::dvec2 pos = ((pixel.position() + sample.position()) / image->dimensions()) * 2.0 - 1.0;
              // Find the ray direction vector in world space
              glm::dvec4 direction =
                glm::dvec4(
                  glm::normalize(
                    glm::dvec3(
                      glm::dvec2(invProj * glm::dvec4(pos, -1.0, 1.0)),
                      -1.0)),
                  0.0);
              // FIXME: Use the sample position and not the camera position
              Ray ray(camera.position(), direction);

              // Walk through the ray tree to compute the radiance for this sample
              glm::dvec3 sampleRadiance = sampleStrategy.computeRadiance(ray, scene);

              // FIXME: Do I need to clamp the sample radiance value?

              // Add this sample's contribution to the pixel
              pixel.value() += sampleRadiance / double(sampleStrategy.numSamples(pixel));

              sampleCount += 1;
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
