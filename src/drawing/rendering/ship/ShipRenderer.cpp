/*
 *  SubjectRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "SubjectRenderer.h"

SubjectRenderer::SubjectRenderer() {
	// dummy info since .obj indices start at 1
	subject.addVertex(Vector3(0.0f, 0.0f, 0.0f));
	subject.addNormal(Vector3(0.0f, 0.0f, 1.0f));
	subject.addTexCoord(Vector2(0.0f, 0.0f));

	// load the model file from the .obj file
	std::ifstream objFile;
	std::string line;

	std::string groupName = "SUBJECT";

	objFile.open(std::string(Platform::getResourcePath() + "/subject/subject.obj").c_str());
	if(! objFile.is_open())
		ProgramLog::report(LOG_FATAL, "Unable to load subject model from file.");

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
				vertex.x = subjs[0];
				vertex.y = subjs[1];
				vertex.z = subjs[2];

				subject.vertices.push_back(vertex);
			} else if(lineType == "vt") {
				// texture coordinate
				float subjs[2] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f", &subjs[0], &subjs[1]);

				Vector2 texCoord;
				texCoord.x = subjs[0];
				texCoord.y = subjs[1];

				subject.texCoords.push_back(texCoord);
			} else if(lineType == "vn") {
				// normals
				float subjs[3] = { };
				sscanf(line.c_str() + 2 * sizeof(char), "%f %f %f", &subjs[0], &subjs[1], &subjs[2]);

				Vector3 normal;
				normal.x = subjs[0];
				normal.y = subjs[1];
				normal.z = subjs[2];

				subject.normals.push_back(normal);
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
								vertex > subject.vertices.size() ||
								normal > subject.normals.size() ||
								texCoord > subject.texCoords.size()
							)
							ProgramLog::report(
									LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);
						faceVertices[i] = vertex;
						faceNormals[i] = normal;
						faceTexCoords[i] = texCoord;
					} else if(sscanf(subjs[i], "%u//%u", &vertex, &normal) == 2) {
						// line has slashes and specifies the vertex and normal
						if(
								vertex > subject.vertices.size() ||
								normal > subject.normals.size()
							)
							ProgramLog::report(
									LOG_FATAL,
									"Model face specification has out-of-bounds index."
								);

						faceVertices[i] = vertex;
						faceNormals[i] = normal;
					} else if(sscanf(subjs[i],"%u", &vertex)) {
						// line has just the vertex
						if(vertex > subject.vertices.size())
								ProgramLog::report(
										LOG_FATAL,
										"Model face specification has out-of-bounds index."
									);
						faceVertices[i] = vertex;
					} else {
							ProgramLog::report(
									LOG_FATAL,
									"Model face specification has an invalid format."
								);
					}
				}

				// save it
				subject.addFace(
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
					subject.addFace(
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

	// set up the model shader program
	modelVertexShader = ShaderTools::makeShader(
			std::string(Platform::getResourcePath() +  "/shaders/model.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	modelFragmentShader = ShaderTools::makeShader(
			std::string(Platform::getResourcePath() + "/shaders/model.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> modelShaders;
	modelShaders.push_back(modelVertexShader);
	modelShaders.push_back(modelFragmentShader);

	modelProgram = ShaderTools::makeProgram(modelShaders);

	// get attribute locations
	positionAttrib = glGetAttribLocation(modelProgram, "position");
	normalAttrib = glGetAttribLocation(modelProgram, "normal");
	texCoordAttrib = glGetAttribLocation(modelProgram, "texCoord");

	// get uniform locations
	MVPMatrixUniform = glGetUniformLocation(modelProgram, "MVPMatrix");
	clockUniform = glGetUniformLocation(modelProgram, "clock");
	textureUniform = glGetUniformLocation(modelProgram, "texture");

	// load textures
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					subject.faceGroups.begin();
			itr != subject.faceGroups.end();
			++itr
		) {
		std::stringstream filename;
		filename <<
				Platform::getResourcePath() <<
				"/subject/textures/" <<
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

SubjectRenderer::~SubjectRenderer() {
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

void SubjectRenderer::render() {
	// set up the shader program
	glUseProgram(modelProgram);

	// update the camera
	camera.update();

	// prepare the MVP matrix
	Matrix4 MVPMatrix;
	MVPMatrix.identity();

	float nearClip = -1.0f;
	float farClip = 20.0f;
	float zoomX = 0.75f;
	float zoomY = 1.0f;

	// perspective projection uniform
//	MVPMatrix *= Matrix4(
//			zoomX, 0.0f, 0.0f, 0.0f,
//			0.0f, zoomY, 0.0f, 0.0f,
//			0.0f, 0.0f, (farClip + nearClip) / (farClip - nearClip), 1.0f,
//			0.0f, 0.0f, (2 * nearClip * farClip) / (nearClip - farClip), 0.0f
//		);

	// reflect z
//	MVPMatrix *= Matrix4(
//			1.0f, 0.0f, 0.0f, 0.0f,
//			0.0f, 1.0f, 0.0f, 0.0f,
//			0.0f, 0.0f, -1.0f, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f
//		);

	// screen height/width factor
	MVPMatrix *= Matrix4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 4.0f/3.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

	// zoom/rotations
	scaleMatrix(0.15f, 0.15f, 0.15f, MVPMatrix);
	MVPMatrix = camera.currentScale * camera.currentRotate * MVPMatrix;

	// back up
//	translateMatrix(0.0f, 0.0f, 3.0f, MVPMatrix);

	float MVPMatrixArray[] = {
			MVPMatrix.m11, MVPMatrix.m21, MVPMatrix.m31, MVPMatrix.m41,
			MVPMatrix.m12, MVPMatrix.m22, MVPMatrix.m32, MVPMatrix.m42,
			MVPMatrix.m13, MVPMatrix.m23, MVPMatrix.m33, MVPMatrix.m43,
			MVPMatrix.m14, MVPMatrix.m24, MVPMatrix.m34, MVPMatrix.m44
		};

//	for(int i = 0; i < 16; ++i) std::cout << MVPMatrixArray[i] << " "; std::cout << std::endl;

	glUniformMatrix4fv(MVPMatrixUniform, 1, GL_FALSE, MVPMatrixArray);

	// clock uniform
	glUniform1f(clockUniform, (float) Platform::getExecutionTimeMicros() / 1000000.0f);

	// texture unit uniform
	glUniform1i(textureUniform, 0);

	// render each group of elements with the appropriate texture
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					subject.faceGroups.begin();
			itr != subject.faceGroups.end();
			++itr
		) {
		// activate the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureIDs[itr->first]);

		for(int i = 0; i < itr->second.size(); ++i) {
			// render triangles
			glBegin(GL_TRIANGLES);

			glVertexAttrib2f(
					texCoordAttrib,
					subject.texCoords[itr->second[i].texCoords[0]].x,
					subject.texCoords[itr->second[i].texCoords[0]].y
				);
			glVertexAttrib3f(
					normalAttrib,
					subject.normals[itr->second[i].normals[0]].x,
					subject.normals[itr->second[i].normals[0]].y,
					subject.normals[itr->second[i].normals[0]].z
				);
			glVertexAttrib3f(
					positionAttrib,
					subject.vertices[itr->second[i].vertices[0]].x,
					subject.vertices[itr->second[i].vertices[0]].y,
					subject.vertices[itr->second[i].vertices[0]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					subject.texCoords[itr->second[i].texCoords[1]].x,
					subject.texCoords[itr->second[i].texCoords[1]].y
				);
			glVertexAttrib3f(
					normalAttrib,
					subject.normals[itr->second[i].normals[1]].x,
					subject.normals[itr->second[i].normals[1]].y,
					subject.normals[itr->second[i].normals[1]].z
				);
			glVertexAttrib3f(
					positionAttrib,
					subject.vertices[itr->second[i].vertices[1]].x,
					subject.vertices[itr->second[i].vertices[1]].y,
					subject.vertices[itr->second[i].vertices[1]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					subject.texCoords[itr->second[i].texCoords[2]].x,
					subject.texCoords[itr->second[i].texCoords[2]].y
				);
			glVertexAttrib3f(
					normalAttrib,
					subject.normals[itr->second[i].normals[2]].x,
					subject.normals[itr->second[i].normals[2]].y,
					subject.normals[itr->second[i].normals[2]].z
				);
			glVertexAttrib3f(
					positionAttrib,
					subject.vertices[itr->second[i].vertices[2]].x,
					subject.vertices[itr->second[i].vertices[2]].y,
					subject.vertices[itr->second[i].vertices[2]].z
				);

			glEnd();
		}
	}
}
