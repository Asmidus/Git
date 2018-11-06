#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <glm.hpp>

using namespace std;

class LoadObject {
public:
	static void loadOBJ(const string aInFilename,
		vector<glm::vec3>& finalVerts,
		vector<glm::vec3>& finalNorms,
		vector<glm::vec2>& finalUVs,
		vector<glm::vec2>& finalMatRanges,
		vector<glm::vec4>& finalMaterials,
		vector<string>& textures,
		bool aVerbose = false);
	static vector<string> explode(string aStr, char aDelim);
};

