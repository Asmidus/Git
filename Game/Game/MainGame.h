#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <Bengine/Sprite.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>

enum class GameState {
	PLAY,
	EXIT
};

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	Bengine::GLSLProgram _colorProgram;
	Bengine::Camera2D _camera;
	Bengine::SpriteBatch _spriteBatch;
	Bengine::GLTexture _tex;
	Bengine::InputManager _inputManager;
	Bengine::FPSLimiter _fpsLimiter;
	std::vector<Bengine::Sprite*> _sprites;
	float x[26002];
	float y[26002];

	float _fps;
	float _frameTime;
	float _maxFPS;
};