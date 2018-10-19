#include "Player.h"
#include <Bengine/InputManager.h>
#include <SDL/SDL.h>


Player::Player() : _currentGun(0) {}


Player::~Player() {
	for (int i = 0; i < _playerGuns.size(); i++) {
		delete _playerGuns[i];
	}
}

void Player::init(Bengine::InputManager* inputManager, Bengine::Camera2D* camera, float speed, glm::vec2 position) {
	_inputManager = inputManager;
	_camera = camera;
	_sprite.destroy();
	_sprite.init(0, 0, 50, 50, "images/player.png");
	setPosition(glm::vec2(0, 0));
	_speed = speed;
	_position = position;
}

bool Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, std::vector<Bullet*>& bullets) {
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
	if (_inputManager->isKeyPressed(SDLK_1) && _playerGuns.size() >= 1) {
		_currentGun = 0;
	}
	if (_inputManager->isKeyPressed(SDLK_2) && _playerGuns.size() >= 2) {
		_currentGun = 1;
	}
	if (_inputManager->isKeyPressed(SDLK_3) && _playerGuns.size() >= 3) {
		_currentGun = 2;
	}
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	glm::vec2 direction = mouseCoords - _position;
	direction = glm::normalize(direction);
	for (int i = 0; i < _playerGuns.size(); i++) {
		_playerGuns[i]->updateCooldown();
	}
	_playerGuns[_currentGun]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT), _position, direction, bullets);
	if (x != 0 || y != 0) {
		_direction = glm::normalize(glm::vec2(x, y));
		_position += _direction * _speed;
	} else {
		_direction = glm::vec2(0, 0);
	}
	collideWithLevel(levelData);
	return true;
}