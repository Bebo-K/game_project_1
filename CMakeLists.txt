cmake_minimum_required(VERSION 4.2)
project(Project)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES
        src/game.cpp
)

if(WIN32)
    set(SOURCE_FILES ${SOURCE_FILES}
            src/winmain.cpp
            win/dpi-aware.manifest
            win/icon.ico
            win/resource.rc
            win/appxmanifest.xml)
    add_executable(game WIN32 ${SOURCE_FILES})
    target_link_libraries(game PRIVATE libgdi32.a)
else()
    add_executable(game ${SOURCE_FILES})
endif()

target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)