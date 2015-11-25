#ifndef RT_RENDER_RENDER_TASK_H_
#define RT_RENDER_RENDER_TASK_H_

#include <cstdio>  // XXX

#include "subimage.h"
#include "task.h"

namespace rt { namespace render {
  template <class SampleStrategy, class DebugStrategy>
  class RenderTask : public Task {
    private:
      Subimage m_subimage;
      ImagePtr m_accumulator, m_preview;
      const scene::Scene *m_scene;
      const scene::Camera *m_camera;
      std::shared_ptr<SampleStrategy> m_sampleStrategy;
      std::shared_ptr<DebugStrategy> m_debugStrategy;
      size_t m_pass;
    public:
      RenderTask(const scene::Scene *scene, const scene::Camera *camera,
                 Subimage subimage, ImagePtr accumulator, ImagePtr preview,
                 std::shared_ptr<SampleStrategy> sampleStrategy,
                 std::shared_ptr<DebugStrategy> debugStrategy,
                 size_t pass)
        : m_scene(scene), m_camera(camera),
        m_subimage(subimage), m_accumulator(accumulator), m_preview(preview),
        m_sampleStrategy(sampleStrategy), m_debugStrategy(debugStrategy),
        m_pass(pass) {}
      ~RenderTask() { m_scene = nullptr;  m_camera = nullptr; }

      void run();

      // Each accumulated pixel is divided by the nominal pass count in a
      // feeble attempt at numeric stability. I'm not sure if it is actually
      // useful, but it might be if floats are used instead of doubles.
      const size_t nominalPassCount = 1;
  };

  template <class SampleStrategy, class DebugStrategy>
  void RenderTask<SampleStrategy, DebugStrategy>::run() {
    m_debugStrategy->startSubimage(m_subimage);

    ImagePtr image = m_subimage.image();

    glm::mat4 invProj = glm::inverse(
        m_camera->projection(double(image->width()) / double(image->height())));

    // Each subimage needs a new random number generator whose seed is
    // based on the coordinates of the subimage. This is to preserve
    // determinism across threads.
    // FIXME: The random seed here is not technically correct. See "Dynamic
    // Creation of Pseudorandom Number Generators", Matumoto
    auto randomEngine = m_sampleStrategy->pseudoRandomNumberEngine(
        (m_subimage.x() + m_subimage.y() * image->width() + 1) * m_pass);
    // Loop through each pixel in this subimage
    for (auto pixel : m_sampleStrategy->pixelWalker(m_subimage)) {
      m_debugStrategy->startPixel(pixel);
      pixel.value() = glm::dvec3(0.0, 0.0, 0.0);
      // Loop through each sample in this pixel
      for (auto sample : m_sampleStrategy->sampleWalker(pixel)) {
        m_debugStrategy->startSample(sample);
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

        // FIXME: Use the sample's position on the viewing plane and not the
        // camera's position for the ray's origin
        Ray ray(m_camera->position(), direction);

        // Walk through the ray tree to compute the radiance for this sample
        glm::dvec3 sampleRadiance =
          m_sampleStrategy->computeRadiance(ray, *m_scene, randomEngine);

        // FIXME: Do I need to clamp the sample radiance value?

        // Add this sample's contribution to the pixel
        pixel.value() += sampleRadiance / double(m_sampleStrategy->numSamples(pixel));

        // TODO: Somehow return the sample count
//        sampleCount += 1;
        m_debugStrategy->endSample(sample);
      }
      // FIXME: These debug calls are certainly not thread safe due to the way
      // the simple drawing API in SDL works
      m_debugStrategy->endPixel(pixel);
    }
    // Add the contribution from this pass to the accumulator
    for (auto pixel : m_subimage) {
      auto accumulatorPixel = m_accumulator->pixelData(pixel.x(), pixel.y());
      *accumulatorPixel += pixel.value() / double(nominalPassCount);
    }
    // Update the preview image with the latest (presumably Monte Carlo) approximation
    Subimage previewSubimage(
        m_preview,
        m_subimage.x(), m_subimage.y(),
        m_subimage.width(), m_subimage.height());
    fprintf(stderr, "m_pass: %d\n", m_pass);
    for (auto pixel : previewSubimage) {
      pixel.value() =
        (*m_accumulator->pixelData(pixel.x(), pixel.y()))
        * (double(nominalPassCount) / double(m_pass));
      //            pixel.value() = glm::dvec3(1.0, 0.0, 1.0);
    }
    m_debugStrategy->endSubimage(previewSubimage);
  }
} }

#endif
