#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "map.h"
#include "ECS/ECS.h" 
// Định nghĩa state cho game
enum class GameState {
    SPLASH,
    COUNTDOWN,
    RUNNING
};

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

    // Các state của game
    GameState state;
    Uint32 countdownStartTime; // Thời điểm bắt đầu countdown

private:
    SDL_Window* window;
    static SDL_Renderer* renderer;
    bool isRunning = false;

    Manager manager;
    Map* map;

    Entity* player;
    Entity* wall;

    SDL_Texture* splashTexture;
    SDL_Texture* countdown3;
    SDL_Texture* countdown2;
    SDL_Texture* countdown1;
    SDL_Texture* countdownGO;

    void renderSplash();
    void renderCountdown();
    void renderRunning();
};

