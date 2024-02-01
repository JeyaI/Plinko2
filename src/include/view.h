#pragma once

#include "model.h"
#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#endif

class View{
protected:
    Model* model;
    SDL_Window* window;
    SDL_Renderer* renderer;
    double deltaTime;
    Uint64 timeNow;
    Uint64 timeLast;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    Mix_Chunk* ballCollideSFX;
    int collisionSFXCooldown;
public:
    View(Model* m);
    ~View();
    void renderFrame();
};
