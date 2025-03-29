#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "map.h"

class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }

	static SDL_Renderer* GetRenderer() { return renderer; }
	static SDL_Event event;


private:
	
	SDL_Window* window;

	static SDL_Renderer* renderer;
	bool isRunning = false;

};