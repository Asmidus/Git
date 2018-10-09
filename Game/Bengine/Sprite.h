#pragma once
#include <GL/glew.h>
#include <string>
#include "Vertex.h"
#include "GLTexture.h"

namespace Bengine {
	class Sprite {
	public:
		Sprite();

		Sprite(GLuint xLoc, GLuint yLoc) : _xLoc(xLoc), _yLoc(yLoc) { _vboID = 0; };

		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

		void drawOffset(float offsetX, float offsetY);
	private:
		int _x;
		int _y;
		int _width;
		int _height;
		GLuint _xLoc;
		GLuint _yLoc;

		GLuint _vboID;
		GLTexture _texture;
	};
}