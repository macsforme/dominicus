/*
 * Mesh.h
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MESH_H
#define MESH_H

// program headers
#include "VectorMath.h"

// library headers
#include <map>
#include <string>
#include <vector>

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

	// constructors
	Mesh() { };

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
	void autoNormal() {
		normals.clear();

		std::map<unsigned int, std::vector<Vector3> > vertexNormals;

		for(
				std::map< std::string,std::vector<Face> >::iterator groupItr =
						faceGroups.begin();
				groupItr != faceGroups.end();
				++groupItr
			) {
			for(
					size_t index = 0;
					index < groupItr->second.size();
					++index
				) {
				Vector3 v1(
						vertices[groupItr->second[index].vertices[1]] -
						vertices[groupItr->second[index].vertices[0]]
					);
				Vector3 v2(
						vertices[groupItr->second[index].vertices[2]] -
						vertices[groupItr->second[index].vertices[1]]
					);

				Vector3 normal = cross(v1, v2);
				normal.norm();

				vertexNormals[groupItr->second[index].vertices[0]].push_back(normal);
				vertexNormals[groupItr->second[index].vertices[1]].push_back(normal);
				vertexNormals[groupItr->second[index].vertices[2]].push_back(normal);

				groupItr->second[index].normals[0] = groupItr->second[index].vertices[0];
				groupItr->second[index].normals[1] = groupItr->second[index].vertices[1];
				groupItr->second[index].normals[2] = groupItr->second[index].vertices[2];
			}
		}

		for(size_t i = 0; i < vertices.size(); ++i) {
			unsigned int count = 0;
			Vector3 total(0.0f, 0.0f, 0.0f);

			for(size_t p = 0; p < vertexNormals[i].size(); ++p) {
				++count;
				total += vertexNormals[i][p];
			}

			total /= count;
			normals.push_back(total);
		}
	}
	void autoTexCoord(unsigned int index, std::string group = "") {
		texCoords.push_back(Vector2(
				vertices[faceGroups[group][index].vertices[0]].x,
				vertices[faceGroups[group][index].vertices[0]].y
			));
		faceGroups[group][index].texCoords[0] = texCoords.size() - 1;

		texCoords.push_back(Vector2(
				vertices[faceGroups[group][index].vertices[1]].x,
				vertices[faceGroups[group][index].vertices[1]].y
			));
		faceGroups[group][index].texCoords[1] = texCoords.size() - 1;

		texCoords.push_back(Vector2(
				vertices[faceGroups[group][index].vertices[2]].x,
				vertices[faceGroups[group][index].vertices[2]].y
			));
		faceGroups[group][index].texCoords[2] = texCoords.size() - 1;
	}
};

#endif
