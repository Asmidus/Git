#pragma once
#include <Bengine/Sprite.h>
#include <Bengine/InputManager.h>
#include "Human.h"

class Player : public Human {
public:
	Player();
	~Player();

	void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
	void init(Bengine::InputManager* inputManager);
	void init(float speed, glm::vec2 position, Bengine::InputManager* inputManager);
private:
	Bengine::InputManager* _inputManager;
};