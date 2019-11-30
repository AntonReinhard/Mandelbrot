#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <ctime>
#include "SDL_setup.h"

SDL_Renderer* gRenderer = nullptr;

SDL_Window* gWindow = nullptr;

int* gActual_screen_width = nullptr;
int* gActual_screen_height = nullptr;
unsigned* gFrame_rate = nullptr;
unsigned* gTicks_per_frame = nullptr;

SDL_Texture* load_texture(const std::string& path)
{
	//The final texture
	SDL_Texture* new_texture = nullptr;

	//Load image at specified path
	const auto loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
		if (new_texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loaded_surface);
	}

	return new_texture;
}

bool init_graphics()
{
	//Initialization flag
	auto success = true;

	gFrame_rate = new unsigned(60);
	gTicks_per_frame = new unsigned(1000 / *gFrame_rate);

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		const auto screen_width = 1920;
		const auto screen_height = 1080;

		const std::string window_flags = "windowed";

		Uint32 flags;
		if (window_flags == "fullscreen") flags = SDL_WINDOW_FULLSCREEN;
		else if (window_flags == "borderless") flags = SDL_WINDOW_BORDERLESS;
		else if (window_flags == "windowed") flags = SDL_WINDOW_OPENGL;
		else flags = 0;
		gWindow = SDL_CreateWindow("Mandelbrot Zoom Chooser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);

		gActual_screen_width = new int();
		gActual_screen_height = new int();
		SDL_GetWindowSize(gWindow, gActual_screen_width, gActual_screen_height);

		if (gWindow == nullptr)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			//SDL_RENDERER_PRESENTVSYNC locks frame rate to the monitors' frame rate

			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//set logical screen resolution
				SDL_RenderSetLogicalSize(gRenderer, 1920, 1080);
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);	//so alpha can be used

				//Initialize PNG loading
				const int img_flags = IMG_INIT_PNG;
				if (!(IMG_Init(img_flags) & img_flags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

			}
		}
	}

	return success;
}