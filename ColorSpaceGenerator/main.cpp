#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <fstream>

struct RGB {
	int r;
	int g;
	int b;
};

unsigned distance(const RGB& a, const RGB& b) {
	return unsigned(sqrt(  (a.r-b.r)*(a.r-b.r)
						 + (a.g-b.g)*(a.g-b.g)
						 + (a.b-b.b)*(a.b-b.b)  ));
}

int main(int argc, char *argv[]) {

	std::ofstream f("out");

	std::srand(time(0));

	const unsigned len = 1000;
	const unsigned gradient_len = 50;

	const unsigned min_distance = 30;
	const unsigned max_distance = 100;

	std::vector<RGB> colors;
	colors.resize(len);

	colors[0] = {0, 0, 0};

	RGB last_color{0, 0, 0};
	RGB target_color{0, 0, 0};
	for (unsigned i = 0; i < len - 1; ++i) {

		if (i % gradient_len == 0) { 	//generate next color goal
			last_color = colors[i];

			if (i == len-gradient_len) {
				target_color.r = 255;
				target_color.g = 255;
				target_color.b = 255;
			} else {
				unsigned dist;
				do {
					target_color.r = rand() % 256;
					target_color.g = rand() % 256;
					target_color.b = rand() % 256;
					dist = distance(target_color, last_color);
				} while (dist > max_distance || dist < min_distance);
			}

			std::cout << "Target color: "  << std::setw(3) << std::setfill('0') << unsigned(target_color.r) << ", " 
										   << std::setw(3) << std::setfill('0') << unsigned(target_color.g) << ", " 
										   << std::setw(3) << std::setfill('0') << unsigned(target_color.b) << std::endl;
		}

		colors[i + 1].r = last_color.r + double(target_color.r - last_color.r) * double(i%gradient_len)/double(gradient_len);
		colors[i + 1].g = last_color.g + double(target_color.g - last_color.g) * double(i%gradient_len)/double(gradient_len);
		colors[i + 1].b = last_color.b + double(target_color.b - last_color.b) * double(i%gradient_len)/double(gradient_len);

		f << colors[i].r << " " << colors[i].g << " " << colors[i].b << std::endl;
	}

	return 0;
}
