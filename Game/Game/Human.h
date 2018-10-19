#pragma once
#include "Agent.h"

class Human : public Agent {
public:
	Human();
	Human(float speed, glm::vec2 position);
	~Human();
	void init(float speed, glm::vec2 position);
	bool update(const std::vector<std::string>& levelData,
				std::vector<Human*>& humans,
				std::vector<Zombie*>& zombies);
private:
	int _frames;
};

