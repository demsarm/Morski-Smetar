# Variables
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I/usr/local/include/ -static-libgcc -static-libstdc++
LDFLAGS =\
  -L /usr/local/lib\
  -L /usr/local/x86_64-w64-mingw32/lib/\
  -L /usr/x86_64-w64-mingw32/lib/\
  /usr/x86_64-w64-mingw32/lib/libSDL2main.a\
  /usr/x86_64-w64-mingw32/lib/libSDL2-static.a\
  /usr/local/x86_64-w64-mingw32/lib/libSDL2_ttf.a\
  /usr/local/x86_64-w64-mingw32/lib/libSDL2_image.a\
  -static-libgcc -static-libstdc++\
  -lSDL2main -lSDL2-static -lSDL2_ttf -lSDL2_image -lsetupapi -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -lm -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lrpcrt4
SRC = $(shell find . -name "*.cpp" ! -name "CMakeCXXCompilerId.cpp")
OBJ = $(SRC:.cpp=.o)
#OBJ = Config/Config.o Config/Data.o Entities/GameObject.o UI/Screen.o UI/Line.o Tools/Random.o Tools/Math.o Tools/Collision.o Tools/Path.o Tools/Overloads.o Entities/Wave.o Entities/Trash.o Entities/Player.o Entities/Enemy.o Entities/Friendly.o Entities/Text.o UI/Window.o Game.o main.o
EXEC = game.exe

# Rules
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

print:
	echo $(OBJ)

.PHONY: all clean