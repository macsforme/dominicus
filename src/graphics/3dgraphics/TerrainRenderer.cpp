// TerrainRenderer.cpp
// Dominicus

#include "graphics/3dgraphics/TerrainRenderer.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "core/GameSystem.h"
#include "geometry/Mesh.h"
#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "math/MatrixMath.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

TerrainRenderer::TerrainRenderer() {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

TerrainRenderer::~TerrainRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void TerrainRenderer::reloadState() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	Mesh* terrainMesh = &(gameState->island);
	std::vector<Mesh::Face>* faceGroup = &(terrainMesh->faceGroups.begin()->second);
	GLfloat* vertDataBufferArray = new GLfloat[24 * faceGroup->size()];
	for(size_t i = 0; i < faceGroup->size(); ++i) {
		vertDataBufferArray[i * 24 + 0] = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].x;
		vertDataBufferArray[i * 24 + 1] = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y;
		vertDataBufferArray[i * 24 + 2] = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].z;

		vertDataBufferArray[i * 24 + 3] = terrainMesh->normals[(*faceGroup)[i].normals[0]].x;
		vertDataBufferArray[i * 24 + 4] = terrainMesh->normals[(*faceGroup)[i].normals[0]].y;
		vertDataBufferArray[i * 24 + 5] = terrainMesh->normals[(*faceGroup)[i].normals[0]].z;

		vertDataBufferArray[i * 24 + 6] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[0]].x;
		vertDataBufferArray[i * 24 + 7] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[0]].y;

		vertDataBufferArray[i * 24 + 8] = terrainMesh->vertices[(*faceGroup)[i].vertices[1]].x;
		vertDataBufferArray[i * 24 + 9] = terrainMesh->vertices[(*faceGroup)[i].vertices[1]].y;
		vertDataBufferArray[i * 24 + 10] = terrainMesh->vertices[(*faceGroup)[i].vertices[1]].z;

		vertDataBufferArray[i * 24 + 11] = terrainMesh->normals[(*faceGroup)[i].normals[1]].x;
		vertDataBufferArray[i * 24 + 12] = terrainMesh->normals[(*faceGroup)[i].normals[1]].y;
		vertDataBufferArray[i * 24 + 13] = terrainMesh->normals[(*faceGroup)[i].normals[1]].z;

		vertDataBufferArray[i * 24 + 14] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[1]].x;
		vertDataBufferArray[i * 24 + 15] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[1]].y;

		vertDataBufferArray[i * 24 + 16] = terrainMesh->vertices[(*faceGroup)[i].vertices[2]].x;
		vertDataBufferArray[i * 24 + 17] = terrainMesh->vertices[(*faceGroup)[i].vertices[2]].y;
		vertDataBufferArray[i * 24 + 18] = terrainMesh->vertices[(*faceGroup)[i].vertices[2]].z;

		vertDataBufferArray[i * 24 + 19] = terrainMesh->normals[(*faceGroup)[i].normals[2]].x;
		vertDataBufferArray[i * 24 + 20] = terrainMesh->normals[(*faceGroup)[i].normals[2]].y;
		vertDataBufferArray[i * 24 + 21] = terrainMesh->normals[(*faceGroup)[i].normals[2]].z;

		vertDataBufferArray[i * 24 + 22] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[2]].x;
		vertDataBufferArray[i * 24 + 23] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[2]].y;
	}

	GLuint* vertElementBufferArray = new GLuint[3 * faceGroup->size()];
	for(size_t i = 0; i < faceGroup->size() * 3; ++i) {
		vertElementBufferArray[i] = i;
	}

	glBufferData(GL_ARRAY_BUFFER, 24 * faceGroup->size() * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * faceGroup->size() * sizeof(GLuint), vertElementBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;
	delete[] vertElementBufferArray;

	// send the noise texture
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &noiseTextureID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(gameGraphics->fourDepthNoiseTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			gameGraphics->fourDepthNoiseTexture->width,
			gameGraphics->fourDepthNoiseTexture->height,
			0,
			(gameGraphics->fourDepthNoiseTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			gameGraphics->fourDepthNoiseTexture->getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);
}

void TerrainRenderer::execute(DrawStackArgList arguments) {
	// prepare variables
	Matrix4 mvMatrix = gameGraphics->currentCamera->mvMatrix;
	float mvMatrixArray[] = {
			mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
			mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
			mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
			mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
		};

	Matrix4 fortressTransformMatrix; fortressTransformMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, fortressTransformMatrix);
	float fortressTransformMatrixArray[] = {
			fortressTransformMatrix.m11, fortressTransformMatrix.m12, fortressTransformMatrix.m13, fortressTransformMatrix.m14,
			fortressTransformMatrix.m21, fortressTransformMatrix.m22, fortressTransformMatrix.m23, fortressTransformMatrix.m24,
			fortressTransformMatrix.m31, fortressTransformMatrix.m32, fortressTransformMatrix.m33, fortressTransformMatrix.m34,
			fortressTransformMatrix.m41, fortressTransformMatrix.m42, fortressTransformMatrix.m43, fortressTransformMatrix.m44
		};

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
	glUseProgram(gameGraphics->getProgramID("terrain"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "mvMatrix"), 1, GL_FALSE, mvMatrixArray);
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "pMatrix"), 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "fortressTransformMatrix"), 1, GL_FALSE, fortressTransformMatrixArray);
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "insideColorMultiplier"), 1.0f, 1.0f, 1.0f, 1.0f);
	float empColorMultiplier = (
			gameState->fortress.emp <= 0.0f || gameState->fortress.emp >= 1.0f ?
			1.0f :
			1.0f - gameState->fortress.emp + gameState->fortress.emp * gameSystem->getFloat("empColorMultiplier")
		);
	glUniform4f(
			glGetUniformLocation(gameGraphics->getProgramID("terrain"), "outsideColorMultiplier"),
			empColorMultiplier,
			empColorMultiplier,
			empColorMultiplier,
			1.0f
		);
	glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "colorChangeRadius"),
			gameState->fortress.emp <= 0.0f || gameState->fortress.emp >= 1.0f ?
			0.0f :
			(1.0f - gameState->fortress.emp) * gameSystem->getFloat("stateEMPRange")
		);

	GLint textureUniforms[] = { 0, 1, 2, 3, 4 };
	glUniform1iv(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "textures"), 5, textureUniforms);
	glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("terrain"), "depth"), (GLfloat) gameSystem->getFloat("terrainDepth"));

	// activate the textures
	for(int i = 0; i < 3; ++i) {
		if(i == 0) glActiveTexture(GL_TEXTURE0);
		else if(i == 1) glActiveTexture(GL_TEXTURE1);
		else if(i == 2) glActiveTexture(GL_TEXTURE2);

		std::string terrainFile;
		if(i == 0) terrainFile = "terrain/green";
		else if(i == 1) terrainFile = "terrain/gray";
		else if(i == 2) terrainFile = "terrain/brown";
		glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(terrainFile.c_str()));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// activate the noise texture
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "position"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "normal"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "texCoord"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "normal"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "texCoord"));

	glDrawElements(GL_TRIANGLES, gameState->island.faceGroups.begin()->second.size() * 3, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "normal"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("terrain"), "texCoord"));
}
