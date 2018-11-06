#include "Shape.h"
#include "LoadObject.h"
#include "vboindexer.hpp"
#include "ResourceManager.h"



Shape::Shape() {}

bool Shape::init() {
	vector<glm::vec3> verts;
	vector<glm::vec3> norms;
	vector<glm::vec2> uvTemp;
	LoadObject::loadOBJ("cube3.obj", verts, norms, uvTemp, matRanges, materials, textures);
	indexVBO(verts, norms, uvTemp, indices, vertices, normals, uvs);
	cout << "Finished loading object" << endl;

	glGenBuffers(4, &Buffer[0]); //Create a buffer objects for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffer[0]);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, 3*vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, Buffer[1]);
	//glBufferData(GL_ARRAY_BUFFER, 4*materials.size()*sizeof(float), materials.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, Buffer[2]);  //Buffers[1] will be the normal for each vertex
	glBufferData(GL_ARRAY_BUFFER, 3*normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, Buffer[3]);
	glBufferData(GL_ARRAY_BUFFER, 2*uvs.size()*sizeof(float), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer[3]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	initialized = true;
	return true;
}

void Shape::draw() {
	glEnable(GL_TEXTURE_2D);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer[3]);
	for (int i = 0; i < matRanges.size(); i++) {
		glVertexAttrib4f(1, materials[textures[i]].Kd.r, materials[textures[i]].Kd.g, materials[textures[i]].Kd.b, materials[textures[i]].Kd.a);
		if (materials[textures[i]].texName != "") {
			GLTexture currTexture = ResourceManager::getTexture("images/" + materials[textures[i]].texName);
			glBindTexture(GL_TEXTURE_2D, currTexture.id);
		}
		//glDrawElements(
		//	GL_TRIANGLES,
		//	(matRanges[i].y-matRanges[i].x + 1)*3,
		//	GL_UNSIGNED_SHORT,
		//	(void*)((int)matRanges[i].x*sizeof(unsigned short))
		//);
		glDrawArrays(GL_TRIANGLES, matRanges[i].x, matRanges[i].y-matRanges[i].x);
	}
}


Shape::~Shape() {}
