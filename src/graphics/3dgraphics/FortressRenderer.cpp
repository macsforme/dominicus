// FortressRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/FortressRenderer.h"

#include <vector>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

FortressRenderer::FortressRenderer() {
	fortressMesh = Mesh("fortress");

	// determine camera origin
	cameraOrigin = (
			fortressMesh.vertices[fortressMesh.faceGroups["cameraorigin"][0].vertices[0]] +
			fortressMesh.vertices[fortressMesh.faceGroups["cameraorigin"][0].vertices[1]] +
			fortressMesh.vertices[fortressMesh.faceGroups["cameraorigin"][0].vertices[2]]
		) / 3.0f;

	// center turret and determine origin
	turretOrigin = (
			fortressMesh.vertices[fortressMesh.faceGroups["turretorigin"][0].vertices[0]] +
			fortressMesh.vertices[fortressMesh.faceGroups["turretorigin"][0].vertices[1]] +
			fortressMesh.vertices[fortressMesh.faceGroups["turretorigin"][0].vertices[2]]
		) / 3.0f;

	bool* vertexMoved = new bool[fortressMesh.vertices.size()];
	for(size_t i = 0; i < fortressMesh.vertices.size(); ++i)
		vertexMoved[i] = false;

	for(
			std::vector<Mesh::Face>::iterator itr = fortressMesh.faceGroups["turret"].begin();
			itr != fortressMesh.faceGroups["turret"].end();
			++itr
		) {
		for(size_t i = 0; i < 3; ++i) {
			if(! vertexMoved[itr->vertices[i]]) {
				fortressMesh.vertices[itr->vertices[i]] -= turretOrigin;

				vertexMoved[itr->vertices[i]] = true;
			}
		}
	}

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	unsigned int totalFaces = 0;
	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					fortressMesh.faceGroups.begin();
			itr != fortressMesh.faceGroups.end();
			++itr
		)
		totalFaces += itr->second.size();

	GLfloat* vertDataBufferArray = new GLfloat[totalFaces * 36];	// 3 vertices + 3 normals + 2 texcoords + 4 colors * 3 faces
	unsigned int bufferIndex = 0;

	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					fortressMesh.faceGroups.begin();
			itr != fortressMesh.faceGroups.end();
			++itr
		) {
		const unsigned int firstIndex = bufferIndex;

		// insert the vertex attribute data
		for(size_t i = 0; i < itr->second.size(); ++i) {
			for(size_t p = 0; p < 3; ++p) {
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 0] = fortressMesh.vertices[itr->second[i].vertices[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 1] = fortressMesh.vertices[itr->second[i].vertices[p]].y;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 2] = fortressMesh.vertices[itr->second[i].vertices[p]].z;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 3] = fortressMesh.normals[itr->second[i].normals[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 4] = fortressMesh.normals[itr->second[i].normals[p]].y;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 5] = fortressMesh.normals[itr->second[i].normals[p]].z;

				vertDataBufferArray[bufferIndex * 36 + p * 12 + 6] = fortressMesh.texCoords[itr->second[i].texCoords[p]].x;
				vertDataBufferArray[bufferIndex * 36 + p * 12 + 7] = fortressMesh.texCoords[itr->second[i].texCoords[p]].y;

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

FortressRenderer::~FortressRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void FortressRenderer::execute(std::map<std::string, void*> arguments) {
	Matrix4 mvMatrix; mvMatrix.identity();
	translateMatrix(gameState->fortress.position.x, gameState->fortress.position.y, gameState->fortress.position.z, mvMatrix);
	mvMatrix = mvMatrix * gameGraphics->currentCamera->mvMatrix;
	float mvMatrixArray[] = {
			mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
			mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
			mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
			mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
		};

	Matrix4 spinnerMvMatrix; spinnerMvMatrix.identity();
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(gameState->fortress.rotation), spinnerMvMatrix);
	spinnerMvMatrix *= mvMatrix;
	float spinnerMvMatrixArray[] = {
			spinnerMvMatrix.m11, spinnerMvMatrix.m12, spinnerMvMatrix.m13, spinnerMvMatrix.m14,
			spinnerMvMatrix.m21, spinnerMvMatrix.m22, spinnerMvMatrix.m23, spinnerMvMatrix.m24,
			spinnerMvMatrix.m31, spinnerMvMatrix.m32, spinnerMvMatrix.m33, spinnerMvMatrix.m34,
			spinnerMvMatrix.m41, spinnerMvMatrix.m42, spinnerMvMatrix.m43, spinnerMvMatrix.m44
		};

	Matrix4 turretMvMatrix; turretMvMatrix.identity();
	translateMatrix((gameState->recoil > 1.0f ? -2.0f + gameState->recoil : -gameState->recoil) * gameSystem->getFloat("stateTurretRecoilDistance"), 0.0f, 0.0f, turretMvMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), radians(gameState->fortress.tilt), turretMvMatrix);
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(gameState->fortress.rotation), turretMvMatrix);
	translateMatrix(turretOrigin.x, turretOrigin.y, turretOrigin.z, turretMvMatrix);
	turretMvMatrix *= mvMatrix;
	float turretMvMatrixArray[] = {
			turretMvMatrix.m11, turretMvMatrix.m12, turretMvMatrix.m13, turretMvMatrix.m14,
			turretMvMatrix.m21, turretMvMatrix.m22, turretMvMatrix.m23, turretMvMatrix.m24,
			turretMvMatrix.m31, turretMvMatrix.m32, turretMvMatrix.m33, turretMvMatrix.m34,
			turretMvMatrix.m41, turretMvMatrix.m42, turretMvMatrix.m43, turretMvMatrix.m44
		};

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

	for(
			std::map<std::string, std::vector<Mesh::Face> >::iterator itr =
					fortressMesh.faceGroups.begin();
			itr != fortressMesh.faceGroups.end();
			++itr
		) {
		// don't draw the missile origin
		if(itr->first == "turretorigin" || itr->first == "cameraorigin" || itr->first == "shellorigin")
			continue;

		// set the texture
		glActiveTexture(GL_TEXTURE0);
		if(itr->first == "spinner")
			glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(std::string("structure/lightgrain")));
		else if(itr->first == "turret")
			glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(std::string("structure/mediumgrain")));
		else
			glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(std::string("structure/" + itr->first).c_str()));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if(itr->first == "turret")
			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "mvMatrix"), 1, GL_FALSE, turretMvMatrixArray);
		else if(itr->first == "spinner")
			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "mvMatrix"), 1, GL_FALSE, spinnerMvMatrixArray);
		else
			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "mvMatrix"), 1, GL_FALSE, mvMatrixArray);

		glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTextureLighting"), "pMatrix"), 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));

		// draw the geometry
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[std::string("elements_" + itr->first).c_str()]);

		glDrawElements(GL_TRIANGLES, itr->second.size() * 3, GL_UNSIGNED_INT, NULL);
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
