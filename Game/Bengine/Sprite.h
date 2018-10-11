#pragma once
#include <GL/glew.h>
#include <string>
#include "Vertex.h"
#include "GLTexture.h"

namespace Bengine {
	class Sprite {
	public:
		Sprite();

		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

		void drawOffset(float offsetX, float offsetY);

		static void initLocation(GLuint x, GLuint y) {
			_xLoc = x;
			_yLoc = y;
		}
	private:
		int _x;
		int _y;
		int _width;
		int _height;
		static GLuint _xLoc;
		static GLuint _yLoc;

		GLuint _vboID;
		GLTexture _texture;
	};
}