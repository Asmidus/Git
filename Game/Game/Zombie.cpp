#include "Zombie.h"
#include "Human.h"

Zombie::Zombie() {}


Zombie::~Zombie() {}

void Zombie::init(float speed, glm::vec2 position) {
	_sprite.destroy();
	_sprite.init(0, 0, 50, 50, "images/zombie.png");
	_speed = speed;
	_position = position;
	_direction = glm::vec2(-1, 0);
	_direction = glm::normalize(_direction);
}

bool Zombie::update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans,
					std::vector<Zombie*>& zombies) {
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr) {
		_direction = glm::normalize(closestHuman->getPosition() - _position);
	}
	collideWithLevel(levelData);
	_position += _direction * _speed;
	if (_health <= 0) {
		_sprite.destroy();
		return true;
	}
	return false;
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = INT_MAX;
	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
