#pragma once
#include <Bengine/Sprite.h>
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>

class Player {
public:
	Player();
	~Player();

	void init(glm::vec2 position, Bengine::InputManager* inputManager, Bengine::Camera2D* camera);
	void update();
	void draw();
private:
	Bengine::Sprite _sprite;
	glm::vec2 _position;
	Bengine::InputManager* _inputManager;
	Bengine::Camera2D* _camera;
};