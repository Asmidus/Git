#pragma once
#include <glm/glm.hpp>
#include <Bengine/Sprite.h>
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"

class Bullet {
public:
	Bullet();
	Bullet(glm::vec2 position, glm::vec2 direction, int size, float speed, float damage);
	~Bullet();

	void init(glm::vec2 position, glm::vec2 direction, int size, float speed, float damage);
	void destroy();
	void draw();
	bool update(const std::vector<std::string>& levelData);
	bool collideWithAgent(Agent* agent);
	bool collideWithWorld(const std::vector<std::string>& levelData);
	glm::vec2 getPosition() { return _position; }
	float getDamage() { return _damage; }
private:
	int _lifeTime;
	float _speed;
	float _damage;
	int _size;
	glm::vec2 _position;
	glm::vec2 _direction;
	Bengine::Sprite _sprite;
};

