#pragma once
#include <string>
#include <vector>
#include <Bengine/ResourceManager.h>
#include <Bengine/SpriteBatch.h>

const int TILE_SIZE = 64;

class Level {
public:
	Level(const std::string & fileName);
	~Level();

	void draw();
	glm::vec2 getPlayerStartPos() { return _playerStartPos; }
	std::vector<std::string> getLevelData() { return _levelData; }
	int getNumHumans() { return _numHumans; }
	int getWidth() { return _levelData[0].size(); }
	int getHeight() { return _levelData.size(); }
	std::vector<glm::vec2> getZombieStartPos() { return _zombieStartPos; }
private:
	std::vector<std::string> _levelData;
	int _numHumans;
	Bengine::SpriteBatch _spriteBatch;
	Bengine::ResourceManager* _resourceManager;		//this is the pointer to the manager in MainGame

	glm::vec2 _playerStartPos;
	std::vector<glm::vec2> _zombieStartPos;
};

