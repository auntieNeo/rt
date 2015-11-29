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
#include "render/gridSubimageWalker.h"
#include "render/simplePixelWalker.h"
#include "render/simpleSampleWalker.h"
#include "render/simpleRayWalker.h"
#include "render/simpleRayCaster.h"
#include "render/simpleRayTracer.h"
#include "render/monteCarloRayTracer.h"
#include "render/simpleSampleDistributionDistribution.h"
#include "render/simpleSampleDistribution.h"
#include "render/simpleHaltingStrategy.h"
#include "render/passCountHaltingStrategy.h"
#include "render/previewWindowDebugStrategy.h"

#include "scene/triangleMesh.h"
#include "scene/triangleMeshBuilder.h"

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
  rt::scene::MaterialPropertiesPtr redDiffuse(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.9, 0.1, 0.1),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0  // smoothness
        ));
  rt::scene::MaterialPropertiesPtr greenDiffuse(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.1, 0.9, 0.1),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0  // smoothness
        ));
  rt::scene::MaterialPropertiesPtr blueDiffuse(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.1, 0.1, 0.9),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0  // smoothness
        ));
  rt::scene::MaterialPropertiesPtr whiteDiffuse(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0xA7, 0x71, 0x43) / (255.0 * 1.5),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0  // smoothness
        ));
  rt::scene::MaterialPropertiesPtr clayMaterial(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.9, 0.9, 0.9),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0  // smoothness
        ));
  rt::scene::MaterialPropertiesPtr mirrorMaterial(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.0, 0.0, 0.0),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0,  // smoothness
        glm::dvec3(1.0, 1.0, 1.0)  // mirror
        ));
  rt::scene::MaterialPropertiesPtr lightMaterial(
      new rt::scene::MaterialProperties(
        glm::dvec3(0.0, 0.0, 0.0),  // ambient
        glm::dvec3(0.0, 0.0, 0.0),  // diffuse
        glm::dvec3(0.0, 0.0, 0.0),  // specular
        0.0,  // smoothness
        glm::dvec3(0.0, 0.0, 0.0),  // mirror
        glm::dvec3(0.0, 0.0, 0.0),  // refraction
        4.0 * glm::dvec3(1.0, 1.0, 1.0)  // emission
//        75.0 * glm::dvec3(1.0, 1.0, 1.0)  // emission
        ));

  // Load the bunny mesh
  rt::scene::TriangleMeshBuilder builder;
  std::unique_ptr<rt::scene::TriangleMesh> bunny =
    builder.build("../sandbox/bunny/reconstruction/bun_zipper.ply",
        clayMaterial,
        glm::dvec3(1.0, 1.0, 1.0) * 100.0,  // scale
        glm::dvec4(2.5, -13.88023, -17.0, 1.0)  // position
        );
  /*
  std::unique_ptr<rt::scene::TriangleMesh> bunny =
    builder.build("../sandbox/test.ply", greenDiffuse,
        glm::dvec3(1.0, 1.0, 1.0) * 5.0,  // scale
        glm::dvec4(4.0, 1.0, 0.0, 1.0),  // position
        glm::angleAxis(M_PI, glm::dvec3(0.0, 1.0, 0.0))
        );
        */
  scene.addObject(std::move(bunny));

  for (int i = 0; i < NUM_SPHERES; ++i) {
    if (i != 2)
      continue;
    double pos = (double(i) / double(NUM_SPHERES - 1)) * 2.0 - 1.0;
    pos *= 5.0;
    std::unique_ptr<rt::scene::Sphere> sphere(
        new rt::scene::Sphere(
          4.0,  // radius
          i == 2 ? lightMaterial : blueDiffuse,
          glm::dvec4(pos, pos, -15.0 + pos, 1.0)  // position
          ));
    scene.addObject(std::move(sphere));
  }

  std::unique_ptr<rt::scene::Plane> backWall(
      new rt::scene::Plane(
        whiteDiffuse,
        glm::dvec4(0.0, 0.0, -20.0, 1.0),  // position
        glm::angleAxis(-M_PI / 2.0, glm::dvec3(0.0, 1.0, 0.0))));  // orientation
  scene.addObject(std::move(backWall));
  std::unique_ptr<rt::scene::Plane> leftWall(
      new rt::scene::Plane(
        whiteDiffuse,
        glm::dvec4(-10.0, 0.0, 0.0, 1.0),  // position
        glm::angleAxis(0.0, glm::dvec3(1.0, 0.0, 0.0))));  // orientation
  scene.addObject(std::move(leftWall));
  std::unique_ptr<rt::scene::Plane> rightWall(
      new rt::scene::Plane(
        whiteDiffuse,
        glm::dvec4(10.0, 0.0, 0.0, 1.0),  // position
        glm::angleAxis(M_PI, glm::dvec3(0.0, 0.0, 1.0))));  // orientation
  scene.addObject(std::move(rightWall));
  std::unique_ptr<rt::scene::Plane> ceiling(
      new rt::scene::Plane(
        whiteDiffuse,
        glm::dvec4(0.0, 10.0, 0.0, 1.0),  // position
        glm::angleAxis(-M_PI / 2.0, glm::dvec3(0.0, 0.0, 1.0))));  // orientation
  scene.addObject(std::move(ceiling));
  std::unique_ptr<rt::scene::Plane> floor(
      new rt::scene::Plane(
        greenDiffuse,
        glm::dvec4(0.0, -10.0, 0.0, 1.0),  // position
        glm::angleAxis(M_PI / 2.0, glm::dvec3(0.0, 0.0, 1.0))));  // orientation
  scene.addObject(std::move(floor));

  std::unique_ptr<rt::scene::PointLight> light(
      new rt::scene::PointLight(
        glm::dvec3(1200.0, 1200.0, 1200.0),      // intensity
        glm::dvec4(-9.0, 9.0, -0.5, 1.0)  // position
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
//      // "Iterate" by selecting the entire image as a subimage
//      rt::render::SimpleSubimageWalker,
      // Iterate through the image in a grid pattern
      rt::render::GridSubimageWalker,
      // Select each pixel in the same order they appear in memory
      rt::render::SimplePixelWalker,
      // Select each sample in the same order they appear in memory
      rt::render::SimpleSampleWalker,
//      // Simply compute the radiance from the first object hit by each ray
//      rt::render::SimpleRayCaster,
//      // Follow the ray recursively for mirror and translucent surfaces
//      rt::render::SimpleRayTracer,
      // Use Monte Carlo method to approximate an integral solution
      rt::render::MonteCarloRayTracer,
      // Use the same sample distribution for each pixel
      rt::render::SimpleSampleDistributionDistribution,
//      // Take a single sample at the center of each pixel
//      rt::render::SimpleSampleDistribution,
      // Take samples in a poisson disk sample distribution at each pixel
      rt::render::PoissonDiskSampleDistribution,
      // Halt after 10000 passes
      rt::render::PassCountHaltingStrategy<10000>,
      // Use 32-bit Mersenne Twister to generate pseudo random numbers
      std::mt19937
    >,
    rt::render::PreviewWindowDebugStrategy<
      rt::render::PreviewWindowImageListener,
      rt::render::PreviewWindowNullPassListener,
      rt::render::PreviewWindowSubimageListener,
      rt::render::PreviewWindowNullPixelListener,
      rt::render::PreviewWindowNullSampleListener,
      rt::render::PreviewWindowNullRayListener
    >
  > simpleRenderer;
  simpleRenderer.renderScene(scene, camera, 500, 500);

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
