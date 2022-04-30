#Windows build of Game
#VPATH is separated with semicolon instead of colon
#rd is used instead of rm

RESOURCE_ARCH = pe-x86-64
#RESOURCE_ARCH = pe-i386

CFLAGS =  -g -Wall -std=c++11 -Wfatal-errors -Wno-write-strings
#FINAL BUILD: add -static-libgcc -static-libstdc++
LIBS =  -lgdi32 -lglew32 -lWs2_32 -lwinmm -lopengl32  -lz -lfreetype.dll -lole32 -luuid


rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
rdirfind=$(foreach d,$(wildcard $(1:=/*)),$(call rdirfind,$d) $(filter-out %.%,$d))

#SRC_PATHS := game_project_1;game_project_1/io;game_project_1/net;game_project_1/gfx;game_project_1/game;game_project_1/game/component;game_project_1/game/system;game_project_1/client;game_project_1/server;game_project_1/gui;game_project_1/gui/menu;game_project_1/gui/widget;game_project_1/test;game_project_1/phys;game_project_1/struct;
#MAIN_SRC := $(wildcard game_project_1/*.cpp)
#STRUCT_SRC :=  $(wildcard game_project_1/types/*.cpp)
#CLIENT_SRC := $(wildcard game_project_1/client/*.cpp)
#SERVER_SRC := $(wildcard game_project_1/server/*.cpp)
#IO_SRC := $(wildcard game_project_1/io/*.cpp)
#NET_SRC := $(wildcard game_project_1/net/*.cpp)
#GFX_SRC := $(wildcard game_project_1/gfx/*.cpp)
#GAME_SRC := $(wildcard game_project_1/game/*.cpp)
#PHYS_SRC := $(wildcard game_project_1/phys/*.cpp)
#UI_SRC := $(wildcard game_project_1/gui/*.cpp)
#MENU_SRC := $(wildcard game_project_1/gui/menu/*.cpp)
#WIDGET_SRC := $(wildcard game_project_1/gui/widget/*.cpp)
#COMPONENT_SRC := $(wildcard game_project_1/component/*.cpp)
#SYSTEM_SRC := $(wildcard game_project_1/system/*.cpp)
#ENTITY_SRC := $(wildcard game_project_1/entity/*.cpp)
#TEST_SRC := $(wildcard game_project_1/test/*.cpp)
#SRC := $(MAIN_SRC) $(STRUCT_SRC) $(CLIENT_SRC) $(SERVER_SRC) $(IO_SRC) $(NET_SRC) $(GFX_SRC) $(GAME_SRC) $(PHYS_SRC) $(UI_SRC) $(MENU_SRC) $(WIDGET_SRC) $(COMPONENT_SRC) $(SYSTEM_SRC) $(ENTITY_SRC) $(TEST_SRC) 
#VPATH = $(SRC)

VPATH := game_project_1;$(call rdirfind,game_project_1)
SRC := $(call rwildcard,game_project_1,*.cpp)
OBJS := $(addprefix obj/,$(notdir $(patsubst %.cpp,%.o,$(SRC))))

#notdir( $(SRC:.cpp=.o)  ))

#foobar.exe: $(IO_SRC) foobar.cpp 
#	g++ $(CFLAGS) -o $@ $^ $(LIBS)

obj/%.o: %.cpp
	g++ $(CFLAGS) -c -I . -o $@ $< 

game.exe: $(OBJS) win/resource.o
	g++ $(CFLAGS) -o $@ $^  $(LIBS)

win/resource.o: win/dpi-aware.manifest win/icon.ico win/resource.rc
	windres win/resource.rc --target=$(RESOURCE_ARCH) $@

.PHONY: clean
clean:
	rd /s "./bin/game.exe"
	rd /s "./obj/*"
	rd /s "./win/resource.o"