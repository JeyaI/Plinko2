#pragma once

#include "model.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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
