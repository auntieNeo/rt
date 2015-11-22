#include <cstdlib>
#include <math.h>
#include <memory>

#include <glm/gtc/quaternion.hpp>

#include "render/debug.h"
#include "render/poissonDiskSampleDistribution.h"

#include "scene/materialProperties.h"
#include "scene/perspectiveCamera.h"
#include "scene/plane.h"
#include "scene/scene.h"
#include "scene/sphere.h"

// TODO: Probably move these includes somewhere else. main.cpp should not be
// concerned with these details.
#include "render/renderer.h"
#include "render/sampleStrategy.h"
#include "render/simpleSubimageWalker.h"
#include "render/simplePixelWalker.h"
#include "render/simpleSampleWalker.h"
#include "render/simpleRayWalker.h"
#include "render/simpleRayCaster.h"
#include "render/simpleSampleDistributionDistribution.h"
#include "render/simpleSampleDistribution.h"
#include "render/simpleHaltingStrategy.h"
#include "render/previewWindowDebugStrategy.h"

int main(int argc, char **argv) {
  rt::scene::Scene scene;

  const int NUM_SPHERES = 3;

  rt::scene::MaterialPropertiesPtr redMaterial(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.8, 0.1, 0.1),  // diffuse
        glm::dvec3(0.1, 0.1, 0.1),  // specular
        600.0  // smoothness
        ));
  rt::scene::MaterialPropertiesPtr grayMaterial(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.1, 0.9, 0.1),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0  // smoothness
        ));

  for (int i = 0; i < NUM_SPHERES; ++i) {
    double pos = (double(i) / double(NUM_SPHERES - 1)) * 2.0 - 1.0;
    pos *= 5.0;
    std::unique_ptr<rt::scene::Sphere> sphere(
        new rt::scene::Sphere(
          8.0,  // radius
          redMaterial,
          glm::dvec4(pos, pos, -40.0 + pos, 1.0)  // position
          ));
    scene.addObject(std::move(sphere));
  }

  std::unique_ptr<rt::scene::Plane> plane(
      new rt::scene::Plane(
        grayMaterial,
        glm::dvec4(0.0, 0.0, -45.0, 1.0),  // position
        glm::angleAxis(-M_PI / 2.0, glm::dvec3(0.0, 1.0, 0.0))));  // orientation
  scene.addObject(std::move(plane));

  std::unique_ptr<rt::scene::PointLight> light(
      new rt::scene::PointLight(
        glm::dvec3(1000.0, 1000.0, 1000.0),      // intensity
        glm::dvec4(0.5, 10.0, -20.0, 1.0)  // position
        ));
  scene.addObject(std::move(light));

  rt::scene::PerspectiveCamera camera(
        1.1,     // fovy
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
      // Simply compute the radiance from the first object hit by each ray
      rt::render::SimpleRayCaster,
      // Use the same sample distribution for each pixel
      rt::render::SimpleSampleDistributionDistribution,
//      // Take a single sample at the center of each pixel
//      rt::render::SimpleSampleDistribution,
      // Take samples in a poisson disk sample distribution at each pixel
      rt::render::PoissonDiskSampleDistribution,
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
