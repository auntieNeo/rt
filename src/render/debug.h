#ifndef RT_RENDER_DEBUG_H_
#define RT_RENDER_DEBUG_H_

namespace rt { namespace render {
  class PixelSampleDistribution;

  /**
   * Debug routine used to display a pixel sample distribution with SDL.
   */
  void displaySampleDistribution(
      const PixelSampleDistribution &dist,
      const char *title);
} }

#endif
