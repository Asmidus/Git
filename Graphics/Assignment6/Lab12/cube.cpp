//FileName:		cube.cpp
//Programmer:	Dan Cliburn
//Date:			4/2/2015
//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "cube.h"
#include <iostream>
using namespace std;

Cube::Cube()
{
	VAO = 0;
	initialized = false;
}

//init() does all of the OpenGL initialization for your model(s)
bool Cube::init()
{
	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);

	//front face
	positions[0][0] = -1.0; //x value
	positions[0][1] = 1.0; //y value
	positions[0][2] = 1.0; //z value
	positions[1][0] = -1.0; //x value
	positions[1][1] = -1.0; //y value
	positions[1][2] = 1.0; //z value
	positions[2][0] = 1.0; //x value
	positions[2][1] = 1.0; //y value
	positions[2][2] = 1.0; //z value
	positions[3][0] = 1.0; //x value
	positions[3][1] = -1.0; //y value
	positions[3][2] = 1.0; //z value

	//two more vertices for right face
	positions[4][0] = 1.0; //x value
	positions[4][1] = 1.0; //y value
	positions[4][2] = -1.0; //z value
	positions[5][0] = 1.0; //x value
	positions[5][1] = -1.0; //y value
	positions[5][2] = -1.0; //z value

	//two more vertices for back face
	positions[6][0] = -1.0; //x value
	positions[6][1] = 1.0; //y value
	positions[6][2] = -1.0; //z value
	positions[7][0] = -1.0; //x value
	positions[7][1] = -1.0; //y value
	positions[7][2] = -1.0; //z value

	glGenBuffers(1, &Buffer); //Create a buffer objects for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0);

	//NOTE: Since many vertices will share the values for color and surface normal, 
	//we'll set those with static vertex attributes in the draw() method.

	initialized = true;
	return true;  //Everything got initialized
}

//draw() explains how to render your model
void Cube::draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a Cube object before it has been initialized." << endl;
		return;
	}

	glBindVertexArray(VAO);

	//First set the shininess in attribute location 3
	glVertexAttrib1f(3, 25.0);	//All sides will have the same "shininess". This might seem
								//counterintuitive, but the smaller this number the more
								//noticable the specular highlights will be.
	//Make front face red
	glVertexAttrib4f(1, 1.0, 0.0, 0.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Make right face green
	glVertexAttrib4f(1, 0.0, 1.0, 0.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, 1.0, 0.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);

	//Make back face blue
	glVertexAttrib4f(1, 0.0, 0.0, 1.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, -1.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	//Make left face yellow
	glVertexAttrib4f(1, 1.0, 1.0, 0.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, -1.0, 0.0, 0.0);  //normal is attribute location 2
	unsigned int indicesLeftFace[4] = { 6, 7, 0, 1 };
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indicesLeftFace); //glDrawElements performs indexed rendering

	//Make top face cyan
	glVertexAttrib4f(1, 0.0, 1.0, 1.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 1.0, 0.0);  //normal is attribute location 2
	unsigned int indicesTopFace[4] = { 6, 0, 4, 2 };
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indicesTopFace); //glDrawElements performs indexed rendering

	//Make bottom face pink
	glVertexAttrib4f(1, 1.0, 0.0, 1.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, 0.0, -1.0, 0.0);  //normal is attribute location 2
	unsigned int indicesBottomFace[4] = { 1, 7, 3, 5 };
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indicesBottomFace); //glDrawElements performs indexed rendering
}