#include "Player.h"



Player::Player() {}


Player::~Player() {}

void Player::init() {
	_sprite.init(-25, -25, 50, 50, "images/Circle.png", Bengine::Color(0, 255, 0, 255));
	_position = glm::vec2(0, 0);
}

void Player::draw() {
	_sprite.drawOffset(_position.x, _position.y);
}

void Player::update() {
	return;
}