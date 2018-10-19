#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>
#include <Bengine/Window.h>
#include "Level.h"
#include "Bullet.h"
#include "Player.h"
#include "Agent.h"
#include "Zombie.h"
#include <list>

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
	void initLevel();
	void initShaders();
	void gameLoop();
	void updatebullets();
	void removeBullet(int index);
	void updateBullets();
	void updateAgents();
	void processInput();
	void drawGame();
	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	Player* player;
	Bengine::GLSLProgram _colorProgram;
	Bengine::Camera2D _camera;
	Bengine::SpriteBatch _spriteBatch;
	Bengine::InputManager _inputManager;
	Bengine::FPSLimiter _fpsLimiter;
	std::vector<Bengine::Sprite*> _sprites;
	//std::vector<Bullet> _bullets;
	std::vector<Bullet*> _bullets;
	std::vector<Human*> _humans;
	std::vector<Level*> _levels;
	std::vector<Zombie*> _zombies;

	float _fps;
	float _frameTime;
	float _maxFPS;
};