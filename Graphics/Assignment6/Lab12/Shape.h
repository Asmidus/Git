#pragma once
#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <vector>
#include <glm.hpp>

class Shape {
	private:
		bool initialized;
		std::vector<float> vertices;
		std::vector<float> textures;
		std::vector<float> normals;
		std::vector<int> faces;
		GLuint Buffer[3];  //We'll need a buffer for the vertex positions

	public:
		Shape();
		~Shape();

		bool init();
		void draw();
};

