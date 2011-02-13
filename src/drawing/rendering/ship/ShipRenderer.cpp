/*
 *  ShipRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ShipRenderer.h"

ShipRenderer::ShipRenderer() {
	// dummy info since .obj indices start at 1
	ship.addVertex(Vector3(0.0f, 0.0f, 0.0f));
	ship.addNormal(Vector3(0.0f, 0.0f, 1.0f));
	ship.addTexCoord(Vector2(0.0f, 0.0f));

	// load the model file from the .obj file
	std::ifstream objFile;
	std::string line;

	std::string groupName = "SUBJECT";

	objFile.open(std::string(platform.dataPath + "/data/models/ship.obj").c_str());
	if(! objFile.is_open())
		ProgramLog::report(LOG_FATAL, "Unable to load ship model from file.");

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
				float faceVertices[4];
				float faceNormals[4];
				float faceTexCoords[4];

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
							ProgramLog::report(
									LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);

						// TODO ** REVIEW IF THESE SHOULD BE INT ARRAYS NOT FLOAT ARRAYS CUS THIS LOOKS LIKE AN INDEX BEING PUT INTO A VECTOR
						faceVertices[i] = (float)vertex;
						faceNormals[i] = (float)normal;
						faceTexCoords[i] = (float)texCoord;
					} else if(sscanf(subjs[i], "%u//%u", &vertex, &normal) == 2) {
						// line has slashes and specifies the vertex and normal
						if(
								vertex > ship.vertices.size() ||
								normal > ship.normals.size()
							)
							ProgramLog::report(
									LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);
						// TODO ** REVIEW IF THESE SHOULD BE INT ARRAYS NOT FLOAT ARRAYS CUS THIS LOOKS LIKE AN INDEX BEING PUT INTO A VECTOR
						faceVertices[i] = (float)vertex;
						faceNormals[i] = (float)normal;
					} else if(sscanf(subjs[i],"%u", &vertex)) {
						// line has just the vertex
						if(vertex > ship.vertices.size())
								ProgramLog::report(
										LOG_FATAL,
										"Model face specification has out-of-bounds index."
									);
						// TODO ** REVIEW IF THESE SHOULD BE INT ARRAYS NOT FLOAT ARRAYS CUS THIS LOOKS LIKE AN INDEX BEING PUT INTO A VECTOR
						faceVertices[i] = (float)vertex;
					} else {
							ProgramLog::report(
									LOG_FATAL,
									"Model face specification has an invalid format."
								);
					}
				}

				// save it
				ship.addFace(
						(unsigned int)faceVertices[0],
						(unsigned int)faceVertices[1],
						(unsigned int)faceVertices[2],
						(unsigned int)faceNormals[0],
						(unsigned int)faceNormals[1],
						(unsigned int)faceNormals[2],
						(unsigned int)faceTexCoords[0],
						(unsigned int)faceTexCoords[1],
						(unsigned int)faceTexCoords[2],
						groupName
					);
				// add the other half of the quad as a second triangle, if necessary
				if(numFaces == 4)
					ship.addFace(
							(unsigned int)faceVertices[2],
							(unsigned int)faceVertices[3],
							(unsigned int)faceVertices[0],
							(unsigned int)faceNormals[2],
							(unsigned int)faceNormals[3],
							(unsigned int)faceNormals[0],
							(unsigned int)faceTexCoords[2],
							(unsigned int)faceTexCoords[3],
							(unsigned int)faceTexCoords[0],
							groupName
						);
			}
		}
	}

	objFile.close();

	// set up the model shader program
	modelVertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/default.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	modelFragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/default.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> modelShaders;
	modelShaders.push_back(modelVertexShader);
	modelShaders.push_back(modelFragmentShader);

	modelProgram = ShaderTools::makeProgram(modelShaders);

	// set attribute locations
	positionAttrib = 0;
	texCoordAttrib = 1;
	glBindAttribLocation(modelProgram, 0, "position");
	glBindAttribLocation(modelProgram, 1, "texCoord");

	ShaderTools::linkProgram(modelProgram);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(modelProgram, "mvpMatrix");
	textureUniform = glGetUniformLocation(modelProgram, "texture");

	// load textures
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					ship.faceGroups.begin();
			itr != ship.faceGroups.end();
			++itr
		) {
		std::stringstream filename;
		filename <<
				platform.dataPath <<
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
}

ShipRenderer::~ShipRenderer() {
	for(
			std::map<std::string,GLuint>::iterator itr = textureIDs.begin();
			itr != textureIDs.end();
			++itr
		)
		glDeleteTextures(1, &(itr->second));

//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);

//	glDeleteProgram(program);
}

void ShipRenderer::render(Matrix4 mvpMatrix) {
	// set up the shader program
	glUseProgram(modelProgram);

	// prepare the MVP matrix
	float mvpMatrixArray[] = {
			mvpMatrix.m11, mvpMatrix.m12, mvpMatrix.m13, mvpMatrix.m14,
			mvpMatrix.m21, mvpMatrix.m22, mvpMatrix.m23, mvpMatrix.m24,
			mvpMatrix.m31, mvpMatrix.m32, mvpMatrix.m33, mvpMatrix.m34,
			mvpMatrix.m41, mvpMatrix.m42, mvpMatrix.m43, mvpMatrix.m44
		};

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrixArray);

	// texture unit uniform
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniform, 0);

	// render each group of elements with the appropriate texture
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					ship.faceGroups.begin();
			itr != ship.faceGroups.end();
			++itr
		) {
		// activate the texture
		glBindTexture(GL_TEXTURE_2D, textureIDs[itr->first]);

		for(size_t i = 0; i < itr->second.size(); ++i) {
			// render triangles
			glBegin(GL_TRIANGLES);

			glVertexAttrib2f(
					texCoordAttrib,
					ship.texCoords[itr->second[i].texCoords[0]].x,
					ship.texCoords[itr->second[i].texCoords[0]].y
				);
			glVertexAttrib3f(
					positionAttrib,
					ship.vertices[itr->second[i].vertices[0]].x,
					ship.vertices[itr->second[i].vertices[0]].y,
					ship.vertices[itr->second[i].vertices[0]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					ship.texCoords[itr->second[i].texCoords[1]].x,
					ship.texCoords[itr->second[i].texCoords[1]].y
				);
			glVertexAttrib3f(
					positionAttrib,
					ship.vertices[itr->second[i].vertices[1]].x,
					ship.vertices[itr->second[i].vertices[1]].y,
					ship.vertices[itr->second[i].vertices[1]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					ship.texCoords[itr->second[i].texCoords[2]].x,
					ship.texCoords[itr->second[i].texCoords[2]].y
				);
			glVertexAttrib3f(
					positionAttrib,
					ship.vertices[itr->second[i].vertices[2]].x,
					ship.vertices[itr->second[i].vertices[2]].y,
					ship.vertices[itr->second[i].vertices[2]].z
				);

			glEnd();
		}
	}
}
