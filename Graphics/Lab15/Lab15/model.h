//FileName:		Model.h
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		This file defines the header for the WorldModel class.
//This Model class differs from ones in previous labs in that it is responsible for loading the shaders and 
//rendering all of the objects in their proper places. Basically, OpenGL and GLEW stuff goes in this class.
//SDL goes in the Viewcontroller. The Viewcontroller then passes the view_matrix to the draw() method of this class.

#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <glew.h> //glew.h is supposed to be included before gl.h. To be safe, you can just include glew.h instead
#include <glm.hpp>
#include "quad.h"
#include "transparentTexModel.h"
#include "multiTexModel.h"
#include "pointSprite.h"
#include "shaderModel.h"
#include "cube.h"
#include "rectangle.h"
#include "Shape.h"
using namespace glm;

class Model
{
protected:
	bool initialized;

	GLuint program;

	//Variables for matrix manipulations
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
	GLuint matrixLoc;

	//Define the objects we want to render in our scene
	TransparentTexModel tree;
	MultiTexModel aWindow;
	PointSprite p;
	ShaderModel <Quad> ground;
	ShaderModel <Quad> wall;
	ShaderModel <Cube> brick;
	ShaderModel <Cube> cube;
	ShaderModel <Rectangle> filter;

	Shape test;
	Shape light;

	GLuint brickNoiseTexID;
	GLint numTexLoc;

	float cube_rot_angle;

	//Variables used to control the overdraw filter
	bool filterToggle;
	GLuint filterTexID;
	GLuint clearFilterTexID;

	float degreesToRadians(float degrees) { return (degrees * (3.14159 / 180.0)); }

public:
	Model();

	void setUpLights();
	void setUpBrickTex();
	void setUpFilter();
	void setViewMatrix(const mat4 &view_matrixIN) { view_matrix = view_matrixIN; }
	void toggleFilter();

	void updateMatrices();
	void updateWorld();

	bool initGLEW();
	bool init();  //initializes the model
	void draw();  //renders the model
};

#endif