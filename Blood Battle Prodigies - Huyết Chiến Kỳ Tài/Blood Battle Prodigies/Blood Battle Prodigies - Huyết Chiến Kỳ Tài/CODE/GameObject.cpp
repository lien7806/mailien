#include "GameObject.h"
#include "TextureManager.h"

GameObject::~GameObject() {
    SDL_DestroyTexture(objTexture);
}


GameObject::GameObject(const char* texturesheet, int x, int y) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void GameObject::Update() {
    xpos++;
    ypos++;

    scrRect.h = 64;
    scrRect.w = 64;
    scrRect.x = 0;
    scrRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = scrRect.w;
    destRect.h = scrRect.h;
}

void GameObject::Render() {
    SDL_RenderCopy(Game::GetRenderer(), objTexture, &scrRect, &destRect);
}
