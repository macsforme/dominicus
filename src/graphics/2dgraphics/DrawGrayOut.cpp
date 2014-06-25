// DrawGrayOut.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawGrayOut.h"

#include <vector>

#include "graphics/GameGraphics.h"
#include "graphics/UILayoutAuthority.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawGrayOut::DrawGrayOut() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "color"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "color"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawGrayOut::~DrawGrayOut() {
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

DrawStackArgList DrawGrayOut::instantiateArgList() {
	DrawStackArgList argList;

	argList["color"] = (void*) new Vector4;		// color of overlay

	return argList;
}

void DrawGrayOut::deleteArgList(DrawStackArgList argList) {
	if(argList.find("color") != argList.end()) delete (Vector4*) argList["color"];
}

void DrawGrayOut::execute(DrawStackArgList argList) {
	// collect arguments
	Vector4 color = *((Vector4*) argList["color"]);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, color.x, color.y, color.z, color.w,
			-1.0f, 1.0f, 0.0f, color.x, color.y, color.z, color.w,
			1.0f, 1.0f, 0.0f, color.x, color.y, color.z, color.w,
			1.0f, -1.0f, 0.0f, color.x, color.y, color.z, color.w
		};

	glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GL_FLOAT), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_BLEND);
}
