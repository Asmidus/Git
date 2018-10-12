#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Bengine {
	class Color {
	public:
		Color() {}
		~Color() {}
		Color(GLubyte red, GLubyte green, GLubyte blue) { r = red; g = green; b = blue; }
		Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) { r = red; g = green; b = blue; a = alpha; }
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};
	struct Vertex {
		struct Position {
			float x;
			float y;
		} position;
		Color color;
		struct UV {
			float u;
			float v;
		} uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setColor(Color col) {
			color = col;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}