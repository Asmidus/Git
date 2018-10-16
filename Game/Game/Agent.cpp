#include "Agent.h"



Agent::Agent() {
	if (Bengine::Sprite::isInitialized()) {
		init();
	}
}


Agent::~Agent() {}

void Agent::init() {
	_width = _height = 50;
	_position = glm::vec2(0, 0);
	_sprite.init(0, 0, _width, _height, "images/Circle.png", Bengine::Color(255, 255, 0, 255));
}

void Agent::update() {
	_position += glm::vec2(rand()%10-5, rand()%10-5);
}

void Agent::draw() {
	_sprite.drawOffset(_position.x - _sprite.getDimensions().x/2, _position.y - _sprite.getDimensions().y/2);
}
