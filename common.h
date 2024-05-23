#ifndef COMMON_H
#define COMMON_H
#include <SDL.h>
#define SCREEN_W	960
#define SCREEN_H	544
#define	FPS		60
#define PRE_FRAME_TICKS	1000 / FPS
#define MAXRES	30;
#define abs(x) (x > 0 ? x : (-x))

enum statusGame{
	PROG_QUIT,
	START_GAME
};

enum resImgId {
	ID_ROCK10,
	ID_ROCK20,
	ID_DIAMOND,
	ID_GOLD100,
	ID_GOLD250,
	ID_GOLD500,
	ID_BOMB,
	ID_BACKGROUND,
	ID_TARGET1,
	ID_TARGET2,
	ID_TARGET3,
	ID_HOOK,
	ID_LINE
};

extern SDL_Window *win;
extern SDL_Renderer *winRenderer;
extern SDL_Event keyEvent;
extern const char imgFile[][30];
#endif
