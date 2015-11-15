#include <cstdlib>
#include <memory>

#include "render/debug.h"
#include "render/poissonDiskSampleDistribution.h"

int main(int argc, char **argv) {
  // FIXME: I don't know why the stack based dist doesn't work
  rt::render::PixelSampleDistributionPtr dist =
    rt::render::PixelSampleDistributionPtr(new rt::render::PoissonDiskSampleDistribution(0.05));

  rt::render::displaySampleDistribution(*dist);

  return EXIT_SUCCESS;
}
