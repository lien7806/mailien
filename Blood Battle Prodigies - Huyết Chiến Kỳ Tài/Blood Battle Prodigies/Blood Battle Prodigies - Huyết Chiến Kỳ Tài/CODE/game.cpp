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
#include "SDL_mixer.h"

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
    guideTexture(nullptr),
    showGuide(false),
    map(nullptr),
    player(nullptr),
    wall(nullptr),
    gokuHypnotized(false)
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
    splashTexture = TextureManager::LoadTexture("assets/modau.png");
    countdown3 = TextureManager::LoadTexture("assets/3.png");
    countdown2 = TextureManager::LoadTexture("assets/2.png");
    countdown1 = TextureManager::LoadTexture("assets/1.png");
    countdownGO = TextureManager::LoadTexture("assets/GO.png");
    guideTexture = TextureManager::LoadTexture("assets/huongdan.png");
    gokuWinsTexture = TextureManager::LoadTexture("assets/gokuwin.png");
    chiaoWinsTexture = TextureManager::LoadTexture("assets/chiaowin.png");
    gameOverTexture = TextureManager::LoadTexture("assets/gameover.png");

    map = new Map("assets/background.png", renderer);
    map->LoadMap("assets/background.png");

    player = &manager.addEntity();
    wall = &manager.addEntity();

    ::chiao = player;
    ::goku = wall;

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
    initAudio();
}
void Game::initAudio() {
    int mixFlags = MIX_INIT_OGG;
    if ((Mix_Init(mixFlags) & mixFlags) != mixFlags) {
        std::cout << "Mix_Init lỗi: " << Mix_GetError() << std::endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer không khởi tạo được! Lỗi: " << Mix_GetError() << std::endl;
        return;
    }
    bgMusic = Mix_LoadMUS("assets/nhacnen.ogg");
    if (!bgMusic) {
        std::cout << "Không load được nhạc nền! Lỗi: " << Mix_GetError() << std::endl;
        return;
    }
    // âm lượng
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
    if (Mix_PlayMusic(bgMusic, -1) == -1) {
        std::cout << "Lỗi khi phát nhạc: " << Mix_GetError() << std::endl;
    }
    if (Mix_PlayingMusic() == 0) {
        std::cout << "Nhạc chưa phát!" << std::endl;
    }
}
void Game::cleanAudio() {
    Mix_FreeMusic(bgMusic);
    bgMusic = nullptr;
    Mix_FreeChunk(ActionComponent::punchSound);
    Mix_FreeChunk(ActionComponent::gameOverSound);
    Mix_FreeChunk(ActionComponent::gokuWinSound);
    Mix_FreeChunk(ActionComponent::chiaoWinSound);
    Mix_FreeChunk(ActionComponent::countdownSound);
    Mix_CloseAudio();
}
void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (state == GameState::SPLASH && !showGuide) {
            state = GameState::COUNTDOWN;
            countdownStartTime = SDL_GetTicks();
        }
        else if (state == GameState::GAME_OVER) {
            resetGame();
        }
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_b && state == GameState::SPLASH) {
            showGuide = !showGuide;
        }
        else if (event.key.keysym.sym == SDLK_r &&    // Reset-> R
            (state == GameState::GOKU_WINS ||
                state == GameState::CHIAO_WINS ||
                state == GameState::GAME_OVER)) {
            resetGame();
        }
        break;
    }
}
void Game::update() {
    manager.refresh();
    switch (state) {
    case GameState::SPLASH:
        break;
    case GameState::COUNTDOWN: {
        if (SDL_GetTicks() - countdownStartTime > 2400)
            state = GameState::RUNNING;
        break;
    }
    case GameState::RUNNING: {
        if (wall->getComponent<HPComponent>().isDead()) {
            state = GameState::CHIAO_WINS;
            endGameStartTime = SDL_GetTicks();
            Mix_PlayChannel(-1, ActionComponent::chiaoWinSound, 0);
        }
        else if (player->getComponent<HPComponent>().isDead()) {
            state = GameState::GOKU_WINS;
            endGameStartTime = SDL_GetTicks();
            Mix_PlayChannel(-1, ActionComponent::gokuWinSound, 0);
        }
        else {
            manager.update();
        }
        break;
    }
    case GameState::GOKU_WINS:
    case GameState::CHIAO_WINS: {
        if (SDL_GetTicks() - endGameStartTime > END_GAME_DELAY) {
            state = GameState::GAME_OVER;
            Mix_PlayChannel(-1, ActionComponent::gameOverSound, 0);
        }
        break;
    }
    case GameState::GAME_OVER:
        break;
    }
}
void Game::render() {
    switch (static_cast<int>(state)) {
    case static_cast<int>(GameState::SPLASH):
        renderSplash();
        break;
    case static_cast<int>(GameState::COUNTDOWN):
        renderCountdown();
        break;
    case static_cast<int>(GameState::RUNNING):
        renderRunning();
        break;
    case static_cast<int>(GameState::GOKU_WINS):
        renderEndGame(gokuWinsTexture);
        break;
    case static_cast<int>(GameState::CHIAO_WINS):
        renderEndGame(chiaoWinsTexture);
        break;
    case static_cast<int>(GameState::GAME_OVER):
        renderEndGame(gameOverTexture);
        break;
    default:
        break;
    }
}
void Game::renderRunning() {
    SDL_RenderClear(renderer);

    map->DrawMap();
    manager.draw();
    int margin = 10;
    int y = margin;

    // Chiaotzu
    {
        auto& hp = wall->getComponent<HPComponent>();
        float hpPct = float(hp.currentHealth) / hp.maxHealth;
        float enPct = float(hp.currentEnergy) / hp.maxEnergy;

        SDL_Rect bg = { margin, y, hp.barWidth, hp.barHeight };
        SDL_Rect fg = { margin, y, int(hp.barWidth * hpPct), hp.barHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // màu nền
        SDL_RenderFillRect(renderer, &bg);
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // màu đỏ
        SDL_RenderFillRect(renderer, &fg);

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
        auto& hp = player->getComponent<HPComponent>();
        float hpPct = float(hp.currentHealth) / hp.maxHealth;
        float enPct = float(hp.currentEnergy) / hp.maxEnergy;

        int x0 = 1200 - margin - hp.barWidth;
        SDL_Rect bg = { x0, y, hp.barWidth, hp.barHeight };
        SDL_Rect fg = { x0 + (hp.barWidth - int(hp.barWidth * hpPct)), y,
                        int(hp.barWidth * hpPct), hp.barHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // màu nền
        SDL_RenderFillRect(renderer, &bg);
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // màu đỏ
        SDL_RenderFillRect(renderer, &fg);

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
    if (showGuide && guideTexture) {
        SDL_RenderCopy(renderer, guideTexture, nullptr, nullptr);
    }
    else if (splashTexture) {
        SDL_RenderCopy(renderer, splashTexture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}

void Game::renderCountdown() {
    SDL_RenderClear(renderer);
    Uint32 elapsed = SDL_GetTicks() - countdownStartTime;
    SDL_Texture* currentTexture = nullptr;
    static bool soundPlayed = false;
    if (elapsed < 600) {
        currentTexture = countdown3;
    }
    else if (elapsed < 1200) {
        currentTexture = countdown2;
    }
    else if (elapsed < 1400) {
        currentTexture = countdown1;
    }
    else if (elapsed < 2400) {
        currentTexture = countdownGO;
    }
    if (!soundPlayed && ActionComponent::countdownSound) {
        Mix_PlayChannel(-1, ActionComponent::countdownSound, 0);
        soundPlayed = true;
    }
    if (currentTexture) {
        SDL_RenderCopy(renderer, currentTexture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}

void Game::renderEndGame(SDL_Texture* texture) {
    SDL_RenderClear(renderer);
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}
void Game::resetGame() {
    player->getComponent<HPComponent>().currentHealth = player->getComponent<HPComponent>().maxHealth;
    player->getComponent<HPComponent>().currentEnergy = 0;
    player->getComponent<TransformComponent>().position = { 1000.0f, 100.0f };

    wall->getComponent<HPComponent>().currentHealth = wall->getComponent<HPComponent>().maxHealth;
    wall->getComponent<HPComponent>().currentEnergy = 0;
    wall->getComponent<TransformComponent>().position = { 100.0f, 100.0f };

    gokuHypnotized = false;
    isFalling = false;
    state = GameState::SPLASH;
    showGuide = false;
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(splashTexture);
    SDL_DestroyTexture(countdown3);
    SDL_DestroyTexture(countdown2);
    SDL_DestroyTexture(countdown1);
    SDL_DestroyTexture(countdownGO);
    SDL_DestroyTexture(guideTexture);
    SDL_DestroyTexture(gokuWinsTexture);
    SDL_DestroyTexture(chiaoWinsTexture);
    SDL_DestroyTexture(gameOverTexture);
    delete map;
    SDL_Quit();
}
