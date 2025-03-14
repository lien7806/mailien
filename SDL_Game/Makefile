# all:
# 	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2
SRC_DIR = .
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.5/include 
LIBRARY_PATHS = -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/Cellar/sdl2_image/2.8.5/lib
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g
LINKER_FLAGS = -lSDL2 -lSDL2_image

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)