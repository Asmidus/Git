#include "stdafx.h"
#include "IOManager.h"
#include <fstream>

namespace Bengine {
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
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
}