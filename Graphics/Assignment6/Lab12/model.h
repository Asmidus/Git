//FileName:		model.h
//Programmer:	Dan Cliburn
//Date:			4/14/2015
//Purpose:		This file defines the header for the Model class.
//The "Model" is just what we want to render in our scene.
//The Model class is responsible for calling all of the OpenGL and GLEW functions.

#ifndef MODEL_H
#define MODEL_H

#include <glew.h> //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <glm.hpp>
#include "LightProperties.h"
#include "cube.h"
#include "Shape.h"
using namespace glm;

class Model
{
	private:
		GLint modelMatrixLoc;
		GLint viewMatrixLoc;
		GLint projectionMatrixLoc;
		mat4 model_matrix;
		mat4 view_matrix;
		mat4 projection_matrix;

		LightProperties lights[3];
		int totalLights;
		GLint totalLightsLoc;
		GLint globalAmbientLightLoc;
		GLuint lightsLoc;

		Shape c;
		float yAngle;
		float xAngle;

		float degreesToRadians(float degrees) { return (degrees * (3.14159 / 180.0)); }

	public:
		Model();

		bool init();  //initializes OpenGL, GLEW, and the objects we want to draw
		void update(float xAngle, float yAngle); //called whenever we want to update the Model object  
		void updateLights(int lightToggles[3]);
		void draw(); //renders the objects we want to draw
};

#endif