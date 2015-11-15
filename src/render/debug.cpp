#include <cstdio>

#include <SDL.h>

#include "pixelSampleDistribution.h"

#include "debug.h"

namespace rt { namespace render {
  const int WINDOW_WIDTH = 500;
  const int WINDOW_HEIGHT = WINDOW_WIDTH;

  void displaySampleDistribution(const PixelSampleDistribution &dist) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Make an SDL window and rendering context
    SDL_CreateWindowAndRenderer(
        WINDOW_WIDTH, WINDOW_HEIGHT,  // Window dimensions
        0,  // Window flags
        &window, &renderer);

    // Clear the render window to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw sample points in green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (glm::vec2 sample : dist.getSamples()) {
      // Print out the sample location for debugging
      fprintf(stderr, "x: %f, y: %f\n", sample.x, sample.y);
      // Draw a cross for each sample in the pixel
      SDL_RenderDrawPoint(renderer,
          (sample.x + 1.0) / 2.0 * WINDOW_WIDTH,
          (sample.y + 1.0) / 2.0 * WINDOW_HEIGHT);
    }

    // Update the front buffer
    SDL_RenderPresent(renderer);

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
