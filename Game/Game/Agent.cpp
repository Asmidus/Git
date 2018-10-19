#include "Agent.h"
#include <algorithm>
#include <iostream>


Agent::Agent() : _health(10) {
	if (Bengine::Sprite::isInitialized()) {
		init();
	}
}

Agent::~Agent() {}

void Agent::init() {
	_size = 50;
	_position = glm::vec2(0, 0);
	_direction = glm::vec2(0, 0);
	_speed = 5;
	_sprite.init(0, 0, _size, _size, "images/Circle.png", Bengine::Color(255, 255, 0, 255));
}

void Agent::destroy() {
	_sprite.destroy();
}

bool Agent::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	//_direction = glm::normalize(glm::vec2(rand()%2, rand()%2));
	_position += _direction * _speed;
	collideWithLevel(levelData);
	return true;
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;
	checkTilePosition(levelData, collideTilePositions, _position.x - _size/2, _position.y - _size/2);
	checkTilePosition(levelData, collideTilePositions, _position.x + _size/2, _position.y - _size/2);
	checkTilePosition(levelData, collideTilePositions, _position.x - _size/2, _position.y + _size/2);
	checkTilePosition(levelData, collideTilePositions, _position.x + _size/2, _position.y + _size/2);
	//if (collideTilePositions.size() == 0) {
	//	return;
	//}
	//for (int i = 0; i < collideTilePositions.size() - 1; i++) {
	//	if (glm::length(collideTilePositions[i] - _position) > glm::length(collideTilePositions[i + 1] - _position)) {
	//		auto temp = collideTilePositions[i];
	//		collideTilePositions[i] = collideTilePositions[i + 1];
	//		collideTilePositions[i + 1] = temp;
	//	}
	//}
	if (collideTilePositions.size() == 0) {
		return false;
	}
	std::sort(collideTilePositions.begin(), collideTilePositions.end(), compareTile(*this));
	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

bool Agent::collideWithAgent(Agent* agent) {
	glm::vec2 distance = _position - agent->_position;
	if (glm::length(distance) <= _size/2 + agent->_size/2) {
		float depth = _size - glm::length(distance);
		glm::vec2 collisionDepthVec = glm::normalize(distance) * depth;
		_position += collisionDepthVec/2.0f;
		agent->_position -= collisionDepthVec/2.0f;
		return true;
	}
	return false;
}

void Agent::draw() {
	_sprite.drawOffset(_position.x - _sprite.getDimensions().x/2, _position.y - _sprite.getDimensions().y/2);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_SIZE), floor(y / (float)TILE_SIZE));							//this value changes if the map is TRANSLATED
	glm::vec2 actualPosition = glm::vec2(floor(x / (float)TILE_SIZE), floor(y / (float)TILE_SIZE));
	if (cornerPos.x < 0 || cornerPos.y < 0 || cornerPos.x > levelData[0].size() || cornerPos.y > levelData.size()) {
		return;
	}
	if (levelData[abs(levelData.size() - 1 - cornerPos.y)][cornerPos.x] != '.') {										//this value changes if the map is FLIPPED
		collideTilePositions.push_back(actualPosition * (float)TILE_SIZE + glm::vec2(TILE_SIZE/2, TILE_SIZE/2));
	}
}

void Agent::collideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUS = (float)_size/2;
	const float TILE_RADIUS = (float)TILE_SIZE/2;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	//glm::vec2 position(_position.x - _width/2, _position.y - _height/2);
	glm::vec2 distVec = _position - tilePos;
	float xdepth = MIN_DISTANCE - abs(distVec.x);
	float ydepth = MIN_DISTANCE - abs(distVec.y);

	//collision occurs
	if (std::max(xdepth, 0.0f) == std::max(ydepth, 0.0f)) { return; }
	if (xdepth > 0 && ydepth > 0) {
		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
			if (distVec.x < 0) {
				_position.x -= xdepth;
			} else {
				_position.x += xdepth;
			}
		} else if (std::max(xdepth, 0.0f) > std::max(ydepth, 0.0f)) {
			if (distVec.y < 0) {
				_position.y -= ydepth;
			} else {
				_position.y += ydepth;
			}
		}
	}
}