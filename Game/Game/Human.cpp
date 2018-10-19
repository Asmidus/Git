#include "Human.h"
#include <random>
#include <ctime>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


Human::Human() : _frames(0) {}

Human::Human(float speed, glm::vec2 position) : _frames(0) {
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

bool Human::update(const std::vector<std::string>& levelData,
				   std::vector<Human*>& humans,
				   std::vector<Zombie*>& zombies) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-1.0f, 1.0f);
	_position += _direction * _speed; 
	_frames++;
	if (_frames%20 == 0) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}
	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}
	if (_health <= 0) {
		_sprite.destroy();
		return true;
	}
	return false;
}