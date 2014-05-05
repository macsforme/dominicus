// DrawRadar.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/DrawRadar.h"

DrawRadar::DrawRadar() : containerDrawer(new DrawContainer()) {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "colorTexture"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "colorTexture"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawRadar::~DrawRadar() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);

	// delete variables
	delete(containerDrawer);
}

void DrawRadar::reloadGraphics() {
	// create a texture with the terrain heightmap for radar
	size_t resolution = (size_t) gameSystem->getFloat("islandTerrainDensity");
	Texture* radarTexture = new Texture(
			resolution,
			resolution,
			Texture::FORMAT_RGBA
		);

	for(
			std::vector<Mesh::Face>::iterator itr = gameState->island.faceGroups.begin()->second.begin();
			itr != gameState->island.faceGroups.begin()->second.end();
			++itr
		) {
		uint8_t colorValue, alphaValue;

		if(gameState->island.vertices[itr->vertices[0]].y >= 0.0f) {
			colorValue = (uint8_t) (
					(gameState->island.vertices[itr->vertices[0]].y + gameSystem->getFloat("terrainDepth")) /
					gameSystem->getFloat("islandMaximumHeight") * 255.0f);
			alphaValue = 0xFF;
//		} else if(gameState->island.vertices[itr->vertices[0]].y > -gameSystem->getFloat("terrainDepth")) {
//			colorValue = 0;
//			alphaValue = (gameState->island.vertices[itr->vertices[0]].y + gameSystem->getFloat("terrainDepth")) / gameSystem->getFloat("terrainDepth") * 0.5f * 0xFF;
		} else {
			colorValue = 0;
			alphaValue = 0;
		}

		radarTexture->setColorAt(
				(uint8_t) ((gameState->island.vertices[itr->vertices[0]].x / (gameSystem->getFloat("islandMaximumWidth") / 2.0f) / 2.0f + 0.5f) * (float) resolution),
				(uint8_t) ((gameState->island.vertices[itr->vertices[0]].z / (gameSystem->getFloat("islandMaximumWidth") / 2.0f) / 2.0f + 0.5f) * (float) resolution),
				colorValue,
				colorValue,
				colorValue,
				alphaValue
			);
	}
	radarTexture->setDepth(8);

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &radarTextureID);
	glBindTexture(GL_TEXTURE_2D, radarTextureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(radarTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			radarTexture->width,
			radarTexture->height,
			0,
			(radarTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			radarTexture->getDataPointer()
	);

	glDisable(GL_TEXTURE_2D);

	delete(radarTexture);
}

Vector2 DrawRadar::getSize(std::map<std::string, void*> arguments) {
	return Vector2(
			gameSystem->getFloat("radarSize") / 100.0f / gameGraphics->aspectRatio * 2.0f,
			gameSystem->getFloat("radarSize") / 100.0f * 2.0f
		);
}

void DrawRadar::execute(std::map<std::string, void*> arguments) {
	// draw container
	containerDrawer->execute(arguments);

	// collect arguments
	UIMetrics* metrics = ((UIMetrics*) arguments["metrics"]);

	// get the actual size so possibly incorrect metrics don't skew the aspect ratio
	Vector2 actualSize = getSize(arguments);
	Vector2 padding = Vector2(
			*((float*) arguments["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
			*((float*) arguments["padding"]) / (float) gameGraphics->resolutionY * 2.0f
		);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	// compute projection matrix
	Matrix4 projectionMatrix;
	projectionMatrix.identity();

	translateMatrix(
			-gameState->fortress.position.x / gameSystem->getFloat("islandMaximumWidth") * 2.0f,
			-gameState->fortress.position.z / gameSystem->getFloat("islandMaximumWidth") * 2.0f,
			0.0f,
			projectionMatrix
		);
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(90.0f), projectionMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(gameState->fortress.rotation), projectionMatrix);

	scaleMatrix(actualSize.x / 2.0f, actualSize.y / 2.0f, 1.0f, projectionMatrix);
	translateMatrix(metrics->position.x, metrics->position.y, 0.0f, projectionMatrix);


	float projectionMatrixArray[] = {
			projectionMatrix.m11, projectionMatrix.m12, projectionMatrix.m13, projectionMatrix.m14,
			projectionMatrix.m21, projectionMatrix.m22, projectionMatrix.m23, projectionMatrix.m24,
			projectionMatrix.m31, projectionMatrix.m32, projectionMatrix.m33, projectionMatrix.m34,
			projectionMatrix.m41, projectionMatrix.m42, projectionMatrix.m43, projectionMatrix.m44
		};

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);
	glScissor(
			(GLint) (((metrics->position.x - actualSize.x / 2.0f + padding.x) + 1.0f) / 2.0f * (float) gameGraphics->resolutionX),
			(GLint) (((metrics->position.y - actualSize.y / 2.0f + padding.y) + 1.0f) / 2.0f * (float) gameGraphics->resolutionY),
			(GLsizei) ((actualSize.x - padding.x * 2.0f) / 2.0f * (float) gameGraphics->resolutionX),
			(GLsizei) ((actualSize.y - padding.y * 2.0f) / 2.0f * (float) gameGraphics->resolutionY)
		);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, projectionMatrixArray);
	glUniform1i(uniforms["texture"], 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, radarTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["texCoord"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["texCoord"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);

/*
	// pathetically hackish code that needs to go
	glUseProgram(0);

	glPointSize(5.0f);

	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POINTS);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(metrics->position.x, metrics->position.y, -0.9f);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
*/
}
