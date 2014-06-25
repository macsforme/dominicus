// DrawTexture.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawTexture.h"

#include <vector>

#include "graphics/GameGraphics.h"
#include "graphics/texture/Texture.h"
#include "graphics/UILayoutAuthority.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawTexture::DrawTexture() {
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawTexture::~DrawTexture() {
	// undo shader setup
	GLsizei shaderCount;
	GLuint* shaders = new GLuint[2];
	glGetAttachedShaders(shaderProgram, 2, &shaderCount, shaders);

	for(size_t i = 0; i < shaderCount; ++i) {
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	delete[] shaders;

	glDeleteProgram(shaderProgram);

	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

DrawStackArgList DrawTexture::instantiateArgList() {
	DrawStackArgList argList;

	argList["metrics"] = (void*) new UIMetrics;		// UI element metrics
	argList["texture"] = (void*) new std::string;	// name of texture to draw

	return argList;
}

void DrawTexture::deleteArgList(DrawStackArgList argList) {
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("texture") != argList.end()) delete (std::string*) argList["texture"];
}

Vector2 DrawTexture::getSize(DrawStackArgList argList) {
	// collect arguments
	std::string texture = *((std::string*) argList["texture"]);

	return Vector2(
			(float) gameGraphics->getTexture(texture)->width / (float) gameGraphics->resolutionX * 2.0f,
			(float) gameGraphics->getTexture(texture)->height / (float) gameGraphics->resolutionY * 2.0f
		);
}

void DrawTexture::execute(DrawStackArgList argList) {
	// collect arguments
	UIMetrics metrics = *((UIMetrics*) argList["metrics"]);
	metrics.size = getSize(argList);
	std::string texture = *((std::string*) argList["texture"]);
	GLuint textureID = gameGraphics->getTextureID(texture);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			metrics.position.x - metrics.size.x / 2.0f,
			metrics.position.y - metrics.size.y / 2.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,

			metrics.position.x - metrics.size.x / 2.0f,
			metrics.position.y + metrics.size.y / 2.0f,
			0.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,

			metrics.position.x + metrics.size.x / 2.0f,
			metrics.position.y + metrics.size.y / 2.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f,

			metrics.position.x + metrics.size.x / 2.0f,
			metrics.position.y - metrics.size.y / 2.0f,
			0.0f,
			1.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			1.0f
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(uniforms["texture"], 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

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
}
