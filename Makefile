CXX=g++
LFLAGS=-L Local/glad/include -L Local/stb
INCLUDES=-I Dist/Headers -I Local/glad/include -I Local/stb
CFLAGS=-c -Wall
LIBS=-lassimp -lglfw -lGL -lm -lglad -lstb_image
#DEPS=glitter.hpp mesh.hpp shader.hpp
SOURCES=Dist/Sources/main.cpp Dist/Sources/mesh.cpp Dist/Sources/shader.cpp
OBJECTS=$(SOURCES:.cpp=.o)
MAIN=a

all:
	$(MAIN)

$(MAIN): $(OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

depend: $(SOURCES)
	makedepend $(INCLUDES) $^
