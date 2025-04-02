#pragma once
#include "Components.h"
#include "SDL.h"
#include "TransformComponent.h" 
#include "../TextureManager.h"

class SpriteComponent : public Component
{
private:
    TransformComponent* transform = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect srcRect, destRect;

public:
    SpriteComponent() = default;
    SpriteComponent(const char* path)
    {
        setTex(path);
    }

    ~SpriteComponent() {
        SDL_DestroyTexture(texture);
    }

    void setTex(const char* path)
    {
        texture = TextureManager::LoadTexture(path);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        int texW, texH;
        SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

        // kích thước
        srcRect.x = srcRect.y = 0;
        srcRect.w = texW;
        srcRect.h = texH;

        destRect.w = texW * transform->scale;
        destRect.h = texH * transform->scale;
    }

    void update() override {
        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
