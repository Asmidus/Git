#pragma once
#include <string>
#include <glew.h>

struct GLTexture {
	GLuint id;
	int width;
	int height;
};

class ImageLoader {
public:
	static GLTexture loadPNG(std::string filePath);
};