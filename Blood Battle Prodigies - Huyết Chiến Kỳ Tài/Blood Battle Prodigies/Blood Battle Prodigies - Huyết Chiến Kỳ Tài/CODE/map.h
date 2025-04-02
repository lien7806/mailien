#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Map {
public:

    Map(const std::string& filePath, SDL_Renderer* renderer);

    ~Map();


    void LoadMap(const std::string& filePath);

    // Vẽ map 
    void DrawMap();

private:
    SDL_Texture* mapTexture;
    SDL_Renderer* renderer;
};

#endif // MAP_H
