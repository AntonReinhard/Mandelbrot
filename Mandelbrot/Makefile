CPPFLAGS= -pthread -O3
LDFLAGS=
LDLIBS= -lgmp -lgmpxx -pthread

Mandelbrot: Mandelbrot.o
	g++ $(LDFLAGS) -o Mandelbrot Mandelbrot.o $(LDLIBS)

Mandelbrot.o: Mandelbrot.cpp Mandelbrot.h
	g++ $(CPPFLAGS) -c Mandelbrot.cpp

