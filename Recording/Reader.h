#ifndef MORSKI_SMETAR_READER_H
#define MORSKI_SMETAR_READER_H

/*
 * This class could be used to read the binary file that the Recorder class writes to.
 * Though notably, I decided to not use it in the end; it might not really be used in the project at all - at least until I figure out a good way to send all the data
 */


#include <vector>
#include "../Tools/BinaryFile.h"

class Reader {
public:
	enum class Type{
		RESET = 0,
		PLAYER = 1,
		ENEMY = 2,
		FRIENDLY_LAND = 3,
		FRIENDLY_WATER = 4,
		TRASH_BOTTLE = 5,
		TRASH_BOX = 6,
		TRASH_CAN = 7,
		COMPLETE_STAGE = 8,
		PLAYER_BOAT = 9
	};
	
	struct Record {
		Type type;
		unsigned x;
		unsigned y;
	};
	
	static Record Unmask(unsigned record); // Use unsigned because the first bit is treated as the sign bit even when SHL-ed
private:
	BinaryFile file;
	
	std::vector<unsigned> records;
	
	std::vector<Record> objects; // Let the user separate the types
	
	Record currentSize{};
	
public:
	Reader() = default;
	~Reader() = default;
	explicit Reader(const std::string& path);
	
	Reader(const Reader& other) = delete;

	void open(const std::string& path);
	int Scan();
	Record Next();
	bool eof() const;
	[[nodiscard]] const std::vector<Record>& getObjects() const;
	// If they want to change the screen size, they can find the COMPLETE_STAGE record and change it idc
	//	[[nodiscard]] const Record& getCurrentSize() const;

};


#endif //MORSKI_SMETAR_READER_H
