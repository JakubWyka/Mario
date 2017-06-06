#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"funkcje.h"
extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
	//#endif
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480


// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};
void DrawSprite(SDL_Surface *screen, SDL_Surface *sprite, int x, int y,int number) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w=MARIOWIDTH;
	dest.h = sprite->h=MARIOHEIGHT;
	SDL_Rect srcrect;
	srcrect.x = 0+number*64;
	srcrect.y = 0;
	srcrect.w = 64;
	srcrect.h = 64;
	SDL_BlitSurface(sprite, &srcrect, screen, &dest);
};

// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};
bool kolizja(int distancex, int distancey, przeszkoda przeszkody[ILOSCPRZESZKOD]){
	int x1 = STARTX - MARIOWIDTH / 2 + distancex;
	int y1 = SCREEN_HEIGHT - MARIOHEIGHT - distancey;

	for (int i = 0; i <= ILOSCPRZESZKOD; i++) {
		int x2 = przeszkody[i].srodekx - przeszkody[i].width / 2;
		int y2 = SCREEN_HEIGHT-przeszkody[i].srodeky - przeszkody[i].height / 2;

		if (x1 == x2 + przeszkody[i].width && y1 <= y2 + przeszkody[i].height && y1 >= y2) 
			return true; 

		if (x1 == x2 + przeszkody[i].width && y1 + MARIOHEIGHT <= y2 + przeszkody[i].height && y1 + MARIOHEIGHT >= y2)
			return true;

		if (x1 <= x2 + przeszkody[i].width && x1 >= x2 && y1 == y2 + przeszkody[i].height) 
			return true;

		if (x1 + MARIOWIDTH <= x2 + przeszkody[i].width && x1 + MARIOWIDTH >= x2 && y1 == y2 + przeszkody[i].height)
			return true;

		if (x1 + MARIOWIDTH == x2 && y1 >= y2 && y1 <= y2 + przeszkody[i].height) 
			return true;

		if (x1 + MARIOWIDTH == x2 && y1 + MARIOHEIGHT >= y2 && y1 + MARIOHEIGHT <= y2 + przeszkody[i].height) 
			return true;

		if (x1 >= x2 && x1 <= x2 + przeszkody[i].width && y1 + MARIOHEIGHT == y2) 
			return true;

		if (x1 + MARIOWIDTH >= x2 && x1 + MARIOWIDTH <= x2 + przeszkody[i].width && y1 + MARIOHEIGHT == y2)
			return true;
		/////////
		if (przeszkody[i].srodekx - przeszkody[i].width / 2 <= STARTX - MARIOWIDTH / 2 + distancex + MARIOWIDTH &&
			przeszkody[i].srodekx - przeszkody[i].width / 2 > STARTX - MARIOWIDTH / 2 + distancex &&
			SCREEN_HEIGHT - przeszkody[i].srodeky - przeszkody[i].height / 2 >= SCREEN_HEIGHT - MARIOHEIGHT - distancey &&
			SCREEN_HEIGHT - przeszkody[i].srodeky - przeszkody[i].height / 2 <= SCREEN_HEIGHT - MARIOHEIGHT - distancey + MARIOHEIGHT)
			return true;
		
		else if (przeszkody[i].srodekx - przeszkody[i].width / 2 + przeszkody[i].width <= STARTX - MARIOWIDTH / 2 + distancex + MARIOWIDTH &&
			przeszkody[i].srodekx - przeszkody[i].width / 2 + przeszkody[i].width > STARTX - MARIOWIDTH / 2 + distancex &&
			SCREEN_HEIGHT - przeszkody[i].srodeky - przeszkody[i].height / 2 >= SCREEN_HEIGHT - MARIOHEIGHT - distancey &&
			SCREEN_HEIGHT - przeszkody[i].srodeky - przeszkody[i].height / 2 <= SCREEN_HEIGHT - MARIOHEIGHT - distancey + MARIOHEIGHT)
			return true;
		
		
	}
	return false;
};
void save(double distancex, double distancey,int lifes,double timeleft) {
	int decimaltimex10 = 10 * timeleft;
	int decimaldistancexx100 = 100 * distancex;
	int decimaldistanceyx100 = 100 * distancey;
	FILE* plik= fopen("save.txt", "w");
	fprintf(plik, "%d %d %d %d", decimaldistancexx100, decimaldistanceyx100,lifes, decimaltimex10);
	fclose(plik);
};
void load(double *distancex, double *distancey, int *lifes, double *timeleft) {
	int decimaltimex10=0;
	int var1 = 0;
	int var2 = 0;
	int zycia = 0;
	FILE* plik = fopen("save.txt", "r");
	fscanf(plik, "%d %d %d %d", &var1, &var2, &zycia, &decimaltimex10);
	*timeleft = decimaltimex10 / 10.0;
	double var3 = var1 / 100.0;
	double var4 = var2 / 100.0;
	*distancex = var3;
	*distancey = var4;
	*lifes = zycia;
	fclose(plik);
};
void new_game(double *distancex, double *distancey, int *lifes, double *timeleft, bool *jump) {
	*distancex = 0;
	*distancey = 0;
	*lifes = LIFES;
	*timeleft = STARTTIME;
	*jump = false;
};