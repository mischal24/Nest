#define SDL_MAIN_HANDLED 

#include "nest.h"
#include <stdio.h>

void testA() {
    printf("hello world!\n");
}

void testB() {
    printf("goodbye world!\n");
}

int main() {
    nest n;

    if (initNest(&n, "New VSCode Nest Project", 800, 600) != 0) {
        return -1;
    }
    
    setCurrentState(&testA, NULL, &testB);

    primitive testS = createPrimitive(PRIMITIVE_RECT, (vector2){0.0f, 0.0f}, 0, hex("#52ba71"));
    drawPrimitive(&n, &testS);

    // primitive testC = createPrimitive(PRIMITIVE_CIRCLE, (vector2){100.0f, 0.0f}, 0, hex("#52ba71"));
    // drawPrimitive(&n, &testC);

    // primitive testT = createPrimitive(PRIMITIVE_TRIANGLE, (vector2){200.0f, 0.0f}, 0, hex("#52ba71"));
    // drawPrimitive(&n, &testT);

    runNest(&n);
    cleanNest(&n);
    
    return 0;
}