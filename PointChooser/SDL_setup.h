#pragma once

#include <SDL.h>

const int LOGICAL_SCREEN_WIDTH = 1920;
const int LOGICAL_SCREEN_HEIGHT = 1080;

extern int* gActual_screen_width;
extern int* gActual_screen_height;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

SDL_Texture* load_texture(const std::string& path);	//loads an image from the specified path

bool init_graphics();							//initializes the Graphics, generates the window etc.