#include "LoadObject.h"
using namespace std;

vector<string> LoadObject::explode(string aStr, char aDelim) {
	vector<string> res;
	string str = aStr.substr(0, aStr.find(aDelim));

	while (str.size() < aStr.size()) {
		res.push_back(str);
		aStr = aStr.substr(aStr.find(aDelim) + 1);
		str = aStr.substr(0, aStr.find(aDelim));
	}

	res.push_back(str);

	return res;
}

void LoadObject::loadOBJ(const string aInFilename,
	vector<glm::vec3>& finalVerts,
	vector<glm::vec3>& finalNorms,
	vector<glm::vec2>& finalUVs,
	vector<glm::vec2>& finalMatRanges,
	vector<glm::vec4>& finalMaterials,
	vector<string>& textures,
	bool aVerbose) {
	if (aVerbose) cout << "Loading OBJ file <"
		<< aInFilename << ">" << endl;
	// Open file
	ifstream objFile(aInFilename.c_str());

	if (objFile.fail()) {
		cout << "Error: could not open file <"
			<< aInFilename << ">" << endl;
		exit(1);
	}
	// Extract verts, normals, textures, and faces
	vector<float> verts, norms, uvs;
	vector<int> faces;
	map<string, int> faceHash;

	string line;
	string mtlFileName;
	int numFaces = 0;
	int hashIndex = 0;
	int prevIndex = 0;
	int numTextures = 0;

	if (aVerbose) cout << "Extracting values from file" << endl;

	// Visit each line of the obj file
	while (getline(objFile, line)) {
		// Extract vertex
		// Line starts with v[space]...
		if (line[0] == 'v' && line[1] == ' ') {
			string lineVals = line.substr(2);
			float val;

			string val0 = lineVals.substr(0, lineVals.find(' '));
			val = (float)atof(val0.c_str());
			verts.push_back(val);

			string val1 = lineVals.substr(val0.length() + 1,
				lineVals.find(' '));
			val = (float)atof(val1.c_str());
			verts.push_back(val);

			string val2 = lineVals.substr(lineVals.find_last_of(' ') + 1);
			val = (float)atof(val2.c_str());
			verts.push_back(val);
		}


		// Extract textures
		// Line starts with vt[space]...
		//else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
		//	string lineVals = line.substr(3);
		//	float val;

		//	string val0 = lineVals.substr(0, lineVals.find(' '));
		//	val = (float)atof(val0.c_str());
		//	textures.push_back(val);

		//	string val1 = lineVals.substr(val0.length() + 1,
		//		lineVals.find(' '));
		//	val = (float)atof(val1.c_str());
		//	textures.push_back(val);
		//}


		// Extract normals
		// Line starts with vn[space]...
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
			string lineVals = line.substr(3);
			float val;

			string val0 = lineVals.substr(0, lineVals.find(' '));
			val = (float)atof(val0.c_str());
			norms.push_back(val);

			string val1 = lineVals.substr(val0.length() + 1,
				lineVals.find(' '));
			val = (float)atof(val1.c_str());
			norms.push_back(val);

			string val2 = lineVals.substr(lineVals.find_last_of(' ') + 1);
			val = (float)atof(val2.c_str());
			norms.push_back(val);
		}

		else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
			string lineVals = line.substr(3);
			float val;

			string val0 = lineVals.substr(0, lineVals.find(' '));
			val = (float)atof(val0.c_str());
			uvs.push_back(val);

			string val1 = lineVals.substr(val0.length() + 1,
				lineVals.find(' '));
			val = (float)atof(val1.c_str());
			uvs.push_back(val);
		}

		else if (line[0] == 'f' && line[1] == ' ') {
			string lineVals = line.substr(2);

			string val0 = lineVals.substr(0, lineVals.find_first_of(' '));

			// If the value for this face includes texture and/or 
			// normal, parse them out
			if (val0.find('/') >= 0) {
				// Get first group of values
				string g1 = val0.substr(0, val0.find(' '));

				// Get second group of values
				string g2 = line.substr(line.find(' ') + 2);
				g2 = g2.substr(g2.find(' ') + 1);
				g2 = g2.substr(0, g2.find(' '));

				string g3 = line.substr(line.find_last_of(' ') + 1);

				if (aVerbose)
					cout << "Face: (" << g1 << ") (" << g2 << ") (" << g3 << ")" << endl;
				string str1 = g1.substr(0, g1.find_first_of('/'));
				int vert1 = (int)atof(str1.c_str());
				string str2 = g2.substr(0, g2.find_first_of('/'));
				int vert2 = (int)atof(str2.c_str());
				string str3 = g3.substr(0, g3.find_first_of('/'));
				int vert3 = (int)atof(str3.c_str());

				string uvStr1 = g1.substr(g1.find_first_of('/') + 1, g1.find_last_of('/') - g1.find_first_of('/') - 1);
				int uv1 = (int)atof(uvStr1.c_str());
				string uvStr2 = g2.substr(g2.find_first_of('/') + 1, g2.find_last_of('/') - g2.find_first_of('/') - 1);
				int uv2 = (int)atof(uvStr2.c_str());
				string uvStr3 = g3.substr(g3.find_first_of('/') + 1, g3.find_last_of('/') - g3.find_first_of('/') - 1);
				int uv3 = (int)atof(uvStr3.c_str());

				string str4 = g1.substr(g1.find_last_of('/') + 1, g1.length());
				int norm = (int)atof(str4.c_str());
				finalVerts.emplace_back(verts[(vert1 - 1)*3], verts[(vert1 - 1)*3+1], verts[(vert1 - 1)*3+2]);
				finalVerts.emplace_back(verts[(vert2 - 1)*3], verts[(vert2 - 1)*3+1], verts[(vert2 - 1)*3+2]);
				finalVerts.emplace_back(verts[(vert3 - 1)*3], verts[(vert3 - 1)*3+1], verts[(vert3 - 1)*3+2]);

				finalUVs.emplace_back(uvs[(uv1 - 1)*2], uvs[(uv1 - 1)*2+1]);
				finalUVs.emplace_back(uvs[(uv2 - 1)*2], uvs[(uv2 - 1)*2+1]);
				finalUVs.emplace_back(uvs[(uv3 - 1)*2], uvs[(uv3 - 1)*2+1]);
				for (int i = 0; i < 3; i++) {					//push back the x y z of the normal for each vertex added
					finalNorms.emplace_back(norms[(norm - 1)*3], norms[(norm - 1)*3+1], norms[(norm - 1)*3+2]);
				}
				numFaces++;
			}
		} else if (line.substr(0, 6) == "mtllib") {
			mtlFileName = line.substr(7, line.length());
		} else if (line.substr(0, 6) == "usemtl") {
			if (numTextures != 0) {
				prevIndex = finalVerts.size();
				finalMatRanges.back().y = prevIndex;
				finalMatRanges.emplace_back(prevIndex, 0);
			} else {
				finalMatRanges.emplace_back(prevIndex, 0);
			}
			numTextures++;
		}
	} /* end getline(file, line) */

	finalMatRanges.back().y = finalVerts.size();
	int x = 0;

	if (aVerbose) cout  << "Finished extracting values from file" << endl
		<< "Quick count check:" << endl
		<< "\tVerts = " << verts.size() << endl
		<< "\tNorms = " << norms.size() << endl
		<< "\tTexts = " << textures.size() << endl
		<< "\tFaces = " << faces.size() << endl;

	objFile.close();
	ifstream mtlFile(mtlFileName.c_str());
	while (getline(mtlFile, line)) {
		if (line.substr(0, 6) == "newmtl") {
			finalMaterials.emplace_back(0);
		}
		if (line.substr(0, 6) == "map_Kd") {
			//textures.push_back(line.substr(line.find_last_of('\\') + 1, line.length()));
			textures.insert(textures.begin(), line.substr(line.find_last_of('\\') + 1, line.length()));
		}
	}
}