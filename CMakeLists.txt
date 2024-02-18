cmake_minimum_required(VERSION 3.28)
project(Morski_Smetar)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(Morski_Smetar main.cpp
        Config/Config.cpp
        Config/Config.hpp
        UI/Window.cpp
        UI/Window.cpp
        Entities/GameObject.cpp
        Entities/GameObject.hpp
        Entities/Player.cpp
        Entities/Player.hpp
        Tools/Path.cpp
        Tools/Path.hpp
        Entities/Wave.cpp
        Entities/Wave.hpp
        Tools/Random.cpp
        Entities/Trash.cpp
        Entities/Trash.hpp
        Tools/Collision.cpp
        Tools/Collision.hpp
        Config/Data.cpp
        Config/Data.hpp
        Entities/Text.cpp
        Entities/Text.hpp
        Tools/Overloads.cpp
        Tools/Overloads.hpp
)

target_link_libraries(Morski_Smetar SDL2::SDL2)
target_link_libraries(Morski_Smetar SDL2_image)
target_link_libraries(Morski_Smetar SDL2_ttf)
