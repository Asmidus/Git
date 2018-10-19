#pragma once
#include <string>
#include <vector>
#include "Bullet.h"



class Gun {
public:
	Gun();
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, int bulletSize, int bulletSpeed, int bulletDamage);
	~Gun();

	void update(bool isMouseDown, glm::vec2 position, glm::vec2 direction, std::vector<Bullet*>& bullets);
	void updateCooldown();
private:
	void fire(glm::vec2 position, glm::vec2 direction, std::vector<Bullet*>& bullets);
	std::string _name;
	int _fireRate;
	int _bulletsPerShot;
	int _bulletSize;
	int _bulletSpeed;
	int _bulletDamage;
	float _spread;
	int _frameCounter;
};