#include "Bullet.h"



Bullet::Bullet() {}

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, int size, float speed, float damage) {
	init(position, direction, size, speed, damage);
}

Bullet::~Bullet() {}

void Bullet::init(glm::vec2 position, glm::vec2 direction, int size, float speed, float damage) {
	_lifeTime = 1000;
	_position = position;
	_direction = direction;
	_speed = speed;
	_damage = damage;
	_size = size;
	_sprite.init(0, 0, size, size, "images/Circle.png");
}

void Bullet::destroy() {
	_sprite.destroy();
}

void Bullet::draw() {
	_sprite.drawOffset(_position.x - _sprite.getDimensions().x/2, _position.y - _sprite.getDimensions().y/2);
}

bool Bullet::update(const std::vector<std::string>& levelData) {
	_position += _direction * _speed;
	if (_lifeTime == 0 || collideWithWorld(levelData)) {
		_sprite.destroy();
		return true;
	}
	_lifeTime--;
	return false;
}

bool Bullet::collideWithAgent(Agent* agent) {
	glm::vec2 distance = _position - agent->getPosition();
	if (glm::length(distance) <= _size/2 + agent->getSize()/2) {
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	glm::vec2 gridPosition;
	gridPosition.x = floor(_position.x/(float)TILE_SIZE);
	gridPosition.y = floor(_position.y/(float)TILE_SIZE);
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return true;
	}
	int x = (levelData.size() - 1 - gridPosition.y);
	return (levelData[(levelData.size() - 1 - gridPosition.y)][gridPosition.x] != '.');
}
