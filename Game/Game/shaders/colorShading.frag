#version 400 core

in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D texSampler;

void main() {
	vec4 textureColor = texture(texSampler, fragmentUV);
	color = textureColor * fragmentColor;
}