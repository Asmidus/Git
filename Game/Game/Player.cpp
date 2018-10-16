#include "Player.h"



Player::Player() {}


Player::~Player() {}

void Player::init() {
	Bengine::Sprite x;
	x.init(0, 0, 50, 50, "images/Circle.png", Bengine::Color(0, 255, 0, 255));
	setSprite(x);
	setPosition(glm::vec2(0, 0));
}