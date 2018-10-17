#pragma once
#include "Agent.h"

class Human : public Agent {
public:
	Human();
	Human(float speed, glm::vec2 position);
	~Human();
	void init(float speed, glm::vec2 position);
	void update(const std::vector<std::string>& levelData,
				std::vector<Human*>& humans,
				std::vector<Zombie*>& zombies);
};

