#ifndef PREVIEW_WINDOW_H_
#define PREVIEW_WINDOW_H_

#include <memory>

#include <SDL.h>

namespace rt { namespace render {
  class PreviewWindow {
    private:
      SDL_Window *m_window;
      SDL_Renderer *m_renderer;
      int m_width, m_height;
    public:
      PreviewWindow();
      ~PreviewWindow();

      int width() const { return m_width; }
      int height() const { return m_height; }

      void openWindow(int width, int height);
      void waitForCloseWindow();

      void drawPixel(int x, int y, int r, int g, int b);

      void update();
  };

  typedef std::shared_ptr<PreviewWindow> PreviewWindowPtr;
} }

#endif
