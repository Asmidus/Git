#pragma once
#include "Agent.h"
class Zombie : public Agent {
public:
	Zombie();
	~Zombie();

	void init(float speed, glm::vec2 position);
	bool update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
	int getClosestHumanIndex() { return _closestHumanIndex; }

private:
	Human* getNearestHuman(std::vector<Human*>& humans);
	int _closestHumanIndex;
};