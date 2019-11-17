#include "Mandelbrot.h"
#include "helpers.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <Windows.h>

int main() {
	const auto start = std::chrono::steady_clock::now();

	const auto resolution = get_option("What resolution do you want to render in",
									   { "16k", "8k", "4k", "FullHD" });

	
	uint32_t image_width = 1920;
	uint32_t image_height = 1080;

	switch(resolution)
	{
	case 1:	//16k
		image_width = 15360;
		image_height = 8640;
		break;

	case 2: //8k
		image_width = 7680;
		image_height = 4320;
		break;

	case 3:	//4k
		image_width = 3840;
		image_height = 2160;
		break;

	case 4:	//FullHD
		image_width = 1920;
		image_height = 1080;
		break;

	default:
		break;
	}

	const auto filepath = get_string("What filepath should the images be written to");

	const auto zoom_factor = get_double("What zoom factor do you want");

	const auto im_num = get_number("How many frames");

	const uint32_t max_threads = get_number("How many threads");

	long double zoom = 4;
	long double zoom_x = -1.284001366739858;
	long double zoom_y = -0.4275712883013326;

	for (uint32_t i = 1; i <= im_num; ++i) {
		std::stringstream filename;
		filename << filepath << "/frame" << std::setfill('0') << std::setw(5) << i << ".bmp";

		while (thread_counter >= max_threads) {
			Sleep(50);
		}

		++thread_counter;
		std::thread t(write_frame, zoom_x, zoom_y, zoom, filename.str(), image_width, image_height);
		t.detach();

		zoom *= zoom_factor;
		std::cout << "\rFrame: " << i << std::flush;
	}

	//wait for remaining threads
	while (thread_counter > 0) {
		Sleep(50);
	}
	
	const auto end = std::chrono::steady_clock::now();
	const auto diff = end - start;
	const auto sec = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
	const auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

	std::cout << "\nTook: " << sec << "." << msec << "s" << std::endl;

	system("PAUSE");

	return 0;
}

//115 sec
//112 nach arithmetik
//5 sec release x64
//3.39 sec
//603 sec with 65535 N_MAX
//422 sec for 25000x22000 with N_MAX = 100
