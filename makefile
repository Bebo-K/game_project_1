#Windows build of Game
#VPATH is separated with semicolon instead of colon
#rd is used instead of rm

CFLAGS =  -g -Wall -std=c++11 -Wfatal-errors -Wno-write-strings
#FINAL BUILD: add -static-libgcc -static-libstdc++
LIBS = -mwindows -lglew32 -lopengl32  -lz

SRC_PATHS := src;src/io;src/gfx;src/game;src/game/components;src/game/systems;src/client;src/server;src/gui;src/test;src/phys;src/structs;
VPATH = $(SRC_PATHS)


MAIN_SRC := $(wildcard src/*.cpp)
CLIENT_SRC := $(wildcard src/client/*.cpp)
SERVER_SRC := $(wildcard src/client/*.cpp)
IO_SRC := $(wildcard src/io/*.cpp)
GFX_SRC := $(wildcard src/gfx/*.cpp)
GAME_SRC := $(wildcard src/game/*.cpp)
PHYS_SRC := $(wildcard src/phys/*.cpp)
STRUCTS_SRC :=  $(wildcard src/structs/*.cpp)
UI_SRC := $(wildcard src/gui/*.cpp)
COMPONENTS_SRC := $(wildcard src/game/components/*.cpp)
SYSTEMS_SRC := $(wildcard src/game/systems/*.cpp)
TEST_SRC := $(wildcard src/test/*.cpp)

SRC := $(MAIN_SRC) $(CLIENT_SRC) $(SERVER_SRC) $(IO_SRC) $(GFX_SRC) $(GAME_SRC) $(COMPONENTS_SRC) $(SYSTEMS_SRC) $(UI_SRC) $(TEST_SRC) $(PHYS_SRC) $(STRUCTS_SRC)

OBJS := $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))


#foobar.exe: $(IO_SRC) foobar.cpp 
#	g++ $(CFLAGS) -o $@ $^ $(LIBS)

game.exe: $(OBJS)
	g++ $(CFLAGS) -o $@ $^ $(LIBS)


obj/%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rd /s "./bin/game.exe"
	rd /s "./obj/*"