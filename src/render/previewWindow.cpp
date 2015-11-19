#include "previewWindow.h"

namespace rt { namespace render {
  PreviewWindow::PreviewWindow()
    : m_window(nullptr), m_renderer(nullptr)
  {
  }

  PreviewWindow::~PreviewWindow() {
  }

  void PreviewWindow::openWindow(int width, int height) {
    m_window = SDL_CreateWindow(
        "Render Preview",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        0);
    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    this->m_update();
  }

  void PreviewWindow::drawPixel(int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
    SDL_RenderDrawPoint(m_renderer, x, y);

    this->m_update();
  }

  void PreviewWindow::waitForCloseWindow() {
    // Wait for the user to close the window
    SDL_Event event;
    bool quit = false;
    while (!quit && (SDL_WaitEvent(&event) >= 0)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }

    // Clean up
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
  }

  void PreviewWindow::m_update() {
    SDL_RenderPresent(m_renderer);
  }
} }
