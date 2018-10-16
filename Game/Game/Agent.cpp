#include "Agent.h"
#include <algorithm>
#include <iostream>


Agent::Agent() {
	if (Bengine::Sprite::isInitialized()) {
		init();
	}
}


Agent::~Agent() {}

void Agent::init() {
	_width = _height = 50;
	_position = glm::vec2(0, 0);
	_direction = glm::vec2(0, 0);
	_speed = 5;
	_sprite.init(0, 0, _width, _height, "images/Circle.png", Bengine::Color(255, 255, 0, 255));
}

void Agent::destroy() {
	_sprite.destroy();
}

void Agent::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	//_direction = glm::normalize(glm::vec2(rand()%2, rand()%2));
	_position += _direction * _speed;
	collideWithLevel(levelData);
}

void Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;
	checkTilePosition(levelData, collideTilePositions, _position.x - _width/2, _position.y - _height/2);
	checkTilePosition(levelData, collideTilePositions, _position.x + _width/2, _position.y - _height/2);
	checkTilePosition(levelData, collideTilePositions, _position.x - _width/2, _position.y + _height/2);
	checkTilePosition(levelData, collideTilePositions, _position.x + _width/2, _position.y + _height/2);
	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
}

void Agent::draw() {
	_sprite.drawOffset(_position.x - _sprite.getDimensions().x/2, _position.y - _sprite.getDimensions().y/2);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_SIZE), floor(52 + y / (float)TILE_SIZE));
	glm::vec2 z = glm::vec2(floor(x / (float)TILE_SIZE), floor(y / (float)TILE_SIZE));
	if (cornerPos.x < 0 || cornerPos.y < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y >= levelData.size()) {
		return;
	}
	if (levelData[abs(cornerPos.y)][cornerPos.x] != '.') {
		collideTilePositions.push_back(z * (float)TILE_SIZE + glm::vec2(TILE_SIZE/2, TILE_SIZE/2));
	}
}

void Agent::collideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUS = (float)_width/2;
	const float TILE_RADIUS = (float)TILE_SIZE/2;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	//glm::vec2 position(_position.x - _width/2, _position.y - _height/2);
	glm::vec2 distVec = glm::vec2(_position.x, _position.y) - tilePos;
	float xdepth = MIN_DISTANCE - abs(distVec.x);
	float ydepth = MIN_DISTANCE - abs(distVec.y);

	//collision occurs
	if (xdepth > 0 || ydepth > 0) {
		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
			if (distVec.x < 0) {
				_position.x -= xdepth;
			} else {
				_position.x += xdepth;
			}
		} else {
			if (distVec.y < 0) {
				_position.y -= ydepth;
			} else {
				_position.y += ydepth;
			}
		}
	}
}