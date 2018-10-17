#include "Human.h"
#include <random>
#include <ctime>


Human::Human() {}

Human::Human(float speed, glm::vec2 position) {
	init(speed, position);
}

Human::~Human() {}

void Human::init(float speed, glm::vec2 position) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	_sprite.destroy();
	_sprite.init(0, 0, 50, 50, "images/human.png");
	_speed = speed;
	_position = position;
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
				   std::vector<Human*>& humans,
				   std::vector<Zombie*>& zombies) {
	_position += _direction * _speed;
	collideWithLevel(levelData);
}