#include "view.h"
#include "Circle.h"

View::View(Model* m){
    model = m;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    deltaTime = 0.0;
    timeNow = 0;
    timeLast = 0;
}

void View::renderFrame(){
    timeLast = timeNow;
    timeNow = SDL_GetPerformanceCounter();

    deltaTime = (double)(timeNow - timeLast)/((double)SDL_GetPerformanceFrequency());

    model->setDeltaTime(deltaTime);

    //std::cout << 1.0/(deltaTime + 0.000001) << "\n";    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF); 
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    std::vector<Disk>& disks = model->viewDisks();
    for(unsigned int i = 0; i < disks.size(); i++){
        DrawCircle(renderer, {(int)disks[i].origin.x, (int)disks[i].origin.y}, disks[i].radius);
        //SDL_RenderDrawLine(renderer, (int)disks[i].origin.x, (int)disks[i].origin.y, (int)disks[i].origin.x + (int)(disks[i].velocity.x), (int)disks[i].origin.y + (int)(disks[i].velocity.y));
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    std::vector<LineSegment>& lineSegments = model->viewLineSegments();
    for(unsigned int i = 0; i < lineSegments.size(); i++){
        LineSegment lineSegment = lineSegments[i];
        SDL_RenderDrawLine(renderer, lineSegment.pointA.x, lineSegment.pointA.y, lineSegment.pointB.x, lineSegment.pointB.y); //implicit double to int
    }

    SDL_RenderPresent(renderer);
}