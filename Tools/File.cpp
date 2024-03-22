#include <iostream>
#include "File.h"

File::File(const std::string &path) {
	this->path = path;
	fout.open(path, std::ios_base::app);
	fin.open(path);
	
	if (!fin.is_open()) {
		std::cerr << "File " << path << " could not be opened for reading." << std::endl;
	}
	if (!fout.is_open()) {
		std::cerr << "File " << path << " could not be opened for writing." << std::endl;
	}
}


[[maybe_unused]] File::File(const File &other) {
	this->path = other.path;
	fin.close();
	fout.close();
	
	fin.open(path);
	fout.open(path, std::ios_base::app); // It just always does append I can't be bothered to find out how to get the mode from the other file
}

File::~File() {
	fin.close();
	fout.close();
}

/**
 * Opens the file with the given modes
 * @param write_mode the mode to open the file for writing
 * @param read_mode the mode to open the file for reading
 */
[[maybe_unused]] void File::open(std::ios_base::openmode write_mode, std::ios_base::openmode read_mode) {
	fin.close();
	fout.close();

	fin.open(path, read_mode);
	fout.open(path, write_mode);
}

/**
 * Reads a line from the file
 * @param includeNewline whether to include the newline character at the end of the line
 * @return the line read
 */
std::string File::readLine(bool includeNewline) {
	if (!fin.is_open()) {
		std::cerr << "File " << path << " is not open for reading." << std::endl;
		return "";
	}
	if (fin.eof()) {
		std::cerr << "File " << path << " is at the end." << std::endl;
		return "";
	}
	std::string line;
	std::getline(fin, line);
	if (includeNewline) {
		line += '\n';
	}
	return line;
}

/**
 * Writes a string to the file
 * @param data the string to be written
 * @param newline whether to add a newline character at the end of the string
 */
void File::write(const std::string &data, bool newline) {
	if (!fout.is_open()) {
		return;
	}
	fout << data << (newline ? "\n" : "");
}

/**
 * Writes a string to the file
 * @param data the string to be written
 * @param newline whether to add a newline character at the end of the string
 */
[[maybe_unused]] lines File::readLines() {
	lines lines;
	std::string line;
	while (!(line = readLine()).empty()) {
		lines.push_back(line);
	}
	return lines;
}

/**
 * Writes a string to the file
 * @param data the string to be written
 * @param newline whether to add a newline character at the end of the string
 */
void File::clearFile() {
	std::ofstream file;
	file.open(path); // Opens the file in insert mode which by some leap of logic clears the file don't ask me about the leap in question
	file.close();
}
