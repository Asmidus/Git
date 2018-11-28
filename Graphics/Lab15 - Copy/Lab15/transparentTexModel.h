//FileName:		transparentTexModel.h
//Programmer:	Dan Cliburn
//Date:			4/2/2015
//Purpose:		This file defines the header for the TransparentTexModel class.
//This class makes invisible any part of its texture that is completely black.

#ifndef TRANSTEXMODEL_H
#define TRANSTEXMODEL_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <string>
#include "texmodel.h"
using namespace std;

class TransparentTexModel : public TexModel
{
public:
	//We'll need to redefine how these two methods work in this class
	virtual void initTexture(string texFileName);
	virtual void draw();
};

#endif