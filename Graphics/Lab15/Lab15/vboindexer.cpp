#include <vector>
#include <map>

#include "vboindexer.hpp"

#include <string.h> // for memcmp

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2){
	return fabs( v1-v2 ) < 0.01f;
}

struct PackedVertex{
	glm::vec3 position;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex,unsigned short> & VertexToOutIndex,
	unsigned short & result
){
	std::map<PackedVertex,unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if ( it == VertexToOutIndex.end() ){
		return false;
	}else{
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec2> & in_uvs,
	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec2> & out_uvs
){
	std::map<PackedVertex,unsigned short> VertexToOutIndex;

	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ ){

		PackedVertex packed = { in_vertices[i]/*, in_normals[i] */};
		

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			//(u.v)/( |u| |v|)
			float dot = glm::dot(out_normals[index], in_normals[i]);
			float magnitude = glm::length(out_normals[index])*glm::length(in_normals[i]);
			float angle = glm::acos(dot/magnitude)*(180/3.14159);
			if (angle < 30) {
				glm::vec3 newNorm = (out_normals[index]+in_normals[i])/2.0f;
				out_normals[index] = newNorm;
				out_indices.push_back(index);
				//out_normals.push_back(newNorm);
			} else {
				out_normals.push_back(in_normals[i]);
				out_vertices.push_back(in_vertices[i]);
				out_uvs.push_back(in_uvs[i]);
				unsigned short newindex = (unsigned short)out_vertices.size() - 1;
				out_indices.push_back(newindex);
				VertexToOutIndex[packed] = newindex;
			}
		}else{ // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_normals .push_back(in_normals[i]);
			out_uvs		.push_back(in_uvs[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices .push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}