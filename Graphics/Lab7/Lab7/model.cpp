//FileName:		model.cpp
//Programmer:	Dan Cliburn
//Date:			4/1/2015
//Purpose:		Defines the methods for the Model class.
//The init() method needs to set up OpenGL and GLEW and prepare all objects to be rendered.
//The draw() method needs to render all objects in the scene.

#include <glew.h> //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "model.h"
#include "LoadShaders.h"
#include <iostream>
using namespace std;
using namespace glm;

Model::Model()
{
	PVM_matrixLoc = 0;
	model_matrix = mat4(1.0);
	view_matrix = mat4(1.0);
	projection_matrix = mat4(1.0);
}

//init() does all of the OpenGL and GLEW initialization.  
//You also want to call the init() methods in here for any of your other classes that use OpenGL 
bool Model::init()
{
	//Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
		return false;
	}

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);

	//The following code was adapted from the OpenGL 4.0 Shading Language Cookbook, by David Wolff
	//to provide information about the hardware and supported versions of OpenGL and GLSL. 
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "GL Vendor: " << vendor << endl;
	cout << "GL Renderer: " << renderer << endl;
	cout << "GL Version: " << version << endl;
	cout << "GLSL Version: " << glslVersion << endl << endl;

	//Finally, we need to load the shaders. I am using the LoadShaders() function written  
	//by the authors of the OpenGL Programming Guide (8th edition).
	ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "shader.vert" },
			{ GL_FRAGMENT_SHADER, "shader.frag" },
			{ GL_NONE, NULL }
	};
	GLuint program;
	if ((program = LoadShaders(shaders)) == 0)
	{
		cout << "Error Loading Shaders" << endl;
		return false;
	}
	glUseProgram(program);

	//Now initialize all of our models
	GLfloat green[4] = { 1.0, 1.0, 1.0, 1.0 };
	top.init("images/top.bmp", green);
	bottom.init("images/bottom.bmp", green);
	body.init("images/body.bmp", green);
	prop.init("images/propellerMid.bmp", green);
	propBlade.init("images/propeller.bmp", green);
	boards.init("images/boards.bmp");

	//Find the location of the PVM_matrix in the shader
	PVM_matrixLoc = glGetUniformLocation(program, "PVM_matrix");

	//Since the projection and view orientation matrices will not change during the program we will calculate them here
	projection_matrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.1, 20.0);
	vec3 eye = vec3(5, 5, 5);
	vec3 aim = vec3(0.0, 0.0, 0.0);
	vec3 up = vec3(0.0, 1.0, 0.0);
	view_matrix = lookAt(eye, aim, up);
	scale = 1.0f;
	count = 2;
	rotation = 0*(3.14159/180.0);
	cout << "Use Q and E to zoom in and out and press SPACE to toggle the windmill animation" << endl;
	return true;  //Everything got initialized
}

