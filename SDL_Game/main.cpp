#include <iostream>
#include <SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello World!";

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit( "SDL_image error:", IMG_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //full screen
    //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

void drawSomething(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
    SDL_RenderDrawPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // red
    SDL_RenderDrawLine(renderer, 100, 100, 200, 200);
    SDL_Rect filled_rect;
    filled_rect.x = SCREEN_WIDTH - 400;
    filled_rect.y = SCREEN_HEIGHT - 150;
    filled_rect.w = 320;
    filled_rect.h = 100;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
    SDL_RenderFillRect(renderer, &filled_rect);
}

SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                     "Loading %s", filename);

	SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
	if (texture == NULL) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                       "Load texture %s", IMG_GetError());
    }
	return texture;
}

void renderTexture(SDL_Texture *texture, int x, int y, 
                   SDL_Renderer* renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
    // SDL_Rect srcRect;
    // srcRect.x = 0; 
    // srcRect.y = 0;  
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    // SDL_Rect dstRect = {100, 100, 200, 200}; 
    // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

int main(int argc, char* argv[])
{
    //Khởi tạo môi trường đồ họa
    //SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    //SDL_Renderer* renderer = createRenderer(window);
    //Quang Huy
    Graphic grap;
    grap.initSDL(SCREEN_WIDTH,SCREEN_HEIGHT,WINDOW_TITLE);
    //Xóa màn hình
    //Tien
    //SDL_RenderClear(renderer);
    grap.prepareScene();
    //Vẽ gì đó
    drawSomething(grap.window, grap.renderer);
    
    // SDL_Texture* background = loadTexture("imgs/background.png", renderer);
    // SDL_RenderCopy( renderer, background, NULL, NULL);
    // SDL_Rect dest; 
    // SDL_Rect src; 
    // dest.x = 100; 
    // dest.y = 100;
    // dest.w = 100; 
    // dest.h = 100; 
    // SDL_Texture* mario = loadTexture("imgs/mario_bg.png", renderer);
    // SDL_QueryTexture(mario, NULL, NULL, &src.w, &src.h);
    // std::cout << src.w << " " << src.h; 
    // // double ratio = 0.5; 
    // dest = {100, 100, 100, 100};
    // SDL_RenderCopy( renderer, mario, NULL, &dest);
    // SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
    // SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
    // renderTexture(mario, 100, 200, renderer);
    
    // SDL_Surface* surface = IMG_Load("imgs/marion_white_bg.png");

    // Uint32 colorkey = SDL_MapRGB(surface->format, 255, 255, 255); // Màu hồng thành trong suốt
    // SDL_SetColorKey(surface, SDL_TRUE, colorkey);

    // SDL_Texture* mario = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_FreeSurface(surface);  
    // SDL_Rect dest; 
    // SDL_Rect src; 
    // dest.x = 100; 
    // dest.y = 100;
    // dest.w = 500; 
    // dest.h = 500; 
    // SDL_QueryTexture(mario, NULL, NULL, &src.w, &src.h);
    // std::cout << src.w << " " << src.h; 
    // double ratio = 0.5; 
    // dest = {100, 100, 500, 500};  
    // SDL_RenderCopy( renderer, mario, NULL, &dest);

    // Render texture lên màn hình
    // SDL_RenderCopy(renderer, mario, NULL, NULL);
    
    //Hiện bản vẽ ra màn hình
    SDL_RenderPresent(renderer);

    //Đợi phím bất kỳ trước khi đóng môi trường đồ họa và kết thúc chương trình
    waitUntilKeyPressed();
    // SDL_DestroyTexture( background );
    // background = NULL;

    quitSDL(window, renderer);
    return 0;
}



