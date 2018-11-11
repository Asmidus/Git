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
	lightInfo.totalLights = 4;
	lightInfo.globalAmbientLight = vec3(0.3, 0.3, 0.3);

	lightInfo.lights[0].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[0].position = vec4(0.0, 0.0, -4.0, 1.0);
	//lightInfo.lights[0].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[0].constantAttenuation = 2.0;
	lightInfo.lights[0].linearAttenuation = 0.0;
	lightInfo.lights[0].quadraticAttenuation = 0.0;
	lightInfo.lights[0].isEnabled = 1;

	lightInfo.lights[1].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[1].position = vec4(0.0, 3.0, 0.0, 1.0);  //positional light since w = 1
	//lightInfo.lights[1].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[1].constantAttenuation = 2.0;
	lightInfo.lights[1].linearAttenuation = 0.0;
	lightInfo.lights[1].quadraticAttenuation = 0.0;
	lightInfo.lights[1].isEnabled = 1;

	lightInfo.lights[2].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[2].position = vec4(5.0, 2.5, 0.0, 1.0);  //positional light since w = 1
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

void Model::setUpFilter() //Create the textures that are used by the filter
{
	//Create a texture that the shaders can write to to hold the texture for the filter
	//glGenTextures(1, &filterTexID);
	//glBindTexture(GL_TEXTURE_2D, filterTexID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 1600, 800, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//Create a texture that holds the values (in this case all zeros) for clearing the previous texture
	//glGenBuffers(1, &clearFilterTexID);
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearFilterTexID);
	//glBufferData(GL_PIXEL_UNPACK_BUFFER, 1600 * 800 * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	//vec4 *data = (vec4 *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	//memset(data, 0x00, 1600 * 800 * sizeof(GLuint));
	//glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	//Bind the texture to an image unit (in this case 0) so that the shaders can access it
	//glBindImageTexture(0, filterTexID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
}

