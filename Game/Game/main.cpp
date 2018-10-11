#include <iostream>
#include "MainGame.h"
#include <GL/glew.h>
#include <Bengine/Sprite.h>

GLuint Bengine::Sprite::_xLoc = 0;
GLuint Bengine::Sprite::_yLoc = 0;


int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();
	return 0;
}