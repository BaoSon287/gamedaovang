#ifndef GAMEMAIN_H
#define GAMEMAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <math.h>
#include "common.h"
#include "resProperties.h"
extern SDL_Texture *loadTexture(const char *filePath);
extern SDL_Texture *loadRenderText(const char *text, SDL_Color color);
struct levelInfo {
    int level;
    int leveltarget;
    int totalRes;
    struct resProperties *reses;
};
levelInfo *getLevel(int lvl)
{
    if(lvl == 1) {
        levelInfo *currentLvl = (levelInfo *)malloc(sizeof(levelInfo));
        currentLvl->level = lvl;
        currentLvl->leveltarget = 500;
        currentLvl->totalRes = 6;
        resProperties *res = (resProperties *)malloc(sizeof(resProperties) * 6);

        res[0].id = ID_DIAMOND;
        res[0].position.x = 600; res[0].position.y = 300;

        res[1].id = ID_BOMB;
        res[1].position.x = 100; res[1].position.y = 200;

        res[2].id = ID_GOLD100;
        res[2].position.x = 400; res[2].position.y = 350;

        res[3].id = ID_GOLD500;
        res[3].position.x = 200; res[3].position.y = 400;

        res[4].id = ID_ROCK10;
        res[4].position.x = 700; res[4].position.y = 100;

        res[5].id = ID_GOLD250;
        res[5].position.x = 300; res[5].position.y = 200;

        currentLvl->reses = res;
        return currentLvl;
    } else if(lvl == 2) {
        levelInfo *currentLvl = (levelInfo *)malloc(sizeof(levelInfo));
        currentLvl->level = lvl;
        currentLvl->leveltarget = 700;
        currentLvl->totalRes = 8;

        resProperties *res = (resProperties *)malloc(sizeof(resProperties) * 8);

        res[0].id = ID_BOMB;
        res[0].position.x = 600; res[0].position.y = 300;

        res[1].id = ID_BOMB;
        res[1].position.x = 100; res[1].position.y = 200;

        res[2].id = ID_GOLD100;
        res[2].position.x = 400; res[2].position.y = 350;

        res[3].id = ID_GOLD250;
        res[3].position.x = 200; res[3].position.y = 400;

        res[4].id = ID_GOLD500;
        res[4].position.x = 700; res[4].position.y = 100;

        res[5].id = ID_ROCK10;
        res[5].position.x = 300; res[5].position.y = 200;

        res[6].id = ID_ROCK10;
        res[6].position.x = 500; res[6].position.y = 100;

        res[7].id = ID_GOLD250;
        res[7].position.x = 200; res[7].position.y = 250;

        currentLvl->reses = res;
        return currentLvl;
    } else if(lvl == 3) {
        levelInfo *currentLvl = (levelInfo *)malloc(sizeof(levelInfo));
        currentLvl->level = lvl;
        currentLvl->leveltarget = 1000;//
        currentLvl->totalRes = 10;

        resProperties *res = (resProperties *)malloc(sizeof(resProperties) * 10);

        res[0].id = ID_DIAMOND;
        res[0].position.x = 600; res[0].position.y = 300;

        res[1].id = ID_BOMB;
        res[1].position.x = 100; res[1].position.y = 200;

        res[2].id = ID_BOMB;
        res[2].position.x = 400; res[2].position.y = 350;

        res[3].id = ID_GOLD100;
        res[3].position.x = 200; res[3].position.y = 400;

        res[4].id = ID_GOLD100;
        res[4].position.x = 700; res[4].position.y = 100;

        res[5].id = ID_ROCK20;
        res[5].position.x = 300; res[5].position.y = 200;

        res[6].id = ID_DIAMOND;
        res[6].position.x = 500; res[6].position.y = 100;

        res[7].id = ID_ROCK10;
        res[7].position.x = 200; res[7].position.y = 250;

        res[8].id = ID_DIAMOND;
        res[8].position.x = 400; res[8].position.y = 200;

        res[9].id = ID_DIAMOND;
        res[9].position.x = 700; res[9].position.y = 250;

        currentLvl->reses = res;
        return currentLvl;
    }

    return NULL;
}

