//
// Created by martind on 3/24/24.
//

#include "SaveManager.h"
#include "../Config/Config.h"
#include "../Tools/filesystem.h"

void SaveManager::WriteSave(int difficulty, int screenW, int screenH, int score, const Player &player, const std::vector<Trash> &trash,
                       const std::vector<Enemy> &enemies, const std::vector<Friendly> &friendlies) {
	file.clearFile();
	
	Record saveData{
			Type::SAVE_DATA,
			screenW,
			screenH,
			(unsigned)difficulty
	};
	file.write<Record>(saveData);
	
	Record scoreRecord{
			Type::SCORE,
			score, // Can't be in 'other' because that's unsigned and score can be negative
			0,
			0
	};
	file.write<Record>(scoreRecord);
	
	Record playerRecord{}; // The {} are there because all hail our lord and savior Clang-Tidy
	playerRecord.type = Type::PLAYER;
	playerRecord.x = player.getRect().x;
	playerRecord.y = player.getRect().y;
	// Screw it bit masking the boatPos it is
	SDL_Rect boatPos = player.getBoatPosition();
	playerRecord.other = (boatPos.x << 16) | boatPos.y;
	file.write<Record>(playerRecord);
	
	for (const Enemy &e : enemies) {
		Record enemyRecord{};
		enemyRecord.type = Type::ENEMY;
		enemyRecord.x = e.getRect().x;
		enemyRecord.y = e.getRect().y;
		enemyRecord.other = (unsigned)((Enemy)e).getAxis(); // Some jank because blah blah blah const qualifiers blah blah
		file.write<Record>(enemyRecord);
	}
	
	for (const Trash &t : trash) {
		Record trashRecord{};
		trashRecord.type = Type::TRASH;
		trashRecord.x = t.getRect().x;
		trashRecord.y = t.getRect().y;
		trashRecord.other = (unsigned)t.getType();
		file.write<Record>(trashRecord);
	}
	
	for (const Friendly &f : friendlies) {
		Record friendlyRecord{};
		friendlyRecord.type = Type::FRIENDLY;
		friendlyRecord.x = f.getRect().x;
		friendlyRecord.y = f.getRect().y;
		friendlyRecord.other = (unsigned)f.getAxis();
		file.write<Record>(friendlyRecord);
	}
}

std::tuple<std::tuple<int, int, int>, int, Player, std::vector<Trash>, std::vector<Enemy>, std::vector<Friendly>> SaveManager::ReadSave() { // saveData, score, player, trash, enemies, friendlies
	std::tuple<int, int, int> saveData;
	int score = 0;
	Player player;
	std::vector<Trash> trash;
	std::vector<Enemy> enemies;
	std::vector<Friendly> friendlies;
	
	while (!file.eof()) {
		auto record = file.read<Record>(); // Clang-Tidy: Use auto when initializing with a template cast to avoid duplicating the type name
		
		switch (record.type) {
			case Type::SCORE: {
				score = record.x;
				break;
			}
			case Type::SAVE_DATA: {
				saveData = {record.x, record.y, (int)record.other};
				break;
			}
			case Type::PLAYER: {
				player = Player();
				player.setRect({record.x, record.y, record.x < WindowData::SCREEN_WIDTH / 3 ? 80 : 160, 115});
				SDL_Rect boatPos{};
				player.setPath(record.x < WindowData::SCREEN_WIDTH / 3 ? "Assets/Player.png" : "Assets/Boat.png");
				boatPos.x = (int)(record.other >> 16);
				boatPos.y = (int)(record.other & 0xFFFF);
				player.setBoatPosition(boatPos);
				break;
			}
			case Type::TRASH: {
				Trash buff(record.x, record.y);
				buff.setType((Trash::Type)record.other);
				buff.setColor({255, 255, 255, 255});
				trash.push_back(buff);
				break;
			}
			case Type::ENEMY: {
				Enemy buff;
				buff.setRect({record.x, record.y, 60, 65});
				buff.setAxis((Enemy::Axis)record.other);
				enemies.emplace_back(buff);
				break;
			}
			case Type::FRIENDLY: {
				Friendly buff(record.x, record.y);
				buff.setAxis((Friendly::Axis)record.other);
				buff.setPath(record.x < WindowData::SCREEN_WIDTH / 3 ? absolutePath("Assets/Tractor.png") : absolutePath("Assets/Boat.png"));
				friendlies.push_back(buff);
				break;
			}
		}
	}
	
	return {saveData, score, player, trash, enemies, friendlies};
}

void SaveManager::open(const std::string& path) {
	file.open(path);
}

void SaveManager::deleteSave() {
	std::remove(file.getPath().c_str()); // Puts the file into the forever box, apparently
}

bool SaveManager::SaveExists() {
	BinaryFile temp;
	temp.open(file.getPath());
	return temp.is_open().first && temp.is_open().second;
}

std::string SaveManager::getPath() {
	return file.getPath();
}


bool SaveManager::saveEmpty() {
	return file.isFileEmpty(file.getPath());
}
