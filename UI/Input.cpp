#include "Input.h"

// These should be overwritten on the first frame but yippee C++ static is yay goofy why must I suffer like this
int Input::mouseX = 0;
int Input::mouseY = 0;
// Silly goofy C++17 way: auto [mouseX, mouseY] = std::pair<int, int>{0, 0}; (Doesn't seem to work either because static or because it's not in a function)
const Uint8* Input::keyState = nullptr;
const Uint8* Input::prevKeyState = nullptr;
Uint32 Input::mouseState = 0;
Uint32 Input::prevMouseState = 0;
std::vector<SDL_Event> Input::events;

// I wrote a Python script to generate this map I don't care if were meant to be learning C++ Python is so much more convenient
std::map<std::string, SDL_Scancode> Input::keyMap = {
	{"0", SDL_SCANCODE_0},
	{"1", SDL_SCANCODE_1},
	{"2", SDL_SCANCODE_2},
	{"3", SDL_SCANCODE_3},
	{"4", SDL_SCANCODE_4},
	{"5", SDL_SCANCODE_5},
	{"6", SDL_SCANCODE_6},
	{"7", SDL_SCANCODE_7},
	{"8", SDL_SCANCODE_8},
	{"9", SDL_SCANCODE_9},
	{"A", SDL_SCANCODE_A},
	{"AC Back", SDL_SCANCODE_AC_BACK},
	{"AC Bookmarks", SDL_SCANCODE_AC_BOOKMARKS},
	{"AC Forward", SDL_SCANCODE_AC_FORWARD},
	{"AC Home", SDL_SCANCODE_AC_HOME},
	{"AC Refresh", SDL_SCANCODE_AC_REFRESH},
	{"AC Search", SDL_SCANCODE_AC_SEARCH},
	{"AC Stop", SDL_SCANCODE_AC_STOP},
	{"Again", SDL_SCANCODE_AGAIN},
	{"AltErase", SDL_SCANCODE_ALTERASE},
	{"'", SDL_SCANCODE_APOSTROPHE},
	{"Application", SDL_SCANCODE_APPLICATION},
	{"AudioMute", SDL_SCANCODE_AUDIOMUTE},
	{"AudioNext", SDL_SCANCODE_AUDIONEXT},
	{"AudioPlay", SDL_SCANCODE_AUDIOPLAY},
	{"AudioPrev", SDL_SCANCODE_AUDIOPREV},
	{"AudioStop", SDL_SCANCODE_AUDIOSTOP},
	{"B", SDL_SCANCODE_B},
	{"\\", SDL_SCANCODE_BACKSLASH},
	{"Backspace", SDL_SCANCODE_BACKSPACE},
	{"BrightnessDown", SDL_SCANCODE_BRIGHTNESSDOWN},
	{"BrightnessUp", SDL_SCANCODE_BRIGHTNESSUP},
	{"C", SDL_SCANCODE_C},
	{"Calculator", SDL_SCANCODE_CALCULATOR},
	{"Cancel", SDL_SCANCODE_CANCEL},
	{"CapsLock", SDL_SCANCODE_CAPSLOCK},
	{"Clear", SDL_SCANCODE_CLEAR},
	{"Clear / Again", SDL_SCANCODE_CLEARAGAIN},
	{"Computer", SDL_SCANCODE_COMPUTER},
	{"Copy", SDL_SCANCODE_COPY},
	{"CrSel", SDL_SCANCODE_CRSEL},
	{"CurrencySubUnit", SDL_SCANCODE_CURRENCYSUBUNIT},
	{"CurrencyUnit", SDL_SCANCODE_CURRENCYUNIT},
	{"Cut", SDL_SCANCODE_CUT},
	{"D", SDL_SCANCODE_D},
	{"DecimalSeparator", SDL_SCANCODE_DECIMALSEPARATOR},
	{"Delete", SDL_SCANCODE_DELETE},
	{"DisplaySwitch", SDL_SCANCODE_DISPLAYSWITCH},
	{"Down", SDL_SCANCODE_DOWN},
	{"E", SDL_SCANCODE_E},
	{"Eject", SDL_SCANCODE_EJECT},
	{"End", SDL_SCANCODE_END},
	{"=", SDL_SCANCODE_EQUALS},
	{"Escape", SDL_SCANCODE_ESCAPE},
	{"Execute", SDL_SCANCODE_EXECUTE},
	{"ExSel", SDL_SCANCODE_EXSEL},
	{"F", SDL_SCANCODE_F},
	{"F1", SDL_SCANCODE_F1},
	{"F10", SDL_SCANCODE_F10},
	{"F11", SDL_SCANCODE_F11},
	{"F12", SDL_SCANCODE_F12},
	{"F13", SDL_SCANCODE_F13},
	{"F14", SDL_SCANCODE_F14},
	{"F15", SDL_SCANCODE_F15},
	{"F16", SDL_SCANCODE_F16},
	{"F17", SDL_SCANCODE_F17},
	{"F18", SDL_SCANCODE_F18},
	{"F19", SDL_SCANCODE_F19},
	{"F2", SDL_SCANCODE_F2},
	{"F20", SDL_SCANCODE_F20},
	{"F21", SDL_SCANCODE_F21},
	{"F22", SDL_SCANCODE_F22},
	{"F23", SDL_SCANCODE_F23},
	{"F24", SDL_SCANCODE_F24},
	{"F3", SDL_SCANCODE_F3},
	{"F4", SDL_SCANCODE_F4},
	{"F5", SDL_SCANCODE_F5},
	{"F6", SDL_SCANCODE_F6},
	{"F7", SDL_SCANCODE_F7},
	{"F8", SDL_SCANCODE_F8},
	{"F9", SDL_SCANCODE_F9},
	{"Find", SDL_SCANCODE_FIND},
	{"G", SDL_SCANCODE_G},
	{"`", SDL_SCANCODE_GRAVE},
	{"H", SDL_SCANCODE_H},
	{"Help", SDL_SCANCODE_HELP},
	{"Home", SDL_SCANCODE_HOME},
	{"I", SDL_SCANCODE_I},
	{"Insert", SDL_SCANCODE_INSERT},
	{"J", SDL_SCANCODE_J},
	{"K", SDL_SCANCODE_K},
	{"KBDIllumDown", SDL_SCANCODE_KBDILLUMDOWN},
	{"KBDIllumToggle", SDL_SCANCODE_KBDILLUMTOGGLE},
	{"KBDIllumUp", SDL_SCANCODE_KBDILLUMUP},
	{"Keypad 0", SDL_SCANCODE_KP_0},
	{"Keypad 00", SDL_SCANCODE_KP_00},
	{"Keypad 000", SDL_SCANCODE_KP_000},
	{"Keypad 1", SDL_SCANCODE_KP_1},
	{"Keypad 2", SDL_SCANCODE_KP_2},
	{"Keypad 3", SDL_SCANCODE_KP_3},
	{"Keypad 4", SDL_SCANCODE_KP_4},
	{"Keypad 5", SDL_SCANCODE_KP_5},
	{"Keypad 6", SDL_SCANCODE_KP_6},
	{"Keypad 7", SDL_SCANCODE_KP_7},
	{"Keypad 8", SDL_SCANCODE_KP_8},
	{"Keypad 9", SDL_SCANCODE_KP_9},
	{"Keypad A", SDL_SCANCODE_KP_A},
	{"Keypad &", SDL_SCANCODE_KP_AMPERSAND},
	{"Keypad @", SDL_SCANCODE_KP_AT},
	{"Keypad B", SDL_SCANCODE_KP_B},
	{"Keypad Backspace", SDL_SCANCODE_KP_BACKSPACE},
	{"Keypad Binary", SDL_SCANCODE_KP_BINARY},
	{"Keypad C", SDL_SCANCODE_KP_C},
	{"Keypad Clear", SDL_SCANCODE_KP_CLEAR},
	{"Keypad ClearEntry", SDL_SCANCODE_KP_CLEARENTRY},
	{"Keypad :", SDL_SCANCODE_KP_COLON},
	{"Keypad ,", SDL_SCANCODE_KP_COMMA},
	{"Keypad D", SDL_SCANCODE_KP_D},
	{"Keypad &&", SDL_SCANCODE_KP_DBLAMPERSAND},
	{"Keypad ||", SDL_SCANCODE_KP_DBLVERTICALBAR},
	{"Keypad Decimal", SDL_SCANCODE_KP_DECIMAL},
	{"Keypad /", SDL_SCANCODE_KP_DIVIDE},
	{"Keypad E", SDL_SCANCODE_KP_E},
	{"Keypad Enter", SDL_SCANCODE_KP_ENTER},
	{"Keypad =", SDL_SCANCODE_KP_EQUALSAS400},
	{"Keypad !", SDL_SCANCODE_KP_EXCLAM},
	{"Keypad F", SDL_SCANCODE_KP_F},
	{"Keypad >", SDL_SCANCODE_KP_GREATER},
	{"Keypad #", SDL_SCANCODE_KP_HASH},
	{"Keypad Hexadecimal", SDL_SCANCODE_KP_HEXADECIMAL},
	{"Keypad {", SDL_SCANCODE_KP_LEFTBRACE},
	{"Keypad", SDL_SCANCODE_KP_LEFTPAREN},
	{"Keypad <", SDL_SCANCODE_KP_LESS},
	{"Keypad MemAdd", SDL_SCANCODE_KP_MEMADD},
	{"Keypad MemClear", SDL_SCANCODE_KP_MEMCLEAR},
	{"Keypad MemDivide", SDL_SCANCODE_KP_MEMDIVIDE},
	{"Keypad MemMultiply", SDL_SCANCODE_KP_MEMMULTIPLY},
	{"Keypad MemRecall", SDL_SCANCODE_KP_MEMRECALL},
	{"Keypad MemStore", SDL_SCANCODE_KP_MEMSTORE},
	{"Keypad MemSubtract", SDL_SCANCODE_KP_MEMSUBTRACT},
	{"Keypad -", SDL_SCANCODE_KP_MINUS},
	{"Keypad *", SDL_SCANCODE_KP_MULTIPLY},
	{"Keypad Octal", SDL_SCANCODE_KP_OCTAL},
	{"Keypad %", SDL_SCANCODE_KP_PERCENT},
	{"Keypad .", SDL_SCANCODE_KP_PERIOD},
	{"Keypad +", SDL_SCANCODE_KP_PLUS},
	{"Keypad +/-", SDL_SCANCODE_KP_PLUSMINUS},
	{"Keypad ^", SDL_SCANCODE_KP_POWER},
	{"Keypad }", SDL_SCANCODE_KP_RIGHTBRACE},
	{"Keypad )", SDL_SCANCODE_KP_RIGHTPAREN},
	{"Keypad Space", SDL_SCANCODE_KP_SPACE},
	{"Keypad Tab", SDL_SCANCODE_KP_TAB},
	{"Keypad |", SDL_SCANCODE_KP_VERTICALBAR},
	{"Keypad XOR", SDL_SCANCODE_KP_XOR},
	{"L", SDL_SCANCODE_L},
	{"Left Alt", SDL_SCANCODE_LALT},
	{"Left Ctrl", SDL_SCANCODE_LCTRL},
	{"Left", SDL_SCANCODE_LEFT},
	{"[", SDL_SCANCODE_LEFTBRACKET},
	{"Left GUI",  SDL_SCANCODE_LGUI},
	{"Left Shift", SDL_SCANCODE_LSHIFT},
	{"M", SDL_SCANCODE_M},
	{"Mail", SDL_SCANCODE_MAIL},
	{"MediaSelect", SDL_SCANCODE_MEDIASELECT},
	{"Menu", SDL_SCANCODE_MENU},
	{"-", SDL_SCANCODE_MINUS},
	{"ModeSwitch", SDL_SCANCODE_MODE},
	{"Mute", SDL_SCANCODE_MUTE},
	{"N", SDL_SCANCODE_N},
	{"Numlock", SDL_SCANCODE_NUMLOCKCLEAR},
	{"O", SDL_SCANCODE_O},
	{"Oper", SDL_SCANCODE_OPER},
	{"Out", SDL_SCANCODE_OUT},
	{"P", SDL_SCANCODE_P},
	{"PageDown", SDL_SCANCODE_PAGEDOWN},
	{"PageUp", SDL_SCANCODE_PAGEUP},
	{"Paste", SDL_SCANCODE_PASTE},
	{"Pause", SDL_SCANCODE_PAUSE},
	{".", SDL_SCANCODE_PERIOD},
	{"Power", SDL_SCANCODE_POWER},
	{"PrintScreen", SDL_SCANCODE_PRINTSCREEN},
	{"Prior", SDL_SCANCODE_PRIOR},
	{"Q", SDL_SCANCODE_Q},
	{"R", SDL_SCANCODE_R},
	{"Right Alt", SDL_SCANCODE_RALT},
	{"Right Ctrl", SDL_SCANCODE_RCTRL},
	{"Return", SDL_SCANCODE_RETURN2},
	{"Right GUI", SDL_SCANCODE_RGUI},
	{"Right", SDL_SCANCODE_RIGHT},
	{"]", SDL_SCANCODE_RIGHTBRACKET},
	{"Right Shift", SDL_SCANCODE_RSHIFT},
	{"S", SDL_SCANCODE_S},
	{"ScrollLock", SDL_SCANCODE_SCROLLLOCK},
	{"Select", SDL_SCANCODE_SELECT},
	{";", SDL_SCANCODE_SEMICOLON},
	{"Separator", SDL_SCANCODE_SEPARATOR},
	{"/", SDL_SCANCODE_SLASH},
	{"Sleep", SDL_SCANCODE_SLEEP},
	{"Space", SDL_SCANCODE_SPACE},
	{"Stop", SDL_SCANCODE_STOP},
	{"SysReq", SDL_SCANCODE_SYSREQ},
	{"T", SDL_SCANCODE_T},
	{"Tab", SDL_SCANCODE_TAB},
	{"ThousandsSeparator", SDL_SCANCODE_THOUSANDSSEPARATOR},
	{"U", SDL_SCANCODE_U},
	{"Undo", SDL_SCANCODE_UNDO},
	{"", SDL_SCANCODE_UNKNOWN},
	{"Up", SDL_SCANCODE_UP},
	{"V", SDL_SCANCODE_V},
	{"VolumeDown", SDL_SCANCODE_VOLUMEDOWN},
	{"VolumeUp", SDL_SCANCODE_VOLUMEUP},
	{"W", SDL_SCANCODE_W},
	{"WWW", SDL_SCANCODE_WWW},
	{"X", SDL_SCANCODE_X},
	{"Y", SDL_SCANCODE_Y},
	{"Z", SDL_SCANCODE_Z}
};

