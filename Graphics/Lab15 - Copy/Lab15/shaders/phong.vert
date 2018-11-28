//This vertex shader is based on Example 7.8 on pages 377 and 378 of the OpenGL Programming Guide (8th edition)
//with some tweaks to support shininess as a vertex property and to allow the eye position to move around the scene.

#version 420 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in float vertexShininess;
layout(location = 4) in vec2 vertexUV;
layout(location = 5) in vec3 emittance;

out vec4 position; //position of the vertex in "eye" coordinates
out vec4 color;
out vec3 normal; //orientation of the normal in "eye" coordinates
out float shininess;
out vec2 uv;
out vec3 fEmittance;

layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

void main()
{
	position = view_matrix * model_matrix * vertexPosition;
	color = vertexColor;
	vec4 n = view_matrix * model_matrix * vec4(vertexNormal, 0.0); //Assumes only isometric scaling of objects
	normal = normalize(vec3(n.x, n.y, n.z));
	shininess = vertexShininess;
	uv = vec2(vertexUV.x, 1.0 - vertexUV.y);
	fEmittance = emittance;
	gl_Position = projection_matrix * position;
}