//draw() explains how to render your model
void Model::draw(float xAngle, float yAngle)
{
	mat4 PVMmatrix;
	if (frameCount % 2 == 0 && trigger) {
		rotation += 5*(3.14159/180.0);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw the center of the propeller
	currentTexture = &prop;
	resetMatrix(xAngle, yAngle);
	model_matrix = rotate(model_matrix, rotation, vec3(0.0f, 0.0f, 1.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, 0.0f, -2.0f));
	model_matrix = translate(model_matrix, vec3(0, 0, 0));
	drawCube();

	//draw the center part of the roof of the windmill
	currentTexture = &boards;
	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, 2.0f, -2.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, 2.0f, -4.0f));
	drawCube();
	//draw the top layer of the windmill
	currentTexture = &top;
	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, 0.0f, -2.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, 0.0f, -4.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, 0.0f, -2.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, 0.0f, -4.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, 0.0f, -4.0f));
	drawCube();

	
	//draw the body of the windmill
	currentTexture = &body;
	for (int i = 1; i < 4; i++) {
		resetMatrix(xAngle, yAngle);
		model_matrix = translate(model_matrix, vec3(0.0f, i*-2.0f, -2.0f));
		drawCube();
	}

	for (int i = 1; i < 4; i++) {
		resetMatrix(xAngle, yAngle);
		model_matrix = translate(model_matrix, vec3(0.0f, i*-2.0f, -4.0f));
		drawCube();
	}

	for (int i = 1; i < 4; i++) {
		resetMatrix(xAngle, yAngle);
		model_matrix = translate(model_matrix, vec3(2.0f, i*-2.0f, -2.0f));
		drawCube();
	}
	for (int i = 1; i < 4; i++) {
		resetMatrix(xAngle, yAngle);
		model_matrix = translate(model_matrix, vec3(2.0f, i*-2.0f, -4.0f));
		drawCube();
	}

	for (int i = 1; i < 4; i++) {
		resetMatrix(xAngle, yAngle);
		model_matrix = translate(model_matrix, vec3(-2.0f, i*-2.0f, -2.0f));
		drawCube();
	}

	for (int i = 1; i < 4; i++) {
		resetMatrix(xAngle, yAngle);
		model_matrix = translate(model_matrix, vec3(-2.0f, i*-2.0f, -4.0f));
		drawCube();
	}

	//draw the bottom of the windmill
	currentTexture = &bottom;
	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, -8.0f, -2.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, -8.0f, -2.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, -8.0f, 0.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, -8.0f, -2.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, -8.0f, -4.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, -8.0f, -4.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, -8.0f, -4.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(0.0f, -8.0f, 0.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, -8.0f, 0.0f));
	drawCube();


	//draw the propeller blades
	currentTexture = &propBlade;
	resetMatrix(xAngle, yAngle);
	model_matrix = rotate(model_matrix, rotation, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = translate(model_matrix, vec3(0.0f, 2.0f, 0.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = rotate(model_matrix, rotation, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = translate(model_matrix, vec3(0.0f, 4.0f, 0.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = rotate(model_matrix, rotation, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = translate(model_matrix, vec3(0.0f, -2.0f, 0.0f));
	drawCube();

	resetMatrix(xAngle, yAngle);
	model_matrix = rotate(model_matrix, rotation, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = translate(model_matrix, vec3(0.0f, -4.0f, 0.0f));
	drawCube();

	//draw the red part of the roof of the windmill
	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, 2.0f, -3.0f));
	model_matrix = rotate(model_matrix, (float)(90*(3.14159 / 180.0)), vec3(0.0f, 1.0f, 0.0f));
	model_matrix = rotate(model_matrix, (float)(45*(3.14159 / 180.0)), vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-2.0f, 2.0f, -5.0f));
	model_matrix = rotate(model_matrix, (float)(90*(3.14159 / 180.0)), vec3(0.0f, 1.0f, 0.0f));
	model_matrix = rotate(model_matrix, (float)(45*(3.14159 / 180.0)), vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, 2.0f, -3.0f));
	model_matrix = rotate(model_matrix, (float)(90*(3.14159 / 180.0)), vec3(0.0f, 1.0f, 0.0f));
	model_matrix = rotate(model_matrix, (float)(-45*(3.14159 / 180.0)), vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(2.0f, 2.0f, -5.0f));
	model_matrix = rotate(model_matrix, (float)(90*(3.14159 / 180.0)), vec3(0.0f, 1.0f, 0.0f));
	model_matrix = rotate(model_matrix, (float)(-45*(3.14159 / 180.0)), vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(1.29f, 1.29f, -2.1f));
	model_matrix = rotate(model_matrix, (float)(45*(3.14159 / 180.0)), vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(1.29f, 1.29f, -5.9f));
	model_matrix = rotate(model_matrix, (float)(45*(3.14159 / 180.0)), vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-1.29f, 1.29f, -2.1f));
	model_matrix = rotate(model_matrix, (float)(-45*(3.14159 / 180.0)), vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	resetMatrix(xAngle, yAngle);
	model_matrix = translate(model_matrix, vec3(-1.29f, 1.29f, -5.9f));
	model_matrix = rotate(model_matrix, (float)(-45*(3.14159 / 180.0)), vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::scale(model_matrix, vec3(1.0f, 1.5f, 1.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	boards.draw();

	glFlush();
	frameCount++;
}

//reset the matrix to its "origin" position but with the required rotation and scaling
void Model::resetMatrix(float xAngle, float yAngle) {
	model_matrix = mat4(1.0f);
	model_matrix = rotate(model_matrix, (float)(yAngle*(3.14159 / 180.0)), vec3(0.0f, 1.0f, 0.0f));
	model_matrix = rotate(model_matrix, (float)(xAngle*(3.14159 / 180.0)), vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::scale(model_matrix, vec3(scale, scale, scale));
}

void Model::drawCube() {
	//Using a given position, draw a 2x2 cube of quads
	mat4 PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	currentTexture->draw();

	model_matrix = translate(model_matrix, vec3(0.0f, 0.0f, -2.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	currentTexture->draw();

	model_matrix = translate(model_matrix, vec3(1.0f, 0.0f, 1.0f));
	model_matrix = rotate(model_matrix, (float)(-90.0*(3.14159/180.0)), vec3(0.0f, 1.0f, 0.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	currentTexture->draw();

	model_matrix = translate(model_matrix, vec3(0.0f, 0.0f, 2.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	currentTexture->draw();

	model_matrix = translate(model_matrix, vec3(0.0f, 1.0f, -1.0f));
	model_matrix = rotate(model_matrix, (float)(-90.0*(3.14159/180.0)), vec3(1.0f, 0.0f, 0.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	currentTexture->draw();

	model_matrix = translate(model_matrix, vec3(0.0f, 0.0f, -2.0f));
	PVMmatrix = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(PVM_matrixLoc, 1, GL_FALSE, value_ptr(PVMmatrix));
	currentTexture->draw();
}