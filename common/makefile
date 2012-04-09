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

all: shader_utils.o

shader_utils.o: shader_utils.cpp
	$(COMPILER) $(INCLUDES) $(FLAGS) -c shader_utils.cpp

clean:
	rm -f *.o
