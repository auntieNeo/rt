#include <cstdlib>
#include <memory>

#include "render/debug.h"
#include "render/poissonDiskSampleDistribution.h"

#include "scene/perspectiveCamera.h"
#include "scene/scene.h"
#include "scene/sphere.h"

// TODO: Probably move these includes somewhere else. main.cpp should not be
// concerned with these details.
#include "render/renderer.h"
#include "render/sampleStrategy.h"
#include "render/simpleSubimageWalker.h"
#include "render/simplePixelWalker.h"
#include "render/simpleSampleWalker.h"
#include "render/simpleSampleDistributionDistribution.h"
#include "render/simpleSampleDistribution.h"
#include "render/simpleHaltingStrategy.h"
#include "render/previewWindowDebugStrategy.h"

int main(int argc, char **argv) {
  rt::scene::Scene scene;

  std::unique_ptr<rt::scene::Sphere> sphere(
      new rt::scene::Sphere(
        1.0,  // 1m radius
        glm::dvec4(0.0, 0.0, 0.0, 1.0)  // Positioned at the origin
        ));
  scene.addObject(std::move(sphere));

  rt::scene::PerspectiveCamera camera(
        1.0,     // fovy
        0.01,    // near
        1000.0,  // far
        glm::dvec4(0.0, 0.0, 5.0, 1.0),  // position
        glm::quat()  // orientation
        );
  // TODO: Put the camera in the scene


  /*
  rt::render::Renderer renderer;
  renderer.renderScene(scene);
  */
  rt::render::Renderer<
    rt::render::SampleStrategy<
      // "Iterate" by selecting the entire image as a subimage
      rt::render::SimpleSubimageWalker,
      // Select each pixel in the same order they appear in memory
      rt::render::SimplePixelWalker,
      // Select each sample in the same order they appear in memory
      rt::render::SimpleSampleWalker,
      // Use the same sample distribution for each pixel
      rt::render::SimpleSampleDistributionDistribution,
      // Take a single sample at the center of each pixel
      rt::render::SimpleSampleDistribution,
      // Halt after taking each sample once
      rt::render::SimpleHaltingStrategy
    >,
    rt::render::PreviewWindowDebugStrategy<
      rt::render::PreviewWindowImageListener,
      rt::render::PreviewWindowNullPassListener,
      rt::render::PreviewWindowNullSubimageListener,
      rt::render::PreviewWindowPixelListener,
      rt::render::PreviewWindowNullSampleListener,
      rt::render::PreviewWindowNullRayListener
    >
  > simpleRenderer;
  simpleRenderer.renderScene(scene, camera, 100, 100);

  /*
  rt::render::Renderer<
    rt::render::SampleStrategy<
      // Select subimages nearer to the center first and radiate outwards
      rt::render::RadialSubimageIterator<
        // Iteratively improve subimages in multiple passes with 5 samples per
        // sample point taken in each pass, round robin style
        rt::render::RoundRobin<5>
      >,
      // Select pixels as if traversing a quad tree (meant to be paired with
      // QuadTreePreview in the DebugStrategy)
      rt::render::QuadTreePixelIterator,
      // Select many samples at a time for performance
      rt::render::BatchSampleIterator
    >,
    rt::render::DebugStrategy<
      // Preview window can draw brackets indicating which subimages are being
      // processed at any given time using this listener.
      rt::render::PreviewWindow::SubimageListener<&g_previewWindow>,
      // Preview pixels drawn with the assumption that pixels will be traversed
      // breadth-first down a quad tree. This has the effect of creating a
      // blurry preview that progressively gets better.
      rt::render::PreviewWindow::QuadTreePixelListener<&g_previewWindow>
    >
  > previewRenderer;
  */

  double radii[] = { 0.3, 0.1, 0.05, 0.01, 0.005 };
  for (double r : radii) {
    break;  // XXX
    rt::render::PixelSampleDistributionPtr dist(
        new rt::render::PoissonDiskSampleDistribution(r));
    const size_t TITLE_SIZE = 256;
    char title[TITLE_SIZE];
    snprintf(title, TITLE_SIZE, "Poisson Disk sample distribution with r=%g", r);
    title[TITLE_SIZE - 1] = '\0';
    rt::render::displaySampleDistribution(*dist, title);
  }



  return EXIT_SUCCESS;
}
