//
// Created by martind on 17.1.2024.
//

#ifndef SDL_GAME_PATH_HPP
#define SDL_GAME_PATH_HPP

#include <string>

/**
 * @brief Returns the absolute path to the root directory of the project
 * @param relativePath The relative path to the file
 * @return The absolute path to the file
 */
std::string absolutePath(const std::string& relativePath);

#endif //SDL_GAME_PATH_HPP
