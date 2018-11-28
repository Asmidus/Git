#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

TextureCache::TextureCache() {}


TextureCache::~TextureCache() {}

GLTexture TextureCache::getTexture(std::string texturePath) {
	auto mit = _textureMap.find(texturePath);						//search the map for the texture
	if (mit == _textureMap.end()) {									//if it does not yet exist
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);	//load the image
		_textureMap.insert(make_pair(texturePath, newTexture));		//insert it into the map
		std::cout << "Loaded texture" << std::endl;
		return newTexture;
	}
	//std::cout << "Used cached texture" << std::endl;
	return mit->second;
}