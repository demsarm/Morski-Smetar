//
// Created by martind on 18.2.2024.
//

#ifndef MORSKI_SMETAR_WAVE_H
#define MORSKI_SMETAR_WAVE_H

#include "GameObject.h"

class Wave : public GameObject {
public:
	[[maybe_unused]] Wave();
	[[maybe_unused]] Wave(int x, int y);
	[[maybe_unused]] Wave(const Wave &other);
	void Update() override;
	
	
	~Wave() override;
};


#endif //MORSKI_SMETAR_WAVE_H
