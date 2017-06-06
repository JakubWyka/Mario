#include"funkcje.h"

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc;
	int x = 0, y = 0, currentdistancex = 0, zycia=LIFES, przesuniecie=0;
	double delta, worldTime, fpsTimer, fps, distancex,distancey, marioSpeedx, marioSpeedy,timeleft,stan;
	bool jump = false, walk = false,col = false;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *mario;
	SDL_Surface *kamien;
	SDL_Surface *platforma;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	int number = 0;//numer tekstury -1
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
		}

	// tryb pe³noekranowy
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "Jakub Wyka 165734");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
		};
	SDL_SetColorKey(charset, true, 0x000000);

	 mario = SDL_LoadBMP("./mario2.bmp");
	 SDL_SetColorKey(mario, true, 0xFFFFFF);//przezroczystosc tla
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,mario);
	//dodanie przeszkody
	kamien = SDL_LoadBMP("./kamien.bmp");
	przeszkoda przeszkody[ILOSCPRZESZKOD];
	przeszkody[0].srodekx = KRZEWX;
	przeszkody[0].srodeky = KRZEWY;
	przeszkody[0].height = 32;
	przeszkody[0].width = 64;

	przeszkody[1].srodekx = 550;
	przeszkody[1].srodeky = KRZEWY;
	przeszkody[1].height = 32;
	przeszkody[1].width = 64;

	platforma = SDL_LoadBMP("./platforma.bmp");
	przeszkody[2].srodekx = PLATFORMX;
	przeszkody[2].srodeky = PLATFORMY;
	przeszkody[2].height = 32;
	przeszkody[2].width = 96;
	

	////////////////
	
	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	t1 = SDL_GetTicks();
	fpsTimer = 0;
	quit = 0;
	frames = 0;
	worldTime = 0;
	marioSpeedx = 0;
	marioSpeedy = 0;
	distancex = 0;
	distancey = 0;
	timeleft = STARTTIME;
	while(!quit && zycia>0) {
	
		t2 = SDL_GetTicks();
		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna³ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		delta = (t2 - t1) * 0.001;
		t1 = t2;
		SDL_FillRect(screen, NULL, niebieski);
		worldTime += delta;
		
		if (walk == true) {
			distancex += marioSpeedx * delta;
			if(jump==false)
			number = (0 + frames / 400) % 3 + 1;
			col = kolizja(distancex, distancey, przeszkody);
			if (col ==false && jump == false&&distancey!=0) {
				stan = 1;
				jump = true;
			}
			if (col== true  && distancey==0) {
				walk = false;
				distancex -= marioSpeedx * delta;
			}
			if (col == true && jump == true) {
				walk = false;
				distancex -= marioSpeedx * delta;
			}
			if (distancex - currentdistancex > 2 || distancex - currentdistancex < -2)
				walk = false;
		}
		if (walk == false)
			number = 0;
		if (jump == true) {
			if (distancey > JUMPHEIGHT)
				stan = 1;//stan spadania
			if (stan==0) {//wznoszenia
				number = 4;
				distancey += marioSpeedy * delta;
				col = kolizja(distancex, distancey, przeszkody);
				if (col == true) {
					stan = 1;
					distancey = distancey - 1;
				}
			}
			else if (stan==1) {
				distancey -= marioSpeedy * delta;
				col = kolizja(distancex, distancey, przeszkody);
				if (col == true) {
					number = 1;
					jump = false;
				}
				if (distancey < 0) {
					number = 0;
					distancey = 0;
					jump = false;
				}
			}
			
		}
		///
		if (distancex > SCREEN_WIDTH/2) {
			przesuniecie = distancex - SCREEN_WIDTH / 2;
		}
		SDL_Rect srcrect;
		srcrect.x = 0+64*number;
		srcrect.y = 0;
		srcrect.w = 64;
		srcrect.h = 64;
		SDL_Rect dstrect;
		dstrect.x = distancex-przesuniecie;
		dstrect.y = SCREEN_HEIGHT - MARIOHEIGHT-distancey;
		dstrect.w =  MARIOWIDTH;
		dstrect.h =  MARIOHEIGHT;

		
		//DrawSprite(screen, mario, STARTX+distancex, SCREEN_HEIGHT-MARIOHEIGHT/2-distancey,4);
		timeleft = STARTTIME - worldTime;
		if (timeleft <= 0) {
			zycia--;
			distancey = 0;
			distancex = 0;
			timeleft = STARTTIME;
			worldTime = 0;
		}
		
		DrawSurface(screen, kamien, KRZEWX-przesuniecie, SCREEN_HEIGHT-KRZEWY);
		DrawSurface(screen, kamien, 550-przesuniecie, SCREEN_HEIGHT - KRZEWY);
		DrawSurface(screen, platforma, PLATFORMX-przesuniecie, SCREEN_HEIGHT - PLATFORMY);
		// tekst informacyjny
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "Czas do konca = %.1lf s   Zycia: %d", timeleft, zycia);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		sprintf(text, "Esc - wyjscie, \030 - skok, \032 - lewo, \033 - prawo");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
		SDL_RenderPresent(renderer);
		
		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_UP){
						if (jump == false) {
							marioSpeedy = SPEEDY;
							jump = true;
							stan = 0;
						}
						}
					
					else if (event.key.keysym.sym == SDLK_LEFT) {
						marioSpeedx = -SPEEDX;
						walk = true;
						currentdistancex = distancex;
						
					}
					else if (event.key.keysym.sym == SDLK_RIGHT) {
						marioSpeedx = SPEEDX;
						walk = true;
						currentdistancex = distancex;
					}
					else if (event.key.keysym.sym == SDLK_s) {
						save(distancex, distancey, zycia, timeleft);
					}
					else if (event.key.keysym.sym == SDLK_l) {
						load(&distancex, &distancey, &zycia, &timeleft);
						worldTime = STARTTIME - timeleft;
					}
					else if (event.key.keysym.sym == SDLK_n) {
						new_game(&distancex, &distancey, &zycia, &timeleft,&jump);
						worldTime = 0;
					}
					break;
				//case SDL_KEYUP:
				//	etiSpeedy = 0.0;
				//	break;
				case SDL_QUIT:
					quit = 1;
					break;
				};
			};
		frames++;
		};

		
		
	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
