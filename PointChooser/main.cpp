#include <iostream>

#include "SDL_setup.h"
#include "../Mandelbrot/Mandelbrot.h"

#undef main

int main(int argc, char** argv)
{
	init_graphics();

	std::cout.precision(16);

	const double zoom_factor = 0.5;
	mpf_class zoom = 3.5;
	mpf_class x_centre = -0.761574;
	mpf_class y_centre = -0.0847596;

	while (true) {
		const mpf_class w = zoom;
		const mpf_class h = w * 1080 / 1920;
		const mpf_class x_min = x_centre - w / 2;
		const mpf_class x_max = x_centre + w / 2;
		const mpf_class y_min = y_centre - h / 2;
		const mpf_class y_max = y_centre + h / 2;

		write_frame(x_centre, y_centre, w, "temp.bmp", 960, 540);

		//wait a little so the image is ready
		SDL_Delay(200);

		auto tex = load_texture("temp.bmp");

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		SDL_RenderCopy(gRenderer, tex, nullptr, nullptr);
		SDL_UpdateWindowSurface(gWindow);

		//Update screen
		SDL_RenderPresent(gRenderer);

		auto clicked = false;
		while (!clicked)
		{
			SDL_Event e;
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//If mouse event happened
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					//Get mouse position
					int x, y;
					SDL_GetMouseState(&x, &y);

					//recalculate the centre
					x_centre = x_min + w / double(LOGICAL_SCREEN_WIDTH) * double(x);
					y_centre = y_min + h / double(LOGICAL_SCREEN_HEIGHT) * double(y);
					zoom *= zoom_factor;
					clicked = true;
					std::cout << "x_centre: " << x_centre << std::endl;
					std::cout << "y_centre: " << y_centre << std::endl;
					std::cout << "zoom:     " << zoom << std::endl;
				}
			}
			SDL_Delay(100);
		}
	}
}