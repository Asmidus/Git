#include "Shape.h"
#include "LoadObject.h"



Shape::Shape() {}

bool Shape::init() {
	LoadObject::loadOBJ("weird.obj", vertices, normals, uvs, faces);
	glGenBuffers(3, &Buffer[0]); //Create a buffer objects for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffer[0]);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer[2]);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(2);
	initialized = true;
	return true;
}

void Shape::draw() {
	glBindVertexArray(VAO);
	glVertexAttrib1f(3, 25.0);	//All sides will have the same "shininess". This might seem
								//counterintuitive, but the smaller this number the more
								//noticable the specular highlights will be.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}


Shape::~Shape() {}
