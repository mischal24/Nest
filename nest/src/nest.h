#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

float deltaTime(void);

typedef enum bool {
    FALSE = 0,
    TRUE = 1
} bool;

typedef struct color{
    Uint8 r;
    Uint8 g;
    Uint8 b;
} color;

color rgb(int r, int g, int b);
color hex(char* h);

typedef void (*stateFunction)(void*);

typedef struct state {
    stateFunction init;
    stateFunction update;
    stateFunction exit;
} state;

void setCurrentState(stateFunction init, stateFunction update, stateFunction exit);
state* getCurrentState(void);

typedef struct nest {
    SDL_Window* window;
    SDL_Renderer* renderer;
} nest;

int initNest(nest* n, const char* title, int width, int height);
void setBackgroundColor(color c);
void runNest(void);
void cleanNest(void);

typedef struct vector2 {
    float x;
    float y;
} vector2;

vector2 vectorZero(void);
vector2 vectorUp(void);
vector2 vectorRight(void);
void vectorSet(vector2* a, float x, float y);
void vectorNeg(vector2* a);
void vectorAdd(vector2* a, vector2* b1, vector2* b2);
void vectorSub(vector2* a, vector2* b1, vector2* b2);
void vectorNormalize(vector2* a);
void vectorScale(vector2* a, float scale);
float vectorLength(vector2* a);
float vectorDistance(vector2* a, vector2* b);
float vectorDotProd(vector2* a, vector2* b);

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

typedef struct entity {
    int id;
    SDL_Texture* tex;
    vector2 position;
    bool isActive;
} entity;

typedef enum {
    RECTANGLE,
    CIRCLE,
    TRIANGLE,
    LINE
} shapeType;

typedef struct primitive {
    entity base;
    shapeType type;
    color color;
    union {
        struct { float width; float height; } rectangle;
        struct { float radius; int segments; } circle;
        struct { float base; float height; float skew; } triangle;
        struct { vector2 endPoint; float width; } line;
    };
} primitive;

primitive newRectangle(vector2 position, float width, float height, color color);
primitive newCircle(vector2 position, float radius, int segments, color color);
primitive newTriangle(vector2 position, float base, float height, float skew, color color);
primitive newLine(vector2 pointA, vector2 pointB, float width, color color);
void drawPrimitive(primitive* p);

typedef SDL_Texture (*texture);

texture textureLoad(char const *path);
bool textureBind(entity* e, texture t);
void textureUnbind(entity* e);

#endif