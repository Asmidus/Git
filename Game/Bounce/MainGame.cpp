#include "stdafx.h"
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

MainGame::MainGame() : _screenWidth(800), _screenHeight(600), _gameState(GameState::PLAY), _maxFPS(60.0f) {
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	GLint offX = _colorProgram.getUniformLocation("offsetX");
	GLint offY = _colorProgram.getUniformLocation("offsetY");
	Bengine::Sprite::initLocation(offX, offY);
	_camera.setPosition(glm::vec2(_screenWidth/2, _screenHeight/2));
	_camera.update();
	player.init(glm::vec2(0, 0), &_inputManager, &_camera);
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
		player.update();
		_camera.update();
		updateBullets();
		_fps = _fpsLimiter.end();
		static int frameCounter = 0;
		if (frameCounter == 10) {
			std::cout << "FPS: "<< _fps << std::endl;
			frameCounter = 0;
		}
		frameCounter++;
	}
}

void MainGame::removeBullet(int index) {
	//Bullet* temp = _bullets[index];
	//_bullets[index] = _bullets.back();
	//_bullets.back() = temp;
	//delete _bullets.back();
	//_bullets.pop_back();
}

void MainGame::updateBullets() {
	//for (int i = 0; i < _bullets.size(); i++) {
	//	if (_bullets[i]->update(_levels[0]->getLevelData())) {
	//		removeBullet(i);
	//	}
	//}
	//for (int i = 0; i < _bullets.size(); i++) {
	//	for (int j = 0; j < _zombies.size(); j++) {
	//		if (_bullets[i]->collideWithAgent(_zombies[j])) {
	//			_zombies[j]->damage(_bullets[i]->getDamage());
	//			removeBullet(i);
	//			break;
	//		}
	//	}
	//}
}

void MainGame::updateAgents() {
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

	player.draw();

	glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	glUniform1i(textureLocation, 0);
	glUniform1f(_colorProgram.getUniformLocation("offsetX"), 0);	//xOffset
	glUniform1f(_colorProgram.getUniformLocation("offsetY"), 0);	//yOffset
	_window.swapBuffer();
}