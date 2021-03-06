CXX=g++
LFLAGS=-L Local/glad/include -L Local/stb
INCLUDES=-I Dist/Headers -I Local/glad/include -I Local/stb -I /usr/include/bullet
CFLAGS=-std=c++11 -Wall -D PROJECT_SOURCE_DIR=\"$(shell pwd)\"
LIBS=-lassimp -lglfw -lGL -lm -ldl -lBulletDynamics -lLinearMath 
SRCDIR=Dist/Sources/
GLADSRC=Local/glad/src/
SOURCES=$(addprefix $(SRCDIR), main.cpp mesh.cpp shader.cpp scene.cpp camera.cpp util.cpp) $(addprefix $(GLADSRC), glad.c)
OBJECTS=$(SOURCES:.cpp=.o) 
MAIN=Build/prog

all: $(OBJECTS) 
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(addprefix $(SRCDIR), *.o) $(addprefix $(GLADSRC), *.o) *~ $(MAIN)

depend: $(SOURCES)
	makedepend $(INCLUDES) $^
