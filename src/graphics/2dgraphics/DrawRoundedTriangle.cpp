// DrawRoundedTriangle.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawRoundedTriangle.h"

#include <cmath>
#include <cstdlib>
#include <vector>

#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "math/ScalarMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawStackArgList DrawRoundedTriangle::instantiateArgList() {
	DrawStackArgList argList;

	argList["border"] = (void*) new float;			// thickness of border in pixels
	argList["borderColor"] = (void*) new Vector4;	// color of border of shape
	argList["insideColor"] = (void*) new Vector4;	// color of inside of shape
	argList["outsideColor"] = (void*) new Vector4;	// color of outside of shape
	argList["position"] = (void*) new Vector2;		// position in screen dimensions
	argList["rotation"] = (void*) new float;		// rotation of shape
	argList["size"] = (void*) new Vector2;			// width/height of shape in screen dimensions
	argList["softEdge"] = (void*) new float;		// thickness of antialiasing of border in pixels

	return argList;
}

void DrawRoundedTriangle::deleteArgList(DrawStackArgList argList) {
	if(argList.find("border") != argList.end()) delete (float*) argList["border"];
	if(argList.find("borderColor") != argList.end()) delete (Vector4*) argList["borderColor"];
	if(argList.find("insideColor") != argList.end()) delete (Vector4*) argList["insideColor"];
	if(argList.find("outsideColor") != argList.end()) delete (Vector4*) argList["outsideColor"];
	if(argList.find("position") != argList.end()) delete (Vector2*) argList["position"];
	if(argList.find("rotation") != argList.end()) delete (float*) argList["rotation"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
	if(argList.find("softEdge") != argList.end()) delete (float*) argList["softEdge"];
}

void DrawRoundedTriangle::execute(DrawStackArgList argList) {
	// set up geometry
	Vector2 position = *((Vector2*) argList["position"]);
	float rotation = *((float*) argList["rotation"]);
	Vector2 size = *((Vector2*) argList["size"]);

	float triangleHeight = cos(asin(size.x * gameGraphics->aspectRatio * 0.5f / size.y)) * size.y;
	float cutOutHeight = tan(asin(triangleHeight / size.y) - radians(45.0f)) * (size.x * gameGraphics->aspectRatio * 0.5f);

	std::vector<VertexEntry> triangleVertices;

	VertexEntry entry;
	entry.highlight = false;
	entry.concave = false;
	entry.curveOriginCoord = Vector2(0.0f, 0.0f);

	entry.position = Vector2(-size.x / 2.0f, -size.y / 2.0f + (size.y - triangleHeight));
	entry.primCoord = Vector2(
			cos(atan(triangleHeight / (size.x * (float) gameGraphics->aspectRatio / 2.0f)) + radians(90.0f)) * 2.0f,
			sin(atan(triangleHeight / (size.x * (float) gameGraphics->aspectRatio / 2.0f)) + radians(90.0f)) * 2.0f
		);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, size.y / 2.0f);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f + (size.y - (triangleHeight - cutOutHeight)));
	entry.primCoord = Vector2(
			entry.primCoord.x / 2.0f * (2.0f - sin(atan(size.x * (float) gameGraphics->aspectRatio / 2.0f / triangleHeight)) * (triangleHeight - cutOutHeight) / size.y * 2.0f),
			entry.primCoord.y / 2.0f * (2.0f - sin(atan(size.x * (float) gameGraphics->aspectRatio / 2.0f / triangleHeight)) * (triangleHeight - cutOutHeight) / size.y * 2.0f)
		);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, size.y / 2.0f);
	entry.primCoord = Vector2(
			-cos(atan(triangleHeight / (size.x * (float) gameGraphics->aspectRatio / 2.0f)) + radians(90.0f)) * 2.0f,
			sin(atan(triangleHeight / (size.x * (float) gameGraphics->aspectRatio / 2.0f)) + radians(90.0f)) * 2.0f
		);
	triangleVertices.push_back(entry);

	entry.position = Vector2(size.x / 2.0f, -size.y / 2.0f + (size.y - triangleHeight));
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f + (size.y - (triangleHeight - cutOutHeight)));
	entry.primCoord = Vector2(
			entry.primCoord.x / 2.0f * (2.0f - sin(atan(size.x * (float) gameGraphics->aspectRatio / 2.0f / triangleHeight)) * (triangleHeight - cutOutHeight) / size.y * 2.0f),
			entry.primCoord.y / 2.0f * (2.0f - sin(atan(size.x * (float) gameGraphics->aspectRatio / 2.0f / triangleHeight)) * (triangleHeight - cutOutHeight) / size.y * 2.0f)
		);
	triangleVertices.push_back(entry);

	entry.curveOriginCoord = Vector2(0.0f, 1.0f);

	entry.position = Vector2(-size.x / 2.0f, -size.y / 2.0f + (size.y - triangleHeight));
	entry.primCoord = Vector2(-2.0f * size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y, 1.0f - 2.0f * cos(asin(size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y)));
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f + (size.y - (triangleHeight - cutOutHeight)));
	entry.primCoord = Vector2(0.0f, 1.0f - 2.0f * (triangleHeight - cutOutHeight) / size.y);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f);
	entry.primCoord = Vector2(0.0f, -1.0f);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f);
	entry.primCoord = Vector2(0.0f, -1.0f);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f + (size.y - (triangleHeight - cutOutHeight)));
	entry.primCoord = Vector2(0.0f, 1.0f - 2.0f * (triangleHeight - cutOutHeight) / size.y);
	triangleVertices.push_back(entry);

	entry.position = Vector2(size.x / 2.0f, -size.y / 2.0f + (size.y - triangleHeight));
	entry.primCoord = Vector2(2.0f * size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y, 1.0f - 2.0f * cos(asin(size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y)));
	triangleVertices.push_back(entry);

	entry.position = Vector2(-size.x / 2.0f, -size.y / 2.0f);
	entry.primCoord = Vector2(-2.0f * size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y, -1.0f);
	triangleVertices.push_back(entry);

	entry.position = Vector2(-size.x / 2.0f, -size.y / 2.0f + (size.y - triangleHeight));
	entry.primCoord = Vector2(-2.0f * size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y, 1.0f - 2.0f * cos(asin(size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y)));
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f);
	entry.primCoord = Vector2(0.0f, -1.0f);
	triangleVertices.push_back(entry);

	entry.position = Vector2(size.x / 2.0f, -size.y / 2.0f + (size.y - triangleHeight));
	entry.primCoord = Vector2(2.0f * size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y, 1.0f - 2.0f * cos(asin(size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y)));
	triangleVertices.push_back(entry);

	entry.position = Vector2(size.x / 2.0f, -size.y / 2.0f);
	entry.primCoord = Vector2(2.0f * size.x * (float) gameGraphics->aspectRatio / 2.0f / size.y, -1.0f);
	triangleVertices.push_back(entry);

	entry.position = Vector2(0.0f, -size.y / 2.0f);
	entry.primCoord = Vector2(0.0f, -1.0f);
	triangleVertices.push_back(entry);

	// apply rotation
	Matrix4 rotationMatrix; rotationMatrix.identity();
	scaleMatrix(gameGraphics->aspectRatio, 1.0f, 1.0f, rotationMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, 1.0f), radians(rotation), rotationMatrix);
	scaleMatrix(1.0f / gameGraphics->aspectRatio, 1.0f, 1.0f, rotationMatrix);

	for(int i = 0; i < triangleVertices.size(); ++i) {
		Vector4 oldPosition(triangleVertices[i].position.x, triangleVertices[i].position.y, 0.0f, 0.0f);
		oldPosition = oldPosition * rotationMatrix;
		triangleVertices[i].position = Vector2(oldPosition.x, oldPosition.y);
	}

	// update vertex buffers
	GLuint* triangleElementBufferArray = new GLuint[triangleVertices.size()];
	for(size_t i = 0; i < triangleVertices.size(); ++i)
		triangleElementBufferArray[i] = i;

	size_t triangleVertexBufferArraySize = triangleVertices.size() * 8;
	GLfloat* triangleVertexBufferArray = new GLfloat[triangleVertexBufferArraySize];

	for(size_t i = 0; i < triangleVertices.size(); ++i) {
		triangleVertexBufferArray[i * 8 + 0] = triangleVertices[i].position.x + position.x;
		triangleVertexBufferArray[i * 8 + 1] = triangleVertices[i].position.y + position.y;
		triangleVertexBufferArray[i * 8 + 2] = triangleVertices[i].primCoord.x;
		triangleVertexBufferArray[i * 8 + 3] = triangleVertices[i].primCoord.y;
		triangleVertexBufferArray[i * 8 + 4] = triangleVertices[i].curveOriginCoord.x;
		triangleVertexBufferArray[i * 8 + 5] = triangleVertices[i].curveOriginCoord.y;
		triangleVertexBufferArray[i * 8 + 6] = 2.0f - *((float*) argList["border"]) * 2.0f / (size.y / 2.0f * (float) gameGraphics->resolutionY);
		triangleVertexBufferArray[i * 8 + 7] = 2.0f;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleVertices.size() * sizeof(GLuint), triangleElementBufferArray,
			GL_STREAM_DRAW);

	delete[] triangleElementBufferArray;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glBufferData(GL_ARRAY_BUFFER, triangleVertexBufferArraySize * sizeof(GLfloat), triangleVertexBufferArray,
			GL_STREAM_DRAW);

	delete[] triangleVertexBufferArray;

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("hudContainer"));

	// set uniforms
	Vector4 insideColor = *((Vector4*) argList["insideColor"]);
	Vector4 borderColor = *((Vector4*) argList["borderColor"]);
	Vector4 outsideColor = *((Vector4*) argList["outsideColor"]);
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("hudContainer"), "insideColor"), insideColor.x, insideColor.y, insideColor.z, insideColor.w);
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("hudContainer"), "borderColor"), borderColor.x, borderColor.y, borderColor.z, borderColor.w);
	glUniform4f(glGetUniformLocation(gameGraphics->getProgramID("hudContainer"), "outsideColor"), outsideColor.x, outsideColor.y, outsideColor.z, outsideColor.w);
	glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("hudContainer"), "softEdge"), *((float*) argList["softEdge"]) * 2.0f / (size.y / 2.0f * (float) gameGraphics->resolutionY));

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "position"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "primCoord"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "curveOriginCoord"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (4 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "border1Dist"), 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "border2Dist"), 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "primCoord"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "curveOriginCoord"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "border1Dist"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "border2Dist"));

	glDrawElements(GL_TRIANGLES, triangleVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "primCoord"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "curveOriginCoord"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "border1Dist"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("hudContainer"), "border2Dist"));
}
