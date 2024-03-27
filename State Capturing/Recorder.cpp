#include "Recorder.h"
#include "../Config/Config.h"
#include "Reader.h"
#include "../Tools/Overloads.h"

#ifdef DEBUG
#include <bitset>
#include <iomanip>
#endif

[[maybe_unused]]
Recorder::Recorder(const std::string &path) {
	file.open(path, std::ios_base::in);
}

void Recorder::open(const std::string &path) {
	file.open(path, std::ios_base::in);
}

void Recorder::clearFile() {
	file.clearFile();
}

[[maybe_unused]] const std::string &Recorder::getPath() const {
	return file.getPath();
}

unsigned Recorder::Mask(Type t, int x, int y) {
	/*
	 * I was originally going to have x and y be both 32-bit integers, but then I'd have to use an uint8 for the type which, considering the number of types, would be a waste of space
	 * I decided 4 bits for type would be enough, the game is not mechanically complex enough to need more
	 * In that case, the x and y would be 14 bits each, giving a limit of 16384 for both x and y
	 * Since 16K resolution is barely still in development from what my research has shown and that's 15360x8640 (less than the 14 bit limit of 16384), this seems good enough
	 * At least until 32k rolls around which doesn't seem to be happening any time soon
	 */

#ifdef DEBUG
	if ((x > WindowData::SCREEN_WIDTH || y > WindowData::SCREEN_HEIGHT) && t != Type::COMPLETE_STAGE) {
//		std::cerr << "Recorded a bad position" << std::endl;
//		std::cerr << "X: " << x << " Y: " << y << std::endl;
	}
	
	if (log(x)/log(2) > 14 || log(y)/log(2) > 14) {
//		std::cerr << "Recorded a position with too many bits" << std::endl;
//		std::cerr << "X: " << x << " Y: " << y << std::endl;
	}
	
	if (log(x)/log(2) < 0 || log(y)/log(2) < 0) {
//		std::cerr << "Recorded a negative position" << std::endl;
//		std::cerr << "T: " << t << " X: " << x << " Y: " << y << std::endl;
	}
#endif
	
	unsigned mask4 = (1 << 4) - 1;
	unsigned mask14 = (1 << 14) - 1;
	
	unsigned tmask = (unsigned)t & mask4;
	unsigned xmask = (unsigned)x & mask14;
	unsigned ymask = (unsigned)y & mask14;
	
	unsigned result = (tmask << 28) | (xmask << 14) | ymask;
	
	return result;
}

void Recorder::Update(const Player &player, const std::vector<Enemy> &enemies, const std::vector<Friendly> &friendlies, const std::vector<Trash> &trash) {
	if (!file.is_open().second) {
		std::cerr << "File is not open" << std::endl;
		return;
	}
	
	Write(Type::PLAYER, player.getRect().x, player.getRect().y);
	
	if (player.getBoatPosition() != (SDL_Rect){0, 0, 0, 0}){
		Write(Type::PLAYER_BOAT, player.getBoatPosition().x, player.getBoatPosition().y);
	}
	
	for (const auto& enemy : enemies) {
		Write(Type::ENEMY, enemy.getRect().x, enemy.getRect().y);
	}
	
	for (const auto& friendly : friendlies) {
		if (friendly.getType() == Friendly::Type::LAND) {
			Write(Type::FRIENDLY_LAND, friendly.getRect().x, friendly.getRect().y);
		} else {
			Write(Type::FRIENDLY_WATER, friendly.getRect().x, friendly.getRect().y);
		}
	}
	
	for (const auto& t : trash) {
		switch (t.getType()) {
			case Trash::Type::BOTTLE:
				Write(Type::TRASH_BOTTLE, t.getRect().x, t.getRect().y);
				break;
			case Trash::Type::BOX:
				Write(Type::TRASH_BOX, t.getRect().x, t.getRect().y);
				break;
			case Trash::Type::CAN:
				Write(Type::TRASH_CAN, t.getRect().x, t.getRect().y);
				break;
			default:
				break;
		}
	}
	
	Write(Type::RESET, 0, 0);
}

/**
 * Completes the stage by writing a record with the screen size
 * @param screenX The width of the screen
 * @param screenY The height of the screen
 * @warning Use the Write() method instead of this function - the only difference is that you have to pass Type::COMPLETE_STAGE as the first argument
 */
[[maybe_unused]]
void Recorder::completeStage(int screenX, int screenY) {
	if (!file.is_open().second) {
		std::cerr << "File is not open" << std::endl;
		return;
	}
	file.write<unsigned>(Mask(COMPLETE_STAGE, screenX, screenY)); // You have to store the window size somewhere when changing stages - this seems to be best
}

void Recorder::Write(Recorder::Type t, int x, int y) {
	
	unsigned r = Mask(t, x, y);

#ifdef DEBUG
	if (t == Type::COMPLETE_STAGE) {
//		std::cout << "Writing a complete stage record" << std::endl;
//		std::cout << "Type: " << std::bitset<4>(t) << " X: " << std::bitset<14>(x) << " Y: " << std::bitset<14>(y) << std::endl;
	}
	
	if (t == Type::RESET) {
//		std::cout << "Writing a reset record" << std::endl;
//		std::cout << "Type: " << std::bitset<4>(t) << " X: " << std::bitset<14>(x) << " Y: " << std::bitset<14>(y) << std::endl;
	}
	
	if (r == 0 && t != Type::RESET) {
//		std::cerr << "Wrote a record with all 0s (T: " << t << " X: " << x << " Y: " << y << ")" << std::endl;
	}
	
	if (r & 0x80000000 && t != Type::COMPLETE_STAGE) {
//		std::cerr << "Wrote a record with bad type" << std::endl;
//		std::cerr << "Type: " << std::bitset<4>(t) << " X: " << std::bitset<14>(x) << " Y: " << std::bitset<14>(y) << " > " << std::bitset<32>(r) << std::endl;
	}
	
	if (x > WindowData::SCREEN_WIDTH || y > WindowData::SCREEN_HEIGHT) {
//		std::cerr << "Got instruction with bad position" << std::endl;
//		std::cerr << "Type: " << std::bitset<4>(t) << " X: " << std::bitset<14>(x) << " Y: " << std::bitset<14>(y) << " > " << std::bitset<32>(r) << std::endl;
	}
	
	if (Reader::Unmask(r).x > WindowData::SCREEN_WIDTH || Reader::Unmask(r).y > WindowData::SCREEN_HEIGHT) {
//		std::cerr << "Wrote a record with bad position" << std::endl;
//		std::cerr << "Type: " << std::bitset<4>(t) << " X: " << std::bitset<14>(x) << " Y: " << std::bitset<14>(y) << " > " << std::bitset<32>(r) << std::endl;
	}
#endif
	
	file.write<unsigned>(r);
	/*
	 * result.type = (Type)(record >> 28);
	result.x = (record >> 14) & 0x3FFF; // "middle" 14 bits
	result.y = record & 0x3FFF;
	 */
}
