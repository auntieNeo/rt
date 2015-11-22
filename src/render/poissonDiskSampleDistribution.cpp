#include <cassert>
#include <cmath>
#include <random>

#include <cstdio>

#include "poissonDiskSampleDistribution.h"

namespace rt { namespace render {
  PoissonDiskSampleDistribution::PoissonDiskSampleDistribution(double r) {
    // TODO: Obtain a pseudo-random seed?
    // Generate the poisson-disk sample distribution
    m_generateSamples(r);
  }

  PoissonDiskSampleDistribution::~PoissonDiskSampleDistribution() {
  }

  std::vector<Sample> PoissonDiskSampleDistribution::getSamples() const {
    return m_samples;
  }

  void PoissonDiskSampleDistribution::m_generateSamples(double r) {
    // Generate the poisson disk sample locations using the algorithm described
    // in "Fast Poisson Disk Sampling in Arbitrary Dimensions", Bridson

    // Initialize random number generating machinery
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    // Step 0: Initialization
    // The cell_size ensures that each grid cell contains no more than one
    // sample
    double cell_size = r / sqrt(2);
    int grid_size = ceil(1.0f / cell_size);
    cell_size = 1.0f / double(grid_size);
    // The value -1 in the grid indicates no sample in the cell
    std::vector<int> grid(grid_size*grid_size, -1);

    // Step 1: Select an initial sample at random from the domain, initialize
    // the "active list", and add the initial sample to the grid
    glm::vec2 initialSample(
        dist(gen),  // x
        dist(gen)   // y
        );
    m_samples.push_back(Sample(initialSample));
    std::vector<int> activeList;
    activeList.push_back(m_samples.size() - 1);
    int i_initial = floor(initialSample.x * grid_size);
    int j_initial = floor(initialSample.y * grid_size);
    assert(i_initial >= 0);
    assert(i_initial < grid_size);
    assert(j_initial >= 0);
    assert(j_initial < grid_size);
    grid.at(i_initial + j_initial * grid_size) = m_samples.size() - 1;

    // Step 2: Look for samples around each sample in the active list with a
    // "dartboard" method, removing samples from the active list when we can
    // find no more.
    while (!activeList.empty()) {
      // Choose a random sample from the active list
      int i = floor(dist(gen) * activeList.size());
      glm::vec2 activeSample = m_samples.at(activeList.at(i)).position();

      // Generate up to k sample points chosen uniformly from the circular
      // annulus between radius r and 2r around the active sample
      const int k = 30;
      int j;
      for (j = 0; j < k; ++j) {
        // FIXME: The distribution of the radius is not uniform here...
        double r_0 = (r * dist(gen)) + r;
        double theta = dist(gen) * 2.0 * M_PI;
        glm::vec2 newSample(
            activeSample.x + r_0 * cos(theta),
            activeSample.y + r_0 * sin(theta));
        // Determine the cell in the grid that contains the new sample
        int i_new = floor(newSample.x * grid_size);
        int j_new = floor(newSample.y * grid_size);
        if ((i_new < 0) || (i_new >= grid_size) ||
            (j_new < 0) || (j_new >= grid_size))
          continue;  // Discard new samples outside of the grid
        // Determine if this new sample is within radius r of any nearby
        // samples
        bool discard = false;
        for (int i = i_new - 1; (i <= i_new + 1) && !discard; ++i) {
          if ((i < 0) || (i >= grid_size))
            continue;  // Skip indices outside of the grid
          for (int j = j_new - 1; (j <= j_new + 1) && !discard; ++j) {
            if ((j < 0) || (j >= grid_size))
              continue;  // Skip indices outside of the grid
            int neighborSampleIndex = grid.at(i + j * grid_size);
            if (neighborSampleIndex < 0)
              continue;  // Skip empty cells
            glm::vec2 neighborSample = m_samples.at(neighborSampleIndex).position();
            if (i == i_new && j == j_new) {
              // Samples sharing a cell would necessarily be too close
              assert(glm::length(neighborSample - newSample) < r);
              discard = true;
            } else if (glm::length(neighborSample - newSample) < r) {
              // Discard new samples that are too close to neighboring samples
              discard = true;
            }
          }
        }
        if (discard)
          continue;  // Look for another position for the new sample
        // We found a suitable sample; add it to the list of samples and add
        // its index to the active list
        m_samples.push_back(Sample(newSample));
        activeList.push_back(m_samples.size() - 1);
        grid.at(i_new + j_new * grid_size) = m_samples.size() - 1;
        break;
      }
      if (j >= k) {
        // No sample point found after k attempts; remove this sample from the
        // active list
        activeList.erase(activeList.begin() + i);
      }
    }
  }

  PoissonDiskSampleDistribution::Iterator::Iterator(
      int index, const std::vector<Sample> *samples)
    : m_index(index), m_samples(samples)
  {
  }

  PoissonDiskSampleDistribution::Iterator::~Iterator() {
  }
} }
