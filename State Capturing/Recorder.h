//
// Created by martind on 3/19/24.
//

#ifndef MORSKI_SMETAR_RECORDER_H
#define MORSKI_SMETAR_RECORDER_H


#include "../Tools/BinaryFile.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"
#include "../Entities/Friendly.h"
#include "../Entities/Trash.h"

class Recorder {
public:
	enum Type{ // I know these values are already implied, but I'd rather be careful since they do somewhat matter
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
	
private:
	BinaryFile file;
	static unsigned Mask(Type t, int x, int y);
public:
	Recorder() = default;
	~Recorder() = default;
	
	[[maybe_unused]] explicit Recorder(const std::string& path);
	Recorder(const Recorder& other) = delete; // Explicitly deleted because editing the same file from two different recorders would be a disaster
	
	void open(const std::string& path);
	
	void clearFile();
	
	[[maybe_unused]] [[nodiscard]] const std::string& getPath() const;
	
	void Write(Type t, int x, int y);
	
	
	void Update(const Player& player, const std::vector<Enemy>& enemies, const std::vector<Friendly>& friendlies, const std::vector<Trash>& trash);
	
	[[maybe_unused]] void completeStage(int screenX, int screenY);
};


#endif //MORSKI_SMETAR_RECORDER_H
