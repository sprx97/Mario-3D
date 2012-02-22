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

main: ../common/shader_utils.o cube.o main.o
	$(COMPILER) $(LIBS) main.o cube.o ../common/shader_utils.o -o main
main.o: main.cpp
	$(COMPILER) $(INCLUDES) -c main.cpp
cube.o: cube.cpp
	$(COMPILER) $(INCLUDES) -c cube.cpp

clean:
	rm -f *.o main
