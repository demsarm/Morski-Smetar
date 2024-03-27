#ifndef MORSKI_SMETAR_SAVEMANAGER_H
#define MORSKI_SMETAR_SAVEMANAGER_H


#include <vector>
#include "../Tools/BinaryFile.h"
#include "../Entities/Player.h"
#include "../Entities/Trash.h"
#include "../Entities/Enemy.h"
#include "../Entities/Friendly.h"

class SaveManager {
public:
	enum class Type {
		SAVE_DATA,
		SCORE,
		PLAYER,
		ENEMY,
		TRASH,
		FRIENDLY
	};
	
	struct Record {
		Type type;
		int x, y;
		unsigned other; // These would be type dependant (i.e. boatPos for player, axis for enemies and friendlies, type for trash)
	};
private:
	BinaryFile file;
public:
	void open(const std::string& path);
	std::string getPath();
	void deleteSave();
	
	void WriteSave(int difficulty, int screenW, int screenH, int score, const Player &player, const std::vector<Trash> &trash,
	               const std::vector<Enemy> &enemies, const std::vector<Friendly> &friendlies);
	std::tuple<std::tuple<int, int, int>, int, Player, std::vector<Trash>, std::vector<Enemy>, std::vector<Friendly>> ReadSave();
	bool SaveExists();
	bool saveEmpty();
};


#endif //MORSKI_SMETAR_SAVEMANAGER_H
