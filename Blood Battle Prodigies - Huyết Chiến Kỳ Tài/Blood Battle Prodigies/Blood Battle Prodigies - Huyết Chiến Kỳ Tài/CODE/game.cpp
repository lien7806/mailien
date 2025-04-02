#include "game.h"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Components.h"
#include "vector2D.h"
#include "Collision.h"
#include "ECS/GokuController.h"



SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Game::Game()
    : isRunning(false),
    state(GameState::SPLASH),
    countdownStartTime(0),
    splashTexture(nullptr),
    countdown3(nullptr),
    countdown2(nullptr),
    countdown1(nullptr),
    countdownGO(nullptr),
    map(nullptr),
    player(nullptr),
    wall(nullptr)
{
}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialised!..." << std::endl;

        // Khởi tạo SDL_image
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
            std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        }

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created!" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
            isRunning = true;
        }
        else {
            isRunning = false;
        }
    }

    // Tải texture cho màn hình mở đầu và countdown 
    splashTexture = TextureManager::LoadTexture("assets/modau.png");
    countdown3 = TextureManager::LoadTexture("assets/3.png");
    countdown2 = TextureManager::LoadTexture("assets/2.png");
    countdown1 = TextureManager::LoadTexture("assets/1.png");
    countdownGO = TextureManager::LoadTexture("assets/GO.png");

    // Tạo Map
    map = new Map("assets/background.png", renderer);
    map->LoadMap("assets/background.png");


    player = &manager.addEntity();
    wall = &manager.addEntity();

     player->addComponent<TransformComponent>(100.0f, 200.0f, 80, 120, 3.0); // kích thước gerenal
    player->addComponent<SpriteComponent>("assets/Generalblue1.png");
    player->addComponent<KeyboardController>();
    player->addComponent<ColliderComponent>("player");

 
    wall->addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 3.0); // kích thước goku
    wall->addComponent<SpriteComponent>("assets/Goku1.png");
    wall->addComponent<ColliderComponent>("wall");
    wall->addComponent<ArrowController>();
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        isRunning = false;
    }

    // kick chuột để sang countdown
    if (state == GameState::SPLASH && event.type == SDL_MOUSEBUTTONDOWN) {
        state = GameState::COUNTDOWN;
        countdownStartTime = SDL_GetTicks();
    }
}

void Game::update() {
    manager.refresh();
   
    switch (state) {
    case GameState::SPLASH:
   
        break;
    case GameState::COUNTDOWN: {
        Uint32 elapsed = SDL_GetTicks() - countdownStartTime;
        // tổng số giây hiển thị
        if (elapsed > 2600) {
            state = GameState::RUNNING;
        }
        break;
    }
    case GameState::RUNNING:
        manager.update();

        // Ví dụ xử lý va chạm
        if (Collision::AABB(player->getComponent<ColliderComponent>().collider,
            wall->getComponent<ColliderComponent>().collider)) {
            player->getComponent<TransformComponent>().scale = 1;
            std::cout << "Wall Hit!" << std::endl;
        }
        break;
    }
}

void Game::render() {
    // Phân nhánh render theo state
    switch (state) {
    case GameState::SPLASH:
        renderSplash();
        break;
    case GameState::COUNTDOWN:
        renderCountdown();
        break;
    case GameState::RUNNING:
        renderRunning();
        break;
    }
}

void Game::renderSplash() {
    SDL_RenderClear(renderer);
    if (splashTexture) {
        SDL_RenderCopy(renderer, splashTexture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}

void Game::renderCountdown() {
    SDL_RenderClear(renderer);
    Uint32 elapsed = SDL_GetTicks() - countdownStartTime;
    SDL_Texture* currentTexture = nullptr;

    // ảnh 1
    if (elapsed < 500) {
        currentTexture = countdown3;
    }
    // ảnh 2
    else if (elapsed < 1200) {
        currentTexture = countdown2;
    }
    // ảnh 3
    else if (elapsed < 1900) {
        currentTexture = countdown1;
    }
    // ành go
    else if (elapsed < 2600) {
        currentTexture = countdownGO;
    }

    if (currentTexture) {
        SDL_RenderCopy(renderer, currentTexture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}

void Game::renderRunning() {
    SDL_RenderClear(renderer);
    // Render map và các entity trong game
    map->DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    // Giải phóng các texture đã tải
    SDL_DestroyTexture(splashTexture);
    SDL_DestroyTexture(countdown3);
    SDL_DestroyTexture(countdown2);
    SDL_DestroyTexture(countdown1);
    SDL_DestroyTexture(countdownGO);
    delete map;
    SDL_Quit();
}
