CXX=g++
LFLAGS=-L Local/glad/include -L Local/stb
INCLUDES=-I Dist/Headers -I Local/glad/include -I Local/stb -I /usr/include/bullet
CFLAGS=-Wall -D PROJECT_SOURCE_DIR=\"$(shell pwd)\"
LIBS=-lassimp -lglfw -lGL -lm -ldl -lBulletDynamics -lLinearMath 
SRCDIR=Dist/Sources/
GLADSRC=Local/glad/src/
SOURCES=$(addprefix $(SRCDIR), main.cpp mesh.cpp shader.cpp) $(addprefix $(GLADSRC), glad.c)
OBJECTS=$(SOURCES:.cpp=.o) 
MAIN=prog

all: 
	$(MAIN)

$(MAIN): $(OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(addprefix $(SRCDIR), *.o) *~ $(MAIN)

depend: $(SOURCES)
	makedepend $(INCLUDES) $^
