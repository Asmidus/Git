#pragma once
#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <vector>
#include <glm.hpp>
#include "LoadObject.h"

using namespace std;

class Shape {
	private:
		bool initialized;
		vector<glm::vec3> vertices;
		vector<glm::vec3> normals;
		vector<glm::vec2> uvs;
		vector<glm::vec2> matRanges;
		map<string, Material> materials;
		vector<string> textures;
		vector<unsigned short> indices;
		glm::vec3 glow;
		GLuint Buffer[4];  //We'll need a buffer for the vertex positions

	public:
		Shape();
		~Shape();

		bool init(string fileName, glm::vec3 objectGlow = glm::vec3(0, 0, 0), bool smooth = true);
		void draw();
};

