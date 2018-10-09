//The texture mapping code is adapted from Example 6.8 in the OpenGL Programming Guide (8th edition)
#version 400 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 in_tex_coord;  //texture coodinates are now being passed in as vertex attributes
layout(location = 2) in vec4 vertexColor;

out vec4 color;
out vec2 vs_tex_coord; //make a new out variable so that texture coordinate information can be available to the fragment shader 

uniform mat4 PVM_matrix;  //Projection, View Orientation, and Modeling matrices all concatentated together

void main()
{
	color = vertexColor;
	vs_tex_coord = in_tex_coord;  //copy the texture coordinate passed into this shader to an out variable

	gl_Position = PVM_matrix * vertexPosition;
}