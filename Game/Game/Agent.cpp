#include "Agent.h"



Agent::Agent() {
	init();
}


Agent::~Agent() {}

void Agent::init() {
	_sprite.init(-25, -25, 50, 50, "images/Circle.png", Bengine::Color(255, 255, 0, 255));
	_position = glm::vec2(0, 0);
}

void Agent::update() {
	_position += glm::vec2(rand()%10-5, rand()%10-5);
}

void Agent::draw() {
	_sprite.drawOffset(_position.x, _position.y);
}
