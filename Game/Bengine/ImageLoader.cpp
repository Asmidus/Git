#include "stdafx.h"
#include "ImageLoader.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"
#include <vector>

namespace Bengine {
	GLTexture ImageLoader::loadPNG(std::string filePath) {
		GLTexture tex = {};
		unsigned long w, h, n;
		std::vector<unsigned char> out;
		std::vector<unsigned char> in;
		if (!IOManager::readFileToBuffer(filePath, in)) {
			fatalError("Failed to load PNG file to buffer");
		}
		int errorCode = decodePNG(out, w, h, &(in[0]), in.size());
		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + errorCode);
		}
		//unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &n, STBI_rgb_alpha);
		glGenTextures(1, &(tex.id));
		tex.width = w;
		tex.height = h;
		glBindTexture(GL_TEXTURE_2D, tex.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		return tex;
	}
}