#ifndef RT_RENDER_SIMPLE_HALTING_STRATEGY_H_
#define RT_RENDER_SIMPLE_HALTING_STRATEGY_H_

#include "image.h"

namespace rt { namespace render {
  class SimpleHaltingStrategy {
    public:
      static bool checkHaltCondition(
          ImagePtr image, size_t passCount, size_t sampleCount)
      {
        return passCount > 0;
      }
  };
} }

#endif
