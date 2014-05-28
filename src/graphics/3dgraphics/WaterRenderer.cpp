// WaterRenderer.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/3dgraphics/WaterRenderer.h"

WaterRenderer::WaterRenderer() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "water"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "water"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["timer"] = glGetUniformLocation(shaderProgram, "timer");
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["towerTransformMatrix"] = glGetUniformLocation(shaderProgram, "towerTransformMatrix");
	uniforms["insideColorMultiplier"] = glGetUniformLocation(shaderProgram, "insideColorMultiplier");
	uniforms["outsideColorMultiplier"] = glGetUniformLocation(shaderProgram, "outsideColorMultiplier");
	uniforms["colorChangeRadius"] = glGetUniformLocation(shaderProgram, "colorChangeRadius");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	float vDist = gameSystem->getFloat("renderingPerspectiveFarClip") * 3.0f;

	GLfloat vertDataBufferArray[] = {
			-vDist, 0.0f, -vDist,
			-vDist, 0.0f, vDist,
			vDist, 0.0f, vDist,
			vDist, 0.0f, -vDist
		};

	GLushort vertElementBufferArray[] = {
			0, 1, 2, 3
		};

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);
}

WaterRenderer::~WaterRenderer() {
	;
}

void WaterRenderer::execute(std::map<std::string, void*> arguments) {
	// prepare variables
	Matrix4 mvpMatrix = gameGraphics->currentCamera->mvMatrix;
	mvpMatrix *= gameGraphics->ppMatrix;
	float mvpMatrixArray[] = {
			mvpMatrix.m11, mvpMatrix.m12, mvpMatrix.m13, mvpMatrix.m14,
			mvpMatrix.m21, mvpMatrix.m22, mvpMatrix.m23, mvpMatrix.m24,
			mvpMatrix.m31, mvpMatrix.m32, mvpMatrix.m33, mvpMatrix.m34,
			mvpMatrix.m41, mvpMatrix.m42, mvpMatrix.m43, mvpMatrix.m44
		};

	Matrix4 towerTransformMatrix; towerTransformMatrix.identity();
	translateMatrix(-gameState->fortress.position.x, -gameState->fortress.position.y, -gameState->fortress.position.z, towerTransformMatrix);
	float towerTransformMatrixArray[] = {
			towerTransformMatrix.m11, towerTransformMatrix.m12, towerTransformMatrix.m13, towerTransformMatrix.m14,
			towerTransformMatrix.m21, towerTransformMatrix.m22, towerTransformMatrix.m23, towerTransformMatrix.m24,
			towerTransformMatrix.m31, towerTransformMatrix.m32, towerTransformMatrix.m33, towerTransformMatrix.m34,
			towerTransformMatrix.m41, towerTransformMatrix.m42, towerTransformMatrix.m43, towerTransformMatrix.m44
		};

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniform1f(uniforms["timer"], (float) (gameState->getGameMills() % 3000) / 3000.0f);
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);
	glUniformMatrix4fv(uniforms["towerTransformMatrix"], 1, GL_FALSE, towerTransformMatrixArray);
	glUniform4f(uniforms["insideColorMultiplier"], 1.0f, 1.0f, 1.0f, 1.0f);
	float shockColorMultiplier = (gameState->fortress.shock >= 0.0f ? 1.0f :
			(1.0f + gameState->fortress.shock) +
			gameSystem->getFloat("shockColorMultiplier") * -gameState->fortress.shock
		);
	glUniform4f(
			uniforms["outsideColorMultiplier"],
			shockColorMultiplier,
			shockColorMultiplier,
			shockColorMultiplier,
			1.0f
		);
	glUniform1f(uniforms["colorChangeRadius"], (gameState->fortress.shock + 1.0f) * gameSystem->getFloat("stateEMPRange"));

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*) 0);

	glEnableVertexAttribArray(attributes["position"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*) 0);

	glDisableVertexAttribArray(attributes["position"]);

	// undo state
	glDisable(GL_BLEND);
}
