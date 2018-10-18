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

		void destroy();

		void init(float x, float y, float width, float height, std::string texturePath, Color color);

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

		void drawOffset(float offsetX, float offsetY);

		glm::vec2 getDimensions();

		static void initLocation(GLuint x, GLuint y) {
			_xLoc = x;
			_yLoc = y;
			_initialized = true;
		}
		static bool isInitialized() { return _initialized; }
	private:
		int _x;
		int _y;
		int _size;
		int _height;
		static GLuint _xLoc;
		static GLuint _yLoc;
		static bool _initialized;

		GLuint _vboID;
		GLTexture _texture;
	};
}