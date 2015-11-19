#ifndef PREVIEW_WINDOW_H_
#define PREVIEW_WINDOW_H_

#include <memory>

#include <SDL.h>

namespace rt { namespace render {
  class PreviewWindow {
    private:
      SDL_Window *m_window;
      SDL_Renderer *m_renderer;
    public:
      PreviewWindow();
      ~PreviewWindow();

      void openWindow(int width, int height);
      void waitForCloseWindow();

      void drawPixel(int x, int y, int r, int g, int b);

    private:
      void m_update();
  };

  typedef std::shared_ptr<PreviewWindow> PreviewWindowPtr;
} }

#endif
