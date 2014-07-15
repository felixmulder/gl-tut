CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lGL -lGLEW -lglfw3 -lX11 -lXrandr -lXxf86vm

all: basic colors

basic: Moving_Triangle.cpp
	$(CC) -o moving $? $(CFLAGS) $(LIBS)

colors: ColorsMoving_Triangle.cpp
	$(CC) -o colors_moving $? $(CFLAGS) $(LIBS)

debug: CC += -g -DDEBUG
debug: all

clean:
	rm -rf *.o moving colors_moving
