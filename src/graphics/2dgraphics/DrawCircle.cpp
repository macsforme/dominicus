// DrawCircle.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawCircle.h"

#include <vector>

#include "graphics/GameGraphics.h"
#include "math/VectorMath.h"

extern GameGraphics* gameGraphics;

DrawStackArgList DrawCircle::instantiateArgList() {
	DrawStackArgList argList;

	argList["border"] = (void*) new float;			// thickness of border in pixels
	argList["borderColor"] = (void*) new Vector4;	// color of border of circle
	argList["insideColor"] = (void*) new Vector4;	// color of inside of circle
	argList["outsideColor"] = (void*) new Vector4;	// color of outside of circle
	argList["position"] = (void*) new Vector2;		// position in screen dimensions
	argList["size"] = (void*) new Vector2;			// width/height of circle in screen dimensions
	argList["softEdge"] = (void*) new float;		// thickness of antialiasing of border in pixels

	return argList;
}

void DrawCircle::deleteArgList(DrawStackArgList argList) {
	if(argList.find("border") != argList.end()) delete (float*) argList["border"];
	if(argList.find("borderColor") != argList.end()) delete (Vector4*) argList["borderColor"];
	if(argList.find("insideColor") != argList.end()) delete (Vector4*) argList["insideColor"];
	if(argList.find("outsideColor") != argList.end()) delete (Vector4*) argList["outsideColor"];
	if(argList.find("position") != argList.end()) delete (Vector2*) argList["position"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
	if(argList.find("softEdge") != argList.end()) delete (float*) argList["softEdge"];
}

void DrawCircle::execute(DrawStackArgList argList) {
	// set up geometry
	Vector2 position = *((Vector2*) argList["position"]);
	Vector2 size = *((Vector2*) argList["size"]);
	float border = *((float*) argList["border"]) * 4.0f / (((Vector2*) argList["size"])->x / 2.0f * (float) gameGraphics->resolutionX);
	std::vector<VertexEntry> quadVertices;

	drawCurve(
			&quadVertices,
			Vector2(-size.x / 4.0f, -size.y / 4.0f),
			size / 2.0f,
			180.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(-size.x / 4.0f, size.y / 4.0f),
			size / 2.0f,
			90.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(size.x / 4.0f, size.y / 4.0f),
			size / 2.0f,
			0.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(size.x / 4.0f, -size.y / 4.0f),
			size / 2.0f,
			270.0f
		);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLuint* elementBufferArray = new GLuint[quadVertices.size()];
	for(size_t i = 0; i < quadVertices.size(); ++i)
		elementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadVertices.size() * sizeof(GLuint), elementBufferArray,
			GL_STREAM_DRAW);

	delete[] elementBufferArray;

	size_t vertexBufferArraySize = quadVertices.size() * 8;
	GLfloat* vertexBufferArray = new GLfloat[vertexBufferArraySize];

	for(size_t i = 0; i < quadVertices.size(); ++i) {
		vertexBufferArray[i * 8 + 0] = quadVertices[i].position.x + position.x;
		vertexBufferArray[i * 8 + 1] = quadVertices[i].position.y + position.y;
		vertexBufferArray[i * 8 + 2] = quadVertices[i].primCoord.x;
		vertexBufferArray[i * 8 + 3] = quadVertices[i].primCoord.y;
		vertexBufferArray[i * 8 + 4] = quadVertices[i].curveOriginCoord.x;
		vertexBufferArray[i * 8 + 5] = quadVertices[i].curveOriginCoord.y;
		vertexBufferArray[i * 8 + 6] = 2.0f - border;
		vertexBufferArray[i * 8 + 7] = 2.0f;
	}

	glBufferData(GL_ARRAY_BUFFER, vertexBufferArraySize * sizeof(GLfloat), vertexBufferArray,
			GL_STREAM_DRAW);

	delete[] vertexBufferArray;

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	Vector4 insideColor = *((Vector4*) argList["insideColor"]);
	Vector4 borderColor = *((Vector4*) argList["borderColor"]);
	Vector4 outsideColor = *((Vector4*) argList["outsideColor"]);
	glUniform4f(uniforms["insideColor"], insideColor.x, insideColor.y, insideColor.z, insideColor.w);
	glUniform4f(uniforms["borderColor"], borderColor.x, borderColor.y, borderColor.z, borderColor.w);
	glUniform4f(uniforms["outsideColor"], outsideColor.x, outsideColor.y, outsideColor.z, outsideColor.w);
	glUniform1f(uniforms["softEdge"], *((float*) argList["softEdge"]) * 4.0f / (((Vector2*) argList["size"])->x / 2.0f * (float) gameGraphics->resolutionX));

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) 0);
	glVertexAttribPointer(attributes["primCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) (2 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["curveOriginCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) (4 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border1Dist"], 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border2Dist"], 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) (7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["primCoord"]);
	glEnableVertexAttribArray(attributes["curveOriginCoord"]);
	glEnableVertexAttribArray(attributes["border1Dist"]);
	glEnableVertexAttribArray(attributes["border2Dist"]);

	glDrawElements(GL_QUADS, quadVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["primiCoord"]);
	glDisableVertexAttribArray(attributes["curveOriginCoord"]);
	glDisableVertexAttribArray(attributes["border1Dist"]);
	glDisableVertexAttribArray(attributes["border2Dist"]);

	// undo state
	glDisable(GL_BLEND);
}
