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

MainGame::MainGame() : _screenWidth(800), _screenHeight(600), _gameState(GameState::PLAY), _maxFPS(2500.0f) {
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame() {
	for (int i = 0; i < _sprites.size(); i++) {
		delete _sprites[i];
	}
}

void MainGame::run() {
	initSystems();
	GLint offX = _colorProgram.getUniformLocation("offsetX");
	GLint offY = _colorProgram.getUniformLocation("offsetY");
	Bengine::Sprite::initLocation(offX, offY);
	player.init();
	//_sprites.push_back(new Bengine::Sprite());
	//_sprites.back()->init(0, 0, 500, 500, "images/PlayerShip.png");
	//_bullets.emplace_back(glm::vec2(1, 1), glm::vec2(1, 1), 5);
	//_sprites.push_back(new Bengine::Sprite());
	//_sprites.back()->init(500, 250, 50, 50, "images/PlayerShip.png");
	//for (int i = 0; i < 20000; i++) {
	//	_sprites.push_back(new Bengine::Sprite(offX, offY));
	//	_sprites.back()->init(0.2*i, 0.1*i, 50, 50, "images/PlayerShip.png");
	//}
	//_tex = Bengine::ResourceManager::getTexture("images/PlayerShip.png");
	//_spriteBatch.begin();
	//for (int i = 0; i < 20000; i++) {
	//	glm::vec4 pos(5*i, 0.0f, 500.0f, 500.0f);
	//	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//	_spriteBatch.draw(pos, uv, _tex.id, 0.0f, Bengine::Color(255, 255, 255));
	//}
	//_spriteBatch.end();
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
		_camera.update();
		for (int i = 0; i < _bullets.size();i++) {
			if (_bullets[i].update()) {
				Bullet temp = _bullets[i];
				_bullets[i] = _bullets.back();
				_bullets.back() = temp;
				_bullets.pop_back();
			}
		}
		for (int i = 0; i < _humans.size(); i++) {
			_humans[i].update();
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
	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0, 10.0));
		player.setPosition(player.getPosition() + glm::vec2(0, 10.0));
	}
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0, -10.0));
		player.setPosition(player.getPosition() + glm::vec2(0, -10.0));
	}
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-10.0, 0));
		player.setPosition(player.getPosition() + glm::vec2(-10.0, 0));
	}
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(10.0, 0));
		player.setPosition(player.getPosition() + glm::vec2(10.0, 0));
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale()*1.03);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale()/1.03);
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		glm::vec2 direction = mouseCoords - player.getPosition();
		direction = glm::normalize(direction);
		_bullets.emplace_back(player.getPosition(), direction, 15);
	}
	if (_inputManager.isKeyPressed(SDLK_SPACE)) {
		//for (int i = 0; i < 100; i++) {
		//	_sprites.push_back(new Bengine::Sprite());
		//	_sprites.back()->init(0, 0, 500, 500, "images/PlayerShip.png");
		//}
		_humans.emplace_back();
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
	for (int i = 0; i < _sprites.size(); i++) {
       		_sprites[i]->drawOffset(i*5, i*5);
	}
	for (int i = 0; i < _bullets.size();i++) {
		_bullets[i].draw();
	}
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i].draw();
	}
	player.draw();
	_window.swapBuffer();
}