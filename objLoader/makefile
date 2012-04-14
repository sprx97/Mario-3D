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

all: gallery

gallery: gallery.o Mesh.o helper.o mpoint.o Material.o draw_flower.o
	$(COMPILER) gallery.o Mesh.o helper.o mpoint.o Material.o draw_flower.o -o gallery $(LIBS)

gallery.o: gallery.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c gallery.cpp
Mesh.o: Mesh.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Mesh.cpp
helper.o: helper.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c helper.cpp
mpoint.o: mpoint.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c mpoint.cpp
Material.o: Material.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c Material.cpp
draw_flower.o: draw_flower.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c draw_flower.cpp

clean:
	rm -f *.o gallery
