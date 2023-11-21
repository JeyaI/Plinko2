#pragma once

#include "model.h"
#include "view.h"
#include "controller.h"

class MVCApp{
private:
    Model* model;
    View* view;
    Controller* controller;
    SDL_Event event;
public:
    MVCApp(Model* m, View* v, Controller* c){
        model = m;
        view = v;
        controller = c;
        event = SDL_Event{};
    }
    void step(){
        controller->handleEvents(event);
        model->step();
        view->renderFrame();
    }//check_for_new_input();think_about_stuff();draw_the_next_frame();
    bool shouldExit(){
        return controller->shouldExit();
    }
};
