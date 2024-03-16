#ifndef MORSKI_SMETAR_FILE_H
#define MORSKI_SMETAR_FILE_H

#include <fstream>
#include <string>
#include <vector>

typedef std::vector<std::string> lines;

class File {
	std::string path;
	std::ifstream fin;
	std::ofstream fout;
public:
	File() = default;
	explicit File(const std::string& path); // Don't know why it's 'explicit', but whatever. All hail Clang-Tidy and its wisdom I guess.
	File(const File& other);
	
	~File();
	
	void open(std::ios_base::openmode mode = std::ios_base::in, std::ios_base::openmode read_mode = std::ios_base::app);
	std::string readLine(bool includeNewline = false);
	void write(const std::string& data, bool newline = true);
	void clearFile();
	
	lines readLines();
};


#endif //MORSKI_SMETAR_FILE_H
