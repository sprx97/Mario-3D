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
	LIBS = -lm -lGL -lglut -lGLEW -lGLU 
	FLAGS =
endif

all: main

main: main.o Cube.o Files.o ../common/shader_utils.o ../objLoader/draw_flower.o ../objLoader/draw_mushroom.o ../objLoader/draw_goomba.o ../objLoader/draw_pipe.o ../objLoader/draw_flag.o ../objLoader/draw_star.o ../objLoader/draw_coin.o ../objLoader/draw_fireball.o ../objLoader/Mesh.o ../objLoader/Material.o ../objLoader/helper.o ../objLoader/mpoint.o
	$(COMPILER) main.o Cube.o Files.o ../common/shader_utils.o ../objLoader/draw_flower.o ../objLoader/draw_mushroom.o ../objLoader/draw_pipe.o ../objLoader/draw_goomba.o ../objLoader/draw_flag.o ../objLoader/draw_star.o ../objLoader/draw_coin.o ../objLoader/draw_fireball.o ../objLoader/Mesh.o ../objLoader/Material.o ../objLoader/helper.o ../objLoader/mpoint.o -o main $(LIBS)
main.o: main.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c main.cpp
Cube.o: Cube.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Cube.cpp
Files.o: Files.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Files.cpp

clean:
	rm -f ../common/*.o ../objLoader/*.o *.o main

