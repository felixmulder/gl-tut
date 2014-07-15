CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lGL -lGLEW -lglfw3 -lX11 -lXrandr -lXxf86vm

SRC = Triangle.cpp

all: $(SRC)
	$(CC) -o triangle $(SRC) $(CFLAGS) $(LIBS)

debug: CC += -g -DDEBUG
debug: all

clean:
	rm -rf *.o triangle
