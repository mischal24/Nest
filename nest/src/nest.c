#include "nest.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Trace

// Open
// Message
// Close

// General

float deltaTime(void) {
    static Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    float t = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    return t;
}

float lerpf(float a, float b, float t) {
    return a + t * (b - a);
}

int lerpi(int a, int b, float t) {
    return (int)(a + t * (b - a) + 0.5f);
}

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

state* getCurrentState(void) {
    return &current;
}

// Loop

static color backgroundColor;
int imgFlags;

void setBackgroundColor(color c) {
    backgroundColor = c;
}

static nest* initializedNest;

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

        n->renderer = SDL_CreateRenderer(n->window, -1, SDL_RENDERER_ACCELERATED);

        if (!n->renderer) {
            return -1;
        }

        imgFlags = IMG_INIT_PNG;
        
        if (!(IMG_Init(imgFlags) && imgFlags)) {
            return -1;
        }
    }

    backgroundColor = rgb(0, 0, 0);

    initializedNest = n;

    return 0;
}

void runNest(void) {
    if (initializedNest) {

        bool running = TRUE;

        while(running)
        {
            SDL_RenderClear(initializedNest->renderer);

            if (current.update) {
                current.update(NULL);
            }

            SDL_Event e;
            while(SDL_PollEvent(&e) > 0)
            {
                if (e.type == SDL_QUIT) {
                    running = FALSE;
                }
            }

            SDL_SetRenderDrawColor(initializedNest->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
            SDL_RenderPresent(initializedNest->renderer);
        }
    }
}

void cleanNest(void) {
    if (initializedNest) {
        if (current.exit) {
            current.exit(NULL);
        }

        SDL_DestroyRenderer(initializedNest->renderer);
        SDL_DestroyWindow(initializedNest->window);
        IMG_Quit();
        SDL_Quit();
    }
}

// Vector

vector2 vectorZero(void) {
    return (vector2){ 0, 0 };
}

vector2 vectorUp(void) {
    return (vector2){ 0, 1 };
}

vector2 vectorRight(void) {
    return (vector2){ 1, 0 };
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
            vectorZero();
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

// Entities

entity entityInit(int id, vector2 pos) {
    entity e;
    e.id = id;
    e.tex = NULL;
    e.position = pos;
    e.isActive = TRUE;
    return e;
}

void entityActive(entity *e, bool isActive) {
    if (isActive == FALSE) {
        e->isActive = FALSE;
    } else {
        e->isActive = TRUE;
    }
}

bool entityIsActive(const entity *e) {
    return e->isActive;
}

bool entityCompare(const entity *a, const entity *b) {
    return a->id == b->id &&
           a->position.x == b->position.x &&
           a->position.y == b->position.y &&
           a->isActive == b->isActive;
}

// Primitives

primitive newRectangle(vector2 position, float width, float height, color color) {
    entity e = entityInit(rand(), vectorZero());
    primitive p;
    p.base = e;
    p.type = RECTANGLE;
    p.color = color;
    p.base.position = position;
    p.rectangle.width = width;
    p.rectangle.height = height;
    return p;
}

primitive newCircle(vector2 position, float radius, int segments, color color) {
    entity e = entityInit(rand(), vectorZero());
    primitive p;
    p.base = e;
    p.type = CIRCLE;
    p.color = color;
    p.base.position = position;
    p.circle.radius = radius;
    p.circle.segments = segments;
    return p;
}

primitive newTriangle(vector2 position, float base, float height, float skew, color color) {
    entity e = entityInit(rand(), vectorZero());
    primitive p;
    p.base = e;
    p.type = TRIANGLE;
    p.color = color;
    p.base.position = position;
    p.triangle.base = base;
    p.triangle.height = height;
    p.triangle.skew = skew;
    return p;
}

primitive newLine(vector2 pointA, vector2 pointB, float width, color color) {
    entity e = entityInit(rand(), vectorZero());
    primitive p;
    p.base = e;
    p.type = LINE;
    p.color = color;
    p.base.position = pointA;
    p.line.endPoint = pointB;
    p.line.width = width;
    return p;
}

void drawPrimitive(primitive* p) {
    switch (p->type) {
        case RECTANGLE: {
            SDL_Rect rect = {
                (int)p->base.position.x, 
                (int)p->base.position.y,
                (int)p->rectangle.width,
                (int)p->rectangle.height
            };
            SDL_SetRenderDrawColor(initializedNest->renderer,
                                   p->color.r,
                                   p->color.g,
                                   p->color.b,
                                   255);
            SDL_RenderDrawRect(initializedNest->renderer, &rect);
            break;
        }
        
        case CIRCLE: {
            int cx = (int)p->base.position.x;
            int cy = (int)p->base.position.y;
            int radius = (int)p->circle.radius;
            
            SDL_SetRenderDrawColor(initializedNest->renderer,
                                   p->color.r,
                                   p->color.g,
                                   p->color.b,
                                   255);
            
            for (int i = 0; i < p->circle.segments; i++) {
                float angle1 = 2.0f * 3.14159f * i / p->circle.segments;
                float angle2 = 2.0f * 3.14159f * (i + 1) / p->circle.segments;
                
                int x1 = (int)(cx + radius * cos(angle1));
                int y1 = (int)(cy + radius * sin(angle1));
                int x2 = (int)(cx + radius * cos(angle2));
                int y2 = (int)(cy + radius * sin(angle2));
                
                SDL_RenderDrawLine(initializedNest->renderer, x1, y1, x2, y2);
            }
            break;
        }
        
        case TRIANGLE: {
            SDL_Point points[3] = {
                { (int)p->base.position.x, (int)p->base.position.y },
                { (int)(p->base.position.x + p->triangle.base), (int)p->base.position.y },
                { (int)(p->base.position.x + p->triangle.base / 2 + p->triangle.skew), (int)(p->base.position.y - p->triangle.height) }
            };
            
            SDL_SetRenderDrawColor(initializedNest->renderer,
                                   p->color.r,
                                   p->color.g,
                                   p->color.b,
                                   255);
            
            SDL_RenderDrawLines(initializedNest->renderer, points, 3);
            SDL_RenderDrawLine(initializedNest->renderer, points[2].x, points[2].y, points[0].x, points[0].y);
            break;
        }
        
        case LINE: {
            SDL_SetRenderDrawColor(initializedNest->renderer,
                                   p->color.r,
                                   p->color.g,
                                   p->color.b,
                                   255);
            SDL_RenderDrawLine(initializedNest->renderer,
                               (int)p->base.position.x,
                               (int)p->base.position.y,
                               (int)p->line.endPoint.x,
                               (int)p->line.endPoint.y);
            break;
        }

        default:
            break;
    }
}

// Textures

texture textureLoad(char const *path) {
    SDL_Texture* t = NULL;
    SDL_Surface* s = IMG_Load(path);
    
    if (!s) {
        return NULL;
    }

    t = SDL_CreateTextureFromSurface(initializedNest->renderer, s);
    SDL_FreeSurface(s);

    if (!t) {
        return NULL;
    }

    return t;
}

bool textureBind(entity* e, texture t)
{
    if (!t) {
        return FALSE;
    }

    e->tex = t;

    int w, h;
    SDL_QueryTexture(e->tex, NULL, NULL, &w, &h);

    SDL_Rect r;
    r.x = e->position.x;
    r.y = e->position.y;
    r.w = w;
    r.h = h;

    SDL_RenderCopy(initializedNest->renderer, e->tex, NULL, &r);

    return TRUE;
}

void textureUnbind(entity* e) {
    if (e->tex != NULL) {
        SDL_DestroyTexture(e->tex);
        e->tex = NULL;
    }
}

// Animations

// Collision

// Gravity

// Camera

// Input

// Audio

// Particles

// Shaders