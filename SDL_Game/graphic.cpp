// Lê Ngọc Quang
void quitSDL(SDL_window* window, SDL_renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
// Hoàng Sỹ Hùng
void presentScreen(){
    SDL_renderPresent(renderer);
}
//Chu Đức Ánh
void prepareScene(){
    SDL_SetRenderDrawImage(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

//Nguyễn Hải Đăng
void  initSDL (int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE){
    if (SDL_Init(INIT_EVERY_THING) != 0) {
        logErrorAndExit("SDL_INIT", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)){
        logErrorAndExit("IMG_INIT", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNKNOWN, SDL_WINDOWPOS_UNKNOWN, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window = NULL){
        logErrorAndExit("SDL_WINDOW", SDL_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer = NULL){
        logErrorAndExit("SDL_RENDERER", SDL_GetError());
    }
}
//Nguyễn Lê Dũng
void logErrorAndExit(const char* msg, const char* error){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s :%s", msg, error);
    SDL_Quit();
}
// Đỗ Mai Liên
void waitUntilKeyPresed() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) {
            return;
        }
    }
}