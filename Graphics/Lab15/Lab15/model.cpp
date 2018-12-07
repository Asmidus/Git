//FileName:		model.cpp
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		Define the methods for the World Model class.
//The init() method needs to set up OpenGL and GLEW and prepare all objects (and their shaders) to be rendered.
//The draw() method positions and renders all objects in the scene and activates the appropriate shader(s).

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "LoadShaders.h"
#include "model.h"
#include "LightProperties.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace glm;
using namespace std;

Model::Model()
{
	filterToggle = false;
	cube_rot_angle = 0.0;
	model_matrix = mat4(1.0);
	view_matrix = mat4(1.0);
	projection_matrix = mat4(1.0);
}

bool Model::initGLEW()
{
	//Next initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
		return false;
	}

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

	return true;
}

void Model::setUpLights()
{
	//IMPORTANT - If you change this structure in any way you need to change it in all fragment shader(s) as well!!!
	struct Lights
	{
		LightProperties lights[4];
		vec3 globalAmbientLight;
		int totalLights;
	} lightInfo;

	//Now, set up the lights for the scene
	lightInfo.totalLights = 3;
	lightInfo.globalAmbientLight = vec3(0.2, 0.2, 0.2);

	lightInfo.lights[0].color = vec4(1, 0, 0, 1.0);
	lightInfo.lights[0].position = vec4(0.0, 2.0, 4.0, 1.0);
	//lightInfo.lights[0].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[0].constantAttenuation = 1.0;
	lightInfo.lights[0].linearAttenuation = 0.0;
	lightInfo.lights[0].quadraticAttenuation = 0.0;
	lightInfo.lights[0].isEnabled = 1;

	lightInfo.lights[1].color = vec4(0, 0, 1, 1.0);
	lightInfo.lights[1].position = vec4(0.0, 2.0, -4.0, 1.0);  //positional light since w = 1
	//lightInfo.lights[1].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[1].constantAttenuation = 2.0;
	lightInfo.lights[1].linearAttenuation = 0.0;
	lightInfo.lights[1].quadraticAttenuation = 0.0;
	lightInfo.lights[1].isEnabled = 1;

	lightInfo.lights[2].color = vec4(0, 1, 0, 1.0);
	lightInfo.lights[2].position = vec4(4, 2, 0, 1.0);  //positional light since w = 1
	//lightInfo.lights[2].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[2].constantAttenuation = 2.0;
	lightInfo.lights[2].linearAttenuation = 0.0;
	lightInfo.lights[2].quadraticAttenuation = 0.0;
	lightInfo.lights[2].isEnabled = 1;

	lightInfo.lights[3].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[3].position = vec4(3.5, 1.75, -3.5, 1.0);  //positional light since w = 1
	//lightInfo.lights[3].spotLightValues = vec4(1.0, 0.95, 4.0, 0.0);
		//If the first parameter to spotLightValues is > 0, then this is a spotlight
		//The second parameter to spotLightValues is the Spot Cosine Cutoff
		//The third parameter to spotLightValues is the Spotlight Exponent
		//The fourth parameter to spotLightValues is unused
	//lightInfo.lights[3].spotConeDirection = vec4(0.25, -1.0, -0.25, 0.0);
	lightInfo.lights[3].constantAttenuation = 0.5;
	lightInfo.lights[3].linearAttenuation = 0.0;
	lightInfo.lights[3].quadraticAttenuation = 0.0;
	lightInfo.lights[3].isEnabled = 0;

	//Pass the light info to the shaders in a Uniform Buffer Object.
	//This allows ALL shaders to be able to access the light information.
	GLuint lightsLoc;
	glGenBuffers(1, &lightsLoc);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsLoc);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lightInfo), &lightInfo, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsLoc); //The 0 needs to match the number used in the shaders for the lights
}

