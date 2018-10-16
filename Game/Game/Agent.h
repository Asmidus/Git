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
	void setPosition(glm::vec2 position) { _position = position; }
	glm::vec2 getDimensions() { return _sprite.getDimensions(); }
	void setSprite(Bengine::Sprite sprite) { _sprite = sprite; }
	Bengine::Sprite getSprite() { return _sprite; }
private:
	int _width, _height;
	glm::vec2 _position;
	Bengine::Sprite _sprite;
};

