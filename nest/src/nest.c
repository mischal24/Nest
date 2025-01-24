#include "nest.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

// Trace

// General

float lerpf(float a, float b, float t) {
    return a + t * (b - a);
}

int lerpi(int a, int b, float t) {
    return (int)(a + t * (b - a) + 0.5f);
}

// States
static state current;

void setCurrentState(stateFunction init, stateFunction update, stateFunction exit) {
    if (current.exit) {
        current.exit(NULL);
    }

    current.init = init ? init : NULL;
    current.update = update ? update : NULL;
    current.exit = exit ? exit : NULL;

    if (current.init) {
        current.init(NULL);
    }
}

state* getCurrentState() {
    return &current;
}

// Loop

int initNest(nest* n, const char* title, int width, int height) {
    if (!n) {
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    else {
        n->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

        if (!n->window) {
            return -1;
        }

        n->surface = SDL_GetWindowSurface(n->window);

        if (!n->surface) {
            return -1;
        }

        n->renderer = SDL_CreateRenderer(n->window, -1, SDL_RENDERER_SOFTWARE);

        if (!n->renderer) {
            SDL_DestroyWindow(n->window);
            return -1;
        }
    }

    return 0;
}

void runNest(nest* n) {
    if (n) {
        SDL_UpdateWindowSurface(n->window);

        int running = 1;
        while(running)
        {
            if (current.update) {
                current.update(NULL);
            }

            SDL_Event e;
            while(SDL_PollEvent(&e) > 0)
            {
                SDL_UpdateWindowSurface(n->window);

                if (e.type == SDL_QUIT) {
                    running = 0;
                }
            }

            SDL_RenderClear(n->renderer);
            SDL_RenderPresent(n->renderer);
        }
    }
}

void cleanNest(nest* n) {
    if (n) {
        if (current.exit) {
            current.exit(NULL);
        }

        SDL_DestroyRenderer(n->renderer);
        SDL_DestroyWindow(n->window);
        SDL_Quit();
    }
}

// Vector

vector2 vectorZero() {
    vector2 temp;
    temp.x = 0.0f;
    temp.y = 0.0f;
    return temp;
}

vector2 vectorUp() {
    vector2 temp;
    temp.x = 0.0f;
    temp.y = 1.0f;
    return temp;
}

vector2 vectorRight() {
    vector2 temp;
    temp.x = 1.0f;
    temp.y = 0.0f;
    return temp;
}

void vectorSet(vector2* a, float x, float y) {
    if (a) {
        a->x = x;
        a->y = y;
    }
}

void vectorNeg(vector2* a) {
    if (a) {
        a->x = -a->x;
        a->y = -a->y;
    }
}

void vectorAdd(vector2* a, vector2* b1, vector2* b2) {
    if (a && b1 && b2) {
        a->x = b1->x + b2->x;
        a->y = b1->y + b2->y;
    }
}

void vectorSub(vector2* a, vector2* b1, vector2* b2) {
    if (a && b1 && b2) {
        a->x = b1->x - b2->x;
        a->y = b1->y - b2->y;
    }
}

void vectorNormalize(vector2* a) {
    if (a) {
        float length = sqrtf(powf(a->x, 2) + powf(a->x, 2));

        if (length <= 0) {
            vectorZero(a);
        }
        else {
            a->x = (a->x / length);
            a->y = (a->y / length);
        }
    }
}

void vectorScale(vector2* a, float scale) {
    if (a) {
        a->x = a->x * scale;
        a->y = a->y * scale;
    }
}

float vectorLength(vector2* a) {
    if (a) {
        return sqrtf(powf(a->x, 2) + powf(a->x, 2));
    }

    return 0.0f;
}

float vectorDistance(vector2* a, vector2* b) {
    if (a && b) {
        float dx = b->x - a->x;
        float dy = b->y - a->y;

        return sqrtf(powf(dx, 2) + powf(dy, 2));
    }

    return 0.0f;
}

float vectorDotProd(vector2* a, vector2* b) {
    if (a && b) {
        return (a->x * b->x) + (a->y * b->y);
    }

    return 0.0f;
}

void vectorLerp(vector2* result, vector2* a, vector2* b, float t) {
    if (result && a && b) {
        result->x = lerpf(a->x, b->x, t);
        result->y = lerpf(a->y, b->y, t);
    }
}

// Angles

float angleDegtoRad(angle deg) {
    if (deg) {
        return *deg * (M_PI/180);
    }

    return 0.0f;
}

float angleRadtoDeg(angle rad) {
    if (rad) {
        return *rad * (180/M_PI);
    }

    return 0.0f;
}

float angleShortestDistance(angle a, angle b) {
    float diff = fmodf(*b - *a + 180.0f, 360.0f) - 180.0f;
    return diff < -180.0f ? diff + 360.0f : diff;
}

float angleShortestDistanceRad(angle a, angle b) {
    float diff = fmodf(*b - *a + M_PI, 2 * M_PI) - M_PI;
    return diff < -M_PI ? diff + 2 * M_PI : diff;
}

float angleNormalizeDeg(angle deg) {
    return fmodf(*deg + 360.0f, 360.0f);
}

float angleNormalizeRad(angle rad) {
    return fmodf(*rad + 2 * M_PI, 2 * M_PI);
}

float angleNormalizeDegSigned(angle deg) {
    float normalized = fmodf(*deg + 180.0f, 360.0f) - 180.0f;
    return normalized < -180.0f ? normalized + 360.0f : normalized;
}

float angleNormalizeRadSigned(angle rad) {
    float normalized = fmodf(*rad + M_PI, 2 * M_PI) - M_PI;
    return normalized < -M_PI ? normalized + 2 * M_PI : normalized;
}

void angleToVector(vector2* v, angle deg) {
    if (v) {
        float rad = *deg * (M_PI / 180.0f);
        v->x = cosf(rad);
        v->y = sinf(rad);
    }
}

void angleToVectorRad(vector2* v, angle rad) {
    if (v) {
        v->x = cosf(*rad);
        v->y = sinf(*rad);
    }
}

float angleLerp(angle a, angle b, float t) {
    float shortest = angleShortestDistance(a, b);
    return *a + t * shortest;
}

float angleLerpRad(angle a, angle b, float t) {
    float shortest = angleShortestDistanceRad(a, b);
    return *a + t * shortest;
}

// Color

color rgb(int r, int g, int b) {
    color c;

    c.r = (Uint8)r;
    c.g = (Uint8)g;
    c.b = (Uint8)b;

    return c;
}

color hex(char* h) {
    color c;

    if (h[0] == '#') {
        h++;
    }

    if (sscanf(h, "%2hhx%2hhx%2hhx", &c.r, &c.g, &c.b) != 3) {
        c.r = c.g = c.b = 0;
    }

    return c;
}

// Primitives

primitive createPrimitive(primitiveType type, vector2 position, angle rotation, color color) {
    primitive p;
    p.type = type;
    p.position = position;
    p.rotation = rotation;
    p.color = color;

    switch (type) {
        case PRIMITIVE_RECT:
            p.shape.rect.width = 100.0f;
            p.shape.rect.height = 100.0f;
            break;
        case PRIMITIVE_CIRCLE:
            p.shape.circle.radius = 50.0f;
            p.shape.circle.segments = 12;
            break;
        case PRIMITIVE_TRIANGLE:
            p.shape.triangle.base = 100.0f;
            p.shape.triangle.height = 100.0f;
            break;
    }

    return p;
}

void drawPrimitive(nest* n, primitive* p) {
    if (n && p) {
        switch (p->type) {
        case PRIMITIVE_RECT:
            SDL_Rect rect = {p->position.x, p->position.y, p->shape.rect.width, p->shape.rect.height};
            SDL_FillRect(n->surface, &rect, SDL_MapRGB(n->surface->format, p->color.r, p->color.g, p->color.b));
            break;
        case PRIMITIVE_CIRCLE:
            break;
        case PRIMITIVE_TRIANGLE:
            break;
        }
    }
}

// Textures

// Entities

// Collision

// Gravity

// Camera

// Input

// Audio