void destroyLevel(levelInfo *lvl)
{
	free(lvl->reses);
	free(lvl);
	lvl = NULL;
}
TTF_Font *gameFont = NULL;
SDL_Color fontColor = { 255, 255, 0 };
int userGrade = 0;
bool rectImpact(SDL_Rect a, SDL_Rect b) {
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;
    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;
    if (bottomA  < topB || topA > bottomB || rightA < leftB || leftA > rightB) {
        return false;
    } else {
        return true;
    }
}
void displayLevelImage(int levelNumber) {
    char imageFilePath[50];
    snprintf(imageFilePath, sizeof(imageFilePath), "res/img/target%d.png", levelNumber);
    SDL_Texture *levelImage = loadTexture(imageFilePath);
    SDL_RenderClear(winRenderer);
    SDL_RenderCopy(winRenderer, levelImage, NULL, NULL);
    SDL_RenderPresent(winRenderer);

    SDL_Delay(2000);
    SDL_DestroyTexture(levelImage);
}
int gameMain(levelInfo *level)
{
	SDL_Texture *gameBg, *resTexture[level->totalRes],  *timeTexture = NULL, *levelTexture, *targetTexture, *gradeTexture, *hook, *line;
	SDL_Rect resRect[level->totalRes], timeRect, levelRect, targetRect, gradeRect, hookRect, lineRect;
	SDL_Point minerPin, hookPin, linePin;
	resProperties resProp;
	int startTime, levelTime, hookTimer, lineTimer;
	int catchedRes = -1;
	bool running = true, hookDown = false, hookGoRight = true, hookBack = false;
	levelTime = SDL_GetTicks();
	char levelStr[4] = { 0 };
	char targetStr[10] = { 0 };
	double hookAngle = 20, lineLen = 0;
	targetStr[0] = '$';

	gameBg = loadTexture(imgFile[ID_BACKGROUND]);
    SDL_snprintf(levelStr, sizeof(levelStr), "%d", level->level);
    levelTexture = loadRenderText(levelStr, fontColor);
	hook = loadTexture(imgFile[ID_HOOK]);
	line = loadTexture(imgFile[ID_LINE]);
    SDL_snprintf(&targetStr[1], sizeof(targetStr) - 1, "%d", level->leveltarget);
	targetTexture = loadRenderText(targetStr, fontColor);
	for(int i = 0; i < level->totalRes; i++) {
		resTexture[i] = loadTexture(imgFile[level->reses[i].id]);
		resRect[i].x = level->reses[i].position.x;
		resRect[i].y = level->reses[i].position.y;
		SDL_QueryTexture(resTexture[i], NULL, NULL, &resRect[i].w, &resRect[i].h);
	}

	minerPin.x = 500;
	minerPin.y = 100;
	timeRect.x = 800; timeRect.y = 15;
	timeRect.w = 26; timeRect.h = 40;
	levelRect.x = 780; levelRect.y = 60;
	levelRect.w = 8 ;
	levelRect.h = 45;
	targetRect.x = 150; targetRect.y = 60;
	targetRect.w = 50 ;
	targetRect.h = 45;
	gradeRect.x = 180; gradeRect.y = 20;
	gradeRect.h = 45;
	hookTimer = 0;
	lineTimer = 0;
	lineLen = 0;
	catchedRes = -1;

	SDL_QueryTexture(hook, NULL, NULL, &hookRect.w, &hookRect.h);
	hookRect.x = minerPin.x - hookRect.w;
	hookRect.y = minerPin.y -21;
	hookPin.x = hookRect.w / 2;
	hookPin.y = 0;
	lineRect.x = minerPin.x - hookRect.w / 2;
	lineRect.y = minerPin.y -21;
	lineRect.h = 0;
	lineRect.w = 3;
	linePin.x = 2;
	linePin.y = 0;

	while(running) {
		char timeStr[3] = { 0 };
		char gradeStr[10] = { 0 };
		startTime = SDL_GetTicks();
		if((startTime - levelTime) / 1000 > 60)
			running = false;
		while(SDL_PollEvent(&keyEvent)) {
			if(keyEvent.type == SDL_QUIT)
				running = false;
			if(keyEvent.type == SDL_KEYDOWN)
				if(keyEvent.key.keysym.sym == SDLK_DOWN && !hookDown)
					hookDown = true;
		}

		if(!hookDown) {
			if(SDL_GetTicks() - hookTimer > 17) {
				hookTimer = SDL_GetTicks();
				if(hookGoRight) {
					hookAngle += 1.5;
					if(hookAngle >= 165)
						hookGoRight = false;
				} else {
					hookAngle -= 1.5;
					if(hookAngle <= 15)
						hookGoRight = true;
				}
			}
		} else {
			if(SDL_GetTicks() - lineTimer > 20) {
				lineTimer = SDL_GetTicks();
				if(!hookBack) {
					lineLen += 5;
				}
				else if(resProp.id == -1)
					lineLen -= 6;
				else {
					lineLen -= 6 /resProp.weight;
					if(hookAngle <= 90) {
						resRect[catchedRes].x = hookRect.x - resRect[catchedRes].w / 2;
						resRect[catchedRes].y = hookRect.y - resRect[catchedRes].h / 2 + 35;
					} else {
						resRect[catchedRes].x = hookRect.x + resRect[catchedRes].w / 2 - 30;
						resRect[catchedRes].y = hookRect.y - resRect[catchedRes].h / 2 + 35;
					}
				}
				if(lineLen < 0)
					lineLen = 0;
			}
    if (lineLen <= 1) {
    hookDown = false;
    hookBack = false;
    if (catchedRes != -1) {
        userGrade += resProp.score;
        if (userGrade >= level->leveltarget) {
          return 2;
        }
        resProp.setId(-1);
					SDL_DestroyTexture(resTexture[catchedRes]);
					resTexture[catchedRes] = NULL;
        catchedRes = -1;
    }
}
}

			if(lineLen >= 500)
				hookBack = true;
			if(hookAngle <= 90) {
				hookRect.x = minerPin.x - hookRect.w - abs(cos(hookAngle / 180.0 * M_PI) * lineLen);
				hookRect.y = minerPin.y + abs(sin(hookAngle / 180.0 * M_PI) * lineLen) - 20;
			} else {
				hookRect.x = minerPin.x - hookRect.w + abs(cos(hookAngle / 180.0 * M_PI) * lineLen);
				hookRect.y = minerPin.y  + abs(sin(hookAngle / 180.0 * M_PI) * lineLen) - 20;
			}
		gradeStr[0] = '$';
		SDL_itoa(userGrade, &gradeStr[1], 10);
		gradeTexture = loadRenderText(gradeStr, fontColor);
		gradeRect.w = 30;
		lineRect.h = lineLen;

		if(timeTexture)
			SDL_DestroyTexture(timeTexture);
		timeTexture = loadRenderText(SDL_itoa(60 - ((startTime - levelTime) / 1000), timeStr, 10), fontColor);

		SDL_RenderCopy(winRenderer, gameBg, NULL, NULL);
		SDL_RenderCopy(winRenderer, levelTexture, NULL, &levelRect);
		SDL_RenderCopy(winRenderer, timeTexture, NULL, &timeRect);
		SDL_RenderCopy(winRenderer, targetTexture, NULL, &targetRect);
		SDL_RenderCopy(winRenderer, gradeTexture, NULL, &gradeRect);
		SDL_RenderCopyEx(winRenderer, hook, NULL, &hookRect, 90 - hookAngle, &hookPin, SDL_FLIP_NONE);
		SDL_RenderCopyEx(winRenderer, line, NULL, &lineRect, 90 - hookAngle, &linePin, SDL_FLIP_NONE);

		for(int i = 0; i < level->totalRes; i++) {
			if(resTexture[i])
				SDL_RenderCopy(winRenderer, resTexture[i], NULL, &resRect[i]);
		}
		SDL_RenderPresent(winRenderer);

		if(catchedRes == -1)
for(int i = 0; i < level->totalRes; i++) {
    if(resTexture[i] == NULL) {
        continue;
    }

    if(rectImpact(hookRect, resRect[i])) {
        if (level->reses[i].id == ID_BOMB) {
            SDL_DestroyTexture(resTexture[i]);
            hookBack = true;
            userGrade=userGrade-500;
            resTexture[i] = NULL;
            printf("Caught a bomb! It disappeared!\n");
        } else {
            catchedRes = i;
            resProp.setId(level->reses[i].id);
            hookBack = true;
            printf("Caught!\n");
        }
        break;
    }
}

	}

	for(int i = 0; i < level->totalRes; i++) {
		if(resTexture[i])
			SDL_DestroyTexture(resTexture[i]);
	}
	SDL_DestroyTexture(gameBg);
	SDL_DestroyTexture(timeTexture);
	SDL_DestroyTexture(levelTexture);
	return userGrade;
}

void gameOver(bool win)
{
	if(win)
		printf("You Win %d\n", userGrade);
	else
		printf("You Lose %d\n", userGrade);
}

int startGame() {
    int lvlNum = 1;
    bool win = false;
    levelInfo *lvl = NULL;
    gameFont = TTF_OpenFont("res/default.ttf", 40);

    while (lvlNum <= 3) {
        displayLevelImage(lvlNum); // Hiển thị ảnh trước khi vào mỗi cấp độ
        userGrade = 0;
        lvl = getLevel(lvlNum);
        if (!lvl) {
            win = true;
            gameOver(win);
            break;
        }
        int result = gameMain(lvl);
        lvlNum++; // Chuyển sang cấp độ tiếp theo sau khi hoàn thành cấp độ hiện tại
        destroyLevel(lvl); // Hủy bộ nhớ được cấp phát cho cấp độ hiện tại
    }

    return 1;
}




#endif
