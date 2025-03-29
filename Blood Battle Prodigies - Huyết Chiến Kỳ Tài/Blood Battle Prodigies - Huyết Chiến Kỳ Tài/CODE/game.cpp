#include <iostream>
#include "game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "map.h"
#include "ECS.h"
#include "Components.h"

// các đx tượng game
GameObject* player1;
GameObject* player2;

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
auto& newPlayer(manager.addEntity());

SDL_Event Game::event;

using namespace std;
Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << " Subsystems Initialised!..." << endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            cout << "Window created!" << endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
            isRunning = true;
        }
        else {
            isRunning = false;
        }
    }

    // ktao Map 
    map = new Map("assets/background.png", renderer); // ktao map vs đg dẫn 
	map->LoadMap("assets/background.png"); // truyền đg dẫn vào hàm LoadMap

	// ktao player
    player1 = new GameObject("assets/Chiaotzu1.png", 0, 0);
    player2 = new GameObject("assets/Goku1.png", 50, 50);
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

void Game::update()
{
    player1->Update();
    player2->Update();
  
}

void Game::render()
{
    SDL_RenderClear(renderer);

    // vẽ map trước
    map->DrawMap(); 

    player1->Render();
    player2->Render();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    if (map) {
        delete map;
        map = nullptr;
    }

    if (player1) {
        delete player1;
        player1 = nullptr;
    }
    if (player2) {
        delete player2;
        player2 = nullptr;
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}