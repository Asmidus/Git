//This is a simple pass-through vertex shader used by the filter 

#version 400 core

layout(location = 0) in vec4 vertexPosition;

void main()
{
	gl_Position = vertexPosition;
}