#include "MainGame.h"
#include <iostream>
#include <Bengine/Bengine.h>
#include <Bengine/ImageLoader.h>
#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>
#include <Bengine/InputManager.h>
#include <string>
#include <random>
#include <time.h>
#include <string>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 2.5f;
const float PLAYER_SPEED = 10.0f;

MainGame::MainGame() : _screenWidth(800), _screenHeight(600), _gameState(GameState::PLAY), _maxFPS(60.0f) , player(nullptr) {
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame() {
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	//player.destroy();
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->destroy();
		delete _humans[i];
	}
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i]->destroy();
		delete _bullets[i];
	}
}

void MainGame::run() {
	initSystems();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	GLint offX = _colorProgram.getUniformLocation("offsetX");
	GLint offY = _colorProgram.getUniformLocation("offsetY");
	Bengine::Sprite::initLocation(offX, offY);
	initLevel();
	_humans.push_back(new Human(1.0f, glm::vec2(64*2, 64*50)));
	gameLoop();
}

void MainGame::initSystems() {
	//Initialize SDL
	Bengine::init();
	_window.create("Test Engine", _screenWidth, _screenHeight, 0);
	initShaders();
	//_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
	srand(time(0));

}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	player = new Player();
	player->init(PLAYER_SPEED, _levels[0]->getPlayerStartPos(), &_inputManager, &_camera);
	player->addGun(new Gun("Blaster", 1, 5, 0.15, 50, 25, 5));
	player->addGun(new Gun("Sniper", 10, 1, 0, 25, 50, 5));
	player->addGun(new Gun("Shotgun", 20, 5, 0.05, 10, 25, 2));
	_humans.push_back(player);

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_int_distribution<int> randX(1*TILE_SIZE + TILE_SIZE/2, (_levels[0]->getWidth() - 2)*TILE_SIZE + TILE_SIZE/2);
	static std::uniform_int_distribution<int> randY(1*TILE_SIZE + TILE_SIZE/2, (_levels[0]->getHeight() - 2)*TILE_SIZE + TILE_SIZE/2);

	for (int i = 0; i < 1000; i++) {
		_humans.push_back(new Human());
		_humans.back()->init(HUMAN_SPEED, glm::vec2(randX(randomEngine), randY(randomEngine)));
	}

	const std::vector<glm::vec2>& zombiePositions = _levels[0]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie());
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");
	_colorProgram.linkShaders();
	_colorProgram.use();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();
		drawGame();
		processInput();
		updateAgents();
		_camera.setPosition(player->getPosition());
		_camera.update();
		updateBullets();
		_fps = _fpsLimiter.end();
		static int frameCounter = 0;
		if (frameCounter == 10) {
			std::cout << "FPS: "<< _fps << std::endl;
			std::cout << "SpriteCount: " << _sprites.size() << std::endl;
			std::cout << "BulletCount: " << _bullets.size() << std::endl;
			std::cout << "HumanCount: " << _humans.size() << std::endl;
			frameCounter = 0;
		}
		frameCounter++;
	}
}

void MainGame::removeBullet(int index) {
	Bullet* temp = _bullets[index];
	_bullets[index] = _bullets.back();
	_bullets.back() = temp;
	delete _bullets.back();
	_bullets.pop_back();
}

void MainGame::updateBullets() {
	for (int i = 0; i < _bullets.size(); i++) {
		if (_bullets[i]->update(_levels[0]->getLevelData())) {
			removeBullet(i);
		}
	}
	for (int i = 0; i < _bullets.size(); i++) {
		for (int j = 0; j < _zombies.size(); j++) {
			if (_bullets[i]->collideWithAgent(_zombies[j])) {
				_zombies[j]->damage(_bullets[i]->getDamage());
				removeBullet(i);
				break;
			}
		}
	}
}

void MainGame::updateAgents() {
	player->update(_levels[0]->getLevelData(), _humans, _zombies, _bullets);
	for (int i = 1; i < _humans.size(); i++) {
		_humans[i]->update(_levels[0]->getLevelData(), _humans, _zombies);
	}
	for (int i = 0; i < _zombies.size(); i++) {
		if (_zombies[i]->update(_levels[0]->getLevelData(), _humans, _zombies)) {
			auto temp = _zombies[i];
			_zombies[i] = _zombies.back();
			_zombies.back() = temp;
			delete _zombies.back();
			_zombies.pop_back();
		}
	}
	for (int i = 0; i < _humans.size(); i++) {
		//for (int j = i + 1; j < _humans.size(); j++) {
		//	_humans[i]->collideWithAgent(_humans[j]);
		//}
	}
	for (int i = 0; i < _zombies.size(); i++) {
		//for (int j = i + 1; j < _zombies.size(); j++) {
		//	_zombies[i]->collideWithAgent(_zombies[j]);
		//}
		//for (int j = 1; j < _humans.size(); j++) {
		//	if (_zombies[i]->collideWithAgent(_humans[j])) {
		//		_zombies.push_back(new Zombie());
		//		_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
		//		auto temp = _humans[j];
		//		_humans[j] = _humans.back();
		//		_humans.back() = temp;
		//		_humans.back()->destroy();
		//		delete _humans.back();
		//		_humans.pop_back();
		//	}
		//}
		int j = _zombies[i]->getClosestHumanIndex();
		if (_zombies[i]->collideWithAgent(_humans[j])) {
			_zombies.push_back(new Zombie());
			_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
			auto temp = _humans[j];
			_humans[j] = _humans.back();
			_humans.back() = temp;
			_humans.back()->destroy();
			delete _humans.back();
			_humans.pop_back();
		}
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale()*1.02);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale()/1.02);
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("texSampler");
	GLint orthoLocation = _colorProgram.getUniformLocation("ortho");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	glUniform1i(textureLocation, 0);
	glUniform1f(_colorProgram.getUniformLocation("offsetX"), 0);	//xOffset
	glUniform1f(_colorProgram.getUniformLocation("offsetY"), 0);	//yOffset
	_levels[0]->draw();
	for (int i = 0; i < _bullets.size();i++) {
		_bullets[i]->draw();
	}
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->draw();
	}
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->draw();
	}
	_window.swapBuffer();
}