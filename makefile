#Windows build of Game
#VPATH is separated with semicolon instead of colon

CFLAGS =  -g -Wall -std=c++14
LIBS = -lopengl32 -lgdi32

SRC_PATHS :=src;src/io;src/gfx;src/game;src/game/components;src/game/systems;
VPATH = $(SRC_PATHS)


MAIN_SRC := $(wildcard src/*.cpp)
IO_SRC := $(wildcard src/io/*.cpp)
GFX_SRC := $(wildcard src/gfx/*.cpp)
GAME_SRC := $(wildcard src/game/*.cpp)
COMPONENTS_SRC := $(wildcard src/game/components/*.cpp)
SYSTEMS_SRC := $(wildcard src/game/systems/*.cpp)

SRC := $(MAIN_SRC) $(IO_SRC) $(GFX_SRC) $(GAME_SRC) $(COMPONENTS_SRC) $(SYSTEMS_SRC)

OBJS := $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))


foobar.exe: $(IO_SRC) foobar.cpp
	g++ $(CFLAGS) -o $@ $^ $(LIBS)

game.exe: $(OBJS)
	g++ $(CFLAGS) -o $@ $^ $(LIBS)


obj/%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rd /s "obj/*  "
	rd /s "bin/game.exe"