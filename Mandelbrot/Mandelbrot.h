#pragma once

#include <cstdint>
#include <complex>
#include <string>
#include <cmath>
#include <condition_variable>
#include <atomic>

#include "bitmap_image.hpp"
#include <gmp.h>

std::mutex m;
std::atomic<int> thread_counter = 0;

constexpr uint32_t gThreshold = 4;
constexpr uint32_t N_MAX = 1024;

typedef std::complex<long double> comp;

void mandelbrot_init()
{
	mpf_set_default_prec(128);
}

inline uint32_t gray_value_of_sequence(mpf_t real, mpf_t imag) {
	mpf_t z_real, z_imag, z_real_square, z_imag_square;
	mpf_init2(z_real, mpf_get_prec(real));
	mpf_init2(z_imag, mpf_get_prec(imag));
	mpf_init2(z_real_square, 64);
	mpf_init2(z_imag_square, 64);

	mpf_t t;
	mpf_init2(t, 64);

	for (uint32_t i = 0; i < N_MAX; ++i) {
		mpf_mul(z_real_square, z_real, z_real);
		mpf_mul(z_imag_square, z_imag, z_imag);
		mpf_add(t, z_real_square, z_imag_square);
		if (mpf_cmp_ui(t, gThreshold) > 0) {
			mpf_t z_mag;
			mpf_init(z_mag);
			mpf_sqrt(z_mag, t);

			return unsigned(1000.0 * std::log2(1.75 + i - std::log2(std::log2(mpf_get_d(z_mag)))) / std::log2(N_MAX));
		}

		// z = z * z + c;

	}

	mpf_clear(z_real);
	mpf_clear(z_imag);
	return 0;
}

inline void write_row(bitmap_image& img, const uint32_t width, const long double xmin, const long double xmax, const long double y, const uint32_t row) {
	const comp deltaX((xmax - xmin) / width, 0);
	comp z(xmin, y);
	for (uint32_t i = 0; i < width; ++i) {
		z += deltaX;
		//img.set_pixel(i, row, jet_colormap[gray_value_of_sequence(z)]);
	}
}

inline void write_frame(const long double x_centre, const long double y_centre, const long double w, const std::string& out, uint32_t image_width, uint32_t image_height) {
	static uint32_t n = 0;

	const auto h = w * long double(image_height) / long double(image_width);

	const auto x_min = x_centre - w / 2;
	const auto x_max = x_centre + w / 2;
	const auto y_min = y_centre - h / 2;
	const auto y_max = y_centre + h / 2;

	bitmap_image frame(image_width, image_height);

	comp x(0, 0);

	const auto deltaY = (y_max - y_min) / image_height;
	auto y = y_min;

	for (uint32_t i = 0; i < image_height; ++i) {
		write_row(frame, image_width, x_min, x_max, y, i);
		y += deltaY;
	}

	frame.save_image(out);
	++n;

	//decrease counter, cause we're done
	std::lock_guard<std::mutex> lk(m);
	--thread_counter;
}
