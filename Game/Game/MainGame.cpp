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

MainGame::MainGame() : _screenWidth(800), _screenHeight(600), _gameState(GameState::PLAY), _maxFPS(60.0f) , player(nullptr) {
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame() {
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	//player.destroy();
	//for (int i = 0; i < _humans.size(); i++) {
	//	_humans[i]->destroy();
	//}
}

void MainGame::run() {
	initSystems();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	GLint offX = _colorProgram.getUniformLocation("offsetX");
	GLint offY = _colorProgram.getUniformLocation("offsetY");
	Bengine::Sprite::initLocation(offX, offY);
	player = new Player();
	player->init(&_inputManager);
	_humans.push_back(player);
	_levels.push_back(new Level("Levels/level1.txt"));
	player->setPosition(_levels[0]->getPlayerStartPos());
	gameLoop();
}

void MainGame::initSystems() {
	//Initialize SDL
	Bengine::init();
	_window.create("Test Engine", _screenWidth, _screenHeight, 0);
	initShaders();
	_fpsLimiter.init(_maxFPS);
	srand(time(0));

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
		for (int i = 0; i < _bullets.size();i++) {
			if (_bullets[i]->update()) {
				Bullet* temp = _bullets[i];
				_bullets[i] = _bullets.back();
				_bullets.back() = temp;
				_bullets.pop_back();
			}
		}
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

void MainGame::updateAgents() {
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[0]->getLevelData(), _humans, _zombies);
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
		_camera.setScale(_camera.getScale()*1.03);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale()/1.03);
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
	glUniform1f(0, 0);	//xOffset
	glUniform1f(1, 0);	//yOffset
	//_spriteBatch.renderBatch();
	_levels[0]->draw();
	//for (int i = 0; i < _sprites.size(); i++) {
 //      		_sprites[i]->drawOffset(i*5, i*5);
	//}
	//for (int i = 0; i < _bullets.size();i++) {
	//	_bullets[i]->draw();
	//}
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->draw();
	}
	_window.swapBuffer();
}