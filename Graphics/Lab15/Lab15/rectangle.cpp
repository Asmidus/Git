//FileName:		rectangle.cpp
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		To define a model that we want to render in our OpenGL program.

#include "rectangle.h"

void Rectangle::defineVertexPositions()
{
	float z = 0.0;
	//Vertex positions
	positions[0][0] = -0.5;  //x value for vertex 0
	positions[0][1] = 0.5;  //y value for vertex 0
	positions[0][2] = z; //z value for vertex 0
	positions[1][0] = -0.5;  //x value for vertex 1
	positions[1][1] = -0.5;  //y value for vertex 1
	positions[1][2] = z; //z value for vertex 1
	positions[2][0] = 0.5;  //x value for vertex 2
	positions[2][1] = 0.5;  //y value for vertex 2
	positions[2][2] = z; //z value for vertex 2
	positions[3][0] = 0.5;  //x value for vertex 3
	positions[3][1] = -0.5;  //y value for vertex 3
	positions[3][2] = z; //z value for vertex 3
}