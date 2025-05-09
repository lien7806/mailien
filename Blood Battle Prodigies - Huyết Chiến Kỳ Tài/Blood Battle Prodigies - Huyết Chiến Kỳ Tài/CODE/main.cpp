﻿#include "game.h"
#include "SDL_image.h"

// Khai báo đối tượng game toàn cục
Game* game = nullptr;

int main(int argc, char* argv[]) {
    const int FPS = 60; // tốc độ cập nhật game

    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    // khởi tạo
    game->init("Blood Battle Prodigies - HuyetChienKyTai", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 765, false); // crong,ccao mh

    while (game->running()) {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();
	delete game;
    return 0;
}