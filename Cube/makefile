OS = $(shell uname -s)

# Mac
ifeq ($(OS), Darwin)
	COMPILER = g++
	INCLUDES = -I/opt/local/include
	LIBS = -framework OpenGL -framework GLUT -framework ApplicationServices -lGLEW -lm
	FLAGS =
endif
# Linux
ifeq ($(OS), Linux)
	COMPILER = g++
	INCLUDES =
	LIBS = -lm -lGL -lglut -lGLEW
	FLAGS =
endif

all: main

main: ../common/shader_utils.o ../objLoader/draw_flower.o ../objLoader/Mesh.o ../objLoader/Material.o ../objLoader/helper.o ../objLoader/mpoint.o Cube.o main.o Files.o
	$(COMPILER) $(LIBS) main.o Cube.o Files.o ../common/shader_utils.o ../objLoader/draw_flower.o ../objLoader/Mesh.o ../objLoader/Material.o ../objLoader/helper.o ../objLoader/mpoint.o -o main
main.o: main.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c main.cpp
Cube.o: Cube.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Cube.cpp
Files.o: Files.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Files.cpp

clean:
	rm -f *.o main