std::map<std::string, int> Input::mouseMap = {
	{"Left", SDL_BUTTON_LEFT},
	{"Middle", SDL_BUTTON_MIDDLE},
	{"Right", SDL_BUTTON_RIGHT},
	{"X1", SDL_BUTTON_X1},
	{"X2", SDL_BUTTON_X2},
	{"left", SDL_BUTTON_LEFT}, // These are duplicate because god knows what the user will type (in case it's not obvious, I'm trying to make things not specific to my use case)
	{"middle", SDL_BUTTON_MIDDLE},
	{"right", SDL_BUTTON_RIGHT}
};

SDL_Scancode Input::mapKey(const std::string &key) {
	return keyMap[key];
}

[[maybe_unused]]
int Input::mapMouseButton(const std::string &button) {
	return mouseMap[button];
}

[[maybe_unused]]
bool Input::getKey(const std::string &key) {
	return keyState[mapKey(key)];
}

[[maybe_unused]]
bool Input::getKeyDown(const std::string &key) {
	return keyState[mapKey(key)] && !prevKeyState[mapKey(key)];
}

[[maybe_unused]]
bool Input::getKeyUp(const std::string &key) {
	return !keyState[mapKey(key)] && prevKeyState[mapKey(key)];
}

[[maybe_unused]]
bool Input::getMouseButton(const std::string& button) {
	int buttonCode = mouseMap[button];
	return mouseState & SDL_BUTTON(buttonCode);
}


[[maybe_unused]]
bool Input::getMouseButtonDown(const std::string &button) {
	int buttonCode = mouseMap[button];
	return (mouseState & SDL_BUTTON(buttonCode)) && !(prevMouseState & SDL_BUTTON(buttonCode));
}

[[maybe_unused]]
bool Input::getMouseButtonUp(const std::string &button) {
	int buttonCode = mouseMap[button];
	return !(mouseState & SDL_BUTTON(buttonCode)) && (prevMouseState & SDL_BUTTON(buttonCode));
}

void Input::Update() {
	prevKeyState = keyState;
	keyState = SDL_GetKeyboardState(nullptr);
	
	prevMouseState = mouseState;
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	
	events.clear();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		events.push_back(event);
	}
}

std::pair<int, int> Input::getMousePosition() {
	return {mouseX, mouseY};
}
