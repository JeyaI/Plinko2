#include "view.h"
#include "Circle.h"

View::View(Model* m){
    model = m;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    TTF_Init();

    font = TTF_OpenFont("Pixellari.ttf", 40);

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

    ballCollideSFX = Mix_LoadWAV("ballCollision3.wav");

    collisionSFXCooldown = 100;

    deltaTime = 0.0;
    timeNow = 0;
    timeLast = 0;

    scoreTexture = nullptr;
}

void View::renderFrame(){
    if(deltaTime <= 1.0/60.0){
        model->setDeltaTime(deltaTime);
    }else{
        model->setDeltaTime(1.0/60.0);
    }

    timeLast = timeNow;
    timeNow = SDL_GetPerformanceCounter();

    deltaTime = (double)(timeNow - timeLast)/((double)SDL_GetPerformanceFrequency());

    //std::cout << 1.0/(deltaTime + 0.000001) << "\n";    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF); 
    SDL_RenderClear(renderer);

    char scoreString[10];

    if(scoreTexture){
        SDL_DestroyTexture(scoreTexture);
    }

    sprintf(scoreString, "%d", model->viewScore());

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreString/*itoa(model->viewScore(), scoreString, 10)*/, {0xFF,0xFF,0xFF});
    if(!scoreSurface){std::cout << TTF_GetError();};

    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if(!scoreTexture){std::cout << TTF_GetError();};
    SDL_Rect scoreRect{0,0, scoreSurface->w, scoreSurface->h};

    //std::cout << scoreRect.w << " " << scoreRect.h << "\n";

    SDL_FreeSurface(scoreSurface);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    std::vector<Disk>& disks = model->viewDisks();
    for(unsigned int i = 0; i < disks.size(); i++){
        DrawCircle(renderer, {(int)disks[i].origin.x, (int)disks[i].origin.y}, disks[i].radius);
        //SDL_RenderDrawLine(renderer, (int)disks[i].origin.x, (int)disks[i].origin.y, (int)disks[i].origin.x + (int)(disks[i].velocity.x), (int)disks[i].origin.y + (int)(disks[i].velocity.y));
    }

    if(model->viewDisksToPlace() > 0){
        Disk& diskPreview = model->viewDiskPreview();
        DrawCircle(renderer, {(int)diskPreview.origin.x, (int)diskPreview.origin.y}, diskPreview.radius);

        for(int i = 0; i < model->viewDisksToPlace() - 1; i++){
            DrawCircle(renderer, {620, 30 + i * 40}, 10);
        }

    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    std::vector<LineSegment>& lineSegments = model->viewLineSegments();
    for(unsigned int i = 0; i < lineSegments.size(); i++){
        LineSegment lineSegment = lineSegments[i];
        SDL_RenderDrawLine(renderer, lineSegment.pointA.x, lineSegment.pointA.y, lineSegment.pointB.x, lineSegment.pointB.y); //implicit double to int
    }

    if(!SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect)){ //draw score text
        std::cout << SDL_GetError();
    }

    SDL_RenderPresent(renderer);

    //sounds
    if(model->playClick() && collisionSFXCooldown == 0){
        if (Mix_PlayChannel( -1, ballCollideSFX, 0 ) == -1) {
            printf("Failed to play effect: %s\n", Mix_GetError());
        }
        collisionSFXCooldown = 100;
    }

    if(collisionSFXCooldown > 0){
        collisionSFXCooldown--;
    }
}

View::~View(){
    SDL_Quit();
    TTF_Quit();
}