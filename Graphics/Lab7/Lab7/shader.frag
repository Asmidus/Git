//The texture mapping code is adapted from Example 6.7 in the OpenGL Programming Guide (8th edition)
#version 400 core

uniform sampler2D tex_image;

in vec4 color;
in vec2 vs_tex_coord;

out vec4 fColor;

void main()
{
	fColor = texture(tex_image, vs_tex_coord);
}