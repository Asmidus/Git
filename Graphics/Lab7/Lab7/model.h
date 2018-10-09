//FileName:		model.h
//Programmer:	Dan Cliburn
//Date:			4/1/2015
//Purpose:		This file defines the header for the Model class.
//The "Model" is just what we want to render in our scene.
//The Model class is responsible for calling all of the OpenGL and GLEW functions.

#ifndef MODEL_H
#define MODEL_H

#include <glew.h> //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <glm.hpp>
#include "texmodel.h"
using namespace glm;

class Model
{
	private:
		GLint PVM_matrixLoc;
		mat4 model_matrix;
		mat4 view_matrix;
		mat4 projection_matrix;

		TexModel bottom;
		TexModel top;
		TexModel prop;
		TexModel propBlade;
		TexModel body;
		TexModel boards;
		TexModel* currentTexture;
		float scale;

		int animationStage;
		int count;

		int frameCount;

	public:
		Model();

		bool init();  //initializes OpenGL, GLEW, and the objects we want to draw
		void draw(float xAngle, float yAngle);  //renders the objects we want to draw
		void resetMatrix(float xAngle, float yAngle);
		void drawCube();
		void updateAnimation();
		void setScale(float newScale) { scale = newScale; }
		float getScale() { return scale; }

		bool trigger;
		float xOffset;
		float yOffset;
		float xOffset2;
		float yOffset2;
		float rotation;
};

#endif