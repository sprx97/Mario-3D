COMPILER = g++
INCLUDES = -L/opt/local/lib -I/opt/local/include
LIBS = -framework OpenGL -framework GLUT -lGLEW -lm

LDLIBS= -lm -I/opt/local/include -L/opt/local/lib -F/opt/local/lib -framework OpenGL -lGLEW -framework GLUT -I/System/Library/Frameworks -framework Cocoa -framework ApplicationServices -framework Carbon -framework QuartzCore -L/Developer/usr/lib -F/System/Library/Frameworks/ApplicationServices.framework -F/System/Library/PrivateFrameworks

LDFLAGS += -L/Developer/usr/lib
LDFLAGS += -F/System/Library/Frameworks
LDFLAGS += -F/System/Library/PrivateFrameworks
LDFLAGS += -framework CoreFoundation 
LDFLAGS += -framework QuartzCore

all: main

main: ../common/shader_utils.o cube.o main.o
	$(COMPILER) $(LDLIBS) $(LDFLAGS) main.o cube.o ../common/shader_utils.o -o main
main.o: main.cpp
	$(COMPILER) $(LDLIBS) $(LDFLAGS) -c main.cpp
cube.o: cube.cpp
	$(COMPILER) $(LDLIBS) $(LDFLAGS) -c cube.cpp

clean:
	rm -f *.o main
