#Windows build of Game
#VPATH is separated with semicolon instead of colon
#rd is used instead of rm

RESOURCE_ARCH = pe-x86-64
#RESOURCE_ARCH = pe-i386

CFLAGS =  -g -std=c++11 -Wwrite-strings
#-Wall -Wfatal-errors
#FINAL BUILD: add -static-libgcc -static-libstdc++
LIBS =  -lgdi32 -lglew32 -lWs2_32 -lwinmm -lopengl32 -lz -lfreetype -lole32 -luuid 


rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
rdirfind=$(foreach d,$(wildcard $(1:=/*)),$(call rdirfind,$d) $(filter-out %.%,$d))

VPATH := game_project_1;$(call rdirfind,game_project_1)
SRC := $(call rwildcard,game_project_1,*.cpp)
OBJS := $(addprefix obj/,$(notdir $(patsubst %.cpp,%.o,$(SRC))))

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