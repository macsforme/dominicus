// MissileRenderer
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/3dgraphics/MissileRenderer.h"

MissileRenderer::MissileRenderer() {
	missileMesh = Mesh("missile");

	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "colorTextureLighting"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "colorTextureLighting"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvMatrix"] = glGetUniformLocation(shaderProgram, "mvMatrix");
	uniforms["pMatrix"] = glGetUniformLocation(shaderProgram, "pMatrix");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");
	uniforms["ambientColor"] = glGetUniformLocation(shaderProgram, "ambientColor");
	uniforms["diffuseColor"] = glGetUniformLocation(shaderProgram, "diffuseColor");
	uniforms["specularColor"] = glGetUniformLocation(shaderProgram, "specularColor");
	uniforms["lightPosition"] = glGetUniformLocation(shaderProgram, "lightPosition");
	uniforms["shininess"] = glGetUniformLocation(shaderProgram, "shininess");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["normal"] = glGetAttribLocation(shaderProgram, "normal");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	unsigned int totalFaces = 0;
	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					missileMesh.faceGroups.begin();
			itr != missileMesh.faceGroups.end();
			++itr
		)
		totalFaces += itr->second.size();

	GLfloat* vertDataBufferArray = new GLfloat[totalFaces * 36];	// 3 vertices + 3 normals + 2 texcoords + 4 colors * 3 faces
	unsigned int bufferIndex = 0;

	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					missileMesh.faceGroups.begin();
			itr != missileMesh.faceGroups.end();
			++itr
		) {
		const unsigned int firstIndex = bufferIndex;

		// insert the vertex attribute data
		for(size_t i = 0; i < itr->second.size(); ++i) {
			for(size_t p = 0; p < 3; ++p) {
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 0] = missileMesh.vertices[itr->second[i].vertices[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 1] = missileMesh.vertices[itr->second[i].vertices[p]].y;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 2] = missileMesh.vertices[itr->second[i].vertices[p]].z;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 3] = missileMesh.normals[itr->second[i].normals[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 4] = missileMesh.normals[itr->second[i].normals[p]].y;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 5] = missileMesh.normals[itr->second[i].normals[p]].z;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 6] = missileMesh.texCoords[itr->second[i].texCoords[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 7] = missileMesh.texCoords[itr->second[i].texCoords[p]].y;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 8] = 1.0f;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 9] = 1.0f;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 10] = 1.0f;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 11] = 1.0f;
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
	glBufferData(GL_ARRAY_BUFFER, totalFaces * 36 * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;
}

MissileRenderer::~MissileRenderer() {
//FIXME destroy variables
}

void MissileRenderer::execute(std::map<std::string, void*> arguments) {
	// prepare variables
	Vector4 lightPosition(1.0f, 1.0f, -1.0f, 0.0f);
	lightPosition = lightPosition * gameGraphics->currentCamera->lightMatrix;

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_MULTISAMPLE);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1i(uniforms["texture"], 0);
	glUniform3f(uniforms["ambientColor"], 0.15f, 0.15f, 0.15f);
	glUniform3f(uniforms["diffuseColor"], 0.5f, 0.5f, 0.5f);
	glUniform3f(uniforms["specularColor"], 0.8f, 0.8f, 0.8f);
	glUniform3f(uniforms["lightPosition"], lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(uniforms["shininess"], 10.0f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*) 0);
	glVertexAttribPointer(attributes["normal"], 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT),
			(GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT),
			(GLvoid*) (8 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["normal"]);
	glEnableVertexAttribArray(attributes["texCoord"]);
	glEnableVertexAttribArray(attributes["color"]);

	for(size_t i = 0; i < gameState->missiles.size(); ++i) {
		if(! gameState->missiles[i].alive)
			continue;

		// calculate the matrix for this missile position
		Matrix4 mvMatrix; mvMatrix.identity();
		rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), radians(gameState->missiles[i].tilt), mvMatrix);
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(gameState->missiles[i].rotation), mvMatrix);
		translateMatrix(gameState->missiles[i].position.x, gameState->missiles[i].position.y, gameState->missiles[i].position.z, mvMatrix);

		mvMatrix = mvMatrix * gameGraphics->currentCamera->mvMatrix;
		float mvMatrixArray[] = {
				mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
				mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
				mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
				mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
			};

		// draw the missile
		for(
				std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					missileMesh.faceGroups.begin();
				itr != missileMesh.faceGroups.end();
				++itr
			) {
			// don't draw the nozzle origin
			if(itr->first == "nozzleorigin")
				continue;

			// set the texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(std::string("structure/" + itr->first).c_str()));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glUniformMatrix4fv(uniforms["mvMatrix"], 1, GL_FALSE, mvMatrixArray);
			glUniformMatrix4fv(uniforms["pMatrix"], 1, GL_FALSE, gameGraphics->ppMatrixArray);

			// draw the geometry
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[std::string("elements_" + itr->first).c_str()]);

			glDrawElements(GL_TRIANGLES, itr->second.size() * 3, GL_UNSIGNED_INT, NULL);
		}
	}

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["normal"]);
	glDisableVertexAttribArray(attributes["texCoord"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_MULTISAMPLE);
}
