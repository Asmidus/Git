#include "Player.h"
#include <Bengine/InputManager.h>
#include <SDL/SDL.h>


Player::Player() {}


Player::~Player() {}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	int x = 0, y = 0;
	if (_inputManager->isKeyPressed(SDLK_w)) {
		//setPosition(getPosition() + glm::vec2(0, 10.0));
		y = 1;
	}
	if (_inputManager->isKeyPressed(SDLK_s)) {
		//setPosition(getPosition() + glm::vec2(0, -10.0));
		y = -1;
	}
	if (_inputManager->isKeyPressed(SDLK_a)) {
		//setPosition(getPosition() + glm::vec2(-10.0, 0));
		x = -1;
	}
	if (_inputManager->isKeyPressed(SDLK_d)) {
		x = 1;
		//setPosition(getPosition() + glm::vec2(10.0, 0));
	}
	if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		//glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		//mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		//glm::vec2 direction = mouseCoords - player.getPosition();
		//direction = glm::normalize(direction);
		//_bullets.emplace_back(player.getPosition(), direction, 15);
	}
	if (x != 0 || y != 0) {
		_direction = glm::normalize(glm::vec2(x, y));
		_position += _direction * _speed;
	}
	collideWithLevel(levelData);
}

void Player::init(Bengine::InputManager* inputManager) {
	_inputManager = inputManager;
	_sprite.destroy();
	_sprite.init(0, 0, 50, 50, "images/player.png");
	setPosition(glm::vec2(0, 0));
}

void Player::init(float speed, glm::vec2 position, Bengine::InputManager * inputManager) {
	init(inputManager);
	_speed = speed;
	_position = position;
}
