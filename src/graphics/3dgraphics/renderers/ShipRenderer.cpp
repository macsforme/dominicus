// ShipRenderer.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/3dgraphics/renderers/ShipRenderer.h"

ShipRenderer::ShipRenderer() {
	// dummy info since .obj indices start at 1
	shipMesh.addVertex(Vector3(0.0f, 0.0f, 0.0f));
	shipMesh.addNormal(Vector3(0.0f, 0.0f, 1.0f));
	shipMesh.addTexCoord(Vector2(0.0f, 0.0f));

	// load the model file from the .obj file
	std::ifstream objFile;
	std::string line;

	std::string groupName = "DEFAULT";

	objFile.open(std::string(platform->dataPath + "/data/models/ship.obj").c_str());
	if(! objFile.is_open())
		gameSystem->log(GameSystem::LOG_FATAL, "Unable to load ship model from file.");

	while(! objFile.eof()) {
		std::getline(objFile, line);

		if(line.length() > 0) {
			// parse the line
			std::string lineType = line.substr(0,line.find(" "));

			// for the valid lines we use, store the data
			if(lineType == "g") {
				// group definition
				groupName = line.substr(2, line.length() - 2);
			} else if(lineType == "s") {
				// smooth face

				// we ignore this
			} else if(lineType == "v") {
				// vertex definition
				float subjs[3] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f %f", &subjs[0], &subjs[1], &subjs[2]);

				Vector3 vertex;
				vertex.x = subjs[0] * 0.15f;
				vertex.y = subjs[1] * 0.15f;
				vertex.z = subjs[2] * 0.15f;

				shipMesh.vertices.push_back(vertex);
			} else if(lineType == "vt") {
				// texture coordinate
				float subjs[2] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f", &subjs[0], &subjs[1]);

				Vector2 texCoord;
				texCoord.x = subjs[0];
				texCoord.y = subjs[1];

				shipMesh.texCoords.push_back(texCoord);
			} else if(lineType == "vn") {
				// normals
				float subjs[3] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f %f", &subjs[0], &subjs[1], &subjs[2]);

				Vector3 normal;
				normal.x = subjs[0];
				normal.y = subjs[1];
				normal.z = subjs[2];

				shipMesh.normals.push_back(normal);
			} else if(lineType == "f") {
				// face definition
				unsigned int faceVertices[4];
				unsigned int faceNormals[4];
				unsigned int faceTexCoords[4];

				char subjs[4][20] = { };
				int numFaces = sscanf(line.c_str() + 2 * sizeof(char), "%s %s %s %s", subjs[0], subjs[1], subjs[2], subjs[3]);

				for(int i = 0; i < numFaces; ++i) {
					// process each vertex/textcoord/normal
					unsigned int vertex = 0;
					unsigned int texCoord = 0;
					unsigned int normal = 0;

					if(sscanf(subjs[i], "%u/%u/%u", &vertex, &texCoord, &normal) == 3) {
						// line has slashes and specifies all three
						if(
								vertex > shipMesh.vertices.size() ||
								normal > shipMesh.normals.size() ||
								texCoord > shipMesh.texCoords.size()
							)
							gameSystem->log(
									GameSystem::LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);
						faceVertices[i] = vertex;
						faceNormals[i] = normal;
						faceTexCoords[i] = texCoord;
					} else if(sscanf(subjs[i], "%u//%u", &vertex, &normal) == 2) {
						// line has slashes and specifies the vertex and normal
						if(
								vertex > shipMesh.vertices.size() ||
								normal > shipMesh.normals.size()
							)
							gameSystem->log(
									GameSystem::LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);

						faceVertices[i] = vertex;
						faceNormals[i] = normal;
					} else if(sscanf(subjs[i],"%u", &vertex)) {
						// line has just the vertex
						if(vertex > shipMesh.vertices.size())
								gameSystem->log(
										GameSystem::LOG_FATAL,
										"Model face specification has out-of-bounds index."
									);
						faceVertices[i] = vertex;
					} else {
							gameSystem->log(
									GameSystem::LOG_FATAL,
									"Model face specification has an invalid format."
								);
					}
				}

				// save it
				shipMesh.addFace(
						faceVertices[0],
						faceVertices[1],
						faceVertices[2],
						faceNormals[0],
						faceNormals[1],
						faceNormals[2],
						faceTexCoords[0],
						faceTexCoords[1],
						faceTexCoords[2],
						groupName
					);
				// add the other half of the quad as a second triangle, if necessary
				if(numFaces == 4)
					shipMesh.addFace(
							faceVertices[2],
							faceVertices[3],
							faceVertices[0],
							faceNormals[2],
							faceNormals[3],
							faceNormals[0],
							faceTexCoords[2],
							faceTexCoords[3],
							faceTexCoords[0],
							groupName
						);
			}
		}
	}

	objFile.close();

	// center propellers and determine offsets
	leftPropellerOrigin = Vector3(0.0f, 0.0f, 0.0f);
	rightPropellerOrigin = Vector3(0.0f, 0.0f, 0.0f);

	bool vertexAccounted[shipMesh.vertices.size()];
	bool vertexMoved[shipMesh.vertices.size()];

	unsigned int vertsAdded;
	Vector3 origin(0.0f, 0.0f, 0.0f);

	// adjust left propeller vertices
	for(unsigned int i = 0; i < shipMesh.vertices.size(); ++i) vertexAccounted[i] = false;
	vertsAdded = 0;

	for(size_t i = 0; i < shipMesh.faceGroups["left_propeller"].size(); ++i) {
		for(size_t p = 0; p < 3; ++p) {
			if(! vertexAccounted[shipMesh.faceGroups["left_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(shipMesh.vertices[shipMesh.faceGroups["left_propeller"][i].vertices[p]]);

				leftPropellerOrigin += *thisVertex;
				vertsAdded += 1;

				vertexAccounted[shipMesh.faceGroups["left_propeller"][i].vertices[p]] = true;
			}
		}
	}

	leftPropellerOrigin /= (float) vertsAdded;

	for(size_t i = 0; i < shipMesh.vertices.size(); ++i) vertexMoved[i] = false;

	for(size_t i = 0; i < shipMesh.faceGroups["left_propeller"].size(); ++i) {
		for(int p = 0; p < 3; ++p) {
			if(! vertexMoved[shipMesh.faceGroups["left_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(shipMesh.vertices[shipMesh.faceGroups["left_propeller"][i].vertices[p]]);
				*thisVertex -= leftPropellerOrigin;

				vertexMoved[shipMesh.faceGroups["left_propeller"][i].vertices[p]] = true;
			}
		}
	}

	// adjust right propeller vertices
	for(size_t i = 0; i < shipMesh.vertices.size(); ++i) vertexAccounted[i] = false;
	vertsAdded = 0;

	for(size_t i = 0; i < shipMesh.faceGroups["right_propeller"].size(); ++i) {
		for(unsigned int p = 0; p < 3; ++p) {
			if(! vertexAccounted[shipMesh.faceGroups["right_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(shipMesh.vertices[shipMesh.faceGroups["right_propeller"][i].vertices[p]]);

				rightPropellerOrigin += *thisVertex;
				vertsAdded += 1;

				vertexAccounted[shipMesh.faceGroups["right_propeller"][i].vertices[p]] = true;
			}
		}
	}

	rightPropellerOrigin /= (float) vertsAdded;

	for(size_t i = 0; i < shipMesh.vertices.size(); ++i) vertexMoved[i] = false;

	for(size_t i = 0; i < shipMesh.faceGroups["right_propeller"].size(); ++i) {
		for(unsigned int p = 0; p < 3; ++p) {
			if(! vertexMoved[shipMesh.faceGroups["right_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(shipMesh.vertices[shipMesh.faceGroups["right_propeller"][i].vertices[p]]);
				*thisVertex -= rightPropellerOrigin;

				vertexMoved[shipMesh.faceGroups["right_propeller"][i].vertices[p]] = true;
			}
		}
	}

	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["useTexture"] = glGetUniformLocation(shaderProgram, "useTexture");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");
	uniforms["useColor"] = glGetUniformLocation(shaderProgram, "useColor");
	uniforms["useLighting"] = glGetUniformLocation(shaderProgram, "useLighting");
	uniforms["lightPosition"] = glGetUniformLocation(shaderProgram, "lightPosition");
	uniforms["lightColor"] = glGetUniformLocation(shaderProgram, "lightColor");
	uniforms["specularColor"] = glGetUniformLocation(shaderProgram, "specularColor");
	uniforms["shininess"] = glGetUniformLocation(shaderProgram, "shininess");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["normal"] = glGetAttribLocation(shaderProgram, "normal");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	unsigned int totalFaces = 0;
	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					shipMesh.faceGroups.begin();
			itr != shipMesh.faceGroups.end();
			++itr
		)
		totalFaces += itr->second.size();

	GLfloat* vertDataBufferArray = new GLfloat[totalFaces * 24];	// 3 vertices + 3 normals + 2 texcoords * 3 faces
	unsigned int bufferIndex = 0;

	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					shipMesh.faceGroups.begin();
			itr != shipMesh.faceGroups.end();
			++itr
		) {
		const unsigned int firstIndex = bufferIndex;

		// insert the vertex attribute data
		for(size_t i = 0; i < itr->second.size(); ++i) {
			for(size_t p = 0; p < 3; ++p) {
				vertDataBufferArray[bufferIndex * 24 + p * 8 + 0] = shipMesh.vertices[itr->second[i].vertices[p]].x;
				vertDataBufferArray[bufferIndex * 24 + p * 8 + 1] = shipMesh.vertices[itr->second[i].vertices[p]].y;
				vertDataBufferArray[bufferIndex * 24 + p * 8 + 2] = shipMesh.vertices[itr->second[i].vertices[p]].z;

				vertDataBufferArray[bufferIndex * 24 + p * 8 + 3] = shipMesh.normals[itr->second[i].normals[p]].x;
				vertDataBufferArray[bufferIndex * 24 + p * 8 + 4] = shipMesh.normals[itr->second[i].normals[p]].y;
				vertDataBufferArray[bufferIndex * 24 + p * 8 + 5] = shipMesh.normals[itr->second[i].normals[p]].z;

				vertDataBufferArray[bufferIndex * 24 + p * 8 + 6] = shipMesh.texCoords[itr->second[i].texCoords[p]].x;
				vertDataBufferArray[bufferIndex * 24 + p * 8 + 7] = shipMesh.texCoords[itr->second[i].texCoords[p]].y;
			}

			++bufferIndex;
		}

		glGenBuffers(1, &(vertexBuffers[std::string("elements_" + itr->first).c_str()]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[std::string("elements_" + itr->first).c_str()]);

		GLuint* vertElementBufferArray = new GLuint[itr->second.size() * 3];

		for(size_t i = 0; i < itr->second.size() * 3; ++i) {
			vertElementBufferArray[i] = firstIndex * 3 + i;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, itr->second.size() * 3 * sizeof(GLuint), vertElementBufferArray,
				GL_STATIC_DRAW);

		delete[] vertElementBufferArray;
	}

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, totalFaces * 24 * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;
}

ShipRenderer::~ShipRenderer() {
/*
	for(
			std::map<std::string,GLuint>::iterator itr = textureIDs.begin();
			itr != textureIDs.end();
			++itr
		)
		glDeleteTextures(1, &(itr->second));

	if(glIsBuffer(vertDataBuffer))
		glDeleteBuffers(1, &vertDataBuffer);

	for(
			std::map<std::string,GLuint>::iterator itr = vertElementBuffers.begin();
			itr != vertElementBuffers.end();
			++itr
		)
		if(glIsBuffer(itr->second))
			glDeleteBuffers(1, &(itr->second));
*/
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);

//	glDeleteProgram(program);
}

void ShipRenderer::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	Camera* camera = ((Camera*) arguments["camera"]);

	// call for an updated camera
	camera->execute();

	// prepare variables
	Matrix4* mvpMatrix = &(camera->shipMatrix);

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1i(uniforms["useTexture"], 1);
	glUniform1i(uniforms["texture"], 0);
	glUniform1i(uniforms["useColor"], 0);
	glUniform1i(uniforms["useLighting"], 1);

	glUniform3f(uniforms["lightPosition"], 1.0f, 1.0f, 0.0f);
	glUniform3f(uniforms["lightColor"], 0.8f, 0.8f, 1.0f);
	glUniform3f(uniforms["specularColor"], 0.3f, 0.3f, 0.3f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) 0);
	glVertexAttribPointer(attributes["normal"], 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
			(GLvoid*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["normal"]);
	glEnableVertexAttribArray(attributes["texCoord"]);

	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					shipMesh.faceGroups.begin();
			itr != shipMesh.faceGroups.end();
			++itr
		) {
		// set the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(std::string("ship/" + itr->first).c_str()));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// set group-specific uniforms
		if(
				itr->first == "cockpit_highlights" ||
				itr->first == "right_propeller" ||
				itr->first == "left_propeller" ||
				itr->first == "underside" ||
				itr->first == "turret"
			)
			glUniform1f(uniforms["shininess"], 0.1f);
		else
			glUniform1f(uniforms["shininess"], 1.0f);

		Matrix4 newMVPMatrix = *mvpMatrix;

		if(itr->first == "left_propeller") {
			newMVPMatrix.identity();

			rotateMatrix(
					Vector3(0.0f, 1.0f, 0.0f),
					radians(
							-360.0f *
							(float) platform->getExecMills() / 1000.0f *
							gameSystem->getFloat("shipPropellerRPM")
						),
					newMVPMatrix
				);

			translateMatrix(
					leftPropellerOrigin.x,
					leftPropellerOrigin.y,
					leftPropellerOrigin.z,
					newMVPMatrix
				);
			newMVPMatrix *= *mvpMatrix;
		} else if(itr->first == "right_propeller") {
			newMVPMatrix.identity();

			rotateMatrix(
					Vector3(0.0f, 1.0f, 0.0f),
					radians(
							-360.0f *
							(float) platform->getExecMills() / 1000.0f *
							gameSystem->getFloat("shipPropellerRPM")
						),
					newMVPMatrix
				);

			translateMatrix(
					rightPropellerOrigin.x,
					rightPropellerOrigin.y,
					rightPropellerOrigin.z,
					newMVPMatrix
				);
			newMVPMatrix *= *mvpMatrix;
		}

		float mvpMatrixArray[] = {
				newMVPMatrix.m11, newMVPMatrix.m12, newMVPMatrix.m13, newMVPMatrix.m14,
				newMVPMatrix.m21, newMVPMatrix.m22, newMVPMatrix.m23, newMVPMatrix.m24,
				newMVPMatrix.m31, newMVPMatrix.m32, newMVPMatrix.m33, newMVPMatrix.m34,
				newMVPMatrix.m41, newMVPMatrix.m42, newMVPMatrix.m43, newMVPMatrix.m44
			};

		glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);

		// draw the geometry
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[std::string("elements_" + itr->first).c_str()]);

		glDrawElements(GL_TRIANGLES, itr->second.size() * 3, GL_UNSIGNED_INT, NULL);
	}

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["normal"]);
	glDisableVertexAttribArray(attributes["texCoord"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}
