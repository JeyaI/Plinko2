#include "controller.h"

void Controller::handleEvents(SDL_Event& e){
    while(SDL_PollEvent(&e) != 0){
        const uint8_t* keys = SDL_GetKeyboardState(nullptr);
        switch(e.syswm.type){
            case SDL_QUIT:
                exitFlag = true;
                break;
            case SDL_KEYDOWN:
                model->go = true;
                break;
        }
    }
}