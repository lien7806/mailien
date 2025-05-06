#pragma once
#include "ECS.h"
#include "Components.h"
#include "../game.h"

class GokuController : public Component {
public:
    TransformComponent* transform;
    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }
    void update() override {
        //  vận tốc mỗi frame để không bị cộng dồn
        transform->velocity.x = 0;
        transform->velocity.y = 0;

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_W] && transform->scope) {
            transform->velocity.y = -60; // nhảy cao tầm
            transform->scope = false;
        }
        if (keystate[SDL_SCANCODE_A]) {
            transform->velocity.x = -1;
        }
        if (keystate[SDL_SCANCODE_D]) {
            transform->velocity.x = 1;
        }
    }
};