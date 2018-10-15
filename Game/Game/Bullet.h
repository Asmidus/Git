#pragma once
#include <glm/glm.hpp>
#include <Bengine/Sprite.h>

class Bullet {
public:
	Bullet();
	Bullet(glm::vec2 position, glm::vec2 direction, float speed);
	~Bullet();

	void init(glm::vec2 position, glm::vec2 direction, float speed);
	void draw();
	bool update();

	glm::vec2 getPosition() { return _position; }
private:
	int _lifeTime;
	float _speed;
	glm::vec2 _position;
	glm::vec2 _direction;
	Bengine::Sprite _sprite;
};

