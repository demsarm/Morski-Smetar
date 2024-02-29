// Description: Path utility functions

#include "Path.h"

#include "filesystem"

/**
 * @brief Returns the absolute path to the root directory of the project
 * @param relativePath The relative path to the file
 * @return The absolute path to the root directory of the project
 */
std::string absolutePath(const std::string& relativePath){
	const std::string &output =
			std::filesystem::current_path().parent_path().string() + "/" + relativePath;
	return output;
}
