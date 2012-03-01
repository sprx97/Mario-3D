OS = $(shell uname -s)

# Mac
ifeq ($(OS), Darwin)
	COMPILER = g++
	INCLUDES = -I/opt/local/include
	LIBS = -framework OpenGL -framework GLUT -framework ApplicationServices -lGLEW -lm
endif
# Linux
ifeq ($(OS), Linux)
	COMPILER = g++
	INCLUDES = 
	LIBS = -lm -lGL -lglut -lGLEW
endif

all: main

main: ../common/shader_utils.o Cube.o main.o Files.o
	$(COMPILER) $(LIBS) main.o Cube.o Files.o ../common/shader_utils.o -o main
main.o: main.cpp
	$(COMPILER) $(INCLUDES) -c main.cpp
Cube.o: Cube.cpp
	$(COMPILER) $(INCLUDES) -c Cube.cpp
Files.o: Files.cpp
	$(COMPILER) $(INCLUDES) -c Files.cpp

clean:
	rm -f *.o main
