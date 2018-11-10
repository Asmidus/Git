//FileName:		multiTexModel.h
//Programmer:	Dan Cliburn
//Date:			4/2/2015
//Purpose:		This file defines the header for the MultiTexModel class.
//This class allows the programmer to specify two textures, the second of
//which will be blended onto the first.

#ifndef MULTITEXMODEL_H
#define MULTITEXMODEL_H

#include <glew.h>  //glew.h is supposed to be included before gl.h. To be safe, you can just include glew.h instead
#include "texmodel.h"
#include <string>
using namespace std;

class MultiTexModel : public TexModel
{
protected:
	GLuint texIDs[2];
	GLuint GLSLprogram;
	GLint tex1_uniform_Loc;
	GLint tex2_uniform_Loc;

public:
	MultiTexModel();

	//We'll need to redefine how these methods work in this class
	virtual void buildMipMaps();
	virtual bool init(string texFileName1, string texFileName2, GLuint program, const GLfloat *c = 0);
	virtual void initTextures(string texFileName, string texFileName2);
	virtual void draw();
};

#endif