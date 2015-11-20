#ifndef RT_RENDER_PREVIEW_WINDOW_DEBUG_STRATEGY_H_
#define RT_RENDER_PREVIEW_WINDOW_DEBUG_STRATEGY_H_

#include <algorithm>

#include "previewWindow.h"

namespace rt { namespace render {
  template <class ImageListener,
            class PassListener,
            class SubimageListener,
            class PixelListener,
            class SampleListener,
            class RayListener>
  // TODO: Rename this class to something like "PreviewWindowDebugStrategy" and
  // then make a more concrete PreviewWindow class that is not a template to
  // simplify static linkage issues. Pass PreviewWindow to the listener classes
  // as needed. ImageListner can construct a PreviewWindow with no problem.
  class PreviewWindowDebugStrategy : public ImageListener, public PassListener, public SubimageListener, public PixelListener, public SampleListener, public RayListener
  {
    private:
      PreviewWindowPtr m_previewWindow;

      PreviewWindowDebugStrategy(PreviewWindowPtr previewWindow);
    public:
      ~PreviewWindowDebugStrategy();

      static PreviewWindowDebugStrategy getDebugStrategy() {
        PreviewWindowPtr previewWindow(new PreviewWindow);
        return PreviewWindowDebugStrategy(previewWindow);
      }
  };

  class PreviewWindowImageListener {
    private:
      PreviewWindowPtr m_previewWindow;
    public:
      PreviewWindowImageListener(PreviewWindowPtr previewWindow)
        : m_previewWindow(previewWindow) {}
      void startImage(const ImagePtr &image) {
        // Open a window to show render progress
        m_previewWindow->openWindow(image->width(), image->height());
      }

      void endImage(const ImagePtr &image) {
        // Block until the user closes the preview window
        m_previewWindow->waitForCloseWindow();
      }
  };

  class PreviewWindowNullImageListener {
    public:
      PreviewWindowNullImageListener(PreviewWindowPtr previewWindow) {}
      void startImage(const ImagePtr image) {}
      void endImage(const ImagePtr image) {}
  };

  class PreviewWindowNullPassListener {
    public:
      PreviewWindowNullPassListener(PreviewWindowPtr previewWindow) {}
      void startPass(const ImagePtr image) {}
      void endPass(const ImagePtr image) {}
  };

  class PreviewWindowNullSubimageListener {
    public:
      PreviewWindowNullSubimageListener(PreviewWindowPtr previewWindow) {}
      void startSubimage(const Subimage &subimage) {}
      void endSubimage(const Subimage &subimage) {}
  };

  class PreviewWindowPixelListener {
    private:
      PreviewWindowPtr m_previewWindow;
    public:
      PreviewWindowPixelListener(PreviewWindowPtr previewWindow)
        : m_previewWindow(previewWindow) {}
      void startPixel(const Pixel &pixel) {}
      void endPixel(const Pixel &pixel) {
        m_previewWindow->drawPixel(
            pixel.x(), -(pixel.y()) + m_previewWindow->height() - 1,
            int(std::min(std::max(pixel.r(), 0.0), 1.0) * 255.0),
            int(std::min(std::max(pixel.g(), 0.0), 1.0) * 255.0),
            int(std::min(std::max(pixel.b(), 0.0), 1.0) * 255.0));
      }
  };

  class PreviewWindowNullPixelListener {
    public:
      PreviewWindowNullPixelListener(PreviewWindowPtr previewWindow) {}
      void startPixel(const Pixel &pixel) {}
      void endPixel(const Pixel &pixel) {}
  };

  class PreviewWindowNullSampleListener {
    public:
      PreviewWindowNullSampleListener(PreviewWindowPtr previewWindow) {}
      void startSample(const Sample &sample) {}
      void endSample(const Sample &sample) {}
  };

  class PreviewWindowNullRayListener {
    public:
      PreviewWindowNullRayListener(PreviewWindowPtr previewWindow) {}
      void startRay(const Ray &ray) {}
      void endRay(const Ray &ray) {}
  };

  template <class ImageListener,
            class PassListener,
            class SubimageListener,
            class PixelListener,
            class SampleListener,
            class RayListener>
  PreviewWindowDebugStrategy<ImageListener, PassListener, SubimageListener, PixelListener, SampleListener, RayListener>::PreviewWindowDebugStrategy(PreviewWindowPtr previewWindow)
    : ImageListener(previewWindow), PassListener(previewWindow), SubimageListener(previewWindow), PixelListener(previewWindow), SampleListener(previewWindow), RayListener(previewWindow)
  {
  }

  template <class ImageListener,
            class PassListener,
            class SubimageListener,
            class PixelListener,
            class SampleListener,
            class RayListener>
  PreviewWindowDebugStrategy<ImageListener, PassListener, SubimageListener, PixelListener, SampleListener, RayListener>::~PreviewWindowDebugStrategy() {
  }
} }

#endif
