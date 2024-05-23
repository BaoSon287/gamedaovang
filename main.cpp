#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "resProperties.h"
#include "common.h"
#include "gamemain.h"
#include "main.h"

int Init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL initialize failed! %s\n", SDL_GetError());
        return 0;
    }
    else {
        win = SDL_CreateWindow("Gold Miner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
        winRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (winRenderer == NULL) {
            printf("Failed to create window renderer! %s\n", SDL_GetError());
            return 0;
        }
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
        printf("SDL image initialize failed! %s\n", IMG_GetError());
        return 0;
    }

    if (TTF_Init() == -1) {
        printf("SDL ttf initialize failed! %s\n", TTF_GetError());
        return 0;
    }

    return 1;
}


void Close_SDL() {
    SDL_DestroyRenderer(winRenderer);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(const char *filePath)
{
   SDL_Surface *loadedSurface;
    SDL_Texture *newTexture;

    loadedSurface = IMG_Load(filePath);
    if (loadedSurface == NULL)
    {
        printf("Failed to load file %s! %s\n", filePath, IMG_GetError());
        newTexture = NULL;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(winRenderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
SDL_Texture *loadRenderText(const char *text, SDL_Color color)
{
	SDL_Surface *textSurface = TTF_RenderText_Solid(gameFont, text, color);
	if(textSurface == NULL) {
		printf("Failed to render text surface! %s\n", TTF_GetError());
		return NULL;
	}

	return SDL_CreateTextureFromSurface(winRenderer, textSurface);
}
int main(int argc, char **argv) {
  if (!Init_SDL()) {
    return 1;
  }

  atexit(Close_SDL);
  startGame();
  return 0;
}


