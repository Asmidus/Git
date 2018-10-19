#pragma once
#include <Bengine/Sprite.h>
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>
#include "Human.h"
#include "Gun.h"

class Player : public Human {
public:
	Player();
	~Player();

	void init(Bengine::InputManager * inputManager, Bengine::Camera2D * camera, float speed, glm::vec2 position);
	bool update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, std::vector<Bullet*>& bullets);
	void addGun(Gun* gun) { _playerGuns.push_back(gun); }
	std::vector<Gun*> getGuns() { return _playerGuns; }
	int getCurrentGun() { return _currentGun; }
private:
	Bengine::InputManager* _inputManager;
	std::vector<Gun*> _playerGuns;
	Bengine::Camera2D* _camera;
	int _currentGun;
};