CXX=g++
LFLAGS=-L Local/glad/include -L Local/stb
INCLUDES=-I Dist/Headers -I Local/glad/include -I Local/stb
CFLAGS=-c -Wall -D PROJECT_SOURCE_DIR=\"$(shell pwd)\"
LIBS=-lassimp -lglfw -lGL -lm -lglad -lstb_image
#DEPS=glitter.hpp mesh.hpp shader.hpp
SRCDIR=Dist/Sources/
SOURCES=$(addprefix $(SRCDIR), main.cpp mesh.cpp shader.cpp)
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
