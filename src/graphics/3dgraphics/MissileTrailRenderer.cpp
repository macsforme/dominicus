// MissileTrailRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/MissileTrailRenderer.h"

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

MissileTrailRenderer::MissileTrailRenderer() {
	// initialize geometry with appropriate length
	missileMesh = Mesh("trail");

	for(size_t i = 0; i < missileMesh.vertices.size(); ++i) {
		missileMesh.vertices[i].x *= gameSystem->getFloat("missileTrailLength");
	}

	for(size_t i = 0; i < missileMesh.texCoords.size(); ++i) {
		missileMesh.texCoords[i].x -= 0.5f;
		missileMesh.texCoords[i].y -= 0.5f;
	}

	missileMesh.autoNormal();

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
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 11] = (1.0f + missileMesh.vertices[itr->second[i].vertices[p]].x / gameSystem->getFloat("missileTrailLength"));
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

	// send the noise texture
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &noiseTextureID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(gameGraphics->noiseTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			gameGraphics->noiseTexture->width,
			gameGraphics->noiseTexture->height,
			0,
			(gameGraphics->noiseTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			gameGraphics->noiseTexture->getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);
}

MissileTrailRenderer::~MissileTrailRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void MissileTrailRenderer::execute(DrawStackArgList arguments) {
	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glEnable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("missileTrail"));

	// set uniforms
	glUniform1i(glGetUniformLocation(gameGraphics->getProgramID("missileTrail"), "texture"), 0);
	glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("missileTrail"), "timer"), gameState->lastUpdateGameTime / 1000.0f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "position"), 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "normal"), 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "texCoord"), 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "color"), 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) (8 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "normal"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "texCoord"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "color"));

	for(size_t i = 0; i < gameState->missiles.size(); ++i) {
		if(! gameState->missiles[i].alive)
			continue;

		// calculate the matrix for this missile trail position
		Matrix4 mvMatrix; mvMatrix.identity();
		if(gameState->missiles[i].position.y - gameSystem->getFloat("missileTrailLength") < 0.0f)
			scaleMatrix(gameState->missiles[i].position.y / gameSystem->getFloat("missileTrailLength"), 1.0f, 1.0f, mvMatrix);
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

		// set the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, noiseTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("missileTrail"), "mvMatrix"), 1, GL_FALSE, mvMatrixArray);
			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("missileTrail"), "pMatrix"), 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));

		// draw the geometry
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements_trail"]);

		glDrawElements(GL_TRIANGLES, missileMesh.faceGroups["trail"].size() * 3, GL_UNSIGNED_INT, NULL);
	}

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "normal"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "texCoord"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("missileTrail"), "color"));
}
