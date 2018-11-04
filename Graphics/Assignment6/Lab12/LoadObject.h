#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class LoadObject {
public:
	static void loadOBJ(const string aInFilename,
		vector<float>& finalVerts,
		vector<float>& finalNorms,
		vector<float>& finalTextures,
		vector<int>& finalFaces,
		bool aVerbose = false);
	static vector<string> explode(string aStr, char aDelim);
};

