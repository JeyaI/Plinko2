#pragma once

#include "model.h"
#include "view.h"

class Controller{
protected:
    Model* model;
    View* view;
    bool exitFlag;
public:
    Controller(Model* m, View* v){
        model = m;
        view = v;
        exitFlag = false;
    }
    void handleEvents(SDL_Event& e);
    bool shouldExit(){
        return exitFlag;
    }
};