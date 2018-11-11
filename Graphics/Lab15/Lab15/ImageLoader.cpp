#include "ImageLoader.h"
#include "picoPNG.h"
#include <vector>
#include <fstream>

bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}
	file.seekg(0, std::ios::end);	//seek to the end of the file
	int fileSize = file.tellg();	//get the file size
	file.seekg(0, std::ios::beg);	//seek back to the beginning of the file
	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);
	file.close();
	return true;
}

GLTexture ImageLoader::loadPNG(std::string filePath) {
	GLTexture tex = {};
	unsigned long w, h, n;
	std::vector<unsigned char> out;
	std::vector<unsigned char> in;
	if (!readFileToBuffer(filePath, in)) {
	}
	int errorCode = decodePNG(out, w, h, &(in[0]), in.size());
	if (errorCode != 0) {
	}
	//unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &n, STBI_rgb_alpha);
	glGenTextures(1, &(tex.id));
	tex.width = w;
	tex.height = h;
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}