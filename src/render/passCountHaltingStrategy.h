#ifndef RT_RENDER_PASS_COUNT_HALTING_STRATEGY_H_
#define RT_RENDER_PASS_COUNT_HALTING_STRATEGY_H_

#include "image.h"

namespace rt { namespace render {
  template <size_t PASS_COUNT>
  class PassCountHaltingStrategy {
    public:
      static bool checkHaltCondition(
          ImagePtr image, size_t passCount, size_t sampleCount)
      {
        return passCount >= PASS_COUNT;
      }
  };
} }

#endif
