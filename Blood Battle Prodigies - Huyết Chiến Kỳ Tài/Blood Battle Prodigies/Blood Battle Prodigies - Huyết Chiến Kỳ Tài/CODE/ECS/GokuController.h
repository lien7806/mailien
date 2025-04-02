// cách di chuyển của goku

#pragma once
#include "ECS.h"
#include "Components.h"
#include "game.h"

class ArrowController : public Component {
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

        if (keystate[SDL_SCANCODE_UP]) {
            transform->velocity.y = -1;
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            transform->velocity.y = 1;
        }
        if (keystate[SDL_SCANCODE_LEFT]) {
            transform->velocity.x = -1;
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
            transform->velocity.x = 1;
        }
    }
};
