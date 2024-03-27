#include "BinaryFile.h"

void BinaryFile::open(const std::string& file, std::ios_base::openmode read_mode, std::ios_base::openmode write_mode) {
	path = file;
	
	fin.close();
	fout.close();
	
	fin.open(path, read_mode | std::ios_base::binary);
	fout.open(path, write_mode | std::ios_base::binary);
}

BinaryFile::BinaryFile(const BinaryFile &other) {
	path = other.path;
	fin.close();
	fout.close();
	
	fin.open(path, std::ios_base::in | std::ios_base::binary);
	fout.open(path, std::ios_base::app | std::ios_base::binary);
}

BinaryFile::BinaryFile(const std::string &path) : path(path) {
	fin.close();
	fout.close();
	
	fin.open(path, std::ios_base::in | std::ios_base::binary);
	fout.open(path, std::ios_base::app | std::ios_base::binary);
}

BinaryFile::~BinaryFile() {
	fin.close();
	fout.close();
}

void BinaryFile::clearFile() {
	std::ofstream temp;
	temp.open(path, std::ios_base::binary);
	temp.close();
}

const std::string & BinaryFile::getPath() const {
	return path;
}


/**
 * Checks if the input and output are open
 * @return Two booleans - first is input, second is output
 */
std::pair<bool, bool> BinaryFile::is_open() {
	return {fin.is_open(), fout.is_open()};
}

bool BinaryFile::eof() const {
	return fin.eof();
}


bool BinaryFile::isFileEmpty(const std::string& filename) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	return file.tellg() == 0;
}
