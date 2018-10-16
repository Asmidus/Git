#include "Player.h"
#include <Bengine/InputManager.h>
#include <SDL/SDL.h>


Player::Player() {}


Player::~Player() {}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	if (_inputManager->isKeyPressed(SDLK_w)) {
		setPosition(getPosition() + glm::vec2(0, 10.0));
	}
	if (_inputManager->isKeyPressed(SDLK_s)) {
		setPosition(getPosition() + glm::vec2(0, -10.0));
	}
	if (_inputManager->isKeyPressed(SDLK_a)) {
		setPosition(getPosition() + glm::vec2(-10.0, 0));
	}
	if (_inputManager->isKeyPressed(SDLK_d)) {
		setPosition(getPosition() + glm::vec2(10.0, 0));
	}
	if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		//glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		//mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		//glm::vec2 direction = mouseCoords - player.getPosition();
		//direction = glm::normalize(direction);
		//_bullets.emplace_back(player.getPosition(), direction, 15);
	}
	collideWithLevel(levelData);
}

void Player::init(Bengine::InputManager* inputManager) {
	_inputManager = inputManager;
	getSprite().destroy();
	Bengine::Sprite x;
	x.init(0, 0, 50, 50, "images/player.png");
	setSprite(x);
	setPosition(glm::vec2(0, 0));
}