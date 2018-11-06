//This vertex shader extends our previous ones and implements Example 8.3 on pages 420 and 421 of the OpenGL 
//Programming Guide (8th edition) to build a procedural brick texture.  Note that to get the texture to orient 
//correctly on all sides of the cube I had to set "MCposition" based on the direction of the surface normal.

#version 420 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;

out vec4 pos; //position of the vertex in "eye" coordinates
out vec4 fColor;
out vec3 normal; //orientation of the normal in "eye" coordinates
out vec2 fragmentUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	pos = viewMatrix * modelMatrix * vertexPosition;
	fColor = vertexColor;
	vec4 n = viewMatrix * modelMatrix * vec4(vertexNormal, 0.0); //Assumes only isometric scaling
	normal = normalize(vec3(n.x, n.y, n.z));
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
	gl_Position = projectionMatrix * pos;
}