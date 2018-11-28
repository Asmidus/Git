#include "stdafx.h"
#include <iostream>
#include <GL/glew.h>
#include <Bengine/Sprite.h>
#include "MainGame.h"

GLuint Bengine::Sprite::_xLoc = 0;
GLuint Bengine::Sprite::_yLoc = 0;
bool Bengine::Sprite::_initialized = false;


int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();
	return 0;
}