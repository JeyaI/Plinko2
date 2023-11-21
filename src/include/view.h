#pragma once

#include "model.h"
#include <SDL.h>

class View{
protected:
    Model* model;
    SDL_Window* window;
    SDL_Renderer* renderer;
    double deltaTime;
    Uint64 timeNow;
    Uint64 timeLast;
public:
    View(Model* m);
    void renderFrame();
};
