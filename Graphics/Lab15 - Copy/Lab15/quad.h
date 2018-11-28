//FileName:		quad.h
//Programmer:	Dan Cliburn
//Date:			4/1/2015
//Purpose:		This file defines the header for a quad class. 
//The quad is centered at the origin in the xy plane
//I added a surface normal to this class so it would work with the Phong Reflectance Model

#ifndef QUAD_H
#define QUAD_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead

class Quad
{
protected:
	bool initialized;

	GLfloat positions[4][3];  //each vertex has an x, y, and z value
	GLfloat color[4];
	GLfloat normal[3];
	GLuint VAO;
	GLuint Buffer;  //We'll need one buffer for the vertex positions and one for the texture coordinates

public:
	Quad();

	virtual void defineVertexPositions();
	virtual void defineVerticeColor(const GLfloat *c);
	virtual bool init(const GLfloat *c = 0);  //initializes the model
	virtual void draw();  //renders the model
};

#endif