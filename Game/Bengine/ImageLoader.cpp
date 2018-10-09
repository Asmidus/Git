#include "stdafx.h"
#include "ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

namespace Bengine {
	GLTexture ImageLoader::loadPNG(std::string filePath) {
		GLTexture tex = {};
		int w, h, n;
		unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &n, STBI_rgb_alpha);
		glGenTextures(1, &(tex.id));
		tex.width = w;
		tex.height = h;
		glBindTexture(GL_TEXTURE_2D, tex.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		return tex;
	}
}