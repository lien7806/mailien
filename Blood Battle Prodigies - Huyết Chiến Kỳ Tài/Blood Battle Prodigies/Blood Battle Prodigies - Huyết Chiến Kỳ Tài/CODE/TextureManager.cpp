#include "TextureManager.h"
#include "SDL_image.h"
#include "game.h"
using namespace std;
SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    if(!tempSurface){
        cout << "Failed " << IMG_GetError()<<endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::GetRenderer(), tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::GetRenderer(), tex, &src, &dest, 0, nullptr, flip);
}
