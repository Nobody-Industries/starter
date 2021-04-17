#include <SDL.h>
#include <stdio.h>

/*

 Copyright (c) 2021 Nobody Industries

 Permission to use, copy, modify, and distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE.

*/

#pragma mark DEFINITIONS

#define APP_NAME "APP"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define FPS 30

#pragma mark GLOBAL VARIABLES

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
Uint32 *pixels;

#pragma mark UTILS

void
error(const char *msg, const char *err)
{
	printf("ERROR: %s [%s]", msg, err);
}

#pragma mark INITIALIZATION & DESTRUCTION

void
init_app()
{
	// SDL & Memory allocation
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		error("SDL_Init Failed", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow(
		APP_NAME,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);
	if(window == NULL) {
		error("SDL_CreateWindow Failed", SDL_GetError());
		goto error_window;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(renderer == NULL) {
		error("SDL_CreateRenderer Failed", SDL_GetError());
		goto error_renderer;
	}
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_STATIC,
		WINDOW_WIDTH,
		WINDOW_HEIGHT);
	if(texture == NULL) {
		error("SDL_CreateTexture Failed", SDL_GetError());
		goto error_texture;
	}
	pixels = (Uint32 *)calloc(WINDOW_HEIGHT * WINDOW_WIDTH, sizeof(Uint32));
	if(pixels == NULL) {
		error("Allocation error", "pixels - init");
		goto error_pixels;
	}
	return;
	// Error handling
error_pixels:
	SDL_DestroyTexture(texture);
error_texture:
	SDL_DestroyRenderer(renderer);
error_renderer:
	SDL_DestroyWindow(window);
error_window:
	SDL_Quit();
	exit(1);
}

void
destroy()
{
	free(pixels);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

#pragma mark DRAW

void
redraw()
{
	SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

#pragma mark SDL

void
on_mouse_event(SDL_Event *evt)
{
	switch(evt->type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONUP: break;
	case SDL_MOUSEBUTTONDOWN: break;
	}
}

#pragma mark MAIN PROGRAM

int
main()
{
	int tick_next = 0;
	SDL_Event event;
	init_app();
	redraw();
	while(1) {
		int tick = SDL_GetTicks();
		if(tick < tick_next)
			SDL_Delay(tick_next - tick);
		tick_next = tick + (1000 / FPS);
		redraw();
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
			case SDL_QUIT: destroy(); return 0;
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEMOTION: on_mouse_event(&event); break;
			}
		}
	}
	destroy();
	return 0;
}
