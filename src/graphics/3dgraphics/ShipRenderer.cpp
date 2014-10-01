// ShipRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/ShipRenderer.h"

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

ShipRenderer::ShipRenderer() {
	shipMesh = Mesh("ship");

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

	GLfloat* vertDataBufferArray = new GLfloat[totalFaces * 36];	// 3 vertices + 3 normals + 2 texcoords + 4 colors * 3 faces
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
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 0] = shipMesh.vertices[itr->second[i].vertices[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 1] = shipMesh.vertices[itr->second[i].vertices[p]].y;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 2] = shipMesh.vertices[itr->second[i].vertices[p]].z;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 3] = shipMesh.normals[itr->second[i].normals[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 4] = shipMesh.normals[itr->second[i].normals[p]].y;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 5] = shipMesh.normals[itr->second[i].normals[p]].z;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 6] = shipMesh.texCoords[itr->second[i].texCoords[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 7] = shipMesh.texCoords[itr->second[i].texCoords[p]].y;

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

	glBufferData(GL_ARRAY_BUFFER, totalFaces * 36 * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;
}

ShipRenderer::~ShipRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void ShipRenderer::execute(DrawStackArgList arguments) {
	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	if(gameGraphics->supportsMultisampling) glEnable(GL_MULTISAMPLE);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("colorTextureLighting"));

	// set uniforms
	glUniform1i(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "texture"), 0);
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "ambientColor"), 0.15f, 0.15f, 0.15f);
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "diffuseColor"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "specularColor"), 0.8f, 0.8f, 0.8f);
	Vector4 lightPosition = Vector4(1.0f, 1.0f, -1.0f, 0.0f) * gameGraphics->currentCamera->lightMatrix;
	glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "shininess"), 10.0f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "position"), 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "normal"), 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "texCoord"), 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "color"), 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) (8 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "normal"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "texCoord"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "color"));

	for(size_t i = 0; i < gameState->ships.size(); ++i) {
		// calculate the matrix for this ship position
		Matrix4 shipMatrix; shipMatrix.identity();
		rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(gameState->ships[i].rotation), shipMatrix);
		translateMatrix(gameState->ships[i].position.x, gameState->ships[i].position.y, gameState->ships[i].position.z, shipMatrix);

		Matrix4 mvMatrix = shipMatrix * gameGraphics->currentCamera->mvMatrix;
		float mvMatrixArray[] = {
				mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
				mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
				mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
				mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
			};

		// draw the ship
		for(
				std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					shipMesh.faceGroups.begin();
				itr != shipMesh.faceGroups.end();
				++itr
			) {
			// don't draw the missile origin or submerged portion
			if(itr->first == "missileorigin" || itr->first == "submerged")
				continue;

			// set the texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(std::string("structure/" + itr->first).c_str()));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "mvMatrix"), 1, GL_FALSE, mvMatrixArray);
			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "pMatrix"), 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));

			// draw the geometry
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[std::string("elements_" + itr->first).c_str()]);

			glDrawElements(GL_TRIANGLES, itr->second.size() * 3, GL_UNSIGNED_INT, NULL);
		}
	}

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "normal"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "texCoord"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTextureLighting"), "color"));

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
}
