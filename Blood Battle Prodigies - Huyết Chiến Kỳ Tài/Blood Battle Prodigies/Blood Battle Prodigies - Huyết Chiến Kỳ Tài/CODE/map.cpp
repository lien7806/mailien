#include "map.h"
#include <iostream>

Map::Map(const std::string& filePath, SDL_Renderer* renderer)
    : renderer(renderer), mapTexture(nullptr)
{
    // Tải texture ngay khi khởi tạo
    LoadMap(filePath);
}

Map::~Map() {
    if (mapTexture) {
        SDL_DestroyTexture(mapTexture);
    }
}

void Map::LoadMap(const std::string& filePath) {
    // Nếu đã có texture rồi, giải phóng nó trước
    if (mapTexture) {
        SDL_DestroyTexture(mapTexture);
        mapTexture = nullptr;
    }

    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    if (!tempSurface) {
        std::cerr << "Lỗi load map texture: " << IMG_GetError() << std::endl;
        return;
    }

    mapTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!mapTexture) {
        std::cerr << "Lỗi tạo texture từ map: " << SDL_GetError() << std::endl;
    }
}

void Map::DrawMap() {
    if (mapTexture) {
        // Render toàn bộ texture (nếu kích thước ảnh phù hợp với cửa sổ)
        SDL_RenderCopy(renderer, mapTexture, nullptr, nullptr);
    }
}
