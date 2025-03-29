#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Map {
public:
    // Constructor nhận vào đường dẫn file ảnh và renderer
    Map(const std::string& filePath, SDL_Renderer* renderer);

    ~Map();

    // Load lại texture nếu cần (có thể dùng để thay đổi map)
    void LoadMap(const std::string& filePath);

    // Vẽ map (ảnh nền) ra màn hình
    void DrawMap();

private:
    SDL_Texture* mapTexture;
    SDL_Renderer* renderer;
};

#endif // MAP_H
