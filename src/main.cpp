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

//emcc src/controller.cpp src/main.cpp src/model.cpp src/physics.cpp src/view.cpp -Isrc/include -sUSE_SDL=2 -o plinko.html -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2
//g++ -static-libgcc -static-libstdc++ -Isrc/include/SDL2 -Isrc/include -Lsrc/lib -o main src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -O3 -lSDL2_mixer
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