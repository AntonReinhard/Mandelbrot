#pragma once

#include <cstdint>
#include <complex>
#include <string>
#include <cmath>
#include <condition_variable>
#include <atomic>
#include <fstream>

#include <gmp.h>
#include <gmpxx.h>
#include "bitmap_image.hpp"

std::mutex m;
std::atomic<int> thread_counter{0};

constexpr uint_fast32_t gThreshold = 4;
constexpr uint_fast32_t N_MAX = 1024;

rgb_t colors[1000];

void mandelbrot_init() {
	mpf_set_default_prec(64);
}

void read_colors() {
	std::ifstream f("colors");
	for (unsigned i = 0; i < 1000; ++i) {
		f >> colors[i].red;
		f >> colors[i].green;
		f >> colors[i].blue;
	}
}

uint_fast32_t gray_value_of_sequence(const mpf_class& real, const mpf_class& imag) {
	mpf_class z_real{0, real.get_prec()};
	mpf_class z_imag{0, imag.get_prec()};
	
	mpf_class z_rt{0, real.get_prec()};

	for (uint_fast32_t i = 0; i < N_MAX; ++i) {
		if ((z_real.get_d() * z_real.get_d() + z_imag.get_d() * z_imag.get_d()) > gThreshold) {
			return unsigned(1000.0 * std::log2(1.75 + i - std::log2(std::log2(sqrt((z_real.get_d() * z_real.get_d() + z_imag.get_d() * z_imag.get_d()))))) / std::log2(N_MAX));
		}

		//z = z*z + c
		z_rt = z_real;
		
		z_real = z_rt * z_rt - z_imag * z_imag + real;
		z_imag = 2 * (z_rt * z_imag)           + imag;
	}
	return 0;
}

void write_row(bitmap_image& img, const uint_fast32_t width, const mpf_class& xmin, const mpf_class& xmax, const mpf_class& y, const uint_fast32_t row) {
	mpf_class delta_x{(xmax - xmin) / width, 64};

	mpf_class c_real{xmin, xmin.get_prec()};

	for (uint_fast32_t i = 0; i < width; ++i) {
		c_real += delta_x;
		const auto g_value = gray_value_of_sequence(c_real, y);
		img.set_pixel(i, row, prism_colormap[g_value]);
	}
}

void write_frame(const mpf_class& x_centre, const mpf_class& y_centre, const mpf_class& w, const std::string& out, uint_fast32_t image_width, uint_fast32_t image_height) {
	static uint_fast32_t n = 0;

	const mpf_class h{w * mpf_class(image_height) / (mpf_class)(image_width), w.get_prec()};

	const mpf_class x_min{x_centre - w / 2, w.get_prec()};
	const mpf_class x_max{x_centre + w / 2, w.get_prec()};
	const mpf_class y_min{y_centre - h / 2, h.get_prec()};
	const mpf_class y_max{y_centre + h / 2, h.get_prec()};

	//std::cout << "\nHeight: " << h << "\nWidth: " << w << std::endl;

	//std::cout << "\nImage width: " << image_width << " Image height: " << image_height << std::endl;

	bitmap_image frame(image_width, image_height);

	const mpf_class delta_y{(y_max - y_min) / image_height, 64};
	auto y = y_min;

	for (uint_fast32_t i = 0; i < image_height; ++i) {
		//std::cout << "\rRow " << i << "/" << image_height << std::endl;

		write_row(frame, image_width, x_min, x_max, y, i);
		y += delta_y;
	}

	frame.save_image(out);
	++n;

	//decrease counter, cause we're done
	std::lock_guard<std::mutex> lk(m);
	--thread_counter;
}
