#include "Bullet.h"



Bullet::Bullet() {}

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed) {
	init(position, direction, speed);
}

Bullet::~Bullet() {}

void Bullet::init(glm::vec2 position, glm::vec2 direction, float speed) {
	_lifeTime = 1000;
	_position = position;
	_direction = direction;
	_speed = speed;
	_sprite.init(0, 0, 10, 10, "images/Circle.png");
}

void Bullet::draw() {
	_sprite.drawOffset(_position.x - _sprite.getDimensions().x/2, _position.y - _sprite.getDimensions().y/2);
}

bool Bullet::update() {
	_position += _direction * _speed;
	if (_lifeTime == 0) {
		return true;
		_sprite.destroy();
	}
	_lifeTime--;
	return false;
}
