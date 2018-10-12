#pragma once
#include <Bengine/Sprite.h>

class Player {
public:
	Player();
	~Player();
	void draw();
	void update();

	glm::vec2 getPosition() { return _position; }
	void setPosition(glm::vec2 position) { _position = position; }
private:
	glm::vec2 _position;
	Bengine::Sprite _sprite;
};

