#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <glm.hpp>

using namespace std;

struct Material {
	string materialName;
	string texName;
	glm::vec4 Kd;
	glm::vec2 range;
};

class LoadObject {
public:
	static void loadOBJ(const string aInFilename,
		vector<glm::vec3>& finalVerts,
		vector<glm::vec3>& finalNorms,
		vector<glm::vec2>& finalUVs,
		vector<glm::vec2>& finalMatRanges,
		map<string, Material>& finalMaterials,
		vector<string>& textures,
		bool aVerbose = false);
	static vector<string> explode(string aStr, char aDelim);
};

