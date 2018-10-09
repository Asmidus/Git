#version 400 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 ortho;
uniform float offsetX;
uniform float offsetY;

void main() {
	gl_Position.xy = (ortho * vec4(vertexPosition.x + offsetX, vertexPosition.y + offsetY, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}