//This method updates the Matrices uniform buffer object with the values of the model, view, 
//and projection matrices making their values accessible to ALL of the shaders.
void Model::updateMatrices()
{
	//IMPORTANT - If you change this structure in any way you need to change it in all the shaders as well!!!
	struct Matrices
	{
		mat4 model_matrix;
		mat4 view_matrix;
		mat4 projection_matrix;
	} matrices;

	matrices.model_matrix = model_matrix;
	matrices.projection_matrix = projection_matrix;
	matrices.view_matrix = view_matrix;
	
	//Pass the matrix info to the shaders in a Uniform Buffer Object.
	//This allows ALL shaders to be able to access the matrix information.
	glBufferData(GL_UNIFORM_BUFFER, sizeof(matrices), &matrices, GL_DYNAMIC_DRAW);//use GL_DYNAMIC_DRAW since it changes a lot
	glBindBufferBase(GL_UNIFORM_BUFFER, 35, matrixLoc);  //The 35 needs to match the number used in the shaders for the matrices
}

bool Model::init()
{
	if (initGLEW() == false)
	{
		return false;
	}

	//Do some OpenGL initialization
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glEnable(GL_DEPTH_TEST);

	//player = new Audio();
	player.playMusic(Audio::background);
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/phong.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/phong.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaders1[] = {
		{ GL_VERTEX_SHADER, "shaders/toon.vert" },
	{ GL_FRAGMENT_SHADER, "shaders/toon.frag" },
	{ GL_NONE, NULL }
	};
	if ((phongShaders = LoadShaders(shaders)) == 0) {
		cout << "Error Loading Shaders" << endl;
		return false;
	}
	if ((toonShaders = LoadShaders(shaders1)) == 0) {
		cout << "Error Loading Shaders" << endl;
		return false;
	}
	glUniform1i(glGetUniformLocation(program, "texSampler"), 0);
	glUseProgram(phongShaders);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	test.init("models/untitled.obj");
	light.init("models/light.obj", false);
	pane.init("models/transparentplane.obj");
	sphere.init("models/sphere.obj", false);

	//Set up the uniform buffer objects that hold data that all of the shaders share. In this
	//application we have two uniform buffer objects: one for the lights and one for the matrices.
	setUpLights();  //The lights don't change as the program runs so we can set them here
	glGenBuffers(1, &matrixLoc);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixLoc);
	//Note that updateMatrices() gets called in draw() so we really do not need to call it here.
	//We call updateMatrices() in draw() since the matrices can change for every frame.

	//Since the projection matrix will not change during the program we can calculate it here
	projection_matrix = frustum(-0.2f, 0.2f, -0.1f, 0.1f, 0.1f, 100.0f);
	initialized = true;
	return true;
}

void Model::updateWorld()
{
	cube_rot_angle += 0.5;

	if (cube_rot_angle > 360.0) cube_rot_angle = 0.0;
}

void Model::draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a Model object before it has been initialized." << endl;
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model_matrix = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.0f)); //position the brick
	updateMatrices();
	test.draw();
	model_matrix = translate(mat4(1.0), vec3(0.0f, 2.0f, 4.0f)); //position the brick
	updateMatrices();
	//light.draw(glm::vec3(0.8, 0, 0));
	model_matrix = translate(mat4(1.0), vec3(0.0f, 2.0f, -4.0f));
	updateMatrices();
	//light.draw(glm::vec3(0, 0, 0.8));
	model_matrix = translate(mat4(1.0), vec3(4.0f, 2.0f, 0.0f));
	updateMatrices();
	//light.draw(glm::vec3(0, 0.8, 0));
	model_matrix = translate(mat4(1.0), vec3(-3.2, 1.5, 0));
	model_matrix = scale(model_matrix, vec3(1, 3, 5));
	updateMatrices();
	//pane.draw();
	model_matrix = translate(mat4(1.0), vec3(4.0f, -0.3f, 0.0f)); //position the brick
	updateMatrices();
	//sphere.draw();

	glFlush();
}

void Model::toggleFilter()
{
	if (filterToggle == true)
		filterToggle = false;
	else
		filterToggle = true;
}