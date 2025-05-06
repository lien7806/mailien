#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "map.h"
#include "ECS/ECS.h" 
#include "SDL_mixer.h"

enum class GameState {
    SPLASH,
    COUNTDOWN,
    RUNNING,
    GOKU_WINS,
    CHIAO_WINS,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void initAudio();
    void cleanAudio();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }
    static SDL_Renderer* GetRenderer() { return renderer; }
    static SDL_Event event;

    GameState state;
    Uint32 countdownStartTime;

private:
    SDL_Window* window;
    static SDL_Renderer* renderer;
    bool isRunning = false;
    Mix_Music* bgMusic;
    bool koSoundPlayed = false;

    Manager manager;
    Map* map;

    Entity* player;
    Entity* wall;

    SDL_Texture* splashTexture;
    SDL_Texture* countdown3;
    SDL_Texture* countdown2;
    SDL_Texture* countdown1;
    SDL_Texture* countdownGO;
    SDL_Texture* guideTexture;
    SDL_Texture* gokuWinsTexture;
    SDL_Texture* chiaoWinsTexture;
    SDL_Texture* gameOverTexture;
    Uint32 endGameStartTime;
    const Uint32 END_GAME_DELAY = 3000; //kthuc
    bool showGuide;
    bool isFalling;
    bool gokuHypnotized;
    void renderSplash();
    void renderCountdown();
    void renderRunning();
    void renderEndGame(SDL_Texture* texture);
    void resetGame();
};
