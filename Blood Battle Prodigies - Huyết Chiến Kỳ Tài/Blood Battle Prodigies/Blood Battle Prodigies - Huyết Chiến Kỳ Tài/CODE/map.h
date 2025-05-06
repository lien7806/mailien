#ifndef MAP_H
#define MAP_H
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

class Map {
public:
    Map(const string& filePath, SDL_Renderer* renderer);
    ~Map();
    void LoadMap(const string& filePath);
    void DrawMap();
private:
    SDL_Texture* mapTexture;
    SDL_Renderer* renderer;
};
#endif // MAP_H
