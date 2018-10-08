#include "MainGame.h"
#include <iostream>
#include <Bengine/Bengine.h>
#include <Bengine/ImageLoader.h>
#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>
#include <string>



MainGame::MainGame() :
	_screenWidth(1600),
	_screenHeight(800),
	_gameState(GameState::PLAY),
	_maxFPS(2500.0f) {
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame() {}

void MainGame::run() {
	initSystems();
	_sprites.push_back(new Bengine::Sprite());
	_sprites.back()->init(0, 0, 500, 500, "images/PlayerShip.png");
	_sprites.push_back(new Bengine::Sprite());
	_sprites.back()->init(500, 250, 50, 50, "images/PlayerShip.png");
	for (int i = 0; i < 26000; i++) {
		_sprites.push_back(new Bengine::Sprite());
		_sprites.back()->init(0.2*i, 0.1*i, 50, 50, "images/PlayerShip.png");
	}
	//_tex = Bengine::ResourceManager::getTexture("images/PlayerShip.png");
	gameLoop();
	for (int i = 0; i < _sprites.size(); i++) {
		delete _sprites[i];
	}
}

void MainGame::initSystems() {
	//Initialize SDL
	Bengine::init();
	_window.create("Test Engine", _screenWidth, _screenHeight, 0);
	initShaders();
	_spriteBatch.init();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");
	_colorProgram.linkShaders();
	_colorProgram.use();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		float startTicks = SDL_GetTicks();
		processInput();
		_camera.update();
		drawGame();
		calculateFPS();
		static int frameCounter = 0;
		if (frameCounter == 10) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		frameCounter++;

		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
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
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0, 10.0 / _camera.getScale()));
				break;
			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0, -10.0 / _camera.getScale()));
				break;
			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-10.0 / _camera.getScale(), 0));
				break;
			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(10.0 / _camera.getScale(), 0));
				break;
			case SDLK_q:
				_camera.setScale(_camera.getScale()*2);
				break;
			case SDLK_e:
				_camera.setScale(_camera.getScale()/2);
				break;
			}
		}
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

	//Bengine::Color col;
	//col.r = 255;
	//col.b = 255;
	//col.g = 255;
	//col.a = 255;
	//_spriteBatch.begin();
	//for (int i = 0; i < 25000; i++) {
	//	glm::vec4 pos(0.1*i, 0.0f, 500.0f, 500.0f);
	//	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//	_spriteBatch.draw(pos, uv, _tex.id, 0.0f, col);
	//}
	//_spriteBatch.end();
	//_spriteBatch.renderBatch();
	for (int i = 0; i < _sprites.size(); i++) {
		_sprites[i]->draw();
	}
	_window.swapBuffer();
}

void MainGame::calculateFPS() {
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	
	float currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;
	prevTicks = currentTicks;
	int count;
	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	} else {
		count = NUM_SAMPLES;
	}
	float frameTimeAverage = 0;
	for (int i = 0; i < NUM_SAMPLES; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;
	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	} else {
		_fps = 0.0f;
	}
}
