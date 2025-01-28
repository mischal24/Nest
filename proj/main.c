#define SDL_MAIN_HANDLED 

#include "nest.h"
#include <stdio.h>
#include <math.h>

primitive s;

void testA() {
    printf("hello world!\n");

    s = newRectangle(vectorZero(), 100, 100, rgb(255, 255, 255));
}

void testB() {
    drawPrimitive(&s);
    textureBind(&s.base, textureLoad("nestLogo.png"));

    static float seconds = 0;
    seconds += (100 * deltaTime());
    seconds = fmod(seconds, 110);

    primitive c = newCircle((vector2){ 50, 150 }, 50, (int)((seconds / 10) + 5), rgb(255, 255, 255));
    drawPrimitive(&c);

    primitive t = newTriangle((vector2){ 0, 285 }, 100, 85, -50 + seconds, rgb(255, 255, 255));
    drawPrimitive(&t);
    
    primitive l = newLine((vector2){ 0, 285 }, (vector2){ 100 - seconds, 385 - seconds }, 0, rgb(255, 255, 255));
    drawPrimitive(&l);
}

void testC() {
    printf("goodbye world!\n");

    textureUnbind(&s.base);
}

int main() {
    nest n;

    if (initNest(&n, "New VSCode Nest Project", 800, 600) != 0) {
        return -1;
    }

    setBackgroundColor(hex("#1e3f45"));
    
    setCurrentState(&testA, &testB, &testC);

    runNest();
    cleanNest();
    
    return 0;
}