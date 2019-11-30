CPPFLAGS= -pthread -O3
LDFLAGS=
LDLIBS= -lgmp -lgmpxx -pthread -lSDL2 -lSDL2_image

PointChooser: main.o SDL_setup.o
	g++ $(LDFLAGS) -o PointChooser main.o SDL_setup.o $(LDLIBS)

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

SDL_setup.o: SDL_setup.cpp SDL_setup.h
	g++ $(CPPFLAGS) -c SDL_setup.cpp
