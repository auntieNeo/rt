#ifndef RT_RENDER_RENDERER_H_
#define RT_RENDER_RENDERER_H_

#include <memory>
#include <thread>

#define GLM_SWIZZLE
#include <glm/glm.hpp>

#include <omp.h>

#include "../ray.h"
#include "../scene/drawableObject.h"
#include "image.h"
#include "renderTask.h"
#include "workerPool.h"

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
      // TODO: Allow user configuration of the number of workers
      WorkerPool workers(std::thread::hardware_concurrency());
      std::shared_ptr<DebugStrategy> debugStrategy = DebugStrategy::getDebugStrategy();
      ImagePtr accumulatorImage(new Image(width, height)),  // Accumulates the result
               passImage(new Image(width, height)),  // Buffer for intermediate passes
               previewImage(new Image(width, height));  // Preview for display
      debugStrategy->startImage(previewImage);
      std::shared_ptr<SampleStrategy> sampleStrategy(
          new SampleStrategy(width, height));
      size_t passCount = 0;
      size_t sampleCount = 0;

      // Loop until the sample strategy determines that we should stop
      while (!sampleStrategy->checkHaltCondition(accumulatorImage, passCount, sampleCount)) {
        // TODO: Give the sample strategy an opportunity to re-calculate the
        // sample distribution here.
        debugStrategy->startPass(previewImage);
        // Loop through each subimage, assigning each to a thread
        for (auto subimage : sampleStrategy->subimageWalker(passImage)) {
          // Dispatch the task of rendering this subimage to a worker thread
          TaskPtr renderTask(
              new RenderTask<SampleStrategy, DebugStrategy>(
                &scene, &camera,
                subimage, accumulatorImage, previewImage,
                sampleStrategy, debugStrategy,
                passCount + 1));
          workers.dispatch(renderTask);
        }
        debugStrategy->endPass(previewImage);
        passCount += 1;
      }
      // TODO: Block until the image has been rendered?
      // TODO: Apply high dynamic range filter
      // TODO: Apply gamma correction
      debugStrategy->endImage(previewImage);
    }
  }
}

#endif
