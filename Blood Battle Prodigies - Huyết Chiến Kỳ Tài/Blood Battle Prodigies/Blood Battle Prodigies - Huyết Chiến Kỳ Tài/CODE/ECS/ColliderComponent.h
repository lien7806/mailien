#pragma once
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "../game.h"    
#include <string>
using namespace std;

class ColliderComponent : public Component{
public:
    SDL_Rect collider;
    string tag;
    TransformComponent* transform;
    ColliderComponent(string t){
        tag = t;
    }
    void init() override{
        if (!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }
    void update() override{
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }
};
