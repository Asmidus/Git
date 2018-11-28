#include "stdafx.h""
#include "Player.h"
#include <Bengine/InputManager.h>
#include <SDL/SDL.h>


Player::Player() {}


Player::~Player() {}

void Player::init(glm::vec2 position, Bengine::InputManager* inputManager, Bengine::Camera2D* camera) {
	_inputManager = inputManager;
	_camera = camera;
	_sprite.destroy();
	_sprite.init(0, 0, 300, 50, "images/glass.png");
	_position = position;
}

void Player::update() {
	//int x = 0, y = 0;
	//if (_inputManager->isKeyPressed(SDLK_w)) {
	//	//setPosition(getPosition() + glm::vec2(0, 10.0));
	//	y = 1;
	//}
	//if (_inputManager->isKeyPressed(SDLK_s)) {
	//	//setPosition(getPosition() + glm::vec2(0, -10.0));
	//	y = -1;
	//}
	//if (_inputManager->isKeyPressed(SDLK_a)) {
	//	//setPosition(getPosition() + glm::vec2(-10.0, 0));
	//	x = -1;
	//}
	//if (_inputManager->isKeyPressed(SDLK_d)) {
	//	x = 1;
	//	//setPosition(getPosition() + glm::vec2(10.0, 0));
	//}
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	//glm::vec2 direction = mouseCoords - _position;
	//direction = glm::normalize(direction);
	_position.x = mouseCoords.x;
	//if (x != 0 || y != 0) {
	//	_direction = glm::normalize(glm::vec2(x, y));
	//	_position += _direction * _speed;
	//} else {
	//	_direction = glm::vec2(0, 0);
	//}
}

void Player::draw() {
	_sprite.drawOffset(_position.x - _sprite.getDimensions().x/2, _position.y);
}