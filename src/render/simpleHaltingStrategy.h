#ifndef SIMPLE_HALTING_STRATEGY_H_
#define SIMPLE_HALTING_STRATEGY_H_

#include "image.h"

namespace rt { namespace render {
  class SimpleHaltingStrategy {
    public:
      SimpleHaltingStrategy();
      ~SimpleHaltingStrategy();

      bool checkHaltCondition(
          ImagePtr image, size_t passCount, size_t sampleCount) const
      {
        return passCount > 0;
      }
  };
} }

#endif
