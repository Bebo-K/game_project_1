#Windows build of Game
#VPATH is separated with semicolon instead of colon
#rd is used instead of rm

RESOURCE_ARCH = pe-x86-64
#RESOURCE_ARCH = pe-i386

CFLAGS =  -g -Wall -std=c++11 -Wfatal-errors -Wno-write-strings
#FINAL BUILD: add -static-libgcc -static-libstdc++
LIBS =  -lgdi32 -lglew32 -lwinmm -lopengl32  -lz -lfreetype.dll

SRC_PATHS := src;src/io;src/gfx;src/game;src/game/component;src/game/system;src/client;src/server;src/gui;src/gui/menu;src/gui/widget;src/test;src/phys;src/struct;
VPATH = $(SRC_PATHS)


MAIN_SRC := $(wildcard src/*.cpp)
CLIENT_SRC := $(wildcard src/client/*.cpp)
SERVER_SRC := $(wildcard src/client/*.cpp)
IO_SRC := $(wildcard src/io/*.cpp)
GFX_SRC := $(wildcard src/gfx/*.cpp)
GAME_SRC := $(wildcard src/game/*.cpp)
PHYS_SRC := $(wildcard src/phys/*.cpp)
STRUCT_SRC :=  $(wildcard src/struct/*.cpp)
UI_SRC := $(wildcard src/gui/*.cpp)
MENU_SRC := $(wildcard src/gui/menu/*.cpp)
WIDGET_SRC := $(wildcard src/gui/widget/*.cpp)
COMPONENT_SRC := $(wildcard src/game/component/*.cpp)
SYSTEM_SRC := $(wildcard src/game/system/*.cpp)
TEST_SRC := $(wildcard src/test/*.cpp)

SRC := $(MAIN_SRC) $(CLIENT_SRC) $(SERVER_SRC) $(IO_SRC) $(GFX_SRC) $(GAME_SRC) $(COMPONENT_SRC) $(SYSTEM_SRC) $(UI_SRC) $(MENU_SRC) $(WIDGET_SRC) $(TEST_SRC) $(PHYS_SRC) $(STRUCT_SRC)

OBJS := $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))

#foobar.exe: $(IO_SRC) foobar.cpp 
#	g++ $(CFLAGS) -o $@ $^ $(LIBS)

game.exe: $(OBJS) win/resource.o
	g++ $(CFLAGS) -o $@ $^ $(LIBS)

obj/%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<

win/resource.o: win/dpi-aware.manifest win/icon.ico win/resource.rc
	windres win/resource.rc --target=$(RESOURCE_ARCH) $@

.PHONY: clean
clean:
	rd /s "./bin/game.exe"
	rd /s "./obj/*"
	rd /s "./win/resource.o"