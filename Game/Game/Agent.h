#pragma once
#include <Bengine/Sprite.h>
#include <vector>
#include "Level.h"

class Zombie;
class Human;

class Agent {
public:
	Agent();
	~Agent();

	struct compareTile {
		compareTile(const Agent& a) : agent(a) {}
		const Agent& agent;

		bool operator()(const glm::vec2& a, const glm::vec2& b) {
			return glm::length(a - agent._position) < glm::length(b - agent._position);
		}
	};

	void init();
	virtual void destroy();
	virtual bool update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
	bool collideWithLevel(const std::vector<std::string>& levelData);
	bool collideWithAgent(Agent* agent);
	void draw();

	glm::vec2 getPosition() { return _position; }
	void setPosition(glm::vec2 position) { _position = position; }
	glm::vec2 getDirection() { return _direction; }
	void setDirection(glm::vec2 direction) { _direction = glm::normalize(direction); }
	glm::vec2 getDimensions() { return _sprite.getDimensions(); }
	int getSize() { return _size; }
	void setSprite(Bengine::Sprite sprite) { _sprite = sprite; }
	Bengine::Sprite getSprite() { return _sprite; }
	void setSpeed(float speed) { _speed = speed; }
	float getSpeed() { return _speed; }
	void damage(int damageDealt) { _health -= damageDealt; }
protected:
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePos);
	int _size;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
	int _health;
	Bengine::Sprite _sprite;
};

