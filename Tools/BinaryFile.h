//
// Created by martind on 3/17/24.
//

#ifndef MORSKI_SMETAR_BINARYFILE_H
#define MORSKI_SMETAR_BINARYFILE_H

#include <fstream>
#include <iostream>

class BinaryFile {
protected:
	std::string path;
	std::ifstream fin;
	std::ofstream fout;

public:
	BinaryFile() = default;
	explicit BinaryFile(const std::string& path);
	BinaryFile(const BinaryFile& other);
	
	~BinaryFile();
	
	[[maybe_unused]]
	void open(const std::string& file, std::ios_base::openmode write_mode = std::ios_base::app, std::ios_base::openmode read_mode = std::ios_base::in);
	void clearFile();
	
	template<typename T>
	T read();
	
	template<typename T>
	void write(const T& data);
	
	const std::string & getPath() const;
	
	std::pair<bool, bool> is_open();
	bool eof() const;
};


// Because templates are funky
template<typename T>
void BinaryFile::write(const T &data) {
	T temp = data; // This seems to be required as it doesn't seem to write correctly without it - I need to have someone explain why to me
	fout.write((char *)&temp, sizeof(temp));
}

template<typename T>
T BinaryFile::read() {
	T data;
	fin.read((char *)(&data), sizeof(T));
	return data;
}

#endif //MORSKI_SMETAR_BINARYFILE_H
