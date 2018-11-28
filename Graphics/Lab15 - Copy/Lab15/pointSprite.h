//FileName:		pointSprite.h
//Programmer:	Dan Cliburn
//Date:			4/2/2015
//Purpose:		This file defines the header for the PointSprite class.

#ifndef POINTSPRITE_H
#define POINTSPRITE_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "transparentTexModel.h"
#include <string>
using namespace std;

class PointSprite : public TransparentTexModel
{
public:
	//We'll need to redefine how these two methods work for this class
	virtual bool init(string texFileName, const GLfloat *c = 0);
	virtual void draw();
};

#endif