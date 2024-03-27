cmake_minimum_required(VERSION 3.28)
project(Morski_Smetar)

link_directories(/usr/lib)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(Morski_Smetar main.cpp
        Config/Config.cpp
        Config/Config.h
        UI/Window.cpp
        UI/Window.h
        Entities/GameObject.cpp
        Entities/GameObject.h
        Entities/Player.cpp
        Entities/Player.h
        Tools/filesystem.cpp
        Tools/filesystem.h
        Entities/Wave.cpp
        Entities/Wave.h
        Tools/Random.cpp
        Tools/Random.h
        Entities/Trash.cpp
        Entities/Trash.h
        Tools/Collision.cpp
        Tools/Collision.h
        Config/Data.cpp
        Config/Data.h
        UI/Text.cpp
        UI/Text.h
        Tools/Overloads.cpp
        Tools/Overloads.h
        Entities/Enemy.cpp
        Entities/Enemy.h
        Tools/Math.cpp
        Tools/Math.h
        Entities/Friendly.cpp
        Entities/Friendly.h
        UI/Line.cpp
        UI/Line.h
        UI/Screen.cpp
        UI/Screen.h
        Tools/File.cpp
        Tools/File.h
        UI/Input.cpp
        UI/Input.h
        UI/Button.cpp
        UI/Button.h
        Tools/MD5.cpp
        Tools/MD5.h
        Tools/BinaryFile.cpp
        Tools/BinaryFile.h
        "State Capturing/Recorder.cpp"
        "State Capturing/Recorder.h"
        Game.cpp
        Game.h
        "State Capturing/Reader.cpp"
        "State Capturing/Reader.h"
        "State Capturing/SaveManager.cpp"
        "State Capturing/SaveManager.h"
)

target_link_libraries(Morski_Smetar SDL2)
target_link_libraries(Morski_Smetar SDL2_image)
target_link_libraries(Morski_Smetar SDL2_ttf)
