/*
 *  ShipRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/rendering/ship/ShipRenderer.h"

ShipRenderer::ShipRenderer() {
	// dummy info since .obj indices start at 1
	ship.addVertex(Vector3(0.0f, 0.0f, 0.0f));
	ship.addNormal(Vector3(0.0f, 0.0f, 1.0f));
	ship.addTexCoord(Vector2(0.0f, 0.0f));

	// load the model file from the .obj file
	std::ifstream objFile;
	std::string line;

	std::string groupName = "SUBJECT";

	objFile.open(std::string(platform->dataPath + "/data/models/ship.obj").c_str());
	if(! objFile.is_open())
		programLog->report(ProgramLog::LOG_FATAL, "Unable to load ship model from file.");

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

				ship.vertices.push_back(vertex);
			} else if(lineType == "vt") {
				// texture coordinate
				float subjs[2] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f", &subjs[0], &subjs[1]);

				Vector2 texCoord;
				texCoord.x = subjs[0];
				texCoord.y = subjs[1];

				ship.texCoords.push_back(texCoord);
			} else if(lineType == "vn") {
				// normals
				float subjs[3] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f %f", &subjs[0], &subjs[1], &subjs[2]);

				Vector3 normal;
				normal.x = subjs[0];
				normal.y = subjs[1];
				normal.z = subjs[2];

				ship.normals.push_back(normal);
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
								vertex > ship.vertices.size() ||
								normal > ship.normals.size() ||
								texCoord > ship.texCoords.size()
							)
							programLog->report(
									ProgramLog::LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);
						faceVertices[i] = vertex;
						faceNormals[i] = normal;
						faceTexCoords[i] = texCoord;
					} else if(sscanf(subjs[i], "%u//%u", &vertex, &normal) == 2) {
						// line has slashes and specifies the vertex and normal
						if(
								vertex > ship.vertices.size() ||
								normal > ship.normals.size()
							)
							programLog->report(
									ProgramLog::LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);

						faceVertices[i] = vertex;
						faceNormals[i] = normal;
					} else if(sscanf(subjs[i],"%u", &vertex)) {
						// line has just the vertex
						if(vertex > ship.vertices.size())
								programLog->report(
										ProgramLog::LOG_FATAL,
										"Model face specification has out-of-bounds index."
									);
						faceVertices[i] = vertex;
					} else {
							programLog->report(
									ProgramLog::LOG_FATAL,
									"Model face specification has an invalid format."
								);
					}
				}

				// save it
				ship.addFace(
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
					ship.addFace(
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

	bool vertexAccounted[ship.vertices.size()];
	bool vertexMoved[ship.vertices.size()];

	unsigned int vertsAdded;
	Vector3 origin(0.0f, 0.0f, 0.0f);

	// adjust left propeller vertices
	for(unsigned int i = 0; i < ship.vertices.size(); ++i) vertexAccounted[i] = false;
	vertsAdded = 0;

	for(size_t i = 0; i < ship.faceGroups["left_propeller"].size(); ++i) {
		for(size_t p = 0; p < 3; ++p) {
			if(! vertexAccounted[ship.faceGroups["left_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(ship.vertices[ship.faceGroups["left_propeller"][i].vertices[p]]);

				leftPropellerOrigin += *thisVertex;
				vertsAdded += 1;

				vertexAccounted[ship.faceGroups["left_propeller"][i].vertices[p]] = true;
			}
		}
	}

	leftPropellerOrigin /= (float) vertsAdded;

	for(size_t i = 0; i < ship.vertices.size(); ++i) vertexMoved[i] = false;

	for(size_t i = 0; i < ship.faceGroups["left_propeller"].size(); ++i) {
		for(int p = 0; p < 3; ++p) {
			if(! vertexMoved[ship.faceGroups["left_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(ship.vertices[ship.faceGroups["left_propeller"][i].vertices[p]]);
				*thisVertex -= leftPropellerOrigin;

				vertexMoved[ship.faceGroups["left_propeller"][i].vertices[p]] = true;
			}
		}
	}

	// adjust right propeller vertices
	for(size_t i = 0; i < ship.vertices.size(); ++i) vertexAccounted[i] = false;
	vertsAdded = 0;

	for(size_t i = 0; i < ship.faceGroups["right_propeller"].size(); ++i) {
		for(unsigned int p = 0; p < 3; ++p) {
			if(! vertexAccounted[ship.faceGroups["right_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(ship.vertices[ship.faceGroups["right_propeller"][i].vertices[p]]);

				rightPropellerOrigin += *thisVertex;
				vertsAdded += 1;

				vertexAccounted[ship.faceGroups["right_propeller"][i].vertices[p]] = true;
			}
		}
	}

	rightPropellerOrigin /= (float) vertsAdded;

	for(size_t i = 0; i < ship.vertices.size(); ++i) vertexMoved[i] = false;

	for(size_t i = 0; i < ship.faceGroups["right_propeller"].size(); ++i) {
		for(unsigned int p = 0; p < 3; ++p) {
			if(! vertexMoved[ship.faceGroups["right_propeller"][i].vertices[p]]) {
				Vector3* thisVertex = &(ship.vertices[ship.faceGroups["right_propeller"][i].vertices[p]]);
				*thisVertex -= rightPropellerOrigin;

				vertexMoved[ship.faceGroups["right_propeller"][i].vertices[p]] = true;
			}
		}
	}

	// set up the model shader program
	modelVertexShader = ShaderTools::makeShader(
			std::string(platform->dataPath +  "/shaders/default.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	modelFragmentShader = ShaderTools::makeShader(
			std::string(platform->dataPath + "/shaders/default.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> modelShaders;
	modelShaders.push_back(modelVertexShader);
	modelShaders.push_back(modelFragmentShader);

	modelProgram = ShaderTools::makeProgram(modelShaders);

	// set attribute locations
	positionAttrib = 0;
	normalAttrib = 1;
	texCoordAttrib = 2;
	colorAttrib = 3;

	glBindAttribLocation(modelProgram, 0, "position");
	glBindAttribLocation(modelProgram, 1, "normal");
	glBindAttribLocation(modelProgram, 2, "texCoord");
	glBindAttribLocation(modelProgram, 3, "color");

	ShaderTools::linkProgram(modelProgram);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(modelProgram, "mvpMatrix");
	useTextureUniform = glGetUniformLocation(modelProgram, "useTexture");
	textureUniform = glGetUniformLocation(modelProgram, "texture");
	useLightingUniform = glGetUniformLocation(modelProgram, "useLighting");
	ambientColorUniform = glGetUniformLocation(modelProgram, "ambientColor");
	lightPositionUniform = glGetUniformLocation(modelProgram, "lightPosition");
	lightColorUniform = glGetUniformLocation(modelProgram, "lightColor");
	specularColorUniform = glGetUniformLocation(modelProgram, "specularColor");
	shininessUniform = glGetUniformLocation(modelProgram, "shininess");

	// load textures
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					ship.faceGroups.begin();
			itr != ship.faceGroups.end();
			++itr
		) {
		std::stringstream filename;
		filename <<
				platform->dataPath <<
				"/data/textures/ship/" <<
				itr->first.c_str() <<
				".bmp";
		BMPImage texture(filename.str().c_str());

		// load the texture into OpenGL
		GLuint textureID = 0;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				(texture.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
				texture.getWidth(),
				texture.getHeight(),
				0,
				(texture.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
				GL_UNSIGNED_BYTE,
				texture.getDataPointer()
		);

		textureIDs[itr->first] = textureID;
	}

	// load buffers
	glGenBuffers(1, &vertDataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);

	// prepare and send the array of vertex attribute data
	unsigned int totalFaces = 0;
	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					ship.faceGroups.begin();
			itr != ship.faceGroups.end();
			++itr
		)
		totalFaces += itr->second.size();

	GLfloat vertDataBufferArray[totalFaces * 24];	// 3 vertices + 3 normals + 2 texcoords * 3 faces
	unsigned int bufferIndex = 0;

	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					ship.faceGroups.begin();
			itr != ship.faceGroups.end();
			++itr
		) {
		const unsigned int firstIndex = bufferIndex;

		// insert the vertex attribute data
		for(size_t i = 0; i < itr->second.size(); ++i) {
			vertDataBufferArray[bufferIndex * 24 + 0] = ship.vertices[itr->second[i].vertices[0]].x;
			vertDataBufferArray[bufferIndex * 24 + 1] = ship.vertices[itr->second[i].vertices[0]].y;
			vertDataBufferArray[bufferIndex * 24 + 2] = ship.vertices[itr->second[i].vertices[0]].z;

			vertDataBufferArray[bufferIndex * 24 + 3] = ship.normals[itr->second[i].normals[0]].x;
			vertDataBufferArray[bufferIndex * 24 + 4] = ship.normals[itr->second[i].normals[0]].y;
			vertDataBufferArray[bufferIndex * 24 + 5] = ship.normals[itr->second[i].normals[0]].z;

			vertDataBufferArray[bufferIndex * 24 + 6] = ship.texCoords[itr->second[i].texCoords[0]].x;
			vertDataBufferArray[bufferIndex * 24 + 7] = ship.texCoords[itr->second[i].texCoords[0]].y;

			vertDataBufferArray[bufferIndex * 24 + 8] = ship.vertices[itr->second[i].vertices[1]].x;
			vertDataBufferArray[bufferIndex * 24 + 9] = ship.vertices[itr->second[i].vertices[1]].y;
			vertDataBufferArray[bufferIndex * 24 + 10] = ship.vertices[itr->second[i].vertices[1]].z;

			vertDataBufferArray[bufferIndex * 24 + 11] = ship.normals[itr->second[i].normals[1]].x;
			vertDataBufferArray[bufferIndex * 24 + 12] = ship.normals[itr->second[i].normals[1]].y;
			vertDataBufferArray[bufferIndex * 24 + 13] = ship.normals[itr->second[i].normals[1]].z;

			vertDataBufferArray[bufferIndex * 24 + 14] = ship.texCoords[itr->second[i].texCoords[1]].x;
			vertDataBufferArray[bufferIndex * 24 + 15] = ship.texCoords[itr->second[i].texCoords[1]].y;

			vertDataBufferArray[bufferIndex * 24 + 16] = ship.vertices[itr->second[i].vertices[2]].x;
			vertDataBufferArray[bufferIndex * 24 + 17] = ship.vertices[itr->second[i].vertices[2]].y;
			vertDataBufferArray[bufferIndex * 24 + 18] = ship.vertices[itr->second[i].vertices[2]].z;

			vertDataBufferArray[bufferIndex * 24 + 19] = ship.normals[itr->second[i].normals[2]].x;
			vertDataBufferArray[bufferIndex * 24 + 20] = ship.normals[itr->second[i].normals[2]].y;
			vertDataBufferArray[bufferIndex * 24 + 21] = ship.normals[itr->second[i].normals[2]].z;

			vertDataBufferArray[bufferIndex * 24 + 22] = ship.texCoords[itr->second[i].texCoords[2]].x;
			vertDataBufferArray[bufferIndex * 24 + 23] = ship.texCoords[itr->second[i].texCoords[2]].y;

			++bufferIndex;
		}

		// create an element buffer and send the element data
		glGenBuffers(1, &(vertElementBuffers[itr->first]));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffers[itr->first]);

		GLuint vertElementBufferArray[itr->second.size() * 3];
		for(size_t i = 0; i < itr->second.size(); ++i) {
			vertElementBufferArray[i * 3 + 0] = (firstIndex + i) * 3 + 0;
			vertElementBufferArray[i * 3 + 1] = (firstIndex + i) * 3 + 1;
			vertElementBufferArray[i * 3 + 2] = (firstIndex + i) * 3 + 2;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
				GL_STATIC_DRAW);
	}

	// send the completed vertex data array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
}

ShipRenderer::~ShipRenderer() {
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

//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);

//	glDeleteProgram(program);
}

void ShipRenderer::render(Matrix4 mvMatrix, Matrix4 pMatrix) {
	// set up the shader program
	glUseProgram(modelProgram);

	// constant uniforms/attributes
	glUniform1i(useTextureUniform, 1);
	glVertexAttrib4f(colorAttrib, 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform1i(useLightingUniform, 1);
	glUniform3f(ambientColorUniform, 0.35f, 0.25f, 0.25f);
	glUniform3f(lightPositionUniform, 1.0f, 1.0f, 0.0f);
	glUniform3f(lightColorUniform, 0.8f, 0.8f, 1.0f);
	glUniform3f(specularColorUniform, 0.3f, 0.3f, 0.3f);

	// texture unit uniform
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniform, 0);

	// activate the vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);

	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), NULL);
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
			(void*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
			(void*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(positionAttrib);
	glEnableVertexAttribArray(normalAttrib);
	glEnableVertexAttribArray(texCoordAttrib);

	// render each group of elements with the appropriate texture
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					ship.faceGroups.begin();
			itr != ship.faceGroups.end();
			++itr
		) {

		// prepare the MVP matrix
		Matrix4 mvpMatrix = mvMatrix;

		// do propeller transformations if necessary
		if(itr->first == "left_propeller") {
			Matrix4 baseMatrix; baseMatrix.identity();
			rotateMatrix(
					Vector3(0.0f, 1.0f, 0.0f),
					radians(
							-360.0f *
							(float) platform->getExecMills() / 1000.0f *
							gamePrefs->getFloat("shipPropellerRPM")
						),
					baseMatrix
				);

			translateMatrix(
					leftPropellerOrigin.x,
					leftPropellerOrigin.y,
					leftPropellerOrigin.z,
					baseMatrix
				);
			mvpMatrix = baseMatrix * mvpMatrix;
		} else if(itr->first == "right_propeller") {
			Matrix4 baseMatrix; baseMatrix.identity();

			rotateMatrix(
					Vector3(0.0f, 1.0f, 0.0f),
					radians(
							-360.0f *
							(float) platform->getExecMills() / 1000.0f *
							gamePrefs->getFloat("shipPropellerRPM")
						),
					baseMatrix
				);
			translateMatrix(
					rightPropellerOrigin.x,
					rightPropellerOrigin.y,
					rightPropellerOrigin.z,
					baseMatrix
				);
			mvpMatrix = baseMatrix * mvpMatrix;
		}

		mvpMatrix *= pMatrix;

		float mvpMatrixArray[] = {
				mvpMatrix.m11, mvpMatrix.m12, mvpMatrix.m13, mvpMatrix.m14,
				mvpMatrix.m21, mvpMatrix.m22, mvpMatrix.m23, mvpMatrix.m24,
				mvpMatrix.m31, mvpMatrix.m32, mvpMatrix.m33, mvpMatrix.m34,
				mvpMatrix.m41, mvpMatrix.m42, mvpMatrix.m43, mvpMatrix.m44
			};

		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrixArray);

		if(
				itr->first == "cockpit_highlights" ||
				itr->first == "right_propeller" ||
				itr->first == "left_propeller" ||
				itr->first == "underside" ||
				itr->first == "turret"
			)
			glUniform1f(shininessUniform, 0.1f);
		else
			glUniform1f(shininessUniform, 1.0f);

		// activate the texture
		glBindTexture(GL_TEXTURE_2D, textureIDs[itr->first]);

		// draw the buffer contents
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffers[itr->first]);
		glDrawElements(
				GL_TRIANGLES,
				itr->second.size() * 3,
				GL_UNSIGNED_INT,
				NULL
			);

	}

	glDisableVertexAttribArray(positionAttrib);
	glDisableVertexAttribArray(normalAttrib);
	glDisableVertexAttribArray(texCoordAttrib);
}
