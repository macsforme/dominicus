// MissileTrailRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/MissileTrailRenderer.h"

#include <vector>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
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

	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "missileTrail"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "missileTrail"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvMatrix"] = glGetUniformLocation(shaderProgram, "mvMatrix");
	uniforms["pMatrix"] = glGetUniformLocation(shaderProgram, "pMatrix");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");
	uniforms["timer"] = glGetUniformLocation(shaderProgram, "timer");

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

	// send the buffer data
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

	glDisable(GL_TEXTURE_2D);
}

MissileTrailRenderer::~MissileTrailRenderer() {
	// undo shader setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));

	if(! glIsShader(shaderProgram)) // sometimes duplicate shaders get optimized out so check for validity
		return;

	GLsizei shaderCount;
	GLuint* shaders = new GLuint[2];
	glGetAttachedShaders(shaderProgram, 2, &shaderCount, shaders);

	for(size_t i = 0; i < shaderCount; ++i) {
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	delete[] shaders;

	glDeleteProgram(shaderProgram);
}

void MissileTrailRenderer::execute(std::map<std::string, void*> arguments) {
	// prepare variables
	Vector4 lightPosition(1.0f, 1.0f, -1.0f, 0.0f);
	lightPosition = lightPosition * gameGraphics->currentCamera->lightMatrix;

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(gameGraphics->supportsMultisampling) glEnable(GL_MULTISAMPLE);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1i(uniforms["texture"], 0);
	glUniform1f(uniforms["timer"], gameState->lastUpdateGameTime / 1000.0f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(attributes["normal"], 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat),
			(GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat),
			(GLvoid*) (8 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["normal"]);
	glEnableVertexAttribArray(attributes["texCoord"]);
	glEnableVertexAttribArray(attributes["color"]);

	for(size_t i = 0; i < gameState->missiles.size(); ++i) {
		if(! gameState->missiles[i].alive)
			continue;

		// calculate the matrix for this missile trail position
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

		// set the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, noiseTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glUniformMatrix4fv(uniforms["mvMatrix"], 1, GL_FALSE, mvMatrixArray);
		glUniformMatrix4fv(uniforms["pMatrix"], 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));

		// draw the geometry
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements_trail"]);

		glDrawElements(GL_TRIANGLES, missileMesh.faceGroups["trail"].size() * 3, GL_UNSIGNED_INT, NULL);
	}

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["normal"]);
	glDisableVertexAttribArray(attributes["texCoord"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
}
