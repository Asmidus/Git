//FileName:		rectangle.h
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		This file defines the header for a Rectangle class.

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "quad.h"

class Rectangle : public Quad
{
public:
	virtual void defineVertexPositions();
};

#endif