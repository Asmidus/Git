#pragma once
#include <Bengine/Sprite.h>

class Agent {
public:
	Agent();
	~Agent();

	void init();
	void update();
	void draw();

	glm::vec2 getPosition() { return _position; }
private:
	int width, height;
	glm::vec2 _position;
	Bengine::Sprite _sprite;
};

