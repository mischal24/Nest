#define SDL_MAIN_HANDLED 

#include "nest.h"

int main() {
    newNest n;

    if (init_nest(&n, "New VSCode Nest Project", 800, 600) != 0) {
        return -1;
    }

    render_loop(&n);
    clean_up(&n);
    return 0;
}