cmake_minimum_required(VERSION 3.28)
project(Morski_Smetar)

link_directories(/usr/lib)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(Morski_Smetar main.cpp
        Config/Config.cpp
        Config/Config.hpp
        UI/Window.cpp
        UI/Window.hpp
        Entities/GameObject.cpp
        Entities/GameObject.hpp
        Entities/Player.cpp
        Entities/Player.hpp
        Tools/Path.cpp
        Tools/Path.hpp
        Entities/Wave.cpp
        Entities/Wave.hpp
        Tools/Random.cpp
        Tools/Random.hpp
        Entities/Trash.cpp
        Entities/Trash.hpp
        Tools/Collision.cpp
        Tools/Collision.hpp
        Config/Data.cpp
        Config/Data.hpp
        UI/Text.cpp
        UI/Text.hpp
        Tools/Overloads.cpp
        Tools/Overloads.hpp
        Entities/Enemy.cpp
        Entities/Enemy.hpp
        Tools/Math.cpp
        Tools/Math.hpp
        Entities/Friendly.cpp
        Entities/Friendly.hpp
        Game.cpp
        Game.hpp
        UI/Line.cpp
        UI/Line.hpp
        UI/Screen.cpp
        UI/Screen.hpp
)

target_link_libraries(Morski_Smetar SDL2)
target_link_libraries(Morski_Smetar SDL2_image)
target_link_libraries(Morski_Smetar SDL2_ttf)
