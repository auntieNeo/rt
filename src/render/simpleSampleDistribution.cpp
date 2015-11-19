#include "simpleSampleDistribution.h"

namespace rt { namespace render {
  SimpleSampleDistribution::SimpleSampleDistribution() {
  }

  SimpleSampleDistribution::~SimpleSampleDistribution() {
  }

  SimpleSampleDistribution::Iterator::Iterator(int index) {
    m_index = index;
  }

  SimpleSampleDistribution::Iterator::~Iterator() {
  }
} }
