CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lGL -lGLEW -lglfw3 -lX11 -lXrandr -lXxf86vm

all: a b

a: TriangleA.cpp
	$(CC) -o triangle_a TriangleA.cpp $(CFLAGS) $(LIBS)

b: TriangleB.cpp
	$(CC) -o triangle_b TriangleB.cpp $(CFLAGS) $(LIBS)

debug: CC += -g -DDEBUG
debug: all

clean:
	rm -rf *.o triangle
