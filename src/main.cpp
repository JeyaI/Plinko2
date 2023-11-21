#include <SDL.h>
#include "mvcApp.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

MVCApp* globalApp;

static void loop(void)   /* this will run often, possibly at the monitor's refresh rate */
{
    if (globalApp->shouldExit()) {
        //deinitialize_the_game();
        #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();  /* this should "kill" the app. */
        #else
        exit(0);
        #endif
    }

    globalApp->step();
}

//emcc src/main.cpp src/myApp.cpp -Isrc/include/ROTLIB -sUSE_SDL=2
//g++ -static-libgcc -static-libstdc++ -Isrc/include/SDL2 -Isrc/include -Lsrc/lib -o main src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -O3
int main(int argc, char** argv)
{
    Model m{};
    View v{&m};
    Controller c{&m, &v};

    MVCApp app{&m, &v, &c};
    globalApp = &app;

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
    #else
    while(true){
        loop();
    }
    #endif
}