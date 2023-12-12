#include "controller.h"

void Controller::handleEvents(SDL_Event& e){
    while(SDL_PollEvent(&e) != 0){
        const uint8_t* keys = SDL_GetKeyboardState(nullptr);
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseX >= 580){
            mouseX = 580;
        }
        if(mouseX <= 60){
            mouseX = 60;
        }
        model->viewDiskPreview().origin.x = (double)mouseX;
        model->viewDiskPreview().origin.y = 40.0;
        switch(e.syswm.type){
            case SDL_QUIT:
                exitFlag = true;
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_r:
                        model->reset();
                        break;
                    default:
                        break;
                }
                model->go = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if(model->viewDisksToPlace() > 0){
                    model->dropDisk({{(double)mouseX, 40}, 20, {0.0, 0.0}, false, false});
                }
        }
    }
}