#include <SDL.h>

#include "pixelSampleDistribution.h"

#include "debug.h"

namespace rt { namespace render {
  void displaySampleDistribution(const PixelSampleDistribution &dist) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Make an SDL window and rendering context
    SDL_CreateWindowAndRenderer(
        500, 500,  // Window dimensions
        0,  // Window flags
        &window, &renderer);

    // TODO: Draw a cross for each sample in the pixel

    // Wait for the user to close the window
    SDL_Event event;
    bool quit = false;
    while ((SDL_WaitEvent(&event) >= 0) && !quit) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
} }
