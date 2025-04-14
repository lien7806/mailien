#include "game.h"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Components.h"
#include "vector2D.h"
#include "Collision.h"
#include "ECS/GokuController.h"
#include "ECS/gravity.h"
#include "ECS/ChiaotzuController.h"
#include "ECS/HPComponent.h"


SDL_Renderer* Game::renderer = nullptr; // RENDERER dùng để vẽ
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

    player->addComponent<TransformComponent>(1000.0f, 100.0f, 41, 21, 4.0); // kích thước chiao, vị trí xhien
    player->addComponent<SpriteComponent>("assets/player2.png");
    player->addComponent<ChiaotzuController>();
    player->addComponent<ColliderComponent>("player");
    player->addComponent<GravityComponent>();
    player->addComponent<HPComponent>(1000, 400, 25, 1000); // thanh máu CHIAO 
    player->addComponent<ActionComponent>();

    wall->addComponent<TransformComponent>(100.0f, 100.0f, 39, 32, 4.0); // kích thước goku, vị trí xhien
    wall->addComponent<SpriteComponent>("assets/player1.png");
    wall->addComponent<ColliderComponent>("wall");
    wall->addComponent<GokuController>();
    wall->addComponent<GravityComponent>();
    wall->addComponent<HPComponent>(1000, 400, 25, 1000); // thanh máu GOKU
    wall->addComponent<ActionComponent>();
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        isRunning = false;
    }

    // kick chuột 
    if (state == GameState::SPLASH && event.type == SDL_MOUSEBUTTONDOWN) {
        state = GameState::COUNTDOWN;
        countdownStartTime = SDL_GetTicks();
    }
}

void Game::update() {
    manager.refresh();
    switch (state) {
    case GameState::SPLASH: break;
    case GameState::COUNTDOWN: {
        if (SDL_GetTicks() - countdownStartTime > 2600) // tgian mở đầu 
            state = GameState::RUNNING;
        break;
    }
    case GameState::RUNNING:
        manager.update();

        //// va chạm → giảm HP
        //if (Collision::AABB(
        //    player->getComponent<ColliderComponent>().collider,
        //    wall->getComponent<ColliderComponent>().collider))
        //{
        //    auto& wHP = wall->getComponent<HPComponent>();
        //    wHP.currentHealth = std::max(0, wHP.currentHealth - 10);
        //    
        //    // ví dụ tăng nlg khi đánh
        //    wHP.currentEnergy = std::min(wHP.maxEnergy, wHP.currentEnergy + 5);

        //    if (wHP.isDead())
        //        wall->destroy();
        //}
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
void Game::renderRunning() {
    SDL_RenderClear(renderer);

    map->DrawMap();
    manager.draw(); // vẽ các entity

    // vẽ thanh ở phía trên
    int margin = 10;
    int y = margin;

    // Chiaotzu
    {
        auto& hp = player->getComponent<HPComponent>();
        float hpPct = float(hp.currentHealth) / hp.maxHealth;
        float enPct = float(hp.currentEnergy) / hp.maxEnergy;

        // thanh máu
        SDL_Rect bg = { margin, y, hp.barWidth, hp.barHeight };
        SDL_Rect fg = { margin, y, int(hp.barWidth * hpPct), hp.barHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // màu nền
        SDL_RenderFillRect(renderer, &bg);
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // màu đỏ
        SDL_RenderFillRect(renderer, &fg);

        //nlg
        int ey = y + hp.barHeight + 4;
        SDL_Rect ebg = { margin, ey, hp.barWidth, hp.barHeight };
        SDL_Rect efg = { margin, ey, int(hp.barWidth * enPct), hp.barHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);// màu nền
        SDL_RenderFillRect(renderer, &ebg);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // màu vàng
        SDL_RenderFillRect(renderer, &efg);
    }

    // Goku
    {
        auto& hp = wall->getComponent<HPComponent>();
        float hpPct = float(hp.currentHealth) / hp.maxHealth;
        float enPct = float(hp.currentEnergy) / hp.maxEnergy;

        // HP bar
        int x0 = 1200 - margin - hp.barWidth;
        SDL_Rect bg = { x0, y, hp.barWidth, hp.barHeight };
        SDL_Rect fg = { x0 + (hp.barWidth - int(hp.barWidth * hpPct)), y,
                        int(hp.barWidth * hpPct), hp.barHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // màu nền
        SDL_RenderFillRect(renderer, &bg);
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // màu đỏ
        SDL_RenderFillRect(renderer, &fg);

        // Energy bar
        int ey = y + hp.barHeight + 4;
        SDL_Rect ebg = { x0, ey, hp.barWidth, hp.barHeight };
        SDL_Rect efg = { x0 + (hp.barWidth - int(hp.barWidth * enPct)), ey,
                         int(hp.barWidth * enPct), hp.barHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // màu nền
        SDL_RenderFillRect(renderer, &ebg);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // màu vàng
        SDL_RenderFillRect(renderer, &efg);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
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
    if (elapsed < 600) {
        currentTexture = countdown3;
    }
    // ảnh 2
    else if (elapsed < 1200) {
        currentTexture = countdown2;
    }
    // ảnh 3
    else if (elapsed < 1800) {
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
