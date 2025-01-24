#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

typedef void (*stateFunction)(void*);

typedef struct state {
    stateFunction init;
    stateFunction update;
    stateFunction exit;
} state;

void setCurrentState(stateFunction init, stateFunction update, stateFunction exit);
state* getCurrentState();

typedef struct nest {
    SDL_Window* window;
    SDL_Surface* surface;
    SDL_Renderer* renderer;
} nest;

int initNest(nest* n, const char* title, int width, int height);
void runNest(nest* n);
void cleanNest(nest* n);

typedef struct vector2 {
    float x;
    float y;
} vector2;

vector2 vector_zero();
vector2 vector_up();
vector2 vector_right();
void vector_set(vector2* a, float x, float y);
void vector_neg(vector2* a);
void vector_add(vector2* a, vector2* b1, vector2* b2);
void vector_sub(vector2* a, vector2* b1, vector2* b2);
void vector_normalize(vector2* a);
void vector_scale(vector2* a, float scale);
float vector_length(vector2* a);
float vector_distance(vector2* a, vector2* b);
float vector_dotprod(vector2* a, vector2* b);

typedef float (*angle);

float angleDegtoRad(angle deg);
float angleRadtoDeg(angle rad);
float angleShortestDistance(angle a, angle b);
float angleShortestDistanceRad(angle a, angle b);
float angleNormalizeDeg(angle deg);
float angleNormalizeRad(angle rad);
float angleNormalizeDegSigned(angle deg);
float angleNormalizeRadSigned(angle rad);
void angleToVector(vector2* v, angle deg);
void angleToVectorRad(vector2* v, angle rad);
float angleLerp(angle a, angle b, float t);
float angleLerpRad(angle a, angle b, float t);

typedef struct color{
    Uint8 r;
    Uint8 g;
    Uint8 b;
} color;

color rgb(int r, int g, int b);
color hex(char* h);

typedef enum primitiveType {
    PRIMITIVE_RECT,
    PRIMITIVE_CIRCLE,
    PRIMITIVE_TRIANGLE
} primitiveType;

typedef struct primitive {
    primitiveType type;
    
    vector2 position;
    angle rotation;
    color color;

    union shape {
        struct { float width, height; } rect;
        struct { float radius; int segments; } circle;
        struct { float base, height; } triangle;
    } shape;
} primitive;

primitive createPrimitive(primitiveType type, vector2 position, angle rotation, color color);
void drawPrimitive(nest* n, primitive* p);

#endif