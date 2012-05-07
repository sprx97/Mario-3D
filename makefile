include $(shell ls config.mak 2>/dev/null)
OS = $(shell uname -s)

# Mac
ifeq ($(OS), Darwin)
	COMPILER = g++
	INCLUDES = -I/opt/local/include
	LIBS = -framework OpenGL -framework GLUT -framework ApplicationServices -lGLEW -lm -lfmodex $(MY_LIBS)
	FLAGS =
endif
# Linux
ifeq ($(OS), Linux)
	COMPILER = g++
	INCLUDES =
	LIBS = -lm -lGL -lglut -lGLEW -lGLU -lfmodex $(MY_LIBS)
	FLAGS =
endif

all: main

main: main.o Cube/Cube.o Shaders/shader_utils.o objLoader/draw_object.o objLoader/draw_shell.o objLoader/draw_flower.o objLoader/draw_mushroom.o objLoader/draw_enemy.o objLoader/draw_koopa.o objLoader/draw_goomba.o objLoader/draw_pipe.o objLoader/draw_flag.o objLoader/draw_star.o objLoader/draw_coin.o objLoader/draw_fireball.o objLoader/Mesh.o objLoader/Material.o objLoader/helper.o objLoader/mpoint.o
	$(COMPILER) main.o Cube/Cube.o Shaders/shader_utils.o objLoader/draw_object.o objLoader/draw_shell.o objLoader/draw_flower.o objLoader/draw_mushroom.o objLoader/draw_pipe.o objLoader/draw_enemy.o objLoader/draw_koopa.o objLoader/draw_goomba.o objLoader/draw_flag.o objLoader/draw_star.o objLoader/draw_coin.o objLoader/draw_fireball.o objLoader/Mesh.o objLoader/Material.o objLoader/helper.o objLoader/mpoint.o -o main $(LIBS)

main.o: main.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c main.cpp
Cube/Cube.o: Cube/Cube.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Cube/Cube.cpp -o Cube/Cube.o
objLoader/draw_object.o: objLoader/draw_object.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_object.cpp -o objLoader/draw_object.o
objLoader/draw_shell.o: objLoader/draw_shell.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_shell.cpp -o objLoader/draw_shell.o
objLoader/draw_star.o: objLoader/draw_star.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_star.cpp -o objLoader/draw_star.o
objLoader/draw_flower.o: objLoader/draw_flower.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_flower.cpp -o objLoader/draw_flower.o
objLoader/draw_mushroom.o: objLoader/draw_mushroom.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_mushroom.cpp -o objLoader/draw_mushroom.o
objLoader/draw_enemy.o: objLoader/draw_enemy.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_enemy.cpp -o objLoader/draw_enemy.o
objLoader/draw_koopa.o: objLoader/draw_koopa.cpp objLoader/draw_object.o objLoader/draw_enemy.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_koopa.cpp -o objLoader/draw_koopa.o
objLoader/draw_goomba.o: objLoader/draw_goomba.cpp objLoader/draw_object.o objLoader/draw_enemy.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_goomba.cpp -o objLoader/draw_goomba.o
objLoader/draw_flag.o: objLoader/draw_flag.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_flag.cpp -o objLoader/draw_flag.o
objLoader/draw_coin.o: objLoader/draw_coin.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_coin.cpp -o objLoader/draw_coin.o
objLoader/draw_fireball.o: objLoader/draw_fireball.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_fireball.cpp -o objLoader/draw_fireball.o
objLoader/draw_pipe.o: objLoader/draw_pipe.cpp objLoader/draw_object.o
	$(COMPILER) $(INCLUDES) $(FLAGS) -c objLoader/draw_pipe.cpp -o objLoader/draw_pipe.o

clean:
	rm -f Shaders/*.o objLoader/*.o Cube/*.o *.o main

