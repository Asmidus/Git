#pragma once
#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <vector>
#include <glm.hpp>s

class Shape {
	private:
		bool initialized;
		std::vector<float> vertices;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<int> faces;
		GLfloat positions[8][3];  //each vertex has an x, y, and z value for its position
		GLuint VAO;
		GLuint Buffer[3];  //We'll need a buffer for the vertex positions

	public:
		Shape();
		~Shape();

		bool init();
		void draw();
};

