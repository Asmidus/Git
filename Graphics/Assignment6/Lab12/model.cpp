//FileName:		model.cpp
//Programmer:	Dan Cliburn
//Date:			4/14/2015
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
	modelMatrixLoc = 0;
	viewMatrixLoc = 0;
	yAngle = xAngle = 0.0;
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

	glClearColor(0.8, 0.8, 0.8, 1.0);
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
	cout << "GLSL Version: " << glslVersion << endl;

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
	glUniform1i(glGetUniformLocation(program, "texSampler"), 0);
	glUseProgram(program);

	//Now initialize all of our models
	c.init();

	//Now, set up the lights for the scene
	totalLights = 1;
	lights[0].color = vec4(1.0, 1.0, 1.0, 1.0);
	lights[0].position = vec4(0.0, 0.0, 4.0, 1.0); //positional light since w = 1
	//lights[0].spotLightValues = vec4(1.0, 0.99, 4.0, 0.0);
	//If the first parameter to spotLightValues is > 0, then this is a spotlight
	//The second parameter to spotLightValues is the Spot Cosine Cutoff
	//The third parameter to spotLightValues is the Spotlight Exponent
	//The fourth parameter to spotLightValues is unused 
	//lights[0].spotConeDirection = vec4(0.0, 0.0, -1.0, 0.0);
	lights[0].constantAttenuation = 1.0;
	lights[0].linearAttenuation = 0.0;
	lights[0].quadraticAttenuation = 0.0;
	lights[0].isEnabled = 1;

	lights[1].color = vec4(1.0, 1.0, 1.0, 1.0);
	lights[1].position = vec4(0.0, 1.0, 0.0, 0.0);  //directional light since w = 0
	//No attenuation for directional lights so we don't need to set those parameters
	lights[1].isEnabled = 1;

	lights[2].color = vec4(1.0, 1.0, 1.0, 1.0);
	lights[2].position = vec4(1.5, 0.0, 0.0, 1.0);  //positional light since w = 1
	lights[2].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0); //Not a spotlight since first parameter is 0.0
	lights[2].constantAttenuation = 1.0;
	lights[2].linearAttenuation = 0.0;
	lights[2].quadraticAttenuation = 0.0;
	lights[2].isEnabled = 1;

	//Pass the light info to the shaders
	globalAmbientLightLoc = glGetUniformLocation(program, "globalAmbientLight");
	totalLightsLoc = glGetUniformLocation(program, "totalLights");
	glUniform3f(globalAmbientLightLoc, 0.2, 0.2, 0.2);  //minimum light level in the scene
	glUniform1i(totalLightsLoc, totalLights);
	//Set up the Uniform Buffer Object for the lights
	glGenBuffers(1, &lightsLoc);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsLoc);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsLoc);

	//Get the locations of the matrices
	modelMatrixLoc = glGetUniformLocation(program, "modelMatrix");
	viewMatrixLoc = glGetUniformLocation(program, "viewMatrix");
	projectionMatrixLoc = glGetUniformLocation(program, "projectionMatrix");
	//Since the projection and view orientation matrices will not change during the program we will calculate them here
	vec3 eye = vec3(2.5, 2.5, 2.5);
	vec3 aim = vec3(0.0, 0.0, 0.0);
	vec3 up = vec3(0.0, 1.0, 0.0);
	view_matrix = lookAt(eye, aim, up);
	projection_matrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.1, 20.0);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, value_ptr(view_matrix));
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, value_ptr(projection_matrix));

	return true;  //Everything got initialized
}

void Model::update(float x, float y)
{
	xAngle = x;
	yAngle = y;
}

void Model::updateLights(int lightToggles[3])
{
	for (int l = 0; l < 3; l++) lights[l].isEnabled = lightToggles[l];
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsLoc);
}

//draw() explains how to render your model
void Model::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculate the rotation based on the xAngle and yAngle variables computed in handleEvents
	vec3 xAxis(1.0, 0.0, 0.0);
	vec3 yAxis(0.0, 1.0, 0.0);
	model_matrix = rotate(mat4(1.0), degreesToRadians(yAngle), yAxis);
	model_matrix = rotate(model_matrix, degreesToRadians(xAngle), xAxis);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, value_ptr(model_matrix));

	c.draw();

	glFlush();
}