void Model::setUpBrickTex()
{
	//GLuint program2 = brick.getGLSLProgram();
	//glUseProgram(program2);
	////Find and set the uniform shader variables for the brick's shaders (shaders2)
	//GLint shaderLoc = glGetUniformLocation(program2, "BrickColor");
	//glUniform3f(shaderLoc, 0.3, 0.1, 0.1);
	//shaderLoc = glGetUniformLocation(program2, "MortarColor");
	//glUniform3f(shaderLoc, 0.5, 0.5, 0.5);
	//shaderLoc = glGetUniformLocation(program2, "BrickSize");
	//glUniform2f(shaderLoc, 0.5, 0.2);
	//shaderLoc = glGetUniformLocation(program2, "BrickPct");
	//glUniform2f(shaderLoc, .95, .95);
	////The following code creates a "noise" texture to send to the fragment shader of the brick
	//const int TexWidth = 400, TexHeight = 400;
	//unsigned char NoiseTex[TexHeight][TexWidth][4];
	//for (int r = 0; r < TexHeight; r++)
	//{
	//	for (int c = 0; c < TexWidth; c++)
	//	{
	//		for (int i = 0; i < 4; i++)
	//		{
	//			NoiseTex[r][c][i] = (unsigned char)(rand() % 128) + 128;
	//		}
	//	}
	//}
	//glGenTextures(1, &brickNoiseTexID);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, brickNoiseTexID);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TexWidth, TexHeight);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TexWidth, TexHeight, GL_RGBA, GL_UNSIGNED_BYTE, NoiseTex);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	//Load the shaders using the LoadShaders() function written by the authors of the OpenGL Programming Guide
	//ShaderInfo shaders1[] = {
	//		{ GL_VERTEX_SHADER, "shaders/light_tex.vert" },
	//		{ GL_FRAGMENT_SHADER, "shaders/light_tex.frag" },
	//		{ GL_NONE, NULL }
	//};
	//if ((program = LoadShaders(shaders1)) == 0)
	//{
	//	cout << "Error Loading Shaders" << endl;
	//	return false;
	//}
	//glUseProgram(program);

	////Find the locations of the uniform variables in shaders1 (the primary shader)
	//numTexLoc = glGetUniformLocation(program, "numTextures");	

	////Now initialize all of our models that use shaders1. Note that I have moved this a little 
	////later in init() from previous labs so that we can pass "program" to the MultiTexModel object.
	//tree.init("images/tree.bmp");
	//aWindow.init("images/glacier.bmp", "images/frame.bmp", program);
	//p.init("images/star.bmp");
	//
	////Set up the brick object to use its own shaders
	//ShaderInfo shaders2[] = {
	//		{ GL_VERTEX_SHADER, "shaders//brick.vert" },
	//		{ GL_FRAGMENT_SHADER, "shaders//brick.frag" },
	//		{ GL_NONE, NULL }
	//};
	//brick.init(shaders2);
	//setUpBrickTex();

	////Set up the wall object to use the toon shaders (shaders3)
	//ShaderInfo shaders3[] = {
	//		{ GL_VERTEX_SHADER, "shaders/toon.vert" },
	//		{ GL_FRAGMENT_SHADER, "shaders/toon.frag" },
	//		{ GL_NONE, NULL }
	//};
	//GLfloat wall_color[4] = { 0.4, 0.4, 0.4, 0.5 }; //The wall will be translucent so make alpha 0.5
	//wall.init(shaders3);
	//((Quad *)wall.getObject())->defineVerticeColor(wall_color);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //The wall will be translucent so define a blending function

	////Set up the ground and cube objects to use the regular phong shaders (shaders4)
	//ShaderInfo shaders4[] = {
	//		{ GL_VERTEX_SHADER, "shaders/phong.vert" },
	//		{ GL_FRAGMENT_SHADER, "shaders/phong.frag" },
	//		{ GL_NONE, NULL }
	//};
	//GLfloat ground_color[4] = { 0.1, 0.15, 0.1, 1.0 };
	//ground.init(shaders4);
	//((Quad *)ground.getObject())->defineVerticeColor(ground_color);
	//cube.init(shaders4);

	//ShaderInfo shaders5[] =
	//{
	//	{ GL_VERTEX_SHADER, "shaders/filter.vert" },
	//	{ GL_FRAGMENT_SHADER, "shaders/filter.frag" },
	//	{ GL_NONE }
	//};
	//filter.init(shaders5);
	//setUpFilter();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/phong.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/phong.frag" },
		{ GL_NONE, NULL }
	};
	GLuint program;
	if ((program = LoadShaders(shaders)) == 0) {
		cout << "Error Loading Shaders" << endl;
		return false;
	}
	glUniform1i(glGetUniformLocation(program, "texSampler"), 0);
	glUseProgram(program);
	test.init("sceneTest2.obj");

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

	//if (filterToggle) //if the filter is on then clear it using the clear texture
	//{
	//	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearFilterTexID);
	//	glBindTexture(GL_TEXTURE_2D, filterTexID);
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1600, 800, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	//}
	//glUseProgram(program); //Make the base shader active (in this case it is the light_tex shader1)
	//glUniform1i(numTexLoc, 1); //set this uniform variable for the objects that have one texture
	//model_matrix = rotate(mat4(1.0), degreesToRadians(45.0f), vec3(0.0f, 1.0f, 0.0f)); //Position the tree
	//updateMatrices();
	//tree.draw();
	//model_matrix = rotate(mat4(1.0), degreesToRadians(135.0f), vec3(0.0f, 1.0f, 0.0f)); //Position the tree
	//updateMatrices();
	//tree.draw();

	//glUniform1i(numTexLoc, 2); //set this uniform variable for the objects that have two textures
	//model_matrix = translate(mat4(1.0), vec3(-5.0f, 0.5f, 5.0f)); //Position the window
	//model_matrix = rotate(model_matrix, degreesToRadians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	//updateMatrices();
	//aWindow.draw();

	//glUniform1i(numTexLoc, 3); //set this uniform variable for the PointSprite objects
	//model_matrix = translate(mat4(1.0), vec3(5.0f, 0.5f, 5.0f)); //Position the sprite
	//updateMatrices();
	//p.draw();

	////Render the objects that use their own shaders
	//glBindTexture(GL_TEXTURE_2D, brickNoiseTexID); //Need to make the noise texture active for the brick
	model_matrix = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.0f)); //position the brick
	updateMatrices();
	test.draw();
	//brick.draw();

	//model_matrix = translate(mat4(1.0), vec3(0.0, -1.0, 0.0)); //position the ground
	//model_matrix = rotate(model_matrix, degreesToRadians(-90.0f), vec3(1.0f, 0.0f, 0.0f)); //Make sure we rotate so that the normal is up!
	//model_matrix = scale(model_matrix, vec3(10.0f, 10.0f, 10.0f));
	//updateMatrices();
	//ground.draw();

	//model_matrix = translate(mat4(1.0), vec3(-4.0f, 1.5f, -4.0f));  //position the cube
	//model_matrix = rotate(model_matrix, degreesToRadians(cube_rot_angle), vec3(0.0f, 1.0f, 0.0f));
	//updateMatrices();
	//cube.draw();

	//model_matrix = translate(mat4(1.0), vec3(0.0f, 4.0f, 2.0f)); //position the wall
	//model_matrix = rotate(model_matrix, degreesToRadians(180.0f), vec3(0.0f, 1.0f, 0.0f)); //Make sure we rotate so that the normal is facing the viewer!
	//model_matrix = scale(model_matrix, vec3(10.0f, 5.0f, 1.0f));
	//updateMatrices();
	//glEnable(GL_BLEND); //The wall will be translucent so enable blending around it
	//wall.draw();
	//glDisable(GL_BLEND);

	//if (filterToggle)  //render the filter if it is on
	//{
	//	glDisable(GL_DEPTH_TEST); //Disabling the depth test ensures that the filter is rendered over the top of the other objects 
	//	filter.draw();
	//	glEnable(GL_DEPTH_TEST);
	//}

	glFlush();
}

void Model::toggleFilter()
{
	if (filterToggle == true)
		filterToggle = false;
	else
		filterToggle = true;
}