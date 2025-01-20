#include "nest.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int init_nest(newNest* engine, const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!engine->window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);
    if (!engine->renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

void clean_up(newNest* engine) {
    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    SDL_Quit();
}

void render_loop(newNest* engine) {
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
        SDL_RenderClear(engine->renderer);
        SDL_RenderPresent(engine->renderer);
    }
}