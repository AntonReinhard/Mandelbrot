#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

std::mutex m;
std::atomic<int> thread_counter = 0;

typedef std::vector<unsigned> image_row;
typedef std::vector<image_row> image_mat;

constexpr uint32_t GRAYVALUES = 255;
//constexpr long double GAMMACORRECT = 0.2;
constexpr uint32_t THRESHOLD = 4;
constexpr uint32_t NMAX = 255;
 
void createPGM(std::ofstream &img, const uint32_t height, const uint32_t width) {
   img.open("img.pgm");
   img << "P2\n"
       << width << "\n"
       << height << "\n"
       << GRAYVALUES << std::endl;
}

auto grayValueOfSequence(const double cReal, const double cImag) -> uint32_t {
   auto z_real = 0.;
   auto z_imag = 0.;
   for(uint32_t i = 0; i < NMAX; ++i) {
      const auto z_im2 = z_imag*z_imag;

      if (z_real * z_real + z_im2 > THRESHOLD) {
         return i;
      }

      z_imag = 2 * z_real * z_imag     + cImag;
      z_real = z_real * z_real - z_im2 + cReal;
   }
   return NMAX;
}
 
void writeRow(image_row *row, const uint32_t width, const double xmin, const double xmax, const double y) {
   const auto delta_x{(xmax - xmin)/width};
   auto c_real{xmin};
   const auto c_imag{y};
   for (unsigned i = 0; i < width; ++i) {
      (*row)[i] = grayValueOfSequence(c_real, c_imag);
      c_real += delta_x;
   }

   //decrease counter, cause we're done
	std::lock_guard<std::mutex> lk(m);
	--thread_counter;
}

int main() {
   const auto start = std::chrono::steady_clock::now();
   
   const uint32_t w = 2500;
   const uint32_t h = 2200;
   
   image_mat img;
   img.resize(h);
   for (auto& i : img) {
      i.resize(w);
   }

   const auto xmin = -1.9;
   const auto xmax = .6;
   const auto ymin = -1.1;
   const auto ymax = 1.1;

   const auto delta_y = (ymax - ymin) / h;
   auto y = ymin;

   for (uint32_t i = 0; i < h; ++i) {

      while (thread_counter >= 6) {
		}

      ++thread_counter;
      std::thread t(writeRow, &img[i], w, xmin, xmax, y);
      t.detach();

      y += delta_y;
      if (!(i%100))
         std::cout << "\rRow " << i << " of " << h << "..." << std::flush;

   }

   std::cout << "\rDone.                         " << std::endl;

   const auto end = std::chrono::steady_clock::now();
   const auto diff = end - start;
   const auto sec = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
   const auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

   std::cout << "Took: " << sec << "." << msec << "s" << std::endl;

   std::ofstream image;
   createPGM(image, h, w);

   for (unsigned row = 0; row < h; ++row) {
      for (unsigned col = 0; col < w; ++col) {
         image << img[row][col] << " ";
      }
      image << "\n";
   }

   image.close();
}

//~2.2 sec for 2.5kx2.2k with NMAX = 255 and 6 thread
//55 sec for 25kx22k with NMAX = 255 and 6 threads
