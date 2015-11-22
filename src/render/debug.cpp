#include <cstdio>

#include <SDL.h>

#include "pixelSampleDistribution.h"

#include "debug.h"

namespace rt { namespace render {
  const int WINDOW_WIDTH = 500;
  const int WINDOW_HEIGHT = WINDOW_WIDTH;

  void displaySampleDistribution(
      const PixelSampleDistribution &dist,
      const char *title)
  {
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Make an SDL window and rendering context
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0);
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE);

    // Clear the render window to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw sample points in green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (Sample sample : dist.getSamples()) {
      // Print out the sample location for debugging
      fprintf(stderr, "x: %f, y: %f\n", sample.x(), sample.y());
      // Draw a point for each sample in the pixel
      SDL_RenderDrawPoint(renderer,
          sample.x() * WINDOW_WIDTH,
          sample.y() * WINDOW_HEIGHT);
    }

    // Update the front buffer
    SDL_RenderPresent(renderer);

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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
} }
