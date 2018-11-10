//FileName:		cube.h
//Programmer:	Dan Cliburn
//Date:			4/2/2015
//Purpose:		This file defines the header for a cube that is centered at the origin

#ifndef CUBE_H
#define CUBE_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead

class Cube
{
private:
	bool initialized;

	GLfloat positions[8][3];  //each vertex has an x, y, and z value for its position
	GLuint VAO;
	GLuint Buffer;  //We'll need a buffer for the vertex positions

public:
	Cube();

	bool init();
	void draw();
};

#endif