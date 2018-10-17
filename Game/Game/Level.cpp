#include "Level.h"
#include <fstream>
#include <iostream>
#include <Bengine/Errors.h>


Level::Level(const std::string& fileName) {
	std::ifstream file;
	file.open(fileName);
	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}
	std::string tmp;
	file >> tmp >> _numHumans;
	std::getline(file, tmp);
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}
	for (int i = 0; i < _levelData.size(); i++) {
		std::cout << _levelData[i] << std::endl;
	}
	_spriteBatch.init();
	_spriteBatch.begin();
	glm::vec4 uvRect(0, 0, 1, 1);
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			//grab the tile
			char tile = _levelData[y][x];
			//get the destination
			glm::vec4 destRect(x * TILE_SIZE, (_levelData.size() - 1 - y) * TILE_SIZE, TILE_SIZE, TILE_SIZE);	// -y to flip
			//draw the appropriate tile
			switch (tile) {
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, uvRect, Bengine::ResourceManager::getTexture("images/red_bricks.png").id, 0.0f, Bengine::Color(255, 255, 255));
				break;
			case 'G':
				_spriteBatch.draw(destRect, uvRect, Bengine::ResourceManager::getTexture("images/glass.png").id, 0.0f, Bengine::Color(255, 255, 255));
				break;
			case 'L':
				_spriteBatch.draw(destRect, uvRect, Bengine::ResourceManager::getTexture("images/light_bricks.png").id, 0.0f, Bengine::Color(255, 255, 255));
				break;
			case '@':
				_playerStartPos.x = x * TILE_SIZE + TILE_SIZE/2;
				_playerStartPos.y = (_levelData.size() - 1 - y) * TILE_SIZE + TILE_SIZE/2;						// -y to flip
				_levelData[y][x] = '.';
				break;
			case 'Z':
				_zombieStartPos.emplace_back(x * TILE_SIZE + TILE_SIZE/2, (_levelData.size() - 1 - y) * TILE_SIZE + TILE_SIZE/2);	//-y to flip
				_levelData[y][x] = '.';
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)\n", tile, x, y);
				break;
			}
		}
	}
	_spriteBatch.end();
}


Level::~Level() {}

void Level::draw() {
	_spriteBatch.renderBatch();
}
