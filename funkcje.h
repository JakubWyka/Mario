#define _USE_MATH_DEFINES
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define LEVEL_WIDTH = 1280;
#define LEVEL_HEIGHT = 480;
#define STARTX 32
#define STARTTIME 120
#define LIFES 3
#define KRZEWX 200
#define KRZEWY 16
#define MARIOWIDTH 64
#define MARIOHEIGHT 64
#define ILOSCPRZESZKOD 3
#define PLATFORMX 400
#define PLATFORMY 95
#define SPEEDX 80.0
#define SPEEDY 50.0
#define JUMPHEIGHT 150
#include<math.h>
#include<stdio.h>
#include<string.h>

struct przeszkoda {
	int srodekx; //wspolrzedna x srodka
	int srodeky;//..y..
	int width; //szerokosc tekstury
	int height;//wysokosc..
};

extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
	//#endif
}

void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawSprite(SDL_Surface *screen, SDL_Surface *sprite, int x, int y, int number);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
bool kolizja(int distancex, int distancey, przeszkoda przeszkody[ILOSCPRZESZKOD]);
void save(double distancex, double distancey, int lifes, double timeleft);
void load(double *distancex, double *distancey, int *lifes, double *timeleft);
void new_game(double *distancex, double *distancey, int *lifes, double *timeleft,bool *jump);