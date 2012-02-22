OS = $(shell uname -s)

# Mac
ifeq ($(OS), Darwin)
COMPILER = g++
INCLUDES = -I/opt/local/include
LIBS = -framework OpenGL -framework GLUT -framework ApplicationServices -lGLEW -lm
endif
ifeq ($(OS), Linux)
	COMPILER = g++
	LIBS = -lm -lGL -lglut -lGLEW
endif

all: main

main: ../common/shader_utils.o Cube.o main.o
	$(COMPILER) $(LIBS) main.o Cube.o ../common/shader_utils.o -o main
main.o: main.cpp
	$(COMPILER) $(INCLUDES) -c main.cpp
Cube.o: Cube.cpp
	$(COMPILER) $(INCLUDES) -c Cube.cpp

clean:
	rm -f *.o main
