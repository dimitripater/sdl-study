/* Stephen's Tutorials (http://stephenmeier.net/)
gcc main.c -o game.exe -I./include -L./lib -lSDL2main -lSDL2
running on gcc 4.8.1, SDL 2.0.1
*/

//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#define SCREEN_w 640
#define SCREEN_H 480
#define SCREEN_SCALE 1
#define SCREEN_NAME "Prototype"
//----------------------------------------------------------------------
void game_init(void);
void game_quit(void);

struct {
    // define "attributes"
    SDL_bool running;

    struct {
        unsigned int w;
        unsigned int h;
        const char* name;
        SDL_Window* window;
        SDL_Renderer* renderer;
    } screen;

    struct {
        unsigned int n;
        SDL_Surface** spritesheet;
    } gfx;

    // define "methods"
    void (*init)(void);
    void (*quit)(void);
} Game = {
        SDL_FALSE,
        {
                SCREEN_SCALE*SCREEN_w,
                SCREEN_SCALE*SCREEN_H,
                SCREEN_NAME,
                NULL, NULL
        },
        {0, NULL},
        game_init,
        game_quit
};//----------------------------------------------------------------------
void game_init(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) {
        printf("SDL error -> %sn", SDL_GetError());
        exit(1);
    }

    unsigned int w = Game.screen.w;
    unsigned int h = Game.screen.h;
    const char* name = Game.screen.name;

    Game.screen.window = SDL_CreateWindow(
            name,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            w, h, 0
    );
    Game.screen.renderer = SDL_CreateRenderer(
            Game.screen.window, -1,
            SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
    );

    Game.running = SDL_TRUE;

    SDL_Surface* surface = SDL_LoadBMP("/home/dimitri/ClionProjects/untitled/spritesheet.bmp");
    int n = ((surface->w/8)*(surface->h/8)+1);

    Game.gfx.n = n;
    Game.gfx.spritesheet = (SDL_Surface**) malloc(sizeof(SDL_Surface*)*n);

    int i, x, y;
    SDL_Rect rect = {0, 0, 8, 8};
    for(i=0; i<n; i++) {
        Game.gfx.spritesheet[i] = SDL_CreateRGBSurface(0, 8, 8, 24, 0x00, 0x00, 0x00, 0x00);
        SDL_SetColorKey(Game.gfx.spritesheet[i], 1, 0xFF00FF);
        SDL_FillRect(Game.gfx.spritesheet[i], 0, 0xFF00FF);
        if(i!=0) {
            x = (i-1)%(surface->w/8);
            y = (i-x)/(surface->w/8);
            rect.x = x*8;
            rect.y = y*8;
            SDL_BlitSurface(surface, &rect, Game.gfx.spritesheet[i], NULL);
        }
    }

    SDL_FreeSurface(surface);
}
//----------------------------------------------------------------------
void game_quit(void) {
    int i;
    for(i=0; i<Game.gfx.n; i++)
        SDL_FreeSurface(Game.gfx.spritesheet[i]);
    free(Game.gfx.spritesheet);
    Game.gfx.spritesheet = NULL;

    SDL_DestroyRenderer(Game.screen.renderer);
    Game.screen.renderer = NULL;

    SDL_DestroyWindow(Game.screen.window);
    Game.screen.window = NULL;

    SDL_Quit();
}

//----------------------------------------------------------------------
int main(int argc, char* argv[]) {

    Game.init();

    int x = Game.screen.w/2-8, y = Game.screen.h/2-8;

    SDL_Rect rect = {0, 0, 8*2, 8*2};
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(Game.screen.renderer, Game.gfx.spritesheet[17]);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(Game.screen.renderer, Game.gfx.spritesheet[18]);
    SDL_Texture* texture3 = SDL_CreateTextureFromSurface(Game.screen.renderer, Game.gfx.spritesheet[29]);
    SDL_Texture* texture4 = SDL_CreateTextureFromSurface(Game.screen.renderer, Game.gfx.spritesheet[30]);

    SDL_Event event;
    while(Game.running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: {
                    Game.running = SDL_FALSE;
                } break;
            }
        }

        SDL_RenderClear(Game.screen.renderer);

        rect.x = 0+x, rect.y = 0+y;
        SDL_RenderCopy(Game.screen.renderer, texture1, NULL, &rect);

        rect.x = 8*2+x, rect.y = 0+y;
        SDL_RenderCopy(Game.screen.renderer, texture2, NULL, &rect);

        rect.x = 0+x, rect.y = 8*2+y;
        SDL_RenderCopy(Game.screen.renderer, texture3, NULL, &rect);

        rect.x = 8*2+x, rect.y = 8*2+y;
        SDL_RenderCopy(Game.screen.renderer, texture4, NULL, &rect);

        SDL_RenderPresent(Game.screen.renderer);
    }

    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture4);

    Game.quit();

    return 0;
}