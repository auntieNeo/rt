#ifndef RT_RENDER_DEBUG_STRATEGY_H_
#define RT_RENDER_DEBUG_STRATEGY_H_

namespace rt { namespace render {
  template <class SubimageListener,
            class PixelListener,
            class SampleListener,
            class RayListener>
  class DebugStrategy {
    public:
      DebugStrategy();
      ~DebugStrategy();

      auto subimageListener() {
        return SubimageListener();
      }

      auto pixelListener() {
        return PixelListener();
      }

      auto sampleListener() {
        return SampleListener();
      }

      auto rayListener() {
        return RayListener();
      }
  };
} }

#endif
