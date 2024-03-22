#ifndef MORSKI_SMETAR_FILE_H
#define MORSKI_SMETAR_FILE_H

#include <fstream>
#include <string>
#include <vector>

typedef std::vector<std::string> lines;

class File {
protected:
	std::string path;
	std::ifstream fin;
	std::ofstream fout;
public:
	File() = default;
	explicit File(const std::string& path); // Don't know why it's 'explicit', but whatever. All hail Clang-Tidy and its wisdom I guess.
	[[maybe_unused]] File(const File& other);
	
	~File();
	
	[[maybe_unused]] void open(std::ios_base::openmode write_mode = std::ios_base::app, std::ios_base::openmode read_mode = std::ios_base::in);
	
	 std::string readLine(bool includeNewline = false);
	void write(const std::string& data, bool newline = true);
	void clearFile();
	
	[[maybe_unused]] virtual lines readLines();
};


#endif //MORSKI_SMETAR_FILE_H
