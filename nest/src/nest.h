#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} newNest;

int init_nest(newNest* engine, const char* title, int width, int height);
void clean_up(newNest* engine);
void render_loop(newNest* engine);

#endif