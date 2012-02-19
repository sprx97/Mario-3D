COMPILER = g++
INCLUDES = -L/opt/local/lib -I/opt/local/include
LIBS = -framework OpenGL -framework GLUT -lGLEW -lm

all: main

main: ../common/shader_utils.o cube.o main.o
	$(COMPILER) $(LIBS) main.o cube.o ../common/shader_utils.o -o main
main.o: main.cpp
	$(COMPILER) $(INCLUDES) -c main.cpp
cube.o: cube.cpp
	$(COMPILER) $(INCLUDES) -c cube.cpp

clean:
	rm -f *.o main
