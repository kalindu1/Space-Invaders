#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "collision.hpp"

#define WIDTH 800
#define HEIGHT 800 
#define IMGSIZE 60
#define ENEMYSIZE 64




int main(int argc, char* argv[]){ 
    // init SDL2...
    
    collision collide;
   // Enemy enemy;


    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "Error init SDL:-  " << SDL_GetError() << std::endl;
    }

    // vars...
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Surface *surf;
    SDL_Texture *space_ship_RED;
    SDL_Texture *bg; 
    SDL_Surface *rectS = NULL;

    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    
    SDL_Texture *enemy;
    //SDL_Surface *surf;

    
    // Event var...
    SDL_Event event;
    
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);


    // making things work
    
    window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // loading stuff;
    
    surf = IMG_Load("res/red_plane.png");
    space_ship_RED = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
    
    surf = IMG_Load("res/bg.png");
    bg = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);


    surf = IMG_Load("res/enemy.png");
    enemy = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);

    
    
       // rects
    
////////////////////////////////////////////////////////////////////////////////////////////////////

    SDL_Rect redSR;
    SDL_Rect redDR;

    redSR.x = 0;
    redSR.y = 0;
    redSR.w = IMGSIZE;
    redSR.h = IMGSIZE;

    redDR.x = WIDTH/2-IMGSIZE;
    redDR.y = 700;
    redDR.w = IMGSIZE;
    redDR.h = IMGSIZE;
   
    SDL_Rect bullet;
    bullet.x = redDR.x+IMGSIZE/2;
    bullet.y = redDR.y+IMGSIZE/2-4;
    bullet.w = 10;
    bullet.h = 10;

    SDL_Rect bullet2;

    bullet2.x = redDR.x+IMGSIZE/2;
    bullet2.y = redDR.y+IMGSIZE/2-4;
    bullet2.w = 10;
    bullet2.h = 10;
    
    SDL_Rect bullet3;
    bullet3.x = redDR.x+IMGSIZE/2;
    bullet3.y = redDR.y+IMGSIZE/2-4;
    bullet3.w = 10;
    bullet3.h = 10;

    SDL_Rect enemySR;
    SDL_Rect enemyDR;

    enemySR.w = ENEMYSIZE;
    enemySR.h = ENEMYSIZE;
    enemySR.x = 0;
    enemySR.y = 0;
    
    enemyDR.w = ENEMYSIZE;
    enemyDR.h = ENEMYSIZE;
    enemyDR.x = 300;
    enemyDR.y = 100;




    // bool running...
    bool running = true;
    bool fired = false;
    bool fired2 = false;
    bool fired3 = false;
    
    bool enemyM_left = false;
    bool enemyM_right = true;



    float vel = 3;

    // main loop

///////////////////////////////////////////////////////////////////////////////////////////////////////
    while (running){
        
        // event handeling

        ////////////////////////////////////////////////////////////////////////////////////////////////

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT){
                running = false;
            }
        }

        frameStart = SDL_GetTicks();
        
        //SDL_SetRenderDrawColor( render, 0, 0, 0, 0 ); 

        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        // frame rate

        /////////////////////////////////////////////////////////////////////////////////////////////////
        
        if (frameDelay > frameTime){                                                                          
            SDL_Delay(frameDelay - frameTime);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        // keyboard event handeling

        ////////////////////////////////////////////////////////////////////////////////////////////////

        if (keyboard[SDL_SCANCODE_LEFT]){
            redDR.x -= vel;
            if (!fired){
                bullet.x -= vel;
            }
            if (!fired2){
                bullet2.x -= vel;
            }
            if (!fired3){
                bullet3.x -= vel;
            }
        }

        if (keyboard[SDL_SCANCODE_RIGHT]){
            redDR.x += vel;
            if (!fired){
                bullet.x += vel;
            }
            if (!fired2){
                bullet2.x += vel;
            }
            if (!fired3){
                bullet3.x += vel;
            }
        }

         

        if (keyboard[SDL_SCANCODE_UP]){
            fired = true;
            //fired2 = true;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        
        // bullet handeling & enemy movement
        
        //////////////////////////////////////////////////////////////////////////////////////////////////

        if (bullet.y < 0){
            bullet.x = redDR.x+IMGSIZE/2;
            bullet.y = redDR.y+IMGSIZE/2-4;
            fired = false;
        }


        if (bullet2.y < 0){
            bullet2.x = redDR.x+IMGSIZE/2;
            bullet2.y = redDR.y+IMGSIZE/2-4;
            fired2 = false;
        }

        if (bullet3.y < 0){
            bullet3.x = redDR.x+IMGSIZE/2;
            bullet3.y = redDR.y+IMGSIZE/2-4;
            fired3 = false;
        }
        
        if (fired){
            bullet.y -= vel;
        }
        
        if (enemyM_right){
            enemyDR.x += 2.5;
        }

        if (enemyDR.x >= 750){
            enemyM_right = false;
            enemyDR.y += 30;
            enemyM_left = true;
        }

        if (enemyM_left){
            enemyDR.x -= 2;
        }

        if (enemyDR.x == 0){
            enemyM_left = false;
            enemyDR.y += 30;
            enemyM_right = true;
        }

        if (bullet.y < 600 && keyboard[SDL_SCANCODE_UP]){
            fired2 = true;
        }


        if (bullet2.y < 300 && keyboard[SDL_SCANCODE_UP]){
            fired3 = true;
        }

        if (fired2){
            bullet2.y -= vel;
        }

        if (fired3){
            bullet3.y -= vel;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        
        if (collide.test_for_collision(enemyDR, redDR)){
            std::cout << "lost" << std::endl;
        }
        

        if (
                collide.test_for_collision(enemyDR, bullet) ||
                collide.test_for_collision(enemyDR, bullet2)||
                collide.test_for_collision(enemyDR, bullet3)
           ){
            std::cout << "won" << std::endl;
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        
        // where the renderering happens
        
        ////////////////////////////////////////////////////////////////////////////////////////////////
        

        SDL_RenderClear(render);
        
        SDL_RenderCopy(render, bg, NULL, NULL);
        SDL_SetRenderDrawColor( render, 0, 255, 0, 255 );

        // Render rect
        SDL_RenderFillRect( render, &bullet );
        SDL_RenderFillRect( render, &bullet2 );
        SDL_RenderFillRect( render, &bullet3 );


        SDL_RenderCopy(render, enemy, &enemySR, &enemyDR);

        SDL_RenderCopy(render, space_ship_RED, &redSR, &redDR);   
        
        SDL_RenderPresent(render);

        frameTime = SDL_GetTicks() - frameStart;


        ///////////////////////////////////////////////////////////////////////////////////////////////////
    }

    SDL_DestroyTexture(enemy); 
    SDL_DestroyTexture(space_ship_RED);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
    return 0;
}
