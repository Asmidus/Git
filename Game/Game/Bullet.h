#pragma once
#include <glm/glm.hpp>
#include <Bengine/Sprite.h>

class Bullet {
public:
	Bullet();
	Bullet(glm::vec2 direction, glm::vec2 position, float speed);
	~Bullet();

	void init(glm::vec2 direction, glm::vec2 position, float speed);
	void draw();
	bool update();
private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
	Bengine::Sprite _sprite;
};

