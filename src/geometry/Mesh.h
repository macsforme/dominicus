// Mesh.h
// Crucible Island

#ifndef MESH_H
#define MESH_H

#include <map>
#include <string>
#include <vector>

#include "math/VectorMath.h"

class Mesh {
public:
	struct Face {
		unsigned int vertices[3];
		unsigned int normals[3];
		unsigned int texCoords[3];
	};

	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	std::map< std::string,std::vector<Face> > faceGroups;

	// vertex manipulation
	void addVertex(Vector3 vertex) { vertices.push_back(vertex); }
	void changeVertex(unsigned int index, Vector3 vertex) {
		vertices[index].x = vertex.x;
		vertices[index].y = vertex.y;
		vertices[index].z = vertex.z;
	}
	void changeVertex(unsigned int index, float x, float y, float z) {
		vertices[index].x = x;
		vertices[index].y = y;
		vertices[index].z = z;
	}

	// normal manipulation
	void addNormal(Vector3 normal) { normals.push_back(normal); }
	void changeNormal(unsigned int index, Vector3 normal) {
		normals[index].x = normal.x;
		normals[index].y = normal.y;
		normals[index].z = normal.z;
	}
	void changeNormal(unsigned int index, float x, float y, float z) {
		normals[index].x = x;
		normals[index].y = y;
		normals[index].z = z;
	}

	// texture coordinate manipulation
	void addTexCoord(Vector2 texCoord) { texCoords.push_back(texCoord); }
	void changeTexCoord(unsigned int index, Vector2 texCoord) {
		texCoords[index].x = texCoord.x;
		texCoords[index].y = texCoord.y;
	}
	void changeTexCoord(unsigned int index, float x, float y) {
		texCoords[index].x = x;
		texCoords[index].y = y;
	}

	// face manipulation
	void addFace(
			unsigned int vert1Index,
			unsigned int vert2Index,
			unsigned int vert3Index,
			std::string group = ""
		) {
		Face face;

		face.vertices[0] = vert1Index;
		face.vertices[1] = vert2Index;
		face.vertices[2] = vert3Index;

		faceGroups[group].push_back(face);
	}
	void addFace(
			unsigned int vert1Index,
			unsigned int vert2Index,
			unsigned int vert3Index,
			unsigned int norm1Index,
			unsigned int norm2Index,
			unsigned int norm3Index,
			std::string group = ""
		) {
		Face face;

		face.vertices[0] = vert1Index;
		face.vertices[1] = vert2Index;
		face.vertices[2] = vert3Index;

		face.normals[0] = norm1Index;
		face.normals[1] = norm2Index;
		face.normals[2] = norm3Index;

		faceGroups[group].push_back(face);
	}
	void addFace(
			unsigned int vert1Index,
			unsigned int vert2Index,
			unsigned int vert3Index,
			unsigned int norm1Index,
			unsigned int norm2Index,
			unsigned int norm3Index,
			unsigned int texCoord1Index,
			unsigned int texCoord2Index,
			unsigned int texCoord3Index,
			std::string group = ""
		) {
		Face face;

		face.vertices[0] = vert1Index;
		face.vertices[1] = vert2Index;
		face.vertices[2] = vert3Index;

		face.normals[0] = norm1Index;
		face.normals[1] = norm2Index;
		face.normals[2] = norm3Index;

		face.texCoords[0] = texCoord1Index;
		face.texCoords[1] = texCoord2Index;
		face.texCoords[2] = texCoord3Index;

		faceGroups[group].push_back(face);
	}

	// utility methods
	void autoNormal();
	void autoTexCoord(unsigned int index, std::string group);

	// constructors
	Mesh() { };
	Mesh(std::string filename);
};

#endif // MESH_H
