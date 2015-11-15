#include <cstdlib>
#include <memory>

#include "render/debug.h"
#include "render/poissonDiskSampleDistribution.h"

int main(int argc, char **argv) {
  double radii[] = { 0.3, 0.1, 0.05, 0.01 };
  for (double r : radii) {
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
