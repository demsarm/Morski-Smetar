#include "Reader.h"

Reader::Reader(const std::string &path) {
	currentSize = {Type::COMPLETE_STAGE, 800, 600};
	file.open(path);
}

void Reader::open(const std::string &path) {
	file.open(path);
	currentSize = {Type::COMPLETE_STAGE ,800, 600};
}

Reader::Record Reader::Unmask(unsigned record)  {
	unsigned mask14 = (1 << 14) - 1;
	
	Record result{};
	result.type = (Type)(record >> 28);
	result.x = (record >> 14) & mask14; // "middle" 14 bits
	result.y = record & mask14;
	return result;
};

int Reader::Scan() {
	if (file.eof()) return 1;
	records.clear();
	
	auto temp = file.read<unsigned>(); // Clang-Tidy: Use auto when initializing with a template cast to avoid duplicating the type name
	while (!file.eof() && Unmask(temp).type != Type::RESET) {
		records.push_back(temp);
		temp = file.read<unsigned>();
	}
//	std::cout << "Scanned " << records.size() << " records" << std::endl;
	for (auto& record : records) {
		Record r = Unmask(record); // The buffer might not be necessary, but I have seen C++ do sierd stuff without 'em
		objects.push_back(r);
	}

//	std::cout << "Read " << enemies.size() << " enemies" << std::endl;
//	std::cout << "Read " << friendlies_land.size() << " friendlies_land" << std::endl;
//	std::cout << "Read " << friendlies_water.size() << " friendlies_water" << std::endl;
//	std::cout << "Read " << trash_bottles.size() << " trash_bottles" << std::endl;
//	std::cout << "Read " << trash_boxes.size() << " trash_boxes" << std::endl;
//	std::cout << "Read " << trash_cans.size() << " trash_cans" << std::endl;
	
	
	return 0;
}

Reader::Record Reader::Next() {
	if (objects.empty()) return {Type::RESET, 0, 0};

	Record r = objects.front();

#ifdef DEBUG
	if (r.type == Type::COMPLETE_STAGE) {
//		std::cout << "Reading a complete stage record" << std::endl;
//		std::cout << "Type: " << (int)r.type << " X: " << r.x << " Y: " << r.y << std::endl;
	}
#endif
	
	objects.erase(objects.begin());
//	std::cout << "Type: " << (int)r.type << " X: " << r.x << " Y: " << r.y << std::endl;

	return r;
}

bool Reader::eof() const {
	return file.eof();
}

const std::vector<Reader::Record> &Reader::getObjects() const {
	return objects;
}
