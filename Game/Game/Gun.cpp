#include "Gun.h"
#include <random>
#include <ctime>



Gun::Gun() {}

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, int bulletSize, int bulletSpeed, int bulletDamage) :
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletSize(bulletSize),
	_bulletSpeed(bulletSpeed),
	_bulletDamage(bulletDamage),
	_frameCounter(fireRate) {}

Gun::~Gun() {}

void Gun::update(bool isMouseDown, glm::vec2 position, glm::vec2 direction, std::vector<Bullet*>& bullets) {
	if (_frameCounter >= _fireRate && isMouseDown) {
		fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::updateCooldown() {
	_frameCounter++;
}

void Gun::fire(glm::vec2 position, glm::vec2 direction, std::vector<Bullet*>& bullets) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1, 1);
	for (int i = 0; i < _bulletsPerShot; i++) {
		bullets.emplace_back(new Bullet(position, glm::normalize(direction + _spread*glm::vec2(randDir(randomEngine), randDir(randomEngine))), _bulletSize, _bulletSpeed, _bulletDamage));
	}
}
